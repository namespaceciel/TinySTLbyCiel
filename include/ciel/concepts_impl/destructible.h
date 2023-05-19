#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DESTRUCTIBLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DESTRUCTIBLE_H_

#include <ciel/type_traits.h>

namespace ciel {

	template<class T>
	concept destructible = ciel::is_nothrow_destructible_v<T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DESTRUCTIBLE_H_