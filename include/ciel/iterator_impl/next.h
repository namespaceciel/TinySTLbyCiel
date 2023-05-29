#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_NEXT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_NEXT_H_

#include <ciel/iterator_impl/advance.h>

namespace ciel {

	template<class InputIt>
	constexpr InputIt next(InputIt it, typename ciel::iterator_traits<InputIt>::difference_type n = 1) {
		ciel::advance(it, n);
		return it;
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_NEXT_H_