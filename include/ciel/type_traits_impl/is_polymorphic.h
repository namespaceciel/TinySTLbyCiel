#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_POLYMORPHIC_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_POLYMORPHIC_H_

#include <ciel/type_traits_impl/integral_constant.h>

namespace ciel {

	namespace is_polymorphic_details {
		//SFINAE: 如果表达式是指向多态类型的指针，且新类型是到 void 的指针，那么结果是指向表达式所指向或引用的最终派生对象的指针
		template<class T>
		ciel::true_type is_polymorphic_helper(decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr))));

		template<class T>
		ciel::false_type is_polymorphic_helper(...);
	}

	template<class T>
	struct is_polymorphic : decltype(is_polymorphic_details::is_polymorphic_helper<T>(nullptr)) {};

	template< class T >
	inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_POLYMORPHIC_H_