#include "librariesh.h"

class Lab2
{
private:
	string StringOfProcess[INDEX];
public:
	Lab2()
	{
		StringOfProcess[0] = "\n1.First process\n";
		StringOfProcess[1] = "2.Second process\n";
		StringOfProcess[2] = "3.Third process\n";
		StringOfProcess[3] = "4.Fourth process\n";
		StringOfProcess[4] = "5.Fifth process\n";
		StringOfProcess[5] = "6.Sixth process\n";
		StringOfProcess[6] = "7.Seventh process\n";
		StringOfProcess[7] = "8.Eighth process\n";
		StringOfProcess[8] = "9.Ninth process\n";
		StringOfProcess[9] = "10.Tenth process\n";
	}

	~Lab2()
	{ }

#ifdef _WIN32
	int catch_input()
	{
		if (_kbhit())
			return _getch();
		else
			return -1;
	}
	void printString(int processNumber)
	{
		char EventID[30];
	 	sprintf_s(EventID, " %d", processNumber);
		HANDLE closeEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EventID);

		sprintf_s(EventID, " %dp", processNumber);
		HANDLE CanPrintEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EventID);

		while (1)
		{
			if (WaitForSingleObject(CanPrintEvent, 1) == WAIT_OBJECT_0)
			{
			//	cout << endl;
				for (int i = 0; i<StringOfProcess[processNumber - 1].length(); i++)      
				{
					if (WaitForSingleObject(closeEvent, 0) == WAIT_OBJECT_0)  	
					{
						CloseHandle(closeEvent);
						CloseHandle(CanPrintEvent);
						return;
					}
					cout << StringOfProcess[processNumber - 1][i];
					Sleep(50);	
				}
				ResetEvent(CanPrintEvent);
			}
			if (WaitForSingleObject(closeEvent, 0) == WAIT_OBJECT_0)
			{
				CloseHandle(closeEvent); CloseHandle(CanPrintEvent);
				return;
			}
		}
		return;
	}

void InitialProcess(char *path)
{
	char choice = 0;  
	int currentNum = 0; 
	bool flag = false;  
	PROCESS_INFORMATION procInf[INDEX];   
	vector<HANDLE> closeEvents;
	vector<HANDLE> CanPrintEvents;
	char EventID[30]; 
	while (1)
		{
			choice = catch_input();
			Sleep(50); 
			switch (choice)
			{
				case '+':
				{
					if (closeEvents.size() < INDEX)
					{
						sprintf(EventID, " %d", closeEvents.size() + 1); // Number to String 
						closeEvents.push_back(CreateEvent(NULL, FALSE, FALSE, EventID)); 

						sprintf_s(EventID, " %dp", CanPrintEvents.size() + 1); // 
						CanPrintEvents.push_back(CreateEvent(NULL, TRUE, FALSE, EventID)); 

						procInf[closeEvents.size() - 1] = CreateNewProcess(path, EventID);
					}
					break;
				}
				case '-':
				{

					if (closeEvents.size() > 0)
					{
						SetEvent(closeEvents.back());
						WaitForSingleObject(procInf[closeEvents.size() - 1].hProcess, INFINITE); 

						if (!CloseHandle(closeEvents.back()))
							printf("Close Handle failed (%d)\n", GetLastError());
						if (!CloseHandle(CanPrintEvents.back()))
							printf("Close Handle failed (%d)\n", GetLastError());

						closeEvents.pop_back();
						CanPrintEvents.pop_back();
						if (currentNum >= closeEvents.size())
						{
							currentNum = 0;
							flag = true;
						}
					}
					break;
				}
			case 'q':
			{
			if (closeEvents.size() > 0)
			{
				while (closeEvents.size() > 0)
				{
					SetEvent(closeEvents.back());
					WaitForSingleObject(procInf[closeEvents.size() - 1].hProcess, INFINITE); // Ожидаем завершения последнего процесса

					if (!CloseHandle(closeEvents.back()) || !CloseHandle(CanPrintEvents.back()) ||
						!CloseHandle(procInf[closeEvents.size() - 1].hProcess)
						|| !CloseHandle(procInf[closeEvents.size() - 1].hThread))
						printf("Close Handle failed (%d)\n", GetLastError());

						closeEvents.pop_back();
						CanPrintEvents.pop_back();
						}
						currentNum = 0;
					}
				printf("\n\n");
				return;
	}
}
			
if (CanPrintEvents.size() > 0 && WaitForSingleObject(CanPrintEvents[currentNum], 0) == WAIT_TIMEOUT) 
			{
				if (currentNum >= CanPrintEvents.size() - 1)
					currentNum = 0;
				else if (!flag) currentNum++;
				flag = false;

				SetEvent(CanPrintEvents[currentNum]);
			}
		}
}

PROCESS_INFORMATION CreateNewProcess(char* path, char *commandline)
{
		STARTUPINFO StartupInfo;
		PROCESS_INFORMATION infProc;

		TCHAR CommandLine[] = TEXT("pr");
		ZeroMemory(&StartupInfo, sizeof(StartupInfo));
		StartupInfo.cb = sizeof(StartupInfo);
		ZeroMemory(&infProc, sizeof(infProc));

		if (!CreateProcess(path, commandline, NULL, NULL, FALSE, NULL, NULL, NULL, &StartupInfo, &infProc))
		{
			cout << "Create Process failed!";
		}
		return infProc;
}
#endif
////////////////////////////////////////////////////////////////////

#ifdef linux
void InitialProcess(char *path)
{
	char symbol;
	int processNumber = 0;
	bool flag = false;

	int currentNum = 0;
	PROCESS_INFORMATION procInf[INDEX];

	Signal_to_print.sa_handler = set_print;
	sigaction(SIGUSR1,&Signal_to_print,NULL);
  
	Signal_to_end.sa_handler = set_end;
	sigaction(SIGUSR2,&Signal_to_end,NULL);
	
	while (1) 
	{ 
	symbol = getchar();

	Sleep(SLEEP_TIME_ONE);
	switch (symbol) 
	{
		case '+': 
		{
			if (processNumber < INDEX - 1) 
			{
				procInf[processNumber] = fork();
				processNumber++;
				switch (procInf[processNumber - 1])
				{
					case 0: 
					{
						flag_to_end = 0;
						while (!flag_to_end) {
							Sleep(SLEEP_TIME_ONE);
							if (flag_to_print) {
								printString(processNumber);
								flag_to_print = 0;
								kill(getppid(),SIGUSR2);
							}
						}
						return;
						break;
					}
					case -1: 
					{
						cout << "CreateProcess failed!" << endl;
						break;
					}
					default: 
					{
						break;
					}
				}

			}
			break;
		}
		case '-': 
		{
			if (processNumber > 0) 
			{
				kill(procInf[processNumber- 1],SIGUSR2);
				processNumber--;
			}
				break;
		}
		case 'q': 
		{
			if (processNumber> 0) 
			{
				for (; processNumber >= 0; processNumber--) 
				{
					kill(procInf[processNumber - 1], SIGUSR2);
					waitpid(procInf[processNumber - 1],NULL,0);
				}
			}
				return;
			}
		}
		if (flag_to_end && processNumber > 0) 
		{
			flag_to_end = 0;
			if (currentNum >= processNumber - 1) 
			{
				currentNum = 0;
			}
			else 
			{
				if (!flag) 
				{
					currentNum++;
				}
				}
			flag = 0;
			kill(procInf[currentNum],SIGUSR1);
		}
	}
}

void printString(int processNumber) 
{ 
	for (int i = 0; i < value[processNumber-1].length(); i++) 
	{
		cout << value[processNumber-1][i];
		fflush(stdout);
		Sleep(SLEEP_TIME);
	}
	return;
}
#endif
};


