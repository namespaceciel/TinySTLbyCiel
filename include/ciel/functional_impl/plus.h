#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_PLUS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_PLUS_H_

#include <ciel/utility_impl/forward.h>

namespace ciel {

	template<class T = void>
	struct plus {
		constexpr T operator()(const T& lhs, const T& rhs) const {
			return lhs + rhs;
		}
	};

	template<>
	struct plus<void> {
		template<class T, class U>
		constexpr auto operator()(T&& lhs, U&& rhs) const -> decltype(ciel::forward<T>(lhs) + ciel::forward<U>(rhs)) {
			return ciel::forward<T>(lhs) + ciel::forward<U>(rhs);
		}

        using is_transparent = void;
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_PLUS_H_