#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_UNDERLYING_TYPE_H
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_UNDERLYING_TYPE_H

#include <ciel/type_traits_impl/is_enum.h>

namespace ciel {

    namespace underlying_type_details {

        template<class T, bool = ciel::is_enum_v<T>>
        struct underlying_type_helper {};

        template<class T>
        struct underlying_type_helper<T, true> {
            using type = __underlying_type (T);
        };
    }

    template<class T>
    struct underlying_type : underlying_type_details::underlying_type_helper<T> {};

    template<class T>
    using underlying_type_t = typename underlying_type<T>::type;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_UNDERLYING_TYPE_H