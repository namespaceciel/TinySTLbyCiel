#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_IDENTITY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_IDENTITY_H_

#include <ciel/utility.h>

namespace ciel {

	struct identity {
		template<class T>
		constexpr T&& operator()(T&& t) const noexcept {
			return ciel::forward<T>(t);
		}
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_IDENTITY_H_