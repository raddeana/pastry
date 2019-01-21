#### 替换通信协议的处理函数
FreeBSD内核. 通信协议的protosw是这样定义的
- ICMP
```
{
    .pr_type = SOCK_RAW,
    .pr_domain = &inetdomain,
    .pr_protocol = IPPROTO_ICMP,
    .pr_flags =	PR_ATOMIC|PR_ADDR|PR_LASTHDR,
    .pr_input =	icmp_input,
    .pr_ctloutput = rip_ctloutput,
    .pr_usrreqs =	&rip_usrreqs
}
```
- TCP
```
{
    .pr_type =		SOCK_STREAM,
    .pr_domain =		&inetdomain,
    .pr_protocol =		IPPROTO_TCP,
    .pr_flags =		PR_CONNREQUIRED|PR_IMPLOPCL|PR_WANTRCVD,
    .pr_input =		tcp_input,
    .pr_ctlinput =		tcp_ctlinput,
    .pr_ctloutput =		tcp_ctloutput,
    .pr_init =		tcp_init,
    .pr_slowtimo =		tcp_slowtimo,
    .pr_drain =		tcp_drain,
    .pr_usrreqs =		&tcp_usrreqs
}
```
- UDP
```
{
    .pr_type = SOCK_DGRAM,
    .pr_domain = &inetdomain,
    .pr_protocol = IPPROTO_UDP,
    .pr_flags = PR_ATOMIC|PR_ADDR,
    .pr_input = udp_input,
    .pr_ctlinput = udp_ctlinput,
    .pr_ctloutput = ip_ctloutput,
    .pr_init = udp_init,
    .pr_usrreqs = &udp_usrreqs
}
```

通信协议mbuf内的信息，替换掉pr_input
```
case MOD_LOAD:
  /* Replace udp_input with udp_input_hook. */
  inetsw[ip_protox[IPPROTO_UDP]].pr_input = udp_input_hook;
  break;
```

#### 设计hook函数
作为hook时使用的函数，需要和原有函数相同的定义
```
int
udp_input_hook(struct mbuf **mp, int *offp, int proto);
```

```
if (iphlen > sizeof (struct ip)) {
	ip_stripoptions(m);
	iphlen = sizeof(struct ip);
}
```

```
ip = mtod(m, struct ip *);
if (m->m_len < iphlen + sizeof(struct udphdr)) {
	if ((m = m_pullup(m, iphlen + sizeof(struct udphdr))) == NULL) {
		UDPSTAT_INC(udps_hdrops);
		return (IPPROTO_DONE);
	}
  
	ip = mtod(m, struct ip *);
}
```

```
uh = (struct udphdr *)((caddr_t)ip + iphlen);
```

```
m->m_len -= iphlen + m->m sizeof(struct udphdr);
m->m_data += iphlen + sizeof(struct udphdr);
```

获取UDP头
```
uh = (struct udphdr *)((caddr_t)ip + iphlen);
```

```
m->m_len -= iphlen + m->m sizeof(struct udphdr);
m->m_data += iphlen + sizeof(struct udphdr);
```

```
printf(("load=%s \n""load=%s \n", , mm->m_data);->m_data);
```

```
m->m_len += iphlen + m->m_len +=  sizeof(struct udphdr);
m->m_data -= iphlen + sizeof(struct udphdr);
```

```
return udp_input(mp, offp, proto); udp_inpu
```
