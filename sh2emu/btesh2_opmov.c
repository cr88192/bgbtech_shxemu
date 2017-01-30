
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
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=(s16)BTESH2_GetAddrWord(cpu, op->imm);
}

void BTSH_Op_MOV_RegLdAbsD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, op->imm);
}


void BTSH_Op_MOV_RegReg_R0R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=cpu->regs[1]; }
void BTSH_Op_MOV_RegReg_R0R2(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=cpu->regs[2]; }
void BTSH_Op_MOV_RegReg_R0R3(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=cpu->regs[3]; }
void BTSH_Op_MOV_RegReg_R0R4(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=cpu->regs[4]; }
void BTSH_Op_MOV_RegReg_R0R5(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=cpu->regs[5]; }
void BTSH_Op_MOV_RegReg_R0R6(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=cpu->regs[6]; }
void BTSH_Op_MOV_RegReg_R0R7(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[0]=cpu->regs[7]; }

void BTSH_Op_MOV_RegReg_R1R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[1]=cpu->regs[0]; }
void BTSH_Op_MOV_RegReg_R1R2(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[1]=cpu->regs[2]; }
void BTSH_Op_MOV_RegReg_R1R3(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[1]=cpu->regs[3]; }
void BTSH_Op_MOV_RegReg_R1R4(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[1]=cpu->regs[4]; }
void BTSH_Op_MOV_RegReg_R1R5(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[1]=cpu->regs[5]; }
void BTSH_Op_MOV_RegReg_R1R6(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[1]=cpu->regs[6]; }
void BTSH_Op_MOV_RegReg_R1R7(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[1]=cpu->regs[7]; }

void BTSH_Op_MOV_RegReg_R2R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[2]=cpu->regs[0]; }
void BTSH_Op_MOV_RegReg_R2R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[2]=cpu->regs[1]; }
void BTSH_Op_MOV_RegReg_R2R3(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[2]=cpu->regs[3]; }

void BTSH_Op_MOV_RegReg_R3R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[3]=cpu->regs[0]; }
void BTSH_Op_MOV_RegReg_R3R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[3]=cpu->regs[1]; }
void BTSH_Op_MOV_RegReg_R3R2(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[3]=cpu->regs[2]; }

void BTSH_Op_MOV_RegReg_R4R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[4]=cpu->regs[0]; }
void BTSH_Op_MOV_RegReg_R4R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[4]=cpu->regs[1]; }

void BTSH_Op_MOV_RegReg_R5R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[5]=cpu->regs[0]; }
void BTSH_Op_MOV_RegReg_R5R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[5]=cpu->regs[1]; }

void BTSH_Op_MOV_RegReg_R6R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[6]=cpu->regs[0]; }
void BTSH_Op_MOV_RegReg_R6R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[6]=cpu->regs[1]; }

void BTSH_Op_MOV_RegReg_R7R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[7]=cpu->regs[0]; }
void BTSH_Op_MOV_RegReg_R7R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[7]=cpu->regs[1]; }

void BTSH_Fixup_MOV_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	switch(op->rn)
	{
	case 0:
		switch(op->rm)
		{
		case 1: op->Run=BTSH_Op_MOV_RegReg_R0R1; break;
		case 2: op->Run=BTSH_Op_MOV_RegReg_R0R2; break;
		case 3: op->Run=BTSH_Op_MOV_RegReg_R0R3; break;
		case 4: op->Run=BTSH_Op_MOV_RegReg_R0R4; break;
		case 5: op->Run=BTSH_Op_MOV_RegReg_R0R5; break;
		case 6: op->Run=BTSH_Op_MOV_RegReg_R0R6; break;
		case 7: op->Run=BTSH_Op_MOV_RegReg_R0R7; break;
		default: break;
		}
		break;
	case 1:
		switch(op->rm)
		{
		case 0: op->Run=BTSH_Op_MOV_RegReg_R1R0; break;
		case 2: op->Run=BTSH_Op_MOV_RegReg_R1R2; break;
		case 3: op->Run=BTSH_Op_MOV_RegReg_R1R3; break;
		case 4: op->Run=BTSH_Op_MOV_RegReg_R1R4; break;
		case 5: op->Run=BTSH_Op_MOV_RegReg_R1R5; break;
		case 6: op->Run=BTSH_Op_MOV_RegReg_R1R6; break;
		case 7: op->Run=BTSH_Op_MOV_RegReg_R1R7; break;
		default: break;
		}
		break;
	case 2:
		switch(op->rm)
		{
		case 0: op->Run=BTSH_Op_MOV_RegReg_R2R0; break;
		case 1: op->Run=BTSH_Op_MOV_RegReg_R2R1; break;
		case 3: op->Run=BTSH_Op_MOV_RegReg_R2R3; break;
		default: break;
		}
		break;
	case 3:
		switch(op->rm)
		{
		case 0: op->Run=BTSH_Op_MOV_RegReg_R3R0; break;
		case 1: op->Run=BTSH_Op_MOV_RegReg_R3R1; break;
		case 2: op->Run=BTSH_Op_MOV_RegReg_R3R2; break;
		default: break;
		}
		break;

	case 4:
		switch(op->rm)
		{
		case 0: op->Run=BTSH_Op_MOV_RegReg_R4R0; break;
		case 1: op->Run=BTSH_Op_MOV_RegReg_R4R1; break;
		default: break;
		}
		break;
	case 5:
		switch(op->rm)
		{
		case 0: op->Run=BTSH_Op_MOV_RegReg_R5R0; break;
		case 1: op->Run=BTSH_Op_MOV_RegReg_R5R1; break;
		default: break;
		}
		break;
	case 6:
		switch(op->rm)
		{
		case 0: op->Run=BTSH_Op_MOV_RegReg_R6R0; break;
		case 1: op->Run=BTSH_Op_MOV_RegReg_R6R1; break;
		default: break;
		}
		break;
	case 7:
		switch(op->rm)
		{
		case 0: op->Run=BTSH_Op_MOV_RegReg_R7R0; break;
		case 1: op->Run=BTSH_Op_MOV_RegReg_R7R1; break;
		default: break;
		}
		break;

	default:
		break;
	}
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

void BTSH_Op_MOV_RegSrIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
	cpu->regs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4)&0x3F3;
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
