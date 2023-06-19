#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DECAY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DECAY_H_

#include <ciel/type_traits_impl/is_array.h>
#include <ciel/type_traits_impl/remove_extent.h>
#include <ciel/type_traits_impl/is_function.h>
#include <ciel/type_traits_impl/add_pointer.h>
#include <ciel/type_traits_impl/remove_cvref.h>
#include <ciel/type_traits_impl/conditional.h>

namespace ciel {

	// 对类型 T 应用左值到右值、数组到指针及函数到函数指针的隐式转换，移除 cv 限定符
	template<class T>
	class decay {
	private:
		using U = ciel::remove_reference_t<T>;

	public:
		using type = ciel::conditional_t<ciel::is_array_v<U>,
										 ciel::remove_extent_t<U>*,
										 ciel::conditional_t<ciel::is_function_v<U>,
															 ciel::add_pointer_t<U>,
															 ciel::remove_cv_t<U>>>;
	};

	template<class T>
	using decay_t = typename decay<T>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_DECAY_H_