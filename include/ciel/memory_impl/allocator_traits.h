#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_TRAITS_H_

#include <ciel/memory_impl/pointer_traits.h>
#include <ciel/memory_impl/construct_at.h>
#include <ciel/memory_impl/destroy_at.h>
#include <ciel/limits.h>

namespace ciel {

	namespace allocator_traits_details {

		template<class T, class Alloc, class = void>
		struct has_pointer {
			using type = T*;
		};

		template<class T, class Alloc>
		struct has_pointer<T, Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::pointer>> {
			using type = typename ciel::remove_reference_t<Alloc>::pointer;
		};

		template<class T, class Ptr, class Alloc, class = void>
		struct has_const_pointer {
			using type = typename ciel::pointer_traits<Ptr>::template rebind<const T>;
		};

		template<class T, class Ptr, class Alloc>
		struct has_const_pointer<T, Ptr, Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::const_pointer>> {
			using type = typename ciel::remove_reference_t<Alloc>::const_pointer;
		};

		template<class Ptr, class Alloc, class = void>
		struct has_void_pointer {
			using type = typename ciel::pointer_traits<Ptr>::template rebind<void>;
		};

		template<class Ptr, class Alloc>
		struct has_void_pointer<Ptr, Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::void_pointer>> {
			using type = typename ciel::remove_reference_t<Alloc>::void_pointer;
		};

		template<class Ptr, class Alloc, class = void>
		struct has_const_void_pointer {
			using type = typename ciel::pointer_traits<Ptr>::template rebind<const void>;
		};

		template<class Ptr, class Alloc>
		struct has_const_void_pointer<Ptr, Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::const_void_pointer>> {
			using type = typename ciel::remove_reference_t<Alloc>::const_void_pointer;
		};

		template<class Ptr, class Alloc, class = void>
		struct has_difference_type {
			using type = typename ciel::pointer_traits<Ptr>::difference_type;
		};

		template<class Ptr, class Alloc>
		struct has_difference_type<Ptr, Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::difference_type>> {
			using type = typename ciel::remove_reference_t<Alloc>::difference_type;
		};

		template<class Diff, class Alloc, class = void>
		struct has_size_type : ciel::make_unsigned<Diff> {};

		template<class Diff, class Alloc>
		struct has_size_type<Diff, Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::size_type>> {
			using type = typename ciel::remove_reference_t<Alloc>::size_type;
		};

		template<class Alloc, class = void>
		struct has_propagate_on_container_copy_assignment : ciel::false_type {};

		template<class Alloc>
		struct has_propagate_on_container_copy_assignment<Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::propagate_on_container_copy_assignment>> {
			using type = typename ciel::remove_reference_t<Alloc>::propagate_on_container_copy_assignment;
		};

		template<class Alloc, class = void>
		struct has_propagate_on_container_move_assignment : ciel::false_type {};

		template<class Alloc>
		struct has_propagate_on_container_move_assignment<Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::propagate_on_container_move_assignment>> {
			using type = typename ciel::remove_reference_t<Alloc>::propagate_on_container_move_assignment;
		};

		template<class Alloc, class = void>
		struct has_propagate_on_container_swap : ciel::false_type {};

		template<class Alloc>
		struct has_propagate_on_container_swap<Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::propagate_on_container_swap>> {
			using type = typename ciel::remove_reference_t<Alloc>::propagate_on_container_swap;
		};

		template<class Alloc, class = void>
		struct has_is_always_equal : ciel::is_empty<Alloc> {};

		template<class Alloc>
		struct has_is_always_equal<Alloc, ciel::void_t<typename ciel::remove_reference_t<Alloc>::is_always_equal>> {
			using type = typename ciel::remove_reference_t<Alloc>::is_always_equal;
		};

		template<class T, class U, class = void>
		struct has_rebind_other : ciel::false_type {};

		template<class T, class U>
		struct has_rebind_other<T, U, ciel::void_t<typename T::template rebind<U>::other>> : ciel::true_type {};

		template<class T, class U, bool = has_rebind_other<T, U>::value>
		struct has_rebind_alloc {
			using type = typename T::template rebind<U>::other;
		};

		template<template<class, class...> class Alloc, class T, class... Args, class U>
		struct has_rebind_alloc<Alloc<T, Args...>, U, true> {
			using type = typename Alloc<T, Args...>::template rebind<U>::other;
		};

		template<template<class, class...> class Alloc, class T, class... Args, class U>
		struct has_rebind_alloc<Alloc<T, Args...>, U, false> {
			using type = Alloc<U, Args...>;
		};

		template<class Alloc, class SizeType, class ConstVoidPtr, class = void>
		struct has_allocate_hint : ciel::false_type {};

		template<class Alloc, class SizeType, class ConstVoidPtr>
		struct has_allocate_hint<Alloc, SizeType, ConstVoidPtr, decltype((void)ciel::declval<Alloc>().allocate(ciel::declval<SizeType>(), ciel::declval<ConstVoidPtr>()))> : ciel::true_type {};

		template<class, class Alloc, class... Args>
		struct has_construct : ciel::false_type {};

		template<class Alloc, class... Args>
		struct has_construct<decltype((void)ciel::declval<Alloc>().construct(ciel::declval<Args...>())), Alloc, Args...> : ciel::true_type {};

		template<class Alloc, class Ptr, class = void>
		struct has_destroy : ciel::false_type {};

		template<class Alloc, class Ptr>
		struct has_destroy<Alloc, Ptr, decltype((void)ciel::declval<Alloc>().destroy(ciel::declval<Ptr>()))> : ciel::true_type {};

		template<class Alloc, class = void>
		struct has_max_size : ciel::false_type {};

		template<class Alloc>
		struct has_max_size<Alloc, decltype((void)ciel::declval<Alloc>().max_size())> : ciel::true_type {};

		template<class Alloc, class = void>
		struct has_select_on_container_copy_construction : ciel::false_type {};

		template<class Alloc>
		struct has_select_on_container_copy_construction<Alloc, decltype((void)ciel::declval<Alloc>().select_on_container_copy_construction())> : ciel::true_type {};

	}	// namespace allocator_traits_details

	template<class Alloc>
	struct allocator_traits {

		using allocator_type = Alloc;
		using value_type = typename allocator_type::value_type;
		using pointer = typename allocator_traits_details::has_pointer<value_type, allocator_type>::type;
		using const_pointer = typename allocator_traits_details::has_const_pointer<value_type, pointer, allocator_type>::type;
		using void_pointer = typename allocator_traits_details::has_void_pointer<pointer, allocator_type>::type;
		using const_void_pointer = typename allocator_traits_details::has_const_void_pointer<pointer, allocator_type>::type;
		using difference_type = typename allocator_traits_details::has_difference_type<pointer, allocator_type>::type;
		using size_type = typename allocator_traits_details::has_size_type<difference_type, allocator_type>::type;
		using propagate_on_container_copy_assignment = typename allocator_traits_details::has_propagate_on_container_copy_assignment<allocator_type>::type;
		using propagate_on_container_move_assignment = typename allocator_traits_details::has_propagate_on_container_move_assignment<allocator_type>::type;
		using propagate_on_container_swap = typename allocator_traits_details::has_propagate_on_container_swap<allocator_type>::type;
		using is_always_equal = typename allocator_traits_details::has_is_always_equal<allocator_type>::type;

		template<class U>
		using rebind_alloc = typename allocator_traits_details::has_rebind_alloc<allocator_type, U>::type;
		template<class U>
		using rebind_traits = allocator_traits<rebind_alloc<U>>;

		[[nodiscard]] static constexpr pointer allocate(allocator_type& a, size_type n) {
			return a.allocate(n);
		}

		template<class A = allocator_type, class = ciel::enable_if_t<allocator_traits_details::has_allocate_hint<A, size_type, const_void_pointer>::value>>
		[[nodiscard]] static constexpr pointer allocate(allocator_type& a, size_type n, const_void_pointer hint) {
			return a.allocate(n, hint);
		}

		template<class A = allocator_type, class = void, class = ciel::enable_if_t<!allocator_traits_details::has_allocate_hint<A, size_type, const_void_pointer>::value>>
		[[nodiscard]] static constexpr pointer allocate(allocator_type& a, size_type n, const_void_pointer hint) {
			return a.allocate(n);
		}

		static constexpr void deallocate(allocator_type& a, pointer p, size_type n) {
			a.deallocate(p, n);
		}

		template<class T, class... Args, class = ciel::enable_if_t<allocator_traits_details::has_construct<void, allocator_type, T*, Args...>::value>>
		static constexpr void construct(allocator_type& a, T* p, Args&& ... args) {
			a.construct(p, ciel::forward<Args>(args)...);
		}

		template<class T, class... Args, class = void, class = ciel::enable_if_t<!allocator_traits_details::has_construct<void, allocator_type, T*, Args...>::value>>
		static constexpr void construct(allocator_type& a, T* p, Args&& ... args) {
			// 显式加上命名空间作用域是为了防止在形参为 std:: 物件时触发 ADL 而跟 std::construct_at() 撞车
			ciel::construct_at(p, ciel::forward<Args>(args)...);
		}

		template<class T, class = ciel::enable_if_t<allocator_traits_details::has_destroy<allocator_type, T*>::value>>
		static constexpr void destroy(allocator_type& a, T* p) {
			a.destroy(p);
		}

		template<class T, class = void, class = ciel::enable_if_t<!allocator_traits_details::has_destroy<allocator_type, T*>::value>>
		static constexpr void destroy(allocator_type& a, T* p) {
			ciel::destroy_at(p);
		}

		// 以下两组 SFINAE 函数如果去掉模板参数 A 而写成 template<class = enable_if_t<false>> 形式会报错的原因为：
		// 			SFINAE 是在模板替换时出现错误才会抛弃掉替换的结果转而尝试下一个可选模板，在上例中根本没有需要被推导的模板参数，也就压根不会实现 SFINAE
		template<class A = allocator_type, class = ciel::enable_if_t<allocator_traits_details::has_max_size<const A>::value>>
		static constexpr size_type max_size(const A& a) noexcept {
			return a.max_size();
		}

		template<class A = allocator_type, class = void, class = ciel::enable_if_t<!allocator_traits_details::has_max_size<const A>::value>>
		static constexpr size_type max_size(const A& a) noexcept {
			return ciel::numeric_limits<size_type>::max() / sizeof(value_type);
		}

		template<class A = allocator_type, class = ciel::enable_if_t<allocator_traits_details::has_select_on_container_copy_construction<const A>::value>>
		static constexpr A select_on_container_copy_construction(const A& a) {
			return a.select_on_container_copy_construction();
		}

		template<class A = allocator_type, class = void, class = ciel::enable_if_t<!allocator_traits_details::has_select_on_container_copy_construction<const A>::value>>
		static constexpr A select_on_container_copy_construction(const A& a) {
			return a;
		}

	};    // struct allocator_traits

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_TRAITS_H_