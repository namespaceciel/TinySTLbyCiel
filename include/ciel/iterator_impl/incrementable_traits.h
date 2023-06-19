#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_TRAITS_H_

#include <ciel/concepts.h>

namespace ciel {

	template<class I>
	struct incrementable_traits {};

	template<class T>
		requires ciel::is_object_v<T>
	struct incrementable_traits<T*> {
		using difference_type = ptrdiff_t;
	};

	template<class T>
	struct incrementable_traits<const T> : incrementable_traits<T> {};

	template<class T>
		requires requires { typename T::difference_type; }
	struct incrementable_traits<T> {
		using difference_type = typename T::difference_type;
	};

	// 不定义公开可访问成员类型 difference_type，但支持减法的类型
	template<class T>
		requires (!requires { typename T::difference_type; }) && requires(const T& a, const T& b) { {a - b} -> ciel::integral; }
	struct incrementable_traits<T>{
		using difference_type = ciel::make_signed_t<decltype(ciel::declval<T>() - ciel::declval<T>())>;
	};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_INCREMENTABLE_TRAITS_H_