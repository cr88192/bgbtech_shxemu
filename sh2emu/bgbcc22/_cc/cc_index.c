#include <bgbccc.h>


void BGBCC_EmitLabel(BGBCC_State *ctx, char *name)
{
	int i;

	bgbcc_print(ctx, "%s: ", name);
}

void BGBCC_CompileBreak(BGBCC_State *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	bgbcc_print(ctx, "*%s ", s);
}

void BGBCC_CompileContinue(BGBCC_State *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	bgbcc_print(ctx, "*%s ", s);
}

void BGBCC_CompileBreakFalse(BGBCC_State *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	bgbcc_print(ctx, "!%s ", s);
}

void BGBCC_CompileBreakTrue(BGBCC_State *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	bgbcc_print(ctx, "?%s ", s);
}

void BGBCC_CompileContinueFalse(BGBCC_State *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	bgbcc_print(ctx, "!%s ", s);
}

void BGBCC_CompileContinueTrue(BGBCC_State *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	bgbcc_print(ctx, "?%s ", s);
}


void BGBCC_CompileJmp(BGBCC_State *ctx, char *s)
{
	bgbcc_print(ctx, "*%s ", s);
}

void BGBCC_CompileJmpFalse(BGBCC_State *ctx, char *s)
{
	bgbcc_print(ctx, "!%s ", s);
}

void BGBCC_CompileJmpTrue(BGBCC_State *ctx, char *s)
{
	bgbcc_print(ctx, "?%s ", s);
}

void BGBCC_CompileJmpCond(BGBCC_State *ctx, char *op, char *s)
{
	if(!op)return;
	bgbcc_print(ctx, "%s%s ", op, s);
}

void BGBCC_CompileJCO(BGBCC_State *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn, char *s)
{
	BCCX_Node *lt, *rt;
	BCCX_Node *t0, *t1;
	int i, j, k;

	ln=BGBCC_ReduceExpr(ctx, ln);
	rn=BGBCC_ReduceExpr(ctx, rn);

	BGBCC_CompileExpr(ctx, ln);
	BGBCC_CompileExpr(ctx, rn);
	BGBCC_CompileJmpCond(ctx, op, s);
}

void BGBCC_CompileJCT(BGBCC_State *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *op;

	l=BGBCC_ReduceExpr(ctx, l);
//	t=BGBCC_InferExpr(ctx, l);

	if(BGBCC_IsUnaryP(l, "!"))
	{
		BGBCC_CompileJCF(ctx, BCCX_Fetch(l, "value"), s);
		return;
	}

	op=NULL;
	if(BGBCC_IsBinaryP(l, "=="))op="==";
	if(BGBCC_IsBinaryP(l, "!="))op="!=";
	if(BGBCC_IsBinaryP(l, "==="))op="==";
	if(BGBCC_IsBinaryP(l, "!=="))op="!=";
	if(BGBCC_IsBinaryP(l, "<"))op="<";
	if(BGBCC_IsBinaryP(l, ">"))op=">";
	if(BGBCC_IsBinaryP(l, "<="))op="<=";
	if(BGBCC_IsBinaryP(l, ">="))op=">=";

	if(!op)
	{
		BGBCC_CompileExpr(ctx, l);
		BGBCC_CompileJmpTrue(ctx, s);
		return;
	}

	BGBCC_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}

void BGBCC_CompileJCF(BGBCC_State *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *op;

	l=BGBCC_ReduceExpr(ctx, l);
//	t=BGBCC_InferExpr(ctx, l);

	if(BGBCC_IsUnaryP(l, "!"))
	{
		BGBCC_CompileJCT(ctx, BCCX_Fetch(l, "value"), s);
		return;
	}

	op=NULL;
	if(BGBCC_IsBinaryP(l, "=="))op="!=";
	if(BGBCC_IsBinaryP(l, "!="))op="==";
	if(BGBCC_IsBinaryP(l, "==="))op="!=";
	if(BGBCC_IsBinaryP(l, "!=="))op="==";
	if(BGBCC_IsBinaryP(l, "<"))op=">=";
	if(BGBCC_IsBinaryP(l, ">"))op="<=";
	if(BGBCC_IsBinaryP(l, "<="))op=">";
	if(BGBCC_IsBinaryP(l, ">="))op="<";

	if(!op)
	{
		BGBCC_CompileExpr(ctx, l);
		BGBCC_CompileJmpFalse(ctx, s);
		return;
	}

	BGBCC_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}

int BGBCC_TryGetSizeofType(BGBCC_State *ctx, BCCX_Node *ty)
{
	char *s;
	
	if(!ctx)return(-1);
	if(!ty)return(-1);

	s=NULL;
	s=BCCX_Get(ty, "name");

	if(ctx->lang==BGBCC_LANG_CS)
	{
		if(!strcmp(s, "char"))return(2);
		if(!strcmp(s, "byte"))return(1);
		if(!strcmp(s, "sbyte"))return(1);
		if(!strcmp(s, "short"))return(2);
		if(!strcmp(s, "int"))return(4);
		if(!strcmp(s, "long"))return(8);
		if(!strcmp(s, "ushort"))return(2);
		if(!strcmp(s, "uint"))return(4);
		if(!strcmp(s, "ulong"))return(8);

		if(!strcmp(s, "bool"))return(1);
		if(!strcmp(s, "decimal"))return(16);
	}

	if(ctx->lang==BGBCC_LANG_JAVA)
	{
		if(!strcmp(s, "char"))return(2);
		if(!strcmp(s, "short"))return(2);
		if(!strcmp(s, "int"))return(4);
		if(!strcmp(s, "long"))return(8);

		if(!strcmp(s, "boolean"))return(1);
	}

	if(!strcmp(s, "float16"))return(2);

	if(!strcmp(s, "int128"))return(16);
	if(!strcmp(s, "float128"))return(16);

	if(!strcmp(s, "char"))return(1);
	if(!strcmp(s, "short"))return(2);
	if(!strcmp(s, "int"))return(4);
	if(!strcmp(s, "llong"))return(8);

	if(ctx->arch==BGBCC_ARCH_X86)
	{
		if(!strcmp(s, "long"))return(4);
	}

	if(ctx->arch==BGBCC_ARCH_X64)
	{
		if(!strcmp(s, "long"))return(8);
	}
	
	return(-1);
}
