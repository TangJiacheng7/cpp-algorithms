// ============================================================================
// 2089. 找出数组排序后的目标下标 (Find Target Indices After Sorting Array)
// 来源：https://leetcode.cn/problems/find-target-indices-after-sorting-array/
// 难度：简单
//
// 题目：给定长度为 n（1 <= n <= 100）的整数数组 nums 和目标值 target，将 nums
//       按非递减顺序排序，返回排序后所有满足 nums[i] == target 的下标 i。
//       返回的下标必须按递增顺序排列；若不存在目标元素，则返回空数组。
//
// 示例：nums = [1,2,5,2,3], target = 2  ->  [1,2]
// ============================================================================
//
// 【思路】
//
// 暴力做法是先排序，再从前往后扫描并收集等于 target 的位置，时间为 O(n log n)。
// 但本题只关心目标元素最终落在哪些下标，不需要真的构造排序后的数组。
//
// 关键观察：排序后，所有小于 target 的元素一定排在 target 前面。设：
//   less_count  = nums 中小于 target 的元素个数
//   equal_count = nums 中等于 target 的元素个数
// 那么所有 target 必然占据连续区间：
//   [less_count, less_count + equal_count)
//
// 因此只需遍历一次原数组同时统计 less_count 和 equal_count，再按该区间生成
// 递增下标即可。这个做法也自然覆盖了 target 不存在、全部元素都等于 target、
// 以及 target 所有出现位置都位于数组两端等边界。
//
// 【复杂度】
//   时间：O(n)，一次计数遍历，生成答案也只需 O(equal_count)。
//   空间：O(equal_count)，用于保存返回结果，不计额外空间则为 O(1)。
//
// 【易错点】
//   1. 连续区间的终点是开区间 less_count + equal_count，不要多生成一个下标。
//   2. equal_count 为 0 时必须返回空数组，不能访问任何目标位置。
//   3. 重复的 target 会占据多个连续下标；不要只返回第一次出现的位置。
//   4. 题目的输入范围很小，但下标存储仍应使用 size_t，避免有符号转换警告。
//
// 【面试追问】
//   1. 如果只需要返回 target 第一次出现的位置？-> 答案就是小于 target 的元素
//      个数；若 equal_count == 0，则不存在。
//   2. 如果 nums 已经有序？-> 直接二分查找左右边界，O(log n) 定位连续区间。
//   3. 如果要求返回 target 在原数组中的所有下标？-> 此时不能排序，应顺序扫描，
//      时间复杂度仍为 O(n)。
//   4. 如果内存受限且只求出现次数？-> 只维护 equal_count，不需要构造结果。
//
// ============================================================================

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

std::vector<int> find_target_indices_after_sorting_array(const std::vector<int>& nums,
                                                         int target) {
    std::size_t less_count = 0;
    std::size_t equal_count = 0;
    for (const int value : nums) {
        if (value < target) {
            ++less_count;
        } else if (value == target) {
            ++equal_count;
        }
    }

    std::vector<int> result(equal_count);
    for (std::size_t offset = 0; offset < equal_count; ++offset) {
        result[offset] = static_cast<int>(less_count + offset);
    }
    return result;
}

}  // namespace

int main() {
    // 题目示例
    assert((find_target_indices_after_sorting_array({1, 2, 5, 2, 3}, 2) ==
            std::vector<int>{1, 2}));

    // 目标值只出现一次
    assert((find_target_indices_after_sorting_array({1, 2, 5, 2, 3}, 3) ==
            std::vector<int>{3}));
    assert((find_target_indices_after_sorting_array({1, 2, 5, 2, 3}, 5) ==
            std::vector<int>{4}));

    // 目标值不存在：分别覆盖小于最小值和大于最大值
    assert(find_target_indices_after_sorting_array({1, 1}, 2).empty());
    assert(find_target_indices_after_sorting_array({3, 4, 5}, 2).empty());

    // 全部元素都等于目标值
    assert((find_target_indices_after_sorting_array({2, 2, 2}, 2) ==
            std::vector<int>{0, 1, 2}));

    // 空数组作为通用实现的安全边界
    assert(find_target_indices_after_sorting_array({}, 1).empty());

    std::cout << "2089 find_target_indices_after_sorting_array: 全部用例通过\n";
    return 0;
}
