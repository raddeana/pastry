#### Linux.XorDDoS
- MalwareMustDie发现Linux.XorDDoS，它构成了分布式拒绝服务攻击的僵尸网络
- Linux.XorDDoS恶意家族主要特点是，用暴力猜解目标主机ssh弱密码的方式
- 入侵目标主机，然后执行相应的shell脚本安装Linux.XorDDoS恶意家族以及恶意RootKit感染客户主机

#### 样本分析

##### 病毒会先设置相应的环境变量
```
/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:/usr/X11R6/bin
```

```
/boot  /var/run/sftp.pid   /lib/udev/udev  /lib/udev/  /var/run
```

##### 能过解密函数，解密出daemonname
##### 创建相应的守护进程
##### 获得自身进程文件的路径，判断是否在/boot目录下
##### 获取进程的共享内存，然后执行后面的恶意操作
##### 这个样本包含安装rootkit的代码，但并未执行，而是能过安装脚本进行rootkit安装的
##### 加载自启动服务项，先将在/boot中运行的主恶意程序，写入到/etc/init.d/随机文件名启动项中
##### 检测是否存在rootkit恶意驱动程序
##### 修改自身文件的用户属性，然后能过解密函数解密出远程恶意服务器的网址
##### 对解密出来的恶意网站，能过RootKit恶意程序隐藏相应的网络端口
##### 创建线程，执行恶意网络连接操作，通过uname获得主机的相关信息
##### 解密相应的返回数据包的数据

#### 执行不同的操作
- 添加DDOS任务
- 创建线程，对解密出来的IP地址，执行DDOS攻击

#### 清理样本
- 清除/lib/udev/目录下的udev程序
- 清除/boot下的随机恶意文件（10个随机字符串数字）
- 清除/etc/cron.hourly/cron.sh和/etc/crontab定时器文件相关内容
- 如果有RootKit驱动模块，需要卸载相应的驱动模块，此次恶意程序主要它来隐藏相关的网络IP端口
- 清除/lib/udev目录下的debug程序

#### IOC
##### MD5
```
0B3456561B7942AA67403CDDC1FAD2BD
```
##### URL
```
zxchk.xicp.net: 5009
23.234.52.54: 5009
```

