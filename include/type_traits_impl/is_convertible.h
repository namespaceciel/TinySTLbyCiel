#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_

#include "integral_constant.h"
#include "../utility_impl/declval.h"
//#include "conjunction.h"
#include "is_void.h"

namespace ciel {
/*
	namespace {
		//逗号表达式: 挨个求值并返回最后一个值，以此实现 SFINAE

		//将 nullptr 强转为返回值为 To 的函数指针，以测试 returnable
		//再强转为 void 以避免潜在的对逗号的运算符重载，下同
		template<class To>
		auto test_returnable(int) -> decltype(void(static_cast<To(*)()>(nullptr)), true_type{});

		template<class>
		false_type test_returnable(...);

		//生成一个 From 的右值引用看其能否被接受 To 形参的函数接受（隐式转换）
		//目前来看 static_cast<void(*)(To)>(nullptr) 与 declval<void(&)(To)>() 貌似效果一样。。
		template<class From, class To>
		auto test_implicitly_convertible(int) -> decltype(void(declval<void(&)(To)>()(declval<From>())), true_type{});

		template<class, class>
		false_type test_implicitly_convertible(...);
	}
*/

	namespace {
		template<class To, class = decltype(static_cast<To(*)()>(nullptr))>
		true_type test_returnable(int);

		template<class>
		false_type test_returnable(...);

		template<class From, class To, class = decltype(declval<void(&)(To)>()(declval<From>()))>
		true_type test_implicitly_convertible(int);

		template<class, class>
		false_type test_implicitly_convertible(...);
	}

	template<class From, class To>
	struct is_convertible : bool_constant<
		((decltype(test_returnable<To>(0))::value && decltype(test_implicitly_convertible<From, To>(0))::value))
		|| (is_void_v<From> && is_void_v<To>)> {};

	template<class From, class To>
	inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

/*
	template<class From, class To>
	struct is_nothrow_convertible : conjunction<is_void<From>, is_void<To>> {};
*/

	template<class From, class To>
	struct is_nothrow_convertible : bool_constant<is_void_v<From> && is_void_v<To>> {};

	namespace {
		template<class From, class To>
		concept test_no_throw_convertible = requires {
			static_cast<To(*)()>(nullptr);
			{ declval<void (&)(To) noexcept>()(declval<From>()) } noexcept;
		};
	}

	//这是偏特化
	template<class From, class To>
		requires test_no_throw_convertible<From, To>
	struct is_nothrow_convertible<From, To> : true_type {};

	template<class From, class To>
	inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_