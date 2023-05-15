#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_RVALUE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_RVALUE_REFERENCE_H_

#include <ciel/type_traits_impl/type_identity.h>

namespace ciel {

	namespace add_rvalue_reference_details {

		template<class T>
		ciel::type_identity<T&&> add_rvalue_reference_helper(int);

		//SFINAE: 防止 void 到 void&& 的编译错误，这时丢弃上述特化，而采用以下版本
		template<class T>
		ciel::type_identity<T> add_rvalue_reference_helper(...);
	}

	template<class T>
	struct add_rvalue_reference : decltype(add_rvalue_reference_details::add_rvalue_reference_helper<T>(0)) {};

	template<class T>
	using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_RVALUE_REFERENCE_H_