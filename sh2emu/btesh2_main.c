#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#include "btesh2_multi.c"

byte kbbuf[256];
byte kbrov;
byte kbirov;

#ifdef __linux
static struct termios old_termios;

void btesh2_ttynoncanon(void)
{
	struct termios new_termios;

	tcgetattr(0, &old_termios);
	new_termios=old_termios;
	new_termios.c_lflag&=(~ICANON);
	new_termios.c_lflag&=(~ECHO);
	new_termios.c_cc[VTIME]=0;
	new_termios.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_termios);
	return;
}

void btesh2_resettermios(void)
{
	tcsetattr(0, TCSANOW, &old_termios);
	return;
}
#endif

byte *loadfile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(path, "rb");
	if(!fd)
		return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

u32 btesh2_spanmmio_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 *mmio;
	u32 i;
	
	mmio=(u32 *)(sp->data);
	switch(reladdr>>2)
	{
//	case 0: i=mmio[0]; break;
//	case 1: i=mmio[1]; break;
//	case 2: i=mmio[2]; break;
//	case 3: i=mmio[3]; break;
	
	case 0x10:
		i=mmio[0x10];
//		printf("SPI_C(R): %08X\n", i);
		break;
	case 0x11:
		i=mmio[0x11];
//		printf("SPI_D(R): %08X\n", i);
		break;

#if 1
	case 0x40:
		i=0;
		if(kbirov!=kbrov)
		{
			i=kbbuf[kbirov++];
//			printf("%c", i);
		}
		break;
//	case 0x41: i=0; break;
	case 0x42:
		i=0;
		if(kbirov!=kbrov)
		{
//			printf(".");
			i|=1;
		}
		break;
	case 0x43: i=0; break;
#endif

//	case 0x80: i=mmio[0x80]; break;
//	case 0x81: i=mmio[0x81]; break;
//	case 0x82: i=mmio[0x82]; break;
//	case 0x83: i=mmio[0x83]; break;
//	case 0x84: i=mmio[0x84]; break;
//	case 0x85: i=mmio[0x85]; break;
//	case 0x86: i=mmio[0x86]; break;
//	case 0x87: i=mmio[0x87]; break;
	case 0x88:	i=0; break;
	case 0x89:	i=(cpu->tr_tops>>30); break;
	case 0x8A:	i=(cpu->tr_tops<<2); break;
	
	case 0x85:
	case 0x86:
		break;

	case 0xC0:	case 0xC1:
	case 0xC2:	case 0xC3:
		i=0; break;
	case 0x100:	case 0x101:
	case 0x102:	case 0x103:
		i=0; break;

//	default: i=-1; break;
	default:
		printf("MMIO_Read: %04X\n", reladdr>>2);
		i=0; break;
	}
	
	return(i);
}

int btesh2_spanmmio_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int v;
	u32 *mmio;

	mmio=(u32 *)(sp->data);
	switch(reladdr>>2)
	{
	case 0:
		printf("GPIO_V: %08X\n", val);
		mmio[0]=val;
		break;
	case 1:
		printf("GPIO_M: %08X\n", val);
		mmio[1]=val;
		break;
	case 2:
		printf("GPIO_E: %08X\n", val);
		mmio[2]=val;
		break;
	case 3:
		printf("GPIO_C: %08X\n", val);
		mmio[3]=val;
		break;
	
	case 0x10:
//		printf("SPI_C(W): %08X\n", val);
		v=btesh2_spimmc_XrCtl(cpu, val);
		mmio[0x10]=v&255;
		if(v&0x10000)
			mmio[0x11]=(v>>8)&255;
		break;
	case 0x11:
//		printf("SPI_D(W): %08X\n", val);
		v=btesh2_spimmc_XrByte(cpu, val);
		mmio[0x11]=v;
		break;
	
	case 0x41:
//		printf("Tx: %c\n", val);
		printf("%c", val);
		fflush(stdout);
		break;
//	case 0x42:
//		break;

	case 0x43:
		break;

	case 0xC0:	case 0xC1:
	case 0xC2:	case 0xC3:
		break;
	case 0x100:	case 0x101:
	case 0x102:	case 0x103:
		break;

	default:
		printf("MMIO_Write: %04X %08X\n", reladdr>>2, val);
		break;
	}

	return(0);
}

u32 btesh2_spanmmio_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_spanmmio_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(sbyte)j;
	return(j);
}

u32 btesh2_spanmmio_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_spanmmio_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(s16)j;
	return(j);
}

int btesh2_spanmmio_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	btesh2_spanmmio_SetD(sp, cpu, reladdr, val);
}

int btesh2_spanmmio_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	btesh2_spanmmio_SetD(sp, cpu, reladdr, val);
}

u32 btesh2_spanmmreg_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 v;

	switch(reladdr>>2)
	{
	case 0: v=cpu->regs[BTESH2_REG_PTEH]; break;
	case 1: v=cpu->regs[BTESH2_REG_PTEL]; break;
	case 2: v=cpu->regs[BTESH2_REG_TTB]; break;
	case 3: v=cpu->regs[BTESH2_REG_TEA]; break;
	case 4: v=cpu->regs[BTESH2_REG_MMUCR]; break;
	default:
		printf("MMREG: Unhandled Get A=%08X\n", reladdr);
		v=0; break;
	}
	return(v);
}

int btesh2_spanmmreg_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	switch(reladdr>>2)
	{
	case 0: cpu->regs[BTESH2_REG_PTEH]=val; break;
	case 1: cpu->regs[BTESH2_REG_PTEL]=val; break;
	case 2: cpu->regs[BTESH2_REG_TTB]=val; break;
	case 3: cpu->regs[BTESH2_REG_TEA]=val; break;
	case 4: cpu->regs[BTESH2_REG_MMUCR]=val; break;
	default:
		printf("MMREG: Unhandled Set A=%08X V=%08X\n", reladdr, val);
		break;
	}
	return(0);
}

int help(char *prgname)
{
	printf("usage: %s [options] image\n", prgname);
	printf("  -sh2        Emulate SH-2\n");
	printf("  -sh4        Emulate SH-4\n");
	printf("  -map <map>  Use symbol map\n");
	return(0);
}

int main(int argc, char *argv[])
{
	BTESH2_MemoryImage *img;
	BTESH2_CpuState *cpu;
	BTESH2_PhysSpan *sp;
	byte *ibuf, *tbuf;
	char *imgname, *mapname, *sdname, *sdclname;
	double dt;
	s64 tdt;
	byte sh4;
	int t0, t1, t2;
	int sz, err, ts;
	int i, j, k, l;
	
	sh4=0;
	imgname=NULL;
	mapname=NULL;
	sdname=NULL;
	sdclname=NULL;
	
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-sh4"))
				{ sh4=BTESH2_ARCH_SH4; continue; }
			if(!strcmp(argv[i], "-sh2"))
				{ sh4=BTESH2_ARCH_SH2; continue; }

			if(!strcmp(argv[i], "-map"))
				{ mapname=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "-sd"))
				{ sdname=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "-sdcl"))
				{ sdclname=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "--help"))
				{ sh4=127; continue; }
				
			printf("unrecognized option %s\n", argv[i]);
			continue;
		}
		
		if(!imgname)
			imgname=argv[i];
	}
	
	if(sh4==127)
	{
		help(argv[0]);
		return(0);
	}

	if(sdclname)
	{
		ibuf=loadfile(sdclname, &sz);
		if(ibuf)
		{
			BTESH2_ProcessSDCL(ibuf, sz);
			free(ibuf);
		}
		
		if(!imgname)
			return(0);
	}

	if(!imgname)
	{
		printf("No image name given\n");
		return(-1);
	}

	if(sdname)
	{
		ibuf=loadfile(sdname, &sz);
		if(ibuf)
		{
			printf("Loaded MMC image, %dMB\n", (sz>>20));
			BTESH2_SPIMMC_SetImage(ibuf, sz);
		}
	}
	
	/* Memory Map in SH4 Mode
	 * 0BCD0000: Peripheral
	 */
	
	if(sh4==BTESH2_ARCH_SH4)
	{
#if 0
		if(!imgname)
		{
			imgname="vmlinux_sh4";
			mapname="System_sh4.map";
		}
#endif

		img=BTESH2_AllocMemoryImage(0);
//		BTESH2_MemoryDefineSpan(img, 0x00000000, 0x00003FFF, NULL, "SRAM");
//		BTESH2_MemoryDefineSpan(img, 0x00000000, 0x0000FFFF, NULL, "SRAM");
//		BTESH2_MemoryDefineSpan(img, 0x10000000, 0x17FFFFFF, NULL, "DRAM");

		BTESH2_MemoryDefineSpan(img, 0x0C000000, 0x17FFFFFF, NULL, "DRAM");

		BTESH2_MemoryDefineSpan(img, 0xF4000000, 0xF400FFFF, NULL, "OCA");
		BTESH2_MemoryDefineSpan(img, 0xF5000000, 0xF500FFFF, NULL, "OCD");
		BTESH2_MemoryDefineSpan(img, 0xF6000000, 0xF600FFFF, NULL, "UTLBA");
		BTESH2_MemoryDefineSpan(img, 0xF7000000, 0xF700FFFF, NULL, "UTLBD");

		cpu=BTESH2_AllocCpuState();
		cpu->memory=img;
		cpu->arch=BTESH2_ARCH_SH4;

		cpu->GetAddrByte=BTESH2_GetAddrByteFMMU;
		cpu->GetAddrWord=BTESH2_GetAddrWordFMMU;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordFMMU;
		cpu->SetAddrByte=BTESH2_SetAddrByteFMMU;
		cpu->SetAddrWord=BTESH2_SetAddrWordFMMU;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordFMMU;

	//	cpu->regs[BTESH2_REG_VBR]=0xFAFAFAFAU;
		cpu->regs[BTESH2_REG_SR]=0x000003F3U;

		cpu->regs[BTESH2_REG_SP]=0x8C00FFFC;
//		cpu->regs[BTESH2_REG_SP]=0x0000FFFC;
	}else
	{
		img=BTESH2_AllocMemoryImage(0);
	//	BTESH2_MemoryDefineSpan(img, 0x00000000, 0x00003FFF, NULL, "SRAM");
		BTESH2_MemoryDefineSpan(img, 0x00000000, 0x0000FFFF, NULL, "SRAM");
		BTESH2_MemoryDefineSpan(img, 0x10000000, 0x17FFFFFF, NULL, "DRAM");

		cpu=BTESH2_AllocCpuState();
		cpu->memory=img;
		cpu->arch=BTESH2_ARCH_SH2;

#if 0
		if(!imgname)
		{
//			imgname="boot.elf";
//			mapname=NULL;

			imgname="vmlinux";
			mapname="System.map";
		}
#endif

	//	cpu->regs[BTESH2_REG_VBR]=0xFAFAFAFAU;
		cpu->regs[BTESH2_REG_SR]=0x000003F3U;

		cpu->regs[BTESH2_REG_SP]=0x0000FFFC;
	//	cpu->regs[BTESH2_REG_SP]=0x00003FFC;
	//	cpu->regs[BTESH2_REG_PC]=0x00003FFC;

		cpu->GetAddrByte=BTESH2_GetAddrBytePhy;
		cpu->GetAddrWord=BTESH2_GetAddrWordPhy;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordPhy;
		cpu->SetAddrByte=BTESH2_SetAddrBytePhy;
		cpu->SetAddrWord=BTESH2_SetAddrWordPhy;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordPhy;
	}

	if(1)
	{
		tbuf=malloc(1024);
		memset(tbuf, 0, 1024);
		sp=BTESH2_AllocPhysSpan();
		sp->base=0xABCD0000;	sp->limit=0xABCDFFFF;
		if(cpu->arch==BTESH2_ARCH_SH4)
			{ sp->base=0x0BCD0000;	sp->limit=0x0BCDFFFF; }
		sp->data=tbuf;			sp->name="MMIO";
		sp->GetB=btesh2_spanmmio_GetB;	sp->GetW=btesh2_spanmmio_GetW;
		sp->GetD=btesh2_spanmmio_GetD;	sp->SetB=btesh2_spanmmio_SetB;
		sp->SetW=btesh2_spanmmio_SetW;	sp->SetD=btesh2_spanmmio_SetD;
		i=BTESH2_MemoryAddSpan(img, sp);
	}

	if(1)
	{
		tbuf=malloc(1024);
		memset(tbuf, 0, 1024);

		sp=BTESH2_AllocPhysSpan();
		sp->base=0xFF000000;	sp->limit=0xFF00FFFF;
		sp->data=tbuf;			sp->name="MMREG";
//		sp->GetB=btesh2_spanmmio_GetB;	sp->GetW=btesh2_spanmmio_GetW;
//		sp->SetB=btesh2_spanmmio_SetB;	sp->SetW=btesh2_spanmmio_SetW;
		sp->GetD=btesh2_spanmmreg_GetD;
		sp->SetD=btesh2_spanmmreg_SetD;
		i=BTESH2_MemoryAddSpan(img, sp);
	}

	cpu->logpc=malloc((1<<18)*sizeof(u32));
	cpu->logsp=malloc((1<<18)*sizeof(u32));
	cpu->mlogpc=1<<18;

	ibuf=loadfile(imgname, &sz);
//	ibuf=loadfile("vmlinux", &sz);
//	ibuf=loadfile("boot.elf", &sz);
	if(!ibuf)
	{
		printf("Failed open kernel\n");
		return(-1);
	}

	i=BTESH2_BootLoadElf(cpu, ibuf, sz, 0x10000000);
	if(i<0)
	{
		printf("Load Failed\n");
		return(-1);
	}
	free(ibuf);

	if(mapname)
	{
		ibuf=loadfile(mapname, &sz);
	//	ibuf=loadfile("System.map", &sz);
		if(ibuf)
		{
			BTESH2_BootLoadMap(cpu, ibuf, sz, 0x10000000);
			free(ibuf);
		}
	}

//	l=99999999;
	l=999999999;

#ifdef __linux
	i = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, i | O_NONBLOCK);
	btesh2_ttynoncanon();
#endif

	printf("Boot PC=%08X\n", cpu->regs[BTESH2_REG_PC]);
//	t0=clock();
//	i=BTESH2_RunCpu(cpu, l);
//	t1=clock();
	ts=CLOCKS_PER_SEC/100; err=0;
	t0=clock(); t2=0; tdt=0;
	while(!err)
	{	
		t1=clock();
		t2=t1-t0;
		if((t2>ts) || (t2<0))
		{
#ifdef _WIN32
			if(_kbhit())
			{
				j=_getch();
				
				if(j=='`')
					{ err=1; continue; }
				
	//			printf("K%c", j);
				kbbuf[kbrov]=j;
				kbrov=(kbrov+1)&255;
			}
#endif

#ifdef __linux
			j=fgetc(stdin);
			if(j>0)
			{
				if(j=='`')
					{ err=1; continue; }
				
	//			printf("K%c", j);
				kbbuf[kbrov]=j;
				kbrov=(kbrov+1)&255;
			}
#endif

			tdt+=t2;
			t0=t1;
			BTESH2_CpuTimerInt(cpu);
			continue;
		}

//		err=BTESH2_RunCpu(cpu, 1000);
		err=BTESH2_RunCpu(cpu, 10000);
		
		if(err==BTESH2_EXC_TRAPSLEEP)
		{
#ifdef _WIN32
			Sleep(1);
#endif
#ifdef __linux
//			usleep(1000);
#endif
			cpu->status=0;
			err=0;
			continue;
		}
	}

//	if(err)
	if(1)
	{
		printf("Status=%04X\n", err);
		BTESH2_DumpRegs(cpu);
	}

	dt=tdt/((double)CLOCKS_PER_SEC);
	printf("%f MIPS\n", cpu->tr_tops/(dt*1000000.0));
	
//	t2=t1-t0; dt=t2/((double)CLOCKS_PER_SEC);
//	printf("%f MIPS\n", cpu->tr_tops/(dt*1000000.0));

#ifdef __linux
	btesh2_resettermios();
#endif

}
