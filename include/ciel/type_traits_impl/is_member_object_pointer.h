#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MEMBER_OBJECT_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MEMBER_OBJECT_POINTER_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_member_pointer.h>
#include <ciel/type_traits_impl/is_member_function_pointer.h>

namespace ciel {

	template<class T>
	struct is_member_object_pointer : bool_constant<is_member_pointer_v<T> && !is_member_function_pointer_v<T>> {};

	template<class T>
	inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MEMBER_OBJECT_POINTER_H_