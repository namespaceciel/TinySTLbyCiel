#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SIGNED_INTEGRAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SIGNED_INTEGRAL_H_

#include <ciel/concepts_impl/integral.h>

namespace ciel {

	template<class T>
	concept signed_integral = ciel::integral<T> && ciel::is_signed_v<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SIGNED_INTEGRAL_H_