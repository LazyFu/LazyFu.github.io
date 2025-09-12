---
layout: post
title: "ArchLinux后续"
date: 2025-09-12 22:12:37 +0800
categories: blog update
tags: archlinux
---

## 连接网络

安装系统时装的NetworkManager，可以使用`nmcli`连接（当然也有GUI，但是我觉得CLI更方便），`nmcli device wifi connect "your_wifi_SSID" password "your_password"`

注意要使用systemd启动并且开机自启NetworkManager，`systemctl enable NetworkManager --now`

在桌面还是什么地方有可能是找不到Wi-Fi适配器的，是因为自动禁用了Wi-Fi设备，需要`rfkill unblock wlan`。

## 时区混乱

虽然在之前已经配置好了时区，但是不同系统读取硬件时钟方式不一样，双系统会使两个系统时间显示不一样，最好在Linux中设置好之后让Windows也使用UTC，执行`timedatectl set-local-rtc 0`。

然后在Windows中管理员运行cmd，输入以下命令修改注册表。

```cmd
reg add "HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\TimeZoneInformation" /v RealTimeIsUniversal /t REG_DWORD /d 1
```

然后重启电脑就可以获得正确时间，Windows可以重启explorer。

可以使用`timedatectl status`查看当前时间状态。

使用systemd配置自动时间同步`systemctl enable --now systemd-timesyncd.service`。

## 滚动更新

啊这个玩意滚的确实快，一天`pacman -Syu`了两次，注意内核升级之后要重新启动，否则内核模块可能不会加载影响使用，我就是USB检测不到才发现的这个问题。

`pacman -Q linux`，`uname -r`对比这两个输出，不一样说明需要重启。
