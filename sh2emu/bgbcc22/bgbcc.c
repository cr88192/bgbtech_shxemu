#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bgbasm.h>
#include <bgbgc.h>
#include <bgbdy.h>
#include <bgbccc.h>

int help(char *fn)
{
	printf("usage: %s [-c] [-o fname] input.c\n", fn);
	printf("BGB static C compiler...\n");
	return(0);
}

int main(int argc, char *argv[], char **env)
{
	char tb[256];
	char *uds[64], *args[256];
	char *ifn, *ofn;
	char *s, *t;
	void *p;
	int n, m, nargs;
	int i;

	dy_init(&argc);

	BGBCC_InitEnv(argc, argv, env);

	nargs=0;
	args[nargs++]=argv[0];

	ifn=NULL;
	ofn=NULL;

	n=0; m=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--"))
				{ i++; break; }
//			if(!strcmp(argv[i], "--pc"))
//				m|=1;

			if(!strcmp(argv[i], "-c"))
			{
				continue;
			}

			if(!strcmp(argv[i], "-o"))
			{
				ofn=argv[i+1];
				i++;
				continue;
			}

			continue;
		}

		if(!ifn)
		{
			ifn=argv[i];
			continue;
		}

//		uds[n++]=argv[i];
	}

	nargs=0; args[nargs++]=argv[0];
	for(; i<argc; i++)args[nargs++]=argv[i];

//	for(i=0; i<n; i++)
//		ccLoadModule(uds[i]);

//	for(i=1; i<argc; i++)
//		ccLoadModule(argv[i]);

	if(!ifn)
	{
		help(argv[0]);
		return(-1);
	}

	if(!ofn)
	{
		strcpy(tb, ifn);
		s=tb; t=s+strlen(s);
		while((t>s) && (*t!='.') && (*t!='/'))
			t--;

		if((t>s) && (*t=='.'))
		{
			strcpy(t, ".o");
		}else
		{
			strcat(tb, ".o");
		}

		ofn=strdup(tb);
	}

	ccCompileModule(ifn, ofn);
}

