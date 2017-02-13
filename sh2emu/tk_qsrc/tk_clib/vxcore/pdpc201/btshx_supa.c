// #include <stdio.h>
// #include <stddef.h>

// #include <vxcore.h>

TK_FILE *btshx_tk_handles[256];
int btshx_tk_nhandles=3;

void tk_setGpioOutputs(int val)
{
	*(int *)GPIO_BASE=val;
}

void tk_putc(int val)
{
	while(P_UART_STAT&8);
//	*(int *)(GPIO_BASE+0x104)=val;
	P_UART_TX=val;
}

void sleep_0();

int tk_kbhit(void)
	{ return(P_UART_STAT&1); }

int tk_getch(void)
{
	while(!(P_UART_STAT&1))
		sleep_0();
	return(P_UART_RX);
}

int tk_puts(char *msg)
{
	char *s;
	
	s=msg;
	while(*s)
		{ tk_putc(*s++); }
}

void tk_puts_n(char *msg, int n)
{
	char *s;
	
	s=msg;
	while(n--)
		{ tk_putc(*s++); }
}

void tk_gets(char *buf)
{
	char *t;
	int i;
	
	t=buf;
	while(1)
	{
		i=tk_getch();
		if((i=='\b') || (i==127))
		{
			if(t>buf)
			{
				tk_puts("\b \b");
				t--;
			}
			*t=0;
			continue;
		}
		if(i=='\r')
			{ tk_putc('\n'); break; }
		if(i=='\n')
			{ tk_putc('\n'); break; }
		tk_putc(i);
		*t++=i;
	}
	*t=0;
}

void tk_gets_n(char *msg, int n)
{
	char *s;
	
	s=msg;
	while(n--)
		{ *s++=tk_getch(); }
}

int __read(int handle, void *buf, size_t len, int *errind)
{
	int i;

	if(handle<3)
	{
//		tk_gets_n(buf, len);
		return(0);
	}

	if(errind)*errind=0;
	i=tk_fread(buf, 1, len, btshx_tk_handles[handle]);
	if(i<0)
		*errind=1;
//	read(handle, buf, len);
	return(i);
}

int __write(int handle, const void *buf, size_t len, int *errind)
{
	void *ptr1;
	int i;

	if(handle<3)
	{
		ptr1=(void *)buf;
		tk_puts_n(ptr1, len);
		return(0);
	}

	if(errind)*errind=0;
	ptr1=(void *)buf;
	i=tk_fwrite(ptr1, 1, len, btshx_tk_handles[handle]);
	if(i<0)
		*errind=1;
//	write(handle, buf, len);
	return(i);
}

void __seek(int handle, long offset, int whence)
{
	tk_fseek(btshx_tk_handles[handle], offset, whence);
//	lseek(handle, offset, whence);
}

long __tell(int handle)
{
	return(tk_ftell(btshx_tk_handles[handle]));
}

int __open(const char *a, int b, int c)
{
	TK_FILE *fd;
	char *s;
	int i;

	s=(char *)a;
	fd=tk_fopen(s, "rb");
	if(!fd)
	{
		*(int *)c=-1;
		return(-1);
	}

	*(int *)c=0;
	
	for(i=3; i<btshx_tk_nhandles; i++)
	{
		if(!btshx_tk_handles[i])
		{
			btshx_tk_handles[i]=fd;
			return(i);
		}
	}
	
	if(btshx_tk_nhandles>=256)
	{
		*(int *)c=-1;
		return(-1);
	}
	
	i=btshx_tk_nhandles++;
	btshx_tk_handles[i]=fd;
	return(i);
	
//	return(open(a, b, c));
}

void __close(int handle)
{
	TK_FILE *fd;

	if(handle<3)
		return;

	fd=btshx_tk_handles[handle];
	btshx_tk_handles[handle]=NULL;
	tk_fclose(fd);
//	close(handle);
}

void __remove(const char *filename)
{
//	vx_remove(filename);
}

void __rename(const char *oldfn, const char *newfn)
{
//	vx_rename(old, new);
}

void __exita(int status)
{
//	vx_exit(status);
}

u32 TK_GetTimeMs(void)
{
	u32 *sreg;
	int ms;

	sreg=(int *)0x007F8000;
//	ms=(P_AIC_RTC_NSEC>>20)|(P_AIC_RTC_SEC_LO*1000);
	ms=sreg[4];
	return(ms);
}

void __datetime(void *ptr)
{
	struct dt_s {
		int year;
		int month;
		int day;
		int hours;
		int minutes;
		int seconds;
		int hundredths;
	} dt, *pdt;

	int ms;
	
	ms=TK_GetTimeMs();
	
	pdt=ptr;
	pdt->hundredths=(ms/10)%100;
	pdt->seconds=(ms/1000)%60;
	pdt->minutes=(ms/60000)%60;
	pdt->hours=0;
	pdt->day=0;
	pdt->month=0;
	pdt->year=0;

//	vx_datetime(ptr);
}

__PDPCLIB_API__ void *_alloca(int sz)
{
//	return((void *)vx_malloc(sz));
	return(TKMM_Malloc(sz));
}

void __allocmem(size_t size, void **ptr)
{
//	*ptr=(void *)vx_malloc(size);
	*ptr=TKMM_Malloc(size);
}

void __freemem(void *ptr)
{
//	vx_free(ptr);
	TKMM_Free(ptr);
}

unsigned char *__envptr;

void __exec(char *cmd, void *env)
{
//	vx_system(cmd);
}

int __start_early()
{
	TKMM_Init();
}

int __start_late()
{
}

char *__get_cmdline()
{
	return("");
}

char **__get_cmdenv()
{
	return(NULL);
}

//int __longj(void *ptr)
//	{ }

// int __setj(jmp_buf env)
// { }
