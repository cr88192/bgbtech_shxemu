/*
RAX( 0): Scratch
RCX( 1): Scratch
RDX( 2): Scratch
RBX( 3): Var Cache
RSP( 4): OS Stack
RBP( 5): Frame / Cache
RSI( 6): Locals + VM Stack
RDI( 7): VM Frame
R8 ( 8): Scratch
R9 ( 9): Scratch
R10(10): Scratch
R11(11): Scratch
R12(12): Cache
R13(13): Cache
R14(14): Cache
R15(15): Cache

[RBP+24]: Trace
[RBP+16]: CPU
[RBP+ 8]/[RSP+504]: Return RIP
[RBP+ 0]/[RSP+496]: Saved RBP
[RBP- 8]/[RSP+488]: Saved RSI
[RBP-16]/[RSP+480]: Saved RDI
[RBP-24]/[RSP+472]: Saved RBX
[RBP-32]/[RSP+464]: Saved R12
[RBP-40]/[RSP+456]: Saved R13
[RBP-48]/[RSP+448]: Saved R14
[RBP-56]/[RSP+440]: Saved R15

Frame 

Args:
	 RCX,  RDX,   R8,   R9
	XMM0, XMM1, XMM2, XMM3

[RSP+8]: Opcode
[RSP+0]: CPU

Locals and stack are placed end-to-end in the frame.
The stack is placed after the locals.

Possible Naive Reg Allocator:
  First Pass: Stat most-used vars in a trace.
  Assign top N vars to cache registers.
  Operations loading/storing this var directed to register.
  Write back when trace ends.

 */

#ifdef _M_X64

#define BTEJ_X64_FRAMESZ	512
#define BTEJ_X64_FRAMEADJ	(BTEJ_X64_FRAMESZ-8)

#define BTEJ_X64_FRIDX_TRACE	1
#define BTEJ_X64_FRIDX_CPU		0
#define BTEJ_X64_FRIDX_RIP		-1
#define BTEJ_X64_FRIDX_RBP		-2
#define BTEJ_X64_FRIDX_RSI		-3
#define BTEJ_X64_FRIDX_RDI		-4
#define BTEJ_X64_FRIDX_RBX		-5
#define BTEJ_X64_FRIDX_R12		-6
#define BTEJ_X64_FRIDX_R13		-7
#define BTEJ_X64_FRIDX_R14		-8
#define BTEJ_X64_FRIDX_R15		-9

int BTESH2_JitLoadFrame(UAX_Context *jctx, int idx, int reg)
{
	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		reg, UAX_REG_RSP, BTEJ_X64_FRAMESZ+(idx*8));
	return(0);
}

int BTESH2_JitStoreFrame(UAX_Context *jctx, int idx, int reg)
{
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RSP, BTEJ_X64_FRAMESZ+(idx*8), reg);
	return(0);
}

int BTESH2_JitLoadReadSyncVMReg(UAX_Context *jctx, int idx)
{
	int reg;
	int i, j, k;

//	return(UAX_REG_Z);

	for(i=0; i<UAX_MAX_CACHEVAR; i++)
		if((jctx->reg_resv&(1<<i)) && (jctx->reg_idx[i]==idx))
	{
		reg=jctx->reg_reg[i];
		if(jctx->reg_live&(1<<i))
			return(reg);
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
		jctx->reg_live|=(1<<i);
		return(reg);
	}
	
	return(UAX_REG_Z);
}

int BTESH2_JitLoadWriteSyncVMReg(UAX_Context *jctx, int idx)
{
	int reg;
	int i, j, k;

//	return(UAX_REG_Z);

	for(i=0; i<UAX_MAX_CACHEVAR; i++)
		if((jctx->reg_resv&(1<<i)) && (jctx->reg_idx[i]==idx))
	{
		reg=jctx->reg_reg[i];
		if(jctx->reg_live&(1<<i))
			return(reg);
//		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
//			reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
		jctx->reg_live|=(1<<i);
		return(reg);
	}
	
	return(UAX_REG_Z);
}

int BTESH2_JitLoadVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		if(UAX_Asm_RegIsXmmP(reg))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_MOVD, reg, reg1); }
		else
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg, reg1); }
		return(0);
	}

	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVSS,
			reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
	}else
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
	}
	return(0);
}

int BTESH2_JitStoreVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadWriteSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		if(UAX_Asm_RegIsXmmP(reg))
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_MOVD, reg1, reg); }
		else
			{ UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg1, reg); }
		return(0);
	}

	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOVSS,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	}else
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	}
	return(0);
}

int BTESH2_JitStoreVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadWriteSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, UAX_REG_EAX, val);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg1, UAX_REG_EAX);

//		UAX_AsmInsnRegImm(jctx, UAX_OP_MOV, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_MOV,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitLoadVMRegZx(UAX_Context *jctx, int idx, int reg)
{
	int reg1, reg1b;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		reg1b=UAX_REG_RAX|(reg1&15);
	
//		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVZXD, reg, reg1);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg, reg1b);
		return(0);
	}

	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVZXD,
		reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
	return(0);
}

int BTESH2_JitInsnVMRegReg(UAX_Context *jctx, int op, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, op, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, op,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitInsnVMRegImm(UAX_Context *jctx, int op, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, op, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, op,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitInsnVMReg(UAX_Context *jctx, int op, int idx)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnReg(jctx, op, reg1);
		return(0);
	}

	UAX_AsmInsnStRegDisp32(jctx, op,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
	return(0);
}

int BTESH2_JitInsnRegVMReg(UAX_Context *jctx, int op, int reg, int idx)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, op, reg, reg1);
		return(0);
	}

	UAX_AsmInsnRegLdRegDisp(jctx, op,
		reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
	return(0);
}


#if 1
int BTESH2_JitAddVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BTESH2_JitInsnVMRegReg(jctx, UAX_OP_ADD, idx, reg)); }
int BTESH2_JitSubVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BTESH2_JitInsnVMRegReg(jctx, UAX_OP_SUB, idx, reg)); }
int BTESH2_JitAndVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BTESH2_JitInsnVMRegReg(jctx, UAX_OP_AND, idx, reg)); }
int BTESH2_JitOrVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BTESH2_JitInsnVMRegReg(jctx, UAX_OP_OR, idx, reg)); }
int BTESH2_JitXorVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BTESH2_JitInsnVMRegReg(jctx, UAX_OP_XOR, idx, reg)); }
int BTESH2_JitCmpVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BTESH2_JitInsnVMRegReg(jctx, UAX_OP_CMP, idx, reg)); }
int BTESH2_JitTestVMReg(UAX_Context *jctx, int idx, int reg)
	{ return(BTESH2_JitInsnVMRegReg(jctx, UAX_OP_TEST, idx, reg)); }

int BTESH2_JitAddVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BTESH2_JitInsnVMRegImm(jctx, UAX_OP_ADD, idx, val)); }
int BTESH2_JitAndVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BTESH2_JitInsnVMRegImm(jctx, UAX_OP_AND, idx, val)); }
int BTESH2_JitOrVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BTESH2_JitInsnVMRegImm(jctx, UAX_OP_OR, idx, val)); }
int BTESH2_JitXorVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BTESH2_JitInsnVMRegImm(jctx, UAX_OP_XOR, idx, val)); }
int BTESH2_JitShlVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BTESH2_JitInsnVMRegImm(jctx, UAX_OP_SHL, idx, val)); }
int BTESH2_JitShrVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BTESH2_JitInsnVMRegImm(jctx, UAX_OP_SHR, idx, val)); }
int BTESH2_JitTestVMRegImm(UAX_Context *jctx, int idx, s32 val)
	{ return(BTESH2_JitInsnVMRegImm(jctx, UAX_OP_TEST, idx, val)); }
#endif

int BTESH2_JitMovVMRegVMReg(UAX_Context *jctx, int didx, int sidx)
{
	int reg1, reg2;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, sidx);
	reg2=BTESH2_JitLoadWriteSyncVMReg(jctx, didx);
	if(reg1!=UAX_REG_Z)
	{
		if(reg2!=UAX_REG_Z)
		{
			UAX_AsmInsnRegReg(jctx, UAX_OP_MOV, reg2, reg1);
			return(1);
		}

		BTESH2_JitStoreVMReg(jctx, didx, reg1);
		return(1);
	}

	if(reg2!=UAX_REG_Z)
	{
		BTESH2_JitLoadVMReg(jctx, sidx, reg2);
		return(1);
	}

	BTESH2_JitLoadVMReg(jctx, sidx, UAX_REG_EAX);
	BTESH2_JitStoreVMReg(jctx, didx, UAX_REG_EAX);
	return(1);
}

int BTESH2_JitInsnVMRegVMReg(UAX_Context *jctx, int op, int didx, int sidx)
{
	int dreg, sreg;

	dreg=BTESH2_JitLoadReadSyncVMReg(jctx, didx);
	sreg=BTESH2_JitLoadReadSyncVMReg(jctx, sidx);
	if(sreg!=UAX_REG_Z)
	{
		if(dreg!=UAX_REG_Z)
		{
			UAX_AsmInsnRegReg(jctx, op, dreg, sreg);
			return(1);
		}

		BTESH2_JitInsnVMRegReg(jctx, op, didx, sreg);
		return(1);
	}

	if(dreg!=UAX_REG_Z)
	{
//		BTESH2_JitLoadVMReg(jctx, sidx, UAX_REG_EAX);
//		UAX_AsmInsnRegReg(jctx, op, dreg, UAX_REG_EAX);
		BTESH2_JitInsnRegVMReg(jctx, op, dreg, sidx);
		return(1);
	}

	BTESH2_JitLoadVMReg(jctx, sidx, UAX_REG_EAX);
	BTESH2_JitInsnVMRegReg(jctx, op, didx, UAX_REG_EAX);
	return(1);
}


#if 0
int BTESH2_JitAddVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_ADD, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_ADD,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitSubVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_SUB, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_SUB,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitAndVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_AND, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_AND,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitOrVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_OR, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_OR,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitXorVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_XOR,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitCmpVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_CMP, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_CMP,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitTestVMReg(UAX_Context *jctx, int idx, int reg)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_TEST, reg1, reg);
		return(0);
	}

	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_TEST,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitAddVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_ADD,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitAndVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_AND, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_AND,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitOrVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_OR, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_OR,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitXorVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_XOR, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_XOR,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitShlVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHL, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_SHL,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitShrVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_SHR, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_SHR,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

int BTESH2_JitTestVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
	int reg1;

	reg1=BTESH2_JitLoadReadSyncVMReg(jctx, idx);
	if(reg1!=UAX_REG_Z)
	{
		UAX_AsmInsnRegImm(jctx, UAX_OP_TEST, reg1, val);
		return(0);
	}

	UAX_AsmInsnStRegDispImm32(jctx, UAX_OP_TEST,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);
	return(0);
}

#endif


int BTESH2_JitLoadVMFReg(UAX_Context *jctx, int idx, int reg)
{
	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOVSS,
			reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4));
	}else
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4));
	}
	return(0);
}

int BTESH2_JitStoreVMFReg(UAX_Context *jctx, int idx, int reg)
{
	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOVSS,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4), reg);
	}else
	{
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4), reg);
	}
	return(0);
}


int BTESH2_JitLoadVMDReg(UAX_Context *jctx, int idx, int reg)
{
	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4));
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_RAX, 32);
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, reg, UAX_REG_RAX);
	}else
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4));
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, reg, 32);
	}
	return(0);
}

int BTESH2_JitStoreVMDReg(UAX_Context *jctx, int idx, int reg)
{
	if(UAX_Asm_RegIsXmmP(reg))
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_RAX, reg);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_RAX, 32);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4),
			UAX_REG_RAX);
	}else
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_MOVQ, UAX_REG_RAX, reg);
		UAX_AsmInsnRegImm(jctx, UAX_OP_ROL, UAX_REG_RAX, 32);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, fregs)+(idx*4),
			UAX_REG_RAX);
	}
	return(0);
}

int BTESH2_JitFlushJNext(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	UAX_AsmXorRegReg(jctx, UAX_REG_R8Q, UAX_REG_R8Q);
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_R8Q);
	return(0);
}

int BTESH2_JitEmitCallFPtr(
	UAX_Context *jctx, BTESH2_CpuState *cpu,
	void *addr)
{
	int i;

	if(UAX_CheckAddrExHeapRel32(addr))
	{
		i=UAX_LookupGlobalAddress(addr);
		if(i<=0)
		{
			i=UAX_RegisterGlobal(NULL, addr);
		}
	}else
	{
		i=0;
	}

	if(i>0)
	{
		UAX_AsmInsnLabel(jctx, UAX_OP_CALL, i);
	}else
	{
		UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)addr);
		UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	}
	return(0);
}

/* EDX -> EAX */
int BTESH2_JitGetAddrDWord(UAX_Context *jctx, BTESH2_CpuState *cpu)
{
	int l0, l1, l2;
	int i;

	if(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU_NoAT)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_ECX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pdata));

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOV, UAX_REG_EAX,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0);

//		UAX_AsmInsnNone(jctx, UAX_OP_RET);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrDWord);
		UAX_EmitLabel(jctx, l1);
		return(0);
	}


	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->GetAddrDWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);

	BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrDWord);
	return(0);
}

int BTESH2_JitGetAddrWord(UAX_Context *jctx, BTESH2_CpuState *cpu)
{
	int l0, l1, l2;
	int i;

	if(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU_NoAT)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_ECX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pdata));

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOVSXW, UAX_REG_EAX,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0);

//		UAX_AsmInsnNone(jctx, UAX_OP_RET);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrWord);
		UAX_EmitLabel(jctx, l1);
		return(0);
	}

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->GetAddrWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrWord);
	return(0);
}

int BTESH2_JitGetAddrByte(UAX_Context *jctx, BTESH2_CpuState *cpu)
{
	int l0, l1, l2;
	int i;

	if(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU_NoAT)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_ECX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pdata));

		UAX_AsmInsnRegLdRegIxDisp(jctx, UAX_OP_MOVSXB, UAX_REG_EAX,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0);

//		UAX_AsmInsnNone(jctx, UAX_OP_RET);
		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrByte);
		UAX_EmitLabel(jctx, l1);
		return(0);
	}

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->GetAddrByte));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->GetAddrByte);
	return(0);
}

/* EDX,R8 -> */
int BTESH2_JitSetAddrDWord(UAX_Context *jctx, BTESH2_CpuState *cpu)
{
	int l0, l1, l2;
	int i;

#if 1
	if(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU_NoAT)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_ECX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pdata));

		UAX_AsmInsnStRegIxDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0, UAX_REG_R8D);

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrDWord);
		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->SetAddrDWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrDWord);
	return(0);
}

int BTESH2_JitSetAddrWord(UAX_Context *jctx, BTESH2_CpuState *cpu)
{
	int l0, l1, l2;
	int i;

#if 1
	if(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU_NoAT)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_ECX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pdata));

		UAX_AsmInsnStRegIxDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0, UAX_REG_R8W);

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrWord);
		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->SetAddrWord));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrWord);
	return(0);
}

int BTESH2_JitSetAddrByte(UAX_Context *jctx, BTESH2_CpuState *cpu)
{
	int l0, l1, l2;
	int i;

#if 1
	if(cpu->GetAddrDWord==BTESH2_GetAddrDWordFMMU_NoAT)
	{
		l0=UAX_GenLabelTemp(jctx);
		l1=UAX_GenLabelTemp(jctx);

		UAX_AsmMovRegReg(jctx, UAX_REG_ECX, UAX_REG_EDX);
		UAX_AsmAndRegImm(jctx, UAX_REG_ECX, 0x1FFFFFFF);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_SUB,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pbase));
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_CMP,
			UAX_REG_ECX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_prng3));

		UAX_AsmInsnLabel(jctx, UAX_OP_JA, l0);

		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RDX, UAX_REG_RDI, offsetof(BTESH2_CpuState, pspan_pdata));

		UAX_AsmInsnStRegIxDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDX, 1, UAX_REG_RCX, 0, UAX_REG_R8B);

		UAX_AsmInsnLabel(jctx, UAX_OP_JMP, l1);

		UAX_EmitLabel(jctx, l0);

		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrByte);
		UAX_EmitLabel(jctx, l1);
		return(0);
	}
#endif

	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);

//	UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(cpu->SetAddrByte));
//	UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	BTESH2_JitEmitCallFPtr(jctx, cpu, cpu->SetAddrByte);
	return(0);
}

int BTESH2_TryJitOpcode(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *tr, BTESH2_Opcode *op)
{
//	return(0);

	if(BTESH2_TryJitOpcode_MovReg(jctx, cpu, tr, op)>0)
		return(1);
	if(BTESH2_TryJitOpcode_ArithReg(jctx, cpu, tr, op)>0)
		return(1);
	if(BTESH2_TryJitOpcode_MovMem(jctx, cpu, tr, op)>0)
		return(1);

//	return(0);

#if 1
	if(BTESH2_TryJitOpcode_FpuOp(jctx, cpu, tr, op)>0)
		return(1);
	if(BTESH2_TryJitOpcode_BranchOp(jctx, cpu, tr, op)>0)
		return(1);
#endif

//	return(0);

	if(BTESH2_TryJitOpcode_CmpOp(jctx, cpu, tr, op)>0)
		return(1);
	if(BTESH2_TryJitOpcode_SignExtOp(jctx, cpu, tr, op)>0)
		return(1);

	return(0);
}

BTESH2_Trace *BTESH2_JTraceForAddr(BTESH2_CpuState *cpu, u32 spc)
{
	BTESH2_Trace *tr0, *tr1, *tr2, *tr3, *tr4;
	u32 spc1;
	int h, h0, h1, h2, h3, hp, hp1;
	int i, j, k;
	
//	return(NULL);
	
	hp=(spc^cpu->csfl)*BTESH2_TR_JHASHPR;
	h=(hp>>BTESH2_TR_JHASHSHR)&(BTESH2_TR_JHASHSZ-1);

	h0=h*4+0;
	tr0=cpu->jcache[h0];
	if(!tr0)
		return(NULL);
	if((tr0->srcpc==spc) && (tr0->csfl==cpu->csfl))
	{
		tr0->jtflag|=BTESH2_TRJTFL_JCACHE;
		return(tr0);
	}

	h1=h*4+1;
	tr1=cpu->jcache[h1];
	if(!tr1)
		return(NULL);

	if((tr1->srcpc==spc) && (tr1->csfl==cpu->csfl))
	{
		cpu->jcache[h0]=tr1;
		cpu->jcache[h1]=tr0;
		tr1->jtflag|=BTESH2_TRJTFL_JCACHE;
		tr0->jtflag|=BTESH2_TRJTFL_JCACHE;
		return(tr1);
	}

	h2=h*4+2;
	tr2=cpu->jcache[h2];
	if(!tr2)
		return(NULL);

	if((tr2->srcpc==spc) && (tr2->csfl==cpu->csfl))
	{
		cpu->jcache[h0]=tr2;
		cpu->jcache[h1]=tr0;
		cpu->jcache[h2]=tr1;
		tr2->jtflag|=BTESH2_TRJTFL_JCACHE;
		tr1->jtflag|=BTESH2_TRJTFL_JCACHE;
		tr0->jtflag|=BTESH2_TRJTFL_JCACHE;
		return(tr2);
	}

	h3=h*4+3;
	tr3=cpu->jcache[h3];
	if(!tr3)
		return(NULL);

	if((tr3->srcpc==spc) && (tr3->csfl==cpu->csfl))
	{
		cpu->jcache[h0]=tr3;
		cpu->jcache[h1]=tr0;
		cpu->jcache[h2]=tr1;
		cpu->jcache[h3]=tr2;
		tr3->jtflag|=BTESH2_TRJTFL_JCACHE;
		tr2->jtflag|=BTESH2_TRJTFL_JCACHE;
		tr1->jtflag|=BTESH2_TRJTFL_JCACHE;
		tr0->jtflag|=BTESH2_TRJTFL_JCACHE;
		return(tr3);
	}

	tr4=cpu->trlinked[h&255];
	while(tr4)
	{
		if((tr4->srcpc==spc) && (tr4->csfl==cpu->csfl))
		{
			BTESH2_JTraceAddTrace(cpu, tr4);
			return(tr4);
		}
		tr4=tr4->lnknext;
	}

	return(NULL);
}

int BTESH2_JTraceAddTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	BTESH2_Trace *tr0, *tr1, *tr2, *tr3;
	u32 spc1;
	int h, h0, h1, h2, h3, hp, hp1;
	int i, j, k;

//	return(-1);

	if(tr->jtflag&BTESH2_TRJTFL_JCACHE)
		return(0);

	tr->jtflag|=BTESH2_TRJTFL_JCACHE;

	hp=(tr->srcpc^tr->csfl)*BTESH2_TR_JHASHPR;
	h=(hp>>BTESH2_TR_JHASHSHR)&(BTESH2_TR_JHASHSZ-1);

	h0=h*4+0;	h1=h*4+1;
	h2=h*4+2;	h3=h*4+3;
	tr0=cpu->jcache[h0];	tr1=cpu->jcache[h1];
	tr2=cpu->jcache[h2];	tr3=cpu->jcache[h3];
	cpu->jcache[h0]=tr;		cpu->jcache[h1]=tr0;
	cpu->jcache[h2]=tr1;	cpu->jcache[h3]=tr2;

	if(tr3 && (tr3!=tr))
	{
		tr3->jtflag&=~BTESH2_TRJTFL_JCACHE;
		if(!(tr3->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
		{
			BTESH2_FlushTrace(cpu, tr3);
			BTESH2_FreeTrace(cpu, tr3);
		}
	}
	
	return(0);
}

int BTESH2_JitLinkTrace(BTESH2_CpuState *cpu, BTESH2_Trace *trj)
{
	int h, hp;

	if(!(trj->jtflag&BTESH2_TRJTFL_LINKED))
	{
		hp=(trj->srcpc^trj->csfl)*BTESH2_TR_JHASHPR;
		h=(hp>>BTESH2_TR_JHASHSHR)&(BTESH2_TR_JHASHSZ-1);

		trj->lnknext=cpu->trlinked[h&255];
		cpu->trlinked[h&255]=trj;
		trj->jtflag|=BTESH2_TRJTFL_LINKED;
		trj->lnkcnt=1;
	}else
	{
		if(trj->lnkcnt<255)
			trj->lnkcnt++;
	}
	
	return(0);
}

int BTESH2_JitUnlinkTrace(BTESH2_CpuState *cpu, BTESH2_Trace *trj)
{
	BTESH2_Trace *trc, *trl, *tr1, *tr2;
	int h, hp;

	if(!trj)
		return(0);

	if(trj->maxpc==0xDEADFEED)
	{
		__debugbreak();
		return;
	}

	if(trj->srcpc==(u32)(-1))
	{
		__debugbreak();
		return;
	}

	if((trj->lnkcnt>0) && (trj->lnkcnt<255))
	{
		trj->lnkcnt--;
		if(!trj->lnkcnt)
		{
			hp=(trj->srcpc^trj->csfl)*BTESH2_TR_JHASHPR;
			h=(hp>>BTESH2_TR_JHASHSHR)&(BTESH2_TR_JHASHSZ-1);

			trc=cpu->trlinked[h&255]; trl=NULL;
			while(trc && (trc!=trj))
				{ trl=trc; trc=trc->lnknext; }
			if(trc)
			{
				if(trl)
					{ trl->lnknext=trc->lnknext; }
				else
					{ cpu->trlinked[h&255]=trc->lnknext; }
			}

			trj->jtflag&=~BTESH2_TRJTFL_LINKED;
			if(!(trj->jtflag&BTESH2_TRJTFL_NOFREE_MASK))
			{
				tr1=trj->trnext;	tr2=trj->trjmpnext;
				trj->trnext=NULL;	trj->trjmpnext=NULL;
				BTESH2_JitUnlinkTrace(cpu, tr1);
				BTESH2_JitUnlinkTrace(cpu, tr2);

				BTESH2_FlushTrace(cpu, trj);
				BTESH2_FreeTrace(cpu, trj);
			}
		}
	}

	return(0);
}

int BTESH2_JitSyncRegs(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *trj)
{
	int idx, reg;
	int i, j, k;
	
	if(!jctx->reg_live)
		return(0);
	
	for(i=0; i<16; i++)
		if(jctx->reg_live&(1<<i))
	{
		idx=jctx->reg_idx[i];
		reg=jctx->reg_reg[i];
		
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
		jctx->reg_live&=~(1<<i);
	}
	return(0);
}

int BTESH2_JitSyncSaveReg(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *trj,
	int reg)
{
	if(reg==UAX_REG_Z)
		return(0);

	if((reg&15)==3)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RBX, UAX_REG_RBX);
		jctx->reg_save|=0x0008;	}
	if((reg&15)==5)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RBP, UAX_REG_RBP);
		jctx->reg_save|=0x0020;	}
	if((reg&15)==6)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);
		jctx->reg_save|=0x0040;	}
	if((reg&15)==7)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
		jctx->reg_save|=0x0080;	}

	if((reg&15)==12)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R12, UAX_REG_R12Q);
		jctx->reg_save|=0x1000;	}
	if((reg&15)==13)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R13, UAX_REG_R13Q);
		jctx->reg_save|=0x2000;	}
	if((reg&15)==14)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R14, UAX_REG_R14Q);
		jctx->reg_save|=0x4000;	}
	if((reg&15)==15)
	{	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_R15, UAX_REG_R15Q);
		jctx->reg_save|=0x8000;	}
	return(0);
}

int BTESH2_JitSyncRestoreRegs(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Trace *trj)
{
	if(jctx->reg_save&0x0008)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RBX, UAX_REG_RBX);
	if(jctx->reg_save&0x0020)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RBP, UAX_REG_RBP);
	if(jctx->reg_save&0x0040)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);
	if(jctx->reg_save&0x0080)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);

	if(jctx->reg_save&0x1000)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R12, UAX_REG_R12Q);
	if(jctx->reg_save&0x2000)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R13, UAX_REG_R13Q);
	if(jctx->reg_save&0x4000)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R14, UAX_REG_R14Q);
	if(jctx->reg_save&0x8000)
		BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_R15, UAX_REG_R15Q);
	return(0);
}

int BTESH2_TryJitTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	byte uax_cachereg[6]={
		UAX_REG_R12D, UAX_REG_R13D, UAX_REG_R14D,
		UAX_REG_R15D,  UAX_REG_EBX,  UAX_REG_ESI};
	byte regidx[64];
	byte regcnt[64];
	UAX_Context *jctx;
	BTESH2_Trace *trj;
	byte nolink;
	byte *ptr;
	int l0, l1, l2;
	int i0, i1, i2;
	int i, j, k, sz;

//	if(cpu->arch==BTESH2_ARCH_SH2)
//		return(0);

	nolink=0;
//	if(cpu->arch==BTESH2_ARCH_SH2)
//		nolink=1;

#if 1
	for(i=0; i<64; i++)
	{
		regcnt[i]=0;
		regidx[i]=i;
	}
#endif

#if 1
	for(i=0; i<tr->nops; i++)
	{
		i0=tr->ops[i]->rm;
		i1=tr->ops[i]->rn;
		i2=tr->ops[i]->ro;
		if((i0<0) || (i0>=BTESH2_REG_ZZR))i0=BTESH2_REG_ZZR;
		if((i1<0) || (i1>=BTESH2_REG_ZZR))i1=BTESH2_REG_ZZR;
		if((i2<0) || (i2>=BTESH2_REG_ZZR))i2=BTESH2_REG_ZZR;
		
		switch(tr->ops[i]->fmid)
		{
		case BTESH2_FMID_REGREG:
		case BTESH2_FMID_REGST:
		case BTESH2_FMID_REGLD:
		case BTESH2_FMID_REGSTDISP:
		case BTESH2_FMID_REGLDDISP:
			regcnt[i0]++;	regcnt[i1]++;	break;
		case BTESH2_FMID_REGRM:
		case BTESH2_FMID_LDREG:
			regcnt[i0]++;	break;
		case BTESH2_FMID_REGRN:
		case BTESH2_FMID_REGIMM:
		case BTESH2_FMID_REGVIMM:
		case BTESH2_FMID_REGLDABS:
		case BTESH2_FMID_STREG:
			regcnt[i1]++;	break;
		case BTESH2_FMID_REGDECST:
			regcnt[i0]++;	regcnt[i1]+=2;	break;
		case BTESH2_FMID_REGINCLD:
			regcnt[i0]+=2;	regcnt[i1]++;	break;
		case BTESH2_FMID_REGSTR0N:
		case BTESH2_FMID_REGLDR0M:
			regcnt[i0]++;
			regcnt[i1]++;
			regcnt[i2]++;
			break;
		case BTESH2_FMID_ABS:
		case BTESH2_FMID_NONE:
		case BTESH2_FMID_IMM:
			break;
		case BTESH2_FMID_FREGREG:
		case BTESH2_FMID_DREGREG:
			break;
		case BTESH2_FMID_FREGRM:
		case BTESH2_FMID_DREGRM:
			break;
		case BTESH2_FMID_FREGRN:
		case BTESH2_FMID_DREGRN:
			break;
		default:
			regcnt[i0]++;
			regcnt[i1]++;
			regcnt[i2]++;
			break;
		}

		switch(tr->ops[i]->nmid)
		{
		case BTESH2_NMID_BT:		case BTESH2_NMID_BF:
		case BTESH2_NMID_BTS:		case BTESH2_NMID_BFS:
		case BTESH2_NMID_CLRT:		case BTESH2_NMID_SETT:
		case BTESH2_NMID_CLRS:		case BTESH2_NMID_SETS:
			regcnt[BTESH2_REG_SR]++;
			break;
		case BTESH2_NMID_CMPHS:		case BTESH2_NMID_CMPHI:
		case BTESH2_NMID_CMPEQ:		case BTESH2_NMID_CMPGE:
		case BTESH2_NMID_CMPGT:		case BTESH2_NMID_TST:
		case BTESH2_NMID_CMPPL:		case BTESH2_NMID_CMPPZ:
		case BTESH2_NMID_SHAL:		case BTESH2_NMID_SHAR:
		case BTESH2_NMID_SHLL:		case BTESH2_NMID_SHLR:
			regcnt[BTESH2_REG_SR]+=2;
			break;
		default:
			break;
		}
	}
	
	regcnt[BTESH2_REG_ZZR]=0;

	for(i=0; i<64; i++)
		for(j=i+1; j<64; j++)
	{
		if(regcnt[j]>regcnt[i])
		{	k=regcnt[i]; regcnt[i]=regcnt[j]; regcnt[j]=k;
			k=regidx[i]; regidx[i]=regidx[j]; regidx[j]=k;	}
	}

	for(i=0; i<64; i++)
	{
		if(regcnt[i]<4)regcnt[i]=0;
	}
#endif

	
#if 1
	if(!tr->trnext && !nolink)
	{
		trj=BTESH2_TraceForAddr(cpu, tr->nxtpc);
		if(trj)
		{
			BTESH2_JitLinkTrace(cpu, trj);
			tr->trnext=trj;
		}
	}
#endif

#if 1
	if(!tr->trjmpnext && tr->jmppc && !nolink)
	{
		trj=BTESH2_TraceForAddr(cpu, tr->jmppc);
		if(trj)
		{
			BTESH2_JitLinkTrace(cpu, trj);
			tr->trjmpnext=trj;
		}
	}
#endif

	jctx=UAX_AllocContext();

	jctx->reg_resv=0;
	jctx->reg_live=0;
	jctx->reg_save=0;
	jctx->jitfl=0;
	
	if(nolink)
	{
//		jctx->jitfl|=BTESH2_UAXJFL_NOPREJMP;
	}
	
#if 1
	for(i=0; i<UAX_MAX_CACHEVAR; i++)
	{
		if(regcnt[i]<=0)
		{
			jctx->reg_reg[i]=UAX_REG_Z;
			continue;
		}
		jctx->reg_idx[i]=regidx[i];
//		jctx->reg_reg[i]=UAX_REG_R12D+i;
		jctx->reg_reg[i]=uax_cachereg[i];
		jctx->reg_resv|=1<<i;
	}
#endif

	
	UAX_SetSection(jctx, UAX_CSEG_TEXT);
	
	UAX_AsmInsnRegImm(jctx, UAX_OP_SUB, UAX_REG_RSP, BTEJ_X64_FRAMEADJ);
//	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_CPU, UAX_REG_RCX);
//	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_TRACE, UAX_REG_RDX);

	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
//	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);

	for(i=0; i<UAX_MAX_CACHEVAR; i++)
	{
		if(!(jctx->reg_resv&(1<<i)))
			continue;
		BTESH2_JitSyncSaveReg(jctx, cpu, tr,
			jctx->reg_reg[i]);

//		UAX_AsmXorRegReg(jctx, jctx->reg_reg[i], jctx->reg_reg[i]);
	}

	UAX_AsmMovRegReg(jctx, UAX_REG_RDI, UAX_REG_RCX);
	BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, (s32)(tr->nxtpc));

//	UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);

	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		UAX_REG_RAX, UAX_REG_RDX, offsetof(BTESH2_Trace, trnext));
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext), UAX_REG_RAX);

#if 1
//	if(tr->jmppc)
//	if(1)
	if(tr->trjmpnext)
	{
		UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
			UAX_REG_RAX, UAX_REG_RDX, offsetof(BTESH2_Trace, trjmpnext));
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext), UAX_REG_RAX);
	}else if(tr->jmppc)
	{
		UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);
		UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
			UAX_REG_RDI, offsetof(BTESH2_CpuState, trjmpnext), UAX_REG_RAX);
	}
#endif

	for(i=0; i<tr->nops; i++)
	{
		if(BTESH2_TryJitOpcode(jctx, cpu, tr, tr->ops[i])>0)
			continue;

		BTESH2_JitSyncRegs(jctx, cpu, tr);

		UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (nlint)(tr->ops[i]));
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
//		UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(tr->ops[i]->Run));
//		UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
		BTESH2_JitEmitCallFPtr(jctx, cpu, tr->ops[i]->Run);
	}

//	UAX_AsmInsnRegReg(jctx, UAX_OP_XOR, UAX_REG_RAX, UAX_REG_RAX);

	BTESH2_JitSyncRegs(jctx, cpu, tr);

	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		UAX_REG_RAX, UAX_REG_RDI, offsetof(BTESH2_CpuState, trnext));

	BTESH2_JitSyncRestoreRegs(jctx, cpu, tr);

#if 1
	l0=UAX_GenLabelTemp(jctx);
//	l1=UAX_GenLabelTemp(jctx);

	/* validate we have next trace */
	UAX_AsmAndRegReg(jctx, UAX_REG_RAX, UAX_REG_RAX);
	UAX_AsmInsnLabel(jctx, UAX_OP_JE, l0|UAX_LBL_NEAR);

	UAX_AsmMovRegReg(jctx, UAX_REG_RDX, UAX_REG_RAX);

	/* validate we run-limit hasn't expired */
	UAX_AsmInsnStRegDisp32(jctx, UAX_OP_DEC,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, tr_runlim));
	UAX_AsmInsnLabel(jctx, UAX_OP_JBE, l0|UAX_LBL_NEAR);

	UAX_AsmMovRegImm(jctx, UAX_REG_EAX, tr->nwops);
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_ADD,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, tr_tops), UAX_REG_RAX);

	/* tail call into next trace */
	UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
	BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
	UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RSP, BTEJ_X64_FRAMEADJ);
	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		UAX_REG_RAX, UAX_REG_RDX, offsetof(BTESH2_Trace, Run));
	UAX_AsmInsnReg(jctx, UAX_OP_JMP, UAX_REG_RAX);

	UAX_EmitLabel(jctx, l0);		/* normal return */
#endif

	BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
//	BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);

	UAX_AsmInsnRegImm(jctx, UAX_OP_ADD, UAX_REG_RSP, BTEJ_X64_FRAMEADJ);
	UAX_AsmInsnNone(jctx, UAX_OP_RET);
	
	sz=UAX_EmitGetOffs(jctx);
	
	ptr=UAX_LinkContext(jctx);
	UAX_FreeContext(jctx);
	
	if(!ptr && (sz>0))
	{
		printf("BTESH2_TryJitTrace: ExHeap Full\n");
		cpu->jit_needflush=1;
		return(-1);
	}
	
	if(!ptr)
		return(0);
//	UAX_DumpMemHex(ptr, ptr+sz);
	tr->Run=(void *)ptr;
	BTESH2_JTraceAddTrace(cpu, tr);	
	
	if(!(tr->Run))
		__debugbreak();
	
	return(1);
}

#else

int BTESH2_TryJitTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	return(0);
}

BTESH2_Trace *BTESH2_JitTraceForAddr(BTESH2_CpuState *cpu, u32 spc)
{
	return(NULL);
}

#endif