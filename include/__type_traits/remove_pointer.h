#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_POINTER_H_

namespace ciel {

	template<class T>
	struct remove_pointer {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T*> {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T* const> {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T* volatile> {
		using type = T;
	};

	template<class T>
	struct remove_pointer<T* const volatile> {
		using type = T;
	};

	template<class T>
	using remove_pointer_t = typename remove_pointer<T>::type;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_POINTER_H_