void BTSH_Op_UpdateForSr(BTESH2_CpuState *cpu);

void BTSH_Op_BF_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	if(!(i&1))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BFS_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	if(!(i&1))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BT_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	if(i&1)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BTS_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	if(i&1)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRA_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
	cpu->trnext=NULL;
//	cpu->trnext=cpu->trjmpnext;
}

void BTSH_Op_BSR_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, BTESH2_REG_PC);
//	cpu->regs[BTESH2_REG_PR]=i;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PR, i);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
//	cpu->trnext=NULL;
	cpu->trnext=cpu->trjmpnext;
}

void BTSH_Op_BRAF_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rm);
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+2;
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+4;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->pc+i+4);
	cpu->trnext=NULL;
}

void BTSH_Op_BSRF_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=BTESH2_GetRegQWord(cpu, BTESH2_REG_PC);
//	BTESH2_SetRegQWord(cpu, BTESH2_REG_PR, j);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PR, j);
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+2;
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+4;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->pc+i+4);
	cpu->trnext=NULL;
}


void BTSH_Op_BREQ_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	if(i==0)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRNE_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	if(i!=0)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGT_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	if(((s32)i)>0)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLE_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	if(((s32)i)<=0)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGE_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	if(((s32)i)>=0)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLT_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	if(((s32)i)<0)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BREQ_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	if(((s32)i)==((s32)j))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRNE_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	if(((s32)i)!=((s32)j))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLT_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	if(((s32)i)<((s32)j))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGT_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	if(((s32)i)>((s32)j))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLE_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	if(((s32)i)<=((s32)j))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGE_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	if(((s32)i)>=((s32)j))
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, op->imm);
		cpu->trnext=cpu->trjmpnext;
	}
}


void BTSH_Op_JMP_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, i);
	cpu->trnext=NULL;
}

void BTSH_Op_JSR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=BTESH2_GetRegQWord(cpu, BTESH2_REG_PC);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PR, j);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, i);
	cpu->trnext=NULL;
}

void BTSH_Op_RTS_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, BTESH2_REG_PR);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, i);
	cpu->trnext=NULL;
}

void BTSH_Op_CLRT_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	i=i&(~1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, i);
}

void BTSH_Op_SETT_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	i=i|1;
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, i);
}

void BTSH_Op_CLRMAC_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
//	cpu->regs[BTESH2_REG_MACL]=0;
//	cpu->regs[BTESH2_REG_MACH]=0;
//	cpu->regs[BTESH2_REG_RLO+BTESH2_REG_MACL]=0;
//	cpu->regs[BTESH2_REG_RLO+BTESH2_REG_MACH]=0;
//	cpu->regs[BTESH2_REG_RHI+BTESH2_REG_MACL]=0;
//	cpu->regs[BTESH2_REG_RHI+BTESH2_REG_MACH]=0;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_MACL, 0);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_MACH, 0);
}

void BTSH_Op_CLRS_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	i=i&(~2);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, i);
}

void BTSH_Op_SETS_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	i=i|2;
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, i);
}

void BTSH_Op_ICLRMD_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
//	i=i&(~1);
	if(op->imm&1)i&=~BTESH2_SRFL_DQ;
	if(op->imm&2)i&=~BTESH2_SRFL_JQ;
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, i);

//	BTSH_Op_UpdateForSr(cpu);

	if(i&BTESH2_SRFL_JQ)
		{ cpu->csfl|=BTESH2_CSFL_SRJQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRJQ; }

	if(i&BTESH2_SRFL_DQ)
		{ cpu->csfl|=BTESH2_CSFL_SRDQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRDQ; }
}

void BTSH_Op_ISETMD_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
//	i=i|1;
	if(op->imm&1)i|=BTESH2_SRFL_DQ;
	if(op->imm&2)i|=BTESH2_SRFL_JQ;
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, i);

//	BTSH_Op_UpdateForSr(cpu);

	if(i&BTESH2_SRFL_JQ)
		{ cpu->csfl|=BTESH2_CSFL_SRJQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRJQ; }

	if(i&BTESH2_SRFL_DQ)
		{ cpu->csfl|=BTESH2_CSFL_SRDQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRDQ; }
}

void BTSH_Op_TRAPSP_SET_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP);
	BTESH2_SetAddrDWord(cpu, i-4, 0x12345678);
}

void BTSH_Op_TRAPSP_CLR_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP);
	i=BTESH2_GetAddrDWord(cpu, i-4);
	if(i!=0x12345678)
		{ cpu->status=BTESH2_EXC_STACK2; }
}

void BTSH_Op_NOP_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}

void BTSH_Op_SLEEP_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	printf("Sleep\n");
	cpu->status=BTESH2_EXC_TRAPSLEEP;
	cpu->trnext=NULL;
	cpu->trjmpnext=NULL;
}

void BTSH_Op_SetRegBankSwap(BTESH2_CpuState *cpu)
{
//	u32 i0, i1, i2, i3;
//	u32 j0, j1, j2, j3;
	u64 i0, i1, j0, j1;
	int i;
	
#if 0
	i0=cpu->regs[BTESH2_REG_R0];	j0=cpu->regs[BTESH2_REG_RBANK+0];
	i1=cpu->regs[BTESH2_REG_R1];	j1=cpu->regs[BTESH2_REG_RBANK+1];
	i2=cpu->regs[BTESH2_REG_R2];	j2=cpu->regs[BTESH2_REG_RBANK+2];
	i3=cpu->regs[BTESH2_REG_R3];	j3=cpu->regs[BTESH2_REG_RBANK+3];
	cpu->regs[BTESH2_REG_R0]=j0;	cpu->regs[BTESH2_REG_RBANK+0]=i0;
	cpu->regs[BTESH2_REG_R1]=j1;	cpu->regs[BTESH2_REG_RBANK+1]=i1;
	cpu->regs[BTESH2_REG_R2]=j2;	cpu->regs[BTESH2_REG_RBANK+2]=i2;
	cpu->regs[BTESH2_REG_R3]=j3;	cpu->regs[BTESH2_REG_RBANK+3]=i3;
	i0=cpu->regs[BTESH2_REG_R4];	j0=cpu->regs[BTESH2_REG_RBANK+4];
	i1=cpu->regs[BTESH2_REG_R5];	j1=cpu->regs[BTESH2_REG_RBANK+5];
	i2=cpu->regs[BTESH2_REG_R6];	j2=cpu->regs[BTESH2_REG_RBANK+6];
	i3=cpu->regs[BTESH2_REG_R7];	j3=cpu->regs[BTESH2_REG_RBANK+7];
	cpu->regs[BTESH2_REG_R4]=j0;	cpu->regs[BTESH2_REG_RBANK+4]=i0;
	cpu->regs[BTESH2_REG_R5]=j1;	cpu->regs[BTESH2_REG_RBANK+5]=i1;
	cpu->regs[BTESH2_REG_R6]=j2;	cpu->regs[BTESH2_REG_RBANK+6]=i2;
	cpu->regs[BTESH2_REG_R7]=j3;	cpu->regs[BTESH2_REG_RBANK+7]=i3;
#endif

#if 0
	for(i=0; i<8; i++)
	{
		i0=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_R0+i];
		i1=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_R0+i];
		i2=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_R16+i];
		i3=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_R16+i];
		j0=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_RBANK+i];
		j1=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_RBANK+i];
		j2=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_RBANK2+i];
		j3=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_RBANK2+i];
		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_R0+i]=j0;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_R0+i]=j1;
		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_R16+i]=j2;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_R16+i]=j3;
		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_RBANK+i]=i0;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_RBANK+i]=i1;
		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_RBANK2+i]=i2;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_RBANK2+i]=i3;
	}
#endif

#if 1
	for(i=0; i<8; i++)
	{
		i0=BTESH2_GetRegQWord(cpu, BTESH2_REG_R0+i);
		i1=BTESH2_GetRegQWord(cpu, BTESH2_REG_R16+i);
		j0=BTESH2_GetRegQWord(cpu, BTESH2_REG_RBANK+i);
		j1=BTESH2_GetRegQWord(cpu, BTESH2_REG_RBANK2+i);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_R0+i, j0);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_R16+i, j1);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_RBANK+i, i0);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_RBANK2+i, i1);
	}
#endif
}

void BTSH_Op_SetRegBank(BTESH2_CpuState *cpu, int rb)
{
//	rb=(rb!=0);

	if(rb)
	{
		if(!(cpu->ctfl&1))
		{
			BTSH_Op_SetRegBankSwap(cpu);
			cpu->ctfl|=1;
		}
	}else
	{
		if(cpu->ctfl&1)
		{
			BTSH_Op_SetRegBankSwap(cpu);
			cpu->ctfl&=~1;
		}
	}
}

void BTSH_Op_UpdateForSr(BTESH2_CpuState *cpu)
{
	u32 v;
	
//	v=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SR];
	v=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);

	if(v&BTESH2_SRFL_JQ)
		{ cpu->csfl|=BTESH2_CSFL_SRJQ; }
	else
	{
//		__debugbreak();
		cpu->csfl&=~BTESH2_CSFL_SRJQ;
	}

	if(v&BTESH2_SRFL_DQ)
		{ cpu->csfl|=BTESH2_CSFL_SRDQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRDQ; }
}

void BTSH_Op_RTE_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 pc, sr, sp;
//	u32 pcl, pch, srl, srh;
//	int sp;
	
	if(cpu->arch==BTESH2_ARCH_SH4)
//	if(0)
	{
		pc=BTESH2_GetRegQWord(cpu, BTESH2_REG_SPC);
		sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SSR);
		BTSH_Op_SetRegBank(cpu,
			sr&BTESH2_SRFL_RB);

//		BTSH_Op_SetRegBank(cpu,
//			cpu->regs[BTESH2_REG_SSR]&BTESH2_SRFL_RB);
//		cpu->regs[BTESH2_REG_PC]=cpu->regs[BTESH2_REG_SPC];
//		cpu->regs[BTESH2_REG_SR]=cpu->regs[BTESH2_REG_SSR];

//		pcl=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SPC];
//		pch=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SPC];
//		srl=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SSR];
//		srh=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SSR];
		
//		if(!(srl&BTESH2_SRFL_JQ))
//		{
//			__debugbreak();
//		}
		
//		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_PC]=pcl;
//		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_PC]=pch;
//		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SR]=srl;
//		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SR]=srh;

		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SR, sr);

//		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_PC]=
//			cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SPC];
//		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_PC]=
//			cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SPC];
//		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SR]=
//			cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SSR];
//		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SR]=
//			cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SSR];
		BTSH_Op_UpdateForSr(cpu);

		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
		return;
	}

	sp=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_SP, sp+8);
	pc=BTESH2_GetAddrDWord(cpu, sp+0);
	sr=BTESH2_GetAddrDWord(cpu, sp+4);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_SR, sr);

//	sp=cpu->regs[BTESH2_REG_SP];
//	cpu->regs[BTESH2_REG_SP]=sp+8;
//	cpu->regs[BTESH2_REG_PC]=BTESH2_GetAddrDWord(cpu, sp+0);
//	cpu->regs[BTESH2_REG_SR]=BTESH2_GetAddrDWord(cpu, sp+4);
	cpu->trnext=NULL;
	cpu->trjmpnext=NULL;
}

void BTSH_Op_TRAPA_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	u32 pc, sr, pch, srh, sph;
//	u32 sp, vbr;

	u64 pc, sr;
	u64 sp, vbr;

	if(cpu->arch==BTESH2_ARCH_SH4)
//	if(0)
	{
		vbr=BTESH2_GetRegQWord(cpu, BTESH2_REG_VBR);

		pc=vbr+0x100;

		BTESH2_SetRegQWord(cpu, BTESH2_REG_EXPEVT, 0x160);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_TRAEVT, op->ro);

		if(!pc || !vbr)
		{
			BTESH2_ThrowTrap(cpu, op->ro);
			return;
		}

		sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);
		sp=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP);

		BTESH2_SetRegQWord(cpu, BTESH2_REG_SSR, sr);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SGR, sp);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SPC, op->pc+2);

		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SR,
			sr|BTESH2_SRFL_BL|BTESH2_SRFL_MD|BTESH2_SRFL_RB);

		BTSH_Op_SetRegBank(cpu, 1);
		BTSH_Op_UpdateForSr(cpu);
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
		return;
	}

	sp=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP)-8;
	sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);

	BTESH2_SetAddrDWord(cpu, sp+4, sr);
	BTESH2_SetAddrDWord(cpu, sp+0, op->pc+2);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_SP, sp);

	
	vbr=BTESH2_GetRegQWord(cpu, BTESH2_REG_VBR);
	pc=BTESH2_GetAddrDWord(cpu, vbr+(op->ro<<2));

	if(pc)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
	}else
	{
		BTESH2_ThrowTrap(cpu, op->ro);
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
	}
}

void BTSH_Op_TrapInt(BTESH2_CpuState *cpu, int ro)
{
	u64 vbr, pc, spc;
	u64 sp, sr, im, exp;

	sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);
	vbr=BTESH2_GetRegQWord(cpu, BTESH2_REG_VBR);

	if(sr&BTESH2_SRFL_BL)
	{
		BTESH2_ThrowTrap(cpu, ro);
		return;
	}

	if(cpu->arch==BTESH2_ARCH_SH4)
	{
		exp=0;
		
		switch(ro)
		{
		case BTESH2_EXC_UDINST: exp=0x180; break;
		case BTESH2_EXC_SLUDINST: exp=0x1A0; break;
		case BTESH2_EXC_INVADDR: exp=0x0E0; break;
		default:
			break;
		}

		if(!vbr)
		{
			BTESH2_ThrowTrap(cpu, ro);
			return;
		}

//		vbrl=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_VBR];
//		vbrh=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_VBR];
//		vbr=((u64)vbrl)|(((u64)vbrh)<<32);
	
		pc=vbr+0x100;

		BTESH2_SetRegQWord(cpu, BTESH2_REG_EXPEVT, exp);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_TRAEVT, ro);

		sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);
		sp=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP);
		spc=BTESH2_GetRegQWord(cpu, BTESH2_REG_PC);

		BTESH2_SetRegQWord(cpu, BTESH2_REG_SSR, sr);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SGR, sp);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SPC, spc);

		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SR,
			sr|BTESH2_SRFL_BL|BTESH2_SRFL_MD|BTESH2_SRFL_RB);

#if 0
		cpu->regs[BTESH2_REG_EXPEVT]=exp;
		cpu->regs[BTESH2_REG_INTEVT]=ro;

		sr=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SR];
		sp=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SP];

		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SPC]=
			cpu->regs[BTESH2_REG_RLO+BTESH2_REG_PC];
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SPC]=
			cpu->regs[BTESH2_REG_RHI+BTESH2_REG_PC];

		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SSR]=sr;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SSR]=srh;

		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SGR]=sp;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SGR]=sph;

		sr&=~(BTESH2_SRFL_JQ|BTESH2_SRFL_DQ);
		sr|=srh&(BTESH2_SRFL_JQ|BTESH2_SRFL_DQ);

		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_PC]=pc;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_PC]=pc>>32;

		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SR]=
			sr|BTESH2_SRFL_BL|BTESH2_SRFL_MD|BTESH2_SRFL_RB;
#endif

		BTSH_Op_SetRegBank(cpu, 1);
		BTSH_Op_UpdateForSr(cpu);

		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
		return;
	}

	pc=BTESH2_GetAddrDWord(cpu, vbr+(ro<<2));
	if(!pc)
	{
//		cpu->status=ro;
		BTESH2_ThrowTrap(cpu, ro);
		return;
	}

//	sp=cpu->regs[BTESH2_REG_SP]-8;
	sp=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP)-8;

	sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);
	spc=BTESH2_GetRegQWord(cpu, BTESH2_REG_PC);

//	BTESH2_SetAddrDWord(cpu, sp+4, cpu->regs[BTESH2_REG_SR]);
//	BTESH2_SetAddrDWord(cpu, sp+0, cpu->regs[BTESH2_REG_PC]);
	BTESH2_SetAddrDWord(cpu, sp+4, sr);
	BTESH2_SetAddrDWord(cpu, sp+0, spc);
//	cpu->regs[BTESH2_REG_SP]=sp;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_SP, sp);
	cpu->status=0;

	if(pc)
	{
//		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL;
//		cpu->regs[BTESH2_REG_PC]=pc;

		BTESH2_SetRegQWord(cpu, BTESH2_REG_SR,
			sr|BTESH2_SRFL_BL);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);

		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
	}
}

void BTSH_Op_TRAP_UD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 pc;
	int sp, vbr;
	int ro;
	
	BTSH_Op_TrapInt(cpu, BTESH2_EXC_UDINST);
	cpu->trnext=NULL;
	cpu->trjmpnext=NULL;
}

void BTSH_Op_TRAP_UDLY(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 pc;
	int sp, vbr;
	int ro;
	
	BTSH_Op_TrapInt(cpu, BTESH2_EXC_SLUDINST);
	cpu->trnext=NULL;
	cpu->trjmpnext=NULL;
}

void BTSH_Op_TRAP_BRK(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 pc;
	int sp, vbr;
	int ro;
	
//	BTSH_Op_TrapInt(cpu, BTESH2_EXC_BREAK);
	BTESH2_ThrowTrap(cpu, BTESH2_EXC_BREAK);

	cpu->trnext=NULL;
	cpu->trjmpnext=NULL;
}

void BTSH_Op_TrapIntIrq(BTESH2_CpuState *cpu, int irq)
{
	u64 pc, spc;
	u64 sp, vbr, sr, im;
//	u32 sph, srh, pch;

	sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);
//	vbr=cpu->regs[BTESH2_REG_VBR];
	vbr=BTESH2_GetRegQWord(cpu, BTESH2_REG_VBR);
//	if(!vbr)
//		return;

	if(sr&BTESH2_SRFL_BL)
		return;

	im=(sr>>4)&15;
	if(im>8)
		return;

//	if(!vbr || (vbr==0xFAFAFAFAU))
//	if(vbr==0xFAFAFAFAU)
//		return;

	pc=BTESH2_GetAddrDWord(cpu, vbr+(irq<<2));
	if(!pc)
		return;

	if(cpu->arch==BTESH2_ARCH_SH4)
//	if(0)
	{
		if(!vbr)
			return;

//		pc=vbr+0x100;
		pc=vbr+0x600;

//		BTESH2_SetRegQWord(cpu, BTESH2_REG_EXPEVT, exp);
//		BTESH2_SetRegQWord(cpu, BTESH2_REG_TRAEVT, ro);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_INTEVT, irq);

		sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);
		sp=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP);
		spc=BTESH2_GetRegQWord(cpu, BTESH2_REG_PC);

		BTESH2_SetRegQWord(cpu, BTESH2_REG_SSR, sr);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SGR, sp);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SPC, spc);

		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SR,
			sr|BTESH2_SRFL_BL|BTESH2_SRFL_MD|BTESH2_SRFL_RB);

#if 0
		cpu->regs[BTESH2_REG_INTEVT]=irq;
	
		sr=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SR];
		sp=cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SP];
		srh=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SR];
		sph=cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SP];

//		if(!(sr&BTESH2_SRFL_JQ))
//		{
//			__debugbreak();
//		}

		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SPC]=
			cpu->regs[BTESH2_REG_RLO+BTESH2_REG_PC];
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SPC]=
			cpu->regs[BTESH2_REG_RHI+BTESH2_REG_PC];
		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SSR]=sr;
		cpu->regs[BTESH2_REG_RLO+BTESH2_REG_SGR]=sp;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SSR]=srh;
		cpu->regs[BTESH2_REG_RHI+BTESH2_REG_SGR]=sph;

		sr&=~(BTESH2_SRFL_JQ|BTESH2_SRFL_DQ);
		sr|=srh&(BTESH2_SRFL_JQ|BTESH2_SRFL_DQ);

		cpu->regs[BTESH2_REG_PC]=pc;
		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL|
			BTESH2_SRFL_MD|BTESH2_SRFL_RB;
#endif

		BTSH_Op_SetRegBank(cpu, 1);
		BTSH_Op_UpdateForSr(cpu);

		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
		return;
	}

	pc=BTESH2_GetAddrDWord(cpu, vbr+(irq<<2));
	if(!pc)
		return;

#if 1
	sp=BTESH2_GetRegQWord(cpu, BTESH2_REG_SP)-8;
	sr=BTESH2_GetRegQWord(cpu, BTESH2_REG_SR);
	spc=BTESH2_GetRegQWord(cpu, BTESH2_REG_PC);
	BTESH2_SetAddrDWord(cpu, sp+4, sr);
	BTESH2_SetAddrDWord(cpu, sp+0, spc);
	BTESH2_SetRegQWord(cpu, BTESH2_REG_SP, sp);
	cpu->status=0;

	if(pc)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SR, sr|BTESH2_SRFL_BL);
		BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, pc);
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
	}
#endif

#if 0
	sp=cpu->regs[BTESH2_REG_SP]-8;
	BTESH2_SetAddrDWord(cpu, sp+4, cpu->regs[BTESH2_REG_SR]);
	BTESH2_SetAddrDWord(cpu, sp+0, cpu->regs[BTESH2_REG_PC]);
	cpu->regs[BTESH2_REG_SP]=sp;

	if(pc)
	{
//		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL;
		cpu->regs[BTESH2_REG_SR]=sr;
		cpu->regs[BTESH2_REG_PC]=pc;
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
	}
#endif
}
