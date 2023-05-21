#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_HAS_MEMBER_ELEMENT_TYPE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_HAS_MEMBER_ELEMENT_TYPE_H_

namespace ciel {

	template<class T>
	concept has_member_element_type = requires { typename T::element_type; };

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_HAS_MEMBER_ELEMENT_TYPE_H_