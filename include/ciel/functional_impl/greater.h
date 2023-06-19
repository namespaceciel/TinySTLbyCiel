#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_GREATER_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_GREATER_H_

#include <ciel/utility_impl/forward.h>

namespace ciel {

	template<class T = void>
	struct greater {
		constexpr bool operator()(const T& lhs, const T& rhs) const {
			return lhs > rhs;
		}
	};

	template<>
	struct greater<void> {
		template<class T, class U>
		constexpr bool operator()(T&& lhs, U&& rhs) const requires requires { ciel::forward<T>(lhs) > ciel::forward<U>(rhs); } {
			return ciel::forward<T>(lhs) > ciel::forward<U>(rhs);
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_GREATER_H_