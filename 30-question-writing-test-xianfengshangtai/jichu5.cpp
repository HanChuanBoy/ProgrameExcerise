#include <stdio.h>

char tt[256];
int main()
{
	int b=2;
	int c=1;
	int a;
	//a=b++;
	//a=++b;
	
	//a=++(c+1);
	//a=(b+1)++;
	int *e=&b;
	char*d=tt+1;
	printf("%d \n",a);
	printf("%d \n",sizeof(tt));
	printf("%d \n",sizeof(e));
}
/*c99
jichu5.cpp(13) : error C2105: '++' needs l-value
sizeof 编译就知道数组名表示的是数组对象;包含数组的长度

*/