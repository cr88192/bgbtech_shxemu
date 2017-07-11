int BTESH2_DecodeOpcode(BTESH2_CpuState *cpu,
	BTESH2_Opcode *op, btesh2_vaddr pc)
{
	static const byte ldow2[16]={1,0,0,1, 0,0,1,0, 1,0,0,0, 0,0,1,1};
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MULL_RegReg;
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_DIV0S;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DIV0S_RegReg;
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
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_ANDQ;
				op->Run=BTSH_Op_ANDQ_RegReg;
			}
			break;
		case 0xA: /* 2--A */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_XOR_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_XORQ;
				op->Run=BTSH_Op_XORQ_RegReg;
			}
			break;
		case 0xB: /* 2--B */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_OR_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_ORQ;
				op->Run=BTSH_Op_ORQ_RegReg;
			}
			break;
		case 0xC: /* 2--C */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPSTR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPSTR_RegReg;
			break;
		case 0xD: /* 2--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_XTRCT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_XTRCT_RegReg;
			break;
		case 0xE: /* 2--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MULUW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MULUW_RegReg;
			break;
		case 0xF: /* 2--F */
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
			break;
		case 0x3: /* 3--3 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPGE;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPGE_RegReg;
			break;
		case 0x4: /* 3--4 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_DIV1;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DIV1_RegReg;
			break;
		case 0x5: /* 3--5 */
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
			break;
		case 0x7: /* 3--7 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_CMPGT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPGT_RegReg;
			break;
		case 0x8: /* 3--8 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SUB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUB_RegReg;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_SUBQ;
				op->Run=BTSH_Op_SUBQ_RegReg;
			}
			break;

		case 0x9: /* 3--9 */
			BTESH2_DecodeOpcode_2A(cpu, op, pc, opw, opw2);
			break;
			
		case 0xA: /* 3--A */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SUBC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUBC_RegReg;
			break;
		case 0xB: /* 3--B */
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
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_ADDQ;
				op->Run=BTSH_Op_ADDQ_RegReg;
			}
			break;
		case 0xD: /* 3--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_DMULS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DMULS_RegReg;
			break;
		case 0xE: /* 3--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_ADDC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_ADDC_RegReg;
			break;
		case 0xF: /* 3--F */
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
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL_Reg;
				break;
			case 0x1: /* 4-10 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_DT;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_DT_Reg;
				break;
			case 0x2: /* 4-20 */
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
				break;
			case 0x1: /* 4-11 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_CMPPZ;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_CMPPZ_Reg;
				break;
			case 0x2: /* 4-11 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHAR;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHAR_Reg;
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
				break;
			case 0x2: /* 4-25 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_ROTCR;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_ROTCR_Reg;
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
				break;
			case 0x1: /* 4-18 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL8;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL8_Reg;
				break;
			case 0x2: /* 4-28 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLL16;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLL16_Reg;
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
				break;
			case 0x1: /* 4-19 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLR8;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLR8_Reg;
				break;
			case 0x2: /* 4-29 */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_SHLR16;
				op->fmid=BTESH2_FMID_REGRN;
				op->Run=BTSH_Op_SHLR16_Reg;
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x1: /* 4-1A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x2: /* 4-2A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x3: /* 4-3A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SGR;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;

			case 0x5: /* 4-5A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
			break;
		case 0xD: /* 4--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SHLD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SHLD_RegReg;
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
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x2: /* 4-2E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x3: /* 4-3E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SSR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			case 0x4: /* 4-4E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_SPC;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
					op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			
			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rm=(opw>>8)&15;
				op->rn=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
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
				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegRegQ;
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
			break;
		case 0xC: /* 6--C */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTUB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTUB_RegReg;
			break;
		case 0xD: /* 6--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTUW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTUW_RegReg;
			break;
		case 0xE: /* 6--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTSB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTSB_RegReg;
			break;
		case 0xF: /* 6--F */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_EXTSW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTSW_RegReg;
			break;
		}
		break;
	case 0x7: /* 7--- */
		op->rn=(opw>>8)&15;
		op->imm=(u32)((sbyte)opw);
		op->nmid=BTESH2_NMID_ADD;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_ADD_RegImm;
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
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_R0StDispQ;
				}
			}
			break;
		case 0x2: /* 82-- */
			break;
		case 0x3: /* 83-- */
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
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_R0LdDispQ;
				}
			}
			break;
		case 0x6: /* 86-- */
			break;
		case 0x7: /* 87-- */
			break;
		case 0x8: /* 88-- */
			op->rn=0;
			op->imm=(sbyte)opw;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPEQ_RegImm;
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
#if 0
			op->imm=((byte)opw);
			op->rn=0;
			op->ro=8;
			op->nmid=BTESH2_NMID_MOV;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_MOV_RegShlImm;
			break;
#endif
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
				op->nmid=BTESH2_NMID_ANDQ;
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
				op->nmid=BTESH2_NMID_XORQ;
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
				op->nmid=BTESH2_NMID_ORQ;
				op->Run=BTSH_Op_ORQ_RegImm;
			}
			break;
		case 0xC: /* CC-- */
			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_TSTB;
			op->fmid=BTESH2_FMID_IMMSTRMN;
			op->Run=BTSH_Op_TSTB_MemImm;
			break;
		case 0xD: /* CD-- */
			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_ANDB;
			op->fmid=BTESH2_FMID_IMMSTRMN;
			op->Run=BTSH_Op_ANDB_MemImm;
			break;
		case 0xE: /* CE-- */
			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_XORB;
			op->fmid=BTESH2_FMID_IMMSTRMN;
			op->Run=BTSH_Op_XORB_MemImm;
			break;
		case 0xF: /* CF-- */
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
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_MOVL;
				op->Run=BTSH_OpJQ_MOV_RegLdAbsDQ;
			}
		}
		break;
	case 0xE: /* E--- */
		op->rn=(opw>>8)&15;
		op->imm=(u32)((sbyte)(opw));
		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;

		if(cpu->csfl&BTESH2_CSFL_SRDQ)
		{
			op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegImmQ;
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

			case 0xE:
				op->rn=((opw>>10)&3)<<2;
				op->rm=((opw>> 8)&3)<<2;
				op->nmid=BTESH2_NMID_FIPR;
				op->fmid=BTESH2_FMID_FREGREG;
				op->Run=BTSH_Op_FIPR_RR;
				break;
			case 0xF:
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

BTESH2_Trace *BTESH2_Trace_Run1(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;

	if(BTESH2_Trace_TraceUpdateJTrig(cpu, tr)>0)
		return(cpu->trnext);

	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	cpu->trnext=tr->trnext;
	cpu->trjmpnext=tr->trjmpnext;
	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
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
			op->fl=BTESH2_OPFL_CTRLF;
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
				op->Run=BTSH_OpJQ_MOV_RegImmQ;
		}
#endif

#if 1
		if((((op->opw>>12)&15)==0x9) &&
			(((pc-2)>>12)==((op->imm+1)>>12)))
		{
			BTESH2_MarkAddrTrapSmc(cpu, op->imm+1);
			if((op->imm+2)>tr->maxpc)
				tr->maxpc=op->imm+2;
			
			op->imm=(s16)BTESH2_GetAddrWord(cpu, op->imm);
			op->fmid=BTESH2_FMID_REGVIMM;
			op->Run=BTSH_Op_MOV_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegImmQ;
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
				(op->fmid==BTESH2_FMID_ABS))
			{
				jmppc=op->imm;
			}

			tr->ops[n++]=op;
			break;
		}else
		{
			tr->ops[n++]=op;
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
