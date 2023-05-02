#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_BASE_OF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_BASE_OF_H_

#include "is_class.h"

namespace ciel {

	namespace {
		template<class B>
		true_type is_convertible_to_base(const volatile B*);

		template<class>
		false_type is_convertible_to_base(const volatile void*);

		template<class Base, class Derived>
		auto is_base_of_helper(int) -> decltype(is_convertible_to_base<Base>(static_cast<Derived*>(nullptr)));

		//SFINAE: 防止 private 继承时，子类指针强转为父类指针时的编译错误，这时丢弃上述特化，而采用以下版本
		template<class, class>
		true_type is_base_of_helper(...);
	}

	template<class Base, class Derived>
	struct is_base_of : bool_constant<is_class_v<Base> && is_class_v<Derived> && decltype(is_base_of_helper<Base, Derived>(0))::value> {};

	template<class Base, class Derived>
	inline constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_BASE_OF_H_