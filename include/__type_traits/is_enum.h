#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ENUM_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ENUM_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_enum : bool_constant<__is_enum(T)> {};

	template<class T>
	inline constexpr bool is_enum_v = is_enum<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ENUM_H_