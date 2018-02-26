void BTSH_Op_MOVT_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
//	cpu->regs[op->rn]=i&1;
	BTESH2_SetRegDWord(cpu, op->rn, i&1);
}

void BTSH_Op_ADD_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	BTESH2_SetRegDWord(cpu, op->rn, i+j);
}

void BTSH_Op_ADD_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	BTESH2_SetRegDWord(cpu, op->rn, i+op->imm);
}

void BTSH_Op_ADD_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	BTESH2_SetRegDWord(cpu, op->ro, i+j);
}

void BTSH_Op_ADDC_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=((u64)i)+j+(s&1);
//	cpu->regs[op->rn]=((u32)k);
	BTESH2_SetRegDWordZx(cpu, op->rn, k);
	s=(s&(~1))|((k>>32)&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_ADDV_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	u32 i, j, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=((s64)((s32)i))+((s32)j);
//	cpu->regs[op->rn]=((u32)k);
	BTESH2_SetRegDWordZx(cpu, op->rn, k);
//	s=(s&(~1))|((k>>32)&1);
	s=(s&(~1))|((k!=((s32)k))&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPEQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i==j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHS_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>=j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGE_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s32)i)>=((s32)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHI_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGT_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s32)i)>((s32)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}


void BTSH_Op_CMPEQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i==j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHS_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>=j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGE_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s32)i)>=((s32)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHI_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGT_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s32)i)>((s32)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPPL_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s32)i)>0);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPPZ_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s32)i)>=0);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}


#if 1
void BTSH_Op_CMPEQ_RegImmQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i==j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHS_RegImmQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>=j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGE_RegImmQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s64)i)>=((s64)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHI_RegImmQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGT_RegImmQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s64)i)>((s64)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPEQ_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i==j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHS_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>=j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGE_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s64)i)>=((s64)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPHI_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(i>j);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPGT_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s64)i)>((s64)j));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPPL_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
//	i=BTESH2_GetRegDWord(cpu, op->rn);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s64)i)>0);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_CMPPZ_RegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k, s;
//	i=BTESH2_GetRegDWord(cpu, op->rn);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=(((s64)i)>=0);
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}
#endif


void BTSH_Op_CMPSTR_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=	(((byte)(i    ))==((byte)(j    )))|
		(((byte)(i>> 8))==((byte)(j>> 8)))|
		(((byte)(i>>16))==((byte)(j>>16)))|
		(((byte)(i>>24))==((byte)(j>>24)));
	s=(s&(~1))|(k&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_DIV1_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	byte q, q0, m, t;
	u32 s;
	s64 i1, i2;
//	u32 i, j, s;
	
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	m=(s>>9)&1;
	q=(s>>8)&1;
	t=s&1;
	
//	printf("DIV1\n");
	i1=BTESH2_GetRegDWordZx(cpu, op->rm);
	i2=BTESH2_GetRegDWordZx(cpu, op->rn);

#if 0
	q0=q;
//	q=(i2>>31)&1;
//	i2=((u32)(i2<<1))|t;
	i2=(i2<<1)|t;
//	i1^=((q0==m)-1);
//	if(q0==m)i1=-i1;
//	i1=(i1^((q0!=m)-1))+(q0==m);
	i1=(i1^((q0^m)-1))+(1-(q0^m));
	i2=i2+i1;
//	if(q0==m)
//		{ i2=i2-i1; }
//	else
//		{ i2=i2+i1; }
//	q=(q^m)^((i2>>32)&1);
	q=m^((i2>>32)&1);
	t=1-(q^m);
#endif

#if 1
	if(m)
	{
		i2=(i2<<1)|t;
//		i1=(i1^(-q))+q;
		if(q)i1=-i1;
		i2=i2+i1;
		q=1-((i2>>32)&1);
		t=q;
	}else
	{
		i2=(i2<<1)|t;
//		i1=(i1^(q-1))+(1-q);
		if(!q)i1=-i1;
		i2=i2+i1;
		q=(i2>>32)&1;
		t=1-q;
	}
#endif

//	s=(s&0xFFFFF0F2)|(m<<9)|(q<<8)|t;
	s=(s&0xFFFFF2F2)|(q<<8)|t;
	BTESH2_SetRegDWordZx(cpu, op->rn, i2);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_DIV0S_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, s;
//	printf("DIV0S\n");
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
//	cpu->regs[op->rn]=i/j;
	
	s=BTESH2_GetRegDWord(cpu, op->rn);
//	s=(s&0x0F2)|((j>>31)<<9)|((i>>31)<<8)|((i^j)>>31);
	s=(s&0xFFFFF0F2)|((j>>31)<<9)|((i>>31)<<8)|((i^j)>>31);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_DIV0U_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, s;
//	printf("DIV0U\n");
//	i=BTESH2_GetRegDWord(cpu, op->rn);
//	j=BTESH2_GetRegDWord(cpu, op->rm);
//	cpu->regs[op->rn]=i/j;

	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
//	s=s&0x0F2;
	s=s&0xFFFFF0F2;
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

#if 1
void BTSH_Op_DIV1_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	byte q, q0, m, t;
	u32 s;
	u64 i1, i2;
//	u32 i, j, s;
	
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	m=(s>>9)&1;
	q=(s>>8)&1;
	t=s&1;
	
//	printf("DIV1\n");
	i1=BTESH2_GetRegQWord(cpu, op->rm);
	i2=BTESH2_GetRegQWord(cpu, op->rn);
	q0=q;
	q=(i2>>31)&1;
	i2=((u64)(i2<<1))|t;
	if(q0==m)
		{ i2=i2-i1; }
	else
		{ i2=i2+i1; }
	q=(q^m)^((i2>>32)&1);
	t=1-(q^m);
//	s=(s&0x0F2)|(m<<9)|(q<<8)|t;
	s=(s&0xFFFFF0F2)|(m<<9)|(q<<8)|t;
//	cpu->regs[op->rn]=i2;
	BTESH2_SetRegQWord(cpu, op->rn, i2);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
//	cpu->regs[op->rn]=i/j;
}

void BTSH_Op_DIV0S_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i1, i2;
	u32 i, j, s;
//	printf("DIV0S\n");
//	i=BTESH2_GetRegDWord(cpu, op->rn);
//	j=BTESH2_GetRegDWord(cpu, op->rm);
	i1=BTESH2_GetRegQWord(cpu, op->rm);
	i2=BTESH2_GetRegQWord(cpu, op->rn);
//	cpu->regs[op->rn]=i/j;
	
	s=BTESH2_GetRegDWord(cpu, op->rn);
//	s=(s&0x0F2)|((j>>31)<<9)|((i>>31)<<8)|((i^j)>>31);
//	s=(s&0xFFFFF0F2)|((j>>31)<<9)|((i>>31)<<8)|((i^j)>>31);
	s=(s&0xFFFFF0F2)|((i1>>63)<<9)|((i2>>63)<<8)|((i1^i2)>>63);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}
#endif


void BTSH_Op_DMULS_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	s32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=((s64)i)*j;
//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
//	cpu->regs[BTESH2_REG_MACL]=(u32)k;
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACH, k>>32);
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_DMULU_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=((u64)i)*j;
//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
//	cpu->regs[BTESH2_REG_MACL]=(u32)k;
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACH, k>>32);
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_DT_Reg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=i-1;
	s=(s&(~1))|((k==0)&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}


void BTSH_Op_EXTSB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(sbyte)i;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_EXTSW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(s16)i;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_EXTUB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(byte)i;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_EXTUW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(u16)i;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

#if 1
void BTSH_OpJQ_EXTSB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(sbyte)i;
	BTESH2_SetRegQWord(cpu, op->rn, k);
//	cpu->regs[BTESH2_REG_RLO+op->rn]=k;
//	cpu->regs[BTESH2_REG_RHI+op->rn]=k>>32;
}

void BTSH_OpJQ_EXTSW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(s16)i;
//	cpu->regs[BTESH2_REG_RLO+op->rn]=k;
//	cpu->regs[BTESH2_REG_RHI+op->rn]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_OpJQ_EXTUB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(byte)i;
//	cpu->regs[BTESH2_REG_RLO+op->rn]=k;
//	cpu->regs[BTESH2_REG_RHI+op->rn]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_OpJQ_EXTUW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=(u16)i;
//	cpu->regs[BTESH2_REG_RLO+op->rn]=k;
//	cpu->regs[BTESH2_REG_RHI+op->rn]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}
#endif

void BTSH_Op_MACL_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 s;
	u64 ia, ja;
	s64 k, l;
	s32 i, j;
	int i1;

//	i=BTESH2_GetRegDWord(cpu, op->rn);
//	j=BTESH2_GetRegDWord(cpu, op->rm);


//	i=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rn]);
//	cpu->regs[op->rn]+=4;
//	j=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]);
//	cpu->regs[op->rm]+=4;

	ia=BTESH2_GetRegQWord(cpu, op->rn);
	ja=BTESH2_GetRegQWord(cpu, op->rm);
	i=BTESH2_GetAddrDWord(cpu, ia);
	j=BTESH2_GetAddrDWord(cpu, ja);
	BTESH2_SetRegQWord(cpu, op->rn, ia+4);
	BTESH2_SetRegQWord(cpu, op->rm, ja+4);

//	l=(((s64)cpu->regs[BTESH2_REG_MACH])<<32)|
//		cpu->regs[BTESH2_REG_MACL];
	l=(((s64)BTESH2_GetRegDWordZx(cpu, BTESH2_REG_MACH))<<32)|
		BTESH2_GetRegDWordZx(cpu, BTESH2_REG_MACL);

	k=((s64)i)*j+l;
	
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	if(s&2)
	{
		i1=k>>47;
		if(i1>( 0)) k=( 1LL<<47)-1;
//		if(i1<(-1)) k=(-1LL<<47);
		if(i1<(-1)) k=~((1LL<<47)-1);
	}
	
//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
//	cpu->regs[BTESH2_REG_MACL]=(u32)k;
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACH, k>>32);
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_MACW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 ia, ja;
	s64 k, l, t;
	u32 s;
	s32 i, j;
	int i1;

//	i=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rn]);
//	cpu->regs[op->rn]+=2;
//	j=(s16)BTESH2_GetAddrWord(cpu, cpu->regs[op->rm]);
//	cpu->regs[op->rm]+=2;

	ia=BTESH2_GetRegQWord(cpu, op->rn);
	ja=BTESH2_GetRegQWord(cpu, op->rm);
	i=BTESH2_GetAddrWord(cpu, ia);
	j=BTESH2_GetAddrWord(cpu, ja);
	BTESH2_SetRegQWord(cpu, op->rn, ia+2);
	BTESH2_SetRegQWord(cpu, op->rm, ja+2);

	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	if(s&2)
	{
//		l=((s32)(cpu->regs[BTESH2_REG_MACL]));
		l=BTESH2_GetRegDWordSx(cpu, BTESH2_REG_MACL);
		k=(s32)(i*j)+l;
		
		t=k; i1=(k>>31);
		if(i1>( 0))t=0x7FFFFFFFULL;
		if(i1<(-1))t=0x80000000ULL;
		
		if(k==t)
		{
//			k=(((u64)cpu->regs[BTESH2_REG_MACH])<<32)|
//				((u32)k);
			k=(((u64)BTESH2_GetRegDWordZx(cpu, BTESH2_REG_MACH))<<32)|
				((u32)k);
		}else
		{
//			k=(((s64)cpu->regs[BTESH2_REG_MACH])<<32)|
//				(1LL<<32)|((u32)t);
			k=(((s64)BTESH2_GetRegDWordZx(cpu, BTESH2_REG_MACH))<<32)|
				(1LL<<32)|((u32)t);
		}
	}else
	{
//		l=(((s64)cpu->regs[BTESH2_REG_MACH])<<32)|
//			cpu->regs[BTESH2_REG_MACL];

		l=(((s64)BTESH2_GetRegDWordSx(cpu, BTESH2_REG_MACH))<<32)|
			BTESH2_GetRegDWordZx(cpu, BTESH2_REG_MACL);
		k=((s64)i)*j+l;
	}

//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
//	cpu->regs[BTESH2_REG_MACL]=(u32)k;
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACH, k>>32);
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_MULL_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=i*j;
//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
//	cpu->regs[BTESH2_REG_MACL]=(u32)k;
//	cpu->regs[BTESH2_REG_RHI+BTESH2_REG_MACL]=k>>31;
	BTESH2_SetRegDWordSx(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_MULL_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 i, j, k;
//	i=(s32)(cpu->regs[op->rn]);
//	j=(s32)(cpu->regs[op->rm]);
	i=BTESH2_GetRegDWordSx(cpu, op->rn);
	j=BTESH2_GetRegDWordSx(cpu, op->rm);
	k=i*j;
//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
//	cpu->regs[BTESH2_REG_RLO+BTESH2_REG_MACL]=(u32)k;
//	cpu->regs[BTESH2_REG_RHI+BTESH2_REG_MACL]=k>>32;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_MULQ_RegRegQ(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 i, j, k;
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i*j;
//	cpu->regs[BTESH2_REG_MACH]=(u32)(k>>32);
//	cpu->regs[BTESH2_REG_RLO+BTESH2_REG_MACL]=(u32)k;
//	cpu->regs[BTESH2_REG_RHI+BTESH2_REG_MACL]=k>>32;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_MULSW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s32 i, j, k;
//	i=(s16)(cpu->regs[op->rn]);
//	j=(s16)(cpu->regs[op->rm]);
	i=(s16)BTESH2_GetRegDWord(cpu, op->rn);
	j=(s16)BTESH2_GetRegDWord(cpu, op->rm);
	k=i*j;
//	cpu->regs[BTESH2_REG_MACL]=(u32)k;
	BTESH2_SetRegDWord(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_MULUW_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s32 i, j, k;
//	i=(u16)(cpu->regs[op->rn]);
//	j=(u16)(cpu->regs[op->rm]);
	i=(u16)BTESH2_GetRegDWord(cpu, op->rn);
	j=(u16)BTESH2_GetRegDWord(cpu, op->rm);
	k=i*j;
//	cpu->regs[BTESH2_REG_MACL]=(u32)k;
	BTESH2_SetRegDWordZx(cpu, BTESH2_REG_MACL, k);
}

void BTSH_Op_NEG_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, k;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=0-i;
	BTESH2_SetRegDWord(cpu, op->rn, k);
}

void BTSH_Op_NEGC_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	s32 i, s;

	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);

	i=BTESH2_GetRegDWord(cpu, op->rm);
	k=0-((s64)i)-(s&1);
//	k=0-(~i)+1;
//	cpu->regs[op->rn]=(u32)k;
	BTESH2_SetRegDWordZx(cpu, op->rn, k);

	s=(s&(~1))|((k>>32)&1);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SUB_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	BTESH2_SetRegDWord(cpu, op->rn, i-j);
}

void BTSH_Op_SUBC_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	s32 i, j, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=((s64)i)-j-(s&1);
	s=(s&(~1))|((k>>32)&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SUBV_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	s64 k;
	u32 i, j, s;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	k=((s64)((s32)i))-((s32)j);
	s=(s&(~1))|((k!=((s32)k))&1);
//	s=(s&(~1))|((k>>32)&1);
	BTESH2_SetRegDWord(cpu, op->rn, k);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);
}

void BTSH_Op_SUB_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	BTESH2_SetRegDWord(cpu, op->ro, i-j);
}

void BTSH_Op_MUL_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	BTESH2_SetRegDWord(cpu, op->ro, i*j);
}

void BTSH_Op_ADD_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	BTESH2_SetRegDWord(cpu, op->rn, i+j);
}

void BTSH_Op_SUB_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	BTESH2_SetRegDWord(cpu, op->rn, i-j);
}

void BTSH_Op_MUL_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=BTESH2_GetRegDWord(cpu, op->rm);
	j=op->imm;
	BTESH2_SetRegDWord(cpu, op->rn, i*j);
}

#if 1
void BTSH_Op_ADDQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	k=i+j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_SUBQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	k=i-j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_MULQ_RegImmReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rm);
	j=op->imm;
	k=i*j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}
#endif

void BTSH_Op_ADDQ_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=op->imm;
	k=i+j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ADDQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
//	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i+j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_ADDQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
//	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i+j;
//	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
//	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_SUBQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
//	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i-j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_SUBQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
//	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i-j;
//	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
//	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_MULQ_RegRegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	i=cpu->regs[op->rn+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rn+BTESH2_REG_RHI]<<32);
//	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	i=BTESH2_GetRegQWord(cpu, op->rn);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=i*j;
//	cpu->regs[op->ro+BTESH2_REG_RLO]=k;
//	cpu->regs[op->ro+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->ro, k);
}

void BTSH_Op_NEGQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=-j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_NOTQ_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
//	j=cpu->regs[op->rm+BTESH2_REG_RLO]|
//		((u64)cpu->regs[op->rm+BTESH2_REG_RHI]<<32);
	j=BTESH2_GetRegQWord(cpu, op->rm);
	k=~j;
//	cpu->regs[op->rn+BTESH2_REG_RLO]=k;
//	cpu->regs[op->rn+BTESH2_REG_RHI]=k>>32;
	BTESH2_SetRegQWord(cpu, op->rn, k);
}

void BTSH_Op_TASB_StReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 ia;
	u32 i, j, s;

	ia=BTESH2_GetRegQWord(cpu, op->rn);
	s=BTESH2_GetRegDWord(cpu, BTESH2_REG_SR);
	i=BTESH2_GetAddrByte(cpu, ia);
	BTESH2_SetAddrByte(cpu, ia, i|0x80);
	s=(s&(~1))|(i==0);
	BTESH2_SetRegDWordL(cpu, BTESH2_REG_SR, s);

//	i=BTESH2_GetRegDWord(cpu, op->rn);
//	j=BTESH2_GetRegDWord(cpu, op->rm);
//	BTESH2_SetRegDWord(cpu, op->rn, i-j);
}

void BTSH_Op_XTRCT_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=BTESH2_GetRegDWord(cpu, op->rn);
	j=BTESH2_GetRegDWord(cpu, op->rm);
	k=(i>>16)|(j<<16);
	BTESH2_SetRegDWord(cpu, op->rn, k);
}
