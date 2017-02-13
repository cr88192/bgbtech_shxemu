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

int BTESH2_JitLoadVMReg(UAX_Context *jctx, int idx, int reg)
{
	UAX_AsmInsnRegLdRegDisp(jctx, UAX_OP_MOV,
		reg, UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4));
	return(0);
}

int BTESH2_JitStoreVMReg(UAX_Context *jctx, int idx, int reg)
{
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitStoreVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
//	UAX_AsmInsnStRegDispImm(jctx, UAX_OP_MOV,
//		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);

	UAX_AsmMovRegImm(jctx, UAX_REG_EAX, val);
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_MOV,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), UAX_REG_EAX);
	return(0);
}

int BTESH2_JitAddVMReg(UAX_Context *jctx, int idx, int reg)
{
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_ADD,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitSubVMReg(UAX_Context *jctx, int idx, int reg)
{
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_SUB,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitAndVMReg(UAX_Context *jctx, int idx, int reg)
{
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_AND,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), reg);
	return(0);
}

int BTESH2_JitAddVMRegImm(UAX_Context *jctx, int idx, s32 val)
{
//	UAX_AsmInsnStRegDispImm(jctx, UAX_OP_MOV,
//		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), val);

	UAX_AsmMovRegImm(jctx, UAX_REG_EAX, val);
	UAX_AsmInsnStRegDispReg(jctx, UAX_OP_ADD,
		UAX_REG_RDI, offsetof(BTESH2_CpuState, regs)+(idx*4), UAX_REG_EAX);
	return(0);
}

int BTESH2_TryJitOpcode(UAX_Context *jctx,
	BTESH2_CpuState *cpu, BTESH2_Opcode *op)
{
	if((op->nmid==BTESH2_NMID_MOV) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOV) && (op->fmid==BTESH2_FMID_REGIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_MOV) && (op->fmid==BTESH2_FMID_REGVIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitAddVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_ADD) && (op->fmid==BTESH2_FMID_REGIMM))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitAddVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

#if 1
	if((op->nmid==BTESH2_NMID_SUB) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitSubVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	if((op->nmid==BTESH2_NMID_AND) && (op->fmid==BTESH2_FMID_REGREG))
	{
		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		BTESH2_JitAndVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}
#endif

	if((op->nmid==BTESH2_NMID_MOVA) && (op->fmid==BTESH2_FMID_REGLDABS))
	{
//		BTESH2_JitLoadVMReg(jctx, op->rm, UAX_REG_EAX);
		UAX_AsmMovRegImm(jctx, UAX_REG_EAX, (s32)(op->imm));
		BTESH2_JitStoreVMReg(jctx, op->rn, UAX_REG_EAX);
		return(1);
	}

	return(0);
}

BTESH2_Trace *BTESH2_JitTraceForAddr(BTESH2_CpuState *cpu, u32 spc)
{
	BTESH2_Trace *tr0, *tr1, *tr2, *tr3;
	u32 spc1;
	int h, h1, h2, h3, hp, hp1;
	int i, j, k;
	
	hp=(spc^cpu->csfl)*BTESH2_TR_JHASHPR;
	h=(hp>>BTESH2_TR_JHASHSHR)&(BTESH2_TR_JHASHSZ-1);

	h0=h*4+0;
	tr0=cpu->jcache[h0];
	if(!tr0)
		return(NULL);
	if((tr0->srcpc==spc) && (tr0->csfl==cpu->csfl))
		{ return(tr); }

	h1=h*4+1;
	tr1=cpu->jcache[h1];
	if(!tr1)
		return(NULL);

	if((tr1->srcpc==spc) && (tr1->csfl==cpu->csfl))
	{
		cpu->jcache[h]=tr1;
		cpu->jcache[h1]=tr;
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
		return(tr2);
	}

	return(NULL);
}

int BTESH2_TryJitTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr)
{
	UAX_Context *jctx;
	byte *ptr;
	int i, j, k, sz;
	
	jctx=UAX_AllocContext();
	
	UAX_SetSection(jctx, UAX_CSEG_TEXT);
	
	UAX_AsmInsnRegImm(jctx, UAX_OP_SUB, UAX_REG_RSP, BTEJ_X64_FRAMEADJ);
	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_CPU, UAX_REG_RCX);
//	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_TRACE, UAX_REG_RDX);

	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
	BTESH2_JitStoreFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);
	UAX_AsmMovRegReg(jctx, UAX_REG_RDI, UAX_REG_RCX);
	BTESH2_JitStoreVMRegImm(jctx, BTESH2_REG_PC, tr->nxtpc);

	for(i=0; i<tr->nops; i++)
	{
		if(BTESH2_TryJitOpcode(jctx, cpu, tr->ops[i])>0)
			continue;

		UAX_AsmMovRegImm(jctx, UAX_REG_RDX, (nlint)(tr->ops[i]));
		UAX_AsmMovRegReg(jctx, UAX_REG_RCX, UAX_REG_RDI);
		UAX_AsmMovRegImm(jctx, UAX_REG_RAX, (nlint)(tr->ops[i]->Run));
		UAX_AsmInsnReg(jctx, UAX_OP_CALL, UAX_REG_RAX);
	}

	BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RDI, UAX_REG_RDI);
	BTESH2_JitLoadFrame(jctx, BTEJ_X64_FRIDX_RSI, UAX_REG_RSI);

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
	
//	UAX_DumpMemHex(ptr, ptr+sz);
	tr->Run=(void *)ptr;
	
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