#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_UNSIGNED_INTEGRAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_UNSIGNED_INTEGRAL_H_

#include <ciel/concepts_impl/signed_integral.h>

namespace ciel {

	template<class T>
	concept unsigned_integral = ciel::integral<T> && !ciel::signed_integral<T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_UNSIGNED_INTEGRAL_H_