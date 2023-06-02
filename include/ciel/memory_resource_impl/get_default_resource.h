#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_GET_DEFAULT_RESOURCE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_GET_DEFAULT_RESOURCE_H_

#include <ciel/memory_resource_impl/new_delete_resource.h>

namespace ciel::pmr {

	ciel::pmr::memory_resource* get_default_resource() noexcept {
		return ciel::pmr::new_delete_resource();
	}

}   // namespace ciel::pmr

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_RESOURCE_IMPL_GET_DEFAULT_RESOURCE_H_