#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_GCD_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_GCD_H_

#include <ciel/concepts.h>

namespace ciel {

	template<ciel::integral M, ciel::integral N>
		requires (!ciel::is_same_v<ciel::remove_cv_t<M>, bool> && !ciel::is_same_v<ciel::remove_cv_t<N>, bool>)
	constexpr ciel::common_type_t<M, N> gcd(M m, N n) {
		return n == 0 ? m : gcd(n, m % n);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_GCD_H_