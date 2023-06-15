#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_BACKWARD_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_BACKWARD_H_

#include <ciel/iterator.h>

namespace ciel {
	// 若 d_last 在 (first, last] 中则行为未定义。该情况下必须用 copy 取代 copy_backward
	template<ciel::legacy_bidirectional_iterator BidirIt1, ciel::legacy_bidirectional_iterator BidirIt2>
	constexpr BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last) {
		while (first != last) {
			*(--d_last) = *(--last);
		}
		return d_last;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_BACKWARD_H_