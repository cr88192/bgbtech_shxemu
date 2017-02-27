#include <bgbccc.h>


// extern BCCX_Node *bgbcp_structs;
// extern BCCX_Node *bgbcp_types;

// char *bgbcc_frcc_lfn;
// int bgbcc_frcc_lln;
// int bgbcc_frcc_reqlfn=0;
// static int bgbcc_frcc_seq=1;

// int bgbcc_frcc_warn;
// int bgbcc_frcc_err;

void BGBCC_FrCC_Warn(BGBCC_FrCC_State *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_warn++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: warning: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_FrCC_Error(BGBCC_FrCC_State *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_error++;
//	bgbcc_frcc_err++;
	printf("%s:%d: error: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_FrCC_Note(BGBCC_FrCC_State *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_note++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: note: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_FrCC_StubWarnLLn(BGBCC_FrCC_State *ctx,
	char *file, int line)
{
	BGBCC_FrCC_Warn(ctx, "StubWarn %s:%d\n", file, line);
}

void BGBCC_FrCC_StubErrorLLn(BGBCC_FrCC_State *ctx,
	char *file, int line)
{
	BGBCC_FrCC_Error(ctx, "StubError %s:%d\n", file, line);
}

char *BGBCC_FrCC_GenSym(BGBCC_FrCC_State *ctx)
{
	char buf[32];

	sprintf(buf, "GS%d", ctx->gs_seq++);
	return(bgbcc_strdup(buf));
}

char *BGBCC_FrCC_GenSym2(BGBCC_FrCC_State *ctx)
{
	return(BGBCC_FrCC_GenSym(ctx));
//	return(BS1_RStrDup(BASM_GenSym()));
}

int BGBCC_FrCC_CompileSwitch(BGBCC_FrCC_State *ctx, BCCX_Node *l)
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
			cl[ncl++]=BGBCC_FrCC_GenSym(ctx);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}

	BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "cond"));

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case"))
		{
			BGBCC_FrBC_StackDup(ctx);
			BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(c, "value"));
			BGBCC_FrCC_CompileJmpCond(ctx, "==", cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_FrCC_CompileJmp(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}


	s=BGBCC_FrCC_GenSym(ctx);
	ctx->breakstack[ctx->breakstackpos++]=s;
	BGBCC_FrCC_CompileBreak(ctx);

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case") ||
			BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_FrCC_EmitLabel(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		BGBCC_FrCC_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_FrCC_EmitLabel(ctx, s);
	BGBCC_FrBC_StackPop(ctx);

	bgbcc_free(cl);
	ctx->breakstackpos--;
	return(0);
}

void BGBCC_FrCC_CompileInitVar(BGBCC_FrCC_State *ctx,
	char *name, BCCX_Node *type, BCCX_Node *value)
{
	BCCX_Node *t;

	if(value)
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, value);

//		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrCC_CompileExprAsType(ctx, type, t);
		BGBCC_FrBC_StackInitVarValue(ctx, name);
	}else
	{
		BGBCC_FrBC_StackInitVar(ctx, name);
	}
}

void BGBCC_FrCC_CompileStatement(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	char *s0, *s1, *s2;
	int i, j, k;

	if(!l)
	{
		return;
	}

	s0=BCCX_Get(l, "fn");
	if(s0 && (s0!=ctx->lfn))
	{
		if(!ctx->reqlfn)
			{ BGBCC_FrBC_StackFn(ctx, s0); }
		ctx->lfn=s0;
	}

	i=BCCX_GetInt(l, "ln");
	if((i>0) && (i!=ctx->lln))
	{
		if(!ctx->reqlfn)
			{ BGBCC_FrBC_StackLn(ctx, i); }
		ctx->lln=i;
	}

	if(ctx->reqlfn)
	{
		BGBCC_FrBC_StackFn(ctx, ctx->lfn);
		BGBCC_FrBC_StackLn(ctx, ctx->lln);
		ctx->reqlfn=0;
	}

	l=BGBCC_FrCC_ReduceExpr(ctx, l);
	if(!l)
	{
		printf("BGBCC_FrCC_CompileStatement: Statement Reduced Away\n");
		return;
	}

	if(BCCX_TagIsP(l, "switch"))
	{
		BGBCC_FrCC_CompileSwitch(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "goto"))
	{
		BGBCC_FrCC_CompileJmp(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "label"))
	{
		BGBCC_FrCC_EmitLabel(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "continue"))
	{
		s0=ctx->contstack[ctx->contstackpos-1];
		BGBCC_FrCC_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "break"))
	{
		s0=ctx->breakstack[ctx->breakstackpos-1];
		BGBCC_FrCC_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "return"))
	{
		t=BCCX_Fetch(l, "value");
		if(!t)
		{
			BGBCC_FrBC_StackRetV(ctx);
			return;
		}

		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrBC_StackRet(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_FrCC_ReduceExpr(ctx, ln);
		rn=BGBCC_FrCC_ReduceExpr(ctx, rn);

		s0=BCCX_Get(l, "op");

		if(s0)
		{
			if(BCCX_TagIsP(ln, "ref"))
			{
				s1=BCCX_Get(ln, "name");
				BGBCC_FrCC_CompileExpr(ctx, ln);
				BGBCC_FrCC_CompileExpr(ctx, rn);
				BGBCC_FrBC_StackBinaryOpStore(ctx, s0, s1);
				return;
			}

			BGBCC_FrCC_CompileExpr(ctx, ln);
			BGBCC_FrCC_CompileExpr(ctx, rn);
			BGBCC_FrBC_StackBinaryOp(ctx, s0);
			BGBCC_FrCC_CompileAssign(ctx, ln);

//			BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
//			BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
//			BGBCC_FrBC_StackBinaryOp(ctx, s0);
//			BGBCC_FrCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
			return;
		}

		if(BCCX_TagIsP(ln, "ref") &&
			BCCX_TagIsP(rn, "binary"))
		{
			s0=BCCX_Get(rn, "op");
			s1=BCCX_Get(ln, "name");

			if(strcmp(s0, "&&") && strcmp(s0, "||"))
			{
				ln2=BCCX_Fetch(rn, "left");
				rn2=BCCX_Fetch(rn, "right");
				ln2=BGBCC_FrCC_ReduceExpr(ctx, ln2);
				rn2=BGBCC_FrCC_ReduceExpr(ctx, rn2);
				BGBCC_FrCC_CompileExpr(ctx, ln2);
				BGBCC_FrCC_CompileExpr(ctx, rn2);
				BGBCC_FrBC_StackBinaryOpStore(ctx, s0, s1);
				return;
			}
		}

		BGBCC_FrCC_CompileExpr(ctx, rn);
		BGBCC_FrCC_CompileAssign(ctx, ln);

//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
//		BGBCC_FrCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc") || BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			BGBCC_FrBC_StackUnaryOpName(ctx, "++", s0);
//			bgbcc_frcc_print(ctx, "$%s inc_s ", s0);
			return;
		}

		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrBC_StackUnaryOp(ctx, "++");
		BGBCC_FrCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec") || BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			BGBCC_FrBC_StackUnaryOpName(ctx, "--", s0);
//			bgbcc_frcc_print(ctx, "$%s dec_s ", s0);
			return;
		}

		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrBC_StackUnaryOp(ctx, "--");
		BGBCC_FrCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "defun"))	{ return; }
//	if(BCCX_TagIsP(l, "vars"))	{ return; }

	if(BCCX_TagIsP(l, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsP(c, "var"))
				{ c=BCCX_Next(c); continue; }

			s0=BCCX_Get(c, "name");
			t=BCCX_FindTag(c, "type");
			v=BCCX_Fetch(c, "value");
//			v=NULL;

			BGBCC_FrCC_CompileInitVar(ctx, s0, t, v);
//			BGBCC_FrCC_EmitVar2(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_FrCC_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_FrCC_CompileStatement(ctx, t);
			return;
		}

		s0=BGBCC_FrCC_GenSym(ctx);
		BGBCC_FrCC_CompileJCF(ctx, t, s0);

		t=BCCX_Fetch(l, "else");
		BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "then"));
		if(t)
		{
			s1=BGBCC_FrCC_GenSym(ctx);
			BGBCC_FrCC_CompileJmp(ctx, s1);
			BGBCC_FrCC_EmitLabel(ctx, s0);
			BGBCC_FrCC_CompileStatement(ctx, t);
			s0=s1;
		}
		BGBCC_FrCC_EmitLabel(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_FrCC_CompileStatement(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "for"))
	{
		s0=BGBCC_FrCC_GenSym(ctx);
		s1=BGBCC_FrCC_GenSym(ctx);
		s2=BGBCC_FrCC_GenSym(ctx);

		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "init"));

//		BGBCC_FrCC_EmitLabel(ctx, s1);
		BGBCC_FrCC_EmitLabel(ctx, s0);
		BGBCC_FrCC_CompileJCF(ctx, BCCX_Fetch(l, "cond"), s2);

		BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		BGBCC_FrCC_EmitLabel(ctx, s1);
		BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "step"));
//		BGBCC_FrCC_CompileContinue(ctx);
		BGBCC_FrCC_CompileJmp(ctx, s0);

		BGBCC_FrCC_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "while"))
	{
		s1=BGBCC_FrCC_GenSym(ctx);
		s2=BGBCC_FrCC_GenSym(ctx);

		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_FrCC_EmitLabel(ctx, s1);

		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_FrCC_BoolExpr(ctx, t);

		if(i==0)return;

		if(i==1)
		{
			BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));
			BGBCC_FrCC_CompileContinue(ctx);
			BGBCC_FrCC_EmitLabel(ctx, s2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_FrCC_CompileJCF(ctx, t, s2);
		BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));
		BGBCC_FrCC_CompileContinue(ctx);
		BGBCC_FrCC_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}


	if(BCCX_TagIsP(l, "do_while"))
	{
		s1=BGBCC_FrCC_GenSym(ctx);
		s2=BGBCC_FrCC_GenSym(ctx);
		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_FrCC_EmitLabel(ctx, s1);
		BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_FrCC_BoolExpr(ctx, t);
		if(i==0)return;

		if(i==1)
		{
			BGBCC_FrCC_CompileContinue(ctx);
			BGBCC_FrCC_EmitLabel(ctx, s2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_FrCC_CompileJCT(ctx, t, s1);
		BGBCC_FrCC_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "try_catch"))
	{
		//lame: for now just spit out try block...
		BGBCC_FrCC_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		return;
	}

	if(BCCX_TagIsP(l, "msvc_asm"))
	{
		BGBCC_FrCC_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "gcc_asm"))
	{
		BGBCC_FrCC_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_FrBC_StackPop(ctx);
//		s0=BGBCC_FrCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
//		BGBCC_FrBC_StackCastSig(ctx, s0);
//		bgbcc_print(ctx, "$%s cast ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "linecomment")) { return; }
	if(BCCX_TagIsP(l, "blockcomment")) { return; }

	BGBCC_FrCC_CompileExpr(ctx, l);
	BGBCC_FrBC_StackPop(ctx);
}


char *BGBCC_FrCC_VarTypeString_FlattenExpr(BGBCC_FrCC_State *ctx,
	char *dst, BCCX_Node *l, int fl)
{
	BCCX_Node *c, *n;
	char *s, *t;
	
	t=dst;
	
	if(BCCX_TagIsP(l, "ref"))
	{
		s=BCCX_Get(l, "name");
		t=BGBCC_FrCC_VarTypeString_FlattenName(ctx, dst, s, fl);
		return(t);
	}

	if(BCCX_TagIsP(l, "emptyindex"))
	{
		*t++='Q';
		n=BCCX_Fetch(l, "array");
		t=BGBCC_FrCC_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsP(l, "sharpcall"))
	{
		*t++='<';
		c=BCCX_Fetch(l, "args");
		while(c)
		{
			t=BGBCC_FrCC_VarTypeString_FlattenExpr(ctx, t, c, fl);
			c=BCCX_Next(c);
		}
		*t++='>';
		n=BCCX_Fetch(l, "value");
		t=BGBCC_FrCC_VarTypeString_FlattenExpr(ctx, t, n, fl);
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
		t=BGBCC_FrCC_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}
	
	return(t);
}

char *BGBCC_FrCC_VarTypeString_FlattenName(BGBCC_FrCC_State *ctx,
	char *t, char *s, int fl)
{
	BCCX_Node *c, *n;
	char *t1;
	int lang;
	int i, j, k;

	t1=t;

	if(ctx)lang=ctx->lang;
		else lang=BGBCC_LANG_C;

	if((lang==BGBCC_LANG_C) || (lang==BGBCC_LANG_CPP))
	{
		if(fl&BGBCC_TYFL_UNSIGNED)
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
			if(!strcmp(s, "bool"))*t++='b';
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

		if(!strcmp(s, "_Bool"))*t++='b';

		if(!strcmp(s, "int64"))*t++='x';
		if(!strcmp(s, "uint64"))*t++='y';

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
		*t=0;
	}

	if(ctx && (t==t1))
	{
		c=ctx->structs;
		while(c)
		{
			if(BCCX_AttrIsP(c, "name", s))
			{
				if(BCCX_TagIsP(c, "enum"))
				{
					*t++='i';
					*t=0;
					break;
				}

				if((ctx->ctxflags&BGBCC_CTXFL_SAVENAMES) ||
					(fl&BGBCC_TYFL_EXPNAME))
				{
					sprintf(t, "X%s;", s);
					t+=strlen(t);
				}else
				{
					i=BGBCC_FrBC_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_FrBC_GetStructID(ctx, s);
					}
					if(i>0)
					{
						sprintf(t, "X%d", i);
						t+=strlen(t);
						break;
					}

					sprintf(t, "X%s;", s);
					t+=strlen(t);
				}
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

int BGBCC_FrCC_VarTypeString_ModifierChar(BGBCC_FrCC_State *ctx, int i)
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

char *BGBCC_FrCC_VarTypeString_FlattenModifiers(BGBCC_FrCC_State *ctx,
	char *t, int fl)
{
	int i, j, k;

	k=0;
	for(i=0; i<30; i++)
	{
		j=BGBCC_FrCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)k++;
	}
	
	if(k>2)
	{
		*t++='M';
		for(i=0; i<31; i++)
		{
			j=BGBCC_FrCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
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
		j=BGBCC_FrCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
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

char *BGBCC_FrCC_VarTypeString_FlattenModifiers2(BGBCC_FrCC_State *ctx,
	char *t, int fl)
{
	int i, j, k;

	for(i=0; i<31; i++)
	{
		j=BGBCC_FrCC_VarTypeString_ModifierChar(ctx, fl&(1<<i));
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

char *BGBCC_FrCC_VarTypeString(BGBCC_FrCC_State *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsP(ty, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)
		{ return(bgbcc_strdup("v")); }

	if(!BCCX_TagIsP(ty, "type"))
		{ return(bgbcc_strdup("v")); }

	if(ty)
	{
#if 1
		c=BCCX_Fetch(ty, "size");
		if(c)
		{
			*t++='A';
			while(c)
			{
				n=BGBCC_FrCC_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsP(n, "int"))
				{
					if(BCCX_TagIsP(n, "if"))
					{
						BGBCC_FrCC_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					BGBCC_FrCC_Error(ctx, "Invalid array size specifier\n");
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetInt(n, "value");
				if(i<0)
				{
					BGBCC_FrCC_Warn(ctx,
						"Negative Size Array\n");
					*t++='1'; c=BCCX_Next(c); continue;
				}
				
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
//			t=BGBCC_FrCC_VarTypeString_FlattenModifiers(ctx, t, i);
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
			t=BGBCC_FrCC_VarTypeString_FlattenExpr(
				ctx, t, n, i|BGBCC_TYFL_EXPNAME);
//			*t=0;
//			return(bgbcc_strdup(buf));
		}else
		{
			if(s)
			{
				t=BGBCC_FrCC_VarTypeString_FlattenName(
					ctx, t, s, i|BGBCC_TYFL_EXPNAME);
			}else
			{
				*t++='v';
			}
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_FrCC_VarImageTypeString(BGBCC_FrCC_State *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsP(ty, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)
		{ return(bgbcc_strdup("v")); }

	if(!BCCX_TagIsP(ty, "type"))
		{ return(bgbcc_strdup("v")); }

	if(ty)
	{
#if 1
		c=BCCX_Fetch(ty, "size");
		if(c)
		{
			*t++='A';
			while(c)
			{
				n=BGBCC_FrCC_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsP(n, "int"))
				{
					if(BCCX_TagIsP(n, "if"))
					{
						BGBCC_FrCC_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					if(BCCX_TagIsP(n, "sizeof"))
					{
						BGBCC_FrCC_Warn(ctx,
							"Sizeof array-size Hack\n");
						*t++='1024'; c=BCCX_Next(c); continue;
					}

					BGBCC_FrCC_Error(ctx, "Invalid array size specifier\n");
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetInt(n, "value");
				sprintf(t, "%d", i);
				t+=strlen(t);

				c=BCCX_Next(c);
				if(c)*t++=',';
			}
//			*t++=';';
		}
#endif

		i=BCCX_GetInt(ty, "flags");
		s=BCCX_Get(ty, "name");

		if(i&BGBCC_TYFL_WIDE)
		{
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
			t=BGBCC_FrCC_VarTypeString_FlattenExpr(ctx, t, n, i);
		}else
		{
			if(s)
			{
				t=BGBCC_FrCC_VarTypeString_FlattenName(ctx, t, s, i);
			}else
			{
				*t++='v';
			}
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_FrCC_VarTypeFlagsString(BGBCC_FrCC_State *ctx, BCCX_Node *ty)
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

		t=BGBCC_FrCC_VarTypeString_FlattenModifiers2(ctx, t, i);
	}
	*t=0;

	return(bgbcc_strdup(buf));
}

void BGBCC_FrCC_EmitVarFunc(BGBCC_FrCC_State *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *args)
{
	char *s, *s1, *s2;
	s64 li;
	int i;

//	s=BGBCC_FrCC_VarTypeString(ctx, ty);
	s=BGBCC_FrCC_VarSigProto(ctx, ty, name, args);
	s1=name;
	s2=BGBCC_FrCC_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";

	if(!s1)s1="_";

//	if(*s)bgbcc_frcc_print(ctx, "%s:%s ", s1, s);
//		else bgbcc_frcc_print(ctx, "%s ", s1);

//	BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARDECL);

	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s1);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_SIG, s);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_FLAGS, s2);

	li=BCCX_GetInt(ty, "flags");
	
	if(BGBCC_FrCC_GetNodeAttribute(ctx, ty, "dllexport") ||
		!strcmp(s1, "main") || !strcmp(s1, "WinMain"))
	{
		li|=BGBCC_TYFL_EXPNAME|BGBCC_TYFL_DLLEXPORT;
	}
	
	BGBCC_FrBC_AttribLong(ctx, FR2C_ATTR_FLAGS, li);

#if 0
	if(v)
	{
//		bgbcc_frcc_print(ctx, "= ");
		BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARVALUE);
		BGBCC_FrCC_EmitVarValueR(ctx, v);
		BGBCC_FrBC_End(ctx);
	}
#endif

//	BGBCC_FrBC_End(ctx);
}

void BGBCC_FrCC_EmitVar(BGBCC_FrCC_State *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	if(!v && BGBCC_FrBC_CheckDefinedContextName(ctx,
		FR2C_CMD_VARDECL, name))
	{
		return;
	}

	s=BGBCC_FrCC_VarImageTypeString(ctx, ty);
	s1=name;
	s2=BGBCC_FrCC_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";

	if(!s1)s1="_";

//	if(*s)bgbcc_frcc_print(ctx, "%s:%s ", s1, s);
//		else bgbcc_frcc_print(ctx, "%s ", s1);

	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_VARDECL, s1);

	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s1);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_SIG, s);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_FLAGS, s2);

	if(v)
	{
//		bgbcc_frcc_print(ctx, "= ");
		BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARVALUE);
		BGBCC_FrCC_EmitVarValueR(ctx, v);
		BGBCC_FrBC_End(ctx);
	}

	BGBCC_FrBC_End(ctx);
}

void BGBCC_FrCC_EmitVar2(BGBCC_FrCC_State *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char tb[256];
	char *s, *s1, *s2;
	int i, op;

	if(!v && BGBCC_FrBC_CheckDefinedContextName(ctx,
		FR2C_CMD_VARDECL, name))
	{
		return;
	}

	s=BGBCC_FrCC_VarImageTypeString(ctx, ty);
	s1=name;

	if(!s)s="v";

	i=BCCX_GetInt(ty, "flags");
//	s2=((i&BGBCC_TYFL_STATIC) && !ctx->cur_struct)?"~":"";

	if(!s1)s1="_";
//	if(*s)bgbcc_frcc_print(ctx, "%s%s:%s ", s1, s2, s);
//		else bgbcc_frcc_print(ctx, "%s%s ", s1, s2);

	op=FR2C_CMD_VARDECL;
	if(i&BGBCC_TYFL_STATIC)
		op=FR2C_CMD_STATICVARDECL;

//	sprintf(tb, "%s%s", s1, s2);
//	BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARDECL);
	BGBCC_FrBC_BeginName(ctx, op, s1);
//	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, tb);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s1);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_SIG, s);

	s2=BGBCC_FrCC_VarTypeFlagsString(ctx, ty);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_FLAGS, s2);

	if(ctx->cur_structdef && BCCX_TagIsP(ctx->cur_structdef, "enum"))
	{
		if(v)
		{
			ctx->cur_idx4=BCCX_GetInt(v, "value");
		}

		BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARVALUE);
//		BGBCC_FrCC_EmitVarValueR(ctx, v);
		BGBCC_FrBC_LiteralInt(ctx, FR2C_ATTR_VALUE,
			ctx->cur_idx4++);
		BGBCC_FrBC_End(ctx);
	}else
	{
		if(v)
		{
//			bgbcc_frcc_print(ctx, "= ");
			BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARVALUE);
			BGBCC_FrCC_EmitVarValueR(ctx, v);
			BGBCC_FrBC_End(ctx);
		}
	}

	BGBCC_FrBC_End(ctx);

	if(ctx->cur_struct)
	{
		s2=BGBCC_FrCC_VarTypeFlagsString(ctx, ty);

		BGBCC_FrCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"field", ctx->cur_idx++, s1);

		BGBCC_FrCC_BindFieldSig(ctx, ctx->cur_struct, s1, s);
		BGBCC_FrCC_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
		if(s2 && (*s2))
			BGBCC_FrCC_BindFieldInfo(ctx, ctx->cur_struct, s1, "flagstr", s2);
	}
}

void BGBCC_FrCC_EmitVar3(BGBCC_FrCC_State *ctx, char *name, char *ty)
{
	char *s, *s1;
	int i;

	if(BGBCC_FrBC_CheckDefinedContextName(ctx,
		FR2C_CMD_VARDECL, name))
	{
		return;
	}

	s=ty;
	s1=name;

	if(!s)s="v";

	if(!s1)s1="_";
//	if(*s)bgbcc_frcc_print(ctx, "%s:%s ", s1, s);
//		else bgbcc_frcc_print(ctx, "%s ", s1);

	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_VARDECL, s1);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s1);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_SIG, s);
	BGBCC_FrBC_End(ctx);

	if(ctx->cur_struct)
	{
		BGBCC_FrCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"method", ctx->cur_idx2++, s1);

		BGBCC_FrCC_BindFieldSig(ctx, ctx->cur_struct, s1, s);
//		BGBCC_FrCC_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
	}
}

void BGBCC_FrCC_EmitVarProperty(BGBCC_FrCC_State *ctx, BCCX_Node *l)
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
		s1=BGBCC_FrCC_GenProtoSig(ctx, t, NULL);
		BGBCC_FrCC_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_FrCC_GenProtoSig(ctx, NULL, n);
		BGBCC_FrCC_EmitVar3(ctx, s2, s1);
	}

//	BGBCC_FrCC_EmitVar3(ctx, "new", s1);
//	BGBCC_FrCC_CompileBlock(ctx, t, s, n, v);

	if(ctx->cur_struct)
	{
		s1=BGBCC_FrCC_VarTypeString(ctx, t);
		s2=BGBCC_FrCC_VarTypeFlagsString(ctx, t);
		i=BCCX_GetInt(t, "flags");

		BGBCC_FrCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"property", ctx->cur_idx3++, s);

		BGBCC_FrCC_BindFieldSig(ctx, ctx->cur_struct, s, s1);
		BGBCC_FrCC_BindFieldInfoI(ctx, ctx->cur_struct, s, "flags", i);
		if(s2 && (*s2))
			BGBCC_FrCC_BindFieldInfo(ctx, ctx->cur_struct, s, "flagstr", s2);
	}
}

void BGBCC_FrCC_CompileVarProperty(BGBCC_FrCC_State *ctx, BCCX_Node *l)
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
		BGBCC_FrCC_CompileBlock(ctx, t, s2, NULL, v);

//		s1=BGBCC_FrCC_GenProtoSig(ctx, t, NULL);
//		BGBCC_FrCC_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_FrCC_CompileBlock(ctx, NULL, s2, n, v);

//		s1=BGBCC_FrCC_GenProtoSig(ctx, NULL, n);
//		BGBCC_FrCC_EmitVar3(ctx, s2, s1);
	}

//	BGBCC_FrCC_EmitVar3(ctx, "new", s1);
//	BGBCC_FrCC_CompileBlock(ctx, t, s, n, v);
#endif

}

int BGBCC_FrCC_CompileVarDummyP(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	if(	BCCX_TagIsP(l, "assign") ||
		BCCX_TagIsP(l, "begin") ||
		BCCX_TagIsP(l, "break") ||
		BCCX_TagIsP(l, "case") ||
		BCCX_TagIsP(l, "case_default") ||
		BCCX_TagIsP(l, "cast") ||
		BCCX_TagIsP(l, "comma") ||
		BCCX_TagIsP(l, "continue") ||
		BCCX_TagIsP(l, "do_while") ||
		BCCX_TagIsP(l, "for") ||
		BCCX_TagIsP(l, "funcall") ||
		BCCX_TagIsP(l, "goto") ||
		BCCX_TagIsP(l, "if") ||
		BCCX_TagIsP(l, "label") ||
		BCCX_TagIsP(l, "methodcall") ||
		BCCX_TagIsP(l, "msvc_asm") ||
		BCCX_TagIsP(l, "postdec") ||
		BCCX_TagIsP(l, "postinc") ||
		BCCX_TagIsP(l, "predec") ||
		BCCX_TagIsP(l, "preinc") ||
		BCCX_TagIsP(l, "ref") ||
		BCCX_TagIsP(l, "return") ||
		BCCX_TagIsP(l, "switch") ||
		BCCX_TagIsP(l, "while"))
		{ return(1); }

	if(BCCX_TagIsP(l, "linecomment")) { return(1); }
	if(BCCX_TagIsP(l, "blockcomment")) { return(1); }

	return(0);
}

void BGBCC_FrCC_CompileVarStatementBlock(
	BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	
	c=l;
	while(c)
	{
		BGBCC_FrCC_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
}

int BGBCC_FrCC_CompileVarStatementCompound(
	BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		BGBCC_FrCC_CompileVarStatementBlock(ctx, c);
 		return(1);
	}

	if(BCCX_TagIsP(l, "if"))
	{
		c=BCCX_Fetch(l, "then");
		BGBCC_FrCC_CompileVarStatement(ctx, c);

		c=BCCX_Fetch(l, "else");
		if(c)
		{
			BGBCC_FrCC_CompileVarStatement(ctx, c);
		}

 		return(1);
	}

	if(
		BCCX_TagIsP(l, "do_while") ||
		BCCX_TagIsP(l, "for") ||
		BCCX_TagIsP(l, "switch") ||
		BCCX_TagIsP(l, "while"))
	{
		c=BCCX_Fetch(l, "body");
		BGBCC_FrCC_CompileVarStatementBlock(ctx, c);
 		return(1);
	}
	return(0);
}

void BGBCC_FrCC_CompileVarStatement(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1;
	int i;

	s=BCCX_Get(l, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)ctx->lln=i;

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

			BGBCC_FrCC_EmitVar2(ctx, s, t, v);

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

		s1=BGBCC_FrCC_GenProtoSig(ctx, t, n);
		BGBCC_FrCC_EmitVar3(ctx, s, s1);
//		BGBCC_FrCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_FrCC_GenProtoSig(ctx, NULL, n);
		BGBCC_FrCC_EmitVar3(ctx, "new", s1);
//		BGBCC_FrCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_FrCC_EmitVarProperty(ctx, l);
		return;
	}

	if(BGBCC_FrCC_CompileVarStatementCompound(ctx, l))
		return;

	if(BGBCC_FrCC_CompileVarDummyP(ctx, l))
		return;

	BGBCC_FrCC_Error(ctx, "Unhandled var statement %s\n", BCCX_Tag(l));
}

void BGBCC_FrCC_CompileVarStatement2(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s;
	int i;

	s=BCCX_Get(l, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)ctx->lln=i;

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

//			BGBCC_FrCC_EmitVar2(ctx, s, t, v);
			BGBCC_FrCC_EmitTopVar(ctx, s, t, v);

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

		BGBCC_FrCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		s="new";
		t=NULL;
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_FrCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_FrCC_CompileVarProperty(ctx, l);
		return;
	}

	if(BGBCC_FrCC_CompileVarDummyP(ctx, l))
		return;

	BGBCC_FrCC_Error(ctx, "Unhandled var statement %s\n", BCCX_Tag(l));
}

BCCX_Node *BGBCC_FrCC_CompileBlock(BGBCC_FrCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	char *cname[16], *bname[16];

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

	name=BGBCC_FrCC_QualifyNameNS(ctx, name);

	BGBCC_FrCC_EmitSigProto(ctx, type, name, args);

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

//	if(ctx->cur_struct)
//		bgbcc_frcc_print(ctx, "METHOD ");
//	else bgbcc_frcc_print(ctx, "FUNC ");

//	BGBCC_FrBC_Begin(ctx, (ctx->cur_struct)?
//		FR2C_CMD_METHOD:FR2C_CMD_FUNCTION);
	BGBCC_FrBC_BeginName(ctx, (ctx->cur_struct)?
		FR2C_CMD_METHOD:FR2C_CMD_FUNCTION, ctx->cf_n);

//	BGBCC_FrCC_EmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);
	BGBCC_FrCC_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_FrBC_Begin(ctx, FR2C_CMD_ARGS);
	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
//			bgbcc_frcc_print(ctx, "... ");
			BGBCC_FrBC_Marker(ctx, FR2C_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_FrCC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_FrBC_End(ctx);

//	bgbcc_frcc_print(ctx, "VARS ");
	BGBCC_FrBC_Begin(ctx, FR2C_CMD_LOCALS);
	c=body;
	while(c)
	{
		BGBCC_FrCC_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_FrBC_End(ctx);


//	bgbcc_frcc_print(ctx, "BODY ");
//	bgbcc_frcc_print(ctx, "\n");

	BGBCC_FrBC_Begin(ctx, FR2C_CMD_BODY);
	ctx->reqlfn=1;

	c=body;
	while(c)
	{
//		bgbcc_frcc_print(ctx, "\t");
		BGBCC_FrCC_CompileStatement(ctx, c);
//		bgbcc_frcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
//	bgbcc_frcc_print(ctx, "ret_v ");
	BGBCC_FrBC_End(ctx);

//	bgbcc_frcc_print(ctx, "END\n\n");
	BGBCC_FrBC_End(ctx);

	return(NULL);
}

char *BGBCC_FrCC_GenProtoSig(BGBCC_FrCC_State *ctx,
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
			s=BGBCC_FrCC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	if(type)
	{
		s=BGBCC_FrCC_VarTypeString(ctx, type);
		if(s) { strcpy(t, s); t+=strlen(t); }
	}else
	{
		*t++='Pv';
//		*t++='v';
	}

	*t++=0;

	return(bgbcc_strdup(tb));
}

void BGBCC_FrCC_EmitSigProto(BGBCC_FrCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

//	return;

//	printf("BGBCC_FrCC_EmitSigProto(A) %s\n", name);

	if(!type)return;

//	printf("BGBCC_FrCC_EmitSigProto(B) %s\n", name);

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
			s=BGBCC_FrCC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_FrCC_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

	BGBCC_FrCC_BindSig(ctx, name, tb);

	s=BGBCC_FrCC_VarTypeFlagsString(ctx, type);
	if(s && (*s))
			BGBCC_FrCC_BindVarInfo(ctx, name, "flagstr", s);
}

char *BGBCC_FrCC_VarSigProto(BGBCC_FrCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

//	return;

//	printf("BGBCC_FrCC_EmitSigProto(A) %s\n", name);

	if(!type)
		return(NULL);

//	printf("BGBCC_FrCC_EmitSigProto(B) %s\n", name);

	i=BCCX_GetInt(type, "flags");
//	if(i&BGBCC_TYFL_STATIC)return;

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
			s=BGBCC_FrCC_VarImageTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_FrCC_VarImageTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;
	
	return(bgbcc_strdup(tb));

//	BGBCC_FrCC_BindSig(ctx, name, tb);
//	s=BGBCC_FrCC_VarTypeFlagsString(ctx, type);
//	if(s && (*s))
//			BGBCC_FrCC_BindVarInfo(ctx, name, "flagstr", s);
}

BCCX_Node *BGBCC_FrCC_CompileProto(BGBCC_FrCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

//	printf("BGBCC_FrCC_CompileProto %s\n", name);

	name=BGBCC_FrCC_QualifyNameNS(ctx, name);

	/* avoid redefining the same stuff */
	if(BGBCC_FrBC_LookupGlobal(ctx, name))
		{ return(NULL); }

	BGBCC_FrCC_EmitSigProto(ctx, type, name, args);

	ctx->cf_n=name;
	ctx->cf_ty=type;

//	bgbcc_frcc_print(ctx, "PROTO ");
//	BGBCC_FrBC_Begin(ctx, FR2C_CMD_PROTOTYPE);
	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_PROTOTYPE, ctx->cf_n);

//	BGBCC_FrCC_EmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);
	BGBCC_FrCC_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_FrBC_Begin(ctx, FR2C_CMD_ARGS);
	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
//			bgbcc_frcc_print(ctx, "... ");
			BGBCC_FrBC_Marker(ctx, FR2C_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_FrCC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_FrBC_End(ctx);

	BGBCC_FrBC_End(ctx);

	return(NULL);
}

void BGBCC_FrCC_CompileStruct(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_FrCC_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0, *s1;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_FrCC_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	cur=BGBCC_FrBC_LookupStructure(ctx, s);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetInt(l, "flags");
	BGBCC_FrCC_BindStructSig(ctx, s, "struct");
	BGBCC_FrCC_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

//	BGBCC_FrBC_Begin(ctx, FR2C_CMD_STRUCT);
	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_STRUCT, BCCX_Get(l, "name"));

	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, BCCX_Get(l, "name"));

//	bgbcc_frcc_print(ctx, "STRUCT %s ", BCCX_Get(l, "name"));
//	bgbcc_frcc_print(ctx, "\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_FrCC_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_FrBC_End(ctx);
//	bgbcc_frcc_print(ctx, "\tEND\n");

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_FrCC_CompileUnion(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BGBCC_FrCC_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_FrCC_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	cur=BGBCC_FrBC_LookupStructure(ctx, s);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetInt(l, "flags");
	BGBCC_FrCC_BindStructSig(ctx, s, "union");
	BGBCC_FrCC_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

//	BGBCC_FrBC_Begin(ctx, FR2C_CMD_UNION);
	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_UNION, BCCX_Get(l, "name"));

	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, BCCX_Get(l, "name"));

//	bgbcc_frcc_print(ctx, "UNION %s ", BCCX_Get(l, "name"));
//	bgbcc_frcc_print(ctx, "\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
//		bgbcc_frcc_print(ctx, "\t");
		BGBCC_FrCC_CompileVarStatement(ctx, c);
//		bgbcc_frcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
//	bgbcc_frcc_print(ctx, "\tEND\n");
	BGBCC_FrBC_End(ctx);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_FrCC_CompileClass(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_FrCC_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0, *qn;
	int i, j, j1, j2;

	s=BCCX_Get(l, "name");
	qn=BGBCC_FrCC_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	cur=BGBCC_FrBC_LookupStructure(ctx, qn);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetInt(l, "flags");
	BGBCC_FrCC_BindStructSig(ctx, qn, "class");
	BGBCC_FrCC_BindStructInfoI(ctx, qn, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=qn;
	j=ctx->cur_idx;
	j1=ctx->cur_idx2;
	j2=ctx->cur_idx3;

	ctx->cur_idx=0;
	ctx->cur_idx2=0;
	ctx->cur_idx3=0;

//	bgbcc_frcc_print(ctx, "CLASS %s ", qn);
//	BGBCC_FrBC_Begin(ctx, FR2C_CMD_CLASS);
	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_CLASS, qn);

	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, qn);

	c=BCCX_Fetch(l, "super");
	if(c)
	{
//		bgbcc_frcc_print(ctx, "EXTENDS ");
		BGBCC_FrBC_Begin(ctx, FR2C_CMD_EXTENDS);
		while(c)
		{
			s=BGBCC_FrCC_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_FrBC_BeginName(ctx, FR2C_CMD_VARDECL, s);
				BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s);
				BGBCC_FrBC_End(ctx);
//				bgbcc_frcc_print(ctx, "%s", s);
			}
			c=BCCX_Next(c);
		}
		BGBCC_FrBC_End(ctx);
//		bgbcc_frcc_print(ctx, " ");
	}

	c=BCCX_Fetch(l, "impl");
	if(c)
	{
//		bgbcc_frcc_print(ctx, "IMPLEMENTS ");
		BGBCC_FrBC_Begin(ctx, FR2C_CMD_IMPLEMENTS);
		while(c)
		{
			s=BGBCC_FrCC_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_FrBC_BeginName(ctx, FR2C_CMD_VARDECL, s);
				BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s);
				BGBCC_FrBC_End(ctx);
//				bgbcc_frcc_print(ctx, "%s", s);
			}
			c=BCCX_Next(c);
		}
//		bgbcc_frcc_print(ctx, " ");
		BGBCC_FrBC_End(ctx);
	}

//	bgbcc_frcc_print(ctx, "\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
//		bgbcc_frcc_print(ctx, "\t");
		BGBCC_FrCC_CompileVarStatement(ctx, c);
//		bgbcc_frcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
//	bgbcc_frcc_print(ctx, "\tEND\n");
	BGBCC_FrBC_End(ctx);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_FrCC_CompileVarStatement2(ctx, c);
		c=BCCX_Next(c);
	}

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
	ctx->cur_idx2=j1;
	ctx->cur_idx3=j2;
}

#if 0
void BGBCC_FrCC_CompileEnum(BGBCC_FrCC_State *ctx, BCCX_Node *l)
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

void BGBCC_FrCC_CompileEnum(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s1, *s2, *os0;
	int op;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_FrCC_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
	BGBCC_FrCC_BindStructSig(ctx, s, "enum");
	BGBCC_FrCC_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;
	ctx->cur_idx4=0;

#if 0
	c=BCCX_Fetch(l, "body");
	while(c)
	{
//		bgbcc_frcc_print(ctx, "\t");
		BGBCC_FrCC_CompileVarStatement(ctx, c);
//		bgbcc_frcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
#endif

	c=BCCX_Fetch(l, "defs");
	while(c)
	{
		if(BCCX_TagIsP(c, "def"))
		{
			op=FR2C_CMD_VARDECL;
			j=BCCX_GetInt(c, "value");
			s1=BCCX_Get(c, "name");
			s2="i";

			if(BGBCC_FrBC_CheckDefinedContextName(ctx,
				FR2C_CMD_VARDECL, s1))
			{
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_FrBC_BeginName(ctx, op, s1);
			BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s1);
			BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_SIG, s2);

			s2="k";
			BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_FLAGS, s2);

			BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARVALUE);
			BGBCC_FrBC_LiteralInt(ctx, FR2C_ATTR_VALUE, j++);
			BGBCC_FrBC_End(ctx);

			BGBCC_FrBC_End(ctx);
		}
		c=BCCX_Next(c);
	}

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

BCCX_Node *BGBCC_FrCC_CompileSProto(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u;
	char *s, *s1;

//	bgbcc_frcc_print(ctx, "S_PROTO ");
//	BGBCC_FrBC_Begin(ctx, FR2C_CMD_S_PROTOTYPE);
	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_S_PROTOTYPE, BCCX_Get(l, "name"));

	t=BCCX_FindTag(l, "type");
//	BGBCC_FrCC_EmitVar(ctx, BCCX_Get(l, "name"), t, NULL);
	BGBCC_FrCC_EmitVarFunc(ctx, BCCX_Get(l, "name"), t,
		BCCX_Fetch(l, "args"));

	BGBCC_FrBC_Begin(ctx, FR2C_CMD_ARGS);
	c=BCCX_Fetch(l, "args");
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			BGBCC_FrBC_Marker(ctx, FR2C_CMD_VARARGS);
//			bgbcc_frcc_print(ctx, "... ");
			c=BCCX_Next(c);
			continue;
		}
		if(!BCCX_TagIsP(c, "var"))
			{ c=BCCX_Next(c); continue; }

		t=BCCX_FindTag(c, "type");
		BGBCC_FrCC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_FrBC_End(ctx);

//	bgbcc_frcc_print(ctx, "END\n");
	BGBCC_FrBC_End(ctx);


	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");
	u=BCCX_Fetch(l, "args");

	s1=BGBCC_FrCC_GenProtoSig(ctx, t, u);
	BGBCC_FrCC_BindStructSig(ctx, s, "func");
	BGBCC_FrCC_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

BCCX_Node *BGBCC_FrCC_GetNodeAttribute(BGBCC_FrCC_State *ctx,
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

//		printf("BGBCC_FrCC_GetNodeAttribute A\n");

		cn1=BCCX_Child(cn);
		while(cn1)
		{
			if(!BCCX_TagIsP(cn1, "attr"))
				{ cn1=BCCX_Next(cn1); continue; }

			s=BCCX_Get(cn1, "name");
//			printf("BGBCC_FrCC_GetNodeAttribute B: %s\n", s);

			if(s && !strcmp(s, name))
				return(cn1);

			cn1=BCCX_Next(cn1);
		}
		cn=BCCX_Next(cn);
	}
	
	return(NULL);
}

char *BGBCC_FrCC_GetNodeAttributeString(BGBCC_FrCC_State *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *attr, *cn;
	char *s;
	
	attr=BGBCC_FrCC_GetNodeAttribute(ctx, l, name);
	if(!attr)return(NULL);

	s=BCCX_Get(attr, "name");
//	printf("BGBCC_FrCC_GetNodeAttributeString: %s\n", s);

	cn=BCCX_Fetch(attr, "args");
	if(!cn)return(NULL);

	if(!BCCX_TagIsP(cn, "string"))
		return(NULL);
//	s=BCCX_Get(cn, "text");
	s=BCCX_Get(cn, "value");
	return(s);
}

void BGBCC_FrCC_CompileTypedef(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

//	s=BCCX_Get(l, "fn");
//	if(s)ctx->lfn=s;
//	i=BCCX_GetInt(l, "ln");
//	if(i)ctx->lln=i;

	if(BCCX_TagIsP(l, "var"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
//		v=BCCX_Fetch(l, "value");

		s1=BGBCC_FrCC_VarTypeString(ctx, t);
		s2=BGBCC_FrCC_VarTypeFlagsString(ctx, t);
		BGBCC_FrCC_BindTypeSig(ctx, s, s1);
		if(s2 && (*s2))
			BGBCC_FrCC_BindTypeInfo(ctx, s, "flagstr", s2);

		s2=BGBCC_FrCC_GetNodeAttributeString(ctx, t, "dyt_name");
//		if(!s2)s2=BGBCC_FrCC_GetNodeAttributeString(ctx, t, "dytname");
		if(s2)
		{
			printf("BGBCC_FrCC_CompileTypedef: DyType %s->%s\n", s, s2);
		
			BGBCC_FrCC_BindTypeInfo(ctx, s, "dytype", s2);
			BGBCC_FrCC_BindDyTypeSig(ctx, s2, s1);
		}

//		c=BCCX_Next(c);
	}

#if 0
	if(BCCX_TagIsP(l, "func"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		v=BCCX_FindTag(l, "args");

		s1=BGBCC_FrCC_GenProtoSig(ctx, t, v);
		BGBCC_FrCC_BindTypeSig(ctx, s, s1);
	}
#endif

	return;
}

void BGBCC_FrCC_CompileStructs(BGBCC_FrCC_State *ctx)
{
	BCCX_Node *c, *t, *n, *l;

	c=ctx->types;
	while(c)
	{
		BGBCC_FrCC_CompileTypedef(ctx, c);
		c=BCCX_Next(c);
	}

	c=ctx->structs;
	while(c)
	{
		l=c;
		if(BCCX_TagIsP(c, "struct"))BGBCC_FrCC_CompileStruct(ctx, c);
		if(BCCX_TagIsP(c, "union"))BGBCC_FrCC_CompileUnion(ctx, c);
		if(BCCX_TagIsP(c, "enum"))BGBCC_FrCC_CompileEnum(ctx, c);
		if(BCCX_TagIsP(c, "s_proto"))BGBCC_FrCC_CompileSProto(ctx, c);
		if(BCCX_TagIsP(c, "func"))BGBCC_FrCC_CompileSProto(ctx, c);

		c=BCCX_Next(c);
	}
}

void BGBCC_FrCC_EmitVarValue(BGBCC_FrCC_State *ctx, BCCX_Node *v)
{
	BGBCC_FrCC_RegisterInfo *decl;
	BCCX_Node *t;
	frbc_register rval;
	char *s;
	int i;

	v=BGBCC_FrCC_ReduceExprConst(ctx, v);

	if(BCCX_TagIsP(v, "int"))
	{
		s=BCCX_Get(v, "tysuf");
		if(s &&
			(!strcmp(s, "L") || !strcmp(s, "LL") ||
			 !strcmp(s, "UL") || !strcmp(s, "ULL") ||
			 !strcmp(s, "LX") || !strcmp(s, "ULX")))
		{
			BGBCC_FrBC_LiteralLong(ctx, FR2C_ATTR_VALUE,
				BCCX_GetInt(v, "value"));
			return;
		}
	
//		bgbcc_frcc_print(ctx, "%s", BCCX_Get(v, "value"));
		BGBCC_FrBC_LiteralInt(ctx, FR2C_ATTR_VALUE,
			BCCX_GetInt(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "real"))
	{
		s=BCCX_Get(v, "tysuf");
		if(s && !strcmp(s, "F"))
		{
			BGBCC_FrBC_LiteralFloat(ctx, FR2C_ATTR_VALUE,
				BCCX_GetFloat(v, "value"));
			return;
		}

		BGBCC_FrBC_LiteralDouble(ctx, FR2C_ATTR_VALUE,
			BCCX_GetFloat(v, "value"));
//		bgbcc_frcc_print(ctx, "%s", BCCX_Get(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "string"))
	{
		BGBCC_FrBC_LiteralStr(ctx, FR2C_ATTR_VALUE,
			BCCX_Get(v, "value"));
//		bgbcc_frcc_print(ctx, "%s", BGBCC_FrCC_CIfy(BCCX_Get(v, "value")));
		return;
	}

	if(BCCX_TagIsP(v, "charstring"))
	{
		s=BCCX_Get(v, "value");
		if(s) { i=BGBCP_ParseChar(&s); }
			else i=0;
		BGBCC_FrBC_LiteralInt(ctx, FR2C_ATTR_VALUE, i);
		return;
	}

	if(BCCX_TagIsP(v, "ref"))
	{
		s=BCCX_Get(v, "name");
		decl=BGBCC_FrBC_LookupGlobal(ctx, s);
		if(decl)
		{
			rval=decl->value;
			if(BGBCC_FrBC_IsRegImmIntP(ctx, rval))
			{
				BGBCC_FrBC_LiteralInt(ctx, FR2C_ATTR_VALUE,
					BGBCC_FrBC_GetRegImmIntValue(ctx, rval));
				return;
			}
			if(BGBCC_FrBC_IsRegImmLongP(ctx, rval))
			{
				BGBCC_FrBC_LiteralLong(ctx, FR2C_ATTR_VALUE,
					BGBCC_FrBC_GetRegImmLongValue(ctx, rval));
				return;
			}

			if(BGBCC_FrBC_IsRegImmFloatP(ctx, rval))
			{
				BGBCC_FrBC_LiteralFloat(ctx, FR2C_ATTR_VALUE,
					BGBCC_FrBC_GetRegImmFloatValue(ctx, rval));
				return;
			}
			if(BGBCC_FrBC_IsRegImmDoubleP(ctx, rval))
			{
				BGBCC_FrBC_LiteralDouble(ctx, FR2C_ATTR_VALUE,
					BGBCC_FrBC_GetRegImmDoubleValue(ctx, rval));
				return;
			}

			if(decl->regtype==FR2C_LITID_FUNCTION)
			{
				BGBCC_FrBC_LiteralGlobalAddr(ctx, FR2C_ATTR_VALUE,
					decl->gblid);
				return;
			}

			if(decl->sig && (*decl->sig=='A'))
			{
				BGBCC_FrBC_LiteralGlobalAddr(ctx, FR2C_ATTR_VALUE,
					decl->gblid);
				return;
			}

			BGBCC_FrCC_Error(ctx, "BGBCC_FrCC_EmitVarValue: "
				"Bad Reg Literal Type %08X\n", rval.val);
			return;
		}

		BGBCC_FrCC_Error(ctx, "BGBCC_FrCC_EmitVarValue: "
			"Undeclared Var %s\n", s);
		return;
	}

	if(BGBCC_FrCC_IsUnaryP(ctx, v, "&"))
	{
		t=BCCX_Fetch(v, "value");

		if(BCCX_TagIsP(t, "ref"))
		{
			s=BCCX_Get(t, "name");
			decl=BGBCC_FrBC_LookupGlobal(ctx, s);
			if(decl)
			{
				BGBCC_FrBC_LiteralGlobalAddr(ctx, FR2C_ATTR_VALUE,
					decl->gblid);
				return;
			}
		}
	}

	if(BCCX_TagIsP(v, "cast"))
	{
		t=BCCX_Fetch(v, "value");
		BGBCC_FrCC_EmitVarValue(ctx, t);
		return;
	}

	BGBCC_FrCC_Error(ctx, "BGBCC_FrCC_EmitVarValue: Bad Value %s\n", 
		BCCX_Tag(v));
	return;
}

void BGBCC_FrCC_EmitVarValueR(BGBCC_FrCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
//		BGBCC_FrBC_Begin(ctx, FR2C_CMD_LIST);
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_FrCC_EmitVarValueR(ctx, c);
//			bgbcc_frcc_print(ctx, " ");
			c=BCCX_Next(c);
		}
//		BGBCC_FrBC_End(ctx);
		return;
	}

	BGBCC_FrCC_EmitVarValue(ctx, v);
}

void BGBCC_FrCC_EmitVarValueR2(BGBCC_FrCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
//		bgbcc_frcc_print(ctx, "LIST ");
		BGBCC_FrBC_Begin(ctx, FR2C_CMD_LIST);

		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_FrCC_EmitVarValueR2(ctx, c);
//			bgbcc_frcc_print(ctx, " ");
			c=BCCX_Next(c);
		}

		BGBCC_FrBC_End(ctx);
//		bgbcc_frcc_print(ctx, "END ");

		return;
	}

	BGBCC_FrCC_EmitVarValue(ctx, v);
}

char *BGBCC_FrCC_QualifyNameNS(BGBCC_FrCC_State *ctx, char *name)
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

void BGBCC_FrCC_EmitTopVar(BGBCC_FrCC_State *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int i, j;

	i=BCCX_GetInt(ty, "flags");

#if 0
	if(!ctx->cur_struct && (i&BGBCC_TYFL_EXTERN))
	{
		s=BGBCC_FrCC_VarTypeString(ctx, ty);
//		s1=name;
		s1=BGBCC_FrCC_QualifyNameNS(ctx, name);
		s2=BGBCC_FrCC_VarTypeFlagsString(ctx, ty);

		BGBCC_FrCC_BindVarSig(ctx, s1, s);
		if(s2 && (*s2))
			BGBCC_FrCC_BindVarInfo(ctx, s1, "flagstr", s2);

		if(!s1)return;
		if(!s1)s1="_";

//		bgbcc_frcc_print(ctx, "EXTERN ");
//		if(*s)bgbcc_frcc_print(ctx, "%s:%s", s1, s);
//			else bgbcc_frcc_print(ctx, "%s", s1);
//		bgbcc_frcc_print(ctx, "\n");
		return;
	}
#endif

	s=BGBCC_FrCC_VarTypeString(ctx, ty);
	s2=BGBCC_FrCC_VarTypeFlagsString(ctx, ty);
//	s1=name;
	s1=BGBCC_FrCC_QualifyNameNS(ctx, name);

	if(!v && BGBCC_FrBC_CheckDefinedContextName(ctx,
		FR2C_CMD_VARDECL, s1))
	{
		return;
	}

	i=BCCX_GetInt(ty, "flags");
//	if(!(i&BGBCC_TYFL_STATIC))
	BGBCC_FrCC_BindVarSig(ctx, s1, s);
	if(s2 && (*s2))
			BGBCC_FrCC_BindVarInfo(ctx, s1, "flagstr", s2);

	if(!s1)return;
	if(!s1)s1="_";

	BGBCC_FrBC_BeginName(ctx, FR2C_CMD_VARDECL, s1);

	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_NAME, s1);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_SIG, s);
	BGBCC_FrBC_AttribStr(ctx, FR2C_ATTR_FLAGS, s2);

	if(v)
	{
		BGBCC_FrBC_Begin(ctx, FR2C_CMD_VARVALUE);
		BGBCC_FrCC_EmitVarValueR2(ctx, v);
		BGBCC_FrBC_End(ctx);
	}

	BGBCC_FrBC_End(ctx);
}

void BGBCC_FrCC_CompileTopStatement(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1;
	int i;

//	printf("BGBCC_FrCC_CompileTopStatement: %s\n", l->tag);

	s=BCCX_Get(l, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)ctx->lln=i;

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

				BGBCC_FrCC_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsP(c, "var"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				v=BCCX_Fetch(c, "value");

				BGBCC_FrCC_EmitTopVar(ctx, s, t, v);
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

		BGBCC_FrCC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "proto"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		BGBCC_FrCC_CompileProto(ctx, t, s, n);
		return;
	}

	if(BCCX_TagIsP(l, "struct"))
	{
		BGBCC_FrCC_CompileStruct(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "union"))
	{
		BGBCC_FrCC_CompileUnion(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "class"))
	{
		BGBCC_FrCC_CompileClass(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "enum"))
	{
		BGBCC_FrCC_CompileEnum(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "import"))
	{
		s=BCCX_Get(l, "name");
//		bgbcc_frcc_print(ctx, "IMPORT %s\n", s);
		BGBCC_FrBC_StubError(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "using"))
	{
		s=BCCX_Get(l, "name");
//		bgbcc_frcc_print(ctx, "IMPORT %s\n", s);
		BGBCC_FrBC_StubError(ctx);
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

		BGBCC_FrCC_BindStructSig(ctx, s, "namespace");
//		bgbcc_frcc_print(ctx, "PACKAGE %s\n", s);
		BGBCC_FrBC_StubError(ctx);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;

#if 1
		if(n)
		{
			c=BCCX_Child(n);
			while(c)
			{
				BGBCC_FrCC_CompileTopStatement(ctx, c);
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

		BGBCC_FrCC_BindStructSig(ctx, s, "namespace");
//		bgbcc_frcc_print(ctx, "PACKAGE %s\n", s);
		BGBCC_FrBC_StubError(ctx);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;
		
		while(c)
		{
			BGBCC_FrCC_CompileTopStatement(ctx, c);
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

	if(BCCX_TagIsP(l, "pragma"))
		{ return; }

	BGBCC_FrCC_Error(ctx, "unhandled top statement %s\n", BCCX_Tag(l));
}


int BGBCC_FrCC_CompileModuleCTX(
	BGBCC_FrCC_State *ctx, char *name, BCCX_Node *l)
{
	BCCX_Node *c;
	char *s;

	ctx->n_warn=0;
	ctx->n_error=0;

	s=BCCX_Get(l, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_Get(l, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_FrCC_CompileStructs(ctx);

//	if(ctx->n_err)return(NULL);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_FrCC_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(-1);
	}

	return(0);
}

char *BGBCC_FrCC_CompileModule(char *name, BCCX_Node *l)
{
	BGBCC_FrCC_State *ctx;
	BCCX_Node *c;
	char *s;


	ctx=bgbcc_malloc(sizeof(BGBCC_FrCC_State));
	memset(ctx, 0, sizeof(BGBCC_FrCC_State));

	ctx->n_warn=0;
	ctx->n_error=0;

	s=BCCX_Get(l, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_Get(l, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_FrCC_CompileStructs(ctx);

//	if(ctx->n_err)return(NULL);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_FrCC_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(NULL);
	}

	return(ctx->ips);
}
