#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COPY_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COPY_CONSTRUCTIBLE_H_

#include <ciel/type_traits_impl/is_constructible.h>
#include <ciel/type_traits_impl/add_lvalue_reference.h>
#include <ciel/type_traits_impl/add_cv.h>

namespace ciel {

	template<class T>
	struct is_copy_constructible : ciel::is_constructible<T, ciel::add_lvalue_reference_t<ciel::add_const_t<T>>> {};

	template<class T>
	struct is_trivially_copy_constructible : ciel::is_trivially_constructible<T, ciel::add_lvalue_reference_t<ciel::add_const_t<T>>> {};

	template<class T>
	struct is_nothrow_copy_constructible : ciel::is_nothrow_constructible<T, ciel::add_lvalue_reference_t<ciel::add_const_t<T>>> {};

	template<class T>
	inline constexpr bool is_copy_constructible_v = is_copy_constructible<T>::value;

	template<class T>
	inline constexpr bool is_trivially_copy_constructible_v = is_trivially_copy_constructible<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_copy_constructible_v = is_nothrow_copy_constructible<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COPY_CONSTRUCTIBLE_H_