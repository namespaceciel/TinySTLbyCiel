#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_USES_ALLOCATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_USES_ALLOCATOR_H_

#include <ciel/type_traits.h>
#include <ciel/utility.h>
#include <ciel/memory_impl/allocator_arg.h>

namespace ciel {

	// 若 T 拥有：
	// 1）内嵌定义类型 allocator_type ，并且该类型能从 Alloc 转换
	// TODO: 2）或者该类型是 std::experimental::erased_type 的别名 (库基础 TS)
	// 则成员常量 value 为 true 。否则 value 为 false

	template<class T, class Alloc>
	struct uses_allocator : false_type {};

	template<class T, class Alloc>
		requires requires { typename T::allocator_type; } && ciel::is_convertible_v<Alloc, typename T::allocator_type>
	struct uses_allocator<T, Alloc> : true_type {};

	template<class T, class Alloc>
	inline constexpr bool uses_allocator_v = uses_allocator<T, Alloc>::value;

	// 传递分配器 alloc 给某类型 T 的构造函数有三种约定：
	// 1、若 T 不使用兼容的分配器（ uses_allocator_v<T, Alloc> 为 false ），则忽略 alloc 。
	// 否则 uses_allocator_v<T, Alloc> 为 true ，而
	// 		2、若 T 使用前导分配器约定（可用 T(allocator_arg, alloc, args...) 调用），则使用分配器构造使用此形式
	// 		3、若 T 使用尾随分配器约定（可用 T(args..., alloc) 调用），则使用分配器构造使用此形式
	// 否则，程序为谬构（这表示 uses_allocator_v<T, Alloc> 为 true ，但类型不遵守二种允许的约定之一）

	template<class T, class Alloc, class... Args>
	struct uses_alloc_ctor_impl {
		static const bool ua = uses_allocator_v<T, ciel::remove_cvref_t<Alloc>>;
		static const bool ic = ciel::is_constructible_v<T, allocator_arg_t, Alloc, Args...>;
		static const int value = ua ? 2 - ic : 0;
	};

	template<class T, class Alloc, class ...Args>
	struct uses_alloc_ctor : ciel::integral_constant<int, uses_alloc_ctor_impl<T, Alloc, Args...>::value> {};

	template<class T, class Alloc, class... Args>
	inline void user_alloc_construct_impl(ciel::integral_constant<int, 0>, T* storage, const Alloc&, Args&& ... args) {
		new(storage) T(ciel::forward<Args>(args)...);
	}

	template<class T, class Alloc, class... Args>
	inline void user_alloc_construct_impl(ciel::integral_constant<int, 1>, T* storage, Alloc& a, Args&& ... args) {
		new(storage) T(allocator_arg, a, ciel::forward<Args>(args)...);
	}

	template<class T, class Alloc, class... Args>
	inline void user_alloc_construct_impl(ciel::integral_constant<int, 2>, T* storage, Alloc& a, Args&& ... args) {
		new(storage) T(ciel::forward<Args>(args)..., a);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_USES_ALLOCATOR_H_