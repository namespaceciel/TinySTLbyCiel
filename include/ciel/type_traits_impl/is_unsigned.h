#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_UNSIGNED_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_UNSIGNED_H_

#include <ciel/type_traits_impl/is_arithmetic.h>

namespace ciel {

	namespace is_unsigned_details {

		template<class T, bool = ciel::is_arithmetic<T>::value>
		struct is_unsigned_helper : ciel::bool_constant<T(0) < T(-1)> {};	//T(-1) > T(0)的写法会与之前的 < 匹配

		template<class T>
		struct is_unsigned_helper<T, false> : ciel::false_type {};
	}

	template<class T>
	struct is_unsigned : is_unsigned_details::is_unsigned_helper<T>::type {};

	template<class T>
	inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_UNSIGNED_H_