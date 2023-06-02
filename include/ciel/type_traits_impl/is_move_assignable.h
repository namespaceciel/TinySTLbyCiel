#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MOVE_ASSIGNABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MOVE_ASSIGNABLE_H_

#include <ciel/type_traits_impl/is_assignable.h>
#include <ciel/type_traits_impl/add_lvalue_reference.h>
#include <ciel/type_traits_impl/add_rvalue_reference.h>

namespace ciel {

	template<class T>
	struct is_move_assignable : ciel::is_assignable<ciel::add_lvalue_reference_t<T>, ciel::add_rvalue_reference_t<T>> {};

	template<class T>
	struct is_trivially_move_assignable : ciel::is_trivially_assignable<ciel::add_lvalue_reference_t<T>, ciel::add_rvalue_reference_t<T>> {};

	template<class T>
	struct is_nothrow_move_assignable : ciel::is_nothrow_assignable<ciel::add_lvalue_reference_t<T>, ciel::add_rvalue_reference_t<T>> {};

	template<class T>
	inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

	template<class T>
	inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_MOVE_ASSIGNABLE_H_