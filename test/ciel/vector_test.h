#ifndef TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_

#include <iostream>
#include <ciel/vector.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace vector_test_details {
	struct S1 {
		int id;	//移动赋值后变为 0，析构变为 -1，用于检查 double free
		~S1(){
			if(id==-1){
				std::cout << "double free'\n";
			}
			id=-1;
		}
	};
}

void vector_test() {
	using namespace vector_test_details;
	// 这里对 vector 分 1、基础操作的正确性，2、复制与移动行为的正确性，3、异常安全 三方面进行测试

	// 1、基础操作的正确性
	{
		{
			ciel::vector<int> v1;
			CHECK(v1.empty());
			CHECK(v1.size() == 0);
			CHECK(v1.capacity() == 0);
		}
		{
			ciel::vector<int> v1(10);
			CHECK(v1.size() == 10);
			for (int i : v1) {
				CHECK(i == 0);
			}
			for (auto it = v1.begin(); it != v1.end(); ++it) {
				CHECK(*it == 0);
			}
			for (auto it = v1.rbegin(); it != v1.rend(); ++it) {
				CHECK(*it == 0);
			}
			v1.clear();
			CHECK(v1.empty());

			ciel::vector<int> v2(10, 5);
			CHECK(v2.size() == 10);
			for (int i : v2) {
				CHECK(i == 5);
			}
			for (auto it = v2.begin(); it != v2.end(); ++it) {
				CHECK(*it == 5);
			}
			for (auto it = v2.rbegin(); it != v2.rend(); ++it) {
				CHECK(*it == 5);
			}
			v1 = v2;
			for (int i : v1) {
				CHECK(i == 5);
			}
			v1.clear();
			v1 = ciel::move(v2);
			CHECK(v2.empty());
			CHECK(v2.capacity() == 0);
			for (int i : v1) {
				CHECK(i == 5);
			}

			v1.resize(1);
			CHECK(v1.size() == 1);
			v1.resize(10, 77);
			CHECK(v1.size() == 10);
			CHECK(v1.back() == 77);

			v1.pop_back();
			CHECK(v1.size() == 9);

			v1.push_back(123);
			CHECK(v1.size() == 10);
			CHECK(v1.back() == 123);

			v1.shrink_to_fit();
			CHECK(v1.size() == 10);

			v1.reserve(98);
			CHECK(v1.size() == 10);
			CHECK(v1.capacity() == 98);

			v1.emplace_back(987);
			CHECK(v1.size() == 11);
			CHECK(v1.back() == 987);

			v1.push_back(v1.size());
			CHECK(v1.size() == 12);
			CHECK(v1.back() == 11);

			v1.shrink_to_fit();
			v1.push_back(v1[0]);    //自引用
			CHECK(v1.size() == 13);
			CHECK(v1.back() == 5);
		}
		{
			ciel::vector<int> v1{654};
			CHECK(v1.capacity() == 1);
			v1.push_back(v1[0]);    //自引用
			CHECK(v1[0] == 654);
			CHECK(v1[1] == 654);
		}
		{
			ciel::vector<int> v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
			for (size_t i = 0; i < v1.size(); ++i) {
				CHECK(v1[i] == i);
				CHECK(v1.at(i) == i);
			}

			ciel::vector<int> v2(v1.begin(), v1.end() - 2);
			CHECK(v2.size() == 8);
			for (size_t i = 0; i < v2.size(); ++i) {
				CHECK(v2[i] == i);
			}
			CHECK(v2.front() == 0);
			CHECK(v2.back() == 7);

			ciel::vector<int> v3(ciel::move(v1));
			for (size_t i = 0; i < v3.size(); ++i) {
				CHECK(v3[i] == i);
			}
		}
		{
			ciel::vector<int> v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

			v1.erase(v1.begin());
			// 1, 2, 3, 4, 5, 6, 7, 8, 9
			CHECK(v1.size() == 9);
			CHECK(v1.front() == 1);
			CHECK(v1[2] == 3);
			CHECK(v1[6] == 7);

			v1.erase(v1.begin() + 4, v1.begin() + 7);
			// 1, 2, 3, 4, 8, 9
			CHECK(v1.size() == 6);
			CHECK(v1.back() == 9);
			CHECK(v1[3] == 4);
			CHECK(v1[4] == 8);

			v1.emplace(v1.begin(), 8);
			// 8, 1, 2, 3, 4, 8, 9
			CHECK(v1.size() == 7);
			CHECK(v1[0] == 8);
			CHECK(v1[1] == 1);

			v1.insert(v1.begin() + 1, {4, 3, 2});
			// 8, 4, 3, 2, 1, 2, 3, 4, 8, 9
			CHECK(v1.size() == 10);
			CHECK(v1[1] == 4);
			CHECK(v1[3] == 2);
			CHECK(v1[4] == 1);
			CHECK(v1.back() == 9);

			ciel::vector<int> v2{123, 543, 12};
			v1.insert(v1.end(), v2.begin(), v2.begin());
			CHECK(v1.size() == 10);

			v1.insert(v1.end() - 1, v2.begin() + 1, v2.end());
			// 8, 4, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9
			CHECK(v1.size() == 12);
			CHECK(v1[v1.size() - 3] == 543);
			CHECK(v1[10] == 12);
			CHECK(v1.back() == 9);

			v1.insert(v1.begin() + 2, 3, 222);
			// 8, 4, 222, 222, 222, 3, 2, 1, 2, 3, 4, 8, 543, 12, 9
			CHECK(v1.size() == 15);
			CHECK(v1[2] == 222);
			CHECK(v1[3] == 222);
			CHECK(v1[4] == 222);

//			v1.insert(v1.begin(), v1.begin() + 1, v1.begin() + 3);    //不合规
		}
	}

	// 2、复制与移动行为的正确性
	// 3、异常安全

	std::cout << "All vector_tests finished.\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_VECTOR_TEST_H_