#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_SIZED_SENTINEL_FOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_SIZED_SENTINEL_FOR_H_

#include <ciel/iterator_impl/sentinel_for.h>

namespace ciel {

	template<class, class>
	inline constexpr bool disable_sized_sentinel_for = false;

	template<class S, class I>
	concept sized_sentinel_for = ciel::sentinel_for<S, I>
		&& !disable_sized_sentinel_for<ciel::remove_cv_t<S>, ciel::remove_cv_t<I>>
		&& requires(const I& i, const S& s) {
			{ s - i } -> ciel::same_as<ciel::iter_difference_t<I>>;
			{ i - s } -> ciel::same_as<ciel::iter_difference_t<I>>;
		};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_SIZED_SENTINEL_FOR_H_