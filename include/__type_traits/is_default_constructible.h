#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H_

#include "is_constructible.h"

namespace ciel {

	template<class T>
	struct is_default_constructible : is_constructible<T> {};

	template<class T>
	struct is_trivially_default_constructible : is_trivially_constructible<T> {};

	template<class T>
	struct is_nothrow_default_constructible : is_nothrow_constructible<T> {};

	template<class T>
	inline constexpr bool is_default_constructible_v = is_default_constructible<T>::value;

	template<class T>
	inline constexpr bool is_trivially_default_constructible_v = is_trivially_default_constructible<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_default_constructible_v = is_nothrow_default_constructible<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_DEFAULT_CONSTRUCTIBLE_H_