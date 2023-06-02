#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COMPOUND_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COMPOUND_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/is_fundamental.h>

namespace ciel {

	// 复合类型是从基础类型构造的类型。任何 C++ 类型为基础或复合之一。
	template<class T>
	struct is_compound : ciel::bool_constant<!ciel::is_fundamental_v<T>> {};

	template<class T>
	inline constexpr bool is_compound_v = is_compound<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_COMPOUND_H_