int BTESH2_DecodeOpcode_2A(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	u32 pc, u16 opw, u16 opw2)
{
	int v;
	int i, j, k;

	op->fl=BTESH2_OPFL_EXTRAWORD;

	switch((opw&15)|((opw>>8)&0xF0))
	{
	case 0x00:
		i=((opw<<12)&0xF0000)|opw2;
		i=(i<<12)>>12;
		op->rn=(opw>>8)&15;
		op->imm=i;
		op->nmid=BTESH2_NMID_MOVI20;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;
		break;
	case 0x01:
		i=((opw<<12)&0xF0000)|opw2;
		i=(i<<12)>>4;
		op->rn=(opw>>8)&15;
		op->imm=i;
		op->nmid=BTESH2_NMID_MOVI20S;
		op->fmid=BTESH2_FMID_REGIMM;
		op->Run=BTSH_Op_MOV_RegImm;
		break;
	case 0x31:
		switch((opw2>>12)&15)
		{
		case 0x0:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=opw2&4095;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispB;
			break;
		case 0x1:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=(opw2&4095)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispW;
			break;
		case 0x2:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=(opw2&4095)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_MOV_RegStDispD;
			break;
		case 0x3:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=(opw2&4095)*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGSTDISP;
			op->Run=BTSH_Op_FMOV_RegStDisp;
			break;

		case 0x4:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=opw2&4095;
			op->nmid=BTESH2_NMID_MOVB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispB;
			break;
		case 0x5:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=(opw2&4095)*2;
			op->nmid=BTESH2_NMID_MOVW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispW;
			break;
		case 0x6:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=(opw2&4095)*4;
			op->nmid=BTESH2_NMID_MOVL;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOV_RegLdDispD;
			break;
		case 0x7:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=(opw2&4095)*4;
			op->nmid=BTESH2_NMID_FMOVS;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_FMOV_RegLdDisp;
			break;
		case 0x8:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=opw2&4095;
			op->nmid=BTESH2_NMID_MOVUB;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispB;
			break;
		case 0x9:
			op->rn=(opw>>8)&15;
			op->rm=(opw>>4)&15;
			op->imm=(opw2&4095)*2;
			op->nmid=BTESH2_NMID_MOVUW;
			op->fmid=BTESH2_FMID_REGLDDISP;
			op->Run=BTSH_Op_MOVU_RegLdDispW;
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}
	return(0);
}
