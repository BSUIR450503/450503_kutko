#pragma once

#include <iostream>
#include <string>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
CRITICAL_SECTION sectionThreads;
#define SLEEP_TIME 1
#define SLEEP_TIME_FOR_PRINT 50
#endif

#ifdef __linux__
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <linux/sched.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
struct termios orig_termios;
pthread_mutex_t * sectionThreads = new pthread_mutex_t();
#define Sleep usleep
#define SLEEP_TIME 1000
#define SLEEP_TIME_FOR_PRINT 40000
#endif

#define MAX_NUBM 10

string stringOfThread[] = { "1. The first thread\n", "2. The second thread\n",
"3. The third thread\n", "4. The fourth thread\n", "5. The fifth thread\n", "6. The sixth thread\n",
"7. The seventh thread\n", "8. The eighth thread\n", "9. The ninth thread\n", "10. The tenth thread\n"};