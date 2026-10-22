# C++ 算法题解库

用 C++ 系统刷算法题，每道题留存思路演进、复杂度分析、易错点和面试追问。
目标：服务 408 数据结构 / 考研机试 / 大厂面试，同时保持长期、连续的编码记录。

<!-- 上面一句话请改成你自己的表述。第一行是别人对你仓库的第一印象。 -->

## 当前进度

已解决 **7** 题：简单 4 题，中等 3 题。

| 分类 | 已解决 |
|---|---|
| 字符串 / 栈 | 1 |
| 数组 / 数学 | 2 |
| 贪心 / 排序 | 4 |
| **合计** | **7** |

## 题解索引

| 编号 | 题目 | 难度 | 核心思路 | 时间复杂度 | 代码 |
|---|---|---|---|---|---|
| 1021 | [删除最外层的括号](https://leetcode.cn/problems/remove-outermost-parentheses/) | 简单 | 跟踪嵌套深度，跳过每个原语的最外层括号 | O(n) | [cpp](src/1021_remove_outer_parentheses.cpp) |
| 2089 | [找出数组排序后的目标下标](https://leetcode.cn/problems/find-target-indices-after-sorting-array/) | 简单 | 统计小于、等于目标值的元素数，直接生成连续下标区间 | O(n) | [cpp](src/2089_find_target_indices_after_sorting_array.cpp) |
| 2333 | [最小差值平方和](https://leetcode.cn/problems/minimum-sum-of-squared-difference/) | 中等 | 合并 k1+k2 为总预算，差值降序后逐层拉平、余量平摊 | O(n log n) | [cpp](src/2333_min_sum_square_diff.cpp) |
| 2778 | [特殊元素平方和](https://leetcode.cn/problems/sum-of-squares-of-special-elements/) | 简单 | 枚举位置 1..n，n % i == 0 的累加平方 | O(n) | [cpp](src/2778_sum_of_squares.cpp) |
| 3550 | [数位和等于下标的最小下标](https://leetcode.cn/problems/smallest-index-with-digit-sum-equal-to-index/) | 简单 | 从左往右扫，逐位剥离求数位和，首个相等即最小下标 | O(n·d) | [cpp](src/3550_smallest_index_with_digit_sum_equal.cpp) |
| 3723 | [数位平方和的最大值](https://leetcode.cn/problems/maximize-sum-of-squares-of-digits/) | 中等 | 平方在定和下是凸的，故除 9 外至多留一个余数数字，降序拼串即最大整数 | O(num) | [cpp](src/3723_maximize_sum_of_squares_of_digits.cpp) |
| 3727 | [最大交替平方和](https://leetcode.cn/problems/maximum-alternating-sum-of-squares/) | 中等 | 只有绝对值重要，正号位放绝对值最大的 ceil(n/2) 个 | O(n log n) | [cpp](src/3727_maximum_alternating_sum_of_squares.cpp) |

<!-- 新增题目时，请在表格里加一行，保持按编号递增。 -->

## 目录结构

```
.
├── src/            # 题解源码，一题一文件，命名 四位编号_题目名.cpp
├── docs/           # git 速查、专题笔记
├── build/          # 编译产物（已 gitignore）
├── Makefile        # 编译和运行
└── .clang-format   # 统一代码风格
```

## 每道题的写法

每个 `.cpp` 文件包含五部分，顺序固定：

1. **题目**：链接、难度、一句话描述（含输入输出约束）
2. **思路**：从暴力解法到最优解法的演进，以及为什么能优化
3. **复杂度**：时间 / 空间，并说明瓶颈在哪
4. **易错点**：边界条件、下标、溢出
5. **面试追问**：这题还能怎么变形？换输入会怎样？

参考 `src/1021_remove_outer_parentheses.cpp`（简单题的标准写法）和
`src/2333_min_sum_square_diff.cpp`（中等题，除常规用例之外还带了一组
「暴力枚举对拍」的随机测试）。

## 如何运行

```bash
make                                        # 编译全部题解
make run NAME=1021_remove_outer_parentheses # 编译并运行单题
make list                                   # 列出 src/ 下所有题目
make clean                                  # 清理 build/
```

只依赖 clang++ / g++ 和 make，不需要额外安装。

## 参考

- LeetCode 题面版权归 LeetCode 所有，本仓库仅保留题号、链接与自己的解法和分析。
- 《算法导论》《STL 源码剖析》

