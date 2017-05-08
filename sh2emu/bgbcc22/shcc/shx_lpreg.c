const byte bgbcc_shx_lcachereg[6]={
	BGBCC_SH_REG_LR13,
	BGBCC_SH_REG_LR11,
	BGBCC_SH_REG_LR10,
	BGBCC_SH_REG_LR9,
	BGBCC_SH_REG_LR8, 255 };

int BGBCC_SHXC_EmitTryGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	int creg;
	int i;

	/* value already in a register? */
	for(i=1; i<4; i++)
	{
		if(!((sctx->regalc_save)&(3<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=3<<i;
			if(fl&1)
				sctx->regalc_dirty|=3<<i;

			if(sctx->regalc_ltcnt[i]>0)
				sctx->regalc_ltcnt[i]--;
			sctx->regalc_utcnt[i]++;
			return(bgbcc_shx_lcachereg[i]);
		}
	}

	return(BGBCC_SH_REG_ZZR);
}

/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_SHXC_EmitGetLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg, int fl)
{
	ccxl_register zreg;
	int creg;
	int i;

	creg=BGBCC_SHXC_EmitTryGetFpRegister(ctx, sctx, reg, fl);
	if((creg>=0) && (creg!=BGBCC_SH_REG_ZZR))
		return(creg);

	for(i=1; i<4; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

	zreg.val=CCXL_REGTY_TEMP|CCXL_REGID_REGMASK;

	/* Check for registers not holding a live value. */
	for(i=2; i<5; i++)
	{
		if(!((sctx->regalc_save)&(3<<i)))
			continue;
		if(!((sctx->regalc_live)&(3<<i)))
		{
			sctx->regalc_map[i+0]=reg;
			sctx->regalc_map[i+1]=zreg;
			sctx->regalc_utcnt[i]=1;
			sctx->regalc_live|=3<<i;
			if(fl&1)
				sctx->regalc_dirty|=1<<i;
			creg=bgbcc_shx_lcachereg[i+1];
			if(fl&2)
				{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
			return(creg);
		}
	}

	/* Check for unallocated registers. */
	for(i=1; i<4; i++)
	{
		if((sctx->regalc_save)&(1<<i))
			continue;
		if((sctx->regalc_live)&(3<<i))
			continue;
		
		sctx->regalc_map[i+0]=reg;
		sctx->regalc_map[i+1]=zreg;
		sctx->regalc_utcnt[i]=1;
		sctx->regalc_save|=3<<i;
		sctx->regalc_live|=3<<i;
		if(fl&1)
			sctx->regalc_dirty|=1<<i;

		creg=bgbcc_shx_lcachereg[i+1];		
		BGBCC_SHXC_EmitSaveFrameReg(ctx, sctx, (creg+0)&15);
		BGBCC_SHXC_EmitSaveFrameReg(ctx, sctx, (creg+1)&15);
		if(fl&2)
			{ BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, reg, creg); }
		
		return(creg);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitReleaseLpRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	int creg;
	int i;

	/* value in register? */
	for(i=1; i<4; i++)
	{
		if(!((sctx->regalc_save)&(3<<i)))
			continue;
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx,
			reg, sctx->regalc_map[i]))
		{
			sctx->regalc_live|=3<<i;
			if(sctx->regalc_utcnt[i]>0)
				sctx->regalc_utcnt[i]--;

			if(!sctx->regalc_utcnt[i])
			{
				if((sctx->regalc_dirty)&(1<<i))
				{
					creg=bgbcc_shx_lcachereg[i+1];
					BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, reg, creg);
					sctx->regalc_dirty&=~(1<<i);
				}

				sctx->regalc_live&=~(3<<i);
			}

			return(1);
		}
	}

	return(0);
}




int BGBCC_SHXC_EmitBinaryVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	int i, j, k;

	nm1=-1;	nm2=-1;
	nm3=-1;	nm4=-1;
	switch(opr)
	{
	case CCXL_BINOP_ADD:
		nm1=BGBCC_SH_NMID_ADDC; nm2=nm1;
		nm3=BGBCC_SH_NMID_CLRT;
		break;
	case CCXL_BINOP_SUB:
		nm1=BGBCC_SH_NMID_SUBC; nm2=nm1;
		nm3=BGBCC_SH_NMID_CLRT;
		break;

	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND; nm2=nm1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR; nm2=nm1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR; nm2=nm1; break;
	default:		nm1=-1; nm2=-1; break;
	}
	
	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitTryGetRegisterDirty(ctx, sctx, dreg);
		ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);

		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			if((ctreg>=0) && (ctreg!=BGBCC_SH_REG_ZZR))
			{
				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg+0, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg+1, cdreg+1);

				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}else
			{
				BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR4);

				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, cdreg+1);

				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}else if((ctreg>=0) && (ctreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

			if(nm3>=0)
				BGBCC_SHX_EmitOpNone(sctx, nm3);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg+0, BGBCC_SH_REG_R4);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg+1, BGBCC_SH_REG_R5);

			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);

		if(nm4>=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm4, BGBCC_SH_REG_R6, BGBCC_SH_REG_R6);
			BGBCC_SHX_EmitOpRegReg(sctx, nm4, BGBCC_SH_REG_R7, BGBCC_SH_REG_R7);
		}

		if(nm3>=0)
			BGBCC_SHX_EmitOpNone(sctx, nm3);
		BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R6, BGBCC_SH_REG_R4);
		BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R7, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		return(1);
	}
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__addlli"; break;
	case CCXL_BINOP_SUB:	s0="__sublli"; break;
	case CCXL_BINOP_MUL:	s0="__smullli"; break;
	case CCXL_BINOP_DIV:	s0="__sdivlli"; break;
	case CCXL_BINOP_AND:	s0="__andlli"; break;
	case CCXL_BINOP_OR:		s0="__orlli"; break;
	case CCXL_BINOP_XOR:	s0="__xorlli"; break;
	case CCXL_BINOP_SHL:	s0="__shllli"; break;
	case CCXL_BINOP_SHR:	s0="__shrlli"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR6);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegLong(
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
		i=BGBCC_SHXC_EmitBinaryVRegVRegLong(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_SHXC_EmitBinaryVRegVRegLong(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	s32 imm;
	int i, j, k;
	
	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}

	nm1=-1;	nm2=-1;
	nm3=-1;	nm4=-1;
	switch(opr)
	{
	case CCXL_UNOP_NEG:
		nm1=BGBCC_SH_NMID_NEGC; nm2=nm1;
		nm3=BGBCC_SH_NMID_CLRT;
		break;
	case CCXL_UNOP_NOT:
		nm1=BGBCC_SH_NMID_NOT; nm2=nm1;
//		nm3=BGBCC_SH_NMID_CLRT;
		break;
	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);

		if((cdreg>=0) && (cdreg!=BGBCC_SH_REG_ZZR))
		{
			if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
			{
				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1,
					csreg+0, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2,
					csreg+1, cdreg+1);

				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}else
			{
				BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);

				if(nm3>=0)
					BGBCC_SHX_EmitOpNone(sctx, nm3);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, cdreg+0);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, cdreg+1);

				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
		}else if((csreg>=0) && (csreg!=BGBCC_SH_REG_ZZR))
		{
			BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);

			if(nm3>=0)
				BGBCC_SHX_EmitOpNone(sctx, nm3);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg+0, BGBCC_SH_REG_R4);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, csreg+1, BGBCC_SH_REG_R5);

			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);

		if(nm3>=0)
			BGBCC_SHX_EmitOpNone(sctx, nm3);
		BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R4, BGBCC_SH_REG_R4);
		BGBCC_SHX_EmitOpRegReg(sctx, nm2, BGBCC_SH_REG_R5, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		return(1);
	}

	
	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__neglli"; break;
	case CCXL_UNOP_NOT:	s0="__notlli"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR4);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitJCmpVRegVRegLong(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register sreg, ccxl_register treg,
	int cmp, int lbl)
{
	int csreg, ctreg;
	int nm1, nm2, nm3, nm4;
	int l0, l1;
	
	l0=lbl; l1=lbl;
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_CMPEQ;
		nm4=BGBCC_SH_NMID_BT;
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_CMPEQ;
		nm4=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_CMPGE;
		nm4=BGBCC_SH_NMID_BF;
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_CMPGT;
		nm4=BGBCC_SH_NMID_BT;
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BT;
		nm3=BGBCC_SH_NMID_CMPGT;
		nm4=BGBCC_SH_NMID_BF;
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BF;
		nm3=BGBCC_SH_NMID_CMPGE;
		nm4=BGBCC_SH_NMID_BT;
		l0=BGBCC_SHX_GenLabel(sctx);
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg+1, csreg+1);
		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, l0);
		BGBCC_SHX_EmitOpRegReg(sctx, nm3, ctreg+0, csreg+0);
		BGBCC_SHX_EmitOpAutoLabel(sctx, nm4, l1);

		if(l0!=lbl)
			BGBCC_SHX_EmitOpLabel(sctx, nm2, l0);
//		if(l1!=lbl)
//			BGBCC_SHX_EmitOpLabel(sctx, nm2, l1);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	return(0);
}



int BGBCC_SHXC_EmitBinaryVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2;
	int i, j, k;
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	s0="__addxli"; break;
	case CCXL_BINOP_SUB:	s0="__subxli"; break;
	case CCXL_BINOP_MUL:	s0="__smulxli"; break;
	case CCXL_BINOP_DIV:	s0="__sdivxli"; break;
	case CCXL_BINOP_AND:	s0="__andxli"; break;
	case CCXL_BINOP_OR:		s0="__orxli"; break;
	case CCXL_BINOP_XOR:	s0="__xorxli"; break;
	case CCXL_BINOP_SHL:	s0="__shlxli"; break;
	case CCXL_BINOP_SHR:	s0="__shrxli"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R2);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegInt128(
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
		i=BGBCC_SHXC_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_SHXC_EmitBinaryVRegVRegInt128(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegInt128(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	char *s0;
	int nm1, nm2, nm3, nm4;
	s32 imm;
	int i, j, k;
	
	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}
	
	s0=NULL;
	switch(opr)
	{
	case CCXL_UNOP_NEG:	s0="__negxli"; break;
	case CCXL_UNOP_NOT:	s0="__notxli"; break;
	}

	if(s0)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, s0);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}
