#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_REFERENCE_WRAPPER_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_REFERENCE_WRAPPER_H_

#include <ciel/utility.h>
#include <ciel/type_traits_impl/enable_if.h>
#include <ciel/type_traits_impl/is_same.h>
#include <ciel/type_traits_impl/remove_cvref.h>
#include <ciel/memory_impl/addressof.h>

namespace ciel {

	namespace detail {
		template<class T> T& FUN(T& t) noexcept { return t; }
		template<class T> void FUN(T&&) = delete;
	}

	template<class T>
	class reference_wrapper {
	public:
		// 类型
		using type = T;

		// 构造/复制/销毁
		template <class U, class = decltype(detail::FUN<T>(ciel::declval<U>()), ciel::enable_if_t<!ciel::is_same_v<reference_wrapper, ciel::remove_cvref_t<U>>>())>
		constexpr reference_wrapper(U&& u) noexcept(noexcept(detail::FUN<T>(ciel::forward<U>(u)))) : _ptr(ciel::addressof(detail::FUN<T>(ciel::forward<U>(u)))) {}
		reference_wrapper(const reference_wrapper&) noexcept = default;

		// 赋值
		reference_wrapper& operator=(const reference_wrapper& x) noexcept = default;

		// 访问
		constexpr operator T& () const noexcept { return *_ptr; }
		constexpr T& get() const noexcept { return *_ptr; }

		template< class... ArgTypes >
		constexpr ciel::invoke_result_t<T&, ArgTypes...>
		operator() ( ArgTypes&&... args ) const {
			return ciel::invoke(get(), ciel::forward<ArgTypes>(args)...);
		}

	private:
		T* _ptr;
	};

// 推导指引
	template<class T>
	reference_wrapper(T&) -> reference_wrapper<T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_REFERENCE_WRAPPER_H_