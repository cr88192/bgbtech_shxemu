/* Try to get Variable as register.
 * Will return a value as a register if possible, but may fail.
 * Returns ZZR if it is unable to do so.
 */

const byte bgbcc_shx_fcachereg[5]={
	BGBCC_SH_REG_FR12, BGBCC_SH_REG_FR13,
	BGBCC_SH_REG_FR14, BGBCC_SH_REG_FR15,
	BGBCC_SH_REG_ZZR};

const byte bgbcc_shx_dcachereg[5]={
	BGBCC_SH_REG_DR6, BGBCC_SH_REG_DR6,
	BGBCC_SH_REG_DR7, BGBCC_SH_REG_DR7,
	BGBCC_SH_REG_ZZR};

int BGBCC_SHXC_EmitRegIsGpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	return(0);
}

int BGBCC_SHXC_EmitRegIsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	return(0);
}

int BGBCC_SHXC_EmitRegIsDpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
		return(1);
	return(0);
}

int BGBCC_SHXC_EmitRegIsLpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	return(0);
}

int BGBCC_SHXC_RegNormalizeReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int dreg)
{
	if((dreg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
		return(BGBCC_SH_REG_FR0+(dreg&7)*2);
	if((dreg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(BGBCC_SH_REG_R0+(dreg&15));
	return(dreg);
}

int BGBCC_SHXC_CheckVRegIsZRegP(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	if(((reg.val&0xFFF)==0xFFF) && ((reg.val>>56)<4))
		return(1);
	return(0);
}

int BGBCC_SHXC_GetStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(sctx->cur_fpscr&BGBCC_SH_FPSCR_SZ);
}

int BGBCC_SHXC_GetStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(sctx->cur_fpscr&BGBCC_SH_FPSCR_PR);
}

int BGBCC_SHXC_UpdateStatusFpscr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	u32 state)
{
	int treg;

	if(state!=sctx->cur_fpscr)
//	if(1)
	{
		treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			treg, state);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
			treg, BGBCC_SH_REG_FPSCR);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
		sctx->cur_fpscr=state;
		return(1);
	}

	return(0);
}

int BGBCC_SHXC_ClearStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr&(~BGBCC_SH_FPSCR_SZ)));
}

int BGBCC_SHXC_ClearStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr&(~BGBCC_SH_FPSCR_PR)));
}

int BGBCC_SHXC_SetStatusFpscrSz(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	return(BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr|BGBCC_SH_FPSCR_SZ));
}

int BGBCC_SHXC_SetStatusFpscrPr(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx,
		sctx->cur_fpscr|BGBCC_SH_FPSCR_PR);
	return(1);
}

int BGBCC_SHXC_SetStatusFpscrFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	u32 fl1;

	if(!sctx->use_fpr && !sctx->use_dbr)
		return(0);

	fl1=sctx->cur_fpscr&(~(BGBCC_SH_FPSCR_PR|BGBCC_SH_FPSCR_SZ));
	BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx, fl1);
	return(1);
}

int BGBCC_SHXC_SetStatusFpscrDouble(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	u32 fl1;

	if(!sctx->use_fpr && !sctx->use_dbr)
		return(0);

	fl1=sctx->cur_fpscr&(~BGBCC_SH_FPSCR_SZ);
	fl1|=BGBCC_SH_FPSCR_PR;
	BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx, fl1);
	return(1);
}

/** Reset active FPSCR to function-local defaults */
int BGBCC_SHXC_ResetFpscrLocal(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	if(!sctx->use_fpr && !sctx->use_dbr)
		return(0);

	BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx,
		sctx->dfl_fpscr);
	return(1);
}

/** Reset active FPSCR to ABI defaults */
int BGBCC_SHXC_ResetFpscrDefaults(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	if(!sctx->use_fpr && !sctx->use_dbr)
		return(0);

	BGBCC_SHXC_UpdateStatusFpscr(ctx, sctx,
		sctx->dfl_fpscr);
//	sctx->cur_fpscr=-1;
	return(1);
}

int BGBCC_SHXC_EmitLoadFrameOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int ofs1, dreg2, treg, sw;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg) &&
		!BGBCC_SHXC_GetStatusFpscrSz(ctx, sctx))
	{
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sw=(sctx->is_le!=0);

		p0=BGBCC_SHX_TryEmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_FMOVS,
			BGBCC_SH_REG_SP, ofs1+4, dreg2+(1^sw));
		if(p0>0)
		{
			BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_FMOVS,
				BGBCC_SH_REG_SP, ofs1+0, dreg2+(0^sw));
			return(1);
		}

#if 1
//		if(ofs1<128)
		if((ofs1<128) && !(sctx->sreg_live&1))
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_R0, ofs1);

			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
				BGBCC_SH_REG_R0, dreg2+(0^sw));
			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
				BGBCC_SH_REG_R0, dreg2+(1^sw));
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
#endif

//		if(sctx->sreg_live&1)
		if(1)
		{
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				treg, ofs1);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_SP, treg);
			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
				treg, dreg2+(0^sw));
			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
				treg, dreg2+(1^sw));

			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs1);
		if(p0>0)
		{
			p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
				BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg2+(0^sw));
			if(p1<=0)
				return(0);

			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_R0, 4);
			BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
				BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg2+(1^sw));
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	dreg2=dreg;
	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
	}else
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
	}

	p0=BGBCC_SHX_TryEmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_FMOVS,
		BGBCC_SH_REG_SP, ofs1, dreg2);
	if(p0>0)
		return(1);

	if(sctx->sreg_live&1)
//	if(1)
	{
		treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			treg, ofs1);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_SP, treg);
		BGBCC_SHX_EmitOpLdRegReg(sctx, BGBCC_SH_NMID_FMOVS, treg, dreg);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
		return(1);
	}

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
			BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg2);
		if(p1>0)
		{
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreStackOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int treg;
	int ofs1, dreg2, sw;
	int i, j, k;

//	ofs1=ofs+(sctx->frm_size);
	ofs1=ofs;

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg) &&
		!BGBCC_SHXC_GetStatusFpscrSz(ctx, sctx))
	{
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sw=(sctx->is_le!=0);

		p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+(1^sw), BGBCC_SH_REG_SP, ofs1+4);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(0^sw), BGBCC_SH_REG_SP, ofs1+0);
			return(1);
		}

//		if(sctx->sreg_live&1)
		if(1)
		{
			treg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				treg, ofs1+4);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_SP, treg);
			BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(1^sw), treg);
			BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(0^sw), treg);

			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, treg);
			return(1);
		}

#if 1
//		if((ofs1+8)<128)
		if((ofs1+4)<128)
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_SP, BGBCC_SH_REG_R0);
//			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
//				BGBCC_SH_REG_R0, ofs1+8);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_R0, ofs1+4);

//			BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg2+0, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(1^sw), BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(0^sw), BGBCC_SH_REG_R0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
#endif

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs1);
		if(p0>0)
		{
			BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(0^sw), BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_R0, 4);
			BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(1^sw), BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}
	
	dreg2=dreg;
	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg))
	{
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
	}else
	{
		BGBCC_SHXC_ClearStatusFpscrSz(ctx, sctx);
	}

	p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
		dreg2, BGBCC_SH_REG_SP, ofs1);
	if(p0>0)
		return(1);

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2, BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
		if(p1>0)
		{
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
	}

	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreFrameOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int ofs1;

	ofs1=ofs+(sctx->frm_size);
	return(BGBCC_SHXC_EmitStoreStackOfsFpReg(ctx, sctx, ofs1, dreg));
}

#if 1
int BGBCC_SHXC_EmitLoadBRegOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int dreg2, sw;
	int p0, p1;
	int i, j, k;

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg) &&
		!BGBCC_SHXC_GetStatusFpscrSz(ctx, sctx))
	{
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sw=(sctx->is_le!=0);

#if 1
		if(ofs<128)
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				breg, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_R0, ofs);

			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
				BGBCC_SH_REG_R0, dreg2+(0^sw));
			BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_FMOVS,
				BGBCC_SH_REG_R0, dreg2+(1^sw));
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
#endif

#if 0
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs+4);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
			breg, BGBCC_SH_REG_R0, dreg2+(1^sw));

		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs+0);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
			breg, BGBCC_SH_REG_R0, dreg2+(0^sw));
#endif

#if 1
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
			breg, BGBCC_SH_REG_R0, dreg2+(0^sw));
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
			breg, BGBCC_SH_REG_R0, dreg2+(1^sw));
#endif

		return(1);
	}

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_FMOVS,
			breg, BGBCC_SH_REG_R0, dreg);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreBRegOfsFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ofs, int dreg)
{
	int dreg2, sw;
	int p0, p1;
	int i, j, k;

	if(BGBCC_SHXC_EmitRegIsDpReg(ctx, sctx, dreg) &&
		!BGBCC_SHXC_GetStatusFpscrSz(ctx, sctx))
	{
		dreg2=BGBCC_SH_REG_FR0+((dreg&7)*2);
		sw=(sctx->is_le!=0);

#if 1
//		if((ofs+8)<128)
		if((ofs+4)<128)
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
				breg, BGBCC_SH_REG_R0);
//			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
//				BGBCC_SH_REG_R0, ofs+8);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
				BGBCC_SH_REG_R0, ofs+4);

//			BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
//				dreg2+0, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegStReg(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(1^sw), BGBCC_SH_REG_R0);

			BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_FMOVS,
				dreg2+(0^sw), BGBCC_SH_REG_R0);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
			return(1);
		}
#endif

#if 0
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs+4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+(1^sw), breg, BGBCC_SH_REG_R0);

		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs+0);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+(0^sw), breg, BGBCC_SH_REG_R0);
#endif

#if 1
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, ofs);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+(0^sw), breg, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, 4);
		BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg2+(1^sw), breg, BGBCC_SH_REG_R0);
#endif

		return(1);
	}

	p0=BGBCC_SHX_TryEmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
		dreg, breg, ofs);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_FMOVS,
			dreg, BGBCC_SH_REG_R0, breg);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLoadBRegIRegScFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	}

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);
	BGBCC_SHX_EmitOpLdReg2Reg(sctx, nmid,
		breg, BGBCC_SH_REG_R0, dreg);
	return(1);
}

int BGBCC_SHXC_EmitStoreBRegIRegScFpReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int nmid, int breg, int ireg, int sc, int dreg)
{
	int nm1, nm2;
	int p0, p1;
	int i, j, k;

	switch(sc)
	{
	case 1: nm1=-1; nm2=-1; break;
	case 2: nm1=BGBCC_SH_NMID_SHLL; nm2=-1; break;
	case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=-1; break;
	}

	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
		ireg, BGBCC_SH_REG_R0);
	if(nm1>=0)
		BGBCC_SHX_EmitOpReg(sctx, nm1, BGBCC_SH_REG_R0);

	BGBCC_SHX_EmitOpRegStReg2(sctx, nmid,
		dreg, BGBCC_SH_REG_R0, breg);
	return(1);
}
#endif

int BGBCC_SHXC_EmitTryGetFpRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	int creg;
	int i;

	/* value already in a register? */
	for(i=0; i<4; i++)
	{
		if(!((sctx->fregalc_save)&(1<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->fregalc_map[i]))
		{
			sctx->fregalc_live|=1<<i;
			if(fl&1)
				sctx->fregalc_dirty|=1<<i;

			if(sctx->fregalc_ltcnt[i]>0)
				sctx->fregalc_ltcnt[i]--;
			sctx->fregalc_utcnt[i]++;
			creg=bgbcc_shx_fcachereg[i];
			if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
			{
				if(creg&1)
					{ BGBCC_DBGBREAK }
				creg=bgbcc_shx_dcachereg[i];
			}
			return(creg);
		}
	}

	return(BGBCC_SH_REG_ZZR);
}

int BGBCC_SHXC_EmitGetDpRegisterI(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register zreg;
	int creg;
	int i;

	for(i=0; i<4; i++)
		if(sctx->fregalc_ltcnt[i]<255)
			sctx->fregalc_ltcnt[i]++;

//	zreg.val=CCXL_REGTY_TEMP|CCXL_REGID_REGMASK;
	zreg.val=CCXL_REGID_REG_Z;

	/* Check for registers not holding a live value. */
	for(i=0; i<4; i+=2)
	{
		if(!((sctx->fregalc_save)&(3<<i)))
			continue;
		if(!((sctx->fregalc_live)&(3<<i)))
		{
			BGBCC_SHXC_EmitSyncFpRegisterIndex(ctx, sctx, i+0);
			BGBCC_SHXC_EmitSyncFpRegisterIndex(ctx, sctx, i+1);

			sctx->fregalc_map[i+0]=reg;
			sctx->fregalc_map[i+1]=zreg;
			sctx->fregalc_utcnt[i]=1;
			sctx->fregalc_live|=3<<i;
			if(fl&1)
				sctx->fregalc_dirty|=1<<i;
			creg=bgbcc_shx_dcachereg[i];
			if(fl&2)
				{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
	}

	/* Check for unallocated registers. */
	for(i=0; i<4; i+=2)
	{
		if((sctx->fregalc_save)&(1<<i))
			continue;
		if((sctx->fregalc_live)&(3<<i))
			continue;
		
		sctx->fregalc_map[i+0]=reg;
		sctx->fregalc_map[i+1]=zreg;
		sctx->fregalc_utcnt[i]=1;
		sctx->fregalc_save|=3<<i;
		sctx->fregalc_live|=3<<i;
		if(fl&1)
			sctx->fregalc_dirty|=1<<i;

		creg=bgbcc_shx_fcachereg[i];
		BGBCC_SHXC_EmitSaveFrameReg(ctx, sctx, creg+0);
		BGBCC_SHXC_EmitSaveFrameReg(ctx, sctx, creg+1);

		creg=bgbcc_shx_dcachereg[i];
		if(fl&2)
			{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_SHXC_EmitGetFpRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register reg1;
	int creg, bi;
	int i;

	creg=BGBCC_SHXC_EmitTryGetFpRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
		return(BGBCC_SHXC_EmitGetDpRegisterI(ctx, sctx, reg, fl));

	for(i=0; i<4; i++)
		if(sctx->fregalc_ltcnt[i]<255)
			sctx->fregalc_ltcnt[i]++;

	/* Check for registers not holding a live value. */
	bi=-1;
	for(i=0; i<4; i++)
	{
		if(!((sctx->fregalc_save)&(1<<i)))
			continue;
		if(!((sctx->fregalc_live)&(1<<i)))
		{
			reg1=sctx->regalc_map[i];
			if((bi<0) || 
				(sctx->regalc_ltcnt[i]>=sctx->regalc_ltcnt[bi]))
					bi=i;
			if(BGBCC_SHXC_CheckVRegIsZRegP(ctx, sctx, reg1))
				{ bi=i; break; }
		}
	}

	if(bi>=0)
	{
		i=bi;

//		BGBCC_SHXC_EmitSyncFpRegisterIndex(ctx, sctx, i);

		BGBCC_SHXC_EmitSyncFpRegisterIndex(ctx, sctx, i);

		sctx->fregalc_map[i]=reg;
		sctx->fregalc_utcnt[i]=1;
		sctx->fregalc_live|=1<<i;
		if(fl&1)
			sctx->fregalc_dirty|=1<<i;
		creg=bgbcc_shx_fcachereg[i];
		if(fl&2)
			{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		return(creg);
	}

	/* Check for unallocated registers. */
	for(i=0; i<4; i++)
	{
		if((sctx->fregalc_save)&(1<<i))
			continue;

//		BGBCC_SHXC_EmitSyncFpRegisterIndex(ctx, sctx, i);
		
		sctx->fregalc_map[i]=reg;
		sctx->fregalc_utcnt[i]=1;
		sctx->fregalc_save|=1<<i;
		sctx->fregalc_live|=1<<i;
		if(fl&1)
			sctx->fregalc_dirty|=1<<i;

		creg=bgbcc_shx_fcachereg[i];		
		BGBCC_SHXC_EmitSaveFrameReg(ctx, sctx, creg);
		if(fl&2)
			{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitReleaseFpRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	static int rchk=0;
	int creg, isdbl;
	int i;

	isdbl=BGBCC_CCXL_IsRegDoubleP(ctx, reg);

	/* value in register? */
	for(i=0; i<4; i++)
	{
		if(!((sctx->fregalc_save)&(1<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->fregalc_map[i]))
		{
			if(isdbl)
				{ sctx->fregalc_live|=3<<i; }
			else
				{ sctx->fregalc_live|=1<<i; }

			if(sctx->fregalc_utcnt[i]>0)
				sctx->fregalc_utcnt[i]--;

			if(!sctx->fregalc_utcnt[i])
			{
				BGBCC_SHXC_EmitSyncFpRegisterIndex(ctx, sctx, i);

				if(isdbl)
				{
					sctx->fregalc_live&=~(3<<i);
				}
				else
				{
					sctx->fregalc_live&=~(1<<i);
				}
			}

			return(1);
		}
	}

	BGBCC_DBGBREAK

	return(0);
}


int BGBCC_SHXC_EmitSyncFpRegisterIndex(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx, int rgidx)
{
	ccxl_register reg;
	int creg;
	int i;

	i=rgidx;

	if(!((sctx->fregalc_save)&(1<<i)))
		return(0);

	if((sctx->fregalc_dirty)&(1<<i))
	{
		reg=sctx->fregalc_map[i];
		creg=bgbcc_shx_fcachereg[i];		

		if((reg.val&CCXL_REGID_REGMASK)==CCXL_REGID_REGMASK)
		{
//			sctx->fregalc_utcnt[i]=0;
			sctx->fregalc_dirty&=~(1<<i);
//			sctx->fregalc_live&=~(1<<i);
			return(0);
		}

		if(BGBCC_CCXL_IsRegDoubleP(ctx, reg))
			creg=bgbcc_shx_dcachereg[i];

		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);

		sctx->fregalc_dirty&=~(1<<i);
	}

	return(1);
}

int BGBCC_SHXC_EmitSyncFpRegisters(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	ccxl_register reg;
	int creg;
	int i;

	/* value in register? */
	for(i=0; i<4; i++)
	{
		BGBCC_SHXC_EmitSyncFpRegisterIndex(ctx, sctx, i);
		sctx->fregalc_utcnt[i]=0;
		sctx->fregalc_live&=~(1<<i);
	}

	return(0);
}

int BGBCC_SHXC_EmitLabelFlushFpRegisters(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	int i;

	for(i=0; i<4; i++)
	{
		if(!((sctx->fregalc_save)&(1<<i)))
			continue;

		sctx->fregalc_map[i].val=-1;
		sctx->fregalc_utcnt[i]=0;
		sctx->fregalc_ltcnt[i]=0;
		sctx->fregalc_dirty&=~(1<<i);
		sctx->fregalc_live&=~(1<<i);
	}

	return(0);
}



int BGBCC_SHXC_EmitBinaryVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i, j, k;

	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_FADD;	nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_FSUB;	nm2=-1; break;
	case CCXL_BINOP_MUL:	nm1=BGBCC_SH_NMID_FMUL;	nm2=-1; break;
	case CCXL_BINOP_DIV:	nm1=BGBCC_SH_NMID_FDIV;	nm2=-1; break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx); }

		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if((cdreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
		}

		if(nm2>=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg, BGBCC_SH_REG_FR0);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_FR0, cdreg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
		}
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegFloat(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_SHXC_EmitBinaryVRegVRegFloat(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	ccxl_register treg;
	int csreg, ctreg, czreg, cdreg;
	int nm1, nm2;
	s32 imm;
	int i, j, k;

	if(opr==CCXL_UNOP_LNOT)
	{
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BT;

//		BGBCC_SHXC_EmitCompareVRegVRegVRegFloat(ctx, sctx, type, dreg, )
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx); }

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if((csreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
		}

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if((csreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
		}

		ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
//		czreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);

		BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, csreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, ctreg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
			ctreg, BGBCC_SH_REG_FPUL);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FLOAT,
			BGBCC_SH_REG_FPUL, cdreg);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
//		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	if((opr==CCXL_UNOP_INC) || (opr==CCXL_UNOP_DEC))
	{
		j=CCXL_BINOP_ADD;
		if(opr==CCXL_UNOP_DEC)
			j=CCXL_BINOP_SUB;
	
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			BGBCC_CCXL_GetRegForDoubleValue(ctx, &treg, 1.0);
		else
			BGBCC_CCXL_GetRegForFloatValue(ctx, &treg, 1.0);
	
		BGBCC_SHXC_EmitBinaryVRegVRegVRegFloat(ctx, sctx,
			type, dreg, j, sreg, treg);
		return(1);
	}

	switch(opr)
	{
	case CCXL_UNOP_MOV:	nm1=BGBCC_SH_NMID_FMOV; nm2=-1; break;
	case CCXL_UNOP_NEG:	nm1=BGBCC_SH_NMID_FNEG; nm2=-1; break;
//	case CCXL_UNOP_ABS:	nm1=BGBCC_SH_NMID_FABS; nm2=-1; break;
	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx); }

		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			csreg=cdreg;
		}else
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_FMOV, csreg, cdreg);
		}

		BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitCompareVRegVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, swst;
	
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BT; swst=0;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BF; swst=0;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BT; swst=1;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BT; swst=0;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BF; swst=0;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BF; swst=1;
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx); }

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if((csreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_R0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((cdreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_R0)
				{ BGBCC_DBGBREAK }
		}

		if(swst)
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, ctreg);
		else
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, csreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, cdreg);
		
		if(nm2==BGBCC_SH_NMID_BF)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_NEG, cdreg, cdreg);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD, cdreg, 1);
		}
		
//		BGBCC_SHX_EmitOpLabel(sctx, nm2, lbl);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegVRegFloat(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	int nm1, nm2, sw;
	
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BT; sw=0;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_FCMPEQ;
		nm2=BGBCC_SH_NMID_BF; sw=0;
		break;
	case CCXL_CMP_LT:
//		nm1=BGBCC_SH_NMID_FCMPGE;
		nm1=BGBCC_SH_NMID_FCMPGT;
//		nm2=BGBCC_SH_NMID_BF;
		nm2=BGBCC_SH_NMID_BT; sw=1;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BT; sw=0;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_FCMPGT;
		nm2=BGBCC_SH_NMID_BF; sw=0;
		break;
	case CCXL_CMP_GE:
//		nm1=BGBCC_SH_NMID_FCMPGE;
		nm1=BGBCC_SH_NMID_FCMPGT;
//		nm2=BGBCC_SH_NMID_BT;
		nm2=BGBCC_SH_NMID_BF; sw=1;
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
			{ BGBCC_SHXC_SetStatusFpscrDouble(ctx, sctx); }
		else
			{ BGBCC_SHXC_SetStatusFpscrFloat(ctx, sctx); }

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		if(BGBCC_CCXL_TypeDoubleP(ctx, type))
		{
			if((csreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK3)!=BGBCC_SH_REG_DR0)
				{ BGBCC_DBGBREAK }
		}
		else
		{
			if((csreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
			if((ctreg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
				{ BGBCC_DBGBREAK }
		}

		if(sw)
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, ctreg);
		else
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
