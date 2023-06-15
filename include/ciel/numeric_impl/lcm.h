#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_LCM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_LCM_H_

#include <ciel/numeric_impl/gcd.h>

namespace ciel {

	template<class M, class N>
	constexpr ciel::common_type_t<M, N> lcm(M m, N n) {
		return m / ciel::gcd(m, n) * n;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_NUMERIC_IMPL_LCM_H_