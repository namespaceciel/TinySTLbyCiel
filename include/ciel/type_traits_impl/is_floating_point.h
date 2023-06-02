#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FLOATING_POINT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FLOATING_POINT_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_same.h>
#include <ciel/type_traits_impl/remove_cv.h>

namespace ciel {

	template<class T>
	struct is_floating_point : ciel::bool_constant<ciel::is_same_v<float, ciel::remove_cv_t<T>>
												|| ciel::is_same_v<double, ciel::remove_cv_t<T>>
										 		|| ciel::is_same_v<long double, ciel::remove_cv_t<T>>> {};

	template<class T>
	inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_FLOATING_POINT_H_