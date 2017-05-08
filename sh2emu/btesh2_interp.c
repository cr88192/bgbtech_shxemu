BTESH2_CpuState *BTESH2_AllocCpuState(void)
{
	BTESH2_CpuState *cpu;
	
	cpu=malloc(sizeof(BTESH2_CpuState));
	memset(cpu, 0, sizeof(BTESH2_CpuState));
	return(cpu);
}

BTESH2_Opcode *BTESH2_AllocOpcode(BTESH2_CpuState *cpu)
{
	BTESH2_Opcode *op, *opsl;
	int i;

	op=cpu->free_opcode;
	if(op)
	{
		cpu->free_opcode=*(BTESH2_Opcode **)op;
		memset(op, 0, sizeof(BTESH2_Opcode));
		return(op);
	}
	
	opsl=malloc(257*sizeof(BTESH2_Opcode));
	*(void **)opsl=cpu->slabs;
	cpu->slabs=opsl;
	
	for(i=0; i<256; i++)
	{
		op=opsl+i+1;
		*(BTESH2_Opcode **)op=cpu->free_opcode;
		cpu->free_opcode=op;
	}

	op=cpu->free_opcode;
	cpu->free_opcode=*(BTESH2_Opcode **)op;
	memset(op, 0, sizeof(BTESH2_Opcode));
	return(op);
}

void BTESH2_FreeOpcode(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	*(BTESH2_Opcode **)op=cpu->free_opcode;
	cpu->free_opcode=op;
}

BTESH2_Trace *BTESH2_AllocTrace(BTESH2_CpuState *cpu)
{
	BTESH2_Trace *tr, *trsl;
	int i;

	tr=cpu->free_trace;
	if(tr)
	{
		cpu->free_trace=*(BTESH2_Trace **)tr;
		memset(tr, 0, sizeof(BTESH2_Trace));
		return(tr);
	}
	
	trsl=malloc(257*sizeof(BTESH2_Trace));
	*(void **)trsl=cpu->slabs;
	cpu->slabs=trsl;
	
	for(i=0; i<256; i++)
	{
		tr=trsl+i+1;
		*(BTESH2_Trace **)tr=cpu->free_trace;
		cpu->free_trace=tr;
	}

	tr=cpu->free_trace;
	cpu->free_trace=*(BTESH2_Trace **)tr;
	memset(tr, 0, sizeof(BTESH2_Trace));
	return(tr);
}

void BTESH2_FreeTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	if(tr->maxpc==0xDEADFEED)
	{
		__debugbreak();
		return;
	}
	tr->maxpc=0xDEADFEED;
	*(BTESH2_Trace **)tr=cpu->free_trace;
	cpu->free_trace=tr;
}

char *btesh2_print_NameForNameID(int id)
{
	char *s;
	s="?";
	switch(id)
	{
	case BTESH2_NMID_INVALID: s="Invalid"; break;
	case BTESH2_NMID_MOV: s="MOV"; break;
	case BTESH2_NMID_MOVB: s="MOV.B"; break;
	case BTESH2_NMID_MOVW: s="MOV.W"; break;
	case BTESH2_NMID_MOVL: s="MOV.L"; break;

	case BTESH2_NMID_ADD: s="ADD"; break;
	case BTESH2_NMID_ADDC: s="ADDC"; break;
	case BTESH2_NMID_ADDV: s="ADDV"; break;

	case BTESH2_NMID_SUB: s="SUB"; break;
	case BTESH2_NMID_SUBC: s="SUBC"; break;
	case BTESH2_NMID_SUBV: s="SUBV"; break;

	case BTESH2_NMID_CMPHS: s="CMPHS"; break;
	case BTESH2_NMID_CMPHI: s="CMPHI"; break;
	case BTESH2_NMID_CMPEQ: s="CMPEQ"; break;
	case BTESH2_NMID_CMPGE: s="CMPGE"; break;
	case BTESH2_NMID_CMPGT: s="CMPGT"; break;

	case BTESH2_NMID_JMP: s="JMP"; break;
	case BTESH2_NMID_JSR: s="JSR"; break;
	case BTESH2_NMID_BRA: s="BRA"; break;
	case BTESH2_NMID_BSR: s="BSR"; break;
	case BTESH2_NMID_BT: s="BT"; break;
	case BTESH2_NMID_BF: s="BF"; break;
	case BTESH2_NMID_BTS: s="BTS"; break;
	case BTESH2_NMID_BFS: s="BFS"; break;

	case BTESH2_NMID_DIV1: s="DIV1"; break;
	case BTESH2_NMID_DMULU: s="DMULU"; break;
	case BTESH2_NMID_DMULS: s="DMULS"; break;
	case BTESH2_NMID_TST: s="TST"; break;
	case BTESH2_NMID_AND: s="AND"; break;
	case BTESH2_NMID_XOR: s="XOR"; break;
	case BTESH2_NMID_OR: s="OR"; break;

	case BTESH2_NMID_DIV0U: s="DIV0U"; break;

	case BTESH2_NMID_DIV0S: s="DIV0S"; break;
	case BTESH2_NMID_CMPSTR: s="CMPSTR"; break;
	case BTESH2_NMID_MULUW: s="MULUW"; break;
	case BTESH2_NMID_MULSW: s="MULSW"; break;
	case BTESH2_NMID_MACL: s="MAC.L"; break;
	case BTESH2_NMID_BSRF: s="BSRF"; break;
	case BTESH2_NMID_BRAF: s="BRAF"; break;
	case BTESH2_NMID_MULL: s="MULL"; break;
	case BTESH2_NMID_CLRT: s="CLRT"; break;
	case BTESH2_NMID_SETT: s="SETT"; break;
	case BTESH2_NMID_CLRMAC: s="CLRMAC"; break;
	case BTESH2_NMID_NOP: s="NOP"; break;
	case BTESH2_NMID_MOVT: s="MOVT"; break;
	case BTESH2_NMID_RTS: s="RTS"; break;
	case BTESH2_NMID_SLEEP: s="SLEEP"; break;
	case BTESH2_NMID_RTE: s="RTE"; break;
	case BTESH2_NMID_SHLL: s="SHLL"; break;
	case BTESH2_NMID_DT: s="DT"; break;
	case BTESH2_NMID_SHAL: s="SHAL"; break;
	case BTESH2_NMID_SHLR: s="SHLR"; break;
	case BTESH2_NMID_CMPPZ: s="CMPPZ"; break;
	case BTESH2_NMID_SHAR: s="SHAR"; break;
	case BTESH2_NMID_ROTL: s="ROTL"; break;
	case BTESH2_NMID_ROTCL: s="ROTCL"; break;
	case BTESH2_NMID_ROTT: s="ROTT"; break;
	case BTESH2_NMID_ROTCR: s="ROTCR"; break;
	case BTESH2_NMID_SHLL2: s="SHLL2"; break;
	case BTESH2_NMID_ROTR: s="ROTR"; break;
	case BTESH2_NMID_CMPPL: s="CMPPL"; break;
	case BTESH2_NMID_SHLL8: s="SHLL8"; break;
	case BTESH2_NMID_SHLL16: s="SHLL16"; break;
	case BTESH2_NMID_SHLR2: s="SHLR2"; break;
	case BTESH2_NMID_SHLR8: s="SHLR8"; break;
	case BTESH2_NMID_SHLR16: s="SHLR16"; break;
	case BTESH2_NMID_MACW: s="MAC.W"; break;
	case BTESH2_NMID_NOT: s="NOT"; break;
	case BTESH2_NMID_SWAPB: s="SWAP.B"; break;
	case BTESH2_NMID_SWAPW: s="SWAP.W"; break;
	case BTESH2_NMID_NEGC: s="NEGC"; break;
	case BTESH2_NMID_NEG: s="NEG"; break;
	case BTESH2_NMID_EXTUB: s="EXTUB"; break;
	case BTESH2_NMID_EXTUW: s="EXTUW"; break;
	case BTESH2_NMID_EXTSB: s="EXTSB"; break;
	case BTESH2_NMID_EXTSW: s="EXTSW"; break;
	case BTESH2_NMID_TRAPA: s="TRAPA"; break;
	case BTESH2_NMID_MOVA: s="MOVA"; break;
	case BTESH2_NMID_TSTB: s="TSTB"; break;

	case BTESH2_NMID_LDC: s="LDC"; break;
	case BTESH2_NMID_LDCL: s="LDC.L"; break;
	case BTESH2_NMID_LDS: s="LDS"; break;
	case BTESH2_NMID_LDSL: s="LDS.L"; break;
	case BTESH2_NMID_STC: s="STC"; break;
	case BTESH2_NMID_STCL: s="STC.L"; break;
	case BTESH2_NMID_STS: s="STS"; break;
	case BTESH2_NMID_STSL: s="STS.L"; break;

	case BTESH2_NMID_CASL: s="CAS.L"; break;
	case BTESH2_NMID_TASB: s="TAS.B"; break;
	case BTESH2_NMID_XTRCT: s="XTRCT"; break;

	case BTESH2_NMID_ANDB: s="AND.B"; break;
	case BTESH2_NMID_XORB: s="XOR.B"; break;
	case BTESH2_NMID_ORB: s="OR.B"; break;

	case BTESH2_NMID_SHAD: s="SHAD"; break;
	case BTESH2_NMID_SHLD: s="SHLD"; break;

	case BTESH2_NMID_INVDLY: s="INVDLY"; break;
	case BTESH2_NMID_CLRS: s="CLRS"; break;
	case BTESH2_NMID_SETS: s="SETS"; break;

	case BTESH2_NMID_FABS:		s="FABS"; break;
	case BTESH2_NMID_FADD:		s="FADD"; break;
	case BTESH2_NMID_FCMPEQ:	s="FCMPEQ"; break;
	case BTESH2_NMID_FCMPGT:	s="FCMPGT"; break;
	case BTESH2_NMID_FCNVDS:	s="FCNVDS"; break;
	case BTESH2_NMID_FCNVSD:	s="FCNVSD"; break;
	case BTESH2_NMID_FDIV:		s="FDIV"; break;
	case BTESH2_NMID_FLDI0:		s="FLDI0"; break;
	case BTESH2_NMID_FLDI1:		s="FLDI1"; break;
	case BTESH2_NMID_FLDS:		s="FLDS"; break;
	case BTESH2_NMID_FLOAT:		s="FLOAT"; break;
	case BTESH2_NMID_FMAC:		s="FMAC"; break;
	case BTESH2_NMID_FMOV:		s="FMOV"; break;
	case BTESH2_NMID_FMOVS:		s="FMOVS"; break;
	case BTESH2_NMID_FMOVD:		s="FMOVD"; break;
	case BTESH2_NMID_FMUL:		s="FMUL"; break;
	case BTESH2_NMID_FNEG:		s="FNEG"; break;
	case BTESH2_NMID_FSCHG:		s="FSCHG"; break;
	case BTESH2_NMID_FSQRT:		s="FSQRT"; break;
	case BTESH2_NMID_FSTS:		s="FSTS"; break;
	case BTESH2_NMID_FSUB:		s="FSUB"; break;
	case BTESH2_NMID_FTRC:		s="FTRC"; break;

	case BTESH2_NMID_MOVCAL: s="MOVCA.L"; break;

	case BTESH2_NMID_MOVI: s="MOV.I"; break;
	case BTESH2_NMID_MOVIV: s="MOV.IV"; break;

	default: s="?"; break;
	}
	return(s);
}

char *btesh2_print_NameForRegID(int id)
{
	char *s;
	switch(id)
	{
	case  0: s="R0"; break;
	case  1: s="R1"; break;
	case  2: s="R2"; break;
	case  3: s="R3"; break;
	case  4: s="R4"; break;
	case  5: s="R5"; break;
	case  6: s="R6"; break;
	case  7: s="R7"; break;
	case  8: s="R8"; break;
	case  9: s="R9"; break;
	case 10: s="R10"; break;
	case 11: s="R11"; break;
	case 12: s="R12"; break;
	case 13: s="R13"; break;
	case 14: s="R14"; break;
	case 15: s="SP"; break;
	
	case BTESH2_REG_SR: s="SR"; break;
	case BTESH2_REG_GBR: s="GBR"; break;
	case BTESH2_REG_VBR: s="VBR"; break;
	case BTESH2_REG_MACH: s="MACH"; break;
	case BTESH2_REG_MACL: s="MACL"; break;
	case BTESH2_REG_PR: s="PR"; break;
	case BTESH2_REG_PC: s="PC"; break;

	case BTESH2_REG_FPSCR: s="FPSCR"; break;
	case BTESH2_REG_FPUL: s="FPUL"; break;

	case BTESH2_REG_MMUCR: s="MMUCR"; break;
	case BTESH2_REG_PTEH: s="PTEH"; break;
	case BTESH2_REG_PTEL: s="PTEL"; break;
	case BTESH2_REG_TTB: s="TTB"; break;
	case BTESH2_REG_TEA: s="TEA"; break;
	
	case (BTESH2_REG_RBANK+0): s="R0_B"; break;
	case (BTESH2_REG_RBANK+1): s="R1_B"; break;
	case (BTESH2_REG_RBANK+2): s="R2_B"; break;
	case (BTESH2_REG_RBANK+3): s="R3_B"; break;
	case (BTESH2_REG_RBANK+4): s="R4_B"; break;
	case (BTESH2_REG_RBANK+5): s="R5_B"; break;
	case (BTESH2_REG_RBANK+6): s="R6_B"; break;
	case (BTESH2_REG_RBANK+7): s="R7_B"; break;

	default: s="?"; break;
	}
	return(s);
}

char *btesh2_print_NameForFRegID(int id)
{
	char *s;
	switch(id)
	{
	case  0: s="FR0"; break;
	case  1: s="FR1"; break;
	case  2: s="FR2"; break;
	case  3: s="FR3"; break;
	case  4: s="FR4"; break;
	case  5: s="FR5"; break;
	case  6: s="FR6"; break;
	case  7: s="FR7"; break;
	case  8: s="FR8"; break;
	case  9: s="FR9"; break;
	case 10: s="FR10"; break;
	case 11: s="FR11"; break;
	case 12: s="FR12"; break;
	case 13: s="FR13"; break;
	case 14: s="FR14"; break;
	case 15: s="FR15"; break;
	default: s="?"; break;
	}
	return(s);
}

char *btesh2_print_NameForDRegID(int id)
{
	char *s;
	switch(id)
	{
	case  0: s="DR0"; break;
	case  1: s="XD0"; break;
	case  2: s="DR1"; break;
	case  3: s="XD1"; break;
	case  4: s="DR2"; break;
	case  5: s="XD2"; break;
	case  6: s="DR3"; break;
	case  7: s="XD3"; break;
	case  8: s="DR4"; break;
	case  9: s="XD4"; break;
	case 10: s="DR5"; break;
	case 11: s="XD5"; break;
	case 12: s="DR6"; break;
	case 13: s="XD6"; break;
	case 14: s="DR7"; break;
	case 15: s="XD7"; break;
	case 16: s="XD0"; break;
	case 18: s="XD1"; break;
	case 20: s="XD2"; break;
	case 22: s="XD3"; break;
	case 24: s="XD4"; break;
	case 26: s="XD5"; break;
	case 28: s="XD6"; break;
	case 30: s="XD7"; break;
	default: s="?"; break;
	}
	return(s);
}

int BTESH2_PrintTrace(BTESH2_CpuState *cpu,
	BTESH2_Trace *tr)
{
	BTESH2_Opcode *op;
	u32 baddr;
	char *snm;
	int i;

	if(!tr)
	{
		printf("BTESH2_PrintTrace: Null Trace\n");
		return(-1);
	}

//	printf("BTESH2_PrintTrace: @=%08X N=%08X\n", tr->srcpc, tr->nxtpc);
	
	i=BTESH2_LookupNameForAddr(cpu, tr->srcpc, &snm, &baddr);
	if(i>0)
	{
		printf("@=%08X N=%08X %s+%06X\n",
			tr->srcpc, tr->nxtpc, snm, tr->srcpc-baddr);
	}else
	{
		printf("@=%08X N=%08X\n", tr->srcpc, tr->nxtpc);
	}
	
//	printf("@=%08X N=%08X\n", tr->srcpc, tr->nxtpc);
	for(i=0; i<tr->nops; i++)
	{
		op=tr->ops[i];
//		printf("  %08X %04X ", tr->srcpc+i*2, op->opw);
		printf("  %08X %04X ", op->pc, op->opw);
		
		snm=btesh2_print_NameForNameID(op->nmid);
		printf("%s ", snm);
		switch(op->fmid)
		{
		case BTESH2_FMID_REGREG:
			printf("%s, %s",
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;
		case BTESH2_FMID_REGRM:
			printf("%s", btesh2_print_NameForRegID(op->rm));
			break;
		case BTESH2_FMID_REGRN:
			printf("%s", btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_REGIMM:
			printf("#%d, %s",
				op->imm,
				btesh2_print_NameForRegID(op->rn));
			break;
		case BTESH2_FMID_REGVIMM:
			printf("#(0x%08X), %s",
				op->imm,
				btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_REGLDABS:
			printf("@(0x%08X), %s <%08X>",
				op->imm,
				btesh2_print_NameForRegID(op->rn),
				BTESH2_GetAddrDWord(cpu, op->imm));
			break;

		case BTESH2_FMID_ABS:
			printf("@(0x%08X)", op->imm);
			break;
		case BTESH2_FMID_IMM:
			printf("#%d", op->imm);
			break;

		case BTESH2_FMID_LDREG:
			printf("@%s", btesh2_print_NameForRegID(op->rm));
			break;
		case BTESH2_FMID_STREG:
			printf("@%s", btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_REGST:
			printf("%s, @%s",
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;
		case BTESH2_FMID_REGLD:
			printf("@%s, %s",
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_REGDECST:
			printf("%s, @-%s",
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;
		case BTESH2_FMID_REGINCLD:
			printf("@%s+, %s",
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_REGLDDISP:
			printf("@(%s, %d), %s",
				btesh2_print_NameForRegID(op->rm), op->imm,
				btesh2_print_NameForRegID(op->rn));
			break;
		case BTESH2_FMID_REGSTDISP:
			printf("%s, @(%s, %d)",
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn), op->imm);
			break;

		case BTESH2_FMID_REGSTR0N:
			printf("%s, @(%s, %s)",
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->ro),
				btesh2_print_NameForRegID(op->rn));
			break;
		case BTESH2_FMID_REGLDR0M:
			printf("@(%s, %s), %s",
				btesh2_print_NameForRegID(op->ro),
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_IMMSTRMN:
			printf("#%d, @(%s, %s)",
				op->imm,
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_FREGRM:
			printf("%s", btesh2_print_NameForFRegID(op->rm));
			break;
		case BTESH2_FMID_FREGRN:
			printf("%s", btesh2_print_NameForFRegID(op->rn));
			break;
		case BTESH2_FMID_FREGREG:
			printf("%s, %s",
				btesh2_print_NameForFRegID(op->rm),
				btesh2_print_NameForFRegID(op->rn));
			break;

		case BTESH2_FMID_DREGRM:
			printf("%s", btesh2_print_NameForDRegID(op->rm));
			break;
		case BTESH2_FMID_DREGRN:
			printf("%s", btesh2_print_NameForDRegID(op->rn));
			break;
		case BTESH2_FMID_DREGREG:
			printf("%s, %s",
				btesh2_print_NameForDRegID(op->rm),
				btesh2_print_NameForDRegID(op->rn));
			break;

		case BTESH2_FMID_IMMSTRN:
			printf("#%d (=> %s), @%s",
				op->imm,
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;
		case BTESH2_FMID_IMMDECSTRN:
			printf("#%d (=> %s), @-%s",
				op->imm,
				btesh2_print_NameForRegID(op->rm),
				btesh2_print_NameForRegID(op->rn));
			break;

		case BTESH2_FMID_NONE:
			break;
		default:
			printf("?");
			break;
		}
		printf("\n");
	}
	return(0);
}

int BTESH2_CpuTimerInt(BTESH2_CpuState *cpu)
{
	BTSH_Op_TrapIntIrq(cpu, BTESH2_EXC_PIT);
	return(0);
}

int BTESH2_CpuUartInt(BTESH2_CpuState *cpu)
{
	BTSH_Op_TrapIntIrq(cpu, BTESH2_EXC_UART0);
	return(0);
}

int BTESH2_CpuNolink(BTESH2_CpuState *cpu)
{
	if(cpu->arch==BTESH2_ARCH_SH2)
		return(1);
	if(cpu->archfl&BTESH2_ARFL_NOLINK)
		return(1);
	return(0);
}

int BTESH2_RunCpu(BTESH2_CpuState *cpu, int lim)
{
	BTESH2_Trace *tr, *trnxt;
	u32 pc, lpc;
	byte ld;
	int t0, t1, t2, ts;
	int l, clkl;

	cpu->tr_runlim=lim;
	lpc=cpu->dbg_lpc; ld=cpu->dbg_ld;
	l=lim; trnxt=NULL;
//	while(!cpu->status && ((l--)>0))
//	while((l--)>0)
	while((cpu->tr_runlim--)>0)
	{
#if 1
		if(BTESH2_CpuNolink(cpu))
		{
			pc=cpu->regs[BTESH2_REG_PC];
			if(pc!=lpc)
			{
				if(cpu->logpc)
				{
					if(cpu->nlogpc<cpu->mlogpc)
					{
						cpu->logsp[cpu->nlogpc]=
							cpu->regs[BTESH2_REG_SP];
						cpu->logpc[cpu->nlogpc++]=pc;
					}
				}

				cpu->trpc[cpu->trpc_rov]=pc;
				cpu->trpc_rov=(cpu->trpc_rov+1)&63;
				lpc=pc; ld=0;
			}else
			{
				if(!ld)
				{
					if(cpu->logpc)
					{
						if(cpu->nlogpc<cpu->mlogpc)
						{
							cpu->logsp[cpu->nlogpc]=
								cpu->regs[BTESH2_REG_SP];
							cpu->logpc[cpu->nlogpc++]=pc|1;
						}
					}

					cpu->trpc[cpu->trpc_rov]=pc|1;
					cpu->trpc_rov=(cpu->trpc_rov+1)&63;
					ld=1;
				}
			}
		}
#endif

//		printf("PC=%08X\n", cpu->regs[BTESH2_REG_PC]);

		if(!trnxt)
		{
			tr=BTESH2_TraceForAddr(cpu, cpu->regs[BTESH2_REG_PC]);
		}else
		{
			tr=trnxt;
		}

//		BTESH2_PrintTrace(cpu, tr);
		cpu->cur_trace=tr;
		trnxt=tr->Run(cpu, tr);
//		cpu->tr_tops+=tr->nops;
		cpu->tr_tops+=tr->nwops;
//		while(trnxt && ((l--)>0))
		while(trnxt && ((cpu->tr_runlim--)>0))
		{
			tr=trnxt;
			trnxt=tr->Run(cpu, tr);
//			cpu->tr_tops+=tr->nops;
			cpu->tr_tops+=tr->nwops;
		}
		
		if(cpu->status)
		{
#if 1
			if(cpu->status==(cpu->status&255))
			{
				BTESH2_RestoreTrap(cpu);
				BTSH_Op_TrapInt(cpu, cpu->status);
				if(!cpu->status)
					continue;
			}
#endif
			break;
		}
	}

	cpu->dbg_lpc=lpc; cpu->dbg_ld=ld;

	return(cpu->status);
}

int BTESH2_DumpTraces(BTESH2_CpuState *cpu)
{
	u32 spa[256];
	FILE *fd;
	BTESH2_Trace *tr;
	char *snm;
	u32 pc, baddr, sp;
	int si;
	int i, j, k;

	for(i=0; i<64; i++)
	{
		j=(cpu->trpc_rov-64+i)&63;
		pc=cpu->trpc[j];
		if(!pc)
			continue;
		if(pc&1)
		{
			printf("@%08X ...\n", pc&(~1));
			continue;
		}

		tr=BTESH2_TraceForAddr(cpu, pc);
		BTESH2_PrintTrace(cpu, tr);
	}
	
	sp=cpu->logsp[0];
	si=0; spa[si]=sp;

	if(cpu->logpc)
	{
		fd=fopen("btesh2_logpc.txt", "wt");
		for(i=0; i<cpu->nlogpc; i++)
		{
			sp=cpu->logsp[i];
			if(sp<spa[si])
			{
				si++;
				spa[si]=sp;
			}else if(sp>spa[si])
			{
				while((si>0) && (sp>spa[si]))
					si--;
				if(sp>spa[si])
					spa[si]=sp;
			}
		
			for(j=0; j<si; j++)
				fputc(' ', fd);
		
			pc=cpu->logpc[i];
			if(!pc)
				continue;
			if(pc&1)
			{
				fprintf(fd, "@%08X ...\n", pc&(~1));
				continue;
			}

			j=BTESH2_LookupNameForAddr(cpu, pc, &snm, &baddr);
			if(j>0)
			{
				fprintf(fd, "@=%08X %s+%06X\n",
					pc, snm, pc-baddr);
			}else
			{
				fprintf(fd, "@=%08X\n", pc);
			}
		}
		fclose(fd);
	}
	return(0);
}

int BTESH2_StatTraces(BTESH2_CpuState *cpu)
{
	int tpair_v[32768];
	int tpair_c[32768];
	BTESH2_Trace *tr;
	int ntp;
	int nt, ntl, tno, ano;
	int i, j, k;
	
	nt=0; ntl=0; tno=0;
	for(i=0; i<(BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL); i++)
	{
		tr=cpu->icache[i];
		if(!tr)continue;
		nt++;
		tno+=tr->nops;
		if(tr->nops>=(BTESH2_TR_MAXOPS-1))
			ntl++;
	}
	
//	ano=tno/nt;
	printf("Avg Tr: nOps=%.3f, AtLim=%.2f%%\n",
		((double)tno)/nt, (ntl*100.0)/nt);
	printf("Nt=%d %.2f%%\n", nt, (nt*100.0)/
		(BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL));
	printf("dCol/dTot=%d/%d %.2f%%\n",
		cpu->tr_dcol, cpu->tr_dtot, (cpu->tr_dcol*100.0)/cpu->tr_dtot);
	
	ntp=0;
	for(i=0; i<65536; i++)
	{
		j=btesh2_nmid_pairs[i];
		if(j)
		{
			tpair_v[ntp]=i;
			tpair_c[ntp]=j;
			ntp++;
		}
	}
	
	for(i=0; i<ntp; i++)
	{
		for(j=i+1; j<ntp; j++)
		{
			if(tpair_c[j]>tpair_c[i])
			{
				k=tpair_c[i]; tpair_c[i]=tpair_c[j]; tpair_c[j]=k;
				k=tpair_v[i]; tpair_v[i]=tpair_v[j]; tpair_v[j]=k;
			}
		}
	}
	
	for(i=0; i<32; i++)
	{
		j=tpair_v[i];
		printf("%2d %-8s %-8s %6d\n", i,
			btesh2_print_NameForNameID((j>>8)&255),
			btesh2_print_NameForNameID((j   )&255),
			tpair_c[i]);
	}

	return(0);
}

int BTESH2_DumpRegs(BTESH2_CpuState *cpu)
{
	u64 d0, d1, d2, d3;
	int i, j, k, l;
	
	BTESH2_DumpTraces(cpu);
//	BTESH2_StatTraces(cpu);

	printf("Current GPRs:\n");
	printf("R0   =%08X | R1  =%08X | R2   =%08X | R3   =%08X\n",
		cpu->regs[ 0], cpu->regs[ 1], cpu->regs[ 2], cpu->regs[ 3]);
	printf("R4   =%08X | R5  =%08X | R6   =%08X | R7   =%08X\n",
		cpu->regs[ 4], cpu->regs[ 5], cpu->regs[ 6], cpu->regs[ 7]);
	printf("R8   =%08X | R9  =%08X | R10  =%08X | R11  =%08X\n",
		cpu->regs[ 8], cpu->regs[ 9], cpu->regs[10], cpu->regs[11]);
	printf("R12  =%08X | R13 =%08X | R14  =%08X | R15  =%08X\n",
		cpu->regs[12], cpu->regs[13], cpu->regs[14], cpu->regs[15]);

	if(cpu->arch==BTESH2_ARCH_SH4)
	{
		printf("Banked GPRs:\n");
		printf("R0_B =%08X | R1_B=%08X | R2_B =%08X | R3_B =%08X\n",
			cpu->regs[32], cpu->regs[33], cpu->regs[34], cpu->regs[35]);
		printf("R4_B =%08X | R5_B=%08X | R6_B =%08X | R7_B =%08X\n",
			cpu->regs[36], cpu->regs[37], cpu->regs[38], cpu->regs[39]);

		printf("FPU Regs (Raw):\n");
		printf("FR0  =%08X | FR1 =%08X | FR2  =%08X | FR3  =%08X\n",
			cpu->fregs[ 0], cpu->fregs[ 1], cpu->fregs[ 2], cpu->fregs[ 3]);
		printf("FR4  =%08X | FR5 =%08X | FR6  =%08X | FR7  =%08X\n",
			cpu->fregs[ 4], cpu->fregs[ 5], cpu->fregs[ 6], cpu->fregs[ 7]);
		printf("FR8  =%08X | FR9 =%08X | FR10 =%08X | FR11 =%08X\n",
			cpu->fregs[ 8], cpu->fregs[ 9], cpu->fregs[10], cpu->fregs[11]);
		printf("FR12 =%08X | FR13=%08X | FR14 =%08X | FR15 =%08X\n",
			cpu->fregs[12], cpu->fregs[13], cpu->fregs[14], cpu->fregs[15]);

		printf("FPU Regs (Raw, Bank B):\n");
		printf("XF0  =%08X | XF1 =%08X | XF2  =%08X | XF3  =%08X\n",
			cpu->fregs[16], cpu->fregs[17], cpu->fregs[18], cpu->fregs[19]);
		printf("XF4  =%08X | XF5 =%08X | XF6  =%08X | XF7  =%08X\n",
			cpu->fregs[20], cpu->fregs[21], cpu->fregs[22], cpu->fregs[23]);
		printf("XF8  =%08X | XF9 =%08X | XF10 =%08X | XF11 =%08X\n",
			cpu->fregs[24], cpu->fregs[25], cpu->fregs[26], cpu->fregs[27]);
		printf("XF12 =%08X | XF13=%08X | XF14 =%08X | XF15 =%08X\n",
			cpu->fregs[28], cpu->fregs[29], cpu->fregs[30], cpu->fregs[31]);

		printf("FPU Regs (Float):\n");
		printf("FR0 =% e |FR1 =% e |FR2 =% e |FR3 =% e\n",
			((float *)cpu->fregs)[ 0], ((float *)cpu->fregs)[ 1],
			((float *)cpu->fregs)[ 2], ((float *)cpu->fregs)[ 3]);
		printf("FR4 =% e |FR5 =% e |FR6 =% e |FR7 =% e\n",
			((float *)cpu->fregs)[ 4], ((float *)cpu->fregs)[ 5],
			((float *)cpu->fregs)[ 6], ((float *)cpu->fregs)[ 7]);
		printf("FR8 =% e |FR9 =% e |FR10=% e |FR11=% e\n",
			((float *)cpu->fregs)[ 8], ((float *)cpu->fregs)[ 9],
			((float *)cpu->fregs)[10], ((float *)cpu->fregs)[11]);
		printf("FR12=% e |FR13=% e |FR14=% e |FR15=% e\n",
			((float *)cpu->fregs)[12], ((float *)cpu->fregs)[13],
			((float *)cpu->fregs)[14], ((float *)cpu->fregs)[15]);

#if 1
		printf("FPU Regs (Double):\n");
		d0=(((u64)cpu->fregs[ 0])<<32)|(cpu->fregs[ 1]);
		d1=(((u64)cpu->fregs[ 2])<<32)|(cpu->fregs[ 3]);
		d2=(((u64)cpu->fregs[ 4])<<32)|(cpu->fregs[ 5]);
		d3=(((u64)cpu->fregs[ 8])<<32)|(cpu->fregs[ 7]);
		printf("DR0 =% e |DR2 =% e |DR4 =% e |DR6 =% e\n",
			*(double *)(&d0), *(double *)(&d1),
			*(double *)(&d2), *(double *)(&d3));
		d0=(((u64)cpu->fregs[ 8])<<32)|(cpu->fregs[ 9]);
		d1=(((u64)cpu->fregs[10])<<32)|(cpu->fregs[11]);
		d2=(((u64)cpu->fregs[12])<<32)|(cpu->fregs[13]);
		d3=(((u64)cpu->fregs[14])<<32)|(cpu->fregs[15]);
		printf("DR8 =% e |DR10=% e |DR12=% e |DR14=% e\n",
			*(double *)(&d0), *(double *)(&d1),
			*(double *)(&d2), *(double *)(&d3));

//		printf("\n");
#endif

#if 0
		printf("FPU Regs (Float, Bank B):\n");
		printf("XF0 =% e |XF1 =% e |XF2 =% e |XF3 =% e\n",
			((float *)cpu->fregs)[16], ((float *)cpu->fregs)[17],
			((float *)cpu->fregs)[18], ((float *)cpu->fregs)[19]);
		printf("XF4 =% e |XF5 =% e |XF6 =% e |XF7 =% e\n",
			((float *)cpu->fregs)[20], ((float *)cpu->fregs)[21],
			((float *)cpu->fregs)[22], ((float *)cpu->fregs)[23]);
		printf("XF8 =% e |XF9 =% e |XF10=% e |XF11=% e\n",
			((float *)cpu->fregs)[24], ((float *)cpu->fregs)[25],
			((float *)cpu->fregs)[26], ((float *)cpu->fregs)[27]);
		printf("XF12=% e |XF13=% e |XF14=% e |XF15=% e\n",
			((float *)cpu->fregs)[28], ((float *)cpu->fregs)[29],
			((float *)cpu->fregs)[30], ((float *)cpu->fregs)[31]);
#endif
#if 0
		printf("FPU Regs (Double, Bank B):\n");
		d0=(((u64)cpu->fregs[16])<<32)|(cpu->fregs[17]);
		d1=(((u64)cpu->fregs[18])<<32)|(cpu->fregs[19]);
		d2=(((u64)cpu->fregs[20])<<32)|(cpu->fregs[21]);
		d3=(((u64)cpu->fregs[22])<<32)|(cpu->fregs[23]);
		printf("XD0 =% e |XD2 =% e |XD4 =% e |XD6 =% e\n",
			*(double *)(&d0), *(double *)(&d1),
			*(double *)(&d2), *(double *)(&d3));
		d0=(((u64)cpu->fregs[24])<<32)|(cpu->fregs[25]);
		d1=(((u64)cpu->fregs[26])<<32)|(cpu->fregs[27]);
		d2=(((u64)cpu->fregs[28])<<32)|(cpu->fregs[29]);
		d3=(((u64)cpu->fregs[30])<<32)|(cpu->fregs[31]);
		printf("XD8 =% e |XD10=% e |XD12=% e |XD14=% e\n",
			*(double *)(&d0), *(double *)(&d1),
			*(double *)(&d2), *(double *)(&d3));

//		printf("\n");
#endif
	}

	printf("Control/Status:\n");
	printf("SR   =%08X | GBR =%08X | VBR  =%08X | fla  =%08X\n",
		cpu->regs[16], cpu->regs[17], cpu->regs[18], cpu->regs[19]);
	printf("MACH =%08X | MACL=%08X | PR   =%08X | PC   =%08X\n",
		cpu->regs[20], cpu->regs[21], cpu->regs[22], cpu->regs[23]);

	if(cpu->arch==BTESH2_ARCH_SH4)
	{
		printf("FPSCR=%08X | FPUL=%08X | flb  =%08X | MMUCR=%08X\n",
			cpu->regs[24], cpu->regs[25], cpu->regs[26], cpu->regs[27]);
		printf("PTEH =%08X | PTEL=%08X | TTB  =%08X | TEA  =%08X\n",
			cpu->regs[28], cpu->regs[29], cpu->regs[30], cpu->regs[31]);

		printf("SSR  =%08X | SPC =%08X | SGR  =%08X | DBR  =%08X\n",
			cpu->regs[40], cpu->regs[41], cpu->regs[42], cpu->regs[43]);
	}
	
	printf("Stack:\n");
	for(i=0; i<8; i++)
	{
		printf("% 4d: ", i*8*4);
		for(j=0; j<8; j++)
		{
			if(j)printf(" ");
			k=i*8+j;
			l=BTESH2_GetAddrDWord(cpu, cpu->regs[15]+k*4);
			printf("%08X", l);
		}
		printf("\n");
	}
	
#if 0
	printf("Trap:\n");
	printf("R0 =%08X R1 =%08X R2 =%08X R3 =%08X\n",
		cpu->trapregs[ 0], cpu->trapregs[ 1],
		cpu->trapregs[ 2], cpu->trapregs[ 3]);
	printf("R4 =%08X R5 =%08X R6 =%08X R7 =%08X\n",
		cpu->trapregs[ 4], cpu->trapregs[ 5],
		cpu->trapregs[ 6], cpu->trapregs[ 7]);
	printf("R8 =%08X R9 =%08X R10=%08X R11=%08X\n",
		cpu->trapregs[ 8], cpu->trapregs[ 9],
		cpu->trapregs[10], cpu->trapregs[11]);
	printf("R12=%08X R13=%08X R14=%08X R15=%08X\n",
		cpu->trapregs[12], cpu->trapregs[13],
		cpu->trapregs[14], cpu->trapregs[15]);
	printf("SR =%08X GBR=%08X VBR=%08X fla=%08X\n",
		cpu->trapregs[16], cpu->trapregs[17],
		cpu->trapregs[18], cpu->trapregs[19]);
	printf("MCH=%08X MCL=%08X PR =%08X PC =%08X\n",
		cpu->trapregs[20], cpu->trapregs[21],
		cpu->trapregs[22], cpu->trapregs[23]);
#endif
	return(0);
}
