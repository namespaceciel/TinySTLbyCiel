#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_CV_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_CV_H_

namespace ciel{

	template<class T>
	struct remove_cv {
		using type = T;
	};

	template<class T>
	struct remove_cv<const T> {
		using type = T;
	};

	template<class T>
	struct remove_cv<volatile T> {
		using type = T;
	};

	template<class T>
	struct remove_cv<const volatile T> {
		using type = T;
	};

	template<class T>
	struct remove_const {
		using type = T;
	};

	template<class T>
	struct remove_const<const T> {
		using type = T;
	};

	template<class T>
	struct remove_volatile {
		using type = T;
	};

	template<class T>
	struct remove_volatile<volatile T> {
		using type = T;
	};

	template<class T>
	using remove_cv_t = typename remove_cv<T>::type;

	template<class T>
	using remove_const_t = typename remove_const<T>::type;

	template<class T>
	using remove_volatile_t = typename remove_volatile<T>::type;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_REMOVE_CV_H_