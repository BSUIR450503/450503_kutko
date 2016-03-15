#ifdef __WIN32__
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
void main()
{
	printf ("This if Child Process\n");
	HANDLE hPipe;
	LPTSTR PipeName = TEXT("\\\\.\\pipe\\MyPipe");
	DWORD NumBytesToRead;
	DWORD NumBytesToWrite;
	NumBytesToRead=sizeof(int);
	NumBytesToWrite=sizeof(int);
	int numb;
	hPipe = CreateFile( 
         PipeName,	      // имя канала
         GENERIC_READ |  // чтение и запись в канал
         GENERIC_WRITE, 
         0,              // нет разделяемых операций 
         NULL,           // защита по умолчанию
         OPEN_EXISTING,  // открытие существующего канала 
         0,              // атрибуты по умолчанию
         NULL);          // нет дополнительных атрибутов 

	ReadFile(hPipe, &numb, sizeof(numb), &NumBytesToRead, NULL);
	printf ("This is numb - %d\n", numb);
	numb=numb*numb*numb;
	WriteFile(hPipe, &numb, sizeof(int), &NumBytesToWrite, NULL);
	CloseHandle(hPipe);
	system("pause");
}
#elif __linux__
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char * argv[])
{	
	printf ("Child Process\n");
	int mypipe[2];
	int numb;
	mypipe[0]=atoi(argv[0]);
	mypipe[1]=atoi(argv[1]);
	read(mypipe[0], &numb, sizeof(int));
	close(mypipe[0]);
	numb=numb*numb*numb;
	write(mypipe[1], &numb, sizeof(int));
	close(mypipe[1]);
	usleep(2000000);
	return 0;
}
#endif
