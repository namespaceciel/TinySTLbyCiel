#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_MEMORY_RESOURCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_MEMORY_RESOURCE_H_

#include <cstddef>

namespace ciel::pmr {

	class memory_resource {
	public:
		[[nodiscard]] void* allocate(size_t bytes, size_t alignment = alignof(std::max_align_t)) {
			return do_allocate(bytes, alignment);
		}

		void deallocate(void* p, size_t bytes, size_t alignment = alignof(std::max_align_t)) {
			do_deallocate(p, bytes, alignment);
		}

		bool is_equal(const memory_resource& other) const noexcept {
			return do_is_equal(other);
		}

		virtual void* do_allocate(size_t bytes, size_t alignment) = 0;

		virtual void do_deallocate(void* p, size_t bytes, size_t alignment) = 0;

		//二个 memory_resource 比较相等，当且仅当从一个 memory_resource 分配的内存能从另一个解分配，反之亦然
		virtual bool do_is_equal(const memory_resource& other) const noexcept = 0;

	};    //class memory_resource

	bool operator==(const memory_resource& a, const memory_resource& b) noexcept {
		return &a == &b || a.is_equal(b);
	}

}   //namespace ciel::pmr

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_MEMORY_RESOURCE_H_