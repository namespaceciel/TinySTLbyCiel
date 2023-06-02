#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COMMON_WITH_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COMMON_WITH_H_

#include <ciel/concepts_impl/same_as.h>
#include <ciel/concepts_impl/common_reference_with.h>

namespace ciel {

	template<class T, class U>
	concept common_with = ciel::same_as<ciel::common_type_t<T, U>, ciel::common_type_t<U, T>>
						  && requires { static_cast<ciel::common_type_t<T, U>>(ciel::declval<T>());
									    static_cast<ciel::common_type_t<T, U>>(ciel::declval<U>()); }
						  && ciel::common_reference_with<ciel::add_lvalue_reference_t<const T>,
						    						     ciel::add_lvalue_reference_t<const U>>
						  && ciel::common_reference_with<ciel::add_lvalue_reference_t<ciel::common_type_t<T, U>>,
													     ciel::common_reference_t<ciel::add_lvalue_reference_t<const T>,
														                          ciel::add_lvalue_reference_t<const U>>>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_COMMON_WITH_H_