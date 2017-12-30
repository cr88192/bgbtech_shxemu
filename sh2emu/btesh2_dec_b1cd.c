int BTESH2_DecodeOpcode_BJX1_CE(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	int i, j, k;

	op->fl=BTESH2_OPFL_EXTRAWORD;

	switch(opw2>>12)
	{
	case 0x0: /* 0--- */
		switch(opw2&15)
		{
		case 0x4: /* 0--4 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispB;
			break;
		case 0x5: /* 0--5 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispW;
			break;
		case 0x6: /* 0--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
			break;
		case 0x7: /* 0--7 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispQ;
			break;

		case 0xC: /* 0--C */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispB;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_MOVUB;
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispUB;
			}
			break;
		case 0xD: /* 0--D */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispW;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_MOVUW;
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispUW;
			}
			break;
		case 0xE: /* 0--E */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_MOVUL;
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispUD;
			}
			break;
		case 0xF: /* 0--F */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=opw&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;
			break;
		}
		break;

	case 0x1: /* 1--- */
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=(opw2   )&15;
		op->imm=(opw&15)*4;
		if(opw&0x0040)op->rn+=BTESH2_REG_R16;
		if(opw&0x0020)op->rm+=BTESH2_REG_R16;
		if(opw&0x0010)op->ro+=BTESH2_REG_R16;
		if(!op->ro)break;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGSTRODISP;
		op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
		if(opw&0x0080)
		{
			op->imm*=2;
			op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispQ;
		}
		break;

	case 0x2: /* 2--- */
		switch(opw2&15)
		{
		case 0x0: /* 2--0 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=opw&0x3F;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispB;
			break;
		case 0x1: /* 2--1 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*2;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispW;
			break;
		case 0x2: /* 2--2 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*4;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispD;
			break;
		case 0x3: /* 2--3 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*8;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispQ;
			break;

		case 0x4: /* 2--4 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*4;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_FMOVS_RegStDisp;
			break;

		case 0x6: /* 2--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*4;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_FMOV_RegLdDisp;
			break;

		case 0x9:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&63);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ANDQ_RegImmReg;
			break;
		case 0xA:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&63);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_XORQ_RegImmReg;
			break;
		case 0xB:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&63);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ORQ_RegImmReg;
			break;

		case 0xE:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&63);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_MULQ_RegImmReg;
			break;
		case 0xF:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(~63LL)|(opw&63);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_MULQ_RegImmReg;
			break;
		}
		break;


	case 0x3:	/* 3--- */
		switch(opw2&15)
		{
		case 0x0:	/* 3--0 */
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<22))>>22;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPEQ_RegImm;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_CMPQEQ;
				op->Run=BTSH_Op_CMPEQ_RegImmQ;
			}
			break;

		case 0x2:	/* 3--2 */
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<22))>>22;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPHS;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPHS_RegImm;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_CMPQHS;
				op->Run=BTSH_Op_CMPHS_RegImmQ;
			}
			break;
		case 0x3:	/* 3--3 */
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<22))>>22;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPGE;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPGE_RegImm;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_CMPQGE;
				op->Run=BTSH_Op_CMPGE_RegImmQ;
			}
			break;

		case 0x6:	/* 3--6 */
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<22))>>22;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPHI;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPHI_RegImm;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_CMPQHI;
				op->Run=BTSH_Op_CMPHI_RegImmQ;
			}
			break;
		case 0x7:	/* 3--7 */
			i=(((byte)opw)<<4)|((opw2>>4)&15);
			i=((s32)(i<<22))>>22;
			op->rn=(opw2>>8)&15;
			op->imm=i;
			op->nmid=BTESH2_NMID_CMPGT;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPGT_RegImm;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0080)
			{
				op->nmid=BTESH2_NMID_CMPQGT;
				op->Run=BTSH_Op_CMPGT_RegImmQ;
			}
			break;

		case 0x8:	/* 3--8 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_SUB;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SUBQ_RegImmReg;
			break;
		case 0x9:	/* 3--9 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)|(~63);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ANDQ_RegImmReg;
			break;

		case 0xC:	/* 3--C */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_ADD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ADDQ_RegImmReg;
			break;

		default:
			break;
		}
		break;

	case 0x4: /* 4--- */
		switch(opw2&15)
		{
		case 0xC: /* 4--C */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((((s32)opw)<<26)>>26);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_SHAD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SHADL_RegImmReg;
			break;
		case 0xD: /* 4--D */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((((s32)opw)<<26)>>26);
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_SHLD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SHLDL_RegImmReg;
			break;
		}
		break;

	case 0x5: /* 5--- */
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=(opw2   )&15;
		op->imm=(opw&15)*4;
		if(opw&0x0040)op->rn+=BTESH2_REG_R16;
		if(opw&0x0020)op->rm+=BTESH2_REG_R16;
		if(opw&0x0010)op->ro+=BTESH2_REG_R16;
		if(!op->ro)break;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGLDRODISP;
		op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
		if(opw&0x0080)
		{
			op->imm*=2;
			op->nmid=BTESH2_NMID_MOVQ;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;
		}
		break;

	case 0x6: /* 6--- */
		switch(opw2&15)
		{
		case 0x0: /* 6--0 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*1;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispB;
			break;
		case 0x1: /* 6--1 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*2;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispW;
			break;
		case 0x2: /* 6--2 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*4;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispD;
			break;
		case 0x3: /* 6--3 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*8;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
			break;
		case 0x4: /* 6--4 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw    )&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispB;
			break;
		case 0x5: /* 6--5 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw    )&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispW;
			break;
		case 0x6: /* 6--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw    )&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
			break;
		case 0x7: /* 6--7 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw    )&15;
			op->imm=0;
			if(opw&0x0040)op->rn+=BTESH2_REG_R16;
			if(opw&0x0020)op->rm+=BTESH2_REG_R16;
			if(opw&0x0010)op->ro+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;
			break;
		case 0x8: /* 6--8 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*1;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0x9: /* 6--9 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*2;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xA: /* 6--A */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*4;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xB: /* 6--B */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*8;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xC: /* 6--C */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*1;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVUB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOVU_RegLdDispB;
			break;
		case 0xD: /* 6--D */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*2;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVUW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOVU_RegLdDispW;
			break;
		case 0xE: /* 6--E */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(opw&0x3F)*4;
			if(opw&0x0080)op->rn+=BTESH2_REG_R16;
			if(opw&0x0040)op->rm+=BTESH2_REG_R16;
			op->nmid=BTESH2_NMID_MOVUL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOVU_RegLdDispD;
			break;
		case 0xF: /* 6--F */
			break;
		default:
			break;
		}
		break;
	case 0x7: /* 7--- */
//		i=((byte)opw)|(((byte)opw2)<<8);
		i=((opw&0x3F)<<8)|((byte)opw2);
		i=((s16)(i<<2))>>2;
		op->rn=(opw2>>8)&15;
		op->imm=(u64)((s16)i);
		if(opw&0x0080)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0040)	break;

		op->nmid=BTESH2_NMID_ADD;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_ADDQ_RegImm;
		break;

	case 0x9: /* 9--- */
		break;
	case 0xA: /* A--- */
		break;
	case 0xB: /* B--- */
		break;

	case 0xD: /* D--- */
		break;
	case 0xE: /* E--- */
		i=((opw&0x3F)<<8)|((byte)opw2);
		i=((s16)(i<<2))>>2;
		op->rn=(opw2>>8)&15;
		op->imm=(u64)((s16)i);

		if(opw&0x0080)	op->rn+=BTESH2_REG_R16;
		if(opw&0x0040)	break;

		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_OpJQ_MOV_RegImmQ;
		break;

	default:
		break;
	}
	return(0);
}

