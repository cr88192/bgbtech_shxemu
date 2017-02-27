#include <bgbccc.h>

bool BGBCC_CCXL_IsRegBasicP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG) &&
		((reg.val&FR2C_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegArgBasicP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG) &&
		((reg.val&FR2C_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegLocalBasicP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL) &&
		((reg.val&FR2C_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegBaseP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG) &&
		((reg.val&FR2C_REGID_REGMASK)<16))
			return(1);
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG) &&
		((reg.val&FR2C_REGID_REGMASK)<16))
			return(1);
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL) &&
		((reg.val&FR2C_REGID_REGMASK)<16))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegBaseExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG) &&
		((reg.val&FR2C_REGID_REGMASK)<256))
			return(1);
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG) &&
		((reg.val&FR2C_REGID_REGMASK)<256))
			return(1);
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL) &&
		((reg.val&FR2C_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegRegExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG) &&
		((reg.val&FR2C_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegArgExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG) &&
		((reg.val&FR2C_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegLocalExtP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL) &&
		((reg.val&FR2C_REGID_REGMASK)<256))
			return(1);
	return(0);
}

bool BGBCC_CCXL_IsRegGlobalP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_GLOBAL)
			return(1);
	return(0);
}

int BGBCC_CCXL_GetRegID(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	return(reg.val&FR2C_REGID_REGMASK);
}

ccxl_type BGBCC_CCXL_MakeTypeID(
	BGBCC_TransState *ctx, int id)
{
	ccxl_type tty;
	tty.val=id;
	return(tty);
}

ccxl_type BGBCC_CCXL_GetRegType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	
	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG) ||
		((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG) ||
		((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL))
	{
		tty.val=(reg.val&FR2C_REGID_TYPEMASK)>>FR2C_REGID_TYPESHIFT;
		return(tty);
	}

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_GLOBAL)
	{
		tty=ctx->reg_globals[reg.val&FR2C_REGID_REGMASK]->type;
//		BGBCC_CCXL_TypeFromSig(ctx, &tty,
//			ctx->reg_globals[reg.val&16777215]->sig);
//		tty.val=FR2C_TY_I;
		return(tty);
	}

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		{ tty.val=FR2C_TY_I; return(tty); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		{ tty.val=FR2C_TY_L; return(tty); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
		{ tty.val=FR2C_TY_F; return(tty); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
		{ tty.val=FR2C_TY_D; return(tty); }

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_STRING)
		{ tty.val=FR2C_TY_SB|FR2C_TY_PTRIDX1; return(tty); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_WSTRING)
		{ tty.val=FR2C_TY_US|FR2C_TY_PTRIDX1; return(tty); }
	
	tty.val=FR2C_TY_UNDEF_I;
	return(tty);
}

ccxl_type BGBCC_CCXL_GetRegDerefType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type bty, dty;
	bty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_TypeDerefType(ctx, bty, &dty);
	return(dty);
}

ccxl_type BGBCC_CCXL_GetRegReturnType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	BGBCC_FrCC_LiteralInfo *obj;
	char *sig;
	ccxl_type bty, dty;
	int i, j, k;

	//HACK
	if(BGBCC_CCXL_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegID(ctx, reg);
		sig=ctx->reg_globals[i]->sig;
		if(*sig=='(')
		{
			sig=BGBCC_CCXL_SigGetReturnSig(ctx, sig);
			BGBCC_CCXL_TypeFromSig(ctx,
				&dty, sig);
			return(dty);
		}
		
		BGBCC_CCXL_TypeFromSig(ctx,
			&bty, sig);

		if(BGBCC_CCXL_TypeFunctionP(ctx, bty))
		{
			i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, bty);
			obj=ctx->literals[i];

			sig=obj->decl->sig;
			sig=BGBCC_CCXL_SigGetReturnSig(ctx, sig);
			BGBCC_CCXL_TypeFromSig(ctx,
				&dty, sig);
			return(dty);
		}

		BGBCC_CCXL_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(dty);
	}

	//More HACK
	bty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeFunctionP(ctx, bty))
	{
		i=BGBCC_CCXL_TypeObjectLiteralIndex(ctx, bty);
		obj=ctx->literals[i];

		sig=obj->decl->sig;
		sig=BGBCC_CCXL_SigGetReturnSig(ctx, sig);
		BGBCC_CCXL_TypeFromSig(ctx,
			&dty, sig);
		return(dty);
	}

	BGBCC_CCXL_StubWarn(ctx);
	dty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_I);

//	bty=BGBCC_CCXL_GetRegType(ctx, reg);
//	BGBCC_CCXL_TypeDerefType(ctx, bty, &dty);
	return(dty);
}

ccxl_type BGBCC_CCXL_GetRegPointerToType(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type bty, dty;
	bty=BGBCC_CCXL_GetRegType(ctx, reg);
	BGBCC_CCXL_TypePointerType(ctx, bty, &dty);
	return(dty);
}

bool BGBCC_CCXL_IsRegIntP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeIntP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegLongP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeLongP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegFloatP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeFloatP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegDoubleP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegPointerP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	ccxl_type tty;
	tty=BGBCC_CCXL_GetRegType(ctx, reg);
	if(BGBCC_CCXL_TypePointerP(ctx, tty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmIntP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmLongP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmFloatP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmDoubleP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmStringP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_STRING)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmWStringP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_WSTRING)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmLiteralP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LITERAL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmGlobalAddrP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_GBLADDR)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt12P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		if((i>=(-2048)) && (i<2048))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImm12P(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f, g;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, reg) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, reg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
			if((i>=(-2048)) && (i<2048))
				return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_EncodeFloat12(f);
		g=BGBCC_DecodeFloat12(i);
		return(f==g);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImm32P(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f, g;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
//			if((i>=(-2048)) && (i<2048))
				return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
//		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
//		i=BGBCC_EncodeFloat12(f);
//		g=BGBCC_DecodeFloat12(i);
		g=(float)f;
		return(f==g);
	}

	return(false);
}

bool BGBCC_CCXL_IsRegImmIntLit12P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i, j;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS32(ctx, i);
		if((j>=0) && (j<4096))
			return(true);
//		if((i>=(-2048)) && (i<2048))
//			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt16P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		if((i>=(-32768)) && (i<32768))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt24P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		if((i>=(-16777216)) && (i<16777216))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmInt32P(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	int i;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
//		if((i>=(-16777216)) && (i<16777216))
			return(true);
	}
	return(false);
}

bool BGBCC_CCXL_IsRegImmP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_STRING)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_WSTRING)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LITERAL)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_GBLADDR)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmZeroP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		{ return(!(reg.val&FR2C_REGINT_MASK)); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		{ return(!(reg.val&FR2C_REGLONG_MASK)); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
		{ return(!(reg.val&FR2C_REGINT_MASK)); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
		{ return(!(reg.val&FR2C_REGLONG_MASK)); }
	return(false);
}

bool BGBCC_CCXL_IsRegImmILP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		return(true);
	return(false);
}

bool BGBCC_CCXL_IsRegImmILFDP(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
		return(true);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
		return(true);
	return(false);
}

#if 0
int BGBCC_CCXL_GetRegImmLit12(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	s64 li;
	double lf;
	int i, j;

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS32(ctx, i);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS64(ctx, li);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmFloatValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF32(ctx, lf);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF64(ctx, lf);
		if((j>=0) && (j<4096))
			return(j);
		return(-1);
	}

	return(-1);
}
#endif

int BGBCC_CCXL_GetRegImmLit12(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	int i;
	i=BGBCC_CCXL_GetRegImmLitIndex(ctx, reg, type);
	if((i>=0) && (i<4096))
		return(i);
	return(-1);
}

int BGBCC_CCXL_GetRegImmLit16(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	int i;
	i=BGBCC_CCXL_GetRegImmLitIndex(ctx, reg, type);
	if((i>=0) && (i<65536))
		return(i);
	return(-1);
}

int BGBCC_CCXL_GetRegImmLit24(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	int i;
	i=BGBCC_CCXL_GetRegImmLitIndex(ctx, reg, type);
	if((i>=0) && (i<16777216))
		return(i);
	return(-1);
}

int BGBCC_CCXL_GetRegImmLitIndex(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	s64 li;
	double lf;
	int i, j;

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS32(ctx, i);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		li=BGBCC_CCXL_GetRegImmLongValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitS64(ctx, li);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmFloatValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF32(ctx, lf);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

//	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		lf=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		j=BGBCC_CCXL_IndexLitF64(ctx, lf);
		return(j);
//		if((j>=0) && (j<65536))
//			return(j);
//		return(-1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmZeroP(ctx, reg))
		{
			return(2);
		}

		if(BGBCC_CCXL_IsRegImmStringP(ctx, reg))
		{
			j=reg.val&FR2C_REGINT_MASK;
			j=(j<<2)|2;
			return(j);
//			if((j>=0) && (j<65536))
//				return(j);
//			return(-1);
		}

		if(BGBCC_CCXL_IsRegImmWStringP(ctx, reg))
		{
			j=reg.val&FR2C_REGINT_MASK;
			j=(j<<3)|1;
			return(j);
//			if((j>=0) && (j<65536))
//				return(j);
//			return(-1);
		}

		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LITERAL)
		{
			j=reg.val&FR2C_REGINT_MASK;
			j=(j<<2)|0;
			return(j);
		}

		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_GBLADDR)
		{
			j=reg.val&FR2C_REGINT_MASK;
			j=(j<<3)|5;
			return(j);
		}
		
		return(-1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmZeroP(ctx, reg))
		{
			return(2);
		}

		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LITERAL)
		{
			j=reg.val&FR2C_REGINT_MASK;
			j=(j<<2)|0;
			return(j);
		}
		
		return(-1);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmZeroP(ctx, reg))
		{
			return(2);
		}

		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LITERAL)
		{
			j=reg.val&FR2C_REGINT_MASK;
			j=(j<<2)|0;
			return(j);
		}
		
		return(-1);
	}

	return(-1);
}

int BGBCC_CCXL_GetRegImmIntValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		{ return((int)(reg.val&FR2C_REGINT_MASK)); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		{ return((int)(reg.val&FR2C_REGINT_MASK)); }

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }

	return(0);
}

s64 BGBCC_CCXL_GetRegImmLongValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	s64 v;

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		{ return((int)(reg.val&FR2C_REGINT_MASK)); }

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
	{
		v=reg.val&FR2C_REGLONG_MASK;
		if(v&((~FR2C_REGLONG_MASK)>>1))
			v|=~FR2C_REGLONG_MASK;
		return(v);
	}

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
		{ return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg)); }

	return(0);
}

double BGBCC_CCXL_GetRegImmFloatValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	return(BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg));
}

double BGBCC_CCXL_GetRegImmDoubleValue(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	u64 fl;
	u32 fi;

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		{ return((int)(reg.val&FR2C_REGINT_MASK)); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		{ return(BGBCC_CCXL_GetRegImmLongValue(ctx, reg)); }

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT)
	{
		fi=reg.val&FR2C_REGINT_MASK;
		return(*(float *)(&fi));
	}

	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE)
	{
		fl=reg.val&FR2C_REGLONG_MASK;
		fl=fl<<8;
		return(*(double *)(&fl));
	}
	
	return(0);
}

int BGBCC_CCXL_GetRegImm12Value(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		return(i&4095);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_EncodeFloat12(f);
		return(i);
	}

#if 0
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_INT)
		{ return(((int)(reg.val&FR2C_REGINT_MASK))&4095); }
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_LONG)
		{ return(((int)(reg.val&FR2C_REGINT_MASK))&4095); }

	if(((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_FLOAT) ||
		((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_IMM_DOUBLE))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_EncodeFloat12(f);
		return(i&4095);
	}
#endif

	return(0);
}

int BGBCC_CCXL_GetRegImm32Value(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	double f;
	float f2;
	int i;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		i=BGBCC_CCXL_GetRegImmIntValue(ctx, reg);
		return(i);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_Float16_Init();
		f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, reg);
//		i=BGBCC_EncodeFloat12(f);
		f2=f;
		i=*(int *)(&f2);
		
		return(i);
	}

	return(0);
}

ccxl_status BGBCC_CCXL_GetRegForIntValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val)
{
	ccxl_register treg;
	
	treg.val=(val&FR2C_REGINT_MASK)|FR2C_REGTY_IMM_INT;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForLongValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s64 val)
{
	ccxl_register treg;
	
	treg.val=(val&FR2C_REGLONG_MASK)|FR2C_REGTY_IMM_LONG;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForFloatValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, f32 val)
{
	ccxl_register treg;
	
	treg.val=((*(u32 *)(&val))&FR2C_REGINT_MASK)|FR2C_REGTY_IMM_FLOAT;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForDoubleValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, f64 val)
{
	ccxl_register treg;
	
	treg.val=(((*(u64 *)(&val))>>8)&FR2C_REGLONG_MASK)|FR2C_REGTY_IMM_DOUBLE;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForStringValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str)
{
	ccxl_register treg;
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	treg.val=(i&FR2C_REGINT_MASK)|FR2C_REGTY_IMM_STRING;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForWStringValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, char *str)
{
	ccxl_register treg;
	int i;
	
	i=BGBCC_CCXL_IndexString(ctx, str);
	treg.val=(i&FR2C_REGINT_MASK)|FR2C_REGTY_IMM_WSTRING;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForLiteralValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val)
{
	ccxl_register treg;
	
	treg.val=(val&FR2C_REGINT_MASK)|FR2C_REGTY_IMM_LITERAL;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}

ccxl_status BGBCC_CCXL_GetRegForGlobalAddrValue(
	BGBCC_TransState *ctx, ccxl_register *rreg, s32 val)
{
	ccxl_register treg;
	
	treg.val=(val&FR2C_REGINT_MASK)|FR2C_REGTY_IMM_GBLADDR;
	*rreg=treg;
	return(FR2C_STATUS_YES);
}


int BGBCC_CCXL_GetRegOprMode1(
	BGBCC_TransState *ctx, ccxl_register reg)
{
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
		return(FR2C_OPMODE_RR);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
		return(FR2C_OPMODE_RA);
	if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
		return(FR2C_OPMODE_RL);
	return(-1);
}

int BGBCC_CCXL_GetRegOprMode2(
	BGBCC_TransState *ctx, ccxl_register rega, ccxl_register regb)
{
	if((rega.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
	{
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE_RR);
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE_RA);
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE_RL);
	}

	if((rega.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
	{
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE_AR);
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE_AA);
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE_AL);
	}

	if((rega.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
	{
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE_LR);
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE_LA);
		if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE_LL);
	}

	return(-1);
}

int BGBCC_CCXL_GetRegOprMode3(
	BGBCC_TransState *ctx, ccxl_register dst,
	ccxl_register rega, ccxl_register regb)
{
	if((dst.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
		{ return(BGBCC_CCXL_GetRegOprMode2(ctx, rega, regb)); }

	if((dst.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
	{
		if((rega.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
		{
			if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
				return(FR2C_OPMODE_ARR);
		}
	}

	if((dst.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
	{
		if((rega.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
		{
			if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
				return(FR2C_OPMODE_LRR);
			if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
				return(FR2C_OPMODE_LRA);
			if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
				return(FR2C_OPMODE_LRL);
		}

		if((rega.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
		{
			if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
				return(FR2C_OPMODE_LAR);
		}

		if((rega.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
		{
			if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
				return(FR2C_OPMODE_LLR);
			if((regb.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
				return(FR2C_OPMODE_LLL);
		}
	}

	return(-1);
}

int BGBCC_CCXL_GetRegOprModeW(
	BGBCC_TransState *ctx, ccxl_register reg, ccxl_type type)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE2_IR);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE2_IA);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE2_IL);
	}
	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE2_LR);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE2_LA);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE2_LL);
	}
	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE2_FR);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE2_FA);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE2_FL);
	}
	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE2_DR);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE2_DA);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE2_DL);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, type))
	{
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE2_PR);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE2_PA);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE2_PL);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_REG)
			return(FR2C_OPMODE2_PR);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_ARG)
			return(FR2C_OPMODE2_PA);
		if((reg.val&FR2C_REGTY_REGMASK)==FR2C_REGTY_LOCAL)
			return(FR2C_OPMODE2_PL);
	}
	return(-1);
}

int BGBCC_CCXL_GetTypeOperationZ(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(FR2C_TY_P);
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(FR2C_TY_P);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
		return(FR2C_TY_S);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<16)
		return(i);
	
	if(i==FR2C_TY_VARIANT)
		return(FR2C_TY_V);
	
	return(-1);
//	return(FR2C_TY_UNDEF_I);
}

int BGBCC_CCXL_GetTypeOperationExtZ(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(FR2C_TY_P);
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(FR2C_TY_P);

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
		return(FR2C_TY_S);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
		return(i);
	
	if(i==FR2C_TY_VARIANT)
		return(FR2C_TY_V);
	
	return(-1);
//	return(FR2C_TY_UNDEF_I);
}
