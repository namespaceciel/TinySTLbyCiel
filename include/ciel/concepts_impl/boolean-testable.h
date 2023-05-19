#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_BOOLEAN_TESTABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_BOOLEAN_TESTABLE_H_

#include <ciel/concepts_impl/convertible_to.h>

namespace ciel {

	template<class B>
	concept __boolean_testable_impl =                // 仅用于阐释
	ciel::convertible_to<B, bool>;

	template<class B>
	concept boolean-testable =                       // 仅用于阐释
	__boolean_testable_impl<B> &&
		requires (B&& b) {
			{ !ciel::forward<B>(b) } -> __boolean_testable_impl;
		};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_BOOLEAN_TESTABLE_H_