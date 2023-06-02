#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_CONTIGUOUS_ITERATOR_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_CONTIGUOUS_ITERATOR_H_

#include <ciel/iterator_impl/random_access_iterator.h>
#include <ciel/memory.h>

namespace ciel {
	// 连续迭代器的作用体现在：如果有一个算法要求传入的是裸指针，那么连续迭代器足以满足其需求
	template<class I>
	concept contiguous_iterator = ciel::random_access_iterator<I>
		&& ciel::derived_from<ciel::iter_concept<I>, ciel::contiguous_iterator_tag>
		&& ciel::is_lvalue_reference_v<ciel::iter_reference_t<I>>
		&& ciel::same_as<ciel::iter_value_t<I>, ciel::remove_cvref_t<ciel::iter_reference_t<I>>>
		&& requires(const I& i) {{ ciel::to_address(i) } -> ciel::same_as<ciel::add_pointer_t<ciel::iter_reference_t<I>>>; };

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_CONTIGUOUS_ITERATOR_H_