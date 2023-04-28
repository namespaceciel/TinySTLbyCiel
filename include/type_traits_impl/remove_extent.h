#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_REMOVE_EXTENT_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_REMOVE_EXTENT_H_

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

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_REMOVE_EXTENT_H_