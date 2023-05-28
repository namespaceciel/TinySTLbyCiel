#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_FORWARD_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_FORWARD_ITERATOR_H_

#include <ciel/iterator_impl/input_iterator.h>
#include <ciel/iterator_impl/iter_concept.h>
#include <ciel/iterator_impl/incrementable.h>
#include <ciel/iterator_impl/sentinel_for.h>

namespace ciel {

	template<class I>
	concept forward_iterator = ciel::input_iterator<I>
		&& ciel::derived_from<ciel::iter_concept<I>, ciel::forward_iterator_tag>
		&& ciel::incrementable<I>
		&& ciel::sentinel_for<I, I>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_FORWARD_ITERATOR_H_