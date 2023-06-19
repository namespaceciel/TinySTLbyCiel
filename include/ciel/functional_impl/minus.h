#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_MINUS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_MINUS_H_

#include <ciel/utility_impl/forward.h>

namespace ciel {

	template<class T = void>
	struct minus {
		constexpr T operator()(const T& lhs, const T& rhs) const {
			return lhs - rhs;
		}
	};

	template<>
	struct minus<void> {
		template<class T, class U>
		constexpr auto operator()(T&& lhs, U&& rhs) const -> decltype(ciel::forward<T>(lhs) - ciel::forward<U>(rhs)) {
			return ciel::forward<T>(lhs) - ciel::forward<U>(rhs);
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_MINUS_H_