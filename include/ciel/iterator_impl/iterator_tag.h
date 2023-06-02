#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TAG_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TAG_H_

namespace ciel {

	struct input_iterator_tag {};

	struct output_iterator_tag {};

	struct forward_iterator_tag : input_iterator_tag {};

	struct bidirectional_iterator_tag : forward_iterator_tag {};

	struct random_access_iterator_tag : bidirectional_iterator_tag {};

	struct contiguous_iterator_tag : random_access_iterator_tag {};

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TAG_H_