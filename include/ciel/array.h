#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_

#include <cstddef>
#include <stdexcept>
#include <iterator>
#include <ciel/type_traits.h>
#include <ciel/utility.h>
#include <ciel/algorithm.h>

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

		[[nodiscard]] constexpr bool empty() const noexcept {
			return N == 0;
		}

		constexpr size_type size() const noexcept {
			return N;
		}

		constexpr size_type max_size() const noexcept {
			return N;
		}

		constexpr void fill(const T& value) {
			ciel::fill_n(m_data, N, value);
		}

		constexpr void swap(array& other) noexcept(ciel::is_nothrow_swappable_v<T>) {
			ciel::swap_ranges(m_data, m_data + N, other.m_data);
		}

	};  //class array

	template<class T, size_t N>
	constexpr bool operator==(array<T, N>& lhs, array<T, N>& rhs) {
		return ciel::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template<size_t I, class T, size_t N>
	constexpr T& get(array<T, N>& a) noexcept {
		static_assert(I < N, "调用 ciel::get(ciel::array) 越界");
		return a[I];
	}

	template<size_t I, class T, size_t N>
	constexpr T&& get(array<T, N>&& a) noexcept {
		static_assert(I < N, "调用 ciel::get(ciel::array) 越界");
		return ciel::move(a[I]);
	}

	template<size_t I, class T, size_t N>
	constexpr const T& get(const array<T, N>& a) noexcept {
		static_assert(I < N, "调用 ciel::get(ciel::array) 越界");
		return a[I];
	}

	template<size_t I, class T, size_t N>
	constexpr const T&& get(const array<T, N>&& a) noexcept {
		static_assert(I < N, "调用 ciel::get(ciel::array) 越界");
		return ciel::move(a[I]);
	}

	template<class T, size_t N>
	constexpr void swap(array<T, N>& lhs, array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
		lhs.swap(rhs);
	}

	namespace array_details {

		template<class T, size_t N, size_t... I>
		constexpr array<ciel::remove_cv_t<T>, N>
		to_array_impl(T (& a)[N], std::index_sequence<I...>) {
			return {{a[I]...}};
		}

		template<class T, size_t N, size_t... I>
		constexpr array<ciel::remove_cv_t<T>, N>
		to_array_impl(T (&& a)[N], std::index_sequence<I...>) {
			return {{ciel::move(a[I])...}};
		}
	}

	template<class T, size_t N>
	constexpr array<ciel::remove_cv_t<T>, N> to_array(T (& a)[N]) {
		static_assert(!ciel::is_array_v<T>, "ciel::to_array 不接受高维数组");
		static_assert(ciel::is_constructible_v<T, T&>, "ciel::to_array 需要元素可构造");
		return array_details::to_array_impl(a, std::make_index_sequence<N>{});
	}

	template<class T, size_t N>
	constexpr array<ciel::remove_cv_t<T>, N> to_array(T (&& a)[N]) {
		static_assert(!ciel::is_array_v<T>, "ciel::to_array 不接受高维数组");
		static_assert(ciel::is_move_constructible_v<T>, "ciel::to_array 需要元素可移动构造");
		return array_details::to_array_impl(ciel::move(a), std::make_index_sequence<N>{});
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ARRAY_H_