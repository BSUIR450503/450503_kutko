#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <string>
#include <iostream>

using namespace std;

struct Lab5
{
	HANDLE hFile;            
	DWORD NumberOfBytes;	  
	CHAR  tempString[100];			  
	DWORD  posReadfile;	  
	OVERLAPPED Overlapped;
};

#endif

#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <dirent.h>
#include <aio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <errno.h> 

struct Lab5 {
  char readFileName[500];
  char writeFileName[500];
  struct aiocb readCb;
  struct aiocb writeCb;
  char buffer[300];
  int bytesRead;
  int bytesWrite;
};
#endif

#define READ 0
#define EXIT 1
#define WRITE 2
#define SIZE_PATH 100