#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MOVE_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MOVE_CONSTRUCTIBLE_H_

#include <ciel/type_traits_impl/is_constructible.h>
#include <ciel/type_traits_impl/add_rvalue_reference.h>

namespace ciel {

	template<class T>
	struct is_move_constructible : is_constructible<T, add_rvalue_reference_t<T>> {};

	template<class T>
	struct is_trivially_move_constructible : is_trivially_constructible<T, add_rvalue_reference_t<T>> {};

	template<class T>
	struct is_nothrow_move_constructible : is_nothrow_constructible<T, add_rvalue_reference_t<T>> {};

	template<class T>
	inline constexpr bool is_move_constructible_v = is_move_constructible<T>::value;

	template<class T>
	inline constexpr bool is_trivially_move_constructible_v = is_trivially_move_constructible<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_move_constructible_v = is_nothrow_move_constructible<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MOVE_CONSTRUCTIBLE_H_