#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_TYPE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_TYPE_H_

#include <ciel/type_traits_impl/decay.h>
#include <ciel/utility_impl/declval.h>

namespace ciel {

	template<class...>
	struct common_type{};

	template<class T>
	struct common_type<T> {
		using type = typename common_type<T, T>::type;
	};

	template<class T1, class T2>
	struct common_type<T1, T2> {
		//这里是通过三元运算符拿到 T1 和 T2 的 common_type（如果存在的话）
		using type = decay_t<decltype(true ? declval<T1>() : declval<T2>())>;
	};

	template<class T1, class T2, class... Rest>
	struct common_type<T1, T2, Rest...> {
		using type = typename common_type<typename common_type<T1, T2>::type, Rest...>::type;
	};

	template<class... T>
	using common_type_t = typename common_type<T...>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_COMMON_TYPE_H_