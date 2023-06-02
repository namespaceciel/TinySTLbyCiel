#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_REMOVE_IF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_REMOVE_IF_H_

#include <ciel/algorithm_impl/find_if.h>
#include <ciel/utility.h>

namespace ciel {

	template<class ForwardIt, class UnaryPredicate>
	ForwardIt remove_if(ForwardIt first, ForwardIt last, UnaryPredicate p) {
		first = ciel::find_if(first, last, p);
		if (first != last) {
			for (ForwardIt i = first; ++i != last;) {
				if (!p(*i)) {
					*first++ = ciel::move(*i);
				}
			}
		}
		return first;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_REMOVE_IF_H_