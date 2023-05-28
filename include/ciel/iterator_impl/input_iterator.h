#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INPUT_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INPUT_ITERATOR_H_

#include <ciel/iterator_impl/input_or_output_iterator.h>
#include <ciel/iterator_impl/indirectly_readable.h>
#include <ciel/iterator_impl/iter_concept.h>

namespace ciel {

	template<class I>
	concept input_iterator = ciel::input_or_output_iterator<I>
		&& ciel::indirectly_readable<I>
		&& requires { typename ciel::iter_concept<I>; }
		&& ciel::derived_from<ciel::iter_concept<I>, ciel::input_iterator_tag>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INPUT_ITERATOR_H_