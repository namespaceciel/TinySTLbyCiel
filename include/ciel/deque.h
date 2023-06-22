#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_DEQUE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_DEQUE_H_

#include <cstddef>
#include <ciel/list.h>

namespace ciel {

	// 这里的实现为了简单起见，deque 的 map （每个小数组的控制中心）用的是 list 实现，避免了重分配
	// 同样为了简单起见，与其他容器不同，deque 即使空初始化依然会分配一块堆内存。注意：这使得移动构造行为变为了先本身进行空初始化再与 other 进行 swap

	template<class T>
	constexpr size_t deque_subarray_size() {
		return ((sizeof(T) <= 4) ? 64 : ((sizeof(T) <= 8) ? 32 : ((sizeof(T) <= 16) ? 16 : ((sizeof(T) <= 32) ? 8 : 4))));
	}

	template<class T, class Pointer, class Reference, class MapIterator, size_t SubarraySize>
	class deque_iterator {
	public:
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = Pointer;
		using reference = Reference;
		using iterator_category = ciel::random_access_iterator_tag;
		using iterator_concept = ciel::random_access_iterator_tag;

	private:
		T* cur;
		MapIterator node;

		T* start() const {
			return *node;
		}

		T* finish() const {
			return start() + SubarraySize;
		}

		template<class, class, size_t>
		friend
		class deque;

	public:
		deque_iterator() = default;

		deque_iterator(T* c, const MapIterator& n) : cur(c), node(n) {}

		deque_iterator(const deque_iterator& other) = default;

		template<class P, class R, class M>
		deque_iterator(const deque_iterator<T, P, R, M, SubarraySize>& other) : cur(other.cur), node(other.node) {}

		pointer operator->() const {
			return cur;
		}

		reference operator*() const {
			return *cur;
		}

		deque_iterator& operator++() {
			if (++cur == finish()) {
				++node;
				cur = start();
			}
			return *this;
		}

		deque_iterator operator++(int) {
			deque_iterator res(*this);
			++*this;
			return res;
		}

		deque_iterator& operator--() {
			if (cur == start()) {
				--node;
				cur = finish() - 1;
			} else {
				--cur;
			}
			return *this;
		}

		deque_iterator operator--(int) {
			deque_iterator res(*this);
			--*this;
			return res;
		}

		deque_iterator& operator+=(difference_type n) {
			auto offset = n + (cur - start());
			if (offset >= 0 && offset < SubarraySize) {
				cur += n;
			} else {
				ciel::advance(node, offset > 0 ? offset / SubarraySize : offset / SubarraySize - 1);
				offset %= SubarraySize;
				cur = start() + offset;
			}
			return *this;
		}

		deque_iterator& operator-=(difference_type n) {
			return *this += -n;
		}

		deque_iterator operator+(difference_type n) const {
			deque_iterator res(*this);
			return res += n;
		}

		deque_iterator operator-(difference_type n) const {
			deque_iterator res(*this);
			return res -= n;
		}

		T* base() {
			return cur;
		}

	};    // class deque_iterator

	template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2, class MapIterator1, class MapIterator2, size_t SubarraySize>
	bool operator==(const deque_iterator<T, Pointer1, Reference1, MapIterator1, SubarraySize>& lhs, const deque_iterator<T, Pointer2, Reference2, MapIterator2, SubarraySize>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2, class MapIterator1, class MapIterator2, size_t SubarraySize>
	bool operator!=(const deque_iterator<T, Pointer1, Reference1, MapIterator1, SubarraySize>& lhs, const deque_iterator<T, Pointer2, Reference2, MapIterator2, SubarraySize>& rhs) {
		return !(lhs.base() == rhs.base());
	}

	template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2, class MapIterator1, class MapIterator2, size_t SubarraySize>
	auto operator-(const deque_iterator<T, Pointer1, Reference1, MapIterator1, SubarraySize>& lhs, const deque_iterator<T, Pointer2, Reference2, MapIterator2, SubarraySize>& rhs) -> decltype(lhs.base() - rhs.base()) {
		return lhs.base() - rhs.base();
	}

	template<class T, class Allocator = ciel::allocator<T>, size_t SubarraySize = deque_subarray_size<T>()>
	class deque {

		static_assert(ciel::is_same_v<typename Allocator::value_type, T>, "ciel::deque 要求 Allocator::value_type 与 T 相同");

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename ciel::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename ciel::allocator_traits<allocator_type>::const_pointer;

		using iterator = deque_iterator<value_type, pointer, reference, typename ciel::list<value_type*, allocator_type>::iterator, SubarraySize>;
		using const_iterator = deque_iterator<value_type, const_pointer, const_reference, typename ciel::list<value_type*, allocator_type>::const_iterator, SubarraySize>;

		using reverse_iterator = ciel::reverse_iterator<iterator>;
		using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;

	private:
		using alloc_traits = ciel::allocator_traits<allocator_type>;

		ciel::list<value_type*, allocator_type> map;
		iterator start;
		iterator finish;
		[[no_unique_address]] allocator_type allocator;

		iterator alloc_range_destroy(allocator_type& a, iterator begin, iterator end) noexcept {
			while (end != begin) {
				alloc_traits::destroy(a, (--end).base());
			}
			return end;
		}

		template<class... Arg>
		iterator alloc_range_construct_n(allocator_type& a, iterator begin, size_type n, Arg&& ... arg) {
			iterator end = begin;
			try {
				for (size_type i = 0; i < n; ++i) {
					alloc_traits::construct(a, (end++).base(), ciel::forward<Arg>(arg)...);
				}
				return end;
			} catch (...) {
				alloc_range_destroy(a, begin, --end);    // 上面的实现里没构造就已经 ++end 了，这里复原
				throw;
			}
		}

		template<ciel::legacy_input_iterator InputIt>
		iterator alloc_range_construct(allocator_type& a, iterator begin, InputIt first, InputIt last) {
			pointer end = begin;
			try {
				while (first != last) {
					alloc_traits::construct(a, (end++).base(), *first++);
				}
				return end;
			} catch (...) {
				alloc_range_destroy(a, begin, --end);    // 上面的实现里没构造就已经 ++end 了，这里复原
				throw;
			}
		}

		void clear_and_get_cap_no_less_than(size_type size) {
			clear();
			start = iterator(map.front(), map.begin());
			finish = start;
			size_type s = size / SubarraySize + 1;
			if (map.size() < s) {
				s -= map.size();
				while (s--) {
					map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
				}
			}
		}

	public:
		deque() : allocator(), map(1, alloc_traits::allocate(allocator, SubarraySize), allocator), start(map.front(), map.begin()), finish(start) {}

		explicit deque(const allocator_type& alloc) : allocator(alloc), map(1, alloc_traits::allocate(allocator, SubarraySize), allocator), start(map.front(), map.begin()), finish(start) {}

		deque(size_type count, const T& value, const allocator_type& alloc = allocator_type()) : allocator(alloc), map(allocator) {
			size_type s = count / SubarraySize + 1;
			while (s--) {
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			start = iterator(map.front(), map.begin());
			finish = alloc_range_construct_n(allocator, start, count, value);
		}

		explicit deque(size_type count, const allocator_type& alloc = allocator_type()) : allocator(alloc), map(allocator) {
			size_type s = count / SubarraySize + 1;
			while (s--) {
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			start = iterator(map.front(), map.begin());
			finish = alloc_range_construct_n(allocator, start, count);
		}

		template<ciel::legacy_input_iterator InputIt>
		deque(InputIt first, InputIt last, const allocator_type& alloc = allocator_type()) : allocator(alloc), map(allocator) {
			size_type s = ciel::distance(first, last) / SubarraySize + 1;
			while (s--) {
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			start = iterator(map.front(), map.begin());
			finish = alloc_range_construct_n(allocator, start, first, last);
		}

		deque(const deque& other) : deque(other.begin(), other.end(), alloc_traits::select_on_container_copy_construction(other.get_allocator())) {}

		deque(const deque& other, const allocator_type& alloc) : deque(other.begin(), other.end(), alloc) {}

		deque(deque&& other) : deque() {
			swap(other);
		}

		// TODO: 如果 alloc != other.get_allocator() ，那么它会导致逐元素移动
		deque(deque&& other, const allocator_type& alloc) : deque() {
			swap(other);
			allocator = alloc;
		}

		deque(std::initializer_list<T> init, const allocator_type& alloc = allocator_type()) : deque(init.begin(), init.end(), alloc) {}

		~deque() {
			clear();
			for (auto ptr : map) {
				alloc_traits::deallocate(allocator, ptr, SubarraySize);
			}
		}

		// 若 alloc_traits::propagate_on_container_copy_assignment::value 为 true ，则用 other 的分配器的副本替换 *this 的分配器
		// 若 *this 的分配器在赋值后将与其旧值比较不相等，则用旧分配器解分配内存，然后在复制元素前用新分配器分配内存
		// 否则，在可行时可能复用 *this 所拥有的内存
		deque& operator=(const deque& other) {
			if (this == ciel::addressof(other)) {
				return *this;
			}
			if (alloc_traits::propagate_on_container_copy_assignment::value) {
				if (allocator != other.allocator) {
					{
						deque tmp(other.allocator);
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
		deque& operator=(deque&& other) noexcept(alloc_traits::is_always_equal::value) {
			if (this == ciel::addressof(other)) {
				return *this;
			}
		}

		deque& operator=(std::initializer_list<T> ilist) {
			clear_and_get_cap_no_less_than(ilist.size());
			finish = alloc_range_construct(allocator, start, ilist.begin(), ilist.end());
			return *this;
		}

		void assign(size_type count, const T& value) {
			clear_and_get_cap_no_less_than(count);
			finish = alloc_range_construct_n(allocator, start, count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		void assign(InputIt first, InputIt last) {
			clear_and_get_cap_no_less_than(ciel::distance(first, last));
			finish = alloc_range_construct(allocator, start, first, last);
		}

		void assign(std::initializer_list<T> ilist) {
			clear_and_get_cap_no_less_than(ilist.size());
			finish = alloc_range_construct(allocator, start, ilist.begin(), ilist.end());
		}

		allocator_type get_allocator() const noexcept {
			return allocator;
		}

		reference at(size_type pos) {
			if (pos >= size()) {
				throw std::out_of_range("ciel::deque 成员函数 at 越界");
			}
			return (*this)[pos];
		}

		const_reference at(size_type pos) const {
			if (pos >= size()) {
				throw std::out_of_range("ciel::deque 成员函数 at 越界");
			}
			return (*this)[pos];
		}

		reference operator[](size_type pos) {
			return *(start + pos);
		}

		const_reference operator[](size_type pos) const {
			return *(start + pos);
		}

		reference front() {
			return *begin();
		}

		const_reference front() const {
			return *begin();
		}

		reference back() {
			return *ciel::prev(end());
		}

		const_reference back() const {
			return *ciel::prev(end());
		}

		iterator begin() noexcept {
			return start;
		}

		const_iterator begin() const noexcept {
			return start;
		}

		const_iterator cbegin() const noexcept {
			return begin();
		}

		iterator end() noexcept {
			return finish;
		}

		const_iterator end() const noexcept {
			return finish;
		}

		const_iterator cend() const noexcept {
			return end();
		}

		reverse_iterator rbegin() noexcept {
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		reverse_iterator rend() noexcept {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crend() const noexcept {
			return rend();
		}

		[[nodiscard]] bool empty() const noexcept {
			return begin() == end();
		}

		[[nodiscard]] size_type size() const noexcept {
			return ciel::distance(begin(), end());
		}

		[[nodiscard]] size_type max_size() const noexcept {
			return ciel::numeric_limits<difference_type>::max();
		}

		void shrink_to_fit() {
			while (map.begin() != start.node) {
				alloc_traits::deallocate(allocator, map.front(), SubarraySize);
				map.pop_front();
			}
			while (map.end().prev() != finish.node) {
				alloc_traits::deallocate(allocator, map.back(), SubarraySize);
				map.pop_back();
			}
		}

		void clear() noexcept {
			finish = alloc_range_destroy(allocator, begin(), end());
		}

		void push_back(const T& value) {
			emplace_back(value);
		}

		void push_back(T&& value) {
			emplace_back(ciel::move(value));
		}

		template<class... Args>
		reference emplace_back(Args&& ... args) {
			if (finish.cur == finish.finish() - 1 && finish.node == map.end().prev()) {    // 这里注意不要 finish.cur + 1 == finish.finish()，因为运算符重载里所有到 finish() 的地方会直接跳到下个小数组的头部
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			finish = alloc_range_construct_n(allocator, finish, 1, ciel::forward<Args>(args)...);
			return back();
		}

		void pop_back() noexcept {
			finish = alloc_range_destroy(allocator, finish - 1, finish);
		}

		void push_front(const T& value) {
			emplace_front(value);
		}

		void push_front(T&& value) {
			emplace_front(ciel::move(value));
		}

		template<class... Args>
		reference emplace_front(Args&& ... args) {
			if (start.cur == start.start() && start.node == map.begin()) {
				map.emplace_front(alloc_traits::allocate(allocator, SubarraySize));
			}
			alloc_range_construct_n(allocator, start - 1, 1, ciel::forward<Args>(args)...);
			--start;
			return front();
		}

		void pop_front() noexcept {
			alloc_range_destroy(allocator, start, start + 1);
			++start;
		}

		void resize(size_type count);

		void resize(size_type count, const value_type& value);

		// TODO: 如果 alloc_traits::propagate_on_container_swap::value 是 true，那么就会用对非成员 swap 的无限定调用交换分配器。否则，不交换它们（且在 get_allocator() != other.get_allocator() 时行为未定义）。
		void swap(deque& other) noexcept(alloc_traits::is_always_equal::value) {
			ciel::swap(map, other.map);
			ciel::swap(start, other.start);
			ciel::swap(finish, other.finish);
			ciel::swap(allocator, other.allocator);
		}

	};    // class deque

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_DEQUE_H_