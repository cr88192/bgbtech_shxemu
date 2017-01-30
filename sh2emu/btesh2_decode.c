int BTESH2_DecodeOpcode(BTESH2_CpuState *cpu, BTESH2_Opcode *op, u32 pc)
{
	u16 opw;
	int i, j, k;
	
	opw=BTESH2_GetAddrWord(cpu, pc);
	op->opw=opw;
	op->pc=pc;
	switch(opw>>12)
	{
	case 0x0: /* 0--- */
		switch(opw&15)
		{
		case 0x0: /* 0--0 */
			break;
		case 0x1: /* 0--1 */
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
				break;
			case 0x2: /* 0-22 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;

			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rn=(opw>>8)&15;
				op->rm=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_STC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
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

			case 0xC: /* 0-C3 */
				op->rn=(opw>>8)&15;
				op->rm=0;
//				op->fl=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT;
				op->nmid=BTESH2_NMID_MOVCAL;
				op->fmid=BTESH2_FMID_REGST;
				op->Run=BTSH_Op_MOV_RegStD;
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
			break;
		case 0x5: /* 0--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_Op_MOV_RegStR0nW;
			break;
		case 0x6: /* 0--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_Op_MOV_RegStR0nD;
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
				op->nmid=BTESH2_NMID_CLRS;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_CLRT_Z;
				break;
			case 0x5: /* 0-58 */
				op->nmid=BTESH2_NMID_SETS;
				op->fmid=BTESH2_FMID_NONE;
				op->Run=BTSH_Op_SETT_Z;
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
				break;
			case 0x1: /* 0-1A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;
			case 0x2: /* 0-2A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;

			case 0x5: /* 0-5A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;
			case 0x6: /* 0-6A */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_STS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
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
			break;
		case 0xD: /* 0--D */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_Op_MOV_RegLdR0mW;
			break;
		case 0xE: /* 0--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->ro=0;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_Op_MOV_RegLdR0mD;
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
			break;
		case 0x1: /* 2--1 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_MOV_RegStW;
			break;
		case 0x2: /* 2--2 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_MOV_RegStD;
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
			break;
		case 0x5: /* 2--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_MOV_RegDecStW;
			break;
		case 0x6: /* 2--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_MOV_RegDecStD;
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
			break;
		case 0x9: /* 2--9 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_AND_RegReg;
			break;
		case 0xA: /* 2--A */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_XOR_RegReg;
			break;
		case 0xB: /* 2--B */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_OR_RegReg;
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
			break;

		case 0x9: /* 3--9 */
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
				break;
			case 0x1: /* 4-12 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				break;
			case 0x2: /* 4-22 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				break;

			case 0x5: /* 4-52 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				break;
			case 0x6: /* 4-62 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_STSL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
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
				break;
			case 0x1: /* 4-13 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_GBR;
				op->nmid=BTESH2_NMID_STCL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				break;
			case 0x2: /* 4-23 */
				op->rn=(opw>>8)&15;
				op->rm=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_STCL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
				break;

			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rn=(opw>>8)&15;
				op->rm=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_STCL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_Op_MOV_RegDecStD;
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
				break;
			case 0x1: /* 4-16 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
				break;
			case 0x2: /* 4-26 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
				break;

			case 0x5: /* 4-56 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
				break;
			case 0x6: /* 4-66 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_LDSL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_FPSCR_RegIncLdD;
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
				break;
			case 0x1: /* 4-17 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_GBR;
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
				break;
			case 0x2: /* 4-27 */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
				break;

			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rm=(opw>>8)&15;
				op->rn=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_LDCL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_Op_MOV_RegIncLdD;
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
				break;
			case 0x1: /* 4-1A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_MACL;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;
			case 0x2: /* 4-2A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_PR;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;

			case 0x5: /* 4-5A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPUL;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;
			case 0x6: /* 4-5A */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_FPSCR;
				op->nmid=BTESH2_NMID_LDS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_FPSCR_RegReg;
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
				break;
			case 0x1: /* 4-1E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_GBR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;
			case 0x2: /* 4-2E */
				op->rm=(opw>>8)&15;
				op->rn=BTESH2_REG_VBR;
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
				break;
			
			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rm=(opw>>8)&15;
				op->rn=32+((opw>>4)&7);
				op->nmid=BTESH2_NMID_LDC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_MOV_RegReg;
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
			break;
		case 0x1: /* 6--1 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_MOV_RegMemW;
			break;
		case 0x2: /* 6--2 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_MOV_RegMemD;
			break;
		case 0x3: /* 6--3 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MOV_RegReg;
			
			switch(op->rn)
			{
			case 0:
				switch(op->rm)
				{
				case 1: op->Run=BTSH_Op_MOV_RegReg_R0R1; break;
				case 2: op->Run=BTSH_Op_MOV_RegReg_R0R2; break;
				case 3: op->Run=BTSH_Op_MOV_RegReg_R0R3; break;
				default: break;
				}
				break;
			case 1:
				switch(op->rm)
				{
				case 0: op->Run=BTSH_Op_MOV_RegReg_R1R0; break;
				case 2: op->Run=BTSH_Op_MOV_RegReg_R1R2; break;
				case 3: op->Run=BTSH_Op_MOV_RegReg_R1R3; break;
				default: break;
				}
				break;
			case 2:
				switch(op->rm)
				{
				case 0: op->Run=BTSH_Op_MOV_RegReg_R2R0; break;
				case 1: op->Run=BTSH_Op_MOV_RegReg_R2R1; break;
				case 3: op->Run=BTSH_Op_MOV_RegReg_R2R3; break;
				default: break;
				}
				break;
			case 3:
				switch(op->rm)
				{
				case 0: op->Run=BTSH_Op_MOV_RegReg_R3R0; break;
				case 1: op->Run=BTSH_Op_MOV_RegReg_R3R1; break;
				case 2: op->Run=BTSH_Op_MOV_RegReg_R3R2; break;
				default: break;
				}
				break;
			default:
				break;
			}
			break;
		case 0x4: /* 6--4 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_MOV_RegIncLdB;
			break;
		case 0x5: /* 6--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_MOV_RegIncLdW;
			break;
		case 0x6: /* 6--6 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_MOV_RegIncLdD;
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
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_SWAP_RegRegB;
			break;
		case 0x9: /* 6--9 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_SWAPW;
			op->fmid=BTESH2_FMID_REGLD;
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
		break;
	case 0x8: /* 8--- */
		switch((opw>>8)&15)
		{
		case 0x0: /* 80-- */
			op->rn=(opw>>4)&15;
			op->imm=opw&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_R0StDispB;
			break;
		case 0x1: /* 81-- */
			op->rn=(opw>>4)&15;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_R0StDispW;
			break;
		case 0x2: /* 82-- */
			break;
		case 0x3: /* 83-- */
			break;
		case 0x4: /* 84-- */
			op->rm=(opw>>4)&15;
			op->imm=opw&15;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_R0LdDispB;
			break;
		case 0x5: /* 85-- */
			op->rm=(opw>>4)&15;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_R0LdDispW;
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
			break;
		case 0x1: /* C1-- */
			op->rn=BTESH2_REG_GBR;
			op->rm=0;
			op->imm=(opw&255)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispW;
			break;
		case 0x2: /* C2-- */
			op->rn=BTESH2_REG_GBR;
			op->rm=0;
			op->imm=(opw&255)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispD;
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
			break;
		case 0x5: /* C5--, MOV.W @(disp,GBR),R0 */
			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=(opw&255)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispW;
			break;
		case 0x6: /* C6--, MOV.L @(disp,GBR),R0 */
			op->rn=0;
			op->rm=BTESH2_REG_GBR;
			op->imm=(opw&255)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispD;
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
			break;
		case 0x8: /* C8-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_TST;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_TST_RegImm;
			break;
		case 0x9: /* C9-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_AND_RegImm;
			break;
		case 0xA: /* CA-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_XOR_RegImm;
			break;
		case 0xB: /* CB-- */
			op->rn=0;
			op->rm=0;
			op->imm=opw&255;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_OR_RegImm;
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
		break;
	case 0xE: /* E--- */
		op->rn=(opw>>8)&15;
		op->imm=(u32)((sbyte)(opw));
		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;
		break;
	case 0xF: /* F--- */
		switch(opw&15)
		{
		case 0x0: /* F--0 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FADD;
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FADD_RR;
			break;
		case 0x1: /* F--1 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FSUB;
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FSUB_RR;
			break;
		case 0x2: /* F--2 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMUL;
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FMUL_RR;
			break;
		case 0x3: /* F--3 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FDIV;
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FDIV_RR;
			break;
		case 0x4: /* F--4 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FCMPEQ;
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FCMPEQ_RR;
			break;
		case 0x5: /* F--5 */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FCMPGT;
			op->fmid=BTESH2_FMID_FREGREG;
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
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FMOV_RR;
			break;
		case 0xD: /* F--D */
			switch((opw>>4)&15)
			{
			case 0x0: /* F-0D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FSTS;
				op->fmid=BTESH2_FMID_FREGRN;
				op->Run=BTSH_Op_FSTS_R;
				break;
			case 0x1: /* F-1D */
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FLDS;
				op->fmid=BTESH2_FMID_FREGRM;
				op->Run=BTSH_Op_FLDS_R;
				break;
			case 0x2: /* F-2D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FLOAT;
				op->fmid=BTESH2_FMID_FREGRN;
				op->Run=BTSH_Op_FLOAT_R;
				break;
			case 0x3: /* F-3D */
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FTRC;
				op->fmid=BTESH2_FMID_FREGRM;
				op->Run=BTSH_Op_FTRC_R;
				break;
			case 0x4: /* F-4D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FNEG;
				op->fmid=BTESH2_FMID_FREGRN;
				op->Run=BTSH_Op_FNEG_R;
				break;
			case 0x5: /* F-5D */
				op->rn=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FABS;
				op->fmid=BTESH2_FMID_FREGRN;
				op->Run=BTSH_Op_FABS_R;
				break;
			case 0x6: /* F-6D */
				op->rn=(opw>>8)&15;
//				op->rm=(opw>>4)&15;
				op->nmid=BTESH2_NMID_FSQRT;
				op->fmid=BTESH2_FMID_FREGRN;
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
				op->fmid=BTESH2_FMID_FREGRN;
				op->Run=BTSH_Op_FCNVSD_R;
				break;
			case 0xB: /* F-BD */
				op->rm=(opw>>8)&15;
				op->nmid=BTESH2_NMID_FCNVDS;
				op->fmid=BTESH2_FMID_FREGRM;
				op->Run=BTSH_Op_FCNVDS_R;
				break;
			case 0xF:
				switch((opw>>8)&15)
				{
				case 0x3:
					op->nmid=BTESH2_NMID_FSCHG;
					op->fmid=BTESH2_FMID_NONE;
					op->Run=BTSH_Op_FSCHG_Z;
					break;
				}
				break;
			}
			break;

		case 0xE: /* F--E */
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->nmid=BTESH2_NMID_FMAC;
			op->fmid=BTESH2_FMID_FREGREG;
			op->Run=BTSH_Op_FMAC_RR;
			break;
		}
		break;
	}
	return(0);
}

void BTESH2_Trace_Run1(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run2(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run3(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run4(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run5(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run6(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run7(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run8(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run9(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run10(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run11(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run12(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run13(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run14(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run15(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops);
}

void BTESH2_Trace_Run16(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Opcode **ops;
	ops=tr->ops;
	cpu->regs[BTESH2_REG_PC]=tr->nxtpc;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops); ops++;
	(*ops)->Run(cpu, *ops); ops++;	(*ops)->Run(cpu, *ops);
}

int BTESH2_DecodeTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr, u32 spc)
{
	BTESH2_Opcode *op, *op1;
	u32 pc;
	int n;
	
	tr->srcpc=spc;
	tr->csfl=cpu->csfl;
	
	pc=spc; n=0;
	while(n<BTESH2_TR_MAXOPS)
	{
		BTESH2_MarkAddrTrapSmc(cpu, pc);
		op=BTESH2_AllocOpcode(cpu);
		BTESH2_DecodeOpcode(cpu, op, pc);
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
			
			op->imm=BTESH2_GetAddrDWord(cpu, op->imm);
			op->fmid=BTESH2_FMID_REGVIMM;
			op->Run=BTSH_Op_MOV_RegImm;
		}
#endif
		
		if(op->fl&BTESH2_OPFL_CTRLF)
		{
			if(op->fl&BTESH2_OPFL_DLYSLOT)
			{
				if(n<=(BTESH2_TR_MAXOPS-2))
				{
					op1=BTESH2_AllocOpcode(cpu);
					BTESH2_DecodeOpcode(cpu, op1, pc);
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

					tr->ops[n++]=op;
					tr->ops[n++]=op1;
					
//					tr->ops[n++]=op1;
//					tr->ops[n++]=op;
					break;
				}
				
				BTESH2_FreeOpcode(cpu, op);
				pc-=2;
				break;
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
	}

	return(0);
}

void BTESH2_FlushTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	int i;
	
	if((tr->nops<0) || (tr->nops>BTESH2_TR_MAXOPS))
	{
		i=-1;
		return;
	}
	
	for(i=0; i<tr->nops; i++)
	{
		BTESH2_FreeOpcode(cpu, tr->ops[i]);
	}
	
	memset(tr, 0, sizeof(BTESH2_Trace));
//	tr->nops=0;
}

BTESH2_Trace *BTESH2_TraceForAddr(BTESH2_CpuState *cpu, u32 spc)
{
	BTESH2_Trace *tr, *tr1;
	u32 spc1;
	int h, h1, hp, hp1;
	int i, j, k;
	
//	spc1=spc^(spc>>16);
//	spc1=spc^(spc>>20);
//	h=((spc1*524287)>>20)&(BTESH2_TR_HASHSZ-1);
//	h=((spc*524287)>>20)&(BTESH2_TR_HASHSZ-1);

//	spc1=spc^(spc>>20);
//	hp=spc*BTESH2_TR_HASHPR;
	hp=(spc^cpu->csfl)*BTESH2_TR_HASHPR;
	h=(hp>>BTESH2_TR_HASHSHR)&(BTESH2_TR_HASHSZ-1);

	tr=cpu->icache[h];
	if(tr)
	{
		if((tr->srcpc==spc) && (tr->csfl==cpu->csfl))
		{
			return(tr);
		}

#if 1
		h1=h|BTESH2_TR_HASHSZ;
		tr1=cpu->icache[h1];

		if(tr1)
		{
			if((tr1->srcpc==spc) && (tr1->csfl==cpu->csfl))
			{
				cpu->icache[h]=tr1;
				cpu->icache[h1]=tr;
				return(tr1);
			}

			cpu->icache[h]=tr1;
			cpu->icache[h1]=tr;
			tr=tr1;
			
			BTESH2_FlushTrace(cpu, tr);
			cpu->tr_dcol++;
		}else
		{
			cpu->icache[h1]=tr;
			tr=BTESH2_AllocTrace(cpu);
			cpu->icache[h]=tr;
		}
#else
		BTESH2_FlushTrace(cpu, tr);
		cpu->tr_dcol++;
#endif
	}else
	{
		tr=BTESH2_AllocTrace(cpu);
		cpu->icache[h]=tr;
	}
	
	cpu->tr_dtot++;
	BTESH2_DecodeTrace(cpu, tr, spc);
	return(tr);
}
