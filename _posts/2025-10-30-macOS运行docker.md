---
layout: post
title:  "macOS运行docker"
date:   2025-10-30 12:30 +0800
categories: blog update
tag: 解决方案
---

macOS需要借助colima创建VM来运行docker daemon，进而运行docker。

```bash
brew install colima docker
```

然后`colima start`来启动虚拟机，之后就可以正常使用docker。

但是这时候可能遇到`docker pull` `docker search`的网络连接问题，所以可以在启动colima时带上环境变量。

```bash
colima start --env HTTPS_PROXY=http://your-proxy:port
```

配置镜像可以在colima配置也可以在docker配置，但是比较麻烦不如直接开代理。
