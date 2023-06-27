#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_SORTED_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_SORTED_H_

#include <ciel/algorithm_impl/is_sorted_until.h>

namespace ciel {

    template<ciel::legacy_forward_iterator ForwardIt>
    bool is_sorted(ForwardIt first, ForwardIt last) {
        return ciel::is_sorted_until(first, last) == last;
    }

    template<ciel::legacy_forward_iterator ForwardIt, class Compare>
    bool is_sorted(ForwardIt first, ForwardIt last, Compare comp) {
        return ciel::is_sorted_until(first, last, comp) == last;
    }

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_IS_SORTED_H_