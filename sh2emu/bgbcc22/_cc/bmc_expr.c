#include <bgbccc.h>


void BGBCC_BMC_CompileAssign(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t;
	char *s;
	int i, j;

	if(BCCX_TagIsP(l, "ref"))
	{
		bgbcc_print(ctx, "%s=", BCCX_Get(l, "name"));
		return;
	}

	if(BGBCC_IsUnaryP(l, "*"))
	{
		bgbcc_print(ctx, "*(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, ")=");
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "->%s=", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex") || BCCX_TagIsP(l, "vector-ref"))
	{
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		bgbcc_print(ctx, "[");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		bgbcc_print(ctx, "]=");
		return;
	}
}

void BGBCC_BMC_CompileExprList(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_BMC_CompileExpr(ctx, cur);
		bgbcc_print(ctx, ", ");
		cur=BCCX_Next(cur);
	}
}

void BGBCC_BMC_CompileExprListReverse(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)
	{
		BGBCC_BMC_CompileExpr(ctx, stk[i]);
		bgbcc_print(ctx, ", ");
	}
}

void BGBCC_BMC_CompileArgsList(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_BMC_CompileExpr(ctx, cur);
		bgbcc_print(ctx, ", ");
		cur=BCCX_Next(cur);
	}
}

void BGBCC_BMC_CompileFuncall(BGBCC_State *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;

	s0=BCCX_Get(l, "name");
	if(s0)
	{
		bgbcc_print(ctx, "%s( ", s0);
		BGBCC_BMC_CompileArgsList(ctx, BCCX_Fetch(l, "args"));
		bgbcc_print(ctx, ") ");
		return;
	}

	bgbcc_print(ctx, "(");
	BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "func"));
	bgbcc_print(ctx, ")(");
	BGBCC_BMC_CompileArgsList(ctx, BCCX_Fetch(l, "args"));
	bgbcc_print(ctx, ")");
	return;
}

void BGBCC_BMC_CompileForm(BGBCC_State *ctx, BCCX_Node *l)
{
	byte *ips[64];
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn;
	int i, j;

	if(BCCX_TagIsP(l, "return"))
	{
		bgbcc_print(ctx, "return(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, ")");
		return;
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		s0=BGBCC_BMC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "(%s)(", s0);
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, ")");
		return;
	}

	if(BCCX_TagIsP(l, "sizeof"))
	{
		s0=BGBCC_BMC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "sizeof(%s)", s0);
		return;
	}

	if(BCCX_TagIsP(l, "sizeof_expr"))
	{
		bgbcc_print(ctx, "sizeof(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, ")");
		return;
	}
	if(BCCX_TagIsP(l, "offsetof"))
	{
		s0=BGBCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "offsetof($%s, $%s)", s0, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		s0=BCCX_Get(l, "op");

		if(s0)
		{
			BGBCC_BMC_CompileAssign(ctx, BCCX_Fetch(l, "left"));

			BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
			bgbcc_print(ctx, "%s", s0);
			BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
			return;
		}

		BGBCC_BMC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex"))
	{
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		bgbcc_print(ctx, "[");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		bgbcc_print(ctx, "]");
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "->%s", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		bgbcc_print(ctx, "++(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")");
		return;
	}

	if(BCCX_TagIsP(l, "predec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		bgbcc_print(ctx, "--(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")");
		return;
	}

	if(BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		bgbcc_print(ctx, "(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")++");
		return;
	}

	if(BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		bgbcc_print(ctx, "(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")--");
		return;
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_BMC_CompileExpr(ctx, t);
			return;
		}

		bgbcc_print(ctx, "if(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")");

		BGBCC_BMC_CompileExprT(ctx, BCCX_Fetch(l, "then"));

		t=BCCX_Fetch(l, "else");
		if(t)
		{
			bgbcc_print(ctx, " else ");
			BGBCC_BMC_CompileExprT(ctx, t);
		}

		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		bgbcc_print(ctx, "{");

		c=BCCX_Child(l);
		while(1)
		{
			if(BCCX_Next(c))
			{
				BGBCC_BMC_CompileStatement(ctx, c);
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_BMC_CompileExprT(ctx, c);
			bgbcc_print(ctx, ";");
			break;
		}

		bgbcc_print(ctx, "}");
		return;
	}

	if(BGBCC_IsBinaryP(l, "&&"))
	{
		bgbcc_print(ctx, "(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		bgbcc_print(ctx, ")&&(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, ")");
		return;
	}

	if(BGBCC_IsBinaryP(l, "||"))
	{
		bgbcc_print(ctx, "(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		bgbcc_print(ctx, ")||(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, ")");
		return;
	}

	if(BCCX_TagIsP(l, "boolify"))
	{
		bgbcc_print(ctx, "(__bool)(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, ")");
		return;
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		if(BCCX_AttrIsP(l, "op", "*"))
		{
			bgbcc_print(ctx, "*(");
			BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
			bgbcc_print(ctx, ")");
			return;
		}

		if(BCCX_AttrIsP(l, "op", "&"))
		{
			t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "value"));

			if(BCCX_TagIsP(t, "ref"))
			{
				s0=BCCX_Get(t, "name");
				bgbcc_print(ctx, "&%s", s0);
				return;
			}

			if(BCCX_TagIsP(t, "objref"))
			{
				bgbcc_print(ctx, "&(");
				BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(t, "value"));
				bgbcc_print(ctx, "->%s)", BCCX_Get(t, "name"));
				return;
			}

			if(BCCX_TagIsP(t, "getindex") ||
				BCCX_TagIsP(t, "vector-ref"))
			{
				bgbcc_print(ctx, "&(");
				BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(t, "array"));
				bgbcc_print(ctx, "[");
				BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(t, "index"));
				bgbcc_print(ctx, "])");
				return;
			}

			BGBCC_Error("Bad expression type for '&'\n");
			return;
		}

		bgbcc_print(ctx, "%s(", op);
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, ")");
		return;
	}

	if(BCCX_TagIsP(l, "binary"))
	{
		bgbcc_print(ctx, "(");
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		bgbcc_print(ctx, ")%s(", op);
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, ")");

		return;
	}

	if(BCCX_TagIsP(l, "funcall"))
	{
		BGBCC_BMC_CompileFuncall(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "methodcall"))
	{
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "->%s(", BCCX_Get(l, "name"));
		BGBCC_BMC_CompileArgsList(ctx, BCCX_Fetch(l, "args"));
		bgbcc_print(ctx, ")");
		return;
	}

	BGBCC_Error("unhandled expr %s\n", BCCX_Tag(l));
	bgbcc_print(ctx, "__null");
}

void BGBCC_BMC_CompileExprT(BGBCC_State *ctx, BCCX_Node *l)
{
	long long li;
	float f;
	BCCX_Node *t, *c, *ct, *cv;
	char *s0, *s1, *s2;
	int i, j;

	l=BGBCC_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "ref"))
	{
		bgbcc_print(ctx, "%s", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "int"))
	{
		bgbcc_print(ctx, "%s", BCCX_Get(l, "value"));
		return;
	}
	if(BCCX_TagIsP(l, "real"))
	{
		bgbcc_print(ctx, "%s", BCCX_Get(l, "value"));
		return;
	}

	if(BCCX_TagIsP(l, "string"))
	{
		bgbcc_print(ctx, "%s", BGBCC_CIfy(BCCX_Get(l, "value")));
		return;
	}

	if(BCCX_TagIsP(l, "complex"))
	{
		s0=BCCX_Get(l, "real"); s1=BCCX_Get(l, "imag");
		if(!s0)s0="0"; if(!s1)s1="0";
		bgbcc_print(ctx, "__complex(%s, %s)", s0, s1);
		return;
	}

	BGBCC_BMC_CompileForm(ctx, l);
	return;
}

void BGBCC_BMC_CompileExpr(BGBCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=0;
	BGBCC_BMC_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_BMC_CompileExprTail(BGBCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=1;
	BGBCC_BMC_CompileExprT(ctx, l);
	ctx->tail=i;
}
