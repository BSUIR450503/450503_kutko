#include "libraries.h"
#include "lab4.h"

int catch_input()
{
	if (_kbhit())
		return _getch();
	else
		return -1;
}

int main() 
{
	int numberThread = 0;
	int currentThread = 1;
	char key;
	Lab4 *arrayThreads[MAX_NUBM];
	while (1)
	{		
			key = catch_input();
			switch (key) 
			{
				case '+': 
					{
						 if (numberThread < MAX_NUBM) 
						 {
							  numberThread++;
							  Lab4 *tempThread = new Lab4(numberThread);
							  arrayThreads[numberThread - 1] = tempThread;
						 };
						 break;
					}
				case '-': 
					{
						 if (numberThread > 0)
						 {
							 arrayThreads[numberThread - 1]->endPrint();
							 numberThread--;
						 };
						 break;
					}
				case 'q':
					{
						 for (int i = 0; i < numberThread; i++)
						 {
						  	 arrayThreads[i]->endPrint();
						 }
						 exit(0);
					 }

				default: 
					{
						 if (arrayThreads[currentThread]->IsPrint() && numberThread > 0)
						 {
							 if (currentThread > numberThread) 
							 {
								 currentThread = 1;
							 }
							 arrayThreads[currentThread - 1]->startPrint();
							 currentThread++;
						 }
						 break;
					}
		
			}
			Sleep(SLEEP_TIME);
	}
	return 0;
}
