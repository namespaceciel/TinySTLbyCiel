#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_H_

#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_input_iterator InputIt, class T>
	constexpr InputIt find(InputIt first, InputIt last, const T& value) {
		for (; first != last; ++first) {
			if (*first == value) {
				return first;
			}
		}
		return last;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_H_