#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ALIGNED_STORAGE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ALIGNED_STORAGE_H_

#include <cstddef>

namespace ciel {

    template<size_t Len, size_t Align>
    struct aligned_storage {
        struct type {
            alignas(Align) unsigned char data[Len];
        };
    };

    template<size_t Len, size_t Align>
    using aligned_storage_t = typename aligned_storage<Len, Align>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_ALIGNED_STORAGE_H_