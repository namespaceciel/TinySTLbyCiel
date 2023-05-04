#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_POINTER_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_POINTER_TRAITS_H_

#include <ciel/type_traits.h>
#include <ciel/memory_impl/addressof.h>

namespace ciel {

	namespace {
		template<class T, class = void>
		struct has_element_type : false_type {};

		template<class T>
		struct has_element_type<T, typename T::element_type> : true_type {};

		template<class Ptr, bool = has_element_type<Ptr>::value>
		struct element_type_of {};

		template<class Ptr>
		struct element_type_of<Ptr, true> {
			using type = typename Ptr::element_type;
		};
		//令 Ptr 为 S<T, Args...>
		template<template<class, class...> class S, class T, class... Args>
		struct element_type_of<S<T, Args...>, true> {
			using type = typename S<T, Args...>::element_type;
		};

		template<template<class, class...> class S, class T, class... Args>
		struct element_type_of<S<T, Args...>, false> {
			using type = T;
		};

/////////////////////////////////////////////////////////////////////////////////////

		template<class T, class = void>
		struct has_difference_type : false_type {};

		template<class T>
		struct has_difference_type<T, typename T::difference_type> : true_type {};

		template<class Ptr, bool = has_difference_type<Ptr>::type>
		struct difference_type_of {
			using type = ptrdiff_t;
		};

		template<class Ptr>
		struct difference_type_of<Ptr, true> {
			using type = typename has_difference_type<Ptr>::type;
		};

/////////////////////////////////////////////////////////////////////////////////////

		template<class T, class U>
		struct has_rebind {
		private:
			template<class X>
			static false_type test(...);
			template<class X>
			static true_type test(typename X::template rebind<U>* = 0);

		public:
			static const bool value = decltype(test<T>(0))::value;
		};

		template<class T, class U, bool = has_rebind<T, U>::value>
		struct rebind_of {
			using type = typename T::template rebind<U>;
		};

		template<template<class, class...> class S, class T, class... Args, class U>
		struct rebind_of<S<T, Args...>, U, true> {
			using type = typename S<T, Args...>::template rebind<U>;
		};

		template<template<class, class...> class S, class T, class... Args, class U>
		struct rebind_of<S<T, Args...>, U, false> {
			using type = S<T, Args...>;
		};
	}

	template<class Ptr>
	struct pointer_traits {

		using pointer = Ptr;
		using element_type = typename element_type_of<Ptr>::type;
		using difference_type = typename difference_type_of<Ptr>::type;

		template<class U>
		using rebind = typename rebind_of<pointer, U>::type;

		static pointer pointer_to(conditional_t<is_void_v<element_type>, void, element_type&> r) {
			return pointer::pointer_to(r);
		}

	};    //struct pointer_traits

	template<class T>
	struct pointer_traits<T*> {

		using pointer = T*;
		using element_type = T;
		using difference_type = ptrdiff_t;

		template<class U>
		using rebind = U*;

		static constexpr pointer pointer_to(conditional_t<is_void_v<element_type>, void, element_type&> r) noexcept {
			return addressof(r);
		}

	};    //struct pointer_traits<T*>

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_POINTER_TRAITS_H_