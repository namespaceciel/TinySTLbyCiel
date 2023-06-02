#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_PREV_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_PREV_H_

#include <ciel/iterator_impl/advance.h>

namespace ciel {

	template<class BidirIt>
	constexpr BidirIt prev(BidirIt it, typename ciel::iterator_traits<BidirIt>::difference_type n = 1) {
		ciel::advance(it, -n);
		return it;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_PREV_H_