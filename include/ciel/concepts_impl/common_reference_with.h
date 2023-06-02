#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COMMON_REFERENCE_WITH_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COMMON_REFERENCE_WITH_H_

#include <ciel/concepts_impl/same_as.h>
#include <ciel/concepts_impl/convertible_to.h>

namespace ciel {

	template<class T, class U>
	concept common_reference_with = ciel::same_as<ciel::common_reference_t<T, U>, ciel::common_reference_t<U, T>>
									&& ciel::convertible_to<T, ciel::common_reference_t<T, U>>
									&& ciel::convertible_to<U, ciel::common_reference_t<T, U>>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COMMON_REFERENCE_WITH_H_