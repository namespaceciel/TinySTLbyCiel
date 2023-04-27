#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_POINTER_H_

#include "remove_reference.h"
#include "type_identity.h"

namespace ciel {

	template<class T>
	type_identity<remove_reference_t<T>*> add_pointer_helper(int);

	//SFINAE: 若T为cv或引用限定的函数类型时启用
	/*
	e.g.
	struct S {
		add_pointer_t<void()const> f;
	};
	*/
	template<class T>
	type_identity<T> add_pointer_helper(...);

	template<class T>
	struct add_pointer : decltype(add_pointer_helper<T>(0)) {};

	template<class T>
	using add_pointer_t = typename add_pointer<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_ADD_POINTER_H_