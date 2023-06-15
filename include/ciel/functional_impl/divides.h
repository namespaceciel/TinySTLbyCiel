#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_DIVIDES_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_DIVIDES_H_

#include <ciel/utility.h>

namespace ciel {

	template<class T = void>
	struct divides {
		constexpr T operator()(const T& lhs, const T& rhs) const {
			return lhs / rhs;
		}
	};

	template<>
	struct divides<void> {
		template<class T, class U>
		constexpr auto operator()(T&& lhs, U&& rhs) const -> decltype(ciel::forward<T>(lhs) / ciel::forward<U>(rhs)) {
			return ciel::forward<T>(lhs) / ciel::forward<U>(rhs);
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_DIVIDES_H_