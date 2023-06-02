#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_BIDIRECTIONAL_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_BIDIRECTIONAL_ITERATOR_H_

#include <ciel/iterator_impl/legacy_forward_iterator.h>

namespace ciel {

	template<class I>
	concept legacy_bidirectional_iterator = ciel::legacy_forward_iterator<I>
										 && requires(I i) {
											    { --i } -> ciel::same_as<I&>;
											    { i-- } -> ciel::convertible_to<const I&>;
											    { *i-- } -> ciel::same_as<ciel::iter_reference_t<I>>;
										    };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_BIDIRECTIONAL_ITERATOR_H_