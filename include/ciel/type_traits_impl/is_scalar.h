#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SCALAR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SCALAR_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_arithmetic.h>
#include <ciel/type_traits_impl/is_enum.h>
#include <ciel/type_traits_impl/is_pointer.h>
#include <ciel/type_traits_impl/is_member_pointer.h>
#include <ciel/type_traits_impl/is_null_pointer.h>

namespace ciel {

	template<class T>
	struct is_scalar : ciel::bool_constant<ciel::is_arithmetic_v<T> || ciel::is_enum_v<T> || ciel::is_pointer_v<T> || ciel::is_member_pointer_v<T> || ciel::is_null_pointer_v<T>> {};

	template<class T>
	inline constexpr bool is_scalar_v = is_scalar<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SCALAR_H_