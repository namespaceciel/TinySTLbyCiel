#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_OUTPUT_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_OUTPUT_ITERATOR_H_

#include <ciel/iterator_impl/indirectly_writable.h>
#include <ciel/iterator_impl/input_or_output_iterator.h>

namespace ciel {

	template<class I, class T>
	concept output_iterator = ciel::input_or_output_iterator<I>
		&& ciel::indirectly_writable<I, T>
		&& requires(I i, T&& t) { *i++ = ciel::forward<T>(t); };

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_OUTPUT_ITERATOR_H_