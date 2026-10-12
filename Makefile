# 算法题解库构建脚本
# 用法：
#   make                       编译 src/ 下所有题解到 build/
#   make run NAME=0001_two_sum 编译并运行单题
#   make list                  列出所有题目
#   make clean                 清理

# 用 := 而不是 ?=，因为 make 对 CXX 有内置默认值（g++），?= 不会生效。
# 需要换编译器时用命令行覆盖：make CXX=g++
CXX      := clang++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -Wpedantic -Wshadow -Wconversion
BUILD    := build

SRCS := $(wildcard src/*.cpp)
BINS := $(patsubst src/%.cpp,$(BUILD)/%,$(SRCS))

.PHONY: all run clean list

all: $(BINS)

$(BUILD)/%: src/%.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $< -o $@

run:
	@test -n "$(NAME)" || { echo "用法: make run NAME=0001_two_sum"; exit 1; }
	@$(MAKE) --no-print-directory $(BUILD)/$(NAME)
	@./$(BUILD)/$(NAME)

list:
	@ls src/*.cpp 2>/dev/null | sed 's|src/||; s|\.cpp$$||' || echo "(还没有题解)"

clean:
	rm -rf $(BUILD)

