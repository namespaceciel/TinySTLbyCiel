#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FORWARD_LIST_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FORWARD_LIST_H_

#include <ciel/memory.h>
#include <ciel/iterator.h>
#include <ciel/algorithm.h>

namespace ciel {

	// 为了所谓的轻量化，forward_list 标准里没有 size()，有点无语
	// 不同于 list，forward_list 不是环状的，好处是 end() 的返回值可以直接是 iterator(nullptr)
	// 注意这个容器的大部分操作区间都是前后双开区间

	struct forward_list_node_base {
		forward_list_node_base* next;

		forward_list_node_base() : next(nullptr) {}
		forward_list_node_base(forward_list_node_base* n) : next(n) {}

		forward_list_node_base(const forward_list_node_base& other) : next(other.next) {}

		void clear() {
			next = nullptr;
		}

	};    // struct forward_list_node_base

	template<class T>
	struct forward_list_node : forward_list_node_base {
		T value;

		template<class... Args>
		forward_list_node(forward_list_node_base* n, Args&& ... args) : value(ciel::forward<Args>(args)...), forward_list_node_base(n) {}

	};    // struct forward_list_node

	template<class T, class Pointer, class Reference>
	class forward_list_iterator {
	public:
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = Pointer;
		using reference = Reference;
		using iterator_category = ciel::forward_iterator_tag;
		using iterator_concept = ciel::forward_iterator_tag;

	private:
		using base_node_type = forward_list_node_base;
		using node_type = forward_list_node<value_type>;

		base_node_type* it;

	public:
		forward_list_iterator() noexcept: it(nullptr) {}

		forward_list_iterator(const base_node_type* p) noexcept: it(const_cast<base_node_type*>(p)) {}    // const_cast 是为了 const_iterator 的初始化

		forward_list_iterator(const forward_list_iterator& other) noexcept: it(other.it) {}

		forward_list_iterator next() const {
			return forward_list_iterator(it->next);
		}

		reference operator*() const noexcept {
			return (static_cast<node_type*>(it))->value;
		}

		// FIXME: 这里感觉不是很理想，因为想通过迭代器的 ->next 直接拿到 next 指针
		pointer operator->() const noexcept {
			return &static_cast<node_type*>(it)->value;
		}

		forward_list_iterator& operator++() noexcept {
			it = it->next;
			return *this;
		}

		forward_list_iterator operator++(int) noexcept {
			forward_list_iterator res(it);
			it = it->next;
			return res;
		}

		base_node_type* base() const noexcept {
			return it;
		}

	};    // class list_iterator

	template<class T, class Pointer, class Reference>
	bool operator==(const forward_list_iterator<T, Pointer, Reference>& lhs, const forward_list_iterator<T, Pointer, Reference>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<class T, class Pointer, class Reference>
	bool operator!=(const forward_list_iterator<T, Pointer, Reference>& lhs, const forward_list_iterator<T, Pointer, Reference>& rhs) {
		return !(lhs == rhs);
	}

	template<class T, class Allocator = ciel::allocator<T>>
	class forward_list {

		static_assert(ciel::is_same_v<typename Allocator::value_type, T>, "ciel::forward_list 要求 Allocator::value_type 与 T 相同");

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;

		using pointer = ciel::allocator_traits<allocator_type>::pointer;
		using const_pointer = ciel::allocator_traits<allocator_type>::const_pointer;

		using iterator = forward_list_iterator<value_type, pointer, reference>;
		using const_iterator = forward_list_iterator<value_type, const_pointer, const_reference>;

	private:
		using base_node_type = forward_list_node_base;
		using node_type = forward_list_node<value_type>;

		using alloc_traits = ciel::allocator_traits<allocator_type>;
		// 不同于 vector 分配的内存完全用于元素本身，forward_list 在内的大多数容器都需要额外空间维持运作，allocator<T> 的分配是有问题的，所以需要 rebind_alloc 出手
		using node_allocator = alloc_traits::template rebind_alloc<node_type>;
		using node_alloc_traits = alloc_traits::template rebind_traits<node_type>;

		base_node_type bb;
		[[no_unique_address]] node_allocator allocator;

		// 由于回收内存的单位是单个节点，所以即使在一次插入多个节点的时候也需要为每个节点单独申请一次内存
		iterator alloc_range_destroy_and_deallocate(node_allocator& a, iterator begin, iterator end) noexcept {    // 注意这里移除的是 (begin, end) 的开区间
			iterator loop = begin.next();
			while (loop != end) {
				node_type* to_be_destroyed = static_cast<node_type*>(loop.base());
				++loop;
				node_alloc_traits::destroy(a, to_be_destroyed);
				node_alloc_traits::deallocate(a, to_be_destroyed, 1);
			}
			begin.base()->next = end.base();
			return end;
		}

		template<class... Arg>
		iterator alloc_range_allocate_and_construct_n(node_allocator& a, iterator begin, size_type n, Arg&& ... arg) {
			if (n == 0) {
				return begin;
			}
			iterator original_begin = begin;
			iterator after_begin = begin.next();
			// 内层 try-catch 是为了在申请到内存后构造时抛出异常而释放掉内存，外层 try-catch 是为了析构并释放已经插入的结点（强异常安全）
			try {
				for (size_type i = 0; i < n; ++i) {
					node_type* construct_place = node_alloc_traits::allocate(a, 1);
					try {
						node_alloc_traits::construct(a, construct_place, after_begin.base(), ciel::forward<Arg>(arg)...);
						begin.base()->next = construct_place;
						++begin;
					} catch (...) {
						node_alloc_traits::deallocate(a, construct_place, 1);
						throw;
					}
				}
				return begin;
			} catch (...) {
				alloc_range_destroy_and_deallocate(a, original_begin.next(), begin);
				throw;
			}
		}

		template<ciel::legacy_input_iterator InputIt>
		iterator alloc_range_allocate_and_construct(node_allocator& a, iterator begin, InputIt first, InputIt last) {
			if (first == last) {
				return begin;
			}
			iterator original_begin = begin;
			iterator after_begin = begin.next();
			// 内层 try-catch 是为了在申请到内存后构造时抛出异常而释放掉内存，外层 try-catch 是为了析构并释放已经插入的结点（强异常安全）
			try {
				while (first != last) {
					node_type* construct_place = node_alloc_traits::allocate(a, 1);
					try {
						node_alloc_traits::construct(a, construct_place, after_begin.base(), *first++);
						begin.base()->next = construct_place;
						++begin;
					} catch (...) {
						node_alloc_traits::deallocate(a, construct_place, 1);
						throw;
					}
				}
				return begin;
			} catch (...) {
				alloc_range_destroy_and_deallocate(a, original_begin.next(), begin);
				throw;
			}
		}

	public:
		forward_list() : bb(), allocator() {}

		explicit forward_list(const allocator_type& alloc) : bb(), allocator(alloc) {}

		forward_list(size_type count, const T& value, const allocator_type& alloc = allocator_type()) : bb(), allocator(alloc) {
			alloc_range_allocate_and_construct_n(allocator, before_begin(), count, value);
		}

		explicit forward_list(size_type count, const allocator_type& alloc = allocator_type()) : bb(), allocator(alloc) {
			alloc_range_allocate_and_construct_n(allocator, before_begin(), count);
		}

		template<ciel::legacy_input_iterator InputIt>
		forward_list(InputIt first, InputIt last, const allocator_type& alloc = allocator_type()) : bb(), allocator(alloc) {
			alloc_range_allocate_and_construct(allocator, before_begin(), first, last);
		}

		forward_list(const forward_list& other) : forward_list(iterator(other.begin().base()), iterator(other.end().base()), other.allocator) {}

		forward_list(const forward_list& other, const allocator_type& alloc) : forward_list(iterator(other.begin().base()), iterator(other.end().base()), alloc) {}

		forward_list(forward_list&& other) : bb(other.bb), allocator(other.allocator) {
			other.bb.clear();
		}

		// TODO: 如果 alloc != other.get_allocator() ，那么它会导致逐元素移动
		forward_list(forward_list&& other, const allocator_type& alloc) : bb(other.bb), allocator(alloc) {
			other.bb.clear();
		}

		forward_list(std::initializer_list<T> init, const allocator_type& alloc = allocator_type()) : forward_list(init.begin(), init.end(), alloc) {}

		~forward_list() {
			clear();
		}

		// TODO: 详见 cppreference 此节对于分配器的注解

		forward_list& operator=(const forward_list& other) {
			clear();
			alloc_range_allocate_and_construct(allocator, before_begin(), iterator(other.begin().base()), iterator(other.end().base()));
			return *this;
		}

		forward_list& operator=(forward_list&& other) noexcept(alloc_traits::is_always_equal::value) {
			clear();
			bb = other.bb;
			other.bb.clear();
			return *this;
		}

		forward_list& operator=(std::initializer_list<T> ilist) {
			clear();
			alloc_range_allocate_and_construct(allocator, before_begin(), ilist.begin(), ilist.end());
			return *this;
		}

		void assign(size_type count, const T& value) {
			clear();
			alloc_range_allocate_and_construct_n(allocator, before_begin(), count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		void assign(InputIt first, InputIt last) {
			clear();
			alloc_range_allocate_and_construct(allocator, before_begin(), first, last);
		}

		void assign(std::initializer_list<T> ilist) {
			clear();
			alloc_range_allocate_and_construct(allocator, before_begin(), ilist.begin(), ilist.end());
		}

		allocator_type get_allocator() const noexcept {
			return allocator;
		}

		reference front() {
			return *begin();
		}

		const_reference front() const {
			return *begin();
		}

		iterator before_begin() noexcept {
			return iterator(&bb);
		}

		const_iterator before_begin() const noexcept {
			return const_iterator(&bb);
		}

		const_iterator cbefore_begin() const noexcept {
			return before_begin();
		}

		iterator begin() noexcept {
			return iterator(bb.next);
		}

		const_iterator begin() const noexcept {
			return const_iterator(bb.next);
		}

		const_iterator cbegin() const noexcept {
			return begin();
		}

		iterator end() noexcept {
			return iterator(nullptr);
		}

		const_iterator end() const noexcept {
			return const_iterator(nullptr);
		}

		const_iterator cend() const noexcept {
			return end();
		}

		[[nodiscard]] bool empty() const noexcept {
			return begin() == end();
		}

		[[nodiscard]] size_type size() const noexcept {
			size_type res = 0;
			iterator loop = iterator(before_begin().base());
			while (loop.base()->next) {
				++loop;
				++res;
			}
			return res;
		}

		[[nodiscard]] size_type max_size() const noexcept {
			return ciel::numeric_limits<difference_type>::max();
		}

		void clear() noexcept {
			alloc_range_destroy_and_deallocate(allocator, before_begin(), end());
		}

		iterator insert_after(iterator pos, const T& value) {
			return alloc_range_allocate_and_construct_n(allocator, pos, 1, value);
		}

		iterator insert_after(iterator pos, T&& value) {
			return alloc_range_allocate_and_construct_n(allocator, pos, 1, ciel::move(value));
		}

		iterator insert_after(iterator pos, size_type count, const T& value) {
			return alloc_range_allocate_and_construct_n(allocator, pos, count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		iterator insert_after(iterator pos, InputIt first, InputIt last) {
			return alloc_range_allocate_and_construct(allocator, pos, first, last);
		}

		iterator insert_after(iterator pos, std::initializer_list<T> ilist) {
			return alloc_range_allocate_and_construct(allocator, pos, ilist.begin(), ilist.end());
		}

		template<class... Args>
		iterator emplace_after(iterator pos, Args&& ... args) {
			return alloc_range_allocate_and_construct_n(allocator, pos, 1, ciel::forward<Args>(args)...);
		}

		iterator erase_after(iterator pos) {
			if (pos.next() == end()) {
				return end();
			}
			return alloc_range_destroy_and_deallocate(allocator, pos, pos.next().next());
		}

		iterator erase_after(iterator first, iterator last) {
			return alloc_range_destroy_and_deallocate(allocator, first, last);
		}

		void push_front(const T& value) {
			emplace_front(value);
		}

		void push_front(T&& value) {
			emplace_front(ciel::move(value));
		}

		template<class... Args>
		reference emplace_front(Args&& ... args) {
			return *alloc_range_allocate_and_construct_n(allocator, before_begin(), 1, ciel::forward<Args>(args)...);
		}

		void pop_front() {
			if (empty()) {
				return;
			}
			alloc_range_destroy_and_deallocate(allocator, before_begin(), begin().next());
		}

		void resize(size_type count) {
			size_type s = size();
			if (s >= count) {
				iterator tmp = ciel::next(before_begin(), count);
				alloc_range_destroy_and_deallocate(allocator, tmp, end());
			} else {
				iterator tmp = ciel::next(before_begin(), s);
				alloc_range_allocate_and_construct_n(allocator, tmp, count - s);
			}
		}

		void resize(size_type count, const value_type& value) {
			size_type s = size();
			if (s >= count) {
				iterator tmp = ciel::next(before_begin(), count);
				alloc_range_destroy_and_deallocate(allocator, tmp, end());
			} else {
				iterator tmp = ciel::next(before_begin(), s);
				alloc_range_allocate_and_construct_n(allocator, tmp, count - s, value);
			}
		}

		void swap(forward_list& other) noexcept(alloc_traits::is_always_equal::value) {
			ciel::swap(bb, other.bb);
			ciel::swap(allocator, other.allocator);
		}

	};    // class forward_list

	template<class T, class Alloc>
	bool operator==(const forward_list<T, Alloc>& lhs, const forward_list<T, Alloc>& rhs) {
		auto l = lhs.begin();
		auto r = rhs.begin();
		while (l != lhs.end() && r != rhs.end()) {
			if (*l == *r) {
				++l;
				++r;
			} else {
				return false;
			}
		}
		return l == lhs.end() && r == rhs.end();
	}

	template<class T, class Alloc>
	void swap(forward_list<T, Alloc>& lhs, forward_list<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	template<class T, class Alloc, class U>
	typename forward_list<T, Alloc>::size_type erase(forward_list<T, Alloc>& c, const U& value) {
		return c.remove_if([&](auto& elem) { return elem == value; });
	}

	template<class T, class Alloc, class Pred>
	typename forward_list<T, Alloc>::size_type erase_if(forward_list<T, Alloc>& c, Pred pred) {
		return c.remove_if(pred);
	}

	template<ciel::legacy_input_iterator InputIt, class Alloc = ciel::allocator<typename ciel::iterator_traits<InputIt>::value_type>>
	forward_list(InputIt, InputIt, Alloc = Alloc()) -> forward_list<typename ciel::iterator_traits<InputIt>::value_type, Alloc>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FORWARD_LIST_H_