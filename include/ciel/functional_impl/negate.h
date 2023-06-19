#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_NEGATE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_NEGATE_H_

#include <ciel/utility_impl/forward.h>

namespace ciel {

	template<class T = void>
	struct negate {
		constexpr T operator()(const T& arg) const {
			return -arg;
		}
	};

	template<>
	struct negate<void> {
		template<class T>
		constexpr auto operator()(T&& arg) const -> decltype(-ciel::forward<T>(arg)) {
			return -ciel::forward<T>(arg);
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_NEGATE_H_