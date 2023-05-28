#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_CONCEPT_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_CONCEPT_H_

#include <ciel/iterator_impl/iterator_traits.h>

namespace ciel {

	namespace iter_concept_details {
		//若特化 iterator_traits<I> 从主模板生成，则令 iter_traits<I> 指代 I ，否则指代 iterator_traits<I>
		//		也就是说原生指针走的是泛化，其它走的是特化
		//检测模板是否被用户特化的简单直接的方法就是为主模板定义一个标识性的成员类型（Clang libc++ 在 iterator_traits 里定义了 __primary_template
		template<class I>
		struct iter_traits : ciel::iterator_traits<I> {};

		template<class I> requires requires { typename ciel::iterator_traits<I>::primary_template; }
		struct iter_traits<I> : I {};

		//三种可能
		template<class I>
		struct iter_concept_helper3 {};
		template<class I>
		struct iter_concept_helper2 : iter_concept_helper3<I> {};
		template<class I>
		struct iter_concept_helper1 : iter_concept_helper2<I> {};

		//若 iter_traits<I>::iterator_concept 合法并指名类型，则 iter_concept<I> 指代该类型。
		template<class I> requires requires { typename iter_traits<I>::iterator_concept; }
		struct iter_concept_helper1<I> {
			using type = iter_traits<I>::iterator_concept;
		};

		//否则，若 iter_traits<I>::iterator_category 合法并指名类型，则 iter_concept<I> 指代该类型。
		template<class I> requires requires { typename iter_traits<I>::iterator_category; }
		struct iter_concept_helper2<I> {
			using type = iter_traits<I>::iterator_category;
		};

		//否则，若 iterator_traits<I> 从主模板生成，则 iter_concept<I> 指代 random_access_iterator_tag 。
		template<class I> requires requires { typename ciel::iterator_traits<I>::primary_template; }
		struct iter_concept_helper3<I> {
			using type = ciel::random_access_iterator_tag;
		};
	}

	template<class I>
	using iter_concept = iter_concept_details::iter_concept_helper1<I>::type;

}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_ITERATOR_IMPL_ITER_CONCEPT_H_