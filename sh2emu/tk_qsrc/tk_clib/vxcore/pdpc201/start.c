/*********************************************************************/
/*																   */
/*  This Program Written by Paul Edwards.							*/
/*  Released to the Public Domain									*/
/*																   */
/*********************************************************************/
/*********************************************************************/
/*																   */
/*  start.c - startup/termination code							   */
/*																   */
/*********************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stddef.h"

#if USE_MEMMGR
#include "__memmgr.h"
#endif

extern FILE *__userFiles[__NFILE];

#define MAXPARMS 50 /* maximum number of arguments we can handle */

#ifdef __OS2__
#define INCL_DOS
#include <os2.h>
#endif

#ifdef __WIN32__
#include <windows.h>
#endif

#if (defined(__MSDOS__) && defined(__WATCOMC__))
#define CTYP __cdecl
#else
#define CTYP
#endif

#ifdef __MSDOS__
/* Must be unsigned as it is used for array index */
extern unsigned char *__envptr;
extern unsigned short __osver;
#endif

#ifdef __VSE__
#undef __CMS__
#endif

#ifdef __MVS__
int __tso = 0; /* is this a TSO environment? */
#endif

#ifdef __MAIN_FP__
#ifdef __VX86__
// int (*__main_fp)(int argc, char **argv, char **env);
#else
// int (*__main_fp)(int argc, char **argv);
#endif

// __PDPCLIB_API__ void **__get_main_fp()
//	{ return((void **)(&__main_fp)); }

#else
int main(int argc, char **argv);
#endif

void __exit(int status);
void CTYP __exita(int status);

char *__get_cmdline();
char **__get_cmdenv();

#if !defined(__MVS__) && !defined(__CMS__) && !defined(__VSE__)
static char buffer1[BUFSIZ + 8];
static char buffer2[BUFSIZ + 8];
static char buffer3[BUFSIZ + 8];
#endif

#if USE_MEMMGR
extern void *__lastsup; /* last thing supplied to memmgr */
#endif

char **__eplist;
char *__plist;

__PDPCLIB_API__ int CTYP __start()
{
	int x;
	int argc;
	static char *argv[MAXPARMS + 1];
	int rc;
	char *p;
	char **penv;

	tk_puts("TK: Start A0\n");
	__start_early();

	p=__get_cmdline();
	penv=__get_cmdenv();

	tk_puts("A1\n");

	__init_stdin();
	
	stdin->hfile = 0;
	stdout->hfile = 1;
	stderr->hfile = 2;

	stdin->quickBin = 0;
	stdin->quickText = 0;
	stdin->textMode = 1;
	stdin->intFno = 0;
	stdin->bufStartR = 0;
	stdin->bufTech = _IOLBF;
	stdin->intBuffer = buffer1;
	stdin->fbuf = stdin->intBuffer + 2;
	*stdin->fbuf++ = '\0';
	*stdin->fbuf++ = '\0';
	stdin->szfbuf = BUFSIZ;
	stdin->endbuf = stdin->fbuf + stdin->szfbuf;
	*stdin->endbuf = '\n';
	stdin->noNl = 0;
	stdin->upto = stdin->endbuf;
	stdin->bufStartR = -stdin->szfbuf;
	stdin->mode = __READ_MODE;
	stdin->ungetCh = -1;
	stdin->update = 0;
	stdin->theirBuffer = 0;
	stdin->permfile = 1;
	stdin->isopen = 1;

	stdout->quickBin = 0;
	stdout->quickText = 0;
	stdout->textMode = 1;
	stdout->bufTech = _IOLBF;
	stdout->intBuffer = buffer2;
	stdout->fbuf = stdout->intBuffer;
	*stdout->fbuf++ = '\0';
	*stdout->fbuf++ = '\0';
	stdout->szfbuf = BUFSIZ;
	stdout->endbuf = stdout->fbuf + stdout->szfbuf;
	*stdout->endbuf = '\n';
	stdout->noNl = 0;
	stdout->upto = stdout->fbuf;
	stdout->bufStartR = 0;
	stdout->mode = __WRITE_MODE;
	stdout->update = 0;
	stdout->theirBuffer = 0;
	stdout->permfile = 1;
	stdout->isopen = 1;

	stderr->quickBin = 0;
	stderr->quickText = 0;
	stderr->textMode = 1;
	stderr->bufTech = _IOLBF;
	stderr->intBuffer = buffer3;
	stderr->fbuf = stderr->intBuffer;
	*stderr->fbuf++ = '\0';
	*stderr->fbuf++ = '\0';
	stderr->szfbuf = BUFSIZ;
	stderr->endbuf = stderr->fbuf + stderr->szfbuf;
	*stderr->endbuf = '\n';
	stderr->noNl = 0;
	stderr->upto = stderr->fbuf;
	stderr->bufStartR = 0;
	stderr->mode = __WRITE_MODE;
	stderr->update = 0;
	stderr->theirBuffer = 0;
	stderr->permfile = 1;
	stderr->isopen = 1;

	for (x=0; x < __NFILE; x++)
	{
		__userFiles[x] = NULL;
	}

	tk_puts("A2\n");

	if(*p=='"')
	{
		p++;
		argv[0] = p;
		while(p && ((*p)!='"'))p++;
		*p++ = '\0';
	}else
	{
		argv[0] = p;
		p = strchr(p, ' ');
		if (p == NULL)
		{
			p = "";
		}
		else
		{
			*p = '\0';
			p++;
		}
	}

	tk_puts("A3\n");

	while (*p == ' ')
	{
		p++;
	}
	if (*p == '\0')
	{
		argv[1] = NULL;
		argc = 1;
	}
	else
	{
		for (x = 1; x < MAXPARMS; )
		{
			char srch = ' ';

			if (*p == '"')
			{
				p++;
				srch = '"';
			}
			argv[x] = p;
			x++;

			if(x>=MAXPARMS)
				break;

			p = strchr(p, srch);
			if (p == NULL)
			{
				break;
			}
			else
			{
				*p = '\0';
				p++;
				while (*p == ' ') p++;
				if (*p == '\0') break; /* strip trailing blanks */
			}
		}
		argv[x] = NULL;
		argc = x;
	}

	tk_puts("A4\n");

	__start_late();

	tk_puts("TK: Start Main\n");

	rc = main(argc, argv);
	__exit(rc);
	return (rc);
}

void __exit(int status)
{
	int x;

	for (x = 0; x < __NFILE; x++)
	{
		if (__userFiles[x] != NULL)
		{
			fclose(__userFiles[x]);
		}
	}
	if (stdout != NULL) fflush(stdout);
	if (stderr != NULL) fflush(stderr);


#if USE_MEMMGR
	memmgrTerm(&__memmgr);
#endif /* USE_MEMMGR */

	__exita(status);
	while(1);
}
