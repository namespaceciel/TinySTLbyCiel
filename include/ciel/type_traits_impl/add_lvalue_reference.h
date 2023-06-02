#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_LVALUE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_LVALUE_REFERENCE_H_

#include <ciel/type_traits_impl/type_identity.h>

namespace ciel {

	namespace add_lvalue_reference_details {

		template<class T>
		ciel::type_identity<T&> add_lvalue_reference_helper(int);

		// SFINAE: 防止 void 到 void& 的编译错误，这时丢弃上述特化，而采用以下版本
		template<class T>
		ciel::type_identity<T> add_lvalue_reference_helper(...);
	}

	template<class T>
	struct add_lvalue_reference : decltype(add_lvalue_reference_details::add_lvalue_reference_helper<T>(0)) {};

	template<class T>
	using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_LVALUE_REFERENCE_H_