#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_N_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_N_H_

#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_input_iterator InputIt, class Size, ciel::legacy_input_iterator OutputIt>
	constexpr OutputIt copy_n(InputIt first, Size count, OutputIt result) {
		if (count > 0) {
			*result++ = *first;
			for (Size i = 1; i < count; ++i) {
				*result++ = *++first;
			}
		}
		return result;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_COPY_N_H_