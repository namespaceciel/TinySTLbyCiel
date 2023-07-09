#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ANY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ANY_H_

#include <any>  // std::bad_any_cast
#include <ciel/utility.h>
#include <ciel/memory.h>

namespace ciel {

    namespace any_details {
        using buffer = ciel::aligned_storage_t<sizeof(void*) * 3, ciel::alignment_of_v<void*>>;

        template<class T>
        using is_small_object = ciel::bool_constant<sizeof(T) <= sizeof(buffer) && ciel::alignment_of_v<buffer> % ciel::alignment_of_v<T> == 0 && ciel::is_nothrow_move_constructible_v<T>>;

        enum class Action {
            Destroy, Copy, Move, Get, TypeInfo
        };

        // handler 需要完整的 any 类型，定义放最下面了
        template<class> struct small_handler;
        template<class> struct large_handler;

        template<class T>
        using handler_type = ciel::conditional_t<is_small_object<T>::value, small_handler<T>, large_handler<T>>;

    }   // namespace any_details

    class any {
    private:
        using action_type = any_details::Action;

        using handle_func_ptr = void* (*)(action_type, any const*, any*, const std::type_info*);

        union storage {
            constexpr storage() : ptr(nullptr) {}
            void*  ptr;
            any_details::buffer buf;
        };

        handle_func_ptr h;
        storage s;

        void* call(action_type a, any* other = nullptr, const std::type_info* info = nullptr) {
            return h(a, this, other, info);
        }

        void* call(action_type a, any* other = nullptr, const std::type_info* info = nullptr) const {
            return h(a, this, other, info);
        }

        template<class> friend struct any_details::small_handler;
        template<class> friend struct any_details::large_handler;
        template<class V> friend V* any_cast(any*) noexcept;

    public:
        constexpr any() noexcept : h(nullptr) {}

        any(const any& other) : h(nullptr) {
            if (other.h) {
                other.call(action_type::Copy, this);
            }
        }

        any(any&& other) noexcept : h(nullptr) {
            if (other.h) {
                other.call(action_type::Move, this);
            }
        }

        // 此重载只有在 decay_t<ValueType> 既非与 any 相同类型，亦非 in_place_type_t 的特化，且 is_copy_constructible_v<decay_t<ValueType>> 为 true 时才会参与重载决议
        template<class ValueType, class T = ciel::decay_t<ValueType>>
            requires (!ciel::is_same_v<T, any> && !ciel::is_inplace_type<T>::value && ciel::is_copy_constructible_v<T>)
        any(ValueType&& value) {
            any_details::handler_type<T>::create(*this, ciel::forward<ValueType>(value));
        }

        // 此重载只有在 is_constructible_v<decay_t<ValueType>, Args...> 与 is_copy_constructible_v<decay_t<ValueType>> 皆为 true 时才会参与重载决议
        template<class ValueType, class... Args, class T = ciel::decay_t<ValueType>>
            requires (ciel::is_constructible_v<T, Args...> && ciel::is_copy_constructible_v<T>)
        explicit any(ciel::in_place_type_t<ValueType>, Args&& ... args) {
            any_details::handler_type<T>::create(*this, ciel::forward<Args>(args)...);
        }

        // 此重载只有在 is_constructible_v<decay_t<ValueType>, std::initializer_list<U>&, Args...> 与 is_copy_constructible_v<decay_t<ValueType>> 皆为 true 时才会参与重载决议
        template<class ValueType, class U, class... Args, class T = ciel::decay_t<ValueType>>
            requires (ciel::is_constructible_v<T, std::initializer_list<U>&, Args...> && ciel::is_copy_constructible_v<T>)
        explicit any(ciel::in_place_type_t<ValueType>, std::initializer_list<U> il, Args&& ... args) {
            any_details::handler_type<T>::create(*this, il, ciel::forward<Args>(args)...);
        }

        any& operator=(const any& rhs) {
            any(rhs).swap(*this);
            return *this;
        }

        any& operator=(any&& rhs) noexcept {
            any(ciel::move(rhs)).swap(*this);
            return *this;
        }

        // 此重载只有在 decay_t<ValueType> 与 any 不是同一类型且 is_copy_constructible_v<decay_t<ValueType>> 为 true 时才会参与重载决议
        template<class ValueType, class T = ciel::decay_t<ValueType>>
            requires (!ciel::is_same_v<T, any> && ciel::is_copy_constructible_v<T>)
        any& operator=(ValueType&& rhs) {
            any(ciel::forward<ValueType>(rhs)).swap(*this);
            return *this;
        }

        ~any() {
            reset();
        }

        // 此重载只有在 is_constructible_v<decay_t<ValueType>, Args...> 与 is_copy_constructible_v<decay_t<ValueType>> 皆为 true 时才会参与重载决议
        template<class ValueType, class... Args, class T = ciel::decay_t<ValueType>>
            requires (ciel::is_constructible_v<T, Args...> && ciel::is_copy_constructible_v<T>)
        T& emplace(Args&& ... args) {
            reset();
            return any_details::handler_type<T>::create(*this, ciel::forward<Args>(args)...);
        }

        // 此重载只有在 is_constructible_v<decay_t<ValueType>, std::initializer_list<U>&, Args...> 与 is_copy_constructible_v<decay_t<ValueType>> 皆为 true 时才会参与重载决议
        template<class ValueType, class U, class... Args, class T = ciel::decay_t<ValueType>>
            requires (ciel::is_constructible_v<T, std::initializer_list<U>&, Args...> && ciel::is_copy_constructible_v<T>)
        T& emplace(std::initializer_list<U> il, Args&& ... args) {
            reset();
            return any_details::handler_type<T>::create(*this, il, ciel::forward<Args>(args)...);
        }

        void reset() noexcept {
            if (h) {
                call(action_type::Destroy);
            }
        }

        void swap(any& other) noexcept {
            if (this == &other)
                return;
            if (h && other.h) {
                any tmp;
                other.call(action_type::Move, &tmp);
                this->call(action_type::Move, &other);
                tmp.call(action_type::Move, this);
            } else if (h) {
                this->call(action_type::Move, &other);
            } else if (other.h) {
                other.call(action_type::Move, this);
            }
        }

        [[nodiscard]] bool has_value() const noexcept {
            return h != nullptr;
        }

        [[nodiscard]] const std::type_info& type() const noexcept {
            if (h) {
                return *static_cast<const std::type_info*>(this->call(action_type::TypeInfo));
            } else {
                return typeid(void);
            }
        }

    };  // class any

    void swap(any& lhs, any& rhs) noexcept {
        lhs.swap(rhs);
    }

    template<class T, class U = ciel::remove_cvref_t<T>>
        requires ciel::is_constructible_v<T, const U&>
    T any_cast(const any& operand) {
        auto res = any_cast<ciel::add_const_t<U>>(&operand);
        if (res == nullptr) {
            throw std::bad_any_cast();
        }
        return static_cast<T>(*res);
    }

    template<class T, class U = ciel::remove_cvref_t<T>>
        requires ciel::is_constructible_v<T, U&>
    T any_cast(any& operand) {
        auto res = any_cast<U>(&operand);
        if (res == nullptr) {
            throw std::bad_any_cast();
        }
        return static_cast<T>(*res);
    }

    template<class T, class U = ciel::remove_cvref_t<T>>
        requires ciel::is_constructible_v<T, U>
    T any_cast(any&& operand) {
        auto res = any_cast<U>(&operand);
        if (res == nullptr) {
            throw std::bad_any_cast();
        }
        return static_cast<T>(ciel::move(*res));
    }

    template<class T>
    const T* any_cast(const any* operand) noexcept {
        return any_cast<T>(const_cast<any*>(operand));
    }

    template<class T>
    T* any_cast(any* operand) noexcept {
        using action_type = any_details::Action;
        if (operand && operand->h) {
            void* p = operand->call(action_type::Get, nullptr, &typeid(T));
            return static_cast<T*>(p);
        }
        return nullptr;
    }

    template<class T, class... Args>
    any make_any(Args&& ... args) {
        return any(ciel::in_place_type<T>, ciel::forward<Args>(args)...);
    }

    template<class T, class U, class... Args>
    any make_any(std::initializer_list<U> il, Args&& ... args) {
        return any(ciel::in_place_type<T>, il, ciel::forward<Args>(args)...);
    }

    namespace any_details {
        template<class T>
        struct small_handler {
            static void* handle(Action act, const any* _this, any* other, const std::type_info* info) {
                switch(act) {
                    case Action::Destroy:
                        destroy(const_cast<any&>(*_this));
                        return nullptr;
                    case Action::Copy:
                        copy(*_this, *other);
                        return nullptr;
                    case Action::Move:
                        move(const_cast<any&>(*_this), *other);
                        return nullptr;
                    case Action::Get:
                        return get(const_cast<any&>(*_this), info);
                    case Action::TypeInfo:
                        return type_info();
                }
            }

            template<class... Args>
            static T& create(any& dest, Args&&... args) {
                T* res = static_cast<T*>(static_cast<void*>(&dest.s.buf));
                ciel::construct_at(res, ciel::forward<Args>(args)...);
                dest.h = &handle;
                return *res;
            }

        private:
            static void destroy(any& _this) {
                T* p = static_cast<T*>(static_cast<void*>(&_this.s.buf));
                ciel::destroy_at(p);
                _this.h = nullptr;
            }

            static void copy(const any& _this, any& dest) {
                create(dest, *static_cast<const T*>(static_cast<const void*>(&_this.s.buf)));
            }

            static void move(any& _this, any& dest) {
                create(dest, ciel::move(*static_cast<T*>(static_cast<void*>(&_this.s.buf))));
                destroy(_this);
            }

            static void* get(any& _this, const std::type_info* info) {
                if (info && *info == typeid(T)) {
                    return static_cast<void*>(&_this.s.buf);
                }
                return nullptr;
            }

            static void* type_info(){
                return const_cast<void*>(static_cast<const void*>(&typeid(T)));
            }

        };  // struct small_handler

        template<class T>
        struct large_handler {
            static void* handle(Action act, const any* _this, any* other, const std::type_info* info) {
                switch(act) {
                    case Action::Destroy:
                        destroy(const_cast<any&>(*_this));
                        return nullptr;
                    case Action::Copy:
                        copy(*_this, *other);
                        return nullptr;
                    case Action::Move:
                        move(const_cast<any&>(*_this), *other);
                        return nullptr;
                    case Action::Get:
                        return get(const_cast<any&>(*_this), info);
                    case Action::TypeInfo:
                        return type_info();
                }
            }

            template<class... Args>
            static T& create(any& dest, Args&&... args) {
                ciel::allocator<T> a;
                using alloc_traits = ciel::allocator_traits<ciel::allocator<T>>;
                T* res = alloc_traits::allocate(a, 1);
                try {
                    alloc_traits::construct(a, res, ciel::forward<Args>(args)...);
                } catch(...) {
                    alloc_traits::deallocate(a, res, 1);
                    throw;
                }
                dest.s.ptr = res;
                dest.h = &handle;
                return *res;
            }

        private:
            static void destroy(any& _this) {
                ciel::allocator<T> a;
                using alloc_traits = ciel::allocator_traits<ciel::allocator<T>>;
                T* p = static_cast<T*>(_this.s.ptr);
                alloc_traits::destroy(a, p);
                alloc_traits::deallocate(a, p, 1);
                _this.h = nullptr;
            }

            static void copy(const any& _this, any& dest) {
                create(dest, *static_cast<const T*>(_this.s.ptr));
            }

            static void move(any& _this, any& dest) {
                dest.s.ptr = _this.s.ptr;
                dest.h = &handle;
                _this.h = nullptr;
            }

            static void* get(any& _this, const std::type_info* info) {
                if (info && *info == typeid(T)) {
                    return static_cast<void*>(_this.s.ptr);
                }
                return nullptr;
            }

            static void* type_info(){
                return const_cast<void*>(static_cast<const void*>(&typeid(T)));
            }

        };  // struct large_handler

    }   // namespace any_details

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ANY_H_