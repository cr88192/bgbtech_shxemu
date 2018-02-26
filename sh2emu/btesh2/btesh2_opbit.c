void BTSH_Op_AND_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=i&j;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_AND_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	k=i&op->imm;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_AND_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	BTESH2_SetRegDWord(cpu, op->ro, i&j);
}

void BTSH_Op_AND_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	BTESH2_SetRegDWord(cpu, op->rn, i&j);
}

void BTSH_Op_ANDQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i&j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ANDQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	k=i&j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ANDQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	k=i&j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ANDQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i&j;
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_ANDB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;

	i=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->rn));
	j=i&op->imm;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->rn), j);
}

void BTSH_Op_NOT_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=~i;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_OR_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=i|j;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_OR_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	k=i|op->imm;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_OR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=i|j;
	BTESH2_SetRegDWord(cpu, op->ro, k);
}

void BTSH_Op_OR_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	k=i|j;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_ORQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i|j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ORQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	k=i|j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ORQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	k=i|j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ORQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i|j;
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_ORB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;

	i=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->rn));
	j=i|op->imm;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->rn),
		j);
}

void BTSH_Op_TST_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=!(i&j);

	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_TST_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	k=!(i&j);

	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_TSTB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, s;

	i=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->rn));
	j=i&op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	s=(s&(~1))|(!j);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_TSTQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	u32 k, s;

	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=!(i&j);

	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_TSTQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	u32 k, s;

	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	k=!(i&j);

	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_XOR_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=i^j;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_XOR_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	k=i^op->imm;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_XOR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=i^j;
	BTESH2_SetRegDWord(cpu, op->ro, k);
}

void BTSH_Op_XOR_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	k=i^j;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_XORQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i^j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_XORQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	k=i^j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_XORQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	k=i^j;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_XORQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i^j;
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_XORB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;

	i=BTESH2_GetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->rn));
	j=i^op->imm;
	BTESH2_SetAddrByte(cpu,
		BTESH2_GetRegQWord(cpu, op->rm)+BTESH2_GetRegQWord(cpu, op->rn),
		j);
}

void BTSH_Op_ROTL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i<<1)|(i>>31);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_ROTR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>>1)|(i<<31);
	s=(s&(~1))|(i&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_ROTCL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i<<1)|(s&1);
	s=(s&(~1))|((i>>31)&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_ROTCR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>>1)|(s<<31);
	s=(s&(~1))|(i&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}


void BTSH_Op_SHAL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i<<1);
	s=(s&(~1))|((i>>31)&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SHLL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i<<1);
	s=(s&(~1))|((i>>31)&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SHAR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s32)i)>>1);
	s=(s&(~1))|(i&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SHLR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>>1);
	s=(s&(~1))|(i&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SHLL2_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, op->rn);
//	cpu->regs[op->rn]=(i<<2);
	BTESH2_SetRegDWord(cpu, op->rn, i<<2);
}

void BTSH_Op_SHLR2_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, op->rn);
//	cpu->regs[op->rn]=(i>>2);
	BTESH2_SetRegDWord(cpu, op->rn, i>>2);
}

void BTSH_Op_SHLL8_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, op->rn);
//	cpu->regs[op->rn]=(i<<8);
	BTESH2_SetRegDWord(cpu, op->rn, i<<8);
}

void BTSH_Op_SHLR8_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, op->rn);
//	cpu->regs[op->rn]=(i>>8);
	BTESH2_SetRegDWord(cpu, op->rn, i>>8);
}

void BTSH_Op_SHLL16_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, op->rn);
//	cpu->regs[op->rn]=(i<<16);
	BTESH2_SetRegDWord(cpu, op->rn, i<<16);
}

void BTSH_Op_SHLR16_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWord(cpu, op->rn);
//	cpu->regs[op->rn]=(i>>16);
	BTESH2_SetRegDWord(cpu, op->rn, i>>16);
}

#if 1
void BTSH_Op_SHLR_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;

	i=BTESH2_GetRegQWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>>1);
	s=(s&(~1))|(i&1);
	BTESH2_SetRegQWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SHLL2_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i<<2);
}

void BTSH_Op_SHLR2_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i>>2);
}

void BTSH_Op_SHLL4_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i<<4);
}

void BTSH_Op_SHLR4_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i>>4);
}

void BTSH_Op_SHLL8_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i<<8);
}

void BTSH_Op_SHLR8_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i>>8);
}

void BTSH_Op_SHLL16_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i<<16);
}

void BTSH_Op_SHLR16_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, i>>16);
}


void BTSH_Op_EXTUL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWordZx(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, (u32)i);
}

void BTSH_Op_EXTSL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=BTESH2_GetRegDWordSx(cpu, op->rn);
	BTESH2_SetRegQWord(cpu, op->rn, (s32)i);
}
#endif

void BTSH_Op_SHAD_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):
		(sh&31)?(((s32)i)>>(32-(sh&31))):(((s32)i)>>31);
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_SHLD_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):(sh&31)?(i>>(32-(sh&31))):0;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}


void BTSH_Op_SHAD_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
//	cpu->regs[op->rm]=op->imm;
	BTESH2_SetRegDWord(cpu, op->rm, op->imm);
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):
		(sh&31)?(((s32)i)>>(32-(sh&31))):(((s32)i)>>31);
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_SHLD_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
//	cpu->regs[op->rm]=op->imm;
	BTESH2_SetRegDWord(cpu, op->rm, op->imm);
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):(sh&31)?(i>>(32-(sh&31))):0;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_SHAD_RegImmP(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
//	cpu->regs[op->rm]=op->imm;
	BTESH2_SetRegDWord(cpu, op->rm, op->imm);
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	sh=(s32)j;
	k=i<<(sh&31);
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_SHLD_RegImmP(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
//	cpu->regs[op->rm]=op->imm;
	BTESH2_SetRegDWord(cpu, op->rm, op->imm);
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	sh=(s32)j;
	k=i<<(sh&31);
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

#if 1
void BTSH_Op_SHADL_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
//	u32 i, j, k;
	s64 i, j, k;
	int sh;
//	i=(s32)(cpu->regs[op->rn]);
	i=BTESH2_GetRegDWordSx(cpu, op->rn);
//	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	sh=(s32)j;
	sh=(sbyte)j;
//	k=(sh>=0)?(i<<(sh&31)):
//		(sh&31)?(((s32)i)>>(32-(sh&31))):(((s32)i)>>31);

//	k=(sh>=0)?(((s64)((s32)i))<<(sh&63)):
//		(sh&63)?(((s64)((s32)i))>>(64-(sh&63))):(((s64)((s32)i))>>63);
	k=(sh>=0)?(i<<(sh&63)):
		(sh&63)?(i>>(64-(sh&63))):(i>>63);

//	BTESH2_SetRegDWord(cpu, op->rn, k);
//	BTESH2_SetRegQWord(cpu, op->rn, (s32)k);
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_SHLDL_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
//	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):(sh&31)?(i>>(32-(sh&31))):0;
//	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_SHADQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
//	i=BTESH2_GetRegDWord(cpu, op->rn);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&63)):
		(sh&63)?(((s64)i)>>(64-(sh&63))):(((s64)i)>>63);
//	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_SHLDQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
//	i=BTESH2_GetRegDWord(cpu, op->rn);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&63)):(sh&63)?(i>>(64-(sh&63))):0;
//	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegQWord(cpu, op->rn, k);
}
#endif

void BTSH_Op_SHAD_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):
		(sh&31)?(((s32)i)>>(32-(sh&31))):(((s32)i)>>31);
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_SHLD_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):(sh&31)?(i>>(32-(sh&31))):0;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

#if 1
void BTSH_Op_SHADL_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
//	i=(s32)(cpu->regs[op->rm]);
	i=BTESH2_GetRegDWordSx(cpu, op->rm);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&63)):
		(sh&63)?(((s64)i)>>(64-(sh&63))):(((s64)i)>>63);
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_SHLDL_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
//	i=(u32)(cpu->regs[op->rm]);
	i=BTESH2_GetRegDWordZx(cpu, op->rm);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&63)):(sh&63)?(i>>(64-(sh&63))):0;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}
#endif

void BTSH_Op_SHLL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	k=i<<j;
	sh=(sbyte)j;
	k=(sh>=0)?(((u32)i)<<sh):(((u32)i)>>(-sh));
	BTESH2_SetRegDWord(cpu, op->ro, k);
}

void BTSH_Op_SHLR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	k=i>>j;
	sh=(sbyte)j;
	k=(sh>=0)?(((u32)i)>>sh):(((u32)i)<<(-sh));
	BTESH2_SetRegDWord(cpu, op->ro, k);
}

void BTSH_Op_SHAL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	sh=(sbyte)j;
	k=(sh>=0)?(((s32)i)<<sh):(((s32)i)>>(-sh));
	BTESH2_SetRegDWord(cpu, op->ro, k);
}

void BTSH_Op_SHAR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	k=((s32)i)>>j;
	sh=(sbyte)j;
	k=(sh>=0)?(((s32)i)>>sh):(((s32)i)<<(-sh));
	BTESH2_SetRegDWord(cpu, op->ro, k);
}

void BTSH_Op_SHLLQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
//	sh=j;
//	k=i<<sh;
	sh=(sbyte)j;
	k=(sh>=0)?(((u64)i)<<sh):(((u64)i)>>(-sh));
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_SHLRQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
//	sh=j;
//	k=i>>sh;
	sh=(sbyte)j;
	k=(sh>=0)?(((u64)i)>>sh):(((u64)i)<<(-sh));
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_SHALQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
//	sh=j;
//	k=i<<sh;
	sh=(sbyte)j;
	k=(sh>=0)?(((s64)i)<<sh):(((s64)i)>>(-sh));
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_SHARQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
//	sh=j;
//	k=((s64)i)>>sh;
	sh=(sbyte)j;
	k=(sh>=0)?(((s64)i)>>sh):(((s64)i)<<(-sh));
	BTESH2_SetRegQWord(cpu, op->ro, k);
}


void BTSH_Op_SHLLL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	k=i<<j;
	sh=(sbyte)j;
//	k=(sh>=0)?(((u32)i)<<sh):(((u32)i)>>(-sh));
	k=(sh>=0)?(i<<(sh&63)):
		(sh&63)?(((u64)i)>>(64-(sh&63))):(((u64)i)>>63);
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_SHLRL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	k=i>>j;
	sh=(sbyte)j;
	k=(sh>=0)?(((u32)i)>>sh):(((u32)i)<<(-sh));
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_SHALL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
//	i=(s32)(cpu->regs[op->rn]);
	i=BTESH2_GetRegDWordSx(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	sh=(sbyte)j;
//	k=(sh>=0)?(((s64)i)<<sh):(((s64)i)>>(-sh));
	k=(sh>=0)?(i<<(sh&63)):
		(sh&63)?(((s64)i)>>(64-(sh&63))):(((s64)i)>>63);
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_SHARL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
//	i=(s32)(cpu->regs[op->rn]);
	i=BTESH2_GetRegDWordSx(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	k=((s32)i)>>j;
	sh=(sbyte)j;
	k=(sh>=0)?(((s32)i)>>sh):(((s32)i)<<(-sh));
	BTESH2_SetRegQWord(cpu, op->ro, k);
}


void BTSH_Op_SHADQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&63)):
		(sh&63)?(((s64)i)>>(64-(sh&63))):(((s64)i)>>63);
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_SHLDQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&63)):(sh&63)?(i>>(64-(sh&63))):0;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_LDSH16_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=BTESH2_GetRegQWord(cpu, op->rn);
//	k=(i<<16)+(op->imm);
	k=(i<<16)+((s32)op->imm);
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_CSELT_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	k=(cpu->regs[BTESH2_REG_SR]&1)?i:j;
	k=(BTESH2_GetRegDWord(cpu, BTESH2_REG_SR)&1)?i:j;
	BTESH2_SetRegDWord(cpu, op->ro, k);
}

void BTSH_Op_CSELT_RegRegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;

	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
//	k=(cpu->regs[BTESH2_REG_SR]&1)?i:j;
	k=(BTESH2_GetRegDWord(cpu, BTESH2_REG_SR)&1)?i:j;
	BTESH2_SetRegQWord(cpu, op->ro, k);

}
