#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_INPUT_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_INPUT_ITERATOR_H_

#include <ciel/iterator_impl/legacy_iterator.h>
#include <ciel/iterator_impl/incrementable_traits.h>
#include <ciel/iterator_impl/indirectly_readable_traits.h>

namespace ciel {

	template<class I>
	concept legacy_input_iterator = ciel::legacy_iterator<I>
								 && ciel::equality_comparable<I>
								 && requires(I i) {
									 typename ciel::incrementable_traits<I>::difference_type;
									 typename ciel::indirectly_readable_traits<I>::value_type;
									 typename ciel::common_reference_t<ciel::iter_reference_t<I>&&, typename ciel::indirectly_readable_traits<I>::value_type&>;
									 *i++;
									 typename ciel::common_reference_t<decltype(*i++)&&, typename ciel::indirectly_readable_traits<I>::value_type&>;
									 requires ciel::signed_integral<typename ciel::incrementable_traits<I>::difference_type>;
								 };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_LEGACY_INPUT_ITERATOR_H_