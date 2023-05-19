#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SWAP_RANGES_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SWAP_RANGES_H_

#include <ciel/algorithm_impl/iter_swap.h>

namespace ciel {

	template<class ForwardIt1, class ForwardIt2>
	constexpr ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2) {
		while (first1 != last1) {
			ciel::iter_swap(first1++, first2++);
		}
		return first2;
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SWAP_RANGES_H_