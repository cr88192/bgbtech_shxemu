int BGBCC_SHXC_EmitConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dreg, ccxl_register sreg)
{
	int dt, st;

	dt=dtype.val;
	st=stype.val;
	
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
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }
		if(st==CCXL_TY_UI)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }

		if(st==CCXL_TY_NL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }
		if(st==CCXL_TY_UNL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }

//		if(st==CCXL_TY_L)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_NL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_UL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
//		if(st==CCXL_TY_UNL)
//			{ return(CCXL_STATUS_ERR_UNHANDLEDTYPE); }
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
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }
		if(st==CCXL_TY_UI)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }
		if(st==CCXL_TY_NL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }
		if(st==CCXL_TY_UNL)
			{ return(BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, dtype, dreg, sreg)); }
	}

	if(dt==CCXL_TY_SB)
	{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
			BGBCC_SH_NMID_EXTSB, dreg, sreg));	}
	if(dt==CCXL_TY_UB)
	{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
			BGBCC_SH_NMID_EXTUB, dreg, sreg));	}

	if((dt==CCXL_TY_SS) || (dt==CCXL_TY_US))
	{
		if(st==CCXL_TY_SS)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSW, dreg, sreg));	}
		if(st==CCXL_TY_US)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUW, dreg, sreg));	}
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
