#include "syscall.h"
int main()
{
	int a = CreateFile("NhatQuang");
	if(a==0)
		Printstring("Create successfully");
	else
			Printstring("Create failed");

	Halt();
}
