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

		n=NULL;
		if((ctx->lang==BGBCC_LANG_C) || (ctx->lang==BGBCC_LANG_CPP))
		{
			n=BGBCP_DefType(ctx, &s);
			if(n)
			{
				n1=BGBCP_VarDefinition(ctx, &s, n);
				if(n1)n=n1;
			}
		}

		if(!n)
			{ n=BGBCP_Expression(ctx, &s); }
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

fourcc BGBCP_LangForName(char *name)
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
		if(!strcmp(name, "S"))lang=BGBCC_LANG_ASM;
		if(!strcmp(name, "ASM"))lang=BGBCC_LANG_ASM;
		if(lang)return(lang);

		if(!strcmp(name, "c"))lang=BGBCC_LANG_C;
		if(!strcmp(name, "cpp"))lang=BGBCC_LANG_CPP;
		if(!strcmp(name, "c++"))lang=BGBCC_LANG_CPP;
		if(!strcmp(name, "c#"))lang=BGBCC_LANG_CS;
		if(!strcmp(name, "cs"))lang=BGBCC_LANG_CS;
		if(!strcmp(name, "bs"))lang=BGBCC_LANG_BS;
		if(!strcmp(name, "bs2"))lang=BGBCC_LANG_BS2;
		if(!strcmp(name, "java"))lang=BGBCC_LANG_JAVA;
		if(!strcmp(name, "s"))lang=BGBCC_LANG_ASM;
		if(!strcmp(name, "asm"))lang=BGBCC_LANG_ASM;
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

		if(!strcmp(s, ".s"))lang=BGBCC_LANG_ASM;
		if(!strcmp(s, ".asm"))lang=BGBCC_LANG_ASM;

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

		if(!strcmp(s, ".S"))lang=BGBCC_LANG_ASM;
		if(!strcmp(s, ".ASM"))lang=BGBCC_LANG_ASM;

		if(!strcmp(s, ".ril"))lang=BGBCC_IMGFMT_RIL3;
		if(!strcmp(s, ".RIL"))lang=BGBCC_IMGFMT_RIL3;
		if(!strcmp(s, ".ril3"))lang=BGBCC_IMGFMT_RIL3;
		if(!strcmp(s, ".RIL3"))lang=BGBCC_IMGFMT_RIL3;
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

fourcc BGBCP_ArchForName(char *name)
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

	if(!strcmp(name, "SH2"))	i=BGBCC_ARCH_SH;
	if(!strcmp(name, "SH4"))	i=BGBCC_ARCH_SH;
	if(!strcmp(name, "SH"))		i=BGBCC_ARCH_SH;

	return(i);
}

fourcc BGBCP_SubArchForName(int arch, char *name)
{
	int i;

	i=0;

	if(!strcmp(name, "SH2"))	i=BGBCC_ARCH_SH_SH2;
	if(!strcmp(name, "SH4"))	i=BGBCC_ARCH_SH_SH4;

	return(i);
}

char *BGBCP_NameForArch(fourcc arch)
{
	char *s;
	
	switch(arch)
	{
	case BGBCC_ARCH_DEFAULT: s="default"; break;
	case BGBCC_ARCH_X86:	s="x86"; break;
	case BGBCC_ARCH_X64:	s="x64"; break;
	case BGBCC_ARCH_ARM:	s="ARM"; break;
	case BGBCC_ARCH_PPC:	s="PPC"; break;
	case BGBCC_ARCH_SH:		s="SH"; break;
	case BGBCC_ARCH_SH64:	s="SH64"; break;

	default: s="unknown"; break;
	}
	return(s);
}

char *BGBCP_NameForSubArch(fourcc arch, fourcc subarch)
{
	char *s;
	
	switch(arch)
	{
	case BGBCC_ARCH_SH_SH2:
		s="SH2"; break;
	case BGBCC_ARCH_SH_SH4:
		s="SH4"; break;
	default:
		s=NULL; break;
	}
	return(s);
}

fourcc BGBCP_ImageFormatForName(char *name)
{
	u32 fmt;
	char *s;

	fmt=0;

	if(name)
	{
		if(!bgbcc_stricmp(name, "O"))fmt=BGBCC_IMGFMT_OBJ;
		if(!bgbcc_stricmp(name, "OBJ"))fmt=BGBCC_IMGFMT_OBJ;
		if(!bgbcc_stricmp(name, "EXE"))fmt=BGBCC_IMGFMT_EXE;
		if(!bgbcc_stricmp(name, "DLL"))fmt=BGBCC_IMGFMT_DLL;
		if(!bgbcc_stricmp(name, "RIL3"))fmt=BGBCC_IMGFMT_RIL3;
		if(fmt)return(fmt);

		s=name+strlen(name);
		while((s>name) && (*s!='.'))s--;

		if(!bgbcc_stricmp(s, ".o"))fmt=BGBCC_IMGFMT_OBJ;
		if(!bgbcc_stricmp(s, ".obj"))fmt=BGBCC_IMGFMT_OBJ;

		if(!bgbcc_stricmp(s, ".exe"))fmt=BGBCC_IMGFMT_EXE;
		if(!bgbcc_stricmp(s, ".dll"))fmt=BGBCC_IMGFMT_DLL;
		if(!bgbcc_stricmp(s, ".elf"))fmt=BGBCC_IMGFMT_EXE;
		if(!bgbcc_stricmp(s, ".so"))fmt=BGBCC_IMGFMT_DLL;
		if(!bgbcc_stricmp(s, ".ril"))fmt=BGBCC_IMGFMT_RIL3;
		if(!bgbcc_stricmp(s, ".ril3"))fmt=BGBCC_IMGFMT_RIL3;
	}
	
	return(fmt);
}

BCCX_Node *BGBCP_ModuleBuffer(char *name, char *modname, char *buf)
{
	char b[256];
//	VFILE *fd;
	BGBCP_ParseState *ctx;
	char *tbuf, *s;
	BCCX_Node *n, *n1, *n2, *c;
	int t0, t1, dt, lang, pprs;
	int i, j, k;

	BGBCC_CCXL_InitTargets();

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
	ctx->subarch=BGBCC_GetSubArch();
	
	BGBCC_CCXL_SetupParserForArch(ctx);

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

	if(lang==BGBCC_LANG_ASM)
	{
		n1=BCCX_NewCData(tbuf);
		n1=BCCX_New1("asm_blob", n1);
//		BCCX_Set(n1, "text", tbuf);
	}else
	{
		BGBCP_SetLinenum(name, tbuf, 1);
		s=tbuf;
		n1=BGBCP_Block(ctx, &s);
	}

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

	s=BGBCP_NameForSubArch(ctx->arch, ctx->subarch);
	if(s)
		BCCX_Set(n, "subarch", s);

	free(tbuf);

	c=ctx->reduce_tmp;
	ctx->reduce_tmp=NULL;
	while(c)
	{
		n1=c->hnext;
		BCCX_DeleteTree(c);
		c=n1;
	}


	return(n);
}
