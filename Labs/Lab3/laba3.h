#include "Libraries.h"
class Laba3 
{
#ifdef _WIN32
	PROCESS_INFORMATION procInformation;
	STARTUPINFO startUpInfo; 
	DWORD  dwRead; 
	DWORD  dwWritten;
	HANDLE hSemaphore;
	HANDLE hSemaphoreClient;
	HANDLE hNamedPipe;
#endif
#ifdef linux
	int pid; 
	int semaphoreId; 
	struct sembuf buf1, buf; 
	char *msg, *segptr;
	int shmid; 

	key_t key;
#endif

	
public:
	Laba3();
	void myCreateProcessec(char *);
	void exitProcess(void);

	bool receiveStringFromChannel(char *, int);
	bool sendStringToChannel(char *, int);

	void prepareToTransferData(void);

	void serverProcess(void);
	void clientProcess(void);
};