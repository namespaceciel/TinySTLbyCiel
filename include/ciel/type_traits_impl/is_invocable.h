#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_INVOCABLE_H
#define TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_INVOCABLE_H

#include <ciel/type_traits_impl/enable_if.h>
#include <ciel/type_traits_impl/is_function.h>
#include <ciel/type_traits_impl/is_base_of.h>
#include <ciel/type_traits_impl/decay.h>
#include <ciel/type_traits_impl/../utility_impl/forward.h>
#include <ciel/type_traits_impl/is_member_pointer.h>
#include <ciel/utility_impl/declval.h>

namespace ciel {
/*
    namespace {
        template<class Fn, class... ArgTypes>
        concept is_invocable_well_formed=requires{ detail::INVOKE(declval<Fn>(), declval<ArgTypes>()...); };

        template<class Fn, class... ArgTypes>
        requires is_invocable_well_formed<Fn, ArgTypes...>
        true_type is_invocable_helper(int);

        template<class Fn, class... ArgTypes>
        false_type is_invocable_helper(...);

        template<class R, class Fn, class... ArgTypes>
        concept is_invocable_r_well_formed=requires{ detail::INVOKE<R>(declval<Fn>(), declval<ArgTypes>()...); };

        template<class R, class Fn, class... ArgTypes>
        requires is_invocable_r_well_formed<R, Fn, ArgTypes...>
        true_type is_invocable_r_helper(int);

        template<class R, class Fn, class... ArgTypes>
        false_type is_invocable_r_helper(...);

        template<class Fn, class... ArgTypes>
        concept is_nothrow_invocable_well_formed=requires{{ detail::INVOKE(declval<Fn>(), declval<ArgTypes>()...) }noexcept; };

        template<class Fn, class... ArgTypes>
        requires is_nothrow_invocable_well_formed<Fn, ArgTypes...>
        true_type is_nothrow_invocable_helper(int);

        template<class Fn, class... ArgTypes>
        false_type is_nothrow_invocable_helper(...);

        template<class R, class Fn, class... ArgTypes>
        concept is_nothrow_invocable_r_well_formed=requires{ detail::INVOKE<R>(declval<Fn>(), declval<ArgTypes>()...); };

        template<class R, class Fn, class... ArgTypes>
        requires is_nothrow_invocable_r_well_formed<R, Fn, ArgTypes...>
        true_type is_nothrow_invocable_r_helper(int);

        template<class R, class Fn, class... ArgTypes>
        false_type is_nothrow_invocable_r_helper(...);
    }

    template<class Fn, class... ArgTypes>
    struct is_invocable : decltype(is_invocable_helper<Fn, ArgTypes...>(0)) {};

    template<class R, class Fn, class... ArgTypes>
    struct is_invocable_r : decltype(is_invocable_r_helper<R, Fn, ArgTypes...>(0)) {};

    template<class Fn, class... ArgTypes>
    struct is_nothrow_invocable : decltype(is_nothrow_invocable_helper<Fn, ArgTypes...>(0)) {};

    template<class R, class Fn, class... ArgTypes>
    struct is_nothrow_invocable_r : decltype(is_nothrow_invocable_r_helper<R, Fn, ArgTypes...>(0)) {};

    template<class Fn, class... ArgTypes>
    inline constexpr bool is_invocable_v = is_invocable < Fn, ArgTypes...>::value;

    template<class R, class Fn, class... ArgTypes>
    inline constexpr bool is_invocable_r_v = is_invocable_r < R, Fn, ArgTypes...>::value;

    template<class Fn, class... ArgTypes>
    inline constexpr bool is_nothrow_invocable_v = is_nothrow_invocable<Fn, ArgTypes...>::value;

    template<class R, class Fn, class... ArgTypes>
    inline constexpr bool is_nothrow_invocable_r_v = is_nothrow_invocable_r<R, Fn, ArgTypes...>::value;
*/
}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_TYPE_TRAITS_IMPL_IS_INVOCABLE_H