#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_

#include <stddef.h>
#include <ciel/allocator.h>
#include <ciel/iterator.h>
#include <ciel/memory.h>    //fill()与fill_n()

namespace ciel{

	template<class T, class Allocator=allocator<T>>
	class vector{

	public:
		using value_type = T;
		using allocator_type = Allocator;
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

	private:
		iterator start;
		iterator finish;    //指向众对象尾部的后一个位置
		iterator end_of_storage;    //指向分配内存尾部的后一个位置

	private:
		void _move(iterator first,iterator last,iterator target){

		}

	public:
		vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr){
		}

		explicit vector(size_type N){
			start = allocator<T>::allocate(N);
			finish = fill_n(start,N,T());
			end_of_storage = finish;
		}

		explicit vector(size_type N, const_reference value){
			start = allocator<T>::allocate(N);
			finish = fill_n(start, N, value);
			end_of_storage = finish;
		}

		vector& operator=(const vector<T>& copied_vec){
			if(*this==copied_vec){
				return *this;
			}
			start = allocator<T>::allocate(copied_vec.capacity());
			finish = start;
			try{    //异常安全
				for (size_type i = 0; i < copied_vec.size(); ++i){
					allocator<T>::construct(start + i, *(copied_vec.begin() + i));
					++finish;
				}
			}
			catch (...){
				~vector();
				throw;
			}
			end_of_storage = start + copied_vec.capacity();
			return *this;
		}

		explicit vector(const vector<T>& copied_vec){
			*this=copied_vec;
		}

		vector& operator=(vector<T>&& moved_vec){
			if(*this==moved_vec){
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

		explicit vector(vector<T>&& moved_vec){
			*this=std::move(moved_vec);
		}

		~vector(){    //先一个个手动调用成员的析构函数，再由vector自己释放内存
			if (start){
				for (--finish; finish >= start; --finish){
					allocator<T>::destroy(finish);
				}
				allocator<T>::deallocate(start);
			}
		}

		reference at(size_type index){
			if (index >= size()){
				throw std::out_of_range("vector成员函数at遇到越界问题");
			}
			return start[index];
		}

		reference operator[](size_type index){
			return start[index];
		}

		reference front(){
			return start[0];
		}

		reference back(){
			return *(finish-1);
		}

		T* data(){
			return start;
		}

		iterator begin(){
			return start;
		}

		const_iterator begin() const{
			return start;
		}

		const_iterator cbegin() const{
			return start;
		}

		iterator end(){
			return finish;
		}

		const_iterator end() const{
			return finish;
		}

		const_iterator cend() const{
			return finish;
		}

		reverse_iterator rbegin(){
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const{
			return const_reverse_iterator(end());
		}

		const_reverse_iterator crbegin() const{
			return const_reverse_iterator(cend());
		}

		reverse_iterator rend(){
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const{
			return const_reverse_iterator(begin());
		}

		const_reverse_iterator crend() const{
			return const_reverse_iterator(begin());
		}

		bool empty() const{
			return start == finish;
		}

		size_type size() const{
			return static_cast<size_type>(finish - start);
		}

		size_type capacity() const{
			return static_cast<size_type>(end_of_storage - start);
		}

		size_type max_size() const{
			return allocator<T>::max_size();
		}

		iterator erase( iterator pos );

		constexpr iterator erase( const_iterator pos );

		iterator erase( iterator first, iterator last );

		constexpr iterator erase( const_iterator first, const_iterator last );

		void clear(){
			if(!empty()){
				for (--finish; finish >= start; --finish){
					allocator<T>::destroy(finish);
				}
			}
		}

	};  //class vector

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_VECTOR_H_