#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_RANK_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_RANK_H_

#include "integral_constant.h"
#include <cstddef>

namespace ciel {

	template<class T>
	struct rank : integral_constant<size_t, 0> {};

	template<class T>
	struct rank<T[]> : integral_constant<size_t, rank<T>::value + 1> {};

	template<class T, std::size_t N>
	struct rank<T[N]> : integral_constant<size_t, rank<T>::value + 1> {};

	template<class T>
	inline constexpr size_t rank_v = rank<T>::value;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_RANK_H_