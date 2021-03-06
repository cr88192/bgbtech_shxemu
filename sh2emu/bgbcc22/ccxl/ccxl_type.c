#include <bgbccc.h>

bool BGBCC_CCXL_TypeSmallTypeP(
	BGBCC_TransState *ctx, ccxl_type dty, ccxl_type sty)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeSmallFloatP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallFloatP(ctx, sty))
		{
			return(true);
		}

		if(BGBCC_CCXL_TypeSmallDoubleP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}

	if(BGBCC_CCXL_TypeSmallDoubleP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallFloatP(ctx, sty))
		{
			return(true);
		}

		if(BGBCC_CCXL_TypeSmallDoubleP(ctx, sty))
		{
			return(true);
		}
		return(false);
	}
	
	return(false);
}

bool BGBCC_CCXL_TypeSmallIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(ctx->arch_sizeof_long==4)
	{
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
			return(true);
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
			return(true);
	}

	return(false);
}

bool BGBCC_CCXL_TypeSmallLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallInt128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
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

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}

	return(false);
}

bool BGBCC_CCXL_TypeIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
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

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
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

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgIntP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgNLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgLongP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSgInt128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_INT128;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeFloatP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
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

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeFloat128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeFloat16P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeRealP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		return(true);
	}

	return(false);
}

bool BGBCC_CCXL_TypeVariantP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_VARIANT)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_VARIANT;
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeSmallFloatP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallDoubleP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
		return(true);
	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallFloat128P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F128)
	{
		ctx->ccxl_tyc_seen|=BGBCC_TYCSEEN_FLOAT128;
		return(true);
	}

	if(BGBCC_CCXL_TypeSmallDoubleP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeSmallInt128P(ctx, ty))
		return(true);

	return(false);
}

bool BGBCC_CCXL_TypeSmallVariantP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	return(true);
}

bool BGBCC_CCXL_TypeBaseILFD_P(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
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

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_D)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_F16)
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

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_I)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_L)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UI)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_NL)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNL)
		return(true);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);

	return(false);
}

int BGBCC_CCXL_GetTypeBaseType(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
		{ return(ty.val&CCXL_TY_BASEMASK); }

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
		{ return(ty.val&CCXL_TYB2_BASEMASK); }

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
		{ return(ty.val&CCXL_TYB3_BASEMASK); }

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		i=ty.val&CCXL_TYOVF_IDXMASK;
		if((i>ctx->n_tyovf) || !ctx->tyovf[i])
			{ BGBCC_DBGBREAK }
		return(ctx->tyovf[i]->base);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(-1);
}

bool BGBCC_CCXL_TypePointerP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_P)
			return(true);
		if(ty.val&CCXL_TY_PTRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if((ty.val&CCXL_TYB2_BASEMASK)==CCXL_TY_P)
			return(true);
		if(ty.val&CCXL_TYB2_PTRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if((ty.val&CCXL_TYB3_BASEMASK)==CCXL_TY_P)
			return(true);
		if(ty.val&CCXL_TYB3_PTRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]->pn!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(false);
}

bool BGBCC_CCXL_TypeArrayP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(ty.val&CCXL_TY_ARRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if(ty.val&CCXL_TYB2_ARRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if(ty.val&CCXL_TYB3_ARRMASK)
			return(true);
		return(false);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		return(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]->an!=0);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
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
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SB)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedByteP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UB)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeSignedShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_SS)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeUnsignedShortP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_US)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVoidP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVarArgsP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_VARARGS)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeVaListP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_VALIST)
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

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_S)
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(false);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNDEF_I)
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
		if(obj && obj->littype==CCXL_LITID_FUNCTION)
			return(true);
		if(obj && obj->littype==CCXL_LITID_PROTOTYPE)
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
		if(!obj)
			return(false);
		if(obj->littype==CCXL_LITID_STRUCT)
			return(true);
		if(obj->littype==CCXL_LITID_UNION)
			return(true);
		if(obj->littype==CCXL_LITID_CLASS)
			return(true);
		return(false);
	}
	
	switch(i)
	{
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:
	case CCXL_TY_VALIST:
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
		if(!obj)
			return(false);
		if(obj->littype==CCXL_LITID_STRUCT)
			return(true);
		if(obj->littype==CCXL_LITID_UNION)
			return(true);
		if(obj->littype==CCXL_LITID_CLASS)
			return(true);
		return(false);
	}
	
	switch(i)
	{
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:
		return(true);
	}
	return(false);
}

bool BGBCC_CCXL_TypeIsTypedefP(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
		return(false);
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
		return(false);

	obj=ctx->literals[i-256];
	if(!obj)
		return(false);
	if(obj->littype==CCXL_LITID_TYPEDEF)
		return(true);
	return(false);
}

bool BGBCC_CCXL_TypeGetTypedefType(
	BGBCC_TransState *ctx, ccxl_type ty, ccxl_type *rty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	int i;

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, ty))
	{
		*rty=ty;
		return(false);
	}
	
	i=BGBCC_CCXL_GetTypeBaseType(ctx, ty);
	if(i<256)
	{
		*rty=ty;
		return(false);
	}

	obj=ctx->literals[i-256];
	if(!obj)
	{
		*rty=ty;
		return(false);
	}

	if(obj->littype==CCXL_LITID_TYPEDEF)
	{
		*rty=obj->decl->type;
		return(true);
	}
	
	*rty=ty;
	return(false);
}

int BGBCC_CCXL_TypeAsOprBasic(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(CCXL_TY_P);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=16)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_S)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNDEF_I)
		return(-1);

	return(BGBCC_CCXL_GetTypeBaseType(ctx, ty));
}

int BGBCC_CCXL_TypeAsOprExtBasic(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(CCXL_TY_P);

	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)>=256)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_S)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_V)
		return(-1);
	if(BGBCC_CCXL_GetTypeBaseType(ctx, ty)==CCXL_TY_UNDEF_I)
		return(-1);

	return(BGBCC_CCXL_GetTypeBaseType(ctx, ty));
}

int BGBCC_CCXL_TypeGetLogicalBaseSize(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz;

	switch(BGBCC_CCXL_GetTypeBaseType(ctx, ty))
	{
	case CCXL_TY_I: sz=4; break;
	case CCXL_TY_L: sz=8; break;
	case CCXL_TY_F: sz=4; break;
	case CCXL_TY_D: sz=8; break;

	case CCXL_TY_SB: sz=1; break;
	case CCXL_TY_UB: sz=1; break;
	case CCXL_TY_SS: sz=2; break;
	case CCXL_TY_US: sz=2; break;
	case CCXL_TY_UI: sz=4; break;
	case CCXL_TY_UL: sz=8; break;

	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=ctx->arch_sizeof_long;
		if(!sz)
			{ sz=-1; }
		break;
	
	case CCXL_TY_F16:
		sz=2; break;
	
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:
		sz=16; break;

	case CCXL_TY_VARIANT:
		sz=8; break;
	case CCXL_TY_VALIST:
//		sz=64; break;
		sz=ctx->arch_sizeof_valist;
		if(!sz)
			{ sz=-1; }
		break;

	default:
		sz=-1; break;
	}
	
	if(ctx->arch_sizeof_ptr)
	{
		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(ctx->arch_sizeof_ptr);
	}
	
	return(sz);
}

int BGBCC_CCXL_TypeGetLogicalBaseAlign(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz, nsz;

	switch(BGBCC_CCXL_GetTypeBaseType(ctx, ty))
	{
	case CCXL_TY_I: sz=4; break;
//	case CCXL_TY_L: sz=8; break;
	case CCXL_TY_F: sz=4; break;
//	case CCXL_TY_D: sz=8; break;

	case CCXL_TY_L:
	case CCXL_TY_D:
	case CCXL_TY_UL:
		sz=8; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_SB: sz=1; break;
	case CCXL_TY_UB: sz=1; break;
	case CCXL_TY_SS: sz=2; break;
	case CCXL_TY_US: sz=2; break;
	case CCXL_TY_UI: sz=4; break;
//	case CCXL_TY_UL: sz=8; break;

	case CCXL_TY_NL:	case CCXL_TY_UNL:
		sz=ctx->arch_sizeof_long;
		if(!sz)
			{ sz=-1; }
		break;
	
	case CCXL_TY_F16:
		sz=2; break;
	
	case CCXL_TY_I128:
	case CCXL_TY_UI128:
	case CCXL_TY_F128:
		sz=16; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;

	case CCXL_TY_VARIANT:
		sz=8; nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		break;
	case CCXL_TY_VALIST:
		sz=ctx->arch_sizeof_valist;
		nsz=ctx->arch_align_max;
		if(nsz && (sz>nsz))
			sz=nsz;
		if(!sz)
			{ sz=-1; }
		break;

	default:
		sz=-1; break;
	}
	
	if(ctx->arch_sizeof_ptr)
	{
		if(BGBCC_CCXL_TypePointerP(ctx, ty))
			return(ctx->arch_sizeof_ptr);
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
	if(sz==32)return(5);
	if(sz==64)return(6);
	if(sz==128)return(7);
	if(sz==256)return(8);
	return(-1);
}

int BGBCC_CCXL_TypeGetLogicalSize(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_type tty;
	int sz, asz, sz1;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		obj=BGBCC_CCXL_LookupStructureForType(ctx, ty);
		if(obj && obj->decl && obj->decl->fxmsize &&
			(obj->decl->fxmsize==obj->decl->fxnsize))
		{
			return(obj->decl->fxmsize);
		}
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, ty, &tty);
		sz1=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);
//		asz=BGBCC_CCXL_TypeGetArraySize(ctx, ty);
		asz=BGBCC_CCXL_TypeGetArrayDimSize(ctx, ty);
		sz=sz1*asz;
		if(sz>0)
			return(sz);
		return(-1);
	}

	sz=BGBCC_CCXL_TypeGetLogicalBaseSize(ctx, ty);
	if(sz>0)
		return(sz);

	return(-1);
}

int BGBCC_CCXL_TypeGetLogicalAlign(
	BGBCC_TransState *ctx, ccxl_type ty)
{
	BGBCC_CCXL_LiteralInfo *obj;
	ccxl_type tty;
	int sz, asz, sz1, al;

	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		obj=BGBCC_CCXL_LookupStructureForType(ctx, ty);
		if(obj && obj->decl && obj->decl->fxmalgn &&
			(obj->decl->fxmalgn==obj->decl->fxnalgn))
		{
			return(obj->decl->fxmalgn);
		}
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
	{
		BGBCC_CCXL_TypeDerefType(ctx, ty, &tty);
		al=BGBCC_CCXL_TypeGetLogicalAlign(ctx, tty);
		return(al);
	}

	al=BGBCC_CCXL_TypeGetLogicalBaseAlign(ctx, ty);
	return(al);
}

ccxl_status BGBCC_CCXL_TypeDerefType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int bt, pn;
	int i, j, k;

//	if(!BGBCC_CCXL_TypePointerP(ctx, ty))

	BGBCC_CCXL_TypeGetTypedefType(ctx, sty, &sty);

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{	
			tty.val=sty.val&(CCXL_TY_BASEMASK|CCXL_TY_PTRMASK);
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		if(sty.val&CCXL_TY_PTRMASK)
		{
			tty.val=sty.val-CCXL_TY_PTRIDX1;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{	
//			tty.val=sty.val&(CCXL_TYB2_BASEMASK|
//				CCXL_TYB2_PTRMASK|CCXL_TY_TYTY_MASK);
			bt=sty.val&CCXL_TYB2_BASEMASK;
			pn=(sty.val>>6)&3;
			tty.val=bt|(pn<<12)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		if(sty.val&CCXL_TYB2_PTRMASK)
		{
			tty.val=sty.val-CCXL_TYB2_PTRIDX1;
			if(!(tty.val&(CCXL_TYB2_ARRMASK|CCXL_TYB2_PTRMASK)))
				tty.val=(tty.val&CCXL_TYB2_BASEMASK)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{	
			tty.val=sty.val&(CCXL_TYB3_BASEMASK|
				CCXL_TYB3_PTRMASK|CCXL_TY_TYTY_MASK);

//			bt=sty.val&CCXL_TYB3_BASEMASK;
//			pn=(sty.val>>6)&3;
//			tty.val=bt|(pn<<12)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		if(sty.val&CCXL_TYB3_PTRMASK)
		{
			tty.val=sty.val-CCXL_TYB3_PTRIDX1;
//			if(!(tty.val&(CCXL_TYB3_ARRMASK|CCXL_TYB3_PTRMASK)))
//				tty.val=(tty.val&CCXL_TYB2_BASEMASK)|CCXL_TY_TYTY_BASIC;
			BGBCC_CCXL_TypeGetTypedefType(ctx, tty, &tty);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_Error(ctx, "Failed to dereference type %08X\n",
			sty.val);
		*rdty=sty;
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
			for(i=0; i<15; i++)
				ovf.asz[i]=ovf.asz[i+1];
			ovf.asz[15]=0;
			ovf.an--;
//			ovf.asz[ovf.an]=0;
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
			return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
		}
	
		i=BGBCC_CCXL_TypeFromOverflow(ctx, &tty, ovf);
		*rdty=tty;
		return(i);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypePointerType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i, j, k;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			tty.val=sty.val&(CCXL_TY_BASEMASK|CCXL_TY_PTRMASK);
			tty.val=tty.val+CCXL_TY_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		tty.val=sty.val+CCXL_TY_PTRIDX1;
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			tty.val=sty.val&(CCXL_TYB2_BASEMASK|
				CCXL_TYB2_PTRMASK|CCXL_TY_TYTY_MASK);
			tty.val|=CCXL_TY_TYTY_BASIC2;
			tty.val=tty.val+CCXL_TYB2_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		tty.val=sty.val+CCXL_TYB2_PTRIDX1;
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			tty.val=sty.val&(CCXL_TYB3_BASEMASK|
				CCXL_TYB3_PTRMASK|CCXL_TY_TYTY_MASK);
			tty.val|=CCXL_TY_TYTY_BASIC3;
			tty.val=tty.val+CCXL_TYB3_PTRIDX1;
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		tty.val=sty.val+CCXL_TYB3_PTRIDX1;
		*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
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
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

ccxl_status BGBCC_CCXL_TypeAutoPromoteType(
	BGBCC_TransState *ctx,
	ccxl_type sty, ccxl_type *rdty)
{
	ccxl_type tty;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, sty) &&
		!BGBCC_CCXL_TypeSgIntP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_I);
		if(rdty)*rdty=tty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeFloat16P(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_F);
		if(rdty)*rdty=tty;
		return(CCXL_STATUS_YES);
	}

#if 0
	if(BGBCC_CCXL_TypeSmallFloatP(ctx, sty) &&
		!BGBCC_CCXL_TypeFloatP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_F);
		if(rdty)*rdty=tty;
		return(CCXL_STATUS_YES);
	}
#endif

#if 0
	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty) &&
		!BGBCC_CCXL_TypeLongP(ctx, sty))
	{
		tty=BGBCC_CCXL_TypeWrapBasicType(CCXL_TY_L);
		*rdty=ty;
		return(CCXL_STATUS_YES);
	}
#endif

	if(rdty)*rdty=sty;
	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TypeGetArraySize(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	int i, j, k;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		i=(sty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		i=(sty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		i=(sty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
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
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
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

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		if((newsz>0) && (newsz<CCXL_TY_BASEARRMAX))
		{
			tty.val=(sty.val)&(~CCXL_TY_ARRMASK);
			tty.val=tty.val|(newsz<<CCXL_TY_ARRSHL);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		if((newsz>0) && (newsz<CCXL_TYB2_BASEARRMAX))
		{
			tty.val=(sty.val)&(~CCXL_TYB2_ARRMASK);
			tty.val=tty.val|(newsz<<CCXL_TYB2_ARRSHL);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		if((newsz>0) && (newsz<CCXL_TYB3_BASEARRMAX))
		{
			tty.val=(sty.val)&(~CCXL_TYB3_ARRMASK);
			tty.val=tty.val|(newsz<<CCXL_TYB3_ARRSHL);
			*rdty=tty;
			return(CCXL_STATUS_YES);
		}

		BGBCC_CCXL_TypeUnpackOverflow(ctx, sty, &ovf);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		ovf.asz[0]=newsz;
		BGBCC_CCXL_TypeFromOverflow(ctx, rdty, ovf);
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	*rdty=sty;
	return(CCXL_STATUS_NO);
}

int BGBCC_CCXL_TypeGetArrayDimSize(
	BGBCC_TransState *ctx, ccxl_type sty)
{
	BGBCC_CCXL_TypeOverflow ovf;
	int i, j, k;

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		i=(sty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		i=(sty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		i=(sty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		return(i);
	}

	if((sty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[sty.val&CCXL_TYOVF_IDXMASK]);
		if(ovf.an>1)
		{
//			i=1;
//			for(j=0; j<ovf.an; j++)
//				i=i*ovf.asz[j];
			i=ovf.asz[0];
//			i=ovf.asz[ovf.an-1];
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
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(0);
}

int BGBCC_CCXL_TypeUnpackOverflow(
	BGBCC_TransState *ctx,
	ccxl_type ty,
	BGBCC_CCXL_TypeOverflow *rovf)
{
	BGBCC_CCXL_TypeOverflow ovf;
	ccxl_type tty;
	int i;

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&CCXL_TY_BASEMASK;
		ovf.pn=(ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL;
		ovf.asz[0]=(ty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		ovf.an=ovf.asz?1:0;
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&CCXL_TYB2_BASEMASK;
		ovf.pn=(ty.val&CCXL_TYB2_PTRMASK)>>CCXL_TYB2_PTRSHL;
		ovf.asz[0]=(ty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		ovf.an=ovf.asz?1:0;
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		memset(&ovf, 0, sizeof(BGBCC_CCXL_TypeOverflow));
		ovf.base=ty.val&CCXL_TYB3_BASEMASK;
		ovf.pn=(ty.val&CCXL_TYB3_PTRMASK)>>CCXL_TYB3_PTRSHL;
		ovf.asz[0]=(ty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		ovf.an=ovf.asz?1:0;
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[ty.val&CCXL_TYOVF_IDXMASK]);
		*rovf=ovf;
		return(CCXL_STATUS_YES);
	}
	
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
//	*rovf=ovf;
	return(CCXL_STATUS_NO);
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

	if((ovf.base>=0) && (ovf.base<CCXL_TY_BASETYMAX) &&
		(ovf.an==0) && (ovf.pn<16))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TY_PTRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<CCXL_TYB3_BASETYMAX) &&
		(ovf.an==0) && (ovf.pn<4))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TYB3_PTRSHL)|
			CCXL_TY_TYTY_BASIC3;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<CCXL_TY_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) &&
		(ovf.asz[0]<CCXL_TY_BASEARRMAX))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TY_PTRSHL)|
			(ovf.asz[0]<<CCXL_TY_ARRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<CCXL_TYB2_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) &&
		(ovf.asz[0]<CCXL_TYB2_BASEARRMAX))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TYB2_PTRSHL)|
			(ovf.asz[0]<<CCXL_TYB2_ARRSHL)|
			CCXL_TY_TYTY_BASIC2;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((ovf.base>=0) && (ovf.base<CCXL_TYB3_BASETYMAX) &&
		(ovf.an==1) && (ovf.asz[0]>0) &&
		(ovf.asz[0]<CCXL_TYB3_BASEARRMAX))
	{
		tty.val=ovf.base|(ovf.pn<<CCXL_TYB3_PTRSHL)|
			(ovf.asz[0]<<CCXL_TYB3_ARRSHL)|
			CCXL_TY_TYTY_BASIC3;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	i=BGBCC_CCXL_TypeIndexOverflow(ctx, ovf);
	if(i>=0)
	{
		tty.val=i|CCXL_TY_TYTY_OVF1;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
//	*rdty=sty;
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
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
	case 'a': bty=CCXL_TY_SB; break;
	case 'b': bty=CCXL_TY_SB; break;
	case 'c': bty=CCXL_TY_SB; break;
	case 'd': bty=CCXL_TY_D; break;
	case 'e': bty=CCXL_TY_D; break;
	case 'f': bty=CCXL_TY_F; break;
	case 'g': bty=CCXL_TY_F128; break;
	case 'h': bty=CCXL_TY_UB; break;
	case 'i': bty=CCXL_TY_I; break;
	case 'j': bty=CCXL_TY_UI; break;
	case 'k': bty=CCXL_TY_F16; break;
	case 'l': bty=CCXL_TY_NL; break;
	case 'm': bty=CCXL_TY_UNL; break;
	case 'n': bty=CCXL_TY_I128; break;
	case 'o': bty=CCXL_TY_UI128; break;
	case 'p': bty=-1; break;
	case 'q': bty=-1; break;
	case 'r': bty=CCXL_TY_VARIANT; break;
	case 's': bty=CCXL_TY_SS; break;
	case 't': bty=CCXL_TY_US; break;
	case 'u': bty=-1; break;
	case 'v': bty=CCXL_TY_V; break;
	case 'w': bty=CCXL_TY_US; break;
	case 'x': bty=CCXL_TY_L; break;
	case 'y': bty=CCXL_TY_UL; break;
	case 'z': bty=CCXL_TY_VARARGS; break;
	case 'C':
		switch(s[1])
		{
		default:  bty=-1; break;
		}
		break;
	case 'D':
		switch(s[1])
		{
		case 'z': bty=CCXL_TY_VALIST; break;
		default:  bty=-1; break;
		}
		break;
	case 'X': case 'Y':
//	case 'U':
		st=BGBCC_CCXL_LookupStructureForSig(ctx, s);
		if(ctx->ril3_norec && !st)
			st=BGBCC_CCXL_GetStructureForSig(ctx, s);

		if(st)
			{ bty=st->litid+256; }
		else
			{ bty=-1; }
		break;
	case 'U':
		st=BGBCC_CCXL_LookupTypedefForSig(ctx, s);
		if(ctx->ril3_norec && !st)
			st=BGBCC_CCXL_GetTypedefForSig(ctx, s);

		if(st)
			{ bty=st->litid+256; }
		else
			{ bty=-1; }
		break;
	default:  bty=-1; break;
	}
	
	if((an==0) && (pn==0) && (bty>=256))
	{
		st=ctx->literals[bty-256];
		if(st && st->decl &&
			(st->littype==CCXL_LITID_TYPEDEF))
		{
			if(st->decl->type.val)
			{
				tty=st->decl->type;
				*rty=tty;
				return(CCXL_STATUS_YES);
			}
		}
	}
	
	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) && (an==0))
	{
		tty.val=bty|(pn<<CCXL_TY_PTRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((bty>=0) && (bty<CCXL_TY_BASETYMAX) &&
		(an==1) && (asz[0]>0) &&
		(asz[0]<CCXL_TY_BASEARRMAX))
	{
		tty.val=bty|(pn<<CCXL_TY_PTRSHL)|
			(asz[0]<<CCXL_TY_ARRSHL)|
			CCXL_TY_TYTY_BASIC;
		*rty=tty;
		return(CCXL_STATUS_YES);
	}

	if((bty>=0) && (bty<CCXL_TYB2_BASETYMAX) &&
		(an==1) && (asz[0]>0) &&
		(asz[0]<CCXL_TYB2_BASEARRMAX))
	{
		tty.val=bty|(pn<<CCXL_TYB2_PTRSHL)|
			(asz[0]<<CCXL_TYB2_ARRSHL)|
			CCXL_TY_TYTY_BASIC2;
		*rty=tty;
		return(CCXL_STATUS_YES);
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
			tty.val=i|CCXL_TY_TYTY_OVF1;
			*rty=tty;
			return(CCXL_STATUS_YES);
		}
	}

	BGBCC_DBGBREAK
	if(*s=='(')
	{
	}

	tty.val=0;
	*rty=tty;
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_BADOPARGS));
	return(CCXL_STATUS_ERR_BADOPARGS);

	tty.val=0;
	*rty=tty;
	return(CCXL_STATUS_NO);
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

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC)
	{
		pn=(ty.val&CCXL_TY_PTRMASK)>>CCXL_TY_PTRSHL;
		asz=(ty.val&CCXL_TY_ARRMASK)>>CCXL_TY_ARRSHL;
		bt=ty.val&CCXL_TY_BASEMASK;
		an=asz?1:0;
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC2)
	{
		pn=(ty.val&CCXL_TYB2_PTRMASK)>>CCXL_TYB2_PTRSHL;
		asz=(ty.val&CCXL_TYB2_ARRMASK)>>CCXL_TYB2_ARRSHL;
		bt=ty.val&CCXL_TYB2_BASEMASK;
		an=asz?1:0;
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_BASIC3)
	{
		pn=(ty.val&CCXL_TYB3_PTRMASK)>>CCXL_TYB3_PTRSHL;
		asz=(ty.val&CCXL_TYB3_ARRMASK)>>CCXL_TYB3_ARRSHL;
		bt=ty.val&CCXL_TYB3_BASEMASK;
		an=asz?1:0;
	}

	if((ty.val&CCXL_TY_TYTY_MASK)==CCXL_TY_TYTY_OVF1)
	{
		i=(ty.val&CCXL_TYOVF_IDXMASK);
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
		case CCXL_TY_I: *t++='i'; break;
		case CCXL_TY_L: *t++='x'; break;
		case CCXL_TY_F: *t++='f'; break;
		case CCXL_TY_D: *t++='d'; break;
		case CCXL_TY_P:
			*t++='P'; *t++='v'; break;
		case CCXL_TY_V: *t++='v'; break;
		case CCXL_TY_NL: *t++='l'; break;
		case CCXL_TY_SB: *t++='c'; break;
		case CCXL_TY_UB: *t++='h'; break;
		case CCXL_TY_SS: *t++='s'; break;
		case CCXL_TY_US: *t++='t'; break;
		case CCXL_TY_UI: *t++='j'; break;
		case CCXL_TY_UL: *t++='y'; break;
		case CCXL_TY_UNL: *t++='m'; break;

		case CCXL_TY_S: *t++='i'; break;

		case CCXL_TY_I128: *t++='n'; break;
		case CCXL_TY_UI128: *t++='o'; break;
		case CCXL_TY_F128: *t++='g'; break;
		case CCXL_TY_F16: *t++='k'; break;
		case CCXL_TY_VARIANT: *t++='r'; break;
		case CCXL_TY_VARARGS: *t++='z'; break;
		case CCXL_TY_VALIST: *t++='D'; *t++='z'; break;

		default:
			break;
		}
		*t++=0;
	}else
	{
		if(ctx->ril_ip)
		{
			st=ctx->literals[bt-256];
			if(st && st->name)
			{
				sprintf(t, "X%s;", st->name);
			}else
			{
				BGBCC_DBGBREAK
			}
		}else
		{
//			st=ctx->literals[bt-256];
			sprintf(t, "X%d", bt-256);
		}
	}
	
	return(bgbcc_strdup(tb));
}

ccxl_type BGBCC_CCXL_TypeWrapBasicType(int ty)
{
	ccxl_type tty;
	tty.val=ty;
	return(tty);
}

int BGBCC_CCXL_TypeCompatibleP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	return(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 0));
}

int BGBCC_CCXL_TypeCompatibleFlP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty, int fl)
{
	ccxl_type dty2, sty2;
	int rt;

	if(dty.val==sty.val)
		return(1);

	if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
		{
			if(dty.val==sty.val)
				return(1);
		
			rt=0;
			switch(dty.val)
			{
			case CCXL_TY_SB:
				rt=(dty.val==sty.val) || (fl&1);	break;
			case CCXL_TY_UB:
				rt=(dty.val==sty.val) || (fl&1);	break;
			case CCXL_TY_SS:
				switch(sty.val)
				{
				case CCXL_TY_SB:	case CCXL_TY_UB:
				case CCXL_TY_SS:
					rt=1; break;				
				default: rt=fl&1; break;
				}
				break;
			case CCXL_TY_US:
				switch(sty.val)
				{
				case CCXL_TY_UB:	case CCXL_TY_US:
					rt=1; break;
				default: rt=fl&1; break;
				}
				break;
			case CCXL_TY_I:		case CCXL_TY_UI:
				rt=1; break;

			case CCXL_TY_NL:	case CCXL_TY_UNL:
				if(ctx->arch_sizeof_long!=4)
					{ rt=0; break; }
				rt=1; break;
			default: rt=0; break;
			}
			return(rt);
//			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeSgLongP(ctx, sty))
			return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, dty))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
			BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, dty))
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, sty))
		{
			BGBCC_CCXL_TypeDerefType(ctx, dty, &dty2);
			BGBCC_CCXL_TypeDerefType(ctx, sty, &sty2);
			
			if(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty2))
				return(1);
		}

		if(BGBCC_CCXL_TypePointerP(ctx, sty))
		{
			BGBCC_CCXL_TypeDerefType(ctx, dty, &dty2);
			BGBCC_CCXL_TypeDerefType(ctx, sty, &sty2);
			
			if(BGBCC_CCXL_TypeCompatibleP(ctx, dty2, sty2))
				return(1);
		}
	}

	if((fl&2) && (ctx->arch_sizeof_ptr==4))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
		{
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
				BGBCC_CCXL_TypeArrayP(ctx, sty) ||
				BGBCC_CCXL_TypeSmallIntP(ctx, sty) ||
				BGBCC_CCXL_TypeSgNLongP(ctx, sty))
			{
				return(1);
			}
		}

#if 1
//		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
		if(BGBCC_CCXL_TypeSgIntP(ctx, dty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, dty))
		{
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
//				BGBCC_CCXL_TypeArrayP(ctx, sty) ||
				BGBCC_CCXL_TypeSmallIntP(ctx, sty))
			{
				return(1);
			}
		}
#endif
	}

	if((fl&2) && (ctx->arch_sizeof_ptr==8))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
		{
			if(BGBCC_CCXL_TypePointerP(ctx, sty) ||
				BGBCC_CCXL_TypeArrayP(ctx, sty) ||
				BGBCC_CCXL_TypeSgLongP(ctx, sty) ||
				BGBCC_CCXL_TypeSgNLongP(ctx, sty))
			{
				return(1);
			}
		}

		if(BGBCC_CCXL_TypeSgLongP(ctx, dty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, dty))
		{
			if(BGBCC_CCXL_TypePointerP(ctx, sty))
			{
				return(1);
			}
		}
	}

	return(0);
}

int BGBCC_CCXL_TypeCompatibleArchP(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	if(BGBCC_CCXL_TypeCompatibleFlP(ctx, dty, sty, 2))
		return(1);
	return(0);
}


#if 0
// int BGBCC_CCXL_GetTypeConvMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
//	if(BGBCC_CCXL_TypeIntP(ctx, sty))
	if(BGBCC_CCXL_TypeSmallIntP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeLongP(ctx, dty))
			{ return(CCXL_OPCONV_I2L); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(CCXL_OPCONV_I2F); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(CCXL_OPCONV_I2D); }
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
			{ return(CCXL_OPCONV_I2P); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeIntP(ctx, dty))
			{ return(CCXL_OPCONV_L2I); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(CCXL_OPCONV_L2F); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(CCXL_OPCONV_L2D); }
		if(BGBCC_CCXL_TypePointerP(ctx, dty))
			{ return(CCXL_OPCONV_L2P); }

		if(BGBCC_CCXL_TypeUnsignedIntP(ctx, dty))
			{ return(CCXL_OPCONV_L2I); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(CCXL_OPCONV_F2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(CCXL_OPCONV_F2L); }
		if(BGBCC_CCXL_TypeDoubleP(ctx, dty))
			{ return(CCXL_OPCONV_F2D); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(CCXL_OPCONV_D2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(CCXL_OPCONV_D2L); }
		if(BGBCC_CCXL_TypeFloatP(ctx, dty))
			{ return(CCXL_OPCONV_D2F); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, sty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, dty))
			{ return(CCXL_OPCONV_P2I); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, dty))
			{ return(CCXL_OPCONV_P2L); }

//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

//	BGBCC_CCXL_TagError(ctx,
//		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}

// int BGBCC_CCXL_GetTypeConvIMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

	if(!BGBCC_CCXL_TypeSmallIntP(ctx, dty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	if(!BGBCC_CCXL_TypeSmallIntP(ctx, sty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	return(BGBCC_CCXL_GetTypeConvILMode(ctx, dty, sty));
}		

// int BGBCC_CCXL_GetTypeConvLMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

	if(!BGBCC_CCXL_TypeSmallLongP(ctx, dty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	if(!BGBCC_CCXL_TypeSmallLongP(ctx, sty))
	{
//		BGBCC_CCXL_TagError(ctx,
//			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}
	return(BGBCC_CCXL_GetTypeConvILMode(ctx, dty, sty));
}		

// int BGBCC_CCXL_GetTypeConvILMode(
	BGBCC_TransState *ctx,
	ccxl_type dty, ccxl_type sty)
{
	int dt, st;

//	if(!BGBCC_CCXL_TypeSmallIntP(ctx, dty))
//		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
//	if(!BGBCC_CCXL_TypeSmallIntP(ctx, sty))
//		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
		
	dt=dty.val; st=sty.val;
	
	if(dt==CCXL_TY_I)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_SXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_SXS); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_SXI); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if(dt==CCXL_TY_UI)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if((dt==CCXL_TY_L) || (dt==CCXL_TY_NL))
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_SXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_SXS); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_SXI); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}

	if((dt==CCXL_TY_UL) || (dt==CCXL_TY_UNL))
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		if(st==CCXL_TY_SS)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_US)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_I)
			{ return(CCXL_OPCONV2_ZXI); }
		if(st==CCXL_TY_UI)
			{ return(CCXL_OPCONV2_ZXI); }

		if(st==CCXL_TY_L)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_NL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
		if(st==CCXL_TY_UNL)
			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
	}
	
	if(dt==CCXL_TY_SB)
		{ return(CCXL_OPCONV2_SXB); }
	if(dt==CCXL_TY_UB)
		{ return(CCXL_OPCONV2_ZXB); }

	if(dt==CCXL_TY_SS)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_SXB); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		return(CCXL_OPCONV2_SXS);
	}

	if(dt==CCXL_TY_US)
	{
		if(st==CCXL_TY_SB)
			{ return(CCXL_OPCONV2_ZXS); }
		if(st==CCXL_TY_UB)
			{ return(CCXL_OPCONV2_ZXB); }
		return(CCXL_OPCONV2_ZXS);
	}

	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}
#endif

bool BGBCC_CCXL_TypeSupportsOperatorP(
	BGBCC_TransState *ctx, ccxl_type ty, int opr)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, ty))
	{
		return(true);
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, ty))
	{
		return(true);
	}

	if(	BGBCC_CCXL_TypeFloatP(ctx, ty) ||
		BGBCC_CCXL_TypeDoubleP(ctx, ty) ||
		BGBCC_CCXL_TypeFloat16P(ctx, ty) ||
		BGBCC_CCXL_TypeFloat128P(ctx, ty))
	{
		if(opr==CCXL_BINOP_ADD)
			return(true);
		if(opr==CCXL_BINOP_SUB)
			return(true);
		if(opr==CCXL_BINOP_MUL)
			return(true);
		if(opr==CCXL_BINOP_DIV)
			return(true);
		return(false);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, ty))
	{
		if(opr==CCXL_BINOP_ADD)
			return(true);
		if(opr==CCXL_BINOP_SUB)
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
//		ltyb=lty.val&CCXL_TY_BASEMASK;
//		rtyb=rty.val&CCXL_TY_BASEMASK;
		ltyb=BGBCC_CCXL_GetTypeBaseType(ctx, lty);
		rtyb=BGBCC_CCXL_GetTypeBaseType(ctx, rty);
		dtyb=CCXL_TY_I;
		
		switch(ltyb)
		{
		case CCXL_TY_I:
			switch(rtyb)
			{
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		case CCXL_TY_UI:
			dtyb=CCXL_TY_UI; break;

		case CCXL_TY_UNL:
			dtyb=CCXL_TY_UNL; break;

		case CCXL_TY_SB:
			switch(rtyb)
			{
			case CCXL_TY_SB:	dtyb=CCXL_TY_SB; break;
			case CCXL_TY_UB:	dtyb=CCXL_TY_UB; break;
			case CCXL_TY_SS:	dtyb=CCXL_TY_SS; break;
			case CCXL_TY_US:	dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		case CCXL_TY_UB:
			switch(rtyb)
			{
			case CCXL_TY_SB:	case CCXL_TY_UB:
				dtyb=CCXL_TY_UB; break;
			case CCXL_TY_SS:	dtyb=CCXL_TY_SS; break;
			case CCXL_TY_US:	dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;

		case CCXL_TY_SS:
			switch(rtyb)
			{
			case CCXL_TY_SB: case CCXL_TY_UB:
			case CCXL_TY_SS:
				dtyb=CCXL_TY_SS; break;
			case CCXL_TY_US:	dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		case CCXL_TY_US:
			switch(rtyb)
			{
			case CCXL_TY_SB:	case CCXL_TY_UB:
			case CCXL_TY_SS:	case CCXL_TY_US:
				dtyb=CCXL_TY_US; break;
			case CCXL_TY_I:		dtyb=CCXL_TY_I; break;
			case CCXL_TY_UI:	dtyb=CCXL_TY_UI; break;
			case CCXL_TY_UNL:	dtyb=CCXL_TY_UNL; break;
			default:			dtyb=CCXL_TY_I; break;
			}
			break;
		default:			dtyb=CCXL_TY_I; break;
		}

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, dtyb);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallLongP(ctx, rty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		{
			*rdty=rty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}
		
		if(BGBCC_CCXL_TypeUnsignedLongP(ctx, lty))
		{
			*rdty=lty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		if(BGBCC_CCXL_TypeUnsignedLongP(ctx, rty))
		{
			*rdty=rty;
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
		}

		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, rty))
//		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
//			return(CCXL_STATUS_YES);
//		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_L);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(	BGBCC_CCXL_TypeFloatP(ctx, rty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

//		if(BGBCC_CCXL_TypePointerP(ctx, dty))
//			{ return(CCXL_OPCONV_L2P); }
		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, lty) ||
		BGBCC_CCXL_TypeFloat16P(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, lty))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, rty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, rty))
		{
//			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F);

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_D);
			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeVariantP(ctx, rty))
		{
			*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
			return(CCXL_STATUS_YES);
		}

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeVariantP(ctx, lty) ||
		BGBCC_CCXL_TypeVariantP(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_VARIANT);
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
		((opr==CCXL_BINOP_ADD) || (opr==CCXL_BINOP_SUB)))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, rty))
		{
			*rdty=lty;
			if(BGBCC_CCXL_TypeArrayP(ctx, lty))
				{ BGBCC_CCXL_TypePointerType(ctx, lty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, rty))
		{
			*rdty=lty;
			if(BGBCC_CCXL_TypeArrayP(ctx, lty))
				{ BGBCC_CCXL_TypePointerType(ctx, lty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty))
		{
			if(opr==CCXL_BINOP_SUB)
			{
				*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I);
				return(CCXL_STATUS_YES);
			}

			if(opr==CCXL_BINOP_ADD)
			{
				*rdty=lty;
				return(CCXL_STATUS_YES);
			}
		}

		printf("Ty %X %X\n", lty.val, rty.val);

		*rdty=lty;
		BGBCC_CCXL_TagError(ctx,
			CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
		return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty) &&
		(opr==CCXL_BINOP_ADD))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, lty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}

		if(BGBCC_CCXL_TypeSmallLongP(ctx, lty))
		{
			*rdty=rty;
			if(BGBCC_CCXL_TypeArrayP(ctx, rty))
				{ BGBCC_CCXL_TypePointerType(ctx, rty, rdty); }

			return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
				CCXL_STATUS_YES:CCXL_STATUS_NO);
//			return(CCXL_STATUS_YES);
		}
	}

	if(BGBCC_CCXL_TypeSmallInt128P(ctx, lty) &&
		BGBCC_CCXL_TypeSmallInt128P(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_I128);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	if(BGBCC_CCXL_TypeSmallFloat128P(ctx, lty) &&
		BGBCC_CCXL_TypeSmallFloat128P(ctx, rty))
	{
		*rdty=BGBCC_CCXL_MakeTypeID(ctx, CCXL_TY_F128);
		return(BGBCC_CCXL_TypeSupportsOperatorP(ctx, *rdty, opr)?
			CCXL_STATUS_YES:CCXL_STATUS_NO);
	}

	*rdty=lty;
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
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
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, lty) &&
		BGBCC_CCXL_TypeBaseSmallILFD_P(ctx, rty))
	{
		return(BGBCC_CCXL_GetTypeBinaryDest(ctx,
			CCXL_BINOP_ADD, lty, rty, rdty));
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, lty) &&
		BGBCC_CCXL_TypeSmallIntP(ctx, rty))
	{
		*rdty=lty;
		return(CCXL_STATUS_YES);
	}

	if(BGBCC_CCXL_TypeArrayOrPointerP(ctx, rty) &&
		BGBCC_CCXL_TypeSmallIntP(ctx, lty))
	{
		*rdty=rty;
		return(CCXL_STATUS_YES);
	}

	if(opr==CCXL_CMP_NV)
	{
		if(lty.val==rty.val)
		{
			*rdty=lty;
			return(CCXL_STATUS_YES);
		}
	}

	*rdty=lty;
	BGBCC_CCXL_TagError(ctx,
		CCXL_TERR_STATUS(CCXL_STATUS_ERR_UNHANDLEDTYPE));
	return(CCXL_STATUS_ERR_UNHANDLEDTYPE);
}
