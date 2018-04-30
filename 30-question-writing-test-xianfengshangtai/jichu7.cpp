#include<stdio.h>
#include<string.h>
#define MEMORY_SIZE 102400
typedef void* PTR;
typedef unsigned char BYTE;
typedef PTR (*NodeFunc)(void);
typedef void (*DoFunc)(int n,PTR p);
typedef struct Memory* PCMemory;
typedef struct Memory
{
	BYTE amem[MEMORY_SIZE];
	int  nMemPoint;
}CMemory;
void MInit(PCMemory pmem); // 内存初始化
PTR MAlloc(PCMemory pmem, int nSize); // 内存分配函数
typedef struct Stack* PCStack;
typedef struct Stack
{
	PTR pContent;
	PCStack pNext;
}CStack;

PCStack SNew(); //堆栈初始化
PCStack SPush(PCStack pstk, PTR p);//堆栈进站
PCStack SPop(PCStack pstk,PTR* pp); //堆栈出栈
typedef struct Tree* PCTree;
typedef struct Tree
{
	PTR pContent;
	PCTree pSon;
	PCTree pBrother;
}CTree;
PCTree TNew();
void TAddSon(PCTree ptree,NodeFunc f);
void TAddBrother(PCTree ptree,NodeFunc f);
void TBrowse(PCTree ptree, DoFunc f, int n);
void TBrowseStack(PCTree ptree, DoFunc f, int n);
static CMemory mem;
PCMemory pmem=&mem;

void MInit(PCMemory pmem)
{
	pmem->nMemPoint = 0;
} 

PTR MAlloc(PCMemory pmem,int nSize)
{
	int nCurMemPoint;
	nCurMemPoint=pmem->nMemPoint;
	pmem->nMemPoint += nSize;
	return (void*)(&(pmem->amem[nCurMemPoint]));
}
 
PCStack SPush(PCStack pstk,PTR p)
{
	PCStack pstkNew;
	pstkNew=(PCStack)MAlloc(pmem,sizeof(CStack));
	pstkNew->pContent=p;
	pstkNew->pNext=pstk;
	return pstkNew;
} 
PCStack Spop(PCStack ptsk, PTR* pp)
{
	if( ptsk != NULL ) 
	{
		*pp = ptsk->pContent;
		ptsk = ptsk->pNext;
		return ptsk;
	}
	return NULL;
} 
PCStack SNew() 
{ 
	return NULL; 
}


PCTree TNew()
{
	PCTree pRoot;
	pRoot =(PCTree)MAlloc(pmem,sizeof(CTree));
	pRoot->pBrother=NULL;
	pRoot->pSon=NULL;
	pRoot->pContent=(PTR)"ROOT";
	return pRoot;
}

void TAddSon(PCTree ptree, NodeFunc f)
{
	PTR p=f();
	PCTree ptreeNew;
	if( p==NULL )
	TAddBrother(ptree,f);
	else
	{
		ptreeNew=(PCTree)MAlloc(pmem,sizeof(CTree));
		ptreeNew->pBrother=NULL;
		ptreeNew->pSon=NULL;
		ptreeNew->pContent=p;
		ptree->pSon= ptreeNew;
		TAddSon(ptreeNew,f);
		TAddBrother(ptree,f);
	}
}

void TAddBrother(PCTree ptree,NodeFunc f)
{
	PTR p = f();
	PCTree ptreeNew;
	if(p==NULL)
		return;
	else
	{
		ptreeNew=(PCTree)MAlloc(pmem,sizeof(CTree));
		ptreeNew->pBrother=NULL;
		ptreeNew->pSon=NULL;
		ptreeNew->pContent=p;
		ptree->pBrother=ptreeNew;
		TAddSon(ptreeNew,f);
	}
}

void TBrowse(PCTree ptree,DoFunc f,int n)
{
	if( ptree != NULL )
	{
		f(n,ptree->pContent);
		TBrowse( ptree->pSon,f,n+1);
		TBrowse( ptree->pBrother,f,n);
	}
}// TBrowse
void TBrowseStack(PCTree ptree, DoFunc f, int n)
{
	PCStack pstk=SNew();
	while(1)
	{
		f(n,ptree->pContent);
		{
			printf("yes \n");
		}
		if(ptree->pSon==NULL)
		{
			while(ptree->pBrother==NULL)
			{
				pstk=Spop(pstk,(PTR*)&ptree);
				n--;
				if(pstk==NULL)
					return;
			}
			ptree=ptree->pBrother;
		}
		else
		{
			pstk=SPush(pstk,ptree);
			n++;
			ptree=ptree->pSon;
		}
	}
} 


static char* aaNode[22]=
{
	"1","2","3",NULL,"4",NULL,"5",NULL,NULL,"6",NULL,"7","8",NULL,"9","10",NULL,"11",NULL,"12",NULL,NULL
}; // P19
static int nNode=0;
PTR Node(){ return aaNode[nNode++]; }
void Out(int n, PTR p)
{
	int i;
	for(i=0;i<n;i++)
	printf("_");
	printf("%s\n",p);
}// Out

int  main()
{
	PCTree ptree;
	MInit(pmem);
	ptree=TNew();
	TAddSon(ptree,Node);
	TBrowseStack(ptree->pSon,Out,0); //P20*/
	TBrowse(ptree->pSon,Out,0);
	return 1;
}
