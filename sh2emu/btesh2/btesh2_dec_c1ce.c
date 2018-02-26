int BTESH2_DecodeOpcode_B64C_CE(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2)
{
	byte rm_xe, rn_xe, ro_xe, ro2_xe;
	byte rm_xf, rn_xf, rn2_xe;
	byte xe, xe_q, xe_o;
	int imm6, imm6sx, imm6zx, imm6nx;
	int imm4_8e, imm4_xe, imm10s_mxe, imm10s_oxe;
	int imm16s;
	int i, j, k;

	xe=((opw&0xFF00)==0xCE00);

	op->fl=BTESH2_OPFL_EXTRAWORD;
	op->Run=NULL;

	i=(((byte)opw)<<8)|((byte)opw2);
	imm16s=(s16)i;

	if(xe)
	{
		xe_q=(opw&0x0080)!=0;
		xe_o=(opw&0x0010)!=0;
	
		rn_xe=(opw2>>8)&15;
		rm_xe=(opw2>>4)&15;
		ro_xe=(opw2>>0)&15;
		ro2_xe=opw&15;
		if(opw&0x0040)rn_xe+=BTESH2_REG_R16;
		if(opw&0x0020)rm_xe+=BTESH2_REG_R16;
		if(opw&0x0010)ro_xe+=BTESH2_REG_R16;
		if(opw&0x0010)ro2_xe+=BTESH2_REG_R16;

		rn_xf=(opw2>>8)&15;
		rm_xf=(opw2>>4)&15;
		if(opw&0x0080)rn_xf+=BTESH2_REG_R16;
		if(opw&0x0040)rm_xf+=BTESH2_REG_R16;

		rn2_xe=(opw2>>8)&15;
		rn2_xe+=BTESH2_REG_R16;

		imm6=(opw&0x3F);
		imm6zx=(opw&0x3F);
		imm6sx=((s32)(imm6<<26))>>26;
		imm6nx=imm6|(~63);
		imm4_8e=0;
		imm4_xe=opw&15;

		i=(((byte)opw)<<4)|((opw2>>4)&15);
		i=((s32)(i<<22))>>22;
		imm10s_mxe=i;

		i=(((byte)opw)<<4)|((opw2>>0)&15);
		i=((s32)(i<<22))>>22;
		imm10s_oxe=i;
	}else
	{
		xe_q=0;
		xe_o=0;
		rn_xe=(opw2>>8)&15;
		rm_xe=(opw2>>4)&15;
		ro_xe=(opw2>>0)&15;
//		ro2_xe=opw&15;

		rn_xf=(opw2>>8)&15;
		rm_xf=(opw2>>4)&15;

		rn2_xe=(opw2>>8)&15;

		imm6=(sbyte)(opw&0xFF);
		imm6zx=(opw&0xFF);
		imm6sx=((s32)(imm6<<24))>>24;
		imm6nx=imm6|(~255);

//		ro2_xe=opw&15;
//		imm4_8e=(opw>>4)&15;

		ro2_xe=(opw>>4)&15;
		imm4_8e=opw&15;
		imm4_xe=(sbyte)(opw&255);
		
		i=(((byte)opw)<<4)|((opw2>>4)&15);
		i=((s32)(i<<20))>>20;
		imm10s_mxe=i;

		i=(((byte)opw)<<4)|((opw2>>0)&15);
		i=((s32)(i<<20))>>20;
		imm10s_oxe=i;
	}

	switch(opw2>>12)
	{
	case 0x0: /* 0--- */
		switch(opw2&15)
		{
		case 0x4: /* 0--4 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*1;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispB;
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGSTR0N;
				op->Run=BTSH_OpJQ_MOV_RegStR0nB;
				break;
			}
			break;
		case 0x5: /* 0--5 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*2;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispW;
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGSTR0N;
				op->Run=BTSH_OpJQ_MOV_RegStR0nW;
				break;
			}
			break;
		case 0x6: /* 0--6 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*4;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGSTR0N;
				op->Run=BTSH_OpJQ_MOV_RegStR0nD;
				break;
			}
			break;
		case 0x7: /* 0--7 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*8;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_OpJQ_MOV_RegStRoDispQ;
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGSTR0N;
				op->Run=BTSH_OpJQ_MOV_RegStR0nQ;
				break;
			}
			break;

		case 0xC: /* 0--C */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*1;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispB;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_MOVUB;
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispUB;
			}
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGLDR0M;
				op->Run=BTSH_OpJQ_MOV_RegLdR0mB;
				break;
			}
			break;
		case 0xD: /* 0--D */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*2;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispW;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_MOVUW;
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispUW;
			}
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGLDR0M;
				op->Run=BTSH_OpJQ_MOV_RegLdR0mW;
				break;
			}
			break;
		case 0xE: /* 0--E */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*4;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_MOVUL;
				op->Run=BTSH_OpJQ_MOV_RegLdRoDispUD;
			}
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGLDR0M;
				op->Run=BTSH_OpJQ_MOV_RegLdR0mD;
				break;
			}
			break;
		case 0xF: /* 0--F */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*8;
//			if(!op->ro)break;
			if(!op->ro && op->imm) break;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_MOV_RegLdRoDispQ;
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGLDR0M;
				op->Run=BTSH_OpJQ_MOV_RegLdR0mQ;
				break;
			}
			break;
		}
		break;

	case 0x1: /* 1--- */
		op->rn=rn_xe;
		op->rm=rm_xe;
		op->ro=ro_xe;
		op->imm=imm4_xe*4;
		if(!op->ro)break;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGSTRODISP;
		op->Run=BTSH_OpJQ_MOV_RegStRoDispD;
		if(xe_q)
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
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispB;
			break;
		case 0x1: /* 2--1 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispW;
			break;
		case 0x2: /* 2--2 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispD;
			break;
		case 0x3: /* 2--3 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*8;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_OpJQ_MOV_RegStDispQ;
			break;

#if 0
		case 0x4: /* 2--4 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_FMOVS_RegStDisp;
			break;

		case 0x6: /* 2--6 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_FMOV_RegLdDisp;
			break;
#endif

		case 0x9: /* 2--9 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6zx;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ANDQ_RegImmReg;
			break;
		case 0xA: /* 2--A */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6zx;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_XORQ_RegImmReg;
			break;
		case 0xB: /* 2--B */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6zx;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ORQ_RegImmReg;
			break;

		case 0xE: /* 2--E */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6zx;
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_MULQ_RegImmReg;
			break;
		case 0xF: /* 2--F */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6nx;
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
			op->rn=rn_xe;
			op->imm=imm10s_mxe;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPEQ_RegImm;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_CMPQEQ;
				op->Run=BTSH_Op_CMPEQ_RegImmQ;
			}
			break;

		case 0x2:	/* 3--2 */
			op->rn=rn_xe;
			op->imm=imm10s_mxe;
			op->nmid=BTESH2_NMID_CMPHS;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPHS_RegImm;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_CMPQHS;
				op->Run=BTSH_Op_CMPHS_RegImmQ;
			}
			break;
		case 0x3:	/* 3--3 */
			op->rn=rn_xe;
			op->imm=imm10s_mxe;
			op->nmid=BTESH2_NMID_CMPGE;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPGE_RegImm;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_CMPQGE;
				op->Run=BTSH_Op_CMPGE_RegImmQ;
			}
			break;

		case 0x6:	/* 3--6 */
			op->rn=rn_xe;
			op->imm=imm10s_mxe;
			op->nmid=BTESH2_NMID_CMPHI;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPHI_RegImm;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_CMPQHI;
				op->Run=BTSH_Op_CMPHI_RegImmQ;
			}
			break;
		case 0x7:	/* 3--7 */
			op->rn=rn_xe;
			op->imm=imm10s_mxe;
			op->nmid=BTESH2_NMID_CMPGT;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPGT_RegImm;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_CMPQGT;
				op->Run=BTSH_Op_CMPGT_RegImmQ;
			}
			break;

		case 0x8:	/* 3--8 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6zx;
			op->nmid=BTESH2_NMID_SUB;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SUBQ_RegImmReg;
			break;
		case 0x9:	/* 3--9 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6nx;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ANDQ_RegImmReg;
			break;

		case 0xC:	/* 3--C */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6zx;
			op->nmid=BTESH2_NMID_ADD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_ADDQ_RegImmReg;
			break;

		case 0xF: /* 3--F */
			switch(opw&15)
			{
			case 0x0: /* e0-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_CMPEQ;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_CMPEQ_RegReg;
				if(xe_q)
				{	op->nmid=BTESH2_NMID_CMPQEQ;
					op->Run=BTSH_Op_CMPEQ_RegRegQ;	}
				break;

			case 0x2: /* e2-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_CMPHS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_CMPHS_RegReg;
				if(xe_q)
				{	op->nmid=BTESH2_NMID_CMPQHS;
					op->Run=BTSH_Op_CMPHS_RegRegQ;	}
				break;
			case 0x3: /* e3-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_CMPGE;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_CMPGE_RegReg;
				if(xe_q)
				{	op->nmid=BTESH2_NMID_CMPQGE;
					op->Run=BTSH_Op_CMPGE_RegRegQ;	}
				break;

			case 0x5: /* e5-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_DMULU;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_DMULU_RegReg;
				break;
			case 0x6: /* e6-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_CMPHI;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_CMPHI_RegReg;
				if(xe_q)
				{	op->nmid=BTESH2_NMID_CMPQHI;
					op->Run=BTSH_Op_CMPHI_RegRegQ;	}
				break;
			case 0x7: /* e7-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_CMPGT;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_CMPGT_RegReg;
				if(xe_q)
				{	op->nmid=BTESH2_NMID_CMPQGT;
					op->Run=BTSH_Op_CMPGT_RegRegQ;	}
				break;
			case 0x8: /* e8-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_SUB;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_SUBQ_RegReg;
				break;

			case 0xC: /* eC-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_ADD;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_ADDQ_RegReg;
				break;
			case 0xD: /* eD-3zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_DMULS;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_DMULS_RegReg;
				break;
			}
			break;

		default:
			break;
		}
		break;

	case 0x4: /* 4--- */
		switch(opw2&15)
		{
		case 0xC: /* 4--C */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6sx;
			op->nmid=BTESH2_NMID_SHAD;
			op->fmid=BTESH2_FMID_REGIMMREG;
				op->Run=BTSH_Op_SHADL_RegImmReg;
			if(((imm6sx>>6)^(imm6sx>>7))&1)
			{
				op->imm=((((s32)imm6sx)>>1)&(~63))|(imm6sx&63);
				op->nmid=BTESH2_NMID_SHADQ;
				op->Run=BTSH_Op_SHADQ_RegImmReg;
				break;
			}
			break;
		case 0xD: /* 4--D */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6sx;
			op->nmid=BTESH2_NMID_SHLD;
			op->fmid=BTESH2_FMID_REGIMMREG;
			op->Run=BTSH_Op_SHLDL_RegImmReg;
			if(((imm6sx>>6)^(imm6sx>>7))&1)
			{
				op->imm=((((s32)imm6sx)>>1)&(~63))|(imm6sx&63);
				op->nmid=BTESH2_NMID_SHLDQ;
				op->Run=BTSH_Op_SHLDQ_RegImmReg;
				break;
			}
			break;

		case 0xF: /* 4--F */
			switch(opw&15)
			{
			case 0x0: /* e0-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOV;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_OpJQ_MOV_RegRegQ;
//				BTSH_FixupJQ_MOV_RegReg(cpu, op);
				break;

			case 0x7: /* e7-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_NOT;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_NOTQ_RegReg;
				break;
			case 0x8: /* e8-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_TST;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_TST_RegReg;
				if(xe_q)
				{
					op->nmid=BTESH2_NMID_TSTQ;
					op->Run=BTSH_Op_TSTQ_RegReg;
				}
				break;
			case 0x9: /* e9-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_AND;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_ANDQ_RegReg;
				break;
			case 0xA: /* eA-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_XOR;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_XORQ_RegReg;
				break;
			case 0xB: /* eB-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_OR;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_ORQ_RegReg;
				break;

			case 0xC: /* eC-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_SHAD;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_SHAD_RegReg;
				op->Run=BTSH_Op_SHADL_RegReg;
				if(xe_q)
				{
					op->nmid=BTESH2_NMID_SHADQ;
					op->Run=BTSH_Op_SHADQ_RegReg;
				}
				break;
			case 0xD: /* eD-4zzF */
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_SHLD;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_SHLDL_RegReg;
				if(xe_q)
				{
					op->nmid=BTESH2_NMID_SHLDQ;
					op->Run=BTSH_Op_SHLDQ_RegReg;
				}
				break;
			}
			break;
		}
		break;

	case 0x5: /* 5--- */
		op->rn=rn_xe;
		op->rm=rm_xe;
		op->ro=ro_xe;
		op->imm=imm4_xe*4;
		if(!op->ro)break;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGLDRODISP;
		op->Run=BTSH_OpJQ_MOV_RegLdRoDispD;
		if(xe_q)
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
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*1;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispB;
			break;
		case 0x1: /* 6--1 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispW;
			break;
		case 0x2: /* 6--2 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispD;
			break;
		case 0x3: /* 6--3 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*8;
			op->nmid=BTESH2_NMID_MOVQ;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOV_RegLdDispQ;
			break;
		case 0x4: /* 6--4 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*1;
//			if(!op->ro)break;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_LEA_RegLdRoDispB;
			break;
		case 0x5: /* 6--5 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*2;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_LEA_RegLdRoDispW;
			break;
		case 0x6: /* 6--6 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*4;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_LEA_RegLdRoDispD;
			break;
		case 0x7: /* 6--7 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->ro=ro2_xe;
			op->imm=imm4_8e*8;
			if(!op->ro)break;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_OpJQ_LEA_RegLdRoDispQ;
			break;
		case 0x8: /* 6--8 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*1;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0x9: /* 6--9 */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*2;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xA: /* 6--A */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*4;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xB: /* 6--B */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*8;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_LEA_RegLdDisp;
			break;
		case 0xC: /* 6--C */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*1;
			op->nmid=BTESH2_NMID_MOVUB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOVU_RegLdDispB;
			break;
		case 0xD: /* 6--D */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*2;
			op->nmid=BTESH2_NMID_MOVUW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOVU_RegLdDispW;
			break;
		case 0xE: /* 6--E */
			op->rn=rn_xf;
			op->rm=rm_xf;
			op->imm=imm6*4;
			op->nmid=BTESH2_NMID_MOVUL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_OpJQ_MOVU_RegLdDispD;
			break;
		case 0xF: /* 6--F */
			switch(opw&15)
			{

			case 0x0: /* e0-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_OpJQ_MOV_RegDecStB;
				break;
			case 0x1: /* e1-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_OpJQ_MOV_RegDecStW;
				break;
			case 0x2: /* e2-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_OpJQ_MOV_RegDecStD;
				break;
			case 0x3: /* e3-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGDECST;
				op->Run=BTSH_OpJQ_MOV_RegDecStQ;
				break;

			case 0x4: /* e4-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVB;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_OpJQ_MOV_RegIncLdB;
				if(xe_o)
				{
					op->nmid=BTESH2_NMID_MOVUB;
//					op->Run=BTSH_OpJQ_MOV_RegIncLdUB;
				}
				break;
			case 0x5: /* e5-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVW;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_OpJQ_MOV_RegIncLdW;
				if(xe_o)
				{
					op->nmid=BTESH2_NMID_MOVUW;
//					op->Run=BTSH_OpJQ_MOV_RegIncLdUW;
				}
				break;
			case 0x6: /* e6-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVL;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_OpJQ_MOV_RegIncLdD;
				if(xe_o)
				{
					op->nmid=BTESH2_NMID_MOVUL;
//					op->Run=BTSH_OpJQ_MOV_RegIncLdUD;
				}
				break;
			case 0x7: /* e7-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_MOVQ;
				op->fmid=BTESH2_FMID_REGINCLD;
				op->Run=BTSH_OpJQ_MOV_RegIncLdQ;
				break;

			case 0x8: /* e8-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_SWAPB;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_SWAP_RegRegB;
				break;
			case 0x9: /* e9-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_SWAPW;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_SWAP_RegRegW;
				break;
			case 0xA: /* eA-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_NEGC;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_NEGC_RegReg;
				break;
			case 0xB: /* eB-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_NEG;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_Op_NEGQ_RegReg;
				break;
			case 0xC: /* eC-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_EXTUB;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_OpJQ_EXTUB_RegReg;
				break;
			case 0xD: /* eD-6zzF */
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_EXTUW;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_OpJQ_EXTUW_RegReg;
				if(xe_q)
				{
					op->nmid=BTESH2_NMID_EXTUL;
					op->Run=BTSH_Op_EXTUL_RegReg;
				}
				break;
			case 0xE: /* eE-6zzF */
				if(xe_q)	break;
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_EXTSB;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_OpJQ_EXTSB_RegReg;
				break;
			case 0xF: /* eF-6zzF */
				if(xe_o)	break;
				op->rn=rn_xe;
				op->rm=rm_xe;
				op->nmid=BTESH2_NMID_EXTSW;
				op->fmid=BTESH2_FMID_REGREG;
				op->Run=BTSH_OpJQ_EXTSW_RegReg;
				if(xe_q)
				{
					op->nmid=BTESH2_NMID_EXTSL;
					op->Run=BTSH_Op_EXTSL_RegReg;
				}
				break;

			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case 0x7: /* 7--- */
		op->rn=rn2_xe;
		op->imm=imm16s;
		op->nmid=BTESH2_NMID_ADD;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_ADDQ_RegImm;
		break;

	case 0x8: /* 8--- */
		switch((opw2>>8)&15)
		{

		case 0x2: /* 82--, BRA/N label */
			op->imm=pc+2*imm16s+6;
			op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BRAN;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BRA_Abs;
			break;
		case 0x3: /* 83--, BSR/N label */
			op->imm=pc+2*imm16s+6;
			op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BSRN;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BSR_Abs;
			break;

#if 0
		case 0x8:
			op->rn=0;
			op->imm=imm16s;
			op->nmid=BTESH2_NMID_CMPEQ;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_CMPEQ_RegImm;
			break;
#endif

		case 0x9:
			op->imm=pc+2*imm16s+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BT;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BT_Abs;
			break;
		case 0xA:
			op->imm=imm16s;
			op->nmid=BTESH2_NMID_LDHF16;
			op->fmid=BTESH2_FMID_IMM;
			op->Run=BTSH_Op_LDHF16_Imm;
			break;
		case 0xB:
			op->imm=pc+2*imm16s+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BF;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BF_Abs;
			break;
		case 0xC:
			op->imm=imm16s;
			op->nmid=BTESH2_NMID_LDIF16;
			op->fmid=BTESH2_FMID_IMM;
			op->Run=BTSH_Op_LDIF16_Imm;
			break;
		case 0xD:
			op->imm=pc+2*imm16s+6;
			op->fl|=BTESH2_OPFL_CTRLF|
				BTESH2_OPFL_INVDLYSLOT;
			op->nmid=BTESH2_NMID_BTS;
			op->fmid=BTESH2_FMID_ABS;
			op->Run=BTSH_Op_BTS_Abs;
			break;
		case 0xE:
//			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
//				op, pc, opw, opw2);
			break;
		case 0xF:
			op->imm=pc+2*imm16s+6;
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

	case 0x9: /* 9--- */
		op->rn=rn2_xe;
		op->imm=imm16s;
		op->nmid=BTESH2_NMID_LDSH16;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_LDSH16_RegImm;
		break;

	case 0xA: /* A--- */
		switch(opw2&15)
		{
		case 0x0: /* A--0 */
			if(xe_q)	break;
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_ADD;
			op->fmid=BTESH2_FMID_REGREGREG;
			op->Run=BTSH_Op_ADDQ_RegRegReg;
			break;
		case 0x1: /* A--1 */
			if(xe_q)	break;
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_SUB;
			op->fmid=BTESH2_FMID_REGREGREG;
			op->Run=BTSH_Op_SUBQ_RegRegReg;
			break;
		case 0x2: /* A--2 */
//			if(xe_q)	break;
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_MULL;
			op->fmid=BTESH2_FMID_REGREGREG;
			op->Run=BTSH_Op_MUL_RegRegReg;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_MULQ;
				op->Run=BTSH_Op_MULQ_RegRegReg;
				break;
			}
			break;
		case 0x3: /* A--3 */
			if(xe_q)	break;
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGREGREG;
			op->Run=BTSH_Op_ANDQ_RegRegReg;
			break;
		case 0x4: /* A--4 */
			if(xe_q)	break;
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGREGREG;
			op->Run=BTSH_Op_ORQ_RegRegReg;
			break;
		case 0x5: /* A--5 */
			if(xe_q)	break;
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGREGREG;
			op->Run=BTSH_Op_XORQ_RegRegReg;
			break;

		case 0x6: /* A--6 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->fmid=BTESH2_FMID_REGREGREG;
			if(xe_q)
			{	op->nmid=BTESH2_NMID_SHLDQ;
				op->Run=BTSH_Op_SHLLQ_RegRegReg;	}
			else
			{	op->nmid=BTESH2_NMID_SHLD;
				op->Run=BTSH_Op_SHLLL_RegRegReg;	}
			break;
		case 0x7: /* A--7 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->fmid=BTESH2_FMID_REGREGREG;
			if(xe_q)
			{	op->nmid=BTESH2_NMID_SHLRQ;
				op->Run=BTSH_Op_SHLRQ_RegRegReg;	}
			else
			{	op->nmid=BTESH2_NMID_SHLR;
				op->Run=BTSH_Op_SHLRL_RegRegReg;	}
			break;

		case 0x8: /* A--8 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->fmid=BTESH2_FMID_REGREGREG;
			if(xe_q)
			{	op->nmid=BTESH2_NMID_SHADQ;
				op->Run=BTSH_Op_SHALQ_RegRegReg;	}
			else
			{	op->nmid=BTESH2_NMID_SHAD;
				op->Run=BTSH_Op_SHALL_RegRegReg;	}
			break;
		case 0x9: /* A--9 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->fmid=BTESH2_FMID_REGREGREG;
			if(xe_q)
			{	op->nmid=BTESH2_NMID_SHARQ;
				op->Run=BTSH_Op_SHARQ_RegRegReg;	}
			else
			{	op->nmid=BTESH2_NMID_SHAR;
				op->Run=BTSH_Op_SHARL_RegRegReg;	}
			break;

		case 0xA: /* A--A */
			if(xe_q)	break;
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_CSELT;
			op->fmid=BTESH2_FMID_REGREGREG;
			op->Run=BTSH_Op_CSELT_RegRegReg;
			break;
		}
		break;
	case 0xB: /* B--- */
		break;

	case 0xC: /* C--- */
		switch((opw2>>8)&15)
		{
#if 1
		case 0x8:
			if(xe)break;
			op->rn=rm_xf;
			op->imm=imm10s_oxe;
			op->nmid=BTESH2_NMID_TST;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_TST_RegImm;
			if(xe_q)
			{
				op->nmid=BTESH2_NMID_TSTQ;
				op->Run=BTSH_Op_TSTQ_RegImm;
			}
			break;
		case 0x9:
			if(xe)break;
			op->rn=rm_xf;
			op->imm=imm10s_oxe;
			op->nmid=BTESH2_NMID_AND;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_ANDQ_RegImm;
			break;
		case 0xA:
			if(xe)break;
			op->rn=rm_xf;
			op->imm=imm10s_oxe;
			op->nmid=BTESH2_NMID_XOR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_XORQ_RegImm;
			break;
		case 0xB:
			if(xe)break;
			op->rn=rm_xf;
			op->imm=imm10s_oxe;
			op->nmid=BTESH2_NMID_OR;
			op->fmid=BTESH2_FMID_REGIMM;
			op->Run=BTSH_Op_ORQ_RegImm;
			break;
#endif

		default:
			break;
		}
		break;

	case 0xD: /* D--- */
		break;
	case 0xE: /* E--- */
		op->rn=rn2_xe;
		op->imm=imm16s;
		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_OpJQ_MOV_RegImmQ;
		break;

	case 0xF: /* F--- */
		switch(opw2&15)
		{
		case 0x0: /* F--0 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_FADD;
			op->fmid=BTESH2_FMID_REGREGREG;
//			op->Run=BTSH_Op_FADD_RRR;
			BTSH_Op_SetFADD_RRR(cpu, op, xe_q);
			break;
		case 0x1: /* F--1 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_FSUB;
			op->fmid=BTESH2_FMID_REGREGREG;
//			op->Run=BTSH_Op_FSUB_RRR;
			BTSH_Op_SetFSUB_RRR(cpu, op, xe_q);
			break;
		case 0x2: /* F--2 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_FMUL;
			op->fmid=BTESH2_FMID_REGREGREG;
//			op->Run=BTSH_Op_FMUL_RRR;
			BTSH_Op_SetFMUL_RRR(cpu, op, xe_q);
			break;
		case 0x3: /* F--3 */
			op->ro=rn_xe;	op->rn=rm_xe;	op->rm=ro2_xe;
			op->nmid=BTESH2_NMID_FDIV;
			op->fmid=BTESH2_FMID_REGREGREG;
//			op->Run=BTSH_Op_FDIV_RRR;
			BTSH_Op_SetFDIV_RRR(cpu, op, xe_q);
			break;

		case 0x6: /* F--6 */
			op->rn=rn_xe;	op->rm=rm_xe;	op->ro=ro2_xe;
			op->imm=0;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_FMOV_RegLdRoDisp;
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGLDR0M;
				op->Run=BTSH_Op_FMOV_RegLdR0m;
				break;
			}
			break;
		case 0x7: /* F--7 */
			op->rn=rn_xe;	op->rm=rm_xe;	op->ro=ro2_xe;
			op->imm=0;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_FMOV_RegStRoDisp;
			if(!op->ro && !op->imm)
			{
				op->fmid=BTESH2_FMID_REGSTR0N;
				op->Run=BTSH_Op_FMOV_RegStR0n;
				break;
			}
			break;

		case 0x8: /* F--8 */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->imm=imm4_xe*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_FMOV_RegLdDisp;
			break;

		case 0xA: /* F--A */
			op->rn=rn_xe;
			op->rm=rm_xe;
			op->imm=imm4_xe*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_FMOVS_RegStDisp;
			break;
		}
		break;

	default:
		break;
	}
	
	if(!op->Run)
		{ __debugbreak(); }
	
	return(0);
}

