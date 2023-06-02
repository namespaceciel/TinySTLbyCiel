#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_ITER_SWAP_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_ITER_SWAP_H_

#include <ciel/utility_impl/swap.h>

namespace ciel {

	template<class ForwardIt1, class ForwardIt2>
	constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b) {
		using ciel::swap;
		swap(*a, *b);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_ITER_SWAP_H_