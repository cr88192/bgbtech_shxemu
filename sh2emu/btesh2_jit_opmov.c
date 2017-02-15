int BTESH2_TryJitOpcode_MovReg(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	if(op->nmid==BTESH2_NMID_MOV)
	{
		if(op->fmid==BTESH2_FMID_REGREG)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGIMM)
		{
			UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}
		
		return(0);
	}

	if(((op->nmid==BTESH2_NMID_STS) ||
		(op->nmid==BTESH2_NMID_LDS)) &&
		(op->fmid==BTESH2_FMID_REGREG) &&
		(op->rn!=BTESH2_REG_FPSCR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if(((op->nmid==BTESH2_NMID_STC) ||
		(op->nmid==BTESH2_NMID_LDC)) &&
		(op->fmid==BTESH2_FMID_REGREG) &&
		(op->rm!=BTESH2_REG_SR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVL) && (op->fmid==BTESH2_FMID_REGVIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
	
	return(0);
}

int BTESH2_TryJitOpcode_ArithReg(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitAddVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitAddVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

#if 1
	if((op->nmid==BTESH2_NMID_SUB) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitSubVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitAndVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_OR) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitOrVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_XOR) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitXorVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitAndVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_OR) && (op->fmid==BTESH2_FMID_REGIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitOrVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_XOR) && (op->fmid==BTESH2_FMID_REGIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitXorVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVA) && (op->fmid==BTESH2_FMID_REGLDABS))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
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
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_EAX, 16);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	return(0);
}


int BTESH2_TryJitOpcode_MovMem(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	if(op->nmid==BTESH2_NMID_MOVL)
	{
		if(op->fmid==BTESH2_FMID_REGLD)
		{
	//		if(op->Run!=BTSH_Op_MOV_RegMemD_FMMU)
	//			return(0);
			if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
				return(0);

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);

	//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
	//		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
	//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDDISP)
		{
			if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
				return(0);

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTDISP)
		{
			if(cpu->SetAddrDWord!=BTESH2_SetAddrDWordFMMU)
				return(0);

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
			BTESH2_JitSetAddrDWord(jctx, cpu);
	//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGST)
		{
			if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
				return(0);

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGINCLD)
		{
			if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
				return(0);

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
			if(cpu->SetAddrDWord!=BTESH2_SetAddrDWordFMMU)
				return(0);

			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 4);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGLDR0M)
		{
			if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
				return(0);
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitGetAddrDWord(jctx, cpu);
			BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
			return(1);
		}

		if(op->fmid==BTESH2_FMID_REGSTR0N)
		{
			if(cpu->SetAddrDWord!=BTESH2_SetAddrDWordFMMU)
				return(0);
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
			BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
			BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
			UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
			BTESH2_JitSetAddrDWord(jctx, cpu);
			return(1);
		}

		return(0);
	}

#if 1
	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGLDDISP))
	{
		if(cpu->GetAddrWord!=BTESH2_GetAddrWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
		BTESH2_JitGetAddrWord(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGLDDISP))
	{
		if(cpu->GetAddrByte!=BTESH2_GetAddrByteFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
		BTESH2_JitGetAddrByte(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGSTDISP))
	{
		if(cpu->SetAddrWord!=BTESH2_SetAddrWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
		BTESH2_JitSetAddrWord(jctx, cpu);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGSTDISP))
	{
		if(cpu->SetAddrByte!=BTESH2_SetAddrByteFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EDX, op->imm);
		BTESH2_JitSetAddrByte(jctx, cpu);
//		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGLD))
	{
		if(cpu->GetAddrWord!=BTESH2_GetAddrWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BTESH2_JitGetAddrWord(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

#if 1
	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGLD))
	{
		if(cpu->GetAddrByte!=BTESH2_GetAddrByteFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BTESH2_JitGetAddrByte(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGST))
	{
		if(cpu->GetAddrWord!=BTESH2_GetAddrWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitSetAddrWord(jctx, cpu);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGST))
	{
		if(cpu->GetAddrByte!=BTESH2_GetAddrByteFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitSetAddrByte(jctx, cpu);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGINCLD))
	{
		if(cpu->GetAddrWord!=BTESH2_GetAddrWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 2);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

		BTESH2_JitGetAddrWord(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGINCLD))
	{
		if(cpu->GetAddrWord!=BTESH2_GetAddrWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 1);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

		BTESH2_JitGetAddrByte(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGDECST))
	{
		if(cpu->SetAddrWord!=BTESH2_SetAddrWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 2);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitSetAddrWord(jctx, cpu);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGDECST))
	{
		if(cpu->SetAddrByte!=BTESH2_SetAddrByteFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		UAX_AsmSubRegImm(jctx, UAX_REG_EDX, 1);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitSetAddrByte(jctx, cpu);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGLDR0M))
	{
		if(cpu->GetAddrWord!=BTESH2_GetAddrWordFMMU)
			return(0);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
		UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitGetAddrWord(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGLDR0M))
	{
		if(cpu->GetAddrByte!=BTESH2_GetAddrByteFMMU)
			return(0);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
		UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitGetAddrByte(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_MOVW) && (op->fmid==BTESH2_FMID_REGSTR0N))
	{
		if(cpu->SetAddrWord!=BTESH2_SetAddrWordFMMU)
			return(0);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
		UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitSetAddrWord(jctx, cpu);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOVB) && (op->fmid==BTESH2_FMID_REGSTR0N))
	{
		if(cpu->SetAddrByte!=BTESH2_SetAddrByteFMMU)
			return(0);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
		UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitSetAddrByte(jctx, cpu);
		return(1);
	}
#endif

	if(((op->nmid==BTESH2_NMID_LDSL) ||
		(op->nmid==BTESH2_NMID_LDCL)) &&
		(op->fmid==BTESH2_FMID_REGINCLD) &&
		(op->rn!=BTESH2_REG_FPSCR) &&
		(op->rn!=BTESH2_REG_SR))
	{
		if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 4);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

		BTESH2_JitGetAddrDWord(jctx, cpu);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
	
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
	if((op->nmid==BTESH2_NMID_FMOVS) && (op->fmid==BTESH2_FMID_REGLD))
	{
		if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BTESH2_JitGetAddrDWord(jctx, cpu);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FMOVS) && (op->fmid==BTESH2_FMID_REGINCLD))
	{
		if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
			return(0);

		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmMovRegReg(jctx, UAX_REG_EAX, UAX_REG_EDX);
		UAX_AsmAddRegImm(jctx, UAX_REG_EAX, 4);
		BTESH2_JitStoreVMReg(jctx, op->rm, UAX_REG_EAX);

		BTESH2_JitGetAddrDWord(jctx, cpu);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_FMOVS) && (op->fmid==BTESH2_FMID_REGLDR0M))
	{
		if(cpu->GetAddrDWord!=BTESH2_GetAddrDWordFMMU)
			return(0);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		BTESH2_JitLoadVMReg(jctx, op->ro, UAX_REG_ECX);
		UAX_AsmAddRegReg(jctx, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitGetAddrDWord(jctx, cpu);
		BTESH2_JitStoreVMFReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	return(0);
}

int BTESH2_TryJitOpcode_BranchOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	if(((op->nmid==BTESH2_NMID_BF) || (op->nmid==BTESH2_NMID_BFS)) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
//		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_PC, UAX_REG_EDX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EDX, (s32)(tr->nxtpc));
		UAX_AsmMovRegImm(jctx, UAX_REG_ECX, (s32)(op->imm));

//		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EAX);
//		UAX_AsmInsnRegImm(jctx, UAX_OP_TEST, UAX_REG_EAX, 1);

		UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(BTESH2_REG_SR*4),
			1);

		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVE, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PC, UAX_REG_EDX);

//		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
//		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, tr->nxtpc);

		return(1);
	}

	if(((op->nmid==BTESH2_NMID_BT) || (op->nmid==BTESH2_NMID_BTS)) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		UAX_AsmMovRegImm(jctx, UAX_REG_EDX, (s32)(tr->nxtpc));
		UAX_AsmMovRegImm(jctx, UAX_REG_ECX, (s32)(op->imm));
		UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(BTESH2_REG_SR*4),
			1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMOVNE, UAX_REG_EDX, UAX_REG_ECX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_PC, UAX_REG_EDX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_BRA) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
		return(1);
	}

	return(0);
}

int BTESH2_TryJitOpcode_CmpOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
#if 1
	if((op->nmid==BTESH2_NMID_CMPEQ) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_R9D);
//		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmCmpRegReg(jctx, UAX_REG_R8D, UAX_REG_R9D);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_CMPGE) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R9D);
		UAX_AsmCmpRegReg(jctx, UAX_REG_R8D, UAX_REG_R9D);
		UAX_AsmInsnReg(jctx, UAX_OP_SETGE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_CMPGT) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R9D);
		UAX_AsmCmpRegReg(jctx, UAX_REG_R8D, UAX_REG_R9D);
		UAX_AsmInsnReg(jctx, UAX_OP_SETG, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_CMPHS) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R9D);
		UAX_AsmCmpRegReg(jctx, UAX_REG_R8D, UAX_REG_R9D);
		UAX_AsmInsnReg(jctx, UAX_OP_SETAE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_CMPHI) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R9D);
		UAX_AsmCmpRegReg(jctx, UAX_REG_R8D, UAX_REG_R9D);
		UAX_AsmInsnReg(jctx, UAX_OP_SETA, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_TST) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_R9D);
		UAX_AsmTestRegReg(jctx, UAX_REG_R8D, UAX_REG_R9D);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_TST) && (op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_R8D);
		UAX_AsmTestRegImm(jctx, UAX_REG_R8D, op->imm);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}

	return(0);
}

int BTESH2_TryJitOpcode_SignExtOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
#if 1
	if((op->nmid==BTESH2_NMID_EXTUB) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZXB, UAX_REG_EAX, UAX_REG_DL);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_EXTUW) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZXW, UAX_REG_EAX, UAX_REG_DX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_EXTSB) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXB, UAX_REG_EAX, UAX_REG_DL);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_EXTSW) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EDX);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVSXW, UAX_REG_EAX, UAX_REG_DX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	return(0);
}
