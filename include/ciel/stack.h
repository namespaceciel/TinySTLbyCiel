#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_STACK_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_STACK_H_

#include <ciel/deque.h>

namespace ciel {

	// TODO: operator<=>

	template<class T, class Container = ciel::deque<T>>
	class stack {
	public:
		using container_type = Container;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using reference = typename container_type::reference;
		using const_reference = typename container_type::const_reference;

	protected:
		container_type c;

	public:
		stack() : stack(container_type()) {}

		explicit stack(const container_type& cont) : c(cont) {}

		explicit stack(container_type&& cont) : c(ciel::move(cont)) {}

		stack(const stack& other) : c(other.c) {}

		stack(stack&& other) noexcept : c(ciel::move(other.c)) {}

		template<ciel::legacy_input_iterator InputIt>
		stack(InputIt first, InputIt last) : c(first, last) {}

		template<class Alloc>
			requires ciel::uses_allocator_v<container_type, Alloc>
		explicit stack(const Alloc& alloc) : c(alloc) {}

		template<class Alloc>
			requires ciel::uses_allocator_v<container_type, Alloc>
		stack(const container_type& cont, const Alloc& alloc) : c(cont, alloc) {}

		template<class Alloc>
			requires ciel::uses_allocator_v<container_type, Alloc>
		stack(container_type&& cont, const Alloc& alloc) : c(ciel::move(cont), alloc) {}

		template<class Alloc>
			requires ciel::uses_allocator_v<container_type, Alloc>
		stack(const stack& other, const Alloc& alloc) : c(other.c, alloc) {}

		template<class Alloc>
			requires ciel::uses_allocator_v<container_type, Alloc>
		stack(stack&& other, const Alloc& alloc) : c(ciel::move(other.c), alloc) {}

		template<ciel::legacy_input_iterator InputIt, class Alloc>
			requires ciel::uses_allocator_v<container_type, Alloc>
		stack(InputIt first, InputIt last, const Alloc& alloc) : c(first, last, alloc) {}

		~stack() = default;

		stack& operator=(const stack& other) = default;

		stack& operator=(stack&& other) noexcept = default;

		reference top() {
			return c.back();
		}

		const_reference top() const {
			return c.back();
		}

		[[nodiscard]] bool empty() const {
			return c.empty();
		}

		[[nodiscard]] size_type size() const {
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
			c.pop_back();
		}

		void swap(stack& other) noexcept(ciel::is_nothrow_swappable_v<Container>) {
			ciel::swap(c, other.c);
		}

//		template<class U, class C>
//		friend bool operator==(const stack<U, C>& lhs, const stack<U, C>& rhs) {
//			return lhs.c == rhs.c;
//		}
//
//		template<class U, class C>
//		friend bool operator!=(const stack<U, C>& lhs, const stack<U, C>& rhs) {
//			return lhs.c != rhs.c;
//		}
//
//		template<class U, class C>
//		friend bool operator<(const stack<U, C>& lhs, const stack<U, C>& rhs) {
//			return lhs.c < rhs.c;
//		}
//
//		template<class U, class C>
//		friend bool operator<=(const stack<U, C>& lhs, const stack<U, C>& rhs) {
//			return lhs.c <= rhs.c;
//		}
//
//		template<class U, class C>
//		friend bool operator>(const stack<U, C>& lhs, const stack<U, C>& rhs) {
//			return lhs.c > rhs.c;
//		}
//
//		template<class U, class C>
//		friend bool operator>=(const stack<U, C>& lhs, const stack<U, C>& rhs) {
//			return lhs.c >= rhs.c;
//		}

	};    // class stack

	template<class T, class Container>
		requires ciel::is_swappable_v<Container>
	void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	template<class T, class Container, class Alloc>
	struct uses_allocator<stack<T, Container>, Alloc> : ciel::uses_allocator<Container, Alloc>::type {};

	template<class Container>
	stack(Container) -> stack<typename Container::value_type, Container>;

	template<ciel::legacy_input_iterator InputIt>
	stack(InputIt, InputIt) -> stack<typename ciel::iterator_traits<InputIt>::value_type>;

	template<class Container, class Alloc>
	    requires ciel::uses_allocator_v<Container, Alloc>
	stack(Container, Alloc) -> stack<typename Container::value_type, Container>;

	template<ciel::legacy_input_iterator InputIt, class Alloc>
	stack(InputIt, InputIt, Alloc) -> stack<typename ciel::iterator_traits<InputIt>::value_type, ciel::deque<typename ciel::iterator_traits<InputIt>::value_type, Alloc>>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_STACK_H_