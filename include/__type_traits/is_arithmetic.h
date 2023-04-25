#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ARITHMETIC_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ARITHMETIC_H_

#include "integral_constant.h"
#include "is_integral.h"
#include "is_floating_point.h"

namespace ciel {

	template<class T>
	struct is_arithmetic : bool_constant<is_integral_v<T> || is_floating_point_v<T> > {};

	template<class T>
	inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ARITHMETIC_H_