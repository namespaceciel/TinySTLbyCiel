#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_IF_NOT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_IF_NOT_H_

#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_input_iterator InputIt, class UnaryPredicate>
	constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q) {
		for (; first != last; ++first) {
			if (!q(*first)) {
				return first;
			}
		}
		return last;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_FIND_IF_NOT_H_