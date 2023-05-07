#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_COPY_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_COPY_H_

namespace ciel {

	template<class InputIt, class NoThrowForwardIt>
	NoThrowForwardIt uninitialized_copy(InputIt first, InputIt last, NoThrowForwardIt d_first)
	{
		using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
		NoThrowForwardIt current = d_first;
		try {
			for (; first != last; ++first, (void) ++current) {
				::new (static_cast<void*>(std::addressof(*current))) T(*first);
			}
			return current;
		} catch (...) {
			for (; d_first != current; ++d_first) {
				d_first->~T();
			}
			throw;
		}
	}

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_MEMORY_IMPL_UNINITIALIZED_COPY_H_