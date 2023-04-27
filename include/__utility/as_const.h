#ifndef TINYSTLBYCIEL_INCLUDE___UTILITY_AS_CONST_H_
#define TINYSTLBYCIEL_INCLUDE___UTILITY_AS_CONST_H_

#include "../type_traits.h"

namespace ciel {

	template<class T>
	constexpr add_const_t<T>& as_const(T& t) noexcept {
		return t;
	}

	template<class T>
	void as_const(const T&&) = delete;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___UTILITY_AS_CONST_H_