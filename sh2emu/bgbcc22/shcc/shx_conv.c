int BGBCC_SHXC_EmitConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dreg, ccxl_register sreg)
{
	int dt, st;

	dt=dtype.val;
	st=stype.val;

	if(dt==st)
	{
		return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
			dtype, dreg, sreg));
	}

	if(BGBCC_CCXL_TypePointerP(ctx, dtype))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, stype))
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeArrayP(ctx, stype))
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(BGBCC_CCXL_TypeSmallIntP(ctx, stype))
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
		if(BGBCC_CCXL_TypeSmallLongP(ctx, stype))
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_I)
	{
		if(st==CCXL_TY_SB)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSB, dreg, sreg));	}
		if(st==CCXL_TY_UB)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUB, dreg, sreg));	}
		if(st==CCXL_TY_SS)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSW, dreg, sreg));	}
		if(st==CCXL_TY_US)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));	}
		if(st==CCXL_TY_I)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype,
				dreg, sreg)); }
		if(st==CCXL_TY_UI)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype,
				dreg, sreg)); }

		if(st==CCXL_TY_NL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
		if(st==CCXL_TY_UNL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(st==CCXL_TY_F)
		{
			BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
			BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

//		if(st==CCXL_TY_L)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_NL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_UL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_UNL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }

		if(BGBCC_CCXL_TypePointerP(ctx, stype))
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
	}

	if(dt==CCXL_TY_UI)
	{
		if(st==CCXL_TY_SB)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSB, dreg, sreg));	}
		if(st==CCXL_TY_UB)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUB, dreg, sreg));	}
		if(st==CCXL_TY_SS)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSW, dreg, sreg));	}
		if(st==CCXL_TY_US)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));	}
		if(st==CCXL_TY_I)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
		if(st==CCXL_TY_UI)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
		if(st==CCXL_TY_NL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
		if(st==CCXL_TY_UNL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }

		if(st==CCXL_TY_F)
		{
			BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
			BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FTRC, BGBCC_SH_NMID_STS, dreg, sreg));
		}

		if(BGBCC_CCXL_TypePointerP(ctx, stype))
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx,
				dtype, dreg, sreg)); }
	}

	if(dt==CCXL_TY_SB)
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB))
		{
			return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSB, dreg, sreg));
		}
	}
	if(dt==CCXL_TY_UB)
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB))
		{
			return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUB, dreg, sreg));
		}
	}

	if((dt==CCXL_TY_SS) || (dt==CCXL_TY_US))
	{
		if(st==CCXL_TY_SS)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSW, dreg, sreg));	}
		if(st==CCXL_TY_US)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));	}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_F)
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB))
		{
			BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
			BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FCNVDS, BGBCC_SH_NMID_FSTS, dreg, sreg));
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(dt==CCXL_TY_D)
	{
		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL) ||
			(st==CCXL_TY_SS) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_SB) || (st==CCXL_TY_UB))
		{
			BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_LDS, BGBCC_SH_NMID_FLOAT, dreg, sreg));
		}

		if(st==CCXL_TY_F)
		{
			BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx);
			return(BGBCC_SHXC_EmitFpConvVRegVReg(ctx, sctx,
				BGBCC_SH_NMID_FLDS, BGBCC_SH_NMID_FCNVSD, dreg, sreg));
		}

		if(st==CCXL_TY_D)
		{
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
