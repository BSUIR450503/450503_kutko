#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#define SIZE_MESSAGE 512

using namespace std;

#ifdef _WIN32
	#include <Windows.h>
	#include <conio.h>
#endif

#ifdef linux
	#include <sys/sem.h>
	#include <sys/shm.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <errno.h>
	#include <unistd.h>
	#include <string.h>
#endif
