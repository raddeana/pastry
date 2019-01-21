### Hijacker
- Hijacker 是一款带有图形化界面的渗透测试工具，Hijacker 集成了 Aircrack-ng、Airodump-ng、MDK3 和 Reaver 等工具
- Hijacker 适用于 ARM 架构的 Android 设备，设备的内部无线适配器必须支持 Monitor 模式
- BCM4439芯片（MSM8974，例如Nexus5、Xperia Z1/Z2、LG G2、LG G Flex、Samsung Galaxy Note3）可以搭配Nexmon（它也支持某些其他芯片）

#### 信息收集
- 查看附近的无线接入点（包括隐藏接入点）和基站
- 查看特定网络的活动信息以及客户端信息
- 收集接入点和基站统计信息
- 从OUI数据库获取目标设备厂商信息
- 查看设备信号强度，并过滤出信号强的目标
- 保存捕获到的.cap数据包文件

#### 渗透测试
- 对目标网络中的所有客户端进行去认证操作
- 使用 MDK3 和自定义选项对目标进行 Beacon Flooding 攻击
- 对特定网络或附近 AP 进行 MDK3 Authentication DoS 攻击
- 捕获 WPA 握手包
- ReaverWPS 破解攻击

#### 工具安装
- Android5+
- 设备root（SuperSU）
- 支持Monirot模式的固件
