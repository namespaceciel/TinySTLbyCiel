#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_IOTA_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_IOTA_H_

#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_forward_iterator ForwardIt, class T>
	constexpr void iota(ForwardIt first, ForwardIt last, T value) {
		while (first != last) {
			*first++ = value;
			++value;
		}
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_IOTA_H_