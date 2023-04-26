#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_H_

#include "is_constructible.h"
#include "add_lvalue_reference.h"
#include "add_cv.h"

namespace ciel {

	template<class T>
	struct is_copy_constructible : is_constructible<T, add_lvalue_reference_t<add_const_t<T>>> {};

	template<class T>
	struct is_trivially_copy_constructible : is_trivially_constructible<T, add_lvalue_reference_t<add_const_t<T>>> {};

	template<class T>
	struct is_nothrow_copy_constructible : is_nothrow_constructible<T, add_lvalue_reference_t<add_const_t<T>>> {};

	template<class T>
	inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

	template<class T>
	inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_H_