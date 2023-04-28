#ifndef TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_REMOVE_REFERENCE_H_
#define TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_REMOVE_REFERENCE_H_

namespace ciel {

	template<class T>
	struct remove_reference {
		using type = T;
	};

	template<class T>
	struct remove_reference<T&> {
		using type = T;
	};

	template<class T>
	struct remove_reference<T&&> {
		using type = T;
	};

	template<class T>
	using remove_reference_t = typename remove_reference<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_TYPE_TRAITS_IMPL_REMOVE_REFERENCE_H_