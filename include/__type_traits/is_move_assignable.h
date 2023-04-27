#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_MOVE_ASSIGNABLE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_MOVE_ASSIGNABLE_H_

#include "is_assignable.h"
#include "add_lvalue_reference.h"
#include "add_rvalue_reference.h"

namespace ciel {
	template<class T>
	struct is_move_assignable : is_assignable<add_lvalue_reference_t<T>, add_rvalue_reference_t<T>> {};

	template<class T>
	struct is_trivially_move_assignable : is_trivially_assignable<add_lvalue_reference_t<T>, add_rvalue_reference_t<T>> {};

	template<class T>
	struct is_nothrow_move_assignable : is_nothrow_assignable<add_lvalue_reference_t<T>, add_rvalue_reference_t<T>> {};

	template<class T>
	inline constexpr bool is_move_assignable_v = is_move_assignable<T>::value;

	template<class T>
	inline constexpr bool is_trivially_move_assignable_v = is_trivially_move_assignable<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_move_assignable_v = is_nothrow_move_assignable<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_MOVE_ASSIGNABLE_H_