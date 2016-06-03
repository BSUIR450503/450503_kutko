#include "libs.h"

#ifdef _WIN32

Lab5 data;
HINSTANCE library;
HANDLE events[3];


DWORD WINAPI readerFuncThread(PVOID folderPATH)
{
	string folder(((const char*)folderPATH));               
	folder.append("\\");
	string fileName = folder + "FileOfRead?.txt";   
	char ReadFilePATH[MAX_PATH];
	BOOL(*Read)(Lab5*) = (BOOL(*)(Lab5*))GetProcAddress(library, "read");
	WIN32_FIND_DATA FindFileData; 
	HANDLE findHandle, hReadFile;    
	BOOL flagReadResult = false;
	findHandle = FindFirstFile(fileName.c_str(), &FindFileData);
	if (findHandle == INVALID_HANDLE_VALUE)
	{
		cout<<" Error: "<<GetLastError()<<endl;
		return 0;
	}
	while (1)
	{
		WaitForSingleObject(events[WRITE], INFINITE);
		strcpy(ReadFilePATH, folder.c_str());
		strcat(ReadFilePATH, FindFileData.cFileName);  
		hReadFile = CreateFile(ReadFilePATH, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		data.hFile = hReadFile;
		flagReadResult = (Read)(&data);
		if (flagReadResult)
			SetEvent(events[READ]);
		else
			cout<<"Error while read from "<<ReadFilePATH<<endl;
		if (FindNextFile(findHandle, &FindFileData))
		{
			CloseHandle(hReadFile);
			continue;
		}
		else break;
	}
	FindClose(findHandle);
	CloseHandle(hReadFile);
	SetEvent(events[EXIT]);
	return 0;
}

DWORD WINAPI writerFuncThread(PVOID outFilePath)
{
	string fileName(((const char*)outFilePath));               
	fileName.append("\\FileOfWrite.txt");
	HANDLE hOutputFile = CreateFile(fileName.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
	HANDLE hWait[2] = { events[READ], events[EXIT] };
	BOOL(*Write)(Lab5*) = (BOOL(*)(Lab5*))GetProcAddress(library, "write");
	while (1)
	{
		int event = WaitForMultipleObjects(2, hWait, FALSE, INFINITE);
		if (event == EXIT)
			break;
		data.hFile = hOutputFile;
		(Write)(&data);
		SetEvent(events[WRITE]);
	}
	CloseHandle(hOutputFile);
	return 0;
}

 int main()
 {
	 HANDLE hEvent;        
	 HANDLE hThreads[2];   
	 char pathToFile[SIZE_PATH];
	 hEvent = CreateEvent(NULL, FALSE, TRUE, TEXT("event"));  
	 events[WRITE] = CreateEvent(NULL, FALSE, TRUE, NULL);   
	 events[READ] = CreateEvent(NULL, FALSE, FALSE, NULL);  
	 events[EXIT] = CreateEvent(NULL, TRUE, FALSE, NULL); 
	 data.Overlapped.Offset = 0;
	 data.Overlapped.OffsetHigh = 0;
	 data.Overlapped.hEvent = hEvent;
	 data.posReadfile = 0;
	 data.NumberOfBytes = sizeof(data.tempString);
	 library = LoadLibrary("lib.dll");
	 cout<<"Enter path to dir: "<<endl;
	 gets(pathToFile);	 
	 hThreads[0] = CreateThread(NULL, 0, readerFuncThread, (LPVOID)pathToFile, 0, NULL); 
	 hThreads[1] = CreateThread(NULL, 0, writerFuncThread, (LPVOID)pathToFile, 0, NULL); 
	 WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
	 CloseHandle(hThreads[0]);
	 CloseHandle(hThreads[1]);
	 CloseHandle(events[WRITE]);
	 CloseHandle(events[READ]);
	 CloseHandle(events[EXIT]);
	 CloseHandle(hEvent);
	 FreeLibrary(library);
	 system("pause");
	 return 0;
 }
 #endif

 #ifdef __linux__
 struct Lab5 {
  char readFileName[500];
  char writeFileName[500];
  struct aiocb readCb;
  struct aiocb writeCb;
  char buffer[300];
  int bytesRead;
  int bytesWrite;
};

char fileNames[20][100];
struct Lab5 data;
pthread_t readThread;
pthread_t writeThread;
int numberOfFiles = 0;
pthread_mutex_t mutex;

void (*asyncronicWrite)(struct Lab5 *data);
void (*asyncronicRead)(struct Lab5 *data);

void findAllFiles() {
	struct dirent *dp;  
    DIR *dirp;     
    dirp = opendir("/home/thandor/Test"); 
    puts("Input files in directory: ");
    while ((dp = readdir(dirp)) != NULL) {  
        if(strstr(dp->d_name, "in") != NULL) {  
            strcpy(fileNames[numberOfFiles], dp->d_name);
            numberOfFiles++;
            puts(dp->d_name);
        }
    }  
}

void *readFunc(void * arg){
	void *ext_library;
	ext_library = dlopen("/home/thandor/lib.so",RTLD_LAZY);
	asyncronicRead = (void(*)(struct Lab5 *data)) dlsym(ext_library, "asyncronicRead");
	for(int i = 0; i < numberOfFiles; i++) {
		pthread_mutex_lock(&mutex);
		strcpy(data.readFileName, fileNames[i]);
		(*asyncronicRead)(&data);
		pthread_mutex_unlock(&mutex);
		puts("Wait for write...");
		usleep(100000);
	}
	dlclose(ext_library); 
	puts("\nDone!");
	return NULL;
}

void *writeFunc(void * arg) {
	usleep(10000);
	strcpy(data.writeFileName, "/home/thandor/Test/out.txt");
	void *ext_library;
	ext_library = dlopen("/home/thandor/lib.so",RTLD_LAZY);
	asyncronicWrite = (void(*)(struct Lab5 *data)) dlsym(ext_library, "asyncronicWrite");
	for(int i = 0; i < numberOfFiles; i++) {
		pthread_mutex_lock(&mutex);
		puts("Writing...");
		(*asyncronicWrite)(&data);
		pthread_mutex_unlock(&mutex);
		puts("Wait for read...");
		usleep(100000);
	}
	dlclose(ext_library);
	return NULL;
}

int main() {
  if(pthread_mutex_init(&mutex, NULL))
	{
	 	printf("Can't create mutex");
	 	return 0;
	}
  remove("/home/thandor/Test/out.txt");
  
  findAllFiles();

  pthread_create(&readThread, NULL, readFunc, NULL);
  pthread_create(&writeThread, NULL, writeFunc, NULL);

  pthread_join(readThread, NULL);
  pthread_join(writeThread, NULL);

  return 0;
}
  #endif