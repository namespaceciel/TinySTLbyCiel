#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SWAPPABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SWAPPABLE_H_

#include <ciel/utility_impl/declval.h>
#include <ciel/utility_impl/move.h>
#include <ciel/type_traits_impl/is_move_constructible.h>
#include <ciel/type_traits_impl/is_move_assignable.h>

namespace ciel {

	template<class T>
	constexpr void swap(T& a, T& b) noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_move_assignable_v<T>) {
		T temp = move(a);
		a = move(b);
		b = move(temp);
	}

	namespace {
		template<class T, class U>
		concept can_swap = requires {
			swap(declval<T&>(), declval<U&>());
			swap(declval<U&>(), declval<T&>());
		};

		template<class T, class U>
		concept can_nothrow_swap = requires {
			{ swap(declval<T&>(), declval<U&>()) } noexcept;
			{ swap(declval<U&>(), declval<T&>()) } noexcept;
		};

		template<class T, class U>
		struct is_swappable_with_helper : false_type {};

		template<class T, class U> requires can_swap<T, U>
		struct is_swappable_with_helper<T, U> : true_type {};

		template<class T, class U>
		struct is_nothrow_swappable_with_helper : false_type {};

		template<class T, class U> requires can_nothrow_swap<T, U>
		struct is_nothrow_swappable_with_helper<T, U> : true_type {};
	}

	template<class T, class U>
	struct is_swappable_with : is_swappable_with_helper<T,U> {};

	template<class T, class U>
	inline constexpr bool is_swappable_with_v = is_swappable_with<T, U>::value;

	template<class T>
	struct is_swappable : bool_constant<is_swappable_with_v<T, T> && !is_void_v<T> && !is_function_v<T>> {};

	template<class T>
	inline constexpr bool is_swappable_v = is_swappable<T>::value;

	template<class T, class U>
	struct is_nothrow_swappable_with : is_nothrow_swappable_with_helper<T, U> {};

	template<class T, class U>
	inline constexpr bool is_nothrow_swappable_with_v = is_nothrow_swappable_with<T, U>::value;

	template<class T>
	struct is_nothrow_swappable : bool_constant<is_nothrow_swappable_with_v<T, T> && !is_void_v<T> && !is_function_v<T>> {};

	template<class T>
	inline constexpr bool is_nothrow_swappable_v = is_nothrow_swappable<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SWAPPABLE_H_