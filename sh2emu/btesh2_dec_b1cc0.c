int BTESH2_DecodeOpcode_BJX1_CC0(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	int i, j, k;

	switch(opw2>>12)
	{
	case 0x0:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=0;
		op->imm=0;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;

		switch(opw2&15)
		{

		case 0x2: /* 0--2 */
			op->rn=(opw2>>8)&15;
			if(opw&4)op->rn|=16;

			op->nmid=BTESH2_NMID_STC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_OpJQ_MOV_RegRegQ;

			switch((opw2>>4)&15)
			{
			case 0x0: /* 0-02 */
				op->rm=BTESH2_REG_SR;
				op->Run=BTSH_Op_MOV_RegRegSr;
				break;
			case 0x1:	op->rm=BTESH2_REG_GBR;		break;
			case 0x2:	op->rm=BTESH2_REG_VBR;		break;
			case 0x3:	op->rm=BTESH2_REG_SSR;		break;
			case 0x4:	op->rm=BTESH2_REG_SPC;		break;
			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rm=BTESH2_REG_RBANK+((opw2>>4)&7);
				if(opw&2)op->rm|=8;
				break;

			default:	op->Run=NULL;				break;
			}
			break;

		case 0x4: /* 0--4 */
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_OpJQ_MOV_RegStR0nB;
			break;
		case 0x5: /* 0--5 */
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_OpJQ_MOV_RegStR0nW;
			break;
		case 0x6: /* 0--6 */
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_OpJQ_MOV_RegStR0nD;
			if(opw&8)
			{
				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegStR0nQ;
			}
			break;

		case 0xA: /* 0--A */
			op->rn=(opw2>>8)&15;
			if(opw&4)op->rn|=16;

			op->nmid=BTESH2_NMID_STS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_OpJQ_MOV_RegRegQ;

			switch((opw2>>4)&15)
			{
			case 0x0:	op->rm=BTESH2_REG_MACH;		break;
			case 0x1:	op->rm=BTESH2_REG_MACL;		break;
			case 0x2:	op->rm=BTESH2_REG_PR;		break;
			case 0x3:	op->rm=BTESH2_REG_SGR;		break;

			case 0x5:	op->rm=BTESH2_REG_FPUL;		break;
			case 0x6:	op->rm=BTESH2_REG_FPSCR;	break;

			default:	op->Run=NULL;				break;
			}
			break;

		case 0xC: /* 0--C */
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_OpJQ_MOV_RegLdR0mB;
			if(opw&0x0001)
			{	op->nmid=BTESH2_NMID_MOVUB;
				op->Run=BTSH_OpJQ_MOVU_RegLdR0mB;	}
			break;
		case 0xD: /* 0--D */
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_OpJQ_MOV_RegLdR0mW;
			if(opw&0x0001)
			{	op->nmid=BTESH2_NMID_MOVUW;
				op->Run=BTSH_OpJQ_MOVU_RegLdR0mW;	}
			break;
		case 0xE: /* 0--E */
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_OpJQ_MOV_RegLdR0mD;
			if(opw&0x0001)
			{	op->nmid=BTESH2_NMID_MOVUL;
				op->Run=BTSH_OpJQ_MOVU_RegLdR0mD;	}
			else if(opw&0x0008)
			{	op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegLdR0mQ;	}
			break;

		default:
			break;
		}
		break;

	case 0x1:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->imm=(opw2&15)*4;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
		if(opw&0x0001)	op->imm+=16*4;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGSTDISP;
		op->Run=BTSH_Op_MOV_RegStDispD;
//		if(cpu->csfl&BTESH2_CSFL_SRJQ)
		if(opw&0x0008)
		{
			op->imm*=2;
			op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegStDispQ;
		}
		break;

	case 0x2: /* 2--- */
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=0;
		op->imm=0;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;

		switch(opw2&15)
		{
		case 0x0: /* 2--0 */
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_OpJQ_MOV_RegStB;
			break;
		case 0x1: /* 2--1 */
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_OpJQ_MOV_RegStW;
			break;
		case 0x2: /* 2--2 */
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_MOV_RegStD;
			op->Run=BTSH_OpJQ_MOV_RegStD;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegStQ;
			}
			break;
//		case 0x3:
//			op->nmid=BTESH2_NMID_CASL;
//			op->fmid=BTESH2_FMID_REGREG;
//			op->Run=BTSH_Op_CASL_RegRegR0D;
//			break;
		case 0x4: /* 2--4 */
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_OpJQ_MOV_RegDecStB;
			break;
		case 0x5: /* 2--5 */
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_OpJQ_MOV_RegDecStW;
			break;
		case 0x6: /* 2--6 */
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_MOV_RegDecStD;
			op->Run=BTSH_OpJQ_MOV_RegDecStD;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegDecStQ;
			}
			break;
//		case 0x7: /* 2--7 */
//			op->nmid=BTESH2_NMID_DIV0S;
//			op->fmid=BTESH2_FMID_REGREG;
//			op->Run=BTSH_Op_DIV0S_RegReg;
//			break;
		case 0x8: /* 2--8 */
			op->nmid=BTESH2_NMID_TST;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_TST_RegReg;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_TSTQ;
				op->Run=BTSH_Op_TSTQ_RegReg;
			}
			break;
		case 0x9: /* 2--9 */
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_AND_RegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_ANDQ;
				op->Run=BTSH_Op_ANDQ_RegReg;
			}
			break;
		case 0xA: /* 2--A */
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_XOR_RegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_XORQ;
				op->Run=BTSH_Op_XORQ_RegReg;
			}
			break;
		case 0xB: /* 2--B */
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_OR_RegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_ORQ;
				op->Run=BTSH_Op_ORQ_RegReg;
			}
			break;

#if 0
		case 0xC: /* 2--C */
			op->nmid=BTESH2_NMID_CMPSTR;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPSTR_RegReg;
			break;
		case 0xD: /* 2--D */
			op->nmid=BTESH2_NMID_XTRCT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_XTRCT_RegReg;
			break;
		case 0xE: /* 2--E */
			op->nmid=BTESH2_NMID_MULUW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MULUW_RegReg;
			break;
		case 0xF: /* 2--F */
			op->nmid=BTESH2_NMID_MULSW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_MULSW_RegReg;
			break;
#endif
		}
		break;

	case 0x3: /* 3--- */
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=0;
		op->imm=0;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;

		switch(opw2&15)
		{
		case 0x0: /* 3--0 */
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPEQ_RegReg;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_CMPQEQ;
				op->Run=BTSH_Op_CMPEQ_RegRegQ;
			}
			break;

		case 0x1: /* 3--1 */
			break;

		case 0x2: /* 3--2 */
			op->nmid=BTESH2_NMID_CMPHS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPHS_RegReg;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_CMPQHS;
				op->Run=BTSH_Op_CMPHS_RegRegQ;
			}
			break;
		case 0x3: /* 3--3 */
			op->nmid=BTESH2_NMID_CMPGE;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPGE_RegReg;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_CMPQGE;
				op->Run=BTSH_Op_CMPGE_RegRegQ;
			}
			break;
#if 0
		case 0x4: /* 3--4 */
			op->nmid=BTESH2_NMID_DIV1;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DIV1_RegReg;
			break;
#endif

#if 1
		case 0x5: /* 3--5 */
			if(opw&0x0008)
				break;
			op->nmid=BTESH2_NMID_DMULU;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DMULU_RegReg;
			break;
#endif

		case 0x6: /* 3--6 */
			op->nmid=BTESH2_NMID_CMPHI;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPHI_RegReg;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_CMPQHI;
				op->Run=BTSH_Op_CMPHI_RegRegQ;
			}
			break;
		case 0x7: /* 3--7 */
			op->nmid=BTESH2_NMID_CMPGT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_CMPGT_RegReg;
			if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_CMPQGT;
				op->Run=BTSH_Op_CMPGT_RegRegQ;
			}
			break;
		case 0x8: /* 3--8 */
			op->nmid=BTESH2_NMID_SUB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUB_RegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_SUBQ;
				op->Run=BTSH_Op_SUBQ_RegReg;
			}
			break;

#if 0
		case 0x9: /* 3--9 */
			break;
			
		case 0xA: /* 3--A */
			op->nmid=BTESH2_NMID_SUBC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUBC_RegReg;
			break;
		case 0xB: /* 3--B */
			op->nmid=BTESH2_NMID_SUBV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SUBV_RegReg;
			break;
#endif
		case 0xC: /* 3--C */
			op->nmid=BTESH2_NMID_ADD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_ADD_RegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_ADDQ;
				op->Run=BTSH_Op_ADDQ_RegReg;
			}
			break;

#if 1
		case 0xD: /* 3--D */
			if(opw&0x0008)
				break;

			op->nmid=BTESH2_NMID_DMULS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_DMULS_RegReg;
			break;
#endif

#if 0
		case 0xE: /* 3--E */
			op->nmid=BTESH2_NMID_ADDC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_ADDC_RegReg;
			break;
		case 0xF: /* 3--F */
			op->nmid=BTESH2_NMID_ADDV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_ADDV_RegReg;
			break;
#endif
		}
		break;

	case 0x4: /* 4--- */
		switch(opw2&15)
		{
		case 0x1: /* 4--1 */
			switch((opw2>>4)&15)
			{
			case 0xC: /* 4-C1 */
				op->rm=(opw2>>8)&15;
				op->rn=0;
				op->imm=pc+4;
				if(opw&4)op->rm+=BTESH2_REG_R16;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_OpJQ_MOV_RegStDispB;
				break;
			case 0xD: /* 4-D1 */
				op->rm=(opw2>>8)&15;
				op->rn=0;
				op->imm=pc+4;
				if(opw&4)op->rm+=BTESH2_REG_R16;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_OpJQ_MOV_RegStDispW;
				break;
			case 0xE: /* 4-E1 */
				op->rm=(opw2>>8)&15;
				op->rn=0;
				op->imm=(pc+4)&(~3);
				if(opw&4)op->rm+=BTESH2_REG_R16;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGSTDISP;
				op->Run=BTSH_OpJQ_MOV_RegStDispD;
				if(opw&8)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegStDispQ;
				}
				break;
			}
			break;
		case 0x5: /* 4--5 */
			switch((opw2>>4)&15)
			{
			case 0xC: /* 4-C5 */
				op->rn=(opw2>>8)&15;
				op->rm=0;
				op->imm=pc+4;
				if(opw&4)op->rn+=BTESH2_REG_R16;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_OpJQ_MOV_RegLdDispB;
				if(opw&0x0001)
				{	op->nmid=BTESH2_NMID_MOVUB;
					op->Run=BTSH_OpJQ_MOVU_RegLdDispB;	}
				break;
			case 0xD: /* 4-D5 */
				op->rn=(opw2>>8)&15;
				op->rm=0;
				op->imm=pc+4;
				if(opw&4)op->rn+=BTESH2_REG_R16;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_OpJQ_MOV_RegLdDispW;
				if(opw&0x0001)
				{	op->nmid=BTESH2_NMID_MOVUW;
					op->Run=BTSH_OpJQ_MOVU_RegLdDispW;	}
				break;
			case 0xE: /* 4-E5 */
				op->rn=(opw2>>8)&15;
				op->rm=0;
				op->imm=(pc+4)&(~3);
				if(opw&4)op->rn+=BTESH2_REG_R16;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_OpJQ_MOV_RegLdDispD;
				if(opw&0x0001)
				{	op->nmid=BTESH2_NMID_MOVUL;
					op->Run=BTSH_OpJQ_MOVU_RegLdDispD;	}
				else if(opw&0x0008)
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
				}
				break;
			case 0xF: /* 4-F5 */
				op->rn=(opw2>>8)&15;
				op->rm=0;
				op->imm=pc+4;
				if(opw&4)op->rn+=BTESH2_REG_R16;
				op->nmid=BTESH2_NMID_LEAB;
				op->fmid=BTESH2_FMID_REGLDDISP;
				op->Run=BTSH_OpJQ_LEA_RegLdDisp;
				break;
			}
			break;

		case 0xA: /* 4--A */
			op->rm=(opw2>>8)&15;
			if(opw&4)op->rm|=16;

			op->nmid=BTESH2_NMID_LDS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_OpJQ_MOV_RegRegQ;

			switch((opw2>>4)&15)
			{
			case 0x0:	op->rn=BTESH2_REG_MACH;		break;
			case 0x1:	op->rn=BTESH2_REG_MACL;		break;
			case 0x2:	op->rn=BTESH2_REG_PR;		break;
			case 0x3:	op->rn=BTESH2_REG_SGR;		break;

			case 0x5:	op->rn=BTESH2_REG_FPUL;		break;
			case 0x6:	op->rn=BTESH2_REG_FPSCR;	break;

			default:	op->Run=NULL;				break;
			}
			break;

		case 0xC: /* 4--C */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&4)op->rn+=BTESH2_REG_R16;
			if(opw&2)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_SHAD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SHADL_RegReg;
			if(opw&8)
			{
				op->nmid=BTESH2_NMID_SHADQ;
				op->Run=BTSH_Op_SHADQ_RegReg;
			}
			break;
		case 0xD: /* 4--D */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&4)op->rn+=BTESH2_REG_R16;
			if(opw&2)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_SHLD;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SHLDL_RegReg;
			if(opw&8)
			{
				op->nmid=BTESH2_NMID_SHLDQ;
				op->Run=BTSH_Op_SHLDQ_RegReg;
			}
			break;

		case 0xE: /* 4--E */
			op->rm=(opw2>>8)&15;
			if(opw&4)op->rm|=16;

			op->nmid=BTESH2_NMID_LDC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_OpJQ_MOV_RegRegQ;

			switch((opw2>>4)&15)
			{
//			case 0x0:	op->rn=BTESH2_REG_SR;		break;
			case 0x1:	op->rn=BTESH2_REG_GBR;		break;
			case 0x2:	op->rn=BTESH2_REG_VBR;		break;
			case 0x3:	op->rn=BTESH2_REG_SSR;		break;
			case 0x4:	op->rn=BTESH2_REG_SPC;		break;
			case 0x8:	case 0x9:	case 0xA:	case 0xB:
			case 0xC:	case 0xD:	case 0xE:	case 0xF:
				op->rn=BTESH2_REG_RBANK+((opw2>>4)&7);
				if(opw&2)op->rn|=8;
				break;
			default:	op->Run=NULL; break;
			}
			break;
		}
		break;

	case 0x5:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->imm=(opw2&15)*4;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
		if(opw&0x0001)	op->imm+=16*4;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGLDDISP;
		op->Run=BTSH_Op_MOV_RegLdDispD;
//		if(cpu->csfl&BTESH2_CSFL_SRJQ)
		if(opw&0x0008)
		{
			op->imm*=2;
			op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
		}
		break;

	case 0x6:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=0;
		op->imm=0;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;

		switch(opw2&15)
		{
		case 0x0: /* 6--0 */
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_OpJQ_MOV_RegMemB;
			if(opw&0x0001)
			{	op->nmid=BTESH2_NMID_MOVUB;
				op->Run=BTSH_OpJQ_MOVU_RegLdDispB;	}
			break;
		case 0x1: /* 6--1 */
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_OpJQ_MOV_RegMemW;
			if(opw&0x0001)
			{	op->nmid=BTESH2_NMID_MOVUW;
				op->Run=BTSH_OpJQ_MOVU_RegLdDispW;	}
			break;
		case 0x2: /* 6--2 */
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_OpJQ_MOV_RegMemD;
			if(opw&0x0001)
			{	op->nmid=BTESH2_NMID_MOVUL;
				op->Run=BTSH_OpJQ_MOVU_RegLdDispD;	}
			else if(opw&0x0008)
			{
				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegMemQ;
			}
			break;
		case 0x3: /* 6--3 */
			op->nmid=BTESH2_NMID_MOV;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_OpJQ_MOV_RegRegQ;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;
			}
			break;

		case 0x4: /* 6--4 */
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_OpJQ_MOV_RegIncLdB;
			break;
		case 0x5: /* 6--5 */
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_MOV_RegIncLdW;
			op->Run=BTSH_OpJQ_MOV_RegIncLdW;
			break;
		case 0x6: /* 6--6 */
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_OpJQ_MOV_RegIncLdD;
			if(opw&0x0008)
			{
				if(opw&1)
				{
				}else
				{
					op->nmid=BTESH2_NMID_MOVQ;
					op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				}
			}
			break;
		case 0x7: /* 6--7 */
			op->nmid=BTESH2_NMID_NOT;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_NOTQ_RegReg;
			break;
#if 0
		case 0x8: /* 6--8 */
			op->nmid=BTESH2_NMID_SWAPB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SWAP_RegRegB;
			break;
		case 0x9: /* 6--9 */
			op->nmid=BTESH2_NMID_SWAPW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_SWAP_RegRegW;
			break;
		case 0xA: /* 6--A */
			op->nmid=BTESH2_NMID_NEGC;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_NEGC_RegReg;
			break;
#endif
		case 0xB: /* 6--B */
			op->nmid=BTESH2_NMID_NEG;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_NEGQ_RegReg;
			break;
		case 0xC: /* 6--C */
			op->nmid=BTESH2_NMID_EXTUB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTUB_RegReg;
			break;
		case 0xD: /* 6--D */
			op->nmid=BTESH2_NMID_EXTUW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTUW_RegReg;
//			if((opw&9)==9)
			if(opw&8)
			{	op->nmid=BTESH2_NMID_EXTUL;
				op->Run=BTSH_Op_EXTUL_RegReg;	}
			break;
		case 0xE: /* 6--E */
			op->nmid=BTESH2_NMID_EXTSB;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTSB_RegReg;
			break;
		case 0xF: /* 6--F */
			op->nmid=BTESH2_NMID_EXTSW;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_EXTSW_RegReg;
//			if((opw&9)==9)
			if(opw&8)
			{	op->nmid=BTESH2_NMID_EXTSL;
				op->Run=BTSH_Op_EXTSL_RegReg;	}
			break;

		default:
			break;
		}
		break;

	case 0x7: /* 7--- */
		op->rn=(opw2>>8)&15;
		op->imm=(u64)((sbyte)(opw2));

//		if(opw&0x0008)	break;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
//		if(opw&0x0002)	op->imm^=~0x01FF;
//		if(opw&0x0001)	op->imm^= 0x0100;
		if(opw&0x0002)	break;
		if(opw&0x0001)	break;

		op->nmid=BTESH2_NMID_ADD;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_ADDQ_RegImm;
		break;

	case 0xA:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=(opw2   )&15;
		op->imm=0;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
		if(opw&0x0001)	op->ro+=BTESH2_REG_R16;
		if(!op->ro)break;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGSTRODISP;
		op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
		if(opw&0x0008)
		{	op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispQ;		}
		break;
	case 0xB:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=(opw2   )&15;
		op->imm=0;
		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
		if(opw&0x0001)	op->ro+=BTESH2_REG_R16;
		if(!op->ro)break;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGLDRODISP;
		op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
		if(opw&0x0008)
		{	op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;		}
		break;

	case 0xE: /* E--- */
		op->rn=(opw2>>8)&15;
		op->imm=(u64)((sbyte)(opw2));

		if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
//		if(opw&0x0002)	op->imm^=~0x01FF;
//		if(opw&0x0001)	op->imm^= 0x0100;
		if(opw&0x0002)	break;
		if(opw&0x0001)	break;

		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_OpJQ_MOV_RegImmQ;
		break;

	case 0xF: /* F--- */
		switch(opw2&15)
		{
		case 0x6: /* F--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
			if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
			op->ro=0;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDR0M;
			op->Run=BTSH_Op_FMOV_RegLdR0m;
			break;
		case 0x7: /* F--7 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
			if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
			op->ro=0;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTR0N;
			op->Run=BTSH_Op_FMOV_RegStR0n;
			break;
		case 0x8: /* F--8 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
			if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLD;
			op->Run=BTSH_Op_FMOV_RegLd;
			break;
		case 0x9: /* F--9 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
			if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGINCLD;
			op->Run=BTSH_Op_FMOV_RegIncLd;
			break;
		case 0xA: /* F--A */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
			if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGST;
			op->Run=BTSH_Op_FMOV_RegSt;
			break;
		case 0xB: /* F--B */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			if(opw&0x0004)	op->rn+=BTESH2_REG_R16;
			if(opw&0x0002)	op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGDECST;
			op->Run=BTSH_Op_FMOV_RegDecSt;
			break;
		}
		break;

	default:
		break;
	}
	return(0);
}
