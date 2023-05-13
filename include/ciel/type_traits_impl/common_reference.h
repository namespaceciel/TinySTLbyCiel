#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_REFERENCE_H_

#include <ciel/type_traits_impl/common_type.h>
#include <ciel/type_traits_impl/is_convertible.h>
#include <ciel/type_traits_impl/remove_cvref.h>
#include <ciel/type_traits_impl/copy_cvref.h>
#include <ciel/utility_impl/declval.h>

namespace ciel {

	//此 traits 存在的意义：
	//		https://stackoverflow.com/questions/59011331/what-is-the-purpose-of-c20-stdcommon-reference

	template<class, class, template<class> class, template<class> class>
	struct basic_common_reference {};

	namespace common_reference_details {

		template<class... T>
		struct common_reference_helper {};

		template<>
		struct common_reference_helper<> {};

		template<class T>
		struct common_reference_helper<T> {
			using type = T;
		};

		//模板形参为 2 时的处理细节

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
二个引用类型 T1 和 T2 的简单共用引用类型定义如下：

若 T1 为 cv1 X & 而 T2 为 cv2 Y & （即都是左值引用）：
 			则其简单共用引用类型为 decltype(false? declval<cv12 X &>() : declval<cv12 Y &>()) ，其中 cv12 为 cv1 与 cv2 的 cv 限定符的并集，若该类型存在且为引用类型；

若 T1 与 T2 均为右值引用类型：
			若 T1 & 和 T2 & 的简单共用引用类型（按照前一条确定）存在，则令 C 代表该类型的对应右值引用类型。
			若 is_convertible_v<T1, C> 与 is_convertible_v<T2, C> 均为 true ，则 T1 与 T2 的简单共用引用类型为 C 。

否则，二个类型之一必须为左值引用类型 A & 而另一个必须为右值引用类型 B && （ A 与 B 可为 cv 限定）。
 			令 D 代表 A & 与 B const & 的简单共用引用类型，若它存在。
 			若 D 存在且 is_convertible_v<B &&, D> 为 true ，则简单共用引用类型为 D 。

否则，无简单共用引用类型。
*/
		template<class T1, class T2>
		using simple_common_reference1 = decltype(true ? declval<copy_cv_t<T1, T2>>() : declval<copy_cv_t<T2, T1>>());

		template<class T1, class T2, class X = remove_reference_t<T1>, class Y = remove_reference_t<T2>>
		struct have_simple_common_reference_type {};

		template<class T1, class T2, class X, class Y>
			requires requires { typename simple_common_reference1<T1, T2>; } && is_reference_v<simple_common_reference1<T1, T2>>
		struct have_simple_common_reference_type<T1&, T2&, X, Y> {
			using type = simple_common_reference1<T1, T2>;
		};

		template<class T1, class T2, class X, class Y>
			requires requires { typename simple_common_reference1<T1&, T2&>; } && is_convertible_v<T1, simple_common_reference1<T1&, T2&>> && is_convertible_v<T2, simple_common_reference1<T1&, T2&>>
		struct have_simple_common_reference_type<T1&&, T2&&, X, Y> {
			using type = simple_common_reference1<T1&, T2&>;
		};

		template<class T1, class T2, class X, class Y>
			requires requires { typename simple_common_reference1<T1, const Y&>; } && is_convertible_v<T2, simple_common_reference1<T1, const Y&>>
		struct have_simple_common_reference_type<T1&, T2&&, X, Y> {
			using type = simple_common_reference1<T1, const Y&>;
		};

		template<class T1, class T2, class X, class Y>
		struct have_simple_common_reference_type<T1&&, T2&, X, Y> : have_simple_common_reference_type<T2&, T1&&> {};

		template<class T1, class T2>
		using have_simple_common_reference_type_t = typename have_simple_common_reference_type<T1, T2>::type;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//替补关系，如果本身存在 type，那么继承到的 type 就会被覆盖。以此对应以下四种存在情况
		template <class T1, class T2> struct common_reference_sub_bullet3;
		template <class T1, class T2> struct common_reference_sub_bullet2 : common_reference_sub_bullet3<T1, T2> {};
		template <class T1, class T2> struct common_reference_sub_bullet1 : common_reference_sub_bullet2<T1, T2> {};

		//一、若 T1 和 T2 都是引用类型，而 T1 和 T2 的简单共用引用类型 S 存在，则成员类型 type 指名 S：
		template<class T1, class T2>
			requires is_reference_v<T1> && is_reference_v<T2> && requires { typename have_simple_common_reference_type_t<T1, T2>; }
		struct common_reference_sub_bullet1<T1, T2> {
			using type = have_simple_common_reference_type_t<T1, T2>;
		};

		//二、否则，若 basic_common_reference<remove_cvref_t<T1>, remove_cvref_t<T2>, T1Q, T2Q>::type 存在，
		//			其中 TiQ 是一元别名模板，满足 TiQ<U> 为 U 带上 Ti 的 cv 及引用限定符，则成员类型 type 指名该类型：
		template <class T>
		struct TiQ {
			template<class U>
			using apply = copy_cvref_t<T, U>;
		};

		template <class T1, class T2>
		using basic_common_reference_t = typename basic_common_reference<remove_cvref_t<T1>, remove_cvref_t<T2>, TiQ<T1>::template apply, TiQ<T2>::template apply>::type;

		template<class T1, class T2>
			requires requires { typename basic_common_reference_t<T1, T2>; }
		struct common_reference_sub_bullet2<T1, T2> {
			using type = basic_common_reference_t<T1, T2>;
		};

		//三、否则，若 decltype(false? val<T1>() : val<T2>()) 是合法类型，其中 val 为函数模板 template<class T> T val(); 则成员类型 type 指名该类型
		template<class T>
		T val();

		template<class T1, class T2>
		using simple_common_reference3 = decltype(true ? val<T1>() : val<T2>());

		template<class T1, class T2>
			requires requires { typename simple_common_reference3<T1, T2>; }
		struct common_reference_sub_bullet3<T1, T2> {
			using type = simple_common_reference3<T1, T2>;
		};

		//四、否则，若 common_type_t<T1, T2> 为合法类型，则成员类型 type 代表该类型
		template<class T1, class T2>
		struct common_reference_sub_bullet3 : common_type<T1, T2> {};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		template<class T1, class T2>
		struct common_reference_helper<T1, T2> : common_reference_sub_bullet1<T1, T2> {};

		template <class T1, class T2, class... Rest>
			requires requires { typename common_reference_helper<T1, T2>::type; }
		struct common_reference_helper<T1, T2, Rest...> : common_reference_helper<typename common_reference_helper<T1, T2>::type, Rest...> {};

	}	//namespace common_reference_details

	template<class... T>
	struct common_reference {
		using type = typename common_reference_details::common_reference_helper<T...>::type;
	};

	template< class... T >
	using common_reference_t = typename common_reference<T...>::type;

}   //namespace ciel

/*
	//一点测试，C 继承自 B，B 继承自 A
 	using namespace ciel;
	static_assert(is_same_v<common_reference_t<A>, A>);
	static_assert(is_same_v<common_reference_t<A&>, A&>);
	static_assert(is_same_v<common_reference_t<A&&>, A&&>);
	static_assert(is_same_v<common_reference_t<A, B, C>, A>);
	static_assert(is_same_v<common_reference_t<B, C>, B>);
	static_assert(is_same_v<common_reference_t<const A&, volatile C>, A>);
	static_assert(is_same_v<common_reference_t<const volatile A, B>, const volatile A>);
	static_assert(is_same_v<common_reference_t<const volatile A&, B>, A>);
	static_assert(is_same_v<common_reference_t<const volatile A, B&&>, const volatile A>);
	static_assert(is_same_v<common_reference_t<int&, double&>, double>);
	static_assert(is_same_v<common_reference_t<int, double>, double>);
	static_assert(is_same_v<common_reference_t<int&, long&>, long>);
*/

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_REFERENCE_H_