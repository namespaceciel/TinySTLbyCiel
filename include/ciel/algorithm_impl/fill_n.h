#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FILL_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FILL_N_H_

#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_input_iterator OutputIt, class Size, class T>
	constexpr OutputIt fill_n(OutputIt first, Size count, const T& value) {
		for (Size i = 0; i < count; ++i) {
			*first++ = value;
		}
		return first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FILL_N_H_