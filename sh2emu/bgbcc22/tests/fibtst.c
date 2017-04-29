//#ifndef __BSCC
#if 0

#include <stdio.h>
#include <time.h>

#endif

#ifndef CLOCKS_PER_SEC
// #define CLOCKS_PER_SEC	60
#define CLOCKS_PER_SEC	1000
#endif

void printf(char *str, ...);
int clock(void);
int puts(char *str);

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

#if 0
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
#endif

void print_hex_n(unsigned int v, int n);
void print_decimal(int val);

#if 1
int main()
{
	int i, j, k, t, dt;

	t=clock();

//	EXPR(3, 4);

	puts("main: A\n");

	print_hex_n(0x12345678, 8);
	puts("\n");

	k=0x12345678;
	for(i=0; i<10; i++)
	{
		j=k/10;
		print_hex_n(j, 8);
		puts("(");
		print_hex_n(k%10, 2);
		puts(") ");
		k=j;
	}
	puts("\n");

//	print_hex_n(0x12345678/10, 8);
//	puts("\n");

//	print_hex_n(0x12345678%10, 8);
//	puts("\n");

	print_decimal(0x12345678);
	puts("\n");


	i=fib(36);
//	i=ltst(100000000);

	puts("main: B\n");

	print_decimal(i);
	puts("\n");

	dt=clock()-t;
//	printf("%d %fms\n", i, (dt*1000.0)/CLOCKS_PER_SEC);
	printf("V=%d Dt=%dms\n", i, dt);
//	printf("V=%X Dt=%Xms\n", i, dt);

	puts("main: C\n");

	printf("hashtst %d\n", hash_str("foobarbaz"));
	puts("main: D\n");
}
#endif