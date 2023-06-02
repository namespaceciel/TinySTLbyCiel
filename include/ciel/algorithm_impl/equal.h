#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_EQUAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_EQUAL_H_

namespace ciel {

	template<class InputIt1, class InputIt2>
	constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}

	template<class InputIt1, class InputIt2, class BinaryPredicate>
	constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p) {
		for (; first1 != last1; ++first1, ++first2) {
			if (!p(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}

	template<class InputIt1, class InputIt2>
	constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
		if (distance(first1, last1) != distance(first2, last2)) {
			return false;
		}
		for (; first1 != last1, first2 != last2; ++first1, ++first2) {
			if (!(*first1 == *first2)) {
				return false;
			}
		}
		return true;
	}

	template<class InputIt1, class InputIt2, class BinaryPredicate>
	constexpr bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate p) {
		if (distance(first1, last1) != distance(first2, last2)) {
			return false;
		}
		for (; first1 != last1, first2 != last2; ++first1, ++first2) {
			if (!p(*first1, *first2)) {
				return false;
			}
		}
		return true;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_EQUAL_H_