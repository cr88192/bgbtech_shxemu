int BTESH2_DecodeOpcode(BTESH2_CpuState *cpu,
	BTESH2_Opcode *op, btesh2_vaddr pc)
{
	if(cpu->subarch==BTESH2_SUBARCH_BJX1_64C)
	{
		return(BTESH2_DecodeOpcode_B64C(cpu, op, pc));
	}
	
	return(BTESH2_DecodeOpcode_SH(cpu, op, pc));
}

int BTESH2_DecodeOpcode_SH(BTESH2_CpuState *cpu,
	BTESH2_Opcode *op, btesh2_vaddr pc)
{
	static const byte ldow2[16]={1,0,0,1, 0,0,1,0, 1,0,0,0, 1,0,1,1};
	u16 opw, opw2;
	int isds;
	int i, j, k;
	
	opw=BTESH2_GetAddrWord(cpu, pc);

	if(ldow2[opw>>12])
		{ opw2=BTESH2_GetAddrWord(cpu, pc+2); }
	else
		{ opw2=0; }

	isds=op->fl&BTESH2_OPFL_DLYSLOT;
	op->fl=0;

	op->opw=opw;
	op->opw2=opw2;
	op->pc=pc;
	switch(opw>>12)
	{
	case 0x0: /* 0--- */
		switch(opw&15)
		{
		case 0x0: /* 0--0 */
			BTESH2_DecodeOpcode_2A(cpu, op, pc, opw, opw2);
			break;
		case 0x1: /* 0--1 */
			BTESH2_DecodeOpcode_2A(cpu, op, pc, opw, opw2);
			break;
		case 0x2: /* 0--2 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 0-02 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_SR;
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegRegSr;
				break;
			case 0x1: /* 0-12 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_GBR;
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x2: /* 0-22 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;

			case 0x3: /* 0-32 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_SSR;
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x4: /* 0-42 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_SPC;
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;

			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rn=(opw>>8)&15;
				op->rm=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			}
			break;
		case 0x3: /* 0--3 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 0-03 */
				op->rm=(opw>>8)&15;
				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
					BTESH2_OPFL_INVDLYSLOT;
				op->nmid=BTESH2_NMID_BSRF;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_BSRF_Reg;
				break;
			case 0x2: /* 0-23 */
				op->rm=(opw>>8)&15;
				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
					BTESH2_OPFL_INVDLYSLOT;
				op->nmid=BTESH2_NMID_BRAF;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_BRAF_Reg;
				break;

			case 0x8:
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->nmid=BTESH2_NMID_PREF;
				op->fmid=BTESH2_FMID_STREG;
				op->Run=BTSH_Op_PREF_StReg;
				break;
			case 0x9:
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->nmid=BTESH2_NMID_OCBI;
				op->fmid=BTESH2_FMID_STREG;
				op->Run=BTSH_Op_OCBI_StReg;
				break;
			case 0xA:
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->nmid=BTESH2_NMID_OCBP;
				op->fmid=BTESH2_FMID_STREG;
				op->Run=BTSH_Op_OCBP_StReg;
				break;
			case 0xB:
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->nmid=BTESH2_NMID_OCBWB;
				op->fmid=BTESH2_FMID_STREG;
				op->Run=BTSH_Op_OCBWB_StReg;
				break;

			case 0xC: /* 0-C3 */
				op->rn=(opw>>8)&15;
				op->rm=0;
//				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT;
				op->nmid=BTESH2_NMID_MOVCAL;
				op->fmid=BTESH2_FMID_REGST;
				op->Run=BTSH_Op_MOV_RegStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegStD;
				break;

			case 0xE:
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->nmid=BTESH2_NMID_ICBI;
				op->fmid=BTESH2_FMID_STREG;
				op->Run=BTSH_Op_NOP_Z;
				break;
			}
			break;
		case 0x4: /* 0--4 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_Op_MOV_RegStR0nB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStR0nB;
			break;
		case 0x5: /* 0--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_Op_MOV_RegStR0nW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegStR0nW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegStR0nQ;
				}
			}
			break;
		case 0x6: /* 0--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_Op_MOV_RegStR0nD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStR0nD;
			break;
		case 0x7: /* 0--7 */
//			if(cpu->csfl&BTESH2_CSFL_SRDQ)
//				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MULL_RegReg;

			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_Op_MULL_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MULL;
					op->Run=BTSH_Op_MULQ_RegRegQ;
				}
				break;
			}
			break;
		case 0x8: /* 0--8 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 0-08 */
				op->nmid=BTESH2_NMID_CLRT;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_CLRT_Z;
				break;
			case 0x1: /* 0-18 */
				op->nmid=BTESH2_NMID_SETT;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_SETT_Z;
				break;
			case 0x2: /* 0-28 */
				op->nmid=BTESH2_NMID_CLRMAC;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_CLRMAC_Z;
				break;
			case 0x3: /* 0-38 */
				if(!cpu->mmu_usetmmu)
				{
					cpu->mmu_usetmmu=1;
					BTESH2_SetupUpdateFMMU(cpu);
				}
				op->nmid=BTESH2_NMID_LDTLB;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_LDTLB_Z;
				op->fl|=BTESH2_OPFL_CTRLF;
				break;
			case 0x4: /* 0-48 */
				switch((opw>>8)&15)
				{
				case 0x0:
					op->nmid=BTESH2_NMID_CLRS;
					op->fmid=BTESH2_FMID_NONE;
					op->Run=BTSH_Op_CLRS_Z;
					break;
				case 0x1: case 0x2: case 0x3:
					op->imm=(opw>>8)&15;
					op->nmid=BTESH2_NMID_ICLRMD;
					op->fmid=BTESH2_FMID_IMM;
					op->Run=BTSH_Op_ICLRMD_Imm;
#if 0
//					if(op->imm&2)
//						{ cpu->csfl&=~BTESH2_CSFL_SRJQ; }
//					if(op->imm&1)
//						{ cpu->csfl&=~BTESH2_CSFL_SRDQ; }
#endif
					op->fl|=BTESH2_OPFL_CTRLF;
					break;

				case 0x8:
					op->nmid=BTESH2_NMID_CLRS;
					op->fmid=BTESH2_FMID_NONE;
					op->Run=BTSH_Op_TRAPSP_CLR_Z;
					break;
				}
				break;
			case 0x5: /* 0-58 */
				switch((opw>>8)&15)
				{
				case 0x0:
					op->nmid=BTESH2_NMID_SETS;
					op->fmid=BTESH2_FMID_NONE;
					op->Run=BTSH_Op_SETS_Z;
					break;
				case 0x1: case 0x2: case 0x3:
					op->imm=(opw>>8)&15;
					op->nmid=BTESH2_NMID_ISETMD;
					op->fmid=BTESH2_FMID_IMM;
					op->Run=BTSH_Op_ISETMD_Imm;
#if 0
//					if(op->imm&2)
//						{ cpu->csfl|=BTESH2_CSFL_SRJQ; }
//					if(op->imm&1)
//						{ cpu->csfl|=BTESH2_CSFL_SRDQ; }
#endif
					op->fl|=BTESH2_OPFL_CTRLF;
					break;

				case 0x8:
					op->nmid=BTESH2_NMID_SETS;
					op->fmid=BTESH2_FMID_NONE;
					op->Run=BTSH_Op_TRAPSP_SET_Z;
					break;
				}
				break;
			}
			break;
		case 0x9: /* 0--9 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 0-09 */
				op->nmid=BTESH2_NMID_NOP;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_NOP_Z;
				break;
			case 0x1: /* 0-19 */
				op->nmid=BTESH2_NMID_DIV0U;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_DIV0U_Z;
				cpu->ftopfl|=BTESH2_FTOPFL_SEEN_DIV0U;
				break;
			case 0x2: /* 0-29 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_MOVT;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_MOVT_Reg;
				break;
			}
			break;
		case 0xA: /* 0--A */
			switch((opw>>4)&15)
			{
			case 0x0: /* 0-0A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_MACH;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x1: /* 0-1A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x2: /* 0-2A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x3: /* 0-3A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_SGR;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;

			case 0x5: /* 0-5A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x6: /* 0-6A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			}
			break;
		case 0xB: /* 0--B */
			switch((opw>>4)&15)
			{
			case 0x0: /* 0-0B */
//				op->rn=(opw>>8)&15;
				op->Run=BTSH_Op_RTS_Z;
				op->nmid=BTESH2_NMID_RTS;
				op->fmid=BTESH2_FMID_NONE;
				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
					BTESH2_OPFL_INVDLYSLOT;
				break;
			case 0x1: /* 0-1B */
				op->fl=BTESH2_OPFL_CTRLF;
				op->nmid=BTESH2_NMID_SLEEP;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_SLEEP_Z;
				break;
			case 0x2: /* 0-2B */
				op->Run=BTSH_Op_RTE_Z;
				op->nmid=BTESH2_NMID_RTE;
				op->fmid=BTESH2_FMID_NONE;
				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
					BTESH2_OPFL_INVDLYSLOT;
				break;
			case 0x3: /* 0-3B */
				op->fl=BTESH2_OPFL_CTRLF;
				op->nmid=BTESH2_NMID_BRK;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_TRAP_BRK;
				break;

			case 0x6: /* 0-6B */
				op->Run=BTSH_Op_RTS_Z;
				op->nmid=BTESH2_NMID_RTSN;
				op->fmid=BTESH2_FMID_NONE;
				op->fl=BTESH2_OPFL_CTRLF|
					BTESH2_OPFL_INVDLYSLOT;
				break;
			}
			break;
		case 0xC: /* 0--C */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_Op_MOV_RegLdR0mB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdR0mB;
			break;
		case 0xD: /* 0--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_Op_MOV_RegLdR0mW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegLdR0mW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegLdR0mQ;
				}
			}
			break;
		case 0xE: /* 0--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_Op_MOV_RegLdR0mD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdR0mD;
			break;
		case 0xF: /* 0--F */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MACL;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MACL_RegReg;
			break;
		}
		break;
	case 0x1: /* 1--- */
		op->rn=(opw>>8)&15;
		op->rm=(opw>>4)&15;
		op->imm=(opw&15)*4;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGSTDISP;
		op->Run=BTSH_Op_MOV_RegStDispD;
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
			op->Run=BTSH_OpJQ_MOV_RegStDispD;
		break;
	case 0x2: /* 2--- */
		switch(opw&15)
		{
		case 0x0: /* 2--0 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_MOV_RegStB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStB;
			break;
		case 0x1: /* 2--1 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_MOV_RegStW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegStW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegStQ;
				}
			}
			break;
		case 0x2: /* 2--2 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_MOV_RegStD;
			if(BTESH2_CheckCpuFmmuP(cpu))
				op->Run=BTSH_Op_MOV_RegStD_FMMU;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStD;
			break;
		case 0x3:
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_CASL;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CASL_RegRegR0D;
			break;
		case 0x4: /* 2--4 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_MOV_RegDecStB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegDecStB;
			break;
		case 0x5: /* 2--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_MOV_RegDecStW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegDecStW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				}
			}
			break;
		case 0x6: /* 2--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_MOV_RegDecStD;
//			if(BTESH2_CheckCpuFmmuP(cpu))
//				op->Run=BTSH_Op_MOV_RegDecStD_FMMU;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegDecStD;
			break;
		case 0x7: /* 2--7 */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_DIV0S;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DIV0S_RegReg;
			cpu->ftopfl|=BTESH2_FTOPFL_SEEN_DIV0S;
			break;
		case 0x8: /* 2--8 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_TST;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_TST_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_TSTQ;
				op->Run=BTSH_Op_TSTQ_RegReg;
			}
			break;
		case 0x9: /* 2--9 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_AND_RegReg;
//			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_ANDQ;
				op->Run=BTSH_Op_ANDQ_RegReg;
			}
			break;
		case 0xA: /* 2--A */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_XOR_RegReg;
//			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_XORQ;
				op->Run=BTSH_Op_XORQ_RegReg;
			}
			break;
		case 0xB: /* 2--B */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_OR_RegReg;
//			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_ORQ;
				op->Run=BTSH_Op_ORQ_RegReg;
			}
			break;
		case 0xC: /* 2--C */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPSTR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPSTR_RegReg;
			break;
		case 0xD: /* 2--D */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_XTRCT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_XTRCT_RegReg;
			break;
		case 0xE: /* 2--E */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MULUW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MULUW_RegReg;
			break;
		case 0xF: /* 2--F */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MULSW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MULSW_RegReg;
			break;
		}
		break;

	case 0x3: /* 3--- */
		switch(opw&15)
		{
		case 0x0: /* 3--0 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPEQ_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQEQ;
				op->Run=BTSH_Op_CMPEQ_RegRegQ;
			}
			break;

		case 0x1: /* 3--1 */
			BTESH2_DecodeOpcode_2A(cpu, op, pc, opw, opw2);
			break;

		case 0x2: /* 3--2 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPHS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPHS_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQHS;
				op->Run=BTSH_Op_CMPHS_RegRegQ;
			}
			break;
		case 0x3: /* 3--3 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPGE;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPGE_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQGE;
				op->Run=BTSH_Op_CMPGE_RegRegQ;
			}
			break;
		case 0x4: /* 3--4 */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_DIV1;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DIV1_RegReg;
			cpu->ftopfl|=BTESH2_FTOPFL_SEEN_DIV1;
			break;
		case 0x5: /* 3--5 */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_DMULU;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DMULU_RegReg;
			break;
		case 0x6: /* 3--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPHI;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPHI_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQHI;
				op->Run=BTSH_Op_CMPHI_RegRegQ;
			}
			break;
		case 0x7: /* 3--7 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPGT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPGT_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQGT;
				op->Run=BTSH_Op_CMPGT_RegRegQ;
			}
			break;
		case 0x8: /* 3--8 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SUB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUB_RegReg;
//			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_SUBQ;
				op->Run=BTSH_Op_SUBQ_RegReg;
			}
			break;

		case 0x9: /* 3--9 */
			BTESH2_DecodeOpcode_2A(cpu, op, pc, opw, opw2);
			break;
			
		case 0xA: /* 3--A */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SUBC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUBC_RegReg;
			break;
		case 0xB: /* 3--B */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SUBV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUBV_RegReg;
			break;
		case 0xC: /* 3--C */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_ADD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_ADD_RegReg;
//			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_ADDQ;
				op->Run=BTSH_Op_ADDQ_RegReg;
			}
			break;
		case 0xD: /* 3--D */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_DMULS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DMULS_RegReg;
			break;
		case 0xE: /* 3--E */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_ADDC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_ADDC_RegReg;
			break;
		case 0xF: /* 3--F */
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
				break;
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_ADDV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_ADDV_RegReg;
			break;
		}
		break;

	case 0x4: /* 4--- */
		switch(opw&15)
		{
		case 0x0: /* 4--0 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-00 */
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					break;
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL_Reg;
				break;
			case 0x1: /* 4-10 */
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					break;
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_DT;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_DT_Reg;
				break;
			case 0x2: /* 4-20 */
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					break;
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHAL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHAL_Reg;
				break;
			}
			break;
		case 0x1: /* 4--1 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-01 */
				op->rn=(opw>>8)&15;
				op->Run=BTSH_Op_SHLR_Reg;
				op->nmid=BTESH2_NMID_SHLR;
				op->fmid=BTESH2_FMID_REGRN;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_Op_SHLR_RegQ;
				break;
			case 0x1: /* 4-11 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_CMPPZ;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_CMPPZ_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_CMPQPZ;
					op->Run=BTSH_Op_CMPPZ_RegQ;
				}
				break;
			case 0x2: /* 4-21 */
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					break;

				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHAR;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHAR_Reg;
				break;

			case 0x6: /* 4-61 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_STHF16;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_STHF16_Reg;
				break;
			case 0x7: /* 4-71 */
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_LDHF16;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_LDHF16_Reg;
				break;

			case 0xA: /* 4-A1 */
				if(!(cpu->csfl&BTESH2_CSFL_SRJQ))
					break;
				op->rm=(opw>>8)&15;
				op->rn=0;
				op->imm=(pc+4)&(~3);
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_OpJQ_MOV_RegStDispQ;
				break;
			case 0xB: /* 4-B1 */
				op->rm=(opw>>8)&15;
				op->rn=0;
				op->imm=(pc+4)&(~3);
				op->nmid=BTESH2_NMID_FMOVS;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_Op_FMOV_RegStDisp;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegStDispB;
				break;
			case 0xC: /* 4-C1 */
				op->rm=(opw>>8)&15;
				op->rn=0;
				op->imm=pc+4;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_Op_MOV_RegStDispB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegStDispB;
				break;
			case 0xD: /* 4-D1 */
				op->rm=(opw>>8)&15;
				op->rn=0;
				op->imm=pc+4;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_Op_MOV_RegStDispW;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_OpJQ_MOV_RegStDispW;
					if(cpu->csfl&BTESH2_CSFL_SRDQ)
					{
						op->imm=(pc+4)&(~3);
						op->nmid=BTESH2_NMID_MOVQ;
						op->Run=BTSH_OpJQ_MOV_RegStDispQ;
					}
				}
				break;
			case 0xE: /* 4-E1 */
				op->rm=(opw>>8)&15;
				op->rn=0;
				op->imm=(pc+4)&(~3);
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_Op_MOV_RegStDispD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegStDispD;
				break;
			}
			break;
		case 0x2: /* 4--2 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-02 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_MACH;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			case 0x1: /* 4-12 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			case 0x2: /* 4-22 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;

			case 0x5: /* 4-52 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			case 0x6: /* 4-62 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			}
			break;
		case 0x3: /* 4--3 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-03 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_SR;
				op->nmid=BTESH2_NMID_STCL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			case 0x1: /* 4-13 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_GBR;
				op->nmid=BTESH2_NMID_STCL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			case 0x2: /* 4-23 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_STCL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;

			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rn=(opw>>8)&15;
				op->rm=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_STCL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			}
			break;
		case 0x4: /* 4--4 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-04 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_ROTL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_ROTL_Reg;
				break;
			case 0x2: /* 4-24 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_ROTCL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_ROTCL_Reg;
				break;

			case 0x3: /* 4-34 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL4;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL4_RegQ;
				break;
			case 0x4: /* 4-44 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_EXTUL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_EXTUL_Reg;
				break;
			case 0x5: /* 4-54 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_EXTSL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_EXTSL_Reg;
				break;
			}
			break;
		case 0x5: /* 4--5 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-05 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_ROTR;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_ROTR_Reg;
				break;
			case 0x1: /* 4-15 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_CMPPL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_CMPPL_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_CMPQPL;
					op->Run=BTSH_Op_CMPPL_RegQ;
				}
				break;
			case 0x2: /* 4-25 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_ROTCR;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_ROTCR_Reg;
				break;
			case 0x3: /* 4-35 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLR4;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLR4_RegQ;
				break;

			case 0xA: /* 4-A5 */
				if(!(cpu->csfl&BTESH2_CSFL_SRJQ))
					break;
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->imm=(pc+4)&(~3);
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
				break;
			case 0xB: /* 4-B5 */
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->imm=(pc+4)&(~3);
				op->nmid=BTESH2_NMID_FMOVS;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_Op_FMOV_RegLdDisp;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegLdDispB;
				break;
			case 0xC: /* 4-C5 */
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->imm=pc+4;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_Op_MOV_RegLdDispB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegLdDispB;
				break;
			case 0xD: /* 4-D5 */
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->imm=pc+4;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_Op_MOV_RegLdDispW;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_OpJQ_MOV_RegLdDispW;
					if(cpu->csfl&BTESH2_CSFL_SRDQ)
					{
						op->imm=(pc+4)&(~3);
						op->nmid=BTESH2_NMID_MOVQ;
						op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
					}
				}
				break;
			case 0xE: /* 4-E5 */
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->imm=(pc+4)&(~3);
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_Op_MOV_RegLdDispD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegLdDispD;
				break;
			case 0xF: /* 4-F5 */
				op->rn=(opw>>8)&15;
				op->rm=0;
				op->imm=pc+4;
				op->nmid=BTESH2_NMID_LEAB;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_Op_LEA_RegLdDisp;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_LEA_RegLdDisp;
				break;
			}
			break;
		case 0x6: /* 4--6 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-06 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_MACH;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			case 0x1: /* 4-16 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			case 0x2: /* 4-26 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			case 0x3: /* 4-36 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SGR;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;

			case 0x5: /* 4-56 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			case 0x6: /* 4-66 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_FPSCR_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			}
			break;
		case 0x7: /* 4--7 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-07 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SR;
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegSrIncLdD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegSrIncLdQ;
				break;
			case 0x1: /* 4-17 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_GBR;
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			case 0x2: /* 4-27 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			case 0x3: /* 4-37 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SSR;
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			case 0x4: /* 4-47 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SPC;
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;

			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rm=(opw>>8)&15;
				op->rn=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			}
			break;
		case 0x8: /* 4--8 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-08 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL2;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL2_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_Op_SHLL2_RegQ;
				break;
			case 0x1: /* 4-18 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL8;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL8_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_Op_SHLL8_RegQ;
				break;
			case 0x2: /* 4-28 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL16;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL16_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_Op_SHLL16_RegQ;
				break;

			case 0x4: /* 4-48 */
				op->rn=BTESH2_REG_SP;
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_PUSH;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			case 0x5: /* 4-58 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_SP;
				op->nmid=BTESH2_NMID_POP;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_MOV_RegIncLdD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;

			case 0x6: /* 4-68 */
				op->rn=BTESH2_REG_SP;
				op->rm=16+((opw>>8)&15);
				op->nmid=BTESH2_NMID_PUSH;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_MOV_RegDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;
			case 0x7: /* 4-78 */
				op->rn=16+((opw>>8)&15);
				op->rm=BTESH2_REG_SP;
				op->nmid=BTESH2_NMID_POP;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_MOV_RegIncLdD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;
			}
			break;
		case 0x9: /* 4--9 */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-09 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLR2;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLR2_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_Op_SHLR2_RegQ;
				break;
			case 0x1: /* 4-19 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLR8;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLR8_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_Op_SHLR8_RegQ;
				break;
			case 0x2: /* 4-29 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLR16;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLR16_Reg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_Op_SHLR16_RegQ;
				break;
			}
			break;
		case 0xA: /* 4--A */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-0A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_MACH;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x1: /* 4-1A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x2: /* 4-2A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x3: /* 4-3A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SGR;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;

			case 0x5: /* 4-5A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x6: /* 4-5A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_FPSCR_RegReg;
//				if(cpu->csfl&BTESH2_CSFL_SRDQ)
//					op->Run=BTSH_OpJQ_MOV_FPSCR_RegReg;
				break;
			}
			break;
		case 0xB: /* 4--B */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-0B */
				op->rm=(opw>>8)&15;
				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
					BTESH2_OPFL_INVDLYSLOT;
				op->nmid=BTESH2_NMID_JSR;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_JSR_Reg;
				break;
			case 0x1: /* 4-1B */
				op->rn=(opw>>8)&15;
//				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT;
				op->nmid=BTESH2_NMID_TASB;
				op->fmid=BTESH2_FMID_STREG;
				op->Run=BTSH_Op_TASB_StReg;
				break;
			case 0x2: /* 4-2B */
				op->rm=(opw>>8)&15;
				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
					BTESH2_OPFL_INVDLYSLOT;
				op->nmid=BTESH2_NMID_JMP;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_JMP_Reg;
				break;

			case 0x8: /* 4-8B, 2A */
				op->rn=(opw>>8)&15;
				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGINCST;
				op->Run=BTSH_Op_MOV_RegIncStB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncStB;
				break;
			case 0x9: /* 4-9B, 2A */
				op->rn=(opw>>8)&15;
				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGINCST;
				op->Run=BTSH_Op_MOV_RegIncStW;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_OpJQ_MOV_RegIncStW;
					if(cpu->csfl&BTESH2_CSFL_SRDQ)
					{
						op->nmid=BTESH2_NMID_MOVQ;
						op->Run=BTSH_OpJQ_MOV_RegIncStQ;
					}
				}
				break;
			case 0xA: /* 4-AB, 2A */
				op->rn=(opw>>8)&15;
				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGINCST;
				op->Run=BTSH_Op_MOV_RegIncStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegIncStD;
				break;

			case 0xC: /* 4-CB, 2A */
				op->rn=(opw>>8)&15;
				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGDECLD;
				op->Run=BTSH_Op_MOV_RegDecLdB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecLdB;
				break;
			case 0xD: /* 4-DB, 2A */
				op->rn=(opw>>8)&15;
				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGDECLD;
				op->Run=BTSH_Op_MOV_RegDecLdW;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_OpJQ_MOV_RegDecLdW;
					if(cpu->csfl&BTESH2_CSFL_SRDQ)
					{
						op->nmid=BTESH2_NMID_MOVQ;
						op->Run=BTSH_OpJQ_MOV_RegDecLdQ;
					}
				}
				break;
			case 0xE: /* 4-EB, 2A */
				op->rn=(opw>>8)&15;
				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGDECLD;
				op->Run=BTSH_Op_MOV_RegDecLdD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegDecLdD;
				break;
			}
			break;
		case 0xC: /* 4--C */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SHAD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SHAD_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_Op_SHADL_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHADQ;
					op->Run=BTSH_Op_SHADQ_RegReg;
				}
			}
			break;
		case 0xD: /* 4--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SHLD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SHLD_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_Op_SHLDL_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHLDQ;
					op->Run=BTSH_Op_SHLDQ_RegReg;
				}
			}
			break;
		case 0xE: /* 4--E */
			switch((opw>>4)&15)
			{
			case 0x0: /* 4-0E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegRegSrB;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
//					op->Run=BTSH_OpJQ_MOV_RegRegSr;
				break;
			case 0x1: /* 4-1E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_GBR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x2: /* 4-2E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x3: /* 4-3E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SSR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x4: /* 4-4E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SPC;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			
			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rm=(opw>>8)&15;
				op->rn=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			}
			break;
		case 0xF: /* 4--F */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MACW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MACW_RegReg;
			break;
		}
		break;

	case 0x5: /* 5--- */
		op->rn=(opw>>8)&15;
		op->rm=(opw>>4)&15;
		op->imm=(opw&15)*4;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGLDDISP;
		op->Run=BTSH_Op_MOV_RegLdDispD;
		if(BTESH2_CheckCpuFmmuP(cpu))
			op->Run=BTSH_Op_MOV_RegLdDispD_FMMU;
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
			op->Run=BTSH_OpJQ_MOV_RegLdDispD;
		break;
	case 0x6: /* 6--- */
		switch(opw&15)
		{
		case 0x0: /* 6--0 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_MOV_RegMemB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegMemB;
			break;
		case 0x1: /* 6--1 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_MOV_RegMemW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegMemW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegMemQ;
				}
			}
			break;
		case 0x2: /* 6--2 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_MOV_RegMemD;
			if(BTESH2_CheckCpuFmmuP(cpu))
				op->Run=BTSH_Op_MOV_RegMemD_FMMU;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegMemD;
			break;
		case 0x3: /* 6--3 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MOV_RegReg;

			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
//				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegRegQ;
				BTSH_FixupJQ_MOV_RegReg(cpu, op);
				break;
			}else if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->nmid=BTESH2_NMID_MOV;
				op->Run=BTSH_OpJQ_MOV_RegRegQ;
				BTSH_FixupJQ_MOV_RegReg(cpu, op);
				break;
			}

			if(isds)
			{
				BTSH_Fixup_MOV_RegReg(cpu, op);
				break;
			}

#if 0
			if(((opw2>>8)&15)==op->rn)
			{
				if((opw2&0xF000)==0x3000)
				{
					if((opw2&0xF00F)==0x300C)
					{	op->nmid=BTESH2_NMID_ADD;
						op->Run=BTSH_Op_ADD_RegRegReg;	}
					else if((opw2&0xF00F)==0x3008)
					{	op->nmid=BTESH2_NMID_SUB;
						op->Run=BTSH_Op_SUB_RegRegReg;	}
				}
				else if((opw2&0xF000)==0x2000)
				{
					if((opw2&0xF00F)==0x2009)
					{	op->nmid=BTESH2_NMID_AND;
						op->Run=BTSH_Op_AND_RegRegReg;	}
					else if((opw2&0xF00F)==0x200B)
					{	op->nmid=BTESH2_NMID_OR;
						op->Run=BTSH_Op_OR_RegRegReg;	}
					else if((opw2&0xF00F)==0x200A)
					{	op->nmid=BTESH2_NMID_XOR;
						op->Run=BTSH_Op_XOR_RegRegReg;	}
				}

				if(op->nmid!=BTESH2_NMID_MOV)
				{
					op->rn=(opw>>4)&15;
					op->rm=(opw2>>4)&15;
					op->ro=(opw2>>8)&15;
					if(op->rm==op->ro)
						op->rm=op->rn;
					op->fmid=BTESH2_FMID_REGREGREG;
					op->fl=BTESH2_OPFL_EXTRAWORD|
						BTESH2_OPFL_INVDLYSLOT;
					break;
				}
			}
#endif

			BTSH_Fixup_MOV_RegReg(cpu, op);
			break;
		case 0x4: /* 6--4 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_MOV_RegIncLdB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegIncLdB;
			break;
		case 0x5: /* 6--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_MOV_RegIncLdW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegIncLdW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				}
			}
			break;
		case 0x6: /* 6--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_MOV_RegIncLdD;
//			if(BTESH2_CheckCpuFmmuP(cpu))
//				op->Run=BTSH_Op_MOV_RegIncLdD_FMMU;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegIncLdD;
			break;
		case 0x7: /* 6--7 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_NOT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_NOT_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_Op_NOTQ_RegReg;
			break;
		case 0x8: /* 6--8 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SWAPB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SWAP_RegRegB;
			break;
		case 0x9: /* 6--9 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SWAPW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SWAP_RegRegW;
			break;
		case 0xA: /* 6--A */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_NEGC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_NEGC_RegReg;
			break;
		case 0xB: /* 6--B */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_NEG;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_NEG_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_Op_NEGQ_RegReg;
			break;
		case 0xC: /* 6--C */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTUB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTUB_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_EXTUB_RegReg;
			break;
		case 0xD: /* 6--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTUW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTUW_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_EXTUW_RegReg;
			break;
		case 0xE: /* 6--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTSB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTSB_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_EXTSB_RegReg;
			break;
		case 0xF: /* 6--F */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTSW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTSW_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_EXTSW_RegReg;
			break;
		}
		break;
	case 0x7: /* 7--- */
		op->rn=(opw>>8)&15;
		op->imm=(u64)((sbyte)opw);
		op->nmid=BTESH2_NMID_ADD;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_ADD_RegImm;
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
		{
			op->Run=BTSH_Op_ADDQ_RegImm;
			break;
		}

		BTSH_Fixup_ADD_RegImm(cpu, op);
		break;
	case 0x8: /* 8--- */
		switch((opw>>8)&15)
		{
		case 0x0: /* 80-- */
			op->rm=0;
			op->rn=(opw>>4)&15;
			op->imm=opw&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_R0StDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_R0StDispB;
			break;
		case 0x1: /* 81-- */
			op->rm=0;
			op->rn=(opw>>4)&15;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_R0StDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_R0StDispW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->imm=(opw&15)*8;
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_R0StDispQ;
				}
			}
			break;

		case 0x2: /* 82-- */
#if 1
			i=(opw<<24)>>24;
			op->imm=pc+2*i+4;
			op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BRAN;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BRA_Abs;
			break;
#endif
			break;

		case 0x3: /* 83-- */
#if 1
			if(opw&0x0080)
			{
				op->rn=((opw>>4)&7)|8;
				if(op->rn==15)op->rn=0;
				op->rm=15;
				op->imm=((opw&15)+16)*4;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_Op_MOV_RegLdDispD;
				if(BTESH2_CheckCpuFmmuP(cpu))
					op->Run=BTSH_Op_MOV_RegLdDispD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegLdDispD;
			}else
			{
				op->rm=((opw>>4)&7)|8;
				if(op->rm==15)op->rm=0;
				op->rn=15;
				op->imm=((opw&15)+16)*4;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_Op_MOV_RegStDispD;
//				if(BTESH2_CheckCpuFmmuP(cpu))
//					op->Run=BTSH_Op_MOV_RegStDispD_FMMU;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegStDispD;
			}
#endif
			break;
		case 0x4: /* 84-- */
			op->rm=(opw>>4)&15;
			op->rn=0;
			op->imm=opw&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_R0LdDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_R0LdDispB;
			break;
		case 0x5: /* 85-- */
			op->rm=(opw>>4)&15;
			op->rn=0;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_R0LdDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_R0LdDispW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->imm=(opw&15)*8;
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_R0LdDispQ;
				}
			}
			break;
		case 0x6: /* 86-- */
			if(opw&0x0080)
			{
				op->rm=15;
				op->rn=8|((opw>>4)&7);
				op->imm=(opw&15)*4;
				op->nmid=BTESH2_NMID_FMOVS;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_Op_FMOV_RegLdDisp;
			}else
			{
				op->rn=15;
				op->rm=8|((opw>>4)&7);
				op->imm=(opw&15)*4;
				op->nmid=BTESH2_NMID_FMOVS;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_Op_FMOV_RegStDisp;
			}
			break;
		case 0x7: /* 87-- */
#if 0
			i=(opw<<24)>>24;
			op->imm=pc+2*i+4;
			op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BRAN;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BRA_Abs;
			break;
#endif

#if 0
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				if(opw&0x0080)
				{
					op->rn=((opw>>4)&7)|8;
					if(op->rn==15)op->rn=0;
					op->rm=15;
					op->imm=(opw&15)*8;
					op->nmid=BTESH2_NMID_MOVQ;
					op->fmid=BTESH2_FMID_REGLDDISP;
					op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
				}else
				{
					op->rm=((opw>>4)&7)|8;
					if(op->rm==15)op->rm=0;
					op->rn=15;
					op->imm=(opw&15)*8;
					op->nmid=BTESH2_NMID_MOVQ;
					op->fmid=BTESH2_FMID_REGSTDISP;
					op->Run=BTSH_OpJQ_MOV_RegStDispQ;
				}
			}else
			{
				if(opw&0x0080)
				{
					op->rm=15;
					op->rn=8|((opw>>4)&7);
					op->imm=((opw&15)+16)*4;
					op->nmid=BTESH2_NMID_FMOVS;
					op->fmid=BTESH2_FMID_REGLDDISP;
					op->Run=BTSH_Op_FMOV_RegLdDisp;
				}else
				{
					op->rn=15;
					op->rm=8|((opw>>4)&7);
					op->imm=((opw&15)+16)*4;
					op->nmid=BTESH2_NMID_FMOVS;
					op->fmid=BTESH2_FMID_REGSTDISP;
					op->Run=BTSH_Op_FMOV_RegStDisp;
				}
			}
			break;
#endif
			break;

		case 0x8: /* 88-- */
			op->rn=0;
			op->imm=(sbyte)opw;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPEQ_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQEQ;
				op->Run=BTSH_Op_CMPEQ_RegImmQ;
			}
			break;
		case 0x9: /* 89-- */
//			i=(opw<<24)>>24;
			i=((sbyte)opw);
			op->imm=pc+2*i+4;
			op->fl=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BT;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BT_Abs;
			break;
		case 0xA: /* 8A-- */
#if 1
//			i=(opw<<16)|(opw2&0xFFFF);

			op->rn=0;
//			op->imm=(((sbyte)opw)<<16)|opw2;
			op->imm=(((sbyte)opw)<<16)|(opw2&0xFFFF);
			op->fl=BTESH2_OPFL_EXTRAWORD|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_MOV;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_MOV_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegImmQ;
				BTSH_FixupJQ_MOV_RegImm(cpu, op);
			}
#endif
			break;
		case 0xB: /* 8B-- */
//			i=(opw<<24)>>24;
			i=((sbyte)opw);
			op->imm=pc+2*i+4;
			op->fl=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BF;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BF_Abs;
			break;
		case 0xC: /* 8C-- */
			BTESH2_DecodeOpcode_BJX1_8C(cpu, op, pc, opw, opw2);
			break;
		case 0xD: /* 8D-- */
//			i=(opw<<24)>>24;
			i=((sbyte)opw);
			op->imm=pc+2*i+4;
			op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BTS;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BTS_Abs;
			break;
		case 0xE: /* 8E-- */
			BTESH2_DecodeOpcode_BJX1_8E(cpu, op, pc, opw, opw2);
			break;
		case 0xF: /* 8F-- */
//			i=(opw<<24)>>24;
			i=((sbyte)opw);
			op->imm=pc+2*i+4;
			op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BFS;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BFS_Abs;
			break;
		}
		break;
	case 0x9: /* 9ndd, MOV.W @(disp,PC),Rn */
		i=((byte)opw);
		op->rn=(opw>>8)&15;
		op->ro=i;
//		op->imm=pc+2*i+2;
		op->imm=pc+2*i+4;
		op->fl=BTESH2_OPFL_PCADLYSLOTW;
		op->nmid=BTESH2_NMID_MOVW;
		op->fmid=BTESH2_FMID_REGLDABS;
		op->Run=BTSH_Op_MOV_RegLdAbsW;
		if(cpu->csfl&BTESH2_CSFL_SRDQ)
		{
			op->imm=((pc+4)&(~3))+(4*i);
			op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegLdAbsQ;
		}else if(cpu->csfl&BTESH2_CSFL_SRJQ)
		{
			op->Run=BTSH_OpJQ_MOV_RegLdAbsWQ;
		}
		break;
	case 0xA: /* A---, BRA label */
		i=(opw<<20)>>20;
		op->imm=pc+2*i+4;
		op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
			BTESH2_OPFL_INVDLYSLOT;
		op->nmid=BTESH2_NMID_BRA;
		op->fmid=BTESH2_FMID_ABS;
		op->Run=BTSH_Op_BRA_Abs;
		break;
	case 0xB: /* B---, BSR label */
		i=(opw<<20)>>20;
		op->imm=pc+2*i+4;
		op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
			BTESH2_OPFL_INVDLYSLOT;
		op->nmid=BTESH2_NMID_BSR;
		op->fmid=BTESH2_FMID_ABS;
		op->Run=BTSH_Op_BSR_Abs;
		break;
	case 0xC: /* C--- */
		switch((opw>>8)&15)
		{
		case 0x0: /* C0-- */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->rm=(opw>>4)&15;
				op->rn=15;
				op->imm=(opw&15)*8;
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_OpJQ_MOV_RegStDispQ;
				break;
			}

//			if(cpu->csfl&BTESH2_CSFL_SRJQ)
//				break;

			op->rn=BTESH2_REG_GBR;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStDispB;
			break;
		case 0x1: /* C1-- */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->rn=8|((opw>>4)&7); if(op->rn==15)op->rn=0;
				op->rm=8|((opw>>0)&7); if(op->rm==15)op->rm=0;
				op->ro=0;
				switch(opw&0x88)
				{
				case 0x00:
					op->nmid=BTESH2_NMID_MOVQ;
					op->fmid=BTESH2_FMID_REGST;
					op->Run=BTSH_OpJQ_MOV_RegStQ;
					break;
				case 0x08:
					op->nmid=BTESH2_NMID_MOVQ;
					op->fmid=BTESH2_FMID_REGLD;
					op->Run=BTSH_OpJQ_MOV_RegMemQ;
					break;
				case 0x80:
					op->nmid=BTESH2_NMID_MOVQ;
					op->fmid=BTESH2_FMID_REGSTR0N;
					op->Run=BTSH_OpJQ_MOV_RegStR0nQ;
					break;
				case 0x88:
					op->nmid=BTESH2_NMID_MOVQ;
					op->fmid=BTESH2_FMID_REGLDR0M;
					op->Run=BTSH_OpJQ_MOV_RegLdR0mQ;
					break;
				}
				break;
			}

			op->rn=BTESH2_REG_GBR;
			op->rm=0;
			op->imm=(opw&255)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegStDispW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegStDispQ;
				}
			}
			break;
		case 0x2: /* C2-- */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				break;

			op->rn=BTESH2_REG_GBR;
			op->rm=0;
			op->imm=(opw&255)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStDispD;
			break;
		case 0x3: /* C3--, TRAPA #imm */
			op->rn=BTESH2_REG_GBR;
			op->rm=0;
			op->imm=opw&255;
			op->ro=opw&255;
//			op->imm=pc+2;
			op->fl=BTESH2_OPFL_CTRLF;
			op->nmid=BTESH2_NMID_TRAPA;
			op->fmid=BTESH2_FMID_IMM;
			op->Run=BTSH_Op_TRAPA_Imm;
			break;
		case 0x4: /* C4--, MOV.B @(disp,GBR),R0 */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->rn=(opw>>4)&15;
				op->rm=15;
				op->imm=(opw&15)*8;
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
				break;
			}

//			if(cpu->csfl&BTESH2_CSFL_SRJQ)
//				break;

			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdDispB;
			break;
		case 0x5: /* C5--, MOV.W @(disp,GBR),R0 */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->rn=8|((opw>>4)&7); if(op->rn==15)op->rn=0;
				op->rm=8|((opw>>0)&7); if(op->rm==15)op->rm=0;
				op->fmid=BTESH2_FMID_REGREG;
				switch(opw&0x88)
				{
				case 0x00:
					op->nmid=BTESH2_NMID_CMPQEQ;
					op->Run=BTSH_Op_CMPEQ_RegRegQ;
					break;
				case 0x08:
					op->nmid=BTESH2_NMID_TSTQ;
					op->Run=BTSH_Op_TSTQ_RegReg;
					break;
				case 0x80:
					op->nmid=BTESH2_NMID_CMPQHI;
					op->Run=BTSH_Op_CMPHI_RegRegQ;
					break;
				case 0x88:
					op->nmid=BTESH2_NMID_CMPQGT;
					op->Run=BTSH_Op_CMPGT_RegRegQ;
					break;
				}
				break;
			}

			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=(opw&255)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOV_RegLdDispW;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
				}
			}
			break;
		case 0x6: /* C6--, MOV.L @(disp,GBR),R0 */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				break;

			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=(opw&255)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdDispD;
			break;

		case 0x7: /* C7--, MOVA @(disp,PC),R0 */
			op->rn=0;
			op->rm=BTESH2_REG_PC;
//			op->imm=(opw&255)*4;
//			op->Run=BTSH_Op_MOV_RegLdDispD;
			i=((byte)opw);
			op->ro=i;
			op->imm=((pc+4)&(~3))+i*4;
			op->fl=BTESH2_OPFL_PCADLYSLOTD;
//			op->fl=BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_MOVA;
			op->fmid=BTESH2_FMID_REGLDABS;
//			op->Run=BTSH_Op_MOV_RegLdAbsD;
			op->Run=BTSH_Op_MOVA_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOVA_RegImm;
			break;
		case 0x8: /* C8-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_TST;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_TST_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_TSTQ;
				op->Run=BTSH_Op_TSTQ_RegImm;
			}
			break;
		case 0x9: /* C9-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_AND_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
//				op->nmid=BTESH2_NMID_ANDQ;
				op->Run=BTSH_Op_ANDQ_RegImm;
			}
			break;
		case 0xA: /* CA-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_XOR_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
//				op->nmid=BTESH2_NMID_XORQ;
				op->Run=BTSH_Op_XORQ_RegImm;
			}
			break;
		case 0xB: /* CB-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_OR_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
//				op->nmid=BTESH2_NMID_ORQ;
				op->Run=BTSH_Op_ORQ_RegImm;
			}
			break;
		case 0xC: /* CC-- */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				BTESH2_DecodeOpcode_BJX1_CC(cpu, op, pc, opw, opw2);
				break;
			}

			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_TSTB;
			op->fmid=BTESH2_FMID_IMMSTRMN;
			op->Run=BTSH_Op_TSTB_MemImm;
			break;
		case 0xD: /* CD-- */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				BTESH2_DecodeOpcode_BJX1_CD(cpu, op, pc, opw, opw2);
				break;
			}

			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_ANDB;
			op->fmid=BTESH2_FMID_IMMSTRMN;
			op->Run=BTSH_Op_ANDB_MemImm;
			break;
		case 0xE: /* CE-- */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				BTESH2_DecodeOpcode_BJX1_CE(cpu, op, pc, opw, opw2);
				break;
			}

			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_XORB;
			op->fmid=BTESH2_FMID_IMMSTRMN;
			op->Run=BTSH_Op_XORB_MemImm;
			break;
		case 0xF: /* CF-- */
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				BTESH2_DecodeOpcode_BJX1_CF(cpu, op, pc, opw, opw2);
				break;
			}

			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_ORB;
			op->fmid=BTESH2_FMID_IMMSTRMN;
			op->Run=BTSH_Op_ORB_MemImm;
			break;
		}
		break;
	case 0xD: /* D--- */
		i=((byte)opw);
		op->rn=(opw>>8)&15;
//		op->imm=(pc&(~3))+(4*i);
//		op->imm=((pc+2)&(~3))+(4*i);
		op->ro=i;
		op->imm=((pc+4)&(~3))+(4*i);
		op->fl=BTESH2_OPFL_PCADLYSLOTD;
//		op->fl=BTESH2_OPFL_INVDLYSLOT;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGLDABS;
		op->Run=BTSH_Op_MOV_RegLdAbsD;
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
		{
			op->Run=BTSH_OpJQ_MOV_RegLdAbsD;
//			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			if(1)
			{
				op->nmid=BTESH2_NMID_MOVL;
				op->Run=BTSH_OpJQ_MOV_RegLdAbsDQ;
			}
		}
		break;
	case 0xE: /* E--- */
		op->rn=(opw>>8)&15;
		op->imm=(u64)((sbyte)(opw));
		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;

//		if(cpu->csfl&BTESH2_CSFL_SRDQ)
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
		{
//			op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegImmQ;
			BTSH_FixupJQ_MOV_RegImm(cpu, op);
			break;
		}
		
		if(isds)
			break;

#if 0
		if(((opw2&0xF00F)==0x400C) && (((opw2>>4)&15)==op->rn))
		{
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->nmid=BTESH2_NMID_SHAD;
			op->fmid=BTESH2_FMID_REGIMM;
			op->fl=BTESH2_OPFL_EXTRAWORD|
				BTESH2_OPFL_INVDLYSLOT;
			op->Run=BTSH_Op_SHAD_RegImm;
			if(((s32)op->imm)>=0)
				op->Run=BTSH_Op_SHAD_RegImmP;
			break;
		}

		if(((opw2&0xF00F)==0x400D) && (((opw2>>4)&15)==op->rn))
		{
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->nmid=BTESH2_NMID_SHLD;
			op->fmid=BTESH2_FMID_REGIMM;
			op->fl=BTESH2_OPFL_EXTRAWORD|
				BTESH2_OPFL_INVDLYSLOT;
			op->Run=BTSH_Op_SHLD_RegImm;
			if(((s32)op->imm)>=0)
				op->Run=BTSH_Op_SHLD_RegImmP;
			break;
		}

		if(((opw2&0xF00F)==0x2002) && (((opw2>>4)&15)==op->rn))
		{
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_IMMSTRN;

			op->fl=BTESH2_OPFL_EXTRAWORD|
				BTESH2_OPFL_INVDLYSLOT;
			op->Run=BTSH_Op_MOV_StRegImmD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_StRegImmD;
			break;
		}

		if(((opw2&0xF00F)==0x2006) && (((opw2>>4)&15)==op->rn))
		{
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_IMMDECSTRN;

			op->fl=BTESH2_OPFL_EXTRAWORD|
				BTESH2_OPFL_INVDLYSLOT;
			op->Run=BTSH_Op_MOV_DecStRegImmD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegDecStImmD;
			break;
		}
#endif

		break;
	case 0xF: /* F--- */
		switch(opw&15)
		{
		case 0x0: /* F--0 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FADD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FADD_RR;
			break;
		case 0x1: /* F--1 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FSUB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FSUB_RR;
			break;
		case 0x2: /* F--2 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMUL;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FMUL_RR;
			break;
		case 0x3: /* F--3 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FDIV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FDIV_RR;
			break;
		case 0x4: /* F--4 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FCMPEQ;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FCMPEQ_RR;
			break;
		case 0x5: /* F--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FCMPGT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FCMPGT_RR;
			break;
		case 0x6: /* F--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_Op_FMOV_RegLdR0m;
			break;
		case 0x7: /* F--7 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_Op_FMOV_RegStR0n;
			break;
		case 0x8: /* F--8 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_FMOV_RegLd;
			break;
		case 0x9: /* F--9 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_FMOV_RegIncLd;
			break;
		case 0xA: /* F--A */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_FMOV_RegSt;
			break;
		case 0xB: /* F--B */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_FMOV_RegDecSt;
			break;
		case 0xC: /* F--C */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMOV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FMOV_RR;
			break;
		case 0xD: /* F--D */
			switch((opw>>4)&15)
			{
			case 0x0: /* F-0D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FSTS;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_FSTS_R;
				break;
			case 0x1: /* F-1D */
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FLDS;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_FLDS_R;
				break;
			case 0x2: /* F-2D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FLOAT;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_FLOAT_R;
				break;
			case 0x3: /* F-3D */
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FTRC;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_FTRC_R;
				break;
			case 0x4: /* F-4D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FNEG;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_FNEG_R;
				break;
			case 0x5: /* F-5D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FABS;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_FABS_R;
				break;
			case 0x6: /* F-6D */
				op->rn=(opw>>8)&15;
//				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_FSQRT;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_FSQRT_R;
				break;
			case 0x7:
				break;
			case 0x8: /* F-8D */
				op->rn=(opw>>8)&15;
//				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_FLDI0;
				op->fmid=BTESH2_FMID_FREGRN;
				op->Run=BTSH_Op_FLDI0_R;
				break;
			case 0x9: /* F-9D */
				op->rn=(opw>>8)&15;
//				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_FLDI1;
				op->fmid=BTESH2_FMID_FREGRN;
				op->Run=BTSH_Op_FLDI1_R;
				break;
			case 0xA: /* F-AD */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FCNVSD;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_FCNVSD_R;
				break;
			case 0xB: /* F-BD */
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FCNVDS;
				op->fmid=BTESH2_FMID_REGRM;
				op->Run=BTSH_Op_FCNVDS_R;
				break;

			case 0xD: /* F-DD */
				op->imm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_PSETMD4;
				op->fmid=BTESH2_FMID_IMM;
				op->Run=BTSH_Op_PSETMD4_Imm;
				break;

			case 0xE: /* F-ED */
				op->rn=((opw>>10)&3)<<2;
				op->rm=((opw>> 8)&3)<<2;
				op->nmid=BTESH2_NMID_FIPR;
				op->fmid=BTESH2_FMID_FREGREG;
				op->Run=BTSH_Op_FIPR_RR;
				break;
			case 0xF: /* F-FD */
				switch((opw>>8)&15)
				{
				case 0x3:
					op->nmid=BTESH2_NMID_FSCHG;
					op->fmid=BTESH2_FMID_NONE;
					op->Run=BTSH_Op_FSCHG_Z;
					break;
				
				case 0x1:	case 0x5:
				case 0x9:	case 0xD:
					op->rm=16;
					op->rn=((opw>>10)&3)<<2;
					op->nmid=BTESH2_NMID_FTRV;
					op->fmid=BTESH2_FMID_FREGRN;
					op->Run=BTSH_Op_FTRV_R;
					break;
				}
				break;
			}
			break;

		case 0xE: /* F--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_FMAC;
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FMAC_RR;
			break;
		}
		break;
	}
	return(0);
}

