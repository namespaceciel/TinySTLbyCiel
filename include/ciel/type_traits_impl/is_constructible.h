#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONSTRUCTIBLE_H_

#include "integral_constant.h"

namespace ciel {

	template<class T, class ...Args>
	struct is_constructible : bool_constant<__is_constructible(T, Args...)> {};

	template<class T, class ...Args>
	struct is_trivially_constructible : bool_constant<__is_trivially_constructible(T, Args...)> {};

	template<class T, class ...Args>
	struct is_nothrow_constructible : bool_constant<__is_nothrow_constructible(T, Args...)> {};

	template<class T, class ...Args>
	inline constexpr bool is_constructible_v = is_constructible<T, Args...>::value;

	template<class T, class ...Args>
	inline constexpr bool is_trivially_constructible_v = is_trivially_constructible<T, Args...>::value;

	template<class T, class ...Args>
	inline constexpr bool is_nothrow_constructible_v = is_nothrow_constructible<T, Args...>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONSTRUCTIBLE_H_