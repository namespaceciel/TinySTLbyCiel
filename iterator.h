//通俗的说下面定义了五个迭代器类型空类，然后通过iterator_traits的模板偏特化提取出迭代器类型
//reverse_iterator接受任意类型的迭代器并且重写运算符的运算方向

#pragma once

#include <cstddef>

namespace ciel{
	//五种迭代器
	class input_iterator_tag{
	};    //class input_iterator_tag
	class output_iterator_tag{
	};    //class output_iterator_tag
	class forward_iterator_tag : public input_iterator_tag{
	};    //class forward_iterator_tag
	class bidirectional_iterator_tag : public forward_iterator_tag{
	};    //class bidirectional_iterator_tag
	class random_access_iterator_tag : public bidirectional_iterator_tag{
	};    //class random_access_iterator_tag


	template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	class iterator    //所有迭代器的父类
	{
	public:
		using iterator_category = Category;
		using value_type = T;
		using difference_type = Distance;
		using pointer = Pointer;
		using reference = Reference;
	};    //class iterator


	template<class Iterator>
	class iterator_traits{
	public:
		using iterator_category = typename Iterator::iterator_category;
		using value_type = typename Iterator::value_type;
		using difference_type = typename Iterator::difference_type;
		using pointer = typename Iterator::pointer;
		using reference = typename Iterator::reference;
	};    //class iterator_traits

	template<class T>
	class iterator_traits<T*>{
	public:
		using iterator_category = random_access_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = T*;
		using reference = T&;
	};    //class iterator_traits<T*>

	template<class T>
	class iterator_traits<const T*>{
	public:
		using iterator_category = random_access_iterator_tag;
		using value_type = T;
		using difference_type = ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;
	};    //class iterator_traits<const T*>


	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&){
		return typename iterator_traits<Iterator>::iterator_category();//默认构造器
	}


	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)//为什么这里是*
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}


	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&)//为什么这里是*
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}


	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type _distance(InputIterator first, InputIterator last, input_iterator_tag){
//		iterator_traits<InputIterator>::difference_type n = 0;
		ptrdiff_t n = 0;
		while (first != last){
			++first;
			++n;
		}
		return n;
	}

	template<class RamdomAccessIterator>
	inline typename iterator_traits<RamdomAccessIterator>::difference_type _distance(RamdomAccessIterator first, RamdomAccessIterator last, random_access_iterator_tag){
		return last - first;
	}

	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last){
		return _distance(first, last, typename iterator_traits<InputIterator>::iterator_category());
	}

	template<class InputIterator, class Distance>
	inline void _advance(InputIterator& i, Distance n, input_iterator_tag){
		while (n--){
			++i;
		}
	}

	template<class BidirectionalIterator, class Distance>
	inline void _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag){
		if (n >= 0){
			while (n--){
				++i;
			}
		}
		else{
			while (n++){
				--i;
			}
		}
	}

	template<class RandomAccessIterator, class Distance>
	inline void _advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag){
		i += n;
	}

	template<class InputIterator, class Distance>
	inline void advance(InputIterator& i, Distance n){
		_advance(i, n, iterator_traits<InputIterator>::iterator_category());//STL源码剖析这里默认构造里加了参数i
	}

	//reverse_iterator
	template<class InputIterator>
	class reverse_iterator{
	private:
		InputIterator cur;

	public:
		using iterator_category = typename iterator_traits<InputIterator>::iterator_category;
		using value_type = typename iterator_traits<InputIterator>::value_type;
		using difference_type = typename iterator_traits<InputIterator>::difference_type;
		using pointer = typename iterator_traits<InputIterator>::pointer;
		using reference = typename iterator_traits<InputIterator>::reference;

		using iterator_type = InputIterator;
		using self = reverse_iterator<InputIterator>;

	public:
		reverse_iterator() = default;

		explicit reverse_iterator(iterator_type I) :cur(I){
		}

		reverse_iterator(const self& RI) :cur(RI.cur){
		}

		iterator_type base() const{
			return cur;
		}

		reference operator*() const{
			return *(cur - 1);
		}

		pointer operator->() const{
			return &(operator*());
		}

		self& operator++(){
			--cur;
			return *this;
		}

		self operator++(int){
			self temp = *this;
			--cur;
			return temp;
		}

		self& operator--(){
			++cur;
			return *this;
		}

		self operator--(int){
			self temp = *this;
			++cur;
			return temp;
		}

		self operator+(difference_type n) const{
			return cur - n;
		}

		self& operator+=(difference_type n) const{
			cur -= n;
			return *this;
		}

		self operator-(difference_type n) const{
			return cur + n;
		}

		self& operator-=(difference_type n) const{
			cur += n;
			return *this;
		}

		reference operator[](difference_type n) const{
			return *(*this + n);
		}

	};    //class reverse_iterator

	template<class Iterator1, class Iterator2>
	inline bool operator==(const reverse_iterator<Iterator1>& left_it, const reverse_iterator<Iterator2>& right_it){
		return left_it.base() == right_it.base();
	}

	template<class Iterator1, class Iterator2>
	inline bool operator!=(const reverse_iterator<Iterator1>& left_it, const reverse_iterator<Iterator2>& right_it){
		return !(left_it == right_it);
	}

}   //namespace ciel