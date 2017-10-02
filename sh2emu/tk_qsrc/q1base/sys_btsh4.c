/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// sys_null.h -- null system driver to aid porting efforts

#include "quakedef.h"
#include "errno.h"

qboolean isDedicated=0;

/*
===============================================================================

FILE IO

===============================================================================
*/

#define MAX_HANDLES             256
FILE    *sys_handles[MAX_HANDLES];

int             findhandle (void)
{
	int             i;
	
	for (i=1 ; i<MAX_HANDLES ; i++)
		if (!sys_handles[i])
			return i;
	Sys_Error ("out of handles");
	return -1;
}

/*
================
filelength
================
*/
int filelength (FILE *f)
{
	int             pos;
	int             end;

	printf("filelength: A\n");
	pos = ftell (f);
	printf("filelength: B\n");
	fseek (f, 0, SEEK_END);
	printf("filelength: C\n");
	end = ftell (f);
	printf("filelength: D\n");
//	fseek (f, pos, SEEK_SET);
	fseek (f, 0, SEEK_SET);
	printf("filelength: E\n");

	return end;
}

int Sys_FileOpenRead (char *path, int *hndl)
{
	FILE    *f;
	int             i;
	
	i = findhandle ();

	f = fopen(path, "rb");
	if (!f)
	{
		*hndl = -1;
		return -1;
	}
	sys_handles[i] = f;
	*hndl = i;
	
	return filelength(f);
}

int Sys_FileOpenWrite (char *path)
{
	FILE    *f;
	int             i;
	
	i = findhandle ();

	f = fopen(path, "wb");
	if (!f)
		Sys_Error ("Error opening %s: %s", path,strerror(errno));
	sys_handles[i] = f;
	
	return i;
}

void Sys_FileClose (int handle)
{
	fclose (sys_handles[handle]);
	sys_handles[handle] = NULL;
}

void Sys_FileSeek (int handle, int position)
{
	fseek (sys_handles[handle], position, SEEK_SET);
}

int Sys_FileRead (int handle, void *dest, int count)
{
	int rlen;
	tk_printf("Sys_FileRead hdl=%d dst=%p sz=%d fd=%p\n",
		handle, dest, count, sys_handles[handle]);
	rlen = fread (dest, 1, count, sys_handles[handle]);
	tk_printf("Sys_FileRead rlen=%d\n", rlen);
	return(rlen);
}

int Sys_FileWrite (int handle, void *data, int count)
{
	return fwrite (data, 1, count, sys_handles[handle]);
}

int     Sys_FileTime (char *path)
{
	FILE    *f;
	
	f = fopen(path, "rb");
	if (f)
	{
		fclose(f);
		return 1;
	}
	
	return -1;
}

void Sys_mkdir (char *path)
{
}


/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_MakeCodeWriteable (nlint startaddr, nlint length)
{
}


void Sys_Error (char *error, ...)
{
//	char tb[1024];
	va_list         argptr;

	tk_printf("Sys_Error: ");   
	va_start(argptr, error);
//	vfprintf(stdout, error, argptr);
//	vsprintf(tb, error, argptr);
	tk_vprintf(error, argptr);
	va_end(argptr);
//	printf("%s\n", tb);

//	exit(1);
	*(int *)-1=-1;
}

void Sys_Printf (char *fmt, ...)
{
	char tb[1024];
	va_list         argptr;
	
	va_start(argptr, fmt);
//	vfprintf(stdout, fmt, argptr);
//	vsprintf(tb, fmt, argptr);
	tk_vprintf(fmt, argptr);
	va_end(argptr);
//	tk_puts(tb);
}

void Sys_Quit (void)
{
	exit (0);
}

u32 TK_GetTimeMs(void);

double Sys_FloatTime (void)
{
	static double t, t0;
//	static float t, t0;
	
//	t += 0.1;

	t0 = (int)TK_GetTimeMs();
	t = t0*0.001;
	
//	printf("\t\t\t\tT=%f\r", t);
	
//	*(int *)-1=-1;
	return t;
}

char *Sys_ConsoleInput (void)
{
	return NULL;
}

void Sys_Sleep (void)
{
}

void Sys_SendKeyEvents (void)
{
}

void Sys_HighFPPrecision (void)
{
}

void Sys_LowFPPrecision (void)
{
}

void sleep_0();

#ifndef _BGBCC
void __debugbreak()
{
//	*(int *)-1=-1;
}
#endif

void Sys_CheckSanity(void)
{
	static byte pat_tst0[16]={
		0x78, 0x56, 0x34, 0x12,
		0xEF, 0xCD, 0xAB, 0x89,
		};
	static int rec=0;
	int *pj, *pk;
	double		time, oldtime, newtime;
	double f, g, h;
	float ff, gf, hf;
	int i, j, k, l;

	if(!rec)
	{
		tk_puts("Q Flt 0: ");
		tk_print_float(3.14159);
		tk_puts("\n");
		time=3.14159*2;
		tk_puts("Q Flt 1: ");
		tk_print_float(time);
		tk_puts("\n");

		tk_printf("Q Flt 2: %f\n", time);
		printf("Q Flt 3: %f\n", time);
		
		*(double *)(&oldtime)=time;
		newtime=*(double *)(&time);
		tk_printf("Q Flt 4: %f\n", oldtime);
		tk_printf("Q Flt 5: %f\n", newtime);
		
		oldtime=time*10.0;
		newtime=time*0.1;
		tk_printf("Q Flt 4-1: %f\n", oldtime);
		tk_printf("Q Flt 5-1: %f\n", newtime);
		
		time=Q_atof("3.14159");
	//	*(int *)-1=-1;
		tk_printf("Q Flt 6: %f\n", time);

		for(i=-1; i<=1; i++)
			for(j=-1; j<=1; j++)
		{
			f=i; g=j;

			tk_printf("%d==%d -> %d\n", i, j, i==j);
			tk_printf("%d!=%d -> %d\n", i, j, i!=j);
			tk_printf("%d< %d -> %d\n", i, j, i< j);
			tk_printf("%d> %d -> %d\n", i, j, i> j);
			tk_printf("%d<=%d -> %d\n", i, j, i<=j);
			tk_printf("%d>=%d -> %d\n", i, j, i>=j);

			if(i==j)	tk_printf("%d==%d -> T\n", i, j);
			else		tk_printf("%d==%d -> F\n", i, j);
			if(i!=j)	tk_printf("%d!=%d -> T\n", i, j);
			else		tk_printf("%d!=%d -> F\n", i, j);
			if(i<j)		tk_printf("%d< %d -> T\n", i, j);
			else		tk_printf("%d< %d -> F\n", i, j);
			if(i>j)		tk_printf("%d> %d -> T\n", i, j);
			else		tk_printf("%d> %d -> F\n", i, j);
			if(i<=j)	tk_printf("%d<=%d -> T\n", i, j);
			else		tk_printf("%d<=%d -> F\n", i, j);
			if(i>=j)	tk_printf("%d>=%d -> T\n", i, j);
			else		tk_printf("%d>=%d -> F\n", i, j);

			tk_printf("%f==%f -> %d\n", f, g, f==g);
			tk_printf("%f!=%f -> %d\n", f, g, f!=g);
			tk_printf("%f< %f -> %d\n", f, g, f< g);
			tk_printf("%f> %f -> %d\n", f, g, f> g);
			tk_printf("%f<=%f -> %d\n", f, g, f<=g);
			tk_printf("%f>=%f -> %d\n", f, g, f>=g);
			
			if(f==g)	tk_printf("%f==%f -> T\n", f, g);
			else		tk_printf("%f==%f -> F\n", f, g);
			if(f!=g)	tk_printf("%f!=%f -> T\n", f, g);
			else		tk_printf("%f!=%f -> F\n", f, g);
			if(f<g)		tk_printf("%f<%f -> T\n", f, g);
			else		tk_printf("%f<%f -> F\n", f, g);
			if(f>g)		tk_printf("%f>%f -> T\n", f, g);
			else		tk_printf("%f>%f -> F\n", f, g);
			if(f<=g)	tk_printf("%f<=%f -> T\n", f, g);
			else		tk_printf("%f<=%f -> F\n", f, g);
			if(f>=g)	tk_printf("%f>=%f -> T\n", f, g);
			else		tk_printf("%f>=%f -> F\n", f, g);
		}

		for(i=0; i<32; i++)
		{
			f=((M_PI*2.0)/16.0)*((double)i);
			g=sqrt(i);
			h=g*g;
			tk_printf("i=%d: ", i);
	//		tk_printf("a=%f sin=%f cos=%f sqrt(i)=%f(^2=%f)\n",
	//			f, sin(f), cos(f), g, h);
			tk_printf("a=%f sin=%f cos=%f ", f, sin(f), cos(f));
			tk_printf("sqrt(i)=%f(^2=%f)\n", g, h);
		}
	}

#if 1
	if(LittleShort(0x1234)!=0x1234)
		__debugbreak();
	if(LittleLong(0x12345678)!=0x12345678)
		__debugbreak();
	if(LittleShort(0xABCD)!=(short)0xABCD)
		__debugbreak();
	if(LittleLong(0x89ABCDEF)!=0x89ABCDEF)
		__debugbreak();

	if(LittleShort(0x12345678)!=0x5678)
		__debugbreak();
	if(LittleShort(0x89ABCDEF)!=(short)0xCDEF)
		__debugbreak();

	if(LittleFloat(M_PI)!=((float)M_PI))
		__debugbreak();

	if(LittleShort(*(short *)(pat_tst0+2))!=0x1234)
		__debugbreak();
	if(LittleLong(*(int *)(pat_tst0+0))!=0x12345678)
		__debugbreak();
	if(LittleShort(*(short *)(pat_tst0+5))!=(short)0xABCD)
		__debugbreak();
	if(LittleLong(*(int *)(pat_tst0+4))!=0x89ABCDEF)
		__debugbreak();

	if(LittleShort(*(int *)(pat_tst0+0))!=0x5678)
		__debugbreak();
	if(LittleShort(*(int *)(pat_tst0+4))!=(short)0xCDEF)
		__debugbreak();


	i=4095;
	if(&pr_functions[i] != pr_functions+i)
		__debugbreak();
	if((byte *)(&pr_functions[i]) !=
		(byte *)pr_functions+i*sizeof(*pr_functions))
			__debugbreak();
	if(sizeof(*pr_functions)!=sizeof(dfunction_t))
		__debugbreak();
	if(sizeof(dfunction_t)!=36)
		__debugbreak();
		
	pj=(int *)pat_tst0+3;
	pk=(int *)pat_tst0;
	if((pj-pk)!=3)
		__debugbreak();
	if(((byte *)pj-(byte *)pk)!=12)
		__debugbreak();
#endif

	i=3; j=4; k=5;
	if((i+j)!=7)
		__debugbreak();
	if((i-k)!=-2)
		__debugbreak();
	if((i*j)!=12)
		__debugbreak();

	if((i&j)!=0)
		__debugbreak();
	if((i|k)!=7)
		__debugbreak();
	if((i^k)!=6)
		__debugbreak();

	i=-6972; j=1;

	if((i>>1)!=(-3486))
		__debugbreak();
	if((((u32)i)>>1)!=(0x7FFFF262))
		__debugbreak();

	if((i>>j)!=(-3486))
		__debugbreak();
	if((((u32)i)>>j)!=(0x7FFFF262))
		__debugbreak();

	k=0;
	if((i<<k)!=i)
		__debugbreak();
	if((i>>k)!=i)
		__debugbreak();

	j=1; k=9;
	if((j<<k)!=512)
		__debugbreak();
	if((j<<8)!=256)
		__debugbreak();
	if((2<<k)!=1024)
		__debugbreak();

	i>>=3;
	if(i!=(-872))
		__debugbreak();
	i<<=19;
	if(i!=(-457179136))
		__debugbreak();
	i=-i;
	if(i!=(457179136))
		__debugbreak();

	i=-6972; j=1;

	f=i; ff=i;
	if(f!=(-6972.0))
		__debugbreak();
	if(ff!=(-6972.0))
		__debugbreak();
		
	f=3.14159; ff=f;
	i=f; j=ff;
	if(i!=3)
		__debugbreak();
	if(j!=3)
		__debugbreak();

	i=0; k=-1;

	f=7.0; g=8.0; h=9.0;
	if((f+g)!=15.0)
		__debugbreak();
	if((f-h)!=-2.0)
		__debugbreak();
	if((f*h)!=63.0)
		__debugbreak();
	if((h/g)!=1.125)
		__debugbreak();

	if(ceil(3.14)!=4.0)
		__debugbreak();
	if(floor(3.14)!=3.0)
		__debugbreak();

	if(i)
		__debugbreak();
	if(!j)
		__debugbreak();
	if(j<=0)
		__debugbreak();
	if(k>=0)
		__debugbreak();
	if(i<0)
		__debugbreak();
	if(i>0)
		__debugbreak();

	i=0; j=1; k=!j; l=!i;
	if(i && j)
		__debugbreak();
	if(i || k)
		__debugbreak();
	if(j && l)	{}
	else	__debugbreak();
	if(j || k) {}
	else	__debugbreak();
	if(k || l) {}
	else	__debugbreak();

	if((i && j) || !(j && l))
		__debugbreak();
	if((i || j) && (j && l)) {}
	else	__debugbreak();

	if((i || j) && (i || k))
		__debugbreak();
	if((i || k) && (i || j))
		__debugbreak();

	if((i && j) || (j && l)) {}
	else	__debugbreak();
	if((j && l) || (i && j)) {}
	else	__debugbreak();

	i='5';
	if (i >= '0' && i <= '9') {}
	else	__debugbreak();
	if (i >= 'a' && i <= 'z')
		{ __debugbreak(); }
	else	{ }

	i='H';
	if (i >= 'A' && i <= 'Z') {}
	else	__debugbreak();

	i=999; j=10;
	if((i%j)!=9)
		__debugbreak();
	i=486; j=7;
	if((i%j)!=3)
		__debugbreak();

	if(!rec)
	{
		i=3; j=4; k=5;
		f=7.0; g=8.0; h=9.0;
		ff=1.1;	gf=2.2;	hf=3.3;
	}else
	{
		i=3; j=4; k=5;
		f=7.0; g=8.0; h=9.0;
	}

	if(rec<3)
	{
		i=rec;
		rec++;
		Sys_CheckSanity();
		rec--;
		
		if(i!=rec)
			__debugbreak();
	}
	
	if(rec<0)
		__debugbreak();
	
	if(!rec)
	{
		tk_puts("Check for: Int Stomp\n");
	
//		if(i!=3)
//			__debugbreak();
		if(j!=4)
			__debugbreak();
		if(k!=5)
			__debugbreak();

		tk_puts("Check for: Double Stomp\n");

		if(f==g)
			__debugbreak();

		if(f!=7.0)
			__debugbreak();
		if(g!=8.0)
			__debugbreak();
		if(h!=9.0)
			__debugbreak();

		tk_puts("Check for: Float Stomp\n");

		if(ff!=1.1)
			__debugbreak();
		if(gf!=2.2)
			__debugbreak();
		if(hf!=3.3)
			__debugbreak();
	}else
	{
		f=rand()*0.01;
		ff=sin(f);
		gf=cos(f);
		g=ff*f;
		h=gf*f;
	}
}

//=============================================================================

int main (int argc, char **argv)
{
	static quakeparms_t    parms;
	double		time, oldtime, newtime;
	double f, g, h;
	int i, j, k;

	tk_puts("Q Main\n");

//	parms.memsize = 8*1024*1024;
	parms.memsize = 24*1024*1024;
	parms.membase = malloc (parms.memsize);
	parms.basedir = ".";

	tk_puts("Q A0\n");

	fprintf(stderr, "malloc'd: %d @ %p..%p\n",
		parms.memsize,
		parms.membase, parms.membase+parms.memsize);

	tk_puts("Q A1\n");

	COM_InitArgv (argc, argv);

	tk_puts("Q A2\n");

	parms.argc = com_argc;
	parms.argv = com_argv;

	printf ("Host_Init\n");
	Host_Init (&parms);

	printf ("Host_Init: Done\n");

	Sys_CheckSanity();

	printf ("Enter Main Loop\n");
    oldtime = Sys_FloatTime () - 0.1;
	while (1)
	{
        newtime = Sys_FloatTime ();
        time = newtime - oldtime;
        
        if(time<0)
        {
			newtime = Sys_FloatTime ();
			oldtime = newtime;
			time=0;
		}

//		printf("\t\t\t\t\tT=%f %f dt=%f\r", newtime, oldtime, time);

		oldtime+=time;

		Host_Frame (time);
//		Host_Frame (0.1);
		sleep_0();
	}
}


