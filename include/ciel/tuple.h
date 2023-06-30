#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_TUPLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_TUPLE_H_

#include <ciel/type_traits.h>
#include <ciel/utility.h>
#include <ciel/memory.h>

namespace ciel {

    // 多亏了 C++17 的折叠表达式，tuple 的实现比以前简单很多

    // 我看 libc++ tuple 的构造函数时，本来以为它的意思是，Up... 的长度短于 tuple 本身的 Tp... 时，它可以对前后两部分分别切出对应的类型和索引，然后传到 tuple_impl 那里对前半边值初始化，后半边默认初始化
    // 我还心想着这也太天才了，自己兴冲冲也写了一个
    // 然后我后来才看到这些构造函数全都要求 sizeof...(Tp) == sizeof...(Up)，所以这个写法貌似压根没有存在的意义= =

    template<class...>
    struct tuple;
    namespace tuple_details {
        template<size_t, class>
        struct tuple_leaf;
        template<class, class...>
        struct tuple_impl;
    }

    template<class... Types>
    struct tuple_size<tuple<Types...>> : ciel::integral_constant<size_t, sizeof...(Types)> {};

    template<size_t I, class Head, class... Tail>
    struct tuple_element<I, tuple<Head, Tail...>> : tuple_element<I - 1, tuple<Tail...>> {};

    template<class Head, class... Tail>
    struct tuple_element<0, tuple<Head, Tail...>> {
        using type = Head;
    };

    template<size_t I, class... Types>
    constexpr tuple_element_t<I, tuple<Types...>>& get(tuple<Types...>& t) noexcept {
        using type = tuple_element_t<I, tuple<Types...>>;
        return static_cast<type&>(static_cast<tuple_details::tuple_leaf<I, type>&>(t.base).get());
    }

    template<size_t I, class... Types>
    constexpr tuple_element_t<I, tuple<Types...>>&& get(tuple<Types...>&& t) noexcept {
        using type = tuple_element_t<I, tuple<Types...>>;
        return static_cast<type&&>(static_cast<tuple_details::tuple_leaf<I, type>&&>(t.base).get());
    }

    template<size_t I, class... Types>
    constexpr tuple_element_t<I, tuple<Types...>> const& get(const tuple<Types...>& t) noexcept {
        using type = tuple_element_t<I, tuple<Types...>>;
        return static_cast<const type&>(static_cast<const tuple_details::tuple_leaf<I, type>&>(t.base).get());
    }

    template<size_t I, class... Types>
    constexpr tuple_element_t<I, tuple<Types...>> const&& get(const tuple<Types...>&& t) noexcept {
        using type = tuple_element_t<I, tuple<Types...>>;
        return static_cast<const type&&>(static_cast<const tuple_details::tuple_leaf<I, type>&&>(t.base).get());
    }

    template<size_t I, class T, class Front, class... Rest>
    struct find_index_in_Args : find_index_in_Args<I + 1, T, Rest...> {};

    template<size_t I, class T, class... Rest>
    struct find_index_in_Args<I, T, T, Rest...> : ciel::integral_constant<size_t, I> {};

    template<size_t I, class T, class... Args>
    inline static constexpr size_t find_index_in_Args_v = find_index_in_Args<I, T, Args...>::value;

    template<class T, class... Types>
    constexpr T& get(tuple<Types...>& t) noexcept {
        constexpr size_t index = find_index_in_Args_v<0, T, Types...>;
        return static_cast<T&>(static_cast<tuple_details::tuple_leaf<index, T>&>(t.base).get());
    }

    template<class T, class... Types>
    constexpr T&& get(tuple<Types...>&& t) noexcept {
        constexpr size_t index = find_index_in_Args_v<0, T, Types...>;
        return static_cast<T&&>(static_cast<tuple_details::tuple_leaf<index, T>&&>(t.base).get());
    }

    template<class T, class... Types>
    constexpr const T& get(const tuple<Types...>& t) noexcept {
        constexpr size_t index = find_index_in_Args_v<0, T, Types...>;
        return static_cast<const T&>(static_cast<const tuple_details::tuple_leaf<index, T>&>(t.base).get());
    }

    template<class T, class... Types>
    constexpr const T&& get(const tuple<Types...>&& t) noexcept {
        constexpr size_t index = find_index_in_Args_v<0, T, Types...>;
        return static_cast<const T&&>(static_cast<const tuple_details::tuple_leaf<index, T>&&>(t.base).get());
    }

    namespace tuple_details {

        template<size_t I, class T>
        struct tuple_leaf {
            T t;

            tuple_leaf() = default;

            template<class U>
            explicit tuple_leaf(U&& u) : t(ciel::forward<U>(u)) {}

            void swap(tuple_leaf& other) {
                ciel::swap(t, other.t);
            }

            void swap(const tuple_leaf& other) const {
                ciel::swap(t, other);
            }

            T& get() {
                return t;
            }

            const T& get() const {
                return t;
            }
        };

        template<size_t I, class T>
            requires (ciel::is_empty_v<T> && !is_final_v<T>)    // 空基类优化版本
        struct tuple_leaf<I, T> : T {
            tuple_leaf() = default;

            template<class U>
            explicit tuple_leaf(U&& u) : T(ciel::forward<U>(u)) {}

            void swap(tuple_leaf& other) {
                ciel::swap(*this, other);
            }

            void swap(const tuple_leaf& other) const {
                ciel::swap(*this, other);
            }

            T& get() {
                return *this;
            }

            const T& get() const {
                return *this;
            }
        };

        template<class Index, class... Types>
        struct tuple_impl;

        template<size_t... I, class... Types>
        struct tuple_impl<ciel::index_sequence<I...>, Types...> : tuple_leaf<I, Types> ... {
            constexpr explicit tuple_impl() = default;

            template<class... UTypes>
            constexpr explicit tuple_impl(UTypes&& ... args) : tuple_leaf<I, Types>(ciel::forward<UTypes>(args))... {}

            template<class Tuple>
            constexpr explicit tuple_impl(Tuple&& t) : tuple_leaf<I, Types>(ciel::forward<tuple_element_t<I, ciel::remove_cvref_t<Tuple>>>(ciel::get<I>(t)))... {}

            constexpr tuple_impl(const tuple_impl&) = default;
            constexpr tuple_impl(tuple_impl&&) noexcept = default;

            void swap(tuple_impl& other) {
                (static_cast<tuple_leaf<I, Types>&>(*this).swap(static_cast<tuple_leaf<I, Types>&>(other)), ...);
            }

            void swap(const tuple_impl& other) const {
                (static_cast<const tuple_leaf<I, Types>&>(*this).swap(static_cast<const tuple_leaf<I, Types>&>(other)), ...);
            }

        };  // struct tuple_impl<ciel::index_sequence<I...>, Types...>

    }    // namespace tuple_details

    template<class...>
    struct take_first_arg;

    template<class Front, class... Rest>
    struct take_first_arg<Front, Rest...> {
        using type = Front;
    };

    template<class... Args>
    using take_first_arg_t = typename take_first_arg<Args...>::type;

    template<class>
    struct is_tuple : ciel::false_type {};

    template<class... Types>
    struct is_tuple<tuple<Types...>> : ciel::true_type {};

    template<class T>
    inline constexpr bool is_tuple_v = is_tuple<T>::value;

    template<class... Types>    // Types... 为空时会用下面的特化，所以主模板中 sizeof...(Types) >= 1 的要求已被抹去
    struct tuple {
        using base_type = tuple_details::tuple_impl<ciel::index_sequence_for<Types...>, Types...>;

        base_type base;

        // 此重载只有在 is_default_constructible_v<Ti> 对所有 i 都是 true 时才会参与重载决议
        // 此构造函数当且仅当 Types... 中至少有一个不可从 {} 复制列表初始化时才是 explicit 的
        constexpr explicit(!(ciel::is_implicitly_default_constructible_v<Types> && ...)) tuple() requires (ciel::is_default_constructible_v<Types> && ...) {}

        // 此重载只有在 is_copy_constructible_v<Ti> 对所有 i 都是 true时才会参与重载决议
        // 此构造函数当且仅当 is_convertible_v<const Ti&, Ti> 对至少一个 i 是 false 时才是 explicit 的
        constexpr explicit(!(ciel::is_convertible_v<const Types&, Types> && ...)) tuple(const Types& ... args) requires (ciel::is_copy_constructible_v<Types> && ...) : base(args...) {}

        // 此重载只有在
        //      sizeof...(Types) == sizeof...(UTypes)，且
        //      is_constructible_v<Ti, Ui> 对所有 i 都是 true，且
        //      令 D 为 remove_cvref_t<U0>，那么
        //          如果 sizeof...(Types) == 1，那么 D 不是 tuple，否则，
        //          如果 sizeof...(Types) == 2 或 sizeof...(Types) == 3，那么要么 D 不是 allocator_arg_t，要么 T0 是 allocator_arg_t
        // 时才会参与重载决议。
        // 此构造函数当且仅当 is_convertible_v<Ui, Ti> 对至少一个 i 是 false 时才是 explicit 的
        // TODO: 如果任何引用元素的初始化会绑定它到临时对象，那么定义此构造函数为被删除
        template<class... UTypes>
            requires (sizeof...(Types) == sizeof...(UTypes))
                && (ciel::is_constructible_v<Types, UTypes> && ...)
                && ((sizeof...(Types) == 1 && !is_tuple_v<ciel::remove_cvref_t<take_first_arg_t<UTypes...>>>)
                    || ((sizeof...(Types) == 2 || sizeof...(Types) == 3)
                    && (!ciel::is_same_v<ciel::remove_cvref_t<take_first_arg_t<UTypes...>>, ciel::allocator_arg_t> || ciel::is_same_v<take_first_arg_t<Types...>, ciel::allocator_arg_t>)))
        constexpr explicit(!(ciel::is_convertible_v<UTypes, Types> && ...)) tuple(UTypes&& ... args) : base(ciel::forward<UTypes>(args)...) {}

        // ---------------------------------------------
        // 令 FWD(other) 为 forward<decltype(other)>(other)，对于所有 i，以 get<i>(FWD(other)) 初始化 tuple 的第 i 个元素
        //
        // 此重载只有在
        //      sizeof...(Types) == sizeof...(UTypes)
        //      is_constructible_v<TI, decltype(get<i>(FWD(other)))> 对所有 i 都是 true，且以下之一成立：
        //          sizeof...(Types) 不是 1，或者
        //          （当 Types... 展开成 T 且 UTypes... 展开成 U 时） is_convertible_v<decltype(other), T>，is_constructible_v<T, decltype(other)> 和 is_same_v<T, U> 都是 false
        // 时才会参与重载决议
        template<class OtherTuple, class = ciel::remove_cvref_t<OtherTuple>>
        struct enable_ctor_from_UTypes_tuple : ciel::false_type {};

        template<class OtherTuple, class... UTypes>
            requires (sizeof...(Types) == sizeof...(UTypes)) && (ciel::is_constructible_v<Types, ciel::copy_cvref_t<OtherTuple, UTypes>> && ...)
            && (sizeof...(Types) != 1 || ((!ciel::is_convertible_v<OtherTuple, Types> && ...) && (!ciel::is_constructible_v<Types, OtherTuple> && ...) && (!ciel::is_same_v<Types, UTypes> && ...)))
        struct enable_ctor_from_UTypes_tuple<OtherTuple, tuple<UTypes...>> : ciel::true_type {};

        // 此构造函数当且仅当 is_convertible_v<decltype(get<I>(FWD(other))), TI> 对至少一个 i 是 false 时才是 explicit 的
        // TODO: 如果任何引用元素的初始化会绑定它到临时对象，那么定义此构造函数为被删除
        template<class... UTypes>
        constexpr explicit(!(ciel::is_convertible_v<UTypes&, Types> && ...)) tuple(tuple<UTypes...>& other) requires enable_ctor_from_UTypes_tuple<decltype(other)>::value : base(ciel::forward<decltype(other)>(other)) {}

        template<class... UTypes>
        constexpr explicit(!(ciel::is_convertible_v<const UTypes&, Types> && ...)) tuple(const tuple<UTypes...>& other) requires enable_ctor_from_UTypes_tuple<decltype(other)>::value : base(ciel::forward<decltype(other)>(other)) {}

        template<class... UTypes>
        constexpr explicit(!(ciel::is_convertible_v<UTypes&&, Types> && ...)) tuple(tuple<UTypes...>&& other) requires enable_ctor_from_UTypes_tuple<decltype(other)>::value : base(ciel::forward<decltype(other)>(other)) {}

        template<class... UTypes>
        constexpr explicit(!(ciel::is_convertible_v<const UTypes&&, Types> && ...)) tuple(const tuple<UTypes...>&& other) requires enable_ctor_from_UTypes_tuple<decltype(other)>::value : base(ciel::forward<decltype(other)>(other)) {}

        // ---------------------------------------------
        // pair 构造函数。以 p 中的对应元素初始化（拥有2个元素的）tuple 的每个元素。
        // 正式而言，令 FWD(p) 为 forward<decltype(p)>(p)，从 get<0>(FWD(p)) 构造第一个元素，从 get<1>(FWD(p)) 构造第二个元素。
        //
        // 此重载只有在
        //      sizeof...(Types) == 2
        //      is_constructible_v<T0, decltype(get<0>(FWD(p)))> 是 true
        //      is_constructible_v<T1, decltype(get<1>(FWD(p)))> 是 true
        // 时才会参与重载决议
        template<class OtherPair, class = ciel::remove_cvref_t<OtherPair>>
        struct enable_ctor_from_pair : ciel::false_type {};

        template<class OtherPair, class First, class Second>
            requires (sizeof...(Types) == 2) && (ciel::is_constructible_v<tuple_element_t<0, tuple>, ciel::copy_cvref_t<OtherPair, First>> && ciel::is_constructible_v<tuple_element_t<1, tuple>, ciel::copy_cvref_t<OtherPair, Second>>)
        struct enable_ctor_from_pair<OtherPair, ciel::pair<First, Second>> : ciel::true_type {};

        // 此构造函数当且仅当 is_convertible_v<decltype(get<0>(FWD(p))), T0> 或 is_convertible_v<decltype(get<1>(FWD(p))), T1> 是 false 时才是 explicit 的。
        // 如果任何引用元素的初始化会绑定它到临时对象，那么定义此构造函数为被删除
        template<class U1, class U2>
        constexpr explicit(!ciel::is_convertible_v<U1&, tuple_element_t<0, tuple>> || !ciel::is_convertible_v<U2&, tuple_element_t<1, tuple>>) tuple(ciel::pair<U1, U2>& p) requires enable_ctor_from_pair<decltype(p)>::value : base(ciel::forward<decltype(p)>(p)) {}

        template<class U1, class U2>
        constexpr explicit(!ciel::is_convertible_v<const U1&, tuple_element_t<0, tuple>> || !ciel::is_convertible_v<const U2&, tuple_element_t<1, tuple>>) tuple(const ciel::pair<U1, U2>& p) requires enable_ctor_from_pair<decltype(p)>::value : base(ciel::forward<decltype(p)>(p)) {}

        template<class U1, class U2>
        constexpr explicit(!ciel::is_convertible_v<U1&&, tuple_element_t<0, tuple>> || !ciel::is_convertible_v<U2&&, tuple_element_t<1, tuple>>) tuple(ciel::pair<U1, U2>&& p) requires enable_ctor_from_pair<decltype(p)>::value : base(ciel::forward<decltype(p)>(p)) {}

        template<class U1, class U2>
        constexpr explicit(!ciel::is_convertible_v<const U1&&, tuple_element_t<0, tuple>> || !ciel::is_convertible_v<const U2&&, tuple_element_t<1, tuple>>) tuple(const ciel::pair<U1, U2>&& p) requires enable_ctor_from_pair<decltype(p)>::value : base(ciel::forward<decltype(p)>(p)) {}

        // ---------------------------------------------

        // TODO
//		template<tuple-like UTuple>
//		constexpr tuple(UTuple&& u);

        constexpr tuple(const tuple&) = default;

        constexpr tuple(tuple&&) = default;

//        template<class Alloc>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a);
//
//        template<class Alloc>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, const Types& ... args);
//
//        template<class Alloc, class... UTypes>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, UTypes&& ... args);
//
//        template<class Alloc, class... UTypes>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, tuple<UTypes...>& other);
//
//        template<class Alloc, class... UTypes>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, const tuple<UTypes...>& other);
//
//        template<class Alloc, class... UTypes>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, tuple<UTypes...>&& other);
//
//        template<class Alloc, class... UTypes>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, const tuple<UTypes...>&& other);
//
//        template<class Alloc, class U1, class U2>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, ciel::pair<U1, U2>& p);
//
//        template<class Alloc, class U1, class U2>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, const ciel::pair<U1, U2>& p);
//
//        template<class Alloc, class U1, class U2>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, ciel::pair<U1, U2>&& p);
//
//        template<class Alloc, class U1, class U2>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, const ciel::pair<U1, U2>&& p);
//
//        // TODO
////		template<class Alloc, tuple-like UTuple>
////		constexpr tuple(ciel::allocator_arg_t, const Alloc& a, UTuple&& u);
//
//        template<class Alloc>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, const tuple& other);
//
//        template<class Alloc>
//        constexpr tuple(ciel::allocator_arg_t, const Alloc& a, tuple&& other);

        constexpr void swap(tuple& other) noexcept((ciel::is_nothrow_swappable_v<Types> && ...)) requires (ciel::is_swappable_v<Types> && ...) {
            base.swap(other.base);
        }

        constexpr void swap(const tuple& other) const noexcept((ciel::is_nothrow_swappable_v<const Types> && ...)) requires (ciel::is_swappable_v<const Types> && ...) {
            base.swap(other.base);
        }

    };    // struct tuple

    template<>
    struct tuple<> {
        constexpr void swap(tuple& other) noexcept {}
        constexpr void swap(const tuple& other) const noexcept {}

    };  // struct tuple<>

    template<class... UTypes>
    tuple(UTypes...) -> tuple<UTypes...>;

    template<class T1, class T2>
    tuple(ciel::pair<T1, T2>) -> tuple<T1, T2>;

    template<class Alloc, class... UTypes>
    tuple(ciel::allocator_arg_t, Alloc, UTypes...) -> tuple<UTypes...>;

    template<class Alloc, class T1, class T2>
    tuple(ciel::allocator_arg_t, Alloc, ciel::pair<T1, T2>) -> tuple<T1, T2>;

    template<class Alloc, class... UTypes>
    tuple(ciel::allocator_arg_t, Alloc, tuple<UTypes...>) -> tuple<UTypes...>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_TUPLE_H_