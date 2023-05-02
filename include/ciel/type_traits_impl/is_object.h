#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_OBJECT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_OBJECT_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_function.h>
#include <ciel/type_traits_impl/is_reference.h>
#include <ciel/type_traits_impl/is_void.h>

namespace ciel {

	//对象类型（即任何函数、引用或 void 类型外的可有 cv 限定的类型）
	template<class T>
	struct is_object : bool_constant<!is_function_v<T> && !is_reference_v<T> && !is_void_v<T>> {};

	template<class T>
	inline constexpr bool is_object_v = is_object<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_OBJECT_H_