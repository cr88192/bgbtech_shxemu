void BTSH_Op_BF_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(!(i&1))
		cpu->regs[BTESH2_REG_PC]=op->imm;
}

void BTSH_Op_BFS_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(!(i&1))
		cpu->regs[BTESH2_REG_PC]=op->imm;
}

void BTSH_Op_BT_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(i&1)
		cpu->regs[BTESH2_REG_PC]=op->imm;
}

void BTSH_Op_BTS_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_SR];
	if(i&1)
		cpu->regs[BTESH2_REG_PC]=op->imm;
}

void BTSH_Op_BRA_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[BTESH2_REG_PC]=op->imm;
}

void BTSH_Op_BSR_Abs(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_PC];
	cpu->regs[BTESH2_REG_PR]=i;
	cpu->regs[BTESH2_REG_PC]=op->imm;
}

void BTSH_Op_BRAF_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rm];
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+2;
	cpu->regs[BTESH2_REG_PC]=op->pc+i+4;
}

void BTSH_Op_BSRF_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rm];
	j=cpu->regs[BTESH2_REG_PC];
	cpu->regs[BTESH2_REG_PR]=j;
//	cpu->regs[BTESH2_REG_PC]=op->pc+i+2;
	cpu->regs[BTESH2_REG_PC]=op->pc+i+4;
}


void BTSH_Op_JMP_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rm];
	cpu->regs[BTESH2_REG_PC]=i;
}

void BTSH_Op_JSR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rm];
	j=cpu->regs[BTESH2_REG_PC];
	cpu->regs[BTESH2_REG_PR]=j;
	cpu->regs[BTESH2_REG_PC]=i;
}

void BTSH_Op_RTS_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[BTESH2_REG_PR];
	cpu->regs[BTESH2_REG_PC]=i;
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

void BTSH_Op_NOP_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}

void BTSH_Op_SLEEP_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	printf("Sleep\n");
}

void BTSH_Op_RTE_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	int sp;
	sp=cpu->regs[BTESH2_REG_SP];
	cpu->regs[BTESH2_REG_SP]=sp+8;
	cpu->regs[BTESH2_REG_PC]=BTESH2_GetAddrDWord(cpu, sp+0);
	cpu->regs[BTESH2_REG_SR]=BTESH2_GetAddrDWord(cpu, sp+4);
}

void BTSH_Op_TRAPA_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 pc;
	int sp, vbr;
	sp=cpu->regs[BTESH2_REG_SP]-8;
	BTESH2_SetAddrDWord(cpu, sp+4, cpu->regs[BTESH2_REG_SR]);
	BTESH2_SetAddrDWord(cpu, sp+0, op->imm);
	cpu->regs[BTESH2_REG_SP]=sp;
	
	vbr=cpu->regs[BTESH2_REG_VBR];
//	cpu->regs[BTESH2_REG_PC]=
	pc=BTESH2_GetAddrDWord(cpu, vbr+(op->ro<<2));

//	if(!cpu->regs[BTESH2_REG_PC] && !(cpu->status))
//		cpu->status=BTESH2_EXC_INVADDR;
	if(pc)
	{
		cpu->regs[BTESH2_REG_PC]=pc;
	}else
	{
		BTESH2_ThrowTrap(cpu, op->ro);
//		cpu->status=op->ro;
	}
}

void BTSH_Op_TrapInt(BTESH2_CpuState *cpu, int ro)
{
	u32 pc;
	int sp, vbr, sr, im;

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
		cpu->regs[BTESH2_REG_PC]=pc;
	}
}

void BTSH_Op_TRAP_UD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 pc;
	int sp, vbr;
	int ro;
	
	BTSH_Op_TrapInt(cpu, BTESH2_EXC_UDINST);
}

void BTSH_Op_TRAP_UDLY(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 pc;
	int sp, vbr;
	int ro;
	
	BTSH_Op_TrapInt(cpu, BTESH2_EXC_SLUDINST);
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

	sp=cpu->regs[BTESH2_REG_SP]-8;
	BTESH2_SetAddrDWord(cpu, sp+4, cpu->regs[BTESH2_REG_SR]);
	BTESH2_SetAddrDWord(cpu, sp+0, cpu->regs[BTESH2_REG_PC]);
	cpu->regs[BTESH2_REG_SP]=sp;

	if(pc)
	{
		cpu->regs[BTESH2_REG_SR]=sr|BTESH2_SRFL_BL;
		cpu->regs[BTESH2_REG_PC]=pc;
	}
}
