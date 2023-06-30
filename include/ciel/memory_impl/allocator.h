#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_H_

#include <ciel/type_traits.h>
#include <ciel/limits.h>
#include <new>
#include <cstddef>

namespace ciel {

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