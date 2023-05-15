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
	namespace is_destructible_details {
		//关于这里为什么用的是 T&：
        //		https://stackoverflow.com/questions/54699526/why-is-is-destructible-defined-using-declvalu-u-and-not-declvalu
		template<class T>
		concept has_dtor = requires { ciel::declval<T&>().~T(); };

		template<class T>
		concept has_noexcept_dtor = requires {{ ciel::declval<T&>().~T() } noexcept; };
	}

	template<class T>
	struct is_destructible
		: ciel::conditional_t<ciel::is_reference_v<T>,
							  ciel::true_type,
							  ciel::conditional_t<ciel::is_void_v<T> || ciel::is_function_v<T> || ciel::is_unbounded_array_v<T>,
												  ciel::false_type,
												  ciel::conditional_t<is_destructible_details::has_dtor<ciel::remove_all_extents_t<T>>,
																	  ciel::true_type,
																	  ciel::false_type>>> {};

	template<class T>
	struct is_trivially_destructible : ciel::bool_constant<__is_trivially_destructible(ciel::remove_all_extents_t<T>)> {};

	template<class T>
	struct is_trivially_destructible<T[]> : ciel::false_type {};

	template<class T>
	struct is_nothrow_destructible
		: ciel::conditional_t<ciel::is_reference_v<T>,
							  ciel::true_type,
							  ciel::conditional_t<ciel::is_void_v<T> || ciel::is_function_v<T> || ciel::is_unbounded_array_v<T>,
												  ciel::false_type,
												  ciel::conditional_t<is_destructible_details::has_noexcept_dtor<ciel::remove_all_extents_t<T>>,
																	  ciel::true_type,
																	  ciel::false_type>>> {};

	template<class T>
	inline constexpr bool is_destructible_v = is_destructible<T>::value;

	template<class T>
	inline constexpr bool is_trivially_destructible_v = is_trivially_destructible<T>::value;

	template<class T>
	inline constexpr bool is_nothrow_destructible_v = is_nothrow_destructible<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_DESTRUCTIBLE_H_