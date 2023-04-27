#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_SCALAR_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_SCALAR_H_

#include "integral_constant.h"
#include "is_arithmetic.h"
#include "is_enum.h"
#include "is_pointer.h"
#include "is_member_pointer.h"
#include "is_null_pointer.h"

namespace ciel {

	template<class T>
	struct is_scalar : bool_constant<is_arithmetic_v<T> || is_enum_v<T> || is_pointer_v<T> || is_member_pointer_v<T> || is_null_pointer_v<T>> {};

	template<class T>
	inline constexpr bool is_scalar_v = is_scalar<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_SCALAR_H_