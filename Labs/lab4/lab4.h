#include "libraries.h"

#ifdef __linux__
	void * threadFunction(void *);
#elif _WIN32
	DWORD WINAPI threadFunction(void *);
#endif

class Lab4 
{
public:

	int numberThread;
	Lab4(int numberThread);
	bool IsPrint();
	bool printEnd();
	bool close();
	void startPrint();
	void print_End();
	void endPrint();

#ifdef __linux__
	pthread_t *thread = new pthread_t();
#endif

#ifdef _WIN32
	HANDLE threadHandle;
	bool flagPrint;
	bool flagClose;
	DWORD threadID;
#endif

};




#ifdef __linux__
char _getch() 
{
	char buf = 0;
	struct termios old = { 0 };
	fflush(stdout);
	if (tcgetattr(0, &old)<0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old)<0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1)<0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old)<0)
		perror("tcsetattr ~ICANON");

	return buf;
}

int _kbhit(void) {
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);

	select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif

Lab4::Lab4(int numberThread)
{
	this->numberThread = numberThread;
	 flagPrint = false;
	 flagClose = false;
#ifdef _WIN32
	
	if (numberThread == 1)
	{
		InitializeCriticalSection(&sectionThreads);
	}
	threadHandle = CreateThread(NULL, 100, threadFunction, (void*)this, 0, &threadID);
}
#elif __linux__
	if (numberThread == 1)
	{
		pthread_mutex_init(sectionThreads, NULL);
	}
	pthread_create(thread, NULL, threadFunction, (void*)this);
}
#endif

bool Lab4::IsPrint()
{
#ifdef __linux__
	if (pthread_mutex_trylock(sectionThreads) != 0) return false;
	pthread_mutex_unlock(sectionThreads);
	return true;
#elif _WIN32
	if (TryEnterCriticalSection(&sectionThreads) == 0) return false;
	LeaveCriticalSection(&sectionThreads);
	return true;
#endif
}

void Lab4::print_End() 
{
#ifdef __linux__
	flagPrint = false;
	pthread_mutex_unlock(sectionThreads);
#elif _WIN32
	flagPrint = false;
	LeaveCriticalSection(&sectionThreads);
#endif
}

bool Lab4::close()
{
	if(flagClose == true) return true;
	flagClose = false;
	return false;
}

bool Lab4::printEnd()
{
	if (flagPrint == true) return false;
	flagPrint = false;
	return true;
}

void Lab4::startPrint()
{
	flagPrint = true;
}

void Lab4::endPrint() 
{
	flagClose = true;
}

#ifdef __linux__
void *threadFunction(void * name)
{
#elif _WIN32
DWORD WINAPI threadFunction(void * name)
{
#endif
	Lab4 *thread = (Lab4*)name;
	while (true) 
	{
		if (!(thread->printEnd()))
		{
#ifdef _WIN32
			EnterCriticalSection(&sectionThreads);
#endif
			for (int i = 0; stringOfThread[(thread->numberThread) - 1][i] != 0; i++)
			{
				cout<<stringOfThread[(thread->numberThread) - 1][i];
				fflush(stdout);
				Sleep(SLEEP_TIME_FOR_PRINT);
			}
			thread->print_End();
		}
		else 
		{
			Sleep(SLEEP_TIME);
		}
		if (thread->close())
		{
			break;
		}
	}
	return 0;
}

