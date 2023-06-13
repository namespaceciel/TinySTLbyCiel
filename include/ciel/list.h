#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_LIST_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_LIST_H_

#include <ciel/memory.h>
#include <ciel/iterator.h>
#include <ciel/algorithm.h>

namespace ciel {

	// 感觉常规实现有点丢西瓜捡芝麻了，让 end 结点作为栈上内存使得移动构造/赋值要考虑的更多（因为其不是指针，所以交换过来再让原 end 结点置空就不可行）
	// 当然我觉得 end 结点作为栈上内存的主要考量估计是保证空 list 不需要内存分配
	// 而 end 结点因为不用存储数据，让 list_node 拆分出 list_node_base 也让事情复杂化了

	struct list_node_base {
		list_node_base* prev;
		list_node_base* next;

		list_node_base() : prev(this), next(this) {}
		list_node_base(list_node_base* p, list_node_base* n) : prev(p), next(n) {}

		list_node_base(const list_node_base& other) : prev(other.prev), next(other.next) {}

		void clear() {
			prev = this;
			next = this;
		}

	};    // struct list_node_base

	template<class T>
	struct list_node : list_node_base {
		T value;

		template<class... Args>
		list_node(list_node_base* p, list_node_base* n, Args&& ... args) : value(ciel::forward<Args>(args)...), list_node_base(p, n) {}

	};    // struct list_node

	template<class T, class Pointer, class Reference>
	class list_iterator {
	public:
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = Pointer;
		using reference = Reference;
		using iterator_category = ciel::bidirectional_iterator_tag;
		using iterator_concept = ciel::bidirectional_iterator_tag;

	private:
		using base_node_type = list_node_base;
		using node_type = list_node<value_type>;

		base_node_type* it;

	public:
		list_iterator() noexcept: it(nullptr) {}

		list_iterator(const base_node_type* p) noexcept: it(const_cast<base_node_type*>(p)) {}    // const_cast 是为了 const_iterator 的初始化

		list_iterator(const list_iterator& other) noexcept: it(other.it) {}

		list_iterator next() const {
			return list_iterator(it->next);
		}

		list_iterator prev() const {
			return list_iterator(it->prev);
		}

		reference operator*() const noexcept {
			return (static_cast<node_type*>(it))->value;
		}

		// FIXME: 这里感觉不是很理想，因为想通过迭代器的 ->next 直接拿到 next 指针
		pointer operator->() const noexcept {
			return &static_cast<node_type*>(it)->value;
		}

		list_iterator& operator++() noexcept {
			it = it->next;
			return *this;
		}

		list_iterator operator++(int) noexcept {
			list_iterator res(it);
			it = it->next;
			return res;
		}

		list_iterator& operator--() noexcept {
			it = it->prev;
			return *this;
		}

		list_iterator operator--(int) noexcept {
			list_iterator res(it);
			it = it->prev;
			return res;
		}

		base_node_type* base() const noexcept {
			return it;
		}

	};    // class list_iterator

	template<class T, class Pointer, class Reference>
	bool operator==(const list_iterator<T, Pointer, Reference>& lhs, const list_iterator<T, Pointer, Reference>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<class T, class Pointer, class Reference>
	bool operator!=(const list_iterator<T, Pointer, Reference>& lhs, const list_iterator<T, Pointer, Reference>& rhs) {
		return !(lhs == rhs);
	}

	template<class T, class Allocator = ciel::allocator<T>>
	class list {

		static_assert(ciel::is_same_v<typename Allocator::value_type, T>, "ciel::list 要求 Allocator::value_type 与 T 相同");

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;

		using pointer = ciel::allocator_traits<allocator_type>::pointer;
		using const_pointer = ciel::allocator_traits<allocator_type>::const_pointer;

		using iterator = list_iterator<value_type, pointer, reference>;
		using const_iterator = list_iterator<value_type, const_pointer, const_reference>;

		using reverse_iterator = ciel::reverse_iterator<iterator>;
		using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;;

	private:
		using base_node_type = list_node_base;
		using node_type = list_node<value_type>;

		using alloc_traits = ciel::allocator_traits<allocator_type>;
		// 不同于 vector 分配的内存完全用于元素本身，list 在内的大多数容器都需要额外空间维持运作，allocator<T> 的分配是有问题的，所以需要 rebind_alloc 出手
		using node_allocator = alloc_traits::template rebind_alloc<node_type>;
		using node_alloc_traits = alloc_traits::template rebind_traits<node_type>;

		base_node_type end_node;
		size_type s;
		[[no_unique_address]] node_allocator allocator;

		// 由于回收内存的单位是单个节点，所以即使在一次插入多个节点的时候也需要为每个节点单独申请一次内存
		iterator alloc_range_destroy_and_deallocate(node_allocator& a, iterator begin, iterator end) noexcept {    // end->prev 指向 before_begin，before_begin->next 指向 end
			s -= ciel::distance(begin, end);
			iterator loop = begin;
			iterator before_begin = begin.prev();
			while (loop != end) {
				node_type* to_be_destroyed = static_cast<node_type*>(loop.base());
				++loop;
				node_alloc_traits::destroy(a, to_be_destroyed);
				node_alloc_traits::deallocate(a, to_be_destroyed, 1);
			}
			before_begin.base()->next = end.base();
			end.base()->prev = before_begin.base();
			return end;
		}

		template<class... Arg>
		iterator alloc_range_allocate_and_construct_n(node_allocator& a, iterator begin, size_type n, Arg&& ... arg) {
			if (n == 0) {
				return begin;
			}
			iterator before_begin = begin.prev();    // 在 before_begin 和 begin 间塞，TODO: begin 应改成 insert_place 之类的名字
			iterator original_before_begin = before_begin;
			// 内层 try-catch 是为了在申请到内存后构造时抛出异常而释放掉内存，外层 try-catch 是为了析构并释放已经插入的结点（强异常安全）
			try {
				for (size_type i = 0; i < n; ++i) {
					node_type* construct_place = node_alloc_traits::allocate(a, 1);
					try {
						node_alloc_traits::construct(a, construct_place, before_begin.base(), begin.base(), ciel::forward<Arg>(arg)...);
						++s;
						before_begin.base()->next = construct_place;
						begin.base()->prev = construct_place;
						++before_begin;
					} catch (...) {
						node_alloc_traits::deallocate(a, construct_place, 1);
						throw;
					}
				}
				return before_begin;
			} catch (...) {
				alloc_range_destroy_and_deallocate(a, original_before_begin.next(), begin);
				throw;
			}
		}

		template<ciel::legacy_input_iterator InputIt>
		iterator alloc_range_allocate_and_construct(node_allocator& a, iterator begin, InputIt first, InputIt last) {
			if (first == last) {
				return begin;
			}
			iterator before_begin = begin.prev();    // 在 before_begin 和 begin 间塞，TODO: begin 应改成 insert_place 之类的名字
			iterator original_before_begin = before_begin;
			// 内层 try-catch 是为了在申请到内存后构造时抛出异常而释放掉内存，外层 try-catch 是为了析构并释放已经插入的结点（强异常安全）
			try {
				while (first != last) {
					node_type* construct_place = node_alloc_traits::allocate(a, 1);
					try {
						node_alloc_traits::construct(a, construct_place, before_begin.base(), begin.base(), *first++);
						++s;
						before_begin.base()->next = construct_place;
						begin.base()->prev = construct_place;
						++before_begin;
					} catch (...) {
						node_alloc_traits::deallocate(a, construct_place, 1);
						throw;
					}
				}
				return before_begin;
			} catch (...) {
				alloc_range_destroy_and_deallocate(a, original_before_begin.next(), begin);
				throw;
			}
		}

	public:
		list() : end_node(), s(0), allocator() {}

		explicit list(const allocator_type& alloc) : end_node(), s(0), allocator(alloc) {}

		list(size_type count, const T& value, const allocator_type& alloc = allocator_type()) : end_node(), s(0), allocator(alloc) {
			alloc_range_allocate_and_construct_n(allocator, end(), count, value);
		}

		explicit list(size_type count, const allocator_type& alloc = allocator_type()) : end_node(), s(0), allocator(alloc) {
			alloc_range_allocate_and_construct_n(allocator, end(), count);
		}

		template<ciel::legacy_input_iterator InputIt>
		list(InputIt first, InputIt last, const allocator_type& alloc = allocator_type()) : end_node(), s(0), allocator(alloc) {
			alloc_range_allocate_and_construct(allocator, end(), first, last);
		}

		list(const list& other) : list(iterator(other.begin().base()), iterator(other.end().base()), other.allocator) {}

		list(const list& other, const allocator_type& alloc) : list(iterator(other.begin().base()), iterator(other.end().base()), alloc) {}

		list(list&& other) : end_node(other.end_node), s(other.s), allocator(other.allocator) {
			end_node.next->prev = &end_node;
			end_node.prev->next = &end_node;
			other.end_node.clear();
			other.s = 0;
		}

		// TODO: 如果 alloc != other.get_allocator() ，那么它会导致逐元素移动
		list(list&& other, const allocator_type& alloc) : end_node(other.end_node), s(other.s), allocator(alloc) {
			end_node.next->prev = &end_node;
			end_node.prev->next = &end_node;
			other.end_node.clear();
			other.s = 0;
		}

		list(std::initializer_list<T> init, const allocator_type& alloc = allocator_type()) : list(init.begin(), init.end(), alloc) {}

		~list() {
			clear();
		}

		// TODO: 详见 cppreference 此节对于分配器的注解

		list& operator=(const list& other) {
			clear();
			alloc_range_allocate_and_construct(allocator, end(), iterator(other.begin().base()), iterator(other.end().base()));
			return *this;
		}

		list& operator=(list&& other) noexcept(alloc_traits::is_always_equal::value) {
			clear();
			end_node = other.end_node;
			end_node.next->prev = &end_node;
			end_node.prev->next = &end_node;
			other.end_node.clear();
			s = other.s;
			other.s = 0;
			return *this;
		}

		list& operator=(std::initializer_list<T> ilist) {
			clear();
			alloc_range_allocate_and_construct(allocator, end(), ilist.begin(), ilist.end());
			return *this;
		}

		void assign(size_type count, const T& value) {
			clear();
			alloc_range_allocate_and_construct_n(allocator, end(), count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		void assign(InputIt first, InputIt last) {
			clear();
			alloc_range_allocate_and_construct(allocator, end(), first, last);
		}

		void assign(std::initializer_list<T> ilist) {
			clear();
			alloc_range_allocate_and_construct(allocator, end(), ilist.begin(), ilist.end());
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

		reference back() {
			return *--end();
		}

		const_reference back() const {
			return *--end();
		}

		iterator begin() noexcept {
			return iterator(end_node.next);
		}

		const_iterator begin() const noexcept {
			return const_iterator(end_node.next);
		}

		const_iterator cbegin() const noexcept {
			return begin();
		}

		iterator end() noexcept {
			return iterator(&end_node);
		}

		const_iterator end() const noexcept {
			return const_iterator(&end_node);
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
			return size() == 0;
		}

		[[nodiscard]] size_type size() const noexcept {
			return s;
		}

		[[nodiscard]] size_type max_size() const noexcept {
			return ciel::numeric_limits<difference_type>::max();
		}

		void clear() noexcept {
			alloc_range_destroy_and_deallocate(allocator, begin(), end());
		}

		iterator insert(iterator pos, const T& value) {
			return alloc_range_allocate_and_construct_n(allocator, pos, 1, value);
		}

		iterator insert(iterator pos, T&& value) {
			return alloc_range_allocate_and_construct_n(allocator, pos, 1, ciel::move(value));
		}

		iterator insert(iterator pos, size_type count, const T& value) {
			return alloc_range_allocate_and_construct_n(allocator, pos, count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		iterator insert(iterator pos, InputIt first, InputIt last) {
			return alloc_range_allocate_and_construct(allocator, pos, first, last);
		}

		iterator insert(iterator pos, std::initializer_list<T> ilist) {
			return alloc_range_allocate_and_construct(allocator, pos, ilist.begin(), ilist.end());
		}

		template<class... Args>
		iterator emplace(iterator pos, Args&& ... args) {
			return alloc_range_allocate_and_construct_n(allocator, pos, 1, ciel::forward<Args>(args)...);
		}

		iterator erase(iterator pos) {
			return alloc_range_destroy_and_deallocate(allocator, pos, pos.next());
		}

		iterator erase(iterator first, iterator last) {
			return alloc_range_destroy_and_deallocate(allocator, first, last);
		}

		void push_back(const T& value) {
			emplace_back(value);
		}

		void push_back(T&& value) {
			emplace_back(ciel::move(value));
		}

		template<class... Args>
		reference emplace_back(Args&& ... args) {
			return *alloc_range_allocate_and_construct_n(allocator, end(), 1, ciel::forward<Args>(args)...);
		}

		void pop_back() {
			alloc_range_destroy_and_deallocate(allocator, end().prev(), end());
		}

		void push_front(const T& value) {
			emplace_front(value);
		}

		void push_front(T&& value) {
			emplace_front(ciel::move(value));
		}

		template<class... Args>
		reference emplace_front(Args&& ... args) {
			return *alloc_range_allocate_and_construct_n(allocator, begin(), 1, ciel::forward<Args>(args)...);
		}

		void pop_front() {
			alloc_range_destroy_and_deallocate(allocator, begin(), begin().next());
		}

		void resize(size_type count) {
			if (size() >= count) {
				iterator tmp = ciel::prev(end(), size() - count);
				alloc_range_destroy_and_deallocate(allocator, tmp, end());
			} else {
				alloc_range_allocate_and_construct_n(allocator, end(), count - size());
			}
		}

		void resize(size_type count, const value_type& value) {
			if (size() >= count) {
				iterator tmp = ciel::prev(end(), size() - count);
				alloc_range_destroy_and_deallocate(allocator, tmp, end());
			} else {
				alloc_range_allocate_and_construct_n(allocator, end(), count - size(), value);
			}
		}

		// TODO: 如果 alloc_traits::propagate_on_container_swap::value 是 true，那么就会用对非成员 swap 的无限定调用交换分配器。否则，不交换它们（且在 get_allocator() != other.get_allocator() 时行为未定义）
		void swap(list& other) noexcept(alloc_traits::is_always_equal::value) {
			ciel::swap(end_node, other.end_node);
			end_node.next->prev = &end_node;
			end_node.prev->next = &end_node;
			other.end_node.next->prev = &other.end_node;
			other.end_node.prev->next = &other.end_node;
			ciel::swap(s, other.s);
			ciel::swap(allocator, other.allocator);
		}

		// merge 应保证稳定性：*this 中与 other 的等价元素始终排在前面
//		void merge(list& other) {
//			merge(other, ciel::less<value_type>());
//		}
//
//		void merge(list&& other) {
//			merge(ciel::move(other), ciel::less<value_type>());
//		}
//
//		template<class Compare>
//		void merge(list& other, Compare comp);
//
//		template<class Compare>
//		void merge(list&& other, Compare comp);

	};    // class list

	template<class T, class Alloc>
	bool operator==(const list<T, Alloc>& lhs, const list<T, Alloc>& rhs) {
		if (lhs.size() != rhs.size()) {
			return false;
		}
		return ciel::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<class T, class Alloc>
	void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	template<class T, class Alloc, class U>
	typename list<T, Alloc>::size_type erase(list<T, Alloc>& c, const U& value) {
		return c.remove_if([&](auto& elem) { return elem == value; });
	}

	template<class T, class Alloc, class Pred>
	typename list<T, Alloc>::size_type erase_if(list<T, Alloc>& c, Pred pred) {
		return c.remove_if(pred);
	}

	template<ciel::legacy_input_iterator InputIt, class Alloc = ciel::allocator<typename ciel::iterator_traits<InputIt>::value_type>>
	list(InputIt, InputIt, Alloc = Alloc()) -> list<typename ciel::iterator_traits<InputIt>::value_type, Alloc>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_LIST_H_