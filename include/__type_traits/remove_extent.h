#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_EXTENT_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_EXTENT_H_

#include <cstddef>

namespace ciel{

	template<class T>
	struct remove_extent {
		using type = T;
	};

	template<class T>
	struct remove_extent<T[]> {
		using type = T;
	};

	template<class T, size_t N>
	struct remove_extent<T[N]> {
		using type = T;
	};

	template<class T>
	using remove_extent_t = typename remove_extent<T>::type;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_EXTENT_H_