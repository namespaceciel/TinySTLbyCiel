#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_WEAK_PTR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_WEAK_PTR_H_

#include <ciel/memory_impl/shared_ptr.h>

namespace ciel {

	template<class T>
	class shared_ptr;

	template<class Y, class T>
	struct is_compatible_with;

	template<class T>
	class weak_ptr {
	public:
		using element_type = ciel::remove_extent_t<T>;

	private:
		element_type* ptr;
		size_t* count;

	public:
		constexpr weak_ptr() noexcept: ptr(), count() {}

		weak_ptr(const weak_ptr& r) noexcept: ptr(r.ptr), count(r.count) {}

		template<class Y>
			requires is_compatible_with<Y, T>::value
		weak_ptr(const weak_ptr<Y>& r) noexcept: ptr(r.ptr), count(r.count) {}

		template<class Y>
			requires is_compatible_with<Y, T>::value
		weak_ptr(const ciel::shared_ptr<Y>& r) noexcept: ptr(r.ptr), count(r.count) {}

		weak_ptr(weak_ptr&& r) noexcept: ptr(ciel::move(r.ptr)), count(ciel::move(r.count)) {}

		template<class Y>
		weak_ptr(weak_ptr<Y>&& r) noexcept : ptr(ciel::move(r.ptr)), count(ciel::move(r.count)) {}

		~weak_ptr() {
			reset();
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
			if (ptr) {
				delete ptr;
				delete count;
				ptr = nullptr;
				count = nullptr;
			}
		}

		void swap(weak_ptr& r) noexcept {
			ciel::swap(ptr, r.ptr);
			ciel::swap(count, r.count);
		}

		size_t use_count() const noexcept {
			return count ? *count : 0;
		}

		bool expired() const noexcept {
			return use_count() == 0;
		}

		ciel::shared_ptr<T> lock() const noexcept {
			return expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
		}

		template<class Y>
		bool owner_before(const weak_ptr<Y>& other) const noexcept {
			return count < other.count;
		}

		template<class Y>
		bool owner_before(const ciel::shared_ptr<Y>& other) const noexcept {
			return count < other.count;
		}

	};    //class weak_ptr

	template<class T>
	void swap(weak_ptr<T>& lhs, weak_ptr<T>& rhs) noexcept {
		lhs.swap(rhs);
	}

	template<class T>
	weak_ptr(ciel::shared_ptr<T>) -> weak_ptr<T>;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_WEAK_PTR_H_
