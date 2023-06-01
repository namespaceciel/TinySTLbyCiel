#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_SET_DEFAULT_RESOURCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_SET_DEFAULT_RESOURCE_H_

#include <ciel/memory_resource_impl/memory_resource.h>

namespace ciel {
	//若 r 非空，则设置默认 memory_resource 指针为 r
	//否则，设置默认 memory_resource 指针为 std::pmr::new_delete_resource()

	ciel::pmr::memory_resource* set_default_resource(ciel::pmr::memory_resource* r) noexcept;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_SET_DEFAULT_RESOURCE_H_