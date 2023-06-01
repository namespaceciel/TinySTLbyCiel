#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_NULL_MEMORY_RESOURCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_NULL_MEMORY_RESOURCE_H_

#include <ciel/memory_resource_impl/memory_resource.h>
#include <new>

namespace ciel::pmr {
	//返回指针 p ，指向导出自 pmr::memory_resource 类型的静态存储期对象，它拥有下列属性：
	//		其 allocate() 函数始终抛出 std::bad_alloc ；
	//		其 deallocate() 函数无效果；
	//		对于任何 memory_resource r ， p->is_equal(r) 返回 &r == p 。
	//		每次调用此函数都返回相同值。

	namespace null_memory_resource_details {

		class null_memory_resource : public ciel::pmr::memory_resource {
		public:
			virtual void* do_allocate(size_t bytes, size_t alignment) override {
				throw std::bad_alloc();
			}

			virtual void do_deallocate(void* p, size_t bytes, size_t alignment) override {}

			virtual bool do_is_equal(const ciel::pmr::memory_resource& other) const noexcept override {
				return &other == this;
			}
		};
	}

	ciel::pmr::memory_resource* null_memory_resource() noexcept {
		static null_memory_resource_details::null_memory_resource res;
		return &res;
	}

}   //namespace ciel::pmr

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_NULL_MEMORY_RESOURCE_H_
