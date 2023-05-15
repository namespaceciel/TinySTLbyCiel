#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNDAMENTAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNDAMENTAL_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_arithmetic.h>
#include <ciel/type_traits_impl/is_void.h>
#include <ciel/type_traits_impl/is_null_pointer.h>

namespace ciel {

	template<class T>
	struct is_fundamental : ciel::bool_constant<ciel::is_arithmetic_v<T> || ciel::is_void_v<T> || ciel::is_null_pointer_v<T>> {};

	template<class T>
	inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNDAMENTAL_H_