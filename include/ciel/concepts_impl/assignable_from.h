#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_ASSIGNABLE_FROM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_ASSIGNABLE_FROM_H_

#include <ciel/concepts_impl/common_reference_with.h>
#include <ciel/utility.h>

namespace ciel {

	template<class LHS, class RHS>
	concept assignable_from = ciel::is_lvalue_reference_v<LHS>
	    				 	  && ciel::common_reference_with<const ciel::remove_reference_t<LHS>&, const ciel::remove_reference_t<RHS>&>
	                          && requires (LHS lhs, RHS&& rhs) { { lhs = ciel::forward<RHS>(rhs) } -> ciel::same_as<LHS>; };

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_ASSIGNABLE_FROM_H_