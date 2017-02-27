#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bgbasm.h>
#include <bgbgc.h>
#include <bgbdy.h>
#include <bgbccc.h>

int main(int argc, char *argv[])
{
	char *uds[64], *args[256];
	void *p;
	int n, m, nargs;
	int i;

	dy_init(&argc);

	nargs=0;
	args[nargs++]=argv[0];

	n=0; m=0;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--"))
				{ i++; break; }
			if(!strcmp(argv[i], "--pc"))
				m|=1;
			continue;
		}
		uds[n++]=argv[i];
	}

	nargs=0; args[nargs++]=argv[0];
	for(; i<argc; i++)args[nargs++]=argv[i];

#ifdef WIN32
	ccLoadLibrary("msvcrt");
#endif

	for(i=0; i<n; i++)
		ccLoadModule(uds[i]);

//	for(i=1; i<argc; i++)
//		ccLoadModule(argv[i]);

	p=ccGetAddr("bscc_main");
	if(p)
	{
		printf("Got(bscc_main) %p\n", p);
//		i=((int(*)())p)();
		i=((int(*)(int argc, char **argv))p)(nargs, args);
		return(i);
	}

	p=ccGetAddr("main");
	if(p && (p!=((void *)(&main))))
	{
		printf("Got(main) %p\n", p);
//		i=((int(*)(int argc, char **argv))p)(nargs, args);
		return(i);
	}

//	fgetc(stdin);
}

