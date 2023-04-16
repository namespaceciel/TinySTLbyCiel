#pragma once

#include <iostream>
#include "../include/array.h"

#define CHECK(x) if(!(x)){std::cerr<<__FUNCTION__<<"() Line "<<__LINE__<<" failed.";abort();}

void array_test(){
	using namespace ciel;

	array<int, 5> a = { 0, 1, 2, 3, 4 };
	array<int, 5> b = { 0, 1, 2, 3 };
	array<int, 5> c = { 4, 3, 2, 1, 0 };
	array<int, 0> d = {};

	CHECK(!a.empty());
	CHECK(a.size() == 5);
	CHECK(a[0] == 0);
	CHECK(a[4] == 4);

	CHECK(!b.empty());
	CHECK(b.size() == 5);
	CHECK(b[0] == 0);
	CHECK(b[3] == 3);

	CHECK(d.empty());
	CHECK(d.size() == 0);

	a.swap(c);
	CHECK(a[0] == 4);
	CHECK(c[0] == 0);

	array<int, 5>::iterator it = a.begin();
	CHECK(*it == 4);

	++it;
	CHECK(*it == 3);

	++it;
	CHECK(*it == 2);

	--it;
	CHECK(*it == 3);

	it += 3;
	CHECK(*it == 0);

	++it;
	CHECK(it == a.end());

	array<int, 5>::reverse_iterator itr = a.rbegin();
	CHECK(*itr == 0);

	itr++;
	CHECK(*itr == 1);

	int* pArray = a.data();
	CHECK(pArray == a.m_data);

	int& nFront = a.front();
	CHECK(nFront == 4);

	int& nBack = a.back();
	CHECK(nBack == 0);

	CHECK(a[0] == a.at(0));

	// global operators
	a[0] = 0;
	a[1] = 1;
	a[2] = 2;
	a[3] = 3;
	a[4] = 4;
	b[0] = 0;
	b[1] = 1;
	b[2] = 2;
	b[3] = 3;
	b[4] = 4;
	c[0] = 0;
	c[1] = 1;
	c[2] = 2;
	c[3] = 3;
	c[4] = 9;

	CHECK((a == b));
	CHECK(!(a != b));

	CHECK(!(a == c));
	CHECK((a != c));

	std::cout << __FUNCTION__ <<"() all tests passed.";
}