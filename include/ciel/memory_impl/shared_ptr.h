#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_SHARED_PTR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_SHARED_PTR_H_

// 标准里的这两个有符号数查不到用意，怀疑是标准委员会想谋杀强迫症
// element_type& operator[](ptrdiff_t idx) const;
// long use_count() const noexcept;

#include <ciel/memory_impl/unique_ptr.h>
#include <atomic>

namespace ciel {

	/*
	在典型的实现中， shared_ptr 只保有两根指针：
			get() 所返回的指针	（也称为存储指针）
			指向控制块的指针
	控制块是一个动态分配的对象，其中包含：
			指向被管理对象的指针或被管理对象本身		（也称为管理指针）
			删除器（类型擦除）
			分配器（类型擦除）
			占有被管理对象的 shared_ptr 的数量
			涉及被管理对象的 weak_ptr 的数量

	存储指针与管理指针不同常见于以下两种场景：
			1、存储指针指向的对象是管理指针指向对象的成员
			2、多态

	实现类型擦除的删除器与分配器需要用到多态，先创建一个带有明确类型删除器与分配器的对象，后用基类指针指向它	TODO: 感觉 deducing this 的 ctrp 可能可以替代它
	但因此也会有一些问题，例如基类指针中的 get_deleter() 虚函数返回值只能为 void*（无法得知 deleter_type）
	上层 get_deleter() 需要手动指定模板参数 Deleter，传入底层函数后通过对比 typeid 来确定是否匹配，否则返回 nullptr

	[[no_unique_address]] 已经无法满足同时存在删除器和分配器的空基类优化了，所以继承了分配器（懒得用 compressed pair）

	由于通过基类指针无从知晓控制块大小，控制块的释放只能由自身完成。EASTL 的实现是在子类重写虚函数（在内部拷贝分配器，显式调用析构函数后由分配器清理内存）
	FIXME: 由于控制块的模板参数里有 allocator 本身，所以应要一个定制的 allocator 并手动指定 value_type 为 control_block_with_pointer<element_type, Deleter, control_block_with_pointer_allocator>，
	 这对于用户指定分配器的要求太苛刻了

	enable_shared_from_this 用到了 crtp
	*/

	template<class T>
	class weak_ptr;

	// 若指针类型 Y* 可转换为指针类型 T* ，或 Y 是 U[N] 类型数组而 T 是 U cv [] （其中 cv 是某个 cv 限定符集合），则说 Y* 兼容 T*
	template<class Y, class T>
	struct is_compatible_with : ciel::bool_constant<ciel::is_convertible_v<Y*, T*> || (ciel::is_bounded_array_v<Y> && ciel::is_unbounded_array_v<T> && ciel::is_same_v<ciel::remove_extent_t<Y>, ciel::remove_cv_t<ciel::remove_extent_t<T>>>)> {};

	// TODO:
	// 若 T 是数组类型 U[N] ，则若 Y(*)[N] 不可转换为 T* 则 (3-4,6) 不参与重载决议
	// 若 T 是数组类型 U[] ，则若 Y(*)[] 不可转换为 T* 则 (3-4,6) 不参与重载决议
	// 否则，若 Y* 不可转换为 T* 则 (3-4,6) 不参与重载决议。

	struct shared_weak_count {
		std::atomic<size_t> shared_count;
		std::atomic<size_t> shared_and_weak_count;

		shared_weak_count(size_t s = 1, size_t w = 1) : shared_count(s), shared_and_weak_count(w) {}

		virtual ~shared_weak_count() noexcept {}

		size_t use_count() const noexcept {
			return shared_count.load(std::memory_order_relaxed);
		}

		void add_ref() noexcept {
			shared_count.fetch_add(1, std::memory_order_relaxed);
			shared_and_weak_count.fetch_add(1, std::memory_order_relaxed);
		}

		void weak_add_ref() noexcept {
			shared_and_weak_count.fetch_add(1, std::memory_order_relaxed);
		}

		void shared_count_release() {
			if (shared_count.fetch_sub(1, std::memory_order_release) == 1) {
				delete_pointer();
			}
			shared_and_weak_count_release();
		}

		void shared_and_weak_count_release() {
			if (shared_and_weak_count.fetch_sub(1, std::memory_order_release) == 1) {
				delete_control_block();
			}
		}

		virtual void* get_deleter(const std::type_info& type) noexcept = 0;
		virtual void delete_pointer() noexcept = 0;
		virtual void delete_control_block() noexcept = 0;

	};    // shared_weak_count

	template<class element_type, class Deleter, class Allocator>
		requires requires { ciel::declval<Deleter>()(static_cast<element_type*>(nullptr)); } && ciel::is_move_constructible_v<Deleter>
	struct control_block_with_pointer : shared_weak_count, Allocator {
	public:
		using pointer = element_type*;
		using deleter_type = Deleter;
		using allocator_type = Allocator;

	private:
		using alloc_traits = ciel::allocator_traits<allocator_type>;

		pointer ptr;
		[[no_unique_address]] deleter_type dlt;

	public:
		control_block_with_pointer(pointer p, deleter_type&& d, allocator_type&& alloc) : ptr(p), dlt(ciel::move(d)), allocator_type(ciel::move(alloc)) {}

		virtual void* get_deleter(const std::type_info& type) noexcept override {
			return (type == typeid(deleter_type)) ? static_cast<void*>(&dlt) : nullptr;
		}

		virtual void delete_pointer() noexcept override {
			dlt(ptr);
//			ptr = nullptr;
		}

		virtual void delete_control_block() noexcept override {
			allocator_type alloc = *this;
			this->~control_block_with_pointer();
			alloc_traits::deallocate(alloc, this, sizeof(*this));
		}

	};    // control_block_with_pointer

	// TODO: struct control_block_with_instance

	template<class element_type, class Deleter>
	class control_block_with_pointer_allocator {
	public:
		using value_type = control_block_with_pointer<element_type, Deleter, control_block_with_pointer_allocator>;
		using pointer = value_type*;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using propagate_on_container_move_assignment = ciel::true_type;

	public:
		constexpr control_block_with_pointer_allocator() noexcept = default;

		constexpr control_block_with_pointer_allocator(const control_block_with_pointer_allocator&) noexcept = default;

		constexpr ~control_block_with_pointer_allocator() = default;

		[[nodiscard]] constexpr pointer allocate(size_t n) {
			if (alignof(value_type) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
				return static_cast<pointer>(::operator new(n, static_cast<std::align_val_t>(alignof(value_type))));
			} else {
				return static_cast<pointer>(::operator new(n));
			}
		}

		constexpr void deallocate(pointer p, size_t) {
			if (alignof(value_type) > __STDCPP_DEFAULT_NEW_ALIGNMENT__) {
				::operator delete((void*)p, static_cast<std::align_val_t>(alignof(value_type)));
			} else {
				::operator delete((void*)p);
			}
		}

	};  // class control_block_with_pointer_allocator

	template<class T>
	class shared_ptr {
	public:
		using element_type = ciel::remove_extent_t<T>;
		using weak_type = ciel::weak_ptr<T>;

	private:
		friend weak_type;

		element_type* pointer;
		shared_weak_count* count;

		template<class Deleter, class Allocator>
		shared_weak_count* alloc_control_block(element_type* ptr, Deleter&& dlt, Allocator&& alloc) {
			using alloc_traits = ciel::allocator_traits<Allocator>;
			control_block_with_pointer<element_type, Deleter, Allocator>* ctlblk = alloc_traits::allocate(alloc, sizeof(control_block_with_pointer<element_type, Deleter, Allocator>));
			try {
				alloc_traits::construct(alloc, ctlblk, ptr, ciel::move(dlt), ciel::move(alloc));
				return ctlblk;
			} catch (...) {
				alloc_traits::deallocate(alloc, ctlblk, sizeof(control_block_with_pointer<element_type, Deleter, Allocator>));
				throw;
			}
		}

	public:
		constexpr shared_ptr() noexcept: pointer(nullptr), count(nullptr) {}

		constexpr shared_ptr(nullptr_t) noexcept: pointer(nullptr), count(nullptr) {}

		template<class Y>
		explicit shared_ptr(Y* ptr) : pointer(ptr), count(alloc_control_block(ptr, ciel::default_delete<T>(), control_block_with_pointer_allocator<element_type, ciel::default_delete<T>>())) {}

		template<class Y, class Deleter>
		shared_ptr(Y* ptr, Deleter d) : pointer(ptr), count(alloc_control_block(ptr, ciel::move(d), control_block_with_pointer_allocator<element_type, Deleter>())) {}

		template<class Deleter>
		shared_ptr(nullptr_t, Deleter d) : pointer(nullptr), count(nullptr) {}

		template<class Y, class Deleter, class Alloc>
		shared_ptr(Y* ptr, Deleter d, Alloc alloc) : pointer(ptr), count(alloc_control_block(ptr, ciel::move(d), ciel::move(alloc))) {}

		template<class Deleter, class Alloc>
		shared_ptr(nullptr_t, Deleter d, Alloc alloc) : pointer(nullptr), count(nullptr) {}

		template<class Y>
		shared_ptr(const shared_ptr<Y>& r, element_type* ptr) noexcept : pointer(ptr), count(r.count) {
			if (count) {
				count->add_ref();
			}
		}

		template<class Y>
		shared_ptr(shared_ptr<Y>&& r, element_type* ptr) noexcept : pointer(ptr), count(r.count) {
			r.pointer = nullptr;
			r.count = nullptr;
		}

		shared_ptr(const shared_ptr& r) noexcept: pointer(r.pointer), count(r.count) {
			if (count) {
				count->add_ref();
			}
		}

		template<class Y>
			requires is_compatible_with<Y, T>::value
		shared_ptr(const shared_ptr<Y>& r) noexcept : pointer(r.pointer), count(r.count) {
			if (count) {
				count->add_ref();
			}
		}

		shared_ptr(shared_ptr&& r) noexcept: pointer(r.pointer), count(r.count) {
			r.pointer = nullptr;
			r.count = nullptr;
		}

		template<class Y>
			requires is_compatible_with<Y, T>::value
		shared_ptr(shared_ptr<Y>&& r) noexcept : pointer(r.pointer), count(r.count) {
			r.pointer = nullptr;
			r.count = nullptr;
		}

		template<class Y>
			requires is_compatible_with<Y, T>::value
		explicit shared_ptr(const weak_ptr<Y>& r) : pointer(r.ptr), count(r.count) {
			if (count) {
				count->add_ref();
			}
		}

		// 若 r.get() 是空指针，则此重载等价于默认构造函数 (1)
		template<class Y, class Deleter>
			requires is_compatible_with<ciel::remove_pointer_t<typename ciel::unique_ptr<Y, Deleter>::pointer>, T>::value
		shared_ptr(ciel::unique_ptr<Y, Deleter>&& r) : pointer(r.release()), count(alloc_control_block(pointer, ciel::move(r.get_deleter()), control_block_with_pointer_allocator<element_type, Deleter>())) {}

		~shared_ptr() {
			if (count) {
				count->shared_count_release();
			}
		}

		shared_ptr& operator=(const shared_ptr& r) noexcept {
			shared_ptr<T>(r).swap(*this);
			return *this;
		}

		template<class Y>
		shared_ptr& operator=(const shared_ptr<Y>& r) noexcept {
			shared_ptr<T>(r).swap(*this);
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& r) noexcept {
			shared_ptr<T>(ciel::move(r)).swap(*this);
			return *this;
		}

		template<class Y>
		shared_ptr& operator=(shared_ptr<Y>&& r) noexcept {
			shared_ptr<T>(ciel::move(r)).swap(*this);
			return *this;
		}

		template<class Y, class Deleter>
		shared_ptr& operator=(ciel::unique_ptr<Y, Deleter>&& r) {
			shared_ptr<T>(ciel::move(r)).swap(*this);
			return *this;
		}

		void reset() noexcept {
			shared_ptr().swap(*this);
		}

		template<class Y>
			requires ciel::is_convertible_v<Y, T>
		void reset(Y* ptr) {
			shared_ptr<T>(ptr).swap(*this);
		}

		template<class Y, class Deleter>
			requires ciel::is_convertible_v<Y, T>
		void reset(Y* ptr, Deleter d) {
			shared_ptr<T>(ptr, d).swap(*this);
		}

		template<class Y, class Deleter, class Alloc>
			requires ciel::is_convertible_v<Y, T>
		void reset(Y* ptr, Deleter d, Alloc alloc) {
			shared_ptr<T>(ptr, d, alloc).swap(*this);
		}

		void swap(shared_ptr& r) noexcept {
			ciel::swap(pointer, r.pointer);
			ciel::swap(count, r.count);
		}

		element_type* get() const noexcept {
			return pointer;
		}

		T& operator*() const noexcept {
			return *get();
		}

		T* operator->() const noexcept {
			return get();
		}

		element_type& operator[](size_t idx) const {
			static_assert(ciel::is_array_v<T>, "ciel::shared_ptr::operator[] 只有在 T 为数组类型时合法");

			return get()[idx];
		}

		size_t use_count() const noexcept {
			return count ? count->use_count() : 0;
		}

		explicit operator bool() const noexcept {
			return get() != nullptr;
		}

		template<class Y>
		bool owner_before(const shared_ptr<Y>& other) const noexcept {
			return count < other.count;
		}

		template<class Y>
		bool owner_before(const weak_ptr<Y>& other) const noexcept {
			return count < other.count;
		}

		template<class D>
		D* get_deleter() const noexcept {
			return count ? static_cast<D*>(count->get_deleter(typeid(ciel::remove_cv_t<D>))) : nullptr;
		}

	};    // class shared_ptr

	template<class Deleter, class T>
	Deleter* get_deleter(const shared_ptr<T>& p) noexcept {
		return p.template get_deleter<Deleter>();
	}

	template<class T>
	void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) noexcept {
		lhs.swap(rhs);
	}

	template<class T>
	shared_ptr(weak_ptr<T>) -> shared_ptr<T>;

	template<class T, class D>
	shared_ptr(ciel::unique_ptr<T, D>) -> shared_ptr<T>;

	template<class T>
	class weak_ptr {
	public:
		using element_type = ciel::remove_extent_t<T>;

	private:
		element_type* pointer;
		shared_weak_count* count;

	public:
		constexpr weak_ptr() noexcept: pointer(nullptr), count(nullptr) {}

		weak_ptr(const weak_ptr& r) noexcept: pointer(r.pointer), count(r.count) {
			if (count) {
				count->weak_add_ref();
			}
		}

		template<class Y>
			requires is_compatible_with<Y, T>::value
		weak_ptr(const weak_ptr<Y>& r) noexcept : pointer(r.pointer), count(r.count) {
			if (count) {
				count->weak_add_ref();
			}
		}

		template<class Y>
			requires is_compatible_with<Y, T>::value
		weak_ptr(const shared_ptr<Y>& r) noexcept : pointer(r.pointer), count(r.count) {
			if (count) {
				count->weak_add_ref();
			}
		}

		weak_ptr(weak_ptr&& r) noexcept: pointer(r.pointer), count(r.count) {
			r.pointer = nullptr;
			r.count = nullptr;
		}

		template<class Y>
			requires ciel::is_convertible_v<Y*, T*>
		weak_ptr(weak_ptr<Y>&& r) noexcept : pointer(r.pointer), count(r.count) {
			r.pointer = nullptr;
			r.count = nullptr;
		}

		~weak_ptr() {
			if (count) {
				count->shared_and_weak_count_release();
			}
		}

		weak_ptr& operator=(const weak_ptr& r) noexcept {
			weak_ptr<T>(r).swap(*this);
			return *this;
		}

		template<class Y>
		weak_ptr& operator=(const weak_ptr<Y>& r) noexcept {
			weak_ptr<T>(r).swap(*this);
			return *this;
		}

		template<class Y>
		weak_ptr& operator=(const ciel::shared_ptr<Y>& r) noexcept {
			weak_ptr<T>(r).swap(*this);
			return *this;
		}

		weak_ptr& operator=(weak_ptr&& r) noexcept {
			weak_ptr<T>(ciel::move(r)).swap(*this);
			return *this;
		}

		template<class Y>
		weak_ptr& operator=(weak_ptr<Y>&& r) noexcept {
			weak_ptr<T>(ciel::move(r)).swap(*this);
			return *this;
		}

		void reset() noexcept {
			if (count) {
				count->shared_and_weak_count_release();
				pointer = nullptr;
				count = nullptr;
			}
		}

		void swap(weak_ptr& r) noexcept {
			ciel::swap(pointer, r.pointer);
			ciel::swap(count, r.count);
		}

		size_t use_count() const noexcept {
			return count ? count->use_count() : 0;
		}

		bool expired() const noexcept {
			return use_count() == 0;
		}

		shared_ptr<T> lock() const noexcept {
			return expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
		}

		template<class Y>
		bool owner_before(const weak_ptr<Y>& other) const noexcept {
			return count < other.count;
		}

		template<class Y>
		bool owner_before(const shared_ptr<Y>& other) const noexcept {
			return count < other.count;
		}

	};    // class weak_ptr

	template<class T>
	void swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs) noexcept {
		lhs.swap(rhs);
	}

	template<class T>
	weak_ptr(ciel::shared_ptr<T>) -> weak_ptr<T>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_SHARED_PTR_H_