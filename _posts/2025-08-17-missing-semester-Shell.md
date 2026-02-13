---
layout: post
title: "missing-semester-Shell"
date: 2025-08-17 18:49:47 +0800
categories: blog update
tags: linux
---

[特殊的参数](https://missing.csail.mit.edu/2020/shell-tools/)

- `$0`- 命令的名字
- `$1` to `$9`- 命令的参数
- `$@`- 所有参数
- `$#`- 参数的数量
- `$?`- 上一个命令的返回值
- `$$`- 当前命令的PID
- `!!`- 整个上一条，包括参数，当权限不足时，`sudo !!`
- `$_`- 上一条命令的最后一个参数，输入一下快捷键快速输入`Esc+.`或者`Alt+.`
- `<(CMD)`- 执行`CMD`并且把输出放到一个临时文件然后用文件名替换`<()`.
- `test`- 用于比较，详见`man test`.
- 通配符- `?`代表一位, `*`代表所有的匹配
- 花括号`{}`- 表示列表
- Shebang- 使用绝对路径或者环境变量, `#!/usr/bin/env python`或者`#!/usr/bin/python`
- 引号- 单引号内的内容是字面上的字符串，双引号内的内容可以是变量值
