#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_TYPE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_TYPE_H_

#include <ciel/type_traits_impl/is_same.h>
#include <ciel/type_traits_impl/decay.h>
#include <ciel/utility_impl/declval.h>

namespace ciel {

	template<class... T>
	struct common_type;

	namespace common_type_details {

		template<class T1, class T2>
		using common_type_alias = decltype(true ? ciel::declval<T1>() : ciel::declval<T2>());

		template<class...>
		struct common_type_helper {};

		template<>
		struct common_type_helper<> {};

		// 若 sizeof...(T) 为 1，则成员 type 指名与 common_type<T, T>::type 相同的类型
		template<class T>
		struct common_type_helper<T> : common_type<T, T> {};

		// 替补关系，如果本身存在 type，那么继承到的 type 就会被覆盖。以此对应以下四种存在情况
		template<class T1, class T2> struct common_type_sub_bullet4 {};
		template<class T1, class T2> struct common_type_sub_bullet3 : common_type_sub_bullet4<T1, T2> {};
		template<class T1, class T2> struct common_type_sub_bullet2 : common_type_sub_bullet3<T1, T2> {};
		template<class T1, class T2> struct common_type_sub_bullet1 : common_type_sub_bullet2<T1, T2> {};

		// 一、若应用 decay 到 T1 与 T2 中至少一个产生相异类型，则成员 type 指名与 common_type_t<decay_t<T1>, decay_t<T2>> 相同的类型，若它存在
		template<class T1, class T2>
			requires (!ciel::is_same_v<T1, ciel::decay_t<T1>> || !ciel::is_same_v<T2, ciel::decay_t<T2>>) && requires { typename common_type<ciel::decay_t<T1>, ciel::decay_t<T2>>::type; }
		struct common_type_sub_bullet1<T1, T2> {
			using type = typename common_type<ciel::decay_t<T1>, ciel::decay_t<T2>>::type;
		};

		// 二、否则，若有对 common_type<T1, T2> 的用户定义特化，则使用该特化
		template<class T1, class T2>
			requires requires { typename common_type<T1, T2>::type; }
		struct common_type_sub_bullet2<T1, T2> {
			using type = typename common_type<T1, T2>::type;
		};

		// 三、否则，若 decay_t<decltype(true ? declval<T1>() : declval<T2>())> 是合法类型，则成员 type 代表该类型
		template<class T1, class T2>
			requires requires { typename ciel::decay_t<common_type_alias<T1, T2>>; }
		struct common_type_sub_bullet3<T1, T2> {
			using type = ciel::decay_t<common_type_alias<T1, T2>>;
		};

		// 四、否则，若 decay_t<decltype(true ? declval<CR1>() : declval<CR2>())> 为合法类型，其中 CR1 与 CR2 分别为 const remove_reference_t<T1>& 与 const remove_reference_t<T2>&，则成员 type 代表该类型
		template<class T1, class T2>
			requires requires { typename ciel::decay_t<common_type_alias<const ciel::remove_reference_t<T1>&, const ciel::remove_reference_t<T2>&>>; }
		struct common_type_sub_bullet4<T1, T2> {
			using type = ciel::decay_t<common_type_alias<const ciel::remove_reference_t<T1>&, const ciel::remove_reference_t<T2>&>>;
		};

		template<class T1, class T2>
		struct common_type_helper<T1, T2> : common_type_sub_bullet1<T1 ,T2> {};

		template<class T1, class T2, class... Rest>
		struct common_type_helper<T1, T2, Rest...> : common_type_helper<typename common_type_helper<T1, T2>::type, Rest...> {};

	}	// namespace common_type_details

	template<class... T>
	struct common_type : common_type_details::common_type_helper<T...> {};

	template<class... T>
	using common_type_t = typename common_type<T...>::type;
/*
关于为什么第 26 行如果改为继承 common_type_helper<T, T> 会编译错误（明明 common_type<T, T> 也只是紧接着要继承 common_type_helper<T, T> ？）：
对应问题代码：
 	ciel::common_type_t<int>

	在能编译的这个版本中，根据继承关系，在编译期模板实例化的顺序分别是：
	common_type<int>
	common_type_helper<int>
	common_type<int, int>
	common_type_helper<int, int>
	common_type_sub_bullet1<int, int>
	common_type_sub_bullet2<int, int> // 在此（43 行）用约束检查了 common_type<T1, T2> 是否定义了 type 成员
	common_type_sub_bullet3<int, int>
	common_type_sub_bullet4<int, int>

	所以在 43 行 common_type<int, int> 已经存在且无 type 成员，放行

	而当 26 行改为直接继承 common_type_helper<T, T> 时，继承关系里就少了个 common_type<int, int>，导致其未实例化

	这时候在 43 行需要检查 common_type<int, int> 的时候，由于其还未实例化，需要实例化它，而它紧接着又要继承 common_type_helper<int, int>，
 	而其这时候是一个 incomplete type（因为它本身实例化了但是基类在那时还没实例化完），报错
*/
}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_TYPE_H_