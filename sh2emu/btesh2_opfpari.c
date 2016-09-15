void BTSH_Op_FABS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rn];
//	j=cpu->regs[op->rm];
	i=i&0x7FFFFFFFU;
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FADD_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	*(float *)(&k)=(*(float *)(&i))+(*(float *)(&j));
	cpu->fregs[op->rn]=k;
}

void BTSH_Op_FADD_DRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=((u64)cpu->fregs[op->rn^0]<<32)|cpu->fregs[op->rn^1];
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];;
	*(double *)(&k)=(*(double *)(&i))+(*(double *)(&j));
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FADD_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		BTSH_Op_FADD_DRR(cpu, op);
		op->Run=BTSH_Op_FADD_DRR;
	}else
	{
		BTSH_Op_FADD_FRR(cpu, op);
		op->Run=BTSH_Op_FADD_FRR;
	}
}

void BTSH_Op_FCMPEQ_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	s=cpu->regs[BTESH2_REG_SR];
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	k=(i==j);
	s=(s&(~1))|k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_FCMPGT_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	s=cpu->regs[BTESH2_REG_SR];
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	k=((*(float *)(&i))>(*(float *)(&j)));
	s=(s&(~1))|k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_FDIV_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	*(float *)(&k)=(*(float *)(&i))/(*(float *)(&j));
	cpu->fregs[op->rn]=k;
}

void BTSH_Op_FDIV_DRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=((u64)cpu->fregs[op->rn^0]<<32)|cpu->fregs[op->rn^1];
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];;
	*(double *)(&k)=(*(double *)(&i))/(*(double *)(&j));
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FDIV_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		BTSH_Op_FDIV_DRR(cpu, op);
		op->Run=BTSH_Op_FDIV_DRR;
	}else
	{
		BTSH_Op_FDIV_FRR(cpu, op);
		op->Run=BTSH_Op_FDIV_FRR;
	}
}

void BTSH_Op_FLDI0_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
//	i=cpu->fregs[op->rn];
//	j=cpu->regs[op->rm];
	i=0;
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FLDI1_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
//	i=cpu->fregs[op->rn];
//	j=cpu->regs[op->rm];
	i=0x3F800000;
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FMAC_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	k=cpu->fregs[0];
	*(float *)(&k)=(*(float *)(&j))*(*(float *)(&k))+(*(float *)(&i));
	cpu->fregs[op->rn]=k;
}

void BTSH_Op_FMOV_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rm];
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FMOV_DRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rm^0];
	j=cpu->fregs[op->rm^1];
	cpu->fregs[op->rn^0]=i;
	cpu->fregs[op->rn^1]=j;
}

void BTSH_Op_FMOV_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		BTSH_Op_FMOV_DRR(cpu, op);
		op->Run=BTSH_Op_FMOV_DRR;
	}else
	{
		BTSH_Op_FMOV_FRR(cpu, op);
		op->Run=BTSH_Op_FMOV_FRR;
	}
}

void BTSH_Op_FMUL_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	*(float *)(&k)=(*(float *)(&i))*(*(float *)(&j));
	cpu->fregs[op->rn]=k;
}

void BTSH_Op_FMUL_DRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=((u64)cpu->fregs[op->rn^0]<<32)|cpu->fregs[op->rn^1];
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];;
	*(double *)(&k)=(*(double *)(&i))*(*(double *)(&j));
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FMUL_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		BTSH_Op_FMUL_DRR(cpu, op);
		op->Run=BTSH_Op_FMUL_DRR;
	}else
	{
		BTSH_Op_FMUL_FRR(cpu, op);
		op->Run=BTSH_Op_FMUL_FRR;
	}
}

void BTSH_Op_FNEG_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rn];
	j=i^0x80000000U;
	cpu->fregs[op->rn]=j;
}

void BTSH_Op_FSQRT_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rn];
	*(float *)(&j)=sqrt(*(float *)(&i));
	cpu->fregs[op->rn]=j;
}

void BTSH_Op_FSQRT_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=((u64)cpu->fregs[op->rn^0]<<32)|cpu->fregs[op->rn^1];
	*(double *)(&k)=sqrt(*(double *)(&i));
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FSQRT_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		BTSH_Op_FSQRT_DR(cpu, op);
		op->Run=BTSH_Op_FSQRT_DR;
	}else
	{
		BTSH_Op_FSQRT_FR(cpu, op);
		op->Run=BTSH_Op_FSQRT_FR;
	}
}

void BTSH_Op_FLDS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rm];
	cpu->regs[BTESH2_REG_FPUL]=i;
}

void BTSH_Op_FSTS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[BTESH2_REG_FPUL];
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FLOAT_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[BTESH2_REG_FPUL];
	*(float *)(&j)=(s32)i;
	cpu->fregs[op->rn]=j;
}

void BTSH_Op_FLOAT_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	i=cpu->regs[BTESH2_REG_FPUL];
	*(double *)(&j)=(s32)i;
	cpu->fregs[op->rn^0]=j>>32;
	cpu->fregs[op->rn^1]=j;
}

void BTSH_Op_FLOAT_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		BTSH_Op_FLOAT_DR(cpu, op);
		op->Run=BTSH_Op_FLOAT_DR;
	}else
	{
		BTSH_Op_FLOAT_FR(cpu, op);
		op->Run=BTSH_Op_FLOAT_FR;
	}
}

void BTSH_Op_FTRC_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rm];
//	*(float *)(&j)=(s32)i;
	j=*(float *)(&i);
	cpu->regs[BTESH2_REG_FPUL]=j;
}

void BTSH_Op_FTRC_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	i=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];
//	i=cpu->fregs[op->rn];
//	*(float *)(&j)=(s32)i;
	j=*(double *)(&i);
	cpu->regs[BTESH2_REG_FPUL]=j;
}

void BTSH_Op_FTRC_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		BTSH_Op_FTRC_DR(cpu, op);
		op->Run=BTSH_Op_FTRC_DR;
	}else
	{
		BTSH_Op_FTRC_FR(cpu, op);
		op->Run=BTSH_Op_FTRC_FR;
	}
}

void BTSH_Op_FSUB_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	*(float *)(&k)=(*(float *)(&i))-(*(float *)(&j));
	cpu->fregs[op->rn]=k;
}

void BTSH_Op_FSUB_DRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j, k;
	i=((u64)cpu->fregs[op->rn^0]<<32)|cpu->fregs[op->rn^1];
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];;
	*(double *)(&k)=(*(double *)(&i))-(*(double *)(&j));
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FSUB_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		BTSH_Op_FSUB_DRR(cpu, op);
		op->Run=BTSH_Op_FSUB_DRR;
	}else
	{
		BTSH_Op_FSUB_FRR(cpu, op);
		op->Run=BTSH_Op_FSUB_FRR;
	}
}


void BTSH_Op_FCNVSD_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j;
	i=cpu->regs[BTESH2_REG_FPUL];
	*(double *)(&k)=*(float *)(&i);
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FCNVDS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j;
	i=cpu->fregs[op->rn^0];
	j=cpu->fregs[op->rn^1];
	k=(((u64)i)<<32)|j;
//	*(float *)(&j)=(s32)i;
	*(float *)(&j)=*(double *)(&k);
	cpu->regs[BTESH2_REG_FPUL]=j;
}

void BTSH_Op_FSCHG_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[BTESH2_REG_FPSCR];
	j=i^BTESH2_FPSCR_SZ;
	cpu->regs[BTESH2_REG_FPSCR]=j;
}

void BTSH_Op_FMOV_RegLd(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		cpu->fregs[op->rn^0]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]+0);
		cpu->fregs[op->rn^1]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]+4);
	}else
	{
		cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]);
	}
}

void BTSH_Op_FMOV_RegSt(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+0, cpu->fregs[op->rm^0]);
		BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+4, cpu->fregs[op->rm^1]);
	}else
	{
		BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->fregs[op->rm]);
	}
}

void BTSH_Op_FMOV_RegDecSt(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		cpu->regs[op->rn]-=8;
		BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+0, cpu->fregs[op->rm^0]);
		BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+4, cpu->fregs[op->rm^1]);
	}else
	{
		cpu->regs[op->rn]-=4;
		BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->fregs[op->rm]);
	}
}

void BTSH_Op_FMOV_RegIncLd(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		cpu->regs[op->rm]+=8;
		cpu->fregs[op->rn^0]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-8);
		cpu->fregs[op->rn^1]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
	}else
	{
		cpu->regs[op->rm]+=4;
		cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
	}
}

void BTSH_Op_FMOV_RegStR0n(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		BTESH2_SetAddrDWord(cpu,
			cpu->regs[op->rn]+cpu->regs[op->ro]+0, cpu->fregs[op->rm^0]);
		BTESH2_SetAddrDWord(cpu,
			cpu->regs[op->rn]+cpu->regs[op->ro]+4, cpu->fregs[op->rm^1]);
	}else
	{
		BTESH2_SetAddrDWord(cpu,
			cpu->regs[op->rn]+cpu->regs[op->ro], cpu->fregs[op->rm]);
	}
}

void BTSH_Op_FMOV_RegLdR0m(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		cpu->fregs[op->rn^0]=BTESH2_GetAddrDWord(cpu,
			cpu->regs[op->rm]+cpu->regs[op->ro]+0);
		cpu->fregs[op->rn^1]=BTESH2_GetAddrDWord(cpu,
			cpu->regs[op->rm]+cpu->regs[op->ro]+4);
	}else
	{
		cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu,
			cpu->regs[op->rm]+cpu->regs[op->ro]);
	}
}

void BTSH_Op_SetFPSCR(BTESH2_CpuState *cpu, u32 v)
{
	cpu->regs[BTESH2_REG_FPSCR]=v;

	if(v&BTESH2_FPSCR_PR)
		{ cpu->csfl|=2; }
	else
		{ cpu->csfl&=~2; }
	if(v&BTESH2_FPSCR_SZ)
		{ cpu->csfl|=4; }
	else
		{ cpu->csfl&=~4; }
}

void BTSH_Op_MOV_FPSCR_RegIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 v;
	cpu->regs[op->rm]+=4;
	v=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
//	cpu->regs[op->rn]=v;
	BTSH_Op_SetFPSCR(cpu, v);
}

void BTSH_Op_MOV_FPSCR_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 v;
	v=cpu->regs[op->rm];
//	cpu->regs[op->rn]=v;
	BTSH_Op_SetFPSCR(cpu, v);
}
