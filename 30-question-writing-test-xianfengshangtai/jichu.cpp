#include <stdio.h>

int main()
{
  int c[2]={100,200};
  int a=(int)c;
  int*b;
  printf("%d \n",a);
  b=&a;
  b=(int*)a;   //指针指向了a表示的地址;
  *b=a;
}
/*c99
涉及到整数转换为指针
一般有两种情况会用到：
   句柄是一种指向指针的指针,但是句柄里面的值是不会变化的;
   线程函数参数;
runing error:

*/