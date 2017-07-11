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
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	return(*(sbyte *)(sp->data+reladdr));
}

u32 btesh2_spandfl_GetW_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	byte *ptr;
	int i;

	ptr=sp->data+reladdr;
//	i=(ptr[0]<<8)|(ptr[1]);
//	return((s16)i);
	i=btesh2_gets16be(ptr);
	return(i);
}

u32 btesh2_spandfl_GetW_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	byte *ptr;
	int i;

	ptr=sp->data+reladdr;

	i=btesh2_gets16le(ptr);
	return(i);

#if 0
#if defined(X86) || defined(X86_64)
	return(*(s16 *)ptr);
#else
	i=(ptr[1]<<8)|(ptr[0]);
	return((s16)i);
#endif
#endif
}

u32 btesh2_spandfl_GetD_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	byte *ptr;
	u32 i;
	
	ptr=sp->data+reladdr;
//	i=(((u32)(ptr[0]))<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3]);
	i=btesh2_getu32be(ptr);
	return(i);
}

u32 btesh2_spandfl_GetD_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	byte *ptr;
	u32 i;
	
	ptr=sp->data+reladdr;

	i=btesh2_getu32le(ptr);

#if 0
#if defined(X86) || defined(X86_64)
	i=*(u32 *)ptr;
#else
	i=(((u32)(ptr[3]))<<24)|(ptr[2]<<16)|(ptr[1]<<8)|(ptr[0]);
#endif
#endif

	return(i);
}

int btesh2_spandfl_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	*ptr=val;
	return(0);
}

int btesh2_spandfl_SetW_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[0]=val>>8;
	ptr[1]=val;
	return(0);
}

int btesh2_spandfl_SetW_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
#if defined(X86) || defined(X86_64)
	*(u16 *)ptr=val;
#else
	ptr[1]=val>>8;
	ptr[0]=val;
#endif
	return(0);
}

int btesh2_spandfl_SetD_BE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
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
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;

#if defined(X86) || defined(X86_64)
	*(u32 *)ptr=val;
#else
	ptr[3]=val>>24;
	ptr[2]=val>>16;
	ptr[1]=val>> 8;
	ptr[0]=val;
#endif

	return(0);
}


#if 1
int btesh2_spandfl_SetB_Fl(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	*ptr=val;
	*sp->dmdflag|=1;
	return(0);
}

int btesh2_spandfl_SetW_FlBE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[0]=val>>8;
	ptr[1]=val;
	*sp->dmdflag|=1;
	return(0);
}

int btesh2_spandfl_SetW_FlLE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[1]=val>>8;
	ptr[0]=val;
	*sp->dmdflag|=1;
	return(0);
}

int btesh2_spandfl_SetD_FlBE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[0]=val>>24;
	ptr[1]=val>>16;
	ptr[2]=val>> 8;
	ptr[3]=val;
	*sp->dmdflag|=1;
	return(0);
}

int btesh2_spandfl_SetD_FlLE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
#if defined(X86) || defined(X86_64)
	*(u32 *)ptr=val;
#else
	ptr[3]=val>>24;
	ptr[2]=val>>16;
	ptr[1]=val>> 8;
	ptr[0]=val;
#endif
	*sp->dmdflag|=1;
	return(0);
}
#endif

u32 btesh2_spandfl_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	if(cpu->csfl&BTESH2_CSFL_LE)
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
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	if(cpu->csfl&BTESH2_CSFL_LE)
	{
		sp->GetD=btesh2_spandfl_GetD_LE;
		if(!sp->dmdflag)
			sp->flags|=BTESH2_SPFL_SIMPLEMEM_LE;
		return(sp->GetD(sp, cpu, reladdr));
	}else
	{
		sp->GetD=btesh2_spandfl_GetD_BE;
		return(sp->GetD(sp, cpu, reladdr));
	}
}

u64 btesh2_spandfl_GetQ(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	if(cpu->csfl&BTESH2_CSFL_LE)
	{
//		sp->GetD=btesh2_spandfl_GetD_LE;
		if(!sp->dmdflag)
			sp->flags|=BTESH2_SPFL_SIMPLEMEM_LE;
		return(sp->GetD(sp, cpu, reladdr+0)+
			(((u64)sp->GetD(sp, cpu, reladdr+4))<<32));
	}else
	{
//		sp->GetD=btesh2_spandfl_GetD_BE;
		return(sp->GetD(sp, cpu, reladdr+4)+
			(((u64)sp->GetD(sp, cpu, reladdr+0))<<32));
	}
}

int btesh2_spandfl_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	if(cpu->csfl&BTESH2_CSFL_LE)
	{
		sp->SetW=btesh2_spandfl_SetW_LE;
		if(sp->dmdflag)
			sp->SetW=btesh2_spandfl_SetW_FlLE;
		return(sp->SetW(sp, cpu, reladdr, val));
	}else
	{
		sp->SetW=btesh2_spandfl_SetW_BE;
		if(sp->dmdflag)
			sp->SetW=btesh2_spandfl_SetW_FlBE;
		return(sp->SetW(sp, cpu, reladdr, val));
	}
}

int btesh2_spandfl_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	if(cpu->csfl&BTESH2_CSFL_LE)
	{
		sp->SetD=btesh2_spandfl_SetD_LE;
		if(sp->dmdflag)
			sp->SetD=btesh2_spandfl_SetD_FlLE;
		return(sp->SetD(sp, cpu, reladdr, val));
	}else
	{
		sp->SetD=btesh2_spandfl_SetD_BE;
		if(sp->dmdflag)
			sp->SetD=btesh2_spandfl_SetD_FlBE;
		return(sp->SetD(sp, cpu, reladdr, val));
	}
}

int btesh2_spandfl_SetQ(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u64 val)
{
	if(cpu->csfl&BTESH2_CSFL_LE)
	{
//		sp->SetD=btesh2_spandfl_SetD_LE;
//		if(sp->dmdflag)
//			sp->SetD=btesh2_spandfl_SetD_FlLE;
		sp->SetD(sp, cpu, reladdr+0, val);
		sp->SetD(sp, cpu, reladdr+4, val>>32);
		return(0);
	}else
	{
		sp->SetD(sp, cpu, reladdr+4, val);
		sp->SetD(sp, cpu, reladdr+0, val>>32);
		return(0);
//		sp->SetD=btesh2_spandfl_SetD_BE;
//		if(sp->dmdflag)
//			sp->SetD=btesh2_spandfl_SetD_FlBE;
//		return(sp->SetD(sp, cpu, reladdr, val));
	}
}

int BTESH2_MemoryDefineSpan(BTESH2_MemoryImage *img,
	btesh2_paddr base, btesh2_paddr limit, byte *data, char *name)
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
	sp->range_n3=limit-base-3;
	sp->data=data;
	sp->name=name;
	
	sp->GetB=btesh2_spandfl_GetB;
	sp->GetW=btesh2_spandfl_GetW;
	sp->GetD=btesh2_spandfl_GetD;
	sp->GetQ=btesh2_spandfl_GetQ;

	sp->SetB=btesh2_spandfl_SetB;
	sp->SetW=btesh2_spandfl_SetW;
	sp->SetD=btesh2_spandfl_SetD;
	sp->SetQ=btesh2_spandfl_SetQ;
	
	i=BTESH2_MemoryAddSpan(img, sp);
	return(i);
}

int BTESH2_MemoryDefineSpan_InitFF(BTESH2_MemoryImage *img,
	btesh2_paddr base, btesh2_paddr limit, byte *data, char *name)
{
	BTESH2_PhysSpan *sp;
	int i;
	
	i=BTESH2_MemoryDefineSpan(img, base, limit, data, name);
	sp=img->span[i];
	memset(sp->data, 0xFF, limit-base);
	return(i);
}

void btesh2_spandmd_demandInit(BTESH2_PhysSpan *sp)
{
	byte **refptr;
	byte *data;
	int i, sz;
	
	refptr=sp->dmdaddr;

	sz=(sp->limit-sp->base)+1;
	data=malloc(sz);
	memset(data, 0, sz);
	sp->data=data;
	
	if(refptr)
		*refptr=data;

	sp->GetB=btesh2_spandfl_GetB;
	sp->GetW=btesh2_spandfl_GetW;
	sp->GetD=btesh2_spandfl_GetD;
	sp->GetQ=btesh2_spandfl_GetQ;

	sp->SetB=btesh2_spandfl_SetB;
	sp->SetW=btesh2_spandfl_SetW;
	sp->SetD=btesh2_spandfl_SetD;
	sp->SetQ=btesh2_spandfl_SetQ;

	if(sp->dmdflag)
		sp->SetB=btesh2_spandfl_SetB_Fl;
}

u32 btesh2_spandmd_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->GetB(sp, cpu, reladdr));
}

u32 btesh2_spandmd_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->GetW(sp, cpu, reladdr));
}

u32 btesh2_spandmd_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->GetD(sp, cpu, reladdr));
}

u64 btesh2_spandmd_GetQ(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->GetQ(sp, cpu, reladdr));
}

int btesh2_spandmd_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->SetB(sp, cpu, reladdr, val));
}

int btesh2_spandmd_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->SetW(sp, cpu, reladdr, val));
}

int btesh2_spandmd_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->SetD(sp, cpu, reladdr, val));
}

int btesh2_spandmd_SetQ(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u64 val)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->SetQ(sp, cpu, reladdr, val));
}

int BTESH2_MemoryDefineSpanDemand(BTESH2_MemoryImage *img,
	btesh2_paddr base, btesh2_paddr limit, byte **addr, int *flag, char *name)
{
	BTESH2_PhysSpan *sp;
	int i, sz;
	
	sp=BTESH2_AllocPhysSpan();
	sp->base=base;
	sp->limit=limit;
	sp->range_n3=limit-base-3;
	sp->dmdaddr=addr;
	sp->dmdflag=flag;
	sp->name=name;
	
	sp->GetB=btesh2_spandmd_GetB;
	sp->GetW=btesh2_spandmd_GetW;
	sp->GetD=btesh2_spandmd_GetD;
	sp->GetQ=btesh2_spandmd_GetQ;

	sp->SetB=btesh2_spandmd_SetB;
	sp->SetW=btesh2_spandmd_SetW;
	sp->SetD=btesh2_spandmd_SetD;
	sp->SetQ=btesh2_spandmd_SetQ;
	
	i=BTESH2_MemoryAddSpan(img, sp);
	return(i);
}


u32 btesh2_spanreg_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	u32 i, j;
	i=sp->GetD(sp, cpu, reladdr&(~3));
	if(cpu->csfl&1)
	{
		j=(i>>((reladdr&3)*8));
	}else
	{
		j=(i>>((3-(reladdr&3))*8));
	}
	j=(sbyte)j;
	return(j);
}

u32 btesh2_spanreg_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	u32 i, j;
	i=sp->GetD(sp, cpu, reladdr&(~3));
	if(cpu->csfl&1)
	{
		j=(i>>((reladdr&3)*8));
	}else
	{
		j=(i>>((3-(reladdr&3))*8));
	}
	j=(s16)j;
	return(j);
}

int btesh2_spanreg_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	int i;
	i=sp->SetD(sp, cpu, reladdr, val);
	return(i);
}

int btesh2_spanreg_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	int i;
	i=sp->SetD(sp, cpu, reladdr, val);
	return(i);
}

int BTESH2_MemoryDefineSpanRegs(BTESH2_MemoryImage *img,
	btesh2_paddr base, btesh2_paddr limit, char *name,
	u32 (*GetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu,
		btesh2_paddr reladdr),
	int (*SetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu,
		btesh2_paddr reladdr, u32 val)
	)
{
	BTESH2_PhysSpan *sp;
	int i, sz;
	
	sp=BTESH2_AllocPhysSpan();
	sp->base=base;
	sp->limit=limit;
	sp->range_n3=limit-base-3;
//	sp->data=(byte *)data;
	sp->name=name;

	sp->GetD=GetD;
	sp->SetD=SetD;
	
	sp->GetB=btesh2_spanreg_GetB;
	sp->GetW=btesh2_spanreg_GetW;
	sp->SetB=btesh2_spanreg_SetB;
	sp->SetW=btesh2_spanreg_SetW;
	
	i=BTESH2_MemoryAddSpan(img, sp);
	return(i);
}


BTESH2_PhysSpan *BTESH2_GetSpanForAddr(BTESH2_CpuState *cpu,
	btesh2_paddr addr, btesh2_paddr lim)
{
	BTESH2_MemoryImage *mem;
	BTESH2_PhysSpan *sp, *sp1;
	int ns, is, ms;
	int l, h, r, m;

	if(addr>lim)
		return(NULL);

	mem=cpu->memory;

//	sp=mem->pspan;
	sp=cpu->pspan;
//	if(sp && ((addr-sp->base)<=sp->range_n3))
//		return(sp);

	if(sp)
	{
		if((addr>=sp->base) && (lim<=sp->limit))
			return(sp);
	}

//	sp1=mem->pspanb;
	sp1=cpu->pspanb;

#if 0
	if(sp1 && ((addr-sp1->base)<=sp1->range_n3))
	{
		mem->pspan=sp1;
		mem->pspanb=sp;
		return(sp1);
	}
#endif

#if 1
	if(sp1)
	{
		if((addr>=sp1->base) && (lim<=sp1->limit))
		{
//			mem->pspan=sp1;
//			mem->pspanb=sp;
			cpu->pspan=sp1;
			cpu->pspanb=sp;			
			return(sp1);
		}
	}
#endif

	ns=mem->nspan;
	sp1=sp;
	
	if(ns<5)
	{
		for(is=0; is<ns; is++)
		{
			sp=mem->span[is];
			if((addr>=sp->base) && (lim<=sp->limit))
			{
//				mem->pspanb=mem->pspan;
//				mem->pspanb=sp1;
//				mem->pspan=sp;

				cpu->pspanb=sp1;
				cpu->pspan=sp;
				return(sp);
			}
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
		{
//			mem->pspanb=mem->pspan;
//			mem->pspanb=sp1;
//			mem->pspan=sp;
			cpu->pspanb=sp1;
			cpu->pspan=sp;
			return(sp);
		}
		if(addr>sp->base)
			{ l=m+1; r=h-l; }
		else
			{ h=m; r=h-l; }
	}
#endif

#if 1
	for(is=0; is<ns; is++)
	{
		sp=mem->span[is];
		if((addr>=sp->base) && (lim<=sp->limit))
		{
//			mem->pspanb=mem->pspan;
//			mem->pspanb=sp1;
//			mem->pspan=sp;
			cpu->pspanb=sp1;
			cpu->pspan=sp;
			return(sp);
		}
	}
#endif

	return(NULL);
}

int BTESH2_ThrowTrap(BTESH2_CpuState *cpu, int status)
{
	int i;

	if(cpu->status)
		return(-1);

	printf("<!>\n");

	cpu->regs[BTESH2_REG_PC]=cpu->ptcpc+2;

	cpu->status=status;
	cpu->trnext=NULL;
	cpu->trjmpnext=NULL;

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


#if 0
int BTESH2_CheckAddrTrapSmc_FlushAddrA(
	BTESH2_CpuState *cpu, btesh2_paddr addr, u32 val)
{
	BTESH2_Trace *tr;
	u32 *pbm;
	int pg, bp, bm;
	int p0, p1;
	int i, j, k, st;

	pg=addr>>12;
	bm=pg>>14;
	
	pbm=cpu->smcdbm[bm];
	if(!pbm)
		return(0);

	bp=pg&16383;

	printf("Flush SMC2 Pg=%05X V=%08X PC=%08X\n",
		addr>>12, val, cpu->ptcpc);

	st=0;
	lsmc=-1;
	for(i=0; i<(BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL); i++)
	{
		tr=cpu->icache[i];
		if(!tr)
			continue;

		p0=tr->srcpc>>12;
//				p1=(tr->maxpc-1)>>12;
		p1=tr->maxpc>>12;

		if((pg>=p0) && (pg<=p1))
//				if((addr>=tr->srcpc) && (addr<tr->maxpc))
		{
//					printf("Flush SMC2 %08X Pg=%05X Tr=%08X..%08X V=%08X\n",
//						addr, addr>>12, tr->srcpc, tr->maxpc, val);
			if(tr==cpu->cur_trace)
				if((addr>=tr->srcpc) && (addr<tr->maxpc))
			{
				printf("Trap SMC\n");
				BTESH2_ThrowTrap(cpu, BTESH2_EXC_TRAPSMC);
				st|=1;
				continue;
			}
			tr->jtflag&=~BTESH2_TRJTFL_ICACHE;
			cpu->icache[i]=NULL;

			if(!(tr->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
			{
				BTESH2_FlushTrace(cpu, tr);
				BTESH2_FreeTrace(cpu, tr);
			}else if(tr->jtflag&BTESH2_TRJTFL_LINKED)
			{
				cpu->jit_needflush=1;
				break;
			}
			st|=4;
		}
	}

#ifdef BTESH2_TR_JHASHSZ
	for(i=0; i<(BTESH2_TR_JHASHSZ*BTESH2_TR_JHASHLVL); i++)
	{
		tr=cpu->jcache[i];
		if(!tr)
			continue;

		p0=tr->srcpc>>12;
//		p1=(tr->maxpc-1)>>12;
		p1=tr->maxpc>>12;

		if((pg>=p0) && (pg<=p1))
//		if((addr>=tr->srcpc) && (addr<tr->maxpc))
		{
//			printf("Flush SMC2 %08X Pg=%05X Tr=%08X..%08X V=%08X\n",
//				addr, addr>>12, tr->srcpc, tr->maxpc, val);
			if(tr==cpu->cur_trace)
				if((addr>=tr->srcpc) && (addr<tr->maxpc))
			{
				printf("Trap SMC\n");
				BTESH2_ThrowTrap(cpu, BTESH2_EXC_TRAPSMC);
				st|=1;
				continue;
			}

			cpu->jcache[i]=NULL;
			tr->jtflag&=~BTESH2_TRJTFL_JCACHE;

			if(!(tr->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
			{
				BTESH2_FlushTrace(cpu, tr);
				BTESH2_FreeTrace(cpu, tr);
			}else if(tr->jtflag&BTESH2_TRJTFL_LINKED)
			{
				cpu->jit_needflush=1;
				break;
			}
			st|=4;
		}
	}
#endif
	
	return(st);
}
#endif

#if 1
int BTESH2_CheckAddrTrapSmc_FlushAddrB(
	BTESH2_CpuState *cpu, btesh2_paddr addr, u32 val)
{
	BTESH2_Trace *tr;
	u32 *pbm;
	int pg, bp, bm;
	int p0, p1;
	int i, j, k, st;

	pg=addr>>12;
	bm=pg>>14;
	
	pbm=cpu->smcdbm[bm];
	if(!pbm)
		return(0);

	bp=pg&16383;

//	printf("Flush SMC2 Pg=%05X V=%08X PC=%08X\n",
//		addr>>12, val, cpu->ptcpc);

//		printf("Check SMC %08X\n", addr);
	st=0;
//		for(i=0; i<BTESH2_TR_HASHSZ; i++)
	for(i=0; i<(BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL); i++)
	{
		tr=cpu->icache[i];
		if(!tr)
			continue;

		p0=tr->srcpc>>12;
		p1=tr->maxpc>>12;

		if((addr>=tr->srcpc) && (addr<tr->maxpc))
		{
			if(tr==cpu->cur_trace)
			{
				printf("Trap SMC\n");
				BTESH2_ThrowTrap(cpu, BTESH2_EXC_TRAPSMC);
				st|=1;
				continue;
			}

			tr->jtflag&=~BTESH2_TRJTFL_ICACHE;
			cpu->icache[i]=NULL;

			if(!(tr->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
			{
				BTESH2_FlushTrace(cpu, tr);
				BTESH2_FreeTrace(cpu, tr);
				cpu->trnext=NULL;
				cpu->trjmpnext=NULL;
			}else if(tr->jtflag&BTESH2_TRJTFL_LINKED)
			{
				cpu->jit_needflush=1;
				cpu->trnext=NULL;
				cpu->trjmpnext=NULL;
				break;
			}

			st|=4;
		}else
		{
			if((pg>=p0) && (pg<=p1))
				st|=2;
		}
	}

#ifdef BTESH2_TR_JHASHSZ
	for(i=0; i<(BTESH2_TR_JHASHSZ*BTESH2_TR_JHASHLVL); i++)
	{
		tr=cpu->jcache[i];
		if(!tr)
			continue;

		p0=tr->srcpc>>12;
		p1=tr->maxpc>>12;

		if((pg>=p0) && (pg<=p1))
//				if((addr>=tr->srcpc) && (addr<tr->maxpc))
		{
			if(tr==cpu->cur_trace)
				if((addr>=tr->srcpc) && (addr<tr->maxpc))
			{
				printf("Trap SMC\n");
				BTESH2_ThrowTrap(cpu, BTESH2_EXC_TRAPSMC);
				st|=1;
				continue;
			}

			cpu->jcache[i]=NULL;
			tr->jtflag&=~BTESH2_TRJTFL_JCACHE;

			if(!(tr->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
			{
				BTESH2_FlushTrace(cpu, tr);
				BTESH2_FreeTrace(cpu, tr);
				cpu->trnext=NULL;
				cpu->trjmpnext=NULL;
			}else if(tr->jtflag&BTESH2_TRJTFL_LINKED)
			{
				cpu->jit_needflush=1;
				cpu->trnext=NULL;
				cpu->trjmpnext=NULL;
				break;
			}
			st|=4;
		}else
		{
			if((pg>=p0) && (pg<=p1))
				st|=2;
		}
	}
#endif

	if(cpu->jit_needflush)
		return(st);

	for(i=0; i<256; i++)
	{
		tr=cpu->trlinked[i];
		while(tr)
		{
			p0=tr->srcpc>>12;
			p1=tr->maxpc>>12;

			if((pg>=p0) && (pg<=p1))
			{
				cpu->jit_needflush=1;
				cpu->trnext=NULL;
				cpu->trjmpnext=NULL;
				return(st);
			}

			tr=tr->lnknext;
		}
	}

	return(st);
}
#endif

int BTESH2_CheckAddrTrapSmc(BTESH2_CpuState *cpu,
	btesh2_paddr addr, u32 val)
{
//	static int lsmc=-1;
	BTESH2_Trace *tr;
	u32 *pbm;
	int pg, bp, bm;
	int p0, p1;
	int i, j, k, st;

#if 1
	if(((addr&4095)+3)>>12)
	{
		i=BTESH2_CheckAddrTrapSmc(cpu, (addr  )&(~3), val);
		j=BTESH2_CheckAddrTrapSmc(cpu, (addr+3)&(~3), val);
		return(i|j);
	}
#endif

	pg=addr>>12;
	bm=pg>>14;
	
	pbm=cpu->smcdbm[bm];
	if(!pbm)
		return(0);

	bp=pg&16383;
//	if(cpu->smcdbm[bm][bp>>5]&(1<<(bp&31)))
	if(pbm[bp>>5]&(1<<(bp&31)))
	{
		cpu->trnext=NULL;
		cpu->trjmpnext=NULL;

		if(pg==cpu->lsmc)
		{
			st=BTESH2_CheckAddrTrapSmc_FlushAddrB(cpu, addr, val);

			if(!st)
			{
		//		cpu->smcdbm[bm][bp>>5]&=~(1<<(bp&31));
				pbm[bp>>5]&=~(1<<(bp&31));
			}
			return(st&1);
		}else
		{
			st=BTESH2_CheckAddrTrapSmc_FlushAddrB(cpu, addr, val);
//			st=BTESH2_CheckAddrTrapSmc_FlushAddrA(cpu, addr, val);

			if(st&(1|4))
				{ cpu->lsmc=pg; }
			else
				{ cpu->lsmc=-1; }
		}

		if(!st)
		{
//			cpu->smcdbm[bm][bp>>5]&=~(1<<(bp&31));
			pbm[bp>>5]&=~(1<<(bp&31));
		}
		return(st&1);
	}
	return(0);
}

int BTESH2_MarkAddrTrapSmc(BTESH2_CpuState *cpu, btesh2_paddr addr)
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

int BTESH2_GetAddrBytePhy(BTESH2_CpuState *cpu, btesh2_paddr addr)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->GetB(sp, cpu, addr-sp->base));
}

int BTESH2_GetAddrWordPhy(BTESH2_CpuState *cpu, btesh2_paddr addr)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->GetW(sp, cpu, addr-sp->base));
}

u32 BTESH2_GetAddrDWordPhy(BTESH2_CpuState *cpu, btesh2_paddr addr)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	u32 i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+3);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return((u32)(-1));
	}
	return(sp->GetD(sp, cpu, addr-sp->base));
}

u64 BTESH2_GetAddrQWordPhy(BTESH2_CpuState *cpu, btesh2_paddr addr)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	u32 i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+7);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return((u64)(-1));
	}
	return(sp->GetQ(sp, cpu, addr-sp->base));
}

int BTESH2_SetAddrBytePhy(BTESH2_CpuState *cpu, btesh2_paddr addr, int val)
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
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetB(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrWordPhy(BTESH2_CpuState *cpu, btesh2_paddr addr, int val)
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
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetW(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrDWordPhy(BTESH2_CpuState *cpu, btesh2_paddr addr, u32 val)
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
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetD(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrQWordPhy(BTESH2_CpuState *cpu, btesh2_paddr addr, u64 val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	if(cpu->status)
		return(-1);
	if(BTESH2_CheckAddrTrapSmc(cpu, addr, val))
		return(-1);

	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+7);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetQ(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrBytePhy2(BTESH2_CpuState *cpu, btesh2_paddr addr, int val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetB(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrWordPhy2(BTESH2_CpuState *cpu, btesh2_paddr addr, int val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+1);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetW(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrDWordPhy2(BTESH2_CpuState *cpu, btesh2_paddr addr, u32 val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+3);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetD(sp, cpu, addr-sp->base, val));
}

int BTESH2_SetAddrQWordPhy2(BTESH2_CpuState *cpu, btesh2_paddr addr, u64 val)
{
	BTESH2_PhysSpan *sp;
	byte *ptr;
	int i;
	
	sp=BTESH2_GetSpanForAddr(cpu, addr, addr+7);
	if(!sp)
	{
//		cpu->regs[BTESH2_REG_FLA]=addr;
		cpu->regs[BTESH2_REG_TEA]=addr;
		BTESH2_ThrowTrap(cpu, BTESH2_EXC_INVADDR);
		return(-1);
	}
	return(sp->SetQ(sp, cpu, addr-sp->base, val));
}

int BTESH2_MemCpyIn(BTESH2_CpuState *cpu, btesh2_paddr addr,
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

int BTESH2_GetAddrByte(BTESH2_CpuState *cpu, btesh2_paddr addr)
	{ return(cpu->GetAddrByte(cpu, addr)); }
int BTESH2_GetAddrWord(BTESH2_CpuState *cpu, btesh2_paddr addr)
	{ return(cpu->GetAddrWord(cpu, addr)); }
u32 BTESH2_GetAddrDWord(BTESH2_CpuState *cpu, btesh2_paddr addr)
	{ return(cpu->GetAddrDWord(cpu, addr)); }
u64 BTESH2_GetAddrQWord(BTESH2_CpuState *cpu, btesh2_paddr addr)
	{ return(cpu->GetAddrQWord(cpu, addr)); }

int BTESH2_SetAddrByte(BTESH2_CpuState *cpu, btesh2_paddr addr, int val)
	{ return(cpu->SetAddrByte(cpu, addr, val)); }
int BTESH2_SetAddrWord(BTESH2_CpuState *cpu, btesh2_paddr addr, int val)
	{ return(cpu->SetAddrWord(cpu, addr, val)); }
int BTESH2_SetAddrDWord(BTESH2_CpuState *cpu, btesh2_paddr addr, u32 val)
	{ return(cpu->SetAddrDWord(cpu, addr, val)); }
int BTESH2_SetAddrQWord(BTESH2_CpuState *cpu, btesh2_paddr addr, u64 val)
	{ return(cpu->SetAddrQWord(cpu, addr, val)); }

int BTESH2_CheckCpuFmmuP(BTESH2_CpuState *cpu)
	{ return(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU); }

u64 BTESH2_GetRegQWord(BTESH2_CpuState *cpu, int rm)
{
	u64 i, j, k;
	i=cpu->regs[rm+BTESH2_REG_RLO]|
		((u64)cpu->regs[rm+BTESH2_REG_RHI]<<32);
	return(i);
}

int BTESH2_SetRegQWord(BTESH2_CpuState *cpu, int rn, u64 val)
{
	cpu->regs[rn+BTESH2_REG_RLO]=val;
	cpu->regs[rn+BTESH2_REG_RHI]=val>>32;
	return(0);
}


u64 BTESH2_GetFRegQWord(BTESH2_CpuState *cpu, int rm)
{
	u64 i, j, k;
	i=cpu->fregs[rm+1]|((u64)cpu->regs[rm+0]<<32);
	return(i);
}

int BTESH2_SetFRegQWord(BTESH2_CpuState *cpu, int rn, u64 val)
{
	cpu->regs[rn+1]=val;
	cpu->regs[rn+0]=val>>32;
	return(0);
}
