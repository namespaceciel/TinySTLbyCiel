#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_

#include "integral_constant.h"
#include "../utility.h"
#include "conjunction.h"

namespace ciel {

		template<class To>
		auto test_returnable(int) -> decltype(void(static_cast<To(*)()>(nullptr)), true_type{});

		template<class>
		false_type test_returnable(...)  ;

		template<class From, class To>
		auto test_implicitly_convertible(int) -> decltype(void(declval<void (&)(To)>()(declval<From>())), true_type{});

		template<class, class>
		false_type test_implicitly_convertible(...) ;


	template<class From, class To>
	struct is_convertible : bool_constant<(decltype(test_returnable<To>(0))::value &&
		decltype(test_implicitly_convertible<From, To>(0))::value) ||
		(is_void<From>::value && is_void<To>::value)> {
	};





	template<class From, class To>
	inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

//	template<class From, class To>
//	inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_CONVERTIBLE_H_