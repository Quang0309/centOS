#include"syscall.h"
int main()
{
	int i;
	for(i = 0;i<128;i++)
	{
		Printint(i);
		Printchar('-');
		Printchar((char) i);
		Printchar('\n');
	}
}
