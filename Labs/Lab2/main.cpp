#include "librariesh.h"
#include "Process.cpp"

int main(int argc, char* argv[])
{
	Lab2 object;
	if (argc == 2)
	{ 
#ifdef _WIN32
		object.printString(atoi(argv[1])); 
#endif
	} 
	else 
		object.InitialProcess(argv[0]);						
}

