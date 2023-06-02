#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_RANDOM_ACCESS_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_RANDOM_ACCESS_ITERATOR_H_

#include <ciel/iterator_impl/legacy_bidirectional_iterator.h>

namespace ciel {

	template<class I>
	concept legacy_random_access_iterator = ciel::legacy_bidirectional_iterator<I>
										 && ciel::totally_ordered<I>
										 && requires(I i, typename ciel::incrementable_traits<I>::difference_type n) {
											    { i += n } -> ciel::same_as<I&>;
											    { i -= n } -> ciel::same_as<I&>;
											    { i + n } -> ciel::same_as<I>;
											    { n + i } -> ciel::same_as<I>;
											    { i - n } -> ciel::same_as<I>;
											    { i - i } -> ciel::same_as<decltype(n)>;
											    { i[n] } -> ciel::convertible_to<ciel::iter_reference_t<I>>;
										    };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_RANDOM_ACCESS_ITERATOR_H_