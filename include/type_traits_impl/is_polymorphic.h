#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_POLYMORPHIC_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_POLYMORPHIC_H_

#include "integral_constant.h"

namespace ciel {

	//SFINAE: 如果表达式是指向多态类型的指针，且新类型是到void的指针，那么结果是指向表达式所指向或引用的最终派生对象的指针
	template<class T>
	true_type is_polymorphic_helper(decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr))));

	template<class T>
	false_type is_polymorphic_helper(...);

	template<class T>
	struct is_polymorphic : decltype(is_polymorphic_helper<T>(nullptr)) {};

	template< class T >
	inline constexpr bool is_polymorphic_v = is_polymorphic<T>::value;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_IS_POLYMORPHIC_H_