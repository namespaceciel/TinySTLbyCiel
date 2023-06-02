#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ADDRESSOF_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ADDRESSOF_H_

namespace ciel {

	template<class T>
	constexpr T* addressof(T& arg) noexcept {
		return __builtin_addressof(arg);
	}

	template<class T>
	const T* addressof(const T&&) = delete;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ADDRESSOF_H_