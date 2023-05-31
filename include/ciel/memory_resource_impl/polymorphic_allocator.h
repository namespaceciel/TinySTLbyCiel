#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_POLYMORPHIC_ALLOCATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_POLYMORPHIC_ALLOCATOR_H_

#include <ciel/memory_resource_impl/memory_resource.h>
#include <ciel/memory_resource_impl/get_default_resource.h>
#include <ciel/limits.h>
#include <new>
#include <ciel/utility.h>
#include <ciel/memory.h>

namespace ciel::pmr {

	template<class T = std::byte>
	class polymorphic_allocator {
	public:
		using value_type = T;

	private:
		memory_resource* mr;

	public:
		polymorphic_allocator() noexcept: mr(ciel::pmr::get_default_resource()) {}

		polymorphic_allocator(const polymorphic_allocator& other) = default;

		template<class U>
		polymorphic_allocator(const polymorphic_allocator<U>& other) noexcept:mr(other.resource()) {}

		polymorphic_allocator(memory_resource* r) : mr(r) {}

		polymorphic_allocator& operator=(const polymorphic_allocator&) = delete;

		[[nodiscard]] T* allocate(size_t n) {
			if (n > ciel::numeric_limits<size_t>::max() / sizeof(T)) {
				throw std::bad_array_new_length();
			}
			return static_cast<T*>(resource()->allocate(n * sizeof(T), alignof(T)));
		}

		void deallocate(T* p, size_t n) {
			resource()->deallocate(p, n * sizeof(T), alignof(T));
		}

		template<class U, class... Args>
		void construct(U* p, Args&& ... args) {
			ciel::user_alloc_construct_impl(typename uses_alloc_ctor<U, polymorphic_allocator&, Args...>::type(), p, *this, ciel::forward<Args>(args)...);
		}

		[[nodiscard]] void* allocate_bytes(size_t nbytes, size_t alignment = alignof(std::max_align_t)) {
			return resource()->allocate(nbytes, alignment);
		}

		void deallocate_bytes(void* p, size_t nbytes, size_t alignment = alignof(std::max_align_t)) {
			resource()->deallocate(p, nbytes, alignment);
		}

		template<class U>
		[[nodiscard]] U* allocate_object(size_t n = 1) {
			if (ciel::numeric_limits<size_t>::max() / sizeof(U) < n) {
				throw std::bad_array_new_length();
			}
			return static_cast<U*>(allocate_bytes(n * sizeof(U), alignof(U)));
		}

		template<class U>
		void deallocate_object(U* p, size_t n = 1) {
			deallocate_bytes(p, n * sizeof(U), alignof(U));
		}

		template<class U, class... CtorArgs>
		[[nodiscard]] U* new_object(CtorArgs&& ... ctor_args) {
			U* p = allocate_object<U>();
			try {
				construct(p, ciel::forward<CtorArgs>(ctor_args)...);
			} catch (...) {
				deallocate_object(p);
				throw;
			}
			return p;
		}

		template<class U>
		void delete_object(U* p) {
			ciel::allocator_traits<polymorphic_allocator>::destroy(*this, p);
			deallocate_object(p);
		}

		polymorphic_allocator select_on_container_copy_construction() const {
			return polymorphic_allocator();
		}

		memory_resource* resource() const {
			return mr;
		}

		friend bool operator==(const polymorphic_allocator& lhs, const polymorphic_allocator& rhs) noexcept {
			return *lhs.resource() == *rhs.resource();
		}

	};    //class polymorphic_allocator

	template<class T1, class T2>
	bool operator==(const polymorphic_allocator<T1>& lhs, const polymorphic_allocator<T2>& rhs) noexcept {
		return *lhs.resource() == *rhs.resource();
	}

}   //namespace ciel::pmr

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_POLYMORPHIC_ALLOCATOR_H_