int BGBCC_SHXC_EmitBinaryVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2, nm3, nm4;
	int i, j, k;

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
//	if(0)
	{
		j=BGBCC_CCXL_GetRegImmIntValue(ctx, treg);
		nm1=-1; k=0;
		if((opr==CCXL_BINOP_ADD) && (j==(sbyte)j))
			{ nm1=BGBCC_SH_NMID_ADD; k=j; }
		if((opr==CCXL_BINOP_SUB) && ((-j)==(sbyte)(-j)))
			{ nm1=BGBCC_SH_NMID_ADD; k=-j; }

		if(sctx->has_bjx1ari && (nm1<0))
		{
			if((opr==CCXL_BINOP_ADD) && (j==(s16)j))
				{ nm1=BGBCC_SH_NMID_ADD; k=j; }
			if((opr==CCXL_BINOP_SUB) && ((-j)==(s16)(-j)))
				{ nm1=BGBCC_SH_NMID_ADD; k=-j; }
		}

		if(nm1>=0)
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitOpRegImm(sctx, nm1, cdreg, k);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		switch(opr)
		{
		case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD; nm2=-1; break;
		case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB; nm2=-1; break;
		case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND; nm2=-1; break;
		case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR; nm2=-1; break;
		case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR; nm2=-1; break;
		default:		nm1=-1; nm2=-1; break;
		}

		if(nm1>=0)
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			
			k=j; nm3=nm1;
			if(opr==CCXL_BINOP_SUB)
				{ nm3=BGBCC_SH_NMID_ADD; k=-j; }
			i=BGBCC_SHX_TryEmitOpRegImm(sctx, nm3, cdreg, k);
			if(i>0)
			{
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}
			
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, j);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}
		
		nm1=-1;	nm2=-1;
		nm3=-1;	nm4=-1;

#if 1
		if(opr==CCXL_BINOP_SHL)
		{
			switch(j)
			{
			case 1: nm1=BGBCC_SH_NMID_SHLL1; break;
			case 2: nm1=BGBCC_SH_NMID_SHLL2; break;
			case 3: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL1; break;
			case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 8: nm1=BGBCC_SH_NMID_SHLL8; break;
			case 9: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL1; break;
			case 10: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 16: nm1=BGBCC_SH_NMID_SHLL16; break;
			case 17: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL1; break;
			case 18: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 24: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL8; break;
			}
		}

		if((opr==CCXL_BINOP_SHRR) ||
			((opr==CCXL_BINOP_SHR) &&
				BGBCC_CCXL_TypeUnsignedP(ctx, type)))
		{
			nm1=-1;	nm2=-1;
			nm3=-1;	nm4=-1;
			switch(j)
			{
			case 1: nm1=BGBCC_SH_NMID_SHLR; break;
			case 2: nm1=BGBCC_SH_NMID_SHLR2; break;
			case 8: nm1=BGBCC_SH_NMID_SHLR8; break;
			case 16: nm1=BGBCC_SH_NMID_SHLR16; break;

			case 3: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR; break;
			case 4: nm1=BGBCC_SH_NMID_SHLR2; nm2=BGBCC_SH_NMID_SHLR2; break;
			case 9: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR; break;
			case 10: nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2; break;
			case 17: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR; break;
			case 18: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2; break;
			case 24: nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8; break;
			}
		}

		if((opr==CCXL_BINOP_SHR) && (j==1) &&
			!BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHAR;
#endif

		if(sctx->has_bjx1ari && (nm2>=0))
			{ nm1=-1; }

		if(nm1>=0)
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg);
			if(nm2>=0)
				BGBCC_SHX_EmitOpReg(sctx, nm2, cdreg);
			if(nm3>=0)
				BGBCC_SHX_EmitOpReg(sctx, nm3, cdreg);
			if(nm4>=0)
				BGBCC_SHX_EmitOpReg(sctx, nm4, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		if(sctx->has_shad)
		{
			if(opr==CCXL_BINOP_SHL)
			{
				nm1=BGBCC_SH_NMID_SHAD; k=j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_SHLD;
			}
			if(opr==CCXL_BINOP_SHR)
			{
				nm1=BGBCC_SH_NMID_SHAD; k=-j;
				if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
					nm1=BGBCC_SH_NMID_SHLD;
			}
		}

		if(nm1>=0)
		{
//			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			
			if(BGBCC_SHX_TryEmitOpRegImmReg(sctx, nm1, cdreg, k, cdreg)>0)
			{
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
//				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
				return(1);
			}
			
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}

#if 1
		if((opr==CCXL_BINOP_DIV) && (j>0))
		{
			if(j==1)
				{ return(1); }

			if(j==2)
			{
				cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_SHX_EmitOpReg(sctx,
					BGBCC_SH_NMID_SHAR, cdreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				return(1);
			}

//			k=(s32)((4294967296LL+(j/2))/j);
			k=(s32)((4294967296LL+(j-1))/j);

			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				ctreg, k);

			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				BGBCC_SHX_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_DMULU, ctreg, cdreg);
			}else
			{
				BGBCC_SHX_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_DMULS, ctreg, cdreg);
			}

			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACH, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
			return(1);
		}

		if((opr==CCXL_BINOP_MOD) && (j>0))
		{
			if(j==1)
				{ return(1); }

//			if(j==2)
			if((j>0) && !(j&(j-1)))
			{
				ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
				BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, j-1);
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_AND, ctreg, cdreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
				return(1);
			}

//			k=(s32)((4294967296LL+(j/2))/j);
			k=(s32)((4294967296LL+(j-1))/j);

			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			csreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);

			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
				BGBCC_SHX_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_DMULU, ctreg, cdreg);
			}else
			{
				BGBCC_SHX_EmitOpRegReg(sctx,
					BGBCC_SH_NMID_DMULS, ctreg, cdreg);
			}

			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACH, csreg);

			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				ctreg, j);
			BGBCC_SHX_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_DMULU, ctreg, csreg);

			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACL, csreg);
			BGBCC_SHX_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_SUB, csreg, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, csreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);

			return(1);
		}
#endif
	}
	
	nm1=-1; nm2=-1;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD;	nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB;	nm2=-1; break;
	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND;	nm2=-1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR;	nm2=-1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR;	nm2=-1; break;

	case CCXL_BINOP_SHL:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;	nm2=-1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLD;
			break;
		}
		break;

	case CCXL_BINOP_SHR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;
			nm2=BGBCC_SH_NMID_NEG;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLD;
			break;
		}
		break;
	case CCXL_BINOP_SHRR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHLD;
			nm2=BGBCC_SH_NMID_NEG;
			break;
		}
		break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
		{
			cdreg=BGBCC_SHXC_EmitTryGetRegisterDirty(ctx, sctx, dreg);

			if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
				cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

			if(nm2>=0)
			{
				csreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, csreg);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, cdreg);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, csreg);
			}else
			{
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, cdreg, cdreg);
			}

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterDirty(ctx, sctx, dreg);

		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

//		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

		if(nm2>=0)
		{
			csreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg, csreg);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, cdreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, csreg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
		}
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

//		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

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
	
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__udivsi3");
		}else
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__sdivsi3");
		}

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_SHXC_EmitScratchSyncRegisters(ctx, sctx);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
//		BGBCC_SHXC_EmitCallName(ctx, sctx, "__moddi3");

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);

		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__umodsi3");
		}else
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__smodsi3");
		}

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitBinaryVRegVRegVRegInt(
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

		if(sctx->has_bjx1ari)
		{
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

				if((opr==CCXL_BINOP_ADD) && (j==(s16)j))
					{ nm1=-1; }		/* more efficient special case */
				if(j==((j<<20)>>20))
					{ nm1=-1; }		/* more efficient special case */
			}

#if 1
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			{
#if 1
				if((opr==CCXL_BINOP_SHL) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHLD; k=j; }
				if((opr==CCXL_BINOP_SHR) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHLD; k=-j; }
//				if((opr==CCXL_BINOP_SHRR) && (j==(j&31)))
//					{ nm1=BGBCC_SH_NMID_SHLD; k=-j; }

				if(opr==CCXL_BINOP_MUL && (shl>=0))
					{ nm1=BGBCC_SH_NMID_SHLD; k=shl; }
				if(opr==CCXL_BINOP_DIV && (shl>=0))
					{ nm1=BGBCC_SH_NMID_SHLD; k=-shl; }

				if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
					{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
#endif
			}else
			{
				if((opr==CCXL_BINOP_SHL) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHAD; k=j; }
				if((opr==CCXL_BINOP_SHR) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHAD; k=-j; }
				if((opr==CCXL_BINOP_SHRR) && (j==(j&31)))
					{ nm1=BGBCC_SH_NMID_SHLD; k=-j; }

#if 1
				if(opr==CCXL_BINOP_MUL && (shl>=0))
					{ nm1=BGBCC_SH_NMID_SHAD; k=shl; }
				if(opr==CCXL_BINOP_DIV && (shl>=0))
					{ nm1=BGBCC_SH_NMID_SHAD; k=-shl; }

				if(opr==CCXL_BINOP_MOD && (shl>0) && ((j-1)==(byte)(j-1)))
					{ nm1=BGBCC_SH_NMID_AND; k=j-1; }
#endif
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

				ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
				BGBCC_SHX_EmitOpRegRegReg(sctx, nm1, csreg, ctreg, cdreg);

				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				return(1);
			}
			
//			if(((sbyte)j)==j)
//			{
//				printf("BGBCC_SHXC_EmitBinaryVRegVRegVRegInt: "
//					"Fallthrough, opr=%d j=%d\n", opr, j);
//			}
		}

		if(!BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
			BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		i=BGBCC_SHXC_EmitBinaryVRegVRegInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR) ||
			(opr==CCXL_BINOP_XOR))
		{
			i=BGBCC_SHXC_EmitBinaryVRegVRegVRegInt(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}

	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, treg))
	{
		if((opr==CCXL_BINOP_ADD) ||
			(opr==CCXL_BINOP_MUL) ||
			(opr==CCXL_BINOP_AND) ||
			(opr==CCXL_BINOP_OR) ||
			(opr==CCXL_BINOP_XOR))
		{
			i=BGBCC_SHXC_EmitBinaryVRegVRegVRegInt(ctx, sctx, type,
				dreg, opr, treg, sreg);
			return(i);
		}
	}
	
	nm1=-1; nm2=-1;
	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD; nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB; nm2=-1; break;
	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND; nm2=-1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR; nm2=-1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR; nm2=-1; break;

	case CCXL_BINOP_SHL:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;	nm2=-1;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLD;
			break;
		}
		break;

	case CCXL_BINOP_SHR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHAD;
			nm2=BGBCC_SH_NMID_NEG;
			if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
				nm1=BGBCC_SH_NMID_SHLD;
			break;
		}
		break;
	case CCXL_BINOP_SHRR:
		if(sctx->has_shad)
		{
			nm1=BGBCC_SH_NMID_SHLD;
			nm2=BGBCC_SH_NMID_NEG;
			break;
		}
		break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

//		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

//		if((nm2<0) &&
//			(nm1!=BGBCC_SH_NMID_SHAD) &&
//			(nm1!=BGBCC_SH_NMID_SHLD))
		if(nm2<0)
		{
			i=BGBCC_SHX_TryEmitOpRegRegReg(sctx,
				nm1, csreg, ctreg, cdreg);
			if(i>0)
			{
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
				BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
				return(1);
			}
		}

//		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		if(nm2>=0)
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg, tr0);
			if(csreg!=cdreg)
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, tr0, cdreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);

//			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg, BGBCC_SH_REG_R0);
//			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R0, cdreg);
		}else if(ctreg==cdreg)
		{
			tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, ctreg, tr0);
			if(csreg!=cdreg)
				BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, tr0, cdreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
		}

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_MUL)
	{
		csreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitTryGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitTryGetRegisterWrite(ctx, sctx, dreg);

		if((csreg<0) || (csreg==BGBCC_SH_REG_ZZR))
			csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		if((ctreg<0) || (ctreg==BGBCC_SH_REG_ZZR))
			ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		if((cdreg<0) || (cdreg==BGBCC_SH_REG_ZZR))
			cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		i=BGBCC_SHX_TryEmitOpRegRegReg(sctx,
			BGBCC_SH_NMID_MULL, csreg, ctreg, cdreg);
		if(i>0)
		{
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
			return(1);
		}

//		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
//		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
//		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MULL, ctreg, csreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_DIV)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);

//		BGBCC_SHXC_EmitCallName(ctx, sctx, "__sdivsi3");
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__udivsi3");
		}else
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__sdivsi3");
		}

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);

//		BGBCC_SHXC_EmitCallName(ctx, sctx, "__moddi3");
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__umodsi3");
		}else
		{
			BGBCC_SHXC_EmitCallName(ctx, sctx, "__smodsi3");
		}

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R5);

		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitBinaryVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_SHXC_EmitBinaryVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		return(BGBCC_SHXC_EmitBinaryVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(	BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_SHXC_EmitBinaryVRegVRegVRegFloat(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		return(BGBCC_SHXC_EmitBinaryVRegVRegVRegLong(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		return(BGBCC_SHXC_EmitBinaryVRegVRegVRegInt128(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitUnaryVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	s32 imm;
	int i, j, k;
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		nm1=-1; // k=0;

		if(opr==CCXL_UNOP_MOV)
		{
			BGBCC_SHXC_EmitMovVRegImm(ctx, sctx, type, dreg, imm);
			return(1);
		}

#if 0
		if(nm1>=0)
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, imm);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R0, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
#endif
	}

	if(opr==CCXL_UNOP_MOV)
	{
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		return(1);
	}

	switch(opr)
	{
	case CCXL_UNOP_MOV:	nm1=BGBCC_SH_NMID_MOV; nm2=-1; break;
	case CCXL_UNOP_NEG:	nm1=BGBCC_SH_NMID_NEG; nm2=-1; break;
	case CCXL_UNOP_NOT:	nm1=BGBCC_SH_NMID_NOT; nm2=-1; break;
	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);

			if(nm2>=0)
			{
				ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_SHX_EmitOpRegReg(sctx, nm2, cdreg, ctreg);
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
			}else
			{
				BGBCC_SHX_EmitOpRegReg(sctx, nm1, cdreg, cdreg);
			}

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		if(nm2>=0)
		{
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, csreg, ctreg);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
			BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, ctreg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, cdreg);
		}
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
			BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_TST, csreg, csreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, cdreg);
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
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD, cdreg, j);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		j=(opr==CCXL_UNOP_INC)?1:(-1);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		i=BGBCC_SHX_TryEmitOpRegImmReg(sctx,
			BGBCC_SH_NMID_ADD, csreg, j, cdreg);
		if(i<=0)
//		if(1)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
			BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD, cdreg, j);
		}
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitUnaryVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_SHXC_EmitUnaryVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_SHXC_EmitUnaryVRegVRegFloat(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, type))
	{
		return(BGBCC_SHXC_EmitUnaryVRegVRegLong(ctx, sctx,
			type, dreg, opr, sreg));
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		return(BGBCC_SHXC_EmitUnaryVRegVRegInt128(ctx, sctx,
			type, dreg, opr, sreg));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}



int BGBCC_SHXC_EmitCompareVRegVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg, int cmp,
	ccxl_register sreg, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	s32 imm;
	int nm1, nm2, cmp1;
	int i;

#if 0
	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
		nm1=-1; // k=0;
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, sreg))
	{
//		imm=BGBCC_CCXL_GetRegImmIntValue(ctx, sreg);
//		nm1=-1; // k=0;

		switch(cmp)
		{
		case CCXL_CMP_EQ:	cmp1=CCXL_CMP_EQ;	break;
		case CCXL_CMP_NE:	cmp1=CCXL_CMP_NE;	break;
		case CCXL_CMP_LT:	cmp1=CCXL_CMP_GT;	break;
		case CCXL_CMP_GT:	cmp1=CCXL_CMP_LT;	break;
		case CCXL_CMP_LE:	cmp1=CCXL_CMP_GE;	break;
		case CCXL_CMP_GE:	cmp1=CCXL_CMP_LE;	break;
		default:			cmp1=-1;			break;
		}

		i=BGBCC_SHXC_EmitCompareVRegVRegVRegInt(ctx, sctx,
			type, dreg, cmp1, treg, sreg);
		return(i);
	}
#endif
	
	switch(cmp)
	{
	case CCXL_CMP_EQ:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_NE:
		nm1=BGBCC_SH_NMID_CMPEQ;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_LT:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GT:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BT;
		break;
	case CCXL_CMP_LE:
		nm1=BGBCC_SH_NMID_CMPGT;
		nm2=BGBCC_SH_NMID_BF;
		break;
	case CCXL_CMP_GE:
		nm1=BGBCC_SH_NMID_CMPGE;
		nm2=BGBCC_SH_NMID_BT;
		break;
	default:
		nm1=-1;
		nm2=-1;
		break;
	}
	
	if((nm1>=0) && (nm2>=0))
	{
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

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

int BGBCC_SHXC_EmitCompareVRegVRegVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr,
	ccxl_register sreg, ccxl_register treg)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, type))
	{
		return(BGBCC_SHXC_EmitCompareVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		return(BGBCC_SHXC_EmitCompareVRegVRegVRegInt(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	if(	BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, type))
	{
		return(BGBCC_SHXC_EmitCompareVRegVRegVRegFloat(ctx, sctx,
			type, dreg, opr, sreg, treg));
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitReturnVoid(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx)
{
	int i, j, k;

	i=BGBCC_SHX_EmitOpAutoLabel(sctx,
		BGBCC_SH_NMID_BRA, sctx->lbl_ret);
	BGBCC_SHX_EmitOpNone(sctx,
		BGBCC_SH_NMID_NOP);
	return(i);
}

int BGBCC_SHXC_EmitReturnVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register sreg)
{
	int rcls;
	int i, j, k;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		BGBCC_CCXL_TypePointerP(ctx, type))
//	{
//		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
//	}

	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);

	if((rcls==BGBCC_SH_REGCLS_GR) ||
		(rcls==BGBCC_SH_REGCLS_VO_REF) ||
		(rcls==BGBCC_SH_REGCLS_AR_REF))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
	}else if((rcls==BGBCC_SH_REGCLS_GR2))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR0);
	}else if((rcls==BGBCC_SH_REGCLS_FR))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_FR0);
	}else if((rcls==BGBCC_SH_REGCLS_FR2))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR0);
	}else if(rcls==BGBCC_SH_REGCLS_VO_GR)
	{
		BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx,
			sreg, BGBCC_SH_REG_R0);
	}else if(rcls==BGBCC_SH_REGCLS_VO_GR2)
	{
		BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx,
			sreg, BGBCC_SH_REG_LR0);
	}else
	{
		BGBCC_DBGBREAK
	}

	i=BGBCC_SHX_EmitOpAutoLabel(sctx,
		BGBCC_SH_NMID_BRA, sctx->lbl_ret);
	BGBCC_SHX_EmitOpNone(sctx,
		BGBCC_SH_NMID_NOP);
	return(i);
}

int BGBCC_SHXC_EmitCsrvVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg)
{
	int ctreg;
	int rcls;
	int i, j, k;

	sctx->is_leaf=0;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
#if 1
//		if(sctx->frm_size>56)
		if(sctx->has_bjx1mov && !sctx->is_pic &&
			BGBCC_CCXL_IsRegGlobalP(ctx, dreg))
		{
//			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
			return(1);
		}
#endif
	
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);
		return(1);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
#if 0
		if(sctx->frm_size>56)
		{
//			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R2);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_R3);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			return(1);
		}
#endif
	
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);
		return(1);
	}


	if(	BGBCC_CCXL_TypeFloatP(ctx, type) ||
		BGBCC_CCXL_TypeFloat16P(ctx, type))
	{	
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_FR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_FR0);
		return(1);
	}

	if(BGBCC_CCXL_TypeDoubleP(ctx, type))
	{	
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_DR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_DR0);
		return(1);
	}
	
	if(type.val==CCXL_TY_V)
	{
		return(1);
	}

	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);
	if(rcls==BGBCC_SH_REGCLS_VO_REF)
	{
		return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitCallBuiltin(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	char *name)
{
	int l0, l1, l2;

#if 0
	if(!strcmp(name, "strcpy"))
	{
		l0=BGBCC_SHX_GenLabelTemp(sctx);
		
		BGBCC_SHX_EmitLabel(sctx, l0);

		BGBCC_SHX_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R5, BGBCC_SH_REG_R3);
		BGBCC_SHX_EmitOpRegStReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R3, BGBCC_SH_REG_R4);
		BGBCC_SHX_EmitOpRegImm(sctx,
			BGBCC_SH_NMID_ADD, BGBCC_SH_REG_R4, 1);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_TST, BGBCC_SH_REG_R3, BGBCC_SH_REG_R3);
		BGBCC_SHX_EmitOpLabel(sctx, BGBCC_SH_NMID_BF, l0);
		return(1);
	}

	if(!strcmp(name, "strcmp"))
	{
		l0=BGBCC_SHX_GenLabelTemp(sctx);
		l1=BGBCC_SHX_GenLabelTemp(sctx);
		
		BGBCC_SHX_EmitLabel(sctx, l0);

		BGBCC_SHX_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R4, BGBCC_SH_REG_R2);
		BGBCC_SHX_EmitOpLdIncRegReg(sctx,
			BGBCC_SH_NMID_MOVB, BGBCC_SH_REG_R5, BGBCC_SH_REG_R3);

		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_TST, BGBCC_SH_REG_R2, BGBCC_SH_REG_R2);
		BGBCC_SHX_EmitOpLabel(sctx, BGBCC_SH_NMID_BT, l1);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_CMPEQ, BGBCC_SH_REG_R2, BGBCC_SH_REG_R3);
		BGBCC_SHX_EmitOpLabel(sctx, BGBCC_SH_NMID_BT, l0);

		BGBCC_SHX_EmitLabel(sctx, l1);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_MOV, BGBCC_SH_REG_R2, BGBCC_SH_REG_R0);
		BGBCC_SHX_EmitOpRegReg(sctx,
			BGBCC_SH_NMID_SUB, BGBCC_SH_REG_R3, BGBCC_SH_REG_R0);

		return(1);
	}
#endif

	return(0);
}

int BGBCC_SHXC_EmitCallBuiltinArgs(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dst,
	char *name,
	int narg, ccxl_register *args)
{
	ccxl_type dty, sty, bty;
	char *s0, *s1;
	int l0, l1;
	int tr0, tr1;
	int sz, al;

	if(!strcmp(name, "__debugbreak"))
	{
		BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_BRK);
		return(1);
	}

#if 1
	if(!strcmp(name, "memcpy") && (narg==3))
	{
		if(!BGBCC_CCXL_IsRegImmIntP(ctx, args[2]))
			return(0);

		sz=BGBCC_CCXL_GetRegImmIntValue(ctx, args[2]);

		dty=BGBCC_CCXL_GetRegType(ctx, args[0]);
		sty=BGBCC_CCXL_GetRegType(ctx, args[1]);
		if(dty.val!=sty.val)
			return(0);
		BGBCC_CCXL_TypeDerefType(ctx, dty, &bty);

		sctx->sreg_live=sctx->sreg_held;
		sctx->sfreg_live=sctx->sfreg_held;

		al=1;
		if(BGBCC_CCXL_TypeSgIntP(ctx, bty) ||
			BGBCC_CCXL_TypeSgNLongP(ctx, bty) ||
			BGBCC_CCXL_TypeSgLongP(ctx, bty) ||
			BGBCC_CCXL_TypeFloatP(ctx, bty) ||
			BGBCC_CCXL_TypeFloat16P(ctx, type) ||
			BGBCC_CCXL_TypeDoubleP(ctx, bty))
				al=4;
		if(BGBCC_CCXL_TypeValueObjectP(ctx, bty))
			al=4;

		tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, args[0], tr0);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, args[1], tr1);
		BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, tr0, tr1, sz, al);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
		return(1);
	}

	if(!strcmp(name, "strcpy") && (narg==2))
	{
		if(!BGBCC_CCXL_IsRegImmStringP(ctx, args[1]))
			return(0);

		dty=BGBCC_CCXL_GetRegType(ctx, args[0]);

		if(BGBCC_CCXL_IsRegLocalP(ctx, args[0]))
		{
			if(BGBCC_CCXL_TypeArrayP(ctx, dty))
			{
				sctx->sreg_live=sctx->sreg_held;
				sctx->sfreg_live=sctx->sfreg_held;

				s0=BGBCC_CCXL_GetRegImmStringValue(ctx, args[1]);
//				l0=BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
				sz=strlen(s0);
				sz=(sz+4)&(~3);
				al=4;

				tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
				tr1=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, 0);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, args[0], tr0);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, args[1], tr1);
				BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx, tr0, tr1, sz, al);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr1);
				return(1);
			}
		}
	}
#endif

	return(0);
}

int BGBCC_SHXC_EmitCallFcnVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register fcn)
{
	ccxl_type tty;
	int i, j, k;

	sctx->is_leaf=0;

	tty=BGBCC_CCXL_GetRegType(ctx, fcn);
	if(sctx->has_bjx1mov && !sctx->is_pic &&
		BGBCC_CCXL_IsRegGlobalP(ctx, fcn) &&
		BGBCC_CCXL_TypeFunctionP(ctx, tty) &&
		!BGBCC_CCXL_TypePointerP(ctx, tty))
	{
		j=BGBCC_CCXL_GetRegID(ctx, fcn);
		k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, j);
		BGBCC_SHX_EmitLoadRegLabelVarRel24(sctx, BGBCC_SH_NMID_JSR,
			BGBCC_SH_REG_R3, k);
		return(1);
	}

	BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
		fcn, BGBCC_SH_REG_R3);
	i=BGBCC_SHX_EmitOpReg(sctx,
		BGBCC_SH_NMID_JSR, BGBCC_SH_REG_R3);
	BGBCC_SHX_EmitOpNone(sctx,
		BGBCC_SH_NMID_NOP);
	return(i);
}

int BGBCC_SHXC_EmitCallName(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	char *name)
{
	int i;

	sctx->is_leaf=0;

	sctx->sreg_live&=~1;
	BGBCC_SHXC_EmitLoadGblNameReg(ctx, sctx,
		name, BGBCC_SH_REG_R3);
//	sctx->sreg_live|=1;
	i=BGBCC_SHX_EmitOpReg(sctx,
		BGBCC_SH_NMID_JSR, BGBCC_SH_REG_R3);
	BGBCC_SHX_EmitOpNone(sctx,
		BGBCC_SH_NMID_NOP);
	return(i);
}

int BGBCC_SHXC_EmitCallVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dst,
	ccxl_register fcn,
	int narg, ccxl_register *args)
{
	ccxl_register treg;
	ccxl_type tty;
	char *fname;
	int tr0, tr1;
	int ni, nf, ns, rcls, retrcls;
	int i, j, k;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		BGBCC_CCXL_TypePointerP(ctx, type))
//	{
//		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		return(1);
//	}

	fname=NULL;
	if((fcn.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		i=(int)(fcn.val&CCXL_REGID_REGMASK);
		fname=ctx->reg_globals[i]->name;
	}
	
	if(fname && BGBCC_SHXC_EmitCallBuiltinArgs(
		ctx, sctx, type, dst, fname, narg, args))
			return(1);

	tty=BGBCC_CCXL_GetRegType(ctx, dst);
	retrcls=BGBCC_SHXC_TypeGetRegClassP(ctx, tty);

	if(retrcls==BGBCC_SH_REGCLS_VO_REF)
	{
//		sctx->sreg_live|=0x0004;
		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
//		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
//			dst, BGBCC_SH_REG_R2);

//		BGBCC_SHXC_EmitLdaFrameVRegReg(ctx, sctx,
//			dst, BGBCC_SH_REG_R2);
//		if(BGBCC_CCXL_IsRegTempP(ctx, dst))
//		{
//			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx,
//				dst, BGBCC_SH_REG_R2);
//		}

		BGBCC_SHXC_EmitLdaValSyncFrameVRegReg(ctx, sctx,
			dst, BGBCC_SH_REG_R2);
	}

	ni=0; nf=0; ns=0;
	for(j=0; j<narg; j++)
	{
		treg=args[j];
		tty=BGBCC_CCXL_GetRegType(ctx, treg);

		rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, tty);

//		if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
//			BGBCC_CCXL_TypePointerP(ctx, tty) ||
//			BGBCC_CCXL_TypeArrayP(ctx, tty) ||
//			BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		if((rcls==BGBCC_SH_REGCLS_GR) ||
			(rcls==BGBCC_SH_REGCLS_VO_REF) ||
			(rcls==BGBCC_SH_REGCLS_AR_REF))
		{
			if(ni<4)
			{
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
				ni++;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_R3;
				tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
				ns++;
				continue;
			}
		}

//		if(BGBCC_CCXL_TypeSmallLongP(ctx, tty))
		if(rcls==BGBCC_SH_REGCLS_GR2)
		{
			if(nf<3)
			{
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR4+ni);
				ni+=2;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_LR1;
				tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}

		if(rcls==BGBCC_SH_REGCLS_VO_GR)
		{
			if(ni<4)
			{
				BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R4+ni);
				ni++;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_R3;
				tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
				BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx, treg, tr0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
				ns++;
				continue;
			}
		}

		if(rcls==BGBCC_SH_REGCLS_VO_GR2)
		{
			if(nf<3)
			{
				BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR4+ni);
				ni+=2;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_LR1;
				tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR2);
				BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx, treg, tr0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}
		
//		if(BGBCC_CCXL_TypeFloatP(ctx, tty))
		if(rcls==BGBCC_SH_REGCLS_FR)
		{
			if(nf<8)
			{
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_FR4+nf);
				nf++;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_FR0;
				tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
				ns++;
				continue;
			}
		}
		
//		if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		if(rcls==BGBCC_SH_REGCLS_FR2)
		{
			if(nf<7)
			{
				if(nf&1)nf++;
//				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
//					treg, BGBCC_SH_REG_DR2+(nf/2));
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_DR4+nf);
				nf+=2;
				continue;
			}else
			{
//				tr0=BGBCC_SH_REG_DR0;
				tr0=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_FR2);
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, tr0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx, ns*4, tr0);
				BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, tr0);
				ns+=2;
				continue;
			}
		}

		BGBCC_CCXL_StubError(ctx);
	}

	if(fname && BGBCC_SHXC_EmitCallBuiltin(ctx, sctx, fname))
		return(1);

	BGBCC_SHXC_ResetFpscrDefaults(ctx, sctx);
	BGBCC_SHXC_EmitCallFcnVReg(ctx, sctx, fcn);

	if(retrcls==BGBCC_SH_REGCLS_VO_REF)
	{
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
	}

	return(1);
}
