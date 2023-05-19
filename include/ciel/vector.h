#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_

#include <cstddef>
#include <iterator>
#include <ciel/memory.h>

namespace ciel {

	template<class T, class Allocator = ciel::allocator<T>>
	class vector {

		static_assert(ciel::is_same_v<typename Allocator::value_type, T>, "ciel::vector 要求 Allocator::value_type 与 T 相同");

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;

		using iterator = value_type*;
		using const_iterator = const value_type*;

		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	private:
		pointer start;
		pointer finish;
		pointer end_of_storage;

	public:
		constexpr vector() noexcept(noexcept(allocator_type()));

		constexpr explicit vector(const allocator_type& alloc) noexcept;

		constexpr vector(size_type count, const T& value, const allocator_type& alloc = allocator_type());

		constexpr explicit vector(size_type count, const allocator_type& alloc = allocator_type());

		template<class InputIt>
		constexpr vector(InputIt first, InputIt last, const allocator_type& alloc = allocator_type());

		constexpr vector(const vector& other);

		constexpr vector(const vector& other, const allocator_type& alloc);

		constexpr vector(vector&& other) noexcept;

		constexpr vector(vector&& other, const allocator_type& alloc);

		constexpr vector(std::initializer_list<T> init, const allocator_type& alloc = allocator_type());










		vector& operator=(const vector<T>& copied_vec) {
			if (*this == copied_vec) {
				return *this;
			}
			start = allocator<T>::allocate(copied_vec.capacity());
			finish = start;
			try {    //异常安全
				for (size_type i = 0; i < copied_vec.size(); ++i) {
					allocator<T>::construct(start + i, *(copied_vec.begin() + i));
					++finish;
				}
			}
			catch (...) {
				~vector();
				throw;
			}
			end_of_storage = start + copied_vec.capacity();
			return *this;
		}

		explicit vector(const vector<T>& copied_vec) {
			*this = copied_vec;
		}

		vector& operator=(vector<T>&& moved_vec) {
			if (*this == moved_vec) {
				return *this;
			}
			start = moved_vec.start;
			finish = moved_vec.finish;
			end_of_storage = moved_vec.end_of_storage;
			moved_vec.start = nullptr;
			moved_vec.finish = nullptr;
			moved_vec.end_of_storage = nullptr;
			return *this;
		}

		explicit vector(vector<T>&& moved_vec) {
			*this = std::move(moved_vec);
		}

		~vector() {    //先一个个手动调用成员的析构函数，再由vector自己释放内存
			if (start) {
				for (--finish; finish >= start; --finish) {
					allocator<T>::destroy(finish);
				}
				allocator<T>::deallocate(start);
			}
		}

		reference at(size_type index) {
			if (index >= size()) {
				throw std::out_of_range("vector成员函数at遇到越界问题");
			}
			return start[index];
		}

		reference operator[](size_type index) {
			return start[index];
		}

		reference front() {
			return start[0];
		}

		reference back() {
			return *(finish - 1);
		}

		T* data() {
			return start;
		}

		iterator begin() {
			return start;
		}

		const_iterator begin() const {
			return start;
		}

		const_iterator cbegin() const {
			return start;
		}

		iterator end() {
			return finish;
		}

		const_iterator end() const {
			return finish;
		}

		const_iterator cend() const {
			return finish;
		}

		reverse_iterator rbegin() {
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const {
			return const_reverse_iterator(cend());
		}

		reverse_iterator rend() {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crend() const {
			return const_reverse_iterator(begin());
		}

		bool empty() const {
			return start == finish;
		}

		size_type size() const {
			return static_cast<size_type>(finish - start);
		}

		size_type capacity() const {
			return static_cast<size_type>(end_of_storage - start);
		}

		size_type max_size() const {
			return allocator<T>::max_size();
		}

		iterator erase(iterator pos);

		constexpr iterator erase(const_iterator pos);

		iterator erase(iterator first, iterator last);

		constexpr iterator erase(const_iterator first, const_iterator last);

		void clear() {
			if (!empty()) {
				for (--finish; finish >= start; --finish) {
					allocator<T>::destroy(finish);
				}
			}
		}

	};  //class vector

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_