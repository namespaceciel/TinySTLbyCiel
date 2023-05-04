#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_NEW_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_NEW_H_

#include <ciel/exception.h>

namespace ciel {

	class bad_alloc : public exception {
	public:
		bad_alloc() noexcept;

		bad_alloc( const bad_alloc&) noexcept;

		bad_alloc& operator=( const bad_alloc& other ) noexcept;

		virtual const char* what() const noexcept;

		virtual ~bad_alloc() noexcept;

	};    //class bad_alloc

	class bad_array_new_length : public bad_alloc {
	public:
		bad_array_new_length() noexcept;

		virtual const char* what() const noexcept;

		virtual ~bad_array_new_length() noexcept;

	};    //class bad_array_new_length

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_NEW_H_