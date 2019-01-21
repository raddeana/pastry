#### 概述
后门病毒Zeus Panda Banker通过伪装成 "您的信用卡在苹果商城消费了了2812.56美元" 的邮件进行传播, 用户点击恶意链接后, 将会下载的Word文档

#### 技术细节
- 用户点击恶意链接后，将会下载的Word文档（如下图）
- 病毒作者将文档伪造成内容被加密保护，用户必须开启宏才可以显示内容，为了防止分析，病毒作者将宏代码高度混淆加密
- Zeus Panda Banker使用ZwWriteVirtualMemory将解密后的PE文件写入到svchost

#### 攻击流程图 & Shellcode
##### 攻击流程图 & Shellcode
Zeus Panda Banker通过 shellcode解密出一个新的Win32PE文件并加载到自身内存空间执行
然后进行修复导入表、重定位、最后修正OEP开始执行功能代码
xx.doc = 宏释放 => PandaBanker.exe <= 远程执行 => Svchost.exe => CreateThread
                  PandaBanker.exe <=> Shell Code



##### 反调试检测
通过遍历进程列表、读取注册表、文件路径和一些系统监控工具的设备名称，判断是否被反病毒人员进行分析
##### 注入Svchost.exe
##### 功能模块分析

#### 自我保护
#### 收集信息
#### 远程指令
- botnet 僵尸网络 
- check_config 检查配置文件 
- send_report 发送报告 
- check_update 检查更新 
- url_config 设置URL 
- url_webinjects 注入URL 
- url_update 更新木马地址 
- url_plugin_vnc32 VNC模块32位下载地址 
- url_plugin_vnc64 VNC模块64位下载地址 
- url_plugin_vnc_backserver VNC反弹服务地址 
- url_plugin_grabber 密码拦截模块下载地址 
- url_plugin_backsocks 反弹SOCK模块下载地址 
- url_plugin_backsocks_backserver 反弹SOCK服务下载地址 
- reserved 未知 
- dgaconfigs 未知 
- grabber_pause 暂停记录 
- grab_softlist 获取软件列表
- grab_pass 获取浏览器中保存的密码
- grab_form 获取网页表单信息
- grab_cert 获取浏览器证书信息
- grab_cookie 获取Cookie
- grab_del_cookie 删除Cookie
- grab_del_cache 删除缓存
- notify_process 通知程序
- url_plugin_keylogger 键盘记录模块下载地址
- keylog_process 键盘记录进程
- screen_process 获取运行的进程列表
- url_plugin_webinject32 web注入模块32位下载地址
- url_plugin_webinject64 web注入模块64位下载地址
- remove_csp 卸载CSP
- inject_vnc 注入VNC
- webfilters 过滤网络请求
- webinjects 注入网页

#### 卸载功能
#### IOCs & URLs
