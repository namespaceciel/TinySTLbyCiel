#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_INTEGRAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_INTEGRAL_H_

#include <ciel/type_traits_impl/integral_constant.h>
#include <ciel/type_traits_impl/remove_cv.h>

namespace ciel {

	namespace is_integral_details {

		template<class T>
		struct is_integral_helper : ciel::false_type {};

		template<>
		struct is_integral_helper<bool> : ciel::true_type {};

		template<>
		struct is_integral_helper<char> : ciel::true_type {};

		template<>
		struct is_integral_helper<signed char> : ciel::true_type {};

		template<>
		struct is_integral_helper<unsigned char> : ciel::true_type {};

		template<>
		struct is_integral_helper<wchar_t> : ciel::true_type {};

		template<>
		struct is_integral_helper<char8_t> : ciel::true_type {};

		template<>
		struct is_integral_helper<char16_t> : ciel::true_type {};

		template<>
		struct is_integral_helper<char32_t> : ciel::true_type {};

		template<>
		struct is_integral_helper<short> : ciel::true_type {};

		template<>
		struct is_integral_helper<unsigned short> : ciel::true_type {};

		template<>
		struct is_integral_helper<int> : ciel::true_type {};

		template<>
		struct is_integral_helper<unsigned int> : ciel::true_type {};

		template<>
		struct is_integral_helper<long> : ciel::true_type {};

		template<>
		struct is_integral_helper<unsigned long> : ciel::true_type {};

		template<>
		struct is_integral_helper<long long> : ciel::true_type {};

		template<>
		struct is_integral_helper<unsigned long long> : ciel::true_type {};
	}

	template<class T>
	struct is_integral : is_integral_details::is_integral_helper<ciel::remove_cv_t<T>> {};

	template<class T>
	inline constexpr bool is_integral_v = is_integral<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_INTEGRAL_H_