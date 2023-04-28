#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_ADD_RVALUE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_ADD_RVALUE_REFERENCE_H_

#include "type_identity.h"

namespace ciel {

	template<class T>
	type_identity<T&&> add_rvalue_reference_helper(int);

	//SFINAE: 防止void到void&&的编译错误，这时丢弃上述特化，而采用以下版本
	template<class T>
	type_identity<T> add_rvalue_reference_helper(...);

	template<class T>
	struct add_rvalue_reference : decltype(add_rvalue_reference_helper<T>(0)) {};

	template<class T>
	using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_ADD_RVALUE_REFERENCE_H_