//#include"stdafx.h"
#include <iostream>
#include<time.h> 
//#include<Windows.h>
using namespace std;
#define MAX 100//自定义物品最大数 
int w[MAX],v[MAX],best[MAX];
int V[MAX][MAX]; //最大价值矩阵;
int W,n;  //W为背包最大载重量,n为物品的数量;
//求最大值函数;
int max1(int x,int y) 
{
   return x>= y?x:y;
}
int min1(int x,int y)
{
  return x>= y ? y:x;
}

void Knaspack()
{
   int Max=min(w[n]-1,W);
   for(int j=1;j <= Max; j++)
		V[n][j]=0;
	for( int  j=w[n];j<=W;j++)
		V[n][j]=v[n];
	for(int i=n-1;i > 1;i--)
	{
		Max=min(w[i]-1,W);
		for(int j=1; j <= Max;j++)
			V[i][j]=V[i+1][j];
		for(int j=w[i]; j <= W; j++)
			V[i][j]=max(V[i+1][j],V[i+1][j-w[i]]+v[i]);
	}
    V[1][W]=V[2][W]; //先假设第一个物品不放入
	if(W >w[1])     
		V[1][W]=max(V[1][W],V[2][W-w[1]]+v[1]);
}

void Traceback()
{
	for(int i=1;i<n;i++) {
		if(V[i][W]== V[i+1][W]) {   //如果当前行的最优值与下一行的最优值相等，则表明该物品不能放入。
			best[i]=0;
		} else {
			best[i]=1;
			W-=w[i];
		}
	}
	best[n]=(V[n][W] )?1:0;
}

void main()
{
	//LARGE_INTEGER begin,end,frequency; 
	//QueryPerformanceFrequency(&frequency); 
	//srand(time(0));
	cout<<"输入商品数量n和背包容量W:";
	cin>>n>>W;
	for(int i=1;i<=n;i++)
	{
		w[i]=rand()%1000;
		v[i]=rand()%1000;
	}
	cout<<"商品的重量和价值如下："<<endl;
	for(int i=1;i<=n;i++)
	{
		cout<<w[i]<<"\t";
		cout<<v[i]<<endl;
	}
	//QueryPerformanceCounter(&begin); 
	Knaspack();//构造矩阵
	Traceback(); //求出解的向量数组
	//QueryPerformanceCounter(&end); 
	int totalW=0;
	int totalV=0;
	//显示可以放入的物品
	cout<<"所选择的商品如下:"<<endl;
	cout<<"序号i:\t重量w:\t价格v:\t"<<endl;
	for(int i=1; i <= n ; i++)
	{
		if(best[i]== 1)
		{
			totalW+=w[i];
			totalV+=v[i];
			cout<<i<<" "<<w[i]<<"\t"<<v[i]<<endl;
		}
	}
	cout<<"放入背包的物品总重量为:"<<totalW<<endl<<"放入背包的物品总价值为:"<<""<<totalV<<endl;
//	cout<<"时间:" <<(double)(end.QuadPart- begin.QuadPart) / frequency.QuadPart<<"s"<<endl;

}
