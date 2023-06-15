#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_GENERATE_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_GENERATE_N_H_

#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_input_iterator OutputIt, class Size, class Generator>
	constexpr OutputIt generate_n(OutputIt first, Size count, Generator g) {
		for (Size i = 0; i < count; ++i) {
			*first++ = g();
		}
		return first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_GENERATE_N_H_