#include <bgbccc.h>
#include <time.h>

// extern BCCX_Node *bgbcp_structs;
// extern BCCX_Node *bgbcp_types;

extern int bgbpp_warn, bgbpp_err;
extern int bgbcp_warn, bgbcp_err;


BCCX_Node *BGBCP_FunArgs(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s;
	int ty, ty2;
	BCCX_Node *n, *n1, *lst, *lste;

	s=*str; lst=NULL; lste=NULL;
	while(s && *s)
	{
		b[0]=0;
		BGBCP_Token(s, b, &ty);
		if(ty==BTK_NULL)break;		
		if((ty==BTK_BRACE) &&
			(!strcmp(b, ")") || !strcmp(b, "]") ||
			!strcmp(b, "}")))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if((ty==BTK_SEPERATOR) && !strcmp(b, ","))
		{
			s=BGBCP_Token(s, b, &ty);
			continue;
		}

		n=BGBCP_Expression(ctx, &s);
//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

		BGBCP_Token(s, b, &ty);
		if((ty==BTK_BRACE) &&
			(!strcmp(b, ")") || !strcmp(b, "]") ||
			!strcmp(b, "}")))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if(strcmp(b, ","))
		{
			BGBCP_Error(s, "BGBCP_FunArgs: "
				"Invalid Token '%s'\n", b);
//			*(int *)(-1)=-1;
		}

		s=BGBCP_Token(s, b, &ty);	//','
	}

	*str=s;
	return(lst);
}

BCCX_Node *BGBCP_GenArgs(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s;
	int ty, ty2;
	BCCX_Node *n, *n1, *lst, *lste;

	s=*str; lst=NULL; lste=NULL;
	while(1)
	{
		b[0]=0;
		BGBCP_TokenGen(s, b, &ty);
		if(ty==BTK_NULL)break;
		if((ty==BTK_BRACE) && !strcmp(b, ">"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		n=BGBCP_Expression(ctx, &s);
//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

		BGBCP_TokenGen(s, b, &ty);
		if((ty==BTK_BRACE) && !strcmp(b, ">"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if(strcmp(b, ","))
		{
			BGBCP_Error(s, "BGBCP_FunArgs: "
				"Invalid Token '%s'\n", b);
//			*(int *)(-1)=-1;
		}

		s=BGBCP_Token(s, b, &ty);	//','
	}

	*str=s;
	return(lst);
}


#if 0
BCCX_Node *BGBCP_ModuleBuffer(char *name, char *modname, char *buf)
{
	char b[256];
	char *tbuf, *s;
	BCCX_Node *t;
	int t0, t1, dt;

	tbuf=malloc(1<<24);
	memset(tbuf, 0, 1<<24);

	t0=clock();

	BGBCP_PushLinenum();
	BGBCP_SetLinenum(name, buf, 1);

	BGBPP_Filter(buf, tbuf);

	BGBCP_PopLinenum();

	t1=clock();
	dt=(1000.0*(t1-t0))/CLOCKS_PER_SEC;
	printf("PreProc took %dms\n", dt);

#if 1
	if(bgbpp_err)
	{
		sprintf(b, "dump/%s_pp_err.txt", modname);
		basm_storetextfile(b, tbuf);

		return(BS1_MM_NULL);
	}

//	printf("PP\n%s\n\n", tbuf);

	sprintf(b, "dump/%s_pp.txt", modname);
	basm_storetextfile(b, tbuf);
#endif


	t0=clock();

	bgbcp_warn=0;
	bgbcp_err=0;

	SET(ctx->structs, BS1_MM_NULL);
	SET(ctx->types, BS1_MM_NULL);

	BGBCP_SetLinenum(name, tbuf, 1);
	s=tbuf;
	t=BGBCP_Block(ctx, &s);

	t=CONS(LIST2S("mod-types", ctx->types), t);
	t=CONS(LIST2S("mod-structs", ctx->structs), t);

	t1=clock();
	dt=(1000.0*(t1-t0))/CLOCKS_PER_SEC;
	printf("Parse took %dms\n", dt);

#if 1
	memset(tbuf, 0, 1<<24);

//	BS1_TY_PrintLN(t);
//	BS1_S_PrintFmt(t);
	BS1_S_Print(tbuf, t, 0);
//	BS1_S_Print(tbuf, t, -1);

	if(bgbcp_err)
	{
		sprintf(b, "dump/%s_ast_err.txt", modname);
		basm_storetextfile(b, tbuf);
		free(tbuf);

		return(BS1_MM_NULL);
	}

	sprintf(b, "dump/%s_ast.txt", modname);
	basm_storetextfile(b, tbuf);
#endif

	free(tbuf);

	return(t);
}

#endif

int BGBCP_LangForName(char *name)
{
	int lang;
	char *s;

	lang=0;

	if(name)
	{
		if(!strcmp(name, "C"))lang=BGBCC_LANG_C;
		if(!strcmp(name, "CPP"))lang=BGBCC_LANG_CPP;
		if(!strcmp(name, "C++"))lang=BGBCC_LANG_CPP;
		if(!strcmp(name, "C#"))lang=BGBCC_LANG_CS;
		if(!strcmp(name, "CS"))lang=BGBCC_LANG_CS;
		if(!strcmp(name, "BS"))lang=BGBCC_LANG_BS;
		if(!strcmp(name, "BS2"))lang=BGBCC_LANG_BS2;
		if(!strcmp(name, "Java"))lang=BGBCC_LANG_JAVA;
		if(lang)return(lang);

		if(!strcmp(name, "c"))lang=BGBCC_LANG_C;
		if(!strcmp(name, "cpp"))lang=BGBCC_LANG_CPP;
		if(!strcmp(name, "c++"))lang=BGBCC_LANG_CPP;
		if(!strcmp(name, "c#"))lang=BGBCC_LANG_CS;
		if(!strcmp(name, "cs"))lang=BGBCC_LANG_CS;
		if(!strcmp(name, "bs"))lang=BGBCC_LANG_BS;
		if(!strcmp(name, "bs2"))lang=BGBCC_LANG_BS2;
		if(!strcmp(name, "java"))lang=BGBCC_LANG_JAVA;
		if(lang)return(lang);

		s=name+strlen(name);
		while((s>name) && (*s!='.'))s--;

		if(!strcmp(s, ".c"))lang=BGBCC_LANG_C;
		if(!strcmp(s, ".h"))lang=BGBCC_LANG_C;

		if(!strcmp(s, ".cpp"))lang=BGBCC_LANG_CPP;
		if(!strcmp(s, ".hpp"))lang=BGBCC_LANG_CPP;
		if(!strcmp(s, ".cs"))lang=BGBCC_LANG_CS;
		if(!strcmp(s, ".java"))lang=BGBCC_LANG_JAVA;

		if(!strcmp(s, ".bs"))lang=BGBCC_LANG_BS;
		if(!strcmp(s, ".js"))lang=BGBCC_LANG_BS;
		if(!strcmp(s, ".es"))lang=BGBCC_LANG_BS;
		if(!strcmp(s, ".bs2"))lang=BGBCC_LANG_BS2;

		if(!strcmp(s, ".C"))lang=BGBCC_LANG_C;
		if(!strcmp(s, ".H"))lang=BGBCC_LANG_C;

		if(!strcmp(s, ".CPP"))lang=BGBCC_LANG_CPP;
		if(!strcmp(s, ".HPP"))lang=BGBCC_LANG_CPP;
		if(!strcmp(s, ".CS"))lang=BGBCC_LANG_CS;
		if(!strcmp(s, ".JAVA"))lang=BGBCC_LANG_JAVA;

		if(!strcmp(s, ".BS"))lang=BGBCC_LANG_BS;
		if(!strcmp(s, ".JS"))lang=BGBCC_LANG_BS;
		if(!strcmp(s, ".ES"))lang=BGBCC_LANG_BS;
		if(!strcmp(s, ".BS2"))lang=BGBCC_LANG_BS2;
	}
	
	return(lang);
}

char *BGBCP_NameForLang(int lang)
{
	char *s;
	
	switch(lang)
	{
	case BGBCC_LANG_C: s="C"; break;
	case BGBCC_LANG_CPP: s="C++"; break;
	case BGBCC_LANG_JAVA: s="Java"; break;
	case BGBCC_LANG_CS: s="C#"; break;
	case BGBCC_LANG_CX: s="CX"; break;
	case BGBCC_LANG_BS: s="BGBScript"; break;
	case BGBCC_LANG_BS2: s="BGBScript2"; break;

	default: s="unknown"; break;
	}
	return(s);
}

int BGBCP_ArchForName(char *name)
{
	int i;
	char *s;

	if(!name)return(0);
	
	i=0;

	if(!strcmp(name, "any"))		i=BGBCC_ARCH_DEFAULT;
	if(!strcmp(name, "default"))	i=BGBCC_ARCH_DEFAULT;

	if(!strcmp(name, "x86"))	i=BGBCC_ARCH_X86;
	if(!strcmp(name, "X86"))	i=BGBCC_ARCH_X86;

	if(!strcmp(name, "x64"))	i=BGBCC_ARCH_X64;
	if(!strcmp(name, "x86-64"))	i=BGBCC_ARCH_X64;
	if(!strcmp(name, "x86_64"))	i=BGBCC_ARCH_X64;
	if(!strcmp(name, "X64"))	i=BGBCC_ARCH_X64;
	if(!strcmp(name, "AMD64"))	i=BGBCC_ARCH_X64;

	if(!strcmp(name, "ARM"))	i=BGBCC_ARCH_ARM;
	if(!strcmp(name, "PPC"))	i=BGBCC_ARCH_PPC;
	if(!strcmp(name, "PPC64"))	i=BGBCC_ARCH_PPC64;

	return(i);
}

char *BGBCP_NameForArch(int arch)
{
	char *s;
	
	switch(arch)
	{
	case BGBCC_ARCH_DEFAULT: s="default"; break;
	case BGBCC_ARCH_X86: s="x86"; break;
	case BGBCC_ARCH_X64: s="x64"; break;
	case BGBCC_ARCH_ARM: s="ARM"; break;
	case BGBCC_ARCH_PPC: s="PPC"; break;

	default: s="unknown"; break;
	}
	return(s);
}

BCCX_Node *BGBCP_ModuleBuffer(char *name, char *modname, char *buf)
{
	char b[256];
//	VFILE *fd;
	BGBCP_ParseState *ctx;
	char *tbuf, *s;
	BCCX_Node *n, *n1, *n2;
	int t0, t1, dt, lang, pprs;
	int i, j, k;

	lang=BGBCP_LangForName(name);
	if(!lang)return(NULL);

//	if((lang==BGBCC_LANG_CS) || (lang==BGBCC_LANG_JAVA) ||
//		(lang==BGBCC_LANG_BS) ||
//		(lang==BGBCC_LANG_BS2))
//	{
//		n=BGBJP_ModuleBuffer(name, modname, buf);
//		return(n);
//	}

	tbuf=malloc(1<<24);
	memset(tbuf, 0, 1<<24);

	ctx=bgbcc_malloc(sizeof(BGBCP_ParseState));
	memset(ctx, 0, sizeof(BGBCP_ParseState));

	ctx->lang=lang;
	ctx->arch=BGBCC_GetArch();

	t0=clock();

	BGBCP_PushLinenum();
	BGBCP_SetLinenum(name, buf, 1);

	pprs=BGBPP_Filter(ctx, buf, tbuf, 1<<24);

	BGBCP_PopLinenum();

	t1=clock();
	dt=(1000.0*(t1-t0))/CLOCKS_PER_SEC;
	printf("PreProc took %dms\n", dt);

//	printf("PP\n%s\n\n", tbuf);

	sprintf(b, "dump/%s_pp.txt", modname);
	BGBCC_StoreTextFile(b, tbuf);

	if(pprs<0)
	{
		printf("Preprocessor Error: Abort\n");
		free(tbuf);
		return(NULL);
	}

	t0=clock();

	bgbcp_warn=0;
	bgbcp_err=0;

	ctx->structs=NULL;
	ctx->types=NULL;

	BGBCP_SetLinenum(name, tbuf, 1);
	s=tbuf;
	n1=BGBCP_Block(ctx, &s);

	t1=clock();
	dt=(1000.0*(t1-t0))/CLOCKS_PER_SEC;
	printf("Parse took %dms\n", dt);

	if(bgbcp_err>0)
	{
		printf("Parse Error: Abort\n");
		free(tbuf);
		return(NULL);
	}

	n=BCCX_New3("module",
		BCCX_New1("types", ctx->types),
		BCCX_New1("structs", ctx->structs),
		BCCX_New1("body", n1));

	s=BGBCP_NameForLang(ctx->lang);
	BCCX_Set(n, "lang", s);

	s=BGBCP_NameForArch(ctx->arch);
	BCCX_Set(n, "arch", s);

	free(tbuf);

	return(n);
}
