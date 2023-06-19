#ifndef TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SIFT_DOWN_H_
#define TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SIFT_DOWN_H_

#include <ciel/iterator.h>

namespace ciel {

	template<ciel::legacy_random_access_iterator RandomIt, class Compare>
	constexpr void sift_down(RandomIt first, Compare&& comp, typename ciel::iterator_traits<RandomIt>::difference_type len, RandomIt head) {    // head 为这次下沉的父节点
		using value_type = typename ciel::iterator_traits<RandomIt>::value_type;
		using difference_type = typename ciel::iterator_traits<RandomIt>::difference_type;
		// 当父节点下标为 i，左右子节点下标分别为 2i + 1, 2i + 2
		difference_type diff = head - first;
		if (len < 2 || (len - 2) / 2 < diff) {
			return;
		}
		diff = diff * 2 + 1;    // 左孩子下标偏移量
		RandomIt left_child = first + diff;

		if (diff + 1 < len && comp(*left_child, *(left_child + difference_type(1)))) {    // 右孩子存在且大于左孩子
			++diff;
			++left_child;    // 转为右孩子
		}

		if (comp(*left_child, *head)) {    // 父节点是否大于最大的孩子
			return;
		}

		value_type tmp_less = ciel::move(*head);
		do {
			*head = ciel::move(*left_child);
			head = left_child;

			if ((len - 2) / 2 < diff) {    // 目前偏移量所代表的节点已经是叶子节点了
				break;
			}
			// 重复操作
			diff = diff * 2 + 1;
			left_child = first + diff;

			if (diff + 1 < len && comp(*left_child, *(left_child + difference_type(1)))) {
				++diff;
				++left_child;
			}
		} while (!comp(*left_child, tmp_less));    // 如果上面下沉的最小元素已经大于目前的孩子节点，循环停止

		*head = ciel::move(tmp_less);
	}

}   // namespace ciel

#endif // TINYSTLBYCIEL_INCLUDE_CIEL_ALGORITHM_IMPL_SIFT_DOWN_H_