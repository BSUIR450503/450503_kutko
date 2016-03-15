#include "test.h"
#ifdef _WIN32
#define create() Create_win()
#elif __linux__
#define create() Create_linux()
#endif
	
int main()
{
	create();
	return 0;
}