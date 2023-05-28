#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_DECLVAL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_DECLVAL_H_

#include <ciel/type_traits_impl/add_rvalue_reference.h>

namespace ciel {
	//将任意类型 T 转换成引用类型，使得在 decltype 说明符的操作数中不必经过构造函数就能使用成员函数（只能用于不求值语境）
	template<class T>
	ciel::add_rvalue_reference_t<T> declval() noexcept;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_UTILITY_IMPL_DECLVAL_H_