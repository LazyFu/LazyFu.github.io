---
layout: post
title: "安装ArchLinux"
date: 2025-09-10 14:56:00 +0800
categories: blog update
tags: archlinux 安装
---

**Gentoo快去死吧。**

在[官网下载页面](https://archlinux.org/download/)下载好ISO镜像之后开机还是黑屏，我不知道为什么，在grub界面选中第一个选项，加nomodeset，然后就可以正常开机了。

开机之后先联网，`ip link`查看接口，这里是wlan0，但是不在UP状态，`ip link set wlan0 up`会显示一个rfkill什么东西，需要`rfkill unblock wlan`解锁，可以用`rfkill`查看状态。

之后就可以正常联网了，使用`iw dev wlan0 scan | grep SSID`查找可用的网络。

然后使用`wpa_passphrase "your_wifi" "your_password" > /etc/wpa_supplicant/wpa_supplicant.conf`自动生成配置文件。

接下来启动wpa_supplicant管理网络连接，`wpa_supplicant -i wlan0 -c /etc/wpa_supplicant/wpa_supplicant.conf -B -D nl80211,wext`。

然后`dhcpcd wlan0`获取IP。

`timedatectl`更新时钟。

准备磁盘，跟[上次Gentoo分区]({% post_url 2025-09-07-安装Gentoo %})没有任何区别。这里略过。

格式化分区也一样，但是这里我根分区选择了ext4，因为使用xfs时在后边初始化系统的时候会有错误（Error不是Warning，保险起见还是换了。

然后挂载分区，根分区挂载在`/mnt`，efi分区挂载在`/mnt/boot`。启动swap，`swapon`。

然后修改`/etc/pacman.d/mirrorlist`，替换一下镜像源，在镜像网站可以找到配置说明。

然后安装必要的包，`pacstrap -K /mnt base linux linux-firmware vim networkmanager man-db udisks2 dosfstools`

> 教程说有必要装CPU微码，但是我装了一个amd-ucode后边出现错误，说是已经在/boot中有这个img，就没装这个。

`vim`文本编辑器，也可以用nano，emacs。

`networkmanager`网络管理器，各种网络管理器装一个就行，不要混用，可能产生冲突。

`man-db`使用手册。

`udisks2 dosfstools`这两个不知道有啥用，是文件系统管理类的。

然后fstab，`genfstab -U /mnt >> /mnt/etc/fstab`，告诉系统要挂载哪些设备。

现在`arch-chroot /mnt`切换目录。

然后设置时区，`ln -sf /usr/share/zoneinfo/Region/City /etc/localtime`。

Region比如Asia，City比如Shanghai。

`hwclock --systohc`生成`/etc/adjtime`（不知道有啥用）。

l10n，编辑`/etc/locale.gen`，取消注释en_US.UTF-8，zh_CN.UTF-8。

然后运行`locale-gen`生成区域设置。

编辑`/etc/locale.conf`，设置LANG变量，比如`LANG=en_US.UTF-8`。

网络设置，编辑`/etc/hostname`，在里边输入主机名就可以。

生成initamfs，`mkinitcpio -P`，不知道有什么用，xfs就是在这里报错。

`passwd`设置root密码。

选择一个启动引导项，我使用GRUB，`pacman -S grub efibootmgr`，安装这俩，然后执行`grub-install --target=x86_64-efi --efi-directory=esp --bootloader-id=GRUB`

esp指的是efi分区的挂载点，即`/boot`。

然后生成主配置文件，`grub-mkconfig -o /boot/grub/grub.cfg`，并且每次更改`/etc/default/grub`就要重新执行这个命令重新生成配置文件。

完成之后就可以输入`exit`或者`ctrl+d`退出了，然后卸载挂载的分区，先卸载`umount /mnt/boot`再卸载`umount /mnt`。

之后就可以`reboot`重启，不出意外应该就可以顺利进入系统了。

PS: 之后我又安装了桌面（plasma），但是没有网络，看了两眼没找到在哪连接，慢慢玩吧。
