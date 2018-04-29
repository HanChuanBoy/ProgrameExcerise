#include <stdio.h>


int main()
{
	int b=1;
	int*a=&b;
	//int*d=&a;
	int c[2]={1,2};
	int (*ptr)[2]=&c;
	int *ptr1[]={a,c};
	int *(ptr2[])={a,c};
	int ptr3[]={b,b};
}
/*c99
jichu4.cpp(8) : error C2440: 'initializing' : cannot convert from 'int **' to 'int *'

*/