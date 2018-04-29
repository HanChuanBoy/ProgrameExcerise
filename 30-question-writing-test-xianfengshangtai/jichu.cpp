#include <stdio.h>

int main()
{
  int a;
  int*b;
  b=&a;
  b=(int*)a;   //指针指向了a表示的地址;
  *b=a;
}