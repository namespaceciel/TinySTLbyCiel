#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOID_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOID_H_

#include <ciel/type_traits_impl/is_same.h>
#include <ciel/type_traits_impl/remove_cv.h>

namespace ciel {

	template<class T>
	struct is_void : ciel::is_same<void, ciel::remove_cv_t<T>> {};

	template<class T>
	inline constexpr bool is_void_v = is_void<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_VOID_H_