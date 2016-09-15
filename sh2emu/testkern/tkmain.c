#include "testkern.h"

#include "tk_mmpage.c"

u32 timer_ticks;

void setGpioOutputs(int val)
{
	*(int *)GPIO_BASE=val;
}

void putc(int val)
{
//	*(int *)(GPIO_BASE+0x104)=val;
	P_UART_TX=val;
}

int kbhit(void)
	{ return(P_UART_STAT&1); }

int getch(void)
{
	while(!(P_UART_STAT&1));
	return(P_UART_RX);
}

void puts(char *msg)
{
	char *s;
	
	s=msg;
	while(*s)
		{ putc(*s++); }
}

void gets(char *buf)
{
	char *t;
	int i;
	
	t=buf;
	while(1)
	{
		i=getch();
		if(i=='\b')
		{
			if(t>buf)
			{
				puts("\b \b");
				t--;
			}
			*t=0;
			continue;
		}
		if(i=='\r')
			{ putc('\n'); break; }
		if(i=='\n')
			{ putc('\n'); break; }
		putc(i);
		*t++=i;
	}
	*t=0;
}

int strcmp(char *s1, char *s2)
{
	while(*s1 && *s2 && (*s1==*s2))
		{ s1++; s2++; }
	if(*s1>*s2)return(1);
//	if(*s2>*s1)return(-1);
	if(*s1<*s2)return(-1);
	return(0);
}

void print_hex(u32 v)
{
	static char *chrs="0123456789ABCDEF";
	putc(chrs[(v>>28)&15]);
	putc(chrs[(v>>24)&15]);
	putc(chrs[(v>>20)&15]);
	putc(chrs[(v>>16)&15]);
	putc(chrs[(v>>12)&15]);
	putc(chrs[(v>> 8)&15]);
	putc(chrs[(v>> 4)&15]);
	putc(chrs[(v    )&15]);
}

void print_decimal(int val)
{
	char tb[256];
	char *t;
	int i, k, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb;
	if(!k)*t++='0';	
	while(k>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
	}
	if(s)*t++='-';
	
	while(t>tb)
		{ t--; putc(*t); }
}

void print_decimal_n(int val, int num)
{
	char tb[256];
	char *t;
	int i, k, n, s;
	
	k=val; s=0;
	if(k<0)
		{ k=-k; s=1; }
	
	t=tb; n=num;
//	if(!k)*t++=0;	
	while(n>0)
	{
		i=k%10;
		*t++='0'+i;
		k=k/10;
		n--;
	}

//	if(s)*t++='-';
	
	while(t>tb)
		{ t--; putc(*t); }
}

#ifdef ARCH_HAS_FPU
void print_float(double val)
{
	int ip, fp, sg;
	
	
//	print_hex(((u32 *)(&val))[0]);
//	print_hex(((u32 *)(&val))[1]);
//	putc(' ');
	
	sg=0;
	if(val<0)
		{ val=-val; sg=1; }
	
//	print_hex(((u32 *)(&val))[0]);
//	print_hex(((u32 *)(&val))[1]);
//	putc(' ');

	ip=(int)val;
	fp=(int)((val-ip)*1000000);

	if(sg)putc('-');
	print_decimal(ip);
	putc('.');
	print_decimal_n(fp, 6);
}
#endif

void printf(char *str, ...)
{
//	void **plst;
	va_list lst;
	char *s, *s1;
	int v;

//	plst=(void **)(&str);
//	plst++;
	va_start(lst, str);
	
	s=str;
	while(*s)
	{
		if(*s!='%')
			{ putc(*s++); continue; }
		if(s[1]=='%')
			{ s+=2; putc('%'); continue; }
		s++;
		switch(*s++)
		{
		case 'd':
//			v=(int)(*plst++);
			v=va_arg(lst, int);
			print_decimal(v);
			break;
		case 'X':
//			v=(int)(*plst++);
			v=va_arg(lst, int);
			print_hex(v);
			break;
		case 's':
//			s1=*plst++;
			s1=va_arg(lst, char *);
			puts(s1);
			break;

		case 'p':
			s1=va_arg(lst, char *);
			print_hex((u32)s1);
			break;

#ifdef ARCH_HAS_FPU
		case 'f':
//			v=(int)(*plst++);
//			v=va_arg(lst, int);
			print_float(va_arg(lst, double));
			break;
#endif

		default:
//			plst++;
			break;
		}
	}
	va_end(lst);
}

void *malloc(int sz)
{
	return(TKMM_Malloc(sz));
}

int free(void *ptr)
{
	return(TKMM_Free(ptr));
}

#ifdef ARCH_HAS_FPU
void float_test0()
{
	float f0, f1, f2, f3;
	u32 v;
	
	f0=1.0;
	f1=3.14159;
	f2=f0+f1;
	f3=f2*f1;

	putc('.');
	print_decimal_n(314159, 6);
	putc('\n');

	v=*(u32 *)(&f3);
	printf("float test=%X\n", v);
	printf("float test 1=%f\n", f1);
	printf("float test 2=%f\n", f2);
	printf("float test 3=%f\n", f3);

//	print_hex();
}
#endif

void tk_main()
{
	char tb[256];
	void *p0, *p1;

	timer_ticks=0;

	setGpioOutputs(0x69);
	puts("tk_main: puts\n");
	tkmm_init();
	
	while(1)
	{
		puts("$ ");

		gets(tb);
		
		puts("got: ");
		puts(tb);
		puts("\n");
		
		if(!strcmp(tb, "quit"))
		{
//			puts("ok, but no\n");
//			puts("ok, dying\n");
//			*(int *)-1=-1;
//			continue;
			puts("ok, exiting main loop\n");
			break;
		}

		if(!strcmp(tb, "time"))
		{
//			puts("ok, but no\n");
//			print_hex(timer_ticks);
//			print_decimal(timer_ticks);
//			puts("\n");

			printf("timer ticks=%d\n", timer_ticks);

			continue;
//			break;
		}

		if(!strcmp(tb, "die"))
//		if(strcmp(tb, "die")==0)
		{
			puts("ok, dying\n");
			*(int *)-1=-1;
		}

#ifdef ARCH_HAS_FPU
		if(!strcmp(tb, "float"))
		{
			float_test0();
			continue;
		}
#endif

		if(!strcmp(tb, "malloc"))
		{
			p0=TKMM_Malloc(256);
			p1=TKMM_Malloc(256);
			TKMM_Free(p0);
			TKMM_Free(p1);
//			p0=TKMM_PageAlloc(256);
//			p1=TKMM_PageAlloc(256);
			printf("got pointer A %p %p\n", p0, p1);

			p0=TKMM_Malloc(256);
			p1=TKMM_Malloc(256);
			TKMM_Free(p0);
			TKMM_Free(p1);
			printf("got pointer B %p %p\n", p0, p1);
			continue;
		}
		
		printf("don't understand %s\n", tb);
	}
}
