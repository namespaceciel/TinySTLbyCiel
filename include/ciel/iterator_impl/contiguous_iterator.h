#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_CONTIGUOUS_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_CONTIGUOUS_ITERATOR_H_

#include <ciel/iterator_impl/random_access_iterator.h>

namespace ciel {

	template<class I>
	concept contiguous_iterator = std::random_access_iterator<I>
		&& std::derived_from<ciel::iter_concept<I>, std::contiguous_iterator_tag>
		&& std::is_lvalue_reference_v<std::iter_reference_t<I>>
		&& std::same_as<std::iter_value_t<I>, std::remove_cvref_t<std::iter_reference_t<I>>>
		&& requires(const I& i) {{ std::to_address(i) } -> std::same_as<std::add_pointer_t<std::iter_reference_t<I>>>; };

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_CONTIGUOUS_ITERATOR_H_