#include <bgbccc.h>


void BGBCC_BS2IL_EmitLabel(BGBCC_State *ctx, char *name)
{
	int i;

	bgbcc_print(ctx, "%s: ", name);
}

void BGBCC_BS2IL_CompileBreak(BGBCC_State *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	bgbcc_print(ctx, "jmp %s; ", s);
}

void BGBCC_BS2IL_CompileContinue(BGBCC_State *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	bgbcc_print(ctx, "jmp %s; ", s);
}

void BGBCC_BS2IL_CompileBreakFalse(BGBCC_State *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	bgbcc_print(ctx, "jmp_false %s; ", s);
}

void BGBCC_BS2IL_CompileBreakTrue(BGBCC_State *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	bgbcc_print(ctx, "jmp_true %s; ", s);
}

void BGBCC_BS2IL_CompileContinueFalse(BGBCC_State *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	bgbcc_print(ctx, "jmp_false %s; ", s);
}

void BGBCC_BS2IL_CompileContinueTrue(BGBCC_State *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	bgbcc_print(ctx, "jmp_true %s; ", s);
}


void BGBCC_BS2IL_CompileJmp(BGBCC_State *ctx, char *s)
{
	bgbcc_print(ctx, "jmp %s; ", s);
}

void BGBCC_BS2IL_CompileJmpFalse(BGBCC_State *ctx, char *s)
{
	bgbcc_print(ctx, "jmp_false %s; ", s);
}

void BGBCC_BS2IL_CompileJmpTrue(BGBCC_State *ctx, char *s)
{
	bgbcc_print(ctx, "jmp_true %s; ", s);
}

void BGBCC_BS2IL_CompileJmpCond(BGBCC_State *ctx, char *op, char *s)
{
	char *op1;

	if(!op)return;
	
	op1=NULL;
	if(!strcmp(op, "=="))op1="eq";
	if(!strcmp(op, "!="))op1="ne";
	if(!strcmp(op, "<"))op1="l";
	if(!strcmp(op, ">"))op1="g";
	if(!strcmp(op, "<="))op1="le";
	if(!strcmp(op, ">="))op1="ge";
	
	if(op1)
		bgbcc_print(ctx, "jmp_%s %s; ", op1, s);
	else bgbcc_print(ctx, "%s%s ", op, s);
}

void BGBCC_BS2IL_CompileJCO(BGBCC_State *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn, char *s)
{
	BCCX_Node *lt, *rt;
	BCCX_Node *t0, *t1;
	int i, j, k;

	ln=BGBCC_ReduceExpr(ctx, ln);
	rn=BGBCC_ReduceExpr(ctx, rn);

	BGBCC_BS2IL_CompileExpr(ctx, ln);
	BGBCC_BS2IL_CompileExpr(ctx, rn);
	BGBCC_BS2IL_CompileJmpCond(ctx, op, s);
}

void BGBCC_BS2IL_CompileJCT(BGBCC_State *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *op;

	l=BGBCC_ReduceExpr(ctx, l);
//	t=BGBCC_InferExpr(ctx, l);

	if(BGBCC_IsUnaryP(l, "!"))
	{
		BGBCC_BS2IL_CompileJCF(ctx, BCCX_Fetch(l, "value"), s);
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
		BGBCC_BS2IL_CompileExpr(ctx, l);
		BGBCC_BS2IL_CompileJmpTrue(ctx, s);
		return;
	}

	BGBCC_BS2IL_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}

void BGBCC_BS2IL_CompileJCF(BGBCC_State *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *op;

	l=BGBCC_ReduceExpr(ctx, l);
//	t=BGBCC_InferExpr(ctx, l);

	if(BGBCC_IsUnaryP(l, "!"))
	{
		BGBCC_BS2IL_CompileJCT(ctx, BCCX_Fetch(l, "value"), s);
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
		BGBCC_BS2IL_CompileExpr(ctx, l);
		BGBCC_BS2IL_CompileJmpFalse(ctx, s);
		return;
	}

	BGBCC_BS2IL_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}
