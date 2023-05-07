#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TAG_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TAG_H_

namespace ciel {

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};
	struct contiguous_iterator_tag : public random_access_iterator_tag {};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITERATOR_TAG_H_