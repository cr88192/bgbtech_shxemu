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
	fseek (f, pos, SEEK_SET);
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
//	printf("Sys_FileRead %d %p %d %p\n",
//		handle, dest, count, sys_handles[handle]);
	return fread (dest, 1, count, sys_handles[handle]);
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

//=============================================================================

int main (int argc, char **argv)
{
	static quakeparms_t    parms;
	double		time, oldtime, newtime;

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


