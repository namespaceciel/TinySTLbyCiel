#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_SENTINEL_FOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_SENTINEL_FOR_H_

#include <ciel/iterator_impl/input_or_output_iterator.h>

namespace ciel {

	template<class S, class I>
	concept sentinel_for = ciel::semiregular<S>
		&& ciel::input_or_output_iterator<I>
		&& ciel::equality_comparable_details::weakly_equality_comparable_with<S, I>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_SENTINEL_FOR_H_