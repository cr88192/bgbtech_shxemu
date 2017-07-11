void BTSH_Op_AND_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->rn]=i&j;
}

void BTSH_Op_AND_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=i&op->imm;
}

void BTSH_Op_AND_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->ro]=i&j;
}

void BTSH_Op_AND_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rm];
	j=op->imm;
	cpu->regs[op->rn]=i&j;
}

void BTSH_Op_ANDQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	k=i&j;
	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_ANDQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=op->imm;
	k=i&j;
	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_ANDQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	k=i&j;
	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_ANDB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;

	i=BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]+cpu->regs[op->rn]);
	j=i&op->imm;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rm]+cpu->regs[op->rn], j);
}

void BTSH_Op_NOT_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rm];
	cpu->regs[op->rn]=~i;
}

void BTSH_Op_OR_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->rn]=i|j;
}

void BTSH_Op_OR_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=i|op->imm;
}

void BTSH_Op_OR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->ro]=i|j;
}

void BTSH_Op_OR_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rm];
	j=op->imm;
	cpu->regs[op->rn]=i|j;
}

void BTSH_Op_ORQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	k=i|j;
	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_ORQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=op->imm;
	k=i|j;
	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_ORQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	k=i|j;
	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_ORB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;

	i=BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]+cpu->regs[op->rn]);
	j=i|op->imm;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rm]+cpu->regs[op->rn], j);
}

void BTSH_Op_TST_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=!(i&j);

	s=cpu->regs[BTESH2_REG_SR];
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_TST_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	j=op->imm;
	k=!(i&j);

	s=cpu->regs[BTESH2_REG_SR];
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_TSTB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, s;

	i=BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]+cpu->regs[op->rn]);
	j=i&op->imm;
	s=cpu->regs[BTESH2_REG_SR];
	s=(s&(~1))|(!j);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_TSTQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	u32 k, s;

	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	k=!(i&j);

	s=cpu->regs[BTESH2_REG_SR];
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_TSTQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	u32 k, s;

	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=op->imm;
	k=!(i&j);

	s=cpu->regs[BTESH2_REG_SR];
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_XOR_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->rn]=i^j;
}

void BTSH_Op_XOR_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=i^op->imm;
}

void BTSH_Op_XOR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->ro]=i^j;
}

void BTSH_Op_XOR_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rm];
	j=op->imm;
	cpu->regs[op->rn]=i^j;
}

void BTSH_Op_XORQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	k=i^j;
	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_XORQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=op->imm;
	k=i^j;
	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_XORQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	k=i^j;
	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_XORB_MemImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;

	i=BTESH2_GetAddrByte(cpu, cpu->regs[op->rm]+cpu->regs[op->rn]);
	j=i^op->imm;
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rm]+cpu->regs[op->rn], j);
}

void BTSH_Op_ROTL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i<<1)|(i>>31);
	s=(s&(~1))|(k&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_ROTR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i>>1)|(i<<31);
	s=(s&(~1))|(i&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_ROTCL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i<<1)|(s&1);
	s=(s&(~1))|((i>>31)&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_ROTCR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i>>1)|(s<<31);
	s=(s&(~1))|(i&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}


void BTSH_Op_SHAL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i<<1);
	s=(s&(~1))|((i>>31)&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_SHLL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i<<1);
	s=(s&(~1))|((i>>31)&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_SHAR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(((s32)i)>>1);
	s=(s&(~1))|(i&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_SHLR_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;

	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i>>1);
	s=(s&(~1))|(i&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}


void BTSH_Op_SHLL2_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=(i<<2);
}

void BTSH_Op_SHLR2_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=(i>>2);
}

void BTSH_Op_SHLL8_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=(i<<8);
}

void BTSH_Op_SHLR8_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=(i>>8);
}

void BTSH_Op_SHLL16_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=(i<<16);
}

void BTSH_Op_SHLR16_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=(i>>16);
}

void BTSH_Op_SHAD_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):
		(sh&31)?(((s32)i)>>(32-(sh&31))):(((s32)i)>>31);
	cpu->regs[op->rn]=k;
}

void BTSH_Op_SHLD_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):(sh&31)?(i>>(32-(sh&31))):0;
	cpu->regs[op->rn]=k;
}


void BTSH_Op_SHAD_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	cpu->regs[op->rm]=op->imm;
	i=cpu->regs[op->rn];
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):
		(sh&31)?(((s32)i)>>(32-(sh&31))):(((s32)i)>>31);
	cpu->regs[op->rn]=k;
}

void BTSH_Op_SHLD_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	cpu->regs[op->rm]=op->imm;
	i=cpu->regs[op->rn];
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):(sh&31)?(i>>(32-(sh&31))):0;
	cpu->regs[op->rn]=k;
}

void BTSH_Op_SHAD_RegImmP(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	cpu->regs[op->rm]=op->imm;
	i=cpu->regs[op->rn];
	j=op->imm;
	sh=(s32)j;
	k=i<<(sh&31);
	cpu->regs[op->rn]=k;
}

void BTSH_Op_SHLD_RegImmP(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	cpu->regs[op->rm]=op->imm;
	i=cpu->regs[op->rn];
	j=op->imm;
	sh=(s32)j;
	k=i<<(sh&31);
	cpu->regs[op->rn]=k;
}


void BTSH_Op_SHAD_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=cpu->regs[op->rm];
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):
		(sh&31)?(((s32)i)>>(32-(sh&31))):(((s32)i)>>31);
	cpu->regs[op->rn]=k;
}

void BTSH_Op_SHLD_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=cpu->regs[op->rm];
	j=op->imm;
	sh=(s32)j;
	k=(sh>=0)?(i<<(sh&31)):(sh&31)?(i>>(32-(sh&31))):0;
	cpu->regs[op->rn]=k;
}


void BTSH_Op_SHLL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=i<<j;
	cpu->regs[op->ro]=k;
}

void BTSH_Op_SHLR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=i>>j;
	cpu->regs[op->ro]=k;
}

void BTSH_Op_SHAR_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	int sh;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=((s32)i)>>j;
	cpu->regs[op->ro]=k;
}

void BTSH_Op_SHLLQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	sh=j;
	k=i<<sh;
	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_SHLRQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	sh=j;
	k=i>>sh;
	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_SHARQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	sh=j;
	k=((s64)i)>>sh;
	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
}

void BTSH_Op_LDSH16_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	int sh;
	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	k=(i<<16)+(op->imm);
	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
}
