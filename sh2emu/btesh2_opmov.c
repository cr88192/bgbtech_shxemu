
void BTSH_Op_MOV_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	cpu->regs[op->rn]=(sbyte)(op->imm);
	cpu->regs[op->rn]=op->imm;
}

void BTSH_Op_MOVA_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=op->imm;
}

void BTSH_Op_MOV_RegShlImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(cpu->regs[op->rn]<<op->ro)|op->imm;
}

void BTSH_Op_MOV_RegLdAbsW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, op->imm);
}

void BTSH_Op_MOV_RegLdAbsD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, op->imm);
}

void BTSH_Op_MOV_RegLdAbsQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu, op->imm));
}

void BTSH_Op_MOVU_RegLdAbsW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(u16)BTESH2_GetAddrWord(cpu, op->imm);
}

void BTSH_Op_MOVU_RegLdAbsWQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		(u16)BTESH2_GetAddrWord(cpu, op->imm));
}

void BTSH_Op_MOV_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[op->rn]=cpu->regs[op->rm]; }

void BTSH_Op_MOV_RegRegSr(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[op->rn]=cpu->regs[op->rm]&0x3F3; }

void BTSH_Op_MOV_RegRegSrB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(cpu->regs[op->rm]&0x3F3)|
		(cpu->regs[op->rn]&(~0x3F3));
}

void BTSH_Op_MOV_RegMemB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegMemW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegMemD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegMemD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu, cpu->regs[op->rm]);
}


void BTSH_Op_MOV_RegStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWordFMMU(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}


void BTSH_Op_MOV_RegDecStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]--;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegDecStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=2;
	BTESH2_SetAddrWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegDecStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegDecStD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWordFMMU(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}


#if 1
void BTSH_Op_MOV_ImmStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn], op->imm);
}

void BTSH_Op_MOV_ImmStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu, cpu->regs[op->rn], op->imm);
}

void BTSH_Op_MOV_ImmStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], op->imm);
}

void BTSH_Op_MOV_ImmStQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu, cpu->regs[op->rn], (u64)((s32)op->imm));
}

void BTSH_Op_MOV_ImmDecStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]--;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn], op->imm);
}
void BTSH_Op_MOV_ImmDecStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=2;
	BTESH2_SetAddrWord(cpu, cpu->regs[op->rn], op->imm);
}
void BTSH_Op_MOV_ImmDecStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], op->imm);
}
void BTSH_Op_MOV_ImmDecStQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=8;
	BTESH2_SetAddrQWord(cpu, cpu->regs[op->rn], (u64)((s32)op->imm));
}
#endif

void BTSH_Op_MOV_RegIncLdB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]++;
	cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]-1);
}
void BTSH_Op_MOV_RegIncLdW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=2;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]-2);
}
void BTSH_Op_MOV_RegIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
}

void BTSH_Op_MOV_RegIncLdD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu, cpu->regs[op->rm]-4);
}

void BTSH_Op_MOV_RegSrIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 v;
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
//	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4)&0x3F3;
	v=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
	cpu->regs[op->rn]=v;

	if(v&BTESH2_SRFL_JQ)
		{ cpu->csfl|=BTESH2_CSFL_SRJQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRJQ; }

	if(v&BTESH2_SRFL_DQ)
		{ cpu->csfl|=BTESH2_CSFL_SRDQ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_SRDQ; }
}


void BTSH_Op_MOV_R0StDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn]+op->imm, cpu->regs[0]);
}

void BTSH_Op_MOV_R0StDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu, cpu->regs[op->rn]+op->imm, cpu->regs[0]);
}

void BTSH_Op_MOV_R0LdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[0]=BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]+op->imm);
}
void BTSH_Op_MOV_R0LdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[0]=BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOV_RegStDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu,
		cpu->regs[op->rn]+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu,
		cpu->regs[op->rn]+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		cpu->regs[op->rn]+op->imm,
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_Op_MOV_RegLdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOV_RegLdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOV_RegLdDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOV_RegLdDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	
	BTESH2_SetRegQWord(cpu, op->rn, 
		BTESH2_GetAddrQWord(cpu, cpu->regs[op->rm]+op->imm));
}

void BTSH_Op_MOV_RegLdDispD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu,
		cpu->regs[op->rm]+op->imm);
}


void BTSH_Op_MOVU_RegLdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(byte)BTESH2_GetAddrByte(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOVU_RegLdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(u16)BTESH2_GetAddrWord(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOVU_RegLdDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(u32)BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+op->imm);
}


void BTSH_Op_MOV_RegStR0nB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStR0nW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStR0nD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegLdR0mB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]);
}

void BTSH_Op_MOV_RegLdR0mW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]);
}

void BTSH_Op_MOV_RegLdR0mD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]);
}


void BTSH_Op_MOV_StRegImmD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	cpu->regs[op->rn]=(sbyte)(op->imm);
//	cpu->regs[op->rn]=op->imm;
	cpu->regs[op->rm]=op->imm;

	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn], op->imm);
}

void BTSH_Op_MOV_DecStRegImmD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
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
void BTSH_Op_MOV_RegIncStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]++;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn]-1, cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegIncStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]+=2;
	BTESH2_SetAddrWord(cpu, cpu->regs[op->rn]-2, cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegIncStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]+=4;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]-4, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegIncStD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]+=4;
	BTESH2_SetAddrDWordFMMU(cpu, cpu->regs[op->rn]-4, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegDecLdB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]--;
	cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegDecLdW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]-=2;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegDecLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]-=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegDecLdD_FMMU(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]-=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWordFMMU(cpu, cpu->regs[op->rm]);
}
#endif

#if 1
void BTSH_Op_MOV_RegStRoDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrByte(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro]+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStRoDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro]*2+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStRoDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro]*4+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStRoDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrQWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro]*8+op->imm,
		BTESH2_GetRegQWord(cpu, op->rm));
}

void BTSH_Op_MOV_RegLdRoDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]+op->imm);
}

void BTSH_Op_MOV_RegLdRoDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]*2+op->imm);
}

void BTSH_Op_MOV_RegLdRoDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]*4+op->imm);
}

void BTSH_Op_MOV_RegLdRoDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu,
			cpu->regs[op->rm]+cpu->regs[op->ro]*8+op->imm));
}
#endif

#if 1
void BTSH_Op_LEA_RegLdDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_LEA_RegLdRoDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(cpu->regs[op->rm]+cpu->regs[op->ro]+op->imm);
}

void BTSH_Op_LEA_RegLdRoDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(cpu->regs[op->rm]+cpu->regs[op->ro]*2+op->imm);
}

void BTSH_Op_LEA_RegLdRoDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(cpu->regs[op->rm]+cpu->regs[op->ro]*4+op->imm);
}

void BTSH_Op_LEA_RegLdRoDispQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(cpu->regs[op->rm]+cpu->regs[op->ro]*8+op->imm);
}
#endif

void BTSH_Op_SWAP_RegRegB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rm];
	i=((i>>8)&255)|((i<<8)&0x0000FF00)|(i&0xFFFF0000);
	cpu->regs[op->rn]=i;
}

void BTSH_Op_SWAP_RegRegW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rm];
	i=((i>>16)&0x0000FFFFU)|((i<<16)&0xFFFF0000U);
	cpu->regs[op->rn]=i;
}

void BTSH_Op_CASL_RegRegR0D(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 m, n, o, s;
	
	cpu->ptcpc=op->pc;
	s=cpu->regs[BTESH2_REG_SR];
	m=cpu->regs[op->rm];
	n=cpu->regs[op->rn];
	o=BTESH2_GetAddrDWord(cpu, cpu->regs[op->ro]);

	if(m!=o)
	{
		cpu->regs[op->rn]=o;
		s=s&(~1);
	}else
	{
		BTESH2_SetAddrDWord(cpu, cpu->regs[op->ro], n);
		cpu->regs[op->rn]=o;
		s=s|1;
	}

	cpu->regs[BTESH2_REG_SR]=s;
}


void BTSH_Op_PREF_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 m, n, o, s;
	
	cpu->ptcpc=op->pc;
	n=cpu->regs[op->rn];
	
	if(!n)
		return;
	o=BTESH2_GetAddrDWord(cpu, n);
}

void BTSH_Op_OCBI_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}

void BTSH_Op_OCBP_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}

void BTSH_Op_OCBWB_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
}
