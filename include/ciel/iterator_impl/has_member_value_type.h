#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_HAS_MEMBER_VALUE_TYPE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_HAS_MEMBER_VALUE_TYPE_H_

namespace ciel {

	template<class T>
	concept has_member_value_type = requires { typename T::value_type; };

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_HAS_MEMBER_VALUE_TYPE_H_