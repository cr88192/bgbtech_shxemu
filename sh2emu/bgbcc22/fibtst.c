//#ifndef __BSCC
#if 0

#include <stdio.h>
#include <time.h>

#endif

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC	60
#endif

#define	EXPR(a, b)	((a)*(a))+((b)*(b))

int fib(int x)
{
	if(x>2)return(fib(x-1)+fib(x-2));
	return(1);
}

#if 0
long long lfib(long long x)
{
	if(x>2)return(lfib(x-1)+lfib(x-2));
	return(1);
}

double dfib(double x)
{
	if(x>2)return(dfib(x-1)+dfib(x-2));
	return(1);
}

int ltst(int x)
{
	int i, j;

	j=0;
	for(i=0; i<x; i++)j++;

	return(j);
}

typedef struct foo_s {
int x, y;
}foo;

int stst(foo *st)
{
	return(st->x+st->y);
}

#endif

//int arrtst[]={1, 2, 3, 4};

int hash_str(char *s)
{
	int i;

	i=0;
	while(*s)i=i*251+(*s++);
	return(i&0xFFF);
}

int bscc_main()
{
	long long li;
	int i, t, dt;

	printf("test\n");

	t=clock();
	i=fib(36);
	dt=clock()-t;

	printf("fibtst %d %d\n", i, dt);

	printf("hashtst %d\n", hash_str("foobarbaz"));

	li=0;
//	li=i;
	if(!li)printf("LI-Z\n");
//	t=i;
//	li=0;

	return(0);
}

int main()
{
	int i, t, dt;

	t=clock();

//	EXPR(3, 4);

	i=fib(36);
//	i=ltst(100000000);

	dt=clock()-t;
	printf("%d %fms\n", i, (dt*1000.0)/CLOCKS_PER_SEC);

	printf("hashtst %d\n", hash_str("foobarbaz"));
}
