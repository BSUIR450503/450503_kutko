#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include<stdlib.h>
#define INDEX 11

#ifdef _WIN32
#include <windows.h>
#include<conio.h>
#endif

#ifdef linux
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <curses.h>
#include <sys/types.h>
#include<vector>
#include <sys/wait.h>


#define SLEEP_TIME 50000
#define SLEEP_TIME_ONE 1000
#define Sleep usleep
#define PROCESS_INFORMATION pid_t
#endif

#ifdef linux 
	int flag_to_print = 0;
	int flag_to_end = 1;

	struct sigaction Signal_to_print;
	struct sigaction Signal_to_end;

	void set_print(int sig)
	{
		flag_to_print = 1;
	}

	void set_end(int sig)
	{
		flag_to_end = 1;
	}
#endif

using namespace std;

