#if 1

int BTESH2_DecodeOpcodeVLIW(BTESH2_CpuState *cpu,
	BTESH2_Opcode **rop1,
	BTESH2_Opcode **rop2,
	BTESH2_Opcode **rop3,
	btesh2_vaddr pc,
	int *rstep)
{
	BTESH2_Opcode *op1, *op2, *op3;
	btesh2_vaddr pc1, pc2, pc3;
	u64 vwop;
	u32 vopw1, vopw2, vopw3;
	u16 vw1, vw2, vw3, vw4;
	u16 opw1, opw2;
	int isds;
	int i, j, k;

	vw1=BTESH2_GetAddrWord(cpu, pc+0);

	if((vw1&0xFFF0)!=0x8CA0)
	{
		op1=BTESH2_AllocOpcode(cpu);		
		op1->rm=BTESH2_REG_ZZR; op1->rn=BTESH2_REG_ZZR; op1->ro=BTESH2_REG_ZZR;

		BTESH2_DecodeOpcode(cpu, op1, pc);
		
		isds=2;
		if(op1->fl&BTESH2_OPFL_EXTRAWORD)
			isds+=2;
		
		*rop1=op1;
		*rop2=NULL;
		*rop3=NULL;
		*rstep=isds;
		return(0);
	}

	vw2=BTESH2_GetAddrWord(cpu, pc+2);
	vw3=BTESH2_GetAddrWord(cpu, pc+4);
	vw4=BTESH2_GetAddrWord(cpu, pc+6);


	pc1=pc+1;
	pc2=pc+3;

	op1=BTESH2_AllocOpcode(cpu);		
	op2=BTESH2_AllocOpcode(cpu);		
	op1->rm=BTESH2_REG_ZZR; op1->rn=BTESH2_REG_ZZR; op1->ro=BTESH2_REG_ZZR;
	op2->rm=BTESH2_REG_ZZR; op2->rn=BTESH2_REG_ZZR; op2->ro=BTESH2_REG_ZZR;

	opw1=0xCE00|((vw2>>8)&255);	opw2=vw3;
	BTESH2_DecodeOpcode_BJX1_CE(cpu, op1, pc1, opw1, opw2);
	opw1=0xCE00|((vw2   )&255);	opw2=vw4;
	BTESH2_DecodeOpcode_BJX1_CE(cpu, op2, pc2, opw1, opw2);

	*rop1=op1;
	*rop2=op2;
	*rop3=NULL;
	*rstep=8;

	return(0);


#if 0
//	vwop=BTESH2_GetAddrQWord(cpu, pc&(~7));

	pc1=pc|0;
	pc2=pc|2;
	pc3=pc|4;

	op1=BTESH2_AllocOpcode(cpu);		
	op2=BTESH2_AllocOpcode(cpu);		
	op3=BTESH2_AllocOpcode(cpu);		
	op1->rm=BTESH2_REG_ZZR; op1->rn=BTESH2_REG_ZZR; op1->ro=BTESH2_REG_ZZR;
	op2->rm=BTESH2_REG_ZZR; op2->rn=BTESH2_REG_ZZR; op2->ro=BTESH2_REG_ZZR;
	op3->rm=BTESH2_REG_ZZR; op3->rn=BTESH2_REG_ZZR; op3->ro=BTESH2_REG_ZZR;

	vopw1=((vwop    )&0xFFFFF);
	vopw2=((vwop>>20)&0xFFFFF);
	vopw3=((vwop>>40)&0xFFFFF);

	opw1=0xCC00|(vopw1>>16);	opw2=vopw1;
	BTESH2_DecodeOpcode_BJX1_CC0W(cpu, op1, pc1, opw1, opw2);
	opw1=0xCC00|(vopw2>>16);	opw2=vopw2;
	BTESH2_DecodeOpcode_BJX1_CC0W(cpu, op2, pc2, opw1, opw2);
	opw1=0xCC00|(vopw3>>16);	opw2=vopw3;
	BTESH2_DecodeOpcode_BJX1_CC0W(cpu, op3, pc3, opw1, opw2);
	
	*rop1=op1;
	*rop2=op2;
	*rop3=op3;

	return(0);
#endif
}

static int btesh2_shuffle_rov;

int BTESH2_ShuffleOpcodeVLIW(BTESH2_CpuState *cpu,
	BTESH2_Opcode **rop1,
	BTESH2_Opcode **rop2,
	BTESH2_Opcode **rop3)
{
	BTESH2_Opcode *op1, *op2, *op3;
	BTESH2_Opcode *op4, *op5, *op6;
	int ok;

	op1=*rop1;	op2=*rop2;	op3=*rop3; ok=0;
	
	if(op3)
	{
		do {
			switch((btesh2_shuffle_rov>>16)&7)
			{
				case 0: op4=op1; op5=op2; op6=op3; ok=1; break;
				case 1: op4=op2; op5=op1; op6=op3; ok=1; break;
				case 2: op4=op3; op5=op2; op6=op1; ok=1; break;

				case 4: op4=op1; op5=op3; op6=op2; ok=1; break;
				case 5: op4=op2; op5=op3; op6=op1; ok=1; break;
				case 6: op4=op3; op5=op1; op6=op2; ok=1; break;
			}
			btesh2_shuffle_rov=btesh2_shuffle_rov*65521+1;
		}while(!ok);
		
		*rop1=op4;
		*rop2=op5;
		*rop3=op6;
		return(1);
	}
	
	if(op2)
	{
		if((btesh2_shuffle_rov>>16)&1)
			{ op4=op2; op5=op1; }
		else
			{ op4=op1; op5=op2; }
		btesh2_shuffle_rov=btesh2_shuffle_rov*65521+1;
		*rop1=op4;
		*rop2=op5;
		*rop3=NULL;
		return(1);
	}

	*rop1=op1;
	*rop2=NULL;
	*rop3=NULL;
	return(1);
}

int BTESH2_DecodeTraceVLIW(BTESH2_CpuState *cpu,
	BTESH2_Trace *tr, btesh2_vaddr spc)
{
	BTESH2_Opcode *op1, *op2, *op3;
	BTESH2_Opcode *op4, *op5, *op6;
	btesh2_vaddr pc, brapc, jmppc;
	int i0, i1, i2, i3, stp;
	int i, j, k, n;
	
	if((spc&7)!=1)
	{
//		BTESH2_DBGBREAK
	}
	
	btesh2_shuffle_rov=btesh2_shuffle_rov*65521+spc;
	
	tr->srcpc=spc;
	tr->csfl=cpu->csfl;
	
	pc=spc; n=0; brapc=0; jmppc=0;
	while((n+3)<BTESH2_TR_MAXOPS)
	{
		BTESH2_MarkAddrTrapSmc(cpu, pc+0);
		BTESH2_MarkAddrTrapSmc(cpu, pc+6);
		
//		BTESH2_DecodeOpcodeVLIW(cpu, op1, op2, op3, pc);
		BTESH2_DecodeOpcodeVLIW(cpu, &op1, &op2, &op3, pc, &stp);

		pc+=stp;
		if(pc>tr->maxpc)
			tr->maxpc=pc;
		
		if(!op1->Run)
		{
			op1->nmid=BTESH2_NMID_INVALID;
			op1->fmid=BTESH2_FMID_NONE;
			op1->fl|=BTESH2_OPFL_CTRLF;
			op1->Run=BTSH_Op_TRAP_UD;
		}

		if(op2 && !op2->Run)
		{
			op2->nmid=BTESH2_NMID_INVALID;
			op2->fmid=BTESH2_FMID_NONE;
			op2->fl|=BTESH2_OPFL_CTRLF;
			op2->Run=BTSH_Op_TRAP_UD;
		}

		if(op3 && !op3->Run)
		{
			op3->nmid=BTESH2_NMID_INVALID;
			op3->fmid=BTESH2_FMID_NONE;
			op3->fl|=BTESH2_OPFL_CTRLF;
			op3->Run=BTSH_Op_TRAP_UD;
		}
		
		if(op1->fl&BTESH2_OPFL_CTRLF)
		{
			if(op1->fl&BTESH2_OPFL_DLYSLOT)
			{
				if(n<=(BTESH2_TR_MAXOPS-6))
				{
//					op4=BTESH2_AllocOpcode(cpu);
//					op5=BTESH2_AllocOpcode(cpu);
//					op6=BTESH2_AllocOpcode(cpu);
//					op4->fl=BTESH2_OPFL_DLYSLOT;	op4->rm=BTESH2_REG_ZZR;
//					op4->rn=BTESH2_REG_ZZR;			op4->ro=BTESH2_REG_ZZR;
//					op5->fl=BTESH2_OPFL_DLYSLOT;	op5->rm=BTESH2_REG_ZZR;
//					op5->rn=BTESH2_REG_ZZR;			op5->ro=BTESH2_REG_ZZR;
//					op6->fl=BTESH2_OPFL_DLYSLOT;	op6->rm=BTESH2_REG_ZZR;
//					op6->rn=BTESH2_REG_ZZR;			op6->ro=BTESH2_REG_ZZR;

					BTESH2_DecodeOpcodeVLIW(cpu, &op4, &op5, &op6, pc, &stp);
					pc+=stp;
					if(pc>tr->maxpc)
						tr->maxpc=pc;

//					if(op4->fl&BTESH2_OPFL_PCADLYSLOTD)
//						op4->imm=((op1->pc+6)&(~3))+(4*op1->ro);
//					if(op4->fl&BTESH2_OPFL_PCADLYSLOTW)
//						op4->imm=(op1->pc+6)+(2*op1->ro);

					if(!op4->Run)
					{
						op4->nmid=BTESH2_NMID_INVALID;
						op4->fmid=BTESH2_FMID_NONE;
						op4->fl=BTESH2_OPFL_CTRLF;
						op4->Run=BTSH_Op_TRAP_UD;
					}

					if(op5 && !op5->Run)
					{
						op5->nmid=BTESH2_NMID_INVALID;
						op5->fmid=BTESH2_FMID_NONE;
						op5->fl=BTESH2_OPFL_CTRLF;
						op5->Run=BTSH_Op_TRAP_UD;
					}

					if(op6 && !op6->Run)
					{
						op6->nmid=BTESH2_NMID_INVALID;
						op6->fmid=BTESH2_FMID_NONE;
						op6->fl=BTESH2_OPFL_CTRLF;
						op6->Run=BTSH_Op_TRAP_UD;
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

					if(	(op1->nmid==BTESH2_NMID_BRA) &&
						(op1->fmid==BTESH2_FMID_ABS))
					{
						brapc=op1->imm;
					}

					if((	(op1->nmid==BTESH2_NMID_BF) ||
							(op1->nmid==BTESH2_NMID_BFS) ||
							(op1->nmid==BTESH2_NMID_BT) ||
							(op1->nmid==BTESH2_NMID_BTS) ||
							(op1->nmid==BTESH2_NMID_BSR)) &&
						(op1->fmid==BTESH2_FMID_ABS))
					{
						jmppc=op1->imm;
					}

					BTESH2_ShuffleOpcodeVLIW(cpu, &op1, &op2, &op3);
					BTESH2_ShuffleOpcodeVLIW(cpu, &op4, &op5, &op6);

					tr->ops[n++]=op1;
					if(op2)
						tr->ops[n++]=op2;
					if(op3)
						tr->ops[n++]=op3;
					tr->ops[n++]=op4;
					if(op5)
						tr->ops[n++]=op5;
					if(op6)
						tr->ops[n++]=op6;

					break;
				}
				
				BTESH2_FreeOpcode(cpu, op1);
				if(op2)
					BTESH2_FreeOpcode(cpu, op2);
				if(op3)
					BTESH2_FreeOpcode(cpu, op3);
				pc-=8;
				break;
			}

			if((	(op1->nmid==BTESH2_NMID_BF) ||
					(op1->nmid==BTESH2_NMID_BT) ||
					(op1->nmid==BTESH2_NMID_BSR)) &&
				(op1->fmid==BTESH2_FMID_ABS))
			{
				jmppc=op1->imm;
			}

			BTESH2_ShuffleOpcodeVLIW(cpu, &op1, &op2, &op3);
			tr->ops[n++]=op1;
			if(op2)
				tr->ops[n++]=op2;
			if(op3)
				tr->ops[n++]=op3;
			break;
		}else
		{
			BTESH2_ShuffleOpcodeVLIW(cpu, &op1, &op2, &op3);
			tr->ops[n++]=op1;
			if(op2)
				tr->ops[n++]=op2;
			if(op3)
				tr->ops[n++]=op3;
		}
	}


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

#endif
