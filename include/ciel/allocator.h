#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALLOCATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALLOCATOR_H_

#include <ciel/utility_impl/forward.h>
#include <stddef.h>
#include <limits>

namespace ciel{

	template<class T>
	class allocator{

	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T&;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

	public:
		allocator() = default;

		template<class U>
		explicit allocator(const allocator<U>& other_allocator){
		}

		~allocator() = default;

		template<class U>
		struct rebind{
			using other=allocator<U>;
		};
//      Example:using AllocatorForDouble=ciel::allocator<int>::rebind<double>::other;

		static pointer allocate(size_type objectNum){
			return static_cast<pointer>(::operator new(sizeof(T) * objectNum));
		}

		static pointer allocate(size_type objectNum, const void* hint){
			return allocate(objectNum);
		}

		static void deallocate(pointer ptr){
			operator delete(ptr);
		}

		static void deallocate(pointer ptr, size_type objectNum){
			deallocate(ptr);
		}

		template<class U, class... Args>
		static void construct(U* ptr, Args&& ...args){
			new(ptr) U(forward<Args>(args)...);
		}

		template<class U>
		static void destroy(U* ptr){
			ptr->~U();
		}

		template<class U>
		static void destroy(U* start_spot, U* finish_spot){
			for (U* i = start_spot; i != finish_spot; ++i){
				destroy(i);
			}
		}

		static size_type max_size(){
			return std::numeric_limits<size_type>::max() / sizeof(value_type);
		}

	};  //class allocator

	template<class T>
	bool operator==(allocator<T> first_allocator, allocator<T> second_allocator){
		return true;
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ALLOCATOR_H_