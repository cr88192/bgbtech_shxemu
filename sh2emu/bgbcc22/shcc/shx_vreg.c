int BGBCC_SHXC_EmitMovRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int sreg, int dreg)
{
	int sreg2, dreg2, sw;

	if(sreg==dreg)
		return(1);

	if(BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, dreg))
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg, dreg);
		return(1);
	}
	
	if(BGBCC_SHX_EmitCheckRegExtGPR(sctx, sreg) &&
		BGBCC_SHX_EmitCheckRegExtGPR(sctx, dreg))
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, dreg))
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_LDS, sreg, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsGpReg(ctx, sctx, dreg))
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FLDS, sreg, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
//		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
//		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sreg2=BGBCC_SH_REG_FR0+(sreg&15);
		dreg2=BGBCC_SH_REG_FR0+(dreg&15);
	
		if(BGBCC_SHXC_GetStatusFpscrSz(ctx, sctx))
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg2, dreg2);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg2+0, dreg2+0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, sreg2+1, dreg2+1);
		}
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		sreg2=BGBCC_SH_REG_R0+(sreg&15);
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+0, dreg2+0);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg2+1, dreg2+1);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		sreg2=BGBCC_SH_REG_R0+(sreg&15);
//		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		dreg2=BGBCC_SH_REG_FR0+(dreg&15);
		sw=(sctx->is_le!=0);

		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_LDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));

		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_LDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FSTS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, sreg) &&
		BGBCC_SHXC_EmitRegIsLpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_R0+(dreg&15);
//		sreg2=BGBCC_SH_REG_FR0+((sreg&7)*2);
		sreg2=BGBCC_SH_REG_FR0+(sreg&15);
		sw=(sctx->is_le!=0);

		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
		
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FLDS, sreg2+0, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(0^sw));
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_FLDS, sreg2+1, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_STS, BGBCC_SH_REG_FPUL, dreg2+(1^sw));

		return(1);
	}

	printf("BGBCC_SHXC_EmitMovRegReg: "
		"Bad MOV Regs Rm=%02X Rn=%02X\n", sreg, dreg);
	BGBCC_CCXL_StubError(ctx);
	return(0);

//	nm=BGBCC_SH_NMID_MOV;
//	if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, creg))
//		nm=BGBCC_SH_NMID_FMOV;
//	BGBCC_SHX_EmitOpRegReg(sctx, nm, creg, dreg);
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
//	return(1);
}

int BGBCC_SHXC_EmitLoadVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int creg, nm, pr;

#if 0
	if(BGBCC_CCXL_IsRegImmStringP(ctx, sreg))
	{
		BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
		return(1);
	}
#endif
	
	creg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
//		nm=BGBCC_SH_NMID_MOV;
//		if(BGBCC_SHXC_EmitRegIsFpReg(ctx, sctx, creg))
//			nm=BGBCC_SH_NMID_FMOV;
//		BGBCC_SHX_EmitOpRegReg(sctx, nm, creg, dreg);
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx, creg, dreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	pr=BGBCC_SHXC_GetVRegPriority(ctx, sctx, sreg);
	if((sctx->vsp_rsv>0) && (pr<sctx->vsp_rsv))
		{ BGBCC_DBGBREAK; }

	BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, dreg);
	return(1);

//	BGBCC_CCXL_StubError(ctx);
//	return(0);
}

int BGBCC_SHXC_EmitStoreVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register dreg, int sreg)
{
	int creg, pr;
	
	creg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
	{
//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, sreg, creg);
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx, sreg, creg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	pr=BGBCC_SHXC_GetVRegPriority(ctx, sctx, dreg);
	if((sctx->vsp_rsv>0) && (pr<sctx->vsp_rsv))
		{ BGBCC_DBGBREAK; }

	BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, sreg);
	return(1);
}

int BGBCC_SHXC_EmitMovVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, ccxl_register sreg)
{
	s64 li;
	int cdreg, csreg;
	int tr0, rcls;
	int i, j, k;

#if 1
	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);
	if((rcls==BGBCC_SH_REGCLS_VO_GR) || (rcls==BGBCC_SH_REGCLS_VO_GR2) ||
		(rcls==BGBCC_SH_REGCLS_VO_QGR))
	{

		if((BGBCC_CCXL_IsRegLocalP(ctx, sreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, sreg)) &&
			(BGBCC_CCXL_IsRegLocalP(ctx, dreg) ||
			BGBCC_CCXL_IsRegArgP(ctx, dreg)))
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, rcls);
//			BGBCC_SHXC_EmitLoadFrameVRegByValReg
			BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx, sreg, tr0);
			BGBCC_SHXC_EmitStoreFrameVRegByValReg(ctx, sctx, dreg, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

			return(1);
		}
	}
#endif

	if(sctx->is_addr64 &&
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			li=(u32)li;
			BGBCC_SHXC_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, sreg))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, sreg);
			BGBCC_SHXC_EmitMovVRegImm(ctx, sctx, type, dreg, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
//	if(0)
//	if(1)
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
		{
			j=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
			BGBCC_SHXC_EmitMovVRegImm(ctx, sctx, type, dreg, j);
			return(1);
		}

		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		
		if((cdreg&0xF0) != (csreg&0xF0))
		{
			if(((cdreg&BGBCC_SH_REG_RTMASK5)==(csreg&BGBCC_SH_REG_RTMASK5)) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0))
			{
			}else
			if(((cdreg&BGBCC_SH_REG_RTMASK5)==(csreg&BGBCC_SH_REG_RTMASK5)) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0))
			{
			}else

//			if(((cdreg&0xF0)==BGBCC_SH_REG_RD0) &&
//				((csreg&0xF0)==BGBCC_SH_REG_RQ0))
			if(((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0))
			{
				csreg=BGBCC_SH_REG_RD0+(csreg&31);
//			}else if(((cdreg&0xF0)==BGBCC_SH_REG_RQ0) &&
//				((csreg&0xF0)==BGBCC_SH_REG_RD0))
			}else if(((cdreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RQ0) &&
				((csreg&BGBCC_SH_REG_RTMASK5)==BGBCC_SH_REG_RD0))
			{
				csreg=BGBCC_SH_REG_RQ0+(csreg&31);
			}else
			{
				printf("BGBCC_SHXC_EmitMovVRegVReg: "
					"Bad MOV Regs Rm=%02X(%016llX) Rn=%02X(%016llX)\n",
					csreg, sreg.val, cdreg, dreg.val);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				cdreg=BGBCC_SH_REG_ZZR;
				csreg=BGBCC_SH_REG_ZZR;
			}
		}
	}else
	{
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	}

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
//			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx, csreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

//		if(BGBCC_CCXL_IsRegDoubleP(ctx, sreg))
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR2);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR1);
//			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR1);
			return(1);
		}

//		if(BGBCC_CCXL_IsRegLongP(ctx, sreg))
		if(BGBCC_CCXL_TypeSgLongP(ctx, type) ||
			BGBCC_CCXL_TypeVariantP(ctx, type))
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR2);
//			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			return(1);
		}

		if(BGBCC_CCXL_TypeFloatP(ctx, type))
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR1);
//			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR1);
			return(1);
		}

//		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R3);
//		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);

		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, tr0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, tr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

		return(1);
	}
}


int BGBCC_SHXC_EmitFpConvVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nm1, int nm2,
	ccxl_register dreg, ccxl_register sreg)
{
	int tr0;
	int cdreg, csreg;
	
	csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
	cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
//	BGBCC_SHXC_EmitMovRegReg(ctx, sctx, csreg, cdreg);

	if(	BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, csreg) ||
		BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, cdreg))
		{ BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx); }
	else
		{ BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx); }

	csreg=BGBCC_SHXC_RegNormalizeReg(ctx, sctx, csreg);
	cdreg=BGBCC_SHXC_RegNormalizeReg(ctx, sctx, cdreg);
	if((nm1==BGBCC_SH_NMID_LDS) || (nm1==BGBCC_SH_NMID_FLDS))
	{
		if(BGBCC_SHX_EmitCheckRegNonBaseGPR(sctx, csreg))
//		if(0)
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, csreg, tr0);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, tr0, BGBCC_SH_REG_FPUL);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, BGBCC_SH_REG_FPUL);
		}
	}
	else
		{ BGBCC_SHX_EmitOpReg(sctx, nm1, csreg); }

	if((nm2==BGBCC_SH_NMID_STS) || (nm2==BGBCC_SH_NMID_FSTS))
	{
		if(BGBCC_SHX_EmitCheckRegNonBaseGPR(sctx, cdreg))
//		if(0)
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_FPUL, tr0);
			BGBCC_SHX_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_MOV, tr0, cdreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_FPUL, cdreg);
		}
	}
	else
		{ BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg); }

	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
	return(1);
}


int BGBCC_SHXC_EmitMovVRegImm(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dreg, s64 imm)
{
	int cdreg, csreg;
	int rcls;
	
	cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((((s32)imm)!=imm) &&
			BGBCC_SHX_EmitCheckRegQuad(sctx, cdreg))
		{
			BGBCC_SHX_EmitLoadRegImm64P(sctx, cdreg, imm);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
	
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, cdreg, imm);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);

	if((rcls==BGBCC_SH_REGCLS_QGR) || (rcls==BGBCC_SH_REGCLS_VO_QGR))
	{
		csreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
//		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, csreg, imm);
		BGBCC_SHX_EmitLoadRegImm64P(sctx, csreg, imm);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, csreg);
		return(1);
	}

	csreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
	BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, csreg, imm);
	BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, csreg);
	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, csreg);

//	BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R0, imm);
//	BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
	return(1);
}


int BGBCC_SHXC_EmitOpNmidVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, int nmid,
	ccxl_register dreg, ccxl_register sreg)
{
	int cdreg, csreg, ctreg;
	
	cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
	csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);

	if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nmid, csreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
		
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, cdreg);
		BGBCC_SHX_EmitOpRegReg(sctx, nmid, cdreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}else
	{
		if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitOpRegReg(sctx, nmid, csreg, ctreg);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}

		ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, ctreg);
		BGBCC_SHX_EmitOpRegReg(sctx, nmid, ctreg, ctreg);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, ctreg);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitJCmpVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	s32 imm;
	int nm1, nm2, nm3;
	
	nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHS;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGE;
			nm2=BGBCC_SH_NMID_BF;
			nm3=BGBCC_SH_NMID_BRLT;
		}
		break;
	case CCXL_CMP_GT:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BT;
			nm3=BGBCC_SH_NMID_BRGT;
		}
		break;
	case CCXL_CMP_LE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHI;
			nm2=BGBCC_SH_NMID_BF;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGT;
			nm2=BGBCC_SH_NMID_BF;
			nm3=BGBCC_SH_NMID_BRLE;
		}
		break;
	case CCXL_CMP_GE:
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			nm1=BGBCC_SH_NMID_CMPHS;
			nm2=BGBCC_SH_NMID_BT;
		}else
		{
			nm1=BGBCC_SH_NMID_CMPGE;
			nm2=BGBCC_SH_NMID_BT;
			nm3=BGBCC_SH_NMID_BRGE;
		}
		break;

	case CCXL_CMP_TST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_NTST:
		nm1=BGBCC_SH_NMID_TST;
		nm2=BGBCC_SH_NMID_BT;
		break;

	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
		{
			imm=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

			csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
				
			if(BGBCC_SHX_TryEmitOpRegImm(sctx, nm1, csreg, imm)>0)
			{
				BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
				BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		}else
		{
			csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
			ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);

			if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
				csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
			if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
				ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		}

#if 1
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1) &&
			BGBCC_SHX_EmitCheckRegBaseGPR(sctx, csreg) &&
			BGBCC_SHX_EmitCheckRegBaseGPR(sctx, ctreg))
		{
//			BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
			BGBCC_SHXC_SetModeDqClear(ctx, sctx);

			BGBCC_SHX_EmitOpCmpRegRegLabel(sctx, nm3, ctreg, csreg, lbl);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

//		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		BGBCC_SHXC_CheckSetModeDqClear(ctx, sctx);

		BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_SHX_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, lbl);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		(BGBCC_CCXL_TypePointerP(ctx, type) && !sctx->is_addr64))
	{
		return(BGBCC_SHXC_EmitJCmpVRegVRegInt(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegVRegFloat(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if((BGBCC_CCXL_TypeSgLongP(ctx, type) ||
		BGBCC_CCXL_TypeSgNLongP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type)) &&
			sctx->is_addr64)
	{
		return(BGBCC_SHXC_EmitJCmpVRegVRegQLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegVRegLong(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegZeroInt(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	int csreg, ctreg;
	int nm1, nm2, nm3;

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(cmp==CCXL_CMP_LT)
		{
//			BGBCC_SHX_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BT, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_GE)
		{
			BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
			BGBCC_SHX_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRAN, lbl);
			return(1);
		}

		if(cmp==CCXL_CMP_LE)
			{ cmp=CCXL_CMP_EQ; }
		if(cmp==CCXL_CMP_GT)
			{ cmp=CCXL_CMP_NE; }
	}
	
	nm1=-1; nm2=-1; nm3=-1;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BREQ;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRNE;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLT;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGT;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPPL;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_BRLE;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPPZ;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_BRGE;
		break;
	default:
		nm1=-1;
		nm2=-1;
		nm3=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);

		if(!BGBCC_SHX_EmitCheckRegBaseGPR(sctx, csreg))
		{
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			return(BGBCC_SHXC_EmitJCmpVRegVRegInt(ctx, sctx,
				type, sreg, treg, cmp, lbl));

//			return(1);
		}

#if 1
//		if(nm3>=0)
		if((nm3>=0) && !(sctx->dfl_dq&1) &&
			BGBCC_SHX_EmitCheckRegBaseGPR(sctx, csreg))
		{
//			BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
			BGBCC_SHXC_SetModeDqClear(ctx, sctx);

			BGBCC_SHX_EmitOpCmpRegLabel(sctx, nm3, csreg, lbl);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}
#endif

//		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		BGBCC_SHXC_SetModeDqClear(ctx, sctx);

		BGBCC_SHX_EmitOpReg(sctx, nm1, csreg);
//		if(sctx->need_farjmp)
//			BGBCC_SHX_EmitOpFarLabel(sctx, nm2, lbl);
//		else
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
//	BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegZero(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg,
	int cmp, int lbl)
{
	ccxl_register treg;
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

	if((BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type)) && !sctx->is_addr64)
	{
		return(BGBCC_SHXC_EmitJCmpVRegZeroInt(ctx, sctx,
			type, sreg, cmp, lbl));
	}

	if((BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type)) && sctx->is_addr64)
	{
		return(BGBCC_SHXC_EmitJCmpVRegZeroQLong(ctx, sctx,
			type, sreg, cmp, lbl));
	}

#if 0
	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_SHXC_EmitJCmpVRegZeroFloat(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}
#endif

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if((BGBCC_CCXL_TypeSgNLongP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type) ||
		BGBCC_CCXL_TypeArrayP(ctx, type)) &&
		sctx->is_addr64)
	{
		BGBCC_CCXL_GetRegForLongValue(ctx, &treg, 0);
		return(BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type))
	{
		BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 0.0);
		return(BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 0.0);
		return(BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx,
			type, sreg, treg, cmp, lbl));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
