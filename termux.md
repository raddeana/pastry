#### Termux
Termux是一个Android下一个高级的终端模拟器，开源且不需要root,支持apt管理软件包，十分方便安装软件包，完美支持 Python, PHP, Ruby, Go, Nodejs, MySQL

#### 基本命令
```cmd
pkg search <query>              // 搜索包
pkg install <package>           // 安装包
pkg uninstall <package>         // 卸载包
pkg reinstall <package>         // 重新安装包
pkg update                      // 更新源
pkg upgrade                     // 升级软件包
pkg list-all                    // 列出可供安装的所有包
pkg list-installed              // 列出已经安装的包
pkg shoe <package>              // 显示某个包的详细信息
pkg files <package>             // 显示某个包的相关文件夹路径
```

```
~ > echo $HOME
/data/data/com.termux/files/home
 ~ > echo $PREFIX
/data/data/com.termux/files/usr
 ~ > echo $TMPPREFIX
/data/data/com.termux/files/usr/tmp/zsh
```

#### 更换国内源
- 设置默认编辑器
```cmd
export EDITOR=vi
```
- 编辑源文件
```cmd
apt edit-sources
```

#### 直接编辑源文件
```cmd
vi  $PREFIX/etc/apt/sources.list
```

#### 安装基本工具
```cmd
pkg update
pkg install vim curl wget git unzip unrar
```

#### Termux优化
主要使用了zsh来替代bash作为默认shell. 使用一键安装脚本来安装, 一步到位, 顺便启动了外置存储, 可以直接访问SD卡下的目录
```cmd
sh -c "$(curl -fsSL https://github.com/Cabbagec/termux-ohmyzsh/raw/master/install.sh)"
```

#### 访问外置存储优化
```cmd
ln -s /data/data/com.termux/files/home/storage/shared/tencent/QQfile_recv QQ
```

```cmd
ln -s /data/data/com.termux/files/home/storage/shared/tencent/TIMfile_recv TIM
```

#### MyZsh主题配色
```cmd
vim .zshrc
```
在 .oh-my-zsh/themes 目录下放着 oh-my-zsh 所有的主题配置文件.
- agnoster
- robbyrussell
- jaischeema
- re5et
- junkfood
- cloud

#### random
尝试random主题, 每打开一个会话配色主题都是随机的
```cmd
ZSH_THEME = "random"
```

#### 修改启动问候语
```cmd
vim $PREFIX/etc/motd
```

#### 管理员身份
- 手机没有root
利用proot工具来模拟某些需要root的环境
```cmd
pkg install proot
```

- 手机已经root
安装tsu, 这是一个su的termux版本, 用来在termux上替代su
```cmd
pkg install tsu
```
```cmd
tsu
```

#### 信息安全
- Metasploit: Metasploit 是一款开源的安全漏洞检测工具
- Nmap: 端口扫描必备工具
- hydra: Hydra 是著名的黑客组织THC的一款开源暴力破解工具这是一个验证性质的工具
- sslscan: SSLscan 主要探测基于ssl的服务，如https
- whatportis: whatportis 是一款可以通过服务查询默认端口，或者是通过端口查询默认服务的工具，简单易用
- SQLmap: SQLmap 是一款用来检测与利用SQL注入漏洞的免费开源工具
- RouterSploit: RouteSploit 框架是一款开源的路由器等嵌入式设备漏洞检测及利用框架
- Slowloris: RouteSploit 框架是一款开源的路由器等嵌入式设备漏洞检测及利用框架
- RED_HAWK: 一款采用PHP语言开发的多合一型渗透测试工具，它可以帮助我们完成信息采集、SQL漏洞扫描和资源爬取等任务
- Cupp: Cupp是一款用Python语言写成的可交互性的字典生成脚本
- Hash-Buster: Hash Buster是一个用python编写的在线破解Hash的脚本
- D-TECT: D-TECT是一个用Python编写的先进的渗透测试工具
- WPSeku: WPSeku 是一个用 Python 写的简单的 WordPress 漏洞扫描器
- XSStrike: XSStrike是一种先进的XSS检测工具。它具有强大的模糊测试引擎.

#### PKG

