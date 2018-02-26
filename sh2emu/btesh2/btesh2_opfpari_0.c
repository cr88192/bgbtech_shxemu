void BTSH_Op_FABS_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rn];
//	j=cpu->regs[op->rm];
	i=i&0x7FFFFFFFU;
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FABS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FABS_FR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	BTSH_Op_FABS_FR(cpu, op);
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
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];
	*(double *)(&k)=(*(double *)(&i))+(*(double *)(&j));
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FADD_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		op->fmid=BTESH2_FMID_DREGREG;
		op->Run=BTSH_Op_FADD_DRR;
		if((op->rm|op->rn)&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FADD_DRR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGREG;
		op->Run=BTSH_Op_FADD_FRR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FADD_FRR(cpu, op);
	}
}

void BTSH_Op_FCMPEQ_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	s=cpu->regs[BTESH2_REG_SR];
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
//	k=(i==j);
	k=((*(float *)(&i))==(*(float *)(&j)));
	s=(s&(~1))|k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_FCMPEQ_DRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	u32 s, k;
	s=cpu->regs[BTESH2_REG_SR];
	i=((u64)cpu->fregs[op->rn^0]<<32)|cpu->fregs[op->rn^1];
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];
	k=((*(double *)(&i))==(*(double *)(&j)));
	s=(s&(~1))|k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_FCMPEQ_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		op->fmid=BTESH2_FMID_DREGREG;
		op->Run=BTSH_Op_FCMPEQ_DRR;
		if((op->rm|op->rn)&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FCMPEQ_DRR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGREG;
		op->Run=BTSH_Op_FCMPEQ_FRR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FCMPEQ_FRR(cpu, op);
	}
}

void BTSH_Op_FCMPGT_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k, s;
	s=cpu->regs[BTESH2_REG_SR];
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	k=((*(float *)(&i))>(*(float *)(&j)));
	s=(s&(~1))|k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_FCMPGT_DRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	u32 s, k;
	s=cpu->regs[BTESH2_REG_SR];
	i=((u64)cpu->fregs[op->rn^0]<<32)|cpu->fregs[op->rn^1];
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];
	k=((*(double *)(&i))>(*(double *)(&j)));
	s=(s&(~1))|k;
	cpu->regs[BTESH2_REG_SR]=s;
}

void BTSH_Op_FCMPGT_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		op->fmid=BTESH2_FMID_DREGREG;
		op->Run=BTSH_Op_FCMPGT_DRR;
		if((op->rm|op->rn)&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FCMPGT_DRR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGREG;
		op->Run=BTSH_Op_FCMPGT_FRR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FCMPGT_FRR(cpu, op);
	}
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
	j=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];
	*(double *)(&k)=(*(double *)(&i))/(*(double *)(&j));
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FDIV_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		op->fmid=BTESH2_FMID_DREGREG;
		op->Run=BTSH_Op_FDIV_DRR;
		if((op->rm|op->rn)&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FDIV_DRR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGREG;
		op->Run=BTSH_Op_FDIV_FRR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FDIV_FRR(cpu, op);
	}
}

void BTSH_Op_FLDI0_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=0;
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FLDI1_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=0x3F800000;
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FLDI0_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FLDI0_FR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	BTSH_Op_FLDI0_FR(cpu, op);
}

void BTSH_Op_FLDI1_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FLDI0_FR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	BTSH_Op_FLDI1_FR(cpu, op);
}

void BTSH_Op_FMAC_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rn];
	j=cpu->fregs[op->rm];
	k=cpu->fregs[op->ro];
	*(float *)(&k)=(*(float *)(&j))*(*(float *)(&k))+(*(float *)(&i));
	cpu->fregs[op->rn]=k;
}

void BTSH_Op_FMAC_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FMAC_FRR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; op->rn^=16; op->ro^=16; }
	BTSH_Op_FMAC_FRR(cpu, op);
}

void BTSH_Op_FIPR_FRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rn+0]; j=cpu->fregs[op->rm+0];
	*(float *)(&k)=(*(float *)(&i))*(*(float *)(&j));
	i=cpu->fregs[op->rn+1]; j=cpu->fregs[op->rm+1];
	*(float *)(&k)+=(*(float *)(&i))*(*(float *)(&j));
	i=cpu->fregs[op->rn+2]; j=cpu->fregs[op->rm+2];
	*(float *)(&k)+=(*(float *)(&i))*(*(float *)(&j));
	cpu->fregs[op->rn+3]=k;
}

void BTSH_Op_FIPR_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FIPR_FRR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; op->rn^=16; op->ro^=16; }
	BTSH_Op_FIPR_FRR(cpu, op);
}

void BTSH_Op_FTRV_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	float tv[4];
	float f, g;
	u32 vi, vj, vk;
	int i, j, k;
	
//	u32 i, j, k;
//	i=cpu->fregs[op->rn+0]; j=cpu->fregs[op->rm+0];
//	*(float *)(&k)=(*(float *)(&i))*(*(float *)(&j));
//	i=cpu->fregs[op->rn+1]; j=cpu->fregs[op->rm+1];
//	*(float *)(&k)+=(*(float *)(&i))*(*(float *)(&j));
//	i=cpu->fregs[op->rn+2]; j=cpu->fregs[op->rm+2];
//	*(float *)(&k)+=(*(float *)(&i))*(*(float *)(&j));
//	cpu->fregs[op->rn+3]=k;

	for(i=0; i<4; i++)
	{
		f=0;
		for(j=0; j<3; j++)
		{
			vi=cpu->fregs[op->rm+j*4+i]; vj=cpu->fregs[op->rn+j];
			f+=(*(float *)(&vi))*(*(float *)(&vj));
		}
		tv[i]=f;
	}
	
	for(i=0; i<4; i++)
	{
		(*(float *)(&vi))=tv[i];
		cpu->fregs[op->rn+i]=vi;
	}
}

void BTSH_Op_FTRV_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FTRV_FR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; op->rn^=16; }
	BTSH_Op_FTRV_FR(cpu, op);
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

void BTSH_Op_FMOV_XRR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rm^0];
	j=cpu->fregs[op->rm^1];
	cpu->fregs[op->rn^0]=i;
	cpu->fregs[op->rn^1]=j;
	i=cpu->fregs[op->rm^0x10];
	j=cpu->fregs[op->rm^0x11];
	cpu->fregs[op->rn^0x10]=i;
	cpu->fregs[op->rn^0x11]=j;
}


void BTSH_Op_FMOV_GRFR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->regs[op->rm];
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FMOV_GRDR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->regs[BTESH2_REG_RHI+op->rm];
	j=cpu->regs[BTESH2_REG_RLO+op->rm];
	cpu->fregs[op->rn^0]=i;
	cpu->fregs[op->rn^1]=j;
}

void BTSH_Op_FMOV_FRGR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rm];
	cpu->regs[op->rn]=i;
}

void BTSH_Op_FMOV_DRGR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j, k;
	i=cpu->fregs[op->rm^0];
	j=cpu->fregs[op->rm^1];
	cpu->regs[BTESH2_REG_RHI+op->rn]=i;
	cpu->regs[BTESH2_REG_RLO+op->rn]=j;
}

void BTSH_Op_FMOVS_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if((op->rm&op->rn)==15)
	{
		__debugbreak();
		return;
	}

	op->fmid=BTESH2_FMID_FREGREG;
	op->Run=BTSH_Op_FMOV_FRR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; op->rn^=16; }
	BTSH_Op_FMOV_FRR(cpu, op);
}

void BTSH_Op_FMOVD_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->fmid=BTESH2_FMID_DREGREG;
	op->Run=BTSH_Op_FMOV_DRR;
//	if((op->rm|op->rn)&1)
//		__debugbreak();
	if(op->rm&1)	{ op->rm^=0x11; }
	if(op->rn&1)	{ op->rn^=0x11; }
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; op->rn^=16; }
	BTSH_Op_FMOV_DRR(cpu, op);
}

void BTSH_Op_FMOV_RR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		BTSH_Op_FMOVD_RR(cpu, op);
	}else
	{
		BTSH_Op_FMOVS_RR(cpu, op);
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
		op->fmid=BTESH2_FMID_DREGREG;
		op->Run=BTSH_Op_FMUL_DRR;
		if((op->rm|op->rn)&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FMUL_DRR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGREG;
		op->Run=BTSH_Op_FMUL_FRR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FMUL_FRR(cpu, op);
	}
}

void BTSH_Op_FNEG_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rn];
	j=i^0x80000000U;
	cpu->fregs[op->rn]=j;
}

void BTSH_Op_FNEG_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FNEG_FR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	BTSH_Op_FNEG_FR(cpu, op);
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
		op->fmid=BTESH2_FMID_DREGRN;
		op->Run=BTSH_Op_FSQRT_DR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FSQRT_DR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGRN;
		op->Run=BTSH_Op_FSQRT_FR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rn^=16; }
		BTSH_Op_FSQRT_FR(cpu, op);
	}
}

void BTSH_Op_FLDS_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->fregs[op->rm];
	cpu->regs[BTESH2_REG_FPUL]=i;
}

void BTSH_Op_FLDS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FLDS_FR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; }
	BTSH_Op_FLDS_FR(cpu, op);
}

void BTSH_Op_FSTS_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[BTESH2_REG_FPUL];
	cpu->fregs[op->rn]=i;
}

void BTSH_Op_FSTS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FSTS_FR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	BTSH_Op_FSTS_FR(cpu, op);
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
		op->fmid=BTESH2_FMID_DREGRN;
		op->Run=BTSH_Op_FLOAT_DR;
		if(op->rn&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FLOAT_DR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGRN;
		op->Run=BTSH_Op_FLOAT_FR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rn^=16; }
		BTSH_Op_FLOAT_FR(cpu, op);
	}
}

void BTSH_Op_FTRC_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	int k;
	i=cpu->fregs[op->rm];
//	*(float *)(&j)=(s32)i;
	k=*(float *)(&i);
	cpu->regs[BTESH2_REG_FPUL]=k;
}

void BTSH_Op_FTRC_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 i, j;
	int k;
	i=((u64)cpu->fregs[op->rm^0]<<32)|cpu->fregs[op->rm^1];
//	i=cpu->fregs[op->rn];
//	*(float *)(&j)=(s32)i;
	k=*(double *)(&i);
	cpu->regs[BTESH2_REG_FPUL]=k;
}

void BTSH_Op_FTRC_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_PR)
	{
		op->fmid=BTESH2_FMID_DREGRM;
		op->Run=BTSH_Op_FTRC_DR;
		if(op->rm&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FTRC_DR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGRM;
		op->Run=BTSH_Op_FTRC_FR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; }
		BTSH_Op_FTRC_FR(cpu, op);
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
		op->fmid=BTESH2_FMID_DREGREG;
		op->Run=BTSH_Op_FSUB_DRR;
		if((op->rm|op->rn)&1)
			__debugbreak();
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FSUB_DRR(cpu, op);
	}else
	{
		op->fmid=BTESH2_FMID_FREGREG;
		op->Run=BTSH_Op_FSUB_FRR;
		if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
			{ op->rm^=16; op->rn^=16; }
		BTSH_Op_FSUB_FRR(cpu, op);
	}
}


void BTSH_Op_FCNVSD_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j;
	i=cpu->regs[BTESH2_REG_FPUL];
	*(double *)(&k)=*(float *)(&i);
	cpu->fregs[op->rn^0]=(u32)(k>>32);
	cpu->fregs[op->rn^1]=(u32)(k    );
}

void BTSH_Op_FCNVSD_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FCNVSD_DR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	BTSH_Op_FCNVSD_DR(cpu, op);
}

void BTSH_Op_FCNVDS_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u64 k;
	u32 i, j;
	i=cpu->fregs[op->rm^0];
	j=cpu->fregs[op->rm^1];
	k=(((u64)i)<<32)|j;
//	*(float *)(&j)=(s32)i;
	*(float *)(&j)=*(double *)(&k);
	cpu->regs[BTESH2_REG_FPUL]=j;
}

void BTSH_Op_FCNVDS_R(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	op->Run=BTSH_Op_FCNVDS_DR;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; }
	BTSH_Op_FCNVDS_DR(cpu, op);
}

void BTSH_Op_FSCHG_Z(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 i, j;
	i=cpu->regs[BTESH2_REG_FPSCR];
	j=i^BTESH2_FPSCR_SZ;
	cpu->regs[BTESH2_REG_FPSCR]=j;
}

void BTSH_Op_FMOV_RegLd_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]);
}

void BTSH_Op_FMOV_RegLd_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn^0]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]+0);
	cpu->fregs[op->rn^1]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]+4);
}

void BTSH_Op_FMOV_RegSt_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->fregs[op->rm]);
}

void BTSH_Op_FMOV_RegSt_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+0, cpu->fregs[op->rm^0]);
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+4, cpu->fregs[op->rm^1]);
}

void BTSH_Op_FMOV_RegLd(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }

	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		op->Run=BTSH_Op_FMOV_RegLd_DR;
		if(op->rn&1)	{ op->rn^=0x11; }
//		op->fmid=BTESH2_FMID_DREGREG;
		op->Run(cpu, op);
	}else
	{
		op->Run=BTSH_Op_FMOV_RegLd_FR;
//		op->fmid=BTESH2_FMID_FREGREG;
		op->Run(cpu, op);
	}
}

void BTSH_Op_FMOV_RegSt(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; }

	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		op->Run=BTSH_Op_FMOV_RegSt_DR;
		if(op->rm&1)	{ op->rm^=0x11; }
//		op->fmid=BTESH2_FMID_DREGREG;
		op->Run(cpu, op);
	}else
	{
		op->Run=BTSH_Op_FMOV_RegSt_FR;
//		op->fmid=BTESH2_FMID_FREGREG;
		op->Run(cpu, op);
	}
}

void BTSH_Op_FMOV_RegDecSt_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=4;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn], cpu->fregs[op->rm]);
}

void BTSH_Op_FMOV_RegDecSt_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rn]-=8;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+0, cpu->fregs[op->rm^0]);
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+4, cpu->fregs[op->rm^1]);
}

void BTSH_Op_FMOV_RegDecSt(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; }

	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		op->Run=BTSH_Op_FMOV_RegDecSt_DR;
		if(op->rm&1)	{ op->rm^=0x11; }
//		op->fmid=BTESH2_FMID_DREGREG;
		op->Run(cpu, op);
	}else
	{
		op->Run=BTSH_Op_FMOV_RegDecSt_FR;
//		op->fmid=BTESH2_FMID_FREGREG;
		op->Run(cpu, op);
	}
}

void BTSH_Op_FMOV_RegIncLd_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=4;
	cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
}

void BTSH_Op_FMOV_RegIncLd_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->regs[op->rm]+=8;
	cpu->fregs[op->rn^0]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-8);
	cpu->fregs[op->rn^1]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]-4);
}

void BTSH_Op_FMOV_RegIncLd(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }

	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		op->Run=BTSH_Op_FMOV_RegIncLd_DR;
		if(op->rn&1)	{ op->rn^=0x11; }
//		op->fmid=BTESH2_FMID_DREGREG;
		op->Run(cpu, op);
	}else
	{
		op->Run=BTSH_Op_FMOV_RegIncLd_FR;
//		op->fmid=BTESH2_FMID_FREGREG;
		op->Run(cpu, op);
	}
}

void BTSH_Op_FMOV_RegStR0n_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro], cpu->fregs[op->rm]);
}

void BTSH_Op_FMOV_RegStR0n_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro]+0, cpu->fregs[op->rm^0]);
	BTESH2_SetAddrDWord(cpu,
		cpu->regs[op->rn]+cpu->regs[op->ro]+4, cpu->fregs[op->rm^1]);
}

void BTSH_Op_FMOV_RegStR0n(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; }

	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		op->Run=BTSH_Op_FMOV_RegStR0n_DR;
		if(op->rm&1)	{ op->rm^=0x11; }
//		op->fmid=BTESH2_FMID_DREGREG;
		op->Run(cpu, op);
	}else
	{
		op->Run=BTSH_Op_FMOV_RegStR0n_FR;
//		op->fmid=BTESH2_FMID_FREGREG;
		op->Run(cpu, op);
	}
}

void BTSH_Op_FMOV_RegLdR0m_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]);
}

void BTSH_Op_FMOV_RegLdR0m_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn^0]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]+0);
	cpu->fregs[op->rn^1]=BTESH2_GetAddrDWord(cpu,
		cpu->regs[op->rm]+cpu->regs[op->ro]+4);
}

void BTSH_Op_FMOV_RegLdR0m(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }

	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
	{
		op->Run=BTSH_Op_FMOV_RegLdR0m_DR;
		if(op->rn&1)	{ op->rn^=0x11; }
//		op->fmid=BTESH2_FMID_DREGREG;
		op->Run(cpu, op);
	}else
	{
		op->Run=BTSH_Op_FMOV_RegLdR0m_FR;
//		op->fmid=BTESH2_FMID_FREGREG;
		op->Run(cpu, op);
	}
}


void BTSH_Op_FMOV_RegLdDisp_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]+op->imm);
}

void BTSH_Op_FMOV_RegLdDisp_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn^0]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]+op->imm+0);
	cpu->fregs[op->rn^1]=BTESH2_GetAddrDWord(cpu, cpu->regs[op->rm]+op->imm+4);
}

void BTSH_Op_FMOV_RegStDisp_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+op->imm, cpu->fregs[op->rm]);
}

void BTSH_Op_FMOV_RegStDisp_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+op->imm+0, cpu->fregs[op->rm^0]);
	BTESH2_SetAddrDWord(cpu, cpu->regs[op->rn]+op->imm+4, cpu->fregs[op->rm^1]);
}

void BTSH_Op_FMOVS_RegLdDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	op->Run=BTSH_Op_FMOV_RegLdDisp_FR;
//	op->fmid=BTESH2_FMID_FREGREG;
	op->Run(cpu, op);
}

void BTSH_Op_FMOVD_RegLdDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rn^=16; }
	op->Run=BTSH_Op_FMOV_RegLdDisp_DR;
	if(op->rn&1)	{ op->rn^=0x11; }
//	op->fmid=BTESH2_FMID_DREGREG;
	op->Run(cpu, op);
}

void BTSH_Op_FMOV_RegLdDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
		{ BTSH_Op_FMOVD_RegLdDisp(cpu, op); }
	else
		{ BTSH_Op_FMOVS_RegLdDisp(cpu, op); }
}

void BTSH_Op_FMOVS_RegStDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; }
	op->Run=BTSH_Op_FMOV_RegStDisp_FR;
//	op->fmid=BTESH2_FMID_FREGREG;
	op->Run(cpu, op);
}

void BTSH_Op_FMOVD_RegStDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_FR)
		{ op->rm^=16; }
	op->Run=BTSH_Op_FMOV_RegStDisp_DR;
	if(op->rm&1)	{ op->rm^=0x11; }
//	op->fmid=BTESH2_FMID_DREGREG;
	op->Run(cpu, op);
}

void BTSH_Op_FMOV_RegStDisp(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if(cpu->regs[BTESH2_REG_FPSCR]&BTESH2_FPSCR_SZ)
		{ BTSH_Op_FMOVD_RegStDisp(cpu, op); }
	else
		{ BTSH_Op_FMOVS_RegStDisp(cpu, op); }
}


void BTSH_Op_FMOVS_RegLdAbs_FR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu, op->imm);
}

void BTSH_Op_FMOVS_RegLdAbs_DR(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	cpu->ptcpc=op->pc;
	cpu->fregs[op->rn]=BTESH2_GetAddrDWord(cpu, op->imm);

	cpu->ptcpc=op->pc;
	BTESH2_SetFRegQWord(cpu, op->rn,
		BTESH2_GetAddrQWord(cpu, op->imm));
}


void BTSH_Op_SetFPSCR(BTESH2_CpuState *cpu, u32 v)
{
	cpu->regs[BTESH2_REG_FPSCR]=v;

	if(v&BTESH2_FPSCR_PR)
		{ cpu->csfl|=BTESH2_CSFL_FPPR; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_FPPR; }
	if(v&BTESH2_FPSCR_SZ)
		{ cpu->csfl|=BTESH2_CSFL_FPSZ; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_FPSZ; }
	if(v&BTESH2_FPSCR_FR)
		{ cpu->csfl|=BTESH2_CSFL_FPFR; }
	else
		{ cpu->csfl&=~BTESH2_CSFL_FPFR; }
}

void BTSH_Op_MOV_FPSCR_RegIncLdD(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 v;
	cpu->ptcpc=op->pc;
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

void BTSH_Op_PSETMD4_Imm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	u32 v;
	int i;

	v=cpu->regs[BTESH2_REG_FPSCR];

	i=op->imm;
	if(i&0x8)	{ v|=BTESH2_FPSCR_SZ; }
	else		{ v&=~BTESH2_FPSCR_SZ; }
	if(i&0x4)	{ v|=BTESH2_FPSCR_PR; }
	else		{ v&=~BTESH2_FPSCR_PR; }
	
	v=(v&(~3))|(i&3);
	
	BTSH_Op_SetFPSCR(cpu, v);
//	cpu->regs[BTESH2_REG_FPSCR]=v;
}
