#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_FLOATING_POINT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_FLOATING_POINT_H_

#include <ciel/type_traits.h>

namespace ciel {

	template<class T>
	concept floating_point = ciel::is_floating_point_v<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_FLOATING_POINT_H_