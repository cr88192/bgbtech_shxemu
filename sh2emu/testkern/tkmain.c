#include "testkern.h"

#include "tk_clib.c"

#include "tk_mmpage.c"
#include "tk_spi.c"
#include "tk_fat.c"
#include "tk_vfile.c"
#include "tk_ramavi.c"

#include "tk_jxgpu.c"

u32 timer_ticks;

TKFAT_ImageInfo *tkfat_fsimg;

void setGpioOutputs(int val)
{
	*(int *)GPIO_BASE=val;
}

void putc(int val)
{
	while(P_UART_STAT&8);
//	*(int *)(GPIO_BASE+0x104)=val;
	P_UART_TX=val;
}

void sleep_0();

int kbhit(void)
	{ return(P_UART_STAT&1); }

int getch(void)
{
	while(!(P_UART_STAT&1))
		sleep_0();
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
		if((i=='\b') || (i==127))
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

void print_hex_n(u32 v, int n)
{
	static char *chrs="0123456789ABCDEF";
	if(n>7)putc(chrs[(v>>28)&15]);
	if(n>6)putc(chrs[(v>>24)&15]);
	if(n>5)putc(chrs[(v>>20)&15]);
	if(n>4)putc(chrs[(v>>16)&15]);
	if(n>3)putc(chrs[(v>>12)&15]);
	if(n>2)putc(chrs[(v>> 8)&15]);
	if(n>1)putc(chrs[(v>> 4)&15]);
	if(n>0)putc(chrs[(v    )&15]);
}

int print_hex_genw(u32 v)
{
	u32 w;
	int i;

	i=1;
	while(v>=16)
//	while(v>>4)
		{ v=v>>4; i++; }

#if 0
	w=v; i=1;
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
	if(w>=16) { w=w>>4; i++; }
#endif

	return(i);
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
	char pcfill;
	char *s, *s1;
	int v, w;

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
		
		if(*s=='0')
		{
			pcfill='0';
			s++;
		}else
		{
			pcfill=' ';
		}
		
		w=0;
		if((*s>='0') && (*s<='9'))
		{
			while((*s>='0') && (*s<='9'))
				w=(w*10)+((*s++)-'0');
		}
		
		switch(*s++)
		{
		case 'd':
//			v=(int)(*plst++);
			v=va_arg(lst, int);
			if(w)
			{
				print_decimal_n(v, w);
			}else
			{
				print_decimal(v);
			}
			break;
		case 'X':
//			if(!w)w=8;
//			v=(int)(*plst++);
			v=va_arg(lst, int);

			if(!w)w=print_hex_genw(v);
//			print_hex(v);
			print_hex_n(v, w);
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

//void *malloc(int sz)
//{
//	return(TKMM_Malloc(sz));
//}

//int free(void *ptr)
//{
//	return(TKMM_Free(ptr));
//}

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

void sleep(int ms)
{
	int t0, te;

	t0=timer_ticks;
	te=t0+ms;
	while(timer_ticks<te)
	{
		sleep_0();
	}
}

void test_framebuf0()
{
	u32 *vreg;
	u32 *vram;
	u32 px;
	int i, j, k;

	vreg=(u32 *)0xA05F8000;
	vreg[(0x44/4)]=0x000D;
//	vreg[(0x5C/4)]=320|(240<<10)|(320<<20);
	vreg[(0x5C/4)]=319|(239<<10)|(321<<20);

	sleep(1);
	
	vram=(u32 *)0xA5000000;
	for(i=0; i<240; i++)
	{
		for(j=0; j<320; j++)
		{
			px=(j<<24)|(i)|((255-(i+j))<<8);
			vram[i*320+j]=px;
		}
	}
	
	sleep(1000);
	vreg[0x11]=0x0000;
}

u32 riff_getfcc(byte *ptr)
{
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

u32 TK_GetCurrentFixMIPS(void)
{
	u32 *sreg;
	int ms;

	sreg=(int *)0x007F8000;
	ms=sreg[7];
	return(ms);
}

static int tk_sndrov;

int TK_EmitAudioSamplesS44K(short *samp, int len)
{
	short *sbufl, *sbufr;
	int i, j, k;
	
	sbufl=(short *)0x007F0000;
	sbufr=(short *)0x007F4000;
	
	for(i=0; i<len; i++)
	{
		sbufl[tk_sndrov]=samp[i*2+0];
		sbufr[tk_sndrov]=samp[i*2+1];
		tk_sndrov=(tk_sndrov+1)&8191;
	}
	return(0);
}

int TK_EmitAudioSamplesM11K(short *samp, int len)
{
	short *sbufl, *sbufr;
	int *sreg;
	int dma, rov2, idma;
	int i, j, k;
	
	sbufl=(short *)0x007F0000;
	sbufr=(short *)0x007F4000;
	
	sreg=(int *)0x007F8000;
	idma=sreg[0];
	dma=(idma+500)&8191;
	k=tk_sndrov-dma;
	k=(k<<(31-13))>>(31-13);
	k=k^(k>>31);
		
	if(k>1100)
		tk_sndrov=dma;
	
//	sreg[0]=
//	sreg[1]=11025;
//	sreg[2]=255;
//	sreg[3]=255;
	
	
	for(i=0; i<len; i++)
	{
//		if(tk_sndrov==idma)
//			break;

		j=samp[i];

		sbufl[tk_sndrov]=j;	sbufr[tk_sndrov]=j;
		tk_sndrov=(tk_sndrov+1)&8191;
//		sbufl[tk_sndrov]=j;	sbufr[tk_sndrov]=j;
//		tk_sndrov=(tk_sndrov+1)&8191;
//		sbufl[tk_sndrov]=j;	sbufr[tk_sndrov]=j;
//		tk_sndrov=(tk_sndrov+1)&8191;
//		sbufl[tk_sndrov]=j;	sbufr[tk_sndrov]=j;
//		tk_sndrov=(tk_sndrov+1)&8191;

		if(idma && (tk_sndrov==idma))
			break;
	}
	
	rov2=tk_sndrov;
	for(i=0; i<len; i++)
	{
		if(rov2==idma)
			break;
		j=samp[i];
		sbufl[rov2]=j;	sbufr[rov2]=j;
		rov2=(rov2+1)&8191;
	}

	return(0);
}

int TK_EmitAudioClear()
{
	short *sbufl, *sbufr;
	int *sreg;
	int dma, rov2, idma;
	int i, j, k;
	
	sbufl=(short *)0x007F0000;
	sbufr=(short *)0x007F4000;
	memset(sbufl, 0, 16384);
	memset(sbufr, 0, 16384);
}

void test_video0()
{
	short sndbuf[8192];
	BGBBTJ_AVI_Context *avi;
	BGBBTJ_Video_Stats *vstat;
	u32 *buf;
	int snlen, mips;
	int t0, t1, t2, te;

	u32 *vreg;
	u32 *vram;
	u32 *sreg;
	u32 px;
	int i, j, k;

//	byte *ird;
//	byte *avi_base;
	
//	ird=P_INITRD_ABSADDR;
//	avi_base=ird+0x200;

//	avi=BGBBTJ_AVI_LoadAVI("TestSonicUnleashed_WindmillIsleDA2_CRAM1.avi");
	avi=BGBBTJ_AVI_LoadAVI("TestOut_CRAM0.avi");
	if(!avi)
	{
		printf("AVI Load Failed\n");
		return;
	}
	
	vstat=BGBBTJ_AVI_GetStats(avi);

	vreg=(u32 *)0xA05F8000;
	vreg[(0x44/4)]=0x000D;
//	vreg[(0x5C/4)]=320|(240<<10)|(320<<20);
	vreg[(0x5C/4)]=319|(239<<10)|(321<<20);

	sleep(1);
	
	vram=(u32 *)0xA5000000;

	sreg=(u32 *)0x007F8000;
	sreg[1]=11025;
	sreg[2]=255;
	sreg[3]=255;

	if((vstat->width==320) && (vstat->height<=240))
	{
		free(avi->fdbuf);
		avi->fdbuf=vram;
	}

//	t0=timer_ticks;
	t0=TK_GetTimeMs();
//	te=t0+60000;
	te=t0+((vstat->num_frames*vstat->frametime)>>10);
	t1=t0;
//	while(t1<te)
	while((avi->frnum+1)<vstat->num_frames)
	{
//		printf("%d %d/%d\r", t1, avi->frnum, vstat->num_frames);

//		t1=timer_ticks;
		t1=TK_GetTimeMs();
		t2=t1-t0;
		buf=BGBBTJ_AVI_FrameRawClrs(avi, t2*1000, BGBBTJ_JPG_BGRA);
		snlen=BGBBTJ_AVI_GetMonoSamplesRate(avi, sndbuf, 8192, 11025);
		t0=t1;
		
//		memset(sndbuf, 0, 8192*2);
		
		TK_EmitAudioSamplesM11K(sndbuf, snlen);
		
		if((vstat->width==320) && (vstat->height<=240))
		{
//			memcpy(vram, buf, vstat->width*vstat->height*4);
		}else if((vstat->width<320) && (vstat->height<=240))
		{
			for(i=0; i<vstat->height; i++)
			{
				memcpy(vram+i*320,
					buf+i*vstat->width,
					vstat->width*4);
			}
		}

		mips=TK_GetCurrentFixMIPS();
//		printf("MIPS=%f   \r", mips/1024.0);
		printf("MIPS=%d.%03d   \r", mips>>10, mips&1023);
	}

	TK_EmitAudioClear();
	vreg[0x11]=0x0000;
	sreg[2]=0;
	sreg[3]=0;
	
	mips=TK_GetCurrentFixMIPS();
//	printf("MIPS=%f\n", mips*(1.0/1024.0));
	printf("MIPS=%d.%03d   \n", mips>>10, mips&1023);
}

void test_video1()
{
	short sndbuf[8192];
	BGBBTJ_AVI_Context *avi;
	BGBBTJ_Video_Stats *vstat;
	u32 *buf;
	int snlen, mips;
	int t0, t1, t2, te;

	u32 *vreg;
	u32 *vram;
	u32 *sreg;
	u32 px;
	int i, j, k;

//	avi=BGBBTJ_AVI_LoadAVI("TestSonicUnleashed_WindmillIsleDA2_CRAM1.avi");
	avi=BGBBTJ_AVI_LoadAVI("TestOut_CRAM0.avi");
	if(!avi)
	{
		printf("AVI Load Failed\n");
		return;
	}
	
	vstat=BGBBTJ_AVI_GetStats(avi);

	sreg=(u32 *)0x007F8000;
	sreg[1]=11025;
	sreg[2]=255;
	sreg[3]=255;

//	t0=timer_ticks;
	t0=TK_GetTimeMs();
	te=t0+((vstat->num_frames*vstat->frametime)>>10);
	t1=t0;
//	while(t1<te)
	while((avi->frnum+1)<vstat->num_frames)
	{
//		printf("%d %d/%d\r", t1, avi->frnum, vstat->num_frames);
//		t1=timer_ticks;
		t1=TK_GetTimeMs();
		t2=t1-t0;
		buf=BGBBTJ_AVI_FrameRawClrs(avi, t2*1000, BGBBTJ_JPG_RAWCON);
		snlen=BGBBTJ_AVI_GetMonoSamplesRate(avi, sndbuf, 8192, 11025);
		t0=t1;

		TK_EmitAudioSamplesM11K(sndbuf, snlen);
	}

	TK_EmitAudioClear();
	sreg[2]=0;
	sreg[3]=0;

	mips=TK_GetCurrentFixMIPS();
//	printf("MIPS=%f\n", mips/1024.0);
	printf("MIPS=%d.%03d   \n", mips>>10, mips&1023);
}

void test_video2()
{
	short sndbuf[8192];
	BGBBTJ_AVI_Context *avi;
	BGBBTJ_Video_Stats *vstat;
	u32 *buf;
	int snlen, mips;
	int t0, t1, t2, te;

	u32 *vreg;
	u32 *vram;
	u32 *sreg;
	u32 px;
	int i, j, k;

	avi=BGBBTJ_AVI_LoadAVI("TestOut_CRAM0.avi");
	if(!avi)
	{
		printf("AVI Load Failed\n");
		return;
	}
	
	vstat=BGBBTJ_AVI_GetStats(avi);

	vreg=(u32 *)0xA05F8000;
	vreg[(0x44/4)]=0x000D;
	vreg[(0x5C/4)]=319|(239<<10)|(321<<20);

	sleep(1);
	
	vram=(u32 *)0xA5000000;

	if((vstat->width==320) && (vstat->height<=240))
	{
		free(avi->fdbuf);
		avi->fdbuf=vram;
	}

	t0=TK_GetTimeMs();
//	te=t0+((vstat->num_frames*vstat->frametime)>>10);
	te=t0+120000;
	t1=t0;
//	while((avi->frnum+1)<vstat->num_frames)
	while(t1<te)
	{
		t1=TK_GetTimeMs();
		t2=t1-t0;
//		buf=BGBBTJ_AVI_FrameRawClrs(avi, t2*1000, BGBBTJ_JPG_BGRA);
		BGBBTJ_AVI_DecodeFrame2(avi, 0, BGBBTJ_JPG_BGRA);
		snlen=BGBBTJ_AVI_GetMonoSamplesRate(avi, sndbuf, 8192, 11025);
		t0=t1;
		
//		TK_EmitAudioSamplesM11K(sndbuf, snlen);

		mips=TK_GetCurrentFixMIPS();
		printf("MIPS=%d.%03d   \r", mips>>10, mips&1023);
	}

	TK_EmitAudioClear();
	vreg[0x11]=0x0000;
//	sreg[2]=0;
//	sreg[3]=0;
	
	mips=TK_GetCurrentFixMIPS();
//	printf("MIPS=%f\n", mips*(1.0/1024.0));
	printf("MIPS=%d.%03d   \n", mips>>10, mips&1023);
}

void help0()
{
	printf("quit\texit main loop\n");
	printf("time\tprint time\n");
	printf("rtc\tprint RTC time\n");
	printf("die\tdeliberate crash\n");
	printf("float\ttest floating point\n");
	printf("malloc\ttest memory allocation\n");
	printf("readmbr\ttest reading SD image\n");
}

int tk_exec(char *cmd)
{
	char tb[1024];
	void *p0, *p1;

	if(!strcmp(cmd, "help") || !strcmp(cmd, "?"))
	{
		help0();
		return(0);
	}
	
	if(!strcmp(cmd, "quit"))
	{
//			puts("ok, but no\n");
//			puts("ok, dying\n");
//			*(int *)-1=-1;
//			continue;
		puts("ok, exiting main loop\n");
		return(-99);
	}

	if(!strcmp(cmd, "time"))
	{
//			puts("ok, but no\n");
//			print_hex(timer_ticks);
//			print_decimal(timer_ticks);
//			puts("\n");

		printf("timer ticks=%d\n", timer_ticks);

		return(0);
//			break;
	}

	if(!strcmp(cmd, "rtc"))
	{
		printf("rtc ticks=%9d:%9d\n",
			P_AIC_RTC_SEC_LO, P_AIC_RTC_NSEC);

		return(0);
//			break;
	}

	if(!strcmp(cmd, "die"))
//		if(strcmp(cmd, "die")==0)
	{
		puts("ok, dying\n");
		*(int *)-1=-1;
		return(0);
	}

#ifdef ARCH_HAS_FPU
	if(!strcmp(cmd, "float"))
	{
		float_test0();
		return(0);
	}
#endif

	if(!strcmp(cmd, "framebuf"))
	{
		test_framebuf0();
		return(0);
	}

	if(!strcmp(cmd, "video"))
	{
		test_video0();
		return(0);
	}

	if(!strcmp(cmd, "video1"))
	{
		test_video1();
		return(0);
	}

	if(!strcmp(cmd, "video2"))
	{
		test_video2();
		return(0);
	}

	if(!strcmp(cmd, "jxgpu"))
	{
		tk_jxgpu_test();
		return(0);
	}

	if(!strcmp(cmd, "malloc"))
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

		return(0);
	}

	if(!strcmp(cmd, "readmbr"))
	{
		printf("init device\n");
		TKSPI_InitDevice();

		printf("reading MBR\n");
					
		TKSPI_ReadSectors(tb, 0, 1);
		
		if((((byte)tb[510])==0x55) && (((byte)tb[511])==0xAA))
		{
			printf("Got 55AA\n");
		}else
		{
			printf("No 55AA\n");
		}
		
		if(!tkfat_fsimg)
		{
			tkfat_fsimg=malloc(sizeof(TKFAT_ImageInfo));
			memset(tkfat_fsimg, 0, sizeof(TKFAT_ImageInfo));
			TKFAT_ReadImageMBR(tkfat_fsimg);
			TKFAT_ReadImageFAT(tkfat_fsimg);
			TKFAT_ListDir(tkfat_fsimg, tkfat_fsimg->clid_root);
		}
		
		return(0);
	}

	printf("don't understand %s\n", tb);
	return(0);
}

void tk_main()
{
	char tb[1024];
	void *p0, *p1;
	int i;

	timer_ticks=0;

	setGpioOutputs(0x69);
	puts("tk_main: puts\n");
	tk_ird_init();
	tkmm_init();
	
	if(kerninit[0])
	{
		tk_exec(kerninit);
	}
	
	while(1)
	{
		puts("$ ");

		gets(tb);
		
		puts("got: ");
		puts(tb);
		puts("\n");

		i=tk_exec(tb);
		if(i==-99)
			break;
	}
}
