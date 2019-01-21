#### 移动互联网深入普及和物联网开始规模应用的今天
- SBC：UP Board， Intel Atom x5-Z8350 四核CPU，4GB RAM，64GB eMMC
- SDR：USRP B200mini + USB电缆


#### 软件准备
- Ubuntu 16.04
```cmd
sudo apt-get update
sudo apt-get upgrade
```

- Git
```cmd
sudo apt-get install git
```

- Shadowsocks、Privoxy
```cmd
sudo apt-get install privoxy python-gevent python-pip
sudo pip install shadowsocks
```

#### eNodeB: OpenAirInterface
```cmd
git clone
https://gitlab.eurecom.fr/oai/openairinterface5g.git
cd openairinterface5g
git checkout develop
source oaienv
cd cmake_targets
./build_oai -I --eNB -x --install-system-files -w USRP
```
#### EPC: OpenAir-CN
```cmd
git clone
https://gitlab.eurecom.fr/oai/openair-cn.git
cd openair-cn
git checkout develop
cd scripts
./build_hss -i
./build_mme -i
./build_spgw -i
./build_hss
./build_mme
./build_spgw
```

### 配置
#### eNodeB
```cmd
tracking_area_code  =  "13";
mobile_country_code =  "460";
mobile_network_code =  "01";

Nid_cell = 0;

// MME parameters:
mme_ip_address = ({ 
  ipv4 = "127.0.1.10";
  ipv6 = "192:168:30::17";
  active = "yes";
  preference = "ipv4";
});

NETWORK_INTERFACES: {
  ENB_INTERFACE_NAME_FOR_S1_MME = "lo";
  ENB_IPV4_ADDRESS_FOR_S1_MME = "127.0.1.2/8";

  ENB_INTERFACE_NAME_FOR_S1U = "lo";
  ENB_IPV4_ADDRESS_FOR_S1U = "127.0.6.2/8";
  ENB_PORT_FOR_S1U = 2152;
};
```

#### EPC
```hosts
127.0.0.1 localhost
127.0.1.1 mini.openair4G.eur mini
127.0.33.1 hss.openair4G.eur hss
```

#### 复制EPC配置文件
```cmd
sudo mkdir -p /usr/local/etc/oai/freeDiameter
sudo cp ~/openair-cn/etc/mme.conf /usr/local/etc/oai
sudo cp ~/openair-cn/etc/hss.conf /usr/local/etc/oai
sudo cp ~/openair-cn/etc/spgw.conf /usr/local/etc/oai
sudo cp ~/openair-cn/etc/acl.conf /usr/local/etc/oai/freeDiameter
sudo cp ~/openair-cn/etc/mme_fd.conf /usr/local/etc/oai/freeDiameter
sudo cp ~/openair-cn/etc/hss_fd.conf /usr/local/etc/oai/freeDiameter
```

#### 编辑 hss.conf
```
## MySQL mandatory options
MYSQL_server = "127.0.0.1";
MYSQL_user   = "root";
MYSQL_pass   = "linux";
MYSQL_db     = "oai_db";
```

- 编辑mme.conf
```
REALM = "openair4G.eur";

S6A : {
  S6A_CONF = "/usr/local/etc/oai/freeDiameter/mme_fd.conf"; # YOUR MME freeDiameter config file path
  HSS_HOSTNAME = "hss";                                     # THE HSS HOSTNAME
};

GUMMEI_LIST = ({MCC="460" ; MNC="01"; MME_GID="4" ; MME_CODE="13"; });   # YOUR GUMMEI CONFIG HERE

TAI_LIST = ({MCC="460"; MNC="01"; TAC = "13"; });                      # YOUR PLMN CONFIG HERE

 NETWORK_INTERFACES : {
  # MME binded interface for S1-C or S1-MME  communication (S1AP), can be ethernet interface, virtual ethernet interface, we don't advise wireless interfaces
  MME_INTERFACE_NAME_FOR_S1_MME         = "lo";                          # YOUR NETWORK CONFIG HERE
  MME_IPV4_ADDRESS_FOR_S1_MME           = "127.0.1.10/8";                # YOUR NETWORK CONFIG HERE

  # MME binded interface for S11 communication (GTPV2-C)
  MME_INTERFACE_NAME_FOR_S11_MME        = "lo";                          # YOUR NETWORK CONFIG HERE
  MME_IPV4_ADDRESS_FOR_S11_MME          = "127.0.8.11/8";                # YOUR NETWORK CONFIG HERE
  MME_PORT_FOR_S11_MME                  = 2123;                          # YOUR NETWORK CONFIG HERE
};

S-GW : {
  # S-GW binded interface for S11 communication (GTPV2-C), if none selected the ITTI message interface is used
  SGW_IPV4_ADDRESS_FOR_S11 = "127.0.8.1/8";            # YOUR NETWORK CONFIG HERE
};
```

#### 编辑spgw.conf
```
S-GW: {
  NETWORK_INTERFACES: {
    # S-GW binded interface for S11 communication (GTPV2-C), if none selected the ITTI message interface is used
    SGW_INTERFACE_NAME_FOR_S11              = "lo";                    # YOUR NETWORK CONFIG HERE
    SGW_IPV4_ADDRESS_FOR_S11                = "127.0.8.1/8";            # YOUR NETWORK CONFIG HERE

    # S-GW binded interface for S1-U communication (GTPV1-U) can be ethernet interface, virtual ethernet interface, we don't advise wireless interfaces
    SGW_INTERFACE_NAME_FOR_S1U_S12_S4_UP    = "lo";                       # YOUR NETWORK CONFIG HERE, USE "lo" if S-GW run on eNB host
    SGW_IPV4_ADDRESS_FOR_S1U_S12_S4_UP      = "127.0.6.1/8";           # YOUR NETWORK CONFIG HERE
    SGW_IPV4_PORT_FOR_S1U_S12_S4_UP         = 2152;                         # PREFER NOT CHANGE UNLESS YOU KNOW WHAT YOU ARE DOING

    # S-GW binded interface for S5 or S8 communication, not implemented, so leave it to none
    SGW_INTERFACE_NAME_FOR_S5_S8_UP         = "none";                       # DO NOT CHANGE (NOT IMPLEMENTED YET)
    SGW_IPV4_ADDRESS_FOR_S5_S8_UP           = "0.0.0.0/24";                 # DO NOT CHANGE (NOT IMPLEMENTED YET)
  };
}


P-GW = {
  NETWORK_INTERFACES : {
    # P-GW binded interface for S5 or S8 communication, not implemented, so leave it to none
    PGW_INTERFACE_NAME_FOR_S5_S8          = "none";                         # DO NOT CHANGE (NOT IMPLEMENTED YET)
    PGW_IPV4_ADDRESS_FOR_S5_S8            = "0.0.0.0/24";                   # DO NOT CHANGE (NOT IMPLEMENTED YET)

    # P-GW binded interface for SGI (egress/ingress internet traffic)
    PGW_INTERFACE_NAME_FOR_SGI            = "eth0";                         # YOUR NETWORK CONFIG HERE
    PGW_IPV4_ADDRESS_FOR_SGI              = "192.168.12.82/24";             # YOUR NETWORK CONFIG HERE
    PGW_MASQUERADE_SGI                    = "yes";                          # YOUR NETWORK CONFIG HERE
  };
  
  # DNS address communicated to UEs
  DEFAULT_DNS_IPV4_ADDRESS     = "192.168.106.12";                            # YOUR NETWORK CONFIG HERE
  DEFAULT_DNS_SEC_IPV4_ADDRESS = "192.168.12.100";                            # YOUR NETWORK CONFIG HERE
}
```

#### HSS freediameter
```conf
Identity = "hss.openair4G.eur";
Realm = "openair4G.eur";
```

#### MME freediameter
```conf
Identity = "mini.openair4G.eur";
Realm = "openair4G.eur";
ConnectPeer= "hss.openair4G.eur" { ConnectTo = "127.0.33.1"; No_SCTP ; No_IPv6; Prefer_TCP; No_TLS; port = 3868;  realm = "openair4G.eur";};
```

#### 运行
- 安装证书
```cmd
cd ~/openair-cn/scripts
./check_hss_s6a_certificate /usr/local/etc/oai/freeDiameter/ hss.openair4G.eur
./check_mme_s6a_certificate /usr/local/etc/oai/freeDiameter/ mini.openair4G.eur
```

- 运行 HSS
```cmd
cd ~/openair-cn/scripts
./run_hss -i ~/openair-cn/src/oai_hss/db/oai_db.sql
```

- 每次运行 HSS
```
cd ~/openair-cn/scripts
./run_hss
```

- 运行MME
```cmd
cd ~/openair-cn/scripts
./run_mme

- SP-GW
- 运行eNB
```cmd
cd ~/openairinterface5g
source oaienv
cd cmake_targets/lte_build_oai/build
sudo -E ./lte-softmodem -O $OPENAIR_DIR/targets/PROJECTS/GENERIC-LTE-EPC/CONF/enb.band3.tm1.usrpb200.conf
```

- 高级玩法
  - 对TD-LTE的支持
  - 精简代码
  - 连接运营商核心网
  - 同一硬件集成LTE+GSM伪基站
  - 架设自己的测试网
