#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/../utility_impl/declval.h>
// #include <ciel/type_traits_impl/conjunction.h>
#include <ciel/type_traits_impl/is_void.h>

namespace ciel {
/*
	namespace is_convertible_details {
		// 逗号表达式: 挨个求值并返回最后一个值，以此实现 SFINAE

		// 将 nullptr 强转为返回值为 To 的函数指针，以测试 returnable
		// 再强转为 void 以避免潜在的对逗号的运算符重载，下同
		template<class To>
		auto test_returnable(int) -> decltype(void(static_cast<To(*)()>(nullptr)), ciel::true_type{});

		template<class>
		ciel::false_type test_returnable(...);

		// 生成一个 From 的右值引用看其能否被接受 To 形参的函数接受（隐式转换）
		// 目前来看 static_cast<void(*)(To)>(nullptr) 与 declval<void(&)(To)>() 貌似效果一样。。
		template<class From, class To>
		auto test_implicitly_convertible(int) -> decltype(void(ciel::declval<void(&)(To)>()(ciel::declval<From>())), ciel::true_type{});

		template<class, class>
		ciel::false_type test_implicitly_convertible(...);
	}
*/

	namespace is_convertible_details {

		template<class To, class = decltype(static_cast<To(*)()>(nullptr))>
		ciel::true_type test_returnable(int);

		template<class>
		ciel::false_type test_returnable(...);

		template<class From, class To, class = decltype(ciel::declval<void(&)(To)>()(ciel::declval<From>()))>
		ciel::true_type test_implicitly_convertible(int);

		template<class, class>
		ciel::false_type test_implicitly_convertible(...);
	}

	template<class From, class To>
	struct is_convertible : ciel::bool_constant<
		((decltype(is_convertible_details::test_returnable<To>(0))::value && decltype(is_convertible_details::test_implicitly_convertible<From, To>(0))::value))
		|| (ciel::is_void_v<From> && ciel::is_void_v<To>)> {};

	template<class From, class To>
	inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

/*
	template<class From, class To>
	struct is_nothrow_convertible : ciel::conjunction<ciel::is_void<From>, ciel::is_void<To>> {};
*/

	template<class From, class To>
	struct is_nothrow_convertible : ciel::bool_constant<ciel::is_void_v<From> && ciel::is_void_v<To>> {};

	namespace is_convertible_details {
		template<class From, class To>
		concept test_no_throw_convertible = requires {
			static_cast<To(*)()>(nullptr);
			{ ciel::declval<void (&)(To) noexcept>()(ciel::declval<From>()) } noexcept;
		};
	}

	template<class From, class To>
		requires is_convertible_details::test_no_throw_convertible<From, To>
	struct is_nothrow_convertible<From, To> : ciel::true_type {};

	template<class From, class To>
	inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_