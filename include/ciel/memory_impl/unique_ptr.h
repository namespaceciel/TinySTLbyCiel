#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNIQUE_PTR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNIQUE_PTR_H_

#include <ciel/memory_impl/default_delete.h>
#include <ciel/memory_impl/allocator_traits.h>

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
		using pointer = allocator_traits_details::has_pointer<element_type, deleter_type>::type;

	private:
		pointer ptr;
		[[no_unique_address]] deleter_type dlt;

	public:
		constexpr unique_ptr() noexcept requires (ciel::is_default_constructible_v<Deleter> && !ciel::is_pointer_v<Deleter>): ptr(), dlt() {}

		constexpr unique_ptr(nullptr_t) noexcept requires (ciel::is_default_constructible_v<Deleter> && !ciel::is_pointer_v<Deleter>): ptr(), dlt() {}

		constexpr explicit unique_ptr(pointer p) noexcept requires (ciel::is_default_constructible_v<Deleter> && !ciel::is_pointer_v<Deleter>): ptr(p), dlt() {}

		//第一组情况
		template<class D = deleter_type, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 0>>
			requires ciel::is_nothrow_copy_constructible_v<D>
		unique_ptr(pointer p, const D& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class D = deleter_type, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 0>>
			requires ciel::is_nothrow_move_constructible_v<D>
		unique_ptr(pointer p, D&& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		//第二组情况
		template<class D = deleter_type, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 1>>
		unique_ptr(pointer p, D& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class D = deleter_type, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 1>>
		unique_ptr(pointer p, D&& d) = delete;

		//第三组情况
		template<class D = deleter_type, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 2>>
		unique_ptr(pointer p, const D& d) noexcept requires ciel::is_constructible_v<D, decltype(d)>: ptr(p), dlt(ciel::forward<decltype(d)>(d)) {}

		template<class D = deleter_type, class = ciel::enable_if_t<unique_ptr_details::unique_ptr_constructor_helper<D>::tag == 2>>
		unique_ptr(pointer p, const D&& d) = delete;

		//TODO: 若 Deleter 不是引用类型，则要求它为不抛出可移动构造 (MoveConstructible) （若 Deleter 是引用，则 get_deleter() 和 u.get_deleter() 在移动构造后引用相同值）
		constexpr unique_ptr(unique_ptr&& u) noexcept requires ciel::is_move_constructible_v<deleter_type>: ptr(u.release()), dlt(ciel::forward<deleter_type>(u.get_deleter())) {}

		//TODO: 此构造函数仅若下列皆为真才参与重载决议：
		//a) unique_ptr<U, E>::pointer 可隐式转换为 pointer
		//b) U 不是数组类型
		//c) Deleter 是引用类型且 E 与 D 为同一类型，或 Deleter 不是引用类型且 E 可隐式转换为 D
		template<class U, class E>
		constexpr unique_ptr(unique_ptr<U, E>&& u) noexcept : ptr(u.release()), dlt(ciel::forward<deleter_type>(u.get_deleter())) {}

		~unique_ptr() {
			reset();
		}

		//TODO:
		//若 Deleter 不是引用类型，则要求它为不抛出可移动赋值 (MoveAssignable)
		//若 Deleter 是引用类型，则要求 remove_reference<Deleter>::type 为不抛出可复制赋值 (CopyAssignable)
		unique_ptr& operator=(unique_ptr&& r) noexcept requires ciel::is_move_assignable_v<deleter_type> {
			reset(r.release());
			dlt = ciel::forward<deleter_type>(r.get_deleter());
			return *this;
		}

		//TODO: 主模板的此赋值运算符仅若 U 非数组类型且 unique_ptr<U,E>::pointer 可隐式转换为 pointer 且 is_assignable<Deleter&, E&&>::value 为 true 才参与重载决议。
		template<class U, class E>
		unique_ptr& operator=(unique_ptr<U, E>&& r) noexcept requires (!ciel::is_array_v<U> && ciel::is_assignable_v<deleter_type&, E&&>) {
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

		//关于这里为什么是三步：如果 pointer 是智能指针类型，在前两步拷贝构造与赋值中如果抛出异常，ptr 仍然能保有指针的所有权（当然这也需要 pointer 类自身满足异常安全）
		//所以如果在 try 块中执行 old.reset(new); 抛出异常的话，old 依然是完整的，new 则需要在 catch 块中由调用方处理
		//并且，如果 ptr 指向的是拥有自身的对象，以常规想法的先销毁后赋值会使得销毁时调用析构函数而析构函数又调用 reset 陷入死循环。先赋空再析构则不会
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

	};    //class unique_ptr

	template<class T, class Deleter>
	class unique_ptr<T[], Deleter>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNIQUE_PTR_H_