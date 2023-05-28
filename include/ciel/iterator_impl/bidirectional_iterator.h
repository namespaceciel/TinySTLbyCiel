#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_BIDIRECTIONAL_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_BIDIRECTIONAL_ITERATOR_H_

#include <ciel/iterator_impl/forward_iterator.h>

namespace ciel {

	template<class I>
	concept bidirectional_iterator = ciel::forward_iterator<I>
		&& ciel::derived_from<ciel::iter_concept<I>, ciel::bidirectional_iterator_tag>
		&& requires(I i) {
			{ --i } -> ciel::same_as<I&>;
			{ i-- } -> ciel::same_as<I>;
		};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_BIDIRECTIONAL_ITERATOR_H_