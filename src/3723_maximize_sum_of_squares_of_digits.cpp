// ============================================================================
// 3723. 数位平方和的最大值 (Maximize Sum of Squares of Digits)
// 来源：https://leetcode.cn/problems/maximize-sum-of-squares-of-digits/
// 难度：中等
//
// 题目：给定正整数 num 和 sum。若正整数 n 的位数恰好为 num、各位数字之和恰好为
//       sum，则称 n 是「好整数」；好整数 n 的分数定义为 n 的各位数字的平方和。
//       返回一个字符串，表示能获得最大分数的好整数 n；若有多个，返回最大的那个；
//       若不存在这样的整数，返回空字符串。
//       约束：1 <= num <= 2e5，1 <= sum <= 2e6（答案长度最大 2e5 位）。
//
// 示例：num = 2,  sum = 3   ->  "30"  （好整数 12、21、30，分数分别 5、5、9）
//       num = 2,  sum = 17  ->  "98"  （89 与 98 分数同为 145，取较大的 98）
//       num = 1,  sum = 10  ->  ""    （1 位数的最大数字和只有 9）
// ============================================================================
//
// 【思路】
//
// 第一步：先排除暴力。
//   最直白的做法是枚举全部 num 位数（10^(num-1) 到 10^num - 1），逐个算数位和与
//   分数，取最优。但 num 最大到 2e5，候选个数是 9 * 10^(num-1) 量级，指数爆炸，
//   连数字本身都存不下，必须换思路：不枚举整数，而是决定「各位数字怎么分配」。
//
// 第二步：看清到底在最大化什么。
//   位数固定、数字和固定，唯一能选的是这 num 个数字的分配方式。分数是平方和，而
//   平方是凸函数：**在总和固定的前提下，把和集中到少数几个大数字上，平方和更大**。
//   交换论证：若某组解里存在两个数字满足 0 < a <= b <= 8，就把 a 减 1、b 加 1，
//   总和不变，而平方和的变化是
//       (a - 1)^2 + (b + 1)^2 - a^2 - b^2 = 2(b - a) + 2 > 0，
//   严格变大，说明原解不是最优。反复交换直到无法再换，可得结论：
//   最优解里除 9 和 0 之外，**至多存在一个数字**。
//
// 第三步：最优解的数字多重集唯一。
//   记 sum = 9k + r（0 <= r < 9）。第二步的结论把形态逼死了：k 个 9，加上当 r > 0
//   时的一个 r，其余位置全是 0。反过来验证，任何「数字和为 sum 且除 9 外至多一个
//   非 9 数字」的解都只能长这样，所以这么摆也确实取到最大分数 81k + r^2。
//
// 第四步：多重集唯一 ⇒ 降序摆放就是最大的整数。
//   能达到最大分数的数字集合只有一个，剩下的自由度只有排列顺序；大数字放高位一定
//   得到更大的整数。于是答案是：k 个 '9'、一个 r（若 r > 0）、以及
//   num - k - (r > 0 ? 1 : 0) 个 '0'。
//
// 可行性（补零为什么不会越界）：题目要求 sum <= 9 * num。当 r > 0 时，由 9k + r <= 9num
//   得 k <= num - 1，故 k + 1 <= num；当 r = 0 时 k <= num。两种情况都能恰好补满 num 位。
//   又因为 sum >= 1，首位必是 9 或 r（至少为 1），不会出现前导零。
//
// 演进路径：暴力枚举（指数级）→ 交换论证锁定「只能有一个非 9 数字」→ 唯一多重集
// → 降序拼接，O(num) 收工。这类「先证明解的唯一形态，再拼字符串」是构造题的常见套路。
//
// 【复杂度】
//   时间：O(num)。只有一次字符串构造 + 一次补零，可行性判定是 O(1)。
//         对比暴力枚举的 O(num * 10^num)，这里从指数级降到线性。
//   空间：O(num)，即返回值自身的长度；除返回值外只用了常数个整数中间量。
//
// 【易错点】
//   1. 可行域是 [1, 9 * num]：上界写成 sum > 9 * num 返回空串；下界要用 sum < 1，
//      sum = 0 时不存在 num 位好整数（首位不能为 0），同样返回空串，别漏。
//   2. 返回值必须是字符串。num 到 2e5，结果有 2e5 位，任何整型都装不下，
//      「枚举整数再 to_string」的路子从第一步就走不通。
//   3. 数字转字符别写 to_string(c)。char 会走 int 重载，拿到的是 ASCII 码
//      （to_string('9') == "57"）；要么 push_back 字符字面量，要么
//      static_cast<char>('0' + r)。
//   4. 降序意味着余数 r 必须排在所有 9 的后面。把 r 放到最前面（如 sum = 17 拼成
//      "89"）分数一样，但不是最大的整数。
//   5. 补的 0 只是用来凑满位数：它既不加数位和，也不加分数，必须放在末尾。
//   6. 提前 reserve(num) 并整块赋值；2e5 次 push_back 触发的多次扩容会白拖常数。
//
// 【面试追问】
//   1. 反过来求「最小分数」的好整数？-> 平方是凸的，就把和尽量摊平：每位放
//      sum / num，余数分摊到前几位，同样是 O(num) 的构造。
//   2. 要求「在最大分数里取最小的整数」？-> 数字多重集不变，改为升序摆放，
//      再把最小的非零数字提到首位即可，本解只需调整排列方向。
//   3. 改成问「有多少个好整数」？-> 变成带数位和约束的计数问题，用数位 DP 做，
//      构造法的线性时间不再适用。
//   4. 如果 num 大到 1e9（输出本身都承受不了）？-> 改成压缩输出「k 个 9 + r + 若干 0」，
//      或只返回最大分数 81k + r^2。
//
// ============================================================================

#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>

namespace {

// 贪心构造：sum = 9k + r，答案是 k 个 '9'、一个 r（若 r > 0）、其余补 '0'。
std::string max_sum_of_squares_of_digits(int num, int sum) {
    const long long digits = static_cast<long long>(num);
    const long long digit_sum = static_cast<long long>(sum);
    if (digit_sum < 1 || digit_sum > 9 * digits) {
        return "";
    }

    const std::size_t nines = static_cast<std::size_t>(digit_sum / 9);
    const int rest = static_cast<int>(digit_sum % 9);

    std::string result;
    result.reserve(static_cast<std::size_t>(num));
    result.assign(nines, '9');
    if (rest != 0) {
        result.push_back(static_cast<char>('0' + rest));
    }
    result.append(static_cast<std::size_t>(num) - result.size(), '0');
    return result;
}

// ---------------------------------------------------------------------------
// 对拍用：照题面老实枚举所有 num 位数（即 [10^(num-1), 10^num)），筛出数位和等于
// sum 的，逐个比分数、比大小。规模小（num <= 4）时才用得起，专门用来暴露
// 「最优数字集合选错」「降序方向搞反」这类错误。
// ---------------------------------------------------------------------------

std::string brute_max_sum_of_squares_of_digits(int num, int sum) {
    int low = 1;
    for (int i = 1; i < num; ++i) {
        low *= 10;  // 10^(num - 1)
    }
    const int high = low * 10;  // 10^num

    long long best_score = -1;
    std::string best;
    for (int n = low; n < high; ++n) {
        int digit_sum = 0;
        long long score = 0;
        for (int x = n; x > 0; x /= 10) {
            const int d = x % 10;
            digit_sum += d;
            score += static_cast<long long>(d) * d;
        }
        if (digit_sum != sum) {
            continue;
        }
        if (score > best_score || (score == best_score && std::to_string(n) > best)) {
            best_score = score;
            best = std::to_string(n);
        }
    }
    return best;
}

}  // namespace

int main() {
    // 题目示例
    assert(max_sum_of_squares_of_digits(2, 3) == "30");
    assert(max_sum_of_squares_of_digits(2, 17) == "98");
    assert(max_sum_of_squares_of_digits(1, 10) == "");

    // 可行域边界：sum 必须落在 [1, 9 * num]
    assert(max_sum_of_squares_of_digits(1, 1) == "1");
    assert(max_sum_of_squares_of_digits(1, 9) == "9");
    assert(max_sum_of_squares_of_digits(1, 0) == "");
    assert(max_sum_of_squares_of_digits(3, 27) == "999");
    assert(max_sum_of_squares_of_digits(3, 28) == "");
    assert(max_sum_of_squares_of_digits(5, 0) == "");

    // 只有一位非零，其余补 0 凑位数
    assert(max_sum_of_squares_of_digits(2, 1) == "10");
    assert(max_sum_of_squares_of_digits(2, 9) == "90");
    assert(max_sum_of_squares_of_digits(4, 9) == "9000");

    // r = 0：全是 9，后面全是 0
    assert(max_sum_of_squares_of_digits(4, 18) == "9900");
    assert(max_sum_of_squares_of_digits(2, 18) == "99");

    // r != 0：k 个 9 之后紧跟余数 r（降序，不能把 r 提到最前），再补 0
    assert(max_sum_of_squares_of_digits(3, 20) == "992");
    assert(max_sum_of_squares_of_digits(4, 19) == "9910");
    assert(max_sum_of_squares_of_digits(3, 17) == "980");

    // 规模上界：num = 2e5。sum = 9 时只有首位非零，其余 199999 位全是 0
    {
        const std::string s = max_sum_of_squares_of_digits(200000, 9);
        assert(s.size() == 200000u);
        assert(s.front() == '9');
        assert(s[1] == '0');
        assert(s.back() == '0');
    }
    // sum = 9 * num = 1800000 是上界，全是 9
    {
        const std::string s = max_sum_of_squares_of_digits(200000, 1800000);
        assert(s.size() == 200000u);
        assert(s.front() == '9' && s.back() == '9');
        assert(s.find('0') == std::string::npos);
    }
    // 超出上界（sum 的约束上限 2e6 也在这一侧）-> 空串
    assert(max_sum_of_squares_of_digits(200000, 1800001) == "");
    assert(max_sum_of_squares_of_digits(200000, 2000000) == "");

    // 穷举对拍（小规模全覆盖）：num = 1..3，sum 取遍 0..9 * num + 1（含非法边界）
    for (int num = 1; num <= 3; ++num) {
        for (int sum = 0; sum <= 9 * num + 1; ++sum) {
            assert(max_sum_of_squares_of_digits(num, sum) ==
                   brute_max_sum_of_squares_of_digits(num, sum));
        }
    }

    // 随机对拍：num 到 4，sum 故意取到可行域之外，贪心构造与暴力枚举必须完全一致
    std::mt19937 rng(20261014u);
    std::uniform_int_distribution<int> num_dist(1, 4);
    for (int iter = 0; iter < 500; ++iter) {
        const int num = num_dist(rng);
        std::uniform_int_distribution<int> sum_dist(0, 9 * num + 2);
        const int sum = sum_dist(rng);
        assert(max_sum_of_squares_of_digits(num, sum) ==
               brute_max_sum_of_squares_of_digits(num, sum));
    }

    std::cout << "3723 max_sum_of_squares_of_digits: 全部用例通过"
                 "（含 num <= 3 全覆盖 + 500 组随机对拍）\n";
    return 0;
}
