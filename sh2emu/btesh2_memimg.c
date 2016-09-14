BTESH2_MemoryImage *BTESH2_AllocMemoryImage(int lim)
{
	BTESH2_MemoryImage *img;
	
	img=malloc(sizeof(BTESH2_MemoryImage));
	memset(img, 0, sizeof(BTESH2_MemoryImage));
	
	if(lim>32)
	{
		img->span=malloc(lim*sizeof(BTESH2_PhysSpan *));
		img->mspan=lim;
	}else
	{
		img->span=img->t_span;
		img->mspan=32;
	}
	return(img);
}

int BTESH2_MemoryAddSpan(BTESH2_MemoryImage *img, BTESH2_PhysSpan *span)
{
	int i;
	
	i=img->nspan++;
	while(i>0)
	{
		if(img->span[i-1]->base<=span->base)
			break;
		img->span[i]=img->span[i-1];
		i--;
	}
	img->span[i]=span;
	return(i);
}

BTESH2_PhysSpan *BTESH2_AllocPhysSpan(void)
{
	BTESH2_PhysSpan *sp;
	
	sp=malloc(sizeof(BTESH2_PhysSpan));
	memset(sp, 0, sizeof(BTESH2_PhysSpan));
	return(sp);
}

u32 btesh2_spandfl_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	return(*(sbyte *)(sp->data+reladdr));
}

u32 btesh2_spandfl_GetW_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	byte *ptr;
	int i;

	ptr=sp->data+reladdr;
	i=(ptr[0]<<8)|(ptr[1]);
	return((s16)i);
}

u32 btesh2_spandfl_GetW_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	byte *ptr;
	int i;

	ptr=sp->data+reladdr;
	i=(ptr[1]<<8)|(ptr[0]);
	return((s16)i);
}

u32 btesh2_spandfl_GetD_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	byte *ptr;
	u32 i;
	
	ptr=sp->data+reladdr;
	i=(((u32)(ptr[0]))<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3]);
	return(i);
}

u32 btesh2_spandfl_GetD_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	byte *ptr;
	u32 i;
	
	ptr=sp->data+reladdr;
	i=(((u32)(ptr[3]))<<24)|(ptr[2]<<16)|(ptr[1]<<8)|(ptr[0]);
	return(i);
}

int btesh2_spandfl_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	*ptr=val;
	return(0);
}

int btesh2_spandfl_SetW_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[0]=val>>8;
	ptr[1]=val;
	return(0);
}

int btesh2_spandfl_SetW_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[1]=val>>8;
	ptr[0]=val;
	return(0);
}

int btesh2_spandfl_SetD_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[0]=val>>24;
	ptr[1]=val>>16;
	ptr[2]=val>> 8;
	ptr[3]=val;
	return(0);
}

int btesh2_spandfl_SetD_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[3]=val>>24;
	ptr[2]=val>>16;
	ptr[1]=val>> 8;
	ptr[0]=val;
	return(0);
}

u32 btesh2_spandfl_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	if(cpu->csfl&1)
	{
		sp->GetW=btesh2_spandfl_GetW_LE;
		return(sp->GetW(sp, cpu, reladdr));
	}else
	{
		sp->GetW=btesh2_spandfl_GetW_BE;
		return(sp->GetW(sp, cpu, reladdr));
	}
}

u32 btesh2_spandfl_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	if(cpu->csfl&1)
	{
		sp->GetD=btesh2_spandfl_GetD_LE;
		return(sp->GetD(sp, cpu, reladdr));
	}else
	{
		sp->GetD=btesh2_spandfl_GetD_BE;
		return(sp->GetD(sp, cpu, reladdr));
	}
}

int btesh2_spandfl_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	if(cpu->csfl&1)
	{
		sp->SetW=btesh2_spandfl_SetW_LE;
		return(sp->SetW(sp, cpu, reladdr, val));
	}else
	{
		sp->SetW=btesh2_spandfl_SetW_BE;
		return(sp->SetW(sp, cpu, reladdr, val));
	}
}

int btesh2_spandfl_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	if(cpu->csfl&1)
	{
		sp->SetD=btesh2_spandfl_SetD_LE;
		return(sp->SetD(sp, cpu, reladdr, val));
	}else
	{
		sp->SetD=btesh2_spandfl_SetD_BE;
		return(sp->SetD(sp, cpu, reladdr, val));
	}
}

int BTESH2_MemoryDefineSpan(BTESH2_MemoryImage *img,
	u32 base, u32 limit, byte *data, char *name)
{
	BTESH2_PhysSpan *sp;
	int i, sz;
	
	if(!data)
	{
		sz=(limit-base)+1;
		data=malloc(sz);
		memset(data, 0, sz);
	}

	sp=BTESH2_AllocPhysSpan();
	sp->base=base;
	sp->limit=limit;
	sp->data=data;
	sp->name=name;
	
	sp->GetB=btesh2_spandfl_GetB;
	sp->GetW=btesh2_spandfl_GetW;
	sp->GetD=btesh2_spandfl_GetD;

	sp->SetB=btesh2_spandfl_SetB;
	sp->SetW=btesh2_spandfl_SetW;
	sp->SetD=btesh2_spandfl_SetD;
	
	i=BTESH2_MemoryAddSpan(img, sp);
	return(i);
}

BTESH2_PhysSpan *BTESH2_GetSpanForAddr(BTESH2_CpuState *cpu,
	u32 addr, u32 lim)
{
	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp;
	int ns, is, ms;
	int l, h, r, m;

	if(addr>lim)
		return(NULL);

	mem=cpu->memory;
	ns=mem->nspan;

	sp=mem->pspan;
	if(sp)
	{
		if((addr>=sp->base) && (lim<=sp->limit))
			return(sp);
	}
	
	if(ns<5)
	{
		for(is=0; is<ns; is++)
		{
			sp=mem->span[is];
			if((addr>=sp->base) && (lim<=sp->limit))
				{ mem->pspan=sp; return(sp); }
		}
		return(NULL);
	}

#if 0
	is=0;
	while(ns>0)
	{
		ms=is+(ns>>1);
		sp=mem->span[ms];
		if((addr>=sp->base) && (lim<=sp->limit))
			{ mem->pspan=sp; return(sp); }
		if(addr>sp->base)
			{ is=ms+1; ns=ns-is; }
		else
			{ ns=ms-is; }
	}
#endif

#if 1
	l=0; h=ns;
	r=h-l; m=l+(r>>1);
	while(r>0)
	{
		m=l+(r>>1);
		sp=mem->span[m];
		if((addr>=sp->base) && (lim<=sp->limit))
			{ mem->pspan=sp; return(sp); }
		if(addr>sp->base)
			{ l=m+1; r=h-l; }
		else
			{ h=m; r=h-l; }
	}
#endif

	return(NULL);
}

int BTESH2_ThrowTrap(BTESH2_CpuState *cpu, int status)
{
	int i;

	if(cpu->status)
		return(-1);

	cpu->status=status;
	for(i=0; i<64; i++)
		cpu->trapregs[i]=cpu->regs[i];
	for(i=0; i<16; i++)
		cpu->trapfregs[i]=cpu->fregs[i];
	return(0);
}

int BTESH2_RestoreTrap(BTESH2_CpuState *cpu)
{
	int i;

	for(i=0; i<64; i++)
		cpu->regs[i]=cpu->trapregs[i];
	for(i=0; i<16; i++)
		cpu->fregs[i]=cpu->trapfregs[i];
	return(0);
}

int BTESH2_CheckAddrTrapSmc(BTESH2_CpuState *cpu, u32 addr, u32 val)
{
	BTESH2_Trace *tr;
	int pg, bp, bm;
	int i, j, k, st;

	pg=addr>>12;
	bp=pg&16383;
	bm=pg>>14;

	if(!cpu->smcdbm[bm])
		return(0);

	if(cpu->smcdbm[bm][bp>>5]&(1<<(bp&31)))
	{
//		printf("Check SMC %08X\n", addr);
		st=0;
		for(i=0; i<BTESH2_TR_HASHSZ; i++)
		{
			tr=cpu->icache[i];
			if(!tr)
				continue;
			if((addr>=tr->srcpc) && (addr<tr->maxpc))
			{
//				printf("Flush SMC %08X Tr=%08X..%08X V=%08X\n",
//					addr, tr->srcpc, tr->maxpc, val);
				if(tr==cpu->cur_trace)
				{
					BTESH2_ThrowTrap(cpu, BTESH2_EXC_TRAPSMC);
					st=1;
					continue;
				}
				BTESH2_FlushTrace(cpu, tr);
				BTESH2_FreeTrace(cpu, tr);
				cpu->icache[i]=NULL;
				break;
			}
		}
		if(!st)
		{
			cpu->smcdbm[bm][bp>>5]&=~(1<<(bp&31));
		}
		return(st);
	}
	return(0);
}

int BTESH2_MarkAddrTrapSmc(BTESH2_CpuState *cpu, u32 addr)
{
	int pg, bp, bm;
	int i, j, k, st;

	pg=addr>>12;
	bp=pg&16383;
	bm=pg>>14;

	if(!cpu->smcdbm[bm])
	{
		cpu->smcdbm[bm]=malloc(512*sizeof(u32));
//		return(0);
	}

	cpu->smcdbm[bm][bp>>5]|=(1<<(bp&31));
	return(0);
}

int BTESH2_GetAddrBytePhy(BTESH2_CpuState *cpu, u32 addr)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->GetB(sp, cpu, addr-sp->base));
}

int BTESH2_GetAddrWordPhy(BTESH2_CpuState *cpu, u32 addr)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->GetW(sp, cpu, addr-sp->base));
}

u32 BTESH2_GetAddrDWordPhy(BTESH2_CpuState *cpu, u32 addr)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	u32 i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+3);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return((u32)(-1));
	}
	return(sp->GetD(sp, cpu, addr-sp->base));
}

int BTESH2_SetAddrBytePhy(BTESH2_CpuState *cpu, u32 addr, int val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);

	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetB(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrWordPhy(BTESH2_CpuState *cpu, u32 addr, int val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);

	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetW(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrDWordPhy(BTESH2_CpuState *cpu, u32 addr, u32 val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);

	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+3);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetD(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrBytePhy2(BTESH2_CpuState *cpu, u32 addr, int val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetB(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrWordPhy2(BTESH2_CpuState *cpu, u32 addr, int val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetW(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrDWordPhy2(BTESH2_CpuState *cpu, u32 addr, u32 val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+3);
	if(!sp)
	{
		cpu->regs[BTESH2_REG_FLA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetD(sp, cpu, addr-sp->base, val));
}

int BTESH2_MemCpyIn(BTESH2_CpuState *cpu, u32 addr,
	byte *buf, int sz)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+sz-1);
	if(!sp || !sp->data)
		{ return(-1); }
	memcpy(sp->data+(addr-sp->base), buf, sz);
	return(0);
}

int BTESH2_GetAddrByte(BTESH2_CpuState *cpu, u32 addr)
	{ return(cpu->GetAddrByte(cpu, addr)); }
int BTESH2_GetAddrWord(BTESH2_CpuState *cpu, u32 addr)
	{ return(cpu->GetAddrWord(cpu, addr)); }
u32 BTESH2_GetAddrDWord(BTESH2_CpuState *cpu, u32 addr)
	{ return(cpu->GetAddrDWord(cpu, addr)); }
int BTESH2_SetAddrByte(BTESH2_CpuState *cpu, u32 addr, int val)
	{ return(cpu->SetAddrByte(cpu, addr, val)); }
int BTESH2_SetAddrWord(BTESH2_CpuState *cpu, u32 addr, int val)
	{ return(cpu->SetAddrWord(cpu, addr, val)); }
int BTESH2_SetAddrDWord(BTESH2_CpuState *cpu, u32 addr, u32 val)
	{ return(cpu->SetAddrDWord(cpu, addr, val)); }
