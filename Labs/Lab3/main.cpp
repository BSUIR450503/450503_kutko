#include "libraries.h"
#include "Laba3.h"

int main (int argc, char * argv[])
{
	Laba3 object; 
	switch(argc) 
	{ 
		case 1: 
			object.myCreateProcessec(argv[0]); 
			break; 
		default: 
			if(!strcmp("server", argv[1])) 
				object.serverProcess(); 
			else if (!strcmp("client", argv[1])) 
			{
				object.prepareToTransferData();
				object.clientProcess();
			}
		break; 
	}
	
	object.exitProcess();
		
	return 0;
}
