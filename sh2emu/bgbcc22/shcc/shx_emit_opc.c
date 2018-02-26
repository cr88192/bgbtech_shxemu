#include <bgbccc.h>

int BGBCC_SHX_EmitCheckRegSafe(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int vs;

	if(ctx->is_simpass || ctx->is_rawasm)
		return(0);

//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0))
	{
		if((reg&15)>=8)
		{
			if((reg&15)==15)
				return(0);
		
//			vs=ctx->reg_save|ctx->reg_vsave;
			vs=ctx->reg_save;
			if(!(vs&(1<<(reg&15))))
				{ BGBCC_DBGBREAK; }
			return(1);
		}
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
		{
//			vs=ctx->freg_save|ctx->freg_vsave;
			vs=ctx->freg_save;
			if(!(vs&(1<<(reg&15))))
				{ BGBCC_DBGBREAK; }
			return(1);
		}
	}

	return(0);
}

int BGBCC_SHX_EmitCheckModeDQ(BGBCC_SHX_Context *ctx)
{
	if(ctx->cur_srmode&BGBCC_SH_SRFL_DQ)
		return(1);
	return(0);
}

int BGBCC_SHX_EmitCheckModeJQ(BGBCC_SHX_Context *ctx)
{
	if(ctx->cur_srmode&BGBCC_SH_SRFL_JQ)
		return(1);
	return(0);
}

/** Used for I-Forms which require DQ Set */
int BGBCC_SHX_EmitCheckModeSetDQ(BGBCC_SHX_Context *ctx, int nmid)
{
//	return(0);

	if(!(ctx->cur_srmode&BGBCC_SH_SRFL_JQ))
	{
		ctx->cnt_set_dq1++;
		return(0);
	}

	ctx->cnt_set_dq1++;
	if(ctx->cur_srmode&BGBCC_SH_SRFL_DQ)
	{
		return(1);
//		return(0);
	}
	return(0);
}

int BGBCC_SHX_EmitCheckModeClrDQ_DbgReject(
	BGBCC_SHX_Context *ctx, int nmid)
{
#if 0
	if(nmid==BGBCC_SH_NMID_MOVW)	return(1);
	if(nmid==BGBCC_SH_NMID_SHAD)	return(1);
	if(nmid==BGBCC_SH_NMID_SHLD)	return(1);
	if(nmid==BGBCC_SH_NMID_SHAR)	return(1);
	if(nmid==BGBCC_SH_NMID_SHLR)	return(1);
	return(0);
#endif

#if 0
	if(nmid==BGBCC_SH_NMID_MULL)	return(0);
//	if(nmid==BGBCC_SH_NMID_SHAD)	return(1);
//	if(nmid==BGBCC_SH_NMID_SHLD)	return(1);
//	if(nmid==BGBCC_SH_NMID_SHAR)	return(1);
//	if(nmid==BGBCC_SH_NMID_SHLR)	return(1);
	return(1);
#endif

	return(0);
}

/** Used for I-Forms which require DQ Clear */
int BGBCC_SHX_EmitCheckModeClrDQ(BGBCC_SHX_Context *ctx, int nmid)
{
	if(!ctx->is_addr64)
		return(1);
//	return(0);

	if(!(ctx->cur_srmode&BGBCC_SH_SRFL_JQ))
	{
		ctx->cnt_set_dq0++;
		if(!ctx->is_addr64)
		{
			if(BGBCC_SHX_EmitCheckModeClrDQ_DbgReject(ctx, nmid))
				return(0);
			return(1);
		}
		return(0);
	}

	ctx->cnt_set_dq0++;
	if(ctx->cur_srmode&BGBCC_SH_SRFL_DQ)
		return(0);

	if(BGBCC_SHX_EmitCheckModeClrDQ_DbgReject(ctx, nmid))
		return(0);

	return(1);
}

/** Check whether the register is a normal register in the current mode. */
int BGBCC_SHX_EmitCheckModalReg(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
	{
		if(ctx->cur_srmode&BGBCC_SH_SRFL_DQ)
			return(0);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
	{
		if(ctx->cur_srmode&BGBCC_SH_SRFL_DQ)
			return(1);
		return(0);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
	{
		if(ctx->cur_srmode&BGBCC_SH_SRFL_DQ)
			return(0);
		return(1);
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(0);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(0);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(0);

	return(1);
}

int BGBCC_SHX_EmitCheckRegQuad(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);
	return(0);
}

int BGBCC_SHX_EmitCheckRegDouble(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR16)
		return(1);
	return(0);
}

int BGBCC_SHX_EmitCheckRegExt4(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_XF0)
		return(1);

	return(0);
}

int BGBCC_SHX_EmitGetRegAsReg3(BGBCC_SHX_Context *ctx, int reg)
{
	if(!ctx->has_bjx1r3mov)
		return(-1);

	if(	((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0))
	{
		if((reg&15)==0)return(7);
		if((reg&15)==15)return(-1);
		if((reg&15)>=8)
			return(reg&7);
		return(-1);
	}
	return(-1);
}

int BGBCC_SHX_EmitOpNone(BGBCC_SHX_Context *ctx, int nmid)
{
	if(!BGBCC_SHX_TryEmitOpNone(ctx, nmid))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpNone(BGBCC_SHX_Context *ctx, int nmid)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_CLRT:	opw=0x0008; break;
	case BGBCC_SH_NMID_SETT:	opw=0x0018; break;
	case BGBCC_SH_NMID_CLRMAC:	opw=0x0028; break;
	case BGBCC_SH_NMID_LDTLB:	opw=0x0038; break;
	case BGBCC_SH_NMID_CLRS:	opw=0x0048; break;
	case BGBCC_SH_NMID_SETS:	opw=0x0058; break;
	case BGBCC_SH_NMID_NOTT:	opw=0x0068; break;

	case BGBCC_SH_NMID_NOP:		opw=0x0009; break;
	case BGBCC_SH_NMID_DIV0U:	opw=0x0019; break;

	case BGBCC_SH_NMID_RTS:		opw=0x000B; break;
	case BGBCC_SH_NMID_SLEEP:	opw=0x001B; break;
	case BGBCC_SH_NMID_RTE:		opw=0x002B; break;
	case BGBCC_SH_NMID_BRK:		opw=0x003B; break;

	case BGBCC_SH_NMID_RTSN:
//		if(!ctx->has_movi20)
//			break;
		opw=0x006B; break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

/** Registers which can be used as a normal GPR */
int BGBCC_SHX_EmitCheckRegGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if(!BGBCC_SHX_EmitCheckModalReg(ctx, reg))
		return(0);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);

	return(0);
}

/** Registers which can be used as an address */
int BGBCC_SHX_EmitCheckRegAddrGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0) && !ctx->is_addr64)
		return(1);
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0) && ctx->is_addr64)
		return(1);
//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
//		return(1);
	return(0);
}

/** Registers which can be used as a basic GPR, any width */
int BGBCC_SHX_EmitCheckRegBaseGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	return(0);
}

/** Registers which can be used as a basic GPR, any width */
int BGBCC_SHX_EmitCheckRegNonBaseGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);
	return(0);
}

/** Registers which can be used as an extended GPR */
int BGBCC_SHX_EmitCheckRegExtGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RH0)
//		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RD16)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ16)
		return(1);

	return(0);
}

int BGBCC_SHX_EmitCheckRegFPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);

//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_XF0)
//		return(1);
//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR16)
//		return(1);

	return(0);
}

int BGBCC_SHX_EmitCheckRegExtFPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_XF0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR16)
		return(1);

	return(0);
}

int BGBCC_SHX_EmitOpReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_SHX_TryEmitOpReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int opw, opw1, opw2;

//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ reg=BGBCC_SH_REG_FR0+(2*(reg&7)); }

	if(((nmid==BGBCC_SH_NMID_CMPQEQ) ||
		(nmid==BGBCC_SH_NMID_TSTQ)) && 
		!ctx->is_betav)
	{
//		return(BGBCC_SHX_TryEmitOpRegReg(ctx, nmid, reg, reg));
		return(BGBCC_SHX_TryEmitOpRegReg(ctx,
			BGBCC_SH_NMID_TSTQ, reg, reg));
	}

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

#if 1
	switch(nmid)
	{
	case BGBCC_SH_NMID_FSTS:	case BGBCC_SH_NMID_FLDS:
	case BGBCC_SH_NMID_FLOAT:	case BGBCC_SH_NMID_FTRC:
	case BGBCC_SH_NMID_FNEG:	case BGBCC_SH_NMID_FABS:
	case BGBCC_SH_NMID_FSQRT:	case BGBCC_SH_NMID_FSRRA:
	case BGBCC_SH_NMID_FCNVSD:	case BGBCC_SH_NMID_FCNVDS:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if(reg&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FMOV:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_SZ)
		{
			if(reg&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FLDI0:
	case BGBCC_SH_NMID_FLDI1:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
			{ BGBCC_DBGBREAK }
		break;
	}
#endif

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BSRF:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0003|((reg&15)<<8); break;
	case BGBCC_SH_NMID_BRAF:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0023|((reg&15)<<8); break;

	case BGBCC_SH_NMID_MOVT:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw=0x0029|((reg&15)<<8); break;
	case BGBCC_SH_NMID_MOVRT:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))		break;
		opw=0x0039|((reg&15)<<8); break;

	case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TST:		/* pseudo-op */
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x2008|((reg&15)<<8)|((reg&15)<<4); break;

	case BGBCC_SH_NMID_CMPQEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TSTQ:	/* pseudo-op */
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))		break;
		if(ctx->is_betav)
			{ opw=0x200C|((reg&15)<<8)|((reg&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x2008|((reg&15)<<8)|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SHLL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4000|((reg&15)<<8); break;
	case BGBCC_SH_NMID_DT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4010|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHAL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4020|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLR:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x4001|((reg&15)<<8); break;
	case BGBCC_SH_NMID_CMPPZ:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4011|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHAR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4021|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4004|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ROTCL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4024|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4005|((reg&15)<<8); break;
	case BGBCC_SH_NMID_CMPPL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4015|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ROTCR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4025|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLL2:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x4008|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLL8:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x4018|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLL16:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x4028|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLR2:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x4009|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLR8:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x4019|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLR16:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x4029|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLL1:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x300C|((reg&15)<<8)|((reg&15)<<4);			break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x400B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_TASB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(ctx->is_betav)								break;
		opw=0x401B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x402B|((reg&15)<<8); break;

	case BGBCC_SH_NMID_FSTS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))	break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF00D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF01D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLOAT:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF02D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FTRC:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF03D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FNEG:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF04D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FABS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF05D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FSQRT:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF06D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FSRRA:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF07D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDI0:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF08D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDI1:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF09D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FCNVSD:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF0AD|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FCNVDS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, reg))		break;
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF0BD|((reg&15)<<8); break;

	case BGBCC_SH_NMID_PUSH:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(ctx->has_bjx1jmp)
			{ opw=0x4048|((reg&15)<<8); }
		else
			{ opw=0x2F06|((reg&15)<<4); }
		break;
	case BGBCC_SH_NMID_POP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(ctx->has_bjx1jmp)
			{ opw=0x4058|((reg&15)<<8); }
		else
			{ opw=0x60F6|((reg&15)<<8); }
		break;

	case BGBCC_SH_NMID_STHF16:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!ctx->has_bjx1mov)	break;
		opw=0x4061|((reg&15)<<8); break;
	case BGBCC_SH_NMID_LDHF16:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!ctx->has_bjx1mov)	break;
		opw=0x4071|((reg&15)<<8); break;

	case BGBCC_SH_NMID_EXTUL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!ctx->is_addr64)	break;
		opw=0x4044|((reg&15)<<8); break;
	case BGBCC_SH_NMID_EXTSL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		if(!ctx->is_addr64)	break;
		opw=0x4054|((reg&15)<<8); break;

	default:
		break;
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, reg))
	{
		opw1=0xCC00;
		
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, reg))
			opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, reg))
			opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, reg))
			opw1|=0x0002;


		switch(nmid)
		{
		case BGBCC_SH_NMID_PUSH:
			if((reg&31)>=16)
				{ opw=0x4068|((reg&15)<<8); }
			else
				{ opw=0x4048|((reg&15)<<8); }
			break;
		case BGBCC_SH_NMID_POP:
			if((reg&31)>=16)
				{ opw=0x4078|((reg&15)<<8); }
			else
				{ opw=0x4058|((reg&15)<<8); }
			break;

//		case BGBCC_SH_NMID_BSRF:
//			opw=opw1; opw2=0x0003|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_BRAF:
//			opw=opw1; opw2=0x0023|((reg&15)<<8); break;

		case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
		case BGBCC_SH_NMID_TST:		/* pseudo-op */
			opw=opw1; opw2=0x2008|((reg&15)<<8)|((reg&15)<<4); break;

		case BGBCC_SH_NMID_EXTUL:
			opw=opw1|0x9; opw2=0x600D|((reg&15)<<8)|((reg&15)<<4); break;
		case BGBCC_SH_NMID_EXTSL:
			opw=opw1|0x9; opw2=0x600F|((reg&15)<<8)|((reg&15)<<4); break;

//		case BGBCC_SH_NMID_SHLL:
//			opw2=0x4000|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_DT:
//			opw2=0x4010|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_SHAL:
//			opw2=0x4020|((reg&15)<<8); break;

//		case BGBCC_SH_NMID_SHLR:
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
//			opw2=0x4001|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_CMPPZ:
//			opw=opw1; opw2=0x4011|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_SHAR:
//			opw2=0x4021|((reg&15)<<8); break;

//		case BGBCC_SH_NMID_ROTL:
//			opw2=0x4004|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_ROTCL:
//			opw2=0x4024|((reg&15)<<8); break;

//		case BGBCC_SH_NMID_ROTR:
//			opw2=0x4005|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_CMPPL:
//			opw=opw1; opw2=0x4015|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_ROTCR:
//			opw2=0x4025|((reg&15)<<8); break;

//		case BGBCC_SH_NMID_SHLL2:
//			opw=opw1; opw2=0x4008|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_SHLL8:
//			opw=opw1; opw2=0x4018|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_SHLL16:
//			opw=opw1; opw2=0x4028|((reg&15)<<8); break;

//		case BGBCC_SH_NMID_SHLR2:
//			opw=opw1; opw2=0x4009|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_SHLR8:
//			opw=opw1; opw2=0x4019|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_SHLR16:
//			opw=opw1; opw2=0x4029|((reg&15)<<8); break;

//		case BGBCC_SH_NMID_SHLL1:
//			opw=opw1; opw2=0x300C|((reg&15)<<8)|((reg&15)<<4);	break;

//		case BGBCC_SH_NMID_JSR:
//			opw=opw1; opw2=0x400B|((reg&15)<<8); break;
//		case BGBCC_SH_NMID_JMP:
//			opw=opw1; opw2=0x402B|((reg&15)<<8); break;

		case BGBCC_SH_NMID_FSTS:
			opw=opw1; opw2=0xF00D|((reg&15)<<8); break;
		case BGBCC_SH_NMID_FLDS:
			opw=opw1; opw2=0xF01D|((reg&15)<<8); break;

		default:
			break;
		}

	}

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, reg))
	{
		opw1=0xCE00;
		
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, reg))
			opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, reg))
			opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, reg))
			opw1|=0x0020;


		switch(nmid)
		{
		case BGBCC_SH_NMID_PUSH:
			if((reg&31)>=16)
				{ opw=0x4068|((reg&15)<<8); }
			else
				{ opw=0x4048|((reg&15)<<8); }
			break;
		case BGBCC_SH_NMID_POP:
			if((reg&31)>=16)
				{ opw=0x4078|((reg&15)<<8); }
			else
				{ opw=0x4058|((reg&15)<<8); }
			break;

		case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
		case BGBCC_SH_NMID_TST:		/* pseudo-op */
			opw=opw1|0x08;
			opw2=0x400F|((reg&15)<<8)|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_CMPQEQ:	/* pseudo-op */
		case BGBCC_SH_NMID_TSTQ:		/* pseudo-op */
			opw=opw1|0x88;
			opw2=0x400F|((reg&15)<<8)|((reg&15)<<4);
			break;

		case BGBCC_SH_NMID_FSTS:
			opw=opw1|0x0; opw2=0xF00D|((reg&15)<<8); break;
		case BGBCC_SH_NMID_FLDS:
			opw=opw1|0x1; opw2=0xF00D|((reg&15)<<8); break;

		case BGBCC_SH_NMID_EXTUL:		/* pseudo-op */
			opw=opw1|0x8D;
			opw2=0x600F|((reg&15)<<8)|((reg&15)<<4);
			break;
		case BGBCC_SH_NMID_EXTSL:		/* pseudo-op */
			opw=opw1|0x8F;
			opw2=0x600F|((reg&15)<<8)|((reg&15)<<4);
			break;

		default:
			break;
		}

	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpImm(BGBCC_SHX_Context *ctx, int nmid, int imm)
{
	if(!BGBCC_SHX_TryEmitOpImm(ctx, nmid, imm))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpImm(BGBCC_SHX_Context *ctx, int nmid, int imm)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_TRAPA:
		if(((byte)imm)!=imm)
			break;
		opw=0xC300|imm; break;

	case BGBCC_SH_NMID_PSETMD4:
		if(!ctx->has_bjx1ari)	break;
		if((imm&15)!=imm)		break;
		opw=0xF0DD|((imm&15)<<8);
		break;
	case BGBCC_SH_NMID_PSETMD12:
		if(!ctx->has_bjx1ari)	break;
		if((imm&4095)!=imm)		break;
		opw=0x8E00|((imm>>4)&255);
		opw2=0xF0DD|((imm&15)<<8);
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpMReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_SHX_TryEmitOpMReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpMReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int opw, opw2;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_PREF:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0083|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBI:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0093|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x00A3|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBWB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x00B3|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ICBI:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x00E3|((reg&15)<<8); break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x400B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_TASB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x401B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x402B|((reg&15)<<8); break;

	default:
		break;
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	int opw, opw1, opw2;
	int rm3, rn3;

//	if((nmid==BGBCC_SH_NMID_MOV) && (rm==rn))
//		return(1);

	if(rm==rn)
	{
		if(nmid==BGBCC_SH_NMID_MOV)
			return(1);

		if((nmid==BGBCC_SH_NMID_EXTUL) ||
			(nmid==BGBCC_SH_NMID_EXTSL))
		{
			return(BGBCC_SHX_TryEmitOpReg(ctx, nmid, rn));
		}
	}

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }
//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

#if 1
	switch(nmid)
	{
	case BGBCC_SH_NMID_FADD:	case BGBCC_SH_NMID_FSUB:
	case BGBCC_SH_NMID_FMUL:	case BGBCC_SH_NMID_FDIV:
	case BGBCC_SH_NMID_FCMPEQ:	case BGBCC_SH_NMID_FCMPGT:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if((rm|rn)&1)
				{ BGBCC_DBGBREAK }
		}
		break;

//	case BGBCC_SH_NMID_FSTS:
	case BGBCC_SH_NMID_FLOAT:
	case BGBCC_SH_NMID_FCNVSD:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if(rn&1)
				{ BGBCC_DBGBREAK }
		}
		break;

//	case BGBCC_SH_NMID_FLDS:
	case BGBCC_SH_NMID_FTRC:
	case BGBCC_SH_NMID_FCNVDS:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if(rm&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FMOV:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_SZ)
		{
			if((rm|rn)&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FMAC:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
			{ BGBCC_DBGBREAK }
		break;
	}
#endif

	opw=-1; opw2=-1;
	if((opw<0) && ctx->is_addr64 &&
		ctx->has_bjx1r3mov)
	{
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
		{
#if 1
			switch(nmid)
			{
			case BGBCC_SH_NMID_CMPQEQ:
				opw=0xC500|((rn3&7)<<4)|((rm3&7)<<0); break;
			case BGBCC_SH_NMID_TSTQ:
				opw=0xC508|((rn3&7)<<4)|((rm3&7)<<0); break;
			case BGBCC_SH_NMID_CMPQHI:
				opw=0xC580|((rn3&7)<<4)|((rm3&7)<<0); break;
			case BGBCC_SH_NMID_CMPQGT:
				opw=0xC588|((rn3&7)<<4)|((rm3&7)<<0); break;
			}
#endif
		}
	}

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_STC:
	case BGBCC_SH_NMID_STS:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if((rm&0xF0)==BGBCC_SH_REG_CR0)
			{ opw=0x0002|((rn&15)<<8)|((rm&15)<<4); break; }
		if((rm&0xF0)==BGBCC_SH_REG_SR0)
			{ opw=0x000A|((rn&15)<<8)|((rm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_LDC:
	case BGBCC_SH_NMID_LDS:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if((rn&0xF0)==BGBCC_SH_REG_CR0)
			{ opw=0x400E|((rm&15)<<8)|((rn&15)<<4); break; }
		if((rn&0xF0)==BGBCC_SH_REG_SR0)
			{ opw=0x400A|((rm&15)<<8)|((rn&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MULL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
		{	opw=0x200D|((rn&15)<<8)|((rm&15)<<4);
			break;	}
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))	break;
		opw=0x0007|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_MULQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)								break;
//		if(ctx->is_betav)
//		{	opw=0x200E|((rn&15)<<8)|((rm&15)<<4);
//			break;	}
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x0007|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_DIV0S:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
		if(ctx->is_betav)
		{	if(rm!=BGBCC_SH_REG_R5)						break;
			if(rn!=BGBCC_SH_REG_R4)						break;
			opw=0x05F8;		break;		}
		opw=0x2007|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_TST:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
		opw=0x2008|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_AND:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x2009|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x200A|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_OR:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x200B|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_CMPSTR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)							break;
		opw=0x200C|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_TSTQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!ctx->is_betav)							break;
		opw=0x200C|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_XTRCT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)
		{
			if((rm==BGBCC_SH_REG_R4) && (rn==BGBCC_SH_REG_R0))
				{ opw=0x08F8;	break; }
			if((rm==BGBCC_SH_REG_R0) && (rn==BGBCC_SH_REG_R4))
				{ opw=0x09F8;	break; }
			break;
		}
		opw=0x200D|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MULUW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
//		if(ctx->is_betav)							break;
		opw=0x200E|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MULSW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
//		if(ctx->is_betav)							break;
		opw=0x200F|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_CMPEQ:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3000|((rn&15)<<8)|((rm&15)<<4);		break;
	/* 3001=Esc32 */
	case BGBCC_SH_NMID_CMPHS:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)							break;
		opw=0x3002|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_CMPGE:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)							break;
		opw=0x3003|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_DIV1:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)
		{	if(rm!=BGBCC_SH_REG_R5)					break;
			if(rn!=BGBCC_SH_REG_R4)					break;
			opw=0x04F8;	break;	}
		opw=0x3004|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_DMULU:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3005|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_CMPHI:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3006|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_CMPGT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3007|((rn&15)<<8)|((rm&15)<<4);		break;

	case BGBCC_SH_NMID_CMPQEQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x3001|((rn&15)<<8)|((rm&15)<<4);		break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x3000|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_CMPQHS:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
			break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x3002|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_CMPQGE:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
			break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x3003|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_CMPQHI:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x3002|((rn&15)<<8)|((rm&15)<<4);		break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x3006|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_CMPQGT:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x3003|((rn&15)<<8)|((rm&15)<<4);		break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x3007|((rn&15)<<8)|((rm&15)<<4);			break;

	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
		opw=0x3008|((rn&15)<<8)|((rm&15)<<4);		break;
	/* 3009=Esc32 */
	case BGBCC_SH_NMID_SUBC:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x300A|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_SUBV:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)							break;
		opw=0x300B|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_ADD:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
		opw=0x300C|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_DMULS:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x300D|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_ADDC:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x300E|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_ADDV:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)							break;
		opw=0x300F|((rn&15)<<8)|((rm&15)<<4);		break;

	case BGBCC_SH_NMID_SHAD:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))	break;
		opw=0x400C|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_SHLD:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))	break;
		opw=0x400D|((rn&15)<<8)|((rm&15)<<4);			break;

	case BGBCC_SH_NMID_SHADQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		if(ctx->is_betav)								break;
		opw=0x400C|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_SHLDQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		if(ctx->is_betav)								break;
		opw=0x400D|((rn&15)<<8)|((rm&15)<<4);			break;

	/* 400E: LDC forms */
	case BGBCC_SH_NMID_MACW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)							break;
		opw=0x400F|((rn&15)<<8)|((rm&15)<<4);		break;

	/* 6oo0..6oo2: MOV @Rm, Rn */
	case BGBCC_SH_NMID_MOV:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
		if(ctx->is_betav)
		{	opw=0x400F|((rn&15)<<8)|((rm&15)<<4);
			break;	}
		opw=0x6003|((rn&15)<<8)|((rm&15)<<4);		break;
	/* 6oo4..6oo6: MOV @Rm+, Rn */
	case BGBCC_SH_NMID_NOT:
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
		if(ctx->is_betav)
		{	opw=0x300B|((rn&15)<<8)|((rm&15)<<4);
			break;	}
		opw=0x6007|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_SWAPB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6008|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_SWAPW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6009|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_NEGC:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x600A|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_NEG:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x600B|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_EXTUB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x600C|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_EXTUW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x600D|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_EXTSB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x600E|((rn&15)<<8)|((rm&15)<<4);			break;
	case BGBCC_SH_NMID_EXTSW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x600F|((rn&15)<<8)|((rm&15)<<4);			break;

	case BGBCC_SH_NMID_FADD:	opw=0xF000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FSUB:	opw=0xF001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMUL:	opw=0xF002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FDIV:	opw=0xF003|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FCMPEQ:	opw=0xF004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FCMPGT:	opw=0xF005|((rn&15)<<8)|((rm&15)<<4); break;
	/* Foo6..FooB: FMOV.S forms */
	case BGBCC_SH_NMID_FMOV:	opw=0xF00C|((rn&15)<<8)|((rm&15)<<4); break;
	/* FooD: Single Arg Forms */
	case BGBCC_SH_NMID_FMAC:	opw=0xF00E|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_FSTS:
		if(rm!=BGBCC_SH_REG_FPUL)break;
		opw=0xF00D|((rn&15)<<8); break;
	case BGBCC_SH_NMID_FLDS:
		if(rn!=BGBCC_SH_REG_FPUL)break;
		opw=0xF01D|((rm&15)<<8); break;
	case BGBCC_SH_NMID_FLOAT:
		if(rm!=BGBCC_SH_REG_FPUL)break;
		opw=0xF02D|((rn&15)<<8); break;
	case BGBCC_SH_NMID_FTRC:
		if(rn!=BGBCC_SH_REG_FPUL)break;
		opw=0xF03D|((rm&15)<<8); break;
	case BGBCC_SH_NMID_FCNVSD:
		if(rm!=BGBCC_SH_REG_FPUL)break;
		opw=0xF0AD|((rn&15)<<8); break;
	case BGBCC_SH_NMID_FCNVDS:
		if(rn!=BGBCC_SH_REG_FPUL)break;
		opw=0xF0BD|((rm&15)<<8); break;

	case BGBCC_SH_NMID_STHF16:
		if(!ctx->has_bjx1mov)	break;
		if(rm!=BGBCC_SH_REG_FPUL)
		{
			opw=0xF01D|((rm&15)<<8);
			opw2=0x4061|((rn&15)<<8);
			break;
		}
		opw=0x4061|((rn&15)<<8); break;
	case BGBCC_SH_NMID_LDHF16:
		if(!ctx->has_bjx1mov)	break;
		if(rn!=BGBCC_SH_REG_FPUL)
		{
			opw=0x4071|((rm&15)<<8);
			opw2=0xF00D|((rn&15)<<8);
			break;
		}
		opw=0x4071|((rm&15)<<8); break;

	default:
		break;
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_STC) ||
			(nmid==BGBCC_SH_NMID_STS)) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_LDC) ||
			(nmid==BGBCC_SH_NMID_LDS)))
	{
		opw1=0xCC00;
		
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm) || 
				BGBCC_SHX_EmitCheckRegQuad(ctx, rn))
			opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))
			opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))
			opw1|=0x0002;

		switch(nmid)
		{
#if 0
		case BGBCC_SH_NMID_STC:
		case BGBCC_SH_NMID_STS:
			if((rm&0xF0)==BGBCC_SH_REG_CR0)
				{ opw=opw1; opw2=0x0002|((rn&15)<<8)|((rm&15)<<4); break; }
			if((rm&0xF0)==BGBCC_SH_REG_SR0)
				{ opw=opw1; opw2=0x000A|((rn&15)<<8)|((rm&15)<<4); break; }
			break;

		case BGBCC_SH_NMID_LDC:
		case BGBCC_SH_NMID_LDS:
			opw1&=~0x0006;
			if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0004;
			if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0002;
			if((rn&0xF0)==BGBCC_SH_REG_CR0)
				{ opw=opw1; opw2=0x400E|((rm&15)<<8)|((rn&15)<<4); break; }
			if((rn&0xF0)==BGBCC_SH_REG_SR0)
				{ opw=opw1; opw2=0x400A|((rm&15)<<8)|((rn&15)<<4); break; }
			break;
#endif

		case BGBCC_SH_NMID_MULL:
			opw=opw1&(~8); opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MULQ:
			opw=opw1|8; opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_TST:
			opw=opw1&(~8); opw2=0x2008|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_AND:
			opw=opw1&(~8); opw2=0x2009|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_XOR:
			opw=opw1&(~8); opw2=0x200A|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_OR:
			opw=opw1&(~8); opw2=0x200B|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_TSTQ:
			opw=opw1|8; opw2=0x2008|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_CMPEQ:
			opw=opw1&(~8); opw2=0x3000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPHS:
			opw=opw1&(~8); opw2=0x3002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPGE:
			opw=opw1&(~8); opw2=0x3003|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_DIV1:
			opw=opw1&(~8); opw2=0x3004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_DMULU:
			opw=opw1&(~8); opw2=0x3005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPHI:
			opw=opw1&(~8); opw2=0x3006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPGT:
			opw=opw1&(~8); opw2=0x3007|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SUB:
			opw=opw1&(~8); opw2=0x3008|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SUBC:
			opw=opw1&(~8); opw2=0x300A|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SUBV:
			opw=opw1&(~8); opw2=0x300B|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_ADD:
			opw=opw1&(~8); opw2=0x300C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_DMULS:
			opw=opw1; opw2=0x300D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_ADDC:
			opw=opw1&(~8); opw2=0x300E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_ADDV:
			opw=opw1&(~8); opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_CMPQEQ:
			opw=opw1|8; opw2=0x3000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQHS:
			opw=opw1|8; opw2=0x3002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQGE:
			opw=opw1|8; opw2=0x3003|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQHI:
			opw=opw1|8; opw2=0x3006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQGT:
			opw=opw1|8; opw2=0x3007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_SHAD:
			opw=opw1&(~8); opw2=0x400C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHLD:
			opw=opw1&(~8); opw2=0x400D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHADQ:
			opw=opw1|8; opw2=0x400C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHLDQ:
			opw=opw1|8; opw2=0x400D|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_DMULU:
//			opw=opw1; opw2=0x3005|((rn&15)<<8)|((rm&15)<<4); break;
//		case BGBCC_SH_NMID_DMULS:
//			opw=opw1; opw2=0x300D|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOV:
			opw=opw1&(~8); opw2=0x6003|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_NOT:
			opw=opw1&(~8); opw2=0x6007|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SWAPB:
			opw=opw1&(~8); opw2=0x6008|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SWAPW:
			opw=opw1&(~8); opw2=0x6009|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_NEGC:
			opw=opw1&(~8); opw2=0x600A|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_NEG:
			opw=opw1&(~8); opw2=0x600B|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTUB:
			opw=opw1&(~8); opw2=0x600C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTUW:
			opw=opw1&(~8); opw2=0x600D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTSB:
			opw=opw1&(~8); opw2=0x600E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTSW:
			opw=opw1&(~8); opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_EXTUL:
			opw=opw1|9; opw2=0x600D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTSL:
			opw=opw1|9; opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FSTS:
//			if(rm!=BGBCC_SH_REG_FPUL)break;
//			opw=opw1; opw2=0xF00D|((rn&15)<<8); break;
//		case BGBCC_SH_NMID_FLDS:
//			if(rn!=BGBCC_SH_REG_FPUL)break;
//			opw=opw1; opw2=0xF01D|((rm&15)<<8); break;

		default:
			break;
		}
	}

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_STC) ||
			(nmid==BGBCC_SH_NMID_STS)) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_LDC) ||
			(nmid==BGBCC_SH_NMID_LDS)))
	{
		opw1=0xCE00;
		
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm) || 
				BGBCC_SHX_EmitCheckRegQuad(ctx, rn))
			opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))
			opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))
			opw1|=0x0020;

		switch(nmid)
		{
#if 1
		case BGBCC_SH_NMID_STC:
		case BGBCC_SH_NMID_STS:
			if((rm&0xF0)==BGBCC_SH_REG_CR0)
				{ opw=opw1|0x1; opw2=0x000A|((rn&15)<<8)|((rm&15)<<4); break; }
			if((rm&0xF0)==BGBCC_SH_REG_SR0)
				{ opw=opw1|0x0; opw2=0x000A|((rn&15)<<8)|((rm&15)<<4); break; }
			break;

		case BGBCC_SH_NMID_LDC:
		case BGBCC_SH_NMID_LDS:
			opw1&=~0x0060;
			if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0040;
			if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0020;
			if((rn&0xF0)==BGBCC_SH_REG_CR0)
				{ opw=opw1|0x5; opw2=0x000A|((rm&15)<<8)|((rn&15)<<4); break; }
			if((rn&0xF0)==BGBCC_SH_REG_SR0)
				{ opw=opw1|0x4; opw2=0x000A|((rm&15)<<8)|((rn&15)<<4); break; }
			break;
#endif

		case BGBCC_SH_NMID_MOV:
			opw=(opw1&(~0x80))|0x00;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_NOT:
			opw=(opw1&(~0x80))|0x7;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_TST:
			opw=(opw1&(~0x80))|0x8;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_TSTQ:
			opw=opw1|0x88;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_AND:
			opw=(opw1&(~0x80))|0x9;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_XOR:
			opw=(opw1&(~0x80))|0xA;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_OR:
			opw=(opw1&(~0x80))|0xB;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_CMPEQ:
			opw=(opw1&(~0x80))|0x00;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPHS:
			opw=(opw1&(~0x80))|0x02;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPGE:
			opw=(opw1&(~0x80))|0x03;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_DMULU:
			opw=(opw1&(~0x80))|0x05;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPHI:
			opw=(opw1&(~0x80))|0x06;
			opw=opw1&(~8); opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPGT:
			opw=(opw1&(~0x80))|0x07;
			opw=opw1&(~8); opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_SUB:
//			opw=(opw1&(~0x80))|(rn&15)|((opw1>>2)&0x0010);
//			opw2=0xA001|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_SUB:
			opw=(opw1&(~0x80))|0x08;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MULL:
			opw=(opw1&(~0x80))|0x09;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MULQ:
			opw=opw1|0x89;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SUBC:
			opw=(opw1&(~0x80))|0x0A;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SUBV:
			opw=(opw1&(~0x80))|0x0B;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_ADD:
//			opw=(opw1&(~0x80))|(rn&15)|((opw1>>2)&0x0010);
//			opw2=0xA000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_ADD:
			opw=(opw1&(~0x80))|0x0C;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_DMULS:
			opw=(opw1&(~0x80))|0x0D;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_ADDC:
			opw=(opw1&(~0x80))|0x0E;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_ADDV:
			opw=(opw1&(~0x80))|0x0F;
			opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_CMPQEQ:
			opw=opw1|0x80; opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQHS:
			opw=opw1|0x82; opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQGE:
			opw=opw1|0x83; opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQHI:
			opw=opw1|0x86; opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_CMPQGT:
			opw=opw1|0x87; opw2=0x300F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_SHLD:
			opw=(opw1&(~0x80))|0x0D;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHAD:
			opw=(opw1&(~0x80))|0x0C;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_SHLDQ:
			opw=(opw1&(~0x80))|0x0D;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHADQ:
			opw=(opw1&(~0x80))|0x0C;
			opw2=0x400F|((rn&15)<<8)|((rm&15)<<4); break;

#if 0
		case BGBCC_SH_NMID_SHLD:
			opw=(opw1&(~0x80))|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHLR:
			opw=(opw1&(~0x80))|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA007|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHAD:
			opw=(opw1&(~0x80))|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA008|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHAR:
			opw=(opw1&(~0x80))|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA009|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_SHLDQ:
			opw=opw1|0x80|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHLRQ:
			opw=opw1|0x80|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA007|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHADQ:
			opw=opw1|0x80|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA008|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SHARQ:
			opw=opw1|0x80|(rn&15)|((opw1>>2)&0x0010);
			opw2=0xA009|((rn&15)<<8)|((rm&15)<<4); break;
#endif

		case BGBCC_SH_NMID_SWAPB:
			opw=(opw1&(~0x80))|0x08;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_SWAPW:
			opw=(opw1&(~0x80))|0x09;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_NEGC:
			opw=(opw1&(~0x80))|0x0A;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_NEG:
			opw=(opw1&(~0x80))|0x0B;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTUB:
			opw=(opw1&(~0x80))|0x0C;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTUW:
			opw=(opw1&(~0x80))|0x0D;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTSB:
			opw=(opw1&(~0x80))|0x0E;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTSW:
			opw=(opw1&(~0x80))|0x0F;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_EXTUL:
			opw=opw1|0x8D;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_EXTSL:
			opw=opw1|0x8F;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
#endif
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;
	int rm3, rn3;

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC100|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(ctx->is_betav)
			{ opw=0x2003|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x2001|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			if(opw1==0xCC00)
				{ __debugbreak(); }
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw1|=8;
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
//			opw1|=1;
			opw=opw1; opw2=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
//			opw1|=1;
			opw=opw1; opw2=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw1&=~8;
//			opw1&=~8;	opw1|=1;
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			opw=opw1; opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCE00;
//		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0040;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1; opw2=0x2003|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			opw=opw1; opw2=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
			opw=opw1; opw2=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw=opw1; opw2=0x2002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
//			opw=opw1;
			opw=(opw1&0xFF0F)|((opw1&0x00C0)>>1);
			opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;
	int rm3, rn3;

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC108|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6000|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x6001|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6002|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x6003|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x6001|((rn&15)<<8)|((rm&15)<<4);		break;

//	case BGBCC_SH_NMID_SWAPB:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
//		opw=0x6008|((rn&15)<<8)|((rm&15)<<4);		break;
//	case BGBCC_SH_NMID_SWAPW:
//		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
//		opw=0x6009|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF008|((rn&15)<<8)|((rm&15)<<4);		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00;
			opw2=0x600C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00;
			opw2=0x600D|((rn&15)<<8)|((rm&15)<<4);
			break;
		default:
			break;
		}
	}
#endif

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			opw=opw1; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1|8; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			opw1|=1;
			opw=opw1; opw2=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
			opw1|=1;
			opw=opw1; opw2=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw1&=~8;	opw1|=1;
			opw=opw1; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=opw1&(~8); opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);	break;

		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)))
	{
		opw1=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0040;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=opw1; opw2=0x6002|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1; opw2=0x6003|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			opw=opw1; opw2=0x600C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
			opw=opw1; opw2=0x600D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw=opw1; opw2=0x600E|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=(opw1&0xFF0F)|((opw1&0x00C0)>>1);
			opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);
			break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStDecReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_STSL:
	case BGBCC_SH_NMID_STCL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((rm&0xF0)==BGBCC_SH_REG_CR0)
			{ opw=0x4003|((rn&15)<<8)|((rm&15)<<4); break; }
		if((rm&0xF0)==BGBCC_SH_REG_SR0)
			{ opw=0x4002|((rn&15)<<8)|((rm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x2005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		if(ctx->is_betav)
			{ opw=0x2007|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x2005|((rn&15)<<8)|((rm&15)<<4);		break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0xF00B|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x2004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x2005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			opw=opw1; opw2=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1|8; opw2=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCE00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0020;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=(opw1&(~0x80))|0x00;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=(opw1&(~0x80))|0x01;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=(opw1&(~0x80))|0x02;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw1&(~0x80))|0x03;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw1, opw2;

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_LDSL:
	case BGBCC_SH_NMID_LDCL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if((rn&0xF0)==BGBCC_SH_REG_CR0)
			{ opw=0x4007|((rm&15)<<8)|((rn&15)<<4); break; }
		if((rn&0xF0)==BGBCC_SH_REG_SR0)
			{ opw=0x4006|((rm&15)<<8)|((rn&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6004|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x6005|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6006|((rn&15)<<8)|((rm&15)<<4);		break;
	case BGBCC_SH_NMID_MOVQ:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x6007|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
		opw=0x6005|((rn&15)<<8)|((rm&15)<<4);		break;

	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF009|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=opw1; opw2=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=opw1; opw2=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)	opw1|=1;
			opw1&=~8;
			opw=opw1; opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=opw1|8; opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			opw1|=1;
			opw=opw1; opw2=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUW:
			opw1|=1;
			opw=opw1; opw2=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVUL:
			opw1&=~8;	opw1|=1;
			opw=opw1; opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCE00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0020;
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=(opw1&(~0x80))|0x04;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=(opw1&(~0x80))|0x05;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=(opw1&(~0x80))|0x06;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw1&(~0x80))|0x07;
			opw2=0x600F|((rn&15)<<8)|((rm&15)<<4); break;
		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegStRegDisp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	if(!BGBCC_SHX_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStRegDisp(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int rn, int disp)
{
	int opw, opw1, opw2, opw3;
	int i;

	if(disp==0)
	{
		i=0;
		if((nmid==BGBCC_SH_NMID_MOVQ) && ((rn&31)==15) &&
				ctx->has_bjx1r3mov)
			i=1;
	
		if(!i)
			return(BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if(ctx->is_betav)
			break;

		if(rm&31)					break;
//		if(rm!=BGBCC_SH_REG_R0)		break;
		if(disp<0)				break;

		if(rn==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(disp>=256)			break;
			opw=0xC000|disp;		break;
		}

//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		
		if(disp>=16)				break;
		opw=0x8000|((rn&15)<<4)|disp;
		break;
	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if(ctx->is_betav)
			break;

		if(rm&31)					break;
//		if(rm!=BGBCC_SH_REG_R0)		break;
		if((disp&1) || (disp<0))	break;

		if(rn==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if((disp>>1)>=256)		break;
			opw=0xC100|disp;		break;
		}

//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))			break;

		if((disp>>1)>=16)			break;
		opw=0x8100|((rn&15)<<4)|(disp>>1);
		break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if((disp&3) || (disp<0))	break;
		if(rn==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(rm!=BGBCC_SH_REG_R0)	break;
			if((disp>>2)>=256)		break;
			opw=0xC200|disp;		break;
		}

//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((disp>>2)>=16)			break;
		opw=0x1000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(ctx->is_betav)
			break;

		if(rm&31)					break;
		if((disp&7) || (disp<0))	break;
		if((disp>>3)>=16)			break;

		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))			break;

		opw=0x8100|((rn&15)<<4)|(disp>>3);
		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_bjx1mov && (ctx->is_simpass!=1))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVL:
			if(!ctx->has_bjx1r3mov)
				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>2)< 16)			break;
			if((disp>>2)>=32)			break;
			if(rn!=15)					break;
#if 1
			if(rm==0)
			{
				opw=0x8370|((disp>>2)&15);
				break;
			}
#endif
//			if((rm<8) || (rm>14))		break;
			if(((rm&31)<8) || ((rm&31)>14))	break;
			opw=0x8300|((rm&7)<<4)|((disp>>2)&15);
			break;

		case BGBCC_SH_NMID_MOVQ:
			if(!ctx->is_addr64)			break;
			if((disp&7) || (disp<0))	break;
			if((disp>>3)>=16)			break;
			if((rn!=BGBCC_SH_REG_R15) && (rn!=BGBCC_SH_REG_RQ15))
				break;
#if 0
			if((rm==BGBCC_SH_REG_R0) || (rm==BGBCC_SH_REG_RQ0))
			{	opw=0x8770|((disp>>3)&15);
				break;	}
			if(((rm&31)<8) || ((rm&31)>14))	break;
			opw=0x8700|((rm&7)<<4)|((disp>>3)&15);
#endif
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			opw=0xC000|((rm&15)<<4)|((disp>>3)&15);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if(!ctx->has_bjx1r3mov)
				break;
			if(BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))	break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if((disp&3) || (disp<0))			break;
			if(rn!=15)							break;
			if(((rm&15)<8) || ((rm&15)>15))		break;
			if((disp>>2)<16)
				{ opw=0x8600|((rm&7)<<4)|((disp>>2)&15); break; }
#if 0
//			if((disp>>2)<32)
			if(((disp>>2)<32) && !ctx->is_addr64)
				{ opw=0x8700|((rm&7)<<4)|((disp>>2)&15); break; }
#endif
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_movi20)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(disp<0)					break;
			if(disp>=4096)				break;

//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x0000|((disp)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (disp<0))	break;
			if((disp>>1)>=4096)			break;

//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x1000|((disp>>1)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;

//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x2000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVS:
			if(BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x3000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVD:
//			if(!BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
//				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>3)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x3000|((disp>>3)&0x0FFF);
			break;
		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(((sbyte)disp)!=disp)		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x2000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x2001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x2002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((sbyte)(disp>>3))!=(disp>>3)))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>3)&255);
			opw2=0x2003|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(ctx->is_mergece)
			{	opw=0x8E00|((disp>>2)&255);
				opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4);
				break;		}
			opw=0x8E00|((disp>>2)&255);
			opw2=0x2004|((rn&15)<<8)|((rm&15)<<4);
			break;

//		case BGBCC_SH_NMID_FMOVS:
//			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
//			opw2=0x7000|((disp>>2)&0x0FFF);
//			break;
		default:
			break;
		}
	}
#endif

#if 0
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
//		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;

		opw3=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
#if 1
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if((disp&63)!=disp)							break;
			opw=opw3|((disp)&63);
			opw2=0x2000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (((disp>>1)&63)!=(disp>>1)))	break;
			opw=opw3|((disp>>1)&63);
			opw2=0x2001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp&3) || (((disp>>2)&63)!=(disp>>2)))	break;
			opw=opw3|((disp>>2)&63);
			opw2=0x2002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((disp>>3)&63)!=(disp>>3)))	break;
			opw=opw3|((disp>>3)&63);
			opw2=0x2003|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 0
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
//			if(opw1&8)					break;
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=32)			break;
			if((disp>>2)>=16)			opw1|=1;
			opw=opw1&(~8);
			opw2=0x1000|((rn&15)<<8)|((rm&15)<<4)|((disp>>2)&15);
			break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!(opw1&8))				break;
			if((disp&7) || (disp<0))	break;
			if((disp>>3)>=32)			break;
			if((disp>>3)>=16)			opw1|=1;
			opw=opw1|8;
			opw2=0x1000|((rn&15)<<8)|((rm&15)<<4)|((disp>>3)&15);
			break;
#endif

		default:
			break;
		}
	}
#endif

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw3=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if((disp&63)!=disp)							break;
			opw=opw3|((disp)&63);
			opw2=0x2000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (((disp>>1)&63)!=(disp>>1)))	break;
			opw=opw3|((disp>>1)&63);
			opw2=0x2001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp&3) || (((disp>>2)&63)!=(disp>>2)))	break;
			opw=opw3|((disp>>2)&63);
			opw2=0x2002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((disp>>3)&63)!=(disp>>3)))	break;
			opw=opw3|((disp>>3)&63);
			opw2=0x2003|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((disp>>2)&15)!=(disp>>2)))	break;
			opw=(opw3&0xFF00)|((opw3&0x00C0)>>1)|((disp>>2)&15);
			opw2=0xF00A|((rn&15)<<8)|((rm&15)<<4);
			break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int opw, opw1, opw2, opw3;
	int i;

//	if(disp==0)
	if((disp==0) &&
		(nmid!=BGBCC_SH_NMID_MOVUB) &&
		(nmid!=BGBCC_SH_NMID_MOVUW))
	{
		i=0;
		if((nmid==BGBCC_SH_NMID_MOVQ) && ((rm&31)==15))
			i=1;
	
		if(!i)
			return(BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn));
	}

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(ctx->is_betav)
			break;

		if(rn&31)					break;
//		if(rn!=BGBCC_SH_REG_R0)		break;
		if(disp<0)				break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(disp>=256)			break;
			opw=0xC400|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		
		if(disp>=16)				break;
		opw=0x8400|((rm&15)<<4)|disp;
		break;
	case BGBCC_SH_NMID_MOVW:
		if(ctx->is_betav)
			break;

		if(rn&31)					break;
//		if(rn!=BGBCC_SH_REG_R0)		break;
		if((disp&1) || (disp<0))	break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if((disp>>1)>=256)		break;
			opw=0xC500|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))				break;

		if((disp>>1)>=16)			break;
		opw=0x8500|((rm&15)<<4)|(disp>>1);
		break;

	case BGBCC_SH_NMID_MOVL:
		if((disp&3) || (disp<0))	break;
		if(rm==BGBCC_SH_REG_GBR)
		{
			if(BGBCC_SHX_EmitCheckModeJQ(ctx))	break;
			if(rm!=BGBCC_SH_REG_R0)	break;
			if((disp>>2)>=256)		break;
			opw=0xC600|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;

		if((disp>>2)>=16)			break;
		opw=0x5000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	case BGBCC_SH_NMID_MOVQ:
		if(ctx->is_betav)
			break;

		if(rn&31)					break;
		if((disp&7) || (disp<0))	break;
		if((disp>>3)>=16)			break;

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))			break;

		opw=0x8500|((rm&15)<<4)|(disp>>3);
		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_bjx1mov && (ctx->is_simpass!=1))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVL:
			if(!ctx->has_bjx1r3mov)
				break;

			if((disp&3) || (disp<0))	break;
			if((disp>>2)< 16)			break;
			if((disp>>2)>=32)			break;
			if(rm!=15)					break;
#if 1
			if(rn==0)
			{
				opw=0x83F0|(disp>>2)&15;
				break;
			}
#endif			
//			if((rn<8) || (rn>14))		break;
			if(((rn&31)<8) || ((rn&31)>14))	break;
			opw=0x8380|((rn&7)<<4)|(disp>>2)&15;
			break;

		case BGBCC_SH_NMID_MOVQ:
			if(!ctx->is_addr64)			break;
			if((disp&7) || (disp<0))	break;
			if((disp>>3)>=16)			break;
			if((rm!=BGBCC_SH_REG_R15) && (rm!=BGBCC_SH_REG_RQ15))
				break;
#if 0
			if((rn==BGBCC_SH_REG_R0) || (rn==BGBCC_SH_REG_RQ0))
			{	opw=0x87F0|((disp>>3)&15);
				break;	}
//			if((rn<8) || (rn>14))		break;
			if(((rn&31)<8) || ((rn&31)>14))	break;
			opw=0x8780|((rn&7)<<4)|((disp>>3)&15);
#endif
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw=0xC400|((rn&15)<<4)|((disp>>3)&15);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if(!ctx->has_bjx1r3mov)
				break;

			if(BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))	break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			if((disp&3) || (disp<0))			break;
			if(rm!=15)							break;
			if(((rn&15)<8) || ((rn&15)>15))		break;
			if((disp>>2)<16)
				{ opw=0x8680|((rn&7)<<4)|((disp>>2)&15); break; }
//			if((disp>>2)<32)
//			if(((disp>>2)<32) && !ctx->is_addr64)
//				{ opw=0x8780|((rn&7)<<4)|((disp>>2)&15); break; }
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_movi20)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(disp<0)					break;
			if(disp>=4096)				break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x4000|((disp)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (disp<0))	break;
			if((disp>>1)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x5000|((disp>>1)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x6000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x7000|((disp>>2)&0x0FFF);
			break;
		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(((sbyte)disp)!=disp)							break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
			opw=0x8E00|((disp)&255);
			opw2=0x6000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))	break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x6001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))	break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))			break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x6002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((sbyte)(disp>>3))!=(disp>>3)))	break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>3)&255);
			opw2=0x6003|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))			break;
			if(ctx->is_mergece)
			{	opw=0x8E00|((disp>>2)&255);
				opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);
				break;			}
			opw=0x8E00|((disp>>2)&255);
			opw2=0x2006|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_MOVUB:
			if(((sbyte)disp)!=disp)		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x600C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x600D|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_LEAB:
			if(((sbyte)disp)!=disp)		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x6008|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_LEAW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x6009|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_LEAL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x600A|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_LEAQ:
			if((disp&7) || (((sbyte)(disp>>3))!=(disp>>3)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>3)&255);
			opw2=0x600B|((rn&15)<<8)|((rm&15)<<4);
			break;

//		case BGBCC_SH_NMID_FMOVS:
//			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
//			opw2=0x7000|((disp>>2)&0x0FFF);
//			break;
		default:
			break;
		}
	}
#endif

#if 0
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCC00;
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
//		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;

		opw3=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
#if 1
		case BGBCC_SH_NMID_MOVB:
			if((disp&63)!=disp)							break;
			opw=opw3|((disp)&63);
			opw2=0x6000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (((disp>>1)&63)!=(disp>>1)))	break;
			opw=opw3|((disp>>1)&63);
			opw2=0x6001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (((disp>>2)&63)!=(disp>>2)))	break;
			opw=opw3|((disp>>2)&63);
			opw2=0x6002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((disp>>3)&63)!=(disp>>3)))	break;
			opw=opw3|((disp>>3)&63);
			opw2=0x6003|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 0
		case BGBCC_SH_NMID_MOVB:
			break;
		case BGBCC_SH_NMID_MOVW:
			break;
		case BGBCC_SH_NMID_MOVL:
//			if(opw1&8)					break;
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=32)			break;
			if((disp>>2)>=16)			opw1|=1;
			opw=opw1&(~8);
			opw2=0x5000|((rn&15)<<8)|((rm&15)<<4)|((disp>>2)&15);
			break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!(opw1&8))				break;
			if((disp&7) || (disp<0))	break;
			if((disp>>3)>=32)			break;
			if((disp>>3)>=16)			opw1|=1;
			opw=opw1|8;
			opw2=0x5000|((rn&15)<<8)|((rm&15)<<4)|((disp>>3)&15);
			break;
#endif

		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)))
	{
		opw3=0xCE00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((disp&63)!=disp)							break;
			opw=opw3|((disp)&63);
			opw2=0x6000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (((disp>>1)&63)!=(disp>>1)))	break;
			opw=opw3|((disp>>1)&63);
			opw2=0x6001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (((disp>>2)&63)!=(disp>>2)))	break;
			opw=opw3|((disp>>2)&63);
			opw2=0x6002|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp&7) || (((disp>>3)&63)!=(disp>>3)))	break;
			opw=opw3|((disp>>3)&63);
			opw2=0x6003|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (((disp>>2)&15)!=(disp>>2)))	break;
			opw=(opw3&0xFF00)|((opw3&0x00C0)>>1)|((disp>>2)&15);
			opw2=0xF008|((rn&15)<<8)|((rm&15)<<4);
			break;

		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg2(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStReg2(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw, opw1, opw2, opw3, opw4;
	int rm3, rn3;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
		return(BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	if((ro!=BGBCC_SH_REG_R0) && (rn==BGBCC_SH_REG_R0))
		{ i=ro; ro=rn; rn=i; }
	if((ro!=BGBCC_SH_REG_RQ0) && (rn==BGBCC_SH_REG_RQ0))
		{ i=ro; ro=rn; rn=i; }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		!(ro&31) && ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC180|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
	case BGBCC_SH_NMID_MOVUB:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if((rm&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0x0004|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVW:
	case BGBCC_SH_NMID_MOVUW:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if((rm&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
//		if(BGBCC_SHX_EmitCheckModeDQ(ctx))				break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))	break;
		opw=0x0005|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVL:
	case BGBCC_SH_NMID_MOVUL:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
		if((rm&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0x0006|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVQ:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))			break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x0007|((rn&15)<<8)|((rm&15)<<4); break; }
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))	break;
		opw=0x0005|((rn&15)<<8)|((rm&15)<<4);			break;

	case BGBCC_SH_NMID_FMOVS:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if((rm&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rn==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegGPR(ctx, rm))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			opw=0x40C1|((rm&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
			opw=0x40D1|((rm&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
			opw=0x40E1|((rm&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
//			opw=0x40D1|((rm&15)<<8); break;
			opw=0x40A1|((rm&15)<<8); break;
		}
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rn==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegFPR(ctx, rm))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			opw=0x40B1|((rm&15)<<8); break;
		}
	}
	
	if((opw<0) && ctx->has_bjx1mov && !ctx->is_mergece)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x404E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x405E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x406E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x407E|((rn&15)<<8); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x403E|((rn&15)<<8); break;

		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw1=0xCC00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;
		opw4=0xCE00|((opw1&15)<<4);

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x0004|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x0005|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw1&=~8;
				opw=opw1;
				opw2=0x0006|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!(opw1&8))	break;
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1|8;
				opw2=0x0006|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;
//			opw=opw1|8;
//			opw2=0xA000|((rn&15)<<8)|((rm&15)<<4)|(ro&15); break;

		case BGBCC_SH_NMID_FMOVS:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			opw=opw1; opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw1=0xCC00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rm))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;
		opw4=0xCE00|((opw1&15)<<4);

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
//			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//				break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdReg2Reg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdReg2Reg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw, opw1, opw2, opw3, opw4;
	int rm3, rn3;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
//		return(BGBCC_SHX_EmitOpRegLdReg(ctx, nmid, rm, rn));
		return(BGBCC_SHX_EmitOpLdRegReg(ctx, nmid, rm, rn));
	}

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	if((ro!=BGBCC_SH_REG_R0) && (rm==BGBCC_SH_REG_R0))
		{ i=ro; ro=rm; rm=i; }
	if((ro!=BGBCC_SH_REG_RQ0) && (rm==BGBCC_SH_REG_RQ0))
		{ i=ro; ro=rm; rm=i; }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->is_addr64 && (nmid==BGBCC_SH_NMID_MOVQ) &&
		!(ro&31) && ctx->has_bjx1r3mov)
	{
#if 1
		rm3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rm);
		rn3=BGBCC_SHX_EmitGetRegAsReg3(ctx, rn);
		if((rm3>=0) && (rn3>=0))
			{ opw=0xC188|((rn3&7)<<4)|((rm3&7)<<0); }
#endif
	}

	if(opw<0)
		switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x000C|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVW:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))			break;
		opw=0x000D|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVL:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x000E|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVQ:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(ctx->is_betav)
			{ opw=0x000F|((rn&15)<<8)|((rm&15)<<4); break; }
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))			break;
		opw=0x000D|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_FMOVS:
		if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF006|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rm==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegGPR(ctx, rn))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=0x40C5|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
			opw=0x40D5|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
			opw=0x40E5|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
//			if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
//			opw=0x40D5|((rn&15)<<8); break;
			opw=0x40A5|((rn&15)<<8); break;

		case BGBCC_SH_NMID_LEAB:
		case BGBCC_SH_NMID_LEAW:
		case BGBCC_SH_NMID_LEAL:
		case BGBCC_SH_NMID_LEAQ:
			opw=0x40F5|((rn&15)<<8); break;
		}
	}

	if((opw<0) && ctx->has_bjx1mov &&
		(rm==BGBCC_SH_REG_PC) &&
		((ro==BGBCC_SH_REG_R0) || (ro==BGBCC_SH_REG_RQ0)) &&
		BGBCC_SHX_EmitCheckRegFPR(ctx, rn))
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_FMOVS:
			opw=0x40B5|((rn&15)<<8); break;
		}
	}

	if((opw<0) && ctx->has_bjx1mov && !ctx->is_mergece)
//	if(0)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40CE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40DE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40EE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40FE|((rn&15)<<8); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_MOVUB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x400E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_MOVUW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x401E|((rn&15)<<8); break;

		case BGBCC_SH_NMID_FMOVS:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x402E|((rn&15)<<8); break;

#if 1
		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x408E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x409E|((rn&15)<<8); break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40AE|((rn&15)<<8); break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(ctx->is_betav)								break;
			if(!(ro&15))break;
			opw=0x8E00|((rm&15)<<4)|(ro&15);
			opw2=0x40BE|((rn&15)<<8); break;
#endif

		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw1=0xCC00;
		opw3=0xCE00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;
		opw4=0xCE00|((opw1&15)<<4);

		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x000C|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;

//			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//				break;
//			opw=opw1;
//			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4);
//			break;
		case BGBCC_SH_NMID_MOVW:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1;
				opw2=0x000D|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;

//			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//				break;
//			opw=opw1;
//			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4);
//			break;
		case BGBCC_SH_NMID_MOVL:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				if(opw1&8)
					opw1|=1;
				opw=opw1;
				opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
//			if(opw1&8)
//			{
//				if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//					break;
//				opw=opw1;
//				opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
//				break;
//			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
//			opw=opw1&(~8);
//			opw2=0xB000|((rn&15)<<8)|((rm&15)<<4)|(ro&15); break;
		case BGBCC_SH_NMID_MOVQ:
			if((ro==BGBCC_SH_REG_R0) ||
				(ro==BGBCC_SH_REG_RD0) ||
				(ro==BGBCC_SH_REG_RQ0))
			{
				opw=opw1|8;
				opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;
//			opw=opw1|8;
//			opw2=0xB000|((rn&15)<<8)|((rm&15)<<4)|(ro&15); break;


		case BGBCC_SH_NMID_MOVUB:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw=opw1|1;
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUW:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw=opw1|1;
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUL:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw1&=~8;	opw=opw1|1;
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
			break;

		case BGBCC_SH_NMID_FMOVS:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=opw1; opw2=0xF006|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm) &&
		(BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) ||
			(nmid==BGBCC_SH_NMID_FMOVS)) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, ro))
	{
		opw1=0xCC00;
		opw3=0xCE00;

		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))		opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw1|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, ro))		opw1|=0x0001;
		opw4=0xCE00|((opw1&15)<<4);

		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw3|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))		opw3|=0x0040;

		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;

#if 0
		case BGBCC_SH_NMID_MOVUB:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw=opw1|1;
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUW:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw=opw1|1;
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVUL:
			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
				break;
			opw1&=~8;	opw=opw1|1;
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

		case BGBCC_SH_NMID_LEAB:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAW:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAL:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAQ:
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0x6007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_FMOVS:
//			if((ro!=BGBCC_SH_REG_R0) && (ro!=BGBCC_SH_REG_RQ0))
//				break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=(opw4&(~0x0080))|(ro&15);
			opw2=0xF006|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg2Disp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_EmitOpLdReg2DispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

#if 1

int BGBCC_SHX_TryEmitOpRegStReg2Disp(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw, opw2;
	int i;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;
	
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
		case BGBCC_SH_NMID_MOVUB:
			if((disp<0) || (disp>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
		case BGBCC_SH_NMID_MOVUW:
			if((disp<0) || (disp&1) || ((disp>>1)>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>1);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
		case BGBCC_SH_NMID_MOVUL:
			if((disp<0) || (disp&3) || ((disp>>2)>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>2);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp<0) || (disp&7) || ((disp>>3)>=16))		break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>3);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_TryEmitOpLdReg2DispReg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	int opw, opw2;
	int i, d;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((disp<0) || (disp>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			if((disp<0) || (disp&1) || ((disp>>1)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>1);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			if((disp<0) || (disp&3) || ((disp>>2)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>2);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp<0) || (disp&7) || ((disp>>3)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, ro))		break;
			if(!(ro&15))break;
			opw=0x8E00|((ro&15)<<4)|(disp>>3);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


#endif

#if 1
int BGBCC_SHX_EmitOpRegRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegRegReg(
	BGBCC_SHX_Context *ctx, int nmid, int rs, int rt, int rn)
{
	int opw, opw1, opw2;
	int i, e;

	if(((nmid==BGBCC_SH_NMID_ADD) ||
		(nmid==BGBCC_SH_NMID_AND) ||
		(nmid==BGBCC_SH_NMID_OR) ||
		(nmid==BGBCC_SH_NMID_XOR)) &&
		BGBCC_SHX_EmitCheckRegGPR(ctx, rs) &&
		BGBCC_SHX_EmitCheckRegGPR(ctx, rt) &&
		BGBCC_SHX_EmitCheckRegGPR(ctx, rn))
	{
		if(rs==rn)
		{
			i=BGBCC_SHX_TryEmitOpRegReg(ctx, nmid, rt, rn);
			return(i);
		}

		if(rt==rn)
		{
			i=BGBCC_SHX_TryEmitOpRegReg(ctx, nmid, rs, rn);
			return(i);
		}
	}

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rs);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rt);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->has_bjx1ari &&
		BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rs) &&
		BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rt) &&
		BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn) &&
		!ctx->is_mergece)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4003|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SUB:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4013|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_MULQ:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4023|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_AND:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4033|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_OR:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4043|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_XOR:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4053|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SHLL:
		case BGBCC_SH_NMID_SHLD:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4063|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SHLR:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4073|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SHAL:
		case BGBCC_SH_NMID_SHAD:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4083|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SHAR:
			if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4093|((rn&15)<<8);
			break;

		case BGBCC_SH_NMID_SHLLQ:
		case BGBCC_SH_NMID_SHLDQ:
			if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4063|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SHLRQ:
			if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4073|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SHALQ:
		case BGBCC_SH_NMID_SHADQ:
			if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4083|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_SHARQ:
			if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
				break;
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x4093|((rn&15)<<8);
			break;
		}
	}

	
//	if((opw<0) && ctx->has_bjx1ari)
	if((opw<0) && ctx->has_bjx1egpr &&
		!ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rs) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rt) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		e=0;

		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		e|=0x0004;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rs))		e|=0x0002;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rt))		e|=0x0001;

		switch(nmid)
		{
		case BGBCC_SH_NMID_SHLLQ:
		case BGBCC_SH_NMID_SHLDQ:
		case BGBCC_SH_NMID_SHALQ:
		case BGBCC_SH_NMID_SHADQ:
		case BGBCC_SH_NMID_SHLRQ:
		case BGBCC_SH_NMID_SHARQ:
		case BGBCC_SH_NMID_MULQ:
			e|=8; break;
		default:
			break;
		}

//		if((e&8) && (e&7))
//			{ BGBCC_DBGBREAK }
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			opw=0xCC30|e;
			opw2=0x0000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SUB:
			opw=0xCC30|e;
			opw2=0x1000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_MULQ:
			opw=0xCC30|e;
			opw2=0x2000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_AND:
			opw=0xCC30|e;
			opw2=0x3000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_OR:
			opw=0xCC30|e;
			opw2=0x4000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_XOR:
			opw=0xCC30|e;
			opw2=0x5000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;

		case BGBCC_SH_NMID_SHLL:
		case BGBCC_SH_NMID_SHLD:
//				opw=0xCC30|e;
			opw=0xCC30|(e&(~8));
			opw2=0x6000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHLR:
//				opw=0xCC30|e;
			opw=0xCC30|(e&(~8));
			opw2=0x7000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHAL:
		case BGBCC_SH_NMID_SHAD:
//				opw=0xCC30|e;
			opw=0xCC30|(e&(~8));
			opw2=0x8000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHAR:
//				opw=0xCC30|e;
			opw=0xCC30|(e&(~8));
			opw2=0x9000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;

//		case BGBCC_SH_NMID_SHLD:
//			opw=0xCC30|(e&(~8));
//			opw2=0x6000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
//			break;
//		case BGBCC_SH_NMID_SHAD:
//			opw=0xCC30|(e&(~8));
//			opw2=0x8000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
//			break;

		case BGBCC_SH_NMID_SHLLQ:
		case BGBCC_SH_NMID_SHLDQ:
			opw=0xCC30|(e|8);
			opw2=0x6000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHALQ:
		case BGBCC_SH_NMID_SHADQ:
			opw=0xCC30|(e|8);
			opw2=0x8000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;

		case BGBCC_SH_NMID_SHLRQ:
			opw=0xCC30|(e|8);
			opw2=0x7000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHARQ:
			opw=0xCC30|(e|8);
			opw2=0x9000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		}
	}

//	if((opw<0) && ctx->has_bjx1ari)
	if((opw<0) && ctx->has_bjx1egpr && ctx->is_mergece &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rs) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rt) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		opw1=0xCE00;

		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rs))		opw1|=0x0020;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rt))		opw1|=0x0010;

		switch(nmid)
		{
		case BGBCC_SH_NMID_SHLLQ:
		case BGBCC_SH_NMID_SHLDQ:
		case BGBCC_SH_NMID_SHALQ:
		case BGBCC_SH_NMID_SHADQ:
		case BGBCC_SH_NMID_SHLRQ:
		case BGBCC_SH_NMID_SHARQ:
		case BGBCC_SH_NMID_MULQ:
			opw1|=0x0080; break;
		default:
			break;
		}
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			opw=opw1|(rt&15);
			opw2=0xA000|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_SUB:
			opw=opw1|(rt&15);
			opw2=0xA001|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_MULQ:
			opw=opw1|(rt&15);
			opw2=0xA002|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_AND:
			opw=opw1|(rt&15);
			opw2=0xA003|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_OR:
			opw=opw1|(rt&15);
			opw2=0xA004|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_XOR:
			opw=opw1|(rt&15);
			opw2=0xA005|((rn&15)<<8)|((rs&15)<<4);
			break;

		case BGBCC_SH_NMID_SHLL:
		case BGBCC_SH_NMID_SHLD:
			opw=opw1|(rt&15);
			opw2=0xA006|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_SHLR:
		case BGBCC_SH_NMID_SHLRQ:
			opw=opw1|(rt&15);
			opw2=0xA007|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_SHAL:
		case BGBCC_SH_NMID_SHAD:
			opw=opw1|(rt&15);
			opw2=0xA008|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_SHAR:
		case BGBCC_SH_NMID_SHARQ:
			opw=opw1|(rt&15);
			opw2=0xA009|((rn&15)<<8)|((rs&15)<<4);
			break;

		case BGBCC_SH_NMID_SHLLQ:
		case BGBCC_SH_NMID_SHLDQ:
			opw=opw1|(rt&15);
			opw2=0xA006|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_SHALQ:
		case BGBCC_SH_NMID_SHADQ:
			opw=opw1|(rt&15);
			opw2=0xA008|((rn&15)<<8)|((rs&15)<<4);
			break;
		}
	}

	if((opw<0) && ctx->has_bjx1ari &&
		BGBCC_SHX_EmitCheckRegFPR(ctx, rs) &&
		BGBCC_SHX_EmitCheckRegFPR(ctx, rt) &&
		BGBCC_SHX_EmitCheckRegFPR(ctx, rn) &&
		!ctx->is_mergece)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_FADD:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x40C3|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_FSUB:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x40D3|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_FMUL:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x40E3|((rn&15)<<8);
			break;
		case BGBCC_SH_NMID_FDIV:
			opw=0x8E00|((rs&15)<<4)|(rt&15);
			opw2=0x40F3|((rn&15)<<8);
			break;
		}
	}

	if((opw<0) && ctx->has_bjx1ari &&
		BGBCC_SHX_EmitCheckRegExtFPR(ctx, rs) &&
		BGBCC_SHX_EmitCheckRegExtFPR(ctx, rt) &&
		BGBCC_SHX_EmitCheckRegExtFPR(ctx, rn) &&
		ctx->is_mergece)
	{
		opw1=0xCE00;

		if(BGBCC_SHX_EmitCheckRegDouble(ctx, rn))	opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))		opw1|=0x0040;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rs))		opw1|=0x0020;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rt))		opw1|=0x0010;
		
		switch(nmid)
		{
		case BGBCC_SH_NMID_FADD:
			opw=opw1|(rt&15);
			opw2=0xF000|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_FSUB:
			opw=opw1|(rt&15);
			opw2=0xF001|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_FMUL:
			opw=opw1|(rt&15);
			opw2=0xF002|((rn&15)<<8)|((rs&15)<<4);
			break;
		case BGBCC_SH_NMID_FDIV:
			opw=opw1|(rt&15);
			opw2=0xF003|((rn&15)<<8)|((rs&15)<<4);
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}
#endif

int BGBCC_SHX_EmitOpRegImm(BGBCC_SHX_Context *ctx,
	int nmid, int reg, int imm)
{
	if(!BGBCC_SHX_TryEmitOpRegImm(ctx, nmid, reg, imm))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegImm(BGBCC_SHX_Context *ctx,
	int nmid, int reg, int imm)
{
	s32 imm1;
	int opw, opw1, opw2;

//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ reg=BGBCC_SH_REG_FR0+(2*(reg&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:	
		if(((sbyte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0x7000|((reg&15)<<8)|(imm&255);
		break;

	case BGBCC_SH_NMID_CMPEQ:	
		if(((sbyte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		opw=0x8800|((reg&15)<<8)|(imm&255);
		break;

	case BGBCC_SH_NMID_TST:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		if(ctx->is_addr64)			break;
		opw=0xC800|((reg&15)<<8)|(imm&255);
		break;
	case BGBCC_SH_NMID_AND:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		if(ctx->is_addr64)			break;
		opw=0xC900|((reg&15)<<8)|(imm&255);
		break;
	case BGBCC_SH_NMID_OR:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		if(ctx->is_addr64)			break;
		opw=0xCA00|((reg&15)<<8)|(imm&255);
		break;
	case BGBCC_SH_NMID_XOR:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		if(ctx->is_addr64)			break;
		opw=0xCB00|((reg&15)<<8)|(imm&255);
		break;

	case BGBCC_SH_NMID_MOV:	
		if(((sbyte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
		opw=0xE000|((reg&15)<<8)|(imm&255);
		break;

	default:
		break;
	}

	if((opw<0) && ctx->has_bjx1ari)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:	
			if(((s16)imm)!=imm)
				break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
			opw=0x8E00|((imm>>8)&255);
			opw2=0x7000|((reg&15)<<8)|(imm&255);
			break;

		case BGBCC_SH_NMID_TST:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
//			if((imm&0xFFF)!=imm)
//				break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xC800|((reg&15)<<4)|(imm&15);
			break;
		case BGBCC_SH_NMID_AND:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
//			if((imm&0xFFF)!=imm)
//				break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xC900|((reg&15)<<4)|(imm&15);
			break;
		case BGBCC_SH_NMID_XOR:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
//			if((imm&0xFFF)!=imm)
//				break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xCA00|((reg&15)<<4)|(imm&15);
			break;
		case BGBCC_SH_NMID_OR:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
//			if((imm&0xFFF)!=imm)
//				break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xCB00|((reg&15)<<4)|(imm&15);
			break;

		case BGBCC_SH_NMID_CMPEQ:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3000|((reg&15)<<8)|((imm&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPHS:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3002|((reg&15)<<8)|((imm&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPGE:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3003|((reg&15)<<8)|((imm&15)<<4);
			break;

		case BGBCC_SH_NMID_CMPHI:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3006|((reg&15)<<8)|((imm&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPGT:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3007|((reg&15)<<8)|((imm&15)<<4);
			break;

		case BGBCC_SH_NMID_LDSH16:
//			imm1=(imm<<20)>>20;
			imm1=(imm<<16)>>16;
			if(imm1!=imm)	break;
//			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, reg))	break;
//			if(reg!=0)
//			{
//				opw=0x8C70|(reg&15);
//				opw2=imm;
//				break;
//			}
//			opw=0x8E00|((imm>>8)&255);
//			opw2=0x8C00|(imm&255);
			opw=0x8E00|((imm>>8)&255);
			opw2=0x9000|(imm&255)|((reg&15)<<8);
			break;

		default:
			break;
		}

	}

#if 0
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, reg))
	{
		opw1=0xCC00;
		
		if(BGBCC_SHX_EmitCheckRegQuad(ctx, reg))
			opw1|=0x0008;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, reg))
			opw1|=0x0004;

		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:	
			if(((sbyte)imm)!=imm)
				break;
			opw=opw1; opw2=0x7000|((reg&15)<<8)|(imm&255);
			break;
		case BGBCC_SH_NMID_MOV:	
			if(((sbyte)imm)!=imm)
				break;
			opw=opw1; opw2=0xE000|((reg&15)<<8)|(imm&255);
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, reg))
	{
		opw1=0xCE00;
		
//		if(BGBCC_SHX_EmitCheckRegQuad(ctx, reg))
//			opw1|=0x0008;
//		if(BGBCC_SHX_EmitCheckRegExt4(ctx, reg))
//			opw1|=0x0080;

		opw1=0x8E00;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, reg))
			opw1=0xCE00;
		
		imm1=(((s32)imm)<<18)>>18;

		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:	
//			if(((sbyte)imm)!=imm)
//			if(imm1!=imm)
			if(((s16)imm)!=imm)
				break;
//			opw=opw1|((imm>>8)&0x3F);
			opw=opw1|((imm>>8)&255);
			opw2=0x7000|((reg&15)<<8)|(imm&255);
			break;
		case BGBCC_SH_NMID_MOV:	
//			if(((sbyte)imm)!=imm)
//			if(imm1!=imm)
			if(((s16)imm)!=imm)
				break;
//			opw=opw1|((imm>>8)&0x3F);
			opw=opw1|((imm>>8)&255);
			opw2=0xE000|((reg&15)<<8)|(imm&255);
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

#if 1
int BGBCC_SHX_EmitOpRegImmReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegImmReg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int imm, int rn)
{
	int opw, opw1, opw2;
	int imm1;
	int i;

	if((nmid==BGBCC_SH_NMID_ADD) && (rm==rn) && (((sbyte)imm)==imm))
	{
		i=BGBCC_SHX_TryEmitOpRegImm(ctx, BGBCC_SH_NMID_ADD, rn, imm);
		return(i);
	}

	if((nmid==BGBCC_SH_NMID_SUB) && (rm==rn) && (((sbyte)(-imm))==(-imm)))
	{
		i=BGBCC_SHX_TryEmitOpRegImm(ctx, BGBCC_SH_NMID_ADD, rn, -imm);
		return(i);
	}
	
	if((nmid==BGBCC_SH_NMID_ADD) && (imm<0))
	{
		i=BGBCC_SHX_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_SUB, rm, -imm, rn);
		return(i);
	}

	if((nmid==BGBCC_SH_NMID_SUB) && (imm<0))
	{
		i=BGBCC_SHX_TryEmitOpRegImmReg(ctx, BGBCC_SH_NMID_ADD, rm, -imm, rn);
		return(i);
	}

	if(!(ctx->has_bjx1ari))
		return(0);

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	if((nmid==BGBCC_SH_NMID_ADD) && (imm<0) && ((-imm)!=imm))
		return(BGBCC_SHX_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SUB, rm, -imm, rn));
	if((nmid==BGBCC_SH_NMID_SUB) && (imm<0) && ((-imm)!=imm))
		return(BGBCC_SHX_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rm, -imm, rn));

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:	
		if(((byte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x300C|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_SUB:	
		if(((byte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x3008|((rn&15)<<8)|((rm&15)<<4);
		break;

	case BGBCC_SH_NMID_AND:	
//		if(((byte)imm)!=imm)
//			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(((byte)imm)==imm)
		{
			opw=0x8E00|(imm&255);
			opw2=0x2009|((rn&15)<<8)|((rm&15)<<4);
			break;
		}
		if(((byte)(-imm))==(-imm))
		{
			opw=0x8E00|(imm&255);
			opw2=0x3009|((rn&15)<<8)|((rm&15)<<4);
			break;
		}
		break;
	case BGBCC_SH_NMID_XOR:	
		if(((byte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x200A|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_OR:	
		if(((byte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x200B|((rn&15)<<8)|((rm&15)<<4);
		break;

	case BGBCC_SH_NMID_MULL:
	case BGBCC_SH_NMID_MULQ:
//		if(((sbyte)imm)!=imm)
//			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
		if(((byte)imm)==imm)
		{
			opw=0x8E00|(imm&255);
			opw2=0x200E|((rn&15)<<8)|((rm&15)<<4);
			break;
		}
		if(((byte)(-imm))==(-imm))
		{
			opw=0x8E00|(imm&255);
			opw2=0x200F|((rn&15)<<8)|((rm&15)<<4);
			break;
		}
		break;

	case BGBCC_SH_NMID_SHAD:	
		if(((sbyte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
		if(0)
		{
//			if(BGBCC_SHX_EmitCheckModeDQ(ctx))
			if(BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
			{
				opw=0x8E00|((imm&255)^0x40);
				opw2=0x400C|((rn&15)<<8)|((rm&15)<<4);
				break;
			}

//			if(BGBCC_SHX_EmitCheckModeDQ(ctx))
//				__debugbreak();

			break;
		}
//		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x8E00|(imm&255);
		opw2=0x400C|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_SHLD:	
		if(((sbyte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))		break;
//		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
//		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
		if(0)
		{
//			if(BGBCC_SHX_EmitCheckModeDQ(ctx))
			if(BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
			{
				opw=0x8E00|((imm&255)^0x40);
				opw2=0x400D|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			break;
		}
//		if(!BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))		break;
		opw=0x8E00|(imm&255);
		opw2=0x400D|((rn&15)<<8)|((rm&15)<<4);
		break;

#if 1
	case BGBCC_SH_NMID_SHADQ:
		if(((sbyte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
//		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
		if(1)
		{
//			if(!BGBCC_SHX_EmitCheckModeDQ(ctx))
//			if(BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
			if(1)
			{
				opw=0x8E00|((imm&255)^0x40);
				opw2=0x400C|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			break;
		}
		opw=0x8E00|(imm&255);
		opw2=0x400C|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_SHLDQ:	
		if(((sbyte)imm)!=imm)
			break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
//		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegBaseGPR(ctx, rn))	break;
//		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))		break;
//		if(!BGBCC_SHX_EmitCheckModeSetDQ(ctx, nmid))
		if(1)
		{
//			if(!BGBCC_SHX_EmitCheckModeDQ(ctx))
//			if(BGBCC_SHX_EmitCheckModeClrDQ(ctx, nmid))
			if(1)
			{
				opw=0x8E00|((imm&255)^0x40);
				opw2=0x400D|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			break;
		}
		opw=0x8E00|(imm&255);
		opw2=0x400D|((rn&15)<<8)|((rm&15)<<4);
		break;
#endif

	}

#if 1
	if((opw<0) && ctx->is_addr64 && ctx->has_bjx1egpr &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rm))
	{
		opw1=0xCE00;
		
//		if(BGBCC_SHX_EmitCheckRegQuad(ctx, rn))
//			opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rn))
			opw1|=0x0080;
		if(BGBCC_SHX_EmitCheckRegExt4(ctx, rm))
			opw1|=0x0040;
		
		switch(nmid)
		{

#if 1
		case BGBCC_SH_NMID_ADD:	
			if((imm&63)!=imm)		break;
			opw=opw1|(imm&63);
			opw2=0x300C|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_SUB:	
			if((imm&63)!=imm)		break;
			opw=opw1|(imm&63);
			opw2=0x3008|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_AND:	
			if((imm&63)==imm)
			{
				opw=opw1|(imm&63);
				opw2=0x2009|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			if(((-imm)&63)==(-imm))
			{
				opw=opw1|(imm&63);
				opw2=0x3009|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			break;
		case BGBCC_SH_NMID_XOR:	
			if((imm&63)!=imm)
				break;
			opw=opw1|(imm&63);
			opw2=0x200A|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_OR:	
			if((imm&63)!=imm)
				break;
			opw=opw1|(imm&63);
			opw2=0x200B|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_MULL:
		case BGBCC_SH_NMID_MULQ:
			if((imm&63)==imm)
			{
				opw=opw1|(imm&63);
				opw2=0x200E|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			if(((-imm)&63)==(-imm))
			{
				opw=opw1|(imm&63);
				opw2=0x200F|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			break;
#endif

#if 1
		case BGBCC_SH_NMID_SHAD:	
//			if(imm1!=imm)
			if(((((s32)imm)<<26)>>26)!=imm)
				break;
			opw=opw1|(imm&63);
			opw2=0x400C|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_SHLD:	
//			if(imm1!=imm)
			if(((((s32)imm)<<26)>>26)!=imm)
				break;
			opw=opw1|(imm&63);
			opw2=0x400D|((rn&15)<<8)|((rm&15)<<4);
			break;
#endif

#if 1
		case BGBCC_SH_NMID_SHADQ:
			if((imm&63)==imm)
			{
				opw=opw1|(imm&63);
				opw2=0x300A|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			if(((-imm)&63)==(-imm))
			{
				opw=opw1|(imm&63);
				opw2=0x300E|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			break;
		case BGBCC_SH_NMID_SHLDQ:	
			if((imm&63)==imm)
			{
				opw=opw1|(imm&63);
				opw2=0x300B|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			if(((-imm)&63)==(-imm))
			{
				opw=opw1|(imm&63);
				opw2=0x300F|((rn&15)<<8)|((rm&15)<<4);
				break;
			}
			break;
#endif

		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}
#endif
