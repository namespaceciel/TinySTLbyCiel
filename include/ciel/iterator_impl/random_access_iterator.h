#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_RANDOM_ACCESS_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_RANDOM_ACCESS_ITERATOR_H_

#include <ciel/iterator_impl/bidirectional_iterator.h>
#include <ciel/iterator_impl/sized_sentinel_for.h>

namespace ciel {

	template<class I>
	concept random_access_iterator = ciel::bidirectional_iterator<I>
		&& ciel::derived_from<ciel::iter_concept<I>, ciel::random_access_iterator_tag>
		&& ciel::totally_ordered<I>
		&& ciel::sized_sentinel_for<I, I>
		&& requires(I i, const I j, const ciel::iter_difference_t<I> n) {
			{ i += n } -> ciel::same_as<I&>;
			{ j + n } -> ciel::same_as<I>;
			{ n + j } -> ciel::same_as<I>;
			{ i -= n } -> ciel::same_as<I&>;
			{ j - n } -> ciel::same_as<I>;
			{ j[n] } -> ciel::same_as<ciel::iter_reference_t<I>>;
		};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_RANDOM_ACCESS_ITERATOR_H_