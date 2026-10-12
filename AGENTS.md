# 仓库约定

本仓库是 LeetCode 题解库。任何人在（包括 AI 助手）新增或修改题解时，请遵循以下约定。

## 文件

- 路径：`src/四位编号_英文题名.cpp`。编号补零到 4 位（`0002` 而不是 `2`），
  这样目录能按编号正确排序。
- 一题一文件，每个文件自带 `main()` 和 `assert` 自测，能独立编译运行。
- 文件头注释固定五段，顺序不能变：
  题目（来源链接 + 难度 + 一句话描述）→ 思路 → 复杂度 → 易错点 → 面试追问。

## 代码

- C++20，只依赖标准库，不引入第三方依赖。
- 解法写成匿名 namespace 里的自由函数，不要用 LeetCode 的 `class Solution` 写法。
- 编译参数是 `-Wall -Wextra -Wpedantic -Wshadow -Wconversion`，**必须零警告**。
  常见踩坑：
  - 不要用变长数组（VLA，如 `int d[n]`），用 `std::vector`。
  - 涉及差值、平方、累加的量一律用 `long long`。
  - 容器元素是 `long long` 时不要用 `greater<int>` 这类窄化比较器。
  - 避免隐式窄化转换，必要时显式 `static_cast`。
  - 返回类型要能装下结果，不要 `long long` 算完用 `int` 返回。
- 有贪心、二分、DP 的题目，额外加一组小规模「暴力枚举对拍」的随机测试，
  用固定种子的 `std::mt19937`，循环里 `assert` 两种解法结果一致。

## README

每加一题更新 README 两处：

1. `## 当前进度`：对应分类 +1，合计 +1。
2. `## 题解索引`：加一行，保持按编号递增，核心思路列写一句话。

## 提交

- 一题一个 commit，代码和 README 放在同一个 commit 里。
- message 格式：`feat: 编号 中文题名`，正文写清核心思路和复杂度。
- 推送前必须本地编译并跑过自测，不能用没跑过的代码提交。

## 环境提示

- 本机编译时 clang 需要可写的临时目录，命令前加 `TMPDIR=/private/tmp`，
  否则会报 `unable to make temporary file: Operation not permitted`。
- 自测命令：`TMPDIR=/private/tmp make`，然后 `./build/<文件名>`。
- 新增或删除 `src/` 下的文件后记得 `make clean`，避免旧的编译产物残留。
