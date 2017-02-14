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
//	i=(ptr[0]<<8)|(ptr[1]);
//	return((s16)i);
	i=btesh2_gets16be(ptr);
	return(i);
}

u32 btesh2_spandfl_GetW_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
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
	BTESH2_CpuState *cpu, u32 reladdr)
{
	byte *ptr;
	u32 i;
	
	ptr=sp->data+reladdr;
//	i=(((u32)(ptr[0]))<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3]);
	i=btesh2_getu32be(ptr);
	return(i);
}

u32 btesh2_spandfl_GetD_LE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
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
#if defined(X86) || defined(X86_64)
	*(u16 *)ptr=val;
#else
	ptr[1]=val>>8;
	ptr[0]=val;
#endif
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
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	*ptr=val;
	*sp->dmdflag|=1;
	return(0);
}

int btesh2_spandfl_SetW_FlBE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[0]=val>>8;
	ptr[1]=val;
	*sp->dmdflag|=1;
	return(0);
}

int btesh2_spandfl_SetW_FlLE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	byte *ptr;
	ptr=sp->data+reladdr;
	ptr[1]=val>>8;
	ptr[0]=val;
	*sp->dmdflag|=1;
	return(0);
}

int btesh2_spandfl_SetD_FlBE(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
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
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
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
	BTESH2_CpuState *cpu, u32 reladdr)
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
	BTESH2_CpuState *cpu, u32 reladdr)
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

int btesh2_spandfl_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
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
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
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
	sp->range_n3=limit-base-3;
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

	sp->SetB=btesh2_spandfl_SetB;
	sp->SetW=btesh2_spandfl_SetW;
	sp->SetD=btesh2_spandfl_SetD;

	if(sp->dmdflag)
		sp->SetB=btesh2_spandfl_SetB_Fl;
}

u32 btesh2_spandmd_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->GetB(sp, cpu, reladdr));
}

u32 btesh2_spandmd_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->GetW(sp, cpu, reladdr));
}

u32 btesh2_spandmd_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->GetD(sp, cpu, reladdr));
}

int btesh2_spandmd_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->SetB(sp, cpu, reladdr, val));
}

int btesh2_spandmd_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->SetW(sp, cpu, reladdr, val));
}

int btesh2_spandmd_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	btesh2_spandmd_demandInit(sp);
	return(sp->SetD(sp, cpu, reladdr, val));
}

int BTESH2_MemoryDefineSpanDemand(BTESH2_MemoryImage *img,
	u32 base, u32 limit, byte **addr, int *flag, char *name)
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

	sp->SetB=btesh2_spandmd_SetB;
	sp->SetW=btesh2_spandmd_SetW;
	sp->SetD=btesh2_spandmd_SetD;
	
	i=BTESH2_MemoryAddSpan(img, sp);
	return(i);
}


u32 btesh2_spanreg_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=sp->GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(sbyte)j;
	return(j);
}

u32 btesh2_spanreg_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=sp->GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(s16)j;
	return(j);
}

int btesh2_spanreg_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=sp->SetD(sp, cpu, reladdr, val);
	return(i);
}

int btesh2_spanreg_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=sp->SetD(sp, cpu, reladdr, val);
	return(i);
}

int BTESH2_MemoryDefineSpanRegs(BTESH2_MemoryImage *img,
	u32 base, u32 limit, char *name,
	u32 (*GetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, u32 reladdr),
	int (*SetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu,
		u32 reladdr, u32 val)
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
	u32 addr, u32 lim)
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
	static int lsmc=-1;
	BTESH2_Trace *tr;
	int pg, bp, bm;
	int p0, p1;
	int i, j, k, st;

	if(((addr&4095)+3)>>12)
	{
		i=BTESH2_CheckAddrTrapSmc(cpu, (addr  )&(~3), val);
		j=BTESH2_CheckAddrTrapSmc(cpu, (addr+3)&(~3), val);
		return(i|j);
	}

	pg=addr>>12;
	bp=pg&16383;
	bm=pg>>14;
	
	if(!cpu->smcdbm[bm])
		return(0);

	if(cpu->smcdbm[bm][bp>>5]&(1<<(bp&31)))
	{
		if(pg==lsmc)
//		if(0)
		{
//			printf("Flush SMC2 Pg=%05X V=%08X PC=%08X\n",
//				addr>>12, val, cpu->ptcpc);

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
						st=1;
						continue;
					}
					BTESH2_FlushTrace(cpu, tr);
					BTESH2_FreeTrace(cpu, tr);
					cpu->icache[i]=NULL;
//					st|=4;
				}
			}
			if(!st)
			{
				cpu->smcdbm[bm][bp>>5]&=~(1<<(bp&31));
			}
			return(st&1);
		}else
		{
	//		printf("Check SMC %08X\n", addr);
			st=0;
	//		for(i=0; i<BTESH2_TR_HASHSZ; i++)
			for(i=0; i<(BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL); i++)
			{
				tr=cpu->icache[i];
				if(!tr)
					continue;

				p0=tr->srcpc>>12;
//				p1=(tr->maxpc-1)>>12;
				p1=tr->maxpc>>12;

	//			if((pg>=p0) && (pg<=p1))
				if((addr>=tr->srcpc) && (addr<tr->maxpc))
				{
//					printf("Flush SMC A=%08X Pg=%05X "
//							"Tr=%08X..%08X V=%08X PC=%08X\n",
//						addr, addr>>12, tr->srcpc, tr->maxpc, val,
//						cpu->ptcpc);
					if(tr==cpu->cur_trace)
	//					if((addr>=tr->srcpc) && (addr<tr->maxpc))
					{
						printf("Trap SMC\n");
						BTESH2_ThrowTrap(cpu, BTESH2_EXC_TRAPSMC);
						st|=1;
						continue;
					}
					BTESH2_FlushTrace(cpu, tr);
					BTESH2_FreeTrace(cpu, tr);
					cpu->icache[i]=NULL;
					st|=4;
	//				break;
				}
#if 1
				else
				{
	//				p0=tr->srcpc>>12;
	//				p1=tr->maxpc>>12;
					if((pg>=p0) && (pg<=p1))
						st|=2;
				}
#endif
			}
			if(st&(1|4))
				{ lsmc=pg; }
			else
				{ lsmc=-1; }
		}
		if(!st)
		{
			cpu->smcdbm[bm][bp>>5]&=~(1<<(bp&31));
		}
		return(st&1);
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

int BTESH2_CheckCpuFmmuP(BTESH2_CpuState *cpu)
	{ return(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU); }
