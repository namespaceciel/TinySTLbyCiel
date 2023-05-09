#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_

#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <ciel/utility_impl/move.h>

namespace ciel {

	template<class T, size_t N>
	struct array {

		using value_type = T;
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

		T m_data[N];

		constexpr reference at(size_type pos) {
			if (pos >= N) {
				throw std::out_of_range("ciel::array 成员函数 at 遇到越界问题");
			}
			return m_data[pos];
		}

		constexpr const_reference at(size_type pos) const {
			if (pos >= N) {
				throw std::out_of_range("ciel::array 成员函数 at 遇到越界问题");
			}
			return m_data[pos];
		}

		constexpr reference operator[](size_type pos) {
			return m_data[pos];
		}

		constexpr const_reference operator[](size_type pos) const {
			return m_data[pos];
		}

		constexpr reference front() {
			return m_data[0];
		}

		constexpr const_reference front() const {
			return m_data[0];
		}

		constexpr reference back() {
			return m_data[N - 1];
		}

		constexpr const_reference back() const {
			return m_data[N - 1];
		}

		constexpr T* data() noexcept {
			return m_data;
		}

		constexpr const T* data() const noexcept {
			return m_data;
		}

		constexpr iterator begin() noexcept {
			return iterator(m_data);
		}

		constexpr const_iterator begin() const noexcept {
			return const_iterator(m_data);
		}

		constexpr const_iterator cbegin() const noexcept {
			return begin();
		}

		constexpr iterator end() noexcept {
			return iterator(m_data + N);
		}

		constexpr const_iterator end() const noexcept {
			return const_iterator(m_data + N);
		}

		constexpr const_iterator cend() const noexcept {
			return end();
		}

		constexpr reverse_iterator rbegin() noexcept {
			return reverse_iterator(end());
		}

		constexpr const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(end());
		}

		constexpr const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		constexpr reverse_iterator rend() noexcept {
			return reverse_iterator(begin());
		}

		constexpr const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(begin());
		}

		constexpr const_reverse_iterator crend() const noexcept {
			return rend();
		}

		[[nodiscard]] constexpr bool empty() const noexcept{
			return N == 0;
		}

		constexpr size_type size() const noexcept{
			return N;
		}

		constexpr size_type max_size() const noexcept{
			return N;
		}

		constexpr void fill( const T& value ){

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