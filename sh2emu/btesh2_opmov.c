
void BTSH_Op_MOV_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	cpu->regs[op->rn]=(sbyte)(op->imm);
	cpu->regs[op->rn]=op->imm;
}

void BTSH_Op_MOVA_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=op->imm;
}

void BTSH_Op_MOV_RegLdAbsW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, op->imm);
}

void BTSH_Op_MOV_RegLdAbsD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, op->imm);
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
	{ cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]); }
void BTSH_Op_MOV_RegMemW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]); }
void BTSH_Op_MOV_RegMemD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]); }


void BTSH_Op_MOV_RegStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ BTESH2_SetAddrByte(cpu, cpu->regs[op->rn], cpu->regs[op->rm]); }
void BTSH_Op_MOV_RegStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ BTESH2_SetAddrWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]); }
void BTSH_Op_MOV_RegStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]); }

void BTSH_Op_MOV_RegDecStB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]--;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegDecStW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]-=2;
	BTESH2_SetAddrWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}
void BTSH_Op_MOV_RegDecStD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegIncLdB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rm]++;
	cpu->regs[op->rn]=(sbyte)BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]-1);
}
void BTSH_Op_MOV_RegIncLdW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rm]+=2;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]-2);
}
void BTSH_Op_MOV_RegIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rm]+=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
}

void BTSH_Op_MOV_RegSrIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rm]+=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4)&0x3F3;
}


void BTSH_Op_MOV_R0StDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ BTESH2_SetAddrByte(cpu, cpu->regs[op->rn]+op->imm, cpu->regs[0]); }
void BTSH_Op_MOV_R0StDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ BTESH2_SetAddrWord(cpu, cpu->regs[op->rn]+op->imm, cpu->regs[0]); }

void BTSH_Op_MOV_R0LdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]+op->imm); }
void BTSH_Op_MOV_R0LdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]+op->imm); }

void BTSH_Op_MOV_RegStDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetAddrByte(cpu,
		cpu->regs[op->rn]+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetAddrWord(cpu,
		cpu->regs[op->rn]+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+op->imm, cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegLdDispB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOV_RegLdDispW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOV_RegLdDispD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_MOV_RegStR0nB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetAddrByte(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStR0nW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetAddrWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegStR0nD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro], cpu->regs[op->rm]);
}

void BTSH_Op_MOV_RegLdR0mB(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=BTESH2_GetAddrByte(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]);
}

void BTSH_Op_MOV_RegLdR0mW(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=BTESH2_GetAddrWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]);
}

void BTSH_Op_MOV_RegLdR0mD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]);
}

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
