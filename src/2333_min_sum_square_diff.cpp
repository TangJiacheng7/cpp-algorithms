// ============================================================================
// 2333. 最小差值平方和 (Minimum Sum of Squared Difference)
// 来源：https://leetcode.cn/problems/minimum-sum-of-squared-difference/
// 难度：中等
//
// 题目：给两个长度均为 n 的数组 nums1、nums2。可以对 nums1 的元素做至多 k1 次
//       +1/-1（总共），对 nums2 做至多 k2 次。求 Σ(nums1[i] - nums2[i])^2 的最小值。
//       元素允许被改成负数。
//
// 示例：nums1 = [1,2,3,4], nums2 = [2,10,20,19], k1 = 0, k2 = 0  ->  579
// ============================================================================
//
// 【思路】
//
// 第一步，化简问题。
//   对 nums1[i] 加 1，和对 nums2[i] 减 1，对差值 nums1[i] - nums2[i] 的效果完全一样。
//   所以 k1 和 k2 可以合并成一个总预算 S = k1 + k2，不需要分开考虑。
//   又因为每个位置的差值可以自由决定往正还是往负调，所以只需关心绝对值：
//       d[i] = |nums1[i] - nums2[i]|
//   问题变成：在「总减少量 <= S」的约束下，最小化 Σ(d[i] - 减少量[i])^2。
//
// 第二步，贪心方向。
//   平方函数是凸的，收益递减。把最大的那个 d 减 1，收益是 2d-1，一定比去减小元素
//   划算。所以要把预算优先堆在最大的差值上——也就是先降序排序。
//
// 第三步，批量拉平，而不是一次减 1。
//   如果每次只减 1，复杂度是 O(S)，而 S 可以到 2e9，必然超时。
//   关键观察：把前 p+1 个最大的元素**一起**往下压，每压低 1 的代价恰好是 (p+1)。
//   所以把 d 降序排序、并在末尾补一个哨兵 0，然后逐层处理：
//     - 把前 p+1 个一起压到 d[p+1] 的高度，代价 cost = (p+1) * (d[p] - d[p+1])
//     - 预算还够 -> 付掉 cost，++p，继续压下一层
//     - 预算不够 -> 用手上剩下的预算把这 p+1 个元素尽量「平摊」下压：
//                  每个压 budget/(p+1) 层，余数 budget%(p+1) 分给最前面几个
//   整个过程中 p 单调递增、每个元素只被碰一次，所以这一步是 O(n)。
//
// 【复杂度】
//   时间：O(n log n)，瓶颈是排序；拉平过程 O(n)。
//   空间：O(n)，存差值数组（末尾多一格哨兵）。
//
// 【易错点】
//   1. 先判断 k1+k2 >= Σd[i]，这种情况答案直接是 0。不先判掉的话，后面的整除逻辑
//      要额外处理「把元素压成负数」这种没有意义的情形。
//   2. 所有和差值、平方相关的量都必须用 long long。差值本身能到 1e5 量级，
//      平方之后是 1e10，int 直接溢出。
//   3. 排序用 greater<long long>，不要用 greater<int>——后者会把 long long 截断成
//      int 再比较。
//   4. 哨兵不能漏：数组末尾补一个 0，压到最后一层时 d[p+1] 才有意义。
//   5. 「先付代价再 ++p」的顺序不能反。cost 是按当前 p（即已拉平的高度层数）
//      算出来的，顺序写反会少付一整层的代价。
//
// 【面试追问】
//   1. 如果 k1、k2 不能互换（比如只准动 nums1）？-> 本题恰好可以互换；不可互换时
//      就变成两个独立预算的分配问题，需要更复杂的方法。
//   2. 如果目标函数从平方换成绝对值？-> 反而更简单：绝对值下每层的边际收益是常数，
//      不用分配，答案直接是 max(0, Σd[i] - S)。
//   3. 如果 n 到 1e6？-> 排序是瓶颈。差值取值范围小的话可以用计数排序把整体降到 O(n)。
//
// ============================================================================

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

namespace {

long long min_sum_square_diff(const std::vector<int>& nums1,
                              const std::vector<int>& nums2,
                              long long k1,
                              long long k2) {
    const std::size_t n = nums1.size();

    // d[i] = |nums1[i] - nums2[i]|；多留一格放哨兵 0
    std::vector<long long> d(n + 1, 0);
    long long need = 0;  // 把所有差值压到 0 需要的总预算
    for (std::size_t i = 0; i < n; ++i) {
        d[i] = std::llabs(static_cast<long long>(nums1[i]) - nums2[i]);
        need += d[i];
    }

    long long budget = k1 + k2;
    if (budget >= need) {
        return 0;
    }

    // 只排前 n 个，d[n] 是哨兵，参与不到排序里
    std::sort(d.begin(), d.begin() + static_cast<std::ptrdiff_t>(n),
              std::greater<long long>());

    std::size_t p = 0;  // 已被拉平到同一高度的元素个数减 1
    while (budget > 0 && p < n) {
        const long long level_gap = d[p] - d[p + 1];
        const long long count = static_cast<long long>(p) + 1;
        const long long cost = level_gap * count;

        if (budget > cost) {
            budget -= cost;
            ++p;
        } else {
            // 预算不够再整体降一层，把它平摊到前 count 个元素上
            const long long each = budget / count;
            const long long rem = budget % count;
            for (std::size_t j = 0; j <= p; ++j) {
                d[j] = d[p] - each - (static_cast<long long>(j) < rem ? 1 : 0);
            }
            break;
        }
    }

    long long result = 0;
    for (std::size_t i = 0; i < n; ++i) {
        result += d[i] * d[i];
    }
    return result;
}

// ---------------------------------------------------------------------------
// 暴力交叉验证用：把小规模输入的所有分配方案枚举一遍。
// 和上面的快速解法对拍，用来证明快速解法没有边界错误。
// ---------------------------------------------------------------------------

void brute_dfs(const std::vector<long long>& d, std::size_t idx, long long left,
               long long acc, long long& best) {
    if (acc >= best) {
        return;  // 剪枝：当前已经不比已知最优更好
    }
    if (idx == d.size()) {
        best = acc;
        return;
    }
    const long long limit = std::min(d[idx], left);
    for (long long take = 0; take <= limit; ++take) {
        const long long value = d[idx] - take;
        brute_dfs(d, idx + 1, left - take, acc + value * value, best);
    }
}

long long brute_min_sum_square_diff(const std::vector<int>& nums1,
                                    const std::vector<int>& nums2,
                                    long long k1,
                                    long long k2) {
    std::vector<long long> d;
    d.reserve(nums1.size());
    for (std::size_t i = 0; i < nums1.size(); ++i) {
        d.push_back(std::llabs(static_cast<long long>(nums1[i]) - nums2[i]));
    }
    long long best = std::numeric_limits<long long>::max();
    brute_dfs(d, 0, k1 + k2, 0, best);
    return best;
}

}  // namespace

int main() {
    // 题目示例
    assert(min_sum_square_diff({1, 2, 3, 4}, {2, 10, 20, 19}, 0, 0) == 579);
    assert(min_sum_square_diff({1, 4, 10, 12}, {5, 8, 6, 9}, 1, 1) == 43);

    // 边界：预算为 0，答案就是原始平方和
    assert(min_sum_square_diff({1}, {4}, 0, 0) == 9);
    // 边界：预算刚好够把所有差值抹平
    assert(min_sum_square_diff({1, 4, 10, 12}, {5, 8, 6, 9}, 8, 7) == 0);
    // 边界：预算远超需要，仍返回 0（不能把元素压成负数再去平方）
    assert(min_sum_square_diff({1, 1}, {2, 2}, 1000, 1000) == 0);
    // 单个元素，预算只够压掉一部分
    assert(min_sum_square_diff({0}, {5}, 3, 0) == 4);

    // 随机对拍：小规模下暴力枚举和贪心解法必须完全一致
    std::mt19937 rng(20261012u);
    std::uniform_int_distribution<int> length_dist(1, 4);
    std::uniform_int_distribution<int> value_dist(-6, 6);
    std::uniform_int_distribution<long long> budget_dist(0, 10);
    for (int iter = 0; iter < 600; ++iter) {
        const int n = length_dist(rng);
        std::vector<int> a(static_cast<std::size_t>(n));
        std::vector<int> b(static_cast<std::size_t>(n));
        for (int i = 0; i < n; ++i) {
            a[static_cast<std::size_t>(i)] = value_dist(rng);
            b[static_cast<std::size_t>(i)] = value_dist(rng);
        }
        const long long k1 = budget_dist(rng);
        const long long k2 = budget_dist(rng);
        assert(min_sum_square_diff(a, b, k1, k2) ==
               brute_min_sum_square_diff(a, b, k1, k2));
    }

    std::cout << "2333 min_sum_square_diff: 全部用例通过（含 600 组随机对拍）\n";
    return 0;
}
