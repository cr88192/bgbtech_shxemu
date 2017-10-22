void BTSH_Op_BF_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(!(i&1))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BFS_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(!(i&1))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BT_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(i&1)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BTS_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(i&1)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
//		cpu->trnext=NULL;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRA_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[BTESH2_REG_PC]=op->imm;
//	cpu->trnext=NULL;
}

void BTSH_Op_BSR_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_PC];
	cpu->regs[BTESH2_REG_PR]=i;
	cpu->regs[BTESH2_REG_PC]=op->imm;
//	cpu->trnext=NULL;
	cpu->trnext=cpu->trjmpnext;
}

void BTSH_Op_BRAF_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rm];
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+2;
	cpu->regs[BTESH2_REG_PC]=op->pc+i+4;
	cpu->trnext=NULL;
}

void BTSH_Op_BSRF_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rm];
	j=cpu->regs[BTESH2_REG_PC];
	cpu->regs[BTESH2_REG_PR]=j;
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+2;
	cpu->regs[BTESH2_REG_PC]=op->pc+i+4;
	cpu->trnext=NULL;
}


void BTSH_Op_BREQ_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	if(i==0)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRNE_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	if(i!=0)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGT_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	if(((s32)i)>0)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLE_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	if(((s32)i)<=0)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGE_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	if(((s32)i)>=0)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLT_RegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	if(((s32)i)<0)
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BREQ_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	if(((s32)i)==((s32)j))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRNE_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	if(((s32)i)!=((s32)j))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLT_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	if(((s32)i)<((s32)j))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGT_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	if(((s32)i)>((s32)j))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRLE_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	if(((s32)i)<=((s32)j))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}

void BTSH_Op_BRGE_RegRegAbs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	if(((s32)i)>=((s32)j))
	{
		cpu->regs[BTESH2_REG_PC]=op->imm;
		cpu->trnext=cpu->trjmpnext;
	}
}


void BTSH_Op_JMP_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rm];
	cpu->regs[BTESH2_REG_PC]=i;
	cpu->trnext=NULL;
}

void BTSH_Op_JSR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rm];
	j=cpu->regs[BTESH2_REG_PC];
	cpu->regs[BTESH2_REG_PR]=j;
	cpu->regs[BTESH2_REG_PC]=i;
	cpu->trnext=NULL;
}

void BTSH_Op_RTS_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_PR];
	cpu->regs[BTESH2_REG_PC]=i;
	cpu->trnext=NULL;
}

void BTSH_Op_CLRT_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	i=i&(~1);
	cpu->regs[BTESH2_REG_SR]=i;
}

void BTSH_Op_SETT_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	i=i|1;
	cpu->regs[BTESH2_REG_SR]=i;
}

void BTSH_Op_CLRMAC_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	cpu->regs[BTESH2_REG_MACL]=0;
	cpu->regs[BTESH2_REG_MACH]=0;
}

void BTSH_Op_CLRS_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	i=i&(~2);
	cpu->regs[BTESH2_REG_SR]=i;
}

void BTSH_Op_SETS_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	i=i|2;
	cpu->regs[BTESH2_REG_SR]=i;
}

void BTSH_Op_ICLRMD_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
//	i=i&(~1);
	if(op->imm&1)i&=~BTESH2_SRFL_DQ;
	if(op->imm&2)i&=~BTESH2_SRFL_JQ;
	cpu->regs[BTESH2_REG_SR]=i;
}

void BTSH_Op_ISETMD_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
//	i=i|1;
	if(op->imm&1)i|=BTESH2_SRFL_DQ;
	if(op->imm&2)i|=BTESH2_SRFL_JQ;
	cpu->regs[BTESH2_REG_SR]=i;
}

void BTSH_Op_TRAPSP_SET_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	BTESH2_SetAddrDWord(cpu, cpu->regs[BTESH2_REG_SP]-4, 0x12345678);
}

void BTSH_Op_TRAPSP_CLR_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetAddrDWord(cpu, cpu->regs[BTESH2_REG_SP]-4);
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
	u32 i0, i1, i2, i3;
	u32 j0, j1, j2, j3;
	
	i0=cpu->regs[0];	j0=cpu->regs[BTESH2_REG_RBANK+0];
	i1=cpu->regs[1];	j1=cpu->regs[BTESH2_REG_RBANK+1];
	i2=cpu->regs[2];	j2=cpu->regs[BTESH2_REG_RBANK+2];
	i3=cpu->regs[3];	j3=cpu->regs[BTESH2_REG_RBANK+3];
	cpu->regs[0]=j0;	cpu->regs[BTESH2_REG_RBANK+0]=i0;
	cpu->regs[1]=j1;	cpu->regs[BTESH2_REG_RBANK+1]=i1;
	cpu->regs[2]=j2;	cpu->regs[BTESH2_REG_RBANK+2]=i2;
	cpu->regs[3]=j3;	cpu->regs[BTESH2_REG_RBANK+3]=i3;
	i0=cpu->regs[4];	j0=cpu->regs[BTESH2_REG_RBANK+4];
	i1=cpu->regs[5];	j1=cpu->regs[BTESH2_REG_RBANK+5];
	i2=cpu->regs[6];	j2=cpu->regs[BTESH2_REG_RBANK+6];
	i3=cpu->regs[7];	j3=cpu->regs[BTESH2_REG_RBANK+7];
	cpu->regs[4]=j0;	cpu->regs[BTESH2_REG_RBANK+4]=i0;
	cpu->regs[5]=j1;	cpu->regs[BTESH2_REG_RBANK+5]=i1;
	cpu->regs[6]=j2;	cpu->regs[BTESH2_REG_RBANK+6]=i2;
	cpu->regs[7]=j3;	cpu->regs[BTESH2_REG_RBANK+7]=i3;
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

void BTSH_Op_RTE_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	int sp;
	
	if(cpu->arch==BTESH2_ARCH_SH4)
//	if(0)
	{
		BTSH_Op_SetRegBank(cpu,
			cpu->regs[BTESH2_REG_SSR]&BTESH2_SRFL_RB);
		cpu->regs[BTESH2_REG_PC]=cpu->regs[BTESH2_REG_SPC];
		cpu->regs[BTESH2_REG_SR]=cpu->regs[BTESH2_REG_SSR];
		cpu->trnext=NULL;
		return;
	}
	
	sp=cpu->regs[BTESH2_REG_SP];
	cpu->regs[BTESH2_REG_SP]=sp+8;
	cpu->regs[BTESH2_REG_PC]=BTESH2_GetAddrDWord(cpu, sp+0);
	cpu->regs[BTESH2_REG_SR]=BTESH2_GetAddrDWord(cpu, sp+4);
	cpu->trnext=NULL;
}

void BTSH_Op_TRAPA_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 pc, sr;
	int sp, vbr;

	if(cpu->arch==BTESH2_ARCH_SH4)
//	if(0)
	{
		vbr=cpu->regs[BTESH2_REG_VBR];
//		pc=BTESH2_GetAddrDWord(cpu, vbr+(op->ro<<2));

		pc=vbr+0x100;
		cpu->regs[BTESH2_REG_EXPEVT]=0x160;
		cpu->regs[BTESH2_REG_TRAEVT]=op->ro;

//		if(!pc)
		if(!pc || !vbr)
		{
			BTESH2_ThrowTrap(cpu, op->ro);
			return;
		}

		sr=cpu->regs[BTESH2_REG_SR];
		sp=cpu->regs[BTESH2_REG_SP];
//		cpu->regs[BTESH2_REG_SPC]=op->imm;
		cpu->regs[BTESH2_REG_SPC]=op->pc+2;
		cpu->regs[BTESH2_REG_SSR]=sr;
		cpu->regs[BTESH2_REG_SGR]=sp;

		cpu->regs[BTESH2_REG_PC]=pc;
		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL|
			BTESH2_SRFL_MD|BTESH2_SRFL_RB;
		BTSH_Op_SetRegBank(cpu, 1);
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
		return;
	}

	sp=cpu->regs[BTESH2_REG_SP]-8;
	BTESH2_SetAddrDWord(cpu, sp+4, cpu->regs[BTESH2_REG_SR]);
//	BTESH2_SetAddrDWord(cpu, sp+0, op->imm);
	BTESH2_SetAddrDWord(cpu, sp+0, op->pc+2);
	cpu->regs[BTESH2_REG_SP]=sp;
	
	vbr=cpu->regs[BTESH2_REG_VBR];
//	cpu->regs[BTESH2_REG_PC]=
	pc=BTESH2_GetAddrDWord(cpu, vbr+(op->ro<<2));

//	if(!cpu->regs[BTESH2_REG_PC] && !(cpu->status))
//		cpu->status=BTESH2_EXC_INVADDR;
	if(pc)
	{
		cpu->regs[BTESH2_REG_PC]=pc;
//		cpu->regs[BTESH2_REG_PC]=pc+4;
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
	}else
	{
		BTESH2_ThrowTrap(cpu, op->ro);
//		cpu->status=op->ro;
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
	}
}

void BTSH_Op_TrapInt(BTESH2_CpuState *cpu, int ro)
{
	u32 pc;
	int sp, vbr, sr, im, exp;

	sr=cpu->regs[BTESH2_REG_SR];
	vbr=cpu->regs[BTESH2_REG_VBR];
//	if(!vbr)
//		return;

	if(sr&BTESH2_SRFL_BL)
	{
//		cpu->status=ro;
		BTESH2_ThrowTrap(cpu, ro);
		return;
	}

//	if(!vbr || (vbr==0xFAFAFAFAU))
//	if(1)
//	{
//		cpu->status=ro;
//		return;
//	}

#if 0
	pc=BTESH2_GetAddrDWord(cpu, vbr+(ro<<2));
	if(!pc)
	{
//		cpu->status=ro;
		BTESH2_ThrowTrap(cpu, ro);
		return;
	}
#endif

	if(cpu->arch==BTESH2_ARCH_SH4)
//	if(0)
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
	
		pc=vbr+0x100;
		cpu->regs[BTESH2_REG_EXPEVT]=exp;
		cpu->regs[BTESH2_REG_INTEVT]=ro;

		sr=cpu->regs[BTESH2_REG_SR];
		sp=cpu->regs[BTESH2_REG_SP];
		cpu->regs[BTESH2_REG_SPC]=cpu->regs[BTESH2_REG_PC];
		cpu->regs[BTESH2_REG_SSR]=sr;
		cpu->regs[BTESH2_REG_SGR]=sp;

		cpu->regs[BTESH2_REG_PC]=pc;
		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL|
			BTESH2_SRFL_MD|BTESH2_SRFL_RB;
		BTSH_Op_SetRegBank(cpu, 1);
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

	sp=cpu->regs[BTESH2_REG_SP]-8;
	BTESH2_SetAddrDWord(cpu, sp+4, cpu->regs[BTESH2_REG_SR]);
	BTESH2_SetAddrDWord(cpu, sp+0, cpu->regs[BTESH2_REG_PC]);
	cpu->regs[BTESH2_REG_SP]=sp;
	cpu->status=0;

	if(pc)
	{
		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL;
//		cpu->regs[BTESH2_REG_SR]=sr;
		cpu->regs[BTESH2_REG_PC]=pc;
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

void BTSH_Op_TrapIntIrq(BTESH2_CpuState *cpu, int irq)
{
	u32 pc;
	int sp, vbr, sr, im;

	sr=cpu->regs[BTESH2_REG_SR];
	vbr=cpu->regs[BTESH2_REG_VBR];
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
		cpu->regs[BTESH2_REG_INTEVT]=irq;
	
		sr=cpu->regs[BTESH2_REG_SR];
		sp=cpu->regs[BTESH2_REG_SP];
		cpu->regs[BTESH2_REG_SPC]=cpu->regs[BTESH2_REG_PC];
		cpu->regs[BTESH2_REG_SSR]=sr;
		cpu->regs[BTESH2_REG_SGR]=sp;

		cpu->regs[BTESH2_REG_PC]=pc;
		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL|
			BTESH2_SRFL_MD|BTESH2_SRFL_RB;
		BTSH_Op_SetRegBank(cpu, 1);
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;
		return;
	}

	pc=BTESH2_GetAddrDWord(cpu, vbr+(irq<<2));
	if(!pc)
		return;

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
}
