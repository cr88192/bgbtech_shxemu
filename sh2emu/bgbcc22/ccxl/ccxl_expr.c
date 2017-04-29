#include <bgbccc.h>


char *BGBCC_CCXL_CIfy(char *s)
{
	static char buf[1024];
	char *t;
	int i;

	t=buf;
	*t++='\"';

	while(*s)
	{
		i=BGBCP_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(buf);
}

void BGBCC_CCXL_CompileAssign(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t;
	char *s;
	int i, j;

	if(BCCX_TagIsP(l, "ref"))
	{
		BGBCC_CCXL_PopStore(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "*"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackStoreIndexConst(ctx, 0);
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackStoreSlot(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex") || BCCX_TagIsP(l, "vector-ref"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		BGBCC_CCXL_StackStoreIndex(ctx);
		return;
	}

	BGBCC_CCXL_Error(ctx, "Bad lvalue in expression\n");
}

void BGBCC_CCXL_CompileExprList(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_CCXL_CompileExpr(ctx, cur);
		cur=BCCX_Next(cur);
	}
}

void BGBCC_CCXL_CompileExprListReverse(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)BGBCC_CCXL_CompileExpr(ctx, stk[i]);
}

void BGBCC_CCXL_CompileArgsList(BGBCC_TransState *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)
	{
		BGBCC_CCXL_CompileExpr(ctx, stk[i]);
	}
}

void BGBCC_CCXL_CompileFuncall(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

	s0=BCCX_Get(l, "name");
	if(s0)
	{
		if(!strcmp(s0, "__BGBCC_DBG"))
		{
			printf("BGBCC Debug\n");
		}

		if(!strcmp(s0, "__builtin_va_end"))
		{
			BGBCC_CCXL_CompileExprListReverse(ctx,
				BCCX_Fetch(l, "args"));
			BGBCC_CCXL_StackVaEnd(ctx);
			BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		if(!strcmp(s0, "__builtin_va_start"))
		{
			BGBCC_CCXL_CompileExprListReverse(ctx,
				BCCX_Fetch(l, "args"));
			BGBCC_CCXL_StackVaStart(ctx);
			BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		if(!strcmp(s0, "__builtin_va_arg"))
		{
			BGBCC_CCXL_CompileExprListReverse(ctx,
				BCCX_Fetch(l, "args"));
			BGBCC_CCXL_StackVaArg(ctx);

//			BGBCC_CCXL_StackPushConstInt(ctx, 0);
			return;
		}

		BGBCC_CCXL_PushMark(ctx);
		BGBCC_CCXL_CompileExprListReverse(ctx,
			BCCX_Fetch(l, "args"));
		BGBCC_CCXL_StackCallName(ctx, s0);
		return;
	}


	c=BCCX_Fetch(l, "func");
	if(!c)c=BCCX_Fetch(l, "value");

	if(!c)
	{
		BGBCC_CCXL_Error(ctx, "funcall missing function\n");
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		return;
	}

	BGBCC_CCXL_PushMark(ctx);
	BGBCC_CCXL_CompileExprListReverse(ctx,
		BCCX_Fetch(l, "args"));
	BGBCC_CCXL_CompileExpr(ctx, c);
	BGBCC_CCXL_StackPopCall(ctx);
	return;
}


void BGBCC_CCXL_CompileMethodcall(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

	BGBCC_CCXL_PushMark(ctx);
	BGBCC_CCXL_CompileExprListReverse(ctx,
		BCCX_Fetch(l, "args"));

	BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
	BGBCC_CCXL_StackLoadSlot(ctx, BCCX_Get(l, "name"));

	BGBCC_CCXL_StackPopCall(ctx);
	return;
}


void BGBCC_CCXL_CompileFormJmpTF(BGBCC_TransState *ctx, BCCX_Node *l,
	ccxl_label brt, ccxl_label brf)
{
//	char *s0, *s1, *s2;
	ccxl_label lbl0, lbl1, lbl2;
	BCCX_Node *ln, *rn;
	int i, j, k;

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&"))
	{
		lbl0=BGBCC_CCXL_GenSym(ctx);

		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, lbl0, brf); j=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, ln, brf); }

		if(j) { BGBCC_CCXL_EmitLabel(ctx, lbl0); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, brt, brf); k=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, rn, brf); }
		if(!k) { BGBCC_CCXL_CompileJmp(ctx, brt); }
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		lbl0=BGBCC_CCXL_GenSym(ctx);

		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, brt, lbl0); j=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, ln, brf); }

		if(j) { BGBCC_CCXL_EmitLabel(ctx, lbl0); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, brt, brf); k=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, rn, brf); }

		if(!k) { BGBCC_CCXL_CompileJmp(ctx, brf); }
		return;
	}
	
	BGBCC_CCXL_CompileForm(ctx, l);
}

void BGBCC_CCXL_CompileForm(BGBCC_TransState *ctx, BCCX_Node *l)
{
	byte *ips[64];
	ccxl_label l0, l1, l2, l3;
	char *s0, *s1, *s2, *s3;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn;
	int i, j, k;

	if(BCCX_TagIsP(l, "return"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackRet(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		s0=BGBCC_CCXL_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		BGBCC_CCXL_StackCastSig(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "var"))
	{
//		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		s0=BGBCC_CCXL_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		BGBCC_CCXL_StackLitTypeSig(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "sizeof"))
	{
		t=BCCX_FindTag(l, "type");
		if(!t)
		{
			u=BCCX_FindTag(l, "tyexpr"); v=NULL;
			if(u)
			{
				v=BCCX_FindTag(u, "var");
				if(!v)v=BCCX_FindTag(u, "proto");
			}
			if(v)t=BCCX_FindTag(v, "type");
		}

		if(!t)
		{
			printf("BGBCC_CCXL_CompileForm: sizeof: bad AST\n");
			BGBCC_CCXL_StackSizeofSig(ctx, "Pv");
			return;
		}

		s0=BGBCC_CCXL_VarTypeString(ctx, t);
		BGBCC_CCXL_StackSizeofSig(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "sizeof_expr"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackSizeofVal(ctx);
		return;
	}
	if(BCCX_TagIsP(l, "offsetof"))
	{
		s0=BGBCC_CCXL_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		BGBCC_CCXL_StackOffsetof(ctx, s0, BCCX_Get(l, "name"));
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
			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_StackBinaryOp(ctx, s0);
			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_CompileAssign(ctx, ln);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, rn);
		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_CompileAssign(ctx, ln);
		return;
	}

	if(BCCX_TagIsP(l, "getindex"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		BGBCC_CCXL_StackLoadIndex(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackLoadSlot(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_CCXL_CompileExpr(ctx, t);
			return;
		}

		BGBCC_CCXL_StackBeginU(ctx, "i");
		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileJCF(ctx, t, l0);

		BGBCC_CCXL_CompileExprT(ctx, BCCX_Fetch(l, "then"));
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_CompileJmp(ctx, l1);
		BGBCC_CCXL_EmitLabel(ctx, l0);

		t=BCCX_Fetch(l, "else");
		BGBCC_CCXL_CompileExprT(ctx, t);
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_StackEndU(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(1)
		{
			if(BCCX_Next(c))
			{
				BGBCC_CCXL_CompileStatement(ctx, c);
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CCXL_CompileExprT(ctx, c);
			break;
		}

		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&"))
	{
		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		l3=BGBCC_CCXL_GenSym(ctx);

		BGBCC_CCXL_StackBeginU(ctx, "i");

#if 1
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, l2, l0); j=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, ln, l0); }

		if(j) { BGBCC_CCXL_EmitLabel(ctx, l2); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, l3, l0); k=1; }
		else
			{ BGBCC_CCXL_CompileJCF(ctx, rn, l0); }
#endif

		if(k) { BGBCC_CCXL_EmitLabel(ctx, l3); }
		BGBCC_CCXL_StackPushConstInt(ctx, 1);
		BGBCC_CCXL_StackSetU(ctx);
		BGBCC_CCXL_CompileJmp(ctx, l1);

		BGBCC_CCXL_EmitLabel(ctx, l0);
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_StackEndU(ctx);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		l3=BGBCC_CCXL_GenSym(ctx);

		BGBCC_CCXL_StackBeginU(ctx, "i");

#if 1
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_CCXL_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, ln, l0, l2); j=1; }
		else
			{ BGBCC_CCXL_CompileJCT(ctx, ln, l0); }

		if(j) { BGBCC_CCXL_EmitLabel(ctx, l2); }

		if(BGBCC_CCXL_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_CCXL_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_CCXL_CompileFormJmpTF(ctx, rn, l0, l3); k=1; }
		else
			{ BGBCC_CCXL_CompileJCT(ctx, rn, l0); }
#endif

		if(k) { BGBCC_CCXL_EmitLabel(ctx, l3); }
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		BGBCC_CCXL_StackSetU(ctx);
		BGBCC_CCXL_CompileJmp(ctx, l1);
		BGBCC_CCXL_EmitLabel(ctx, l0);
		BGBCC_CCXL_StackPushConstInt(ctx, 1);
		BGBCC_CCXL_StackSetU(ctx);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_StackEndU(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "boolify"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackCastBool(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		if(BCCX_AttrIsP(l, "op", "*"))
		{
			BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
//			BGBCC_CCXL_StackPushConstInt(ctx, 0);
//			BGBCC_CCXL_StackLoadIndex(ctx);
			BGBCC_CCXL_StackLoadIndexConst(ctx, 0);
			return;
		}

		if(BCCX_AttrIsP(l, "op", "&"))
		{
			t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "value"));

			if(BCCX_TagIsP(t, "ref"))
			{
				s0=BCCX_Get(t, "name");
				BGBCC_CCXL_StackLoadAddr(ctx, s0);
				return;
			}

			if(BCCX_TagIsP(t, "objref"))
			{
				BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(t, "value"));
				BGBCC_CCXL_StackLoadSlotAddr(ctx, BCCX_Get(t, "name"));
				return;
			}

			if(BCCX_TagIsP(t, "getindex") ||
				BCCX_TagIsP(t, "vector-ref"))
			{
				BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(t, "array"));
				BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(t, "index"));
				BGBCC_CCXL_StackLoadIndexAddr(ctx);
				return;
			}

			BGBCC_CCXL_Error(ctx, "Bad expression type for '&'\n");
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));

		BGBCC_CCXL_StackUnaryOp(ctx, BCCX_Get(l, "op"));
		return;
	}

	if(BCCX_TagIsP(l, "binary"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "right"));

		BGBCC_CCXL_StackBinaryOp(ctx, BCCX_Get(l, "op"));
		return;
	}

	if(BCCX_TagIsP(l, "comma"))
	{
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

		BGBCC_CCXL_CompileExpr(ctx, ln);
		BGBCC_CCXL_StackPop(ctx);
		BGBCC_CCXL_CompileExpr(ctx, rn);
		return;
	}

	if(BCCX_TagIsP(l, "funcall"))
	{
		BGBCC_CCXL_CompileFuncall(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "methodcall"))
	{
		BGBCC_CCXL_CompileMethodcall(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "new"))
	{
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "throw"))
	{
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "list"))
	{
		BGBCC_CCXL_StackPushConstInt(ctx, 0);
		BGBCC_CCXL_StubWarn(ctx);
		return;
	}

	BGBCC_CCXL_Error(ctx, "Unhandled expr %s\n", BCCX_Tag(l));
	BGBCC_CCXL_StackPushConstInt(ctx, 0);
}

void BGBCC_CCXL_CompileExprT(BGBCC_TransState *ctx, BCCX_Node *l)
{
	long long li;
	float f;
	BCCX_Node *t, *c, *ct, *cv;
	char *s0, *s1, *s2;
	int i, j;

	l=BGBCC_CCXL_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "ref"))
	{
		s0=BCCX_Get(l, "name");	
		BGBCC_CCXL_PushLoad(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "int"))
	{
		BGBCC_CCXL_StackPushConstInt(ctx, BCCX_GetInt(l, "value"));
		return;
	}
	if(BCCX_TagIsP(l, "real"))
	{
		BGBCC_CCXL_StackPushConstDouble(ctx, BCCX_GetFloat(l, "value"));
		return;
	}

	if(BCCX_TagIsP(l, "string"))
	{
		BGBCC_CCXL_StackPushConstString(ctx, BCCX_Get(l, "value"));
		return;
	}

	if(BCCX_TagIsP(l, "charstring"))
	{
		s0=BCCX_Get(l, "value");
		if(s0) { i=BGBCP_ParseChar(&s0); }
			else i=0;
		BGBCC_CCXL_StackPushConstInt(ctx, i);
		return;
	}

	if(BCCX_TagIsP(l, "complex"))
	{
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	BGBCC_CCXL_CompileForm(ctx, l);
	return;
}

void BGBCC_CCXL_CompileExpr(BGBCC_TransState *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=0;
	BGBCC_CCXL_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_CCXL_CompileExprTail(BGBCC_TransState *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=1;
	BGBCC_CCXL_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_CCXL_CompileExprAsType(BGBCC_TransState *ctx,
	BCCX_Node *ty, BCCX_Node *l)
{
	char *s;
	int i;

	BGBCC_CCXL_CompileExpr(ctx, l);
	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	BGBCC_CCXL_StackCastSig(ctx, s);
}
