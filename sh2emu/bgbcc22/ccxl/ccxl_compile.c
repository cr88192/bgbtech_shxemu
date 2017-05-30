#include <bgbccc.h>


void BGBCC_CCXL_Warn(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_warn++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: warning: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_Error(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_error++;
//	bgbcc_frcc_err++;
	printf("%s:%d: error: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_Note(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_note++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: note: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_StubWarnLLn(BGBCC_TransState *ctx,
	char *file, int line)
{
	BGBCC_CCXL_Warn(ctx, "StubWarn %s:%d\n", file, line);
}

void BGBCC_CCXL_StubErrorLLn(BGBCC_TransState *ctx,
	char *file, int line)
{
	BGBCC_CCXL_Error(ctx, "StubError %s:%d\n", file, line);
}

void BGBCC_CCXL_TagErrorLLn(BGBCC_TransState *ctx, int tag,
	char *file, int line)
{
	BGBCC_CCXL_Error(ctx, "TagError(%04X) %s:%d\n", tag, file, line);
}

void BGBCC_CCXL_TagWarnLLn(BGBCC_TransState *ctx, int tag,
	char *file, int line)
{
	BGBCC_CCXL_Warn(ctx, "TagWarn(%04X) %s:%d\n", tag, file, line);
}

ccxl_label BGBCC_CCXL_GenSym(BGBCC_TransState *ctx)
{
//	char buf[32];
	ccxl_label l;
//	int id;

	l.id=CCXL_LBL_GENSYMBASE+(ctx->gs_seq++);
	return(l);

//	sprintf(buf, "GS%d", ctx->gs_seq++);
//	return(bgbcc_strdup(buf));
}

ccxl_label BGBCC_CCXL_GenSym2(BGBCC_TransState *ctx)
{
	return(BGBCC_CCXL_GenSym(ctx));
//	return(BS1_RStrDup(BASM_GenSym()));
}

int BGBCC_CCXL_CompileSwitch(BGBCC_TransState *ctx, BCCX_Node *l)
{
	ccxl_label *cl;
	char *s;
	ccxl_label l0;
	int i, j, ncl;
	BCCX_Node *c, *t, *u, *v;

	cl=bgbcc_malloc(4096*sizeof(ccxl_label));

	ncl=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case") ||
			BCCX_TagIsP(c, "case_default"))
		{
			cl[ncl++]=BGBCC_CCXL_GenSym(ctx);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "cond"));

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case"))
		{
			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(c, "value"));
			BGBCC_CCXL_CompileJmpCond(ctx, "==", cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_CCXL_CompileJmp(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}


	l0=BGBCC_CCXL_GenSym(ctx);
	ctx->breakstack[ctx->breakstackpos++]=l0;
	BGBCC_CCXL_CompileBreak(ctx);

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case") ||
			BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_CCXL_EmitLabel(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		BGBCC_CCXL_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_EmitLabel(ctx, l0);
	BGBCC_CCXL_StackPop(ctx);

	bgbcc_free(cl);
	ctx->breakstackpos--;
	return(0);
}

int BGBCC_CCXL_CompileInitArray(BGBCC_TransState *ctx,
	char *name, char *sig, BCCX_Node *l)
{
	BCCX_Node *cur;
	ccxl_type ty, bty;
	char *s1;
	int idx;

	BGBCC_CCXL_TypeFromSig(ctx, &ty, sig);
	BGBCC_CCXL_TypeDerefType(ctx, ty, &bty);

	s1=BGBCC_CCXL_TypeGetSig(ctx, bty);
	
	cur=BCCX_Child(l); idx=0;
	while(cur)
	{
		BGBCC_CCXL_CompileExpr(ctx, cur);
		BGBCC_CCXL_StackCastSig(ctx, s1);

		BGBCC_CCXL_PushLoad(ctx, name);
		BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
		idx++;

		cur=BCCX_Next(cur);
	}
}

void BGBCC_CCXL_CompileInitVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *type, BCCX_Node *value)
{
	char *s0;
	BCCX_Node *t;

	if(value)
	{
		s0=BGBCC_CCXL_VarTypeString(ctx, type);
		if(*s0=='A')
		{
			BGBCC_CCXL_StackInitVar(ctx, name);
			BGBCC_CCXL_CompileInitArray(ctx, name, s0, value);
			return;
		}

		t=BGBCC_CCXL_ReduceExpr(ctx, value);

//		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_CompileExprAsType(ctx, type, t);
		BGBCC_CCXL_StackInitVarValue(ctx, name);
	}else
	{
		BGBCC_CCXL_StackInitVar(ctx, name);
	}
}

void BGBCC_CCXL_CompileStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	BCCX_Node *ni, *nc, *ns, *nb;
	ccxl_label l0, l1, l2;
	char *s0, *s1, *s2;
	int i0, i1, i2, i3;
	int i, j, k;

	if(!l)
	{
		return;
	}

	s0=BCCX_Get(l, "fn");
	if(s0 && (s0!=ctx->lfn))
	{
		if(!ctx->reqlfn)
			{ BGBCC_CCXL_StackFn(ctx, s0); }
		ctx->lfn=s0;
	}

	i=BCCX_GetInt(l, "ln");
	if((i>0) && (i!=ctx->lln))
	{
		if(!ctx->reqlfn)
			{ BGBCC_CCXL_StackLn(ctx, i); }
		ctx->lln=i;
	}

	if(ctx->reqlfn)
	{
		BGBCC_CCXL_StackFn(ctx, ctx->lfn);
		BGBCC_CCXL_StackLn(ctx, ctx->lln);
		ctx->reqlfn=0;
	}

	l=BGBCC_CCXL_ReduceExpr(ctx, l);
	if(!l)
	{
		printf("BGBCC_CCXL_CompileStatement: Statement Reduced Away\n");
		return;
	}

	if(BCCX_TagIsP(l, "switch"))
	{
		BGBCC_CCXL_CompileSwitch(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "goto"))
	{
		BGBCC_CCXL_CompileJmp(ctx,
			BGBCC_CCXL_LabelFromName(ctx, BCCX_Get(l, "name")));
		return;
	}

	if(BCCX_TagIsP(l, "label"))
	{
		BGBCC_CCXL_EmitLabel(ctx,
			BGBCC_CCXL_LabelFromName(ctx, BCCX_Get(l, "name")));
		return;
	}

	if(BCCX_TagIsP(l, "continue"))
	{
		l0=ctx->contstack[ctx->contstackpos-1];
		BGBCC_CCXL_CompileJmp(ctx, l0);
		return;
	}

	if(BCCX_TagIsP(l, "break"))
	{
		l0=ctx->breakstack[ctx->breakstackpos-1];
		BGBCC_CCXL_CompileJmp(ctx, l0);
		return;
	}

	if(BCCX_TagIsP(l, "return"))
	{
		t=BCCX_Fetch(l, "value");
		if(!t)
		{
			BGBCC_CCXL_StackRetV(ctx);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackRet(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		s0=BCCX_Get(l, "op");

		if(s0)
		{
			if(BCCX_TagIsP(ln, "ref"))
			{
				s1=BCCX_Get(ln, "name");
				BGBCC_CCXL_CompileExpr(ctx, ln);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
				return;
			}

			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_StackBinaryOp(ctx, s0);
			BGBCC_CCXL_CompileAssign(ctx, ln);
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
				ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
				rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
				BGBCC_CCXL_CompileExpr(ctx, ln2);
				BGBCC_CCXL_CompileExpr(ctx, rn2);
				BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
				return;
			}
		}

//		if(BCCX_TagIsP(rn, "ref"))
//		{
//			s0=BCCX_Get(rn, "name");
//			if(!strcmp(s0, "cshift_empty"))
//			{
//				k=0;
//			}
//		}

		BGBCC_CCXL_CompileExpr(ctx, rn);
		BGBCC_CCXL_CompileAssign(ctx, ln);
		return;
	}

	if(BCCX_TagIsP(l, "preinc") || BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			BGBCC_CCXL_StackUnaryOpName(ctx, "++", s0);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec") || BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			BGBCC_CCXL_StackUnaryOpName(ctx, "--", s0);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "defun"))	{ return; }

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

			BGBCC_CCXL_CompileInitVar(ctx, s0, t, v);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_CCXL_CompileStatement(ctx, t);
			return;
		}

		l0=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileJCF(ctx, t, l0);

		t=BCCX_Fetch(l, "else");
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "then"));
		if(t)
		{
			l1=BGBCC_CCXL_GenSym(ctx);
			BGBCC_CCXL_CompileJmp(ctx, l1);
			BGBCC_CCXL_EmitLabel(ctx, l0);
			BGBCC_CCXL_CompileStatement(ctx, t);
			l0=l1;
		}
		BGBCC_CCXL_EmitLabel(ctx, l0);
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_CCXL_CompileStatement(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "for"))
	{
		ni=BCCX_Fetch(l, "init");
		nc=BCCX_Fetch(l, "cond");
		nb=BCCX_Fetch(l, "body");
		ns=BCCX_Fetch(l, "step");

		i=1;
		i=i&&BGBCC_CCXL_IsFixIntAssignRVP(ctx, ni, &s0, &i0);
		i=i&&BGBCC_CCXL_IsFixIntCompareRVP(ctx, nc, &s1, &s2, &i1);
		i=i&&(	BGBCC_CCXL_IsTagVarRVP(ctx, ns, "postinc", s0) ||
				BGBCC_CCXL_IsTagVarRVP(ctx, ns, "preinc", s0));
		i=i&&(!strcmp(s0, s1));
		i=i&&(!strcmp(s2, "<") || !strcmp(s2, "<="));
		i=i&&(	BCCX_TagIsP(nb, "assign") ||
				BCCX_TagIsP(nb, "funcall") ||
				BCCX_TagIsP(nb, "setindex"));
		
		if(i && ((i1-i0)<=8))
		{
			if(!strcmp(s2, "<="))i1++;
			for(i=i0; i<i1; i++)
			{
				BGBCC_CCXL_StackPushConstInt(ctx, i);
				BGBCC_CCXL_PopStore(ctx, s0);
				BGBCC_CCXL_CompileStatement(ctx, nb);
			}

			return;
		}

		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);

		ctx->contstack[ctx->contstackpos++]=l1;
		ctx->breakstack[ctx->breakstackpos++]=l2;

		BGBCC_CCXL_CompileStatement(ctx, ni);

		BGBCC_CCXL_EmitLabel(ctx, l0);
		BGBCC_CCXL_CompileJCF(ctx, nc, l2);

		BGBCC_CCXL_CompileStatement(ctx, nb);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_CompileStatement(ctx, ns);
		BGBCC_CCXL_CompileJmp(ctx, l0);

		BGBCC_CCXL_EmitLabel(ctx, l2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "while"))
	{
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);

		ctx->contstack[ctx->contstackpos++]=l1;
		ctx->breakstack[ctx->breakstackpos++]=l2;

		BGBCC_CCXL_EmitLabel(ctx, l1);

		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);

		if(i==0)return;

		if(i==1)
		{
			BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));
			BGBCC_CCXL_CompileContinue(ctx);
			BGBCC_CCXL_EmitLabel(ctx, l2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_CCXL_CompileJCF(ctx, t, l2);
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));
		BGBCC_CCXL_CompileContinue(ctx);
		BGBCC_CCXL_EmitLabel(ctx, l2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}


	if(BCCX_TagIsP(l, "do_while"))
	{
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		ctx->contstack[ctx->contstackpos++]=l1;
		ctx->breakstack[ctx->breakstackpos++]=l2;

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)return;

		if(i==1)
		{
			BGBCC_CCXL_CompileContinue(ctx);
			BGBCC_CCXL_EmitLabel(ctx, l2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_CCXL_CompileJCT(ctx, t, l1);
		BGBCC_CCXL_EmitLabel(ctx, l2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "funcall"))
	{
		BGBCC_CCXL_CompileFuncallStmt(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "try_catch"))
	{
		//lame: for now just spit out try block...
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		return;
	}

	if(BCCX_TagIsP(l, "msvc_asm"))
	{
		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "gcc_asm"))
	{
		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackPop(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "asm_blob"))
	{
		c=BCCX_Child(l);
		s0=BCCX_Text(c);

		BGBCC_CCXL_AddAsmBlob(ctx, s0);
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "linecomment")) { return; }
	if(BCCX_TagIsP(l, "blockcomment")) { return; }

	BGBCC_CCXL_CompileExpr(ctx, l);
	BGBCC_CCXL_StackPop(ctx);
}


char *BGBCC_CCXL_VarTypeString_FlattenExpr(BGBCC_TransState *ctx,
	char *dst, BCCX_Node *l, int fl)
{
	BCCX_Node *c, *n;
	char *s, *t;
	
	t=dst;
	
	if(BCCX_TagIsP(l, "ref"))
	{
		s=BCCX_Get(l, "name");
		t=BGBCC_CCXL_VarTypeString_FlattenName(ctx, dst, s, fl);
		return(t);
	}

	if(BCCX_TagIsP(l, "emptyindex"))
	{
		*t++='Q';
		n=BCCX_Fetch(l, "array");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsP(l, "sharpcall"))
	{
		*t++='<';
		c=BCCX_Fetch(l, "args");
		while(c)
		{
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, c, fl);
			c=BCCX_Next(c);
		}
		*t++='>';
		n=BCCX_Fetch(l, "value");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
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
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}
	
	return(t);
}

char *BGBCC_CCXL_VarTypeString_FlattenName(BGBCC_TransState *ctx,
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

		if(!strcmp(s, "builtin_va_list") ||
				!strcmp(s, "gnuc_va_list"))
			{ *t++='D'; *t++='z'; }

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
					(fl&BGBCC_TYFL_EXPNAME) || ctx->ril_ip)
				{
					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
					}

					sprintf(t, "X%s;", s);
					t+=strlen(t);
				}else
				{
					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
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

int BGBCC_CCXL_VarTypeString_ModifierChar(BGBCC_TransState *ctx, int i)
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

char *BGBCC_CCXL_VarTypeString_FlattenModifiers(BGBCC_TransState *ctx,
	char *t, int fl)
{
	int i, j, k;

	k=0;
	for(i=0; i<30; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)k++;
	}
	
	if(k>2)
	{
		*t++='M';
		for(i=0; i<31; i++)
		{
			j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
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
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)
		{
			*t++='F';
			if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
			if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
			if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
			if(j&0xFF)*t++=j&0xFF;
		}
	}
	return(t);
}

char *BGBCC_CCXL_VarTypeString_FlattenModifiers2(BGBCC_TransState *ctx,
	char *t, int fl)
{
	int i, j, k;

	for(i=0; i<31; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
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

char *BGBCC_CCXL_VarTypeString(BGBCC_TransState *ctx, BCCX_Node *ty)
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
				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsP(n, "int"))
				{
					if(BCCX_TagIsP(n, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetInt(n, "value");
				if(i<0)
				{
					BGBCC_CCXL_Warn(ctx,
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
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(
				ctx, t, n, i|BGBCC_TYFL_EXPNAME);
		}else
		{
			if(s)
			{
				t=BGBCC_CCXL_VarTypeString_FlattenName(
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

char *BGBCC_CCXL_VarImageTypeString(BGBCC_TransState *ctx, BCCX_Node *ty)
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
				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsP(n, "int"))
				{
					if(BCCX_TagIsP(n, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					if(BCCX_TagIsP(n, "sizeof"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Sizeof array-size Hack\n");
						*t++='1024'; c=BCCX_Next(c); continue;
					}

					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
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
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, i);
		}else
		{
			if(s)
			{
				t=BGBCC_CCXL_VarTypeString_FlattenName(ctx, t, s, i);
			}else
			{
				*t++='v';
			}
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_CCXL_VarTypeFlagsString(BGBCC_TransState *ctx, BCCX_Node *ty)
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

		t=BGBCC_CCXL_VarTypeString_FlattenModifiers2(ctx, t, i);
	}
	*t=0;

	return(bgbcc_strdup(buf));
}

void BGBCC_CCXL_EmitVarFunc(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *args)
{
	char *s, *s1, *s2;
	s64 li;
	int i;

//	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	s=BGBCC_CCXL_VarSigProto(ctx, ty, name, args);
	s1=name;
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";

	if(!s1)s1="_";

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	li=BCCX_GetInt(ty, "flags");
	
	if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "dllexport") ||
		!strcmp(s1, "main") || !strcmp(s1, "WinMain"))
	{
		li|=BGBCC_TYFL_EXPNAME|BGBCC_TYFL_DLLEXPORT;
	}
	
	BGBCC_CCXL_AttribLong(ctx, CCXL_ATTR_FLAGS, li);
}

void BGBCC_CCXL_EmitVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s=BGBCC_CCXL_VarImageTypeString(ctx, ty);
	s1=name;
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";

	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	if(v)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_EmitVarValueR2(ctx, v);
		BGBCC_CCXL_End(ctx);
	}

	BGBCC_CCXL_End(ctx);
}

void BGBCC_CCXL_EmitVar2(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char tb[256];
	char *s, *s1, *s2;
	int i, op;

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s=BGBCC_CCXL_VarImageTypeString(ctx, ty);
	s1=name;

	if(!s)s="v";

	i=BCCX_GetInt(ty, "flags");
	if(!s1)s1="_";

	op=CCXL_CMD_VARDECL;
	if(i&BGBCC_TYFL_STATIC)
		op=CCXL_CMD_STATICVARDECL;

	BGBCC_CCXL_BeginName(ctx, op, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);

	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	if(ctx->cur_structdef && BCCX_TagIsP(ctx->cur_structdef, "enum"))
	{
		if(v)
		{
			ctx->cur_idx4=BCCX_GetInt(v, "value");
		}

		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
			ctx->cur_idx4++);
		BGBCC_CCXL_End(ctx);
	}else
	{
		if(v)
		{
			BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
			BGBCC_CCXL_EmitVarValueR2(ctx, v);
			BGBCC_CCXL_End(ctx);
		}
	}

	BGBCC_CCXL_End(ctx);

	if(ctx->cur_struct)
	{
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"field", ctx->cur_idx++, s1);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s1, s);
//		BGBCC_CCXL_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindFieldInfo(ctx, ctx->cur_struct, s1, "flagstr", s2);
	}
}

void BGBCC_CCXL_EmitVar3(BGBCC_TransState *ctx, char *name, char *ty)
{
	char *s, *s1;
	int i;

	if(BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s=ty;
	s1=name;

	if(!s)s="v";
	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_End(ctx);

	if(ctx->cur_struct)
	{
//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"method", ctx->cur_idx2++, s1);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s1, s);
	}
}

void BGBCC_CCXL_EmitVarProperty(BGBCC_TransState *ctx, BCCX_Node *l)
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
		s1=BGBCC_CCXL_GenProtoSig(ctx, t, NULL);
		BGBCC_CCXL_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_CCXL_GenProtoSig(ctx, NULL, n);
		BGBCC_CCXL_EmitVar3(ctx, s2, s1);
	}

	if(ctx->cur_struct)
	{
		s1=BGBCC_CCXL_VarTypeString(ctx, t);
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);
		i=BCCX_GetInt(t, "flags");

//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"property", ctx->cur_idx3++, s);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s, s1);
//		BGBCC_CCXL_BindFieldInfoI(ctx, ctx->cur_struct, s, "flags", i);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindFieldInfo(ctx, ctx->cur_struct, s, "flagstr", s2);
	}
}

void BGBCC_CCXL_CompileVarProperty(BGBCC_TransState *ctx, BCCX_Node *l)
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
		BGBCC_CCXL_CompileBlock(ctx, t, s2, NULL, v);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_CCXL_CompileBlock(ctx, NULL, s2, n, v);
	}
#endif

}

int BGBCC_CCXL_CompileVarDummyP(BGBCC_TransState *ctx, BCCX_Node *l)
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

void BGBCC_CCXL_CompileVarStatementBlock(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	
	c=l;
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
}

int BGBCC_CCXL_CompileVarStatementCompound(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		BGBCC_CCXL_CompileVarStatementBlock(ctx, c);
 		return(1);
	}

	if(BCCX_TagIsP(l, "if"))
	{
		c=BCCX_Fetch(l, "then");
		BGBCC_CCXL_CompileVarStatement(ctx, c);

		c=BCCX_Fetch(l, "else");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatement(ctx, c);
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
		BGBCC_CCXL_CompileVarStatementBlock(ctx, c);
 		return(1);
	}
	return(0);
}

void BGBCC_CCXL_CompileVarStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s0, *s1;
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
//			v=NULL;

			BGBCC_CCXL_EmitVar2(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
//		s0=BCCX_Get(l, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetInt(l, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_CCXL_GenProtoSig(ctx, t, n);
		BGBCC_CCXL_EmitVar3(ctx, s, s1);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_CCXL_GenProtoSig(ctx, NULL, n);
		BGBCC_CCXL_EmitVar3(ctx, "new", s1);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_CCXL_EmitVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CCXL_CompileVarStatementCompound(ctx, l))
		return;

	if(BGBCC_CCXL_CompileVarDummyP(ctx, l))
		return;

	BGBCC_CCXL_Error(ctx, "Unhandled var statement %s\n", BCCX_Tag(l));
}

void BGBCC_CCXL_CompileVarStatement2(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t, *ntl;
	char *s, *s0;
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

			BGBCC_CCXL_EmitTopVar(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
//		s0=BCCX_Get(l, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetInt(l, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");
		ntl=BCCX_Fetch(l, "argdecls");

		BGBCC_CCXL_CompileBlock2(ctx, t, s, n, v, ntl);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		s="new";
		t=NULL;
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_CCXL_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_CCXL_CompileVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CCXL_CompileVarDummyP(ctx, l))
		return;

	BGBCC_CCXL_Error(ctx, "Unhandled var statement %s\n", BCCX_Tag(l));
}

BCCX_Node *BGBCC_CCXL_ArgDeclsTypeForName(BGBCC_TransState *ctx,
	BCCX_Node *adecl, char *name)
{
	BCCX_Node *c, *t, *n, *u, *v, *c2;
	char *s0, *s1;
	
	if(!adecl || !name)
		return(NULL);
	
	c=adecl;
	while(c)
	{
		if(!BCCX_TagIsP(c, "vars"))
		{
			c=BCCX_Next(c);
			continue;
		}
		
		c2=BCCX_Child(c);
		while(c2)
		{
			if(!BCCX_TagIsP(c2, "var"))
				{ c2=BCCX_Next(c2); continue; }

			s0=BCCX_Get(c2, "name");
			t=BCCX_FindTag(c2, "type");
//			v=BCCX_Fetch(c2, "value");

			if(!strcmp(s0, name))
				return(t);

//			BGBCC_CCXL_CompileInitVar(ctx, s0, t, v);
			c2=BCCX_Next(c2);
		}

		c=BCCX_Next(c);
	}
	
	return(NULL);
}

BCCX_Node *BGBCC_CCXL_CompileBlock(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	return(BGBCC_CCXL_CompileBlock2(ctx, type, name, args, body, NULL));
}

BCCX_Node *BGBCC_CCXL_CompileBlock2(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body,
	BCCX_Node *adecl)
{
	ccxl_label cname[16], bname[16];

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u, *t2;
	BCCX_Node *ocf_ty;
	char *ocf_n;
	char *s0, *s1;
	int tk;
	int i, j, k;

	name=BGBCC_CCXL_QualifyNameNS(ctx, name);

	BGBCC_CCXL_EmitSigProto(ctx, type, name, args);

	i=0;

	//hack: disable extern inline bastards...
	if(type)i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_EXTERN)
		return(NULL);

	ctx->contstack=cname;
	ctx->breakstack=bname;

	ocf_n=ctx->cf_n;
	ocf_ty=ctx->cf_ty;
	ctx->cf_n=name;
	ctx->cf_ty=type;

	tk=0;
	if(BCCX_TagIsP(body, "begin"))
	{
		tk=BCCX_GetInt(body, "tokens");
		body=BCCX_Child(body);
	}

	BGBCC_CCXL_BeginName(ctx, (ctx->cur_struct)?
		CCXL_CMD_METHOD:CCXL_CMD_FUNCTION, ctx->cf_n);

	BGBCC_CCXL_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_SRCTOK, tk);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		s0=BCCX_Get(c, "name");
		
		if(adecl)
		{
			t2=BGBCC_CCXL_ArgDeclsTypeForName(ctx, adecl, s0);
			if(t2)t=t2;
		}
		
		BGBCC_CCXL_EmitVar(ctx, s0, t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_LOCALS);
	c=body;
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_BODY);
	ctx->reqlfn=1;

//	BGBCC_CCXL_StackFn(ctx, ctx->lfn);
//	BGBCC_CCXL_StackLn(ctx, ctx->lln);

	c=body;
	while(c)
	{
		BGBCC_CCXL_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);
	BGBCC_CCXL_End(ctx);

	ctx->cf_n=ocf_n;
	ctx->cf_ty=ocf_ty;

	return(NULL);
}

char *BGBCC_CCXL_GenProtoSig(BGBCC_TransState *ctx,
	BCCX_Node *type, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

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
			s=BGBCC_CCXL_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	if(type)
	{
		s=BGBCC_CCXL_VarTypeString(ctx, type);
		if(s) { strcpy(t, s); t+=strlen(t); }
	}else
	{
		*t++='Pv';
	}

	*t++=0;

	return(bgbcc_strdup(tb));
}

void BGBCC_CCXL_EmitSigProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

	if(!type)return;

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
			s=BGBCC_CCXL_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_CCXL_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

//	BGBCC_CCXL_BindSig(ctx, name, tb);

//	s=BGBCC_CCXL_VarTypeFlagsString(ctx, type);
//	if(s && (*s))
//			BGBCC_CCXL_BindVarInfo(ctx, name, "flagstr", s);
}

char *BGBCC_CCXL_VarSigProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

	if(!type)
		return(NULL);

	i=BCCX_GetInt(type, "flags");

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
			s=BGBCC_CCXL_VarImageTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_CCXL_VarImageTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;
	
	return(bgbcc_strdup(tb));
}

BCCX_Node *BGBCC_CCXL_CompileProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

	name=BGBCC_CCXL_QualifyNameNS(ctx, name);

	/* avoid redefining the same stuff */
	if(BGBCC_CCXL_LookupGlobal(ctx, name))
		{ return(NULL); }

	BGBCC_CCXL_EmitSigProto(ctx, type, name, args);

	ctx->cf_n=name;
	ctx->cf_ty=type;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_PROTOTYPE, ctx->cf_n);
	BGBCC_CCXL_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_CCXL_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_End(ctx);

	return(NULL);
}

void BGBCC_CCXL_CompileStruct(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0, *s1;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_CCXL_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	cur=BGBCC_CCXL_LookupStructure(ctx, s);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetInt(l, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "struct");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_STRUCT, BCCX_Get(l, "name"));

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, BCCX_Get(l, "name"));

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_End(ctx);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CCXL_CompileUnion(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_CCXL_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	cur=BGBCC_CCXL_LookupStructure(ctx, s);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetInt(l, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "union");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_UNION, BCCX_Get(l, "name"));

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, BCCX_Get(l, "name"));

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CCXL_CompileClass(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0, *qn;
	int i, j, j1, j2;

	s=BCCX_Get(l, "name");
	qn=BGBCC_CCXL_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	cur=BGBCC_CCXL_LookupStructure(ctx, qn);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetInt(l, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, qn, "class");
//	BGBCC_CCXL_BindStructInfoI(ctx, qn, "flags", i);

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

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_CLASS, qn);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, qn);

	c=BCCX_Fetch(l, "super");
	if(c)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_EXTENDS);
		while(c)
		{
			s=BGBCC_CCXL_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
				BGBCC_CCXL_End(ctx);
			}
			c=BCCX_Next(c);
		}
		BGBCC_CCXL_End(ctx);
	}

	c=BCCX_Fetch(l, "impl");
	if(c)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_IMPLEMENTS);
		while(c)
		{
			s=BGBCC_CCXL_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
				BGBCC_CCXL_End(ctx);
			}
			c=BCCX_Next(c);
		}
		BGBCC_CCXL_End(ctx);
	}

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement2(ctx, c);
		c=BCCX_Next(c);
	}

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
	ctx->cur_idx2=j1;
	ctx->cur_idx3=j2;
}

void BGBCC_CCXL_CompileEnum(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s1, *s2, *os0;
	int op;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_CCXL_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "enum");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;
	ctx->cur_idx4=0;

	c=BCCX_Fetch(l, "defs");
	while(c)
	{
		if(BCCX_TagIsP(c, "def"))
		{
			op=CCXL_CMD_VARDECL;
			j=BCCX_GetInt(c, "value");
			s1=BCCX_Get(c, "name");
			s2="i";

			if(BGBCC_CCXL_CheckDefinedContextName(ctx,
				CCXL_CMD_VARDECL, s1))
			{
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CCXL_BeginName(ctx, op, s1);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s2);

			s2="k";
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

			BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
			BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, j++);
			BGBCC_CCXL_End(ctx);

			BGBCC_CCXL_End(ctx);
		}
		c=BCCX_Next(c);
	}

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

BCCX_Node *BGBCC_CCXL_CompileSProto(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u;
	char *s, *s1;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_S_PROTOTYPE, BCCX_Get(l, "name"));

	t=BCCX_FindTag(l, "type");
	BGBCC_CCXL_EmitVarFunc(ctx, BCCX_Get(l, "name"), t,
		BCCX_Fetch(l, "args"));

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
	c=BCCX_Fetch(l, "args");
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}
		if(!BCCX_TagIsP(c, "var"))
			{ c=BCCX_Next(c); continue; }

		t=BCCX_FindTag(c, "type");
		BGBCC_CCXL_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);
	BGBCC_CCXL_End(ctx);


	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");
	u=BCCX_Fetch(l, "args");

//	s1=BGBCC_CCXL_GenProtoSig(ctx, t, u);
//	BGBCC_CCXL_BindStructSig(ctx, s, "func");
//	BGBCC_CCXL_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

BCCX_Node *BGBCC_CCXL_GetNodeAttribute(BGBCC_TransState *ctx,
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

		cn1=BCCX_Child(cn);
		while(cn1)
		{
			if(!BCCX_TagIsP(cn1, "attr"))
				{ cn1=BCCX_Next(cn1); continue; }

			s=BCCX_Get(cn1, "name");
			if(s && !strcmp(s, name))
				return(cn1);

			cn1=BCCX_Next(cn1);
		}
		cn=BCCX_Next(cn);
	}
	
	return(NULL);
}

char *BGBCC_CCXL_GetNodeAttributeString(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *attr, *cn;
	char *s;
	
	attr=BGBCC_CCXL_GetNodeAttribute(ctx, l, name);
	if(!attr)return(NULL);

	s=BCCX_Get(attr, "name");

	cn=BCCX_Fetch(attr, "args");
	if(!cn)return(NULL);

	if(!BCCX_TagIsP(cn, "string"))
		return(NULL);
	s=BCCX_Get(cn, "value");
	return(s);
}

void BGBCC_CCXL_CompileTypedef(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

	if(BCCX_TagIsP(l, "var"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");

//		s1=BGBCC_CCXL_VarTypeString(ctx, t);
//		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);
//		BGBCC_CCXL_BindTypeSig(ctx, s, s1);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindTypeInfo(ctx, s, "flagstr", s2);

		s2=BGBCC_CCXL_GetNodeAttributeString(ctx, t, "dyt_name");
		if(s2)
		{
			printf("BGBCC_CCXL_CompileTypedef: DyType %s->%s\n", s, s2);
		
//			BGBCC_CCXL_BindTypeInfo(ctx, s, "dytype", s2);
//			BGBCC_CCXL_BindDyTypeSig(ctx, s2, s1);
		}
	}

	return;
}

void BGBCC_CCXL_CompileStructs(BGBCC_TransState *ctx)
{
	BCCX_Node *c, *t, *n, *l;
	BGBCC_CCXL_LiteralInfo *litobj;
	int i;

	c=ctx->types;
	while(c)
	{
		BGBCC_CCXL_CompileTypedef(ctx, c);
		c=BCCX_Next(c);
	}

	c=ctx->structs;
	while(c)
	{
		l=c;
		if(BCCX_TagIsP(c, "struct"))BGBCC_CCXL_CompileStruct(ctx, c);
		if(BCCX_TagIsP(c, "union"))BGBCC_CCXL_CompileUnion(ctx, c);
		if(BCCX_TagIsP(c, "enum"))BGBCC_CCXL_CompileEnum(ctx, c);
		if(BCCX_TagIsP(c, "s_proto"))BGBCC_CCXL_CompileSProto(ctx, c);
		if(BCCX_TagIsP(c, "func"))BGBCC_CCXL_CompileSProto(ctx, c);

		c=BCCX_Next(c);
	}

#if 1
	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 0);
	}
#endif
}

void BGBCC_CCXL_EmitVarValue(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BGBCC_CCXL_RegisterInfo *decl;
	BCCX_Node *t;
	ccxl_register rval;
	char *s;
	int i;

	v=BGBCC_CCXL_ReduceExprConst(ctx, v);

	if(BCCX_TagIsP(v, "int"))
	{
		s=BCCX_Get(v, "tysuf");
		if(s &&
			(!strcmp(s, "L") || !strcmp(s, "LL") ||
			 !strcmp(s, "UL") || !strcmp(s, "ULL") ||
			 !strcmp(s, "LX") || !strcmp(s, "ULX")))
		{
			BGBCC_CCXL_LiteralLong(ctx, CCXL_ATTR_VALUE,
				BCCX_GetInt(v, "value"));
			return;
		}
	
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
			BCCX_GetInt(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "real"))
	{
		s=BCCX_Get(v, "tysuf");
		if(s && !strcmp(s, "F"))
		{
			BGBCC_CCXL_LiteralFloat(ctx, CCXL_ATTR_VALUE,
				BCCX_GetFloat(v, "value"));
			return;
		}

		BGBCC_CCXL_LiteralDouble(ctx, CCXL_ATTR_VALUE,
			BCCX_GetFloat(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "string"))
	{
		BGBCC_CCXL_LiteralStr(ctx, CCXL_ATTR_VALUE,
			BCCX_Get(v, "value"));
		return;
	}

	if(BCCX_TagIsP(v, "charstring"))
	{
		s=BCCX_Get(v, "value");
		if(s) { i=BGBCP_ParseChar(&s); }
			else i=0;
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, i);
		return;
	}

	if(BCCX_TagIsP(v, "ref"))
	{
		s=BCCX_Get(v, "name");
		decl=BGBCC_CCXL_LookupGlobal(ctx, s);
		if(decl)
		{
			rval=decl->value;
			if(BGBCC_CCXL_IsRegImmIntP(ctx, rval))
			{
				BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmIntValue(ctx, rval));
				return;
			}
			if(BGBCC_CCXL_IsRegImmLongP(ctx, rval))
			{
				BGBCC_CCXL_LiteralLong(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmLongValue(ctx, rval));
				return;
			}

			if(BGBCC_CCXL_IsRegImmFloatP(ctx, rval))
			{
				BGBCC_CCXL_LiteralFloat(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmFloatValue(ctx, rval));
				return;
			}
			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, rval))
			{
				BGBCC_CCXL_LiteralDouble(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmDoubleValue(ctx, rval));
				return;
			}

			if(decl->regtype==CCXL_LITID_FUNCTION)
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}

			if(decl->sig && (*decl->sig=='A'))
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}

			if(ctx->cf_n)
				return;

			BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: "
				"Bad Reg Literal Type %08X\n", rval.val);
			return;
		}

		if(ctx->cf_n)
			return;

		BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: "
			"Undeclared Var %s\n", s);
		return;
	}

	if(BGBCC_CCXL_IsUnaryP(ctx, v, "&"))
	{
		t=BCCX_Fetch(v, "value");

		if(BCCX_TagIsP(t, "ref"))
		{
			s=BCCX_Get(t, "name");
			decl=BGBCC_CCXL_LookupGlobal(ctx, s);
			if(decl)
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}
		}
	}

	if(BCCX_TagIsP(v, "cast"))
	{
		t=BCCX_Fetch(v, "value");
		BGBCC_CCXL_EmitVarValue(ctx, t);
		return;
	}

	if(ctx->cf_n)
		return;

	s=BCCX_Tag(v);
	BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: Bad Value %s\n", s);
	return;
}

void BGBCC_CCXL_EmitVarValueR(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_CCXL_EmitVarValueR(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	BGBCC_CCXL_EmitVarValue(ctx, v);
}

void BGBCC_CCXL_EmitVarValueR2(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_LIST);

		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_CCXL_EmitVarValueR2(ctx, c);
			c=BCCX_Next(c);
		}

		BGBCC_CCXL_End(ctx);
		return;
	}

	BGBCC_CCXL_EmitVarValue(ctx, v);
}

char *BGBCC_CCXL_QualifyNameNS(BGBCC_TransState *ctx, char *name)
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

void BGBCC_CCXL_EmitTopVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int i, j;

	i=BCCX_GetInt(ty, "flags");

	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
	s1=BGBCC_CCXL_QualifyNameNS(ctx, name);

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, s1))
	{
		return;
	}

	i=BCCX_GetInt(ty, "flags");
//	BGBCC_CCXL_BindVarSig(ctx, s1, s);
//	if(s2 && (*s2))
//			BGBCC_CCXL_BindVarInfo(ctx, s1, "flagstr", s2);

	if(!s1)return;
	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	if(v)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_EmitVarValueR2(ctx, v);
		BGBCC_CCXL_End(ctx);
	}

	BGBCC_CCXL_End(ctx);
}

void BGBCC_CCXL_CompileTopStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t, *ntl;
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
			if(BCCX_TagIsP(c, "proto"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				n=BCCX_Fetch(c, "args");

				BGBCC_CCXL_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsP(c, "var"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				v=BCCX_Fetch(c, "value");

				BGBCC_CCXL_EmitTopVar(ctx, s, t, v);
			}

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
//		s0=BCCX_Get(l, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetInt(l, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");
//		tk=BCCX_GetInt(l, "tokens");
		ntl=BCCX_Fetch(l, "argdecls");

		BGBCC_CCXL_CompileBlock2(ctx, t, s, n, v, ntl);
		return;
	}

	if(BCCX_TagIsP(l, "proto"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		BGBCC_CCXL_CompileProto(ctx, t, s, n);
		return;
	}

	if(BCCX_TagIsP(l, "struct"))
	{
		BGBCC_CCXL_CompileStruct(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "union"))
	{
		BGBCC_CCXL_CompileUnion(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "class"))
	{
		BGBCC_CCXL_CompileClass(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "enum"))
	{
		BGBCC_CCXL_CompileEnum(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "import"))
	{
		s=BCCX_Get(l, "name");
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "using"))
	{
		s=BCCX_Get(l, "name");
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "package"))
	{
		s=BCCX_Get(l, "name");
		n=BCCX_FindTag(l, "body");

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

//		BGBCC_CCXL_BindStructSig(ctx, s, "namespace");
		BGBCC_CCXL_StubError(ctx);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;

#if 1
		if(n)
		{
			c=BCCX_Child(n);
			while(c)
			{
				BGBCC_CCXL_CompileTopStatement(ctx, c);
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
		c=BCCX_Child(l);

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

//		BGBCC_CCXL_BindStructSig(ctx, s, "namespace");
		BGBCC_CCXL_StubError(ctx);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;
		
		while(c)
		{
			BGBCC_CCXL_CompileTopStatement(ctx, c);
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

	if(BCCX_TagIsP(l, "asm_blob"))
	{
		c=BCCX_Child(l);
		BGBCC_CCXL_AddAsmBlob(ctx, BCCX_Text(c));
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	BGBCC_CCXL_Error(ctx, "unhandled top statement %s\n", BCCX_Tag(l));
}


int BGBCC_CCXL_CompileModuleCTX(
	BGBCC_TransState *ctx, char *name, BCCX_Node *l)
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

	BGBCC_CCXL_CompileStructs(ctx);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(-1);
	}

	return(0);
}

char *BGBCC_CCXL_CompileModule(char *name, BCCX_Node *l)
{
	BGBCC_TransState *ctx;
	BCCX_Node *c;
	char *s;


	ctx=bgbcc_malloc(sizeof(BGBCC_TransState));
	memset(ctx, 0, sizeof(BGBCC_TransState));

	ctx->n_warn=0;
	ctx->n_error=0;

	s=BCCX_Get(l, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_Get(l, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_CCXL_CompileStructs(ctx);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(NULL);
	}

//	return(ctx->ips);
	return(NULL);
}
