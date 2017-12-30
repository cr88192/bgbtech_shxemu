int BTESH2_Trace_TraceUpdateJTrig(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	tr->excnt++;
	if(tr->jtrig)
	{
		tr->jtrig--;
		if(!tr->jtrig)
		{
			BTESH2_TryJitTrace(cpu, tr);
			tr->Run(cpu, tr);
			return(1);
		}
	}
	return(0);
}


void BTESH2_Trace_CheckSanity(BTESH2_CpuState *cpu)
{
	if(!(cpu->regs[BTESH2_REG_SR]&BTESH2_SRFL_JQ))
		__debugbreak();

	if(!(cpu->regs[BTESH2_REG_SR]&BTESH2_SRFL_DQ))
	{
		if(cpu->csfl&BTESH2_CSFL_SRDQ)
			__debugbreak();
	}else
	{
		if(!(cpu->csfl&BTESH2_CSFL_SRDQ))
			__debugbreak();
	}
}

// #define BTESH2_TREX_SANITY		BTESH2_Trace_CheckSanity(cpu);
#define BTESH2_TREX_SANITY

BTESH2_Trace *BTESH2_Trace_Run1(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run2(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run3(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run4(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run5(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run6(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run7(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run8(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run9(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run10(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	BTESH2_TREX_SANITY
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run11(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run12(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run13(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run14(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

#if 0
BTESH2_Trace *BTESH2_Trace_Run14B(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run14C(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run14D(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}
#endif

BTESH2_Trace *BTESH2_Trace_Run15(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run16(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run17(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run18(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run19(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run20(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run21(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run22(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run23(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run24(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run25(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run26(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run27(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run28(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run29(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run30(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run31(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}

BTESH2_Trace *BTESH2_Trace_Run32(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;
	BTESH2_TREX_SANITY
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
	return(cpu->trnext);
}



int btesh2_nmid_pairs[256*256];

int BTESH2_DecodeTrace(BTESH2_CpuState *cpu,
	BTESH2_Trace *tr, btesh2_vaddr spc)
{
	BTESH2_Opcode *op, *op1;
	static int rov;
	btesh2_vaddr pc, brapc, jmppc;
	int i0, i1, i2, i3;
	int i, j, k, n;
	
	tr->srcpc=spc;
	tr->csfl=cpu->csfl;
	
	pc=spc; n=0; brapc=0; jmppc=0;
	while(n<BTESH2_TR_MAXOPS)
	{
		BTESH2_MarkAddrTrapSmc(cpu, pc);
		op=BTESH2_AllocOpcode(cpu);
		
		op->rm=BTESH2_REG_ZZR;
		op->rn=BTESH2_REG_ZZR;
		op->ro=BTESH2_REG_ZZR;
		
		BTESH2_DecodeOpcode(cpu, op, pc);
		pc+=2;
		if(op->fl&BTESH2_OPFL_EXTRAWORD)
			pc+=2;
		if(pc>tr->maxpc)
			tr->maxpc=pc;
		
		if(!op->Run)
		{
			op->nmid=BTESH2_NMID_INVALID;
			op->fmid=BTESH2_FMID_NONE;
			op->fl|=BTESH2_OPFL_CTRLF;
			op->Run=BTSH_Op_TRAP_UD;
		}

#if 1
		if((((op->opw>>12)&15)==0xD) &&
			(((pc-2)>>12)==((op->imm+3)>>12)))
		{
			BTESH2_MarkAddrTrapSmc(cpu, op->imm+3);
			if((op->imm+4)>tr->maxpc)
				tr->maxpc=op->imm+4;
			
			op->imm=(s32)BTESH2_GetAddrDWord(cpu, op->imm);
			op->fmid=BTESH2_FMID_REGVIMM;
			op->Run=BTSH_Op_MOV_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegImmQ;
				BTSH_FixupJQ_MOV_RegImm(cpu, op);
			}
		}
#endif

#if 1
		if((((op->opw>>12)&15)==0x9) &&
			(((pc-2)>>12)==((op->imm+1)>>12)))
		{
			if(!(cpu->csfl&BTESH2_CSFL_SRDQ))
			{
				BTESH2_MarkAddrTrapSmc(cpu, op->imm+1);
				if((op->imm+2)>tr->maxpc)
					tr->maxpc=op->imm+2;
				
				op->imm=(s16)BTESH2_GetAddrWord(cpu, op->imm);
				op->fmid=BTESH2_FMID_REGVIMM;
				op->Run=BTSH_Op_MOV_RegImm;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_OpJQ_MOV_RegImmQ;
					BTSH_FixupJQ_MOV_RegImm(cpu, op);
				}
			}
		}
#endif
		
		if(op->fl&BTESH2_OPFL_CTRLF)
		{
			if(op->fl&BTESH2_OPFL_DLYSLOT)
			{
				if(n<=(BTESH2_TR_MAXOPS-2))
				{
					op1=BTESH2_AllocOpcode(cpu);
					op1->fl=BTESH2_OPFL_DLYSLOT;
					op1->rm=BTESH2_REG_ZZR;
					op1->rn=BTESH2_REG_ZZR;
					op1->ro=BTESH2_REG_ZZR;

					BTESH2_DecodeOpcode(cpu, op1, pc);
					pc+=2;
					if(op1->fl&BTESH2_OPFL_EXTRAWORD)
						pc+=2;
					if(pc>tr->maxpc)
						tr->maxpc=pc;

//					if(op1->fl&BTESH2_OPFL_PCADLYSLOTD)
//						op1->imm=((op1->pc+4)&(~3))+(4*op1->ro);
					if(op1->fl&BTESH2_OPFL_PCADLYSLOTD)
						op1->imm=((op1->pc+6)&(~3))+(4*op1->ro);
//					if(op1->fl&BTESH2_OPFL_PCADLYSLOTD)
//						op1->imm=((op1->pc+8)&(~3))+(4*op1->ro);
					if(op1->fl&BTESH2_OPFL_PCADLYSLOTW)
						op1->imm=(op1->pc+6)+(2*op1->ro);

					if(!op1->Run)
					{
						op1->nmid=BTESH2_NMID_INVALID;
						op1->fmid=BTESH2_FMID_NONE;
						op1->fl=BTESH2_OPFL_CTRLF;
						op1->Run=BTSH_Op_TRAP_UD;
					}

#if 1
					if(op1->fl&BTESH2_OPFL_INVDLYSLOT)
					{
						op1->nmid=BTESH2_NMID_INVDLY;
						op1->fmid=BTESH2_FMID_NONE;
						op1->fl=BTESH2_OPFL_CTRLF;
						op1->Run=BTSH_Op_TRAP_UDLY;

						tr->ops[n++]=op1;
//						tr->ops[n++]=op;
						break;
					}
#endif

#if 1
					if(	(op->nmid==BTESH2_NMID_BRA) &&
						(op->fmid==BTESH2_FMID_ABS) &&
						(n<=(BTESH2_TR_MAXOPS/2)))
					{
						if(op1->nmid==BTESH2_NMID_NOP)
						{
							BTESH2_FreeOpcode(cpu, op1);
						}else
						{
//							tr->ops[n++]=op;
							tr->ops[n++]=op1;
						}

						pc=op->imm;
						BTESH2_FreeOpcode(cpu, op);

						continue;
					}
#endif

					if(op1->nmid==BTESH2_NMID_NOP)
					{
						tr->ops[n++]=op;
						BTESH2_FreeOpcode(cpu, op1);
//						pc-=2;
					}else
					{
						tr->ops[n++]=op;
						tr->ops[n++]=op1;
					}

//					if((	(op->nmid==BTESH2_NMID_BRA) ||
//							(op->nmid==BTESH2_NMID_BSR)) &&
					if(	(op->nmid==BTESH2_NMID_BRA) &&
						(op->fmid==BTESH2_FMID_ABS))
					{
						brapc=op->imm;
					}

					if((	(op->nmid==BTESH2_NMID_BF) ||
							(op->nmid==BTESH2_NMID_BFS) ||
							(op->nmid==BTESH2_NMID_BT) ||
							(op->nmid==BTESH2_NMID_BTS) ||
							(op->nmid==BTESH2_NMID_BSR)) &&
						(op->fmid==BTESH2_FMID_ABS))
					{
						jmppc=op->imm;
					}

//					tr->ops[n++]=op1;
//					tr->ops[n++]=op;
					break;
				}
				
				BTESH2_FreeOpcode(cpu, op);
				pc-=2;
				break;
			}

			if((	(op->nmid==BTESH2_NMID_BF) ||
					(op->nmid==BTESH2_NMID_BT) ||
					(op->nmid==BTESH2_NMID_BSR)) &&
//				(op->fmid==BTESH2_FMID_ABS))
//			if((	(op->nmid==BTESH2_NMID_BF) ||
//					(op->nmid==BTESH2_NMID_BT)) &&
				(op->fmid==BTESH2_FMID_ABS))
			{
				jmppc=op->imm;
//				if(op->fl&BTESH2_OPFL_EXTRAWORD)
//					jmppc=0;
			}

			tr->ops[n++]=op;
			break;
		}else
		{
			tr->ops[n++]=op;
		}
	}

//	brapc=0;
//	jmppc=0;

	tr->nxtpc=pc;
	tr->nops=n;
	tr->nwops=(pc-spc)/2;
	
	if(brapc)
	{
		tr->nxtpc=brapc;
	}
	tr->jmppc=jmppc;
	
#if 1
	for(i=0; (i+1)<n; i++)
	{
		i0=tr->ops[i+0]->nmid;
		i1=tr->ops[i+1]->nmid;
		
		if((i0==BTESH2_NMID_MOV) &&
			(tr->ops[i+0]->fmid==BTESH2_FMID_REGIMM))
				i0=BTESH2_NMID_MOVI;
		if(((i0==BTESH2_NMID_MOVL) || (i0==BTESH2_NMID_MOVW)) &&
			(tr->ops[i+0]->fmid==BTESH2_FMID_REGVIMM))
				i0=BTESH2_NMID_MOVIV;

		if((i1==BTESH2_NMID_MOV) &&
			(tr->ops[i+1]->fmid==BTESH2_FMID_REGIMM))
				i1=BTESH2_NMID_MOVI;
		if(((i1==BTESH2_NMID_MOVL) || (i1==BTESH2_NMID_MOVW)) &&
			(tr->ops[i+1]->fmid==BTESH2_FMID_REGVIMM))
				i1=BTESH2_NMID_MOVIV;
		
		j=i0*256+i1;
		btesh2_nmid_pairs[j]++;
	}
#endif
	
	switch(n)
	{
	case  1: tr->Run=BTESH2_Trace_Run1; break;
	case  2: tr->Run=BTESH2_Trace_Run2; break;
	case  3: tr->Run=BTESH2_Trace_Run3; break;
	case  4: tr->Run=BTESH2_Trace_Run4; break;
	case  5: tr->Run=BTESH2_Trace_Run5; break;
	case  6: tr->Run=BTESH2_Trace_Run6; break;
	case  7: tr->Run=BTESH2_Trace_Run7; break;
	case  8: tr->Run=BTESH2_Trace_Run8; break;
	case  9: tr->Run=BTESH2_Trace_Run9; break;
	case 10: tr->Run=BTESH2_Trace_Run10; break;
	case 11: tr->Run=BTESH2_Trace_Run11; break;
	case 12: tr->Run=BTESH2_Trace_Run12; break;
	case 13: tr->Run=BTESH2_Trace_Run13; break;
	case 14: tr->Run=BTESH2_Trace_Run14; break;
	case 15: tr->Run=BTESH2_Trace_Run15; break;
	case 16: tr->Run=BTESH2_Trace_Run16; break;

	case 17: tr->Run=BTESH2_Trace_Run17; break;
	case 18: tr->Run=BTESH2_Trace_Run18; break;
	case 19: tr->Run=BTESH2_Trace_Run19; break;
	case 20: tr->Run=BTESH2_Trace_Run20; break;
	case 21: tr->Run=BTESH2_Trace_Run21; break;
	case 22: tr->Run=BTESH2_Trace_Run22; break;
	case 23: tr->Run=BTESH2_Trace_Run23; break;
	case 24: tr->Run=BTESH2_Trace_Run24; break;

	case 25: tr->Run=BTESH2_Trace_Run25; break;
	case 26: tr->Run=BTESH2_Trace_Run26; break;
	case 27: tr->Run=BTESH2_Trace_Run27; break;
	case 28: tr->Run=BTESH2_Trace_Run28; break;
	case 29: tr->Run=BTESH2_Trace_Run29; break;
	case 30: tr->Run=BTESH2_Trace_Run30; break;
	case 31: tr->Run=BTESH2_Trace_Run31; break;
	case 32: tr->Run=BTESH2_Trace_Run32; break;

	default: break;
	}

//	tr->jtrig=252;
//	tr->jtrig=126;
//	tr->jtrig=62;
	tr->jtrig=30;
	tr->excnt=0;
//	BTESH2_TryJitTrace(cpu, tr);

	return(0);
}

void BTESH2_FlushTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	int i;

	if(tr->maxpc==0xDEADFEED)
	{
		__debugbreak();
		return;
	}
	
	if((tr->nops<0) || (tr->nops>BTESH2_TR_MAXOPS))
	{
		__debugbreak();
		i=-1;
		return;
	}
	
	if(tr->jtflag&BTESH2_TRJTFL_NOSTOMP_MASK)
		__debugbreak();
	
	if(tr->srcpc==(btesh2_vaddr)(-1))
	{
		if(!(tr->jtflag&BTESH2_TRJTFL_ICACHE) ||
			(tr->jtflag&BTESH2_TRJTFL_NOSTOMP_MASK))
				__debugbreak();
		i=-2;
		return;
	}
	
	tr->Run=NULL;
	BTESH2_JitUnlinkTrace(cpu, tr->trnext);
	BTESH2_JitUnlinkTrace(cpu, tr->trjmpnext);
	
	for(i=0; i<tr->nops; i++)
	{
		BTESH2_FreeOpcode(cpu, tr->ops[i]);
	}
	
	memset(tr, 0, sizeof(BTESH2_Trace));

	tr->srcpc=-1;
	tr->csfl=-1;
	
//	tr->nops=0;
}

void BTESH2_FlushTracesFull(BTESH2_CpuState *cpu)
{
	BTESH2_Trace *tr, *tr1;
	int i, j, k;
	
	cpu->trnext=NULL;
	cpu->trjmpnext=NULL;

	for(i=0; i<(BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL); i++)
	{
		tr=cpu->icache[i];
		if(!tr)
			continue;
		cpu->icache[i]=NULL;

		tr->jtflag&=~BTESH2_TRJTFL_ICACHE;
		if(!(tr->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
		{
			BTESH2_FlushTrace(cpu, tr);
			BTESH2_FreeTrace(cpu, tr);
		}
	}

#ifdef BTESH2_TR_JHASHSZ
	for(i=0; i<(BTESH2_TR_JHASHSZ*BTESH2_TR_JHASHLVL); i++)
	{
		tr=cpu->jcache[i];
		if(!tr)
			continue;
		cpu->jcache[i]=NULL;

//		tr->jtflag&=~BTESH2_TRJTFL_LINKED;

		tr->jtflag&=~BTESH2_TRJTFL_JCACHE;
		if(!(tr->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
		{
			BTESH2_FlushTrace(cpu, tr);
			BTESH2_FreeTrace(cpu, tr);
		}
	}
#endif

	for(i=0; i<256; i++)
	{
		tr=cpu->trlinked[i];
		cpu->trlinked[i]=NULL;
		
		while(tr)
		{
			tr1=tr->lnknext;
			tr->jtflag&=~BTESH2_TRJTFL_LINKED;
			tr->trnext=NULL;	tr->trjmpnext=NULL;
			BTESH2_FlushTrace(cpu, tr);
			BTESH2_FreeTrace(cpu, tr);
			tr=tr1;
		}
	}
}

force_inline BTESH2_Trace *BTESH2_TraceForAddr(
	BTESH2_CpuState *cpu, btesh2_vaddr spc)
{
	BTESH2_Trace *tr, *tr1, *tr2;
	btesh2_vaddr spc1;
	int h, h0, h1, hp, hp1;
	int i, j, k;
	
//	spc1=spc^(spc>>16);
//	spc1=spc^(spc>>20);
//	h=((spc1*524287)>>20)&(BTESH2_TR_HASHSZ-1);
//	h=((spc*524287)>>20)&(BTESH2_TR_HASHSZ-1);

//	spc1=spc^(spc>>20);
//	hp=spc*BTESH2_TR_HASHPR;
	hp=(spc^cpu->csfl)*BTESH2_TR_HASHPR;
	h=(hp>>BTESH2_TR_HASHSHR)&(BTESH2_TR_HASHSZ-1);

	h0=h*2+0;
	tr=cpu->icache[h0];
	if(tr)
	{
		if((tr->srcpc==spc) && (tr->csfl==cpu->csfl))
//		if(tr->srcpc==spc)
		{
			return(tr);
		}

#if 1
//		h1=h|BTESH2_TR_HASHSZ;
		h1=h*2+1;
		tr1=cpu->icache[h1];

		if(tr1)
		{
			tr->jtflag|=BTESH2_TRJTFL_ICACHE;
			tr1->jtflag|=BTESH2_TRJTFL_ICACHE;

			if((tr1->srcpc==spc) && (tr1->csfl==cpu->csfl))
			{
				cpu->icache[h0]=tr1;
				cpu->icache[h1]=tr;
				return(tr1);
			}

			tr2=BTESH2_JTraceForAddr(cpu, spc);
			if(tr2)
			{
				cpu->icache[h0]=tr2;
				cpu->icache[h1]=tr;
				tr->jtflag|=BTESH2_TRJTFL_ICACHE;
				tr2->jtflag|=BTESH2_TRJTFL_ICACHE;
				tr1->jtflag&=~BTESH2_TRJTFL_ICACHE;
//				if(tr->jtrig>0)
//				if(!(tr1->jtflag&3))
				if(!(tr1->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
				{
					BTESH2_FlushTrace(cpu, tr1);
					BTESH2_FreeTrace(cpu, tr1);
				}
				return(tr2);
			}

			tr1->jtflag|=BTESH2_TRJTFL_ICACHE;
			tr->jtflag|=BTESH2_TRJTFL_ICACHE;

			cpu->icache[h0]=tr1;
			cpu->icache[h1]=tr;
			tr=tr1;
			
//			if(tr->jtrig>0)
			if(!(tr->jtflag&BTESH2_TRJTFL_NOSTOMP_MASK))
			{
				BTESH2_FlushTrace(cpu, tr);
				cpu->tr_dcol++;
				tr->jtflag|=BTESH2_TRJTFL_ICACHE;
			}else
			{
				tr=BTESH2_AllocTrace(cpu);
				cpu->icache[h0]=tr;
				tr->jtflag|=BTESH2_TRJTFL_ICACHE;
			}
		}else
		{
			tr2=BTESH2_JTraceForAddr(cpu, spc);
			if(tr2)
			{
				cpu->icache[h0]=tr2;
				cpu->icache[h1]=tr;
				tr2->jtflag|=BTESH2_TRJTFL_ICACHE;
				tr->jtflag|=BTESH2_TRJTFL_ICACHE;
				return(tr2);
			}

			cpu->icache[h1]=tr;
			tr->jtflag|=BTESH2_TRJTFL_ICACHE;

			tr=BTESH2_AllocTrace(cpu);
			cpu->icache[h0]=tr;
			tr->jtflag|=BTESH2_TRJTFL_ICACHE;
		}
#else
		BTESH2_FlushTrace(cpu, tr);
		cpu->tr_dcol++;
#endif
	}else
	{
		tr2=BTESH2_JTraceForAddr(cpu, spc);
		if(tr2)
		{
			cpu->icache[h0]=tr2;
			tr2->jtflag|=BTESH2_TRJTFL_ICACHE;
			return(tr2);
		}

		tr=BTESH2_AllocTrace(cpu);
		cpu->icache[h0]=tr;
		tr->jtflag|=BTESH2_TRJTFL_ICACHE;
	}
	
	if(cpu->jit_needflush)
	{
		tr->srcpc=-2;
	
		printf("BTESH2_TraceForAddr: Full Flush\n");
		cpu->jit_needflush=0;
		BTESH2_FlushTracesFull(cpu);
		UAX_ExHeapResetMark();
		return(BTESH2_TraceForAddr(cpu, spc));
	}
	
	cpu->tr_dtot++;
	BTESH2_DecodeTrace(cpu, tr, spc);
	return(tr);
}
