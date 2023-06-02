#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_WRAP_ITER_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_WRAP_ITER_H_

#include <ciel/iterator_impl/iterator_traits.h>
#include <ciel/memory.h>

namespace ciel {

	// TODO: 添加边界检查

	template<class Iter>
	class wrap_iter {
	public:
		using iterator_type = Iter;
		using value_type = ciel::iterator_traits<iterator_type>::value_type;
		using difference_type = ciel::iterator_traits<iterator_type>::difference_type;
		using pointer = ciel::iterator_traits<iterator_type>::pointer;
		using reference = ciel::iterator_traits<iterator_type>::reference;
		using iterator_category = ciel::iterator_traits<iterator_type>::iterator_category;
		using iterator_concept = ciel::contiguous_iterator_tag;

	private:
		iterator_type it;

	public:
		constexpr explicit wrap_iter(iterator_type other) noexcept: it(other) {}

		constexpr wrap_iter() noexcept: it() {}

		template<class U>
		requires ciel::is_convertible_v<U, iterator_type>
		constexpr wrap_iter(const wrap_iter<U>& other) noexcept : it(other.base()) {}

		constexpr reference operator*() const noexcept {
			return *it;
		}

		constexpr pointer operator->() const noexcept {
			return ciel::to_address(it);
		}

		constexpr wrap_iter& operator++() noexcept {
			++it;
			return *this;
		}

		constexpr wrap_iter operator++(int) noexcept {
			wrap_iter res(*this);
			++it;
			return res;
		}

		constexpr wrap_iter& operator--() noexcept {
			--it;
			return *this;
		}

		constexpr wrap_iter operator--(int) noexcept {
			wrap_iter res(*this);
			--it;
			return res;
		}

		constexpr wrap_iter operator+(difference_type n) const noexcept {
			wrap_iter res(*this);
			res += n;
			return res;
		}

		constexpr wrap_iter& operator+=(difference_type n) noexcept {
			it += n;
			return *this;
		}

		constexpr wrap_iter operator-(difference_type n) const noexcept {
			return *this + (-n);
		}

		constexpr wrap_iter& operator-=(difference_type n) noexcept {
			*this += (-n);
			return *this;
		}

		constexpr reference operator[](difference_type n) const noexcept {
			return it[n];
		}

		constexpr iterator_type base() const noexcept {
			return it;
		}

	};    // class wrap_iter

	template<class Iter1, class Iter2>
	constexpr bool operator==(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool operator!=(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
		return !(lhs == rhs);
	}

	template<class Iter1, class Iter2>
	constexpr bool operator<(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
		return lhs.base() < rhs.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool operator>(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
		return lhs.base() > rhs.base();
	}

	template<class Iter1, class Iter2>
	constexpr bool operator<=(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
		return !(lhs > rhs);
	}

	template<class Iter1, class Iter2>
	constexpr bool operator>=(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) {
		return !(lhs < rhs);
	}

	template<class Iter1, class Iter2>
	constexpr auto operator-(const wrap_iter<Iter1>& lhs, const wrap_iter<Iter2>& rhs) -> decltype(lhs.base() - rhs.base()) {
		return lhs.base() - rhs.base();
	}

	template<class Iter>
	constexpr wrap_iter<Iter> operator+(typename wrap_iter<Iter>::difference_type n, wrap_iter<Iter> wi) noexcept {
		wi += n;
		return wi;
	}

	template<class Iter>
	struct pointer_traits<wrap_iter<Iter>> {
		using pointer = wrap_iter<Iter>;
		using element_type = pointer_traits<Iter>::element_type;
		using difference_type = pointer_traits<Iter>::difference_type;

		constexpr static element_type* to_address(pointer p) noexcept {
			return ciel::to_address(p.base());
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_WRAP_ITER_H_