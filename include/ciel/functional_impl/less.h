#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_LESS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_LESS_H_

#include <ciel/utility_impl/forward.h>

namespace ciel {

	template<class T = void>
	struct less {
		constexpr bool operator()(const T& lhs, const T& rhs) const {
			return lhs < rhs;
		}
	};

	template<>
	struct less<void> {
		template<class T, class U>
		constexpr bool operator()(T&& lhs, U&& rhs) const requires requires { ciel::forward<T>(lhs) < ciel::forward<U>(rhs); } {
			return ciel::forward<T>(lhs) < ciel::forward<U>(rhs);
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_LESS_H_