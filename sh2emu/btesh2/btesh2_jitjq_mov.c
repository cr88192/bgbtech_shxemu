int BTESH2_TryJitOpcodeJQ_MovReg(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	u32 v;
	int reg1, reg2;

	if(op->nmid==BTESH2_NMID_MOV)
	{
		if(op->fmid==BTESH2_FMID_REGREG)
		{
			BTESH2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGIMM)
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rn, (s64)(op->imm));
			return(1);
		}
		
		return(0);
	}

	if(((op->nmid==BTESH2_NMID_STS) ||
		(op->nmid==BTESH2_NMID_LDS)) &&
		(op->fmid==BTESH2_FMID_REGREG) &&
		(op->rn!=BTESH2_REG_FPSCR))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
		return(1);
	}

	if(((op->nmid==BTESH2_NMID_STC) ||
		(op->nmid==BTESH2_NMID_LDC)) &&
		(op->fmid==BTESH2_FMID_REGREG) &&
		(op->rm!=BTESH2_REG_SR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
		return(1);
	}

	if((	(op->nmid==BTESH2_NMID_MOVL) ||
			(op->nmid==BTESH2_NMID_MOVW)) &&
		(op->fmid==BTESH2_FMID_REGVIMM))
	{
		BTESH2_JitStoreVMRegImm(jctx, op->rn, (s64)(op->imm));
		return(1);
	}

	if((	(op->nmid==BTESH2_NMID_MOVI20) ||
			(op->nmid==BTESH2_NMID_MOVI20S)) &&
		(op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitStoreVMRegImm(jctx, op->rn, (s64)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_LDHF16) &&
		(op->fmid==BTESH2_FMID_IMM))
	{
		v=BTESH2_LDHF16_ImmToFU32(NULL, op->imm);
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_FPUL, v);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_LDIF16) &&
		(op->fmid==BTESH2_FMID_IMM))
	{
		v=(s32)(op->imm<<16);
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_FPUL, v);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_LDSH16) &&
		(op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, 16);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RCX, (s16)(op->imm));
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}

#if 1
	if(	(op->nmid==BTESH2_NMID_FLDS) &&
		(op->fmid==BTESH2_FMID_REGRM))
	{
//		BTESH2_JitMovVMRegVMReg(jctx, BTESH2_REG_FPUL, op->rm);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_RCX);
		return(1);
	}
	
	if(	(op->nmid==BTESH2_NMID_FSTS) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
//		BTESH2_JitMovVMRegVMReg(jctx, op->rn, BTESH2_REG_FPUL);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_RCX);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_ECX);
		return(1);
	}
#endif

#if 1
	if(	(op->nmid==BTESH2_NMID_FLDI0) &&
		(op->fmid==BTESH2_FMID_FREGRN))
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, UAX_REG_ECX, 0x00000000);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_ECX);
		return(1);
	}

	if(	(op->nmid==BTESH2_NMID_FLDI1) &&
		(op->fmid==BTESH2_FMID_FREGRN))
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, UAX_REG_ECX, 0x3F800000);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_ECX);
		return(1);
	}
#endif

#if 1
	if(	(op->nmid==BTESH2_NMID_FLDS) &&
		(op->fmid==BTESH2_FMID_FREGRM))
	{
//		BTESH2_JitMovVMRegVMReg(jctx, BTESH2_REG_FPUL, op->rm);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_RCX);
		return(1);
	}
	
	if(	(op->nmid==BTESH2_NMID_FSTS) &&
		(op->fmid==BTESH2_FMID_FREGRN))
	{
//		BTESH2_JitMovVMRegVMReg(jctx, op->rn, BTESH2_REG_FPUL);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_RCX);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_ECX);
		return(1);
	}
#endif
	
	return(0);
}



int BTESH2_TryJitOpcodeJQ_ArithReg(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	int dreg, sreg;

	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			dreg=BTESH2_JitLoadReadSyncVMReg(jctx, op->rn);
			if(dreg!=UAX_REG_Z)
			{
				UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, dreg, 1);
				return(1);
			}

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, 1);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_ADD, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitAddVMRegImm(jctx, op->rn, (s64)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SUB) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_SUB, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_AND, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitAndVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_OR) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_OR, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_OR) && (op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitOrVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_XOR) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_XOR, op->rn, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_XOR) && (op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitXorVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGREGREG))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
		BTESH2_JitInsnVMRegVMReg(jctx, UAX_OP_ADD, op->ro, op->rm);
		return(1);
	}

#if 1
	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RCX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BTESH2_JitAddVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SUB) && (op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SUB, UAX_REG_RCX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BTESH2_JitAddVMRegImm(jctx, op->rn, -(s32)(op->imm)); }
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RCX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BTESH2_JitAndVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}

	if((op->nmid==BTESH2_NMID_OR) && (op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_OR, UAX_REG_RCX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BTESH2_JitOrVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}

	if((op->nmid==BTESH2_NMID_XOR) && (op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(op->rm!=op->rn)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_XOR, UAX_REG_RCX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		}else
			{ BTESH2_JitXorVMRegImm(jctx, op->rn, (s32)(op->imm)); }
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_SUB) && (op->fmid==BTESH2_FMID_REGREGREG))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
		BTESH2_JitInsnVMRegVMReg(jctx, UAX_OP_SUB, op->ro, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGREGREG))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
		BTESH2_JitInsnVMRegVMReg(jctx, UAX_OP_AND, op->ro, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_OR) && (op->fmid==BTESH2_FMID_REGREGREG))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
		BTESH2_JitInsnVMRegVMReg(jctx, UAX_OP_OR, op->ro, op->rm);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_XOR) && (op->fmid==BTESH2_FMID_REGREGREG))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->ro, op->rn);
		BTESH2_JitInsnVMRegVMReg(jctx, UAX_OP_XOR, op->ro, op->rm);
		return(1);
	}

#if 1
	if(((op->nmid==BTESH2_NMID_MULL) ||
		(op->nmid==BTESH2_NMID_MULQ)) &&
		(op->fmid==BTESH2_FMID_REGREGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_IMUL, UAX_REG_RDX, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, op->ro, UAX_REG_RDX);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_MOVA) && (op->fmid==BTESH2_FMID_REGLDABS))
	{
		BTESH2_JitStoreVMRegImm(jctx, op->rn, (s64)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MULL) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RCX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_IMUL, UAX_REG_RDX, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_MACL, UAX_REG_RDX);
		return(1);
	}

#if 1
	if(((op->nmid==BTESH2_NMID_MULL) ||
		(op->nmid==BTESH2_NMID_MULQ)) &&
		(op->fmid==BTESH2_FMID_REGIMMREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		UAX_AsmMovRegImm(jctx, UAX_REG_RCX, (s32)(op->imm));
		UAX_AsmInsnRegReg(jctx, UAX_OP_IMUL, UAX_REG_RDX, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 0
	if((op->nmid==BTESH2_NMID_SWAPW) && (op->fmid==BTESH2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			BTESH2_JitInsnVMRegImm(jctx, UAX_OP_ROL, op->rn, 16);
			return(1);
		}
		
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_EAX, 16);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SWAPB) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_AX, 8);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_NEG) && (op->fmid==BTESH2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			BTESH2_JitInsnVMReg(jctx, UAX_OP_NEG, op->rn);
			return(1);
		}
		
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_NOT) && (op->fmid==BTESH2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			BTESH2_JitInsnVMReg(jctx, UAX_OP_NOT, op->rn);
			return(1);
		}
		
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		UAX_AsmInsnReg(jctx, UAX_OP_NOT, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}

#if 1

#if 0
	if((op->nmid==BTESH2_NMID_ADDC) && (op->fmid==BTESH2_FMID_REGREG))
	{
#if 1
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_R8D, UAX_REG_EAX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_R9D, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_ECX, UAX_REG_R8D);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EAX, UAX_REG_R9D);
		
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif
	}
#endif

#if 0
	if((op->nmid==BTESH2_NMID_SUBC) && (op->fmid==BTESH2_FMID_REGREG))
	{
#if 1
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		BTESH2_JitLoadVMRegZx(jctx, op->rn, UAX_REG_RCX);
		BTESH2_JitLoadVMRegZx(jctx, op->rm, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_RCX, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_RCX, UAX_REG_RAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RCX, 32);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RCX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif

#if 0
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_ECX, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_R8D, UAX_REG_EAX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_R9D, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_ECX, UAX_REG_R8D);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, UAX_REG_ECX, UAX_REG_EAX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETC, UAX_REG_AL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EAX, UAX_REG_R9D);
		
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif
	}
#endif

#endif

//	return(0);

#if 0
	if((	(op->nmid==BTESH2_NMID_SHAL) ||
			(op->nmid==BTESH2_NMID_SHLL)) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_EAX, 1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_ECX, 31);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 0
	if((op->nmid==BTESH2_NMID_SHAR) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_EAX, 1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 1);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 0
	if((op->nmid==BTESH2_NMID_SHLR) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_EAX, 1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 1);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_SHLL2) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitShlVMRegImm(jctx, op->rn, 2);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SHLL8) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitShlVMRegImm(jctx, op->rn, 8);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SHLL16) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitShlVMRegImm(jctx, op->rn, 16);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SHLR2) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitShrVMRegImm(jctx, op->rn, 2);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SHLR8) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitShrVMRegImm(jctx, op->rn, 8);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SHLR16) && (op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitShrVMRegImm(jctx, op->rn, 16);
		return(1);
	}
#endif


#if 1
	if((op->nmid==BTESH2_NMID_SHAD) &&
		(op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{
#if 0
			if(!(op->imm&31))
			{
				BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
				UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_ECX, 31);
				BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
				return(1);
			}
#endif
		
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_ECX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RCX, -op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_ECX);
			if(op->imm)
				UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}

	if((op->nmid==BTESH2_NMID_SHLD) &&
		(op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{
//			if(!(op->imm&31))
//			{
//				UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, UAX_REG_ECX, 0);
//				BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
//				return(1);
//			}
		
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RCX, -op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			if(op->imm)
				UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}
#endif


#if 1
	if((op->nmid==BTESH2_NMID_SHADQ) &&
		(op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{		
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RCX, -op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			if(op->imm)
				UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}

	if((op->nmid==BTESH2_NMID_SHLDQ) &&
		(op->fmid==BTESH2_FMID_REGIMMREG))
	{
		if(((sbyte)op->imm)<0)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RCX, -op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else if(((sbyte)op->imm)>0)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			if(op->imm)
				UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, UAX_REG_RCX, op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}else
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}
#endif

	return(0);
}



int BTESH2_TryJitOpcodeJQ_FpuOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
#if 1
	if((op->nmid==BTESH2_NMID_FMUL) && (op->fmid==BTESH2_FMID_FREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MULSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FADD) && (op->fmid==BTESH2_FMID_FREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADDSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FMOV) && (op->fmid==BTESH2_FMID_FREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
//		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM0);
//		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FMOV) && (op->fmid==BTESH2_FMID_DREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FSUB) && (op->fmid==BTESH2_FMID_FREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUBSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FDIV) && (op->fmid==BTESH2_FMID_FREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_DIVSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FNEG) && (op->fmid==BTESH2_FMID_FREGRN))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_XOR,
			UAX_REG_ECX, (s32)(0x80000000));
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_ECX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FABS) && (op->fmid==BTESH2_FMID_FREGRN))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND,
			UAX_REG_ECX, (s32)(0x7FFFFFFF));
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_ECX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FMUL) && (op->fmid==BTESH2_FMID_DREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MULSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FADD) && (op->fmid==BTESH2_FMID_DREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADDSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FSUB) && (op->fmid==BTESH2_FMID_DREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUBSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FDIV) && (op->fmid==BTESH2_FMID_DREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_DIVSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FMUL) && (op->fmid==BTESH2_FMID_FREGREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MULSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FADD) && (op->fmid==BTESH2_FMID_FREGREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADDSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FSUB) && (op->fmid==BTESH2_FMID_FREGREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUBSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FDIV) && (op->fmid==BTESH2_FMID_FREGREGREG))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_DIVSS, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMFReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}
#endif


#if 1
	if((op->nmid==BTESH2_NMID_FMUL) && (op->fmid==BTESH2_FMID_DREGREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MULSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FADD) && (op->fmid==BTESH2_FMID_DREGREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADDSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FSUB) && (op->fmid==BTESH2_FMID_DREGREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUBSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FDIV) && (op->fmid==BTESH2_FMID_DREGREGREG))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rn, UAX_REG_XMM0);
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_DIVSD, UAX_REG_XMM0, UAX_REG_XMM1);
		BTESH2_JitStoreVMDReg(jctx, op->ro, UAX_REG_XMM0);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FTRC) && (op->fmid==BTESH2_FMID_FREGRM))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTTSS2SI, UAX_REG_ECX, UAX_REG_XMM0);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_ECX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FLOAT) && (op->fmid==BTESH2_FMID_FREGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSI2SS, UAX_REG_XMM0, UAX_REG_ECX);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FTRC) && (op->fmid==BTESH2_FMID_DREGRM))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTTSD2SI, UAX_REG_EAX, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTTSD2SI, UAX_REG_RCX, UAX_REG_XMM0);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_ECX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FLOAT) && (op->fmid==BTESH2_FMID_DREGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSI2SD, UAX_REG_XMM0, UAX_REG_ECX);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FCNVSD) && (op->fmid==BTESH2_FMID_DREGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSS2SD, UAX_REG_XMM1, UAX_REG_XMM0);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FCNVDS) && (op->fmid==BTESH2_FMID_DREGRM))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSD2SS, UAX_REG_XMM1, UAX_REG_XMM0);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_XMM1);
		return(1);
	}
#endif

#if 1
	if(op->nmid==BTESH2_NMID_FMOVS)
	{
		if(op->fmid==BTESH2_FMID_FREGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_FREGST)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_FREGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_RCX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_FREGDECST)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_FREGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_FREGSTR0N)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

#if 1
		if(op->fmid==BTESH2_FMID_FREGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_FREGSTDISP)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BTESH2_FMID_FREGLDRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_FREGSTRODISP)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

		return(0);
	}
#endif

	return(0);
}


int BTESH2_TryJitOpcodeJQ_BranchOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	BTESH2_Trace *trj;
	int l0, l1, l2;
	int nm1, nm2, nm3;

//	return(0);

#if 1
	if((	(op->nmid==BTESH2_NMID_BF) ||
			(op->nmid==BTESH2_NMID_BFS)) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		if(jctx->jitfl&BTESH2_UAXJFL_PREJMP)
		{
//			__debugbreak();
			return(1);
		}
	
		if(tr->trjmpnext)
//		if(0)
		{
			l0=UAX_GenLabelTemp(jctx);
		
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JNE, l0|UAX_LBL_NEAR);

			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BTESH2_CpuState, trjmpnext));

			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s64)(op->imm));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JNE, l0|UAX_LBL_NEAR);
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s64)(op->imm));
			BTESH2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

//	return(0);

#if 1
	if((	(op->nmid==BTESH2_NMID_BT) ||
			(op->nmid==BTESH2_NMID_BTS)) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		if(jctx->jitfl&BTESH2_UAXJFL_PREJMP)
		{
//			__debugbreak();
			return(1);
		}

		if(tr->trjmpnext)
//		if(0)
		{
			l0=UAX_GenLabelTemp(jctx);
		
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);

			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BTESH2_CpuState, trjmpnext));

			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s64)(op->imm));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s64)(op->imm));
			BTESH2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

#if 1
	if(((op->nmid==BTESH2_NMID_BRA) ||
		(op->nmid==BTESH2_NMID_BRAN)) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		if(op->imm==tr->nxtpc)
			return(1);
	
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s64)(op->imm));
		BTESH2_JitFlushJNext(jctx, cpu, tr);

		return(1);
	}
#endif

#if 1
	if(((op->nmid==BTESH2_NMID_BSR) ||
		(op->nmid==BTESH2_NMID_BSRN)) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PR, (s64)(tr->nxtpc));
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s64)(op->imm));

//		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
//			UAX_REG_R8Q, UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trjmpnext));
//		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
//			UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RCX, UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trjmpnext));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_RCX);

		BTESH2_JitFlushJNext(jctx, cpu, tr);

		return(1);
	}
#endif

#if 1
	if(((op->nmid==BTESH2_NMID_RTS) ||
		(op->nmid==BTESH2_NMID_RTSN)) &&
		(op->fmid==BTESH2_FMID_NONE))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_PR, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PC, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_RAX);

		BTESH2_JitFlushJNext(jctx, cpu, tr);

		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_JSR) &&
		(op->fmid==BTESH2_FMID_REGRM))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_PC, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PC, UAX_REG_RDX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PR, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_RAX);

		BTESH2_JitFlushJNext(jctx, cpu, tr);

		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_JMP) &&
		(op->fmid==BTESH2_FMID_REGRM))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PC, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_RAX);

		BTESH2_JitFlushJNext(jctx, cpu, tr);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_BSRF) &&
		(op->fmid==BTESH2_FMID_REGRM))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RDX, (s32)(op->pc+4));
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_PC, UAX_REG_RCX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PC, UAX_REG_RDX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PR, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RDX, UAX_REG_RDX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_RDX);

		BTESH2_JitFlushJNext(jctx, cpu, tr);

		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_BRAF) &&
		(op->fmid==BTESH2_FMID_REGRM))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RCX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RCX, (s64)(op->pc+4));
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PC, UAX_REG_RCX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RCX, UAX_REG_RCX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_RCX);

		BTESH2_JitFlushJNext(jctx, cpu, tr);

		return(1);
	}
#endif


#if 0
	if((	(op->nmid==BTESH2_NMID_BREQ) ||
			(op->nmid==BTESH2_NMID_BRNE) ||
			(op->nmid==BTESH2_NMID_BRGT) ||
			(op->nmid==BTESH2_NMID_BRLT) ||
			(op->nmid==BTESH2_NMID_BRGE) ||
			(op->nmid==BTESH2_NMID_BRLE)) &&
		(op->fmid==BTESH2_FMID_REG1ABS))
	{
		if(jctx->jitfl&BTESH2_UAXJFL_PREJMP)
		{
			return(1);
		}
		
		if(op->nmid==BTESH2_NMID_BREQ)
			nm1=UAX_OP_JNE;
		if(op->nmid==BTESH2_NMID_BRNE)
			nm1=UAX_OP_JE;
		if(op->nmid==BTESH2_NMID_BRGE)
			nm1=UAX_OP_JL;
		if(op->nmid==BTESH2_NMID_BRLE)
			nm1=UAX_OP_JG;
		if(op->nmid==BTESH2_NMID_BRGT)
			nm1=UAX_OP_JLE;
		if(op->nmid==BTESH2_NMID_BRLT)
			nm1=UAX_OP_JGE;
	
		if(tr->trjmpnext)
		{
			l0=UAX_GenLabelTemp(jctx);
			BTESH2_JitCmpVMRegImm(jctx, op->rn, 0);
			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BTESH2_CpuState, trjmpnext));
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
			BTESH2_JitCmpVMRegImm(jctx, op->rn, 0);
			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			BTESH2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

#if 0
	if((	(op->nmid==BTESH2_NMID_BREQ) ||
			(op->nmid==BTESH2_NMID_BRNE) ||
			(op->nmid==BTESH2_NMID_BRGT) ||
			(op->nmid==BTESH2_NMID_BRLT) ||
			(op->nmid==BTESH2_NMID_BRGE) ||
			(op->nmid==BTESH2_NMID_BRLE)) &&
		(op->fmid==BTESH2_FMID_REG2ABS))
	{
		if(jctx->jitfl&BTESH2_UAXJFL_PREJMP)
		{
			return(1);
		}
		
		if(op->nmid==BTESH2_NMID_BREQ)
			nm1=UAX_OP_JNE;
		if(op->nmid==BTESH2_NMID_BRNE)
			nm1=UAX_OP_JE;
		if(op->nmid==BTESH2_NMID_BRGE)
			nm1=UAX_OP_JL;
		if(op->nmid==BTESH2_NMID_BRLE)
			nm1=UAX_OP_JG;
		if(op->nmid==BTESH2_NMID_BRGT)
			nm1=UAX_OP_JLE;
		if(op->nmid==BTESH2_NMID_BRLT)
			nm1=UAX_OP_JGE;
	
		if(tr->trjmpnext)
		{
			l0=UAX_GenLabelTemp(jctx);

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
			BTESH2_JitCmpVMReg(jctx, op->rn, UAX_REG_EAX);

			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RCCTX,
				offsetof(BTESH2_CpuState, trjmpnext));
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RCCTX, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
			BTESH2_JitCmpVMReg(jctx, op->rn, UAX_REG_EAX);

//			BTESH2_JitCmpVMRegImm(jctx, op->rn, 0);
			UAX_AsmInsnLabel(jctx, nm1, l0|UAX_LBL_NEAR);
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			BTESH2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

	return(0);
}



int BTESH2_TryJitOpcodeJQ_MovMem(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	u32 v;
	int i;

	if(op->nmid==BTESH2_NMID_MOVL)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_RCX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGDECST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_IMMSTRN)
		{
			UAX_AsmMovRegImm(jctx, UAX_REG_R8D, (s32)op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_R8D);
//			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_IMMDECSTRN)
		{
			UAX_AsmMovRegImm(jctx, UAX_REG_R8D, (s32)op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_R8D);
//			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDABS)
		{
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (s32)(op->imm));
			BTESH2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

#if 1
		if(op->fmid==BTESH2_FMID_REGLDRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 4, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}
#endif

		return(0);
	}

	if(op->nmid==BTESH2_NMID_MOVQ)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}

#if 1
		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif

#if 1
		if(op->fmid==BTESH2_FMID_REGLDRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 8, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitGetAddrQWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_RCX, UAX_REG_RAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 8, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitSetAddrQWord(jctx, cpu);
			return(1);
		}
#endif
	}

#if 1
	if((op->nmid==BTESH2_NMID_PUSH) &&
		(op->fmid==BTESH2_FMID_REGRM))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SP, UAX_REG_RDX);
		UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 8);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SP, UAX_REG_RDX);
		BTESH2_JitSetAddrQWord(jctx, cpu);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_POP) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SP, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 8);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SP, UAX_REG_RCX);
		BTESH2_JitGetAddrQWord(jctx, cpu);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RAX);
		return(1);
	}
#endif

//	return(0);

	if(op->nmid==BTESH2_NMID_MOVW)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 2);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_RCX);

			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGDECST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 2);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDABS)
		{
			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (s64)(op->imm));
			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

#if 1
		if(op->fmid==BTESH2_FMID_REGLDRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 2, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 2, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}
#endif

		return(0);
	}

	if(op->nmid==BTESH2_NMID_MOVB)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 1);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_RCX);

			BTESH2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGDECST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 1);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmAddRegReg(jctx, UAX_REG_RDX, UAX_REG_RCX);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

#if 1
		if(op->fmid==BTESH2_FMID_REGLDRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 1, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 1, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}
#endif

		return(0);
	}

	if(op->nmid==BTESH2_NMID_MOVUB)
	{
		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_RCX, UAX_REG_AL);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 1, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitGetAddrByte(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_RCX, UAX_REG_AL);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}

	if(op->nmid==BTESH2_NMID_MOVUW)
	{
		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_RDX, op->imm);
			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_EAX, UAX_REG_AX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDRODISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, 2, UAX_REG_RCX, (s32)op->imm);
			BTESH2_JitGetAddrWord(jctx, cpu);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZX, UAX_REG_EAX, UAX_REG_AX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
			return(1);
		}
	}

#if 0
	if(((op->nmid==BTESH2_NMID_LDSL) ||
		(op->nmid==BTESH2_NMID_LDCL)) &&
		(op->fmid==BTESH2_FMID_REGLD) &&
		(op->rn!=BTESH2_REG_FPSCR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
//		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_RDX);
		BTESH2_JitGetAddrQWord(jctx, cpu);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}

	if(((op->nmid==BTESH2_NMID_STSL) ||
		(op->nmid==BTESH2_NMID_STCL)) &&
		(op->fmid==BTESH2_FMID_REGST))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
		BTESH2_JitSetAddrQWord(jctx, cpu);
		return(1);
	}
#endif

#if 1
	if(((op->nmid==BTESH2_NMID_LDSL) ||
		(op->nmid==BTESH2_NMID_LDCL)) &&
		(op->fmid==BTESH2_FMID_REGINCLD) &&
		(op->rn!=BTESH2_REG_FPSCR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 8);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_RCX);

		BTESH2_JitGetAddrQWord(jctx, cpu);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}

	if(((op->nmid==BTESH2_NMID_STSL) ||
		(op->nmid==BTESH2_NMID_STCL)) &&
		(op->fmid==BTESH2_FMID_REGDECST))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8Q);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_RDX);
		UAX_AsmSubRegImm(jctx, UAX_REG_RDX, 8);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		BTESH2_JitSetAddrQWord(jctx, cpu);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_LEAB) ||
		(op->nmid==BTESH2_NMID_LEAW) ||
		(op->nmid==BTESH2_NMID_LEAL) ||
		(op->nmid==BTESH2_NMID_LEAQ))
	{
		if(op->fmid==BTESH2_FMID_REGLDRODISP)
		{
			i=1;
			if(op->nmid==BTESH2_NMID_LEAB)i=1;
			if(op->nmid==BTESH2_NMID_LEAW)i=2;
			if(op->nmid==BTESH2_NMID_LEAL)i=4;
			if(op->nmid==BTESH2_NMID_LEAQ)i=8;
			if(op->ro==BTESH2_REG_R0)i=1;
		
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_RCX);
			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, i, UAX_REG_RCX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}

#if 1
		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			i=1;
			if(op->nmid==BTESH2_NMID_LEAB)i=1;
			if(op->nmid==BTESH2_NMID_LEAW)i=2;
			if(op->nmid==BTESH2_NMID_LEAL)i=4;
			if(op->nmid==BTESH2_NMID_LEAQ)i=8;
			if(op->ro==BTESH2_REG_R0)i=1;
		
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
//			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
//			UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_LEA,
//				UAX_REG_EDX, UAX_REG_EDX, i, UAX_REG_ECX, (s32)(op->imm));
			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_LEA,
				UAX_REG_RDX, UAX_REG_RDX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
			return(1);
		}
#endif
	}

#ifdef UAX_WINX64
// #if 0
	if( (op->nmid==BTESH2_NMID_STS) &&
		(op->fmid==BTESH2_FMID_REGREG) &&
		(op->rn==BTESH2_REG_FPSCR))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
		return(1);
	}

	if( (op->nmid==BTESH2_NMID_LDS) &&
		(op->fmid==BTESH2_FMID_REGREG) &&
		(op->rn==BTESH2_REG_FPSCR))
	{
//		BTESH2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
//		return(1);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
//		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
		BTESH2_JitEmitCallFPtr(jctx, cpu, (void *)BTSH_Op_SetFPSCR);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_LDSL) &&
		(op->fmid==BTESH2_FMID_REGINCLD) &&
		(op->rn==BTESH2_REG_FPSCR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_RDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_RCX, 8);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_RCX);

		BTESH2_JitGetAddrQWord(jctx, cpu);
		UAX_AsmMovRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);

//		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RCCTX);
		BTESH2_JitEmitCallFPtr(jctx, cpu, (void *)BTSH_Op_SetFPSCR);

		return(1);
	}

	if((op->nmid==BTESH2_NMID_PSETMD4) &&
		(op->fmid==BTESH2_FMID_IMM))
	{
		v=0;
		i=op->imm;
		
		if(i&2)
		{
			if(i&0x8)	{ v|=BTESH2_FPSCR_SZ; }
			if(i&0x4)	{ v|=BTESH2_FPSCR_PR; }
			if(i&0x1)	{ v|=BTESH2_FPSCR_FR; }

			UAX_AsmMovRegImm(jctx, UAX_REG_RDX, v);
			BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPSCR, UAX_REG_RDX);

			UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
			BTESH2_JitEmitCallFPtr(jctx, cpu, (void *)BTSH_Op_SetFPSCR);

			return(1);
		}
		
		if(i&0x8)	{ v|=BTESH2_FPSCR_SZ; }
		else		{ v&=~BTESH2_FPSCR_SZ; }
		if(i&0x4)	{ v|=BTESH2_FPSCR_PR; }
		else		{ v&=~BTESH2_FPSCR_PR; }
		v=v|(i&3);
	
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPSCR, UAX_REG_RDX);

		UAX_AsmAndRegImm(jctx, UAX_REG_RDX,
			~(BTESH2_FPSCR_SZ|BTESH2_FPSCR_PR|3));
		UAX_AsmOrRegImm(jctx, UAX_REG_RDX, v);

//		UAX_AsmMovRegImm(jctx, UAX_REG_EDX, v);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPSCR, UAX_REG_RDX);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, (void *)BTSH_Op_SetFPSCR);

		return(1);
	}
#endif
	
	return(0);
}



int BTESH2_TryJitOpcodeJQ_SignExtOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	int l0, l1, l2;

#if 1
	if((op->nmid==BTESH2_NMID_EXTUB) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZXB, UAX_REG_EAX, UAX_REG_DL);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);

		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, 255);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_EXTUW) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZXW, UAX_REG_EAX, UAX_REG_DX);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);

		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, 65535);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_EXTSB) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXB, UAX_REG_EAX, UAX_REG_DL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_EXTSW) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_EAX, UAX_REG_DX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_EXTUL) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_ECX, UAX_REG_EDX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_EXTSL) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RCX, UAX_REG_EDX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RCX);
		return(1);
	}
#endif

//	return(0);

	if((op->nmid==BTESH2_NMID_NOP) &&
		(op->fmid==BTESH2_FMID_NONE))
	{
		return(1);
	}

#if 0
	if((op->nmid==BTESH2_NMID_SHAD) &&
		(op->fmid==BTESH2_FMID_REGIMM))
	{
		if(((s32)op->imm)>=0)
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL,
				UAX_REG_EDX, (op->imm)&31);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else if(op->imm&31)
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR,
				UAX_REG_EDX, 32-((op->imm)&31));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_EDX, 31);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}
	}

	if((op->nmid==BTESH2_NMID_SHLD) &&
		(op->fmid==BTESH2_FMID_REGIMM))
	{
		if(((s32)op->imm)>=0)
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL,
				UAX_REG_EDX, (op->imm)&31);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else if(op->imm&31)
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHR,
				UAX_REG_EDX, 32-((op->imm)&31));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			return(1);
		}else
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rm, (s32)op->imm);
			BTESH2_JitStoreVMRegImm(jctx, op->rn, 0);
			return(1);
		}
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_SHAD) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RDX, UAX_REG_EDX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 64);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

//		UAX_EmitLabel(jctx, l1);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RDX, 63);

		UAX_EmitLabel(jctx, l2);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_SHLD) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 64);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHR, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

//		UAX_EmitLabel(jctx, l1);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_EDX, UAX_REG_EDX);

		UAX_EmitLabel(jctx, l2);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_SHAD) &&
		(op->fmid==BTESH2_FMID_REGREGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
//		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RDX, UAX_REG_EDX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 64);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

//		UAX_EmitLabel(jctx, l1);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RDX, 63);

		UAX_EmitLabel(jctx, l2);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		BTESH2_JitStoreVMReg(jctx, op->ro, UAX_REG_RDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_SHAR) &&
		(op->fmid==BTESH2_FMID_REGREGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
//		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RDX, UAX_REG_EDX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 64);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

//		UAX_EmitLabel(jctx, l1);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RDX, 63);

		UAX_EmitLabel(jctx, l2);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		BTESH2_JitStoreVMReg(jctx, op->ro, UAX_REG_RDX);
		return(1);
	}
#endif



#if 1
	if((op->nmid==BTESH2_NMID_DIV1) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
		l2=UAX_GenLabelTemp(jctx);
	
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_R8Q);

#if 0
//		rn<<=1;
//		rn|=sr&1;
		SHL RDX, 1
		MOV EAX, R8
		AND EAX, 1
		OR RDX, EAX
		
		TEST R8, 0x200
		JZ L1
			if(sr&0x100)
				rm=-rm;
			MOV RAX, RCX
			NEG RAX
			TEST R8, 0x100
			CMOVNZ RCX, RAX

			MOV R9Q, R8Q
			AND R8Q, ~0x101
			OR R9Q, 0x101

			rn=rn+rm;
			ADD EDX, ECX
			CMOVNC R8Q, R9Q
			
//			q=1-((rn>>32)&1);
//			t=q;
			
		JMP L2
		L1:
			if(!(sr&0x100))
				rm=-rm;
			MOV RAX, RCX
			NEG RAX
			TEST R8, 0x100
			CMOVZ RCX, RAX

			AND R8Q, ~0x101
			OR R8Q, 1
			MOV R9Q, R8Q
			XOR R9Q, 0x101

//			rn=rn+rm;
			ADD EDX, ECX
			CMOVC R8Q, R9Q

			q=(rn>>32)&1;
			t=1-q;
		L2:

		if(sr&0x200)
		{
			if(sr&0x100)
				rm=-rm;
			rn=rn+rm;
			q=1-((rn>>32)&1);
			t=q;
		}else
		{
			if(!(sr&0x100))
				rm=-rm;
			rn=rn+rm;
			q=(rn>>32)&1;
			t=1-q;
		}


		SHL RDX, 1
		MOV EAX, R8
		AND EAX, 1
		OR RDX, EAX
		TEST R8, 0x200
		JZ L1
			MOV RAX, RCX
			NEG RAX
			TEST R8, 0x100
			CMOVNZ RCX, RAX
			MOV R9Q, R8Q
			AND R8Q, ~0x101
			OR R9Q, 0x101
			ADD EDX, ECX
			CMOVNC R8Q, R9Q
			JMP L2
		L1:
			MOV RAX, RCX
			NEG RAX
			TEST R8, 0x100
			CMOVZ RCX, RAX
			AND R8Q, ~0x101
			OR R8Q, 1
			MOV R9Q, R8Q
			XOR R9Q, 0x101
			ADD EDX, ECX
			CMOVC R8Q, R9Q
		L2:

#endif

#if 0
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHL,		UAX_REG_RDX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_EAX, UAX_REG_R8D);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_EAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR,		UAX_REG_RDX, UAX_REG_RAX);		
		UAX_AsmInsnRegImm(jctx, UAX_OP_TEST,	UAX_REG_R8Q, 0x200);
		UAX_AsmInsnLabel(jctx,	UAX_OP_JE,		l1|UAX_LBL_NEAR);

		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_RAX, UAX_REG_RCX);
		UAX_AsmInsnReg   (jctx,	UAX_OP_NEG,		UAX_REG_RAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_TEST,	UAX_REG_R8Q, 0x100);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE,	UAX_REG_RCX, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_R8Q);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_R8Q, ~0x101);
		UAX_AsmInsnRegImm(jctx, UAX_OP_OR,		UAX_REG_R9Q, 0x101);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD,		UAX_REG_RDX, UAX_REG_RCX);

		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV,		UAX_REG_RAX, 0x100000000ULL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,	UAX_REG_RDX, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVE,	UAX_REG_R8Q, UAX_REG_R9Q);
		UAX_AsmInsnLabel(jctx,	UAX_OP_JMP,		l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_RAX, UAX_REG_RCX);
		UAX_AsmInsnReg   (jctx,	UAX_OP_NEG,		UAX_REG_RAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_TEST,	UAX_REG_R8Q, 0x100);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVE,	UAX_REG_RCX, UAX_REG_RAX);

		UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_R8Q, ~0x101);
		UAX_AsmInsnRegImm(jctx, UAX_OP_OR,		UAX_REG_R8Q, 0x001);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_R8Q);
		UAX_AsmInsnRegImm(jctx, UAX_OP_XOR,		UAX_REG_R9Q, 0x101);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD,		UAX_REG_RDX, UAX_REG_RCX);

		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV,		UAX_REG_RAX, 0x100000000ULL);
		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,	UAX_REG_RDX, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE,	UAX_REG_R8Q, UAX_REG_R9Q);

		UAX_EmitLabel(jctx, l2);
#endif

#if 1
		if(cpu->ftopfl&BTESH2_FTOPFL_SEEN_DIV0S)
		{
//			__debugbreak();
		
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL,		UAX_REG_RDX, 1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_EAX, UAX_REG_R8D);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_EAX, 1);
			UAX_AsmInsnReg   (jctx,	UAX_OP_NEG,		UAX_REG_R9Q);
			UAX_AsmInsnRegReg(jctx, UAX_OP_OR,		UAX_REG_RDX, UAX_REG_RAX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_TEST,	UAX_REG_R8Q, 0x100);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE,	UAX_REG_RCX, UAX_REG_R9Q);
			UAX_AsmInsnRegImm(jctx, UAX_OP_TEST,	UAX_REG_R8Q, 0x200);
			UAX_AsmInsnLabel(jctx,	UAX_OP_JE,		l1|UAX_LBL_NEAR);

			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_R8Q);
			UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_R8Q, ~0x101);
			UAX_AsmInsnRegImm(jctx, UAX_OP_MOV,	
				UAX_REG_RAX, 0x100000000ULL);
			UAX_AsmInsnRegReg(jctx, UAX_OP_ADD,		UAX_REG_RDX, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_OR,		UAX_REG_R9Q, 0x101);
			UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,	UAX_REG_RDX, UAX_REG_RAX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVE,	UAX_REG_R8Q, UAX_REG_R9Q);
			UAX_AsmInsnLabel(jctx,	UAX_OP_JMP,		l2|UAX_LBL_NEAR);

			UAX_EmitLabel(jctx, l1);
			UAX_AsmInsnReg   (jctx,	UAX_OP_NEG,		UAX_REG_RCX);

			UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_R8Q, ~0x101);
			UAX_AsmInsnRegImm(jctx, UAX_OP_OR,		UAX_REG_R8Q, 0x001);
			UAX_AsmInsnRegImm(jctx, UAX_OP_MOV,	
				UAX_REG_RAX, 0x100000000ULL);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_R8Q);
			UAX_AsmInsnRegReg(jctx, UAX_OP_ADD,		UAX_REG_RDX, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_XOR,		UAX_REG_R9Q, 0x101);

			UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,	UAX_REG_RDX, UAX_REG_RAX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE,	UAX_REG_R8Q, UAX_REG_R9Q);

			UAX_EmitLabel(jctx, l2);
		}else
		{
			UAX_AsmInsnRegImm(jctx, UAX_OP_SHL,		UAX_REG_RDX, 1);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_EAX, UAX_REG_R8D);
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_EAX, 1);
			UAX_AsmInsnReg   (jctx,	UAX_OP_NEG,		UAX_REG_R9Q);
			UAX_AsmInsnRegReg(jctx, UAX_OP_OR,		UAX_REG_RDX, UAX_REG_RAX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_TEST,	UAX_REG_R8Q, 0x100);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVE,	UAX_REG_RCX, UAX_REG_R9Q);

			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_R8Q);
			UAX_AsmInsnRegImm(jctx, UAX_OP_MOV,
				UAX_REG_RAX, 0x100000000ULL);
			UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_R8Q, ~0x101);
			UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_R9Q, ~0x101);
			UAX_AsmInsnRegReg(jctx, UAX_OP_ADD,		UAX_REG_RDX, UAX_REG_RCX);
			UAX_AsmInsnRegImm(jctx, UAX_OP_OR,		UAX_REG_R8Q, 0x001);
			UAX_AsmInsnRegImm(jctx, UAX_OP_OR,		UAX_REG_R9Q, 0x100);

//			UAX_AsmInsnRegImm(jctx, UAX_OP_AND,		UAX_REG_R8Q, ~0x101);
//			UAX_AsmInsnRegImm(jctx, UAX_OP_OR,		UAX_REG_R8Q, 0x001);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV,		UAX_REG_R9Q, UAX_REG_R8Q);
//			UAX_AsmInsnRegReg(jctx, UAX_OP_ADD,		UAX_REG_RDX, UAX_REG_RCX);
//			UAX_AsmInsnRegImm(jctx, UAX_OP_XOR,		UAX_REG_R9Q, 0x101);


			UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,	UAX_REG_RDX, UAX_REG_RAX);
			UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE,	UAX_REG_R8Q, UAX_REG_R9Q);
		}
#endif

		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_R8Q);

#if 0
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSX, UAX_REG_RDX, UAX_REG_EDX);

		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST,
			UAX_REG_ECX, UAX_REG_ECX);
		UAX_AsmInsnLabel(jctx, UAX_OP_JS, l0|UAX_LBL_NEAR);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 63);
//		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 64);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_RDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

//		UAX_EmitLabel(jctx, l1);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_RDX, 63);

		UAX_EmitLabel(jctx, l2);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_RDX);
		BTESH2_JitStoreVMReg(jctx, op->ro, UAX_REG_RDX);
#endif

		return(1);
	}
#endif

	return(0);
}
