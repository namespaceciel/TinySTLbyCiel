#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MOVE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MOVE_H_

#include <ciel/iterator.h>
#include <ciel/utility_impl/move.h>

namespace ciel {
	// 若 d_first 在范围 [first, last) 中则行为未定义。此情况下可用 move_backward 代替 move
	template<ciel::legacy_input_iterator InputIt, ciel::legacy_input_iterator OutputIt>
	constexpr OutputIt move(InputIt first, InputIt last, OutputIt d_first) {
		while (first != last) {
			*d_first++ = ciel::move(*first++);
		}
		return d_first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_MOVE_H_