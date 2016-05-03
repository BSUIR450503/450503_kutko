#include "laba3.h"

Laba3::Laba3()
{
#ifdef _WIN32
	hNamedPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\mypipe"), PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, SIZE_MESSAGE, SIZE_MESSAGE, NMPWAIT_USE_DEFAULT_WAIT, NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout<<"Error in create pipe\n";
		system("pause");
		exit(1);
	}
	hSemaphore = CreateSemaphore(NULL, 0, 1, TEXT("Semaphore"));
	hSemaphoreClient = CreateSemaphore(NULL, 0, 1, TEXT("ClientSemaphore"));
#endif
#ifdef linux
	key = ftok("/home/LabSPOVM", 0);

	 if((shmid = shmget(key, SIZE_MESSAGE, IPC_CREAT|IPC_EXCL|0660)) == -1)
	   {
	      if((shmid = shmget(key, SIZE_MESSAGE, 0)) == -1)
	      {
			 perror("shmget");
			 exit(1);
	      }
	   }

	   if((segptr = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
	      printf("Can't attach shared memory\n");
	      exit(1);
	   }


	semaphoreId = semget(key, 1, 0666 | IPC_CREAT);

	if (semaphoreId < 0)
	{
		perror("semget");
		exit(EXIT_FAILURE);
	}

	if(semctl(semaphoreId, 0, SETVAL, (int)0) < 0)
	{
		perror("semctl");
		exit(EXIT_FAILURE);
	}

	buf1.sem_num = 0;
	buf1.sem_op = 1;
	buf1.sem_flg = 0;
	
	semop(semaphoreId, &(buf1), 1);
#endif

}

void Laba3::myCreateProcessec(char *path)
{
#ifdef _WIN32
	char *name = (char*)malloc(sizeof(char) * 100);
	strcpy(name, path);
	strcat(name, " server");
	ZeroMemory(&(startUpInfo), sizeof(STARTUPINFO));
	startUpInfo.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, name, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &(startUpInfo), &(procInformation)))
	{
		cout<<"Error in create process\n";
		exit(0);
	}
	strcpy(name, path);
	strcat(name, " client");
	Sleep(1000);
	if (!CreateProcess(NULL, name, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &(startUpInfo), &(procInformation)))
	{
		cout<<"Error in create process\n";
		exit(0);
	}
#endif
#ifdef linux
	switch(pid = fork())
	{
		case -1:
			printf("error %d\n", pid);
			break;
		case 0:
			execlp("xterm", "xterm", "-e", path, "client", NULL);
			break;
		default:
			execlp("xterm", "xterm", "-e", path, "server", NULL); 
	}
	
	return ;
#endif
}



void Laba3::serverProcess(void)
{
#ifdef _WIN32
	cout<<"Server are wait to transfer data with client"<<endl;
	if (!ConnectNamedPipe(hNamedPipe, NULL))
	{
		CloseHandle(hNamedPipe);
		exit(0);
	}
	WaitForSingleObject(hSemaphore, INFINITE);
	cout<<"Server are ready now"<<endl;
	char *buffer = NULL;
	buffer = (char *)malloc(sizeof(char) * SIZE_MESSAGE);
	while (1)
	{
		if(!sendStringToChannel("Ready for transfer data...", 28))
			break;
		ReleaseSemaphore(hSemaphoreClient, 1, NULL);
		WaitForSingleObject(hSemaphore, INFINITE);
		cout<<"\nMessage to client"<<endl;
		fflush(stdin);
		gets(buffer);
		if(!sendStringToChannel(buffer, strlen(buffer) + 1))
			break;
		ReleaseSemaphore(hSemaphoreClient, 1, NULL);
		if (!strcmp(buffer, "exit"))
		{
			exitProcess();	
			return;
		}
		WaitForSingleObject(hSemaphore, INFINITE);
		if(receiveStringFromChannel(*&buffer, SIZE_MESSAGE))
			cout<<"Message from client: "<<buffer<<endl;
		if (!strcmp(buffer, "exit"))
		{
			exitProcess();	
			return;
		}
	}
	return ;
#endif
#ifdef linux
	buf.sem_num = 0;
	buf.sem_op = 0;
	buf.sem_flg = 0;
	printf("Server ");
	while(1) {
		char *message = NULL;
		message = (char *)malloc(SIZE_MESSAGE * sizeof(char));

		if(semop(semaphoreId, &(buf), 1) < 0) 
			perror("semop");

		cout<<"\nInpur for client:";
		fflush(stdin);	
		gets(message);

		sendStringToChannel(message, strlen(message) + 1);

		buf.sem_op = 3;
		semop(semaphoreId, &(buf), 1);

		buf.sem_op = 0;	
		semop(semaphoreId, &(buf), 1);

		if(!strcmp("exit", message))
			return ;

		receiveStringToChannel(*&message, SIZE_MESSAGE);
		cout<<"Output from client: "<<message<<endl;

		buf.sem_op = 3;
		semop(semaphoreId, &(buf), 1);

		buf.sem_op = 0;	
	}
	return ;
#endif
}

void Laba3::clientProcess()
{
#ifdef _WIN32
	cout<<"\nClient "<<endl;
	ReleaseSemaphore(hSemaphore, 1, NULL);
	char *buffer = NULL;
	buffer = (char *)malloc(sizeof(char) * SIZE_MESSAGE);
	while (1)
	{
		WaitForSingleObject(hSemaphoreClient, INFINITE);
		if(receiveStringFromChannel(*&buffer, SIZE_MESSAGE))
		{
			cout<<"Server: "<<buffer<<endl;
			ReleaseSemaphore(hSemaphore, 1, NULL);
		}	
		WaitForSingleObject(hSemaphoreClient, INFINITE);
		if(receiveStringFromChannel(*&buffer, SIZE_MESSAGE))
		{
			if (!strcmp(buffer, "exit"))
			{
				CloseHandle(hSemaphoreClient);
				return ;
			}
			char input[SIZE_MESSAGE];	
			cout<<"Message from server: "<<buffer<<endl;
			cout << "Message to server: ";
			fflush(stdin);
			cin.getline(input, SIZE_MESSAGE); 
			if(!sendStringToChannel(input, strlen(input) + 1))
				break;
			if (!strcmp(input, "exit"))
			{
				ReleaseSemaphore(hSemaphore, 1, NULL);
				CloseHandle(hSemaphoreClient);
				return ;
			}
		}
		ReleaseSemaphore(hSemaphore, 1, NULL);
	}
	return ;
#endif
#ifdef linux
	while(1) {
		char *message = NULL;
		message = (char *)malloc(SIZE_MESSAGE * sizeof(char));

		semop(semaphoreId, &(buf1), 1);

		receiveStringToChannel(*&message, SIZE_MESSAGE);

		if(!strcmp("exit", message))
		{
			buf1.sem_op = -1;
			semop(semaphoreId, &(buf1), 1);

			return ;
		}

		cout<<"Output from server: "<<message<<endl;

		cout<<"\nInput for server: ";
	
		fflush(stdin);	
		gets(message);

		sendStringToChannel(message, strlen(message) + 1);

		buf1.sem_op = -1;
		semop(semaphoreId, &(buf1), 1);

		buf1.sem_op = -2;	
		semop(semaphoreId, &(buf1), 1);

		receiveStringToChannel(*&message, SIZE_MESSAGE);
	
		printf("Server: %s\n", message);

		buf1.sem_op = -1;
		semop(semaphoreId, &(buf1), 1);

		buf1.sem_op = -2;		
	}

	return ;
#endif
}

void Laba3::prepareToTransferData(void)
{
#ifdef _WIN32
	hSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, TEXT("Semaphore"));
	hSemaphoreClient = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, TEXT("ClientSemaphore"));	
	hNamedPipe = CreateFile(TEXT("\\\\.\\pipe\\mypipe"), GENERIC_READ | GENERIC_WRITE, 
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cout<<"Error in open file"<<endl;
		exit(0);
	}	
	return ;
#endif
#ifdef linux
	buf1.sem_op = -1;
	semop(semaphoreId, &(buf1), 1);
	buf1.sem_op = -2;

	if((shmid = shmget(key, SIZE_MESSAGE, 0)) == -1)
        {
	     perror("shmget");
	     exit(1);
        }
	
	return ;
#endif
}

bool Laba3::receiveStringFromChannel(char *message, int size)
{
#ifdef _WIN32
	if (ReadFile(hNamedPipe, message, SIZE_MESSAGE, &(dwRead), NULL))
		return true;
	return false;
#endif
#ifdef linux
	if(strcpy(message, segptr))
		return true;
	else
		return false;
#endif
}

bool Laba3::sendStringToChannel(char *message, int size)
{
#ifdef _WIN32
	if (!WriteFile(hNamedPipe, message, size, &(dwWritten), NULL))
		return false;
	return true;
#endif
#ifdef linux
	if(strcpy(segptr, message))
		return true;
	else
		return false;
#endif
}

void Laba3::exitProcess()
{
#ifdef _WIN32
	CloseHandle(hNamedPipe);
	CloseHandle(hSemaphore);
	system("pause");
#endif
#ifdef linux
	semctl(semaphoreId, 0 ,IPC_RMID, 1);
#endif
}
