#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_DESTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_DESTRUCTIBLE_H_

#include <ciel/type_traits_impl/conditional.h>
#include <ciel/type_traits_impl/is_reference.h>
#include <ciel/type_traits_impl/is_void.h>
#include <ciel/type_traits_impl/is_function.h>
#include <ciel/type_traits_impl/is_unbounded_array.h>
#include <ciel/type_traits_impl/is_object.h>
#include <ciel/type_traits_impl/remove_all_extents.h>
#include <ciel/type_traits_impl/is_class.h>
#include <ciel/type_traits_impl/is_enum.h>
#include <ciel/type_traits_impl/is_union.h>
#include <ciel/utility_impl/declval.h>

namespace ciel {

	//若 T 是引用类型，则提供等于 true 的成员常量 value。
	//若 T 是（可以有 cv 限定的）void、函数类型或未知边界数组，则 value 等于 false。
	//若 T 是对象类型，则对于作为 remove_all_extents<T>::type 的类型 U，若表达式 declval<U&>().~U() 在不求值语境合法，则 value 等于 true 。否则，value 等于 false。
	namespace {
        //https://stackoverflow.com/questions/54699526/why-is-is-destructible-defined-using-declvalu-u-and-not-declvalu
		template<class T>
		concept has_dtor = requires { declval<T&>().~T(); };

		template<class T>
		concept has_noexcept_dtor = requires {{ declval<T&>().~T() } noexcept; };
	}

	template<class T>
	struct is_destructible
		: conditional_t<is_reference_v<T>,
						true_type,
						conditional_t<is_void_v<T> || is_function_v<T> || is_unbounded_array_v<T>,
									  false_type,
									  conditional_t<!is_object_v<T>,
									  				false_type,
													conditional_t<has_dtor<remove_all_extents_t<T>>,
																  true_type,
																  false_type>>>> {};

	//同上，并且 remove_all_extents<T>::type 要么是非类类型，要么是拥有平凡析构函数的类类型。
	template<class T>
	struct is_trivially_destructible
		: conditional_t<is_reference_v<T>,
						true_type,
						conditional_t<is_void_v<T> || is_function_v<T> || is_unbounded_array_v<T>,
									  false_type,
									  conditional_t<!is_object_v<T>,
									  				false_type,
													conditional_t<(!is_class_v<remove_all_extents_t<T>> && !is_union_v<remove_all_extents_t<T>> && !is_enum_v<remove_all_extents_t<T>>) || __has_trivial_destructor(remove_all_extents_t<T>),
																  true_type,
																  false_type>>>> {};

	template<class T>
	struct is_nothrow_destructible
		: conditional_t<is_reference_v<T>,
						true_type,
						conditional_t<is_void_v<T> || is_function_v<T> || is_unbounded_array_v<T>,
									  false_type,
									  conditional_t<!is_object_v<T>,
													false_type,
													conditional_t<has_noexcept_dtor<remove_all_extents_t<T>>,
																  true_type,
																  false_type>>>> {};

	template<class T>
	inline constexpr bool is_destructible_v = is_destructible<T>::value;

	template<class T>
	inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_DESTRUCTIBLE_H_