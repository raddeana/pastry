攻击者必须在目标设备的Wi-Fi范围内，才能窃取敏感的用户数据，并在成功利用后执行恶意代码，才有可能导致设备被完全接管

### FragAttacks漏洞中的编程错误漏洞如下
- CVE-2020-24588：聚合攻击（接受非SPP A-MSDU帧）
- CVE-2020-24587：混合密钥攻击（重新组合在不同密钥下加密的片段）
- CVE-2020-24586：片段缓存攻击（当重新连接到网络时没有从内存中清除片段）

### Wi-Fi实施漏洞如下
- CVE-2020-26145：将明文广播片段作为全帧接受（在加密网络中）
- CVE-2020-26144: 接受以EtherType EAPOL的RFC1042头开始的明文A-MSDU帧（在加密的网络中）
- CVE-2020-26140: 在一个受保护的网络中接受明文数据帧
- CVE-2020-26143: 在一个受保护的网络中接受碎片化的明文数据帧

### Vanhoef发现的其他实施漏洞如下
- CVE-2020-26139: 转发EAPOL帧，即使发送者还没有被验证（应该只影响AP）
- CVE-2020-26146: 用不连续的包号重新组合加密的片段
- CVE-2020-26147: 重新组装混合的加密/纯文本片段
- CVE-2020-26142: 将片段帧作为全帧处理
- CVE-2020-26141: 没有验证片段帧的TKIP MIC

GitHub上还提供了一个开源工具，可以用来检查用户网络上的接入点和Wi-Fi客户端是否受到FragAttacks漏洞的影响
Github地址：https://github.com/vanhoefm/fragattacks