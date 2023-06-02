#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SWAPPABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SWAPPABLE_H_

#include <ciel/concepts_impl/ranges::swap.h>

namespace ciel {

	template<class T>
	concept swappable = requires(T& a, T& b) { ciel::ranges::swap(a, b); };

	template<class T, class U>
	concept swappable_with = ciel::common_reference_with<T, U>
							&& requires(T&& t, U&& u) {
								ciel::ranges::swap(ciel::forward<T>(t), ciel::forward<T>(t));
								ciel::ranges::swap(ciel::forward<U>(u), ciel::forward<U>(u));
								ciel::ranges::swap(ciel::forward<T>(t), ciel::forward<U>(u));
								ciel::ranges::swap(ciel::forward<U>(u), ciel::forward<T>(t));
							};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SWAPPABLE_H_