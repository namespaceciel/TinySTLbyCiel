#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_INVOKE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_INVOKE_H_

#include <ciel/type_traits_impl/is_same.h>
#include <ciel/type_traits_impl/is_base_of.h>
#include <ciel/type_traits_impl/is_member_pointer.h>
#include <ciel/type_traits_impl/is_void.h>
#include <ciel/type_traits_impl/decay.h>
#include <ciel/utility_impl/declval.h>
#include <ciel/utility_impl/forward.h>
#include <ciel/memory_impl/addressof.h>

namespace ciel {

	template<class T>
	class reference_wrapper;

	template<class T>
	struct is_reference_wrapper : ciel::false_type {};

	template<class U>
	struct is_reference_wrapper<reference_wrapper<U>> : ciel::true_type {};

	// 仅用于阐述的 INVOKE(f, t1, t2, ..., tN) 操作定义如下，给定类型 T1 为 remove_cvref_t<decltype(t1)>

	// 如果 f 是指向类 T 的成员函数指针，那么 INVOKE(f, t1, t2, ..., tN) 等价于
	// 		(t1.*f)(t2, ..., tN)，如果 is_same_v<T, T1> || is_base_of_v<T, T1> 是 true
	// 		(t1.get().*f)(t2, ..., tN)，如果 T1 是 reference_wrapper 的特化
	// 		((*t1).*f)(t2, ..., tN)，如果 T1 不满足以上两个条件

	template<class F, class T, class Arg1, class T1 = ciel::remove_cvref_t<Arg1>, class... Args>
		requires (ciel::is_function_v<F> && (ciel::is_same_v<T, T1> || ciel::is_base_of_v<T, T1>))
	auto INVOKE(F T::*f, Arg1&& t1, Args&& ... args) -> decltype((ciel::forward<Arg1>(t1).*f)(ciel::forward<Args>(args)...)) {
		return (ciel::forward<Arg1>(t1).*f)(ciel::forward<Args>(args)...);
	}

	template<class F, class T, class Arg1, class T1 = ciel::remove_cvref_t<Arg1>, class... Args>
		requires (ciel::is_function_v<F> && is_reference_wrapper<T1>::value)
	auto INVOKE(F T::*f, Arg1&& t1, Args&& ... args) -> decltype((ciel::forward<Arg1>(t1).get().*f)(ciel::forward<Args>(args)...)) {
		return (ciel::forward<Arg1>(t1).get().*f)(ciel::forward<Args>(args)...);
	}

	template<class F, class T, class Arg1, class T1 = ciel::remove_cvref_t<Arg1>, class... Args>
		requires (ciel::is_function_v<F> && !(ciel::is_same_v<T, T1> || ciel::is_base_of_v<T, T1>) && !is_reference_wrapper<T1>::value)
	auto INVOKE(F T::*f, Arg1&& t1, Args&& ... args) -> decltype(((*ciel::forward<Arg1>(t1)).*f)(ciel::forward<Args>(args)...)) {
		return ((*ciel::forward<Arg1>(t1)).*f)(ciel::forward<Args>(args)...);
	}

	// 否则，如果 N == 1 并且 f 是指向类 T 的数据成员指针，那么 INVOKE(f, t1) 等价于
	// 		t1.*f，如果 is_same_v<T, T1> || is_base_of_v<T, T1> 是 true
	//		t1.get().*f，如果 T1 是 reference_wrapper 的特化
	//		(*t1).*f，如果 T1 不满足以上两个条件

	template<class F, class T, class Arg1, class T1 = ciel::remove_cvref_t<Arg1>>
		requires (!ciel::is_function_v<F> && (ciel::is_same_v<T, T1> || ciel::is_base_of_v<T, T1>))
	auto INVOKE(F T::*f, Arg1&& t1) -> decltype(ciel::forward<Arg1>(t1).*f) {
		return ciel::forward<Arg1>(t1).*f;
	}

	template<class F, class T, class Arg1, class T1 = ciel::remove_cvref_t<Arg1>>
		requires (!ciel::is_function_v<F> && is_reference_wrapper<T1>::value)
	auto INVOKE(F T::*f, Arg1&& t1) -> decltype(ciel::forward<Arg1>(t1).get().*f) {
		return ciel::forward<Arg1>(t1).get().*f;
	}

	template<class F, class T, class Arg1, class T1 = ciel::remove_cvref_t<Arg1>>
		requires (!ciel::is_function_v<F> && !(ciel::is_same_v<T, T1> || ciel::is_base_of_v<T, T1>) && !is_reference_wrapper<T1>::value)
	auto INVOKE(F T::*f, Arg1&& t1) -> decltype((*ciel::forward<Arg1>(t1)).*f) {
		return (*ciel::forward<Arg1>(t1)).*f;
	}

	// 否则，INVOKE(f, t1, t2, ..., tN) 等价于 f(t1, t2, ..., tN) (即 f 是 函数对象 (FunctionObject) ）

	template<class F, class... Args>
		requires (!ciel::is_member_pointer_v<F>)
	auto INVOKE(F&& f, Args&& ... args) -> decltype(ciel::forward<F>(f)(ciel::forward<Args>(args)...)) {
		return ciel::forward<F>(f)(ciel::forward<Args>(args)...);
	}

	// 仅用于阐述的 INVOKE<R>(f, t1, t2, ..., tN) 操作在 R 是（可能有 cv 限定的）void 时定义为 static_cast<void>(INVOKE(f, t1, t2, ..., tN))
	// 否则定义为隐式转换到 R 的 INVOKE(f, t1, t2, ..., tN)

	template<class R, class F, class... Args>
	R INVOKE(F&& f, Args&& ... args) {
		if constexpr (ciel::is_void_v<R>) {
			// 我不太确定这个 static_cast<void> 有什么意义，但是没准是涉及到调用约定
			static_cast<void>(INVOKE(ciel::forward<F>(f), ciel::forward<Args>(args)...));
		} else {
			return INVOKE(ciel::forward<F>(f), ciel::forward<Args>(args)...);
		}
	}

	// TODO: 如果 reference_converts_from_temporary_v<R, decltype(INVOKE(f, t1, t2, ..., tN))> 是 true，那么 INVOKE<R>(f, t1, t2, ..., tN) 非良构

	// 仅若 F 能以参数 ArgTypes... 在不求值语境中调用才得到定义
	template<class F, class... ArgTypes>
	struct invoke_result {};

	template<class F, class... ArgTypes>
		requires requires { INVOKE(ciel::declval<F>(), ciel::declval<ArgTypes>()...); }
	struct invoke_result<F, ArgTypes...> {
		using type = decltype(INVOKE(ciel::declval<F>(), ciel::declval<ArgTypes>()...));
	};

	template<class F, class... ArgTypes>
	using invoke_result_t = typename invoke_result<F, ArgTypes...>::type;

	template<class Fn, class... ArgTypes>
	struct is_invocable : ciel::false_type {};

	template<class Fn, class... ArgTypes>
		requires requires { INVOKE(ciel::declval<Fn>(), ciel::declval<ArgTypes>()...); }
	struct is_invocable<Fn, ArgTypes...> : ciel::true_type {};

	template<class R, class Fn, class... ArgTypes>
	struct is_invocable_r : ciel::false_type {};

	template<class R, class Fn, class... ArgTypes>
		requires requires { INVOKE<R>(ciel::declval<Fn>(), ciel::declval<ArgTypes>()...); }
	struct is_invocable_r<R, Fn, ArgTypes...> : ciel::true_type {};

	template<class Fn, class... ArgTypes>
	struct is_nothrow_invocable : ciel::false_type {};

	template<class Fn, class... ArgTypes>
		requires requires {{ INVOKE(ciel::declval<Fn>(), ciel::declval<ArgTypes>()...) } noexcept; }
	struct is_nothrow_invocable<Fn, ArgTypes...> : ciel::true_type {};

	template<class R, class Fn, class... ArgTypes>
	struct is_nothrow_invocable_r : ciel::false_type {};

	template<class R, class Fn, class... ArgTypes>
		requires requires {{ INVOKE<R>(ciel::declval<Fn>(), ciel::declval<ArgTypes>()...) } noexcept; }
	struct is_nothrow_invocable_r<R, Fn, ArgTypes...> : ciel::true_type {};

	template<class Fn, class... ArgTypes>
	inline constexpr bool is_invocable_v = is_invocable<Fn, ArgTypes...>::value;

	template<class R, class Fn, class... ArgTypes>
	inline constexpr bool is_invocable_r_v = is_invocable_r<R, Fn, ArgTypes...>::value;

	template<class Fn, class... ArgTypes>
	inline constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<Fn, ArgTypes...>::value;

	template<class R, class Fn, class... ArgTypes>
	inline constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<R, Fn, ArgTypes...>::value;

	template<class F, class... Args>
		requires is_invocable_v<F, Args...>
	constexpr invoke_result_t<F, Args...> invoke(F&& f, Args&& ... args) noexcept(is_nothrow_invocable_v<F, Args...>) {
		return INVOKE(ciel::forward<F>(f), ciel::forward<Args>(args)...);
	}

	template<class R, class F, class... Args>
		requires is_invocable_r_v<R, F, Args...>
	constexpr R invoke_r(F && f, Args && ...args) noexcept(is_nothrow_invocable_r_v<R, F, Args...>) {
		return INVOKE<R>(ciel::forward<F>(f), ciel::forward<Args>(args)...);
	}

	namespace reference_wrapper_details {
		template<class T>
		T& FUN(T& t) noexcept {
			return t;
		}

		template<class T>
		void FUN(T&&) = delete;
	}

	template<class T>
	class reference_wrapper {
	public:
		using type = T;

		template<class U>
			requires (!ciel::is_same_v<ciel::decay_t<U>, reference_wrapper>) && requires { reference_wrapper_details::FUN(ciel::declval<U>()); }
		constexpr reference_wrapper(U&& u) noexcept(noexcept(reference_wrapper_details::FUN(ciel::declval<U>()))): ptr(ciel::addressof(reference_wrapper_details::FUN<T>(ciel::forward<U>(u)))) {}

		constexpr reference_wrapper(const reference_wrapper&) noexcept = default;

		constexpr reference_wrapper& operator=(const reference_wrapper&) noexcept = default;

		constexpr operator T&() const noexcept {
			return *ptr;
		}

		constexpr T& get() const noexcept {
			return *ptr;
		}

		template<class... ArgTypes>
		constexpr invoke_result_t<T&, ArgTypes...> operator()(ArgTypes&& ... args) const noexcept(is_nothrow_invocable_v<T&, ArgTypes...>) {
			return invoke(get(), ciel::forward<ArgTypes>(args)...);
		}

	private:
		T* ptr;
	};

	template<class T>
	reference_wrapper(T&) -> reference_wrapper<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_INVOKE_H_