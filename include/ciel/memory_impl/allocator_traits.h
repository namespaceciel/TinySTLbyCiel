#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_TRAITS_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_TRAITS_H_

#include <ciel/type_traits.h>
#include <ciel/memory_impl/pointer_traits.h>

namespace ciel {

	namespace {
		template<class T, class Alloc, class = void>
		struct has_pointer {
			using type = T*;
		};

		template<class T, class Alloc>
		struct has_pointer<T, Alloc, typename remove_reference_t<Alloc>::pointer> {
			using type = typename remove_reference_t<Alloc>::pointer;
		};

/////////////////////////////////////////////////////////////////////////////////////

		template<class T, class Ptr, class Alloc, class = void>
		struct has_const_pointer {
			using type = typename pointer_traits<Ptr>::template rebind<const T>;
		};

		template<class T, class Ptr, class Alloc>
		struct has_const_pointer<T, Ptr, Alloc, typename remove_reference_t<Alloc>::const_pointer> {
			using type = typename remove_reference_t<Alloc>::const_pointer;
		};

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////

	}

	template<class Alloc>
	struct allocator_traits {

		using allocator_type = Alloc;
		using value_type = typename allocator_type::value_type;
		using pointer = typename has_pointer<value_type, allocator_type>::type;
		using const_pointer = typename has_const_pointer<value_type, pointer, allocator_type>::type;
		using void_pointer = typename has_void_pointer<pointer, allocator_type>::type;
		using const_void_pointer = typename has_const_void_pointer<pointer, allocator_type>::type;
		using difference_type = typename has_alloc_traits_difference_type<allocator_type, pointer>::type;
		using size_type = typename has_size_type<allocator_type, difference_type>::type;
		using propagate_on_container_copy_assignment = typename has_propagate_on_container_copy_assignment<allocator_type>::type;
		using propagate_on_container_move_assignment = typename has_propagate_on_container_move_assignment<allocator_type>::type;
		using propagate_on_container_swap = typename has_propagate_on_container_swap<allocator_type>::type;
		using is_always_equal = typename has_is_always_equal<allocator_type>::type;

	};    //struct allocator_traits

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_ALLOCATOR_TRAITS_H_