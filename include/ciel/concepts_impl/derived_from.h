#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DERIVED_FROM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DERIVED_FROM_H_

#include <ciel/type_traits.h>

namespace ciel {
	//注意当 Base 是 Derived 的私有或受保护基类时，此行为异于 std::is_base_of
	template<class Derived, class Base>
	concept derived_from = ciel::is_base_of_v<Base, Derived> && ciel::is_convertible_v<const volatile Derived*, const volatile Base*>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_DERIVED_FROM_H_