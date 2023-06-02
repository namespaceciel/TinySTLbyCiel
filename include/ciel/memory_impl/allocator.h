#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_H_

#include <ciel/type_traits.h>
#include <ciel/limits.h>
#include <new>
#include <cstddef>

namespace ciel {
/*
	namespace allocator_details {
		template<bool, class Unique>
		struct non_trivial_if {};

		template<class Unique>
		struct non_trivial_if<true, Unique> {
			constexpr non_trivial_if() noexcept {}
		};
	}

	这里 Clang libc++ 为了在 C++20 与之前的标准达成 ABI 兼容，让 allocator<void> 作为平凡类

		保持 ABI 稳定意味着不更改函数接口（返回类型和数字、类型和参数顺序）、数据类型或数据结构的定义、定义的常量等
		可以添加新的函数和数据类型，但必须保留现有的不变
	 	例如，如果一个库使用 32 位整数来指示函数的偏移量，而后被切换到 64 位整数，那么使用该库的已编译代码将无法正确访问该字段（或其后的任何字段）
		访问数据结构成员在编译期间被转换为内存地址和偏移量，如果数据结构发生变化，那么这些偏移量将不会指向代码期望它们指向的位置，结果充其量是不可预测的。

	当 allocator 的模板形参 T 不为 void 时，通过继承特化的（有用户自定义的默认构造函数）非平凡类，使得 allocator<T> 为非平凡类

	non_trivial_if 的模板第二形参为 CRTP 实现编译期多态，同时避免了菱形继承所产生的 ABI（内存布局）不一致性问题
*/
	template<class T>
	class allocator {

		static_assert(!ciel::is_const_v<T> && !ciel::is_volatile_v<T>, "分配器要求无 cv 限定的对象类型");

	public:
		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using propagate_on_container_move_assignment = ciel::true_type;

	public:
		constexpr allocator() noexcept = default;

		constexpr allocator(const allocator&) noexcept = default;

		template<class U>
		constexpr allocator(const allocator<U>&) noexcept {}

		constexpr ~allocator() = default;

		// 没看懂 allocate 和 deallocate 这里的标准库实现。。。目前来看像是为了标准里的这一要求：
		// 				  在常量表达式的求值中，此函数必须解分配在同一表达式的求值内分配的存储。
		// TODO: 搞懂过对齐对象用对齐版本的 ::operator new 的必要性
		[[nodiscard]] constexpr T* allocate(size_t n) {
			if (ciel::numeric_limits<size_t>::max() / sizeof(T) < n) {
				throw std::bad_array_new_length();
			}
			if (ciel::is_constant_evaluated()) {
				return static_cast<T*>(::operator new(sizeof(T) * n));
			} else if (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
				return static_cast<T*>(::operator new(sizeof(T) * n, static_cast<std::align_val_t>(alignof(T))));
			} else {
				return static_cast<T*>(::operator new(sizeof(T) * n));
			}
		}

		constexpr void deallocate(T* p, size_t) {
			if (ciel::is_constant_evaluated()) {
				::operator delete((void*)p);
			} else if (alignof(T) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
				::operator delete((void*)p, static_cast<std::align_val_t>(alignof(T)));
			} else {
				::operator delete((void*)p);
			}
		}

	};  // class allocator

	template<class T1, class T2>
	constexpr bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept {
		return true;
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_H_