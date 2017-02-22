int BTESH2_TryJitOpcode_BranchOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	BTESH2_Trace *trj;
	int l0, l1, l2;

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
		{
			l0=UAX_GenLabelTemp(jctx);
		
//			UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
//				UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(BTESH2_REG_SR*4),
//				1);
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JNE, l0|UAX_LBL_NEAR);

			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext));

			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
//			UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
//				UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(BTESH2_REG_SR*4),
//				1);
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JNE, l0|UAX_LBL_NEAR);
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			BTESH2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

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
		{
			l0=UAX_GenLabelTemp(jctx);
		
//			UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
//				UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(BTESH2_REG_SR*4),
//				1);
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);

			UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
				UAX_REG_R8Q, UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext));

			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
				UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}else
		{
			l0=UAX_GenLabelTemp(jctx);
//			UAX_AsmInsnStRegDispImm(jctx, UAX_OP_TEST,
//				UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(BTESH2_REG_SR*4),
//				1);
			BTESH2_JitTestVMRegImm(jctx, BTESH2_REG_SR, 1);
			UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);
			BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
			BTESH2_JitFlushJNext(jctx, cpu, tr);
			UAX_EmitLabel(jctx, l0);
			return(1);
		}

		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_BRA) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		if(op->imm==tr->nxtpc)
			return(1);
	
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));
		BTESH2_JitFlushJNext(jctx, cpu, tr);

		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_BSR) &&
		(op->fmid==BTESH2_FMID_ABS))
	{
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PR, (s32)(tr->nxtpc));
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(op->imm));

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
		return(1);
	}
#endif

	return(0);
}

BTESH2_Opcode *BTESH2_TryJitOpcode_CmpOp_GetBrOpA(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	BTESH2_Opcode *op1, *op2;
	
	if(tr->nops<3)
		return(NULL);
	
	op1=tr->ops[tr->nops-1];
	if(((op1->nmid==BTESH2_NMID_BT) ||
		(op1->nmid==BTESH2_NMID_BF)) &&
		(op1->fmid==BTESH2_FMID_ABS))
			{ return(op1); }
	op2=tr->ops[tr->nops-2];
	if(((op2->nmid==BTESH2_NMID_BTS) ||
		(op2->nmid==BTESH2_NMID_BFS)) &&
		(op2->fmid==BTESH2_FMID_ABS))
			{ return(op2); }
	return(NULL);
}

int BTESH2_TryJitOpcode_CmpOp_OpThrashSrT(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	int rt;
	switch(op->nmid)
	{
		case BTESH2_NMID_BT:		case BTESH2_NMID_BF:
		case BTESH2_NMID_BTS:		case BTESH2_NMID_BFS:
		case BTESH2_NMID_CLRT:		case BTESH2_NMID_SETT:
		case BTESH2_NMID_CLRS:		case BTESH2_NMID_SETS:
		case BTESH2_NMID_CMPHS:		case BTESH2_NMID_CMPHI:
		case BTESH2_NMID_CMPEQ:		case BTESH2_NMID_CMPGE:
		case BTESH2_NMID_CMPGT:		case BTESH2_NMID_TST:
		case BTESH2_NMID_CMPPL:		case BTESH2_NMID_CMPPZ:
		case BTESH2_NMID_CMPSTR:	case BTESH2_NMID_MOVT:
		case BTESH2_NMID_SHAL:		case BTESH2_NMID_SHAR:
		case BTESH2_NMID_SHLL:		case BTESH2_NMID_SHLR:
		case BTESH2_NMID_ROTL:		case BTESH2_NMID_ROTCL:
		case BTESH2_NMID_ROTR:		case BTESH2_NMID_ROTCR:
		case BTESH2_NMID_DT:		case BTESH2_NMID_DIV1:
		case BTESH2_NMID_DIV0S:		case BTESH2_NMID_DIV0U:
		case BTESH2_NMID_ADDC:		case BTESH2_NMID_ADDV:
		case BTESH2_NMID_SUBC:		case BTESH2_NMID_SUBV:
		case BTESH2_NMID_NEGC:		case BTESH2_NMID_TASB:
		case BTESH2_NMID_LDC:		case BTESH2_NMID_LDCL:
		case BTESH2_NMID_FCMPEQ:	case BTESH2_NMID_FCMPGT:
		case BTESH2_NMID_TSTB:
			rt=1; break;
		default:
			rt=0; break;
	}
	return(rt);
}

BTESH2_Opcode *BTESH2_TryJitOpcode_CmpOp_GetBrOpB(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	BTESH2_Opcode *op1, *op2;
	int i, j, k;

	if(jctx->jitfl&BTESH2_UAXJFL_NOPREJMP)
		return(NULL);

	op1=BTESH2_TryJitOpcode_CmpOp_GetBrOpA(jctx, cpu, tr, op);
	if(!op1)
		return(NULL);
	
	i=tr->nops-1;
	while((i>0) && (tr->ops[i]!=op1))i--;
	i--;
	while((i>0) && (tr->ops[i]!=op))
	{
		op2=tr->ops[i];
		if(BTESH2_TryJitOpcode_CmpOp_OpThrashSrT(jctx, cpu, tr, op2))
			return(NULL);
		i--;
	}
	
	if(i<=0)
		return(NULL);
	
	return(op1);
}

int BTESH2_TryJitOpcode_CmpOp_OpRR(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op,
	int setcc)
{
	BTESH2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BTESH2_JitLoadReadSyncVMReg(jctx, BTESH2_REG_SR);
	r1=BTESH2_JitLoadReadSyncVMReg(jctx, op->rn);
	r2=BTESH2_JitLoadReadSyncVMReg(jctx, op->rm);

#if 1
	op1=BTESH2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

	if(op1 &&
		((op1->nmid==BTESH2_NMID_BT) ||
		 (op1->nmid==BTESH2_NMID_BTS)))
	{
		jd=1;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JG; break;
			case UAX_OP_SETL: jcc=UAX_OP_JL; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETA: jcc=UAX_OP_JA; break;
			case UAX_OP_SETB: jcc=UAX_OP_JB; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JBE; break;
			default: jcc=-1; break;
		}
	}

	if(op1 &&
		((op1->nmid==BTESH2_NMID_BF) ||
		 (op1->nmid==BTESH2_NMID_BFS)))
	{
		jd=0;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETL: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JL; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JG; break;
			case UAX_OP_SETA: jcc=UAX_OP_JBE; break;
			case UAX_OP_SETB: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JB; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JA; break;
			default: jcc=-1; break;
		}
	}

	if(!(tr->trjmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		r6=r2;
		
		jctx->jitfl|=BTESH2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_EDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_EAX;
		if(r6==UAX_REG_Z)	r6=UAX_REG_ECX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);
//		l2=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, r4); }

		if((r6==r2) && (r5==r1))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2); }
		else if(r6==r2)
			{ BTESH2_JitCmpVMReg(jctx, op->rn, r2); }
		else if(r5==r1)
			{ BTESH2_JitInsnRegVMReg(jctx, UAX_OP_CMP, r1, op->rm); }
		else
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			BTESH2_JitCmpVMReg(jctx, op->rn, UAX_REG_ECX);
		}

#if 0
		if(r6!=r2)
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			BTESH2_JitCmpVMReg(jctx, op->rn, UAX_REG_ECX);
		}else if(r5==UAX_REG_Z)
			{ BTESH2_JitCmpVMReg(jctx, op->rn, r2); }
		else
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2); }
#endif

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext));
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(tr->jmppc));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, r4); }
		return(1);
	}
#endif

	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2);
				UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
				return(1);
			}

			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
			BTESH2_JitInsnRegVMReg(jctx, UAX_OP_CMP, r1, op->rm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
		BTESH2_JitCmpVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
		return(1);
	}else
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, r1, r2);
				UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
				BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
				return(1);
			}

			BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
			BTESH2_JitInsnRegVMReg(jctx, UAX_OP_CMP, r1, op->rm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
			return(1);
		}
	}

	BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
	BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
	BTESH2_JitCmpVMReg(jctx, op->rn, UAX_REG_ECX);
	UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
	BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	return(1);
}

int BTESH2_TryJitOpcode_CmpOp_OpRI(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op,
	int setcc, int idx, s64 imm)
{
	BTESH2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BTESH2_JitLoadReadSyncVMReg(jctx, BTESH2_REG_SR);
	r1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);

#if 1
	op1=BTESH2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

	if(op1 &&
		((op1->nmid==BTESH2_NMID_BT) ||
		 (op1->nmid==BTESH2_NMID_BTS)))
	{
		jd=1;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JG; break;
			case UAX_OP_SETL: jcc=UAX_OP_JL; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETA: jcc=UAX_OP_JA; break;
			case UAX_OP_SETB: jcc=UAX_OP_JB; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JBE; break;
			default: jcc=-1; break;
		}
	}

	if(op1 &&
		((op1->nmid==BTESH2_NMID_BF) ||
		 (op1->nmid==BTESH2_NMID_BFS)))
	{
		jd=0;
		switch(setcc)
		{
			case UAX_OP_SETE: jcc=UAX_OP_JNE; break;
			case UAX_OP_SETNE: jcc=UAX_OP_JE; break;
			case UAX_OP_SETG: jcc=UAX_OP_JLE; break;
			case UAX_OP_SETL: jcc=UAX_OP_JGE; break;
			case UAX_OP_SETGE: jcc=UAX_OP_JL; break;
			case UAX_OP_SETLE: jcc=UAX_OP_JG; break;
			case UAX_OP_SETA: jcc=UAX_OP_JBE; break;
			case UAX_OP_SETB: jcc=UAX_OP_JAE; break;
			case UAX_OP_SETAE: jcc=UAX_OP_JB; break;
			case UAX_OP_SETBE: jcc=UAX_OP_JA; break;
			default: jcc=-1; break;
		}
	}

	if(!(tr->trjmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		
		jctx->jitfl|=BTESH2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_EDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_ECX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, r4); }

		if(r5==r1)
			{ UAX_AsmInsnRegImm(jctx, UAX_OP_CMP, r1, (s32)op->imm); }
		else
			{ BTESH2_JitInsnVMRegImm(jctx, UAX_OP_CMP, idx, (s32)op->imm); }

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext));
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(tr->jmppc));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, r4); }
		return(1);
	}
#endif

	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
			UAX_AsmCmpRegImm(jctx, r1, (s32)imm);
			UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
		UAX_AsmCmpRegImm(jctx, UAX_REG_ECX, (s32)imm);
		UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
		return(1);
	}

	if(r1!=UAX_REG_Z)
	{
		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		UAX_AsmCmpRegImm(jctx, r1, (s32)imm);
		UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}

	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
	BTESH2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
	UAX_AsmCmpRegImm(jctx, UAX_REG_ECX, (s32)imm);
	UAX_AsmInsnReg(jctx, setcc, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
	BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	return(1);
}

#if 1
int BTESH2_TryJitOpcode_TestOp_OpRR(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	BTESH2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BTESH2_JitLoadReadSyncVMReg(jctx, BTESH2_REG_SR);
	r1=BTESH2_JitLoadReadSyncVMReg(jctx, op->rn);
	r2=BTESH2_JitLoadReadSyncVMReg(jctx, op->rm);

#if 1
	op1=BTESH2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;
	
	if(op1 &&
		((op1->nmid==BTESH2_NMID_BT) ||
		 (op1->nmid==BTESH2_NMID_BTS)))
			{ jd=1; jcc=UAX_OP_JE; }
	if(op1 &&
		((op1->nmid==BTESH2_NMID_BF) ||
		 (op1->nmid==BTESH2_NMID_BFS)))
			{ jd=0; jcc=UAX_OP_JNE; }

	if(!(tr->trjmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		r6=r2;
		
		jctx->jitfl|=BTESH2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_EDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_EAX;
		if(r6==UAX_REG_Z)	r6=UAX_REG_ECX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, r4); }

		if((r6==r2) && (r5==r1))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, r1, r2); }
		else if(r6==r2)
			{ BTESH2_JitTestVMReg(jctx, op->rn, r2); }
		else if(r5==r1)
		{
//			BTESH2_JitInsnRegVMReg(jctx, UAX_OP_TEST, r1, op->rm);
			BTESH2_JitInsnVMRegReg(jctx, UAX_OP_TEST, op->rm, r1);
		}
		else
		{
			BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
			BTESH2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
		}

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext));
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(tr->jmppc));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, r4); }
		return(1);
	}
#endif

	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, r1, r2);
				UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
				return(1);
			}

			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
//			BTESH2_JitInsnRegVMReg(jctx, UAX_OP_TEST, r1, op->rm);
			BTESH2_JitInsnVMRegReg(jctx, UAX_OP_TEST, op->rm, r1);
			UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
		BTESH2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
		return(1);
	}else
	{
		if(r1!=UAX_REG_Z)
		{
			if(r2!=UAX_REG_Z)
			{
				BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
				UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
				UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
				UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, r1, r2);
				UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
				UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
				BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
				return(1);
			}

			BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
//			BTESH2_JitInsnRegVMReg(jctx, UAX_OP_TEST, r1, op->rm);
			BTESH2_JitInsnVMRegReg(jctx, UAX_OP_TEST, op->rm, r1);
			UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
			BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
			return(1);
		}
	}

	BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
	BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
	BTESH2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
	UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
	BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	return(1);
}

int BTESH2_TryJitOpcode_TestOp_OpRI(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op,
	int idx, s64 imm)
{
	BTESH2_Opcode *op1;
	int jcc, jd;
	int l0, l1, l2;
	int r0, r1, r2;
	int r4, r5, r6;

	r0=BTESH2_JitLoadReadSyncVMReg(jctx, BTESH2_REG_SR);
	r1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);

#if 1
	op1=BTESH2_TryJitOpcode_CmpOp_GetBrOpB(jctx, cpu, tr, op);
	jcc=-1;

	if(op1 &&
		((op1->nmid==BTESH2_NMID_BT) ||
		 (op1->nmid==BTESH2_NMID_BTS)))
			{ jd=1; jcc=UAX_OP_JE; }
	if(op1 &&
		((op1->nmid==BTESH2_NMID_BF) ||
		 (op1->nmid==BTESH2_NMID_BFS)))
			{ jd=0; jcc=UAX_OP_JNE; }

	if(!(tr->trjmpnext))
		jcc=-1;

	if(op1 && (jcc>0))
	{
		r4=r0;
		r5=r1;
		
		jctx->jitfl|=BTESH2_UAXJFL_PREJMP;

		if(r4==UAX_REG_Z)	r4=UAX_REG_EDX;
		if(r5==UAX_REG_Z)	r5=UAX_REG_ECX;
	
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		if(r4!=r0)
			{ BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, r4); }

		if(r5==r1)
			{ UAX_AsmInsnRegImm(jctx, UAX_OP_TEST, r1, (s32)op->imm); }
		else
			{ BTESH2_JitInsnVMRegImm(jctx, UAX_OP_TEST, idx, (s32)op->imm); }

		UAX_AsmInsnLabel(jctx, jcc, l0|UAX_LBL_NEAR);

		if(jd==1)
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }
		else
			{ UAX_AsmOrRegImm(jctx, r4, 1); }

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1|UAX_LBL_NEAR);

		UAX_EmitLabel(jctx, l0);		

		if(jd==1)
			{ UAX_AsmOrRegImm(jctx, r4, 1); }
		else
			{ UAX_AsmAndRegImm(jctx, r4, (s32)(~1)); }

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_R8Q, UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext));
		BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(tr->jmppc));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);

		UAX_EmitLabel(jctx, l1);
		if(r4!=r0)
			{ BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, r4); }
		return(1);
	}
#endif

#if 1
	if(r0!=UAX_REG_Z)
	{
		if(r1!=UAX_REG_Z)
		{
			UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
			UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
			UAX_AsmTestRegImm(jctx, r1, (s32)imm);
			UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
			UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
			return(1);
		}

		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		UAX_AsmAndRegImm(jctx, r0, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
		UAX_AsmTestRegImm(jctx, UAX_REG_ECX, (s32)imm);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, r0, UAX_REG_EAX);
		return(1);
	}
#endif

#if 1
	if(r1!=UAX_REG_Z)
	{
		UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		UAX_AsmTestRegImm(jctx, r1, (s32)imm);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
	}
#endif

#if 1
	UAX_AsmXorRegReg(jctx, UAX_REG_EAX, UAX_REG_EAX);
	BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
	BTESH2_JitLoadVMReg(jctx, idx, UAX_REG_ECX);
	UAX_AsmTestRegImm(jctx, UAX_REG_ECX, (s32)imm);
	UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_EDX, UAX_REG_EAX);
	BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	return(1);
#endif

#if 0
	BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
	UAX_AsmMovRegImm(jctx, UAX_REG_ECX, (s32)imm);
	BTESH2_JitTestVMReg(jctx, idx, UAX_REG_ECX);
	UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
	UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
	BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
	return(1);
#endif
}
#endif


int BTESH2_TryJitOpcode_CmpOp(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
	int r0, r1, r2;
	int l0, l1, l2;

#if 1
	if((op->nmid==BTESH2_NMID_CMPEQ) &&
		(op->fmid==BTESH2_FMID_REGIMM))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETE, op->rn, (s32)op->imm);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_CMPEQ) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETE);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_CMPGE) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETGE);
		return(1);
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_CMPGT) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETG);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_CMPHS) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETAE);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_CMPHI) &&
		(op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRR(
			jctx, cpu, tr, op, UAX_OP_SETA);
		return(1);
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_TST) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_TryJitOpcode_TestOp_OpRR(
			jctx, cpu, tr, op);
		return(1);

#if 0
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_ECX);
		BTESH2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_TST) && (op->fmid==BTESH2_FMID_REGIMM))
	{
#if 1
		BTESH2_TryJitOpcode_TestOp_OpRI(jctx, cpu, tr, op,
			op->rn, (s32)op->imm);
		return(1);
#endif

#if 0
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		UAX_AsmMovRegImm(jctx, UAX_REG_ECX, (s32)op->imm);
		BTESH2_JitTestVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif
	}
#endif

//	return(0);

#if 1
	if((op->nmid==BTESH2_NMID_CMPPL) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETG, op->rn, 0);
		return(1);

#if 0
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmCmpRegImm(jctx, UAX_REG_ECX, 0);
		UAX_AsmInsnReg(jctx, UAX_OP_SETG, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_CMPPZ) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_TryJitOpcode_CmpOp_OpRI(jctx, cpu, tr, op,
			UAX_OP_SETGE, op->rn, 0);
		return(1);

#if 0
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmCmpRegImm(jctx, UAX_REG_ECX, 0);
		UAX_AsmInsnReg(jctx, UAX_OP_SETGE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		return(1);
#endif
	}
#endif

#if 1
	if((op->nmid==BTESH2_NMID_DT) &&
		(op->fmid==BTESH2_FMID_REGRN))
	{
		BTESH2_JitLoadVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_EDX, (s32)(~1));
		BTESH2_JitLoadVMReg(jctx, op->rn, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_DEC, UAX_REG_ECX);
		UAX_AsmInsnReg(jctx, UAX_OP_SETE, UAX_REG_AL);
		UAX_AsmOrRegReg(jctx, UAX_REG_DL, UAX_REG_AL);
		BTESH2_JitStoreVMReg(jctx, BTESH2_REG_SR, UAX_REG_EDX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_ECX);
		return(1);
	}
#endif

	return(0);
}
