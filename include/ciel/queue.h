#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_QUEUE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_QUEUE_H_

#include <ciel/vector.h>
#include <ciel/list.h>

namespace ciel {

	// TODO: ciel::deque
	template<class T, class Container = ciel::list<T>>
	class queue {

		static_assert(ciel::is_same_v<T, typename Container::value_type>, "ciel::queue 的 T 与底层容器的 T 不同");

	public:
		using container_type = Container;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;

	protected:
		container_type c;

	public:
		queue() : queue(container_type()) {}

		explicit queue(const container_type& cont) : c(cont) {}

		explicit queue(container_type&& cont) : c(ciel::move(cont)) {}

		queue(const queue& other) : c(other.c) {}

		queue(queue&& other) : c(ciel::move(other.c)) {}

		template<ciel::legacy_input_iterator InputIt>
		queue(InputIt first, InputIt last) : c(first, last) {}

		// TODO: 以下这些构造函数仅若 uses_allocator<Container, Alloc> 为 true ，即底层容器是知分配器容器（对所有标准库容器为 true ）才参与重载决议

		template<class Alloc>
		explicit queue(const Alloc& alloc) : c(alloc) {}

		template<class Alloc>
		queue(const container_type& cont, const Alloc& alloc) : c(cont, alloc) {}

		template<class Alloc>
		queue(container_type&& cont, const Alloc& alloc) : c(ciel::move(cont), alloc) {}

		template<class Alloc>
		queue(const queue& other, const Alloc& alloc) : c(other.c, alloc) {}

		template<class Alloc>
		queue(queue&& other, const Alloc& alloc) : c(ciel::move(other.c), alloc) {}

		template<ciel::legacy_input_iterator InputIt, class Alloc>
		queue(InputIt first, InputIt last, const Alloc& alloc) : c(first, last, alloc) {}

		~queue() = default;

		queue& operator=(const queue& other) = default;

		queue& operator=(queue&& other) = default;

		reference front() {
			return c.front();
		}

		const_reference front() const {
			return c.front();
		}

		reference back() {
			return c.back();
		}

		const_reference back() const {
			return c.back();
		}

		[[nodiscard]] bool empty() const {
			return c.empty();
		}

		size_type size() const {
			return c.size();
		}

		void push(const value_type& value) {
			c.push_back(value);
		}

		void push(value_type&& value) {
			c.push_back(ciel::move(value));
		}

		template<class... Args>
		decltype(auto) emplace(Args&& ... args) {
			return c.emplace_back(ciel::forward<Args>(args)...);
		}

		void pop() {
			c.pop_front();
		}

		void swap(queue& other) noexcept(ciel::is_nothrow_swappable_v<container_type>) {
			ciel::swap(c, other.c);
		}

	};    // class queue

	template<class T, class Container>
		requires ciel::is_swappable_v<Container>
	void swap(queue<T, Container>& lhs, queue<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	template<class T, class Container, class Alloc>
		requires ciel::uses_allocator_v<Container, Alloc>
	struct uses_allocator<queue<T, Container>, Alloc> : ciel::uses_allocator<Container, Alloc>::type {};

	template<class Container>
	queue(Container) -> queue<typename Container::value_type, Container>;

	template<ciel::legacy_input_iterator InputIt>
	queue(InputIt, InputIt) -> queue<typename ciel::iterator_traits<InputIt>::value_type>;

	template<class Container, class Alloc>
		requires ciel::uses_allocator_v<Container, Alloc>
	queue(Container, Alloc) -> queue<typename Container::value_type, Container>;

//	template<class InputIt, class Alloc>
//	queue(InputIt, InputIt, Alloc) -> queue<typename ciel::iterator_traits<InputIt>::value_type, std::deque<typename ciel::iterator_traits<InputIt>::value_type, Alloc>>;

	template<class T, class Container = ciel::vector<T>, class Compare = ciel::less<typename Container::value_type>>
	    requires ciel::legacy_random_access_iterator<typename Container::iterator>
	class priority_queue {

		static_assert(ciel::is_same_v<T, typename Container::value_type>, "ciel::priority_queue 的 T 与底层容器的 T 不同");

	public:
		using container_type = Container;
		using value_compare = Compare;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;

	protected:
		container_type c;
		[[no_unique_address]] value_compare comp;

	public:
		priority_queue() : priority_queue(value_compare(), container_type()) {}

		explicit priority_queue(const value_compare& compare) : priority_queue(compare, container_type()) {}

		priority_queue(const value_compare& compare, const container_type& cont) : c(cont), comp(compare) {
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		priority_queue(const value_compare& compare, container_type&& cont) : c(ciel::move(cont)), comp(compare) {
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		priority_queue(const priority_queue& other) = default;

		priority_queue(priority_queue&& other) = default;

		template<ciel::legacy_input_iterator InputIt>
		priority_queue(InputIt first, InputIt last, const value_compare& compare = value_compare()) : c(first, last), comp(compare) {
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		template<ciel::legacy_input_iterator InputIt>
		priority_queue(InputIt first, InputIt last, const value_compare& compare, const container_type& cont) : c(cont), comp(compare) {
			c.insert(c.end(), first, last);
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		template<ciel::legacy_input_iterator InputIt>
		priority_queue(InputIt first, InputIt last, const value_compare& compare, container_type&& cont) : c(ciel::move(cont)), comp(compare) {
			c.insert(c.end(), first, last);
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		// TODO: 以下这些重载只有在 uses_allocator<container_type, Alloc>::value 为 true ，即底层容器为知分配器容器（对所有标准库容器为真）时才会参与重载决议

		template<class Alloc>
		explicit priority_queue(const Alloc& alloc) : c(alloc), comp(value_compare()) {}

		template<class Alloc>
		priority_queue(const value_compare& compare, const Alloc& alloc) : c(alloc), comp(compare) {}

		template<class Alloc>
		priority_queue(const value_compare& compare, const container_type& cont, const Alloc& alloc) : c(cont, alloc), comp(compare) {
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		template<class Alloc>
		priority_queue(const value_compare& compare, container_type&& cont, const Alloc& alloc) : c(ciel::move(cont), alloc), comp(compare) {
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		template<class Alloc>
		priority_queue(const priority_queue& other, const Alloc& alloc) : c(other.c, alloc), comp(other.comp) {}

		template<class Alloc>
		priority_queue(priority_queue&& other, const Alloc& alloc) : c(ciel::move(other.c), alloc), comp(ciel::move(other.comp)) {}

		template<ciel::legacy_input_iterator InputIt, class Alloc>
		priority_queue(InputIt first, InputIt last, const Alloc& alloc) : c(alloc), comp(value_compare()) {
			c.insert(c.end(), first, last);
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		template<ciel::legacy_input_iterator InputIt, class Alloc>
		priority_queue(InputIt first, InputIt last, const value_compare& compare, const Alloc& alloc) : c(alloc), comp(compare) {
			c.insert(c.end(), first, last);
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		template<ciel::legacy_input_iterator InputIt, class Alloc>
		priority_queue(InputIt first, InputIt last, const value_compare& compare, const container_type& cont, const Alloc& alloc) : c(cont, alloc), comp(compare) {
			c.insert(c.end(), first, last);
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		template<ciel::legacy_input_iterator InputIt, class Alloc>
		priority_queue(InputIt first, InputIt last, const value_compare& compare, container_type&& cont, const Alloc& alloc) : c(ciel::move(cont), alloc), comp(compare) {
			c.insert(c.end(), first, last);
			ciel::make_heap(c.begin(), c.end(), comp);
		}

		~priority_queue() = default;

		priority_queue& operator=(const priority_queue& other) = default;

		priority_queue& operator=(priority_queue&& other) = default;

		const_reference top() const {
			return c.front();
		}

		[[nodiscard]] bool empty() const {
			return c.empty();
		}

		size_type size() const {
			return c.size();
		}

		void push(const value_type& value) {
			c.push_back(value);
			ciel::push_heap(c.begin(), c.end(), comp);
		}

		void push(value_type&& value) {
			c.push_back(ciel::move(value));
			ciel::push_heap(c.begin(), c.end(), comp);
		}

		template<class... Args>
		void emplace(Args&& ... args) {
			c.emplace_back(ciel::forward<Args>(args)...);
			ciel::push_heap(c.begin(), c.end(), comp);
		}

		void pop() {
			ciel::pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}

		void swap(priority_queue& other) noexcept(ciel::is_nothrow_swappable_v<container_type> && ciel::is_nothrow_swappable_v<value_compare>) {
			using ciel::swap;
			swap(c, other.c);
			swap(comp, other.comp);
		}

		// 为了 queue_test 方便开的后门
		const container_type& container() const {
			return c;
		}

	};    // class priority_queue

	template<class T, class Container, class Compare>
		requires ciel::is_swappable_v<Container> && ciel::is_swappable_v<Compare>
	void swap(priority_queue<T, Container, Compare>& lhs, priority_queue<T, Container, Compare>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	template<class T, class Container, class Compare, class Alloc>
	struct uses_allocator<priority_queue<T, Compare, Container>, Alloc> : ciel::uses_allocator<Container, Alloc>::type {};

	template<class Comp, class Container>
	priority_queue(Comp, Container) -> priority_queue<typename Container::value_type, Container, Comp>;

	template<class InputIt, class Comp = ciel::less<typename ciel::iterator_traits<InputIt>::value_type>, class Container = ciel::vector<typename ciel::iterator_traits<InputIt>::value_type>>
	priority_queue(InputIt, InputIt, Comp = Comp(), Container = Container()) -> priority_queue<typename ciel::iterator_traits<InputIt>::value_type, Container, Comp>;

	template<class Comp, class Container, class Alloc>
	priority_queue(Comp, Container, Alloc) -> priority_queue<typename Container::value_type, Container, Comp>;

	template<class InputIt, class Alloc>
	priority_queue(InputIt, InputIt, Alloc) -> priority_queue<typename ciel::iterator_traits<InputIt>::value_type, ciel::vector<typename ciel::iterator_traits<InputIt>::value_type, Alloc>, ciel::less<typename ciel::iterator_traits<InputIt>::value_type>>;

	template<class InputIt, class Comp, class Alloc>
	priority_queue(InputIt, InputIt, Comp, Alloc) -> priority_queue<typename ciel::iterator_traits<InputIt>::value_type, ciel::vector<typename ciel::iterator_traits<InputIt>::value_type, Alloc>, Comp>;

	template<class InputIt, class Comp, class Container, class Alloc>
	priority_queue(InputIt, InputIt, Comp, Container, Alloc) -> priority_queue<typename Container::value_type, Container, Comp>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_QUEUE_H_