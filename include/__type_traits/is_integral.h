#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_INTEGRAL_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_INTEGRAL_H_

#include "integral_constant.h"

namespace ciel {

	template<class T>
	struct is_integral : bool_constant<requires(T t, T* p, void (* f)(T))
	{
		reinterpret_cast<T>(t); // 排除class类型
		f(0); // 排除枚举类型
		p + t; // 排除除整型以外所有尚未排除的类型
	}> {
	};

	template<class T>
	inline constexpr bool is_integral_v = is_integral<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_INTEGRAL_H_