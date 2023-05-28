#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DEFAULT_DELETE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DEFAULT_DELETE_H_

#include <ciel/type_traits.h>

namespace ciel {

	template<class T>
	struct default_delete {
		constexpr default_delete() noexcept = default;

		//default_delete 的转换构造函数模板令从 unique_ptr<Derived> 到 unique_ptr<Base> 的隐式转换可行
		template<class U, class = ciel::enable_if_t<ciel::is_convertible_v<U*, T*>>>
		default_delete(const default_delete<U>& d) noexcept {}

		void operator()(T* ptr) const {
			delete ptr;
		}
	};

	template<class T>
	struct default_delete<T[]> {
		constexpr default_delete() noexcept = default;

		template<class U, class = ciel::enable_if_t<ciel::is_convertible_v<U(*)[], T(*)[]>>>
		default_delete(const default_delete<U[]>& d) noexcept {}

		template<class U, class = ciel::enable_if_t<ciel::is_convertible_v<U(*)[], T(*)[]>>>
		void operator()(U* ptr) const {
			delete[] ptr;
		}
	};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_DEFAULT_DELETE_H_