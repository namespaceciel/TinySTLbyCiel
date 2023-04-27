#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_DESTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_DESTRUCTIBLE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_destructible : bool_constant<__is_destructible(T)> {};

	template<class T>
	struct is_trivially_destructible : bool_constant<__is_trivially_destructible(T)> {};

	template<class T>
	struct is_nothrow_destructible : bool_constant<__is_nothrow_destructible(T)> {};

	template<class T>
	inline constexpr bool is_destructible_v = is_destructible<T>::value;

	template<class T>
	inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_DESTRUCTIBLE_H_