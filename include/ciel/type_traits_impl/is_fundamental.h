#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNDAMENTAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNDAMENTAL_H_

#include "integral_constant.h"
#include "is_arithmetic.h"
#include "is_void.h"
#include "is_null_pointer.h"

namespace ciel {

	template<class T>
	struct is_fundamental : bool_constant<is_arithmetic_v<T> || is_void_v<T> || is_null_pointer_v<T>> {};

	template<class T>
	inline constexpr bool is_fundamental_v = is_fundamental<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FUNDAMENTAL_H_