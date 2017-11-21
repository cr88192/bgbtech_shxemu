/*
Native Quadword Ops
*/


int BGBCC_SHXC_EmitBinaryVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int tr0;
	int nm1, nm2;
	int i, j, k, shl;

#if 0
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

		nm1=-1; k=0;
		if((opr==CCXL_BINOP_ADD) && (j==(sbyte)j))
			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
		if((opr==CCXL_BINOP_SUB) && ((-j)==(sbyte)(-j)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

		if(nm1>=0)
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitOpRegImm(sctx, nm1, cdreg, k);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
	}
#endif

	nm1=-1;
	if(opr==CCXL_BINOP_ADD)
		nm1=BGBCC_SH_NMID_ADD;
	if(opr==CCXL_BINOP_SUB)
		nm1=BGBCC_SH_NMID_SUB;
	if(opr==CCXL_BINOP_MUL)
		nm1=BGBCC_SH_NMID_MULL;

	if(opr==CCXL_BINOP_AND)
		nm1=BGBCC_SH_NMID_AND;
	if(opr==CCXL_BINOP_OR)
		nm1=BGBCC_SH_NMID_OR;
	if(opr==CCXL_BINOP_XOR)
		nm1=BGBCC_SH_NMID_XOR;

	if(nm1>=0)
	{
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHLDQ;
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHLRQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}else
	{
		if(opr==CCXL_BINOP_SHL)
			nm1=BGBCC_SH_NMID_SHADQ;
		if(opr==CCXL_BINOP_SHR)
			nm1=BGBCC_SH_NMID_SHARQ;
		if(opr==CCXL_BINOP_SHRR)
			nm1=BGBCC_SH_NMID_SHLRQ;
	}

	if(nm1>=0)
	{
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		BGBCC_SHX_EmitOpRegRegReg(sctx, nm1, cdreg, ctreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_MUL)
	{
		ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterDirty(ctx, sctx, dreg);

		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MULL, ctreg, cdreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_DIV)
	{
		BGBCC_SHXC_EmitScratchSyncRegisters(ctx, sctx);
	
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ5);

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ4);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ BGBCC_SHXC_EmitCallName(ctx, sctx, "__udivsq"); }
		else
			{ BGBCC_SHXC_EmitCallName(ctx, sctx, "__sdivsq"); }

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_SHXC_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ5);

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_RQ5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ4);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_RQ0);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{ BGBCC_SHXC_EmitCallName(ctx, sctx, "__umodsq"); }
		else
			{ BGBCC_SHXC_EmitCallName(ctx, sctx, "__smodsq"); }

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_RQ0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_RQ0);
		return(1);
	}


	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegQLong(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int tr0;
	int nm1, nm2;
	int i, j, k, shl;
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);

		if((j==0) && ((opr==CCXL_BINOP_ADD) || (opr==CCXL_BINOP_SUB) ||
			(opr==CCXL_BINOP_OR) || (opr==CCXL_BINOP_XOR)))
		{
			BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
			return(1);
		}

		shl=-1;
		if(!(j&(j-1)))
		{
			for(i=0; i<31; i++)
				if(j==(1<<i))
					{ shl=i; break; }
		}

		nm1=-1; k=0;
		if((opr==CCXL_BINOP_ADD) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
		if((opr==CCXL_BINOP_SUB) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_SUB; k=j; }
		if((opr==CCXL_BINOP_MUL) && (j==(sbyte)j))
			{ nm1=BGBCC_SH_NMID_MULL; k=j; }

		if((opr==CCXL_BINOP_ADD) && ((-j)==(byte)(-j)))
			{ nm1=BGBCC_SH_NMID_SUB; k=-j; }
		if((opr==CCXL_BINOP_SUB) && ((-j)==(byte)(-j)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

		if((opr==CCXL_BINOP_AND) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_AND; k=j; }
		if((opr==CCXL_BINOP_OR) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_OR; k=j; }
		if((opr==CCXL_BINOP_XOR) && (j==(byte)j))
			{ nm1=BGBCC_SH_NMID_XOR; k=j; }

		if((nm1<0) && (j==((j<<12)>>12)))
		{
			if(opr==CCXL_BINOP_ADD)
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if(opr==CCXL_BINOP_SUB)
				{ nm1=BGBCC_SH_NMID_SUB; k=j; }
			if(opr==CCXL_BINOP_MUL)
				{ nm1=BGBCC_SH_NMID_MULL; k=j; }

			if(opr==CCXL_BINOP_AND)
				{ nm1=BGBCC_SH_NMID_AND; k=j; }
			if(opr==CCXL_BINOP_OR)
				{ nm1=BGBCC_SH_NMID_OR; k=j; }
			if(opr==CCXL_BINOP_XOR)
				{ nm1=BGBCC_SH_NMID_XOR; k=j; }
		}

#if 1
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			if((opr==CCXL_BINOP_SHL) && (j==(j&31)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=j; }
			if((opr==CCXL_BINOP_SHR) && (j==(j&31)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }

			if(opr==CCXL_BINOP_MUL && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=shl; }
			if(opr==CCXL_BINOP_DIV && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-shl; }

			if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
				{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
		}else
		{
			if((opr==CCXL_BINOP_SHL) && (j==(j&31)))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=j; }
			if((opr==CCXL_BINOP_SHR) && (j==(j&31)))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=-j; }
			if((opr==CCXL_BINOP_SHRR) && (j==(j&31)))
				{ nm1=BGBCC_SH_NMID_SHLDQ; k=-j; }

			if(opr==CCXL_BINOP_MUL && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=shl; }
			if(opr==CCXL_BINOP_DIV && (shl>=0))
				{ nm1=BGBCC_SH_NMID_SHADQ; k=-shl; }

			if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
				{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
		}
#endif

		if(nm1>=0)
		{
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
			i=BGBCC_SHX_TryEmitOpRegImmReg(sctx, nm1, csreg, k, cdreg);
			if(i>0)
			{
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}

			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_QGR);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
			BGBCC_SHX_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);

			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			return(1);
		}

		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		i=BGBCC_SHXC_EmitBinaryVRegVRegQLong(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegQLong(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	nm1=-1;
	if(opr==CCXL_BINOP_ADD)
		nm1=BGBCC_SH_NMID_ADD;
	if(opr==CCXL_BINOP_SUB)
		nm1=BGBCC_SH_NMID_SUB;
	if(opr==CCXL_BINOP_MUL)
		nm1=BGBCC_SH_NMID_MULL;

	if(opr==CCXL_BINOP_AND)
		nm1=BGBCC_SH_NMID_AND;
	if(opr==CCXL_BINOP_OR)
		nm1=BGBCC_SH_NMID_OR;
	if(opr==CCXL_BINOP_XOR)
		nm1=BGBCC_SH_NMID_XOR;

	if(opr==CCXL_BINOP_SHL)
		nm1=BGBCC_SH_NMID_SHADQ;
	if(opr==CCXL_BINOP_SHR)
		nm1=BGBCC_SH_NMID_SHARQ;

	if(nm1>=0)
	{
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		BGBCC_SHX_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
	i=BGBCC_SHXC_EmitBinaryVRegVRegQLong(ctx, sctx, type, dreg, opr, treg);
	return(i);
}

int BGBCC_SHXC_EmitUnaryVRegVRegQLong(
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

	nm1=-1;
	if(opr==CCXL_UNOP_NEG)
		nm1=BGBCC_SH_NMID_NEG;
	if(opr==CCXL_UNOP_NOT)
		nm1=BGBCC_SH_NMID_NOT;

	if(nm1>=0)
	{
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if(opr==CCXL_UNOP_LNOT)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_TST, cdreg, cdreg);
			ctreg=cdreg&31;
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, ctreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_TST, csreg, csreg);
		ctreg=cdreg&31;
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, ctreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if((opr==CCXL_UNOP_INC) ||
		(opr==CCXL_UNOP_DEC))
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			j=(opr==CCXL_UNOP_INC)?1:(-1);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
//			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD, cdreg, j);
			BGBCC_SHX_EmitOpLdRegDispReg(sctx,
				BGBCC_SH_NMID_LEAB, cdreg, j, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		j=(opr==CCXL_UNOP_INC)?1:(-1);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_SHX_EmitOpLdRegDispReg(sctx,
			BGBCC_SH_NMID_LEAB, csreg, j, cdreg);

#if 0
		i=BGBCC_SHX_TryEmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_ADD, csreg, j, cdreg);
		if(i<=0)
//		if(1)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD, cdreg, j);
		}
#endif

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitJCmpVRegVRegQLong(
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

#if 0
		if(nm3>=0)
		{
			BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

			BGBCC_SHX_EmitOpCmpRegRegLabel(sctx, nm3, ctreg, csreg, lbl);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}
#endif

		BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, csreg);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);

		BGBCC_SHX_EmitOpAutoLabel(sctx, nm2, lbl);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}
