#include <bgbccc.h>


void BGBCC_FrCC_EmitLabel(BGBCC_FrCC_State *ctx, char *name)
{
	int i;

	BGBCC_FrBC_EmitLabel(ctx, name);
}

void BGBCC_FrCC_CompileBreak(BGBCC_FrCC_State *ctx)
{
	char *s;

	s=ctx->breakstack[ctx->breakstackpos-1];
	BGBCC_FrBC_EmitJump(ctx, s);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_FrCC_CompileContinue(BGBCC_FrCC_State *ctx)
{
	char *s;

	s=ctx->contstack[ctx->contstackpos-1];
	BGBCC_FrBC_EmitJump(ctx, s);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_FrCC_CompileBreakFalse(BGBCC_FrCC_State *ctx)
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

void BGBCC_FrCC_CompileBreakTrue(BGBCC_FrCC_State *ctx)
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

void BGBCC_FrCC_CompileContinueFalse(BGBCC_FrCC_State *ctx)
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

void BGBCC_FrCC_CompileContinueTrue(BGBCC_FrCC_State *ctx)
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


void BGBCC_FrCC_CompileJmp(BGBCC_FrCC_State *ctx, char *s)
{
	frbc_register reg;
	frbc_type ty;

	BGBCC_FrBC_EmitJump(ctx, s);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_FrCC_CompileJmpFalse(BGBCC_FrCC_State *ctx, char *s)
{
	frbc_register reg;
	frbc_type ty;

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_EQ, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_FrCC_CompileJmpTrue(BGBCC_FrCC_State *ctx, char *s)
{
	frbc_register reg;
	frbc_type ty;

	BGBCC_FrBC_PopRegister(ctx, &reg);
	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	BGBCC_FrBC_EmitJumpRegZero(ctx, ty, FR2C_CMP_NE, reg, s);
	BGBCC_FrBC_RegisterCheckRelease(ctx, reg);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
}

void BGBCC_FrCC_CompileJmpCond(BGBCC_FrCC_State *ctx, char *op, char *s)
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

void BGBCC_FrCC_CompileJCO(BGBCC_FrCC_State *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn, char *s)
{
	BCCX_Node *lt, *rt;
	BCCX_Node *t0, *t1;
	int i, j, k;

	ln=BGBCC_FrCC_ReduceExpr(ctx, ln);
	rn=BGBCC_FrCC_ReduceExpr(ctx, rn);

	BGBCC_FrCC_CompileExpr(ctx, ln);
	BGBCC_FrCC_CompileExpr(ctx, rn);
	BGBCC_FrCC_CompileJmpCond(ctx, op, s);
}

void BGBCC_FrCC_CompileJCT(BGBCC_FrCC_State *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *s0;
	char *op;

	if(!l)
	{
		printf("BGBCC_FrCC_CompileJCT: No Expr\n");
		BGBCC_FrCC_CompileJmp(ctx, s);
		return;
	}

	l=BGBCC_FrCC_ReduceExpr(ctx, l);

	if(BGBCC_FrCC_IsUnaryP(ctx, l, "!"))
	{
		BGBCC_FrCC_CompileJCF(ctx, BCCX_Fetch(l, "value"), s);
		return;
	}

	if(BGBCC_FrCC_IsBinaryP(ctx, l, "&&") ||
			BGBCC_FrCC_IsBinaryP(ctx, l, "||"))
	{
		s0=BGBCC_FrCC_GenSym(ctx);
		BGBCC_FrCC_CompileFormJmpTF(ctx, l, s, s0);
		BGBCC_FrCC_EmitLabel(ctx, s0);
		return;
	}

	op=NULL;
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "=="))op="==";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "!="))op="!=";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "==="))op="==";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "!=="))op="!=";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "<"))op="<";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, ">"))op=">";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "<="))op="<=";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, ">="))op=">=";

	if(!op)
	{
		BGBCC_FrCC_CompileExpr(ctx, l);
		BGBCC_FrCC_CompileJmpTrue(ctx, s);
		return;
	}

	BGBCC_FrCC_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}

void BGBCC_FrCC_CompileJCF(BGBCC_FrCC_State *ctx, BCCX_Node *l, char *s)
{
	BCCX_Node *t;
	char *s0;
	char *op;

	if(!l)
	{
		printf("BGBCC_FrCC_CompileJCF: No Expr\n");
		return;
	}

	l=BGBCC_FrCC_ReduceExpr(ctx, l);
//	t=BGBCC_FrCC_InferExpr(ctx, l);

	if(BGBCC_FrCC_IsUnaryP(ctx, l, "!"))
	{
		BGBCC_FrCC_CompileJCT(ctx, BCCX_Fetch(l, "value"), s);
		return;
	}

	if(BGBCC_FrCC_IsBinaryP(ctx, l, "&&") ||
			BGBCC_FrCC_IsBinaryP(ctx, l, "||"))
	{
		s0=BGBCC_FrCC_GenSym(ctx);
		BGBCC_FrCC_CompileFormJmpTF(ctx, l, s0, s);
		BGBCC_FrCC_EmitLabel(ctx, s0);
		return;
	}

	op=NULL;
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "=="))op="!=";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "!="))op="==";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "==="))op="!=";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "!=="))op="==";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "<"))op=">=";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, ">"))op="<=";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, "<="))op=">";
	if(BGBCC_FrCC_IsBinaryP(ctx, l, ">="))op="<";

	if(!op)
	{
		BGBCC_FrCC_CompileExpr(ctx, l);
		BGBCC_FrCC_CompileJmpFalse(ctx, s);
		return;
	}

	BGBCC_FrCC_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), s);
}

int BGBCC_FrCC_TryGetSizeofType(BGBCC_FrCC_State *ctx, BCCX_Node *ty)
{
	static int rcp=0;
	int sza[2], ala[2];
	char *s;
	int i;
	
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
	
	if(!rcp)
	{
		rcp=1;
		s=BGBCC_FrCC_VarTypeString(ctx, ty);
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

int BGBCC_FrCC_GetMinMaxSizeofType(BGBCC_FrCC_State *ctx, BCCX_Node *ty,
	int *rmsz, int *rnsz, int *rmal, int *rnal)
{
	static int rcp=0;
	int sza[2], ala[2];
	char *s;
	int i;
	
	if(!rcp)
	{
		rcp=1;
		s=BGBCC_FrCC_VarTypeString(ctx, ty);
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

int BGBCC_FrCC_TryGetSizeofName(BGBCC_FrCC_State *ctx, char *name)
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

int BGBCC_FrCC_GetMinMaxSizeofName(BGBCC_FrCC_State *ctx, char *name,
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

int BGBCC_FrCC_GetMinMaxSizeofDerefName(BGBCC_FrCC_State *ctx, char *name,
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
