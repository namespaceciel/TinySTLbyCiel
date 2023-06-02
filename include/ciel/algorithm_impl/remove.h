#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_REMOVE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_REMOVE_H_

#include <ciel/algorithm_impl/find.h>
#include <ciel/utility.h>

namespace ciel {

	template<class ForwardIt, class T>
	constexpr ForwardIt remove(ForwardIt first, ForwardIt last, const T& value) {
		first = ciel::find(first, last, value);
		if (first != last) {
			for (ForwardIt i = first; ++i != last;) {
				if (!(*i == value)) {
					*first++ = ciel::move(*i);
				}
			}
		}
		return first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_REMOVE_H_