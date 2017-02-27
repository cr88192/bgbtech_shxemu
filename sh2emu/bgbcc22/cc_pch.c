#include <bgbscr1.h>
#include <bgbasm.h>

#include <time.h>

extern int bs1_pp_warn, bs1_pp_err;
extern int bs1_cparse_warn, bs1_cparse_err;

extern elem bs1_cparse_structs;
extern elem bs1_cparse_types;


int BS1_BSCH_SaveChunk(void *fd, char *name, byte *buf, int sz)
{
	byte ch[64];
	byte *tbuf;
	int i, j, k;

	tbuf=malloc(sz*2);
	memset(tbuf, 0, sz*2);

	j=sz;
	i=BS1_MiniLZ_Encode(buf, tbuf, j);
	k=BS1_MiniLZ_DataAdler32(buf, j, 0);

	memset(ch, 0, 64);
	strcpy(ch, name);

	ch[48]=i&0xFF;		ch[49]=(i>>8)&0xFF;
	ch[50]=(i>>16)&0xFF;	ch[51]=(i>>24)&0xFF;
	ch[52]=j&0xFF;		ch[53]=(j>>8)&0xFF;
	ch[54]=(j>>16)&0xFF;	ch[55]=(j>>24)&0xFF;
	ch[56]=k&0xFF;		ch[57]=(k>>8)&0xFF;
	ch[58]=(k>>16)&0xFF;	ch[59]=(k>>24)&0xFF;
	ch[60]='B';		ch[61]='S';
	ch[62]='C';		ch[63]='H';

	basm_fwrite(ch, 1, 64, fd);
	basm_fwrite(tbuf, 64, (i+63)/64, fd);

	free(tbuf);
}

int BS1_BSCH_FindChunk(void *fd, char *name,
	int *offs, int *csz, int *dsz, int *crc)
{
	byte ch[64];
	int i, j, k;

	basm_fseek(fd, 0, 0);
	while(1)
	{
		memset(ch, 0, 64);
		basm_fread(ch, 1, 64, fd);
		if(!ch[0])break;

		i=ch[48]|(ch[49]<<8)|(ch[50]<<16)|(ch[51]<<24);
		j=ch[52]|(ch[53]<<8)|(ch[54]<<16)|(ch[55]<<24);
		k=ch[56]|(ch[57]<<8)|(ch[58]<<16)|(ch[59]<<24);

		if(!strcmp(ch, name))
		{
			*offs=basm_ftell(fd);
			if(csz)*csz=i;
			if(dsz)*dsz=j;
			if(crc)*crc=k;
			return(0);
		}

		basm_fseek(fd, 64*((i+63)/64), 1);
	}
	return(-1);
}

byte *BS1_BSCH_LoadChunk(void *fd, char *name, int *rsz)
{
	byte *buf, *tbuf;
	int offs, csz, dsz, crc;
	int i, j, k;

	i=BS1_BSCH_FindChunk(fd, name, &offs, &csz, &dsz, &crc);
	if(i<0)return(NULL);

	buf=malloc(csz);
	tbuf=malloc(dsz);

	basm_fseek(fd, offs, 0);
	basm_fread(buf, 1, csz, fd);

	i=BS1_MiniLZ_DecodeSafe(buf, csz, tbuf, dsz);
	if((i<0) || (i!=dsz)) { free(buf); free(tbuf); return(NULL); }
	i=BS1_MiniLZ_DataAdler32(tbuf, dsz, 0);
	if(i!=crc) { free(buf); free(tbuf); return(NULL); }

	free(buf);
	if(rsz)*rsz=dsz;
	return(tbuf);
}

elem BS1_BSCH_LoadModuleAST(char *name)
{
	char tb[256];
	elem t;
	void *fd;
	char *buf;
	int i, j, k, sz;

	sprintf(tb, "%s.pco", name);
	fd=basm_fopen(tb, "rb");
	if(!fd)
	{
		printf("BS1_BSCH_LoadModuleAST: fail open '%s'\n", tb);
		return(BS1_MM_NULL);
	}

	buf=BS1_BSCH_LoadChunk(fd, "ParseTree", &sz);
	if(!buf)
	{
		printf("BS1_BSCH_LoadModuleAST: fail load AST in '%s'\n", tb);
		return(BS1_MM_NULL);
	}

	t=BS1_ParseS_FromString(buf);
	free(buf);

	return(t);
}


int BS1_CComp_PrecompileModule(char *name)
{
	char tb[256];
	unsigned char ch[64];
	elem t;
	void *ofd;
	char *s, *buf, *tbuf, *tbuf2;
	int i, j, k, sz, t0, t1, dt;

	printf("Precompile %s\n", name);

	BS1_CComp_Force();


	buf=basm_loadfile(name, &sz);
	if(!buf)
	{
		printf("BS1_CComp_PrecompileModule: fail load '%s'\n", name);
		return(-1);
	}

	printf("BS1_CComp_PrecompileModule: %s %d bytes\n", name, sz);

	sprintf(tb, "%s.pco", name);
	ofd=basm_fopen(tb, "wb");

	if(!ofd)
	{
		printf("BS1_CComp_PrecompileModule: fail open out '%s'\n", tb);
		free(buf);
		return(-1);
	}

	tbuf=malloc(1<<24);
	memset(tbuf, 0, 1<<24);

	BS1_BSCH_SaveChunk(ofd, "Source", buf, sz);


	t0=clock();

	BS1_CParse_PushLinenum();
	BS1_CParse_SetLinenum(name, buf, 1);

	BS1_PP_Filter(buf, tbuf);

	BS1_CParse_PopLinenum();

	t1=clock();
	dt=(1000.0*(t1-t0))/CLOCKS_PER_SEC;
	printf("PreProc took %dms\n", dt);

	BS1_BSCH_SaveChunk(ofd, "PreProc", tbuf, strlen(tbuf));

	t0=clock();

	bs1_cparse_warn=0;
	bs1_cparse_err=0;

	SET(bs1_cparse_structs, BS1_MM_NULL);
	SET(bs1_cparse_types, BS1_MM_NULL);

	BS1_CParse_SetLinenum(name, tbuf, 1);
	s=tbuf;
	t=BS1_CParse_Block(&s);

	t=CONS(LIST2S("mod-types", bs1_cparse_types), t);
	t=CONS(LIST2S("mod-structs", bs1_cparse_structs), t);

	t1=clock();
	dt=(1000.0*(t1-t0))/CLOCKS_PER_SEC;
	printf("Parse took %dms\n", dt);

	BS1_S_Print(tbuf, t, -1);
	BS1_BSCH_SaveChunk(ofd, "ParseTree", tbuf, strlen(tbuf));

	memset(ch, 0, 64);
	basm_fwrite(ch, 1, 64, ofd);

	basm_fclose(ofd);

	free(tbuf);

	return(t);
}

