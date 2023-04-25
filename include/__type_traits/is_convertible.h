#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_CONVERTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_CONVERTIBLE_H_

#include "integral_constant.h"
#include "../utility.h"
#include "conjunction.h"

namespace ciel {

	namespace detail {

		template<class T>
		auto test_returnable(int) -> decltype(void(static_cast<T(*)()>(nullptr)), true_type{});

		template<class>
		auto test_returnable(...) -> false_type;

		template<class From, class To>
		auto test_implicitly_convertible(int) -> decltype(void(declval<void (&)(To)>()(declval<From>())), true_type{});

		template<class, class>
		auto test_implicitly_convertible(...) -> false_type;

	}    //namespace detail

	template<class From, class To>
	struct is_convertible : bool_constant<(decltype(detail::test_returnable<To>(0))::value &&
		decltype(detail::test_implicitly_convertible<From, To>(0))::value) ||
		(is_void<From>::value && is_void<To>::value)> {
	};

	template<class From, class To>
	struct is_nothrow_convertible : conjunction<is_void<From>, is_void<To>> {};

	template<class From, class To> requires requires {
		static_cast<To(*)()>(nullptr); {
		declval<void (&)(To) noexcept>()
			(
				declval<From>()
			)
		}
		noexcept;
	}
	struct is_nothrow_convertible<From, To> : true_type {};

	template<class From, class To>
	inline constexpr bool is_convertible_v = is_convertible<From, To>::value;

	template<class From, class To>
	inline constexpr bool is_nothrow_convertible_v = is_nothrow_convertible<From, To>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_IS_CONVERTIBLE_H_