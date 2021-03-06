/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

	/* size of physical memory; with code, we'll run out of space!*/

int
main()
{
    	int n;
	int a[100];
	int i=0;
	int j;
	int temp;
	Printstring("Input n: ");
	n = Readint();
	for(i=0;i<n;i++)
	{
		Printint(i);
		Printchar(':');
		a[i] = Readint();
	}
	for(i=0;i<n;i++)
	{
		
		for (j=i+1;j<n;j++)
		{
			if(compare(a[i],a[j])==-1)
				{
					temp = a[i];
					a[i] = a[j];
					a[j] = temp;
				}
		}
	}
	for(i=0;i<n;i++)
	{
		Printint(a[i]);
		Printchar(' ');
	}
}
