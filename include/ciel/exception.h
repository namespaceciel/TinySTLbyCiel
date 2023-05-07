#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_EXCEPTION_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_EXCEPTION_H_

namespace ciel {

	class exception {
	public:
		exception() noexcept = default;

		exception(const exception&) noexcept = default;

		exception& operator=(const exception&) noexcept = default;

		virtual const char* what() const noexcept {
			return "ciel::exception";
		}

		virtual ~exception() noexcept = default;

	};    //class exception

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_EXCEPTION_H_