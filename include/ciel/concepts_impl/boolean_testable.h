#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_BOOLEAN_TESTABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_BOOLEAN_TESTABLE_H_

#include <ciel/concepts_impl/convertible_to.h>
#include <ciel/utility.h>

namespace ciel {

	// B 与 !B 都可以转化为 bool 类型
	template<class B>
	concept boolean_testable_helper = ciel::convertible_to<B, bool>;

	template<class B>
	concept boolean_testable = boolean_testable_helper<B> && requires(B&& b) {{ !ciel::forward<B>(b) } -> boolean_testable_helper; };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_BOOLEAN_TESTABLE_H_