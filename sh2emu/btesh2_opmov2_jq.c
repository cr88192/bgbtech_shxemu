#define BTSH_OPJQ_MOV_RRNN(rn, rm)	\
	void BTSH_OpJQ_MOV_RegReg_R##rn##R##rm(			\
		BTESH2_CpuState *cpu, BTESH2_Opcode *op)	\
	{	cpu->regs[BTESH2_REG_RLO+rn]=cpu->regs[BTESH2_REG_RLO+rm]; \
		cpu->regs[BTESH2_REG_RHI+rn]=cpu->regs[BTESH2_REG_RHI+rm]; }

#define BTSH_OPJQ_MOV_RRN16(rn)	\
	BTSH_OPJQ_MOV_RRNN(rn, 0)	\
	BTSH_OPJQ_MOV_RRNN(rn, 1)	\
	BTSH_OPJQ_MOV_RRNN(rn, 2)	\
	BTSH_OPJQ_MOV_RRNN(rn, 3)	\
	BTSH_OPJQ_MOV_RRNN(rn, 4)	\
	BTSH_OPJQ_MOV_RRNN(rn, 5)	\
	BTSH_OPJQ_MOV_RRNN(rn, 6)	\
	BTSH_OPJQ_MOV_RRNN(rn, 7)	\
	BTSH_OPJQ_MOV_RRNN(rn, 8)	\
	BTSH_OPJQ_MOV_RRNN(rn, 9)	\
	BTSH_OPJQ_MOV_RRNN(rn, 10)	\
	BTSH_OPJQ_MOV_RRNN(rn, 11)	\
	BTSH_OPJQ_MOV_RRNN(rn, 12)	\
	BTSH_OPJQ_MOV_RRNN(rn, 13)	\
	BTSH_OPJQ_MOV_RRNN(rn, 14)	\
	BTSH_OPJQ_MOV_RRNN(rn, 15)

#define BTSH_OPJQ_MOV_RRN4(rn)	\
	BTSH_OPJQ_MOV_RRNN(rn, 0)	\
	BTSH_OPJQ_MOV_RRNN(rn, 1)	\
	BTSH_OPJQ_MOV_RRNN(rn, 2)	\
	BTSH_OPJQ_MOV_RRNN(rn, 3)

#define BTSH_OPJQ_MOV_RRN8H(rn)	\
	BTSH_OPJQ_MOV_RRNN(rn, 8)	\
	BTSH_OPJQ_MOV_RRNN(rn, 9)	\
	BTSH_OPJQ_MOV_RRNN(rn, 10)	\
	BTSH_OPJQ_MOV_RRNN(rn, 11)	\
	BTSH_OPJQ_MOV_RRNN(rn, 12)	\
	BTSH_OPJQ_MOV_RRNN(rn, 13)	\
	BTSH_OPJQ_MOV_RRNN(rn, 14)	\
	BTSH_OPJQ_MOV_RRNN(rn, 15)

BTSH_OPJQ_MOV_RRN16(0)
BTSH_OPJQ_MOV_RRN16(1)
BTSH_OPJQ_MOV_RRN4(2)
BTSH_OPJQ_MOV_RRN4(3)

BTSH_OPJQ_MOV_RRN16(4)
BTSH_OPJQ_MOV_RRN16(5)
BTSH_OPJQ_MOV_RRN4(6)
BTSH_OPJQ_MOV_RRN4(7)

BTSH_OPJQ_MOV_RRN4(8)
BTSH_OPJQ_MOV_RRN4(9)
BTSH_OPJQ_MOV_RRN4(10)
BTSH_OPJQ_MOV_RRN4(11)

BTSH_OPJQ_MOV_RRN4(12)
BTSH_OPJQ_MOV_RRN4(13)
BTSH_OPJQ_MOV_RRN4(14)
BTSH_OPJQ_MOV_RRN4(15)

BTSH_OPJQ_MOV_RRN8H(12)
BTSH_OPJQ_MOV_RRN8H(13)
BTSH_OPJQ_MOV_RRN8H(14)


#define BTSH_OPJQ_MOV_CASE_RRNN(rn, rm)	\
		case  rm: op->Run=BTSH_OpJQ_MOV_RegReg_R##rn##R##rm; break;

#define BTSH_OPJQ_MOV_CASE_RRN16(rn)	\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 0)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 1)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 2)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 3)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 4)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 5)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 6)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 7)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 8)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 9)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 10)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 11)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 12)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 13)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 14)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 15)

#define BTSH_OPJQ_MOV_CASE_RRN4(rn)	\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 0)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 1)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 2)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 3)

#define BTSH_OPJQ_MOV_CASE_RRN8H(rn)	\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 8)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 9)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 10)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 11)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 12)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 13)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 14)		\
	BTSH_OPJQ_MOV_CASE_RRNN(rn, 15)

void BTSH_FixupJQ_MOV_RegReg(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	switch(op->rn)
	{
	case 0:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN16(0)
		default: break;
		}
		break;
	case 1:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN16(1)
		default: break;
		}
		break;
	case 2:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(2)
		default: break;
		}
		break;
	case 3:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(3)
		default: break;
		}
		break;

	case 4:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN16(4)
		default: break;
		}
		break;
	case 5:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN16(5)
		default: break;
		}
		break;
	case 6:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(6)
		default: break;
		}
		break;
	case 7:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(7)
		default: break;
		}
		break;

	case 8:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(8)
		default: break;
		}
		break;
	case 9:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(9)
		default: break;
		}
		break;

	case 10:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(10)
		default: break;
		}
		break;
	case 11:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(11)
		default: break;
		}
		break;

	case 12:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(12)
		BTSH_OPJQ_MOV_CASE_RRN8H(12)
		default: break;
		}
		break;
	case 13:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(13)
		BTSH_OPJQ_MOV_CASE_RRN8H(13)
		default: break;
		}
		break;
	case 14:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(14)
		BTSH_OPJQ_MOV_CASE_RRN8H(14)
		default: break;
		}
		break;
	case 15:
		switch(op->rm)
		{
		BTSH_OPJQ_MOV_CASE_RRN4(15)
		default: break;
		}
		break;


	default:
		break;
	}
}

void BTSH_OpJQ_MOV_RegImm_R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+0]=op->imm;
	cpu->regs[BTESH2_REG_RHI+0]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+1]=op->imm;
	cpu->regs[BTESH2_REG_RHI+1]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R2(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+2]=op->imm;
	cpu->regs[BTESH2_REG_RHI+2]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R3(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+3]=op->imm;
	cpu->regs[BTESH2_REG_RHI+3]=op->imm>>32;	}

void BTSH_OpJQ_MOV_RegImm_R4(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+4]=op->imm;
	cpu->regs[BTESH2_REG_RHI+4]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R5(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+5]=op->imm;
	cpu->regs[BTESH2_REG_RHI+5]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R6(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+6]=op->imm;
	cpu->regs[BTESH2_REG_RHI+6]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R7(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+7]=op->imm;
	cpu->regs[BTESH2_REG_RHI+7]=op->imm>>32;	}

void BTSH_OpJQ_MOV_RegImm_R8(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+8]=op->imm;
	cpu->regs[BTESH2_REG_RHI+8]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R9(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+9]=op->imm;
	cpu->regs[BTESH2_REG_RHI+9]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R10(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+10]=op->imm;
	cpu->regs[BTESH2_REG_RHI+10]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R11(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+11]=op->imm;
	cpu->regs[BTESH2_REG_RHI+11]=op->imm>>32;	}

void BTSH_OpJQ_MOV_RegImm_R12(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+12]=op->imm;
	cpu->regs[BTESH2_REG_RHI+12]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R13(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+13]=op->imm;
	cpu->regs[BTESH2_REG_RHI+13]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R14(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+14]=op->imm;
	cpu->regs[BTESH2_REG_RHI+14]=op->imm>>32;	}
void BTSH_OpJQ_MOV_RegImm_R15(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{	cpu->regs[BTESH2_REG_RLO+15]=op->imm;
	cpu->regs[BTESH2_REG_RHI+15]=op->imm>>32;	}

void BTSH_FixupJQ_MOV_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	switch(op->rn)
	{
	case 0:		op->Run=BTSH_OpJQ_MOV_RegImm_R0; break;
	case 1:		op->Run=BTSH_OpJQ_MOV_RegImm_R1; break;
	case 2:		op->Run=BTSH_OpJQ_MOV_RegImm_R2; break;
	case 3:		op->Run=BTSH_OpJQ_MOV_RegImm_R3; break;
	case 4:		op->Run=BTSH_OpJQ_MOV_RegImm_R4; break;
	case 5:		op->Run=BTSH_OpJQ_MOV_RegImm_R5; break;
	case 6:		op->Run=BTSH_OpJQ_MOV_RegImm_R6; break;
	case 7:		op->Run=BTSH_OpJQ_MOV_RegImm_R7; break;
	case 8:		op->Run=BTSH_OpJQ_MOV_RegImm_R8; break;
	case 9:		op->Run=BTSH_OpJQ_MOV_RegImm_R9; break;
	case 10:	op->Run=BTSH_OpJQ_MOV_RegImm_R10; break;
	case 11:	op->Run=BTSH_OpJQ_MOV_RegImm_R11; break;
	case 12:	op->Run=BTSH_OpJQ_MOV_RegImm_R12; break;
	case 13:	op->Run=BTSH_OpJQ_MOV_RegImm_R13; break;
	case 14:	op->Run=BTSH_OpJQ_MOV_RegImm_R14; break;
	case 15:	op->Run=BTSH_OpJQ_MOV_RegImm_R15; break;
	}
}

#if 0
void BTSH_OpJQ_ADD_RegImm_R0(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+0]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R1(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+1]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R2(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+2]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R3(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+3]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R4(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+4]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R5(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+5]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R6(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+6]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R7(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+7]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R8(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+8]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R9(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+9]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R10(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+10]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R11(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+11]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R12(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+12]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R13(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+13]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R14(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+14]+=op->imm; }
void BTSH_OpJQ_ADD_RegImm_R15(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
	{ cpu->regs[BTESH2_REG_RLO+15]+=op->imm; }

void BTSH_Fixup_ADD_RegImm(BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	switch(op->rn)
	{
	case  0: op->Run=BTSH_OpJQ_ADD_RegImm_R0; break;
	case  1: op->Run=BTSH_OpJQ_ADD_RegImm_R1; break;
	case  2: op->Run=BTSH_OpJQ_ADD_RegImm_R2; break;
	case  3: op->Run=BTSH_OpJQ_ADD_RegImm_R3; break;
	case  4: op->Run=BTSH_OpJQ_ADD_RegImm_R4; break;
	case  5: op->Run=BTSH_OpJQ_ADD_RegImm_R5; break;
	case  6: op->Run=BTSH_OpJQ_ADD_RegImm_R6; break;
	case  7: op->Run=BTSH_OpJQ_ADD_RegImm_R7; break;
	case  8: op->Run=BTSH_OpJQ_ADD_RegImm_R8; break;
	case  9: op->Run=BTSH_OpJQ_ADD_RegImm_R9; break;
	case 10: op->Run=BTSH_OpJQ_ADD_RegImm_R10; break;
	case 11: op->Run=BTSH_OpJQ_ADD_RegImm_R11; break;
	case 12: op->Run=BTSH_OpJQ_ADD_RegImm_R12; break;
	case 13: op->Run=BTSH_OpJQ_ADD_RegImm_R13; break;
	case 14: op->Run=BTSH_OpJQ_ADD_RegImm_R14; break;
	case 15: op->Run=BTSH_OpJQ_ADD_RegImm_R15; break;
	default: break;
	}
}
#endif
