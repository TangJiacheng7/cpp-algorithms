// ============================================================================
// 2778. 特殊元素平方和 (Sum of Squares of Special Elements)
// 来源：https://leetcode.cn/problems/sum-of-squares-of-special-elements/
// 难度：简单
//
// 题目：nums 是下标从 1 开始、长度为 n 的数组。
//       若 n % i == 0，则称 nums[i] 是「特殊元素」。
//       返回所有特殊元素的平方和。
//
// 示例：nums = [1,2,3,4]  ->  1^2 + 2^2 + 4^2 = 21
// ============================================================================
//
// 【思路】
//
// 题目给的是 1-based 下标，而 C++ 的 vector 是 0-based，所以「位置 i」对应
// nums[i - 1]。直接枚举 i = 1..n，判断 n % i == 0，命中就累加平方。
//
// 注意判断方向：是 n % i == 0（i 整除 n，即 i 是 n 的因子），
// 不是 i % n == 0。后者只会命中 i = n 这一个位置。
//
// 不需要额外数组。原解法先把特殊元素挑进一个临时数组再遍历求和，两趟循环加一块
// O(n) 的额外空间；直接累加是一趟循环、零额外空间。
//
// 【复杂度】
//   时间：O(n)，n 个位置各判断一次。
//   空间：O(1)。
//
// 【易错点】
//   1. 下标差 1：位置 i 对应 nums[i - 1]。
//   2. 判断方向：n % i，不是 i % n。
//   3. 用 long long 累加。本题数据范围内 int 够用，但如果 n 或元素值放大
//      （n = 1e5、元素 = 1e5 时上限约 1e15），int 会直接溢出。
//   4. n = 1 是合法输入，此时只有位置 1 命中。
//
// 【面试追问】
//   1. 如果只想知道「哪些位置特殊」，不需要遍历全部？-> 枚举到 √n 即可：
//      i 是因子时 n/i 也是因子，复杂度降到 O(√n)。但本题答案依赖每个特殊位置的
//      元素值，读取本身仍是 O(n)。
//   2. 如果题目改成「i 是 n 的倍数」？-> 只有 i = n 一个位置命中，答案就是
//      nums[n-1]^2。
//   3. 如果 n 是质数？-> 因子只有 1 和 n，答案是 nums[0]^2 + nums[n-1]^2。
//
// ============================================================================

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

namespace {

long long sum_of_squares(const std::vector<int>& nums) {
    const std::size_t n = nums.size();
    long long total = 0;

    // i 是 1-based 的位置编号
    for (std::size_t i = 1; i <= n; ++i) {
        if (n % i == 0) {
            const long long value = nums[i - 1];
            total += value * value;
        }
    }
    return total;
}

}  // namespace

int main() {
    // 题目示例 1：n = 4，因子 1、2、4
    assert(sum_of_squares({1, 2, 3, 4}) == 21);
    // 题目示例 2：n = 6，因子 1、2、3、6
    assert(sum_of_squares({2, 7, 1, 19, 18, 3}) == 63);
    // n = 1，只有位置 1 命中
    assert(sum_of_squares({7}) == 49);
    // n = 3 是质数，因子只有 1 和 3
    assert(sum_of_squares({1, 5, 3}) == 1 + 9);
    // 含 0 和负数：平方后都是非负
    assert(sum_of_squares({0, 5, 0, 0}) == 25);
    assert(sum_of_squares({-3, 1, 1, 1}) == 9 + 1 + 1);

    std::cout << "2778 sum_of_squares: 全部用例通过\n";
    return 0;
}
