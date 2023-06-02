#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MOVE_BACKWARD_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MOVE_BACKWARD_H_

#include <ciel/utility_impl/move.h>

namespace ciel {
	// 若 d_last 在 (first, last] 内则行为未定义。该情况下必须用 move 代替 move_backward
	template<class BidirIt1, class BidirIt2>
	constexpr BidirIt2 move_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
		while (first != last) {
			*(--d_last) = ciel::move(*(--last));
		}
		return d_last;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MOVE_BACKWARD_H_