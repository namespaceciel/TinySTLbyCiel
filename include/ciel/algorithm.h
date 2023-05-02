#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_H_

namespace ciel{

	template< class InputIt, class OutputIt >
	OutputIt copy( InputIt first, InputIt last, OutputIt d_first ){

	}

	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& filled_value){
		for (; first != last; ++first){
			*first = filled_value;
		}
	}

	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& filled_value){
		for (; n > 0; --n, ++first){
			*first = filled_value;
		}
		return first;
	}

} //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_H_