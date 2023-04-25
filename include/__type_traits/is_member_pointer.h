#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_MEMBER_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_MEMBER_POINTER_H_

#include "integral_constant.h"
#include "remove_cv.h"

namespace ciel {

	template<class T>
	struct is_member_pointer_helper : false_type {};

	template<class T, class U>
	struct is_member_pointer_helper<T U::*> : true_type {};

	template<class T>
	struct is_member_pointer : is_member_pointer_helper<remove_cv_t<T>> {};

	template<class T>
	inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_MEMBER_POINTER_H_