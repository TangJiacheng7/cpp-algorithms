// ============================================================================
// 3550. 数位和等于下标的最小下标 (Smallest Index With Digit Sum Equal to Index)
// 来源：https://leetcode.cn/problems/smallest-index-with-digit-sum-equal-to-index/
// 难度：简单
//
// 题目：给你一个整数数组 nums。返回最小的下标 i，使得 nums[i] 的数位和
//       （把它的每一位数字相加求和）等于 i；不存在这样的下标就返回 -1。
//       数位和按非负整数定义：0 的数位和是 0。
//       约束：1 <= nums.length <= 100，0 <= nums[i] <= 1000。
//
// 示例：nums = [1,3,2]  ->  2（nums[2] = 2，数位和 2 == 下标 2）
//       nums = [1,10,11]  ->  1（nums[1] = 10，数位和 1 == 下标 1）
//       nums = [1,2,3]  ->  -1（三个位置都不匹配）
// ============================================================================
//
// 【思路】
//
// 题目要的是**最小**的 i，不是任意一个。所以从左往右扫描，遇到第一个满足
// digit_sum(nums[i]) == i 的下标就直接返回——先遇到的天然最小，不需要比较、
// 排序或者记录候选集合。
//
// 数位和用「取余 + 整除」逐位剥离：t % 10 取出最低位，t /= 10 丢掉它，
// 直到 t 变成 0。循环条件必须是 t > 0：如果写成 t >= 0，t 永远到不了负数，
// 循环不会结束。也正是因为这个条件，0 的数位和天然是 0，不需要特判。
//
// 另一种等价写法是用 std::to_string 转成字符串后逐字符减 '0' 累加（见下面
// 对拍里的实现）。两者结果一致，但取余版本不分配字符串、常数更小。
//
// 本题没有别的结构可挖：一次线性扫描就是最优，因为最坏情况下（答案是 -1）
// 每个元素都必须被读一遍。数据范围很小（n ≤ 100、元素 ≤ 1000），也可以先
// 打表 0..1000 的数位和再查表，但那只省常数，不改复杂度。
//
// 【复杂度】
//   时间：O(n · d)，n 是数组长度，d 是元素的最多位数（元素 ≤ 1000，d ≤ 4）。
//         最坏要扫完整个数组才能确定返回 -1。n ≤ 100，随手就是 0 ms。
//   空间：O(1)，只用几个整数变量（下面随机对拍里的字符串版本不计入）。
//
// 【易错点】
//   1. 循环条件写错：t >= 0 会死循环，t != 0 在负数上会把负号也卷进求和。
//   2. 下标是 0-based，nums[i] 直接对应 i，不要做「位置 1 开始」的偏移——
//      那是个常见的照搬习惯，错一位就整题错。
//   3. 类型：本题元素 ≤ 1000（数位和 ≤ 27）、下标 < 100，全用 int 不会溢出。
//      但若把元素范围放大到 long long（如 1e18），剥离数位时的中间量和累加量
//      都要跟着放大，否则重演 2778 里「累加量悄悄溢出」的坑。
//   4. -1 的分支只有「扫完都没命中」这一种，别漏。
//   5. nums[i] = 0 是合法输入，数位和是 0，只有 i = 0 时会命中。别因为
//      「0 没有数位」就返回错误值或者跳过它。
//
// 【面试追问】
//   1. 如果要求「最大的」i？-> 从右往左扫，或者扫完记住最后一个命中位置。
//   2. 如果要返回所有命中的下标？-> 用 vector 收集候选，不能提前 return，
//      最小下标就是结果数组的第一个元素。
//   3. 如果 nums 很大又要在同一数组上反复查询？-> 数位和的取值只有 0..27，
//      可以按数位和建桶，桶里只留最小下标，查询变成 O(1)；但预处理仍是 O(n)。
//   4. 如果要支持负数？-> 先定义清楚「数位和」是取绝对值求和还是带符号逐位相加，
//      否则 t % 10 在负数上返回负值，不同实现会得出不同答案。
//
// ============================================================================

#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

// 逐位剥离求数位和。约定只处理非负整数，0 返回 0。
int digit_sum(int value) {
    int sum = 0;
    int t = value;
    while (t > 0) {
        sum += t % 10;
        t /= 10;
    }
    return sum;
}

int smallest_index_with_digit_sum_equal(const std::vector<int>& nums) {
    for (std::size_t i = 0; i < nums.size(); ++i) {
        if (digit_sum(nums[i]) == static_cast<int>(i)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// ---------------------------------------------------------------------------
// 对拍用：换一套实现（字符串逐字符求和）并且不提前返回，而是扫完整个数组
// 再取最小的命中下标。和上面的「从左往右第一个命中就返回」相互印证，
// 专门用来暴露方向性错误和 t > 0 这类边界错误。
// ---------------------------------------------------------------------------

int digit_sum_by_string(int value) {
    int sum = 0;
    for (const char c : std::to_string(value)) {
        sum += c - '0';
    }
    return sum;
}

int brute_smallest_index(const std::vector<int>& nums) {
    int best = -1;
    for (std::size_t i = 0; i < nums.size(); ++i) {
        if (best == -1 && digit_sum_by_string(nums[i]) == static_cast<int>(i)) {
            best = static_cast<int>(i);
        }
    }
    return best;
}

}  // namespace

int main() {
    // 题目示例
    assert(smallest_index_with_digit_sum_equal({1, 3, 2}) == 2);
    assert(smallest_index_with_digit_sum_equal({1, 10, 11}) == 1);

    // 全都不命中
    assert(smallest_index_with_digit_sum_equal({100}) == -1);
    assert(smallest_index_with_digit_sum_equal({9, 9}) == -1);
    // 命中多个时返回最小的那个
    assert(smallest_index_with_digit_sum_equal({0, 10, 5, 3}) == 0);
    assert(smallest_index_with_digit_sum_equal({9, 10, 2}) == 1);
    // 单元素：0 的数位和是 0，下标也是 0
    assert(smallest_index_with_digit_sum_equal({0}) == 0);
    assert(smallest_index_with_digit_sum_equal({1}) == -1);
    // 题目示例 3：一个都不命中
    assert(smallest_index_with_digit_sum_equal({1, 2, 3}) == -1);
    // 元素上界参与命中：1000 的数位和是 1
    assert(smallest_index_with_digit_sum_equal({1, 1000}) == 1);
    // 命中位置出现在末尾（500 的数位和是 5，前面几个下标都不匹配）
    assert(smallest_index_with_digit_sum_equal({500, 500, 500, 500, 500, 5}) == 5);
    // 数位和的上界是 27（999），远超 n ≤ 100 时可能出现的下标，只会落空
    assert(smallest_index_with_digit_sum_equal({999, 999}) == -1);

    // 随机对拍：两套数位和实现 + 两种扫描策略必须完全一致
    std::mt19937 rng(20261012u);
    std::uniform_int_distribution<int> length_dist(1, 100);
    std::uniform_int_distribution<int> value_dist(0, 1000);
    for (int iter = 0; iter < 2000; ++iter) {
        const int n = length_dist(rng);
        std::vector<int> nums(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) {
            nums[static_cast<std::size_t>(i)] = value_dist(rng);
        }
        assert(smallest_index_with_digit_sum_equal(nums) ==
               brute_smallest_index(nums));
    }

    std::cout << "3550 smallest_index_with_digit_sum_equal: 全部用例通过"
                 "（含 2000 组随机对拍）\n";
    return 0;
}
