#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_READABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_READABLE_H_

#include <ciel/iterator_impl/iter_alias.h>

namespace ciel {

	template<class In>
	concept indirectly_readable_helper = requires(const In in) {
		typename ciel::iter_value_t<In>;
		typename ciel::iter_reference_t<In>;
		typename ciel::iter_rvalue_reference_t<In>;
		{ *in } -> ciel::same_as<ciel::iter_reference_t<In>>;
		{ ranges::iter_move(in) } -> ciel::same_as<ciel::iter_rvalue_reference_t<In>>;
	}
		&& ciel::common_reference_with<ciel::iter_reference_t<In>&&, ciel::iter_value_t<In>&>
		&& ciel::common_reference_with<ciel::iter_reference_t<In>&&, ciel::iter_rvalue_reference_t<In>&&>
		&& ciel::common_reference_with<ciel::iter_rvalue_reference_t<In>&&, const ciel::iter_value_t<In>&>;

	template<class In>
	concept indirectly_readable = indirectly_readable_helper<ciel::remove_cvref_t<In>>;

	template<indirectly_readable T>
	using iter_common_reference_t = ciel::common_reference_t<ciel::iter_reference_t<T>, ciel::iter_value_t<T>&>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INDIRECTLY_READABLE_H_