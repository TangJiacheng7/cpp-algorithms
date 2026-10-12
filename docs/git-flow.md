# Git 速查（日常够用版）

## 一、日常三步

```bash
git add -A
git commit -m "feat: 完成两数之和的哈希表解法"
git push
```

## 二、搞不清状态时，先敲这句

```bash
git status
```

任何时候卡住了，先 `git status`，它会告诉你现在在哪、有什么没提交。

## 三、查看

| 命令 | 作用 |
|---|---|
| `git log --oneline -10` | 最近 10 条提交，一行一个 |
| `git log --oneline --graph --all` | 带分支图的历史 |
| `git diff` | 工作区 vs 暂存区（还没 add 的改动） |
| `git diff --staged` | 暂存区 vs 最后一次提交（已 add 的改动） |
| `git show <commit>` | 看某次提交具体改了什么 |

## 四、撤销

| 场景 | 命令 | 说明 |
|---|---|---|
| 丢弃某文件未提交的改动 | `git restore <文件>` | 不可恢复，谨慎 |
| 取消 add（保留改动） | `git restore --staged <文件>` | 安全 |
| 改最后一次提交信息 | `git commit --amend -m "新信息"` | 只能用于**还没 push** 的提交 |
| 补文件到最后一次提交 | `git add <文件>` 然后 `git commit --amend --no-edit` | 同上 |

**已经 push 出去的提交不要用 amend**——会造成本地和远端历史不一致。已经推上去的，就在下一次提交里修。

## 五、分支

小项目在 `main` 上直接提交是可以的。等你开始做正式项目（Lab、课程设计），用这样一套：

```bash
git switch -c feat/vector-impl     # 建并切到新分支
# ... 改代码、提交 ...
git switch main                    # 切回 main
git merge feat/vector-impl         # 合并
git push                           # 推 main
git branch -d feat/vector-impl     # 删掉已合并的分支
```

## 六、常见报错怎么处理

**`Author identity unknown` / `Please tell me who you are`**

没配 git 身份：

```bash
git config --global user.name "你的名字"
git config --global user.email "你的邮箱@example.com"
```

**`Permission denied (publickey)`**

SSH key 没配好。检查：

```bash
ls -l ~/.ssh/id_ed25519.pub     # 公钥存在吗
ssh -T git@github.com           # 测试
```

如果提示 key 没加载：

```bash
eval "$(ssh-agent -s)"
ssh-add --apple-use-keychain ~/.ssh/id_ed25519
```

**`Updates were rejected because the remote contains work that you do not have`**

远端有你本地没有的提交（比如在网页上改过 README）。先拉：

```bash
git pull --rebase
git push
```

**`fatal: not a git repository`**

你不在仓库目录里，或者还没 `git init`。用 `pwd` 和 `ls -a` 确认。

**clone / push 特别慢或超时**

配代理（端口换成你自己代理软件的）：

```bash
git config --global http.proxy http://127.0.0.1:7890
git config --global https.proxy http://127.0.0.1:7890
```

取消：

```bash
git config --global --unset http.proxy
git config --global --unset https.proxy
```

**误把大文件提交了**

```bash
git rm --cached <大文件>
echo "<大文件路径>" >> .gitignore
git commit -m "chore: 移除误提交的大文件"
```

注意：这只会从**当前版本**移除，它仍然留在历史里，仓库体积不会变小。真要从历史里彻底删掉需要 `git filter-repo`，属于进阶操作，遇到再说。

## 七、提交信息规范

| 前缀 | 用在什么情况 |
|---|---|
| `feat:` | 新功能 |
| `fix:` | 修 bug |
| `docs:` | 只改文档 |
| `refactor:` | 重构，不改外部行为 |
| `test:` | 加/改测试 |
| `perf:` | 性能优化 |
| `chore:` | 构建脚本、依赖、配置 |

避免：`update`、`修改`、`111`、`.` 这种等于没写的提交信息。
