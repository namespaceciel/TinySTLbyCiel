#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_REMOVE_POINTER_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_REMOVE_POINTER_H_

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

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_REMOVE_POINTER_H_