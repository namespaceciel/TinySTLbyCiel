#ifndef TINYSTLBYCIEL_TEST_CIEL_ITERATOR_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_ITERATOR_TEST_H_

#include <iostream>
#include <ciel/iterator.h>

namespace iterator_test_details {
	struct Struct {};
	struct IterHasFiveMembers {
		using difference_type = int;
		using value_type = long;
		using pointer = long long;
		using reference = char;
		using iterator_category = size_t;
	};
	struct IterHasFourMembers {
		using difference_type = int;
		using value_type = long;
		using reference = char;
		using iterator_category = size_t;
	};
/*
	struct InputIter{
		int* v;
		bool operator==(InputIter& other) const{return v==other.v;}
		bool operator!=(InputIter& other) const{return v!=other.v;}
		int operator*() const{return *v;}
	};
	struct ForwardIter:InputIter{
		int* operator++(){return nullptr;}
		int* operator++(int){return nullptr;}
	};
	struct BidirectionalIter:ForwardIter{
		int* operator--(){return nullptr;}
		int* operator--(int){return nullptr;}
	};
	struct RandomAccessIter:BidirectionalIter{
		int* operator+=(int){return nullptr;}
		int* operator-=(int){return nullptr;}
		int operator[](size_t){return 0;}
		int* operator+(int){return nullptr;}
		int* operator-(int){return nullptr;}
	};
	bool operator==(InputIter&,InputIter&){return true;}
	bool operator!=(InputIter&,InputIter&){return true;}
*/
}

void iterator_test() {
	using namespace iterator_test_details;

	//iterator_traits
	static_assert(ciel::is_same_v<ciel::iterator_traits<int*>::difference_type, ptrdiff_t>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<int*>::value_type, int>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<int*>::pointer, int*>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<int*>::reference, int&>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<int*>::iterator_category, ciel::random_access_iterator_tag>);

	static_assert(ciel::is_same_v<ciel::iterator_traits<const int*>::difference_type, ptrdiff_t>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const int*>::value_type, int>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const int*>::pointer, const int*>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const int*>::reference, const int&>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const int*>::iterator_category, ciel::random_access_iterator_tag>);

	static_assert(ciel::is_same_v<ciel::iterator_traits<const Struct*>::difference_type, ptrdiff_t>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const Struct*>::value_type, Struct>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const Struct*>::pointer, const Struct*>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const Struct*>::reference, const Struct&>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<const Struct*>::iterator_category, ciel::random_access_iterator_tag>);

	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFiveMembers>::difference_type, int>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFiveMembers>::value_type, long>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFiveMembers>::pointer, long long>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFiveMembers>::reference, char>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFiveMembers>::iterator_category, size_t>);

	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFourMembers>::difference_type, int>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFourMembers>::value_type, long>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFourMembers>::pointer, void>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFourMembers>::reference, char>);
	static_assert(ciel::is_same_v<ciel::iterator_traits<IterHasFourMembers>::iterator_category, size_t>);

	std::cout << "All iterator_tests finished.\n\n";
};

#endif //TINYSTLBYCIEL_TEST_CIEL_ITERATOR_TEST_H_