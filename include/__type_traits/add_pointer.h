#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_POINTER_H_

#include "remove_reference.h"
#include "type_identity.h"

namespace ciel {

	template<class T>
	struct add_pointer : type_identity<remove_reference_t<T>*> {};

	template<class T>
	using add_pointer_t = typename add_pointer<T>::type;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_POINTER_H_