int BTESH2_TryJitOpcode_MovReg(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	int reg1, reg2;

	if(op->nmid==BTESH2_NMID_MOV)
	{
		if(op->fmid==BTESH2_FMID_REGREG)
		{
			BTESH2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
			return(1);
		
//			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
//			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGIMM)
		{
			BTESH2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));
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

//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
//		return(1);
	}

	if(((op->nmid==BTESH2_NMID_STC) ||
		(op->nmid==BTESH2_NMID_LDC)) &&
		(op->fmid==BTESH2_FMID_REGREG) &&
		(op->rm!=BTESH2_REG_SR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		BTESH2_JitMovVMRegVMReg(jctx, op->rn, op->rm);
		return(1);

//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
//		return(1);
	}

	if((	(op->nmid==BTESH2_NMID_MOVL) ||
			(op->nmid==BTESH2_NMID_MOVW)) &&
		(op->fmid==BTESH2_FMID_REGVIMM))
	{
		BTESH2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((	(op->nmid==BTESH2_NMID_MOVI20) ||
			(op->nmid==BTESH2_NMID_MOVI20S)) &&
		(op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}
	
	return(0);
}

int BTESH2_TryJitOpcode_ArithReg(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_ADD, op->rn, op->rm);
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//		BTESH2_JitAddVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitAddVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_SUB) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_SUB, op->rn, op->rm);
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//		BTESH2_JitSubVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitInsnVMRegVMReg(jctx,
			UAX_OP_AND, op->rn, op->rm);
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//		BTESH2_JitAndVMReg(jctx, op->rn, UAX_REG_EAX);
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
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//		BTESH2_JitOrVMReg(jctx, op->rn, UAX_REG_EAX);
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
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
//		BTESH2_JitXorVMReg(jctx, op->rn, UAX_REG_EAX);
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


	if((op->nmid==BTESH2_NMID_MOVA) && (op->fmid==BTESH2_FMID_REGLDABS))
	{
		BTESH2_JitStoreVMRegImm(jctx, op->rn, (s32)(op->imm));
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MULL) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_IMUL, UAX_REG_EAX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_MACL, UAX_REG_EAX);
		return(1);
	}

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

	if((op->nmid==BTESH2_NMID_NEG) && (op->fmid==BTESH2_FMID_REGREG))
	{
		if(op->rm==op->rn)
		{
			BTESH2_JitInsnVMReg(jctx, UAX_OP_NEG, op->rn);
			return(1);
		}
		
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

#if 1

#if 1
	if((op->nmid==BTESH2_NMID_ADDC) && (op->fmid==BTESH2_FMID_REGREG))
	{
#if 0
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		BTESH2_JitLoadVMRegZx(jctx, op->rn, UAX_REG_RCX);
		BTESH2_JitLoadVMRegZx(jctx, op->rm, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_RCX, UAX_REG_RAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_EDX, (s32)(~1));
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RAX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, UAX_REG_RCX, UAX_REG_RAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, UAX_REG_RCX, 32);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_RCX, 1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif

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

#if 1
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

#if 1
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

#if 1
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

	return(0);
}


int BTESH2_TryJitOpcode_MovMem(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	if(op->nmid==BTESH2_NMID_MOVL)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);

	//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
	//		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
	//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGDECST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_IMMSTRN)
		{
			UAX_AsmMovRegImm(jctx, UAX_REG_R8D, (s32)op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_R8D);
//			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_IMMDECSTRN)
		{
			UAX_AsmMovRegImm(jctx, UAX_REG_R8D, (s32)op->imm);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_R8D);
//			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		return(0);
	}

	if(op->nmid==BTESH2_NMID_MOVW)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitGetAddrWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitGetAddrWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 2);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

			BTESH2_JitGetAddrWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGDECST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 2);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitGetAddrWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitSetAddrWord(jctx, cpu);
			return(1);
		}

		return(0);
	}

	if(op->nmid==BTESH2_NMID_MOVB)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitGetAddrByte(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitGetAddrByte(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 1);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

			BTESH2_JitGetAddrByte(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGDECST)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 1);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitGetAddrByte(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitSetAddrByte(jctx, cpu);
			return(1);
		}

		return(0);
	}

	if(((op->nmid==BTESH2_NMID_LDSL) ||
		(op->nmid==BTESH2_NMID_LDCL)) &&
		(op->fmid==BTESH2_FMID_REGLD) &&
		(op->rn!=BTESH2_REG_FPSCR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
		BTESH2_JitGetAddrDWord(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if(((op->nmid==BTESH2_NMID_STSL) ||
		(op->nmid==BTESH2_NMID_STCL)) &&
		(op->fmid==BTESH2_FMID_REGST))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitSetAddrDWord(jctx, cpu);
		return(1);
	}

	if(((op->nmid==BTESH2_NMID_LDSL) ||
		(op->nmid==BTESH2_NMID_LDCL)) &&
		(op->fmid==BTESH2_FMID_REGINCLD) &&
		(op->rn!=BTESH2_REG_FPSCR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 4);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

		BTESH2_JitGetAddrDWord(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if(((op->nmid==BTESH2_NMID_STSL) ||
		(op->nmid==BTESH2_NMID_STCL)) &&
		(op->fmid==BTESH2_FMID_REGDECST))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 4);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitSetAddrDWord(jctx, cpu);
		return(1);
	}

#ifdef UAX_WIN64
	if((op->nmid==BTESH2_NMID_LDSL) &&
		(op->fmid==BTESH2_FMID_REGINCLD) &&
		(op->rn==BTESH2_REG_FPSCR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 4);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

		BTESH2_JitGetAddrDWord(jctx, cpu);
		UAX_AsmMovRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, (void *)BTSH_Op_SetFPSCR);

		return(1);
	}
#endif
	
	return(0);
}

int BTESH2_TryJitOpcode_FpuOp(UAX_Context *jctx,
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
	if((op->nmid==BTESH2_NMID_FTRC) && (op->fmid==BTESH2_FMID_FREGRM))
	{
		BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTTSS2SI, UAX_REG_EAX, UAX_REG_XMM0);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FLOAT) && (op->fmid==BTESH2_FMID_FREGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSI2SS, UAX_REG_XMM0, UAX_REG_EAX);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FTRC) && (op->fmid==BTESH2_FMID_DREGRM))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
//		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTTSD2SI, UAX_REG_EAX, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTTSD2SI, UAX_REG_RAX, UAX_REG_XMM0);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FLOAT) && (op->fmid==BTESH2_FMID_DREGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_EAX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSI2SD, UAX_REG_XMM0, UAX_REG_EAX);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM0);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FCNVSD) && (op->fmid==BTESH2_FMID_FREGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSS2SD, UAX_REG_XMM1, UAX_REG_XMM0);
		BTESH2_JitStoreVMDReg(jctx, op->rn, UAX_REG_XMM1);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_FCNVDS) && (op->fmid==BTESH2_FMID_FREGRM))
	{
		BTESH2_JitLoadVMDReg(jctx, op->rm, UAX_REG_XMM0);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CVTSD2SS, UAX_REG_XMM1, UAX_REG_XMM0);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_FPUL, UAX_REG_XMM1);
		return(1);
	}
#endif

	if(op->nmid==BTESH2_NMID_FMOVS)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGDECST)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			BTESH2_JitLoadVMFReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		return(0);
	}

	return(0);
}

int BTESH2_TryJitOpcode_SignExtOp(UAX_Context *jctx,
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
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
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
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
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
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_EXTSW) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXW, UAX_REG_EAX, UAX_REG_DX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_NOP) &&
		(op->fmid==BTESH2_FMID_NONE))
	{
		return(1);
	}

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

	if((op->nmid==BTESH2_NMID_SHAD) &&
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
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SAR, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l1);
		UAX_AsmInsnRegImm(jctx, UAX_OP_SAR, UAX_REG_EDX, 31);

		UAX_EmitLabel(jctx, l2);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
		return(1);
	}

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
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHL, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, UAX_REG_ECX, 31);
		UAX_AsmInsnLabel(jctx, UAX_OP_JE, l1|UAX_LBL_NEAR);
		UAX_AsmInsnReg(jctx, UAX_OP_NEG, UAX_REG_ECX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_ECX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_SHR, UAX_REG_EDX, UAX_REG_CL);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l2|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_EDX, UAX_REG_EDX);

		UAX_EmitLabel(jctx, l2);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
		return(1);
	}

	return(0);
}
