#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DECAY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DECAY_H_

#include <ciel/type_traits_impl/remove_reference.h>
#include <ciel/type_traits_impl/is_array.h>
#include <ciel/type_traits_impl/remove_extent.h>
#include <ciel/type_traits_impl/is_function.h>
#include <ciel/type_traits_impl/add_pointer.h>
#include <ciel/type_traits_impl/remove_cv.h>
#include <ciel/type_traits_impl/conditional.h>

namespace ciel {

	//对类型 T 应用左值到右值、数组到指针及函数到函数指针的隐式转换，移除 cv 限定符
	template<class T>
	class decay {
	private:
		using U = remove_reference_t<T>;

	public:
		using type = conditional_t<is_array_v<U>,
								   remove_extent_t<U>*,
								   conditional_t<is_function_v<U>,
								       			 add_pointer_t<U>,
								       			 remove_cv_t<U>>>;
	};

	template<class T>
	using decay_t = typename decay<T>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DECAY_H_