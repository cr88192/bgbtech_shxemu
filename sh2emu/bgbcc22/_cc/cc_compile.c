#include <bgbccc.h>


// extern BCCX_Node *bgbcp_structs;
// extern BCCX_Node *bgbcp_types;

char *bgbcc_lfn;
int bgbcc_lln;
int bgbcc_reqlfn=0;
static int bgbcc_seq=1;

int bgbcc_warn;
int bgbcc_err;

void BGBCC_Warn(char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	bgbcc_warn++;
	printf("%s:%d: warning: ", bgbcc_lfn, bgbcc_lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_Error(char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	bgbcc_err++;
	printf("%s:%d: error: ", bgbcc_lfn, bgbcc_lln);
	vprintf(str, lst);

	va_end(lst);
}

char *BGBCC_GenSym()
{
	char buf[32];

	sprintf(buf, "GS%d", bgbcc_seq++);
	return(bgbcc_strdup(buf));
}

char *BGBCC_GenSym2()
{
	return(BGBCC_GenSym());
//	return(BS1_RStrDup(BASM_GenSym()));
}

void bgbcc_puts(BGBCC_State *ctx, char *str)
{
	int i, j;

	if(!ctx->ip)
	{
		ctx->ips=malloc(16384);
		ctx->ipe=ctx->ips+16384;
		ctx->ip=ctx->ips;
	}

	if((ctx->ip+strlen(str))>=ctx->ipe)
	{
		i=ctx->ipe-ctx->ips;
		j=ctx->ip-ctx->ips;

		while(i<=(j+strlen(str)))i=i+(i>>1);

		ctx->ips=realloc(ctx->ips, i);
		ctx->ipe=ctx->ips+i;
		ctx->ip=ctx->ips+j;
	}

	strcpy(ctx->ip, str);
	ctx->ip+=strlen(str);
}

void bgbcc_print(BGBCC_State *ctx, char *str, ...)
{
	static char tbuf[4096];
	va_list lst;

	va_start(lst, str);

	vsprintf(tbuf, str, lst);
	bgbcc_puts(ctx, tbuf);

	va_end(lst);
}


int BGBCC_CompileSwitch(BGBCC_State *ctx, BCCX_Node *l)
{
	char **cl, *s;
	int i, j, ncl;
	BCCX_Node *c, *t, *u, *v;

	cl=bgbcc_malloc(4096*sizeof(char *));

	ncl=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case") ||
			BCCX_TagIsP(c, "case_default"))
		{
			cl[ncl++]=BGBCC_GenSym();
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}

	BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "cond"));

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case"))
		{
			bgbcc_print(ctx, "dup ");
			BGBCC_CompileExpr(ctx, BCCX_Fetch(c, "value"));
			BGBCC_CompileJmpCond(ctx, "==", cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_CompileJmp(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}


	s=BGBCC_GenSym();
	ctx->breakstack[ctx->breakstackpos++]=s;
	BGBCC_CompileBreak(ctx);

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case") ||
			BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_EmitLabel(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		BGBCC_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_EmitLabel(ctx, s);
	bgbcc_print(ctx, "pop ");

	bgbcc_free(cl);
	ctx->breakstackpos--;
	return(0);
}

void BGBCC_CompileStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	char *s0, *s1, *s2;
	int i, j, k;

	bgbcc_print(ctx, "\n");

	s0=BCCX_Get(l, "fn");
	if(s0 && (s0!=bgbcc_lfn))
	{
		if(!bgbcc_reqlfn)
			bgbcc_print(ctx, "$%s fn ", BGBCC_CIfy(s0));
		bgbcc_lfn=s0;
	}

	i=BCCX_GetInt(l, "ln");
	if((i>0) && (i!=bgbcc_lln))
	{
		if(!bgbcc_reqlfn)bgbcc_print(ctx, "%d ln ", i);
		bgbcc_lln=i;
	}

	if(bgbcc_reqlfn)
	{
		bgbcc_print(ctx, "$%s fn ", BGBCC_CIfy(bgbcc_lfn));
		bgbcc_print(ctx, "%d ln ", bgbcc_lln);
		bgbcc_reqlfn=0;
	}

	l=BGBCC_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "switch"))
	{
		BGBCC_CompileSwitch(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "goto"))
	{
		BGBCC_CompileJmp(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "label"))
	{
		BGBCC_EmitLabel(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "continue"))
	{
		s0=ctx->contstack[ctx->contstackpos-1];
		BGBCC_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "break"))
	{
		s0=ctx->breakstack[ctx->breakstackpos-1];
		BGBCC_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "return"))
	{
		t=BCCX_Fetch(l, "value");
		if(!t)
		{
			bgbcc_print(ctx, "ret_v ");
			return;
		}

		BGBCC_CompileExpr(ctx, t);
		bgbcc_print(ctx, "ret ");
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		s0=BCCX_Get(l, "op");

		if(s0)
		{
			BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
			BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
			bgbcc_print(ctx, "%s ", s0);
			BGBCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
			return;
		}

		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		BGBCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc") || BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			bgbcc_print(ctx, "$%s inc_s ", s0);
			return;
		}

		BGBCC_CompileExpr(ctx, t);
		bgbcc_print(ctx, "inc ");
		BGBCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec") || BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			bgbcc_print(ctx, "$%s dec_s ", s0);
			return;
		}

		BGBCC_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dec ");
		BGBCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "defun"))	{ return; }
	if(BCCX_TagIsP(l, "vars"))	{ return; }

#if 0
	if(BCCX_TagIsP(l, "defun"))	{ return; }

	if(BCCX_TagIsP(l, "vars"))
	{
		c=CDR(l);
		while(BS1_CONSP(c))
		{
			n=CAR(c);
			v=BS1_MM_NULL;

			if(BS1_CONSP(CAR(n)))
			{
				v=CADR(n);
				n=CAR(n);
			}

			t=CDR(n);
			n=CAR(n);

			i=TOINT(CADR(t));

			if((v!=BS1_MM_NULL) && !(i&BGBCC_TYFL_STATIC))
			{
				BGBCC_CompileExpr(ctx, v);
				BGBCC_CompileAssign(ctx, n);
			}

//			BGBCC_EmitVar(ctx, n, t, v);

			c=CDR(c);
		}
		return;
	}
#endif

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_CompileStatement(ctx, t);
			return;
		}

		s0=BGBCC_GenSym();
		BGBCC_CompileJCF(ctx, t, s0);

		t=BCCX_Fetch(l, "else");
		BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "then"));
		if(t)
		{
			s1=BGBCC_GenSym();
			BGBCC_CompileJmp(ctx, s1);
			BGBCC_EmitLabel(ctx, s0);
			BGBCC_CompileStatement(ctx, t);
			s0=s1;
		}
		BGBCC_EmitLabel(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_CompileStatement(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "for"))
	{
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();
		s2=BGBCC_GenSym();

		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "init"));

//		BGBCC_EmitLabel(ctx, s1);
		BGBCC_EmitLabel(ctx, s0);
		BGBCC_CompileJCF(ctx, BCCX_Fetch(l, "cond"), s2);

		BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		BGBCC_EmitLabel(ctx, s1);
		BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "step"));
//		BGBCC_CompileContinue(ctx);
		BGBCC_CompileJmp(ctx, s0);

		BGBCC_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "while"))
	{
		s1=BGBCC_GenSym();
		s2=BGBCC_GenSym();

		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_EmitLabel(ctx, s1);

		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);

		if(i==0)return;

		if(i==1)
		{
			BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));
			BGBCC_CompileContinue(ctx);
			BGBCC_EmitLabel(ctx, s2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_CompileJCF(ctx, t, s2);
		BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));
		BGBCC_CompileContinue(ctx);
		BGBCC_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}


	if(BCCX_TagIsP(l, "do_while"))
	{
		s1=BGBCC_GenSym();
		s2=BGBCC_GenSym();
		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_EmitLabel(ctx, s1);
		BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==0)return;

		if(i==1)
		{
			BGBCC_CompileContinue(ctx);
			BGBCC_EmitLabel(ctx, s2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_CompileJCT(ctx, t, s1);
		BGBCC_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "try_catch"))
	{
//		s1=BGBCC_GenSym();
//		s2=BGBCC_GenSym();
//		ctx->contstack[ctx->contstackpos++]=s1;
//		ctx->breakstack[ctx->breakstackpos++]=s2;

		//lame: for now just spit out try block...
		BGBCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		return;
	}

	if(BCCX_TagIsP(l, "msvc_asm"))
	{
		BGBCC_Warn("inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "gcc_asm"))
	{
		BGBCC_Warn("inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "linecomment")) { return; }
	if(BCCX_TagIsP(l, "blockcomment")) { return; }

	BGBCC_CompileExpr(ctx, l);
	bgbcc_print(ctx, "pop ");
}


char *BGBCC_VarTypeString_FlattenExpr(BGBCC_State *ctx,
	char *dst, BCCX_Node *l, int fl)
{
	BCCX_Node *c, *n;
	char *s, *t;
	
	t=dst;
	
	if(BCCX_TagIsP(l, "ref"))
	{
		s=BCCX_Get(l, "name");
		t=BGBCC_VarTypeString_FlattenName(ctx, dst, s, fl);
		return(t);
	}

	if(BCCX_TagIsP(l, "emptyindex"))
	{
		*t++='Q';
		n=BCCX_Fetch(l, "array");
		t=BGBCC_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsP(l, "sharpcall"))
	{
		*t++='<';
		c=BCCX_Fetch(l, "args");
		while(c)
		{
			t=BGBCC_VarTypeString_FlattenExpr(ctx, t, c, fl);
			c=BCCX_Next(c);
		}
		*t++='>';
		n=BCCX_Fetch(l, "value");
		t=BGBCC_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsP(l, "anytype"))
	{
		*t++='C';
		*t++='z';
		return(t);
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		s=BCCX_Get(l, "op");

		if(!strcmp(s, "..."))
		{
			*t++='C';
			*t++='z';
			return(t);
		}

		if(!strcmp(s, "*"))*t++='P';
		if(!strcmp(s, "&"))*t++='R';
		if(!strcmp(s, "^"))*t++='P';

		n=BCCX_Fetch(l, "value");
		t=BGBCC_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}
	
	return(t);
}

char *BGBCC_VarTypeString_FlattenName(BGBCC_State *ctx,
	char *t, char *s, int i)
{
	BCCX_Node *c, *n;
	char *t1;
	int lang;

	t1=t;

	if(ctx)lang=ctx->lang;
		else lang=BGBCC_LANG_C;

	if((lang==BGBCC_LANG_C) || (lang==BGBCC_LANG_CPP))
	{
		if(i&BGBCC_TYFL_UNSIGNED)
		{
			if(!strcmp(s, "void"))*t++='v';
			if(!strcmp(s, "char"))*t++='h';
			if(!strcmp(s, "short"))*t++='t';
			if(!strcmp(s, "int"))*t++='j';
			if(!strcmp(s, "long"))*t++='m';
			if(!strcmp(s, "llong"))*t++='y';
			if(!strcmp(s, "int128"))*t++='o';

			if(!strcmp(s, "float"))
				{ *t++='C'; *t++='f'; }
			if(!strcmp(s, "double"))
				{ *t++='C'; *t++='d'; }
			if(!strcmp(s, "float128"))
				{ *t++='C'; *t++='n'; }
			if(!strcmp(s, "float16"))
				{ *t++='C'; *t++='k'; }
		}
	}

	if(t==t1)
	{
		if(!strcmp(s, "void"))*t++='v';
		if(!strcmp(s, "short"))*t++='s';
		if(!strcmp(s, "int"))*t++='i';
		if(!strcmp(s, "float"))*t++='f';
		if(!strcmp(s, "double"))*t++='d';

		if((lang==BGBCC_LANG_C) || (lang==BGBCC_LANG_CPP))
		{
			if(!strcmp(s, "char"))*t++='c';
			if(!strcmp(s, "long"))*t++='l';
			if(!strcmp(s, "llong"))*t++='x';
		}else if((lang==BGBCC_LANG_CS) ||
			(lang==BGBCC_LANG_BS2))
		{
			if(!strcmp(s, "char"))*t++='w';

//			if(!strcmp(s, "object"))*t++='r';
//			if(!strcmp(s, "string"))*t++='r';

			if(!strcmp(s, "object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "string"))
				{ *t++='C'; *t++='s'; }

			if(!strcmp(s, "bool"))*t++='b';

			if(!strcmp(s, "sbyte"))*t++='c';
			if(!strcmp(s, "byte"))*t++='h';
			if(!strcmp(s, "ushort"))*t++='t';
			if(!strcmp(s, "uint"))*t++='j';

			if(!strcmp(s, "long"))*t++='x';
			if(!strcmp(s, "ulong"))*t++='y';

			if(!strcmp(s, "decimal"))*t++='g';
		}else if(lang==BGBCC_LANG_JAVA)
		{
			if(!strcmp(s, "char"))*t++='w';
			if(!strcmp(s, "long"))*t++='l';

			if(!strcmp(s, "byte"))*t++='c';
			if(!strcmp(s, "boolean"))*t++='b';

			if(!strcmp(s, "Object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "String"))
				{ *t++='C'; *t++='t'; }
		}else
		{
			if(!strcmp(s, "char"))*t++='c';
			if(!strcmp(s, "long"))*t++='l';
			if(!strcmp(s, "llong"))*t++='x';
		}

		if(!strcmp(s, "int128"))*t++='n';
		if(!strcmp(s, "float128"))*t++='g';
		if(!strcmp(s, "float16"))*t++='k';

		if(!strcmp(s, "uint128"))*t++='o';

		if(!strcmp(s, "variant"))*t++='r';
		if(!strcmp(s, "variantf"))
			{ *t++='C'; *t++='r'; }

		if(!strcmp(s, "variant_this"))
			{ *t++='C'; *t++='x'; }
		if(!strcmp(s, "variant_cls"))
			{ *t++='C'; *t++='y'; }
		if(!strcmp(s, "variant_va"))
			{ *t++='C'; *t++='z'; }

		if(!strcmp(s, "vec2"))
			{ *t++='C'; *t++='a'; }
		if(!strcmp(s, "vec3"))
			{ *t++='C'; *t++='b'; }
		if(!strcmp(s, "vec4"))
			{ *t++='C'; *t++='c'; }
		if(!strcmp(s, "quat"))
			{ *t++='C'; *t++='q'; }

		if(!strcmp(s, "vec2d"))
			{ *t++='D'; *t++='a'; }
		if(!strcmp(s, "vec3d"))
			{ *t++='D'; *t++='b'; }
		if(!strcmp(s, "vec4d"))
			{ *t++='D'; *t++='c'; }
		if(!strcmp(s, "quatd"))
			{ *t++='D'; *t++='q'; }
	}

	if(ctx && (t==t1))
	{
		c=ctx->structs;
		while(c)
		{
			if(BCCX_AttrIsP(c, "name", s))
			{
				sprintf(t, "X%s;", s);
				t+=strlen(t);
				break;
			}
			c=BCCX_Next(c);
		}
	}

	if(t==t1)
	{
		sprintf(t, "U%s;", s);
		t+=strlen(t);
	}
		
	return(t);
}

int BGBCC_VarTypeString_ModifierChar(BGBCC_State *ctx, int i)
{
	int c;

	if(!i)return(0);

	if(ctx->arch==BGBCC_ARCH_X86)
	{
		switch(i)
		{
		case BGBCC_TYFL_CDECL: c='c'; break;
		case BGBCC_TYFL_STDCALL: c='w'; break;
		default: c=0; break;
		}
		if(c)return(c);
	}
	if(ctx->arch==BGBCC_ARCH_X64)
	{
		switch(i)
		{
		case BGBCC_TYFL_WIN64: c='w'; break;
		case BGBCC_TYFL_AMD64: c='f'; break;
		default: c=0; break;
		}
		if(c)return(c);
	}

	switch(i)
	{
	case BGBCC_TYFL_ABSTRACT: c='a'; break;
	case BGBCC_TYFL_BIGENDIAN: c='b'; break;
	//c (cdecl)
	case BGBCC_TYFL_DELEGATE: c='d'; break;
	case BGBCC_TYFL_EVENT: c='e'; break;
	//f (fastcall/amd64)
	case BGBCC_TYFL_GETTER: c='g'; break;
	case BGBCC_TYFL_SETTER: c='h'; break;
	case BGBCC_TYFL_INLINE: c='i'; break;
	case BGBCC_TYFL_FINAL: c='j'; break;
	case BGBCC_TYFL_CONST: c='k'; break;
	case BGBCC_TYFL_LTLENDIAN: c='l'; break;
	//m
	case BGBCC_TYFL_NATIVE: c='n'; break;
	case BGBCC_TYFL_OVERRIDE: c='o'; break;
	case BGBCC_TYFL_PUBLIC: c='p'; break;
	case BGBCC_TYFL_PRIVATE: c='q'; break;
	case BGBCC_TYFL_PROTECTED: c='r'; break;
	case BGBCC_TYFL_STATIC: c='s'; break;
	//t (thiscall)
	//u (special)
	case BGBCC_TYFL_VIRTUAL: c='v'; break;
	//w (stdcall/win64)
	case BGBCC_TYFL_XCALL: c='x'; break;
	case BGBCC_TYFL_TRANSIENT: c='y'; break;
	//z

//	case BGBCC_TYFL_TRANSIENT: c=('C'<<8)|'t'; break;

//	case BGBCC_TYFL_ASYNC: c=('C'<<8)|'a'; break;
	case BGBCC_TYFL_BYREF: c=('C'<<8)|'b'; break;
	case BGBCC_TYFL_INTERFACE: c=('C'<<8)|'i'; break;

	case BGBCC_TYFL_SYNCHRONIZED: c=('C'<<8)|'s'; break;
	case BGBCC_TYFL_THREAD: c=('D'<<8)|'t'; break;
	default: c=0; break;
	}
	return(c);
}

char *BGBCC_VarTypeString_FlattenModifiers(BGBCC_State *ctx,
	char *t, int fl)
{
	int i, j, k;

	k=0;
	for(i=0; i<30; i++)
	{
		j=BGBCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)k++;
	}
	
	if(k>2)
	{
		*t++='M';
		for(i=0; i<31; i++)
		{
			j=BGBCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
//			if(j)*t++=j;
			if(j)
			{
				if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
				if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
				if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
				if(j&0xFF)*t++=j&0xFF;
			}
		}
		*t++=';';
		
		return(t);
	}

	for(i=0; i<31; i++)
	{
		j=BGBCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)
		{
			*t++='F';
//			*t++=j;
			if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
			if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
			if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
			if(j&0xFF)*t++=j&0xFF;
		}
	}
	return(t);
}

char *BGBCC_VarTypeString_FlattenModifiers2(BGBCC_State *ctx,
	char *t, int fl)
{
	int i, j, k;

	for(i=0; i<31; i++)
	{
		j=BGBCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)
		{
			if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
			if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
			if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
			if(j&0xFF)*t++=j&0xFF;
		}
	}
	return(t);
}

char *BGBCC_VarTypeString(BGBCC_State *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsP(ty, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)return(bgbcc_strdup("v"));

	if(!BCCX_TagIsP(ty, "type"))
		return(bgbcc_strdup("v"));

	if(ty)
	{
#if 1
		c=BCCX_Fetch(ty, "size");
		if(c)
		{
			*t++='A';
			while(c)
			{
				n=BGBCC_ReduceForm(ctx, c);

				if(!BCCX_TagIsP(n, "int"))
				{
					BGBCC_Error("Invalid array size specifier\n");
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetInt(n, "value");
				sprintf(t, "%d", i);
				t+=strlen(t);

				c=BCCX_Next(c);
				if(c)*t++=',';
			}
			*t++=';';
		}
#endif

		i=BCCX_GetInt(ty, "flags");
		s=BCCX_Get(ty, "name");

//		if(ctx && (ctx->cur_struct || (ctx->lang!=BGBCC_LANG_C)))
//		{
//			t=BGBCC_VarTypeString_FlattenModifiers(ctx, t, i);
//		}

		if(i&BGBCC_TYFL_WIDE)
		{
//			*t++='W';
			i=BCCX_GetInt(ty, "ind");
			while(i--)*t++='W';
		}else
		{
			i=BCCX_GetInt(ty, "ind");
			while(i--)*t++='P';
		}

		i=BCCX_GetInt(ty, "flags");

		n=BCCX_Fetch(ty, "expr");
		if(n)
		{
			t=BGBCC_VarTypeString_FlattenExpr(ctx, t, n, i);
//			*t=0;
//			return(bgbcc_strdup(buf));
		}else
		{
			if(s)
			{
				t=BGBCC_VarTypeString_FlattenName(ctx, t, s, i);
			}else
			{
				*t++='v';
			}
		}

#if 0
//		if(lang==BGBCC_LANG_C)
		if(0)
		{
			c=BCCX_Fetch(ty, "size");
			while(c)
			{
				n=BGBCC_ReduceForm(ctx, c);

				if(!BCCX_TagIsP(n, "int"))
				{
					BGBCC_Error("Invalid array size specifier\n");
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetInt(n, "value");
				sprintf(t, "%d", i);
				t+=strlen(t);

				c=BCCX_Next(c);
				if(c)*t++=',';
			}
		}
#endif
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_VarTypeFlagsString(BGBCC_State *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsP(ty, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)return(bgbcc_strdup(""));

	if(!BCCX_TagIsP(ty, "type"))
		return(bgbcc_strdup(""));

	if(ty)
	{
		i=BCCX_GetInt(ty, "flags");
		s=BCCX_Get(ty, "name");

		t=BGBCC_VarTypeString_FlattenModifiers2(ctx, t, i);
	}
	*t=0;

	return(bgbcc_strdup(buf));
}

void BGBCC_EmitVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1;
	int i;

	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;

	if(!s)s="v";

	if(!s1)s1="_";
	if(*s)bgbcc_print(ctx, "%s:%s ", s1, s);
		else bgbcc_print(ctx, "%s ", s1);

	if(v)
	{
		bgbcc_print(ctx, "= ");
		BGBCC_EmitVarValueR(ctx, v);
	}
}

void BGBCC_EmitVar2(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;

	if(!s)s="v";

	i=BCCX_GetInt(ty, "flags");
	s2=((i&BGBCC_TYFL_STATIC) && !ctx->cur_struct)?"~":"";

	if(!s1)s1="_";
	if(*s)bgbcc_print(ctx, "%s%s:%s ", s1, s2, s);
		else bgbcc_print(ctx, "%s%s ", s1, s2);

	if(v)
	{
		bgbcc_print(ctx, "= ");
		BGBCC_EmitVarValueR(ctx, v);
	}

	if(ctx->cur_struct)
	{
		s2=BGBCC_VarTypeFlagsString(ctx, ty);

		BGBCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"field", ctx->cur_idx++, s1);

		BGBCC_BindFieldSig(ctx, ctx->cur_struct, s1, s);
		BGBCC_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
		if(s2 && (*s2))
			BGBCC_BindFieldInfo(ctx, ctx->cur_struct, s1, "flagstr", s2);
	}
}

void BGBCC_EmitVar3(BGBCC_State *ctx, char *name, char *ty)
{
	char *s, *s1;
	int i;

	s=ty;
	s1=name;

	if(!s)s="v";

	if(!s1)s1="_";
	if(*s)bgbcc_print(ctx, "%s:%s ", s1, s);
		else bgbcc_print(ctx, "%s ", s1);

	if(ctx->cur_struct)
	{
		BGBCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"method", ctx->cur_idx2++, s1);

		BGBCC_BindFieldSig(ctx, ctx->cur_struct, s1, s);
//		BGBCC_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
	}
}

void BGBCC_EmitVarProperty(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");

	n=BCCX_New1("var", t);
	BCCX_Set(n, "name", "value");

	v=BCCX_FindTag(l, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_GenProtoSig(ctx, t, NULL);
		BGBCC_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_GenProtoSig(ctx, NULL, n);
		BGBCC_EmitVar3(ctx, s2, s1);
	}

//	BGBCC_EmitVar3(ctx, "new", s1);
//	BGBCC_CompileBlock(ctx, t, s, n, v);

	if(ctx->cur_struct)
	{
		s1=BGBCC_VarTypeString(ctx, t);
		s2=BGBCC_VarTypeFlagsString(ctx, t);
		i=BCCX_GetInt(t, "flags");

		BGBCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"property", ctx->cur_idx3++, s);

		BGBCC_BindFieldSig(ctx, ctx->cur_struct, s, s1);
		BGBCC_BindFieldInfoI(ctx, ctx->cur_struct, s, "flags", i);
		if(s2 && (*s2))
			BGBCC_BindFieldInfo(ctx, ctx->cur_struct, s, "flagstr", s2);
	}
}

void BGBCC_CompileVarProperty(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

#if 1
	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");

	n=BCCX_New1("var", t);
	BCCX_Set(n, "name", "value");

	v=BCCX_FindTag(l, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_CompileBlock(ctx, t, s2, NULL, v);

//		s1=BGBCC_GenProtoSig(ctx, t, NULL);
//		BGBCC_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_CompileBlock(ctx, NULL, s2, n, v);

//		s1=BGBCC_GenProtoSig(ctx, NULL, n);
//		BGBCC_EmitVar3(ctx, s2, s1);
	}

//	BGBCC_EmitVar3(ctx, "new", s1);
//	BGBCC_CompileBlock(ctx, t, s, n, v);
#endif

}

int BGBCC_CompileVarDummyP(BGBCC_State *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "assign") ||
		BCCX_TagIsP(l, "if") ||
		BCCX_TagIsP(l, "do_while") ||
		BCCX_TagIsP(l, "for") ||
		BCCX_TagIsP(l, "methodcall") ||
		BCCX_TagIsP(l, "while") ||
		BCCX_TagIsP(l, "return") ||
		BCCX_TagIsP(l, "switch") ||
		BCCX_TagIsP(l, "funcall"))
		{ return(1); }

	if(BCCX_TagIsP(l, "linecomment")) { return(1); }
	if(BCCX_TagIsP(l, "blockcomment")) { return(1); }

	return(0);
}

void BGBCC_CompileVarStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1;
	int i;

	s=BCCX_Get(l, "fn");
	if(s)bgbcc_lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsP(c, "var"))
				{ c=BCCX_Next(c); continue; }

			s=BCCX_Get(c, "name");
			t=BCCX_FindTag(c, "type");
			//v=BCCX_Fetch(c, "value");
			v=NULL;

			BGBCC_EmitVar2(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_GenProtoSig(ctx, t, n);
		BGBCC_EmitVar3(ctx, s, s1);
//		BGBCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_GenProtoSig(ctx, NULL, n);
		BGBCC_EmitVar3(ctx, "new", s1);
//		BGBCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_EmitVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CompileVarDummyP(ctx, l))
		return;

	BGBCC_Warn("unhandled var statement %s\n", BCCX_Tag(l));
}

void BGBCC_CompileVarStatement2(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s;
	int i;

	s=BCCX_Get(l, "fn");
	if(s)bgbcc_lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsP(c, "var"))
				{ c=BCCX_Next(c); continue; }

			s=BCCX_Get(c, "name");
			t=BCCX_FindTag(c, "type");
			v=BCCX_Fetch(c, "value");

//			BGBCC_EmitVar2(ctx, s, t, v);
			BGBCC_EmitTopVar(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		s="new";
		t=NULL;
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_CompileVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CompileVarDummyP(ctx, l))
		return;

	BGBCC_Warn("unhandled var statement %s\n", BCCX_Tag(l));
}

BCCX_Node *BGBCC_CompileBlock(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	char *cname[16], *bname[16];

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

	name=BGBCC_QualifyNameNS(ctx, name);

	BGBCC_EmitSigProto(ctx, type, name, args);

	ctx->contstack=cname;
	ctx->breakstack=bname;

	ctx->cf_n=name;
	ctx->cf_ty=type;

	i=0;

	//hack: disable extern inline bastards...
	if(type)i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_EXTERN)
		return(NULL);

	if(BCCX_TagIsP(body, "begin"))
		body=BCCX_Child(body);

	if(ctx->cur_struct)
		bgbcc_print(ctx, "METHOD ");
	else bgbcc_print(ctx, "FUNC ");

	BGBCC_EmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);

	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			bgbcc_print(ctx, "... ");
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	if(!ctx->cur_struct)
	{
		i=0;
		if(type)i=BCCX_GetInt(type, "flags");
		if(ctx->arch==BGBCC_ARCH_X86)
		{
			if(i&BGBCC_TYFL_STDCALL)bgbcc_print(ctx, "$stdcall ");
			if(i&BGBCC_TYFL_CDECL)bgbcc_print(ctx, "$cdecl ");
		}
		if(ctx->arch==BGBCC_ARCH_X64)
		{
			if(i&BGBCC_TYFL_WIN64)bgbcc_print(ctx, "$win64 ");
			if(i&BGBCC_TYFL_AMD64)bgbcc_print(ctx, "$amd64 ");
		}
		if(i&BGBCC_TYFL_XCALL)bgbcc_print(ctx, "$xcall ");

		if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
		if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");
		if(i&BGBCC_TYFL_PROXY)bgbcc_print(ctx, "$proxy ");
	}

	bgbcc_print(ctx, "VARS ");

	c=body;
	while(c)
	{
		BGBCC_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}


	bgbcc_print(ctx, "BODY ");
	bgbcc_print(ctx, "\n");

	bgbcc_reqlfn=1;

	c=body;
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_CompileStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}

	bgbcc_print(ctx, "ret_v ");
	bgbcc_print(ctx, "END\n\n");

	return(NULL);
}

char *BGBCC_GenProtoSig(BGBCC_State *ctx,
	BCCX_Node *type, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

//	if(!type)return(NULL);

	c=args; t=tb;
	*t++='(';
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			*t++='z';
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTag(c, "type");
		if(n)
		{
			s=BGBCC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	if(type)
	{
		s=BGBCC_VarTypeString(ctx, type);
		if(s) { strcpy(t, s); t+=strlen(t); }
	}else
	{
		*t++='v';
	}

	*t++=0;

	return(bgbcc_strdup(tb));
}

void BGBCC_EmitSigProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

//	return;

//	printf("BGBCC_EmitSigProto(A) %s\n", name);

	if(!type)return;

//	printf("BGBCC_EmitSigProto(B) %s\n", name);

	i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_STATIC)return;

	c=args; t=tb;
	*t++='(';
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			*t++='z';
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTag(c, "type");
		if(n)
		{
			s=BGBCC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

	BGBCC_BindSig(ctx, name, tb);

	s=BGBCC_VarTypeFlagsString(ctx, type);
	if(s && (*s))
			BGBCC_BindVarInfo(ctx, name, "flagstr", s);
}

BCCX_Node *BGBCC_CompileProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

//	printf("BGBCC_CompileProto %s\n", name);

	name=BGBCC_QualifyNameNS(ctx, name);

	BGBCC_EmitSigProto(ctx, type, name, args);

	ctx->cf_n=name;
	ctx->cf_ty=type;

	bgbcc_print(ctx, "PROTO ");
	BGBCC_EmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);

	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			bgbcc_print(ctx, "... ");
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	i=BCCX_GetInt(type, "flags");
	if(ctx->arch==BGBCC_ARCH_X86)
	{
		if(i&BGBCC_TYFL_STDCALL)bgbcc_print(ctx, "$stdcall ");
		if(i&BGBCC_TYFL_CDECL)bgbcc_print(ctx, "$cdecl ");
	}
	if(ctx->arch==BGBCC_ARCH_X64)
	{	
		if(i&BGBCC_TYFL_WIN64)bgbcc_print(ctx, "$win64 ");
		if(i&BGBCC_TYFL_AMD64)bgbcc_print(ctx, "$amd64 ");
	}
	if(i&BGBCC_TYFL_XCALL)bgbcc_print(ctx, "$xcall ");

	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");
	if(i&BGBCC_TYFL_PROXY)bgbcc_print(ctx, "$proxy ");

	bgbcc_print(ctx, "END\n");

	return(NULL);
}

void BGBCC_CompileStruct(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *t, *n;
	char *s, *os0, *s1;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
	BGBCC_BindStructSig(ctx, s, "struct");
	BGBCC_BindStructInfoI(ctx, s, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	bgbcc_print(ctx, "STRUCT %s ", BCCX_Get(l, "name"));
	bgbcc_print(ctx, "\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_CompileVarStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
	bgbcc_print(ctx, "\tEND\n");

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CompileUnion(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
	BGBCC_BindStructSig(ctx, s, "union");
	BGBCC_BindStructInfoI(ctx, s, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	bgbcc_print(ctx, "UNION %s ", BCCX_Get(l, "name"));
	bgbcc_print(ctx, "\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_CompileVarStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
	bgbcc_print(ctx, "\tEND\n");

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CompileClass(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *t, *n;
	char *s, *os0, *qn;
	int i, j, j1, j2;

	s=BCCX_Get(l, "name");
	qn=BGBCC_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
	BGBCC_BindStructSig(ctx, qn, "class");
	BGBCC_BindStructInfoI(ctx, qn, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=qn;
	j=ctx->cur_idx;
	j1=ctx->cur_idx2;
	j2=ctx->cur_idx3;

	ctx->cur_idx=0;
	ctx->cur_idx2=0;
	ctx->cur_idx3=0;

	bgbcc_print(ctx, "CLASS %s ", qn);

	c=BCCX_Fetch(l, "super");
	if(c)
	{
		bgbcc_print(ctx, "EXTENDS ");
		while(c)
		{
			s=BGBCC_VarTypeString(ctx, c);
			if(s)bgbcc_print(ctx, "%s", s);
			c=BCCX_Next(c);
		}
		bgbcc_print(ctx, " ");
	}

	c=BCCX_Fetch(l, "impl");
	if(c)
	{
		bgbcc_print(ctx, "IMPLEMENTS ");
		while(c)
		{
			s=BGBCC_VarTypeString(ctx, c);
			if(s)bgbcc_print(ctx, "%s", s);
			c=BCCX_Next(c);
		}
		bgbcc_print(ctx, " ");
	}

	bgbcc_print(ctx, "\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_CompileVarStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
	bgbcc_print(ctx, "\tEND\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CompileVarStatement2(ctx, c);
		c=BCCX_Next(c);
	}

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
	ctx->cur_idx2=j1;
	ctx->cur_idx3=j2;
}

#if 0
void BGBCC_CompileEnum(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *v;

	c=BCCX_Fetch(l, "body");
	while(BS1_CONSP(c))
	{
		n=CAAR(c);
		v=CADAR(c);
		t=BS1_MM_NULL;

		SET(ctx->mlenv, CONS(n, ctx->mlenv));
		SET(ctx->mtenv, CONS(t, ctx->mtenv));
		SET(ctx->mvenv, CONS(v, ctx->mvenv));

		c=CDR(c);
	}
}
#endif

BCCX_Node *BGBCC_CompileSProto(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u;
	char *s, *s1;

	bgbcc_print(ctx, "S_PROTO ");

	t=BCCX_FindTag(l, "type");
	BGBCC_EmitVar(ctx, BCCX_Get(l, "name"), t, NULL);

	c=BCCX_Fetch(l, "args");
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
			{ bgbcc_print(ctx, "... "); c=BCCX_Next(c); continue; }
		if(!BCCX_TagIsP(c, "var"))
			{ c=BCCX_Next(c); continue; }

		t=BCCX_FindTag(c, "type");
		BGBCC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	bgbcc_print(ctx, "END\n");


	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");
	u=BCCX_Fetch(l, "args");

	s1=BGBCC_GenProtoSig(ctx, t, u);
	BGBCC_BindStructSig(ctx, s, "func");
	BGBCC_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

BCCX_Node *BGBCC_GetNodeAttribute(BGBCC_State *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *cn, *cn1;
	char *s;

	cn=BCCX_Child(l);
	while(cn)
	{
		if(!BCCX_TagIsP(cn, "declspec") &&
			!BCCX_TagIsP(cn, "attribute"))
				{ cn=BCCX_Next(cn); continue; }

//		printf("BGBCC_GetNodeAttribute A\n");

		cn1=BCCX_Child(cn);
		while(cn1)
		{
			if(!BCCX_TagIsP(cn1, "attr"))
				{ cn1=BCCX_Next(cn1); continue; }

			s=BCCX_Get(cn1, "name");
//			printf("BGBCC_GetNodeAttribute B: %s\n", s);

			if(s && !strcmp(s, name))
				return(cn1);

			cn1=BCCX_Next(cn1);
		}
		cn=BCCX_Next(cn);
	}
	
	return(NULL);
}

char *BGBCC_GetNodeAttributeString(BGBCC_State *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *attr, *cn;
	char *s;
	
	attr=BGBCC_GetNodeAttribute(ctx, l, name);
	if(!attr)return(NULL);

	s=BCCX_Get(attr, "name");
//	printf("BGBCC_GetNodeAttributeString: %s\n", s);

	cn=BCCX_Fetch(attr, "args");
	if(!cn)return(NULL);

	if(!BCCX_TagIsP(cn, "string"))
		return(NULL);
//	s=BCCX_Get(cn, "text");
	s=BCCX_Get(cn, "value");
	return(s);
}

void BGBCC_CompileTypedef(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

//	s=BCCX_Get(l, "fn");
//	if(s)bgbcc_lfn=s;
//	i=BCCX_GetInt(l, "ln");
//	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "var"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
//		v=BCCX_Fetch(l, "value");

		s1=BGBCC_VarTypeString(ctx, t);
		s2=BGBCC_VarTypeFlagsString(ctx, t);
		BGBCC_BindTypeSig(ctx, s, s1);
		if(s2 && (*s2))
			BGBCC_BindTypeInfo(ctx, s, "flagstr", s2);

		s2=BGBCC_GetNodeAttributeString(ctx, t, "dyt_name");
//		if(!s2)s2=BGBCC_GetNodeAttributeString(ctx, t, "dytname");
		if(s2)
		{
			printf("BGBCC_CompileTypedef: DyType %s->%s\n", s, s2);
		
			BGBCC_BindTypeInfo(ctx, s, "dytype", s2);
			BGBCC_BindDyTypeSig(ctx, s2, s1);
		}

//		c=BCCX_Next(c);
	}

#if 0
	if(BCCX_TagIsP(l, "func"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		v=BCCX_FindTag(l, "args");

		s1=BGBCC_GenProtoSig(ctx, t, v);
		BGBCC_BindTypeSig(ctx, s, s1);
	}
#endif

	return;
}

void BGBCC_CompileStructs(BGBCC_State *ctx)
{
	BCCX_Node *c, *t, *n, *l;

	c=ctx->types;
	while(c)
	{
		BGBCC_CompileTypedef(ctx, c);
		c=BCCX_Next(c);
	}

	c=ctx->structs;
	while(c)
	{
		l=c;
		if(BCCX_TagIsP(c, "struct"))BGBCC_CompileStruct(ctx, c);
		if(BCCX_TagIsP(c, "union"))BGBCC_CompileUnion(ctx, c);
//		if(BCCX_TagIsP(c, "enum"))BGBCC_CompileEnum(ctx, c);
		if(BCCX_TagIsP(c, "s_proto"))BGBCC_CompileSProto(ctx, c);
		if(BCCX_TagIsP(c, "func"))BGBCC_CompileSProto(ctx, c);

		c=BCCX_Next(c);
	}
}

void BGBCC_EmitVarValue(BGBCC_State *ctx, BCCX_Node *v)
{
	if(BCCX_TagIsP(v, "int"))
	{
		bgbcc_print(ctx, "%s", BCCX_Get(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "real"))
	{
		bgbcc_print(ctx, "%s", BCCX_Get(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "string"))
	{
		bgbcc_print(ctx, "%s", BGBCC_CIfy(BCCX_Get(v, "value")));
		return;
	}

	BGBCC_Error("BGBCC_EmitVarValue: Bad Value %s\n", BCCX_Tag(v));
}

void BGBCC_EmitVarValueR(BGBCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_EmitVarValueR(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}

		return;
	}

	BGBCC_EmitVarValue(ctx, v);
}

void BGBCC_EmitVarValueR2(BGBCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		bgbcc_print(ctx, "LIST ");

		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_EmitVarValueR2(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}

		bgbcc_print(ctx, "END ");

		return;
	}

	BGBCC_EmitVarValue(ctx, v);
}

char *BGBCC_QualifyNameNS(BGBCC_State *ctx, char *name)
{
	char tb[256];
	char *s;

	if(ctx->cur_struct)
	{
		sprintf(tb, "%s/%s", ctx->cur_struct, name);
		s=bgbcc_strdup(tb);
		return(s);
	}

	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		s=bgbcc_strdup(tb);
		return(s);
	}

	return(name);
}

void BGBCC_EmitTopArray(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int ord, cnt;
	int i, j, k;

	s=BGBCC_VarTypeString(ctx, ty);
	//s1=name;
	s1=BGBCC_QualifyNameNS(ctx, name);

	i=BCCX_GetInt(ty, "flags");
	if(!(i&BGBCC_TYFL_STATIC))BGBCC_BindSig(ctx, s1, s);

	if(!s1)return;
	if(!s1)s1="_";

	if(v && BCCX_TagIsP(v, "list"))
	{
		bgbcc_print(ctx, "ARR ");
		i=BCCX_GetInt(ty, "flags");
		if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
		if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

		bgbcc_print(ctx, "%s:%s ", s1, s);

		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_EmitVarValueR(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}

		bgbcc_print(ctx, "END\n");

		return;
	}

	bgbcc_print(ctx, "VAR ");
	i=BCCX_GetInt(ty, "flags");
	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

	bgbcc_print(ctx, "%s:%s\n", s1, s);
}

void BGBCC_EmitTopVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int i, j;

	i=BCCX_GetInt(ty, "flags");
	if(!ctx->cur_struct && (i&BGBCC_TYFL_EXTERN))
	{
		s=BGBCC_VarTypeString(ctx, ty);
//		s1=name;
		s1=BGBCC_QualifyNameNS(ctx, name);
		s2=BGBCC_VarTypeFlagsString(ctx, ty);

		BGBCC_BindVarSig(ctx, s1, s);
		if(s2 && (*s2))
			BGBCC_BindVarInfo(ctx, s1, "flagstr", s2);

		if(!s1)return;
		if(!s1)s1="_";

		bgbcc_print(ctx, "EXTERN ");

		if(*s)bgbcc_print(ctx, "%s:%s", s1, s);
			else bgbcc_print(ctx, "%s", s1);

		bgbcc_print(ctx, "\n");
		return;
	}


	if(!ctx->cur_struct)
	{
		c=BCCX_Fetch(ty, "size"); i=0;
		while(c) { i++; c=BCCX_Next(c); }

		if(i)
		{
			BGBCC_EmitTopArray(ctx, name, ty, v);
			return;
		}
	}

	s=BGBCC_VarTypeString(ctx, ty);
	s2=BGBCC_VarTypeFlagsString(ctx, ty);
//	s1=name;
	s1=BGBCC_QualifyNameNS(ctx, name);

	i=BCCX_GetInt(ty, "flags");
//	if(!(i&BGBCC_TYFL_STATIC))
	BGBCC_BindVarSig(ctx, s1, s);
	if(s2 && (*s2))
			BGBCC_BindVarInfo(ctx, s1, "flagstr", s2);

	if(!s1)return;
	if(!s1)s1="_";

	if(!ctx->cur_struct)
	{
		bgbcc_print(ctx, "VAR ");

		i=BCCX_GetInt(ty, "flags");
		if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
		if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

		if(i&BGBCC_TYFL_CONST)bgbcc_print(ctx, "$const ");
		if(i&BGBCC_TYFL_FINAL)bgbcc_print(ctx, "$final ");
		if(i&BGBCC_TYFL_VOLATILE)bgbcc_print(ctx, "$volatile ");
		if(i&BGBCC_TYFL_NATIVE)bgbcc_print(ctx, "$native ");
	}else
	{
		bgbcc_print(ctx, "FIELD ");
	}

	if(*s)bgbcc_print(ctx, "%s:%s", s1, s);
		else bgbcc_print(ctx, "%s", s1);

	if(v)
	{
		bgbcc_print(ctx, " = ");
		BGBCC_EmitVarValueR2(ctx, v);
	}

	bgbcc_print(ctx, "\n");
}

void BGBCC_CompileTopStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1;
	int i;

//	printf("BGBCC_CompileTopStatement: %s\n", l->tag);

	s=BCCX_Get(l, "fn");
	if(s)bgbcc_lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(BCCX_TagIsP(c, "proto"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				n=BCCX_Fetch(c, "args");

				BGBCC_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsP(c, "var"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				v=BCCX_Fetch(c, "value");

				BGBCC_EmitTopVar(ctx, s, t, v);
			}

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "proto"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		BGBCC_CompileProto(ctx, t, s, n);
		return;
	}

	if(BCCX_TagIsP(l, "struct"))
	{
		BGBCC_CompileStruct(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "union"))
	{
		BGBCC_CompileUnion(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "class"))
	{
		BGBCC_CompileClass(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "import"))
	{
		s=BCCX_Get(l, "name");
		bgbcc_print(ctx, "IMPORT %s\n", s);
		return;
	}

	if(BCCX_TagIsP(l, "using"))
	{
		s=BCCX_Get(l, "name");
		bgbcc_print(ctx, "IMPORT %s\n", s);
		return;
	}

	if(BCCX_TagIsP(l, "package"))
	{
		s=BCCX_Get(l, "name");
		n=BCCX_FindTag(l, "body");
//		c=BCCX_Fetch(l, "body");

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

		BGBCC_BindStructSig(ctx, s, "namespace");
		bgbcc_print(ctx, "PACKAGE %s\n", s);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;

#if 1
		if(n)
		{
			c=BCCX_Child(n);
			while(c)
			{
				BGBCC_CompileTopStatement(ctx, c);
				c=BCCX_Next(c);
			}
			ctx->cur_ns=s1;
		}
#endif
		return;
	}

	if(BCCX_TagIsP(l, "namespace"))
	{
		s=BCCX_Get(l, "name");
//		n=BCCX_FindTag(l, "body");
//		c=BCCX_Fetch(l, "body");

		c=BCCX_Child(l);

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

		BGBCC_BindStructSig(ctx, s, "namespace");
		bgbcc_print(ctx, "PACKAGE %s\n", s);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;
		
		while(c)
		{
			BGBCC_CompileTopStatement(ctx, c);
			c=BCCX_Next(c);
		}
		ctx->cur_ns=s1;
		
		return;
	}

	if(BCCX_TagIsP(l, "null_statement"))
		{ return; }

	if(BCCX_TagIsP(l, "funcall"))
		{ return; }

	if(BCCX_TagIsP(l, "linecomment")) { return; }
	if(BCCX_TagIsP(l, "blockcomment")) { return; }

	BGBCC_Warn("unhandled top statement %s\n", BCCX_Tag(l));
}


char *BGBCC_CompileModule(char *name, BCCX_Node *l)
{
	BGBCC_State *ctx;
	BCCX_Node *c;
	char *s;


	ctx=bgbcc_malloc(sizeof(BGBCC_State));
	memset(ctx, 0, sizeof(BGBCC_State));

	bgbcc_warn=0;
	bgbcc_err=0;

	s=BCCX_Get(l, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_Get(l, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_CompileStructs(ctx);

//	if(bgbcc_err)return(NULL);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(bgbcc_err)
	{
		printf("Error Count %d\n", bgbcc_err);
		return(NULL);
	}

	return(ctx->ips);
}
