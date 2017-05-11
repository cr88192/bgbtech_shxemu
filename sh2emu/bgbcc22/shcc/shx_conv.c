int BGBCC_SHXC_EmitConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dreg, ccxl_register sreg)
{
	int nm1, nm2;
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

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R5);
			BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R5, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
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

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
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

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_EXTSB,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
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

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_EXTUB,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
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

		if(st==CCXL_TY_SB)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTSB, dreg, sreg));	}
		if(st==CCXL_TY_UB)
		{	return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				BGBCC_SH_NMID_EXTUB, dreg, sreg));	}

		if((st==CCXL_TY_I) || (st==CCXL_TY_UI) ||
			(st==CCXL_TY_NL) || (st==CCXL_TY_UNL))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;
			return(BGBCC_SHXC_EmitOpNmidVRegVReg(ctx, sctx, dtype,
				nm1, dreg, sreg));
		}

		if((st==CCXL_TY_L) || (st==CCXL_TY_UL))
		{
			nm1=BGBCC_SH_NMID_EXTSW;
			if(dt==CCXL_TY_US)
				nm1=BGBCC_SH_NMID_EXTUW;

			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((dt==CCXL_TY_L) || (dt==CCXL_TY_UL))
	{
		if((st==CCXL_TY_UB) || (st==CCXL_TY_US) ||
			(st==CCXL_TY_UI) || (st==CCXL_TY_UNL))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R5, 0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			return(1);
		}

		if((st==CCXL_TY_SB) || (st==CCXL_TY_SS) ||
			(st==CCXL_TY_I) || (st==CCXL_TY_NL))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
//			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
//				BGBCC_SH_REG_R5, 0);

			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R4, BGBCC_SH_REG_R5);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, -32);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SHAD,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R5);

			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			return(1);
		}

		if((st==CCXL_TY_I128) || (st==CCXL_TY_UI128))
		{
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R6);
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R6, 0, BGBCC_SH_REG_R4);
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R6, 4, BGBCC_SH_REG_R5);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			return(1);
		}
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
