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
			(!bgbcp_strcmp1(b, ")") || !bgbcp_strcmp1(b, "]") ||
			!bgbcp_strcmp1(b, "}")))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if((ty==BTK_SEPERATOR) && !bgbcp_strcmp1(b, ","))
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
			(!bgbcp_strcmp1(b, ")") || !bgbcp_strcmp1(b, "]") ||
			!bgbcp_strcmp1(b, "}")))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if(bgbcp_strcmp1(b, ","))
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
		if((ty==BTK_BRACE) && !bgbcp_strcmp1(b, ">"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		n=BGBCP_Expression(ctx, &s);
//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

		BGBCP_TokenGen(s, b, &ty);
		if((ty==BTK_BRACE) && !bgbcp_strcmp1(b, ">"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if(bgbcp_strcmp1(b, ","))
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

#if 0
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

#if 0
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
		if(!bgbcp_strcmp(name, "C"))lang=BGBCC_LANG_C;
		if(!bgbcp_strcmp(name, "CPP"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(name, "C++"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(name, "C#"))lang=BGBCC_LANG_CS;
		if(!bgbcp_strcmp(name, "CS"))lang=BGBCC_LANG_CS;
		if(!bgbcp_strcmp(name, "BS"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(name, "BS2"))lang=BGBCC_LANG_BS2;
		if(!bgbcp_strcmp(name, "Java"))lang=BGBCC_LANG_JAVA;
		if(!bgbcp_strcmp(name, "S"))lang=BGBCC_LANG_ASM;
		if(!bgbcp_strcmp(name, "ASM"))lang=BGBCC_LANG_ASM;
		if(lang)return(lang);

		if(!bgbcp_strcmp(name, "c"))lang=BGBCC_LANG_C;
		if(!bgbcp_strcmp(name, "cpp"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(name, "c++"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(name, "c#"))lang=BGBCC_LANG_CS;
		if(!bgbcp_strcmp(name, "cs"))lang=BGBCC_LANG_CS;
		if(!bgbcp_strcmp(name, "bs"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(name, "bs2"))lang=BGBCC_LANG_BS2;
		if(!bgbcp_strcmp(name, "java"))lang=BGBCC_LANG_JAVA;
		if(!bgbcp_strcmp(name, "s"))lang=BGBCC_LANG_ASM;
		if(!bgbcp_strcmp(name, "asm"))lang=BGBCC_LANG_ASM;
		if(lang)return(lang);

		s=name+strlen(name);
		while((s>name) && (*s!='.'))s--;

		if(!bgbcp_strcmp(s, ".c"))lang=BGBCC_LANG_C;
		if(!bgbcp_strcmp(s, ".h"))lang=BGBCC_LANG_C;

		if(!bgbcp_strcmp(s, ".cpp"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(s, ".hpp"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(s, ".cs"))lang=BGBCC_LANG_CS;
		if(!bgbcp_strcmp(s, ".java"))lang=BGBCC_LANG_JAVA;

		if(!bgbcp_strcmp(s, ".bs"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(s, ".js"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(s, ".es"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(s, ".bs2"))lang=BGBCC_LANG_BS2;

		if(!bgbcp_strcmp(s, ".s"))lang=BGBCC_LANG_ASM;
		if(!bgbcp_strcmp(s, ".asm"))lang=BGBCC_LANG_ASM;

		if(!bgbcp_strcmp(s, ".C"))lang=BGBCC_LANG_C;
		if(!bgbcp_strcmp(s, ".H"))lang=BGBCC_LANG_C;

		if(!bgbcp_strcmp(s, ".CPP"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(s, ".HPP"))lang=BGBCC_LANG_CPP;
		if(!bgbcp_strcmp(s, ".CS"))lang=BGBCC_LANG_CS;
		if(!bgbcp_strcmp(s, ".JAVA"))lang=BGBCC_LANG_JAVA;

		if(!bgbcp_strcmp(s, ".BS"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(s, ".JS"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(s, ".ES"))lang=BGBCC_LANG_BS;
		if(!bgbcp_strcmp(s, ".BS2"))lang=BGBCC_LANG_BS2;

		if(!bgbcp_strcmp(s, ".S"))lang=BGBCC_LANG_ASM;
		if(!bgbcp_strcmp(s, ".ASM"))lang=BGBCC_LANG_ASM;

		if(!bgbcp_strcmp(s, ".ril"))lang=BGBCC_IMGFMT_RIL3;
		if(!bgbcp_strcmp(s, ".RIL"))lang=BGBCC_IMGFMT_RIL3;
		if(!bgbcp_strcmp(s, ".ril3"))lang=BGBCC_IMGFMT_RIL3;
		if(!bgbcp_strcmp(s, ".RIL3"))lang=BGBCC_IMGFMT_RIL3;

		if(!bgbcp_strcmp(s, ".dll"))lang=BGBCC_IMGFMT_DLL;
		if(!bgbcp_strcmp(s, ".DLL"))lang=BGBCC_IMGFMT_DLL;
		if(!bgbcp_strcmp(s, ".exe"))lang=BGBCC_IMGFMT_EXE;
		if(!bgbcp_strcmp(s, ".EXE"))lang=BGBCC_IMGFMT_EXE;
		if(!bgbcp_strcmp(s, ".elf"))lang=BGBCC_IMGFMT_ELXE;
		if(!bgbcp_strcmp(s, ".ELF"))lang=BGBCC_IMGFMT_ELXE;
		if(!bgbcp_strcmp(s, ".so"))lang=BGBCC_IMGFMT_ELSO;
		if(!bgbcp_strcmp(s, ".SO"))lang=BGBCC_IMGFMT_ELSO;

		if(!bgbcp_strcmp(s, ".rom"))lang=BGBCC_IMGFMT_ROM;
		if(!bgbcp_strcmp(s, ".ROM"))lang=BGBCC_IMGFMT_ROM;
		if(!bgbcp_strcmp(s, ".bin"))lang=BGBCC_IMGFMT_ROM;
		if(!bgbcp_strcmp(s, ".BIN"))lang=BGBCC_IMGFMT_ROM;
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

	if(!bgbcp_strcmp(name, "any"))		i=BGBCC_ARCH_DEFAULT;
	if(!bgbcp_strcmp(name, "default"))	i=BGBCC_ARCH_DEFAULT;

	if(!bgbcp_strcmp(name, "x86"))	i=BGBCC_ARCH_X86;
	if(!bgbcp_strcmp(name, "X86"))	i=BGBCC_ARCH_X86;

	if(!bgbcp_strcmp(name, "x64"))	i=BGBCC_ARCH_X64;
	if(!bgbcp_strcmp(name, "x86-64"))	i=BGBCC_ARCH_X64;
	if(!bgbcp_strcmp(name, "x86_64"))	i=BGBCC_ARCH_X64;
	if(!bgbcp_strcmp(name, "X64"))	i=BGBCC_ARCH_X64;
	if(!bgbcp_strcmp(name, "AMD64"))	i=BGBCC_ARCH_X64;

	if(!bgbcp_strcmp(name, "ARM"))	i=BGBCC_ARCH_ARM;
	if(!bgbcp_strcmp(name, "PPC"))	i=BGBCC_ARCH_PPC;
	if(!bgbcp_strcmp(name, "PPC64"))	i=BGBCC_ARCH_PPC64;

	if(!bgbcp_strcmp(name, "SH2"))	i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "SH4"))	i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "SH"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "SH2L"))	i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "SH4L"))	i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "SH2B"))	i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "SH4B"))	i=BGBCC_ARCH_SH;

	if(!bgbcp_strcmp(name, "BJX1"))			i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1_32"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1_64"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1L"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1L_32"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1L_64"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1B"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1B_32"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1B_64"))		i=BGBCC_ARCH_SH;

	if(!bgbcp_strcmp(name, "BJX1_64u"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1L_64u"))	i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1B_64u"))	i=BGBCC_ARCH_SH;

	if(!bgbcp_strcmp(name, "BJX1_64B"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1L_64B"))	i=BGBCC_ARCH_SH;

	if(!bgbcp_strcmp(name, "BJX1_64C"))		i=BGBCC_ARCH_SH;
	if(!bgbcp_strcmp(name, "BJX1L_64C"))	i=BGBCC_ARCH_SH;

	return(i);
}

fourcc BGBCP_SubArchForName(int arch, char *name)
{
	int i;

	i=0;

	if(!bgbcp_strcmp(name, "SH2"))	i=BGBCC_ARCH_SH_SH2;
	if(!bgbcp_strcmp(name, "SH4"))	i=BGBCC_ARCH_SH_SH4;
	if(!bgbcp_strcmp(name, "SH2L"))	i=BGBCC_ARCH_SH_SH2L;
	if(!bgbcp_strcmp(name, "SH4L"))	i=BGBCC_ARCH_SH_SH4L;
	if(!bgbcp_strcmp(name, "SH2B"))	i=BGBCC_ARCH_SH_SH2B;
	if(!bgbcp_strcmp(name, "SH4B"))	i=BGBCC_ARCH_SH_SH4B;

	if(!bgbcp_strcmp(name, "BJX1"))			i=BGBCC_ARCH_SH_BX1L;
	if(!bgbcp_strcmp(name, "BJX1_32"))		i=BGBCC_ARCH_SH_BX1L;
	if(!bgbcp_strcmp(name, "BJX1_64"))		i=BGBCC_ARCH_SH_BX6L;
	if(!bgbcp_strcmp(name, "BJX1L"))		i=BGBCC_ARCH_SH_BX1L;
	if(!bgbcp_strcmp(name, "BJX1L_32"))		i=BGBCC_ARCH_SH_BX1L;
	if(!bgbcp_strcmp(name, "BJX1L_64"))		i=BGBCC_ARCH_SH_BX6L;
	if(!bgbcp_strcmp(name, "BJX1B"))		i=BGBCC_ARCH_SH_BX1B;
	if(!bgbcp_strcmp(name, "BJX1B_32"))		i=BGBCC_ARCH_SH_BX1B;
	if(!bgbcp_strcmp(name, "BJX1B_64"))		i=BGBCC_ARCH_SH_BX6B;

	if(!bgbcp_strcmp(name, "BJX1_64B"))		i=BGBCC_ARCH_SH_BX6M;
	if(!bgbcp_strcmp(name, "BJX1L_64B"))	i=BGBCC_ARCH_SH_BX6M;
	
	if(!bgbcp_strcmp(name, "BJX1_64u"))		i=BGBCC_ARCH_SH_BX6U;
	if(!bgbcp_strcmp(name, "BJX1L_64u"))	i=BGBCC_ARCH_SH_BX6U;
	if(!bgbcp_strcmp(name, "BJX1B_64u"))	i=BGBCC_ARCH_SH_BX6V;

	if(!bgbcp_strcmp(name, "BJX1_64C"))		i=BGBCC_ARCH_SH_BX6C;
	if(!bgbcp_strcmp(name, "BJX1L_64C"))	i=BGBCC_ARCH_SH_BX6C;

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

	case BGBCC_ARCH_SH_SH2L:
		s="SH2L"; break;
	case BGBCC_ARCH_SH_SH4L:
		s="SH4L"; break;

	case BGBCC_ARCH_SH_SH2B:
		s="SH2B"; break;
	case BGBCC_ARCH_SH_SH4B:
		s="SH4B"; break;

	case BGBCC_ARCH_SH_BX1L:
		s="BJX1L"; break;
	case BGBCC_ARCH_SH_BX1B:
		s="BJX1B"; break;
	case BGBCC_ARCH_SH_BX6L:
		s="BJX1L_64"; break;
	case BGBCC_ARCH_SH_BX6B:
		s="BJX1B_64"; break;

	case BGBCC_ARCH_SH_BX6U:
		s="BJX1L_64u"; break;
	case BGBCC_ARCH_SH_BX6V:
		s="BJX1B_64u"; break;

	case BGBCC_ARCH_SH_BX6M:
		s="BJX1L_64B"; break;

	case BGBCC_ARCH_SH_BX6C:
		s="BJX1L_64C"; break;

	default:
		s=NULL; break;
	}
	return(s);
}

char *BGBCP_DescForSubArch(fourcc arch, fourcc subarch)
{
	char *s;
	
	switch(arch)
	{
	case BGBCC_ARCH_SH_SH2:
		s="SH2 Default Endian"; break;
	case BGBCC_ARCH_SH_SH4:
		s="SH4 Default Endian"; break;

	case BGBCC_ARCH_SH_SH2L:
		s="SH2 Little Endian"; break;
	case BGBCC_ARCH_SH_SH4L:
		s="SH4 Little Endian"; break;

	case BGBCC_ARCH_SH_SH2B:
		s="SH2 Big Endian"; break;
	case BGBCC_ARCH_SH_SH4B:
		s="SH4 Big Endian"; break;

	case BGBCC_ARCH_SH_BX1L:
		s="BJX1 32-bit Little Endian"; break;
	case BGBCC_ARCH_SH_BX1B:
		s="BJX1 32-bit Big Endian"; break;
	case BGBCC_ARCH_SH_BX6L:
		s="BJX1 64-bit Little Endian"; break;
	case BGBCC_ARCH_SH_BX6B:
		s="BJX1 64-bit Big Endian"; break;

	default:
		s=NULL; break;
	}
	return(s);
}

int BGBCP_QueryArchList(fourcc *rarch, int narch)
{
	int ntgt;
	
	ntgt=0;
	rarch[ntgt++]=BGBCC_ARCH_SH_SH2;
	rarch[ntgt++]=BGBCC_ARCH_SH_SH4;
	rarch[ntgt++]=BGBCC_ARCH_SH_SH2B;
	rarch[ntgt++]=BGBCC_ARCH_SH_SH4B;
	rarch[ntgt++]=BGBCC_ARCH_SH_SH2L;
	rarch[ntgt++]=BGBCC_ARCH_SH_SH4L;
	return(ntgt);
}

int BGBCP_DumpTargets(void)
{
	fourcc tgts[256];
	int ntgt;
	int i, j, k;

	ntgt=BGBCP_QueryArchList(tgts, 256);
	
	for(i=0; i<ntgt; i++)
	{
		printf("%-4s (%s)\n",
			BGBCP_NameForSubArch(tgts[i], tgts[i]), 
			BGBCP_DescForSubArch(tgts[i], tgts[i]));
	}
	return(0);
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
		if(!bgbcc_stricmp(name, "ELF"))fmt=BGBCC_IMGFMT_ELXE;
		if(!bgbcc_stricmp(name, "SO"))fmt=BGBCC_IMGFMT_ELSO;
		if(!bgbcc_stricmp(name, "RIL3"))fmt=BGBCC_IMGFMT_RIL3;
		if(fmt)return(fmt);

		s=name+strlen(name);
		while((s>name) && (*s!='.'))s--;

		if(!bgbcc_stricmp(s, ".o"))fmt=BGBCC_IMGFMT_OBJ;
		if(!bgbcc_stricmp(s, ".obj"))fmt=BGBCC_IMGFMT_OBJ;

		if(!bgbcc_stricmp(s, ".exe"))fmt=BGBCC_IMGFMT_EXE;
		if(!bgbcc_stricmp(s, ".dll"))fmt=BGBCC_IMGFMT_DLL;
		if(!bgbcc_stricmp(s, ".elf"))fmt=BGBCC_IMGFMT_ELXE;
		if(!bgbcc_stricmp(s, ".so"))fmt=BGBCC_IMGFMT_ELSO;
		if(!bgbcc_stricmp(s, ".ril"))fmt=BGBCC_IMGFMT_RIL3;
		if(!bgbcc_stricmp(s, ".ril3"))fmt=BGBCC_IMGFMT_RIL3;

		if(!bgbcc_stricmp(s, ".rom"))fmt=BGBCC_IMGFMT_ROM;
		if(!bgbcc_stricmp(s, ".bin"))fmt=BGBCC_IMGFMT_ROM;
	}
	
	return(fmt);
}

BCCX_Node *BGBCP_ModuleBuffer(char *name, char *modname, char *buf)
{
	static char *tbuf=NULL;
	char b[256];
//	VFILE *fd;
	BGBCP_ParseState *ctx;
//	char *tbuf;
	char *s;
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

	if(!tbuf)
	{
		tbuf=malloc(1<<24);
//		memset(tbuf, 0, 1<<24);
	}

	*tbuf=0;
//	memset(tbuf, 0, 1<<16);

	ctx=bgbcc_malloc(sizeof(BGBCP_ParseState));
	memset(ctx, 0, sizeof(BGBCP_ParseState));

	ctx->lang=lang;
	ctx->arch=BGBCC_GetArch();
	ctx->subarch=BGBCC_GetSubArch();
	
	ctx->tuidx=BGBCC_GenSymInt();
	
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

//	sprintf(b, "dump/%s_pp.txt", modname);
//	BGBCC_StoreTextFile(b, tbuf);

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
//		n1=BGBCP_Block(ctx, &s);
		n1=BGBCP_Toplevel(ctx, &s);
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

	n=BCCX_NewCst3(&bgbcc_rcst_module, "module",
		BCCX_NewCst1(&bgbcc_rcst_types, "types", ctx->types),
		BCCX_NewCst1(&bgbcc_rcst_structs, "structs", ctx->structs),
		BCCX_NewCst1(&bgbcc_rcst_body, "body", n1));

	s=BGBCP_NameForLang(ctx->lang);
	BCCX_SetCst(n, &bgbcc_rcst_lang, "lang", s);

	s=BGBCP_NameForArch(ctx->arch);
	BCCX_SetCst(n, &bgbcc_rcst_arch, "arch", s);

	BCCX_SetIntCst(n, &bgbcc_rcst_index, "index", ctx->tuidx);

	s=BGBCP_NameForSubArch(ctx->arch, ctx->subarch);
	if(s)
		BCCX_SetCst(n, &bgbcc_rcst_subarch, "subarch", s);

//	free(tbuf);

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
