#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_SORTED_UNTIL_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_SORTED_UNTIL_H_

#include <ciel/iterator.h>

namespace ciel {

    template<ciel::legacy_forward_iterator ForwardIt>
    ForwardIt is_sorted_until(ForwardIt first, ForwardIt last) {
        return is_sorted_until(first, last, ciel::less<>());
    }

    template<ciel::legacy_forward_iterator ForwardIt, class Compare>
    ForwardIt is_sorted_until(ForwardIt first, ForwardIt last, Compare comp) {
        if (first != last) {
            ForwardIt next = first;
            while (++next != last) {
                if (comp(*next, *first)) {
                    return next;
                }
                first = next;
            }
        }
        return last;
    }

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_SORTED_UNTIL_H_