#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_TYPE_IDENTITY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_TYPE_IDENTITY_H_

namespace ciel {

	template<class T>
	struct type_identity {
		using type = T;
	};

	template<class T>
	using type_identity_t = typename type_identity<T>::type;
/*
	type_identity 能用于在模板实参推导中建立非推导语境：

	template<class T>
	void f(T, T);

	template<class T>
	void g(T, type_identity_t<T>);

	f(4.2, 0); // 错误：对 'T' 推导出冲突的类型
	g(4.2, 0); // OK ：调用 g<double>
*/
}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_TYPE_IDENTITY_H_