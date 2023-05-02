#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MEMBER_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MEMBER_POINTER_H_

#include "integral_constant.h"
#include "remove_cv.h"

namespace ciel {

	namespace {
		template<class T>
		struct is_member_pointer_helper : false_type {};

		template<class T, class U>
		struct is_member_pointer_helper<T U::*> : true_type {};
	}

	template<class T>
	struct is_member_pointer : is_member_pointer_helper<remove_cv_t<T>> {};

	template<class T>
	inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MEMBER_POINTER_H_