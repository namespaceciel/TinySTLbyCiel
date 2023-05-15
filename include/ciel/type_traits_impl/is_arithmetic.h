#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ARITHMETIC_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ARITHMETIC_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_integral.h>
#include <ciel/type_traits_impl/is_floating_point.h>

namespace ciel {

	template<class T>
	struct is_arithmetic : ciel::bool_constant<ciel::is_integral_v<T> || ciel::is_floating_point_v<T> > {};

	template<class T>
	inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_ARITHMETIC_H_