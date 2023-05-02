#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COPY_ASSIGNABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COPY_ASSIGNABLE_H_

#include "is_assignable.h"
#include "add_lvalue_reference.h"

namespace ciel {

	template<class T>
	struct is_copy_assignable : is_assignable<add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>> {};

	template<class T>
	struct is_trivially_copy_assignable : is_trivially_assignable<add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>> {};

	template<class T>
	struct is_nothrow_copy_assignable : is_nothrow_assignable<add_lvalue_reference_t<T>, add_lvalue_reference_t<const T>> {};

	template<class T>
	inline constexpr bool is_copy_assignable_v = is_copy_assignable<T>::value;

	template<class T>
	inline constexpr bool is_trivially_copy_assignable_v = is_trivially_copy_assignable<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_copy_assignable_v = is_nothrow_copy_assignable<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COPY_ASSIGNABLE_H_