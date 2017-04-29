#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <bgbasm.h>
#include <bgbccc.h>

// char *(*bgbpp_loadfile_fp)(char *name, int *sz);

int main(int argc, char *argv[])
{
	BCCX_Node *tmp;
	char *buf, *s;
	int sz;

//	bgbpp_loadfile_fp=(char*(*)(char*,int*))basm_loadfile;

	buf=bgbcc_loadfile(argv[1], &sz);
	tmp=BGBCP_ModuleBuffer(argv[1], argv[1], buf);

	BCCX_PrintFD(stdout, tmp);

	s=BGBCC_CompileModule(argv[1], tmp);
	printf("MOD %s\n", s);
}

