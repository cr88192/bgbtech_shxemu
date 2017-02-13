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

u32 BTESH2_FastMapVirtToPhys(BTESH2_CpuState *cpu, u32 addr)
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
		if((addr>>29)==7)
			return(addr);
		if((addr>>29)!=6)
		{
//			if(addr!=(addr&0x1FFFFFFFU))
//				{ h=-1; }
			return(addr&0x1FFFFFFFU);
//		return(addr);
		}
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

int BTESH2_GetAddrByteFMMU(BTESH2_CpuState *cpu, u32 addr)
{
	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	u32 addr1;

#if 1
	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		addr1=addr&0x1FFFFFFF;
		mem=cpu->memory;
		sp=mem->pspan;
		if(sp && ((addr1-sp->base)<=sp->range_n3))
		{
			if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(*(sbyte *)(sp->data+(addr1-sp->base))); }
			return(sp->GetB(sp, cpu, addr1-sp->base));
		}
		sp1=mem->pspanb;
		if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
		{
			mem->pspan=sp1; mem->pspanb=sp;
			if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(*(sbyte *)(sp1->data+(addr1-sp1->base))); }
			return(sp1->GetB(sp1, cpu, addr1-sp1->base));
		}
		return(BTESH2_GetAddrBytePhy(cpu, addr1));
	}
#endif

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_GetAddrBytePhy(cpu, addr1));
}

int BTESH2_GetAddrWordFMMU(BTESH2_CpuState *cpu, u32 addr)
{
	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	u32 addr1;

#if 1
	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		addr1=addr&0x1FFFFFFF;
		mem=cpu->memory;
		sp=mem->pspan;
		if(sp && ((addr1-sp->base)<=sp->range_n3))
		{
			if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(btesh2_gets16le(sp->data+(addr1-sp->base))); }
			return(sp->GetW(sp, cpu, addr1-sp->base));
		}
		sp1=mem->pspanb;
//		if(sp1 && (addr1>=sp1->base) && ((addr1+3)<=sp1->limit))
		if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
		{
			mem->pspan=sp1; mem->pspanb=sp;
			if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(btesh2_gets16le(sp1->data+(addr1-sp1->base))); }
			return(sp1->GetW(sp1, cpu, addr1-sp1->base));
		}
		return(BTESH2_GetAddrWordPhy(cpu, addr1));
	}
#endif

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_GetAddrWordPhy(cpu, addr1));
}

default_inline u32 BTESH2_GetAddrDWordFMMU(BTESH2_CpuState *cpu, u32 addr)
{
	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	u32 addr1;

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		addr1=addr&0x1FFFFFFF;
		mem=cpu->memory;
		sp=mem->pspan;
//		if(sp && (addr1>=sp->base) && ((addr1+3)<=sp->limit))
		if(sp && ((addr1-sp->base)<=sp->range_n3))
		{
			if(sp->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(btesh2_getu32le(sp->data+(addr1-sp->base))); }
			return(sp->GetD(sp, cpu, addr1-sp->base));
		}
		sp1=mem->pspanb;
//		if(sp1 && (addr1>=sp1->base) && ((addr1+3)<=sp1->limit))
		if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
		{
			mem->pspan=sp1; mem->pspanb=sp;
			if(sp1->flags&BTESH2_SPFL_SIMPLEMEM_LE)
				{ return(btesh2_getu32le(sp1->data+(addr1-sp1->base))); }
			return(sp1->GetD(sp1, cpu, addr1-sp1->base));
		}
		return(BTESH2_GetAddrDWordPhy(cpu, addr1));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_GetAddrDWordPhy(cpu, addr1));
}

int BTESH2_SetAddrByteFMMU(BTESH2_CpuState *cpu, u32 addr, int val)
{
	u32 addr1;

	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);
	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrBytePhy2(cpu, addr1, val));
}

int BTESH2_SetAddrWordFMMU(BTESH2_CpuState *cpu, u32 addr, int val)
{
	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp;
	u32 addr1;

	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		addr1=addr&0x1FFFFFFF;
		mem=cpu->memory;
		sp=mem->pspan;
		if(sp && (addr1>=sp->base) && ((addr1+3)<=sp->limit))
			{ return(sp->SetW(sp, cpu, addr1-sp->base, val)); }
		return(BTESH2_SetAddrWordPhy2(cpu, addr1, val));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrWordPhy2(cpu, addr1, val));
}

int BTESH2_SetAddrDWordFMMU(BTESH2_CpuState *cpu, u32 addr, u32 val)
{
	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	u32 addr1;

	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);

	if(((addr&0x80000000) ||
		!(cpu->regs[BTESH2_REG_MMUCR]&BTESH2_MMUCR_AT)) &&
		((addr>>29)<6))
	{
		addr1=addr&0x1FFFFFFF;
		mem=cpu->memory;
		sp=mem->pspan;
//		if(sp && (addr1>=sp->base) && ((addr1+3)<=sp->limit))
		if(sp && ((addr1-sp->base)<=sp->range_n3))
			{ return(sp->SetD(sp, cpu, addr1-sp->base, val)); }
		sp1=mem->pspan;
//		if(sp1 && (addr1>=sp1->base) && ((addr1+3)<=sp1->limit))
		if(sp1 && ((addr1-sp1->base)<=sp1->range_n3))
		{
			mem->pspan=sp1; mem->pspanb=sp;
			return(sp1->SetD(sp1, cpu, addr1-sp1->base, val));
		}
		return(BTESH2_SetAddrDWordPhy2(cpu, addr1, val));
	}

	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrDWordPhy2(cpu, addr1, val));
}
