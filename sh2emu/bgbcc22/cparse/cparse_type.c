#include <bgbccc.h>

// BCCX_Node *bgbcp/_structs=NULL;
// BCCX_Node *bgbcp/_types=NULL;

#if 0
BCCX_Node *BGBCP_LookupStructI(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *c;
	char *s;

	c=ctx->structs;
	while(c)
	{
		s=BCCX_Get(c, "name");
		if(s && !strcmp(s, name))
			return(c);
		c=BCCX_Next(c);
	}
	return(NULL);
}
#endif

BCCX_Node *BGBCP_LookupStructI(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	BCCX_Node *c;
	int hi;
	char *s;

	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi&=255;

	c=ctx->struct_hash[hi];
	if(c)
	{
		s=BCCX_Get(c, "name");
		if(s && !strcmp(s, name))
			return(c);
	}

	c=ctx->structs;
	while(c)
	{
		s=BCCX_Get(c, "name");
		if(s && !strcmp(s, name))
		{
			ctx->struct_hash[hi]=c;
			return(c);
		}
		c=BCCX_Next(c);
	}
	return(NULL);
}

int BGBCP_HashTypeNameI(BGBCP_ParseState *ctx, char *name)
{
	int hi;
	char *s;

	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
//	hi=(hi>>8)&1023;
	hi=hi&1023;
	
	return(hi);
}

BCCX_Node *BGBCP_LookupTypeI(BGBCP_ParseState *ctx, char *name)
{
	static char *tk_name=NULL;
	char tb[256];
	BCCX_Node *c;
	int hi;
	char *s, *sn;

	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
//	hi=(hi>>8)&1023;
	hi=hi&1023;

	if(!tk_name)tk_name=bgbcc_strdup("name");

#if 0
	c=ctx->type_hash[hi];
	if(c)
	{
		//s=BCCX_Get(c, "name");
		s=BCCX_Get_SI(c, tk_name);
		if(s && !strcmp(s, name))
			return(c);
	}
#endif

//	sn=bgbcc_strdup(name);

#if 1
	c=ctx->type_hash[hi];
	while(c)
	{
		//s=BCCX_Get(c, "name");
		s=BCCX_Get_SI(c, tk_name);
		if(s && !strcmp(s, name))
//		if(s==sn)
			return(c);
		c=c->hnext;
	}
#endif

#if 0
	c=ctx->types;
	while(c)
	{
//		s=BCCX_Get(c, "name");
		s=BCCX_Get_SI(c, tk_name);
//		if(s && !strcmp(s, name))
		if(s==sn)
		{
//			c->hnext=ctx->type_hash[hi];
			ctx->type_hash[hi]=c;
			return(c);
		}
		c=BCCX_Next(c);
	}
#endif

	return(NULL);
}

BCCX_Node *BGBCP_LookupStruct(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	BCCX_Node *c;
	char *s;
	int i;

	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		c=BGBCP_LookupStructI(ctx, tb);
		if(c)return(c);
	}

	if(ctx->cur_nsi)
	{
		for(i=0; ctx->cur_nsi[i]; i++)
		{
			sprintf(tb, "%s/%s", ctx->cur_nsi[i], name);
			c=BGBCP_LookupStructI(ctx, tb);
			if(c)return(c);
		}
	}

	c=BGBCP_LookupStructI(ctx, name);
	if(c)return(c);

	return(NULL);
}

#if 0
// BCCX_Node *BGBCP_LookupSProto(BCCX_Node *body)
{
	BCCX_Node *c, t, sy;

	sy=SYM("s_proto");

	c=ctx->structs;
	while(c!=BS1_MM_NULL)
	{
		t=CAR(c);
		if(CAR(t)==sy)
		{
			if(BS1_EQVP(CDDR(t), body))
				return(t);
		}
		c=CDR(c);
	}
//	printf("BGBCP_LookupStruct: fail find %s\n", name);
	return(BS1_MM_NULL);
}
#endif

#if 0
BCCX_Node *BGBCP_LookupTypeI(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *c;
	char *s;

	c=ctx->types;
	while(c)
	{
		s=BCCX_Get(c, "name");
		if(s && !strcmp(s, name))
			return(c);
		c=BCCX_Next(c);
	}
	return(NULL);
}
#endif

BCCX_Node *BGBCP_LookupType(BGBCP_ParseState *ctx, char *name)
{
	char tb[256];
	BCCX_Node *c;
	char *s;
	int i;

	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		c=BGBCP_LookupTypeI(ctx, tb);
		if(c)return(c);
	}

	if(ctx->cur_nsi)
	{
		for(i=0; ctx->cur_nsi[i]; i++)
		{
			sprintf(tb, "%s/%s", ctx->cur_nsi[i], name);
			c=BGBCP_LookupTypeI(ctx, tb);
			if(c)return(c);
		}
	}

	c=BGBCP_LookupTypeI(ctx, name);
	if(c)return(c);

	return(NULL);
}


int BGBCP_HandleTypedef(BGBCP_ParseState *ctx, BCCX_Node *n)
{
	BCCX_Node *c, *t;
	char *s;
	int i;

	if(BCCX_TagIsP(n, "vars"))
	{
		c=BCCX_Child(n);
		while(c)
		{
			t=BCCX_Clone(c);
			
#if 1
			s=BCCX_Get(c, "name");
			if(!s) { c=BCCX_Next(c); continue; }
			i=BGBCP_HashTypeNameI(ctx, s);

			t->hnext=ctx->type_hash[i];
			ctx->type_hash[i]=t;
#endif		

			ctx->types=BCCX_AddEnd2(ctx->types, &ctx->e_types, t);
			c=BCCX_Next(c);
		}
		return(0);
	}

	BCCX_Print(n);

	BGBCP_Error(NULL, "BGBCP_HandleTypedef: "
			"Bad typedef definition\n");
	return(-1);
}

BCCX_Node *BGBCP_GetStruct(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_New("struct");
	BCCX_Set(n, "name", name);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
	return(n);
}

BCCX_Node *BGBCP_GetUnion(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_New("union");
	BCCX_Set(n, "name", name);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
	return(n);
}

BCCX_Node *BGBCP_GetEnum(BGBCP_ParseState *ctx, char *name)
{
	BCCX_Node *n;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	n=BCCX_New("enum");
	BCCX_Set(n, "name", name);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
	return(n);
}

BCCX_Node *BGBCP_GetStructJ(BGBCP_ParseState *ctx, char *name, int ty)
{
	char tb[256];
	BCCX_Node *n;
	char *s, *s1;

	n=BGBCP_LookupStruct(ctx, name);
	if(n)return(n);

	s=NULL;
	if(ty==1)s="struct";
	if(ty==2)s="union";
	if(ty==3)s="class";
	if(ty==4)s="interface";
	if(!s)return(NULL);

	s1=name;
	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		s1=bgbcc_strdup(tb);
	}

	n=BCCX_New(s);
	BCCX_Set(n, "name", s1);
	ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
	return(n);
}

int BGBCP_CheckTypeName(BGBCP_ParseState *ctx, char *name)
{
	char *s;
	s=BGBCP_LookupTypeSig(ctx, name);
	if(s)return(1);
	return(0);

//	return(1);
}


static char *bgbcp_basetypes_c[]={
"char", "short", "int", "long", "float", "double", "void",
"__int64",
"__int128", "__float128", "__float16", "__m64", "__m128",
"__vec2", "__vec3", "__vec4", "__quat", 
"__vec2d", "__vec3d", "__vec4d", "__quatd", 
"__mat2", "__mat3", "__mat4",

"__v2f", "__v3f", "__v4f",
"__m2f", "__m3f", "__m4f",
"__variant", "__variantf",
"__variant_va", "__variant_this", "__variant_cls",
"__gnuc_va_list", "__builtin_va_list",
"_Bool",
NULL
};

static char *bgbcp_basetypes_j[]={
"char", "byte", "short", "ushort", "int", "uint", "long", "ulong",
"float", "double", "void", "string", "variant",
"__int128", "__uint128",
"__float128", "__float16", "__m64", "__m128",
"__vec2", "__vec3", "__vec4", "__quat", 
"__vec2d", "__vec3d", "__vec4d", "__quatd", 
"__mat2", "__mat3", "__mat4",

"__v2f", "__v3f", "__v4f",
"__m2f", "__m3f", "__m4f",
"__variantf",
NULL
};

static int bgbcp_chktoklst(char *str, char **lst)
{
	char *s;
	int i;

	for(i=0; lst[i]; i++)
		if(!strcmp(lst[i], str))
			return(1);

	s="__type_";
	if(!strncmp(str, s, strlen(s)))
		return(1);

	return(0);
}

int BGBCP_DefTypeFlag(BGBCP_ParseState *ctx, char *tag)
{
	int i, n;

	i=0;

#if 0
	n=BGBCC_GetOrdinalLang(tag, ctx->lang);
	if(n<0)return(0);

	switch(n)
	{
	case BOTK_STATIC:			i=BGBCC_TYFL_STATIC; break;
	case BOTK_VOLATILE:			i=BGBCC_TYFL_VOLATILE; break;

	case BOTK_SIGNED:			i=BGBCC_TYFL_SIGNED; break;
	case BOTK_UNSIGNED:			i=BGBCC_TYFL_UNSIGNED; break;
	case BOTK_EXTERN:			i=BGBCC_TYFL_EXTERN; break;
	case BOTK_CONST:			i=BGBCC_TYFL_CONST; break;

	case BOTK_VIRTUAL:			i=BGBCC_TYFL_VIRTUAL; break;

	case BOTK_PUBLIC:			i=BGBCC_TYFL_PUBLIC; break;
	case BOTK_PRIVATE:			i=BGBCC_TYFL_PRIVATE; break;
	case BOTK_PROTECTED:		i=BGBCC_TYFL_PROTECTED; break;

	case BOTK_FINAL:			i=BGBCC_TYFL_FINAL; break;
	case BOTK_NATIVE:			i=BGBCC_TYFL_NATIVE; break;
	case BOTK_ABSTRACT:			i=BGBCC_TYFL_ABSTRACT; break;

	case BOTK__COMPLEX:			i=BGBCC_TYFL_UNSIGNED; break;
	case BOTK__IMAGINARY:		i=BGBCC_TYFL_UNSIGNED; break;

	case BOTK_INLINE:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___INLINE:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___FORCEINLINE:	i=BGBCC_TYFL_INLINE; break;

	case BOTK___STDCALL:		i=BGBCC_TYFL_STDCALL; break;
	case BOTK___CDECL:			i=BGBCC_TYFL_CDECL; break;
	case BOTK___PROXY:			i=BGBCC_TYFL_PROXY; break;

	case BOTK___W64:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___PTR32:			i=BGBCC_TYFL_INLINE; break;
	case BOTK___PTR64:			i=BGBCC_TYFL_INLINE; break;

	case BOTK___WIN64:			i=BGBCC_TYFL_WIN64; break;
	case BOTK___AMD64:			i=BGBCC_TYFL_AMD64; break;
	case BOTK___XCALL:			i=BGBCC_TYFL_XCALL; break;

	case BOTK___PACKED:			i=BGBCC_TYFL_PACKED; break;
	case BOTK___GC:				i=BGBCC_TYFL_GC; break;
	case BOTK___NOGC:			i=BGBCC_TYFL_NOGC; break;

	case BOTK___WIDE:			i=BGBCC_TYFL_PERSISTENT; break;
		
	case BOTK___PUBLIC:			i=BGBCC_TYFL_PUBLIC; break;
	case BOTK___PRIVATE:		i=BGBCC_TYFL_PRIVATE; break;
	case BOTK___PROTECTED:		i=BGBCC_TYFL_PROTECTED; break;

	case BOTK___FINAL:			i=BGBCC_TYFL_FINAL; break;

	case BOTK___VIRTUAL:		i=BGBCC_TYFL_VIRTUAL; break;
	case BOTK___NATIVE:			i=BGBCC_TYFL_NATIVE; break;
	case BOTK___ABSTRACT:		i=BGBCC_TYFL_ABSTRACT; break;

	default: break;
	}

	if(i)return(i);

	return(0);
#endif

	if(!strcmp(tag, "static"))i=BGBCC_TYFL_STATIC;
//	if(!strcmp(tag, "const"))i=BGBCC_TYFL_STATIC;
	if(!strcmp(tag, "volatile"))i=BGBCC_TYFL_VOLATILE;

//	if(n==BOTK_STATIC)i=BGBCC_TYFL_STATIC;
//	if(n==BOTK_VOLATILE)i=BGBCC_TYFL_VOLATILE;

	if(	(ctx->lang==BGBCC_LANG_C) ||
		(ctx->lang==BGBCC_LANG_CPP))
	{
#if 1
		if(!strcmp(tag, "signed"))i=BGBCC_TYFL_SIGNED;
		if(!strcmp(tag, "unsigned"))i=BGBCC_TYFL_UNSIGNED;

		if(!strcmp(tag, "extern"))i=BGBCC_TYFL_EXTERN;
		if(!strcmp(tag, "const"))i=BGBCC_TYFL_CONST;
		if(!strcmp(tag, "inline"))i=BGBCC_TYFL_INLINE;

		if(!strcmp(tag, "register"))i=BGBCC_TYFL_INLINE;

		if(!strcmp(tag, "__const"))i=BGBCC_TYFL_CONST;

		if(!strcmp(tag, "__inline"))i=BGBCC_TYFL_INLINE;
		if(!strcmp(tag, "__forceinline"))i=BGBCC_TYFL_INLINE;

		if(!strcmp(tag, "__stdcall"))i=BGBCC_TYFL_STDCALL;
		if(!strcmp(tag, "__cdecl"))i=BGBCC_TYFL_CDECL;
		if(!strcmp(tag, "__proxy"))i=BGBCC_TYFL_PROXY;

		if(!strcmp(tag, "__w64"))i=BGBCC_TYFL_INLINE;
		if(!strcmp(tag, "__ptr64"))i=BGBCC_TYFL_INLINE;
		if(!strcmp(tag, "__ptr32"))i=BGBCC_TYFL_INLINE;

		if(!strcmp(tag, "__win64"))i=BGBCC_TYFL_WIN64;
		if(!strcmp(tag, "__amd64"))i=BGBCC_TYFL_AMD64;
		if(!strcmp(tag, "__xcall"))i=BGBCC_TYFL_XCALL;

		if(!strcmp(tag, "__packed"))i=BGBCC_TYFL_PACKED;
		if(!strcmp(tag, "__gc"))i=BGBCC_TYFL_GC;
		if(!strcmp(tag, "__nogc"))i=BGBCC_TYFL_NOGC;

		if(!strcmp(tag, "_Complex"))i=BGBCC_TYFL_UNSIGNED;
		if(!strcmp(tag, "_Imaginary"))i=BGBCC_TYFL_UNSIGNED;
		if(!strcmp(tag, "__wide"))i=BGBCC_TYFL_WIDE;

		if(!strcmp(tag, "__ltlendian"))i=BGBCC_TYFL_LTLENDIAN;
		if(!strcmp(tag, "__bigendian"))i=BGBCC_TYFL_BIGENDIAN;

		if(!strcmp(tag, "__transient"))i=BGBCC_TYFL_ABSTRACT;
		if(!strcmp(tag, "__thread"))i=BGBCC_TYFL_THREAD;
		if(!strcmp(tag, "__dynamic"))i=BGBCC_TYFL_DYNAMIC;

		if(!strcmp(tag, "__getter"))i=BGBCC_TYFL_GETTER;
		if(!strcmp(tag, "__setter"))i=BGBCC_TYFL_SETTER;
#endif
	}

	if(ctx->lang==BGBCC_LANG_C)
	{
		if(!strcmp(tag, "__public"))i=BGBCC_TYFL_PUBLIC;
		if(!strcmp(tag, "__private"))i=BGBCC_TYFL_PRIVATE;
		if(!strcmp(tag, "__protected"))i=BGBCC_TYFL_PROTECTED;
		if(!strcmp(tag, "__final"))i=BGBCC_TYFL_FINAL;

		if(!strcmp(tag, "__virtual"))i=BGBCC_TYFL_VIRTUAL;
		if(!strcmp(tag, "__native"))i=BGBCC_TYFL_NATIVE;
		if(!strcmp(tag, "__abstract"))i=BGBCC_TYFL_ABSTRACT;
	}

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		if(!strcmp(tag, "virtual"))i=BGBCC_TYFL_VIRTUAL;
	}

	if(	(ctx->lang==BGBCC_LANG_CPP) ||
		(ctx->lang==BGBCC_LANG_JAVA) ||
		(ctx->lang==BGBCC_LANG_CS))
	{
		if(!strcmp(tag, "public"))i=BGBCC_TYFL_PUBLIC;
		if(!strcmp(tag, "private"))i=BGBCC_TYFL_PRIVATE;
		if(!strcmp(tag, "protected"))i=BGBCC_TYFL_PROTECTED;

		if(!strcmp(tag, "abstract"))i=BGBCC_TYFL_ABSTRACT;
	}

	if(ctx->lang==BGBCC_LANG_CS)
	{
		if(!strcmp(tag, "virtual"))i=BGBCC_TYFL_VIRTUAL;
		if(!strcmp(tag, "new"))i=BGBCC_TYFL_VIRTUAL;
		if(!strcmp(tag, "override"))i=BGBCC_TYFL_VIRTUAL;
		
		if(!strcmp(tag, "internal"))i=BGBCC_TYFL_INLINE;

		if(!strcmp(tag, "sealed"))i=BGBCC_TYFL_FINAL;
		if(!strcmp(tag, "readonly"))i=BGBCC_TYFL_FINAL;
	}

//	if(	(ctx->lang==BGBCC_LANG_JAVA) ||
//		(ctx->lang==BGBCC_LANG_CS))

	if(ctx->lang==BGBCC_LANG_JAVA)
	{
		if(!strcmp(tag, "final"))i=BGBCC_TYFL_FINAL;

		if(!strcmp(tag, "transient"))i=BGBCC_TYFL_TRANSIENT;
		if(!strcmp(tag, "native"))i=BGBCC_TYFL_NATIVE;

		if(!strcmp(tag, "abstract"))i=BGBCC_TYFL_ABSTRACT;
	}

	return(i);
}

BCCX_Node *BGBCP_DefClassC(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1, *s2;
	int i, j, ty, ty2, fl, cty;
	BCCX_Node *n, *n1, *n2, *nl;

	s=*str;

//	bty=NULL;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token(s, b, &ty);
		i=BGBCP_DefTypeFlag(ctx, b);
		if(i)
		{
			fl|=i;
			s=BGBCP_Token(s, b, &ty);
			j++;
			continue;
		}

		break;
	}

	if(!strcmp(b, "struct") || !strcmp(b, "union") ||
		!strcmp(b, "class") || !strcmp(b, "interface") ||
		!strcmp(b, "__class") || !strcmp(b, "__interface"))
	{
//		j=!strcmp(b, "struct");

		s=BGBCP_Token(s, b, &ty);	//struct

		cty=0;
		if(!strcmp(b, "struct"))cty=1;
		if(!strcmp(b, "union"))cty=2;

		if(!strcmp(b, "class"))
		{
			cty=3;	//C++, class==struct
//			if(fl&BGBCC_TYFL_GC)cty=3;
//				else fl|=BGBCC_TYFL_CLASS;
			if(!(fl&BGBCC_TYFL_GC))
				fl|=BGBCC_TYFL_NOGC;

		}

//		if(!strcmp(b, "interface"))cty=4;

		if(!strcmp(b, "__class"))cty=3;
		if(!strcmp(b, "__interface"))cty=4;

		nl=NULL;
		while(1)
		{
			n1=BGBCP_DeclAttributeC(ctx, &s);
			if(!n1)break;
			nl=BCCX_AddEnd(nl, n1);
		}

		s1=BGBCP_Token(s, b, &ty);	//name, '{', ...
		BGBCP_Token(s1, b2, &ty2);	//name, '{', ...

		n=NULL;
		if(ty==BTK_NAME)
		{
			s=BGBCP_Token(s, b, &ty);	//name
			n=BGBCP_GetStructJ(ctx, b, cty);

			s1=BGBCP_Token(s, b, &ty);	//name, '{', ...
			BGBCP_Token(s1, b2, &ty2);	//name, '{', ...
		}

		if(n && !strcmp(b, ":"))
		{
			s=BGBCP_Token(s, b, &ty);	//':'

			n1=NULL;
			if(!n1)n1=BCCX_New("super");
			while(1)
			{
				n2=BGBCP_DefType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token(s, b, &ty);
				if(strcmp(b, ","))break;
				s=BGBCP_Token(s, b, &ty);
			}
			BCCX_Add(n, n1);
			BGBCP_Token(s, b, &ty);
		}

		if(!n)
		{
			s1=BGBCP_Token(s, b, &ty);	//name, '{', ...
			BGBCP_Token(s1, b2, &ty2);	//name, '{', ...

			if(!strcmp(b, "{"))
			{
//				s1=s;
				i=0; j=1;
				while(*s1 && j)
				{
					s1=BGBCP_Token(s1, b2, &ty2);
					if(!strcmp(b2, "{"))j++;
					if(!strcmp(b2, "}"))j--;
					s2=b2;
					while(*s2)i=i*127+(*s2++);
					i=i*127+' ';
				}
				sprintf(b2, "HTX%08X", i);
				s1=bgbcc_strdup(b2);
			}else
			{
				s1=BGBCP_GetLastFileName();
				i=BGBCP_GetLastLineNumber();
				sprintf(b2, "%s:%d", s1, i);
				s1=b2; i=0; while(*s1)i=i*127+(*s1++);

				sprintf(b2, "HFN%08X", i);
				s1=bgbcc_strdup(b2);
			}


//			s1=BGBCC_GenSym2();
			n=BGBCP_GetStructJ(ctx, s1, cty);
		}

		if(!strcmp(b, "{"))
		{
			s=BGBCP_Token(s, b, &ty); //'{'
			n1=BGBCP_Block(ctx, &s);
			BCCX_AddV(n, BCCX_New1V("body", n1));

//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
		}

		BCCX_SetInt(n, "flags", fl);

		n1=n;
		s1=BCCX_Get(n1, "name");
		n=BCCX_New("type");
		BCCX_Set(n, "name", s1);
		BCCX_SetInt(n, "flags", fl);
		BCCX_SetInt(n, "ind", 0);

		if(nl)BCCX_AddV(n, BCCX_New1V("attributes", nl));

		*str=s;
		return(n);
	}

	if(!strcmp(b, "enum"))
	{
		s=BGBCP_Token(s, b, &ty);	//enum
		s1=BGBCP_Token(s, b, &ty);	//name|'{'
		BGBCP_Token(s1, b2, &ty2);	//name|'{'|...

		if(ty==BTK_NAME)
		{
			s=BGBCP_Token(s, b, &ty);	//name
			n=BGBCP_GetEnum(ctx, b);

			BGBCP_Token(s, b2, &ty2);	//'{', ...
			if(!strcmp(b2, "{"))
			{
				s=BGBCP_Token(s, b2, &ty2); //'{'
				n1=BGBCP_EnumVarsList(ctx, &s);
				BCCX_AddV(n, BCCX_New1V("defs", n1));
			}
		}else if(!strcmp(b, "{"))
		{
			s=BGBCP_Token(s, b, &ty); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_New("enum");
			BCCX_Set(n, "name", s1);
			BCCX_AddV(n, BCCX_New1V("defs", n1));
			ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);
		}

		s1=BCCX_Get(n, "name");
		n=BCCX_New("type");
		BCCX_Set(n, "name", s1);
		BCCX_SetInt(n, "flags", fl);
		BCCX_SetInt(n, "ind", 0);

		*str=s;
		return(n);
	}

	return(NULL);
}

BCCX_Node *BGBCP_DeclAttributeC(BGBCP_ParseState *ctx, char **str)
{
	char b[4096], b2[4096];
	char *s, *s1, *bty;
	int i, j, ty, ty2, fl;
	BCCX_Node *n, *n1, *n2, *nl;

	s=*str;

	BGBCP_Token(s, b, &ty);
	if(ty!=BTK_NAME)return(NULL);

	if(!strcmp(b, "__declspec"))
	{
		nl=NULL;
	
		s=BGBCP_Token(s, b, &ty);	//__declspec
		s=BGBCP_Token(s, b, &ty);	//'('
		while(1)
		{
			BGBCP_Token(s, b, &ty);
			if(!strcmp(b, ")"))
			{
				s=BGBCP_Token(s, b, &ty);
				break;
			}
			if(!strcmp(b, ","))
			{
				s=BGBCP_Token(s, b, &ty);
				continue;
			}
			if(ty!=BTK_NAME)break;

			s=BGBCP_Token(s, b, &ty);	//name
			BGBCP_Token(s, b2, &ty2);	//'('
			if(!strcmp(b2, "("))
			{
				s=BGBCP_Token(s, b2, &ty2);	//'('
				n2=BGBCP_FunArgs(ctx, &s);
				n1=BCCX_New1("attr", BCCX_New1V("args", n2));
				BCCX_Set(n1, "name", b);
			}else
			{
				n1=BCCX_New("attr");
				BCCX_Set(n1, "name", b);
			}

			nl=BCCX_AddEnd(nl, n1);
		}
			
		n=BCCX_New1("declspec", nl);
		*str=s;
		return(n);
	}

	if(!strcmp(b, "__attribute__"))
	{
		nl=NULL;
	
		s=BGBCP_Token(s, b, &ty);	//__attribute__
		s=BGBCP_Token(s, b, &ty);	//'('
		s=BGBCP_Token(s, b, &ty);	//'('
		while(1)
		{
			BGBCP_Token(s, b, &ty);
			if(!strcmp(b, ")"))
			{
				s=BGBCP_Token(s, b, &ty);	//')'
				s=BGBCP_Token(s, b, &ty);	//')'
				break;
			}
			if(!strcmp(b, ","))
			{
				s=BGBCP_Token(s, b, &ty);
				continue;
			}
			if(ty!=BTK_NAME)break;

			s=BGBCP_Token(s, b, &ty);	//name
			BGBCP_Token(s, b2, &ty2);	//'('
			if(!strcmp(b2, "("))
			{
				s=BGBCP_Token(s, b2, &ty2);	//'('
				n2=BGBCP_FunArgs(ctx, &s);
				n1=BCCX_New1("attr", BCCX_New1V("args", n2));
				BCCX_Set(n1, "name", b);
			}else
			{
				n1=BCCX_New("attr");
				BCCX_Set(n1, "name", b);
			}

			nl=BCCX_AddEnd(nl, n1);
		}

		n=BCCX_New1("attribute", nl);
		*str=s;
		return(n);
	}
	
	return(NULL);
}

BCCX_Node *BGBCP_DefTypeC(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1, *bty;
	int i, j, ty, ty2, fl;
	BCCX_Node *n, *n1, *n2, *attrl, *attrle;

	s=*str;

//	BGBCP_Token(s, b, &ty);
//	BGBCP_Warn(s, "BGBCP_DefTypeC: token=%s\n", b);

	bty=NULL;

	fl=0; j=0; attrl=NULL; attrle=NULL;
	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(ty!=BTK_NAME)break;

		i=BGBCP_DefTypeFlag(ctx, b);
		if(i)
		{
			fl|=i;
			s=BGBCP_Token(s, b, &ty);
			j++;
			continue;
		}

		if(bgbcp_chktoklst(b, bgbcp_basetypes_c))
		{
			s=BGBCP_Token(s, b, &ty);

			if(!strcmp(b, "int") && bty)
			{
				if(!bty)bty=bgbcc_strdup(b);
				j++;
				continue;	//short int, ...
			}else if(!strcmp(b, "long"))
			{
				if(bty && !strcmp(bty, "long"))
				{
					bty=bgbcc_strdup("llong");
				}else
				{
					bty=bgbcc_strdup("long");
				}
			}else
			{
				bty=bgbcc_strdup(b);
				if(!strncmp(bty, "__type_", 7))bty+=7;
				if(!strncmp(bty, "__", 2))bty+=2;
			}

			j++;
			continue;
		}

		n1=BGBCP_DeclAttributeC(ctx, &s);
		if(n1)
		{
			attrl=BCCX_AddEnd2(attrl, &attrle, n1);
			//j++;
			continue;
		}

		break;
	}

	if(!strcmp(b, "struct") || !strcmp(b, "union") ||
		!strcmp(b, "enum") || !strcmp(b, "__class") ||
		!strcmp(b, "__interface") ||
		(!strcmp(b, "class") && (ctx->lang==BGBCC_LANG_CPP)))
	{
		n=BGBCP_DefClassC(ctx, &s);
		if(attrl)BCCX_Add(n, attrl);

		*str=s;
		return(n);	
	}

#if 0
	if(!strcmp(b, "struct"))return(BGBCP_DefClassC(ctx, str));
	if(!strcmp(b, "union"))return(BGBCP_DefClassC(ctx, str));
	if(!strcmp(b, "enum"))return(BGBCP_DefClassC(ctx, str));

	if(!strcmp(b, "__class"))return(BGBCP_DefClassC(ctx, str));
	if(!strcmp(b, "__interface"))return(BGBCP_DefClassC(ctx, str));

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		if(!strcmp(b, "class"))return(BGBCP_DefClassC(ctx, str));
//		if(!strcmp(b, "interface"))return(BGBCP_DefClassC(ctx, str));
	}
#endif

#if 0
	if(!strcmp(b, "struct") || !strcmp(b, "union"))
	{
		j=!strcmp(b, "struct");

		s=BGBCP_Token(s, b, &ty);	//struct
		s1=BGBCP_Token(s, b, &ty);	//name, '{', ...
		BGBCP_Token(s1, b2, &ty2);	//name, '{', ...

		if(ty==BTK_NAME)
		{
			s=BGBCP_Token(s, b, &ty);	//name
			if(j)n=BGBCP_GetStruct(ctx, b);
				else n=BGBCP_GetUnion(ctx, b);

			BGBCP_Token(s, b2, &ty2);	//'{', ...
			if(!strcmp(b2, "{"))
			{
				s=BGBCP_Token(s, b2, &ty2); //'{'
				n1=BGBCP_Block(ctx, &s);
				BCCX_AddV(n, BCCX_New1V("body", n1));
			}
		}else if(!strcmp(b, "{"))
		{
			s=BGBCP_Token(s, b, &ty); //'{'
			n1=BGBCP_Block(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_New(j?"struct":"union");
			BCCX_Set(n, "name", s1);
			BCCX_AddV(n, BCCX_New1V("body", n1));

			ctx->structs=BCCX_AddEnd(ctx->structs, n);
		}


		n1=n;
		s1=BCCX_Get(n, "name");
		n=BCCX_New("type");
		BCCX_Set(n, "name", s1);
		BCCX_SetInt(n, "flags", fl);
		BCCX_SetInt(n, "ind", 0);

//		printf("ESTR %s\n", s);
//		*(int *)-1=-1;

		*str=s;
		return(n);
	}

	if(!strcmp(b, "enum"))
	{
		s=BGBCP_Token(s, b, &ty);	//enum
		s1=BGBCP_Token(s, b, &ty);	//name|'{'
		BGBCP_Token(s1, b2, &ty2);	//name|'{'|...

		if(ty==BTK_NAME)
		{
			s=BGBCP_Token(s, b, &ty);	//name
			n=BGBCP_GetEnum(ctx, b);

			BGBCP_Token(s, b2, &ty2);	//'{', ...
			if(!strcmp(b2, "{"))
			{
				s=BGBCP_Token(s, b2, &ty2); //'{'
				n1=BGBCP_EnumVarsList(ctx, &s);
				BCCX_AddV(n, BCCX_New1V("defs", n1));
			}
		}else if(!strcmp(b, "{"))
		{
			s=BGBCP_Token(s, b, &ty); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_New("enum");
			BCCX_Set(n, "name", s1);
			BCCX_AddV(n, BCCX_New1V("defs", n1));
			ctx->structs=BCCX_AddEnd(ctx->structs, n);
		}

		s1=BCCX_Get(n, "name");
		n=BCCX_New("type");
		BCCX_Set(n, "name", s1);
		BCCX_SetInt(n, "flags", fl);
		BCCX_SetInt(n, "ind", 0);

		*str=s;
		return(n);
	}
#endif

	n=NULL;
	if(!bty)n=BGBCP_LookupType(ctx, b);

//	n=BGBCP_LookupType(ctx, b);
	if(n)
	{
		s=BGBCP_Token(s, b, &ty);

		while(1)
		{
			BGBCP_Token(s, b2, &ty2);
			i=BGBCP_DefTypeFlag(ctx, b2);
			if(i)
			{
				fl|=i;
				s=BGBCP_Token(s, b2, &ty2);
				j++;
				continue;
			}
			break;
		}

		n=BCCX_FindTag(n, "type");

		n=BCCX_Clone(n);
		i=BCCX_GetInt(n, "flags");
		BCCX_SetInt(n, "flags", i|fl);

		if(attrl)BCCX_Add(n, attrl);

		*str=s;
		return(n);
	}

	if(ctx->lang==BGBCC_LANG_CPP)
	{
		n=BGBCP_LookupStruct(ctx, b);
		if(n)
		{
			n1=n;
			s1=BCCX_Get(n1, "name");
			i=BCCX_GetInt(n1, "flags");

			n=BCCX_New("type");
			BCCX_Set(n, "name", s1);
			BCCX_SetInt(n, "flags", i|fl);
			BCCX_SetInt(n, "ind", 0);

			if(attrl)BCCX_Add(n, attrl);

			*str=s;
			return(n);
		}
	}

	if(ctx->lang==BGBCC_LANG_CX)
	{
		if(BGBCP_CheckTypeName(ctx, b))
		{
			while(1)
			{
				BGBCP_Token(s, b2, &ty2);
				i=BGBCP_DefTypeFlag(ctx, b2);
				if(i)
				{
					fl|=i;
					s=BGBCP_Token(s, b2, &ty2);
					j++;
					continue;
				}
				break;
			}

			n=BCCX_New("type");
			BCCX_Set(n, "name", b);
			BCCX_SetInt(n, "flags", fl);
			BCCX_SetInt(n, "ind", 0);

			if(attrl)BCCX_Add(n, attrl);

			*str=s;
			return(n);
		}
	}

	if(j)
	{
		if(!bty)bty="int";

		n=BCCX_New("type");
		BCCX_Set(n, "name", bty);
		BCCX_SetInt(n, "flags", fl);
		BCCX_SetInt(n, "ind", 0);

		if(attrl)BCCX_Add(n, attrl);

#if 0
		BGBCP_Token(s, b, &ty);
		if(!strcmp(b, ":"))
		{
			s=BGBCP_Token(s, b, &ty);	//':'
			n1=BGBCP_Expression(ctx, &s);
			n1=BCCX_New1("bits", n1);
			BCCX_Add(n, n1);
		}
#endif

		*str=s;
		return(n);
	}

	return(NULL);
}


BCCX_Node *BGBCP_DefClassJ(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1;
	int i, j, ty, ty2, fl, cty;
	BCCX_Node *n, *n1, *n2;

	s=*str;

//	bty=NULL;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token(s, b, &ty);
		i=BGBCP_DefTypeFlag(ctx, b);
		if(i)
		{
			fl|=i;
			s=BGBCP_Token(s, b, &ty);
			j++;
			continue;
		}

		break;
	}

	if(!strcmp(b, "struct") || !strcmp(b, "union") ||
		!strcmp(b, "class") || !strcmp(b, "interface"))
	{
//		j=!strcmp(b, "struct");

		s=BGBCP_Token(s, b, &ty);	//struct

		cty=0;
		if(!strcmp(b, "struct"))cty=1;
		if(!strcmp(b, "union"))cty=2;
		if(!strcmp(b, "class"))cty=3;
		if(!strcmp(b, "interface"))cty=4;

		s1=BGBCP_Token(s, b, &ty);	//name, '{', ...
		BGBCP_Token(s1, b2, &ty2);	//name, '{', ...

		n=NULL;
		if(ty==BTK_NAME)
		{
			s=BGBCP_Token(s, b, &ty);	//name
			n=BGBCP_GetStructJ(ctx, b, cty);

			s1=BGBCP_Token(s, b, &ty);	//name, '{', ...
			BGBCP_Token(s1, b2, &ty2);	//name, '{', ...
		}

		if(n && !strcmp(b, ":"))
		{
			s=BGBCP_Token(s, b, &ty);	//':'

			n1=BGBCP_DefType(ctx, &s);
			BCCX_AddV(n, BCCX_New1V("super", n1));

			BGBCP_Token(s, b, &ty);
			n1=NULL;
			while(!strcmp(b, ","))
			{
				if(!n1)n1=BCCX_New("impl");
				s=BGBCP_Token(s, b, &ty);	//','
				n2=BGBCP_DefType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token(s, b, &ty);
			}
			BCCX_Add(n, n1);
			BGBCP_Token(s, b, &ty);
		}

		if(n && !strcmp(b, "extends"))
		{
			s=BGBCP_Token(s, b, &ty);	//'extends'

			n1=BGBCP_DefType(ctx, &s);
			BCCX_AddV(n, BCCX_New1V("super", n1));
			BGBCP_Token(s, b, &ty);
		}

		if(n && !strcmp(b, "implements"))
		{
			s=BGBCP_Token(s, b, &ty);	//'implements'

			n1=BCCX_New("impl");

			while(1)
			{
				n2=BGBCP_DefType(ctx, &s);
				BCCX_Add(n1, n2);

				BGBCP_Token(s, b, &ty);
				if(strcmp(b, ","))
					break;
				s=BGBCP_Token(s, b, &ty);	//','
			}
			BCCX_Add(n, n1);
			BGBCP_Token(s, b, &ty);
		}

		if(!n)
		{
			s1=BGBCC_GenSym2();
			n=BGBCP_GetStructJ(ctx, s1, cty);
		}

		if(!strcmp(b, "{"))
		{
			s=BGBCP_Token(s, b, &ty); //'{'
			n1=BGBCP_Block(ctx, &s);
			BCCX_AddV(n, BCCX_New1V("body", n1));

//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
		}

		BCCX_SetInt(n, "flags", fl);

		n1=n;
		s1=BCCX_Get(n1, "name");
		n=BCCX_New("classdef");
		BCCX_Set(n, "name", s1);
		BCCX_SetInt(n, "flags", fl);
		BCCX_SetInt(n, "ind", 0);

		*str=s;
		return(n);
	}

	if(!strcmp(b, "enum"))
	{
		s=BGBCP_Token(s, b, &ty);	//enum
		s1=BGBCP_Token(s, b, &ty);	//name|'{'
		BGBCP_Token(s1, b2, &ty2);	//name|'{'|...

		if(ty==BTK_NAME)
		{
			s=BGBCP_Token(s, b, &ty);	//name
			n=BGBCP_GetEnum(ctx, b);

			BGBCP_Token(s, b2, &ty2);	//'{', ...
			if(!strcmp(b2, "{"))
			{
				s=BGBCP_Token(s, b2, &ty2); //'{'
				n1=BGBCP_EnumVarsList(ctx, &s);
				BCCX_AddV(n, BCCX_New1V("defs", n1));
			}
		}else if(!strcmp(b, "{"))
		{
			s=BGBCP_Token(s, b, &ty); //'{'
			n1=BGBCP_EnumVarsList(ctx, &s);

			s1=BGBCC_GenSym2();
			n=BCCX_New("enum");
			BCCX_Set(n, "name", s1);
			BCCX_AddV(n, BCCX_New1V("defs", n1));
			ctx->structs=BCCX_AddEnd(ctx->structs, n);
		}

		s1=BCCX_Get(n, "name");
		n=BCCX_New("enumdef");
		BCCX_Set(n, "name", s1);
		BCCX_SetInt(n, "flags", fl);
		BCCX_SetInt(n, "ind", 0);

		*str=s;
		return(n);
	}

	return(NULL);
}

BCCX_Node *BGBCP_DefTypeJ(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1, *bty, *vty;
	int i, j, ty, ty2, fl;
	BCCX_Node *n, *n1, *n2;

	s=*str;

	fl=0; j=0;
	while(1)
	{
		BGBCP_Token(s, b, &ty);
		i=BGBCP_DefTypeFlag(ctx, b);
		if(!i)break;

		fl|=i;
		s=BGBCP_Token(s, b, &ty);
		j++;
		continue;
	}

	bty=NULL;

//	if(!strcmp(b, "class"))return(NULL);
//	if(!strcmp(b, "struct"))return(NULL);
//	if(!strcmp(b, "interface"))return(NULL);
//	if(!strcmp(b, "enum"))return(NULL);

	if(!strcmp(b, "class"))return(BGBCP_DefClassJ(ctx, str));
	if(!strcmp(b, "struct"))return(BGBCP_DefClassJ(ctx, str));
	if(!strcmp(b, "union"))return(BGBCP_DefClassJ(ctx, str));
	if(!strcmp(b, "interface"))return(BGBCP_DefClassJ(ctx, str));
	if(!strcmp(b, "enum"))return(BGBCP_DefClassJ(ctx, str));

	if(bgbcp_chktoklst(b, bgbcp_basetypes_j))
	{
		s=BGBCP_Token(s, b, &ty);

		bty=bgbcc_strdup(b);

		if(!strncmp(bty, "__type_", 7))bty+=7;
		if(!strncmp(bty, "__", 2))bty+=2;
	}

	if(!bty && (ty==BTK_NAME))
	{
		s=BGBCP_Token(s, b, &ty);
		BGBCP_Token(s, b2, &ty2);
		while(!strcmp(b2, "."))
		{
			s=BGBCP_Token(s, b2, &ty2);	//'.'
			strcat(b, "/");
			s=BGBCP_Token(s, b2, &ty2);	//name

			if(ty2!=BTK_NAME)return(NULL);

			strcat(b, b2);
			BGBCP_Token(s, b2, &ty2);
		}

		vty=bgbcc_strdup(b);
	}
	if(fl)bty=vty;

	//bty: known valid type name
	//vty: expected valid

	if(bty)
	{
		n=BGBCP_LookupType(ctx, bty);
		if(n)
		{
			n=BCCX_FindTag(n, "type");
			n=BCCX_Clone(n);
			i=BCCX_GetInt(n, "flags");
			BCCX_SetInt(n, "flags", i|fl);
		}else
		{
			n=BCCX_New("type");
			BCCX_Set(n, "name", bty);
			BCCX_SetInt(n, "flags", fl);
			BCCX_SetInt(n, "ind", 0);
		}
	}else if(vty)
	{
		n=BGBCP_LookupType(ctx, vty);
		if(n)
		{
			n=BCCX_FindTag(n, "type");

			n=BCCX_Clone(n);
			i=BCCX_GetInt(n, "flags");
			BCCX_SetInt(n, "flags", i|fl);
		}else if(BGBCP_CheckTypeName(ctx, vty))
		{
			n=BCCX_New("type");
			BCCX_Set(n, "name", vty);
			BCCX_SetInt(n, "flags", fl);
			BCCX_SetInt(n, "ind", 0);
		}else
		{
			return(NULL);
		}
	}else
	{
		return(NULL);
	}

	BGBCP_Token(s, b, &ty);
	while(!strcmp(b, "["))
	{
		s=BGBCP_Token(s, b, &ty);
		BGBCP_Token(s, b, &ty);
		if(!strcmp(b, "]"))
		{
			s=BGBCP_Token(s, b, &ty);
			BCCX_Add(n, BCCX_New("array"));
			BGBCP_Token(s, b, &ty);
			continue;
		}

		n1=BGBCP_FunArgs(ctx, &s);
		BCCX_AddV(n, BCCX_New1V("array", n1));

		BGBCP_Token(s, b, &ty);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_DefType(BGBCP_ParseState *ctx, char **str)
{
	if((ctx->lang==BGBCC_LANG_C) || (ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_CX))
		return(BGBCP_DefTypeC(ctx, str));

	if((ctx->lang==BGBCC_LANG_JAVA) || (ctx->lang==BGBCC_LANG_CS))
		return(BGBCP_DefTypeJ(ctx, str));

	return(NULL);
}

BCCX_Node *BGBCP_EnumVarsList(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *lst;
	int i;

	s=*str;
	lst=NULL;
	i=0;

	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(!strcmp(b, ";") || !strcmp(b, "}"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		s=BGBCP_Token(s, b, &ty);
		BGBCP_Token(s, b2, &ty2);
		if(!strcmp(b2, "="))
		{
			s=BGBCP_Token(s, b2, &ty);	//'='
//			s=BGBCP_Token(s, b2, &ty);	//<val>
//			i=atoi(b2);
			n1=BGBCP_Expression(ctx, &s);
			n1=BGBCP_ReduceExpr(ctx, n1);
			i=BCCX_GetInt(n1, "value");
		}

		n=BCCX_New("def");
		BCCX_Set(n, "name", b);
		BCCX_SetInt(n, "value", i++);
		lst=BCCX_AddEnd(lst, n);

		s=BGBCP_Token(s, b, &ty);
		if(!strcmp(b, ";"))break;
		if(!strcmp(b, "}"))break;

		if(strcmp(b, ","))
		{
			BGBCP_Error(s, "PDSCR_CParse_EnumVarsList: "
				"Invalid token %s in vars list\n", b);
			return(NULL);
		}
	}

	*str=s;
	return(lst);
}

