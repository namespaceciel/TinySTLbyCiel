#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_TYPE_LIST_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_TYPE_LIST_H_

//Not C++ standard

#include <cstddef>

namespace ciel {

	template<class H, class T>
	struct type_list {
		using head = H;
		using tail = T;
	};

	template<class TypeList, size_t size, bool = (size <= sizeof(typename TypeList::head))>
	struct find_first;

	template<class H, class T, size_t size>
	struct find_first<type_list<H, T>, size, true> {
		using type = H;
	};

	template<class H, class T, size_t size>
	struct find_first<type_list<H, T>, size, false> {
		using type = typename find_first<T, size>::type;
	};
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_TYPE_LIST_H_