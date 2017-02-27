#include <bgbccc.h>


void BGBCC_CCXL_EmitLabel(BGBCC_TransState *ctx, char *name)
{
	int i;

	BGBCC_FrBC_EmitLabel(ctx, name);
}

void BGBCC_CCXL_CompileBreak(BGBCC_TransState *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	BGBCC_FrBC_EmitJump(ctx, s);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinue(BGBCC_TransState *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	BGBCC_FrBC_EmitJump(ctx, s);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileBreakFalse(BGBCC_TransState *ctx)
{
	frbc_register reg;
	frbc_type ty;
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_EQ, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileBreakTrue(BGBCC_TransState *ctx)
{
	frbc_register reg;
	frbc_type ty;
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_NE, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinueFalse(BGBCC_TransState *ctx)
{
	frbc_register reg;
	frbc_type ty;
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_EQ, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinueTrue(BGBCC_TransState *ctx)
{
	frbc_register reg;
	frbc_type ty;
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_NE, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}


void BGBCC_CCXL_CompileJmp(BGBCC_TransState *ctx, char *s)
{
	frbc_register reg;
	frbc_type ty;

	BGBCC_FrBC_EmitJump(ctx, s);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpFalse(BGBCC_TransState *ctx, char *s)
{
	frbc_register reg;
	frbc_type ty;

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_EQ, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpTrue(BGBCC_TransState *ctx, char *s)
{
	frbc_register reg;
	frbc_type ty;

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_NE, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpCond(BGBCC_TransState *ctx, char *op, char *s)
{
	frbc_register rega, regb;
	frbc_type ty;
	int opr;

	if(!op)return;
	
	opr=-1;
	if(!strcmp(op, "=="))	{ opr=FR2C_CMP_EQ; }
	if(!strcmp(op, "!="))	{ opr=FR2C_CMP_NE; }
	if(!strcmp(op, "<"))	{ opr=FR2C_CMP_LT; }
	if(!strcmp(op, ">"))	{ opr=FR2C_CMP_GT; }
	if(!strcmp(op, "<="))	{ opr=FR2C_CMP_LE; }
	if(!strcmp(op, ">="))	{ opr=FR2C_CMP_GE; }
	
	BGBCC_FrBC_PopRegister(ctx, &regb);
	BGBCC_FrBC_PopRegister(ctx, &rega);
	ty=BGBCC_FrBC_GetRegType(ctx, rega);
	BGBCC_FrBC_EmitJumpRegCmp(ctx, ty, opr, rega, regb, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, rega);
	BGBCC_FrBC_RegisterCheckRelease(ctx, regb);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJCO(BGBCC_TransState *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn, char *s)
{
	BCCX_Node *lt, *rt;
	BCCX_Node *t0, *t1;
	int i, j, k;

	ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
	rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

	BGBCC_CCXL_CompileExpr(ctx, ln);
	BGBCC_CCXL_CompileExpr(ctx, rn);
	BGBCC_CCXL_CompileJmpCond(ctx, op, s);
}

void BGBCC_CCXL_CompileJCT(BGBCC_TransState *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *s0;
	char *op;

	if(!l)
	{
		printf("BGBCC_CCXL_CompileJCT: No Expr\n");
		BGBCC_CCXL_CompileJmp(ctx, s);
		return;
	}

	l=BGBCC_CCXL_ReduceExpr(ctx, l);

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "!"))
	{
		BGBCC_CCXL_CompileJCF(ctx, BCCX_Fetch(l, "value"), s);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&") ||
			BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		s0=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileFormJmpTF(ctx, l, s, s0);
		BGBCC_CCXL_EmitLabel(ctx, s0);
		return;
	}

	op=NULL;
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "=="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "==="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!=="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<"))op="<";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">"))op=">";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<="))op="<=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">="))op=">=";

	if(!op)
	{
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_CompileJmpTrue(ctx, s);
		return;
	}

	BGBCC_CCXL_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}

void BGBCC_CCXL_CompileJCF(BGBCC_TransState *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *s0;
	char *op;

	if(!l)
	{
		printf("BGBCC_CCXL_CompileJCF: No Expr\n");
		return;
	}

	l=BGBCC_CCXL_ReduceExpr(ctx, l);
//	t=BGBCC_CCXL_InferExpr(ctx, l);

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "!"))
	{
		BGBCC_CCXL_CompileJCT(ctx, BCCX_Fetch(l, "value"), s);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&") ||
			BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		s0=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileFormJmpTF(ctx, l, s0, s);
		BGBCC_CCXL_EmitLabel(ctx, s0);
		return;
	}

	op=NULL;
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "=="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "==="))op="!=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "!=="))op="==";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<"))op=">=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">"))op="<=";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, "<="))op=">";
	if(BGBCC_CCXL_IsBinaryP(ctx, l, ">="))op="<";

	if(!op)
	{
		BGBCC_CCXL_CompileExpr(ctx, l);
		BGBCC_CCXL_CompileJmpFalse(ctx, s);
		return;
	}

	BGBCC_CCXL_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}

int BGBCC_CCXL_TryGetSizeofType(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	static int rcp=0;
	int sza[2], ala[2];
	char *s;
	int i;
	
	if(!ctx)return(-1);
	if(!ty)return(-1);

	s=NULL;
	s=BCCX_Get(ty, "name");

	if(!strcmp(s, "char"))return(1);
	if(!strcmp(s, "short"))return(2);
	if(!strcmp(s, "int"))return(4);
	if(!strcmp(s, "llong"))return(8);

	if(!strcmp(s, "float16"))return(2);
	if(!strcmp(s, "int128"))return(16);
	if(!strcmp(s, "float128"))return(16);

//	if(ctx->arch==BGBCC_ARCH_X86)
	if(1)
	{
		if(!strcmp(s, "long"))return(4);
	}

//	if(ctx->arch==BGBCC_ARCH_X64)
	if(0)
	{
		if(!strcmp(s, "long"))return(8);
	}
	
	if(!rcp)
	{
		rcp=1;
		s=BGBCC_CCXL_VarTypeString(ctx, ty);
		i=BGBCC_FrBC_GetSigMinMaxSize(ctx, s, sza, ala);
		rcp=0;
		if(i>=0)
		{
			if(sza[0]==sza[1])
				return(sza[0]);
		}

		return(-1);
	}
	
	return(-1);
}

int BGBCC_CCXL_GetMinMaxSizeofType(BGBCC_TransState *ctx, BCCX_Node *ty,
	int *rmsz, int *rnsz, int *rmal, int *rnal)
{
	static int rcp=0;
	int sza[2], ala[2];
	char *s;
	int i;
	
	if(!rcp)
	{
		rcp=1;
		s=BGBCC_CCXL_VarTypeString(ctx, ty);
		i=BGBCC_FrBC_GetSigMinMaxSize(ctx, s, sza, ala);
		rcp=0;
		if(i>=0)
		{
			if(rmsz)*rmsz=sza[0];
			if(rnsz)*rnsz=sza[1];
			if(rmal)*rmal=ala[0];
			if(rnal)*rnal=ala[1];
			return(0);
		}

		return(-1);
	}
	
	return(-1);
}

int BGBCC_CCXL_TryGetSizeofName(BGBCC_TransState *ctx, char *name)
{
	static int rcp=0;
	int sza[2], ala[2];
	frbc_register treg;
	frbc_type ty;
	char *s;
	int i;

	i=BGBCC_FrBC_TryLookupAsRegister(ctx, name, &treg);
	if(i<=0)return(-1);

	ty=BGBCC_FrBC_GetRegType(ctx, treg);
	s=BGBCC_FrBC_TypeGetSig(ctx, ty);

	i=BGBCC_FrBC_GetSigMinMaxSize(ctx, s, sza, ala);
	rcp=0;
	if(i>=0)
	{
		if(sza[0]==sza[1])
			return(sza[0]);
	}

	return(-1);
}

int BGBCC_CCXL_GetMinMaxSizeofName(BGBCC_TransState *ctx, char *name,
	int *rmsz, int *rnsz, int *rmal, int *rnal)
{
	static int rcp=0;
	int sza[2], ala[2];
	frbc_register treg;
	frbc_type ty;
	char *s;
	int i;

	i=BGBCC_FrBC_TryLookupAsRegister(ctx, name, &treg);
	if(i<=0)return(-1);

	ty=BGBCC_FrBC_GetRegType(ctx, treg);
	s=BGBCC_FrBC_TypeGetSig(ctx, ty);

	i=BGBCC_FrBC_GetSigMinMaxSize(ctx, s, sza, ala);
	rcp=0;
	if(i>=0)
	{
		if(rmsz)*rmsz=sza[0];
		if(rnsz)*rnsz=sza[1];
		if(rmal)*rmal=ala[0];
		if(rnal)*rnal=ala[1];
		return(0);
	}

	return(-1);
}

int BGBCC_CCXL_GetMinMaxSizeofDerefName(BGBCC_TransState *ctx, char *name,
	int *rmsz, int *rnsz, int *rmal, int *rnal)
{
	static int rcp=0;
	int sza[2], ala[2];
	frbc_register treg;
	frbc_type ty;
	char *s;
	int i;

	i=BGBCC_FrBC_TryLookupAsRegister(ctx, name, &treg);
	if(i<=0)return(-1);

	ty=BGBCC_FrBC_GetRegDerefType(ctx, treg);
	s=BGBCC_FrBC_TypeGetSig(ctx, ty);

	i=BGBCC_FrBC_GetSigMinMaxSize(ctx, s, sza, ala);
	rcp=0;
	if(i>=0)
	{
		if(rmsz)*rmsz=sza[0];
		if(rnsz)*rnsz=sza[1];
		if(rmal)*rmal=ala[0];
		if(rnal)*rnal=ala[1];
		return(0);
	}

	return(-1);
}
