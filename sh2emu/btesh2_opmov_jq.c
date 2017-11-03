void BTSH_OpJQ_MOV_RegImmQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetRegQWord(cpu, op->rn, op->imm);
}

void BTSH_OpJQ_MOVA_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetRegQWord(cpu, op->rn, op->imm);
}

void BTSH_OpJQ_MOV_RegLdAbsW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, op->imm);
}

void BTSH_OpJQ_MOV_RegLdAbsD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, op->imm);
}

void BTSH_OpJQ_MOV_RegLdAbsWQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		(s16)BTESH2_GetAddrWord(cpu, op->imm));
}

void BTSH_OpJQ_MOV_RegLdAbsDQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrDWord(cpu, op->imm));
}

void BTSH_OpJQ_MOV_RegLdAbsQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu, op->imm));
}

void BTSH_OpJQ_MOVU_RegLdAbsW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(u16)BTESH2_GetAddrWord(cpu, op->imm);
}

void BTSH_OpJQ_MOVU_RegLdAbsWQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		(u16)BTESH2_GetAddrWord(cpu, op->imm));
}


void BTSH_OpJQ_MOV_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn+BTESH2_REG_RLO]=cpu->regs[op->rm+BTESH2_REG_RLO];
	cpu->regs[op->rn+BTESH2_REG_RHI]=cpu->regs[op->rm+BTESH2_REG_RHI];
}

void BTSH_OpJQ_MOV_RegMemB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm));
}
void BTSH_OpJQ_MOV_RegMemW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm));
}
void BTSH_OpJQ_MOV_RegMemD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegMemQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu,
			BTESH2_GetRegQWord(cpu, op->rm)));
}

void BTSH_OpJQ_MOV_RegMemD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu, cpu->regs[op->rm]);
}


void BTSH_OpJQ_MOV_RegStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu, BTESH2_GetRegQWord(cpu, op->rn), cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn),
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegStD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWordFMMU(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}


void BTSH_OpJQ_MOV_RegDecStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]--;
	BTESH2_SetAddrByte(cpu, BTESH2_GetRegQWord(cpu, op->rn), cpu->regs[op->rm]);
}
void BTSH_OpJQ_MOV_RegDecStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=2;
	BTESH2_SetAddrWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), cpu->regs[op->rm]);
}
void BTSH_OpJQ_MOV_RegDecStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegDecStQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=8;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn),
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegDecStD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWordFMMU(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}


#if 1
void BTSH_OpJQ_MOV_ImmStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu, BTESH2_GetRegQWord(cpu, op->rn), op->imm);
}

void BTSH_OpJQ_MOV_ImmStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), op->imm);
}

void BTSH_OpJQ_MOV_ImmStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), op->imm);
}

void BTSH_OpJQ_MOV_ImmStQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn),
		(u64)((s32)op->imm));
}

void BTSH_OpJQ_MOV_ImmDecStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]--;
	BTESH2_SetAddrByte(cpu, BTESH2_GetRegQWord(cpu, op->rn), op->imm);
}
void BTSH_OpJQ_MOV_ImmDecStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=2;
	BTESH2_SetAddrWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), op->imm);
}
void BTSH_OpJQ_MOV_ImmDecStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWord(cpu, BTESH2_GetRegQWord(cpu, op->rn), op->imm);
}
void BTSH_OpJQ_MOV_ImmDecStQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=8;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn),
		(u64)((s32)op->imm));
}
#endif

void BTSH_OpJQ_MOV_RegIncLdB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]++;
	cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)-1);
}
void BTSH_OpJQ_MOV_RegIncLdW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=2;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)-2);
}
void BTSH_OpJQ_MOV_RegIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)-4);
}

void BTSH_OpJQ_MOV_RegIncLdQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=8;
	BTESH2_SetRegQWord(cpu, op->rn, 
		BTESH2_GetAddrDWord(cpu,
			BTESH2_GetRegQWord(cpu, op->rm)-8));
}

void BTSH_OpJQ_MOV_RegIncLdD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu, cpu->regs[op->rm]-4);
}

void BTSH_OpJQ_MOV_RegSrIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 v;
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
//	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
//		BTESH2_GetRegQWord(cpu, op->rm)-4)&0x3F3;
	v=BTESH2_GetAddrDWord(cpu, BTESH2_GetRegQWord(cpu, op->rm)-4);
	cpu->regs[op->rn]=v;

	if(v&BTESH2_SRFL_JQ)
		{ cpu->csfl|=BTESH2_CSFL_SRJQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRJQ; }
}


void BTSH_OpJQ_MOV_RegSrIncLdQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 v;
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=8;
//	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4)&0x3F3;
	v=BTESH2_GetAddrQWord(cpu, cpu->regs[op->rm]-8);
	cpu->regs[op->rn]=v;

	if(v&BTESH2_SRFL_JQ)
		{ cpu->csfl|=BTESH2_CSFL_SRJQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRJQ; }
}

void BTSH_OpJQ_MOV_R0StDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+op->imm,
		cpu->regs[0]);
}

void BTSH_OpJQ_MOV_R0StDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+op->imm,
		cpu->regs[0]);
}

void BTSH_OpJQ_MOV_R0StDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+op->imm,
		BTESH2_GetRegQWord(cpu, 0));
}

void BTSH_OpJQ_MOV_R0LdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[0]=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}
void BTSH_OpJQ_MOV_R0LdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[0]=BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}

void BTSH_OpJQ_MOV_R0LdDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, 0, 
		BTESH2_GetAddrQWord(cpu,
			BTESH2_GetRegQWord(cpu, op->rm)+op->imm));
}

void BTSH_OpJQ_MOV_RegStDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+op->imm, cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+op->imm, cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+op->imm, cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+op->imm,
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegLdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}

void BTSH_OpJQ_MOV_RegLdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}

void BTSH_OpJQ_MOV_RegLdDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}

void BTSH_OpJQ_MOV_RegLdDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	
	BTESH2_SetRegQWord(cpu, op->rn, 
		BTESH2_GetAddrQWord(cpu,
			BTESH2_GetRegQWord(cpu, op->rm)+op->imm));
}

void BTSH_OpJQ_MOV_RegLdDispD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}


void BTSH_OpJQ_MOVU_RegLdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(byte)BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}

void BTSH_OpJQ_MOVU_RegLdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(u16)BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}

void BTSH_OpJQ_MOVU_RegLdDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(u32)BTESH2_GetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}


void BTSH_OpJQ_MOV_RegStR0nB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+BTESH2_GetRegQWord(cpu, op->ro),
		cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStR0nW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+BTESH2_GetRegQWord(cpu, op->ro),
		cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStR0nD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+BTESH2_GetRegQWord(cpu, op->ro),
		cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStR0nQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+
			BTESH2_GetRegQWord(cpu, op->ro),
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegLdR0mB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->ro));
}

void BTSH_OpJQ_MOV_RegLdR0mW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->ro));
}

void BTSH_OpJQ_MOV_RegLdR0mD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->ro));
}

void BTSH_OpJQ_MOV_RegLdR0mQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu,
			BTESH2_GetRegQWord(cpu, op->rm)+
			BTESH2_GetRegQWord(cpu, op->ro)));
}

void BTSH_OpJQ_MOV_StRegImmD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	cpu->regs[op->rn]=(sbyte)(op->imm);
//	cpu->regs[op->rn]=op->imm;
	cpu->regs[op->rm]=op->imm;

	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn], op->imm);
}

void BTSH_OpJQ_MOV_DecStRegImmD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	cpu->regs[op->rn]=(sbyte)(op->imm);
//	cpu->regs[op->rn]=op->imm;
	cpu->regs[op->rm]=op->imm;

	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn], op->imm);
}

#if 1
void BTSH_OpJQ_MOV_RegIncStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]++;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)-1,
		cpu->regs[op->rm]);
}
void BTSH_OpJQ_MOV_RegIncStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]+=2;
	BTESH2_SetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)-2,
		cpu->regs[op->rm]);
}
void BTSH_OpJQ_MOV_RegIncStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]+=4;
	BTESH2_SetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)-4,
		cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegIncStQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]+=8;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)-8,
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegIncStD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]+=4;
	BTESH2_SetAddrDWordFMMU(cpu, cpu->regs[op->rn]-4, cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegDecLdB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]--;
	cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm));
}
void BTSH_OpJQ_MOV_RegDecLdW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]-=2;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm));
}
void BTSH_OpJQ_MOV_RegDecLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]-=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegDecLdQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]-=8;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu,
			BTESH2_GetRegQWord(cpu, op->rm)));
}

void BTSH_OpJQ_MOV_RegDecLdD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]-=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu, cpu->regs[op->rm]);
}
#endif

#if 1
void BTSH_OpJQ_MOV_RegStRoDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+
		BTESH2_GetRegQWord(cpu, op->ro)+op->imm,
		cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStRoDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+
		BTESH2_GetRegQWord(cpu, op->ro)*2+op->imm,
		cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStRoDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+
		BTESH2_GetRegQWord(cpu, op->ro)*4+op->imm,
		cpu->regs[op->rm]);
}

void BTSH_OpJQ_MOV_RegStRoDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rn)+
		BTESH2_GetRegQWord(cpu, op->ro)*8+op->imm,
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_OpJQ_MOV_RegLdRoDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)+op->imm);
}

void BTSH_OpJQ_MOV_RegLdRoDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)*2+op->imm);
}

void BTSH_OpJQ_MOV_RegLdRoDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)*4+op->imm);
}

void BTSH_OpJQ_MOV_RegLdRoDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu,
			BTESH2_GetRegQWord(cpu, op->rm)+
			BTESH2_GetRegQWord(cpu, op->ro)*8+op->imm));
}

void BTSH_OpJQ_MOV_RegLdRoDispUB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(byte)BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)+op->imm);
}

void BTSH_OpJQ_MOV_RegLdRoDispUW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(u16)BTESH2_GetAddrWord(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)*2+op->imm);
}
#endif

#if 1
void BTSH_OpJQ_LEA_RegLdDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(BTESH2_GetRegQWord(cpu, op->rm)+op->imm);
}

void BTSH_OpJQ_LEA_RegLdRoDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)+op->imm);
}

void BTSH_OpJQ_LEA_RegLdRoDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)*2+op->imm);
}

void BTSH_OpJQ_LEA_RegLdRoDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)*4+op->imm);
}

void BTSH_OpJQ_LEA_RegLdRoDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(BTESH2_GetRegQWord(cpu, op->rm)+
		BTESH2_GetRegQWord(cpu, op->ro)*8+op->imm);
}
#endif

void BTSH_OpJQ_PREF_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 m, n, o, s;
	
	cpu->ptcpc=op->pc;
	n=cpu->regs[op->rn];
	
	if(!n)
		return;
	o=BTESH2_GetAddrDWord(cpu, n);
}

void BTSH_OpJQ_OCBI_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}

void BTSH_OpJQ_OCBP_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}

void BTSH_OpJQ_OCBWB_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}
