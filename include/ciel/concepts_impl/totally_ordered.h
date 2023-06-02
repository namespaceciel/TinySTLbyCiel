#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_TOTALLY_ORDERED_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_TOTALLY_ORDERED_H_

#include <ciel/concepts_impl/equality_comparable.h>

namespace ciel {

	namespace totally_ordered_details {
		template<class T, class U>
		concept partially_ordered_with = requires(const ciel::remove_reference_t<T>& t, const ciel::remove_reference_t<U>& u) {
			{ t < u } -> ciel::boolean_testable;
			{ t > u } -> ciel::boolean_testable;
			{ t <= u } -> ciel::boolean_testable;
			{ t >= u } -> ciel::boolean_testable;
			{ u < t } -> ciel::boolean_testable;
			{ u > t } -> ciel::boolean_testable;
			{ u <= t } -> ciel::boolean_testable;
			{ u >= t } -> ciel::boolean_testable;
		};
	}

	template<class T>
	concept totally_ordered = ciel::equality_comparable<T> && totally_ordered_details::partially_ordered_with<T, T>;

	template<class T, class U>
	concept totally_ordered_with = ciel::totally_ordered<T>
								&& ciel::totally_ordered<U>
								&& ciel::equality_comparable_with<T, U>
								&& ciel::totally_ordered<ciel::common_reference_t<const ciel::remove_reference_t<T>&, const ciel::remove_reference_t<U>&>>
								&& totally_ordered_details::partially_ordered_with<T, U>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_TOTALLY_ORDERED_H_