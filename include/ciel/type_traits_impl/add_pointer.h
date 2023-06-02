#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_POINTER_H_

#include <ciel/type_traits_impl/remove_reference.h>
#include <ciel/type_traits_impl/type_identity.h>

namespace ciel {

	namespace add_pointer_details {

		template<class T>
		ciel::type_identity<ciel::remove_reference_t<T>*> add_pointer_helper(int);

		/*
		SFINAE: 若 T 为 cv 或引用限定的函数类型时启用
		e.g.
		struct S {
			add_pointer_t<void()const> f;
		};
		*/
		template<class T>
		ciel::type_identity<T> add_pointer_helper(...);
	}

	template<class T>
	struct add_pointer : decltype(add_pointer_details::add_pointer_helper<T>(0)) {};

	template<class T>
	using add_pointer_t = typename add_pointer<T>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_POINTER_H_