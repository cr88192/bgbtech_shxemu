#ifdef WIN32
#include <windows.h>
#include <winbase.h>
#endif

#include <bgbccc.h>
#include <bgbasm.h>
// #include <rpnil.h>
// #include <sxil.h>

#include <bgbgc.h>
#include <bgbdy.h>

// #include <bgbsvm.h>

#include <time.h>

extern int bgbpp_warn, bgbpp_err;
extern int bgbcp_warn, bgbcp_err;

extern BCCX_Node *bgbcp_structs;
extern BCCX_Node *bgbcp_types;

void *bgbcc_mutex;

char *bgbcc_loads[4096];
int bgbcc_nloads=0;

char *bgbcc_src[256];	//source paths
int bgbcc_nsrc;		//num source paths

char *bgbcc_inc[256];	//source paths
int bgbcc_ninc;		//num source paths

char *bgbcc_lib[256];	//source paths
int bgbcc_nlib;		//num source paths

int bgbcc_arch;

char *(*bgbpp_loadfile_fp)(char *name, int *sz);

static void bccx_print_vf(void *p, char *b)
	{ vfwrite(b, 1, strlen(b), (VFILE *)p); }

void BCCX_DumpVF(void *fd, BCCX_Node *node)
	{ BCCX_PrintBuf(node, -1, bccx_print_vf, fd); }
void BCCX_PrintVF(void *fd, BCCX_Node *node)
	{ BCCX_PrintBuf(node, 0, bccx_print_vf, fd); }

int BGBCC_BindSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

//	DYLL_MetaPath_BindKey(name, "func");
	dyllMetaBindKey(name, "func");
	sprintf(tb, "%s:sig", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);

//	sprintf(tb, "FN:%s", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

int BGBCC_BindTypeSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

//	DYLL_MetaPath_BindKey(name, "type");
	dyllMetaBindKey(name, "type");

	sprintf(tb, "%s:sig", name);

//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);

//	sprintf(tb, "TY:%s", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

int BGBCC_BindVarSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];
	char *ts;

	ts="var";
	if(ctx->cur_struct)ts="field";

	dyllMetaBindKey(name, ts);
	sprintf(tb, "%s:sig", name);
	dyllMetaBindKey(tb, sig);
	return(0);
}

int BGBCC_BindVarInfo(BGBCC_State *ctx, char *name, char *key, char *val)
{
	char tb[256];

	sprintf(tb, "%s:%s", name, key);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_BindTypeInfo(BGBCC_State *ctx, char *name, char *key, char *val)
	{ return(BGBCC_BindVarInfo(ctx, name, key, val)); }

int BGBCC_BindDyTypeSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

	sprintf(tb, "bgbdy/%s", name);
	dyllMetaBindKey(tb, "dytype");
	sprintf(tb, "bgbdy/%s:sig", name);
	dyllMetaBindKey(tb, sig);
	return(0);
}

char *BGBCP_LookupSig(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	char *s;

//	s=DYLL_MetaPath_LookupKey(name);
	s=dyllMetaLookupKey(name);
	if(!s)return(NULL);
	if(strcmp(s, "func"))return(NULL);

	sprintf(tb, "%s:sig", name);
//	s=DYLL_MetaPath_LookupKey(tb);
	s=dyllMetaLookupKey(tb);
	return(s);

//	sprintf(tb, "FN:%s", name);
//	return(DYLL_MetaPath_LookupKey(tb));
}

char *BGBCP_LookupTypeSig(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	char *s;

//	s=DYLL_MetaPath_LookupKey(name);
	s=dyllMetaLookupKey(name);

	if(!s)return(NULL);
	if(strcmp(s, "type"))return(NULL);

	sprintf(tb, "%s:sig", name);
//	s=DYLL_MetaPath_LookupKey(tb);
	s=dyllMetaLookupKey(name);
	return(s);

//	sprintf(tb, "TY:%s", name);
//	return(DYLL_MetaPath_LookupKey(tb));
}

int BGBCC_BindStructSig(BGBCC_State *ctx, char *name, char *sig)
{
	char tb[256];

//	DYLL_MetaPath_BindKey(name, sig);
	dyllMetaBindKey(name, sig);

//	sprintf(tb, "S:%s", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

int BGBCC_BindStructInfo(BGBCC_State *ctx, char *name, char *key, char *val)
{
	char tb[256];

//	sprintf(tb, "SI:%s:%s", name, key);
	sprintf(tb, "%s:%s", name, key);
//	DYLL_MetaPath_BindKey(tb, val);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_BindStructInfoIdx(BGBCC_State *ctx,
	char *name, char *key, int idx, char *val)
{
	char tb[256];

//	sprintf(tb, "SI:%s:%s.%d", name, key, idx);
	sprintf(tb, "%s:%s.%d", name, key, idx);
//	DYLL_MetaPath_BindKey(tb, val);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_BindFieldSig(BGBCC_State *ctx, char *cls, char *name, char *sig)
{
	char tb[256];

//	sprintf(tb, "F:%s:%s", cls, name);
//	sprintf(tb, "%s:%s", cls, name);

	sprintf(tb, "%s/%s", cls, name);
//	DYLL_MetaPath_BindKey(tb, "field");
	dyllMetaBindKey(tb, "field");

	sprintf(tb, "%s/%s:sig", cls, name);
//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);
	return(0);
}

int BGBCC_BindFieldInfo(BGBCC_State *ctx,
	char *cls, char *name, char *key, char *val)
{
	char tb[256];

//	sprintf(tb, "FI:%s/%s:%s", cls, name, key);
	sprintf(tb, "%s/%s:%s", cls, name, key);
//	DYLL_MetaPath_BindKey(tb, val);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_BindStructInfoI(BGBCC_State *ctx,
	char *name, char *key, int val)
{
	char tb[64];

	sprintf(tb, "%d", val);
	BGBCC_BindStructInfo(ctx, name, key, tb);
	return(0);
}

int BGBCC_BindFieldInfoI(BGBCC_State *ctx,
	char *cls, char *name, char *key, int val)
{
	char tb[64];

	sprintf(tb, "%d", val);
	BGBCC_BindFieldInfo(ctx, cls, name, key, tb);
	return(0);
}

char *BGBCP_LookupPPKey(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	char *s;

	sprintf(tb, "CPreProc/%s", name);
//	s=DYLL_MetaPath_LookupKey(tb);
	s=dyllMetaLookupKey(tb);
	return(s);
}

int BGBCC_BindPPKey(BGBCP_ParseState *ctx, char *name, char *sig)
{
	char tb[256];

	sprintf(tb, "CPreProc/%s", name);

//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);
	return(0);
}

int BGBCC_StoreTextFile(char *name, char *buf)
{
	VFILE *fd;

//	basm_storetextfile(b, tbuf);

	fd=vffopen(name, "wt");
	if(fd)
	{
		vfwrite(buf, 1, strlen(buf), fd);
		vfclose(fd);
	}

	return(0);
}

int BGBCC_StoreFile(char *name, void *buf, int sz)
{
	VFILE *fd;

//	basm_storetextfile(b, tbuf);

	fd=vffopen(name, "wb");
	if(fd)
	{
		vfwrite(buf, 1, sz, fd);
		vfclose(fd);
	}

	return(0);
}

int BGBCC_GetArch()
	{ return(bgbcc_arch); }

char *bgbcc_loadfile(char *name, int *rsz)
{
	VFILE *fd;
	void *buf;
	int sz;

	fd=vffopen(name, "rb");
	if(!fd)return(NULL);

	vfseek(fd, 0, 2);
	sz=vftell(fd);
	vfseek(fd, 0, 0);

	buf=bgbcc_malloc(sz+16);
	memset(buf, 0, sz+16);
	vfread(buf, 1, sz, fd);

	vfclose(fd);

	if(rsz)*rsz=sz;
	return(buf);

#if 0
	char *buf, *buf1;
	int i, sz;

	buf=basm_loadfile(name, &sz);

	if(buf)
	{
		buf1=bgbcc_malloc(sz);
		memcpy(buf1, buf, sz);
		basm_free(buf);

		if(rsz)*rsz=sz;
		return(buf1);
	}

	return(NULL);
#endif
}

char *BGBCC_ReadFile(char *name, int *rsz)
{
	char tb[256];
	char *buf;
	int i, sz;

	buf=bgbcc_loadfile(name, &sz);

	if(buf)
	{
		if(rsz)*rsz=sz;
		return(buf);
	}

	for(i=0; i<bgbcc_nsrc; i++)
	{
		sprintf(tb, "%s/%s", bgbcc_src[i], name);
		buf=bgbcc_loadfile(tb, &sz);
		if(buf)
		{
			if(rsz)*rsz=sz;
			return(buf);
		}
	}

	buf=BGBPP_LoadInclude(NULL, name, &sz);

	if(buf)
	{
		if(rsz)*rsz=sz;
		return(buf);
	}

	return(NULL);
}

int BGBCC_LoadCSourceBuffer(char *name, char *buf)
{
	char tb[256], tb1[256];
	VFILE *fd;
	FILE *fd2;
	BCCX_Node *t;
	byte *obj;
	char *s, *mod, *buf1, *buf2;
	int i, sz, t0, t1, dt;

//	BGBGC_InitSig();
	dyllMetaInit();

	if(!buf) return(-1);

#if 0
	if(!name)
	{
//		name=BASM_GenSym();

		s=buf; i=0;
		while(*s)i=(i*8191)+(*s++);
		sprintf(tb, "TMP_%08X", i);
		name=dystring(tb);
	}
#endif

	mod=NULL;
	if(name)
	{
		strcpy(tb, name);
		s=tb;
		while(*s)
		{
			if(*s=='/')*s='_';
			if(*s=='\\')*s='_';
			if(*s==':')*s='_';
			s++;
		}

		while((s>tb) && (*s!='.'))s--;
		if(s>tb)*s=0;

		mod=dystrdup(tb);
	}

	t0=clock();
	t=BGBCP_ModuleBuffer(name, mod, buf);

//	BGBGC_Collect();

	if(!t) return(-1);

#if 1
	if(mod)
	{
		sprintf(tb1, "dump/%s_ast.txt", mod);
		fd=vffopen(tb1, "wt");
		if(fd)
		{
			BCCX_PrintVF(fd, t);
			vfclose(fd);
		}
	}
#endif

#if 0
	if(mod)
	{
		sprintf(tb1, "dump/%s_ast.txt", mod);
		fd2=fopen(tb1, "wt");
		if(fd)
		{
			BCCX_PrintFD(fd2, t);
			fclose(fd2);
		}
	}
#endif

	t1=clock();
	dt=t1-t0;
	printf("BGBCC_LoadModule: Parse took %dms\n",
		(int)(1000*(dt/(float)CLOCKS_PER_SEC)));

//	BGBGC_Collect();

	buf1=BGBCC_CompileModule(tb, t);


	if(!buf1) return(-1);

	dyllMetaCommit();
//	BGBGC_SaveSig();
	BGBCC_CleanupAll();

#if 1
	if(mod)
	{
		sprintf(tb1, "dump/%s_ril.txt", mod);
		fd=vffopen(tb1, "wt");
		if(fd)
		{
			vfwrite(buf1, 1, strlen(buf1), fd);
			vfclose(fd);
		}
	}
#endif

//	SXIL_BeginModule(mod);
//	sxil_puts(buf1);
//	buf2=SXIL_EndModuleBuffer(NULL);

	if(!buf2) return(-1);

#if 1
	if(mod)
	{
		sprintf(tb1, "dump/%s_asm.txt", mod);
		fd=vffopen(tb1, "wt");
		if(fd)
		{
			vfwrite(buf2, 1, strlen(buf2), fd);
			vfclose(fd);
		}
	}
#endif

	BASM_BeginAssembly(mod);
	basm_puts(buf2);
	BASM_EndAssembly();

	dt=clock()-t1;
	printf("BGBCC_LoadModule: Compile took %dms\n",
		(int)(1000*(dt/(float)CLOCKS_PER_SEC)));

	return(0);
}

int BGBCC_LoadCSource(char *name, char *out)
{
	char tb[256], tb1[256];
	VFILE *fd;
	BCCX_Node *t;
	byte *obj;
	char *s, *buf, *buf1, *buf2;
	int i, sz, t0, t1, dt;

	if(BGBCC_CacheCheckFile(name))
	{
		strcpy(tb, name);
		s=tb;
		while(*s)
		{
			if(*s=='/')*s='_';
			if(*s=='\\')*s='_';
			if(*s==':')*s='_';
			s++;
		}

		while((s>tb) && (*s!='.'))s--;
		if(s>tb)*s=0;

		sprintf(tb1, "dump/cache/%s.o", tb);

		fd=vffopen(tb1, "rb");
		if(fd)
		{
			obj=vf_bufferin_sz(fd, &sz);
			vfclose(fd);

			if(obj)
			{
				i=BASM_LoadObjectBuffer(tb, obj, sz);
				gcfree(obj);
				if(i>=0)return(0);
			}
		}
	}

//	buf=bgbcc_loadfile(name, &sz);
	buf=BGBCC_ReadFile(name, &sz);

	if(!buf)
	{
		for(i=0; i<bgbcc_nsrc; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_src[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)break;
		}
	}

	if(!buf)
	{
		printf("BGBCC_LoadModule: fail load '%s'\n", name);
		return(-1);
	}

	printf("BGBCC_LoadModule: %s %d bytes\n", name, sz);

	strcpy(tb, name);
	s=tb;
	while(*s)
	{
		if(*s=='/')*s='_';
		if(*s=='\\')*s='_';
		if(*s==':')*s='_';
		s++;
	}

	while((s>tb) && (*s!='.'))s--;
	if(s>tb)*s=0;

//	printf("BGBCC_LoadModule: Begin Parse\n");

	t0=clock();
//	t=BS1_CParse_ModuleBuffer(name, tb, buf);
	t=BGBCP_ModuleBuffer(name, tb, buf);

	if(!t)
	{
//		free(buf);
		return(-1);
	}

//	BCCX_PrintFD(stdout, t);

#if 1
	sprintf(tb1, "dump/%s_ast.txt", tb);
	fd=vffopen(tb1, "wt");
	if(fd)
	{
		BCCX_PrintVF(fd, t);
		vfclose(fd);
	}
#endif

	t1=clock();
	dt=t1-t0;
	printf("BGBCC_LoadModule: Parse took %dms\n",
		(int)(1000*(dt/(float)CLOCKS_PER_SEC)));

	buf1=BGBCC_CompileModule(tb, t);

//	BGBGC_SaveSig();
	dyllMetaCommit();

	BGBCC_CleanupAll();

	if(!buf1)
	{
//		free(buf);
		return(-1);
	}

//	printf("%s\n", buf1);

#if 1
	sprintf(tb1, "dump/%s_ril.txt", tb);
	fd=vffopen(tb1, "wt");
	if(fd)
	{
		vfwrite(buf1, 1, strlen(buf1), fd);
		vfclose(fd);
	}
#endif

//	SXIL_BeginModule(tb);
//	sxil_puts(buf1);
//	buf2=SXIL_EndModuleBuffer(out);

	if(!buf2)
	{
//		free(buf);
		return(-1);
	}

//	printf("%s\n", buf2);

#if 1
	sprintf(tb1, "dump/%s_asm.txt", tb);
	fd=vffopen(tb1, "wt");
	if(fd)
	{
		vfwrite(buf2, 1, strlen(buf2), fd);
		vfclose(fd);
	}
#endif

	BASM_BeginAssembly(tb);
	basm_puts(buf2);
	if(out)
	{
		BASM_EndAssemblyFile(out);
	} else
	{
//		BASM_EndAssembly();
//		obj=BASM_EndAssemblyObjBuf(&sz);
//		BASM_LoadObjectBuffer(obj, sz);

		obj=BASM_EndAssemblyCacheObjBuf(&sz);

		sprintf(tb1, "dump/cache/%s.o", tb);
		fd=vffopen(tb1, "wb");
		if(fd)
		{
			vfwrite(obj, 1, sz, fd);
			vfclose(fd);

			BGBCC_UpdateCheckFile(name);
		}
	}

	dt=clock()-t1;
	printf("BGBCC_LoadModule: Compile took %dms\n",
		(int)(1000*(dt/(float)CLOCKS_PER_SEC)));


	if(!out)
		bgbcc_loads[bgbcc_nloads++]=basm_strdup(name);

//	free(buf);
	return(i);
}

int BGBCC_LoadCHeader(char *name)
{
	char tb[256], tb1[256];
	VFILE *fd;
	BCCX_Node *t;
	byte *obj;
	char *s, *buf, *buf1, *buf2;
	int i, sz, t0, t1, dt;

	if(BGBCC_CacheCheckFile(name)>0)
	{
		//header is up-to-date and known to metadata?...

//		s=DYLL_MetaPath_LookupKey(name);
		s=dyllMetaLookupKey(name);
		if(s && (s!=UNDEFINED) && !strcmp(s, "header"))
		{
			printf("BGBCC_LoadCHeader: '%s' is fresh\n", name);
			return(0);
		}

		printf("BGBCC_LoadCHeader: '%s' lacks metadata\n", name);
	}else
	{
		printf("BGBCC_LoadCHeader: '%s' is stale\n", name);
	}

	buf=bgbcc_loadfile(name, &sz);

	if(!buf)
	{
		for(i=0; i<bgbcc_nsrc; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_src[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)break;
		}
	}

	if(!buf)
	{
		buf=BGBPP_LoadInclude(NULL, name, &sz);
	}

	if(!buf)
	{
		printf("BGBCC_LoadCHeader: fail load '%s'\n", name);
		return(-1);
	}

	printf("BGBCC_LoadCHeader: %s %d bytes\n", name, sz);

	strcpy(tb, name);
	s=tb;
	while(*s)
	{
		if(*s=='/')*s='_';
		if(*s=='\\')*s='_';
		if(*s==':')*s='_';
		s++;
	}

	while((s>tb) && (*s!='.'))s--;
	if(s>tb)*s=0;

//	printf("BGBCC_LoadModule: Begin Parse\n");

	t0=clock();
	t=BGBCP_ModuleBuffer(name, tb, buf);

	if(!t)
	{
//		free(buf);
		return(-1);
	}

//	BCCX_PrintFD(stdout, t);

#if 1
	sprintf(tb1, "dump/%s_ast.txt", tb);
	fd=vffopen(tb1, "wt");
	if(fd)
	{
		BCCX_PrintVF(fd, t);
		vfclose(fd);
	}
#endif

	t1=clock();
	dt=t1-t0;
	printf("BGBCC_LoadModule: Parse took %dms\n",
		(int)(1000*(dt/(float)CLOCKS_PER_SEC)));

	buf1=BGBCC_CompileModule(tb, t);


//	DYLL_MetaPath_BindKey(name, "header");
	dyllMetaBindKey(name, "header");

//	BGBGC_SaveSig();
	dyllMetaCommit();

	BGBCC_CleanupAll();

	BGBCC_UpdateCheckFile(name);

	bgbcc_loads[bgbcc_nloads++]=basm_strdup(name);

//	free(buf1);
	return(0);
}

int BGBCC_LoadLibrary(char *name)
{
	char tb[256];
	char *s;
	int i, j;

	for(i=0; i<bgbcc_nloads; i++)
		if(!strcmp(bgbcc_loads[i], name))
			return(1);

	i=BLNK_LoadLibrary(name);

	if(i<0)
	{
		for(j=0; j<bgbcc_nlib; j++)
		{
			sprintf(tb, "%s/%s", bgbcc_lib[j], name);
			i=BLNK_LoadLibrary(tb);
			if(i>=0)break;
		}
	}

	if(i<0)return(-1);

	bgbcc_loads[bgbcc_nloads++]=
		basm_strdup(name);
	return(i);
}

int BGBCC_LoadLibrary2(char *name)
{
	char tb[256];
	int i;

	sprintf(tb, "%s.a", name);
	i=BGBCC_LoadLibrary(tb);
	if(i>=0)return(i);

	sprintf(tb, "lib%s.a", name);
	i=BGBCC_LoadLibrary(tb);
	if(i>=0)return(i);

#ifdef linux
	sprintf(tb, "%s.so", name);
	i=BGBCC_LoadLibrary(tb);
	if(i>=0)return(i);

	sprintf(tb, "lib%s.so", name);
	i=BGBCC_LoadLibrary(tb);
	if(i>=0)return(i);
#endif

#ifdef WIN32
	sprintf(tb, "%s.lib", name);
	i=BGBCC_LoadLibrary(tb);
	if(i>=0)return(i);

	sprintf(tb, "%s.dll", name);
	i=BGBCC_LoadLibrary(tb);
	if(i>=0)return(i);
#endif

	return(-1);
}

int BGBCC_LoadLibraryGeneric(char *name)
{
	int i;

	for(i=0; i<bgbcc_nloads; i++)
		if(!strcmp(bgbcc_loads[i], name))
			return(1);

	i=BGBCC_LoadLibrary2(name);
	if(i<0)return(-1);

	bgbcc_loads[bgbcc_nloads++]=
		basm_strdup(name);
	return(i);
}

int BGBCC_LoadModule(char *name)
{
	char *s;
	int i;

	//check if already loaded
	for(i=0; i<bgbcc_nloads; i++)
		if(!strcmp(bgbcc_loads[i], name))
			return(1);

	s=name+strlen(name);
	while((s>name) && (*s!='.'))s--;

	if(!strcmp(s, ".c") || !strcmp(s, ".C"))
	{
		i=BGBCC_LoadCSource(name, NULL);
		return(i);
	}

	if(!strcmp(s, ".h") || !strcmp(s, ".H"))
	{
		i=BGBCC_LoadCHeader(name);
		return(i);
	}

	if(!strcmp(s, ".a") || !strcmp(s, ".A") ||
		!strcmp(s, ".o") || !strcmp(s, ".O") ||
		!strcmp(s, ".obj") || !strcmp(s, ".OBJ") ||
		!strcmp(s, ".lib") || !strcmp(s, ".LIB") ||
		!strcmp(s, ".dll") || !strcmp(s, ".DLL") ||
		!strcmp(s, ".so") || !strcmp(s, ".SO"))
	{
		i=BGBCC_LoadLibrary(name);
		return(i);
	}

	if(!strcmp(s, ".txt") || !strcmp(s, ".TXT"))
	{
		i=BGBCC_LoadCompileScript(name);
		return(i);
	}

	return(-1);
}

int BGBCC_LoadModuleMulti(char *name, char **arg)
{
	char *buf;
	char *t;
	int i;

	buf=gcralloc(1024);

	t=buf;
	for(i=0; arg[i]; i++)
	{
		sprintf(t, "#include \"%s\"\n", arg[i]);
		t+=strlen(t);
	}

	sprintf(t, "\n\n");
	t+=strlen(t);

	i=BGBCC_LoadCSourceBuffer(name, buf);
	return(i);
}

int BGBCC_CompileFile(char *name, char *out)
{
	char *s;
	int i;

	s=name+strlen(name);
	while((s>name) && (*s!='.'))s--;

	if(!strcmp(s, ".c") || !strcmp(s, ".C"))
	{
		i=BGBCC_LoadCSource(name, out);
		return(i);
	}

	if(!strcmp(s, ".h") || !strcmp(s, ".H"))
	{
		i=BGBCC_LoadCSource(name, out);
		return(i);
	}

	return(-1);
}

int BGBCC_LoadCompileScript(char *name)
{
	char tb[1024];
	char *ta[64];
	char **a;
	char *s, *t, *buf, *base;
	int i, sz, nsrc, nlib;

	//check if already loaded
	for(i=0; i<bgbcc_nloads; i++)
		if(!strcmp(bgbcc_loads[i], name))
			return(1);

	s=name;
	buf=bgbcc_loadfile(name, &sz);

	if(!buf)
	{
		for(i=0; i<bgbcc_nsrc; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_src[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)
			{
				s=basm_strdup(name);
				break;
			}
		}
	}

	if(!buf)
	{
		printf("BGBCC_LoadCompileScript: fail load '%s'\n", name);
		return(-1);
	}

	printf("BGBCC_LoadCompileScript: %s %d bytes\n", name, sz);

	strcpy(tb, s);
	s=tb+strlen(tb);
	while((s>tb) && (*s!='/') && (*s!='\\'))s--;
	if(*s=='/')s++;
	*s=0;

	base=basm_strdup(tb);
	nsrc=bgbcc_nsrc;
	nlib=bgbcc_nlib;

	s=buf;
	while(*s)
	{
		while(*s && (*s<=' '))s++;
		t=tb;
		while(*s)
		{
			while(*s && *s!='\n')*t++=*s++;
			*t++=0;

			while((t>tb) && (*t<=' '))t--;
				if((t>tb) && (*t=='\\'))
					{ *t=0; continue; }
			break;
		}

		if(!tb[0])continue;
		if(tb[0]=='/')continue;
		if(tb[0]==';')continue;
		if(tb[0]=='#')continue;

//		a=sxil_split(tb);
		a=gcrsplit(tb);
		if(!a[0])continue;

		if(!strcmp(a[0], "include"))
		{
			for(i=1; a[i]; i++)
				BGBPP_AddIncludePathFront(a[i]);
			continue;
		}

		if(!strcmp(a[0], "library"))
		{
			for(i=1; a[i]; i++)
				BGBCC_LoadLibraryGeneric(a[i]);
			continue;
		}

		if(!strcmp(a[0], "module"))
		{
			for(i=1; a[i]; i++)
			{
				sprintf(tb, "%s/%s", base, a[i]);
				BGBCC_LoadModule(tb);
			}
			continue;
		}

		if(!strcmp(a[0], "multi"))
		{
			for(i=0; a[i+2]; i++)
			{
				sprintf(tb, "%s/%s", base, a[i]);
				ta[i]=gcrstrdup(tb);
			}
			ta[i]=NULL;

			BGBCC_LoadModuleMulti(a[1], ta);
			continue;
		}
	}

	return(0);
}



BCCX_Node *BGBCC_LoadCSourceGast(char *name)
{
	char tb[256], tb1[256];
	FILE *fd;
	BCCX_Node *t;
	byte *obj;
	char *s, *buf, *buf1, *buf2, *mod;
	int i, sz, t0, t1, dt;

	buf=bgbcc_loadfile(name, &sz);

	if(!buf)
	{
		for(i=0; i<bgbcc_nsrc; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_src[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)break;
		}
	}

	if(!buf)
	{
		buf=BGBPP_LoadInclude(NULL, name, &sz);
	}

	if(!buf)
	{
		printf("BGBCC_LoadC_GastSBXE: fail load '%s'\n", name);
		return(NULL);
	}

	printf("BGBCC_LoadC_GastSBXE: %s %d bytes\n", name, sz);

	strcpy(tb, name);
	s=tb;
	while(*s)
	{
		if(*s=='/')*s='_';
		if(*s=='\\')*s='_';
		if(*s==':')*s='_';
		s++;
	}

	while((s>tb) && (*s!='.'))s--;
	if(s>tb)*s=0;

	mod=bgbcc_strdup(tb);


	t0=clock();
	t=BGBCP_ModuleBuffer(name, mod, buf);

	if(!t)return(NULL);

#if 1
	if(mod)
	{
		sprintf(tb1, "dump/%s_ast.txt", mod);
		fd=fopen(tb1, "wt");
		if(fd)
		{
			BCCX_PrintFD(fd, t);
			fclose(fd);
		}

#if 0
		buf1=malloc(1<<24);
		i=SBXE_WriteNodeList(buf1, t);
		printf("SBXE Encoded %d bytes\n", i);

		sprintf(tb1, "dump/%s_ast.sbxe", mod);
		fd=fopen(tb1, "wb");
		if(fd && (i>0))
		{
			fwrite(buf1, 1, i, fd);
			fclose(fd);
		}
		free(buf1);
#endif
	}
#endif

//	i=SBXE_WriteNodeList(obuf, t);
//	printf("SBXE Encoded %d bytes\n", i);
//	if(*rsz)*rsz=i;

//	bgbcc_loads[bgbcc_nloads++]=strdup(name);
	return(t);
}

int BGBCC_LoadCSourceGastSBXE(char *name, byte *obuf, int *rsz)
{
	BCCX_Node *t;
	int i;

	t=BGBCC_LoadCSourceGast(name);
	if(!t)return(-1);

	i=SBXE_WriteNodeList(obuf, t);
	printf("SBXE Encoded %d bytes\n", i);
	if(*rsz)*rsz=i;
	return(0);
}

dyxNode *BCCX_dyxClone(BCCX_Node *n) //AH:ignore
{
	dyxNode *t, *t1;
	BCCX_Node *c;
	BCCX_Attr *ac;

	if(!n)return(NULL);

	t=gctalloc("_dyxnode_t", sizeof(dyxNode));
	t->ns=dystrdup(n->ns);
	t->tag=dystrdup(n->tag);
	t->text=dystrdup(n->text);

	ac=n->attr;
	while(ac)
	{
		dyxSetNS(t, ac->ns, ac->var, ac->val);
		ac=ac->next;
	}

	c=n->down;
	while(c)
	{
		t1=BCCX_dyxClone(c);
		dyxAdd(t, t1);
		c=c->next;
	}

	return(t);	
}

int BGBCC_LoadCSourceBSVM(char *name)
{
	BCCX_Node *t;
	dyxNode *tn;
	byte *tbuf;
	int i, sz;
	
//	tbuf=malloc(1<<20);
//	i=BGBCC_LoadCSourceGastSBXE(name, tbuf, &sz);
//	if(i<0) { free(tbuf); return(-1); }
//	BSVM_EvalGastSBXE(tbuf, sz, NULL);

	t=BGBCC_LoadCSourceGast(name);
	tn=BCCX_dyxClone(t);
//	BSVM_EvalGast(tn, NULL);

	return(0);
}

int BGBCC_LoadModuleBSVM(char *name)
{
	char *s;
	int i;

	//check if already loaded
	for(i=0; i<bgbcc_nloads; i++)
		if(!strcmp(bgbcc_loads[i], name))
			return(1);

	s=name+strlen(name);
	while((s>name) && (*s!='.'))s--;

	if(!strcmp(s, ".c") || !strcmp(s, ".C"))
	{
		i=BGBCC_LoadCSourceBSVM(name);
		return(i);
	}

	if(!strcmp(s, ".h") || !strcmp(s, ".H"))
	{
		i=BGBCC_LoadCSourceBSVM(name);
		return(i);
	}

	return(-1);
}


#if 1
BCCX_Node *BGBCC_LoadCSourceAST(char *name)
{
	char tb[256], tb1[256];
	FILE *fd;
	BCCX_Node *t;
	byte *obj;
	char *s, *buf, *buf1, *buf2, *mod;
	int i, sz, t0, t1, dt;

	buf=bgbcc_loadfile(name, &sz);

	if(!buf)
	{
		for(i=0; i<bgbcc_nsrc; i++)
		{
			sprintf(tb, "%s/%s", bgbcc_src[i], name);
			buf=bgbcc_loadfile(tb, &sz);
			if(buf)break;
		}
	}

	if(!buf)
	{
		buf=BGBPP_LoadInclude(NULL, name, &sz);
	}

	if(!buf)
	{
		printf("BGBCC_LoadC_GastSBXE: fail load '%s'\n", name);
		return(NULL);
	}

	printf("BGBCC_LoadC_GastSBXE: %s %d bytes\n", name, sz);

	strcpy(tb, name);
	s=tb;
	while(*s)
	{
		if(*s=='/')*s='_';
		if(*s=='\\')*s='_';
		if(*s==':')*s='_';
		s++;
	}

	while((s>tb) && (*s!='.'))s--;
	if(s>tb)*s=0;

	mod=bgbcc_strdup(tb);

	t0=clock();
	t=BGBCP_ModuleBuffer(name, mod, buf);

	if(!t)return(NULL);

#if 1
	if(mod)
	{
		sprintf(tb1, "dump/%s_ast.txt", mod);
		fd=fopen(tb1, "wt");
		if(fd)
		{
			BCCX_PrintFD(fd, t);
			fclose(fd);
		}
	}
#endif
	return(t);
}

int BGBCC_LoadCSourceFRBC(char *name, byte *obuf, int *rsz)
{
	BGBCC_FrCC_State *ctx;
	BCCX_Node *t;
	int i, sz;

	t=BGBCC_LoadCSourceAST(name);
	if(!t)return(-1);

	BGBCC_FrCC_CompileModuleCTX(ctx, name, t);
	sz=BGBCC_FrBC_FlattenImage(ctx, obuf, 1<<20);
	if(sz<0)return(-1);
	if(*rsz)*rsz=sz;

//	i=SBXE_WriteNodeList(obuf, t);
//	printf("SBXE Encoded %d bytes\n", i);
//	if(*rsz)*rsz=i;
	return(0);
}
#endif

#if 0
int BGBCC_InitEnv(int argc, char **argv, char **env)
{
	return(BGBCC_Init());
}

int BGBCC_Init()
{
	char buf[256];
	static int init=0;
	int i;
	char *s;

	if(init)return(-1);
	init=1;

	BGBGC_InitDyLL();
	BASM_Init();
//	SXIL_Init();

//	bgbpp_loadfile_fp=(char*(*)(char*,int*))basm_loadfile;

#ifdef linux
	BGBPP_AddIncludePathFront("/usr/include");
	BGBPP_AddIncludePathFront("/usr/local/include");
//	inc_ok=1;
#else
	BGBPP_AddIncludePathFront("cc_usr/include");
	BGBPP_AddIncludePathFront("cc_usr/bscc/include");
#endif

	BGBPP_AddIncludePathFront("build/include");
	BGBPP_AddIncludePathFront("include");
//	BGBPP_AddIncludePathFront("");

//	if(SXIL_GetSSE()>=2)
//		BGBCC_LoadModule("cc_usr/bscc/dlib/vecnf.c");

#ifdef _WIN32
	BGBPP_AddStaticDefine(NULL, "_WIN32", "");
#endif

#ifdef linux
	BGBPP_AddStaticDefine(NULL, "linux", "");
#endif

#ifdef __i386__
	BGBPP_AddStaticDefine(NULL, "__i386__", "");
	bgbcc_arch=BGBCC_ARCH_X86;
#endif
#ifdef __x86_64__
	BGBPP_AddStaticDefine(NULL, "__x86_64__", "");
	bgbcc_arch=BGBCC_ARCH_X64;
#endif

#ifdef _M_IX86
	BGBPP_AddStaticDefine(NULL, "_M_IX86", "600");
	bgbcc_arch=BGBCC_ARCH_X86;
#endif
#ifdef _M_X64
	BGBPP_AddStaticDefine(NULL, "_M_X64", "");
	bgbcc_arch=BGBCC_ARCH_X64;
#endif

	BGBPP_AddStaticDefine(NULL, "_BGBMETA", "");

	BGBPP_AddStaticDefine(NULL, "__inline__", "");

	BGBPP_AddStaticDefine(NULL, "__GNUC__", "1");

	BGBPP_AddStaticDefine(NULL, "POINTER_32", "");
	BGBPP_AddStaticDefine(NULL, "POINTER_64", "");

	BGBPP_AddStaticDefine(NULL, "WINGDIAPI", "__declspec(dllimport)");
	BGBPP_AddStaticDefine(NULL, "CALLBACK", "");

#ifdef _M_IX86
	BGBPP_AddStaticDefine(NULL, "APIENTRY", "__stdcall");
#else
	BGBPP_AddStaticDefine(NULL, "APIENTRY", "");
#endif

#if 1
#ifdef _MSC_VER
	sprintf(buf, "%d", _MSC_VER);
	BGBPP_AddStaticDefine(NULL, "_MSC_VER", buf);
#endif

#ifdef _MSC_FULL_VER
	sprintf(buf, "%d", _MSC_FULL_VER);
	BGBPP_AddStaticDefine(NULL, "_MSC_FULL_VER", buf);
#endif
#endif

#ifdef _INTEGRAL_MAX_BITS
	sprintf(buf, "%d", _INTEGRAL_MAX_BITS);
	BGBPP_AddStaticDefine(NULL, "_INTEGRAL_MAX_BITS", buf);
#endif

	BGBPP_AddStaticDefine(NULL, "__STDC__", "1");
	BGBPP_AddStaticDefine(NULL, "__STDC_HOSTED__", "1");
	BGBPP_AddStaticDefine(NULL, "__STDC_VERSION__", "199901");


	bgbcc_mutex=thMutex();

	return(0);
}
#endif

#if 1
int BGBCC_InitEnv(int argc, char **argv, char **env)
{
	char *ta[16];
	char buf[256];
	static int init=0;
	int inc_ok;
	int i, m;
	char *s, *t;
	char *mach_name, *gcc_ver;

	if(init)return(-1);
	init=1;

//	BASM_Init();
	BGBGC_InitDyLL();
//	SXIL_Init();

	bgbcc_mutex=thMutex();

	mach_name=NULL;
	gcc_ver=NULL;
#ifdef __GNUC__
	sprintf(buf, "%d.%d.%d",
		__GNUC__,  __GNUC_MINOR__,  __GNUC_PATCHLEVEL__);
	gcc_ver=strdup(buf);
#endif

	m=0;
	for(i=1; argv && i<argc; i++)
	{
		if(argv[i][0]=='/')
		{
			if(!strncmp(argv[i], "/FZx", 4))
			{
				m|=16;
				continue;
			}
			if(!strncmp(argv[i], "/FZy", 4))
			{
				m|=32;
				continue;
			}

			if(!strncmp(argv[i], "/I", 2))
			{
				ccAddInclude(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "/L", 2))
			{
				ccAddLibrary(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "/S", 2))
			{
				ccAddSource(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i], "/D", 2))
			{
				ccAddDefineString(argv[i]+2);
				continue;
			}

			continue;
		}

		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "--"))
				{ i++; break; }
			if(!strcmp(argv[i], "--pc"))
				m|=1;

			if(!strncmp(argv[i], "-I", 2))
			{
				ccAddInclude(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "-L", 2))
			{
				ccAddLibrary(argv[i]+2);
				continue;
			}
			if(!strncmp(argv[i], "-S", 2))
			{
				ccAddSource(argv[i]+2);
				continue;
			}

			if(!strncmp(argv[i], "-D", 2))
			{
				ccAddDefineString(argv[i]+2);
				continue;
			}

			continue;
		}
	}

	inc_ok=0;
	for(i=0; env && env[i]; i++)
	{
//		printf("ENV %s\n", env[i]);
#if 1
		if(!(m&32) && !strnicmp(env[i], "Include=", strlen("Include=")))
//		if(0)
		{
			s=env[i]+strlen("Include=");
			while(*s)
			{
				while(*s==' ')s++;

				t=buf;
				while(*s && (*s!=';'))
					*t++=*s++;
				*t++=0;

				while(*s==';')s++;

				if(buf[0])
				{
					ccAddInclude(buf);
					inc_ok=1;
				}
			}
		}
#endif

		if(!strnicmp(env[i], "MACHTYPE=", strlen("MACHTYPE=")))
		{
			s=env[i]+strlen("MACHTYPE=");
			mach_name=s;
		}
	}

#ifdef linux
	if(gcc_ver && mach_name)
	{
		sprintf(buf, "/usr/lib/gcc/%s/%s/include", mach_name, gcc_ver);
		BGBPP_AddIncludePathFront(buf);
	}

	if(mach_name)
	{
		sprintf(buf, "/usr/%s/include", mach_name);
		BGBPP_AddIncludePathFront(buf);
	}

	BGBPP_AddIncludePathFront("/usr/include");
	BGBPP_AddIncludePathFront("/usr/local/include");
	inc_ok=1;
#endif

	if(!inc_ok)
	{
		BGBPP_AddIncludePathFront("cc_usr/include");
		BGBPP_AddIncludePathFront("cc_usr/bscc/include");
//		BGBPP_AddIncludePathFront("build/include");
//		BGBPP_AddIncludePathFront("include");
//		BGBPP_AddIncludePathFront("");
	}

#ifdef _WIN32
	BGBPP_AddStaticDefine(NULL, "_WIN32", "");
//	BGBPP_AddStaticDefine(NULL, "HWND", "void *");
#endif

#ifdef linux
	BGBPP_AddStaticDefine(NULL, "linux", "");
#endif

#ifdef __i386__
	BGBPP_AddStaticDefine(NULL, "__i386__", "");
	bgbcc_arch=BGBCC_ARCH_X86;
#endif
#ifdef __x86_64__
	BGBPP_AddStaticDefine(NULL, "__x86_64__", "");
	bgbcc_arch=BGBCC_ARCH_X64;
#endif

#ifdef _M_IX86
//	BGBPP_AddStaticDefine(NULL, "_X86_", "");
	BGBPP_AddStaticDefine(NULL, "_M_IX86", "600");
	bgbcc_arch=BGBCC_ARCH_X86;
#endif
#ifdef _M_X64
	BGBPP_AddStaticDefine(NULL, "_M_X64", "");
	bgbcc_arch=BGBCC_ARCH_X64;
#endif

	BGBPP_AddStaticDefine(NULL, "_BGBMETA", "");

	BGBPP_AddStaticDefine(NULL, "__inline__", "");

	BGBPP_AddStaticDefine(NULL, "__GNUC__", "1");

//	ta[0]="v"; ta[1]=NULL;
//	BGBPP_AddStaticDefineArgs(NULL, "__declspec", ta, "");

//	ta[0]="v"; ta[1]=NULL;
//	BGBPP_AddStaticDefineArgs(NULL, "DECLARE_HANDLE", ta, "typedef void *v");

	BGBPP_AddStaticDefine(NULL, "POINTER_32", "");
	BGBPP_AddStaticDefine(NULL, "POINTER_64", "");

	BGBPP_AddStaticDefine(NULL, "WINGDIAPI", "__declspec(dllimport)");
	BGBPP_AddStaticDefine(NULL, "CALLBACK", "");

#ifdef _M_IX86
	BGBPP_AddStaticDefine(NULL, "APIENTRY", "__stdcall");
#else
	BGBPP_AddStaticDefine(NULL, "APIENTRY", "");
#endif

#if 1
#ifdef _MSC_VER
	sprintf(buf, "%d", _MSC_VER);
	BGBPP_AddStaticDefine(NULL, "_MSC_VER", buf);
#endif

#ifdef _MSC_FULL_VER
	sprintf(buf, "%d", _MSC_FULL_VER);
	BGBPP_AddStaticDefine(NULL, "_MSC_FULL_VER", buf);
#endif
#endif

#ifdef _INTEGRAL_MAX_BITS
	sprintf(buf, "%d", _INTEGRAL_MAX_BITS);
	BGBPP_AddStaticDefine(NULL, "_INTEGRAL_MAX_BITS", buf);
#endif

//	BGBPP_AddStaticDefine(NULL, "__midl", "0");

#if 0
	BGBPP_AddStaticDefine(NULL, "DBG", "0");
	BGBPP_AddStaticDefine(NULL, "_DBG_MEMCPY_INLINE_", "0");
	BGBPP_AddStaticDefine(NULL, "_WIN32_WCE", "0x0600");
	BGBPP_AddStaticDefine(NULL, "WINXP", "0x06000000");
	BGBPP_AddStaticDefine(NULL, "NTDDI_WXP", "0x06000000");
	BGBPP_AddStaticDefine(NULL, "NTDDI_XPSP2", "NTDDI_WINXPSP2");
	BGBPP_AddStaticDefine(NULL, "NTDDI_WIN2003", "NTDDI_WS03");
	
	BGBPP_AddStaticDefine(NULL, "NTDDI_WINLH", "NTDDI_VERSION");
	BGBPP_AddStaticDefine(NULL, "NTDDK_VERSION", "NTDDI_VERSION");
#endif

	BGBPP_AddStaticDefine(NULL, "__STDC__", "1");
	BGBPP_AddStaticDefine(NULL, "__STDC_HOSTED__", "1");
	BGBPP_AddStaticDefine(NULL, "__STDC_VERSION__", "199901");

	return(0);
}

int BGBCC_Init()
	{ return(BGBCC_InitEnv(0, NULL, NULL)); }
#endif

BGBCC_API void ccBindKey(char *key, char *val)
{
//	DYLL_MetaPath_BindKey(key, val);
	dyllMetaBindKey(key, val);
}

BGBCC_API char *ccLookupKey(char *key)
{
	char *s;
//	s=DYLL_MetaPath_LookupKey(key);
	s=dyllMetaLookupKey(key);
	if(s==UNDEFINED)return(NULL);
	return(s);
}

BGBCC_API void ccAddInclude(char *path)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBPP_AddIncludePathFront(path);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API void ccAddLibrary(char *path)
{
	int i;

	BGBCC_Init();

	for(i=0; i<bgbcc_nlib; i++)
		if(!strcmp(bgbcc_lib[i], path))
			return;

	thLockMutex(bgbcc_mutex);
	i=bgbcc_nlib++;
	bgbcc_lib[i]=basm_strdup(path);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API void ccAddSource(char *path)
{
	int i;

	BGBCC_Init();

	for(i=0; i<bgbcc_nsrc; i++)
		if(!strcmp(bgbcc_src[i], path))
			return;

	thLockMutex(bgbcc_mutex);
	i=bgbcc_nsrc++;
	bgbcc_src[i]=basm_strdup(path);
	thUnlockMutex(bgbcc_mutex);
}

void ccAddDefineString(char *str)
{
	char tb[256], tb2[256];
	char *s, *t;
	
	s=str; t=tb;
	while(*s && (*s!='='))
		*t++=*s++;
	*t++=0;

	if(*s=='=')s++;

	t=tb2;
	while(*s)
		*t++=*s++;
	*t++=0;

	BGBPP_AddStaticDefine(NULL, tb, tb2);
}

BGBCC_API int ccLoadModule(char *file)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBCC_LoadModule(file);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API int ccLoadModuleMulti(char *name, char **args)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBCC_LoadModuleMulti(name, args);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API int ccCompileModule(char *file, char *out)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBCC_CompileFile(file, out);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API int ccLoadLibrary(char *name)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBCC_LoadLibraryGeneric(name);
	thUnlockMutex(bgbcc_mutex);
}


BGBCC_API int ccPrecompileModule(char *file)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
//	BGBCC_PrecompileModule(file);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API int ccLoadCompilerScript(char *file)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBCC_LoadCompileScript(file);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API int ccLoadModuleBSVM(char *file)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBCC_LoadModuleBSVM(file);
	thUnlockMutex(bgbcc_mutex);
}

BGBCC_API int ccSubmitCSource(char *name, char *buf)
{
	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	BGBCC_LoadCSourceBuffer(name, buf);
	thUnlockMutex(bgbcc_mutex);
}


BGBCC_API void *ccGetAddr(char *sym)
{
	void *p;

	BGBCC_Init();

	thLockMutex(bgbcc_mutex);
	p=BASM_GetPtr(sym);
	thUnlockMutex(bgbcc_mutex);

	printf("ccGetAddr: %p\n", p);
	return(p);
}

BGBCC_API void ccSetAddr(char *sym, void *ptr)
{
	char buf[256];
	void *p;

	BGBCC_Init();
	thLockMutex(bgbcc_mutex);
//	BLNK_AssignSym(sym, ptr);
	dyllSetAddr(sym, ptr);
	thUnlockMutex(bgbcc_mutex);
}


BGBCC_API void *ccGetPtr(char *sym)
{
	void *p; p=ccGetAddr(sym);
	if(p)return(*(void **)p);
	return(NULL);
}

BGBCC_API void ccSetPtr(char *sym, void *v)
{
	void *p; p=ccGetAddr(sym);
	if(p)*(void **)p=v;
}

BGBCC_API int ccGetInt(char *sym)
{
	void *p; p=ccGetAddr(sym);
	if(p)return(*(int *)p);
	return(0);
}

BGBCC_API void ccSetInt(char *sym, int v)
{
	void *p; p=ccGetAddr(sym);
	if(p)*(int *)p=v;
}

BGBCC_API s64 ccGetLongLong(char *sym)
{
	void *p; p=ccGetAddr(sym);
	if(p)return(*(s64 *)p);
	return(0);
}

BGBCC_API void ccSetLongLong(char *sym, s64 v)
{
	void *p; p=ccGetAddr(sym);
	if(p)*(s64 *)p=v;
}

BGBCC_API float ccGetFloat(char *sym)
{
	void *p; p=ccGetAddr(sym);
	if(p)return(*(float *)p);
	return(0);
}

BGBCC_API void ccSetFloat(char *sym, float v)
{
	void *p; p=ccGetAddr(sym);
	if(p)*(float *)p=v;
}

BGBCC_API double ccGetDouble(char *sym)
{
	void *p; p=ccGetAddr(sym);
	if(p)return(*(double *)p);
	return(0);
}

BGBCC_API void ccSetDouble(char *sym, double v)
{
	void *p; p=ccGetAddr(sym);
	if(p)*(double *)p=v;
}
