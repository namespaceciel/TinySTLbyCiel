#ifndef TINYSTLBYCIEL_INCLUDE___UTILITY_DECLVAL_H_
#define TINYSTLBYCIEL_INCLUDE___UTILITY_DECLVAL_H_

#include "../type_traits.h"

namespace ciel {

	template<class T>
	add_rvalue_reference_t<T> declval() noexcept;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___UTILITY_DECLVAL_H_