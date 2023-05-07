#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_NEW_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_NEW_H_

#include <ciel/exception.h>

namespace ciel {

	class bad_alloc : public exception {
	public:
		bad_alloc() noexcept = default;

		bad_alloc(const bad_alloc&) noexcept = default;

		bad_alloc& operator=(const bad_alloc& other) noexcept = default;

		virtual const char* what() const noexcept {
			return "ciel::bad_alloc";
		}

		virtual ~bad_alloc() noexcept = default;

	};    //class bad_alloc

	class bad_array_new_length : public bad_alloc {
	public:
		bad_array_new_length() noexcept = default;

		bad_array_new_length(const bad_array_new_length&) noexcept = default;

		bad_array_new_length& operator=(const bad_array_new_length&) noexcept = default;

		virtual const char* what() const noexcept {
			return "ciel::bad_array_new_length";
		}

		virtual ~bad_array_new_length() noexcept = default;

	};    //class bad_array_new_length

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_NEW_H_