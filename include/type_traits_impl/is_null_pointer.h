#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_NULL_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_NULL_POINTER_H_

#include <cstddef>
#include "is_same.h"
#include "remove_cv.h"

namespace ciel {

	template<class T>
	struct is_null_pointer : is_same<std::nullptr_t, remove_cv_t<T>> {};

	template<class T>
	inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_NULL_POINTER_H_