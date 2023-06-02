#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SAME_AS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SAME_AS_H_

#include <ciel/type_traits.h>

namespace ciel {

	namespace same_as_details {
		template<class T, class U>
		concept same_as_helper = ciel::is_same_v<T, U>;
	}

	template<class T, class U>
	concept same_as = same_as_details::same_as_helper<T, U> && same_as_details::same_as_helper<U, T>;
/*
1、为什么要将 is_same_v 先转换为 same_as_helper ？
不同于 type_traits 中的 is_same_v 只是一个编译期 bool 常量，概念之间可以进行合取（conjunction）与析取（disjunction），
而编译器底层有办法把它们重新分解为不可分割约束（atomic constraint）。

所以考虑到一个需要偏特化的场景：
	template<class T, class U>
 		requires same_as<T, U>
	void f(T, U) { std::cout << "T 与 U 为同类型且不为整型"; }

 	template<class T, class U>
 		requires same_as<T, U> && std::integral<T>
	void f(T, U) { std::cout << "T 与 U 同为整型"; }

因为概念有分解的能力，所以以上代码可以做到偏特化，如果是 type_traits 中一堆编译期 bool 常量的合取与析取则做不到这一点。

2、为什么 same_as 需要是双向的 same_as_helper 的合取？
因为如果只有单向的话，same_as<T, U> 与 same_as<U, T> 将会是两个不同的概念，放在偏特化场景下是无法比较的，
只有当一个概念完全包含另一个概念时，它们才能实现偏特化。
*/
}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_SAME_AS_H_