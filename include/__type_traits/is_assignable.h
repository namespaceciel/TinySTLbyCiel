#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ASSIGNABLE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ASSIGNABLE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T, class U>
	struct is_assignable : bool_constant<__is_assignable(T, U)> {};

	template<class T, class U>
	struct is_trivially_assignable : bool_constant<__is_trivially_assignable(T, U)> {};

	template<class T, class U>
	struct is_nothrow_assignable : bool_constant<__is_nothrow_assignable(T, U)> {};

	template<class T, class U>
	inline constexpr bool is_assignable_v = is_assignable<T, U>::value;

	template<class T, class U>
	inline constexpr bool is_trivially_assignable_v = is_trivially_assignable<T, U>::value;

	template<class T, class U>
	inline constexpr bool is_nothrow_assignable_v = is_nothrow_assignable<T, U>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_ASSIGNABLE_H_