#### 贪狼Rootkit
"贪狼" Rootkit病毒主要通过ghost系统等渠道感染用户计算机
- Platform.dll 功能模块更新、加载等  
- ConsoleApplication8.dll 门罗币挖矿  
- HydraClient.dll DDOS攻击  
- StevenRobot.dll 上报、下拉配置  
- mLoader.dll 插件加载器  
- UserFramework.dll 插件核心框架、  
- KernelManager.dll 插件管理  
- HSManager.dll 主页劫持  
- WebBrowser.dll 后台广告刷量  
- AppManage64.dll 浏览器劫持  
- SSLHijack1.0.6-win64.dll HTTPS劫持  
- Hijack.dll 浏览器劫持  
- KernelHijack.sys 内核劫持模块

各模块通过联网下载到本地执行，被RC4或AES加密；且都有统一的导出名称BsProcessStartup、BsDllStartup、BsCleanup、BsEnvironment
模块被注入到目标进程后直接调用BsProcessStartup或BsDllStartup
BsProcessStartup和BsDllStartup首先确定加载基址，并获取peb、teb相关信息初始化导出结构体BsEnvironment
然后进行重定位修复、导入表初始化、CRT初始化、添加异常处理表等前置工作，最后进入主功能函数

#### 核心驱动模块
- 驱动加了VMP壳保护，加载后注册进程回调、映像回调、注册表回调、关机回调，并创建3个内核线程
- 通过进程回调APC注入内置Platform.dll 到 lsass.exe，Platform.dll 执行真正的病毒行为
- 映像回调拦截浏览器进程的杀软模块加载
- 注册表回调保护自身驱动服务项
- 关机回调回写自身驱动文件和注册表服务项

#### R3层核心框架Platform.dll
- 加载内置的ConsoleApplication8.dll（挖矿模块）；
- 挂钩NtQuerySystemInformation（针对taskmgr.exe，进程名过滤）；
- 挂钩LdrLoadDll（针对360se.exe、360chrome.exe），过滤杀软的浏览器保护模块
- 访问C&C服务器，内置多个备用服务器地址；上传机器信息并下载配置文件
- 根据配置文件进一步下载其他模块并加载执行
