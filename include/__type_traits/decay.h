#ifndef TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_DECAY_H_
#define TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_DECAY_H_

#include "remove_reference.h"
#include "is_array.h"
#include "remove_extent.h"
#include "is_function.h"
#include "add_pointer.h"
#include "remove_cvref.h"
#include "conditional.h"

namespace ciel {

	template<class T>
	class decay {
	private:
		using U = remove_reference_t<T>;

	public:
		using type = conditional_t<is_array_v<U>,
								   remove_extent_t<U>*,
								   conditional_t<is_function_v<U>, add_pointer_t<U>, remove_cvref<U>>>;
	};

	template<class T>
	using decay_t = typename decay<T>::type;
}

#endif //TINYSTLBYCIEL_INCLUDE___TYPE_TRAITS_DECAY_H_