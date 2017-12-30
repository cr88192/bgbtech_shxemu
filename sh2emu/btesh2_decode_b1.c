int BTESH2_DecodeOpcode_BJX1_Only64(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	return(-1);
}

int BTESH2_DecodeOpcode_BJX1_8E(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	int i, j, k;

	op->fl=BTESH2_OPFL_EXTRAWORD;

	switch(opw2>>12)
	{
	case 0x0: /* 0--- */
		switch(opw2&15)
		{
		case 0x0:
			break;
		case 0x1:
			break;
		case 0x2:
			break;
		case 0x3:
			break;
		case 0x4: /* 0--4 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*1;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_MOV_RegStRoDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStRoDispB;
			break;
		case 0x5: /* 0--5 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*2;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_MOV_RegStRoDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStRoDispW;
			break;
		case 0x6: /* 0--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*4;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_MOV_RegStRoDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
			break;
		case 0x7:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*8;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_MOV_RegStRoDispQ;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStRoDispQ;
			break;
		case 0x8:
			break;
		case 0x9:
			break;
		case 0xA:
			break;
		case 0xB:
			break;
		case 0xC: /* 0--C */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*1;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_MOV_RegLdRoDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispB;
			break;
		case 0xD: /* 0--D */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*2;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_MOV_RegLdRoDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispW;
			break;
		case 0xE: /* 0--E */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*4;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_MOV_RegLdRoDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
			break;
		case 0xF:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*8;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_MOV_RegLdRoDispQ;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;
			break;
		default:
			break;
		}
		break;
	case 0x1: /* 1--- */
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=(opw2   )&15;
		op->imm=((sbyte)opw)*4;
		if(!op->ro)break;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGSTRODISP;
		op->Run=BTSH_Op_MOV_RegStRoDispD;
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
			op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
		break;
	case 0x2: /* 2--- */
		switch(opw2&15)
		{
		case 0x0:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(sbyte)opw;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStDispB;
			break;
		case 0x1:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStDispW;
			break;
		case 0x2:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStDispD;
			break;
		case 0x3:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*8;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispQ;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegStDispQ;
			break;

		case 0x4:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_FMOVS_RegStDisp;
			break;
#if 0
		case 0x5:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_FMOVD;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_FMOVD_RegStDisp;
			break;
#endif
		case 0x6:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_FMOV_RegLdDisp;
			break;
#if 0
		case 0x7:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*8;
			op->nmid=BTESH2_NMID_FMOVD;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_FMOVD_RegLdDisp;
			break;
#endif

		case 0x9:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((byte)opw);
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_AND_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_ANDQ;
				op->Run=BTSH_Op_ANDQ_RegImmReg;
			}
			break;
		case 0xA:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((byte)opw);
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_XOR_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_XORQ;
				op->Run=BTSH_Op_XORQ_RegImmReg;
			}
			break;
		case 0xB:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((byte)opw);
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_OR_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_ORQ;
				op->Run=BTSH_Op_ORQ_RegImmReg;
			}
			break;

		case 0xE:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((byte)opw);
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_MUL_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->nmid=BTESH2_NMID_MULQ;
				op->Run=BTSH_Op_MULQ_RegImmReg;
			}
			break;
		case 0xF:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(~255LL)|((byte)opw);
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_MUL_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->nmid=BTESH2_NMID_MULQ;
				op->Run=BTSH_Op_MULQ_RegImmReg;
			}
			break;

		default:
			break;
		}
		break;

	case 0x3: /* 3--- */
		switch(opw2&15)
		{
		case 0x0:
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<20))>>20;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPEQ_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQEQ;
				op->Run=BTSH_Op_CMPEQ_RegImmQ;
			}
			break;

		case 0x2:
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<20))>>20;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPHS;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPHS_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQHS;
				op->Run=BTSH_Op_CMPHS_RegImmQ;
			}
			break;
		case 0x3:
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<20))>>20;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPGE;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPGE_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQGE;
				op->Run=BTSH_Op_CMPGE_RegImmQ;
			}
			break;

		case 0x6:
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<20))>>20;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPHI;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPHI_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQHI;
				op->Run=BTSH_Op_CMPHI_RegImmQ;
			}
			break;
		case 0x7:
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<20))>>20;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPGT;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPGT_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRDQ)
			{
				op->nmid=BTESH2_NMID_CMPQGT;
				op->Run=BTSH_Op_CMPGT_RegImmQ;
			}
			break;

		case 0x8:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((byte)opw);
			op->nmid=BTESH2_NMID_SUB;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SUB_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_SUBQ;
				op->Run=BTSH_Op_SUBQ_RegImmReg;
			}
			break;
		case 0x9:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((byte)opw)|(~255);
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_AND_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_ANDQ;
				op->Run=BTSH_Op_ANDQ_RegImmReg;
			}
			break;

		case 0xC:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((byte)opw);
			op->nmid=BTESH2_NMID_ADD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ADD_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
//				op->nmid=BTESH2_NMID_ADDQ;
				op->Run=BTSH_Op_ADDQ_RegImmReg;
			}
			break;

		default:
			break;
		}
		break;
	case 0x4: /* 4--- */
		switch(opw2&15)
		{
#if 1
		case 0x3:
			op->ro=(opw2>>8)&15;
//			op->rn=(opw2>>4)&15;
//			op->rm=(opw2>>0)&15;
			op->rn=(opw>>4)&15;
			op->rm=(opw>>0)&15;
			op->fmid=BTESH2_FMID_REGREGREG;
			switch((opw2>>4)&15)
			{
			case 0x0:
				op->nmid=BTESH2_NMID_ADD;
				op->Run=BTSH_Op_ADDQ_RegRegReg;
				break;
			case 0x1:
				op->nmid=BTESH2_NMID_SUB;
				op->Run=BTSH_Op_SUBQ_RegRegReg;
				break;
			case 0x2:
				op->nmid=BTESH2_NMID_MULL;
				op->Run=BTSH_Op_MULQ_RegRegReg;
				break;
			case 0x3:
				op->nmid=BTESH2_NMID_AND;
				op->Run=BTSH_Op_ANDQ_RegRegReg;
				break;
			case 0x4:
				op->nmid=BTESH2_NMID_OR;
				op->Run=BTSH_Op_ORQ_RegRegReg;
				break;
			case 0x5:
				op->nmid=BTESH2_NMID_XOR;
				op->Run=BTSH_Op_XORQ_RegRegReg;
				break;
			case 0x6:
				op->nmid=BTESH2_NMID_SHLD;
				op->Run=BTSH_Op_SHLL_RegRegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHLDQ;
					op->Run=BTSH_Op_SHLLQ_RegRegReg;
				}else if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_Op_SHLLL_RegRegReg;
				}
				break;
			case 0x7:
				op->nmid=BTESH2_NMID_SHLR;
				op->Run=BTSH_Op_SHLR_RegRegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHLRQ;
					op->Run=BTSH_Op_SHLRQ_RegRegReg;
				}else if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_Op_SHLRL_RegRegReg;
				}
				break;
			case 0x8:
				op->nmid=BTESH2_NMID_SHAD;
				op->Run=BTSH_Op_SHAL_RegRegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHADQ;
					op->Run=BTSH_Op_SHALQ_RegRegReg;
				}else if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_Op_SHALL_RegRegReg;
				}
				break;
			case 0x9:
				op->nmid=BTESH2_NMID_SHAR;
				op->Run=BTSH_Op_SHAR_RegRegReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHARQ;
					op->Run=BTSH_Op_SHARQ_RegRegReg;
				}else if(cpu->csfl&BTESH2_CSFL_SRJQ)
				{
					op->Run=BTSH_Op_SHARL_RegRegReg;
				}
				break;

			case 0xC:
				op->nmid=BTESH2_NMID_FADD;
				op->Run=BTSH_Op_FADD_RRR;
				break;
			case 0xD:
				op->nmid=BTESH2_NMID_FSUB;
				op->Run=BTSH_Op_FSUB_RRR;
				break;
			case 0xE:
				op->nmid=BTESH2_NMID_FMUL;
				op->Run=BTSH_Op_FMUL_RRR;
				break;
			case 0xF:
				op->nmid=BTESH2_NMID_FDIV;
				op->Run=BTSH_Op_FDIV_RRR;
				break;
			}
			break;
#endif
		case 0x4:
			switch((opw2>>4)&15)
			{
			case 0x8:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_IMMSTRN;
				op->Run=BTSH_Op_MOV_ImmStB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmStB;
				break;
			case 0x9:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_IMMSTRN;
				op->Run=BTSH_Op_MOV_ImmStW;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmStW;
				break;
			case 0xA:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_IMMSTRN;
				op->Run=BTSH_Op_MOV_ImmStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmStD;
				break;
			case 0xB:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_IMMSTRN;
				op->Run=BTSH_Op_MOV_ImmStQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmStQ;
				break;

			case 0xC:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_IMMDECSTRN;
				op->Run=BTSH_Op_MOV_ImmDecStB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmDecStB;
				break;
			case 0xD:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_IMMDECSTRN;
				op->Run=BTSH_Op_MOV_ImmDecStW;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmDecStW;
				break;
			case 0xE:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_IMMDECSTRN;
				op->Run=BTSH_Op_MOV_ImmDecStD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmDecStD;
				break;
			case 0xF:
				op->rn=(opw2>>8)&15;
				op->imm=((sbyte)opw);
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_IMMDECSTRN;
				op->Run=BTSH_Op_MOV_ImmDecStQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_ImmDecStQ;
				break;
			}
			break;

#if 1
		case 0x5:
			i=(sbyte)opw;
			op->rn=(opw2>>8)&15;
			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->fmid=BTESH2_FMID_REG1ABS;
			switch((opw2>>4)&15)
			{
			case 0:
				op->nmid=BTESH2_NMID_BREQ;
				op->Run=BTSH_Op_BREQ_RegAbs;
				break;
			case 1:
				op->nmid=BTESH2_NMID_BRNE;
				op->Run=BTSH_Op_BRNE_RegAbs;
				break;
			case 2:
				op->nmid=BTESH2_NMID_BRGT;
				op->Run=BTSH_Op_BRGT_RegAbs;
				break;
			case 3:
				op->nmid=BTESH2_NMID_BRLE;
				op->Run=BTSH_Op_BRLE_RegAbs;
				break;
			case 4:
				op->nmid=BTESH2_NMID_BRGE;
				op->Run=BTSH_Op_BRGE_RegAbs;
				break;
			case 5:
				op->nmid=BTESH2_NMID_BRLT;
				op->Run=BTSH_Op_BRLT_RegAbs;
				break;
			}
			break;

		case 0x6:
		case 0x7:
			i=(sbyte)opw;
			op->rn=8|((opw2>>8)&7);
			op->rm=8|((opw2>>4)&7);
			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->fmid=BTESH2_FMID_REG2ABS;

			j=(opw2&1)|((opw2>>6)&2)|((opw2>>9)&4);
			switch(j)
			{
			case 0:
				op->nmid=BTESH2_NMID_BREQ;
				op->Run=BTSH_Op_BREQ_RegRegAbs;
				break;
			case 1:
				op->nmid=BTESH2_NMID_BRNE;
				op->Run=BTSH_Op_BRNE_RegRegAbs;
				break;
			case 2:
				op->nmid=BTESH2_NMID_BRGT;
				op->Run=BTSH_Op_BRGT_RegRegAbs;
				break;
			case 3:
				op->nmid=BTESH2_NMID_BRLE;
				op->Run=BTSH_Op_BRLE_RegRegAbs;
				break;
			case 4:
				op->nmid=BTESH2_NMID_BRGE;
				op->Run=BTSH_Op_BRGE_RegRegAbs;
				break;
			case 5:
				op->nmid=BTESH2_NMID_BRLT;
				op->Run=BTSH_Op_BRLT_RegRegAbs;
				break;
			}
			break;
#endif

		case 0xC:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw);
			op->nmid=BTESH2_NMID_SHAD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SHAD_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
#if 1
				if(((opw>>6)^(opw>>7))&1)
				{
					op->imm=((((s32)op->imm)>>1)&(~63))|(op->imm&63);
					op->nmid=BTESH2_NMID_SHADQ;
					op->Run=BTSH_Op_SHADQ_RegImmReg;
					break;
				}else
				{
					op->Run=BTSH_Op_SHADL_RegImmReg;
				}
#endif

#if 0
				if(((opw>>6)^(opw>>7))&1)
				{
					op->imm=((((s32)op->imm)>>1)&(~63))|(op->imm&63);
					op->Run=BTSH_Op_SHADL_RegImmReg;
					if(!(cpu->csfl&BTESH2_CSFL_SRDQ))
					{
						op->nmid=BTESH2_NMID_SHADQ;
						op->Run=BTSH_Op_SHADQ_RegImmReg;
					}
					break;
				}
			
				op->Run=BTSH_Op_SHADL_RegImmReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHADQ;
					op->Run=BTSH_Op_SHADQ_RegImmReg;
				}
#endif
			}
			break;
		case 0xD:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw);
			op->nmid=BTESH2_NMID_SHLD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SHLD_RegImmReg;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
#if 1
				if(((opw>>6)^(opw>>7))&1)
				{
					op->imm=((((s32)op->imm)>>1)&(~63))|(op->imm&63);
					op->nmid=BTESH2_NMID_SHLDQ;
					op->Run=BTSH_Op_SHLDQ_RegImmReg;
					break;
				}else
				{
					op->Run=BTSH_Op_SHLDL_RegImmReg;
				}
#endif

#if 0
				if(((opw>>6)^(opw>>7))&1)
				{
					op->imm=((((s32)op->imm)>>1)&(~63))|(op->imm&63);
					op->Run=BTSH_Op_SHLDL_RegImmReg;
					if(!(cpu->csfl&BTESH2_CSFL_SRDQ))
					{
						op->nmid=BTESH2_NMID_SHLDQ;
						op->Run=BTSH_Op_SHLDQ_RegImmReg;
					}
					break;
				}
			
				op->Run=BTSH_Op_SHLDL_RegImmReg;
				if(cpu->csfl&BTESH2_CSFL_SRDQ)
				{
					op->nmid=BTESH2_NMID_SHLDQ;
					op->Run=BTSH_Op_SHLDQ_RegImmReg;
				}
#endif
			}
			break;

		case 0xE:
			op->rm=(opw >>4)&15;
			op->ro=(opw    )&15;
			op->rn=(opw2>>8)&15;
			op->imm=0;

			switch((opw2>>4)&15)
			{
			case 0x0: /* 4-0E */
				op->nmid=BTESH2_NMID_MOVUB;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				op->Run=BTSH_Op_MOV_RegLdRoDispUB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegLdRoDispUB;
				break;
			case 0x1: /* 4-1E */
				op->nmid=BTESH2_NMID_MOVUW;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				if(op->ro)
				{
					op->Run=BTSH_Op_MOV_RegLdRoDispUW;
					if(cpu->csfl&BTESH2_CSFL_SRJQ)
						op->Run=BTSH_OpJQ_MOV_RegLdRoDispUW;
				}else
				{
					op->Run=BTSH_OpJQ_MOVU_RegLdR0mW;
				}
				break;

			case 0x2: /* 4-2E */
				op->nmid=BTESH2_NMID_FMOVS;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				op->Run=BTSH_Op_FMOV_RegLdRoDisp;
				break;
			case 0x3: /* 4-3E */
				op->nmid=BTESH2_NMID_FMOVS;
				op->fmid=BTESH2_FMID_REGSTRODISP;
				op->Run=BTSH_Op_FMOV_RegStRoDisp;
				break;

			case 0x4: /* 4-4E */
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGSTRODISP;
				op->Run=BTSH_Op_MOV_RegStRoDispB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegStRoDispB;
				break;
			case 0x5: /* 4-5E */
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGSTRODISP;
				if(op->ro)
				{
					op->Run=BTSH_Op_MOV_RegStRoDispW;
					if(cpu->csfl&BTESH2_CSFL_SRJQ)
						op->Run=BTSH_OpJQ_MOV_RegStRoDispW;
				}else
				{
					op->Run=BTSH_OpJQ_MOV_RegStR0nW;
				}
				break;
			case 0x6: /* 4-6E */
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGSTRODISP;
				if(op->ro)
				{
					op->Run=BTSH_Op_MOV_RegStRoDispD;
					if(cpu->csfl&BTESH2_CSFL_SRJQ)
						op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
				}
				else
				{
					op->Run=BTSH_OpJQ_MOV_RegStR0nD;
				}
				break;
			case 0x7: /* 4-7E */
				if(!(cpu->csfl&BTESH2_CSFL_SRJQ))
					break;
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGSTRODISP;
				op->Run=BTSH_Op_MOV_RegStRoDispQ;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
				if(op->ro)
					op->Run=BTSH_OpJQ_MOV_RegStRoDispQ;
				else
					op->Run=BTSH_OpJQ_MOV_RegStR0nQ;
				break;

			case 0x8: /* 4-8E */
				op->nmid=BTESH2_NMID_LEAB;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				op->Run=BTSH_Op_LEA_RegLdRoDispB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_LEA_RegLdRoDispB;
				break;
			case 0x9: /* 4-9E */
				op->nmid=BTESH2_NMID_LEAW;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				op->Run=BTSH_Op_LEA_RegLdRoDispW;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_LEA_RegLdRoDispW;
				if(!op->ro)
					op->Run=BTSH_OpJQ_LEA_RegLdRoDispB;
				break;
			case 0xA: /* 4-AE */
				op->nmid=BTESH2_NMID_LEAL;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				op->Run=BTSH_Op_LEA_RegLdRoDispD;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_LEA_RegLdRoDispD;
				if(!op->ro)
					op->Run=BTSH_OpJQ_LEA_RegLdRoDispB;
				break;
			case 0xB: /* 4-BE */
				op->nmid=BTESH2_NMID_LEAQ;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				op->Run=BTSH_Op_LEA_RegLdRoDispQ;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_LEA_RegLdRoDispQ;
				if(!op->ro)
					op->Run=BTSH_OpJQ_LEA_RegLdRoDispB;
				break;

			case 0xC: /* 4-CE */
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				op->Run=BTSH_Op_MOV_RegLdRoDispB;
				if(cpu->csfl&BTESH2_CSFL_SRJQ)
					op->Run=BTSH_OpJQ_MOV_RegLdRoDispB;
				break;
			case 0xD: /* 4-DE */
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				if(op->ro)
				{
					op->Run=BTSH_Op_MOV_RegLdRoDispW;
					if(cpu->csfl&BTESH2_CSFL_SRJQ)
						op->Run=BTSH_OpJQ_MOV_RegLdRoDispW;
				}else
				{
					op->Run=BTSH_OpJQ_MOV_RegLdR0mW;					
				}
				break;
			case 0xE: /* 4-EE */
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGLDRODISP;
				if(op->ro)
				{
					op->Run=BTSH_Op_MOV_RegLdRoDispD;
					if(cpu->csfl&BTESH2_CSFL_SRJQ)
						op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
				}else
				{
					op->Run=BTSH_OpJQ_MOV_RegLdR0mD;					
				}
				break;
			case 0xF: /* 4-FE */
				if(!(cpu->csfl&BTESH2_CSFL_SRJQ))
					break;

				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGLDRODISP;
//				op->Run=BTSH_Op_MOV_RegLdRoDispQ;
//				if(cpu->csfl&BTESH2_CSFL_SRJQ)
				if(op->ro)
					op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;
				else
					op->Run=BTSH_OpJQ_MOV_RegLdR0mQ;					
				break;
			}
			break;

		default:
			break;
		}
		break;
	case 0x5:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=(opw2   )&15;
		op->imm=((sbyte)opw)*4;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGLDRODISP;
		op->Run=BTSH_Op_MOV_RegLdRoDispD;
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
		break;
	case 0x6:
		switch(opw2&15)
		{
		case 0x0:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*1;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdDispB;
			break;
		case 0x1:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdDispW;
			break;
		case 0x2:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdDispD;
			break;
		case 0x3:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*8;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispQ;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
			break;
		case 0x4: /* 6--4 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*1;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispB;
			break;
		case 0x5: /* 6--5 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispW;
			break;
		case 0x6: /* 6--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*4;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
			break;
		case 0x7: /* 6--7 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*8;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispQ;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;
			break;
		case 0x8:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*1;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0x9:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xA:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xB:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*8;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xC:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*1;
			op->nmid=BTESH2_NMID_MOVUB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispB;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOVU_RegLdDispB;
			break;
		case 0xD:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_MOVUW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOVU_RegLdDispW;
			break;
		case 0xE:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_MOVUL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOVU_RegLdDispD;
			break;
		case 0xF:
			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
				op, pc, opw, opw2);
			break;
		default:
			break;
		}
		break;
	case 0x7: /* 7--- */
//		i=((byte)opw)|(((byte)opw2)<<8);
		i=(((byte)opw)<<8)|((byte)opw2);
		op->rn=(opw2>>8)&15;
		op->imm=(u64)((s16)i);
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
		switch((opw2>>8)&15)
		{

		case 0x2: /* 82--, BRA/N label */
			i=(((byte)opw)<<8)|((byte)opw2);
			i=(s16)i;

			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BRAN;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BRA_Abs;
			break;
		case 0x3: /* 83--, BSR/N label */
			i=(((byte)opw)<<8)|((byte)opw2);
			i=(s16)i;
			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BSRN;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BSR_Abs;
			break;

		case 0x8:
			i=(((byte)opw)<<8)|(opw2&255);
			i=((s32)(i<<16))>>16;
			op->rn=0;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPEQ_RegImm;
			break;
		case 0x9:
//			i=((byte)opw)|(((byte)opw2)<<8);
			i=(((byte)opw)<<8)|((byte)opw2);
			i=(s16)i;
			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BT;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BT_Abs;
			break;
		case 0xA:
//			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
//				op, pc, opw, opw2);
			i=(((byte)opw)<<8)|((byte)opw2);
			op->imm=i;
			op->nmid=BTESH2_NMID_LDHF16;
			op->fmid=BTESH2_FMID_IMM;
			op->Run=BTSH_Op_LDHF16_Imm;
			break;
		case 0xB:
//			i=((byte)opw)|(((byte)opw2)<<8);
			i=(((byte)opw)<<8)|((byte)opw2);
			i=(s16)i;
			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BF;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BF_Abs;
			break;
		case 0xC:
//			i=((byte)opw)|(((byte)opw2)<<8);
			i=(((byte)opw)<<8)|((byte)opw2);
			i=(s16)i;
			op->imm=i;
//			op->nmid=BTESH2_NMID_LDSH16;
//			op->fmid=BTESH2_FMID_REGIMM;
//			op->Run=BTSH_Op_LDSH16_RegImm;
			op->nmid=BTESH2_NMID_LDIF16;
			op->fmid=BTESH2_FMID_IMM;
			op->Run=BTSH_Op_LDIF16_Imm;
			break;
		case 0xD:
//			i=((byte)opw)|(((byte)opw2)<<8);
			i=(((byte)opw)<<8)|((byte)opw2);
			i=(s16)i;
			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BTS;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BTS_Abs;
			break;
		case 0xE:
			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
				op, pc, opw, opw2);
			break;
		case 0xF:
//			i=((byte)opw)|(((byte)opw2)<<8);
			i=(((byte)opw)<<8)|((byte)opw2);
			i=(s16)i;
			op->imm=pc+2*i+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BFS;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BFS_Abs;
			break;
		default:
			break;
		}
		break;

#if 0
	case 0x9: /* 8Edd-9ndd, MOVA @(disp,PC),Rn */
		op->rn=(opw2>>8)&15;
		op->rm=BTESH2_REG_PC;
//		i=((byte)opw)|(((byte)opw2)<<8);
		i=((opw<<12)&0x1F000)|(opw2&0xFFF);
		op->ro=i;
		op->imm=((pc+6)&(~3))+i*4;
		op->fl|=BTESH2_OPFL_PCADLYSLOTD;
		
		j=(opw>>5)&7;
		switch(j)
		{
		case 0:
			op->nmid=BTESH2_NMID_MOVA;
			op->fmid=BTESH2_FMID_REGLDABS;
			op->Run=BTSH_Op_MOVA_RegImm;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOVA_RegImm;
			break;
		case 1: case 3: case 5:
			if(j==3)
				op->rn+=BTESH2_REG_RHI;
			op->imm=pc+2*i+6;
			op->nmid=BTESH2_NMID_MOVUW;
			op->fmid=BTESH2_FMID_REGLDABS;
			op->Run=BTSH_Op_MOVU_RegLdAbsW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
			{
				op->Run=BTSH_OpJQ_MOVU_RegLdAbsW;
				if(j==5)
					op->Run=BTSH_OpJQ_MOVU_RegLdAbsWQ;
			}
			break;

		case 2: case 4:
			if(j==4)
				op->rn+=16;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDABS;
			op->Run=BTSH_Op_FMOVS_RegLdAbs_FR;
			break;
		case 6: case 7:
			if(j==6)
				op->rn+=32;
			if(op->rn&1)
				op->rn^=0x11;
			op->nmid=BTESH2_NMID_FMOVD;
			op->fmid=BTESH2_FMID_REGLDABS;
			op->Run=BTSH_Op_FMOVS_RegLdAbs_DR;
			break;
		default:
			break;
		}
		break;
#endif

	case 0x9:
		i=(((byte)opw)<<8)|((byte)opw2);
		op->rn=(opw2>>8)&15;
		op->imm=(s16)i;
		op->nmid=BTESH2_NMID_LDSH16;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_LDSH16_RegImm;
		break;

	case 0xA: /* A---, BRA label */
//		i=(opw2<<20)>>20;
//		i=(i<<8)|((byte)opw);
//		i=((opw<<12)&0xFF000)|(opw2&0xFFF);
		i=((opw&0x00FF)<<12)|(opw2&0x0FFF);
		i=((s32)(i<<12))>>12;

		op->imm=pc+2*i+6;
		op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
			BTESH2_OPFL_INVDLYSLOT;
		op->nmid=BTESH2_NMID_BRA;
		op->fmid=BTESH2_FMID_ABS;
		op->Run=BTSH_Op_BRA_Abs;
		break;
	case 0xB: /* B---, BSR label */
//		i=(opw2<<20)>>20;
//		i=(i<<8)|((byte)opw);
//		i=((opw<<12)&0xFF000)|(opw2&0x00FFF);
		i=((opw&0x00FF)<<12)|(opw2&0x0FFF);
		i=((s32)(i<<12))>>12;
		op->imm=pc+2*i+6;
		op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
			BTESH2_OPFL_INVDLYSLOT;
		op->nmid=BTESH2_NMID_BSR;
		op->fmid=BTESH2_FMID_ABS;
		op->Run=BTSH_Op_BSR_Abs;
		break;

	case 0xC: /* C--- */
		switch((opw2>>8)&15)
		{
		case 0x8:
			i=(((byte)opw)<<4)|(opw2&15);
			i=(((s32)i)<<20)>>20;
			op->rn=(opw2>>4)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_TST;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_TST_RegImm;
			break;
		case 0x9:
			i=(((byte)opw)<<4)|(opw2&15);
			i=(((s32)i)<<20)>>20;
			op->rn=(opw2>>4)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_AND_RegImm;
			break;
		case 0xA:
			i=(((byte)opw)<<4)|(opw2&15);
			i=(((s32)i)<<20)>>20;
			op->rn=(opw2>>4)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_XOR_RegImm;
			break;
		case 0xB:
			i=(((byte)opw)<<4)|(opw2&15);
			i=(((s32)i)<<20)>>20;
			op->rn=(opw2>>4)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_OR_RegImm;
			break;

		default:
			break;
		}
		break;

#if 0
	case 0xD: /* D--- */
		i=((opw&31)<<8)|((byte)opw2);
		op->rn=(opw2>>8)&15;
		op->ro=i;

		switch((opw>>5)&7)
		{
		case 0: case 2: case 4: case 6:
			op->imm=((pc+6)&(~3))+(4*i);		
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDABS;
			op->Run=BTSH_Op_MOV_RegLdAbsD;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdAbsD;
			break;
		case 1: case 3: case 5:
			op->imm=pc+2*i+6;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDABS;
			op->Run=BTSH_Op_MOV_RegLdAbsW;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdAbsW;
			break;
		case 7:
			op->imm=((pc+6)&(~3))+(4*i);		
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGLDABS;
			op->Run=BTSH_Op_MOV_RegLdAbsQ;
			if(cpu->csfl&BTESH2_CSFL_SRJQ)
				op->Run=BTSH_OpJQ_MOV_RegLdAbsQ;
			break;
		}
		break;
#endif

	case 0xE: /* E--- */
//		i=(((byte)opw)<<8)|((byte)opw2);
		i=((opw&255)<<8)|(opw2&255);
		op->rn=(opw2>>8)&15;
		op->imm=(u32)((s16)i);
		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;
		if(cpu->csfl&BTESH2_CSFL_SRJQ)
			op->Run=BTSH_OpJQ_MOV_RegImmQ;
		break;

	case 0xF: /* F--- */
		switch(opw2&15)
		{
		case 0xC:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->rn|=(opw&0x0C)<<2;
			op->rm|=(opw&0x03)<<4;

			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGREG;
			op->Run=BTSH_Op_FMOVS_RR;

			switch((opw>>4)&15)
			{
			case 0x0:
				op->nmid=BTESH2_NMID_FMOVS;
				op->Run=BTSH_Op_FMOV_FRR;
				break;

			case 0x3:
				op->nmid=BTESH2_NMID_FMOVIS;
				op->Run=BTSH_Op_FMOV_GRFR;
				break;

			case 0x5:
				op->nmid=BTESH2_NMID_FMOVD;
				if(op->rn&0x10)op->rn^=0x30;
				if(op->rn&0x01)op->rn^=0x11;
				if(op->rm&0x10)op->rm^=0x30;
				if(op->rm&0x01)op->rm^=0x11;
				op->Run=BTSH_Op_FMOV_DRR;
				break;

			case 0x7:
				op->nmid=BTESH2_NMID_FMOVID;
				op->Run=BTSH_Op_FMOV_GRDR;
				break;

			case 0x8:
				op->nmid=BTESH2_NMID_MOVL;
				op->Run=BTSH_Op_MOV_RegReg;
				break;

			case 0xA:
				op->nmid=BTESH2_NMID_MOVQ;
				op->Run=BTSH_OpJQ_MOV_RegRegQ;
				break;

			case 0xC:
				op->nmid=BTESH2_NMID_FMOVSI;
				op->Run=BTSH_Op_FMOV_FRGR;
				break;
			case 0xD:
				op->nmid=BTESH2_NMID_FMOVDI;
				op->Run=BTSH_Op_FMOV_DRGR;
				break;

			case 0xF:
				if(op->rn&1)op->rn^=0x21;
				if(op->rm&1)op->rm^=0x21;
				op->nmid=BTESH2_NMID_FMOVX;
				op->Run=BTSH_Op_FMOV_XRR;
				break;
			}
			break;
		
		case 0xD:
			switch((opw2>>4)&15)
			{
			case 0xD: /* F-DD */
				op->imm=((opw2>>8)&15)|((opw&255)<<4);
				op->nmid=BTESH2_NMID_PSETMD12;
				op->fmid=BTESH2_FMID_IMM;
				op->Run=BTSH_Op_PSETMD12_Imm;
				break;
			default:
				break;
			}
			break;
		}
		break;

	default:
		break;
	}
	return(0);
}

int BTESH2_DecodeOpcode_BJX1(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	op->fl=BTESH2_OPFL_EXTRAWORD;

	switch(opw>>8)
	{
	case 0x8A:
		op->rn=0;
		op->imm=(((sbyte)opw)<<16)|(opw2&0xFFFF);
		op->fl=BTESH2_OPFL_EXTRAWORD;
		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;
		break;
	case 0x8E:
		BTESH2_DecodeOpcode_BJX1_8E(cpu, op, pc, opw, opw2);
		break;
	default:
		break;
	}
	return(0);
}

int BTESH2_DecodeOpcode_BJX1_8C(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	int i, j, k;

	op->fl=BTESH2_OPFL_EXTRAWORD;

//	switch(opw2>>12)
	switch((opw>>4)&15)
	{

	default:
		break;
	}
	return(0);
}


int BTESH2_DecodeOpcode_BJX1_CC(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	int i, j, k;

	op->fl=BTESH2_OPFL_EXTRAWORD;

//	switch(opw2>>12)
	switch((opw>>4)&15)
	{
	case 0x0:
		BTESH2_DecodeOpcode_BJX1_CC0(cpu, op, pc, opw, opw2);
		break;
	case 0x3:
		op->ro=(opw2>>8)&15;
		op->rn=(opw2>>4)&15;
		op->rm=(opw2>>0)&15;
//		if(opw&0x0004)	op->ro+=BTESH2_REG_RHI;
//		if(opw&0x0002)	op->rn+=BTESH2_REG_RHI;
//		if(opw&0x0001)	op->rm+=BTESH2_REG_RHI;

		if(opw&0x0004)	op->ro+=BTESH2_REG_R16;
		if(opw&0x0002)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0001)	op->rm+=BTESH2_REG_R16;

		op->fmid=BTESH2_FMID_REGREGREG;

		switch((opw2>>12)&15)
		{
		case 0x0:
			op->nmid=BTESH2_NMID_ADD;
			op->Run=BTSH_Op_ADD_RegRegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_ADDQ;
				op->Run=BTSH_Op_ADDQ_RegRegReg;		}
			break;
		case 0x1:
			op->nmid=BTESH2_NMID_SUB;
			op->Run=BTSH_Op_SUB_RegRegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_SUBQ;
				op->Run=BTSH_Op_SUBQ_RegRegReg;		}
			break;
		case 0x2:
			op->nmid=BTESH2_NMID_MULL;
			op->Run=BTSH_Op_MUL_RegRegReg;
			if(opw&0x0008)
			{	op->nmid=BTESH2_NMID_MULQ;
				op->Run=BTSH_Op_MULQ_RegRegReg;		}
			break;
		case 0x3:
			op->nmid=BTESH2_NMID_AND;
			op->Run=BTSH_Op_AND_RegRegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_ANDQ;
				op->Run=BTSH_Op_ANDQ_RegRegReg;		}
			break;
		case 0x4:
			op->nmid=BTESH2_NMID_OR;
			op->Run=BTSH_Op_OR_RegRegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_ORQ;
				op->Run=BTSH_Op_ORQ_RegRegReg;		}
			break;
		case 0x5:
			op->nmid=BTESH2_NMID_XOR;
			op->Run=BTSH_Op_XOR_RegRegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_XORQ;
				op->Run=BTSH_Op_XORQ_RegRegReg;		}
			break;

		case 0x6:
			op->nmid=BTESH2_NMID_SHLD;
			op->Run=BTSH_Op_SHLLL_RegRegReg;
			if(opw&0x0008)
			{	op->nmid=BTESH2_NMID_SHLDQ;
				op->Run=BTSH_Op_SHLLQ_RegRegReg;		}
			break;
		case 0x7:
			op->nmid=BTESH2_NMID_SHLR;
			op->Run=BTSH_Op_SHLRL_RegRegReg;
			if(opw&0x0008)
			{	op->nmid=BTESH2_NMID_SHLRQ;
				op->Run=BTSH_Op_SHLRQ_RegRegReg;		}
			break;
		case 0x8:
			op->nmid=BTESH2_NMID_SHAD;
			op->Run=BTSH_Op_SHALL_RegRegReg;
			if(opw&0x0008)
			{
//				op->nmid=BTESH2_NMID_SHADQ;
				op->Run=BTSH_Op_SHALQ_RegRegReg;		}
			break;
		case 0x9:
			op->nmid=BTESH2_NMID_SHAR;
			op->Run=BTSH_Op_SHARL_RegRegReg;
			if(opw&0x0008)
			{	op->nmid=BTESH2_NMID_SHARQ;
				op->Run=BTSH_Op_SHARQ_RegRegReg;		}
			break;

		default:
			break;
		}
		break;

#if 0
	case 0x7:
		op->rn=opw&15;
//		op->imm=(s16)opw2;
		op->imm=opw2;
		op->nmid=BTESH2_NMID_LDSH16;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_LDSH16_RegImm;
		break;
#endif

	default:
		break;
	}
	return(0);
}

int BTESH2_DecodeOpcode_BJX1_CD(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	int i, j, k;

	switch(opw2>>12)
	{
	default:
		break;
	}
	return(0);
}

int BTESH2_DecodeOpcode_BJX1_CF(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	int i, j, k;

	switch(opw2>>12)
	{
	default:
		break;
	}
	return(0);
}
