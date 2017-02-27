#include <bgbccc.h>


char *BGBCC_FrCC_CIfy(char *s)
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

void BGBCC_FrCC_CompileAssign(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t;
	char *s;
	int i, j;

	if(BCCX_TagIsP(l, "ref"))
	{
//		bgbcc_print(ctx, "=%s ", BCCX_Get(l, "name"));
		BGBCC_FrBC_PopStore(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BGBCC_FrCC_IsUnaryP(ctx, l, "*"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_FrBC_StackStoreIndexConst(ctx, 0);
//		bgbcc_print(ctx, "0 storeindex ");
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_FrBC_StackStoreSlot(ctx, BCCX_Get(l, "name"));
//		bgbcc_print(ctx, "$%s storeslot ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex") || BCCX_TagIsP(l, "vector-ref"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		BGBCC_FrBC_StackStoreIndex(ctx);
//		bgbcc_print(ctx, "storeindex ");
		return;
	}

	BGBCC_FrCC_Error(ctx, "Bad lvalue in expression\n");
}

void BGBCC_FrCC_CompileExprList(BGBCC_FrCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_FrCC_CompileExpr(ctx, cur);
		cur=BCCX_Next(cur);
	}
}

void BGBCC_FrCC_CompileExprListReverse(BGBCC_FrCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)BGBCC_FrCC_CompileExpr(ctx, stk[i]);
}

void BGBCC_FrCC_CompileArgsList(BGBCC_FrCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)
	{
		BGBCC_FrCC_CompileExpr(ctx, stk[i]);
//		bgbcc_print(ctx, ", ");
	}
}

void BGBCC_FrCC_CompileFuncall(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

#if 0
	if(BCCX_AttrIsP(l, "name", "__fvec") ||
		BCCX_AttrIsP(l, "name", "__fvec2") ||
		BCCX_AttrIsP(l, "name", "__fvec3") ||
		BCCX_AttrIsP(l, "name", "__fvec4"))
	{
		c=BCCX_Fetch(l, "args"); i=0;
		while(c)
		{
			BGBCC_FrCC_CompileExpr(ctx, c);
			bgbcc_print(ctx, "$f cast ");
			c=BCCX_Next(c); i++;
		}

		if((i<2) || (i>4))
			BGBCC_FrCC_Error("__fvec only valid with "
				"2, 3, or 4 args\n");
		bgbcc_print(ctx, "fvec%d ", i);
		return;
	}

	if(BCCX_AttrIsP(l, "name", "__qvec"))
	{
		c=BCCX_Fetch(l, "args"); i=0;
		while(c)
		{
			BGBCC_FrCC_CompileExpr(ctx, c);
			bgbcc_print(ctx, "$f cast ");
			c=BCCX_Next(c); i++;
		}

		if(i!=4)BGBCC_FrCC_Error("__qvec only valid with "
			"4 args\n");
		bgbcc_print(ctx, "quat ");
		return;
	}
#endif

	s0=BCCX_Get(l, "name");
	if(s0)
	{
#if 0
		i=0;
		s1="__builtin_";
		if(!strncmp(s0, s1, strlen(s1)))i=1;
		if(!strcmp(s0, "sqrt"))i=1;
		if(!strcmp(s0, "sin"))i=1;
		if(!strcmp(s0, "cos"))i=1;
		if(!strcmp(s0, "tan"))i=1;

		if(!strcmp(s0, "creal"))i=1;
		if(!strcmp(s0, "cimag"))i=1;

		if(i)
		{
			bgbcc_print(ctx, "mark ");
			c=BCCX_Fetch(l, "args");
			BGBCC_FrCC_CompileExprListReverse(ctx,
				BCCX_Fetch(l, "args"));
			bgbcc_print(ctx, "$%s builtin ", s0);
			return;
		}
#endif

		if(!strcmp(s0, "__BGBCC_DBG"))
		{
			printf("BGBCC Debug\n");
		}

//		bgbcc_print(ctx, "%s( ", s0);
//		BGBCC_FrCC_CompileArgsList(ctx, BCCX_Fetch(l, "args"));
//		bgbcc_print(ctx, ") ");

		BGBCC_FrBC_PushMark(ctx);
		BGBCC_FrCC_CompileExprListReverse(ctx,
			BCCX_Fetch(l, "args"));
		BGBCC_FrBC_StackCallName(ctx, s0);
//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "func"));
//		bgbcc_print(ctx, "call ");

		return;
	}


	c=BCCX_Fetch(l, "func");
	if(!c)c=BCCX_Fetch(l, "value");

	if(!c)
	{
		BGBCC_FrCC_Error(ctx, "funcall missing function\n");
		BGBCC_FrBC_StackPushConstInt(ctx, 0);
		return;
	}

//	bgbcc_print(ctx, "mark ");
	BGBCC_FrBC_PushMark(ctx);
	BGBCC_FrCC_CompileExprListReverse(ctx,
		BCCX_Fetch(l, "args"));
	BGBCC_FrCC_CompileExpr(ctx, c);
	BGBCC_FrBC_StackPopCall(ctx);
//	bgbcc_print(ctx, "call ");
	return;
}


void BGBCC_FrCC_CompileMethodcall(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

	BGBCC_FrBC_PushMark(ctx);
	BGBCC_FrCC_CompileExprListReverse(ctx,
		BCCX_Fetch(l, "args"));

	BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
	BGBCC_FrBC_StackLoadSlot(ctx, BCCX_Get(l, "name"));

	BGBCC_FrBC_StackPopCall(ctx);
	return;
}


void BGBCC_FrCC_CompileFormJmpTF(BGBCC_FrCC_State *ctx, BCCX_Node *l,
	char *brt, char *brf)
{
	char *s0, *s1, *s2;
	BCCX_Node *ln, *rn;
	int i, j, k;

	if(BGBCC_FrCC_IsBinaryP(ctx, l, "&&"))
	{
		s0=BGBCC_FrCC_GenSym(ctx);

		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_FrCC_ReduceExpr(ctx, ln);
		rn=BGBCC_FrCC_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_FrCC_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, ln, s0, brf); j=1; }
		else
			{ BGBCC_FrCC_CompileJCF(ctx, ln, brf); }

		if(j) { BGBCC_FrCC_EmitLabel(ctx, s0); }

		if(BGBCC_FrCC_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, rn, brt, brf); k=1; }
		else
			{ BGBCC_FrCC_CompileJCF(ctx, rn, brf); }

//		BGBCC_FrCC_EmitLabel(ctx, s0);
		if(!k) { BGBCC_FrCC_CompileJmp(ctx, brt); }

//		BGBCC_FrCC_CompileJCF(ctx, ln, brf);
//		BGBCC_FrCC_CompileJCF(ctx, rn, brf);
//		BGBCC_FrCC_CompileJmp(ctx, brt);
		return;
	}

	if(BGBCC_FrCC_IsBinaryP(ctx, l, "||"))
	{
		s0=BGBCC_FrCC_GenSym(ctx);

		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_FrCC_ReduceExpr(ctx, ln);
		rn=BGBCC_FrCC_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_FrCC_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, ln, brt, s0); j=1; }
		else
			{ BGBCC_FrCC_CompileJCF(ctx, ln, brf); }

		if(j) { BGBCC_FrCC_EmitLabel(ctx, s0); }

		if(BGBCC_FrCC_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, rn, brt, brf); k=1; }
		else
			{ BGBCC_FrCC_CompileJCF(ctx, rn, brf); }

//		BGBCC_FrCC_EmitLabel(ctx, s0);
		if(!k) { BGBCC_FrCC_CompileJmp(ctx, brf); }

//		BGBCC_FrCC_CompileJCT(ctx, ln, brt);
//		BGBCC_FrCC_CompileJCT(ctx, rn, brt);
//		BGBCC_FrCC_CompileJmp(ctx, brf);
		return;
	}
	
	BGBCC_FrCC_CompileForm(ctx, l);
}

void BGBCC_FrCC_CompileForm(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	byte *ips[64];
	char *s0, *s1, *s2, *s3;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn;
	int i, j, k;

	if(BCCX_TagIsP(l, "return"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_FrBC_StackRet(ctx);
//		bgbcc_print(ctx, "ret ");
		return;
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		s0=BGBCC_FrCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		BGBCC_FrBC_StackCastSig(ctx, s0);
//		bgbcc_print(ctx, "$%s cast ", s0);
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
			printf("BGBCC_FrCC_CompileForm: sizeof: bad AST\n");
			BGBCC_FrBC_StackSizeofSig(ctx, "Pv");
//			bgbcc_print(ctx, "0 ");
			return;
		}

		s0=BGBCC_FrCC_VarTypeString(ctx, t);
		BGBCC_FrBC_StackSizeofSig(ctx, s0);
//		bgbcc_print(ctx, "$%s sizeof_t ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "sizeof_expr"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_FrBC_StackSizeofVal(ctx);
//		bgbcc_print(ctx, "sizeof ");
		return;
	}
	if(BCCX_TagIsP(l, "offsetof"))
	{
		s0=BGBCC_FrCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		BGBCC_FrBC_StackOffsetof(ctx, s0, BCCX_Get(l, "name"));
//		bgbcc_print(ctx, "$%s $%s offsetof ", s0, BCCX_Get(l, "name"));
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
			BGBCC_FrCC_CompileExpr(ctx, ln);
			BGBCC_FrCC_CompileExpr(ctx, rn);
			BGBCC_FrBC_StackBinaryOp(ctx, s0);
			BGBCC_FrBC_StackDup(ctx);
			BGBCC_FrCC_CompileAssign(ctx, ln);

//			BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
//			BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
//			BGBCC_FrBC_StackBinaryOp(ctx, s0);
//			BGBCC_FrBC_StackDup(ctx);
//			BGBCC_FrCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
			return;
		}

		BGBCC_FrCC_CompileExpr(ctx, rn);
		BGBCC_FrBC_StackDup(ctx);
		BGBCC_FrCC_CompileAssign(ctx, ln);

//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
//		BGBCC_FrBC_StackDup(ctx);
//		BGBCC_FrCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		BGBCC_FrBC_StackLoadIndex(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_FrBC_StackLoadSlot(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrBC_StackUnaryOp(ctx, "++");
		BGBCC_FrBC_StackDup(ctx);
		BGBCC_FrCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrBC_StackUnaryOp(ctx, "--");
		BGBCC_FrBC_StackDup(ctx);
		BGBCC_FrCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrBC_StackDup(ctx);
		BGBCC_FrBC_StackUnaryOp(ctx, "++");
		BGBCC_FrCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_FrCC_CompileExpr(ctx, t);
		BGBCC_FrBC_StackDup(ctx);
		BGBCC_FrBC_StackUnaryOp(ctx, "--");
		BGBCC_FrCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_FrCC_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_FrCC_CompileExpr(ctx, t);
			return;
		}

//		bgbcc_print(ctx, "beginu ");
		BGBCC_FrBC_StackBeginU(ctx, "i");
		s0=BGBCC_FrCC_GenSym(ctx);
		s1=BGBCC_FrCC_GenSym(ctx);
		BGBCC_FrCC_CompileJCF(ctx, t, s0);

		BGBCC_FrCC_CompileExprT(ctx, BCCX_Fetch(l, "then"));
//		bgbcc_print(ctx, "setu ");
		BGBCC_FrBC_StackSetU(ctx);

		BGBCC_FrCC_CompileJmp(ctx, s1);
		BGBCC_FrCC_EmitLabel(ctx, s0);

		t=BCCX_Fetch(l, "else");
		BGBCC_FrCC_CompileExprT(ctx, t);
//		bgbcc_print(ctx, "setu ");
		BGBCC_FrBC_StackSetU(ctx);

		BGBCC_FrCC_EmitLabel(ctx, s1);

//		bgbcc_print(ctx, "endu ");
		BGBCC_FrBC_StackEndU(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(1)
		{
			if(BCCX_Next(c))
			{
				BGBCC_FrCC_CompileStatement(ctx, c);
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_FrCC_CompileExprT(ctx, c);
			break;
		}

		return;
	}

	if(BGBCC_FrCC_IsBinaryP(ctx, l, "&&"))
	{
		s0=BGBCC_FrCC_GenSym(ctx);
		s1=BGBCC_FrCC_GenSym(ctx);
		s2=BGBCC_FrCC_GenSym(ctx);
		s3=BGBCC_FrCC_GenSym(ctx);

		BGBCC_FrBC_StackBeginU(ctx, "i");

#if 1
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_FrCC_ReduceExpr(ctx, ln);
		rn=BGBCC_FrCC_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_FrCC_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, ln, s2, s0); j=1; }
		else
			{ BGBCC_FrCC_CompileJCF(ctx, ln, s0); }

		if(j) { BGBCC_FrCC_EmitLabel(ctx, s2); }

		if(BGBCC_FrCC_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, rn, s3, s0); k=1; }
		else
			{ BGBCC_FrCC_CompileJCF(ctx, rn, s0); }
#endif

#if 0
		BGBCC_FrCC_CompileJCF(ctx, BCCX_Fetch(l, "left"), s0);
		BGBCC_FrCC_CompileJCF(ctx, BCCX_Fetch(l, "right"), s0);

//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
//		BGBCC_FrCC_CompileJmpFalse(ctx, s0);

//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
//		BGBCC_FrCC_CompileJmpFalse(ctx, s0);
#endif

		if(k) { BGBCC_FrCC_EmitLabel(ctx, s3); }
		BGBCC_FrBC_StackPushConstInt(ctx, 1);
		BGBCC_FrBC_StackSetU(ctx);
		BGBCC_FrCC_CompileJmp(ctx, s1);

		BGBCC_FrCC_EmitLabel(ctx, s0);
		BGBCC_FrBC_StackPushConstInt(ctx, 0);
		BGBCC_FrBC_StackSetU(ctx);

		BGBCC_FrCC_EmitLabel(ctx, s1);
		BGBCC_FrBC_StackEndU(ctx);
		return;
	}

	if(BGBCC_FrCC_IsBinaryP(ctx, l, "||"))
	{
		s0=BGBCC_FrCC_GenSym(ctx);
		s1=BGBCC_FrCC_GenSym(ctx);
		s2=BGBCC_FrCC_GenSym(ctx);
		s3=BGBCC_FrCC_GenSym(ctx);

		BGBCC_FrBC_StackBeginU(ctx, "i");

#if 1
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_FrCC_ReduceExpr(ctx, ln);
		rn=BGBCC_FrCC_ReduceExpr(ctx, rn);
		j=0; k=0;

		if(BGBCC_FrCC_IsBinaryP(ctx, ln, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, ln, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, ln, s0, s2); j=1; }
		else
			{ BGBCC_FrCC_CompileJCT(ctx, ln, s0); }

		if(j) { BGBCC_FrCC_EmitLabel(ctx, s2); }

		if(BGBCC_FrCC_IsBinaryP(ctx, rn, "&&") ||
				BGBCC_FrCC_IsBinaryP(ctx, rn, "||"))
			{ BGBCC_FrCC_CompileFormJmpTF(ctx, rn, s0, s3); k=1; }
		else
			{ BGBCC_FrCC_CompileJCT(ctx, rn, s0); }
#endif

#if 0
		BGBCC_FrCC_CompileJCT(ctx, BCCX_Fetch(l, "left"), s0);
		BGBCC_FrCC_CompileJCT(ctx, BCCX_Fetch(l, "right"), s0);

//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
//		BGBCC_FrCC_CompileJmpTrue(ctx, s0);

//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
//		BGBCC_FrCC_CompileJmpTrue(ctx, s0);
#endif

		if(k) { BGBCC_FrCC_EmitLabel(ctx, s3); }
		BGBCC_FrBC_StackPushConstInt(ctx, 0);
		BGBCC_FrBC_StackSetU(ctx);
		BGBCC_FrCC_CompileJmp(ctx, s1);
		BGBCC_FrCC_EmitLabel(ctx, s0);
		BGBCC_FrBC_StackPushConstInt(ctx, 1);
		BGBCC_FrBC_StackSetU(ctx);

		BGBCC_FrCC_EmitLabel(ctx, s1);
		BGBCC_FrBC_StackEndU(ctx);
		return;
	}

	if(BCCX_TagIsP(l, "boolify"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_FrBC_StackCastBool(ctx);
//		bgbcc_print(ctx, "boolify ");
		return;
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		if(BCCX_AttrIsP(l, "op", "*"))
		{
			BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
			BGBCC_FrBC_StackPushConstInt(ctx, 0);
			BGBCC_FrBC_StackLoadIndex(ctx);
			return;
		}

		if(BCCX_AttrIsP(l, "op", "&"))
		{
			t=BGBCC_FrCC_ReduceExpr(ctx, BCCX_Fetch(l, "value"));

			if(BCCX_TagIsP(t, "ref"))
			{
				s0=BCCX_Get(t, "name");
				BGBCC_FrBC_StackLoadAddr(ctx, s0);
				return;
			}

			if(BCCX_TagIsP(t, "objref"))
			{
				BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(t, "value"));
				BGBCC_FrBC_StackLoadSlotAddr(ctx, BCCX_Get(t, "name"));
				return;
			}

			if(BCCX_TagIsP(t, "getindex") ||
				BCCX_TagIsP(t, "vector-ref"))
			{
				BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(t, "array"));
				BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(t, "index"));
				BGBCC_FrBC_StackLoadIndexAddr(ctx);
				return;
			}

			BGBCC_FrCC_Error(ctx, "Bad expression type for '&'\n");
			return;
		}

		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));

		BGBCC_FrBC_StackUnaryOp(ctx, BCCX_Get(l, "op"));

//		s0=NULL;
//		if(BCCX_AttrIsP(l, "op", "+"))s0="";
//		if(BCCX_AttrIsP(l, "op", "-"))s0="neg";
//		if(BCCX_AttrIsP(l, "op", "~"))s0="not";
//		if(BCCX_AttrIsP(l, "op", "!"))s0="lnot";
//		if(BCCX_AttrIsP(l, "op", "*"))s0="defer";
//		if(BCCX_AttrIsP(l, "op", "&"))s0="loada";

//		bgbcc_print(ctx, "%s ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "binary"))
	{
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));

		BGBCC_FrBC_StackBinaryOp(ctx, BCCX_Get(l, "op"));
		return;
	}

	if(BCCX_TagIsP(l, "comma"))
	{
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_FrCC_ReduceExpr(ctx, ln);
		rn=BGBCC_FrCC_ReduceExpr(ctx, rn);

		BGBCC_FrCC_CompileExpr(ctx, ln);
		BGBCC_FrBC_StackPop(ctx);
		BGBCC_FrCC_CompileExpr(ctx, rn);
		return;
	}

	if(BCCX_TagIsP(l, "funcall"))
	{
		BGBCC_FrCC_CompileFuncall(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "methodcall"))
	{
		BGBCC_FrCC_CompileMethodcall(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "new"))
	{
		BGBCC_FrCC_StubError(ctx);
//		bgbcc_print(ctx, "mark ");
//		c=BCCX_Fetch(l, "args");
//		BGBCC_FrCC_CompileExprListReverse(ctx, c);
		
//		s0=BGBCC_FrCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
//		bgbcc_print(ctx, "$%s new ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "throw"))
	{
		BGBCC_FrCC_StubError(ctx);
//		BGBCC_FrCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
//		bgbcc_print(ctx, "throw ");
		return;
	}

	if(BCCX_TagIsP(l, "list"))
	{
		BGBCC_FrBC_StackPushConstInt(ctx, 0);
		BGBCC_FrCC_StubWarn(ctx);
		return;
	}

	BGBCC_FrCC_Error(ctx, "Unhandled expr %s\n", BCCX_Tag(l));
	BGBCC_FrBC_StackPushConstInt(ctx, 0);
//	bgbcc_print(ctx, "push_null ");
}

void BGBCC_FrCC_CompileExprT(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	long long li;
	float f;
	BCCX_Node *t, *c, *ct, *cv;
	char *s0, *s1, *s2;
	int i, j;

	l=BGBCC_FrCC_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "ref"))
	{
		s0=BCCX_Get(l, "name");

#if 0
		if((ctx->lang==BGBCC_FrCC_LANG_JAVA) ||
			(ctx->lang==BGBCC_FrCC_LANG_CS))
		{
			if(!strcmp(s0, "true"))
				{ bgbcc_print(ctx, "push_true "); return; }
			if(!strcmp(s0, "false"))
				{ bgbcc_print(ctx, "push_false "); return; }
			if(!strcmp(s0, "null"))
				{ bgbcc_print(ctx, "push_null "); return; }
		}
#endif
	
		BGBCC_FrBC_PushLoad(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "int"))
	{
		BGBCC_FrBC_StackPushConstInt(ctx, BCCX_GetInt(l, "value"));
		return;
	}
	if(BCCX_TagIsP(l, "real"))
	{
		BGBCC_FrBC_StackPushConstDouble(ctx, BCCX_GetFloat(l, "value"));
		return;
	}

	if(BCCX_TagIsP(l, "string"))
	{
		BGBCC_FrBC_StackPushConstString(ctx, BCCX_Get(l, "value"));
		return;
	}

	if(BCCX_TagIsP(l, "charstring"))
	{
		s0=BCCX_Get(l, "value");
		if(s0) { i=BGBCP_ParseChar(&s0); }
			else i=0;
		BGBCC_FrBC_StackPushConstInt(ctx, i);
		return;
	}

	if(BCCX_TagIsP(l, "complex"))
	{
		BGBCC_FrCC_StubError(ctx);
//		s0=BCCX_Get(l, "real"); s1=BCCX_Get(l, "imag");
//		if(!s0)s0="0"; if(!s1)s1="0";
//		bgbcc_print(ctx, "%s %s complex ", s0, s1);
		return;
	}

	BGBCC_FrCC_CompileForm(ctx, l);
	return;
}

void BGBCC_FrCC_CompileExpr(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=0;
	BGBCC_FrCC_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_FrCC_CompileExprTail(BGBCC_FrCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=1;
	BGBCC_FrCC_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_FrCC_CompileExprAsType(BGBCC_FrCC_State *ctx,
	BCCX_Node *ty, BCCX_Node *l)
{
	char *s;
	int i;

	BGBCC_FrCC_CompileExpr(ctx, l);
	s=BGBCC_FrCC_VarTypeString(ctx, ty);
	BGBCC_FrBC_StackCastSig(ctx, s);
}
