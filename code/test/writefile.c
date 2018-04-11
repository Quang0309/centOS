#include "syscall.h"
int main()
{
	int a = Open("NhatQuang",1);
	int b = WriteFile("Nguyen Trinh Nhat Quang",23,a);
	Printint(b);
	Halt();
}
