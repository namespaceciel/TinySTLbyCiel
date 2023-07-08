#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_REVERSE_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_REVERSE_ITERATOR_H_

#include <ciel/iterator_impl/random_access_iterator.h>
#include <ciel/iterator_impl/prev.h>

namespace ciel {

	namespace reverse_iterator_details {

		template<class Iter>
		struct what_iterator_concept {
			using iterator_concept = ciel::bidirectional_iterator_tag;
		};

		template<class Iter>
		    requires ciel::random_access_iterator<Iter>
		struct what_iterator_concept<Iter> {
			using iterator_concept = ciel::random_access_iterator_tag;
		};

		template<class Iter>
		struct what_iterator_category {
			using iterator_category = typename ciel::iterator_traits<Iter>::iterator_category;
		};

		template<class Iter>
            requires ciel::derived_from<typename ciel::iterator_traits<Iter>::iterator_category, ciel::random_access_iterator_tag>
		struct what_iterator_category<Iter> {
			using iterator_category = ciel::random_access_iterator_tag;
		};
	}

	template<class Iter>
		requires ciel::legacy_bidirectional_iterator<Iter> || ciel::bidirectional_iterator<Iter>
	class reverse_iterator {
	public:
		using iterator_type = Iter;
		using iterator_concept = typename reverse_iterator_details::what_iterator_concept<Iter>::iterator_concept;
		using iterator_catetory = typename reverse_iterator_details::what_iterator_category<Iter>::iterator_category;
		using value_type = ciel::iter_value_t<Iter>;
		using difference_type = ciel::iter_difference_t<Iter>;
		using pointer = typename ciel::iterator_traits<Iter>::pointer;
		using reference = ciel::iter_reference_t<Iter>;

	protected:
		iterator_type current;

	public:
		constexpr reverse_iterator() : current() {}

		constexpr explicit reverse_iterator(iterator_type x) : current(x) {}

		template<class U>
			requires (!ciel::is_same_v<Iter, U> && ciel::convertible_to<const U&, Iter>)
		constexpr reverse_iterator(const reverse_iterator<U>& other):current(other.base()) {}

		template<class U>
			requires (!ciel::is_same_v<Iter, U> && ciel::convertible_to<const U&, Iter> && ciel::assignable_from<Iter&, const U&>)
		constexpr reverse_iterator& operator=(const reverse_iterator<U>& other) {
			current = other.base();
			return *this;
		}

		constexpr iterator_type base() const {
			return current;
		}

		constexpr reference operator*() const {
			iterator_type tmp = current;
			return *--tmp;
		}

		constexpr pointer operator->() const requires ciel::is_pointer_v<Iter> {
			return current - 1;
		}

		constexpr pointer operator->() const requires (!ciel::is_pointer_v<Iter> && requires(const Iter i) { i.operator->(); }) {
			return ciel::prev(current).operator->();
		}

		constexpr reverse_iterator& operator++() {
			--current;
			return *this;
		}

		constexpr reverse_iterator operator++(int) {
			reverse_iterator res(*this);
			--current;
			return res;
		}

		constexpr reverse_iterator& operator--() {
			++current;
			return *this;
		}

		constexpr reverse_iterator operator--(int) {
			reverse_iterator res(*this);
			++current;
			return res;
		}

		constexpr reverse_iterator operator+(difference_type n) const {
			return reverse_iterator(current - n);
		}

		constexpr reverse_iterator& operator+=(difference_type n) {
			current -= n;
			return *this;
		}

		constexpr reverse_iterator operator-(difference_type n) const {
			return reverse_iterator(current + n);
		}

		constexpr reverse_iterator& operator-=(difference_type n) {
			current += n;
			return *this;
		}

		constexpr reference operator[](difference_type n) const {
			return *(*this + n);
		}

		friend constexpr ciel::iter_rvalue_reference_t<Iter> iter_move(const reverse_iterator& i) noexcept(ciel::is_nothrow_copy_constructible_v<Iter> && noexcept(ciel::ranges::iter_move(--ciel::declval<Iter&>()))) {
			auto tmp = i.base();
			return ciel::ranges::iter_move(--tmp);
		}

	};    // class reverse_iterator

	template<class Iter>
	constexpr reverse_iterator<Iter> make_reverse_iterator(Iter i) {
		return reverse_iterator<Iter>(i);
	}

	template<class Iterator1, class Iterator2>
	constexpr bool operator==(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) requires requires{{ lhs.base() == rhs.base() } -> ciel::boolean_testable; } {
		return lhs.base() == rhs.base();
	}

	template<class Iterator1, class Iterator2>
	constexpr bool operator!=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)requires requires{{ lhs.base() != rhs.base() } -> ciel::boolean_testable; } {
		return lhs.base() != rhs.base();
	}

	template<class Iterator1, class Iterator2>
	constexpr bool operator<(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)requires requires{{ lhs.base() > rhs.base() } -> ciel::boolean_testable; } {
		return lhs.base() > rhs.base();
	}

	template<class Iterator1, class Iterator2>
	constexpr bool operator<=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)requires requires{{ lhs.base() >= rhs.base() } -> ciel::boolean_testable; } {
		return lhs.base() >= rhs.base();
	}

	template<class Iterator1, class Iterator2>
	constexpr bool operator>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)requires requires{{ lhs.base() < rhs.base() } -> ciel::boolean_testable; } {
		return lhs.base() < rhs.base();
	}

	template<class Iterator1, class Iterator2>
	constexpr bool operator>=(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs)requires requires{{ lhs.base() <= rhs.base() } -> ciel::boolean_testable; } {
		return lhs.base() <= rhs.base();
	}

	template<class Iterator1, std::three_way_comparable_with<Iterator1> Iterator2>
	constexpr std::compare_three_way_result_t<Iterator1, Iterator2> operator<=>(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) {
		return rhs.base() <=> lhs.base();
	}

	template<class Iter>
	constexpr reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& it) {
		return reverse_iterator<Iter>(it.base() - n);
	}

	template<class Iterator1, class Iterator2>
	constexpr auto operator-(const reverse_iterator<Iterator1>& lhs, const reverse_iterator<Iterator2>& rhs) -> decltype(rhs.base() - lhs.base()) {
		return rhs.base() - lhs.base();
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_REVERSE_ITERATOR_H_