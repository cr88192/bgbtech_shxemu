/*
 * Fast MMU assumes an MMU done in a similar style to the x86 MMU.
 * TTB holds a page-table directory.
 * Each PTD entry points to a page table.
 * Each page-table consists of a page-table entry.
 * Each PTE encodes the physical page address.
 * Similarly, pages are assumed to be 4kB.
 *
 * This is not a proper SuperH MMU, but rather a special case.
 */

int BTESH2_FastMapCheckPteValid(BTESH2_CpuState *cpu, u32 pte)
{
	if(!(pte&0x0100))
		return(0);
	return(1);
}

int BTESH2_FastMapCheckPteSane(BTESH2_CpuState *cpu, u32 pte)
{
	if(!(pte&0x0100))
		return(1);

	if(!(pte&0x0010))
		return(0);
	if( (pte&0x0080))
		return(0);
	return(1);
}

btesh2_paddr BTESH2_FastMapVirtToPhys(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
	u64 tlbe0, tlbe1, tlbe2;
	u32 tlbh, ptde, pte;
	u32 pgtab;
	int h, pg, pgt;

	if(addr&0x80000000)
	{
		if((addr>>29)==7)
			return(addr);
		if((addr>>29)!=6)
		{
			return(addr&0x1FFFFFFF);
		}
	}

	if(!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT))
	{
//		if((addr>>29)==7)
//			return(addr);
#if 0
		if((addr>>29)!=6)
		{
//			if(addr!=(addr&0x1FFFFFFFU))
//				{ h=-1; }
			return(addr&0x1FFFFFFFU);
//		return(addr);
		}
#endif
		return(addr&0x1FFFFFFFU);
	}
	
//	addr=addr&0x1FFFFFFFU;
	
	tlbh=(addr>>12)*65521;
	h=(tlbh>>16)&63;
	tlbe0=cpu->tlbe[h];
	if(((tlbe0>>32)&(~4095))==(addr&(~4095)))
		return((tlbe0&(~4095))|(addr&4095));
	tlbe1=cpu->tlbe[h+64];
	if(((tlbe1>>32)&(~4095))==(addr&(~4095)))
	{
		cpu->tlbe[h+ 0]=tlbe1;
		cpu->tlbe[h+64]=tlbe0;
		return((tlbe1&(~4095))|(addr&4095));
	}
	
	pgtab=cpu->regs[BTESH2_REG_TTB];
	pg=addr>>12;
	pgt=pg>>10;

	ptde=BTESH2_GetAddrDWordPhy(cpu, (pgtab&(~4095))+pgt*4);
	if(!BTESH2_FastMapCheckPteValid(cpu, ptde))
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(0);
	}

	pte=BTESH2_GetAddrDWordPhy(cpu, (ptde&(~4095))+(pg&1023)*4);
	if(!BTESH2_FastMapCheckPteValid(cpu, pte))
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(0);
	}
	
	tlbe2=(((u64)(addr&(~4095)))<<32)|pte;
	
	cpu->tlbe[h]=tlbe2;
	cpu->tlbe[h+64]=tlbe0;
	
	return((tlbe2&(~4095))|(addr&4095));
}

force_inline void BTESH2_FMMU_SetupForPSpan(BTESH2_CpuState *cpu)
{
	BTESH2_PhysSpan *sp, *sp1;
	
	sp=cpu->pspan;
	if(sp && (sp->flags&BTESH2_SPFL_SIMPLEMEM_LE))
	{
		cpu->pspan_pbase=sp->base;
		cpu->pspan_prng3=sp->range_n3;
		cpu->pspan_pdata=sp->data;
		return;
	}

	sp1=cpu->pspanb;
	if(sp1 && (sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE))
	{
		cpu->pspan_pbase=sp1->base;
		cpu->pspan_prng3=sp1->range_n3;
		cpu->pspan_pdata=sp1->data;
		return;
	}
	
	cpu->pspan_pbase=0;
	cpu->pspan_prng3=0;
	cpu->pspan_pdata=NULL;
}


int BTESH2_GetAddrByteFMMU_NoAT(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
//	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1, ra1;
	int i;

	addr1=addr&0x1FFFFFFF;
	
	ra1=addr1-cpu->pspan_pbase;
	if(ra1<cpu->pspan_prng3)
		{ return(*(sbyte *)(cpu->pspan_pdata+ra1)); }
	
//	mem=cpu->memory;
//	sp=mem->pspan;
	sp=cpu->pspan;
	if(sp && ((addr1-sp->base)<=sp->range_n3))
	{
		if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
			{ return(*(sbyte *)(sp->data+(addr1-sp->base))); }
		return(sp->GetB(sp, cpu, addr1-sp->base));
	}
//	sp1=mem->pspanb;
	sp1=cpu->pspanb;
	if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
	{
//		mem->pspan=sp1; mem->pspanb=sp;
		cpu->pspan=sp1; cpu->pspanb=sp;
		BTESH2_FMMU_SetupForPSpan(cpu);
		if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
			{ return(*(sbyte *)(sp1->data+(addr1-sp1->base))); }
		return(sp1->GetB(sp1, cpu, addr1-sp1->base));
	}

//	if((addr>>29)>=6)
//		addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrBytePhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

int BTESH2_GetAddrByteFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
//	BTESH2_MemoryImage *mem;
//	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;
	int i;

#if 1
	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_GetAddrByteFMMU_NoAT(cpu, addr));
	}
#endif

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrBytePhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

int BTESH2_GetAddrWordFMMU_NoAT(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
//	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1, ra1;
	int i;

	addr1=addr&0x1FFFFFFF;

	ra1=addr1-cpu->pspan_pbase;
	if(ra1<cpu->pspan_prng3)
		{ return(btesh2_gets16le(cpu->pspan_pdata+ra1)); }

//	mem=cpu->memory;
//	sp=mem->pspan;
	sp=cpu->pspan;

#if 0
	if(sp && ((addr1-sp->base)<=sp->range_n3))
	{
		if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
			{ return(btesh2_gets16le(sp->data+(addr1-sp->base))); }
		return(sp->GetW(sp, cpu, addr1-sp->base));
	}
#endif

#if 1
	if(sp)
	{
		ra1=addr1-sp->base;
		if(ra1<=sp->range_n3)
		{
			if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(btesh2_gets16le(sp->data+ra1)); }
			return(sp->GetW(sp, cpu, ra1));
		}
	}
#endif

//	sp1=mem->pspanb;
	sp1=cpu->pspanb;
//	if(sp1 && (addr1>=sp1->base) && ((addr1+3)<=sp1->limit))
	if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
	{
//		mem->pspan=sp1; mem->pspanb=sp;
		cpu->pspan=sp1; cpu->pspanb=sp;
		BTESH2_FMMU_SetupForPSpan(cpu);
		if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
			{ return(btesh2_gets16le(sp1->data+(addr1-sp1->base))); }
		return(sp1->GetW(sp1, cpu, addr1-sp1->base));
	}

//	if((addr>>29)>=6)
//		addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

int BTESH2_GetAddrWordFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
//	BTESH2_MemoryImage *mem;
//	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;
	int i;

#if 1
	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_GetAddrWordFMMU_NoAT(cpu, addr));
	}
#endif

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

default_inline u32 BTESH2_GetAddrDWordFMMU_NoAT(
	BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
//	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1, ra1;
	int i;

	addr1=addr&0x1FFFFFFF;

	ra1=addr1-cpu->pspan_pbase;
	if(ra1<cpu->pspan_prng3)
		{ return(btesh2_getu32le(cpu->pspan_pdata+ra1)); }

//	mem=cpu->memory;
//	sp=mem->pspan;
	sp=cpu->pspan;
#if 0
	if(sp && ((addr1-sp->base)<=sp->range_n3))
	{
		if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
			{ return(btesh2_getu32le(sp->data+(addr1-sp->base))); }
		return(sp->GetD(sp, cpu, addr1-sp->base));
	}
#endif

#if 1
	if(sp)
	{
		ra1=addr1-sp->base;
		if(ra1<=sp->range_n3)
		{
			if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(btesh2_getu32le(sp->data+ra1)); }
			return(sp->GetD(sp, cpu, ra1));
		}
	}
#endif

//	sp1=mem->pspanb;
	sp1=cpu->pspanb;
	if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
	{
//		mem->pspan=sp1; mem->pspanb=sp;
		cpu->pspan=sp1; cpu->pspanb=sp;
		BTESH2_FMMU_SetupForPSpan(cpu);
		if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
			{ return(btesh2_getu32le(sp1->data+(addr1-sp1->base))); }
		return(sp1->GetD(sp1, cpu, addr1-sp1->base));
	}

//	if((addr>>29)>=6)
//		addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrDWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

default_inline u32 BTESH2_GetAddrDWordFMMU(
	BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
	btesh2_vaddr addr1;
	int i;

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_GetAddrDWordFMMU_NoAT(cpu, addr));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_GetAddrDWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

default_inline u64 BTESH2_GetAddrQWordFMMU(
	BTESH2_CpuState *cpu, btesh2_vaddr addr)
{
	btesh2_vaddr addr1;
	u64 v;
	int i;

#if 0
	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_GetAddrQWordFMMU_NoAT(cpu, addr));
	}
#endif

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	v=BTESH2_GetAddrQWordPhy(cpu, addr1);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(v);
}

#if 0
int BTESH2_SetAddrByteFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);
	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrBytePhy2(cpu, addr1, val));
}
#endif

#if 1
force_inline int BTESH2_SetAddrByteFMMU_NoAT_I(
	BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1, ra1;
	int i;

	addr1=addr&0x1FFFFFFF;

	ra1=addr1-cpu->pspan_pbase;
	if(ra1<cpu->pspan_prng3)
		{ return(btesh2_setu8le(cpu->pspan_pdata+ra1, val)); }

	sp=cpu->pspan;
	if(sp && ((addr1-sp->base)<=sp->range_n3))
	{
		if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
		{
			btesh2_setu8le(sp->data+(addr1-sp->base), val);
			return(0);
		}
		return(sp->SetB(sp, cpu, addr1-sp->base, val));
	}
	sp1=cpu->pspanb;
	if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
	{
		cpu->pspan=sp1; cpu->pspanb=sp;
		BTESH2_FMMU_SetupForPSpan(cpu);
		if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
		{
			btesh2_setu8le(sp1->data+(addr1-sp1->base), val);
			return(0);
		}
		return(sp1->SetB(sp1, cpu, addr1-sp1->base, val));
	}

//	if((addr>>29)>=6)
//		addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_SetAddrBytePhy2(cpu, addr1, val);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

int BTESH2_SetAddrByteFMMU_NoAT(
	BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
//	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
//		return(-1);

	return(BTESH2_SetAddrByteFMMU_NoAT_I(cpu, addr, val));
}

int BTESH2_SetAddrByteFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
//	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
//		return(-1);

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_SetAddrByteFMMU_NoAT_I(cpu, addr, val));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrBytePhy2(cpu, addr1, val));
}
#endif


#if 0
int BTESH2_SetAddrWordFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
//	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		addr1=addr&0x1FFFFFFF;
//		mem=cpu->memory;
//		sp=mem->pspan;
		sp=cpu->pspan;
		if(sp && (addr1>=sp->base) && ((addr1+3)<=sp->limit))
			{ return(sp->SetW(sp, cpu, addr1-sp->base, val)); }
		return(BTESH2_SetAddrWordPhy2(cpu, addr1, val));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrWordPhy2(cpu, addr1, val));
}
#endif

#if 1
force_inline int BTESH2_SetAddrWordFMMU_NoAT_I(
	BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1, ra1;
	int i;

	addr1=addr&0x1FFFFFFF;

	ra1=addr1-cpu->pspan_pbase;
	if(ra1<cpu->pspan_prng3)
		{ btesh2_setu16le(cpu->pspan_pdata+ra1, val); return(0); }

	sp=cpu->pspan;
	if(sp && ((addr1-sp->base)<=sp->range_n3))
	{
		if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
		{
			btesh2_setu16le(sp->data+(addr1-sp->base), val);
			return(0);
		}
		return(sp->SetW(sp, cpu, addr1-sp->base, val));
	}
	sp1=cpu->pspanb;
	if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
	{
		cpu->pspan=sp1; cpu->pspanb=sp;
		BTESH2_FMMU_SetupForPSpan(cpu);
		if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
		{
			btesh2_setu16le(sp1->data+(addr1-sp1->base), val);
			return(0);
		}
		return(sp1->SetW(sp1, cpu, addr1-sp1->base, val));
	}

//	if((addr>>29)>=6)
//		addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_SetAddrWordPhy2(cpu, addr1, val);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

int BTESH2_SetAddrWordFMMU_NoAT(
	BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
//	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
//		return(-1);

	return(BTESH2_SetAddrWordFMMU_NoAT_I(cpu, addr, val));
}

int BTESH2_SetAddrWordFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
//	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
//		return(-1);

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_SetAddrWordFMMU_NoAT_I(cpu, addr, val));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrWordPhy2(cpu, addr1, val));
}
#endif

force_inline int BTESH2_SetAddrDWordFMMU_NoAT_I(
	BTESH2_CpuState *cpu, btesh2_vaddr addr, u32 val)
{
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1, ra1;
	int i;

	addr1=addr&0x1FFFFFFF;

	ra1=addr1-cpu->pspan_pbase;
	if(ra1<cpu->pspan_prng3)
		{ btesh2_setu32le(cpu->pspan_pdata+ra1, val); return(0); }

	sp=cpu->pspan;
	if(sp && ((addr1-sp->base)<=sp->range_n3))
	{
		if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
		{
			btesh2_setu32le(sp->data+(addr1-sp->base), val);
			return(0);
		}
		return(sp->SetD(sp, cpu, addr1-sp->base, val));
	}
	sp1=cpu->pspanb;
	if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
	{
		cpu->pspan=sp1; cpu->pspanb=sp;
		BTESH2_FMMU_SetupForPSpan(cpu);
		if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
		{
			btesh2_setu32le(sp1->data+(addr1-sp1->base), val);
			return(0);
		}
		return(sp1->SetD(sp1, cpu, addr1-sp1->base, val));
	}

//	if((addr>>29)>=6)
//		addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	i=BTESH2_SetAddrDWordPhy2(cpu, addr1, val);
	BTESH2_FMMU_SetupForPSpan(cpu);
	return(i);
}

int BTESH2_SetAddrDWordFMMU_NoAT(BTESH2_CpuState *cpu,
	btesh2_vaddr addr, u32 val)
{
//	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
//	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
//		return(-1);

	return(BTESH2_SetAddrDWordFMMU_NoAT_I(cpu, addr, val));
}

int BTESH2_SetAddrDWordFMMU(BTESH2_CpuState *cpu,
	btesh2_vaddr addr, u32 val)
{
//	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
//	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
//		return(-1);

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_SetAddrDWordFMMU_NoAT_I(cpu, addr, val));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrDWordPhy2(cpu, addr1, val));
}

int BTESH2_SetAddrQWordFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, u64 val)
{
//	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	btesh2_vaddr addr1;

	if(cpu->status)
		return(-1);
//	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
//		return(-1);

#if 0
	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		return(BTESH2_SetAddrQWordFMMU_NoAT_I(cpu, addr, val));
	}
#endif

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrQWordPhy2(cpu, addr1, val));
}

int BTESH2_SetupUpdateFMMU(BTESH2_CpuState *cpu)
{
	if(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)
	{
		cpu->GetAddrByte=BTESH2_GetAddrByteFMMU;
		cpu->GetAddrWord=BTESH2_GetAddrWordFMMU;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordFMMU;
		cpu->GetAddrQWord=BTESH2_GetAddrQWordFMMU;

		cpu->SetAddrByte=BTESH2_SetAddrByteFMMU;
		cpu->SetAddrWord=BTESH2_SetAddrWordFMMU;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordFMMU;
		cpu->SetAddrQWord=BTESH2_SetAddrQWordFMMU;
	}else
	{
		cpu->GetAddrByte=BTESH2_GetAddrByteFMMU_NoAT;
		cpu->GetAddrWord=BTESH2_GetAddrWordFMMU_NoAT;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordFMMU_NoAT;
//		cpu->GetAddrQWord=BTESH2_GetAddrQWordFMMU_NoAT;
		cpu->GetAddrQWord=BTESH2_GetAddrQWordFMMU;

		cpu->SetAddrByte=BTESH2_SetAddrByteFMMU_NoAT;
		cpu->SetAddrWord=BTESH2_SetAddrWordFMMU_NoAT;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordFMMU_NoAT;
//		cpu->SetAddrQWord=BTESH2_SetAddrQWordFMMU_NoAT;
		cpu->SetAddrQWord=BTESH2_SetAddrQWordFMMU;
	}
	cpu->jit_needflush=1;
	return(0);
}