void BTSH_Op_MOVT_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[BTESH2_REG_SR];
	cpu->regs[op->rn]=i&1;
}

void BTSH_Op_ADD_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->rn]=i+j;
}

void BTSH_Op_ADD_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	cpu->regs[op->rn]=i+op->imm;
}

void BTSH_Op_ADDC_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=((u64)i)+j+(s&1);
	cpu->regs[op->rn]=((u32)k);
	s=(s&(~1))|((k>>32)&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_ADDV_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	u32 i, j, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=((s64)((s32)i))+((s32)j);
	cpu->regs[op->rn]=((u32)k);
//	s=(s&(~1))|((k>>32)&1);
	s=(s&(~1))|((k!=((s32)k))&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPEQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	j=op->imm;
	s=cpu->regs[BTESH2_REG_SR];
	k=(i==j);
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPEQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i==j);
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPHS_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i>=j);
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPGE_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=(((s32)i)>=((s32)j));
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPHI_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=(i>j);
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPGT_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=(((s32)i)>((s32)j));
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPPL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(((s32)i)>0);
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPPZ_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=(((s32)i)>=0);
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_CMPSTR_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=	(((byte)(i    ))==((byte)(j    )))|
		(((byte)(i>> 8))==((byte)(j>> 8)))|
		(((byte)(i>>16))==((byte)(j>>16)))|
		(((byte)(i>>24))==((byte)(j>>24)));
	s=(s&(~1))|(k&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_DIV1_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	byte q, q0, m, t;
	u32 s;
	s64 i1, i2;
//	u32 i, j, s;
	
	s=cpu->regs[BTESH2_REG_SR];
	m=(s>>9)&1;
	q=(s>>8)&1;
	t=s&1;
	
//	printf("DIV1\n");
	i1=((u32)cpu->regs[op->rm]);
	i2=((u32)cpu->regs[op->rn]);
	q0=q;
	q=(i2>>31)&1;
	i2=((u32)(i2<<1))|t;
	if(q0==m)
		{ i2=i2-i1; }
	else
		{ i2=i2+i1; }
	q=(q^m)^((i2>>32)&1);
	t=1-(q^m);
	s=(s&0x0F2)|(m<<9)|(q<<8)|t;
	cpu->regs[op->rn]=i2;
	cpu->regs[BTESH2_REG_SR]=s;
//	cpu->regs[op->rn]=i/j;
}

void BTSH_Op_DIV0S_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, s;
//	printf("DIV0S\n");
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
//	cpu->regs[op->rn]=i/j;
	
	s=cpu->regs[op->rn];
	s=(s&0x0F2)|((j>>31)<<9)|((i>>31)<<8)|((i^j)>>31);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_DIV0U_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, s;
//	printf("DIV0U\n");
//	i=cpu->regs[op->rn];
//	j=cpu->regs[op->rm];
//	cpu->regs[op->rn]=i/j;

	s=cpu->regs[BTESH2_REG_SR];
	s=s&0x0F2;
	cpu->regs[BTESH2_REG_SR]=s;
}


void BTSH_Op_DMULS_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	s32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=((s64)i)*j;
	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
	cpu->regs[BTESH2_REG_MACL]=(u32)k;
}

void BTSH_Op_DMULU_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=((u64)i)*j;
	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
	cpu->regs[BTESH2_REG_MACL]=(u32)k;
}

void BTSH_Op_DT_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k, s;
	i=cpu->regs[op->rn];
	s=cpu->regs[BTESH2_REG_SR];
	k=i-1;
	s=(s&(~1))|((k==0)&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}


void BTSH_Op_EXTSB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=cpu->regs[op->rm];
	k=(sbyte)i;
	cpu->regs[op->rn]=k;
}

void BTSH_Op_EXTSW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=cpu->regs[op->rm];
	k=(s16)i;
	cpu->regs[op->rn]=k;
}

void BTSH_Op_EXTUB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=cpu->regs[op->rm];
	k=(byte)i;
	cpu->regs[op->rn]=k;
}

void BTSH_Op_EXTUW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=cpu->regs[op->rm];
	k=(u16)i;
	cpu->regs[op->rn]=k;
}

void BTSH_Op_MACL_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 s;
	s64 k, l;
	s32 i, j;
	int i1;

//	i=cpu->regs[op->rn];
//	j=cpu->regs[op->rm];

	i=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rn]);
	cpu->regs[op->rn]+=4;
	j=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]);
	cpu->regs[op->rm]+=4;

	l=(((s64)cpu->regs[BTESH2_REG_MACH])<<32)|
		cpu->regs[BTESH2_REG_MACL];
	k=((s64)i)*j+l;
	
	s=cpu->regs[BTESH2_REG_SR];
	if(s&2)
	{
		i1=k>>47;
		if(i1>( 0)) k=( 1LL<<47)-1;
		if(i1<(-1)) k=(-1LL<<47);
	}
	
	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
	cpu->regs[BTESH2_REG_MACL]=(u32)k;
}

void BTSH_Op_MACW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k, l, t;
	u32 s;
	s32 i, j;
	int i1;

	i=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rn]);
	cpu->regs[op->rn]+=2;
	j=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]);
	cpu->regs[op->rm]+=2;

	s=cpu->regs[BTESH2_REG_SR];
	if(s&2)
	{
		l=((s32)(cpu->regs[BTESH2_REG_MACL]));
		k=(s32)(i*j)+l;
		
		t=k; i1=(k>>31);
		if(i1>( 0))t=0x7FFFFFFFULL;
		if(i1<(-1))t=0x80000000ULL;
		
		if(k==t)
		{
			k=(((u64)cpu->regs[BTESH2_REG_MACH])<<32)|
				((u32)k);
		}else
		{
			k=(((s64)cpu->regs[BTESH2_REG_MACH])<<32)|
				(1LL<<32)|((u32)t);
//			k=(1LL<<32)|((u32)t);
		}
	}else
	{
		l=(((s64)cpu->regs[BTESH2_REG_MACH])<<32)|
			cpu->regs[BTESH2_REG_MACL];
		k=((s64)i)*j+l;
	}

	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
	cpu->regs[BTESH2_REG_MACL]=(u32)k;
}

void BTSH_Op_MULL_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s32 i, j, k;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=i*j;
//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
	cpu->regs[BTESH2_REG_MACL]=(u32)k;
}

void BTSH_Op_MULSW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s32 i, j, k;
	i=(s16)(cpu->regs[op->rn]);
	j=(s16)(cpu->regs[op->rm]);
	k=i*j;
	cpu->regs[BTESH2_REG_MACL]=(u32)k;
}

void BTSH_Op_MULUW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s32 i, j, k;
	i=(u16)(cpu->regs[op->rn]);
	j=(u16)(cpu->regs[op->rm]);
	k=i*j;
	cpu->regs[BTESH2_REG_MACL]=(u32)k;
}

void BTSH_Op_NEG_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=cpu->regs[op->rm];
	k=0-i;
	cpu->regs[op->rn]=k;
}

void BTSH_Op_NEGC_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	s32 i, s;

	s=cpu->regs[BTESH2_REG_SR];

	i=cpu->regs[op->rm];
	k=0-((s64)i)-(s&1);
//	k=0-(~i)+1;
	cpu->regs[op->rn]=(u32)k;

	s=(s&(~1))|((k>>32)&1);
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_SUB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	cpu->regs[op->rn]=i-j;
}

void BTSH_Op_SUBC_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	s32 i, j, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=((s64)i)-j-(s&1);
	s=(s&(~1))|((k>>32)&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_SUBV_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	u32 i, j, s;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	s=cpu->regs[BTESH2_REG_SR];
	k=((s64)((s32)i))-((s32)j);
	s=(s&(~1))|((k!=((s32)k))&1);
//	s=(s&(~1))|((k>>32)&1);
	cpu->regs[op->rn]=k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_TASB_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, s;

	s=cpu->regs[BTESH2_REG_SR];
	i=BTESH2_GetAddrByte(cpu, cpu->regs[op->rn]);
	BTESH2_SetAddrByte(cpu, cpu->regs[op->rn], i|0x80);
	s=(s&(~1))|(i==0);
	cpu->regs[BTESH2_REG_SR]=s;

//	i=cpu->regs[op->rn];
//	j=cpu->regs[op->rm];
//	cpu->regs[op->rn]=i-j;
}

void BTSH_Op_XTRCT_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->regs[op->rn];
	j=cpu->regs[op->rm];
	k=(i>>16)|(j<<16);
	cpu->regs[op->rn]=k;
}
