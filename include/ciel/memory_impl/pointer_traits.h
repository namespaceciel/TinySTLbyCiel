#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_POINTER_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_POINTER_TRAITS_H_

#include <ciel/type_traits.h>
#include <ciel/memory_impl/addressof.h>

namespace ciel {

	namespace pointer_traits_details {

		template<class T, class = void>
		struct has_element_type : ciel::false_type {};

		template<class T>
		struct has_element_type<T, ciel::void_t<typename T::element_type>> : ciel::true_type {};

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
		struct has_difference_type : ciel::false_type {};

		template<class T>
		struct has_difference_type<T, ciel::void_t<typename T::difference_type>> : ciel::true_type {};

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
			static ciel::false_type test(...);
			template<class X>
			static ciel::true_type test(typename X::template rebind<U>* = 0);

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
		using element_type = typename pointer_traits_details::element_type_of<Ptr>::type;
		using difference_type = typename pointer_traits_details::difference_type_of<Ptr>::type;

		template<class U>
		using rebind = typename pointer_traits_details::rebind_of<pointer, U>::type;

		static pointer pointer_to(ciel::conditional_t<ciel::is_void_v<element_type>, void, element_type&> r) {
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

		static constexpr pointer pointer_to(ciel::conditional_t<ciel::is_void_v<element_type>, void, element_type&> r) noexcept {
			return ciel::addressof(r);
		}

	};    //struct pointer_traits<T*>

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_POINTER_TRAITS_H_