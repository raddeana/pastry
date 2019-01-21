#### 病毒初始化工作
- 判断程序文件名是否为iis_agent32.exe
- 判断注册表的HKCU\SOFTWARE\ZenisService项中，是否存在Active值
- 检测并结束掉以下进程
  - sql -- MSSQL数据库进程
  - taskmgr -- 任务管理器进程
  - regedit -- 注册表编辑器进程
  - backup -- 备份程序相关进程
  
#### 文件加密部分
Zenis采用的加密手段相对比较传统，是用RSA 1024 + RC4的方式对文件进行加密
病毒在每个用户的机器中会生成一对RSA 1024 Session Key，而对每一个文件会生成一个RC4的会话密钥

##### 使用的密钥概述
- RPUBKEY RSA 2048 Root Public Key
- RPIVKEY RSA 2048 Root Private Key
- SPUBKEY RSA 1024 Session Public Key
- SPIVKEY RSA 1024 Session Private Key
- FILEKEY RC4 256Bit KEY
- USERFLGKEY RC4 512Bit KEY
- ENCFILEKEY 被SPUBKEY加密的FILEKEY
- ENCRYPTED 先使用USERFLGKEY加密SPIVKEY再用RPUBKEY加密后写入Zenis-Instructions.html尾部

#### 不会加密
- Program Files 	 Microsoft.NET 
- ProgramData 	 Internet Explorer 
- PerfLogs 	 Kaspersky Lab 
- $Recycle.Bin 	 McAfee 
- Microsoft 	 Avira 
- Microsoft Help 	 spytech software 
- Microsoft App 	 sysconfig 
- Certification Kit 	 Avast 
- Windows Defender 	 Dr.Web 
- ESET 	 Symantec 
- COMODO 	 Symantec_Client_Security 
- Windows NT 	 system volume information 
- Windows Kits 	 AVG 
- Windows Mail 	 Microsoft Shared 
- Windows Media Player 	 Common Files 
- Windows Multimedia Platform 	 Outlook Express 
- Windows Phone Kits 	 Movie Maker 
- Windows Phone 	 Chrome 
- Silverlight Kits 	 Mozilla Firefox 
- Temp 	 Opera 
- Windows Photo Viewer 	 YandexBrowser 
- Windows Protable Devices 	 ntldr 
- Windows Slidebar 	 Wsus 
- WindowsPowerShell 	 Public 
 
