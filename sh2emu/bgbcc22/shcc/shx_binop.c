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
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, j);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
		
		nm1=-1;	nm2=-1;
		nm3=-1;	nm4=-1;

#if 1
		if(opr==CCXL_BINOP_SHL)
		{
			switch(j)
			{
			case 1: nm1=BGBCC_SH_NMID_SHLL; break;
			case 2: nm1=BGBCC_SH_NMID_SHLL2; break;
			case 3: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL; break;
			case 4: nm1=BGBCC_SH_NMID_SHLL2; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 8: nm1=BGBCC_SH_NMID_SHLL8; break;
			case 9: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL; break;
			case 10: nm1=BGBCC_SH_NMID_SHLL8; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 16: nm1=BGBCC_SH_NMID_SHLL16; break;
			case 17: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL; break;
			case 18: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL2; break;
			case 24: nm1=BGBCC_SH_NMID_SHLL16; nm2=BGBCC_SH_NMID_SHLL8; break;
			}
		}

		if((opr==CCXL_BINOP_SHRR) ||
			((opr==CCXL_BINOP_SHR) &&
				BGBCC_CCXL_TypeUnsignedP(ctx, type)))
		{
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

#if 0
			case 11:
				nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR;
				break;
			case 12:
				nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR2;
				break;
			case 13:
				nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR;
				break;
			case 14:
				nm1=BGBCC_SH_NMID_SHLR8; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR2;
				break;

			case 19:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR;
				break;
			case 20:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR2;
				break;
			case 21:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR;
				break;
			case 22:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR2;
				nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR2;
				break;

			case 25:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8;
				nm3=BGBCC_SH_NMID_SHLR;
				break;
			case 26:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8;
				nm3=BGBCC_SH_NMID_SHLR2;
				break;
			case 27:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8;
				nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR;
				break;
			case 28:
				nm1=BGBCC_SH_NMID_SHLR16; nm2=BGBCC_SH_NMID_SHLR8;
				nm3=BGBCC_SH_NMID_SHLR2; nm4=BGBCC_SH_NMID_SHLR2;
				break;
#endif
			}
		}

		if((opr==CCXL_BINOP_SHR) && (j==1))
			nm1=BGBCC_SH_NMID_SHAR;
#endif

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

		if(nm1>=0)
		{
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
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

			k=(s32)((4294967296LL+(j/2))/j);

			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				ctreg, k);
			BGBCC_SHX_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_DMULS, ctreg, cdreg);
			BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
				BGBCC_SH_REG_MACH, cdreg);

			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
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
				return(1);
			}

			k=(s32)((4294967296LL+(j/2))/j);

			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			csreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);

			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV, ctreg, k);

			BGBCC_SHX_EmitOpRegReg(sctx,
				BGBCC_SH_NMID_DMULS, ctreg, cdreg);
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
			return(1);
		}
#endif
	}
	
	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD;	nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB;	nm2=-1; break;
	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND;	nm2=-1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR;	nm2=-1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR;	nm2=-1; break;

	case CCXL_BINOP_SHL:
		nm1=BGBCC_SH_NMID_SHAD;	nm2=-1;
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			nm1=BGBCC_SH_NMID_SHLD;
		break;

	case CCXL_BINOP_SHR:
		nm1=BGBCC_SH_NMID_SHAD;
		nm2=BGBCC_SH_NMID_NEG;
		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
			nm1=BGBCC_SH_NMID_SHLD;
		break;
	case CCXL_BINOP_SHRR:
		nm1=BGBCC_SH_NMID_SHLD;
		nm2=BGBCC_SH_NMID_NEG;
//		if(BGBCC_CCXL_TypeUnsignedP(ctx, type))
//			nm1=BGBCC_SH_NMID_SHLD;
		break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		if(nm2>=0)
		{
			csreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg, csreg);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, csreg, cdreg);
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
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MULL, ctreg, cdreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
			BGBCC_SH_REG_MACL, cdreg);

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_DIV)
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__sdivsi3");
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__moddi3");
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
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
	int nm1, nm2;
	int i;
	
	if(BGBCC_CCXL_RegisterIdentEqualP(ctx, dreg, sreg))
	{
		i=BGBCC_SHXC_EmitBinaryVRegVRegInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegImmIntP(ctx, treg))
	{
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, type, dreg, sreg);
		i=BGBCC_SHXC_EmitBinaryVRegVRegInt(ctx, sctx, type, dreg, opr, treg);
		return(i);
	}
	
	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD; nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB; nm2=-1; break;
	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND; nm2=-1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR; nm2=-1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR; nm2=-1; break;

	case CCXL_BINOP_SHL:	nm1=BGBCC_SH_NMID_SHAD;	nm2=-1; break;

	case CCXL_BINOP_SHR:
		nm1=BGBCC_SH_NMID_SHAD;
		nm2=BGBCC_SH_NMID_NEG;
		break;
	case CCXL_BINOP_SHRR:
		nm1=BGBCC_SH_NMID_SHLD;
		nm2=BGBCC_SH_NMID_NEG;
		break;

	default:		nm1=-1; nm2=-1; break;
	}

	if(nm1>=0)
	{
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		if(nm2>=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R0, cdreg);
		}else
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
		}

		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, treg);
		return(1);
	}

	if(opr==CCXL_BINOP_MUL)
	{
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);

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
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__sdivsi3");
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		return(1);
	}

	if(opr==CCXL_BINOP_MOD)
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, treg, BGBCC_SH_REG_R5);
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__moddi3");
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
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

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
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
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		if(nm2>=0)
		{
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, csreg, ctreg);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, ctreg, cdreg);
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
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_TST, csreg, csreg);
		BGBCC_SHX_EmitOpReg(sctx, BGBCC_SH_NMID_MOVT, cdreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
		BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, sreg);
		return(1);
	}

	if((opr==CCXL_UNOP_INC) ||
		(opr==CCXL_UNOP_DEC))
	{
		j=(opr==CCXL_UNOP_INC)?1:(-1);
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV, csreg, cdreg);
		BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD, cdreg, j);
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
	int nm1, nm2;
	
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
		cdreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, dreg);

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

	if(BGBCC_CCXL_TypeFloatP(ctx, type) ||
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

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
	}

	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, type);

	if((rcls==BGBCC_SH_REGCLS_GR) ||
		(rcls==BGBCC_SH_REGCLS_VO_GR) ||
		(rcls==BGBCC_SH_REGCLS_VO_REF) ||
		(rcls==BGBCC_SH_REGCLS_AR_REF))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
	}

	if((rcls==BGBCC_SH_REGCLS_GR2))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_LR0);
	}

	if((rcls==BGBCC_SH_REGCLS_FR))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_FR0);
	}

	if((rcls==BGBCC_SH_REGCLS_FR2))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_DR0);
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
	int i, j, k;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		if(sctx->frm_size>56)
		{
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
			return(1);
		}
	
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		return(1);
	}

	if(BGBCC_CCXL_TypeSmallLongP(ctx, type))
	{
		if(sctx->frm_size>56)
		{
			ctreg=BGBCC_SHXC_ScratchAllocReg(ctx, sctx, BGBCC_SH_REGCLS_GR);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R0, BGBCC_SH_REG_R2);
			BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_R3);
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR2);
			return(1);
		}
	
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		return(1);
	}
	return(0);
}

int BGBCC_SHXC_EmitCallFcnVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register fcn)
{
	int i;

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

	BGBCC_SHXC_EmitLoadGblNameReg(ctx, sctx,
		name, BGBCC_SH_REG_R3);
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
	int ni, nf, ns, rcls;
	int i, j, k;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		BGBCC_CCXL_TypePointerP(ctx, type))
//	{
//		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		return(1);
//	}

	tty=BGBCC_CCXL_GetRegType(ctx, dst);
	rcls=BGBCC_SHXC_TypeGetRegClassP(ctx, tty);

	if(rcls==BGBCC_SH_REGCLS_VO_REF)
	{
		sctx->sreg_live|=0x0004;
//		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
//			dst, BGBCC_SH_REG_R2);
		BGBCC_SHXC_EmitLdaFrameVRegReg(ctx, sctx,
			dst, BGBCC_SH_REG_R2);
		if(BGBCC_CCXL_IsRegTempP(ctx, dst))
		{
			BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx,
				dst, BGBCC_SH_REG_R2);
		}
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
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_R3);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx,
					ns*4, BGBCC_SH_REG_R3);
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
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR1);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx,
					ns*4, BGBCC_SH_REG_LR1);
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
				BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_R3);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx,
					ns*4, BGBCC_SH_REG_R3);
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
				BGBCC_SHXC_EmitLoadFrameVRegByValReg(ctx, sctx,
					treg, BGBCC_SH_REG_LR1);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx,
					ns*4, BGBCC_SH_REG_LR1);
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
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_FR0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx,
					ns*4, BGBCC_SH_REG_FR0);
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
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_DR2+(nf/2));
				nf+=2;
				continue;
			}else
			{
				BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx,
					treg, BGBCC_SH_REG_DR0);
				BGBCC_SHXC_EmitStoreStackOfsReg(ctx, sctx,
					ns*4, BGBCC_SH_REG_DR0);
				ns+=2;
				continue;
			}
		}

		BGBCC_CCXL_StubError(ctx);
	}

	BGBCC_SHXC_ResetFpscrDefaults(ctx, sctx);
	BGBCC_SHXC_EmitCallFcnVReg(ctx, sctx, fcn);
	return(1);
}
