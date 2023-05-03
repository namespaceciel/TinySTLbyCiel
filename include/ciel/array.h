#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_

#include <stddef.h>
#include <ciel/iterator_legacy.h>
#include <ciel/utility_impl/move.h>

namespace ciel {

	template<class T, size_t N>
	struct array {

		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using iterator = T*;
		using const_iterator = const T*;

		using reverse_iterator = ciel::reverse_iterator<iterator>;
		using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;

		T m_data[N];

//		array& operator=(array<T, N> moved_data) {
//			for (size_type i = 0; i < N; ++i) {
//				m_data[i] = moved_data[i];
//			}
//		}

		reference at(size_type index) {
			if (index >= N) {
				throw std::out_of_range("array成员函数at遇到越界问题");
			}
			return m_data[index];
		}

		reference operator[](size_type index) {
			return m_data[index];
		}

		reference front() {
			return m_data[0];
		}

		reference back() {
			return m_data[N - 1];
		}

		T* data() {
			return m_data;
		}

		iterator begin() {
			return m_data;
		}

		const_iterator begin() const {
			return m_data;
		}

		const_iterator cbegin() const {
			return m_data;
		}

		iterator end() {
			return m_data + N;
		}

		const_iterator end() const {
			return m_data + N;
		}

		const_iterator cend() const {
			return m_data + N;
		}

		reverse_iterator rbegin() {
			return reverse_iterator(&m_data[N]);
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(&m_data[N]);
		}

		const_reverse_iterator crbegin() const {
			return const_reverse_iterator(&m_data[N]);
		}

		reverse_iterator rend() {
			return reverse_iterator(&m_data[0]);
		}

		const_reverse_iterator rend() const {
			return const_reverse_iterator(&m_data[0]);
		}

		const_reverse_iterator crend() const {
			return const_reverse_iterator(&m_data[0]);
		}

		bool empty() {
			return N == 0;
		}

		size_type size() {
			return N;
		}

		size_type max_size() {
			return N;
		}

		void fill(const_reference copied_obj) {
			for (size_type i = 0; i < N; ++i) {
				m_data[i] = copied_obj;
			}
		}

		void swap(array<T, N>& swapped_obj) {
			T temp_obj;
			for (size_type i = 0; i < N; ++i) {
				temp_obj = move(m_data[i]);
				m_data[i] = move(swapped_obj[i]);
				swapped_obj[i] = move(temp_obj);
			}
		}
	};  //class array

	template<class T, size_t N>
	inline bool operator==(array<T, N>& left_array, array<T, N>& right_array) {
		for (size_t i = 0; i < N; ++i) {
			if (left_array[i] != right_array[i]) {
				return false;
			}
		}
		return true;
	}

	template<class T, size_t N>
	inline bool operator!=(array<T, N>& left_array, array<T, N>& right_array) {
		return !(left_array == right_array);
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_