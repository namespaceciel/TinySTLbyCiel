#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_FORWARD_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_FORWARD_ITERATOR_H_

#include <ciel/iterator_impl/legacy_input_iterator.h>

namespace ciel {

	template<class I>
	concept legacy_forward_iterator = ciel::legacy_input_iterator<I>
							  	   && ciel::constructible_from<I>
								   && ciel::is_lvalue_reference_v<ciel::iter_reference_t<I>>
								   && ciel::same_as<ciel::remove_cvref_t<ciel::iter_reference_t<I>>, typename ciel::indirectly_readable_traits<I>::value_type>
								   && requires(I i) {
									      { i++ } -> ciel::convertible_to<const I&>;
									      { *i++ } -> ciel::same_as<ciel::iter_reference_t<I>>;
								      };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_FORWARD_ITERATOR_H_