#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INPUT_OR_OUTPUT_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INPUT_OR_OUTPUT_ITERATOR_H_

#include <ciel/iterator_impl/weakly_incrementable.h>

namespace ciel {

	template<class I>
	concept input_or_output_iterator = requires(I i) {
		{ *i } -> referenceable;
	}
		&& ciel::weakly_incrementable<I>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INPUT_OR_OUTPUT_ITERATOR_H_