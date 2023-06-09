#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_DEQUE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_DEQUE_H_

#include <cstddef>
#include <ciel/list.h>

namespace ciel {

    // TODO: operator<=>

	// 这里的实现为了简单起见，deque 的 map （每个小数组的控制中心）用的是 list 实现，避免了重分配
	// 同样为了简单起见，与其他容器不同，deque 即使空初始化依然会分配一块堆内存。注意：这使得移动构造行为变为了先本身进行空初始化再与 other 进行 swap
    // 注意：deque_iterator 的 finish() 仅作为哨兵存在，deque_iterator 在正常操作时只要到 finish 会自动跳到下一块小数组的起点，永远不可能停留在 finish

	template<class T>
	constexpr size_t deque_subarray_size() noexcept {
		return ((sizeof(T) <= 4) ? 64 : ((sizeof(T) <= 8) ? 32 : ((sizeof(T) <= 16) ? 16 : ((sizeof(T) <= 32) ? 8 : 4))));
	}

	template<class T, class Pointer, class Reference, class MapIterator, size_t SubarraySize>
	struct deque_iterator {
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = Pointer;
		using reference = Reference;
		using iterator_category = ciel::random_access_iterator_tag;
		using iterator_concept = ciel::random_access_iterator_tag;

		T* cur;
		MapIterator node;

		T* start() const noexcept {
			return *node;
		}

		T* finish() const noexcept {
			return start() + SubarraySize;
		}

		deque_iterator() noexcept = default;

		deque_iterator(T* c, const MapIterator& n) noexcept : cur(c), node(n) {}

		deque_iterator(const deque_iterator& other) noexcept = default;

		template<class P, class R, class M>
		deque_iterator(const deque_iterator<T, P, R, M, SubarraySize>& other) noexcept : cur(other.base()), node(other.node) {}

		pointer operator->() const noexcept {
			return cur;
		}

		reference operator*() const noexcept {
			return *cur;
		}

		deque_iterator& operator++() noexcept {
			if (++cur == finish()) {
				++node;
				cur = start();
			}
			return *this;
		}

		deque_iterator operator++(int) noexcept {
			deque_iterator res(*this);
			++*this;
			return res;
		}

		deque_iterator& operator--() noexcept {
			if (cur == start()) {
				--node;
				cur = finish() - 1;
			} else {
				--cur;
			}
			return *this;
		}

		deque_iterator operator--(int) noexcept {
			deque_iterator res(*this);
			--*this;
			return res;
		}

		deque_iterator& operator+=(difference_type n) noexcept {
            difference_type offset = n + (cur - start());
			if (offset >= 0 && offset < SubarraySize) {
				cur += n;
			} else {
				ciel::advance(node, offset > 0 ? offset / static_cast<difference_type>(SubarraySize) : offset / static_cast<difference_type>(SubarraySize) - 1);    // 这个忘记转有符号数卡了我好久= =
				offset %= SubarraySize;
				cur = start() + offset;
			}
			return *this;
		}

		deque_iterator& operator-=(difference_type n) noexcept {
			return *this += -n;
		}

		deque_iterator operator+(difference_type n) const noexcept {
			deque_iterator res(*this);
			return res += n;
		}

		deque_iterator operator-(difference_type n) const noexcept {
			deque_iterator res(*this);
			return res -= n;
		}

        T* base() const noexcept {
            return cur;
        }

	};    // struct deque_iterator

	template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2, class MapIterator, size_t SubarraySize>
	bool operator==(const deque_iterator<T, Pointer1, Reference1, MapIterator, SubarraySize>& lhs, const deque_iterator<T, Pointer2, Reference2, MapIterator, SubarraySize>& rhs) {
		return lhs.base() == rhs.base();
	}

	template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2, class MapIterator, size_t SubarraySize>
	bool operator!=(const deque_iterator<T, Pointer1, Reference1, MapIterator, SubarraySize>& lhs, const deque_iterator<T, Pointer2, Reference2, MapIterator, SubarraySize>& rhs) {
		return !(lhs.base() == rhs.base());
	}

	template<class T, class Pointer1, class Pointer2, class Reference1, class Reference2, class MapIterator, size_t SubarraySize>
    typename ciel::iterator_traits<deque_iterator<T, Pointer1, Reference1, MapIterator, SubarraySize>>::difference_type operator-(const deque_iterator<T, Pointer1, Reference1, MapIterator, SubarraySize>& lhs, const deque_iterator<T, Pointer2, Reference2, MapIterator, SubarraySize>& rhs) {
		return SubarraySize * (ciel::distance(rhs.node, lhs.node) - 1) + (lhs.base() - lhs.start()) + (rhs.finish() - rhs.base());
	}

	template<class T, class Allocator = ciel::allocator<T>, size_t SubarraySize = deque_subarray_size<T>()>
	class deque {

		static_assert(ciel::is_same_v<typename Allocator::value_type, T>, "ciel::deque 要求 Allocator::value_type 与 T 相同");

	public:
		using value_type = T;
		using allocator_type = Allocator;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = typename ciel::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename ciel::allocator_traits<allocator_type>::const_pointer;

		using iterator = deque_iterator<value_type, pointer, reference, typename ciel::list<value_type*, allocator_type>::iterator, SubarraySize>;
		using const_iterator = deque_iterator<value_type, const_pointer, const_reference, typename ciel::list<value_type*, allocator_type>::iterator, SubarraySize>;

		using reverse_iterator = ciel::reverse_iterator<iterator>;
		using const_reverse_iterator = ciel::reverse_iterator<const_iterator>;

	private:
		using alloc_traits = ciel::allocator_traits<allocator_type>;

		ciel::list<value_type*, allocator_type> map;
		iterator start;
		iterator finish;
		[[no_unique_address]] allocator_type allocator;

		iterator alloc_range_destroy(allocator_type& a, iterator begin, iterator end) noexcept {
			while (end != begin) {
				alloc_traits::destroy(a, (--end).base());
			}
			return end;
		}

		template<class... Arg>
		iterator alloc_range_construct_n(allocator_type& a, iterator begin, size_type n, Arg&& ... arg) {
			iterator end = begin;
			try {
				for (size_type i = 0; i < n; ++i) {
					alloc_traits::construct(a, (end++).base(), ciel::forward<Arg>(arg)...);
				}
				return end;
			} catch (...) {
				alloc_range_destroy(a, begin, --end);    // 上面的实现里没构造就已经 ++end 了，这里复原
				throw;
			}
		}

		template<ciel::legacy_input_iterator InputIt>
		iterator alloc_range_construct(allocator_type& a, iterator begin, InputIt first, InputIt last) {
            iterator end = begin;
			try {
				while (first != last) {
					alloc_traits::construct(a, (end++).base(), *first++);
				}
				return end;
			} catch (...) {
				alloc_range_destroy(a, begin, --end);    // 上面的实现里没构造就已经 ++end 了，这里复原
				throw;
			}
		}

		void clear_and_get_cap_no_less_than(size_type size) {
			clear();
            start = iterator(map.front(), map.begin());
            finish = start;
			size_type s = size / SubarraySize + 1;
			if (map.size() < s) {
				s -= map.size();
				while (s--) {
					map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
				}
			}
		}

        size_type front_space() {
            size_type res = start.cur - start.start();
            auto cur = start.node;
            while (cur != map.begin()) {
                res += SubarraySize;
                --cur;
            }
            return res;
        }

        size_type back_space() {
            size_type res = finish.finish() - finish.cur - 1;
            auto cur = finish.node;
            while (cur != map.end().prev()) {
                res += SubarraySize;
                ++cur;
            }
            return res;
        }

        // 关于 insert / emplace / erase 的注意事项请看 vector
        iterator alloc_range_nothrow_move(allocator_type& a, iterator begin, iterator first, iterator last) noexcept {
            iterator end = begin;
            iterator boundary = first;
            while (first != last && end != boundary) {
                alloc_traits::construct(a, (end++).base(), ciel::move(*first++));
            }
            while (first != last) {
                *begin++ = ciel::move(*first++);
            }
            return begin;
        }

        iterator alloc_range_nothrow_move_backward(allocator_type& a, iterator end, iterator first, iterator last) noexcept {
            iterator begin = end;
            iterator boundary = last;
            while (first != last && begin != boundary) {
                alloc_traits::construct(a, (--begin).base(), ciel::move(*--last));
            }
            while (first != last) {
                *--begin = ciel::move(*--last);
            }
            return end;
        }

        // 看插入位置离哪头更近就移动两边
        template<class Args>
        iterator insert_n(iterator pos, size_type count, Args&& args) {
            if (!count) {
                return pos;
            }
            if (ciel::distance(begin(), pos) < ciel::distance(pos, end())) {  // 头部往前移动
                size_type bs = front_space();
                if (count > bs) {
                    bs = (count - bs) / SubarraySize + 1;
                    while (bs--) {
                        map.emplace_front(alloc_traits::allocate(allocator, SubarraySize));
                    }
                }

                iterator boundary = start;
                iterator res = pos - 1;
                start = alloc_range_nothrow_move(allocator, start - count, start, pos);

                auto left_movement = pos - boundary;
                auto loop_break = left_movement < count ? left_movement : count;
                for (size_type i = 0; i < loop_break; ++i) {    // 全部都是赋值
                    *--pos = ciel::forward<Args>(args);
                }
                if (left_movement < count) {    // 赋值完还有一部分需要构造
                    alloc_range_construct_n(allocator, start + loop_break, count - left_movement, ciel::forward<Args>(args)) - 1;
                }
                return res;
            } else {    // 尾部往后移动
                size_type bs = back_space();
                if (count >= bs) {
                    bs = (count - bs) / SubarraySize + 1;
                    while (bs--) {
                        map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
                    }
                }

                iterator boundary = finish;
                finish = alloc_range_nothrow_move_backward(allocator, finish + count, pos, finish);
                auto left_movement = boundary - pos;
                auto loop_break = left_movement < count ? left_movement : count;
                for (size_type i = 0; i < loop_break; ++i) {    // 全部都是赋值
                    *(pos++) = ciel::forward<Args>(args);
                }
                if (left_movement < count) {    // 赋值完还有一部分需要构造
                    pos = alloc_range_construct_n(allocator, boundary, count - left_movement, ciel::forward<Args>(args)) - 1;
                }
                return pos;
            }
        }

	public:
		deque() : allocator(), map(1, alloc_traits::allocate(allocator, SubarraySize), allocator), start(map.front(), map.begin()), finish(start) {}

		explicit deque(const allocator_type& alloc) : allocator(alloc), map(1, alloc_traits::allocate(allocator, SubarraySize), allocator), start(map.front(), map.begin()), finish(start) {}

		deque(size_type count, const T& value, const allocator_type& alloc = allocator_type()) : allocator(alloc), map(allocator) {
			size_type s = count / SubarraySize + 1;
			while (s--) {
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			start = iterator(map.front(), map.begin());
			finish = alloc_range_construct_n(allocator, start, count, value);
		}

		explicit deque(size_type count, const allocator_type& alloc = allocator_type()) : allocator(alloc), map(allocator) {
			size_type s = count / SubarraySize + 1;
			while (s--) {
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			start = iterator(map.front(), map.begin());
			finish = alloc_range_construct_n(allocator, start, count);
		}

		template<ciel::legacy_input_iterator InputIt>
		deque(InputIt first, InputIt last, const allocator_type& alloc = allocator_type()) : allocator(alloc), map(allocator) {
			size_type s = ciel::distance(first, last) / SubarraySize + 1;
			while (s--) {
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			start = iterator(map.front(), map.begin());
			finish = alloc_range_construct(allocator, start, first, last);
		}

		deque(const deque& other) : deque(other.begin(), other.end(), alloc_traits::select_on_container_copy_construction(other.get_allocator())) {}

		deque(const deque& other, const allocator_type& alloc) : deque(other.begin(), other.end(), alloc) {}

		deque(deque&& other) noexcept : deque() {
			swap(other);
		}

		deque(deque&& other, const allocator_type& alloc) : deque() {
            if (alloc == other.get_allocator()) {
                swap(other);
                allocator = alloc;
            } else {
                deque(other, alloc);
            }
		}

		deque(std::initializer_list<T> init, const allocator_type& alloc = allocator_type()) : deque(init.begin(), init.end(), alloc) {}

		~deque() {
			clear();
			for (auto ptr : map) {
				alloc_traits::deallocate(allocator, ptr, SubarraySize);
			}
		}

		// 若 alloc_traits::propagate_on_container_copy_assignment::value 为 true ，则用 other 的分配器的副本替换 *this 的分配器
		// 若 *this 的分配器在赋值后将与其旧值比较不相等，则用旧分配器解分配内存，然后在复制元素前用新分配器分配内存
		// 否则，在可行时可能复用 *this 所拥有的内存
		deque& operator=(const deque& other) {
			if (this == ciel::addressof(other)) {
				return *this;
			}
			if (alloc_traits::propagate_on_container_copy_assignment::value) {
				if (allocator != other.allocator) {
					{
						deque tmp(other.allocator);
						swap(tmp);
					}
					clear_and_get_cap_no_less_than(other.size());
					finish = alloc_range_construct(allocator, start, other.begin(), other.end());
					return *this;
				}
				allocator = other.allocator;
			}
			clear_and_get_cap_no_less_than(other.size());
			finish = alloc_range_construct(allocator, start, other.begin(), other.end());
			return *this;
		}

		// 若 alloc_traits::propagate_on_container_move_assignment::value 为 true ，则用 other 的分配器的副本替换 *this 的分配器
		// 若它为 false 且 *this 与 other 的分配器不比较相等，则 *this 不能接管 other 所拥有的内存的所有权且必须单独地移动赋值每个元素，并用其自身的分配器按需分配额外内存
		deque& operator=(deque&& other) noexcept(alloc_traits::is_always_equal::value) {
			if (this == ciel::addressof(other)) {
				return *this;
			}
            if (!alloc_traits::propagate_on_container_move_assignment::value && allocator != other.allocator) {
                clear_and_get_cap_no_less_than(other.size());
                finish = alloc_range_construct(allocator, start, other.begin(), other.end());
                return *this;
            }
            if (alloc_traits::propagate_on_container_move_assignment::value) {
                allocator = other.allocator;
            }
            clear();
            swap(other);
            return *this;
		}

		deque& operator=(std::initializer_list<T> ilist) {
			clear_and_get_cap_no_less_than(ilist.size());
			finish = alloc_range_construct(allocator, start, ilist.begin(), ilist.end());
			return *this;
		}

		void assign(size_type count, const T& value) {
			clear_and_get_cap_no_less_than(count);
			finish = alloc_range_construct_n(allocator, start, count, value);
		}

		template<ciel::legacy_input_iterator InputIt>
		void assign(InputIt first, InputIt last) {
			clear_and_get_cap_no_less_than(ciel::distance(first, last));
			finish = alloc_range_construct(allocator, start, first, last);
		}

		void assign(std::initializer_list<T> ilist) {
			clear_and_get_cap_no_less_than(ilist.size());
			finish = alloc_range_construct(allocator, start, ilist.begin(), ilist.end());
		}

		allocator_type get_allocator() const noexcept {
			return allocator;
		}

		reference at(size_type pos) {
			if (pos >= size()) {
				throw std::out_of_range("ciel::deque 成员函数 at 越界");
			}
			return (*this)[pos];
		}

		const_reference at(size_type pos) const {
			if (pos >= size()) {
				throw std::out_of_range("ciel::deque 成员函数 at 越界");
			}
			return (*this)[pos];
		}

		reference operator[](size_type pos) {
			return *(start + pos);
		}

		const_reference operator[](size_type pos) const {
			return *(start + pos);
		}

		reference front() {
			return *begin();
		}

		const_reference front() const {
			return *begin();
		}

		reference back() {
			return *ciel::prev(end());
		}

		const_reference back() const {
			return *ciel::prev(end());
		}

		iterator begin() noexcept {
			return start;
		}

		const_iterator begin() const noexcept {
			return start;
		}

		const_iterator cbegin() const noexcept {
			return begin();
		}

		iterator end() noexcept {
			return finish;
		}

		const_iterator end() const noexcept {
			return finish;
		}

		const_iterator cend() const noexcept {
			return end();
		}

		reverse_iterator rbegin() noexcept {
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const noexcept {
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const noexcept {
			return rbegin();
		}

		reverse_iterator rend() noexcept {
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const noexcept {
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crend() const noexcept {
			return rend();
		}

		[[nodiscard]] bool empty() const noexcept {
			return begin() == end();
		}

		[[nodiscard]] size_type size() const noexcept {
			return ciel::distance(begin(), end());
		}

		[[nodiscard]] size_type max_size() const noexcept {
			return ciel::numeric_limits<difference_type>::max();
		}

		void shrink_to_fit() {
			while (map.begin() != start.node) {
				alloc_traits::deallocate(allocator, map.front(), SubarraySize);
				map.pop_front();
			}
			while (map.end().prev() != finish.node) {
				alloc_traits::deallocate(allocator, map.back(), SubarraySize);
				map.pop_back();
			}
		}

		void clear() noexcept {
			finish = alloc_range_destroy(allocator, begin(), end());
		}

        iterator insert(const_iterator pos, const T& value) {
            return insert_n(pos, 1, value);
        }

        iterator insert(const_iterator pos, T&& value) {
            return insert_n(pos, 1, ciel::move(value));
        }

        iterator insert(const_iterator pos, size_type count, const T& value) {
            return insert_n(pos, count, value);
        }

        template<ciel::legacy_input_iterator InputIt>
        iterator insert(const_iterator pos, InputIt first, InputIt last) {
            size_type count = distance(first, last);
            if (!count) {
                return pos;
            }
            auto front_distance = ciel::distance(begin(), iterator(pos));
            auto back_distance = ciel::distance(iterator(pos), end());
            if (front_distance < back_distance) {  // 头部往前移动
                size_type bs = front_space();
                if (count > bs) {
                    bs = (count - bs) / SubarraySize + 1;
                    while (bs--) {
                        map.emplace_front(alloc_traits::allocate(allocator, SubarraySize));
                    }
                }

                iterator boundary = start;
                iterator res = pos - 1;
                start = alloc_range_nothrow_move(allocator, start - count, start, pos);

                iterator insert_begin = start + front_distance;

                auto left_movement = pos - boundary;

                auto loop_break = left_movement < count ? left_movement : count;
                if (left_movement < count) {
                    insert_begin = alloc_range_construct(allocator, insert_begin, first, first + count - left_movement);
                }
                ciel::copy_n(first + count - left_movement, loop_break, insert_begin);
                return res;
            } else {    // 尾部往后移动
                size_type bs = back_space();
                if (count >= bs) {
                    bs = (count - bs) / SubarraySize + 1;
                    while (bs--) {
                        map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
                    }
                }

                iterator boundary = finish;
                finish = alloc_range_nothrow_move_backward(allocator, finish + count, pos, finish);
                auto left_movement = boundary - pos;
                auto loop_break = left_movement < count ? left_movement : count;
                ciel::copy_n(first, loop_break, iterator(pos));
                if (left_movement < count) {    // 赋值完还有一部分需要构造
                    pos = alloc_range_construct(allocator, boundary, first + left_movement, last) - 1;
                }
                return pos;
            }
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template<class... Args>
        iterator emplace(const_iterator pos, Args&&... args) {
            if (pos == begin()) {
                emplace_front(ciel::forward<Args>(args)...);
                return begin();
            } else if (pos == end()) {
                emplace_back(ciel::forward<Args>(args)...);
                return iterator(pos);
            } else {
                return insert_n(pos, 1, value_type(ciel::forward<Args>(args)...));
            }
        }

        iterator erase(const_iterator pos) {
            return erase(pos, pos + 1);
        }

        iterator erase(const_iterator first, const_iterator last) {
            auto distance = ciel::distance(first, last);
            if (!distance) {
                return last;
            }
            if (ciel::distance(begin(), iterator(first)) < ciel::distance(iterator(last), end())) {
                iterator old_begin = start;
                start = ciel::move_backward(begin(), iterator(first), iterator(last));
                alloc_range_destroy(allocator, old_begin, start);
            } else {
                iterator new_end = ciel::move(iterator(last), end(), iterator(first));
                finish = alloc_range_destroy(allocator, new_end, finish);
            }
            return end();
        }

		void push_back(const T& value) {
			emplace_back(value);
		}

		void push_back(T&& value) {
			emplace_back(ciel::move(value));
		}

		template<class... Args>
		reference emplace_back(Args&& ... args) {
			if (finish.cur == finish.finish() - 1 && finish.node == map.end().prev()) {    // 这里注意不要 finish.cur + 1 == finish.finish()，因为运算符重载里所有到 finish() 的地方会直接跳到下个小数组的头部
				map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
			}
			finish = alloc_range_construct_n(allocator, finish, 1, ciel::forward<Args>(args)...);
			return back();
		}

		void pop_back() noexcept {
			finish = alloc_range_destroy(allocator, finish - 1, finish);
		}

		void push_front(const T& value) {
			emplace_front(value);
		}

		void push_front(T&& value) {
			emplace_front(ciel::move(value));
		}

		template<class... Args>
		reference emplace_front(Args&& ... args) {
			if (start.cur == start.start() && start.node == map.begin()) {
				map.emplace_front(alloc_traits::allocate(allocator, SubarraySize));
			}
			alloc_range_construct_n(allocator, start - 1, 1, ciel::forward<Args>(args)...);
            --start;
			return front();
		}

		void pop_front() noexcept {
			alloc_range_destroy(allocator, start, start + 1);
			++start;
		}

		void resize(size_type count) {
            if (size() >= count) {
                finish = alloc_range_destroy(allocator, finish - (size() - count), finish);
            } else {
                size_type needed_space = count - size();
                size_type bs = back_space();
                if (needed_space >= bs) {
                    bs = (needed_space - bs) / SubarraySize + 1;
                    while (bs--) {
                        map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
                    }
                }
                finish = alloc_range_construct_n(allocator, finish, needed_space);
            }
        }

		void resize(size_type count, const value_type& value) {
            if (size() >= count) {
                finish = alloc_range_destroy(allocator, finish - (size() - count), finish);
            } else {
                size_type needed_space = count - size();
                size_type bs = back_space();
                if (needed_space >= bs) {
                    bs = (needed_space - bs) / SubarraySize + 1;
                    while (bs--) {
                        map.emplace_back(alloc_traits::allocate(allocator, SubarraySize));
                    }
                }
                finish = alloc_range_construct_n(allocator, finish, needed_space, value);
            }
        }

		void swap(deque& other) noexcept(alloc_traits::is_always_equal::value) {
			ciel::swap(map, other.map);
			ciel::swap(start, other.start);
			ciel::swap(finish, other.finish);
			ciel::swap(allocator, other.allocator);
		}

	};    // class deque

    template<class T, class Alloc>
    constexpr bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs) {
        if (lhs.size() != rhs.size()) {
            return false;
        }
        return ciel::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template<class T, class Alloc>
    void swap(deque<T, Alloc>& lhs, deque<T, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }

    template<class T, class Alloc, class U>
    typename deque<T, Alloc>::size_type erase(deque<T, Alloc>& c, const U& value) {
        auto it = ciel::remove(c.begin(), c.end(), value);
        auto r = ciel::distance(it, c.end());
        c.erase(it, c.end());
        return r;
    }

    template<class T, class Alloc, class Pred>
    typename deque<T, Alloc>::size_type erase_if(deque<T, Alloc>& c, Pred pred) {
        auto it = ciel::remove_if(c.begin(), c.end(), pred);
        auto r = ciel::distance(it, c.end());
        c.erase(it, c.end());
        return r;
    }

    template<ciel::legacy_input_iterator InputIt, class Alloc = ciel::allocator<typename ciel::iterator_traits<InputIt>::value_type>>
    deque(InputIt, InputIt, Alloc = Alloc()) -> deque<typename ciel::iterator_traits<InputIt>::value_type, Alloc>;

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_DEQUE_H_