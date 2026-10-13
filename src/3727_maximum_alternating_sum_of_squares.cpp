// ============================================================================
// 3727. 最大交替平方和 (Maximum Alternating Sum of Squares)
// 来源：https://leetcode.cn/problems/maximum-alternating-sum-of-squares/
// 难度：中等
//
// 题目：给你一个整数数组 nums，可以按任意顺序重新排列元素。定义重排后数组 arr 的
//       交替得分为 score = arr[0]^2 - arr[1]^2 + arr[2]^2 - arr[3]^2 + ...
//       （偶数下标取正号，奇数下标取负号）。返回重排后能得到的最大交替得分。
//       约束：1 <= nums.length <= 1e5，-4e4 <= nums[i] <= 4e4。
//
// 示例：nums = [1,2,3]           ->  12  （重排 [2,1,3]：4 - 1 + 9）
//       nums = [1,-1,2,-2,3,-3]  ->  16  （重排后：9 - 1 + 4 - 1 + 9 - 4）
// ============================================================================
//
// 【思路】
//
// 第一步，看穿符号从哪来。
//   把每一项写成符号 × 平方：score = Σ sign(i) · arr[i]^2，
//   其中 sign(i) = +1（i 为偶数）、-1（i 为奇数）。
//   也就是说，**一个元素拿正号还是负号，只由它落在哪个下标决定，跟它本身的值无关**。
//
// 第二步，抵消原数组的正负号。
//   平方会抹掉符号，所以 nums[i] 是正是负对得分毫无影响，真正起作用的只有它的
//   绝对值 |nums[i]|。于是先把所有元素取绝对值，原数组的正负号从此可以忘掉。
//
// 第三步，贪心分配。
//   重排后，偶数下标（正号位）有 ceil(n/2) 个，奇数下标（负号位）有 floor(n/2) 个。
//   得分 = （正号位上所有元素的平方和）-（负号位上所有元素的平方和）。
//   要让这个差最大，就该把**绝对值最大的那些元素塞进正号位**，剩下的进负号位。
//
//   交换论证（为什么这样最优）：假设存在两个元素 a、b，满足 |a| > |b|，却把 a 放
//   在负号位、b 放在正号位。把两者对调，得分变化 = (a^2 - b^2) - (b^2 - a^2)
//   = 2(a^2 - b^2) > 0，说明原配置不是最优的。反复做这种交换，直到「任一正号位的
//   绝对值都不小于任一负号位」为止——此时恰好是：把绝对值升序排序后，前 floor(n/2)
//   个（最小的）给负号位，其余（最大的 ceil(n/2) 个）给正号位。
//
// 演进路径：暴力做法是枚举全部 n! 种排列取最大值（下面的随机对拍里就是它），阶乘级；
// 一旦想通「只有绝对值重要、符号由下标奇偶决定」，问题就塌缩成一趟排序 + 一趟累加。
//
// 【复杂度】
//   时间：O(n log n)，瓶颈在排序；后面的分配是一趟 O(n)。
//   空间：O(n)，存绝对值的临时数组（就地改 nums 也能省掉，但仍躲不开一次排序）。
//         若元素值域很小，可用计数排序把整体降到 O(n + V)，V 为值域大小。
//
// 【易错点】
//   1. 别漏取绝对值：原数组可能有负数，而真正比较的是大小，用带符号的值去排序会错。
//   2. 负号位个数是 n / 2（下取整），正号位是 (n + 1) / 2（上取整）。n 为奇数时正号位
//      比负号位多一个，别把两者当成一样多。
//   3. 全程用 long long：元素绝对值到 4e4，平方是 1.6e9；n 到 1e5，总分能到 8e13 量级，
//      int 必溢出，累加量必须是 long long。
//   4. 先取绝对值再平方：abs 的结果要提升成 long long 再自乘，别让 int 中途溢出。
//
// 【面试追问】
//   1. 如果要求「最小」交替得分？-> 反过来即可：把绝对值小的放正号位、大的放负号位，
//      等价于排序后从两端取数。
//   2. 如果正负号不绑下标，而是允许自己给每个元素指定符号（正负个数之差固定）？
//      -> 本质不变，仍是排序 + 贪心分配，按「能拿正号的个数」取最大的那批。
//   3. 如果 n 放大到 1e7？-> 排序成为瓶颈。数值范围小（|nums[i]| <= 4e4）时用桶排序，
//      可以做到 O(n + V)。
//   4. 如果把平方换成别的函数？-> 只要 f 单调不减且偶函数（f(x) = f(-x)），
//      上面的交换论证一字不改仍然成立；f 若不再单调，结论就不一定了。
//
// ============================================================================

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

namespace {

// 贪心正解：只有绝对值重要，正号位放绝对值最大的 ceil(n/2) 个。
long long max_alternating_sum_of_squares(const std::vector<int>& nums) {
    std::vector<long long> values;
    values.reserve(nums.size());
    for (const int x : nums) {
        values.push_back(std::llabs(static_cast<long long>(x)));
    }
    std::sort(values.begin(), values.end());

    const std::size_t n = values.size();
    const std::size_t negative_slots = n / 2;  // 奇数下标（负号位）的个数

    long long score = 0;
    for (std::size_t i = 0; i < n; ++i) {
        const long long square = values[i] * values[i];
        if (i < negative_slots) {
            score -= square;  // 最小的 floor(n/2) 个放负号位
        } else {
            score += square;  // 其余（较大的 ceil(n/2) 个）放正号位
        }
    }
    return score;
}

// ---------------------------------------------------------------------------
// 对拍用：枚举所有排列，逐个计算交替得分取最大。规模小（n <= 7）时才用得起。
// 直接把原数组排序后做 next_permutation，符号、绝对值全部如实参与计算，
// 专门用来暴露「漏取绝对值」「正负号位个数搞反」这类错误。
// ---------------------------------------------------------------------------

long long brute_max_alternating_sum_of_squares(const std::vector<int>& nums) {
    std::vector<int> perm = nums;
    std::sort(perm.begin(), perm.end());

    long long best = std::numeric_limits<long long>::min();
    do {
        long long score = 0;
        for (std::size_t i = 0; i < perm.size(); ++i) {
            const long long value = perm[i];
            const long long square = value * value;
            if (i % 2 == 0) {
                score += square;
            } else {
                score -= square;
            }
        }
        if (score > best) {
            best = score;
        }
    } while (std::next_permutation(perm.begin(), perm.end()));
    return best;
}

}  // namespace

int main() {
    // 题目示例
    assert(max_alternating_sum_of_squares({1, 2, 3}) == 12);
    assert(max_alternating_sum_of_squares({1, -1, 2, -2, 3, -3}) == 16);

    // 单元素：只有一个正号位，结果就是它的平方
    assert(max_alternating_sum_of_squares({5}) == 25);
    assert(max_alternating_sum_of_squares({-5}) == 25);
    assert(max_alternating_sum_of_squares({0}) == 0);

    // 两个元素：一正一负号位，取「大平方 - 小平方」
    assert(max_alternating_sum_of_squares({3, -7}) == 40);  // 49 - 9
    assert(max_alternating_sum_of_squares({7, 3}) == 40);

    // 奇数长度：正号位比负号位多一个，(25 + 16 + 9) - (4 + 1) = 45
    assert(max_alternating_sum_of_squares({1, 2, 3, 4, 5}) == 45);

    // 全零，或绝对值全相等 -> 0
    assert(max_alternating_sum_of_squares({0, 0, 0, 0}) == 0);
    assert(max_alternating_sum_of_squares({-2, 2, -2, 2}) == 0);

    // 绝对值上界参与运算，考验 long long
    assert(max_alternating_sum_of_squares({40000}) == 1600000000LL);
    assert(max_alternating_sum_of_squares({40000, -40000}) == 0);
    assert(max_alternating_sum_of_squares({40000, 40000, -40000}) == 1600000000LL);

    // 大规模：5e4 个 40000（进正号位）+ 5e4 个 1（进负号位），
    // 结果约 8e13，远超 int 的范围，专门防溢出。
    {
        std::vector<int> big;
        big.reserve(100000);
        for (int i = 0; i < 50000; ++i) {
            big.push_back(40000);
        }
        for (int i = 0; i < 50000; ++i) {
            big.push_back(1);
        }
        assert(max_alternating_sum_of_squares(big) ==
               50000LL * 1600000000LL - 50000LL);
    }

    // 随机对拍：小规模下枚举全部排列，贪心解法和暴力结果必须完全一致
    std::mt19937 rng(20261013u);
    std::uniform_int_distribution<int> length_dist(1, 7);
    std::uniform_int_distribution<int> value_dist(-30, 30);
    for (int iter = 0; iter < 800; ++iter) {
        const int n = length_dist(rng);
        std::vector<int> nums(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) {
            nums[static_cast<std::size_t>(i)] = value_dist(rng);
        }
        assert(max_alternating_sum_of_squares(nums) ==
               brute_max_alternating_sum_of_squares(nums));
    }

    std::cout << "3727 max_alternating_sum_of_squares: 全部用例通过"
                 "（含 800 组随机对拍）\n";
    return 0;
}
