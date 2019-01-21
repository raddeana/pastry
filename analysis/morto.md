#### 母体（Morto.exe）
- 将伪装clb.dll释放到C:\Windows, 这样就会比C:\Windows\System32下的合法clb.dll先加载（Windows寻找文件的特性，根目录的先被读取）
- 写入恶意代码 HKLM\System\WPA\md
- 运行regedit, regedit会自动加载恶意的clb.dll

#### Loader（clb.dll）
- 将自己备份一份cache.txt到C:\Windows\offline web pages下
- 运行\md下的恶意代码

#### Payload
- 释放\\tsclient\a\%smoto
- 释放\\tsclient\a\a.dll和\\tsclient\a\r.reg
- DNS请求和进行RDP爆破
- 修改注册表
- 将cache.txt注册为服务
