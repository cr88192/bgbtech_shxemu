int UAX_ExHeapResetMark()
{
	return(0);
}

int BTESH2_JitLinkTrace(BTESH2_CpuState *cpu, BTESH2_Trace *trj)
{
	int h, hp;

	if(!(trj->jtflag&BTESH2_TRJTFL_LINKED))
	{
		hp=(trj->srcpc^trj->csfl)*BTESH2_TR_HASHPR;
		h=(hp>>BTESH2_TR_HASHSHR)&(BTESH2_TR_HASHSZ-1);

		trj->lnknext=cpu->trlinked[h&255];
		cpu->trlinked[h&255]=trj;
		trj->jtflag|=BTESH2_TRJTFL_LINKED;
	}
	
	return(0);
}

int BTESH2_JitUnlinkTrace(BTESH2_CpuState *cpu, BTESH2_Trace *trj)
{
	return(0);
}

BTESH2_Trace *BTESH2_JTraceForAddr(BTESH2_CpuState *cpu, u32 spc)
{
	BTESH2_Trace *tr0, *tr1, *tr2, *tr3, *tr4;
	u32 spc1;
	int h, h0, h1, h2, h3, hp, hp1;
	int i, j, k;
	
//	return(NULL);
	
	hp=(spc^cpu->csfl)*BTESH2_TR_HASHPR;
	h=(hp>>BTESH2_TR_HASHSHR)&(BTESH2_TR_HASHSZ-1);

	tr4=cpu->trlinked[h&255];
	while(tr4)
	{
		if((tr4->srcpc==spc) && (tr4->csfl==cpu->csfl))
		{
//			BTESH2_JTraceAddTrace(cpu, tr4);
			return(tr4);
		}
		tr4=tr4->lnknext;
	}

	return(NULL);
}

int BTESH2_TryJitTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Trace *trj;
	byte nolink;

//	nolink=0;
	nolink=1;

#if 1
	if(!tr->trnext && !nolink)
	{
		trj=BTESH2_TraceForAddr(cpu, tr->nxtpc);
		if(trj)
		{
			BTESH2_JitLinkTrace(cpu, trj);
			tr->trnext=trj;
		}
	}
#endif

#if 1
	if(!tr->trjmpnext && tr->jmppc && !nolink)
	{
		trj=BTESH2_TraceForAddr(cpu, tr->jmppc);
		if(trj)
		{
			BTESH2_JitLinkTrace(cpu, trj);
			tr->trjmpnext=trj;
		}
	}
#endif

	return(0);
}
