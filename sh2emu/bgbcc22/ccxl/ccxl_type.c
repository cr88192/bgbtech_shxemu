#include <bgbccc.h>

bool BGBCC_CCXL_TypeSmallIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UI)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_US)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_US)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeUnsignedP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_US)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UB)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNL)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_I)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_L)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UI)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeFloatP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_F)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeDoubleP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_D)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeBaseILFD_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_D)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_US)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeBaseSmallILFD_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_D)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_US)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeBaseSmallILFDP_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);
	return(BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, ty));
}

bool BGBCC_CCXL_TypeBaseSmallIL_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_L)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_US)
		return(true);

	return(false);
}

int BGBCC_CCXL_GetTypeBaseType(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
		{ return(ty.val&FR2C_TY_BASEMASK); }

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
		{ return(ty.val&FR2C_TYB2_BASEMASK); }

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&FR2C_TYOVF_IDXMASK]->base);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(-1);
}

bool BGBCC_CCXL_TypePointerP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_P)
			return(true);
		if(ty.val&FR2C_TY_PTRMASK)
			return(true);
		return(false);
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		if((ty.val&FR2C_TYB2_BASEMASK)==FR2C_TY_P)
			return(true);
		if(ty.val&FR2C_TYB2_PTRMASK)
			return(true);
		return(false);
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&FR2C_TYOVF_IDXMASK]->pn!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeArrayP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		if(ty.val&FR2C_TY_ARRMASK)
			return(true);
		return(false);
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		if(ty.val&FR2C_TYB2_ARRMASK)
			return(true);
		return(false);
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&FR2C_TYOVF_IDXMASK]->an!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeArrayOrPointerP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSignedByteP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SB)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedByteP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UB)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSignedShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_SS)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_US)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVoidP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_V)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeEqualP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	return(dty.val==sty.val);
}

bool BGBCC_CCXL_TypeBaseEqualP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	if(dty.val==sty.val)
		return(true);

	if(BGBCC_CCXL_TypeIntP(ctx, dty) && BGBCC_CCXL_TypeIntP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeLongP(ctx, dty) && BGBCC_CCXL_TypeLongP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeFloatP(ctx, dty) && BGBCC_CCXL_TypeFloatP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeDoubleP(ctx, dty) && BGBCC_CCXL_TypeDoubleP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, dty) &&
			BGBCC_CCXL_TypePointerP(ctx, sty))
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeBaseSmallEqualP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int i, j;

	if(dty.val==sty.val)
		return(true);

	if(BGBCC_CCXL_TypeSmallIntP(ctx, dty) &&
			BGBCC_CCXL_TypeSmallIntP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeLongP(ctx, dty) && BGBCC_CCXL_TypeLongP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeFloatP(ctx, dty) && BGBCC_CCXL_TypeFloatP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypeDoubleP(ctx, dty) && BGBCC_CCXL_TypeDoubleP(ctx, sty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, dty) &&
			BGBCC_CCXL_TypePointerP(ctx, sty))
		return(true);
	
	if(BGBCC_CCXL_TypeValueObjectP(ctx, dty) &&
		BGBCC_CCXL_TypeValueObjectP(ctx, sty))
	{
		i=BGBCC_CCXL_GetTypeBaseType(ctx, dty);
		j=BGBCC_CCXL_GetTypeBaseType(ctx, sty);
		
		if(i==j)
			return(true);
		return(false);
	}
	
	return(false);
}

bool BGBCC_CCXL_TypeOprBasicP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=16)
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_S)
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_V)
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNDEF_I)
		return(false);

	return(true);
}

int BGBCC_CCXL_TypeObjectLiteralIndex(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)return(-1);
	return(i-256);
}

bool BGBCC_CCXL_TypeFunctionP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i>=256)
	{
		obj=ctx->literals[i-256];
		if(obj->littype==FR2C_LITID_FUNCTION)
			return(true);
		return(false);
	}
	return(false);
}

bool BGBCC_CCXL_TypeValueObjectP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<16)
		return(false);
	
	if(i>=256)
	{
		obj=ctx->literals[i-256];
		if(obj->littype==FR2C_LITID_STRUCT)
			return(true);
		if(obj->littype==FR2C_LITID_UNION)
			return(true);
		if(obj->littype==FR2C_LITID_CLASS)
			return(true);
		return(false);
	}
	
	switch(i)
	{
	case FR2C_TY_I128:
	case FR2C_TY_UI128:
	case FR2C_TY_F128:
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeIsObjectP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

//	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
//		return(false);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<16)
		return(false);
	
	if(i>=256)
	{
		obj=ctx->literals[i-256];
		if(obj->littype==FR2C_LITID_STRUCT)
			return(true);
		if(obj->littype==FR2C_LITID_UNION)
			return(true);
		if(obj->littype==FR2C_LITID_CLASS)
			return(true);
		return(false);
	}
	
	switch(i)
	{
	case FR2C_TY_I128:
	case FR2C_TY_UI128:
	case FR2C_TY_F128:
		return(true);
	}
	return(false);
}

int BGBCC_CCXL_TypeAsOprBasic(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(FR2C_TY_P);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=16)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_S)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_V)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNDEF_I)
		return(-1);

	return(BGBCC_CCXL_GetTypeBaseType(ctx, ty));
}

int BGBCC_CCXL_TypeAsOprExtBasic(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(FR2C_TY_P);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=256)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_S)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_V)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==FR2C_TY_UNDEF_I)
		return(-1);

	return(BGBCC_CCXL_GetTypeBaseType(ctx, ty));
}

int BGBCC_CCXL_TypeGetLogicalBaseSize(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz;

	switch(BGBCC_CCXL_GetTypeBaseType(ctx, ty))
	{
	case FR2C_TY_I: sz=4; break;
	case FR2C_TY_L: sz=8; break;
	case FR2C_TY_F: sz=4; break;
	case FR2C_TY_D: sz=8; break;

	case FR2C_TY_SB: sz=1; break;
	case FR2C_TY_UB: sz=1; break;
	case FR2C_TY_SS: sz=2; break;
	case FR2C_TY_US: sz=2; break;
	case FR2C_TY_UI: sz=4; break;
	case FR2C_TY_UL: sz=8; break;
	
	default:
		sz=-1; break;
	}
	
	return(sz);
}

int BGBCC_CCXL_TypeGetLogicalBaseShift(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz;
	sz=BGBCC_CCXL_TypeGetLogicalBaseSize(ctx, ty);
	
	if(sz<0)return(sz);
	if(sz<1)return(-1);
	if(sz==1)return(0);
	if(sz==2)return(1);
	if(sz==4)return(2);
	if(sz==8)return(3);
	if(sz==16)return(4);
	return(-1);
}

ccxl_status BGBCC_CCXL_TypeDerefType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i, j, k;

//	if(!BGBCC_CCXL_TypePointerP(ctx, ty))

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{	
			tty.val=sty.val&(FR2C_TY_BASEMASK|FR2C_TY_PTRMASK);
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		if(sty.val&FR2C_TY_PTRMASK)
		{
			tty.val=sty.val-FR2C_TY_PTRIDX1;
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{	
			tty.val=sty.val&(FR2C_TYB2_BASEMASK|FR2C_TYB2_PTRMASK);
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		if(sty.val&FR2C_TYB2_PTRMASK)
		{
			tty.val=sty.val-FR2C_TYB2_PTRIDX1;
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&FR2C_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
			for(i=0; i<15; i++)
				ovf.asz[i]=ovf.asz[i+1];
			ovf.asz[15]=0;
			ovf.an--;
		}else if(ovf.an)
		{
			ovf.an=0;
		}else if(ovf.pn)
		{
			ovf.pn--;
		}else
		{
			BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
				sty.val);
			*rdty=sty;
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}
	
		i=BGBCC_CCXL_TypeFromOverflow(ctx, &tty, ovf);
		*rdty=tty;
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypePointerType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i, j, k;

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			tty.val=sty.val&(FR2C_TY_BASEMASK|FR2C_TY_PTRMASK);
			tty.val=tty.val+FR2C_TY_PTRIDX1;
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		tty.val=sty.val+FR2C_TY_PTRIDX1;
		*rdty=tty;
		return(FR2C_STATUS_YES);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			tty.val=sty.val&(FR2C_TYB2_BASEMASK|FR2C_TYB2_PTRMASK);
			tty.val|=FR2C_TY_TYTY_BASIC2;
			tty.val=tty.val+FR2C_TYB2_PTRIDX1;
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		tty.val=sty.val+FR2C_TYB2_PTRIDX1;
		*rdty=tty;
		return(FR2C_STATUS_YES);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&FR2C_TYOVF_IDXMASK]);
		if(ovf.an)
		{
			ovf.an=0;
			ovf.pn++;
		}else
		{
			ovf.pn++;
		}
	
		i=BGBCC_CCXL_TypeFromOverflow(ctx, &tty, ovf);
		*rdty=tty;
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypeAutoPromoteType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	ccxl_type tty;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, sty) &&
		!BGBCC_CCXL_TypeIntP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(FR2C_TY_I);
		if(rdty)*rdty=tty;
		return(FR2C_STATUS_YES);
	}

#if 0
	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty) &&
		!BGBCC_CCXL_TypeLongP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(FR2C_TY_L);
		*rdty=ty;
		return(FR2C_STATUS_YES);
	}
#endif

	if(rdty)*rdty=sty;
	return(FR2C_STATUS_NO);
}

int BGBCC_CCXL_TypeGetArraySize(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	int i, j, k;

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		i=(sty.val&FR2C_TY_ARRMASK)>>FR2C_TY_ARRSHL;
		return(i);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		i=(sty.val&FR2C_TYB2_ARRMASK)>>FR2C_TYB2_ARRSHL;
		return(i);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&FR2C_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
			i=1;
			for(j=0; j<ovf.an; j++)
				i=i*ovf.asz[j];
		}else if(ovf.an)
		{
			i=ovf.asz[0];
		}else
		{
			i=0;
		}
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(0);
}

int BGBCC_CCXL_TypeArrayAdjustSize(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty,
	int newsz)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i;

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		if((newsz>0) && (newsz<FR2C_TY_BASEARRMAX))
		{
			tty.val=(sty.val)&(~FR2C_TY_ARRMASK);
			tty.val=tty.val|(newsz<<FR2C_TY_ARRSHL);
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(FR2C_STATUS_YES);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		if((newsz>0) && (newsz<FR2C_TYB2_BASEARRMAX))
		{
			tty.val=(sty.val)&(~FR2C_TYB2_ARRMASK);
			tty.val=tty.val|(newsz<<FR2C_TYB2_ARRSHL);
			*rdty=tty;
			return(FR2C_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(FR2C_STATUS_YES);
	}

	if((sty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&FR2C_TYOVF_IDXMASK]);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(FR2C_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(FR2C_STATUS_NO);
}

int BGBCC_CCXL_TypeUnpackOverflow(
	BGBCC_TransState *ctx,
	ccxl_type ty,
	BGBCC_CCXL_TypeOverflow *rovf)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i;

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&FR2C_TY_BASEMASK;
		ovf.pn=(ty.val&FR2C_TY_PTRMASK)>>FR2C_TY_PTRSHL;
		ovf.asz[0]=(ty.val&FR2C_TY_ARRMASK)>>FR2C_TY_ARRSHL;
		ovf.an=ovf.asz?1:0;
		*rovf=ovf;
		return(FR2C_STATUS_YES);
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&FR2C_TYB2_BASEMASK;
		ovf.pn=(ty.val&FR2C_TYB2_PTRMASK)>>FR2C_TYB2_PTRSHL;
		ovf.asz[0]=(ty.val&FR2C_TYB2_ARRMASK)>>FR2C_TYB2_ARRSHL;
		ovf.an=ovf.asz?1:0;
		*rovf=ovf;
		return(FR2C_STATUS_YES);
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[ty.val&FR2C_TYOVF_IDXMASK]);
		*rovf=ovf;
		return(FR2C_STATUS_YES);
	}
	
	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
//	*rovf=ovf;
	return(FR2C_STATUS_NO);
}

int BGBCC_CCXL_TypeIndexOverflow(
	BGBCC_TransState *ctx,
	BGBCC_CCXL_TypeOverflow ovf)
{
	int i, j, k;
	
	for(i=0; i<ctx->n_tyovf; i++)
	{
		if(!memcmp(ctx->tyovf[i], &(ovf),
			sizeof(BGBCC_CCXL_TypeOverflow)))
				return(i);
	}
	
	i=ctx->n_tyovf++;
	ctx->tyovf[i]=bgbcc_malloc(sizeof(BGBCC_CCXL_TypeOverflow));
	memcpy(ctx->tyovf[i], &(ovf), sizeof(BGBCC_CCXL_TypeOverflow));
	return(i);
}

ccxl_status BGBCC_CCXL_TypeFromOverflow(
	BGBCC_TransState *ctx,
	ccxl_type *rty,
	BGBCC_CCXL_TypeOverflow ovf)
{
	ccxl_type tty;
	int i, j, k;

	if((ovf.base>=0) && (ovf.base<FR2C_TY_BASETYMAX) && (ovf.an==0))
	{
		tty.val=ovf.base|(ovf.pn<<FR2C_TY_PTRSHL)|
			FR2C_TY_TYTY_BASIC;
		*rty=tty;
		return(FR2C_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<FR2C_TY_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) &&
		(ovf.asz[0]<FR2C_TY_BASEARRMAX))
	{
		tty.val=ovf.base|(ovf.pn<<FR2C_TY_PTRSHL)|
			(ovf.asz[0]<<FR2C_TY_ARRSHL)|
			FR2C_TY_TYTY_BASIC;
		*rty=tty;
		return(FR2C_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<FR2C_TYB2_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) &&
		(ovf.asz[0]<FR2C_TYB2_BASEARRMAX))
	{
		tty.val=ovf.base|(ovf.pn<<FR2C_TYB2_PTRSHL)|
			(ovf.asz[0]<<FR2C_TYB2_ARRSHL)|
			FR2C_TY_TYTY_BASIC2;
		*rty=tty;
		return(FR2C_STATUS_YES);
	}

	i=BGBCC_CCXL_TypeIndexOverflow(ctx, ovf);
	if(i>=0)
	{
		tty.val=i|FR2C_TY_TYTY_OVF1;
		*rty=tty;
		return(FR2C_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
//	*rdty=sty;
	return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypeFromSig(
	BGBCC_TransState *ctx,
	ccxl_type *rty, char *sig)
{
	int asz[16];
	BGBCC_CCXL_TypeOverflow ovf;
	BGBCC_CCXL_LiteralInfo *st;
	ccxl_type tty;
	char *s;
	int an, pn, bty;
	int i, j, k;
	
	s=sig; an=0; pn=0; bty=-1;
	
	if((*s=='A') && (s[1]>='0') && (s[1]<='9'))
	{
		s++; i=0;
		while(*s)
		{
			if(*s==';')
			{
				asz[an++]=i;
				s++; break;
			}
			if(*s==',')
			{
				asz[an++]=i;
				i=0; s++;
				continue;
			}
			if((*s>='0') && (*s<='9'))
			{
				i=(i*10)+((*s++)-'0');
				continue;
			}
			asz[an++]=i;
			break;
		}
	}
	
	while(*s=='P')
		{ pn++; s++; }
	switch(*s)
	{
	case 'a': bty=FR2C_TY_SB; break;
	case 'b': bty=FR2C_TY_SB; break;
	case 'c': bty=FR2C_TY_SB; break;
	case 'd': bty=FR2C_TY_D; break;
	case 'e': bty=FR2C_TY_D; break;
	case 'f': bty=FR2C_TY_F; break;
	case 'g': bty=FR2C_TY_F128; break;
	case 'h': bty=FR2C_TY_UB; break;
	case 'i': bty=FR2C_TY_I; break;
	case 'j': bty=FR2C_TY_UI; break;
	case 'k': bty=FR2C_TY_F16; break;
	case 'l': bty=FR2C_TY_NL; break;
	case 'm': bty=FR2C_TY_UNL; break;
	case 'n': bty=FR2C_TY_I128; break;
	case 'o': bty=FR2C_TY_UI128; break;
	case 'p': bty=-1; break;
	case 'q': bty=-1; break;
	case 'r': bty=FR2C_TY_VARIANT; break;
	case 's': bty=FR2C_TY_SS; break;
	case 't': bty=FR2C_TY_US; break;
	case 'u': bty=-1; break;
	case 'v': bty=FR2C_TY_V; break;
	case 'w': bty=FR2C_TY_US; break;
	case 'x': bty=FR2C_TY_L; break;
	case 'y': bty=FR2C_TY_UL; break;
	case 'z': bty=FR2C_TY_VARARGS; break;
	case 'X': case 'Y':
		st=BGBCC_CCXL_LookupStructureForSig(ctx, s);
		if(st)
			{ bty=st->litid+256; }
		else
			{ bty=-1; }
		break;
	default:  bty=-1; break;
	}
	
	if((bty>=0) && (bty<FR2C_TY_BASETYMAX) && (an==0))
	{
		tty.val=bty|(pn<<FR2C_TY_PTRSHL)|
			FR2C_TY_TYTY_BASIC;
		*rty=tty;
		return(FR2C_STATUS_YES);
	}

	if((bty>=0) && (bty<FR2C_TY_BASETYMAX) &&
		(an==1) && (asz[0]>0) &&
		(asz[0]<FR2C_TY_BASEARRMAX))
	{
		tty.val=bty|(pn<<FR2C_TY_PTRSHL)|
			(asz[0]<<FR2C_TY_ARRSHL)|
			FR2C_TY_TYTY_BASIC;
		*rty=tty;
		return(FR2C_STATUS_YES);
	}

	if((bty>=0) && (bty<FR2C_TYB2_BASETYMAX) &&
		(an==1) && (asz[0]>0) &&
		(asz[0]<FR2C_TYB2_BASEARRMAX))
	{
		tty.val=bty|(pn<<FR2C_TYB2_PTRSHL)|
			(asz[0]<<FR2C_TYB2_ARRSHL)|
			FR2C_TY_TYTY_BASIC2;
		*rty=tty;
		return(FR2C_STATUS_YES);
	}

	if(bty>=0)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=bty;
		ovf.pn=pn;
		ovf.an=an;
		for(i=0; i<16; i++)
			ovf.asz[i]=0;
		for(i=0; i<an; i++)
			ovf.asz[i]=asz[i];

		i=BGBCC_CCXL_TypeIndexOverflow(ctx, ovf);
		if(i>=0)
		{
			tty.val=i|FR2C_TY_TYTY_OVF1;
			*rty=tty;
			return(FR2C_STATUS_YES);
		}
	}

	if(*s=='(')
	{
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);

	tty.val=0;
	*rty=tty;
	return(FR2C_STATUS_NO);
}

char *BGBCC_CCXL_TypeGetSig(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	char tb[256];
	int bsz[16];
	BGBCC_CCXL_LiteralInfo *st;
	char *s, *t;
	int pn, an, asz, bt;
	int i, j, k, l;

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		pn=(ty.val&FR2C_TY_PTRMASK)>>FR2C_TY_PTRSHL;
		asz=(ty.val&FR2C_TY_ARRMASK)>>FR2C_TY_ARRSHL;
		bt=ty.val&FR2C_TY_BASEMASK;
		an=asz?1:0;
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC2)
	{
		pn=(ty.val&FR2C_TYB2_PTRMASK)>>FR2C_TYB2_PTRSHL;
		asz=(ty.val&FR2C_TYB2_ARRMASK)>>FR2C_TYB2_ARRSHL;
		bt=ty.val&FR2C_TYB2_BASEMASK;
		an=asz?1:0;
	}

	if((ty.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		i=(ty.val&FR2C_TYOVF_IDXMASK);
		bt=ctx->tyovf[i]->base;
		pn=ctx->tyovf[i]->pn;
		an=ctx->tyovf[i]->an;
		
		asz=1;
		for(j=0; j<an; j++)
		{
			k=ctx->tyovf[i]->asz[j];
			bsz[j]=k;
			asz=asz*k;
		}
	}
		
	t=tb;
	
	if(an>1)
	{
		sprintf(t, "A%d", bsz[0]);
		t+=strlen(t);
		for(i=1; i<an; i++)
		{
			sprintf(t, ",%d", bsz[i]);
			t+=strlen(t);
		}
		*t++=';';
		*t=0;
	}else if(an==1)
	{
//		sprintf(t, "A%d;", asz);
		sprintf(t, "A%d", asz);
		t+=strlen(t);
	}

	if(pn)
	{
		i=pn;
		while(i--)*t++='P';
		*t=0;
	}
		
	if(bt<256)
	{
		switch(bt)
		{
		case FR2C_TY_I: *t++='i'; break;
		case FR2C_TY_L: *t++='x'; break;
		case FR2C_TY_F: *t++='f'; break;
		case FR2C_TY_D: *t++='d'; break;
		case FR2C_TY_P:
			*t++='P'; *t++='v'; break;
		case FR2C_TY_V: *t++='v'; break;
		case FR2C_TY_NL: *t++='l'; break;
		case FR2C_TY_SB: *t++='c'; break;
		case FR2C_TY_UB: *t++='h'; break;
		case FR2C_TY_SS: *t++='s'; break;
		case FR2C_TY_US: *t++='t'; break;
		case FR2C_TY_UI: *t++='j'; break;
		case FR2C_TY_UL: *t++='y'; break;
		case FR2C_TY_UNL: *t++='m'; break;

		case FR2C_TY_S: *t++='i'; break;
		default:
			break;
		}
		*t++=0;
	}else
	{
//		st=ctx->literals[bt-256];
		sprintf(t, "X%d", bt-256);
	}
	
	return(bgbcc_strdup(tb));
}

ccxl_type BGBCC_CCXL_TypeWrapBasicType(int ty)
{
	ccxl_type tty;
	tty.val=ty;
	return(tty);
}


int BGBCC_CCXL_GetTypeConvMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
//	if(BGBCC_CCXL_TypeIntP(ctx, sty))
	if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeLongP(ctx, dty))
			{ return(FR2C_OPCONV_I2L); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(FR2C_OPCONV_I2F); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(FR2C_OPCONV_I2D); }
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
			{ return(FR2C_OPCONV_I2P); }

//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeIntP(ctx, dty))
			{ return(FR2C_OPCONV_L2I); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(FR2C_OPCONV_L2F); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(FR2C_OPCONV_L2D); }
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
			{ return(FR2C_OPCONV_L2P); }

		if(BGBCC_CCXL_TypeUnsignedIntP(ctx, dty))
			{ return(FR2C_OPCONV_L2I); }

//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(FR2C_OPCONV_F2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(FR2C_OPCONV_F2L); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(FR2C_OPCONV_F2D); }

//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(FR2C_OPCONV_D2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(FR2C_OPCONV_D2L); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(FR2C_OPCONV_D2F); }

//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(FR2C_OPCONV_P2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(FR2C_OPCONV_P2L); }

//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

//	BGBCC_CCXL_TagError(ctx,
//		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
}

int BGBCC_CCXL_GetTypeConvIMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

	if(!BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}
	if(!BGBCC_CCXL_TypeSmallIntP(ctx, sty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}
	return(BGBCC_CCXL_GetTypeConvILMode(ctx, dty, sty));
}		

int BGBCC_CCXL_GetTypeConvLMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

	if(!BGBCC_CCXL_TypeSmallLongP(ctx, dty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}
	if(!BGBCC_CCXL_TypeSmallLongP(ctx, sty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}
	return(BGBCC_CCXL_GetTypeConvILMode(ctx, dty, sty));
}		

int BGBCC_CCXL_GetTypeConvILMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

//	if(!BGBCC_CCXL_TypeSmallIntP(ctx, dty))
//		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
//	if(!BGBCC_CCXL_TypeSmallIntP(ctx, sty))
//		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		
	dt=dty.val; st=sty.val;
	
	if(dt==FR2C_TY_I)
	{
		if(st==FR2C_TY_SB)
			{ return(FR2C_OPCONV2_SXB); }
		if(st==FR2C_TY_UB)
			{ return(FR2C_OPCONV2_ZXB); }
		if(st==FR2C_TY_SS)
			{ return(FR2C_OPCONV2_SXS); }
		if(st==FR2C_TY_US)
			{ return(FR2C_OPCONV2_ZXS); }
		if(st==FR2C_TY_I)
			{ return(FR2C_OPCONV2_SXI); }
		if(st==FR2C_TY_UI)
			{ return(FR2C_OPCONV2_ZXI); }

		if(st==FR2C_TY_L)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_NL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UNL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if(dt==FR2C_TY_UI)
	{
		if(st==FR2C_TY_SB)
			{ return(FR2C_OPCONV2_ZXB); }
		if(st==FR2C_TY_SS)
			{ return(FR2C_OPCONV2_ZXS); }
		if(st==FR2C_TY_I)
			{ return(FR2C_OPCONV2_ZXI); }

		if(st==FR2C_TY_UB)
			{ return(FR2C_OPCONV2_ZXB); }
		if(st==FR2C_TY_US)
			{ return(FR2C_OPCONV2_ZXS); }
		if(st==FR2C_TY_UI)
			{ return(FR2C_OPCONV2_ZXI); }

		if(st==FR2C_TY_L)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_NL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UNL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if((dt==FR2C_TY_L) || (dt==FR2C_TY_NL))
	{
		if(st==FR2C_TY_SB)
			{ return(FR2C_OPCONV2_SXB); }
		if(st==FR2C_TY_UB)
			{ return(FR2C_OPCONV2_ZXB); }
		if(st==FR2C_TY_SS)
			{ return(FR2C_OPCONV2_SXS); }
		if(st==FR2C_TY_US)
			{ return(FR2C_OPCONV2_ZXS); }
		if(st==FR2C_TY_I)
			{ return(FR2C_OPCONV2_SXI); }
		if(st==FR2C_TY_UI)
			{ return(FR2C_OPCONV2_ZXI); }

		if(st==FR2C_TY_L)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_NL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UNL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if((dt==FR2C_TY_UL) || (dt==FR2C_TY_UNL))
	{
		if(st==FR2C_TY_SB)
			{ return(FR2C_OPCONV2_ZXB); }
		if(st==FR2C_TY_UB)
			{ return(FR2C_OPCONV2_ZXB); }
		if(st==FR2C_TY_SS)
			{ return(FR2C_OPCONV2_ZXS); }
		if(st==FR2C_TY_US)
			{ return(FR2C_OPCONV2_ZXS); }
		if(st==FR2C_TY_I)
			{ return(FR2C_OPCONV2_ZXI); }
		if(st==FR2C_TY_UI)
			{ return(FR2C_OPCONV2_ZXI); }

		if(st==FR2C_TY_L)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_NL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==FR2C_TY_UNL)
			{ return(FR2C_STATUS_ERR_UNHANDLEDTYPE); }
	}
	
	if(dt==FR2C_TY_SB)
		{ return(FR2C_OPCONV2_SXB); }
	if(dt==FR2C_TY_UB)
		{ return(FR2C_OPCONV2_ZXB); }

	if(dt==FR2C_TY_SS)
	{
		if(st==FR2C_TY_SB)
			{ return(FR2C_OPCONV2_SXB); }
		if(st==FR2C_TY_UB)
			{ return(FR2C_OPCONV2_ZXB); }
		return(FR2C_OPCONV2_SXS);
	}

	if(dt==FR2C_TY_US)
	{
		if(st==FR2C_TY_SB)
			{ return(FR2C_OPCONV2_ZXS); }
		if(st==FR2C_TY_UB)
			{ return(FR2C_OPCONV2_ZXB); }
		return(FR2C_OPCONV2_ZXS);
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
}

bool BGBCC_CCXL_TypeSupportsOperatorP(
	BGBCC_TransState *ctx, ccxl_type ty, int opr)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeLongP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, ty))
	{
		if(opr==FR2C_BINOP_ADD)
			return(true);
		if(opr==FR2C_BINOP_SUB)
			return(true);
		if(opr==FR2C_BINOP_MUL)
			return(true);
		if(opr==FR2C_BINOP_DIV)
			return(true);
		return(false);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
	{
		if(opr==FR2C_BINOP_ADD)
			return(true);
		if(opr==FR2C_BINOP_SUB)
			return(true);
		if(opr==FR2C_BINOP_MUL)
			return(true);
		if(opr==FR2C_BINOP_DIV)
			return(true);
		return(false);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
	{
		if(opr==FR2C_BINOP_ADD)
			return(true);
		if(opr==FR2C_BINOP_SUB)
			return(true);
		return(false);
	}
	
	return(false);
}

ccxl_status BGBCC_CCXL_GetTypeBinaryDest(
	BGBCC_TransState *ctx, int opr,
	ccxl_type lty, ccxl_type rty,
	ccxl_type *rdty)
{
	int ltyb, rtyb, dtyb;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallIntP(ctx, rty))
	{
//		ltyb=lty.val&FR2C_TY_BASEMASK;
//		rtyb=rty.val&FR2C_TY_BASEMASK;
		ltyb=BGBCC_CCXL_GetTypeBaseType(ctx, lty);
		rtyb=BGBCC_CCXL_GetTypeBaseType(ctx, rty);
		dtyb=FR2C_TY_I;
		
		switch(ltyb)
		{
		case FR2C_TY_I:
			switch(rtyb)
			{
			case FR2C_TY_I:		dtyb=FR2C_TY_I; break;
			case FR2C_TY_UI:	dtyb=FR2C_TY_UI; break;
			default:			dtyb=FR2C_TY_I; break;
			}
			break;
		case FR2C_TY_UI:
			dtyb=FR2C_TY_UI; break;

		case FR2C_TY_SB:
			switch(rtyb)
			{
			case FR2C_TY_SB:	dtyb=FR2C_TY_SB; break;
			case FR2C_TY_UB:	dtyb=FR2C_TY_UB; break;
			case FR2C_TY_SS:	dtyb=FR2C_TY_SS; break;
			case FR2C_TY_US:	dtyb=FR2C_TY_US; break;
			case FR2C_TY_I:		dtyb=FR2C_TY_I; break;
			case FR2C_TY_UI:	dtyb=FR2C_TY_UI; break;
			default:			dtyb=FR2C_TY_I; break;
			}
			break;
		case FR2C_TY_UB:
			switch(rtyb)
			{
			case FR2C_TY_SB:	case FR2C_TY_UB:
				dtyb=FR2C_TY_UB; break;
			case FR2C_TY_SS:	dtyb=FR2C_TY_SS; break;
			case FR2C_TY_US:	dtyb=FR2C_TY_US; break;
			case FR2C_TY_I:		dtyb=FR2C_TY_I; break;
			case FR2C_TY_UI:	dtyb=FR2C_TY_UI; break;
			default:			dtyb=FR2C_TY_I; break;
			}
			break;

		case FR2C_TY_SS:
			switch(rtyb)
			{
			case FR2C_TY_SB: case FR2C_TY_UB:
			case FR2C_TY_SS:
				dtyb=FR2C_TY_SS; break;
			case FR2C_TY_US:	dtyb=FR2C_TY_US; break;
			case FR2C_TY_I:		dtyb=FR2C_TY_I; break;
			case FR2C_TY_UI:	dtyb=FR2C_TY_UI; break;
			default:			dtyb=FR2C_TY_I; break;
			}
			break;
		case FR2C_TY_US:
			switch(rtyb)
			{
			case FR2C_TY_SB:	case FR2C_TY_UB:
			case FR2C_TY_SS:	case FR2C_TY_US:
				dtyb=FR2C_TY_US; break;
			case FR2C_TY_I:		dtyb=FR2C_TY_I; break;
			case FR2C_TY_UI:	dtyb=FR2C_TY_UI; break;
			default:			dtyb=FR2C_TY_I; break;
			}
			break;
		default:			dtyb=FR2C_TY_I; break;
		}

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, dtyb);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			FR2C_STATUS_YES:FR2C_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallLongP(ctx, rty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		{
			*rdty=rty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
		}
		
		if(BGBCC_CCXL_TypeUnsignedLongP(ctx, lty))
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeUnsignedLongP(ctx, rty))
		{
			*rdty=rty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
		}

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_L);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			FR2C_STATUS_YES:FR2C_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_I);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_L);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, rty))
//		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_L);
//			return(FR2C_STATUS_YES);
//		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		BGBCC_CCXL_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_L);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_L);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, dty))
//			{ return(FR2C_OPCONV_L2P); }
		BGBCC_CCXL_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		BGBCC_CCXL_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		BGBCC_CCXL_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
		((opr==FR2C_BINOP_ADD) || (opr==FR2C_BINOP_SUB)))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=lty;
			if(BGBCC_CCXL_TypeArrayP(ctx, lty))
				{ BGBCC_CCXL_TypePointerType(ctx, lty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=lty;
			if(BGBCC_CCXL_TypeArrayP(ctx, lty))
				{ BGBCC_CCXL_TypePointerType(ctx, lty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			if(opr==FR2C_BINOP_SUB)
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, FR2C_TY_I);
				return(FR2C_STATUS_YES);
			}
		}

		BGBCC_CCXL_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
		return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty) &&
		(opr==FR2C_BINOP_ADD))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, lty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				FR2C_STATUS_YES:FR2C_STATUS_NO);
//			return(FR2C_STATUS_YES);
		}
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_GetTypeCompareBinaryDest(
	BGBCC_TransState *ctx, int opr,
	ccxl_type lty, ccxl_type rty,
	ccxl_type *rdty)
{
	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
		BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
	{
		*rdty=lty;
		if(BGBCC_CCXL_TypeArrayP(ctx, lty))
			{ BGBCC_CCXL_TypePointerType(ctx, lty, rdty); }
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, lty) &&
		BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, rty))
	{
		return(BGBCC_CCXL_GetTypeBinaryDest(ctx,
			FR2C_BINOP_ADD, lty, rty, rdty));
	}

	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
}
