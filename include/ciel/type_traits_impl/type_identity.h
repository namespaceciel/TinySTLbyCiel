#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_TYPE_IDENTITY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_TYPE_IDENTITY_H_

namespace ciel {

	template<class T>
	struct type_identity {
		using type = T;
	};

	template<class T>
	using type_identity_t = typename type_identity<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_TYPE_IDENTITY_H_