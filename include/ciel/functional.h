#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_H_

#include "utility.h"

namespace ciel {

	//greater
	template<class T=void>
	struct greater {
		constexpr bool operator()(const T& lhs, const T& rhs) const {
			return lhs > rhs;
		}
	};

	template<>
	struct greater<void> {
		template<class T, class U>
		constexpr auto operator()(T&& lhs, U&& rhs) const -> decltype(forward<T>(lhs) > forward<U>(rhs)) {
			return forward<T>(lhs) > forward<U>(rhs);
		}
	};

	//greater_equal
	template<class T=void>
	struct greater_equal {
		constexpr bool operator()(const T& lhs, const T& rhs) const {
			return lhs >= rhs;
		}
	};

	template<>
	struct greater_equal<void> {
		template<class T, class U>
		constexpr auto operator()(T&& lhs, U&& rhs) const -> decltype(forward<T>(lhs) >= forward<U>(rhs)) {
			return forward<T>(lhs) >= forward<U>(rhs);
		}
	};

	//less
	template<class T=void>
	struct less {
		constexpr bool operator()(const T& lhs, const T& rhs) const {
			return lhs < rhs;
		}
	};

	template<>
	struct less<void> {
		template<class T, class U>
		constexpr auto operator()(T&& lhs, U&& rhs) const -> decltype(forward<T>(lhs) < forward<U>(rhs)) {
			return forward<T>(lhs) < forward<U>(rhs);
		}
	};

	//less_equal
	template<class T=void>
	struct less_equal {
		constexpr bool operator()(const T& lhs, const T& rhs) const {
			return lhs <= rhs;
		}
	};

	template<>
	struct less_equal<void> {
		template<class T, class U>
		constexpr auto operator()(T&& lhs, U&& rhs) const -> decltype(forward<T>(lhs) <= forward<U>(rhs)) {
			return forward<T>(lhs) <= forward<U>(rhs);
		}
	};


}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_H_