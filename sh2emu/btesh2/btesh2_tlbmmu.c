/*
 * TLB MMU
 * Will be closer to the SH4 MMU, but with some BJX1 restrictions.
 *
 * TLB will have 64 slots and be 4 way associative.
 */

btesh2_paddr BTESH2_TMMU_MapVirtToPhys(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
	u64 tlbe0, tlbe1, tlbe2, tlbe3, tlbe4;
	u32 tlbh, ptde, pte;
	u32 pgtab;
	int h, pg, pgt;

	if(addr&0x80000000)
	{
		if((addr>>29)==7)
			return(addr);
		if((addr>>29)!=6)
			{ return(addr&0x1FFFFFFF); }
	}

	if(!(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT))
		{ return(addr&0x1FFFFFFFU); }
	
	tlbh=(addr>>12)*65521;
	h=(tlbh>>16)&63;

	tlbe0=cpu->tmmu_tlb[h][0];
	if(((tlbe0>>32)&(~4095))==(addr&(~4095)))
		return((tlbe0&(~4095))|(addr&4095));

	tlbe1=cpu->tmmu_tlb[h][1];
	if(((tlbe1>>32)&(~4095))==(addr&(~4095)))
	{
		cpu->tmmu_tlb[h][0]=tlbe1;
		cpu->tmmu_tlb[h][1]=tlbe0;
		return((tlbe1&(~4095))|(addr&4095));
	}

	tlbe2=cpu->tmmu_tlb[h][2];
	if(((tlbe2>>32)&(~4095))==(addr&(~4095)))
	{
		cpu->tmmu_tlb[h][1]=tlbe2;
		cpu->tmmu_tlb[h][2]=tlbe1;
		return((tlbe2&(~4095))|(addr&4095));
	}

	tlbe3=cpu->tmmu_tlb[h][3];
	if(((tlbe3>>32)&(~4095))==(addr&(~4095)))
	{
		cpu->tmmu_tlb[h][2]=tlbe3;
		cpu->tmmu_tlb[h][3]=tlbe2;
		return((tlbe3&(~4095))|(addr&4095));
	}
	
	cpu->status=BTESH2_EXC_TLBMISS;
	return(addr&0x1FFFFFFF);
}


int BTESH2_GetAddrByteTMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
	btesh2_vaddr addr1;
	int i;

#if 1
	if(((addr&0x80000000) ||
		!(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_GetAddrByteFMMU_NoAT(cpu, addr));
	}
#endif

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrBytePhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

int BTESH2_GetAddrWordTMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
//	BTESH2_MemoryImage *mem;
//	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;
	int i;

#if 1
	if(((addr&0x80000000) ||
		!(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_GetAddrWordFMMU_NoAT(cpu, addr));
	}
#endif

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

default_inline u32 BTESH2_GetAddrDWordTMMU(
	BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
	btesh2_vaddr addr1;
	int i;

	if(((addr&0x80000000) ||
		!(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_GetAddrDWordFMMU_NoAT(cpu, addr));
	}

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrDWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

default_inline u64 BTESH2_GetAddrQWordTMMU(
	BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
	btesh2_vaddr addr1;
	u64 v;
	int i;

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	v=BTESH2_GetAddrQWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(v);
}

int BTESH2_SetAddrByteTMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);

	if(((addr&0x80000000) ||
		!(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_SetAddrByteFMMU_NoAT_I(cpu, addr, val));
	}

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrBytePhy2(cpu, addr1, val));
}

int BTESH2_SetAddrWordTMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);

	if(((addr&0x80000000) ||
		!(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_SetAddrWordFMMU_NoAT_I(cpu, addr, val));
	}

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrWordPhy2(cpu, addr1, val));
}

int BTESH2_SetAddrDWordTMMU(BTESH2_CpuState *cpu,
	btesh2_vaddr addr, u32 val)
{
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);

	if(((addr&0x80000000) ||
		!(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_SetAddrDWordFMMU_NoAT_I(cpu, addr, val));
	}

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrDWordPhy2(cpu, addr1, val));
}

int BTESH2_SetAddrQWordTMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, u64 val)
{
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);

	addr1=BTESH2_TMMU_MapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrQWordPhy2(cpu, addr1, val));
}

int BTESH2_TMMU_LDTLB(BTESH2_CpuState *cpu)
{
	u64 tlbe0, tlbe1, tlbe2, tlbe3, tlbe4;
	u32 tlbh, ptde, pte;
	u32 pteh, ptel;
	u32 pgtab;
	int h, pg, pgt;

//	pteh=cpu->regs[BTESH2_REG_PTEH];
//	ptel=cpu->regs[BTESH2_REG_PTEL];

	pteh=BTESH2_GetRegQWord(cpu, BTESH2_REG_PTEH);
	ptel=BTESH2_GetRegQWord(cpu, BTESH2_REG_PTEL);

	tlbh=(pteh>>12)*65521;
	h=(tlbh>>16)&63;

	tlbe4=(((u64)pteh)<<32)|ptel;

	tlbe0=cpu->tmmu_tlb[h][0];
	tlbe1=cpu->tmmu_tlb[h][1];
	tlbe2=cpu->tmmu_tlb[h][2];
	tlbe3=cpu->tmmu_tlb[h][3];

	cpu->tmmu_tlb[h][0]=tlbe4;
	cpu->tmmu_tlb[h][1]=tlbe0;
	cpu->tmmu_tlb[h][2]=tlbe1;
	cpu->tmmu_tlb[h][3]=tlbe2;
	return(0);
}

int BTESH2_SetupUpdateTMMU(BTESH2_CpuState *cpu)
{
	if(BTESH2_GetRegQWord(cpu, BTESH2_REG_MMUCR)&BTESH2_MMUCR_AT)
	{
		cpu->GetAddrByte=BTESH2_GetAddrByteTMMU;
		cpu->GetAddrWord=BTESH2_GetAddrWordTMMU;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordTMMU;
		cpu->GetAddrQWord=BTESH2_GetAddrQWordTMMU;

		cpu->SetAddrByte=BTESH2_SetAddrByteTMMU;
		cpu->SetAddrWord=BTESH2_SetAddrWordTMMU;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordTMMU;
		cpu->SetAddrQWord=BTESH2_SetAddrQWordTMMU;

		cpu->OpLdTlb=BTESH2_TMMU_LDTLB;
	}else
	{
		cpu->GetAddrByte=BTESH2_GetAddrByteFMMU_NoAT;
		cpu->GetAddrWord=BTESH2_GetAddrWordFMMU_NoAT;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordFMMU_NoAT;
		cpu->GetAddrQWord=BTESH2_GetAddrQWordFMMU;

		cpu->SetAddrByte=BTESH2_SetAddrByteFMMU_NoAT;
		cpu->SetAddrWord=BTESH2_SetAddrWordFMMU_NoAT;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordFMMU_NoAT;
		cpu->SetAddrQWord=BTESH2_SetAddrQWordFMMU;

		cpu->OpLdTlb=BTESH2_TMMU_LDTLB;
	}
	cpu->jit_needflush=1;
	return(0);
}
