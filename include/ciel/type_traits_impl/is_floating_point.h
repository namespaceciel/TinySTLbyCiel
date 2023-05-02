#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FLOATING_POINT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FLOATING_POINT_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_same.h>
#include <ciel/type_traits_impl/remove_cv.h>

namespace ciel {

	template<class T>
	struct is_floating_point : bool_constant<is_same_v<float, remove_cv_t<T>>
										 || is_same_v<double, remove_cv_t<T>>
										 || is_same_v<long double, remove_cv_t<T>>> {};

	template<class T>
	inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FLOATING_POINT_H_