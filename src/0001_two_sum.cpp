// ============================================================================
// 0001. 两数之和 (Two Sum)
// 来源：https://leetcode.cn/problems/two-sum/
// 难度：简单
//
// 题目：给定整数数组 nums 和目标值 target，返回和为 target 的两个元素的下标。
//       保证恰好存在一个答案，同一个元素不能使用两次。
//
// 示例：nums = [2, 7, 11, 15], target = 9  ->  [0, 1]
// ============================================================================
//
// 【思路】
//
// 暴力解法：枚举所有 (i, j) 且 i < j，检查 nums[i] + nums[j] == target。
//           时间 O(n^2)，空间 O(1)。n = 10^4 时约 5*10^7 次操作，勉强能过，
//           但 n = 10^5 就超时了。
//
// 优化方向：暴力解法慢在"对每个 i 都要重新扫一遍 j"。
//           换个角度：固定 nums[i] 后，我需要的那个值是被唯一确定的，
//           就是 need = target - nums[i]。所以问题变成"need 这个值出现过吗"。
//           而"值出现过吗"正是哈希表擅长的。
//
// 最优解法（本文件实现）：一次遍历 + 哈希表
//           遍历到 i 时，先查哈希表里有没有 target - nums[i]：
//             - 有   -> 那个元素的下标是 j，直接返回 {j, i}，且必然 j < i
//             - 没有 -> 把 (nums[i] -> i) 存进哈希表，继续
//           时间 O(n) 期望，空间 O(n)。
//
// 为什么"先查后插"而不是"先插后查"：
//           如果先插后查，当 target == 2 * nums[i] 时（例如 nums = [3, 3], target = 6，
//           或 nums = [3, 4], target = 6），刚插入的自己会被自己查到，返回下标 i 两次。
//           先查后插保证任何一对下标只在"后出现的那个元素"处被发现，天然不会重复。
//
// 【复杂度】
//   时间：O(n) 期望。每个元素最多一次 find、一次 insert，均为均摊 O(1)。
//         最坏 O(n^2)，出现在所有元素哈希值全冲突时（正常哈希函数下不会发生）。
//   空间：O(n)，哈希表最多存 n 个元素。
//
// 【易错点】
//   1. 先查后插，别写反（见上面的分析）。
//   2. 返回的是下标不是数值。
//   3. 代码里的下标用 std::size_t 遍历，返回时转成 int，和题目要求一致。
//   4. 值域是 int 时 target - nums[i] 在极端值下可能溢出。本题数据范围不触发，
//      但如果值域换成 long long，need 也必须用 long long 存。
//
// 【面试追问】
//   1. 如果数组已经排好序？-> 双指针，左右收缩，O(n) 时间 O(1) 空间，比哈希表更省内存。
//   2. 如果要返回所有不重复的数对？-> 排序 + 双指针 + 跳过重复元素。
//      注意这时返回的是"数值对"而不是"下标对"。
//   3. 如果要求 O(1) 额外空间且不能改数组？-> 本题这么做没有已知解，O(n) 空间是下界。
//   4. 哈希冲突很严重怎么办（n 很大、值域很窄）？
//      -> 开放寻址 vs 链地址法的取舍。可以改用排序 + 二分，或者自己开桶。
//
// ============================================================================

#include <cassert>
#include <cstddef>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace {

// 返回和为 target 的两个元素下标；无解时返回空 vector。
// 题目保证有唯一解，空 vector 只是防御性处理。
std::vector<int> two_sum(const std::vector<int>& nums, int target) {
    // 值 -> 下标
    std::unordered_map<int, std::size_t> seen;
    seen.reserve(nums.size() * 2);  // 预留空间，减少中途 rehash

    for (std::size_t i = 0; i < nums.size(); ++i) {
        const int need = target - nums[i];
        if (const auto it = seen.find(need); it != seen.end()) {
            return {static_cast<int>(it->second), static_cast<int>(i)};
        }
        seen.emplace(nums[i], i);
    }
    return {};
}

}  // namespace

int main() {
    // 基本情况
    assert((two_sum({2, 7, 11, 15}, 9) == std::vector<int>{0, 1}));
    // 答案不在数组开头
    assert((two_sum({3, 2, 4}, 6) == std::vector<int>{1, 2}));
    // 两个相同元素：专门验证"先查后插"，写反了这里会返回 {1, 1}
    assert((two_sum({3, 3}, 6) == std::vector<int>{0, 1}));
    // 含负数
    assert((two_sum({-3, 4, 3, 90}, 0) == std::vector<int>{0, 2}));
    // 单元素无解
    assert(two_sum({5}, 5).empty());

    std::cout << "0001 two_sum: 全部用例通过\n";
    return 0;
}
