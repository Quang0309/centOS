#include "syscall.h"
int main()
{
	OpenFileId a = Open("NhatQuang", 1);
	if (a!=-1)
	{
		int b = CloseFile(a);
		int c = CloseFile(3);
		Printint(a);
		Printint(b);
		Printint(c);
}
	Halt();
}
