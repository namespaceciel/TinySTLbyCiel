#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_EQUALITY_COMPARABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_EQUALITY_COMPARABLE_H_

#include <ciel/concepts_impl/boolean_testable.h>
#include <ciel/concepts_impl/common_reference_with.h>

namespace ciel {

	namespace equality_comparable_details {
		template<class T, class U>
		concept weakly_equality_comparable_with = requires(const ciel::remove_reference_t<T>& t, const ciel::remove_reference_t<U>& u) {
			{ t == u } -> ciel::boolean_testable;
			{ t != u } -> ciel::boolean_testable;
			{ u == t } -> ciel::boolean_testable;
			{ u != t } -> ciel::boolean_testable;
		};
	}

	template<class T>
	concept equality_comparable = equality_comparable_details::weakly_equality_comparable_with<T, T>;

	template<class T, class U>
	concept equality_comparable_with = ciel::equality_comparable<T>
									&& ciel::equality_comparable<U>
									&& ciel::common_reference_with<const ciel::remove_reference_t<T>&, const ciel::remove_reference_t<U>&>
									&& ciel::equality_comparable<ciel::common_reference_t<const ciel::remove_reference_t<T>&, const ciel::remove_reference_t<U>&>>
									&& equality_comparable_details::weakly_equality_comparable_with<T, U>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_EQUALITY_COMPARABLE_H_