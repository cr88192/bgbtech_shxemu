int BTESH2_DecodeOpcode_BJX1_Only64(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	u32 pc, u16 opw, u16 opw2)
{
}

int BTESH2_DecodeOpcode_BJX1_8E(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	u32 pc, u16 opw, u16 opw2)
{
	int i, j, k;

	op->fl=BTESH2_OPFL_EXTRAWORD;

	switch(opw2>>12)
	{
	case 0x0:
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
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_MOV_RegStRoDispB;
			break;
		case 0x5: /* 0--5 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_MOV_RegStRoDispW;
			break;
		case 0x6: /* 0--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTRODISP;
			op->Run=BTSH_Op_MOV_RegStRoDispD;
			break;
		case 0x7:
			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
				op, pc, opw, opw2);
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
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_MOV_RegLdRoDispB;
			break;
		case 0xD: /* 0--D */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_MOV_RegLdRoDispW;
			break;
		case 0xE: /* 0--E */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_MOV_RegLdRoDispD;
			break;
		case 0xF:
			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
				op, pc, opw, opw2);
			break;
		default:
			break;
		}
		break;
	case 0x1:
		op->rn=(opw2>>8)&15;
		op->rm=(opw2>>4)&15;
		op->ro=(opw2   )&15;
		op->imm=((sbyte)opw)*4;
		op->nmid=BTESH2_NMID_MOVL;
		op->fmid=BTESH2_FMID_REGSTRODISP;
		op->Run=BTSH_Op_MOV_RegStRoDispD;
		break;
	case 0x2:
		switch(opw2&15)
		{
		case 0x0:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=(sbyte)opw;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispB;
			break;
		case 0x1:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispW;
			break;
		case 0x2:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispD;
			break;
		case 0x3:
			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
				op, pc, opw, opw2);
			break;
		case 0x4:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_FMOV_RegLdDisp;
			break;
		case 0x5:
			break;
		case 0x6:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_FMOV_RegStDisp;
			break;

		default:
			break;
		}
		break;

	case 0x3:
		switch(opw2&15)
		{
		default:
			break;
		}
		break;
	case 0x4:
		switch(opw2&15)
		{
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
			break;
		case 0x1:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispW;
			break;
		case 0x2:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispD;
			break;
		case 0x3:
			BTESH2_DecodeOpcode_BJX1_Only64(cpu,
				op, pc, opw, opw2);
			break;
		case 0x4: /* 6--4 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*1;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispB;
			break;
		case 0x5: /* 6--5 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*2;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispW;
			break;
		case 0x6: /* 6--6 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*4;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispD;
			break;
		case 0x7: /* 6--7 */
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->ro=(opw>>4)&15;
			op->imm=(opw&15)*8;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDRODISP;
			op->Run=BTSH_Op_LEA_RegLdRoDispQ;
			break;
		case 0x8:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*1;
			op->nmid=BTESH2_NMID_LEAB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			break;
		case 0x9:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_LEAW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			break;
		case 0xA:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_LEAL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			break;
		case 0xB:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*8;
			op->nmid=BTESH2_NMID_LEAQ;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_LEA_RegLdDisp;
			break;
		case 0xC:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*1;
			op->nmid=BTESH2_NMID_MOVUB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispB;
			break;
		case 0xD:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*2;
			op->nmid=BTESH2_NMID_MOVUW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispW;
			break;
		case 0xE:
			op->rn=(opw2>>8)&15;
			op->rm=(opw2>>4)&15;
			op->imm=((sbyte)opw)*4;
			op->nmid=BTESH2_NMID_MOVUL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispD;
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
		i=((byte)opw)|(((byte)opw2)<<8);
		op->rn=(opw2>>8)&15;
		op->imm=(u32)((s16)i);
		op->nmid=BTESH2_NMID_ADD;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_ADD_RegImm;
		BTSH_Fixup_ADD_RegImm(cpu, op);
		break;
	case 0x8: /* 8--- */
		switch((opw2>>8)&15)
		{
		default:
			break;
		}
		break;
	case 0x9: /* 8Edd-9ndd, MOVA @(disp,PC),Rn */
		op->rn=(opw2>>8)&15;
		op->rm=BTESH2_REG_PC;
		i=((byte)opw)|(((byte)opw2)<<8);
		op->ro=i;
		op->imm=((pc+6)&(~3))+i*4;
		op->fl|=BTESH2_OPFL_PCADLYSLOTD;
		op->nmid=BTESH2_NMID_MOVA;
		op->fmid=BTESH2_FMID_REGLDABS;
		op->Run=BTSH_Op_MOVA_RegImm;
		break;
	case 0xA: /* A---, BRA label */
		i=(opw2<<20)>>20;
		i=(i<<8)|((byte)opw);
		op->imm=pc+2*i+6;
		op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
			BTESH2_OPFL_INVDLYSLOT;
		op->nmid=BTESH2_NMID_BRA;
		op->fmid=BTESH2_FMID_ABS;
		op->Run=BTSH_Op_BRA_Abs;
		break;
	case 0xB: /* B---, BSR label */
		i=(opw<<20)>>20;
		i=(i<<8)|((byte)opw);
		op->imm=pc+2*i+6;
		op->fl|=BTESH2_OPFL_CTRLF|BTESH2_OPFL_DLYSLOT|
			BTESH2_OPFL_INVDLYSLOT;
		op->nmid=BTESH2_NMID_BSR;
		op->fmid=BTESH2_FMID_ABS;
		op->Run=BTSH_Op_BSR_Abs;
		break;

	case 0xE: /* E--- */
		i=((byte)opw)|(((byte)opw2)<<8);
		op->rn=(opw2>>8)&15;
		op->imm=(u32)((s16)i);
		op->nmid=BTESH2_NMID_MOV;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;
		break;

	default:
		break;
	}
	return(0);
}

int BTESH2_DecodeOpcode_BJX1(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	u32 pc, u16 opw, u16 opw2)
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
