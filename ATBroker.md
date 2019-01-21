#### ATBroker.exe 简介
通过利用atbroker.exe自启病毒母体，然后通过rundll32加载恶意模块，最后实现文件的md5修改、释放和执行挖矿程序

- ATBroker.exe（C:\Windows\System32目录下），源于微软的 "轻松访问中心"
- "轻松访问中心" 的一项功能是帮助用户启动辅助功能应用程序，常用的包括讲述人，屏幕键盘和放大镜
- 这意味着第三方程序也可以通过注册 "轻松访问中心" 的方式来启动

##### ATBroker.exe 攻击手段
- 锁屏或者登录时
- 开机启动时
- 运行atbroker.exe时
- 按下 Ctrl + Alt + Del 时
- 触发 UAC 时

##### zec币挖矿攻击
- 隐藏母体，让病毒长期驻存和活跃
- 不断改变MD5逃避杀软查杀

##### 自启动
- 注册表HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Accessibility\ATs下创建了一个Zlocal39的项
- 其中StartExe的值指向病毒母体，StartParams的内容是运行参数 woshiyizhixiaomaolv
- 注册表 HKCU\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Accessibility\Session1 下的 Configuration 的值为 Zlocal39
- 通过processmon 的开机监控发现，系统登录时会启动atbroker.exe，然后atbroker.exe接着执行恶意程序Zloc.exejPnAtDoPy.exe，参数 woshiyizhixiaomaolv

##### 样本分析
- IDA 打开 Zloc.exe jPnAtDoPy.exe (MD5:27d6209b6e10183a126906a714390577)
- Zloc.exejPnAtDoPy.exe主要是在temp目录下释放文件名随机，后缀名为lbll的模块并加载
- 释放并加载ibll文件后，接着起rundll32加载idTemp.data，函数入口CgvSEcXgGkbWwkFa
- IDA打开idTemp.data (MD5：201136f459c3982bc75d440f8afe40c4), 该模块主要是创建线程和释放挖矿文件到 C:\ProgramData\GTOrigin 目录下, 并解压挖矿文件到%appdata%/Wtrl目录下
- Sub_10004A20()关键代码
- 释放文件
- 到该目录下查看这些文件，其中ini的文件为加密的配置文件，Wtrl.cab是包含挖矿模块（后来证实是antpool的zec挖矿模块）的压缩包
- Wtrl.cab的文件最后被释放到 %appdata%/Wtrl 目录下
- 接着rundll32调起Mine.exe（MD5：c849f42721cc0a0d66f405602126ecf0）, 运行参数 woshiyizhixiaomaolv
- Mine.exe调起ThundCloud.exe（d9c6ab06dd0ea7703eff952d40c4e4ce），运行参数依然是 woshiyizhixiaomaolv
- ThundCloud.exe最后加载同目录下的挖矿模块，用wireshark 抓包得到矿池地址
