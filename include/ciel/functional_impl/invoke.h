#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_INVOKE_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_INVOKE_H_

#include <ciel/type_traits_impl/is_function.h>
#include <ciel/type_traits_impl/is_base_of.h>
#include <ciel/type_traits_impl/decay.h>
#include <ciel/utility.h>
#include <ciel/type_traits_impl/is_object.h>

namespace ciel {


		template<class>
		constexpr bool is_reference_wrapper_v = false;
		template<class U>
		constexpr bool is_reference_wrapper_v<ciel::reference_wrapper<U>> = true;

		template<class C, class Pointed, class T1, class... Args>
		constexpr decltype(auto) invoke_memptr(Pointed C::* f, T1&& t1, Args&&... args)
		{
			if constexpr (ciel::is_function_v<Pointed>)
			{
				if constexpr (ciel::is_base_of_v<C, ciel::decay_t<T1>>)
					return (ciel::forward<T1>(t1).*f)(ciel::forward<Args>(args)...);
				else if constexpr (is_reference_wrapper_v<ciel::decay_t<T1>>)
				return (t1.get().*f)(ciel::forward<Args>(args)...);
				else
				return ((*ciel::forward<T1>(t1)).*f)(ciel::forward<Args>(args)...);
			}
			else
			{
				static_assert(ciel::is_object_v<Pointed> && sizeof...(args) == 0);
				if constexpr (ciel::is_base_of_v<C, ciel::decay_t<T1>>)
					return ciel::forward<T1>(t1).*f;
				else if constexpr (is_reference_wrapper_v<ciel::decay_t<T1>>)
				return t1.get().*f;
				else
				return (*ciel::forward<T1>(t1)).*f;
			}
		}


}   //namespace ciel

#endif //TINYSTLBYCIEL_INCLUDE_CIEL_FUNCTIONAL_IMPL_INVOKE_H_
