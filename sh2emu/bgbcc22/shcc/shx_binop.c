int BGBCC_SHXC_EmitBinaryVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register treg)
{
	int csreg, ctreg, cdreg;
	int nm1, nm2;
	int i, j, k;

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
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, j);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R0, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
		
		if(opr==CCXL_BINOP_SHL)
		{
			switch(j)
			{
			case 1: nm1=BGBCC_SH_NMID_SHLL; break;
			case 2: nm1=BGBCC_SH_NMID_SHLL2; break;
			case 8: nm1=BGBCC_SH_NMID_SHLL8; break;
			case 16: nm1=BGBCC_SH_NMID_SHLL16; break;
			}
		}

		if(opr==CCXL_BINOP_SHRR)
		{
			switch(j)
			{
			case 1: nm1=BGBCC_SH_NMID_SHLR; break;
			case 2: nm1=BGBCC_SH_NMID_SHLR2; break;
			case 8: nm1=BGBCC_SH_NMID_SHLR8; break;
			case 16: nm1=BGBCC_SH_NMID_SHLR16; break;
			}
		}

		if((opr==CCXL_BINOP_SHR) && (j==1))
			nm1=BGBCC_SH_NMID_SHAR;

		if(nm1>=0)
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitOpReg(sctx, nm1, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}

		if(opr==CCXL_BINOP_SHL)
			{ nm1=BGBCC_SH_NMID_SHAD; k=j; }
		if(opr==CCXL_BINOP_SHR)
			{ nm1=BGBCC_SH_NMID_SHAD; k=-j; }

		if(nm1>=0)
		{
			cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
			BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
				BGBCC_SH_REG_R0, j);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R0, cdreg);
			BGBCC_SHXC_EmitReleaseRegister(ctx, sctx, dreg);
			return(1);
		}
	}
	
	switch(opr)
	{
	case CCXL_BINOP_ADD:	nm1=BGBCC_SH_NMID_ADD;	nm2=-1; break;
	case CCXL_BINOP_SUB:	nm1=BGBCC_SH_NMID_SUB;	nm2=-1; break;
	case CCXL_BINOP_AND:	nm1=BGBCC_SH_NMID_AND;	nm2=-1; break;
	case CCXL_BINOP_OR:		nm1=BGBCC_SH_NMID_OR;	nm2=-1; break;
	case CCXL_BINOP_XOR:	nm1=BGBCC_SH_NMID_XOR;	nm2=-1; break;

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
		cdreg=BGBCC_SHXC_EmitGetRegisterDirty(ctx, sctx, dreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);
		if(nm2>=0)
		{
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, ctreg, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R0, cdreg);
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
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

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
		cdreg=BGBCC_SHXC_EmitGetRegisterWrite(ctx, sctx, dreg);
		csreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, sreg);
		ctreg=BGBCC_SHXC_EmitGetRegisterRead(ctx, sctx, treg);

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

	BGBCC_CCXL_StubError(ctx);
	return(0);
}


int BGBCC_SHXC_EmitUnaryVRegVRegInt(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg,
	int opr, ccxl_register sreg)
{
	int csreg, cdreg;
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
			BGBCC_SHX_EmitOpRegReg(sctx, nm2, csreg, BGBCC_SH_REG_R0);
			BGBCC_SHX_EmitOpRegReg(sctx, nm1, BGBCC_SH_REG_R0, cdreg);
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
	return(i);
}

int BGBCC_SHXC_EmitReturnVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register sreg)
{
	int i, j, k;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHXC_EmitLoadVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R0);
	}

	i=BGBCC_SHX_EmitOpAutoLabel(sctx,
		BGBCC_SH_NMID_BRA, sctx->lbl_ret);
	return(i);
}

int BGBCC_SHXC_EmitCsrvVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register dreg)
{
	int i, j, k;

	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
		BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
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
	return(i);
}

int BGBCC_SHXC_EmitCallVReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register fcn,
	int narg, ccxl_register *args)
{
	ccxl_register treg;
	int ni, nf, ns;
	int i, j, k;

//	if(BGBCC_CCXL_TypeSmallIntP(ctx, type) ||
//		BGBCC_CCXL_TypePointerP(ctx, type))
//	{
//		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
//		return(1);
//	}

	ni=0; nf=0; ns=0;
	for(j=0; j<narg; j++)
	{
		treg=args[j];
		if(BGBCC_CCXL_IsRegSmallIntP(ctx, treg) ||
			BGBCC_CCXL_IsRegPointerP(ctx, treg))
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
	}
	
	BGBCC_SHXC_EmitCallFcnVReg(ctx, sctx, fcn);
	return(1);
}
