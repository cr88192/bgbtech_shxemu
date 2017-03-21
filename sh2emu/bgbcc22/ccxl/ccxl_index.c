#include <bgbccc.h>

ccxl_label BGBCC_CCXL_LabelFromName(BGBCC_TransState *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->n_goto; i++)
	{
		if(!strcmp(ctx->goto_name[i], name))
			return(ctx->goto_lbl[i]);
	}
	
	if(!ctx->goto_name)
	{
		i=256;
		ctx->goto_name=bgbcc_malloc(i*sizeof(char *));
		ctx->goto_lbl=bgbcc_malloc(i*sizeof(ccxl_label));
		ctx->m_goto=i;
	}
	
	if((ctx->n_goto+1)>=ctx->m_goto)
	{
		i=ctx->m_goto+(ctx->m_goto>>1);
		ctx->goto_name=bgbcc_realloc(ctx->goto_name, i*sizeof(char *));
		ctx->goto_lbl=bgbcc_realloc(ctx->goto_lbl, i*sizeof(ccxl_label));
		ctx->m_goto=i;
	}
	
	i=ctx->n_goto++;
	ctx->goto_name[i]=bgbcc_strdup(name);
	ctx->goto_lbl[i]=BGBCC_CCXL_GenSym(ctx);
	return(ctx->goto_lbl[i]);
}

void BGBCC_CCXL_CompileBreak(BGBCC_TransState *ctx)
{
	ccxl_label l;

	l=ctx->breakstack[ctx->breakstackpos-1];
	BGBCC_CCXL_EmitJump(ctx, l);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinue(BGBCC_TransState *ctx)
{
	ccxl_label l;

	l=ctx->contstack[ctx->contstackpos-1];
	BGBCC_CCXL_EmitJump(ctx, l);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileBreakFalse(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label l;

	l=ctx->breakstack[ctx->breakstackpos-1];

	BGBCC_CCXL_PopRegister(ctx, &reg);
	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_EQ, reg, l);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileBreakTrue(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label l;

	l=ctx->breakstack[ctx->breakstackpos-1];

	BGBCC_CCXL_PopRegister(ctx, &reg);
	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_NE, reg, l);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinueFalse(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label l;

	l=ctx->contstack[ctx->contstackpos-1];

	BGBCC_CCXL_PopRegister(ctx, &reg);
	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_EQ, reg, l);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileContinueTrue(BGBCC_TransState *ctx)
{
	ccxl_register reg;
	ccxl_type ty;
	ccxl_label lbl;

	lbl=ctx->contstack[ctx->contstackpos-1];

	BGBCC_CCXL_PopRegister(ctx, &reg);
	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_NE, reg, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}


void BGBCC_CCXL_CompileJmp(BGBCC_TransState *ctx, ccxl_label lbl)
{
	ccxl_register reg;
	ccxl_type ty;

	BGBCC_CCXL_EmitJump(ctx, lbl);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpFalse(BGBCC_TransState *ctx, ccxl_label lbl)
{
	ccxl_register reg;
	ccxl_type ty;

	BGBCC_CCXL_PopRegister(ctx, &reg);
	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_EQ, reg, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpTrue(BGBCC_TransState *ctx, ccxl_label lbl)
{
	ccxl_register reg;
	ccxl_type ty;

	BGBCC_CCXL_PopRegister(ctx, &reg);
	ty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_EmitJumpRegZero(ctx, ty, CCXL_CMP_NE, reg, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, reg);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJmpCond(BGBCC_TransState *ctx,
	char *op, ccxl_label lbl)
{
	ccxl_register rega, regb;
	ccxl_type ty;
	int opr;

	if(!op)return;
	
	opr=-1;
	if(!strcmp(op, "=="))	{ opr=CCXL_CMP_EQ; }
	if(!strcmp(op, "!="))	{ opr=CCXL_CMP_NE; }
	if(!strcmp(op, "<"))	{ opr=CCXL_CMP_LT; }
	if(!strcmp(op, ">"))	{ opr=CCXL_CMP_GT; }
	if(!strcmp(op, "<="))	{ opr=CCXL_CMP_LE; }
	if(!strcmp(op, ">="))	{ opr=CCXL_CMP_GE; }
	
	BGBCC_CCXL_PopRegister(ctx, &regb);
	BGBCC_CCXL_PopRegister(ctx, &rega);
	ty=BGBCC_CCXL_GetRegType(ctx, rega);
	BGBCC_CCXL_EmitJumpRegCmp(ctx, ty, opr, rega, regb, lbl);
	BGBCC_CCXL_RegisterCheckRelease(ctx, rega);
	BGBCC_CCXL_RegisterCheckRelease(ctx, regb);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
}

void BGBCC_CCXL_CompileJCO(BGBCC_TransState *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn, ccxl_label lbl)
{
	BCCX_Node *lt, *rt;
	BCCX_Node *t0, *t1;
	int i, j, k;

	ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
	rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

	BGBCC_CCXL_CompileExpr(ctx, ln);
	BGBCC_CCXL_CompileExpr(ctx, rn);
	BGBCC_CCXL_CompileJmpCond(ctx, op, lbl);
}

void BGBCC_CCXL_CompileJCT(BGBCC_TransState *ctx,
	BCCX_Node *l, ccxl_label lbl)
{
	BCCX_Node *t;
	ccxl_label lbl1;
	char *op;

	if(!l)
	{
		printf("BGBCC_CCXL_CompileJCT: No Expr\n");
		BGBCC_CCXL_CompileJmp(ctx, lbl);
		return;
	}

	l=BGBCC_CCXL_ReduceExpr(ctx, l);

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "!"))
	{
		BGBCC_CCXL_CompileJCF(ctx, BCCX_Fetch(l, "value"), lbl);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&") ||
			BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		lbl1=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileFormJmpTF(ctx, l, lbl, lbl1);
		BGBCC_CCXL_EmitLabel(ctx, lbl1);
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
		BGBCC_CCXL_CompileJmpTrue(ctx, lbl);
		return;
	}

	BGBCC_CCXL_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), lbl);
}

void BGBCC_CCXL_CompileJCF(BGBCC_TransState *ctx, BCCX_Node *l, ccxl_label lbl)
{
	BCCX_Node *t;
	ccxl_label lbl1;
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
		BGBCC_CCXL_CompileJCT(ctx, BCCX_Fetch(l, "value"), lbl);
		return;
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&") ||
			BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		lbl1=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileFormJmpTF(ctx, l, lbl1, lbl);
		BGBCC_CCXL_EmitLabel(ctx, lbl1);
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
		BGBCC_CCXL_CompileJmpFalse(ctx, lbl);
		return;
	}

	BGBCC_CCXL_CompileJCO(ctx, op,
		BCCX_Fetch(l, "left"), BCCX_Fetch(l, "right"), lbl);
}

int BGBCC_CCXL_TryGetSizeofType(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	static int rcp=0;
	int sza[2], ala[2];
	char *s;
	int i;
	
	if(!ctx)return(-1);
	if(!ty)return(-1);

	i=BCCX_GetInt(ty, "ind");
	if(i)
	{
		if(ctx->arch_sizeof_ptr)
			return(ctx->arch_sizeof_ptr);
		return(-1);
	}

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
		if(!strcmp(s, "long") || !strcmp(s, "ulong"))
		{
			if(ctx->arch_sizeof_long)
				return(ctx->arch_sizeof_long);
			return(4);
		}
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
		i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
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
		i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
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
	ccxl_register treg;
	ccxl_type ty;
	char *s;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, &treg, false);
	if(i<=0)return(-1);

	ty=BGBCC_CCXL_GetRegType(ctx, treg);
	s=BGBCC_CCXL_TypeGetSig(ctx, ty);

	i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
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
	ccxl_register treg;
	ccxl_type ty;
	char *s;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, &treg, false);
	if(i<=0)return(-1);

	ty=BGBCC_CCXL_GetRegType(ctx, treg);
	s=BGBCC_CCXL_TypeGetSig(ctx, ty);

	i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
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
	ccxl_register treg;
	ccxl_type ty;
	char *s;
	int i;

	i=BGBCC_CCXL_TryLookupAsRegister(ctx, name, &treg, false);
	if(i<=0)return(-1);

	ty=BGBCC_CCXL_GetRegDerefType(ctx, treg);
	s=BGBCC_CCXL_TypeGetSig(ctx, ty);

	i=BGBCC_CCXL_GetSigMinMaxSize(ctx, s, sza, ala);
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
