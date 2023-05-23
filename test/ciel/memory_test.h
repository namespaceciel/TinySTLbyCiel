#ifndef TINYSTLBYCIEL_TEST_CIEL_MEMORY_TEST_H_
#define TINYSTLBYCIEL_TEST_CIEL_MEMORY_TEST_H_

#include <iostream>
#include <ciel/memory.h>

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
			std::cout << "Working...\n";
			ptr.reset();
		}

		~Task() { std::cout << "Dying...\n"; }

	private:
		Task() = default;

		ciel::unique_ptr<Task> ptr;    //ptr == this

		void accept(ciel::unique_ptr<Task>&& tmp) {
			ptr = ciel::move(tmp);
		}
	};
}

void memory_test() {
	using namespace memory_test_details;

	//unique_ptr
	Task* t = Task::create_worker();
	t->work_and_die();
}

#endif //TINYSTLBYCIEL_TEST_CIEL_MEMORY_TEST_H_