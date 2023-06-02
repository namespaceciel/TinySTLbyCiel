#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_NEW_DELETE_RESOURCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_NEW_DELETE_RESOURCE_H_

#include <ciel/memory_resource_impl/memory_resource.h>
#include <new>

namespace ciel::pmr {
	// 返回指针 p ，指向导出自 pmr::memory_resource 类型的静态存储期对象，它拥有下列属性：
	// 		其 allocate() 函数用 ::operator new 分配内存；
	// 		其 deallocate() 函数用 ::operator delete 解分配内存；
	// 		对于任何 memory_resource r ， p->is_equal(r) 返回 &r == p.
	// 		每次调用此函数时返回相同值。

	namespace new_delete_resource_details {

		class new_delete_resource : public ciel::pmr::memory_resource {
		public:
			virtual void* do_allocate(size_t bytes, size_t alignment) override {
				return ::operator new(bytes, std::align_val_t(alignment));
			}

			virtual void do_deallocate(void* p, size_t bytes, size_t alignment) override {
				::operator delete(p, bytes, std::align_val_t(alignment));
			}

			virtual bool do_is_equal(const ciel::pmr::memory_resource& other) const noexcept override {
				return &other == this;
			}
		};
	}

	ciel::pmr::memory_resource* new_delete_resource() noexcept {
		static new_delete_resource_details::new_delete_resource res;
		return &res;
	}

}   // namespace ciel::pmr

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_NEW_DELETE_RESOURCE_H_