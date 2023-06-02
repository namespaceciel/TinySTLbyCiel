#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CONVERTIBLE_TO_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CONVERTIBLE_TO_H_

#include <ciel/type_traits.h>

namespace ciel {

	template<class From, class To>
	concept convertible_to = ciel::is_convertible_v<From, To> && requires { static_cast<To>(ciel::declval<From>()); };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_CONVERTIBLE_TO_H_