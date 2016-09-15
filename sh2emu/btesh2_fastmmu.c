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
		
	if(addr&0x80000000)
	{
		if((addr>>29)==7)
			return(addr);
		if((addr>>29)!=6)
		{
			return(addr&0x1FFFFFFF);
		}
	}
	
	addr=addr&0x1FFFFFFFU;
	
	tlbh=(addr>>12)*65521;
	h=(tlbh>>16)&63;
	tlbe0=cpu->tlbe[h];
	if(((tlbe0>>32)&(~4095))==(addr&(~4095)))
		return((tlbe0&(~4095))|(addr&4095));
	tlbe1=cpu->tlbe[h+64];
	if(((tlbe1>>32)&(~4095))==(addr&(~4095)))
		return((tlbe1&(~4095))|(addr&4095));
	
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
	u32 addr1;
	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_GetAddrBytePhy(cpu, addr1));
}

int BTESH2_GetAddrWordFMMU(BTESH2_CpuState *cpu, u32 addr)
{
	u32 addr1;
	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_GetAddrWordPhy(cpu, addr1));
}

u32 BTESH2_GetAddrDWordFMMU(BTESH2_CpuState *cpu, u32 addr)
{
	u32 addr1;
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
	u32 addr1;

	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);
	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrWordPhy2(cpu, addr1, val));
}

int BTESH2_SetAddrDWordFMMU(BTESH2_CpuState *cpu, u32 addr, u32 val)
{
	u32 addr1;

	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);
	addr1=BTESH2_FastMapVirtToPhys(cpu, addr);
	return(BTESH2_SetAddrDWordPhy2(cpu, addr1, val));
}
