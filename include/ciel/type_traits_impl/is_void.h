#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOID_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOID_H_

#include "is_same.h"
#include "remove_cv.h"

namespace ciel {

	template<class T>
	struct is_void : is_same<void, remove_cv_t<T>> {};

	template<class T>
	inline constexpr bool is_void_v = is_void<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOID_H_