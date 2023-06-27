#ifndef TINYSTLBYCIEL_TEST_CIEL_QUEUE_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_QUEUE_TEST_H_

#include <iostream>
#include <algorithm>
#include <random>
#include <ciel/queue.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

void queue_test() {

	// queue
	{
		// TODO
	}

	// priority_queue
	{
		ciel::priority_queue<int> pq;
		CHECK(pq.empty());
		CHECK(pq.size() == 0);
		pq.push(1);
		CHECK(pq.top() == 1);
		pq.emplace(5);
		pq.push(5);
		CHECK(pq.size() == 3);
		CHECK(pq.top() == 5);
		CHECK(ciel::is_heap(pq.container().begin(), pq.container().end()));
		pq.pop();
		CHECK(pq.size() == 2);
		CHECK(pq.top() == 5);
	}
	{
		// 使用标准库 shuffle 打乱数次，检查 ciel::priority_queue 的排序
		std::random_device rd;
		std::mt19937 g(rd());

		ciel::vector<size_t> v;
		for (size_t i = 0; i < 10000; ++i) {
			v.emplace_back(i);
		}
		for (size_t loop = 0; loop < 100; ++loop) {
			std::shuffle(v.begin(), v.end(), g);

			ciel::priority_queue<size_t, ciel::vector<size_t>, ciel::greater<size_t>> pq(ciel::greater<size_t>(), ciel::move(v));
			CHECK(v.empty());
			CHECK(ciel::is_heap(pq.container().begin(), pq.container().end(), ciel::greater<size_t>()));

			while (!pq.empty()) {
				v.emplace_back(pq.top());
				pq.pop();
			}
			for (size_t i = 0; i < 10000; ++i) {
				CHECK(v[i] == i);
			}
		}
	}
	{
		// 使用标准库 shuffle 打乱数次，检查 ciel::sort_heap 的排序
		std::random_device rd;
		std::mt19937 g(rd());

		ciel::vector<size_t> v;
		for (size_t i = 0; i < 10000; ++i) {
			v.emplace_back(i);
		}
		for (size_t loop = 0; loop < 100; ++loop) {
			std::shuffle(v.begin(), v.end(), g);
			ciel::make_heap(v.begin(), v.end());
			ciel::sort_heap(v.begin(), v.end());
			for (size_t i = 0; i < 10000; ++i) {
				CHECK(v[i] == i);
			}
		}
	}

	std::cout << "All queue_tests finished.\n";
}

#endif // TINYSTLBYCIEL_TEST_CIEL_QUEUE_TEST_H_