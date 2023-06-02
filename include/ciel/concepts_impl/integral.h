#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_INTEGRAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_INTEGRAL_H_

#include <ciel/type_traits.h>

namespace ciel {

	template<class T>
	concept integral = ciel::is_integral_v<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_INTEGRAL_H_