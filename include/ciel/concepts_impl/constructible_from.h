#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CONSTRUCTIBLE_FROM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CONSTRUCTIBLE_FROM_H_

#include <ciel/concepts_impl/destructible.h>

namespace ciel {

	template<class T, class... Args>
	concept constructible_from = ciel::destructible<T> && ciel::is_constructible_v<T, Args...>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CONSTRUCTIBLE_FROM_H_