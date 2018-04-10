#include "syscall.h"
int main()
{
	char b = 'B';
	char a;
	a = Readchar();
	b = Readchar();
	Printchar(a);
	Printchar(b);
	Halt();
}
