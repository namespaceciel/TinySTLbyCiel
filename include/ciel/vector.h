#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_

#include <cstddef>
#include <ciel/iterator.h>
#include <ciel/memory.h>
#include <ciel/algorithm.h>

namespace ciel {

	// TODO: 应有一个对平凡类型的特化版本 vector，在插入删除（中间元素）和扩容时可以改用更底层的函数处理

	// vector 对所存元素类型 T 的移动构造函数是否为 noexcept 比较在意，这里仅对需要扩容的操作做出强异常保证
	// 对于在非末尾时调用 insert, emplace 与 erase 的操作会直接调用 noexcept 的元素移动，若抛出异常则直接中断程序

	template<class T, class Allocator = ciel::allocator<T>>
	class vector {

		static_assert(ciel::is_same_v<typename Allocator::value_type, T>, "ciel::vector 要求 Allocator::value_type 与 T 相同");

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;

		using pointer = typename ciel::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename ciel::allocator_traits<allocator_type>::const_pointer;

		using iterator = ciel::wrap_iter<pointer>;
		using const_iterator = ciel::wrap_iter<const_pointer>;

		using reverse_iterator = ciel::reverse_iterator<iterator>;
		using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;

	private:
		using alloc_traits = ciel::allocator_traits<allocator_type>;

		pointer start;
		pointer finish;
		pointer end_cap;
		[[no_unique_address]] allocator_type allocator;

		constexpr pointer alloc_range_destroy(allocator_type& a, pointer begin, pointer end) noexcept {
			while (end != begin) {
				alloc_traits::destroy(a, --end);
			}
			return end;
		}

		template<class... Arg>
		constexpr pointer alloc_range_construct_n(allocator_type& a, pointer begin, size_type n, Arg&& ... arg) {
			pointer end = begin;
			try {
				for (size_type i = 0; i < n; ++i) {
					alloc_traits::construct(a, end++, ciel::forward<Arg>(arg)...);
				}
				return end;
			} catch (...) {
				alloc_range_destroy(a, begin, --end);    // 上面的实现里没构造就已经 ++end 了，这里复原
				throw;
			}
		}

		template<ciel::legacy_input_iterator InputIt>
		constexpr pointer alloc_range_construct(allocator_type& a, pointer begin, InputIt first, InputIt last) {
			pointer end = begin;
			try {
				while (first != last) {
					alloc_traits::construct(a, end++, *first++);
				}
				return end;
			} catch (...) {
				alloc_range_destroy(a, begin, --end);    // 上面的实现里没构造就已经 ++end 了，这里复原
				throw;
			}
		}

		template<ciel::legacy_input_iterator InputIt>
		constexpr pointer alloc_range_move(allocator_type& a, pointer begin, InputIt first, InputIt last) noexcept(ciel::is_nothrow_move_constructible_v<value_type>) {
			if (ciel::is_nothrow_move_constructible_v<value_type>) {    // 为了保证强异常安全，只有在 value_type 的移动构造不抛异常时才会选择此函数
				pointer end = begin;
				while (first != last) {
					alloc_traits::construct(a, end++, ciel::move(*first++));
				}
				return end;
			} else {
				return alloc_range_construct(a, begin, first, last);
			}
		}

		/*
		这个函数主要用于调用 insert 或 emplace 时后半段旧元素的移动

		O  O  O  O  O  O  O  O  O  O

		在中间插入 3 个元素，旧元素往后移动
		         -  -  -
		O  O  O  N  N  N  O  O  O  O  O  O  O
		                  ----------  -------
		                    移动赋值   placement new
		*/
		constexpr pointer alloc_range_nothrow_move_backward(allocator_type& a, pointer end, pointer first, pointer last) noexcept {
			pointer begin = end;
			pointer boundary = last;
			while (first != last && begin != boundary) {
				alloc_traits::construct(a, --begin, ciel::move(*--last));
			}
			while (first != last) {
				*--begin = ciel::move(*--last);
			}
			return end;
		}

		// step1: 仅分配新内存
		constexpr pointer reserve_step1(size_type new_cap) {
			if (new_cap > max_size()) {
				throw std::length_error("ciel::vector::reserve 需求容量超出 max_size");
			}
			return alloc_traits::allocate(allocator, new_cap);
		}

		// step2: 移动旧元素，析构旧元素，释放旧内存
		constexpr void reserve_step2(pointer new_start, size_type new_cap) {
			try {
				alloc_range_move(allocator, new_start, start, finish);
			} catch (...) {
				alloc_traits::deallocate(allocator, new_start, new_cap);
				throw;
			}
			size_type old_size = size();
			clear();
			alloc_traits::deallocate(allocator, start, capacity());
			start = new_start;
			finish = start + old_size;
			end_cap = start + new_cap;
		}

		/*
		O  O  O  O

		在中间插入 2 个元素，旧元素往后移动
		         -  -
		O  O  O  N  N  O
		         -  -
		    移动赋值 placement new
		*/
		template<class Args>
		constexpr iterator insert_n(iterator pos, size_type count, Args&& args) {
			if (!count) {
				return pos;
			}
			// 涉及到扩容时，需要将被插入位置的前后两段分别移动构造到正确位置，再在中间全部是原地构造。不涉及扩容则需要注意分别处理初始化与未初始化内存
			if (size_type new_size = size() + count; new_size > capacity()) {
				size_type new_cap = capacity() ? capacity() * 2 : 1;
				while (new_size > new_cap) {
					new_cap *= 2;
				}

				size_type idx = pos - begin();
				pointer new_start = alloc_traits::allocate(allocator, new_cap);
				pointer new_pos = new_start + idx;

				try {
					alloc_range_move(allocator, new_start, begin(), pos);
					alloc_range_move(allocator, new_pos + count, pos, end());
				} catch (...) {
					alloc_traits::deallocate(allocator, new_start, new_cap);
					throw;
				}

				clear();
				alloc_traits::deallocate(allocator, start, capacity());
				start = new_start;
				finish = start + new_size;
				end_cap = start + new_cap;
				return iterator(alloc_range_construct_n(allocator, new_pos, count, ciel::forward<Args>(args)) - 1);
			} else {
				pointer boundary = finish;
				finish = alloc_range_nothrow_move_backward(allocator, finish + count, pos.base(), finish);
				auto left_movement = boundary - pos.base();
				auto loop_break = left_movement < count ? left_movement : count;
				for (size_type i = 0; i < loop_break; ++i) {    // 全部都是赋值
					*(pos++) = ciel::forward<Args>(args);
				}
				if (left_movement < count) {    // 赋值完还有一部分需要构造
					pos = iterator(alloc_range_construct_n(allocator, boundary, count - left_movement, ciel::forward<Args>(args)) - 1);
				}
				return pos;
			}
		}

		void clear_and_get_cap_no_less_than(size_type size) {
			clear();
			if (capacity() < size) {
				alloc_traits::deallocate(allocator, start, capacity());
				start = nullptr;    // 防止申请内存时抛出异常，自身析构时 double free
				start = alloc_traits::allocate(allocator, size);
				end_cap = start + size;
			}
		}

	public:
		constexpr vector() noexcept(noexcept(allocator_type())): start(nullptr), finish(nullptr), end_cap(nullptr), allocator() {}

		constexpr explicit vector(const allocator_type& alloc) noexcept: start(nullptr), finish(nullptr), end_cap(nullptr), allocator(alloc) {}

		constexpr vector(size_type count, const value_type& value, const allocator_type& alloc = allocator_type())
			: allocator(alloc),
			  start(count ? alloc_traits::allocate(allocator, count) : nullptr),
			  finish(alloc_range_construct_n(allocator, start, count, value)),
			  end_cap(finish) {}

		constexpr explicit vector(size_type count, const allocator_type& alloc = allocator_type())
			: allocator(alloc),
			  start(count ? alloc_traits::allocate(allocator, count) : nullptr),
			  finish(alloc_range_construct_n(allocator, start, count)),
			  end_cap(finish) {}

		// TODO: 若 first 和 last 都只是输入迭代器，会调用 O(N) 次 T 的复制构造函数，并且会进行 O(log N) 次重分配。
		template<ciel::legacy_input_iterator InputIt>
		constexpr vector(InputIt first, InputIt last, const allocator_type& alloc = allocator_type())
			: allocator(alloc),
			  start(ciel::distance(first, last) ? alloc_traits::allocate(allocator, ciel::distance(first, last)) : nullptr),
			  finish(alloc_range_construct(allocator, start, first, last)),
			  end_cap(finish) {}

		constexpr vector(const vector& other) : vector(other.begin(), other.end(), alloc_traits::select_on_container_copy_construction(other.get_allocator())) {}

		// TODO: 在进行类模板实参推导时，只会从首个实参推导模板形参 Allocator。(C++23 起)
		constexpr vector(const vector& other, const allocator_type& alloc) : vector(other.begin(), other.end(), alloc) {}

		constexpr vector(vector&& other) noexcept : start(other.start), finish(other.finish), end_cap(other.end_cap), allocator(ciel::move(other.allocator)) {
			other.start = nullptr;
			other.finish = nullptr;
			other.end_cap = nullptr;
		}

		// 如果 alloc != other.get_allocator() ，那么它会导致逐元素移动。（此时移动后不保证 other 为空）
		// TODO: 在进行类模板实参推导时，只会从首个实参推导模板形参 Allocator。(C++23 起)
		constexpr vector(vector&& other, const allocator_type& alloc) {
			if (alloc == other.get_allocator()) {
				allocator = alloc;
				start = other.start;
				finish = other.finish;
				end_cap = other.end_cap;
				other.start = nullptr;
				other.finish = nullptr;
				other.end_cap = nullptr;
			} else {
				vector(other, alloc);
			}
		}

		constexpr vector(std::initializer_list<value_type> init, const allocator_type& alloc = allocator_type()) : vector(init.begin(), init.end(), alloc) {}

		constexpr ~vector() {    // 先一个个手动调用成员的析构函数，再由 vector 自己释放内存
			if (start) {
				clear();
				alloc_traits::deallocate(allocator, start, capacity());
			}
		}

		// 若 alloc_traits::propagate_on_container_copy_assignment::value 为 true ，则用 other 的分配器的副本替换 *this 的分配器。
		// 若 *this 的分配器在赋值后将与其旧值比较不相等，则用旧分配器解分配内存，然后在复制元素前用新分配器分配内存。
		// 否则，在可行时可能复用 *this 所拥有的内存。
		constexpr vector& operator=(const vector& other) {
			if (this == ciel::addressof(other)) {
				return *this;
			}
			if (alloc_traits::propagate_on_container_copy_assignment::value) {
				if (allocator != other.allocator) {
					{
						vector tmp(other.allocator);
						(*this).swap(tmp);
					}
					clear_and_get_cap_no_less_than(other.size());
					finish = alloc_range_construct(allocator, start, other.begin(), other.end());
					return *this;
				}
				allocator = other.allocator;
			}
			clear_and_get_cap_no_less_than(other.size());
			finish = alloc_range_construct(allocator, start, other.begin(), other.end());
			return *this;
		}

		// 若 alloc_traits::propagate_on_container_move_assignment::value 为 true ，则用 other 的分配器的副本替换 *this 的分配器
		// 若它为 false 且 *this 与 other 的分配器不比较相等，则 *this 不能接管 other 所拥有的内存的所有权且必须单独地移动赋值每个元素，并用其自身的分配器按需分配额外内存

		// 第 1 种情况下，指向 other 的迭代器在此后应保持合法
		constexpr vector& operator=(vector&& other) noexcept(alloc_traits::propagate_on_container_move_assignment::value || alloc_traits::is_always_equal::value) {
			if (this == ciel::addressof(other)) {
				return *this;
			}
			if (alloc_traits::propagate_on_container_move_assignment::value) {
				allocator = other.allocator;
			}
			if (alloc_traits::propagate_on_container_move_assignment::value || alloc_traits::is_always_equal::value) {
				if (start) {
					clear();
					alloc_traits::deallocate(allocator, start, capacity());
					start = nullptr;
					finish = nullptr;
					end_cap = nullptr;
				}
				swap(other);
			} else {
				clear_and_get_cap_no_less_than(other.size());
				finish = alloc_range_construct(allocator, start, other.begin(), other.end());
			}
			return *this;
		}

		constexpr vector& operator=(std::initializer_list<value_type> ilist) {
			if (capacity() < ilist.size()) {
				vector tmp(ilist, allocator);
				clear();
				swap(tmp);
			} else {
				clear();
				finish = alloc_range_construct(allocator, start, ilist.begin(), ilist.end());
			}
			return *this;
		}

		constexpr void assign(size_type count, const value_type& value) {
			clear_and_get_cap_no_less_than(count);
			finish = alloc_range_construct_n(allocator, start, count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		constexpr void assign(InputIt first, InputIt last) {
			clear_and_get_cap_no_less_than(ciel::distance(first, last));
			finish = alloc_range_construct(allocator, start, first, last);
		}

		constexpr void assign(std::initializer_list<value_type> ilist) {
			clear_and_get_cap_no_less_than(ilist.size());
			finish = alloc_range_construct(allocator, start, ilist.begin(), ilist.end());
		}

		constexpr allocator_type get_allocator() const noexcept {
			return allocator;
		}

		constexpr reference at(size_type pos) {
			if (pos >= size()) {
				throw std::out_of_range("ciel::vector 成员函数 at 越界");
			}
			return start[pos];
		}

		constexpr const_reference at(size_type pos) const {
			if (pos >= size()) {
				throw std::out_of_range("ciel::vector 成员函数 at 越界");
			}
			return start[pos];
		}

		constexpr reference operator[](size_type pos) {
			return start[pos];
		}

		constexpr const_reference operator[](size_type pos) const {
			return start[pos];
		}

		constexpr reference front() {
			return start[0];
		}

		constexpr const_reference front() const {
			return start[0];
		}

		constexpr reference back() {
			return *(finish - 1);
		}

		constexpr const_reference back() const {
			return *(finish - 1);
		}

		constexpr T* data() noexcept {
			return start;
		}

		constexpr const T* data() const noexcept {
			return start;
		}

		constexpr iterator begin() noexcept {
			return iterator(start);
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator(start);
		}

		constexpr const_iterator cbegin() const noexcept {
			return begin();
		}

		constexpr iterator end() noexcept {
			return iterator(finish);
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(finish);
		}

		constexpr const_iterator cend() const noexcept {
			return end();
		}

		constexpr reverse_iterator rbegin() noexcept {
			return reverse_iterator(end());
		}

		constexpr const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(end());
		}

		constexpr const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		constexpr reverse_iterator rend() noexcept {
			return reverse_iterator(begin());
		}

		constexpr const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(begin());
		}

		constexpr const_reverse_iterator crend() const noexcept {
			return rend();
		}

		[[nodiscard]] constexpr bool empty() const noexcept {
			return begin() == end();
		}

		[[nodiscard]] constexpr size_type size() const noexcept {
			return finish - start;
		}

		[[nodiscard]] constexpr size_type max_size() const noexcept {
			return alloc_traits::max_size(allocator);
		}

		// reserve 需要被拆分成两个步骤，原因在于如果代码被 emplace_back 复用时出现自引用（如 v.emplace_back(v[0])），需要在分配新内存后插入新元素完成才能移动旧元素并清理旧内存
		constexpr void reserve(size_type new_cap) {
			if (new_cap <= capacity()) {
				return;
			}
			pointer new_start = reserve_step1(new_cap);
			reserve_step2(new_start, new_cap);
		}

		[[nodiscard]] constexpr size_type capacity() const noexcept {
			return end_cap - start;
		}

		constexpr void shrink_to_fit() {
			if (size() == capacity()) {
				return;
			}
			pointer new_start = alloc_traits::allocate(allocator, size());
			reserve_step2(new_start, size());
		}

		constexpr void clear() noexcept {
			finish = alloc_range_destroy(allocator, start, finish);
		}

		constexpr iterator insert(const_iterator pos, const value_type& value) {
			return insert_n(pos, 1, value);
		}

		constexpr iterator insert(const_iterator pos, value_type&& value) {
			return insert_n(pos, 1, ciel::move(value));
		}

		constexpr iterator insert(const_iterator pos, size_type count, const value_type& value) {
			return insert_n(pos, count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
			size_type count = ciel::distance(first, last);
			if (!count) {
				return pos;
			}
			// 涉及到扩容时，需要将被插入位置的前后两段分别移动构造到正确位置，再在中间全部是原地构造。不涉及扩容则需要注意分别处理初始化与未初始化内存
			if (size_type new_size = size() + count; new_size > capacity()) {
				size_type new_cap = capacity() ? capacity() * 2 : 1;
				while (new_size > new_cap) {
					new_cap *= 2;
				}

				size_type idx = pos - begin();
				pointer new_start = alloc_traits::allocate(allocator, new_cap);
				pointer new_pos = new_start + idx;

				try {
					alloc_range_move(allocator, new_start, begin(), iterator(pos));
					alloc_range_move(allocator, new_pos + count, iterator(pos), end());
				} catch (...) {
					alloc_traits::deallocate(allocator, new_start, new_cap);
					throw;
				}

				clear();
				alloc_traits::deallocate(allocator, start, capacity());
				start = new_start;
				finish = start + new_size;
				end_cap = start + new_cap;
				return iterator(alloc_range_construct(allocator, new_pos, first, last) - 1);
			} else {
				pointer boundary = finish;
				finish = alloc_range_nothrow_move_backward(allocator, finish + count, const_cast<pointer>(pos.base()), finish);
				auto left_movement = boundary - pos.base();
				auto loop_break = left_movement < count ? left_movement : count;
				ciel::copy_n(first, loop_break, iterator(pos));
				if (left_movement < count) {    // 赋值完还有一部分需要构造
					pos = iterator(alloc_range_construct(allocator, boundary, first + left_movement, last) - 1);
				}
				return pos;
			}
		}

		constexpr iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
			return insert(pos, ilist.begin(), ilist.end());
		}

		template<class... Args>
		constexpr iterator emplace(const_iterator pos, Args&& ... args) {
			if (pos == end()) {
				emplace_back(ciel::forward<Args>(args)...);
				return iterator(finish - 1);
			} else {
				return insert_n(pos, 1, value_type{ciel::forward<Args>(args)...});
			}
		}

		constexpr iterator erase(const_iterator pos) {
			return erase(pos, pos + 1);
		}

		constexpr iterator erase(const_iterator first, const_iterator last) {
			auto distance = ciel::distance(first, last);
			if (!distance) {
				return last;
			}
			auto new_end = ciel::move(iterator(last), end(), iterator(first));
			finish = alloc_range_destroy(allocator, new_end.base(), finish);
			return end();
		}

		constexpr void push_back(const T& value) {
			emplace_back(value);
		}

		constexpr void push_back(T&& value) {
			emplace_back(ciel::move(value));
		}

		template<class... Args>
		constexpr reference emplace_back(Args&& ... args) {
			if (size_type new_cap = capacity(); size() == new_cap) {
				new_cap = new_cap ? 2 * new_cap : 1;
				pointer new_start = reserve_step1(new_cap);
				try {
					alloc_range_construct_n(allocator, new_start + size(), 1, ciel::forward<Args>(args)...);
				} catch (...) {
					alloc_traits::deallocate(allocator, new_start, new_cap);
					throw;
				}
				reserve_step2(new_start, new_cap);
				++finish;
			} else {
				finish = alloc_range_construct_n(allocator, finish, 1, ciel::forward<Args>(args)...);
			}
			return back();
		}

		constexpr void pop_back() noexcept {
			if (!empty()) {
				finish = alloc_range_destroy(allocator, finish - 1, finish);
			}
		}

		constexpr void resize(size_type count) {
			if (size() >= count) {
				finish = alloc_range_destroy(allocator, finish - (size() - count), finish);
			} else {
				finish = alloc_range_construct_n(allocator, finish, count - size());
			}
		}

		constexpr void resize(size_type count, const value_type& value) {
			if (size() >= count) {
				finish = alloc_range_destroy(allocator, finish - (size() - count), finish);
			} else {
				finish = alloc_range_construct_n(allocator, finish, count - size(), value);
			}
		}

		constexpr void swap(vector& other) noexcept(alloc_traits::propagate_on_container_swap::value || alloc_traits::is_always_equal::value) {
			ciel::swap(start, other.start);
			ciel::swap(finish, other.finish);
			ciel::swap(end_cap, other.end_cap);
			ciel::swap(allocator, other.allocator);
		}

	};  // class vector

	template<class T, class Alloc>
	constexpr bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
		if (lhs.size() != rhs.size()) {
			return false;
		}
		return ciel::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<class T, class Alloc>
	constexpr void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	template<class T, class Alloc, class U>
	constexpr typename vector<T, Alloc>::size_type erase(vector<T, Alloc>& c, const U& value) {
		auto it = ciel::remove(c.begin(), c.end(), value);
		auto r = ciel::distance(it, c.end());
		c.erase(it, c.end());
		return r;
	}

	template<class T, class Alloc, class Pred>
	constexpr typename vector<T, Alloc>::size_type erase_if(vector<T, Alloc>& c, Pred pred) {
		auto it = ciel::remove_if(c.begin(), c.end(), pred);
		auto r = ciel::distance(it, c.end());
		c.erase(it, c.end());
		return r;
	}

	template<ciel::legacy_input_iterator InputIt, class Alloc = ciel::allocator<typename ciel::iterator_traits<InputIt>::value_type>>
	vector(InputIt, InputIt, Alloc = Alloc()) -> vector<typename ciel::iterator_traits<InputIt>::value_type, Alloc>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_