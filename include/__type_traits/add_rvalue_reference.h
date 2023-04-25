#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_RVALUE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_RVALUE_REFERENCE_H_

#include "type_identity.h"

namespace ciel {

	template<class T>
	struct add_rvalue_reference : type_identity<T&&> {};

	template<class T>
	using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_RVALUE_REFERENCE_H_