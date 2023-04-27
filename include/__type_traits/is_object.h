#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_OBJECT_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_OBJECT_H_

#include "integral_constant.h"
#include "is_function.h"
#include "is_reference.h"
#include "is_void.h"

//对象类型（即任何函数、引用或 void 类型外的可有 cv 限定的类型）

namespace ciel {

	template<class T>
	struct is_object : bool_constant<!is_function_v<T> && !is_reference_v<T> && !is_void_v<T>> {};

	template<class T>
	inline constexpr bool is_object_v = is_object<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_OBJECT_H_