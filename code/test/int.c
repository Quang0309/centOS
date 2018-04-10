#include "syscall.h"
int main()
{
	int a = Readint();
	int b = Readint();
	Printint(a);
	Printint(b);
	Halt();
}
