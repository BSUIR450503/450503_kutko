#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
void Create_win()
{
	printf ("In OS Windows\n");
	printf ("This is Parent Process\n");
	PROCESS_INFORMATION ProcInfo;
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO); 
	ZeroMemory(&ProcInfo, sizeof(ProcInfo));
	char * ChildName = "child.exe";
	HANDLE hPipe;
	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe");
	int numb;
	printf ("Input number\n");
	scanf("%d", &numb);
	DWORD NumberOfBytesWritten;
	DWORD NumberOfBytesRead;
	NumberOfBytesWritten=sizeof(int);
	NumberOfBytesRead=sizeof(int);
	
	hPipe = CreateNamedPipe( 
          PipeName,			  
          PIPE_ACCESS_DUPLEX,       
          PIPE_TYPE_BYTE |       
          PIPE_READMODE_BYTE |   
          PIPE_WAIT,               
          1, 
		  4096,			 
          4096,		            
          NMPWAIT_USE_DEFAULT_WAIT,  
          NULL);                    
	
	if (hPipe == INVALID_HANDLE_VALUE) 
    {
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
		system("pause");
        return;
    }
	
	if((CreateProcess(NULL, ChildName, NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcInfo))==0)
	{
		printf("create client process: error code %d\n", (int)GetLastError());
		system("pause");
		return;
	}
	
	if((ConnectNamedPipe(hPipe, NULL))==0)
	{
		printf("client could not connect\n");
		system("pause");
		return;
	}
	WriteFile(hPipe, &numb, sizeof(int), &NumberOfBytesWritten, NULL);
	WaitForSingleObject(ProcInfo.hProcess, INFINITE);
	ReadFile(hPipe, &numb, sizeof(numb), &NumberOfBytesRead, NULL);
	printf ("This is new numb - %d\n", numb);
	CloseHandle(hPipe);
	CloseHandle( ProcInfo.hProcess);
	CloseHandle( ProcInfo.hThread);
	system("pause");
	return ;
}
#elif __linux__
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void Create_linux()
{
	printf ("In OS Linux\n");
	pid_t pid;
	int mypipe[2];
	int numb=20;
	pipe(mypipe);
	pid=fork();
	if(pid==0)
	{
		char readdscr[4], writedscr[4];
		sprintf (readdscr,"%d",mypipe[0]);
		sprintf (writedscr, "%d", mypipe[1]);
		execl("child", readdscr, writedscr, NULL); 		
	}
	else if(pid< 0)
		{
			printf ("ERROR\n");
			return ;
		}
	else
		{
			printf ("Parent process\n");
			printf ("input number\n");
			scanf("%d", &numb);
			write(mypipe[1], &numb, sizeof(int));
			close(mypipe[1]);
			wait(NULL);
			read(mypipe[0], &numb, sizeof(int));
			printf ("New numb are - %d\n", numb);
			close(mypipe[0]);	
			return ;
		}
}
#endif
