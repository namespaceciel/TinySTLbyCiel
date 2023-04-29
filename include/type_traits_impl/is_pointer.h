#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_POINTER_H_

#include "integral_constant.h"
#include "remove_cv.h"

namespace ciel {

	namespace {
		template<class T>
		struct is_pointer_helper : false_type {};

		template<class T>
		struct is_pointer_helper<T*> : true_type {};
	}

	template<class T>
	struct is_pointer : is_pointer_helper<remove_cv_t<T>> {};

	template<class T>
	inline constexpr bool is_pointer_v = is_pointer<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_POINTER_H_