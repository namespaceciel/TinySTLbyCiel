#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_EQUALITY_COMPARABLE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_EQUALITY_COMPARABLE_H_

namespace ciel {

	template < class T >
	concept equality_comparable = __WeaklyEqualityComparableWith<T, T>;
	
	template <class T, class U>
	concept equality_comparable_with =
	ciel::equality_comparable<T> &&
	ciel::equality_comparable<U> &&
		ciel::common_reference_with<
		const ciel::remove_reference_t<T>&,
		const ciel::remove_reference_t<U>&> &&
	ciel::equality_comparable<
	ciel::common_reference_t<
		const ciel::remove_reference_t<T>&,
		const ciel::remove_reference_t<U>&>> &&
		__WeaklyEqualityComparableWith<T, U>;
	
	template<class T, class U>
	concept __WeaklyEqualityComparableWith = // 仅用于解释
	requires(const ciel::remove_reference_t<T>& t,
	const ciel::remove_reference_t<U>& u) {
{ t == u } -> boolean-testable;
{ t != u } -> boolean-testable;
{ u == t } -> boolean-testable;
{ u != t } -> boolean-testable;
};

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_CONCEPTS_IMPL_EQUALITY_COMPARABLE_H_
