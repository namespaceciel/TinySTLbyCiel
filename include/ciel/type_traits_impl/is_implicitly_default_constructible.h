#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H_

// Not C++ standard

#include <ciel/type_traits_impl/is_default_constructible.h>

namespace ciel {

	namespace is_implicitly_default_constructible_details {
		template<class T>
		void is_implicitly_default_constructible_helper(T);
	}

	template<class T, class = void, class = typename ciel::is_default_constructible<T>::type>
	struct is_implicitly_default_constructible : ciel::false_type {};

	template<class T>
	struct is_implicitly_default_constructible<T, decltype(is_implicitly_default_constructible_details::is_implicitly_default_constructible_helper<const T&>({})), ciel::true_type> : ciel::true_type {};

	template<class T>
	struct is_implicitly_default_constructible<T, decltype(is_implicitly_default_constructible_details::is_implicitly_default_constructible_helper<const T&>({})), ciel::false_type> : ciel::false_type {};

	template<class T>
	inline constexpr bool is_implicitly_default_constructible_v = is_implicitly_default_constructible<T>::value;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_IMPLICITLY_DEFAULT_CONSTRUCTIBLE_H_