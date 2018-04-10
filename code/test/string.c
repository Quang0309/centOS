#include "syscall.h"
int main()
{
	char c[1000];
	Readstring(c,1000);
	Printstring(c);
	
	Halt();
}
