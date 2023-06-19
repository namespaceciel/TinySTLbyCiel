#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNIQUE_PTR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNIQUE_PTR_H_

#include <ciel/memory_impl/default_delete.h>
#include <ciel/memory_impl/allocator_traits.h>
#include <ciel/functional.h>

namespace ciel {

	namespace unique_ptr_details {
		template<class Deleter>
		struct unique_ptr_constructor_helper {
			static_assert(!ciel::is_reference_v<Deleter>, "ciel::unique_ptr 的删除器不能为右值引用");

			inline static constexpr int tag = 0;
		};

		template<class Deleter>
		struct unique_ptr_constructor_helper<Deleter&> {
			inline static constexpr int tag = 1;
		};

		template<class Deleter>
		struct unique_ptr_constructor_helper<const Deleter&> {
			inline static constexpr int tag = 2;
		};
	}

	template<class T, class Deleter = ciel::default_delete<T>>
	class unique_ptr {
	public:
		using element_type = T;
		using deleter_type = Deleter;
		using pointer = typename allocator_traits_details::has_pointer<element_type, deleter_type>::type;    // pointer 可以为智能指针，但必须满足可空指针 (NullablePointer)

	private:
		pointer ptr;
		[[no_unique_address]] deleter_type dlt;

	public:
		constexpr unique_ptr() noexcept requires (ciel::is_default_constructible_v<deleter_type> && !ciel::is_pointer_v<deleter_type>): ptr(nullptr), dlt() {}

		constexpr unique_ptr(nullptr_t) noexcept requires (ciel::is_default_constructible_v<deleter_type> && !ciel::is_pointer_v<deleter_type>): ptr(nullptr), dlt() {}

		constexpr explicit unique_ptr(pointer p) noexcept requires (ciel::is_default_constructible_v<deleter_type> && !ciel::is_pointer_v<deleter_type>): ptr(p), dlt() {}

		// 第一组情况
		template<class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 0>>
			requires ciel::is_nothrow_copy_constructible_v<D>
		unique_ptr(pointer p, const A& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 0>>
			requires ciel::is_nothrow_move_constructible_v<D>
		unique_ptr(pointer p, A&& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		// 第二组情况
		template<class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 1>>
		unique_ptr(pointer p, A& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 1>>
		unique_ptr(pointer p, A&& d) = delete;

		// 第三组情况
		template<class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 2>>
		unique_ptr(pointer p, const A& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 2>>
		unique_ptr(pointer p, const A&& d) = delete;

		// TODO: 若 Deleter 不是引用类型，则要求它为不抛出可移动构造 (MoveConstructible) （若 Deleter 是引用，则 get_deleter() 和 u.get_deleter() 在移动构造后引用相同值）
		constexpr unique_ptr(unique_ptr&& u) noexcept requires ciel::is_move_constructible_v<deleter_type>: ptr(u.release()), dlt(ciel::forward<deleter_type>(u.get_deleter())) {}

		template<class U, class E>
			requires(ciel::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && !ciel::is_array_v<U> && ((ciel::is_reference_v<deleter_type> && ciel::is_same_v<deleter_type, E>) || (!ciel::is_reference_v<deleter_type> && ciel::is_convertible_v<E, deleter_type>)))
		constexpr unique_ptr(unique_ptr<U, E>&& u) noexcept : ptr(u.release()), dlt(ciel::forward<deleter_type>(u.get_deleter())) {}

		~unique_ptr() {
			reset();
		}

		// TODO:
		// 若 Deleter 不是引用类型，则要求它为不抛出可移动赋值 (MoveAssignable)
		// 若 Deleter 是引用类型，则要求 remove_reference<Deleter>::type 为不抛出可复制赋值 (CopyAssignable)
		unique_ptr& operator=(unique_ptr&& r) noexcept requires ciel::is_move_assignable_v<deleter_type> {
			reset(r.release());
			dlt = ciel::forward<deleter_type>(r.get_deleter());
			return *this;
		}

		template<class U, class E>
			requires (!ciel::is_array_v<U> && ciel::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && ciel::is_assignable_v<deleter_type&, E&&>)
		unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept {
			reset(r.release());
			dlt = ciel::forward<E>(r.get_deleter());
			return *this;
		}

		unique_ptr& operator=(nullptr_t) noexcept {
			reset();
			return *this;
		}

		pointer release() noexcept {
			pointer res = ptr;
			ptr = pointer();
			return res;
		}

		// 关于这里为什么是三步：如果 pointer 是智能指针类型，在前两步拷贝构造与赋值中如果抛出异常，ptr 仍然能保有指针的所有权（当然这也需要 pointer 类自身满足异常安全）
		// 所以如果在 try 块中执行 old.reset(new); 抛出异常的话，old 依然是完整的，new 则需要在 catch 块中由调用方处理
		// 并且，如果 ptr 指向的是拥有自身的对象，以常规想法的先销毁后赋值会使得销毁时调用析构函数而析构函数又调用 reset 陷入死循环（见 memory_test）
		void reset(pointer p = pointer()) noexcept {
			pointer tmp = ptr;
			ptr = p;
			if (tmp) {
				get_deleter()(tmp);
			}
		}

		void swap(unique_ptr& other) noexcept {
			ciel::swap(ptr, other.ptr);
			ciel::swap(dlt, other.dlt);
		}

		pointer get() const noexcept {
			return ptr;
		}

		deleter_type& get_deleter() noexcept {
			return dlt;
		}

		const deleter_type& get_deleter() const noexcept {
			return dlt;
		}

		explicit operator bool() const noexcept {
			return get() != nullptr;
		}

		typename ciel::add_lvalue_reference_t<T> operator*() const noexcept(noexcept(*ciel::declval<pointer>())) {
			return *get();
		}

		pointer operator->() const noexcept {
			return get();
		}

	};    // class unique_ptr

	template<class T, class Deleter>
	class unique_ptr<T[], Deleter> {
	public:
		using element_type = T;
		using deleter_type = Deleter;
		using pointer = typename allocator_traits_details::has_pointer<element_type, deleter_type>::type;    // pointer 可以为智能指针，但必须满足可空指针 (NullablePointer)，即需要实现 operator bool()

	private:
		pointer ptr;
		[[no_unique_address]] deleter_type dlt;

	public:
		constexpr unique_ptr() noexcept requires (ciel::is_default_constructible_v<deleter_type> && !ciel::is_pointer_v<deleter_type>): ptr(), dlt() {}

		constexpr unique_ptr(nullptr_t) noexcept requires (ciel::is_default_constructible_v<deleter_type> && !ciel::is_pointer_v<deleter_type>): ptr(), dlt() {}

		template<class U>
		explicit unique_ptr(U p) noexcept requires (ciel::is_default_constructible_v<deleter_type> && !ciel::is_pointer_v<deleter_type>): ptr(p), dlt() {}

		// 第一组情况
		template<class U, class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 0>>
			requires ciel::is_nothrow_copy_constructible_v<D>
		unique_ptr(U p, const A& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class U, class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 0>>
			requires ciel::is_nothrow_move_constructible_v<D>
		unique_ptr(U p, A&& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		// 第二组情况
		template<class U, class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 1>>
		unique_ptr(U p, A& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class U, class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 1>>
		unique_ptr(U p, A&& d) = delete;

		// 第三组情况
		template<class U, class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 2>>
		unique_ptr(U p, const A& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class U, class D = deleter_type, class A = ciel::remove_cvref_t<D>, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 2>>
		unique_ptr(U p, const A&& d) = delete;

		// TODO: 若 Deleter 不是引用类型，则要求它为不抛出可移动构造 (MoveConstructible) （若 Deleter 是引用，则 get_deleter() 和 u.get_deleter() 在移动构造后引用相同值）
		constexpr unique_ptr(unique_ptr&& u) noexcept requires ciel::is_move_constructible_v<deleter_type>: ptr(u.release()), dlt(ciel::forward<deleter_type>(u.get_deleter())) {}

		template<class U, class E>
			requires (ciel::is_array_v<U>
				&& ciel::is_same_v<pointer, element_type*>
				&& ciel::is_same_v<typename unique_ptr<U, E>::pointer, typename unique_ptr<U, E>::element_type*>
				&& ciel::is_convertible_v<typename unique_ptr<U, E>::element_type(*)[], element_type(*)[]>
				&& ((ciel::is_reference_v<deleter_type> && ciel::is_same_v<deleter_type, E>) || (!ciel::is_reference_v<deleter_type> && ciel::is_convertible_v<E, deleter_type>)))
		constexpr unique_ptr(unique_ptr<U, E>&& u) noexcept : ptr(u.release()), dlt(ciel::forward<deleter_type>(u.get_deleter())) {}

		~unique_ptr() {
			reset();
		}

		// TODO:
		// 若 Deleter 不是引用类型，则要求它为不抛出可移动赋值 (MoveAssignable)
		// 若 Deleter 是引用类型，则要求 remove_reference<Deleter>::type 为不抛出可复制赋值 (CopyAssignable)
		unique_ptr& operator=(unique_ptr&& r) noexcept requires ciel::is_move_assignable_v<deleter_type> {
			reset(r.release());
			dlt = ciel::forward<deleter_type>(r.get_deleter());
			return *this;
		}

		template<class U, class E>
			requires (ciel::is_array_v<U>
				&& ciel::is_same_v<pointer, element_type*>
				&& ciel::is_same_v<typename unique_ptr<U, E>::pointer, typename unique_ptr<U, E>::element_type*>
				&& ciel::is_convertible_v<typename unique_ptr<U, E>::element_type(*)[], element_type(*)[]>
				&& ciel::is_assignable_v<deleter_type&, E&&>)
		unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept {
			reset(r.release());
			dlt = ciel::forward<E>(r.get_deleter());
			return *this;
		}

		unique_ptr& operator=(nullptr_t) noexcept {
			reset();
			return *this;
		}

		pointer release() noexcept {
			pointer res = ptr;
			ptr = pointer();
			return res;
		}

		template<class U>
			requires (ciel::is_same_v<U, pointer> || (ciel::is_same_v<pointer, element_type*> && ciel::is_pointer_v<U> && ciel::is_convertible_v<ciel::remove_pointer_t<U>(*)[], element_type(*)[]>))
		void reset(U p) noexcept {
			pointer tmp = ptr;
			ptr = p;
			if (tmp) {
				get_deleter()(tmp);
			}
		}

		void reset(nullptr_t p = nullptr) noexcept {
			reset(pointer());
		}

		void swap(unique_ptr& other) noexcept {
			ciel::swap(ptr, other.ptr);
			ciel::swap(dlt, other.dlt);
		}

		pointer get() const noexcept {
			return ptr;
		}

		deleter_type& get_deleter() noexcept {
			return dlt;
		}

		const deleter_type& get_deleter() const noexcept {
			return dlt;
		}

		explicit operator bool() const noexcept {
			return get() != nullptr;
		}

		T& operator[](size_t i) const {
			return get()[i];
		}

	};    // class unique_ptr<T[], Deleter>

	template<class T1, class D1, class T2, class D2>
	bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
		return x.get() == y.get();
	}

	template<class T1, class D1, class T2, class D2>
	bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
		return ciel::less<ciel::common_type_t<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer>>()(x.get(), y.get());
	}

	template<class T1, class D1, class T2, class D2>
	bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
		return !(y < x);
	}

	template<class T1, class D1, class T2, class D2>
	bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
		return y < x;
	}

	template<class T1, class D1, class T2, class D2>
	bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
		return !(x < y);
	}

	template<class T1, class D1, class T2, class D2>
		requires std::three_way_comparable_with<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer>
	std::compare_three_way_result_t<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer> operator<=>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y) {
		return std::compare_three_way{}(x.get(), y.get());
	}

	template<class T, class D>
	bool operator==(const unique_ptr<T, D>& x, nullptr_t) noexcept {
		return !x;
	}

	template<class T, class D>
	bool operator<(const unique_ptr<T, D>& x, nullptr_t) {
		return ciel::less<typename unique_ptr<T, D>::pointer>()(x.get(), nullptr);
	}

	template<class T, class D>
	bool operator<(nullptr_t, const unique_ptr<T, D>& y) {
		return ciel::less<typename unique_ptr<T, D>::pointer>()(nullptr, y.get());
	}

	template<class T, class D>
	bool operator<=(const unique_ptr<T, D>& x, nullptr_t) {
		return !(nullptr < x);
	}

	template<class T, class D>
	bool operator<=(nullptr_t, const unique_ptr<T, D>& y) {
		return !(y < nullptr);
	}

	template<class T, class D>
	bool operator>(const unique_ptr<T, D>& x, nullptr_t) {
		return nullptr < x;
	}

	template<class T, class D>
	bool operator>(nullptr_t, const unique_ptr<T, D>& y) {
		return y < nullptr;
	}

	template<class T, class D>
	bool operator>=(const unique_ptr<T, D>& x, nullptr_t) {
		return !(x < nullptr);
	}

	template<class T, class D>
	bool operator>=(nullptr_t, const unique_ptr<T, D>& y) {
		return !(nullptr < y);
	}

	template<class T, class D>
		requires std::three_way_comparable<typename unique_ptr<T, D>::pointer>
	std::compare_three_way_result_t<typename unique_ptr<T, D>::pointer> operator<=>(const unique_ptr<T, D>& x, nullptr_t) {
		return std::compare_three_way{}(x.get(), static_cast<typename unique_ptr<T, D>::pointer>(nullptr));
	}

	template<class T, class D>
		requires ciel::is_swappable_v<D>
	void swap(unique_ptr<T, D>& lhs, unique_ptr<T, D>& rhs) noexcept {
		lhs.swap(rhs);
	}

	// make_unique 不允许构造已知边界的数组，原因大概是 unique_ptr 本身只需要有未知数组版本就足够使用了
	// 	而如果 make_unique 允许构造已知边界数组，返回值将为 unique_ptr<T[N]>，毫无疑问会重载到通常版本，这显然是很蠢的
	template<class T, class... Args>
		requires (!ciel::is_array_v<T>)
	unique_ptr<T> make_unique(Args&&... args) {
		return unique_ptr<T>(new T(ciel::forward<Args>(args)...));
	}

	template<class T>
		requires ciel::is_unbounded_array_v<T>
	unique_ptr<T> make_unique(size_t size) {
		return unique_ptr<T>(new ciel::remove_extent_t<T>[size]());
	}

	template<class T, class... Args>
		requires ciel::is_bounded_array_v<T>
	auto make_unique(Args&&... args) = delete;

	template<class T>
		requires (!ciel::is_array_v<T>)
	unique_ptr<T> make_unique_for_overwrite() {
		return unique_ptr<T>(new T);
	}

	template<class T>
		requires ciel::is_unbounded_array_v<T>
	unique_ptr<T> make_unique_for_overwrite(size_t size) {
		return unique_ptr<T>(new ciel::remove_extent_t<T>[size]);
	}

	template<class T, class... Args>
	auto make_unique_for_overwrite(Args&&... args) = delete;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNIQUE_PTR_H_