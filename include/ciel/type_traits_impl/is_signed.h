#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SIGNED_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SIGNED_H_

#include <ciel/type_traits_impl/is_arithmetic.h>

namespace ciel {

	namespace is_signed_details {

		template<class T, bool = ciel::is_arithmetic<T>::value>
		struct is_signed_helper : ciel::bool_constant<T(-1) < T(0)> {};

		template<class T>
		struct is_signed_helper<T, false> : ciel::false_type {};
	}

	template<class T>
	struct is_signed : is_signed_details::is_signed_helper<T>::type {};

	template<class T>
	inline constexpr bool is_signed_v = is_signed<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_SIGNED_H_