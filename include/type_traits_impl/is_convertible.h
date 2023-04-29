#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_

#include "integral_constant.h"
#include "../utility_impl/declval.h"

namespace ciel {

	namespace {
		//逗号表达式: 挨个求值并返回最后一个值，以此实现SFINAE

		//将nullptr强转为返回值为To的函数指针，以测试returnable
		//再强转为void以避免潜在的对逗号的运算符重载，下同
		template<class To>
		auto test_returnable(int) -> decltype(void(static_cast<To(*)()>(nullptr)), true_type{});

		template<class>
		false_type test_returnable(...);

		//生成一个From的右值引用看其能否被接受To形参的函数接受（隐式转换）
		//目前来看static_cast<void(*)(To)>(nullptr)与declval<void(&)(To)>()貌似效果一样。。
		template<class From, class To>
		auto test_implicitly_convertible(int) -> decltype(void(declval<void(&)(To)>()(declval<From>())), true_type{});

		template<class, class>
		false_type test_implicitly_convertible(...);
	}

	template<class From, class To>
	struct is_convertible : bool_constant<
		((decltype(test_returnable<To>(0))::value && decltype(test_implicitly_convertible<From, To>(0))::value))
		|| (is_void_v<From> && is_void_v<To>)> {};

	template<class From, class To>
	inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_