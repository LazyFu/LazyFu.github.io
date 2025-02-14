---
layout: post
title:  "将frp部署在服务器和Windows实现远程连接"
date:   2025-02-14 14:48:32 +0800
categories: blog update
---

## 服务器端

获取frp发布版，打开压缩包，递归复制文件夹改名

```shell
wget https://github.com/fatedier/frp/releases/download/v0.61.1/frp_0.61.1_linux_amd64.tar.gz
tar -zxvf frp_0.61.1_linux_amd64.tar.gz
cp -r frp_0.61.1_linux_amd64 frp
```

`cd frp`之后修改服务器配置文件`vim frps.toml`

配置参考<https://github.com/fatedier/frp/blob/dev/conf/frps_full_example.toml>

```toml
bindAddr = "0.0.0.0"
bindPort = 7000

log.to = "./frps.log"
log.level = "info"
log.maxDays = 7

auth.method = "token"
auth.token = "111" #修改token
```

修改完之后可以运行`./frps -c frps.toml`，之后没有输出，等待一会`^c`中断程序，`cat frps.log`查看运行日志，看是否有*successful*字样

然后将*frp*目录移动到系统文件夹

```shell
cd ..
sudo mv frp /usr/local/
```

可以使用*systemd*托管服务自动运行，在`/etc/systemd/system/`目录下创建一个名为`frps.service`的文件,内容如下：

```service
[Unit]
Description=FRP Server Service

[Service]
Type=simple
# 如有需要，可以指定非root用户：User=frpuser
WorkingDirectory=/usr/local/frp
ExecStart=/usr/local/frp/frps -c /usr/local/frp/frps.toml
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

>**[Service]** 部分
>
>`WorkingDirectory`：设定 frps 所在目录，保证配置文件中使用的相对路径正确。
>
>`ExecStart`：指定启动 frps 的命令，注意这里使用绝对路径。
>
>`Restart=always`与`RestartSec=10`：确保 frps 意外退出后10秒内自动重启。
>
>**[Install]** 部分
>
>`WantedBy=multi-user.target`：使服务在系统进入多用户模式时自动启动。

修改完成后保存并执行以下命令：

```shell
sudo systemctl daemon-reload
sudo systemctl start frps.service
sudo systemctl status frps.service
```

这将重新加载所有服务的配置文件，然后启动*frps*，最后可以查看服务运行状态

## 客户端

同样下载[frp_windows](https://github.com/fatedier/frp/releases/download/v0.61.1/frp_0.61.1_windows_amd64.zip)，可能会报毒但是不用管他,详细情况可以查看issue[2095](https://github.com/fatedier/frp/issues/2095)

解压后编辑`frpc.toml`，参考<https://github.com/fatedier/frp/blob/dev/conf/frpc_full_example.toml>如下：

```toml
serverAddr = "0.0.0.0" # 替换为服务器IP
serverPort = 7000 #与frps.toml保持一致

log.to = "./frpc.log"
log.level = "info"
log.maxDays = 3

auth.method = "token"
auth.token = "111" #与frps.toml保持一致

transport.tls.enable = true #传输使用tls加密但好像没用

[[proxies]]
name = "remoteDesk"
type = "tcp"
localIP = "127.0.0.1"
localPort = 3389 #远程桌面默认端口 
remotePort = 6000
```

之后在powershell的frp目录下运行`frpc.exe -c frpc.toml`，同样没有输出，可以查看日志

同样可以将frpc在Windows注册为服务，首先创建 *.bat* 文件，内容如下：

```cmd
cd C:\Users\xx\frp #cd到frp文件夹绝对路径，xx->你的用户名
frpc.exe -c frpc.toml
```

然后在管理员权限的cmd中

```cmd
#binPath为frp文件夹绝对路径
sc create frpc binPath= "C:\Users\xx\frp" start= auto
```

## 继续

服务器防火墙要开启配置文件中指定端口，如6000和7000

```shell
sudo iptables -A INPUT -p tcp --dport 7000 -j ACCEPT
sudo iptables -A INPUT -p tcp --dport 6000 -j ACCEPT
```

另一个电脑使用windows-app，输入服务器IP和端口如`x.x.x.x:xx`，即可连接