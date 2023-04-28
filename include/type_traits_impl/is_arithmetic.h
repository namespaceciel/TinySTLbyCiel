#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_ARITHMETIC_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_ARITHMETIC_H_

#include "integral_constant.h"
#include "is_integral.h"
#include "is_floating_point.h"

namespace ciel {

	template<class T>
	struct is_arithmetic : bool_constant<is_integral_v<T> || is_floating_point_v<T> > {};

	template<class T>
	inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_ARITHMETIC_H_