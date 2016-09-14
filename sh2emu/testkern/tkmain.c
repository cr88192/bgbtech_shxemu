#define GPIO_BASE 0xABCD0000

#define UART_BASE 0xABCD0100

#define UART_RX		(UART_BASE+0x00)
#define UART_TX		(UART_BASE+0x04)
#define UART_STAT	(UART_BASE+0x08)
#define UART_CTRL	(UART_BASE+0x0C)

#define P_UART_RX	(*(u32 *)UART_RX)
#define P_UART_TX	(*(u32 *)UART_TX)
#define P_UART_STAT	(*(u32 *)UART_STAT)
#define P_UART_CTRL	(*(u32 *)UART_CTRL)

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef __builtin_va_list va_list;
#endif

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)

#endif

typedef unsigned int u32;

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
	if(!k)*t++=0;	
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
		default:
//			plst++;
			break;
		}
	}
	va_end(lst);
}

void tk_main()
{
	char tb[256];

	timer_ticks=0;

	setGpioOutputs(0x69);
	puts("tk_main: puts\n");
	while(1)
	{
		puts("$ ");

		gets(tb);
		
		puts("got: ");
		puts(tb);
		puts("\n");
		
		if(!strcmp(tb, "quit"))
		{
			puts("ok, but no\n");
			continue;
//			break;
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
		
		printf("don't understand %s\n", tb);
	}
}
