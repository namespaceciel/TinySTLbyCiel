#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_ALL_EXTENTS_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_ALL_EXTENTS_H_

#include <cstddef>

namespace ciel {

	template<class T>
	struct remove_all_extents {
		using type = T;
	};

	template<class T>
	struct remove_all_extents<T[]> {
		using type = typename remove_all_extents<T>::type;
	};

	template<class T, size_t N>
	struct remove_all_extents<T[N]> {
		using type = typename remove_all_extents<T>::type;
	};

	template<class T>
	using remove_all_extents_t = typename remove_all_extents<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_ALL_EXTENTS_H_