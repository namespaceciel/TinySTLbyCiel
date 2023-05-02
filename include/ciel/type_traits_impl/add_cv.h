#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_CV_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_CV_H_

namespace ciel {

	template<class T>
	struct add_cv {
		using type = const volatile T;
	};

	template<class T>
	struct add_const {
		using type = const T;
	};

	template<class T>
	struct add_volatile {
		using type = volatile T;
	};

	template<class T>
	using add_cv_t = typename add_cv<T>::type;

	template<class T>
	using add_const_t = typename add_const<T>::type;

	template<class T>
	using add_volatile_t = typename add_volatile<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ADD_CV_H_