#ifndef TINYSTLBYCIEL_TEST_CIEL_MEMORY_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_MEMORY_TEST_H_

#include <iostream>
#include <ciel/memory.h>

#ifndef CHECK
#define CHECK(x) if (!(x)) { std::cerr << "Line " << __LINE__ << " : " << #x << " failed.\n"; }
#endif

namespace memory_test_details {
	struct Task {
	public:
		static Task* create_worker() {
			ciel::unique_ptr<Task> tmp{new Task{}};
			Task* res = tmp.get();
			res->accept(ciel::move(tmp));
			return res;
		}
		void work_and_die() {
//			std::cout << "Working...\n";
			ptr.reset();
		}
//		~Task() { std::cout << "Dying...\n"; }
	private:
		ciel::unique_ptr<Task> ptr;    //ptr == this
		void accept(ciel::unique_ptr<Task>&& tmp) {
			ptr = ciel::move(tmp);
		}
	};
	struct A {};
	struct B : A {};
	struct C : private B {};
	struct EmptyDeleter {
		void operator()(...) {}
	};

	void IntFunctionDeleter(int* ptr) {
		delete ptr;
	}
	struct StructOperatorDeleter {
		void operator()(int* ptr) {
			delete ptr;
		}
	};

	struct Constructor_And_Destructor_Counter {
		inline static size_t ctor = 0;
		inline static size_t dtor = 0;
		Constructor_And_Destructor_Counter() {
			++ctor;
		};
		~Constructor_And_Destructor_Counter() {
			++dtor;
		}
	};

	struct Loop_Check2;
	struct Loop_Check1 {
		ciel::shared_ptr<Loop_Check2> p;
		inline static size_t ctor = 0;
		inline static size_t dtor = 0;
		Loop_Check1() {
			++ctor;
		};
		~Loop_Check1() {
			++dtor;
		}
	};
	struct Loop_Check2 {
		ciel::weak_ptr<Loop_Check1> p;
		inline static size_t ctor = 0;
		inline static size_t dtor = 0;
		Loop_Check2() {
			++ctor;
		};
		~Loop_Check2() {
			++dtor;
		}
	};
}

void memory_test() {
	using namespace memory_test_details;

	// unique_ptr
	{
		Task* t = Task::create_worker();
		t->work_and_die();

		ciel::unique_ptr<int> ptr1;
		ciel::unique_ptr<int> ptr2(nullptr);
		int* id{};
		ciel::unique_ptr<int> ptr3(id);
		CHECK(ptr1 == nullptr);
		CHECK(ptr2 == nullptr);
		CHECK(ptr3 == nullptr);

		ciel::unique_ptr<int> ptr4(new int{1});
		CHECK(*ptr4 == 1);

		ciel::unique_ptr<int> ptr5(ciel::move(ptr4));
		CHECK(*ptr5 == 1);
		CHECK(ptr4 == nullptr);

		ciel::unique_ptr<B, EmptyDeleter> ptr6;
		ciel::unique_ptr<A, EmptyDeleter> ptr7(ciel::move(ptr6));

		ciel::unique_ptr<int> ptr8 = ciel::make_unique<int>(0);
		ciel::unique_ptr<A> ptr9 = ciel::make_unique<A>();

		ciel::unique_ptr<int[]> ptr10;
		ciel::unique_ptr<int[]> ptr11(ciel::move(ptr10));
		ciel::unique_ptr<int[]> ptr12(new int[]{0, 1, 2, 3, 4});
		for (size_t i = 0; i < 5; ++i) {
			CHECK(ptr12[i] == i);
		}
		ciel::swap(ptr11, ptr12);
		for (size_t i = 0; i < 5; ++i) {
			CHECK(ptr11[i] == i);
		}
		ptr10 = ciel::make_unique<int[]>(5);
		for (size_t i = 0; i < 5; ++i) {
			CHECK(ptr10[i] == 0);
		}

		int* arr = new int[]{0, 1, 2, 3, 4};
		ciel::unique_ptr<int[]> ptr13(arr);
		ciel::unique_ptr<int> ptr14(arr + 3);
		CHECK(ptr13 < ptr14);
		CHECK(ptr13 <= ptr14);
		CHECK(ptr13 != ptr14);
		ptr14.release();

		ciel::unique_ptr<ciel::unique_ptr<int>> ptr15(new ciel::unique_ptr<int>(new int(5)));
		ciel::unique_ptr<ciel::unique_ptr<int>> ptr16(ciel::move(ptr15));

		ciel::unique_ptr<int, void (*)(int*)> ptr17(new int{5}, IntFunctionDeleter);
		ciel::unique_ptr<int, StructOperatorDeleter> ptr18(new int{5}, StructOperatorDeleter{});
		ciel::unique_ptr<int, std::function<void(int*)>> ptr19(new int{5}, [](int* ptr) { delete ptr; });
		ciel::unique_ptr<int[], std::function<void(int*)>> ptr20(new int[]{1,5}, [](int* ptr) { delete[] ptr; });
	}

	// shared_ptr
	{
		using CADC = Constructor_And_Destructor_Counter;
		{
			ciel::shared_ptr<CADC> ptr;
		}
		CHECK(CADC::ctor == 0 && CADC::dtor == 0);
		{
			ciel::shared_ptr<CADC> ptr(new CADC);
		}
		CHECK(CADC::ctor == 1 && CADC::dtor == 1);
		{
			ciel::shared_ptr<CADC> ptr(new CADC);
			ciel::shared_ptr<CADC> ptr2(ptr);
		}
		CHECK(CADC::ctor == 2 && CADC::dtor == 2);
		{
			ciel::shared_ptr<CADC[]> ptr(new CADC[3]);
			ciel::shared_ptr<CADC[]> ptr2(ptr);
		}
		CHECK(CADC::ctor == 5 && CADC::dtor == 5);

		Loop_Check1* lc1 = new Loop_Check1;
		Loop_Check2* lc2 = new Loop_Check2;
		{
			ciel::shared_ptr<Loop_Check1> ptr1(lc1);
			ciel::shared_ptr<Loop_Check2> ptr2(lc2);
			ptr1->p = ptr2;
			ptr2->p = ptr1;
		}
		CHECK(Loop_Check1::ctor == 1 && Loop_Check1::dtor == 1 && Loop_Check2::ctor == 1 && Loop_Check2::dtor == 1);
	}

	std::cout << "All memory_tests finished.\n";
}

#endif //TINYSTLBYCIEL_TEST_CIEL_MEMORY_TEST_H_