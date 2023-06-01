#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H_

#include <ciel/iterator_impl/iterator_traits.h>
#include <ciel/memory_impl/addressof.h>
#include <ciel/memory_impl/destroy.h>

namespace ciel {

	template<class ForwardIt>
	void uninitialized_value_construct(ForwardIt first, ForwardIt last) {
		using Value = typename ciel::iterator_traits<ForwardIt>::value_type;
		ForwardIt current = first;
		try {
			for (; current != last; ++current) {
				::new(const_cast<void*>(static_cast<const volatile void*>(ciel::addressof(*current)))) Value();
			}
		} catch (...) {
			ciel::destroy(first, current);
			throw;
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_VALUE_CONSTRUCT_H_