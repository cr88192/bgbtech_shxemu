#ifndef BTESH2_CPU_H
#define BTESH2_CPU_H

#define BTESH2_ARCH_SH2				0x01
#define BTESH2_ARCH_SH4				0x02
#define BTESH2_ARCH_SH4_NOLINK		0x12
#define BTESH2_ARCH_NOLINK			0x10

#define BTESH2_ARFL_NOLINK			0x01
#define BTESH2_ARFL_NOJIT			0x02

#define BTESH2_REG_SP		15

#define BTESH2_REG_SR		16
#define BTESH2_REG_GBR		17
#define BTESH2_REG_VBR		18
#define BTESH2_REG_FLA		19

#define BTESH2_REG_MACH		20
#define BTESH2_REG_MACL		21
#define BTESH2_REG_PR		22
#define BTESH2_REG_PC		23

#define BTESH2_REG_FPSCR	24
#define BTESH2_REG_FPUL		25
#define BTESH2_REG_FLB		26

#define BTESH2_REG_MMUCR	27
#define BTESH2_REG_PTEH		28
#define BTESH2_REG_PTEL		29
#define BTESH2_REG_TTB		30
#define BTESH2_REG_TEA		31

#define BTESH2_REG_RBANK	32

#define BTESH2_REG_SSR		40
#define BTESH2_REG_SPC		41
#define BTESH2_REG_SGR		42
#define BTESH2_REG_DBR		43

#define BTESH2_REG_ZZR		47		//Null Register (Placeholder Reg)


#define BTESH2_OPFL_CTRLF		1	//opcode effects control-flow
#define BTESH2_OPFL_FAULT		2	//opcode may fault
#define BTESH2_OPFL_DLYSLOT		4	//delay-slot
#define BTESH2_OPFL_INVDLYSLOT	8	//invalid in delay-slot
#define BTESH2_OPFL_PCADLYSLOTW	16	//adjust PC in delay slot (W)
#define BTESH2_OPFL_PCADLYSLOTD	32	//adjust PC in delay slot (D)
#define BTESH2_OPFL_EXTRAWORD	64	//uses an extra word

#define BTESH2_SPFL_SIMPLEMEM_LE	1	//span is simple-case memory (LE)

#define BTESH2_EXC_POWERON		0x00		//power on
#define BTESH2_EXC_STACK1		0x01		//first stack
#define BTESH2_EXC_RESET		0x02		//reset
#define BTESH2_EXC_STACK2		0x03		//second stack
#define BTESH2_EXC_UDINST		0x04		//illegal instruction
#define BTESH2_EXC_SLUDINST		0x06		//slot illegal instruction
#define BTESH2_EXC_INVADDR		0x09		//invalid address
#define BTESH2_EXC_DMAINVADDR	0x0A		//DMAC/DTC address error
#define BTESH2_EXC_NMI			0x0B		//NMI
#define BTESH2_EXC_UBC			0x0C		//UBC
#define BTESH2_EXC_PIT			0x10		//PIT
#define BTESH2_EXC_EMAC			0x11		//EMAC interface
#define BTESH2_EXC_UART0		0x12		//UART/Console (0)
#define BTESH2_EXC_UART2		0x13		//UART/Console (2)
#define BTESH2_EXC_GPIO			0x15		//GPIO
#define BTESH2_EXC_UART1		0x17		//UART/Console (1)
#define BTESH2_EXC_AICCNTDN		0x19		//AIC Countdown

#define BTESH2_EXC_TRAPSMC		0x1000		//Trapped Self-Modifying Code
#define BTESH2_EXC_TRAPSLEEP	0x1001		//Sleep

#define BTESH2_SRFL_T			0x00000001	//
#define BTESH2_SRFL_S			0x00000002	//

#define BTESH2_SRFL_BL			0x10000000	//Block Ints
#define BTESH2_SRFL_RB			0x20000000	//Register Bank
#define BTESH2_SRFL_MD			0x40000000	//Mode (User/Super)

#define BTESH2_FPSCR_RM0		0x00000000	//
#define BTESH2_FPSCR_RM1		0x00000001	//
#define BTESH2_FPSCR_RM2		0x00000002	//
#define BTESH2_FPSCR_RM3		0x00000003	//
#define BTESH2_FPSCR_RMM		0x00000003	//

#define BTESH2_FPSCR_FI			0x00000004	//
#define BTESH2_FPSCR_FU			0x00000008	//
#define BTESH2_FPSCR_FO			0x00000010	//
#define BTESH2_FPSCR_FZ			0x00000020	//
#define BTESH2_FPSCR_FV			0x00000040	//
#define BTESH2_FPSCR_EI			0x00000080	//
#define BTESH2_FPSCR_EU			0x00000100	//
#define BTESH2_FPSCR_EO			0x00000200	//
#define BTESH2_FPSCR_EZ			0x00000400	//
#define BTESH2_FPSCR_EV			0x00000800	//
#define BTESH2_FPSCR_CI			0x00001000	//
#define BTESH2_FPSCR_CU			0x00002000	//
#define BTESH2_FPSCR_CO			0x00004000	//
#define BTESH2_FPSCR_CZ			0x00008000	//
#define BTESH2_FPSCR_CV			0x00010000	//
#define BTESH2_FPSCR_CE			0x00020000	//
#define BTESH2_FPSCR_DN			0x00040000	//
#define BTESH2_FPSCR_PR			0x00080000	//
#define BTESH2_FPSCR_SZ			0x00100000	//
#define BTESH2_FPSCR_FR			0x00200000	//

#define BTESH2_MMUCR_AT			0x00000001	//
#define BTESH2_MMUCR_TI			0x00000004	//
#define BTESH2_MMUCR_SV			0x00000100	//
#define BTESH2_MMUCR_SQMD		0x00000200	//

#define BTESH2_CSFL_LE			0x01	//Little Endian
#define BTESH2_CSFL_FPPR		0x02	//FPSCR.PR
#define BTESH2_CSFL_FPSZ		0x04	//FPSCR.SZ
#define BTESH2_CSFL_FPFR		0x08	//FPSCR.FR
#define BTESH2_CSFL_FPVE		0x10	//FPSCR.VE

#define BTESH2_NMID_UNK			0x00	//Unknown
#define BTESH2_NMID_MOV			0x01	//MOV
#define BTESH2_NMID_MOVB		0x02	//MOV.B
#define BTESH2_NMID_MOVW		0x03	//MOV.W
#define BTESH2_NMID_MOVL		0x04	//MOV.L
#define BTESH2_NMID_ADD			0x05	//ADD
#define BTESH2_NMID_ADDC		0x06	//ADDC
#define BTESH2_NMID_ADDV		0x07	//ADDV
#define BTESH2_NMID_SUB			0x08	//SUB
#define BTESH2_NMID_SUBC		0x09	//SUBC
#define BTESH2_NMID_SUBV		0x0A	//SUBV
#define BTESH2_NMID_CMPHS		0x0B	//CMPHS
#define BTESH2_NMID_CMPHI		0x0C	//CMPHI
#define BTESH2_NMID_CMPEQ		0x0D	//CMPEQ
#define BTESH2_NMID_CMPGE		0x0E	//CMPGE
#define BTESH2_NMID_CMPGT		0x0F	//CMPGT
#define BTESH2_NMID_JMP			0x10	//JMP
#define BTESH2_NMID_JSR			0x11	//JSR
#define BTESH2_NMID_BRA			0x12	//BRA
#define BTESH2_NMID_BSR			0x13	//BSR
#define BTESH2_NMID_BT			0x14	//BT
#define BTESH2_NMID_BF			0x15	//BF
#define BTESH2_NMID_BTS			0x16	//BTS
#define BTESH2_NMID_BFS			0x17	//BFS
#define BTESH2_NMID_DIV1		0x18	//DIV1
#define BTESH2_NMID_DMULU		0x19	//DMULU
#define BTESH2_NMID_DMULS		0x1A	//DMULS
#define BTESH2_NMID_TST			0x1B	//TST
#define BTESH2_NMID_AND			0x1C	//AND
#define BTESH2_NMID_XOR			0x1D	//XOR
#define BTESH2_NMID_OR			0x1E	//OR
#define BTESH2_NMID_INVALID		0x1F	//Invalid
#define BTESH2_NMID_DIV0S		0x20	//
#define BTESH2_NMID_CMPSTR		0x21	//
#define BTESH2_NMID_MULUW		0x22	//
#define BTESH2_NMID_MULSW		0x23	//
#define BTESH2_NMID_MACL		0x24	//
#define BTESH2_NMID_BSRF		0x25	//
#define BTESH2_NMID_BRAF		0x26	//
#define BTESH2_NMID_MULL		0x27	//
#define BTESH2_NMID_CLRT		0x28	//
#define BTESH2_NMID_SETT		0x29	//
#define BTESH2_NMID_CLRMAC		0x2A	//
#define BTESH2_NMID_NOP			0x2B	//
#define BTESH2_NMID_MOVT		0x2C	//
#define BTESH2_NMID_RTS			0x2D	//
#define BTESH2_NMID_SLEEP		0x2E	//
#define BTESH2_NMID_RTE			0x2F	//
#define BTESH2_NMID_SHLL		0x30	//
#define BTESH2_NMID_DT			0x31	//
#define BTESH2_NMID_SHAL		0x32	//
#define BTESH2_NMID_SHLR		0x33	//
#define BTESH2_NMID_CMPPZ		0x34	//
#define BTESH2_NMID_SHAR		0x35	//
#define BTESH2_NMID_ROTL		0x36	//
#define BTESH2_NMID_ROTCL		0x37	//
#define BTESH2_NMID_ROTT		0x38	//
#define BTESH2_NMID_ROTCR		0x39	//
#define BTESH2_NMID_SHLL2		0x3A	//
#define BTESH2_NMID_ROTR		0x3B	//
#define BTESH2_NMID_CMPPL		0x3C	//
#define BTESH2_NMID_SHLL8		0x3D	//
#define BTESH2_NMID_SHLL16		0x3E	//
#define BTESH2_NMID_SHLR2		0x3F	//
#define BTESH2_NMID_SHLR8		0x40	//
#define BTESH2_NMID_SHLR16		0x41	//
#define BTESH2_NMID_MACW		0x42	//
#define BTESH2_NMID_NOT			0x43	//
#define BTESH2_NMID_SWAPB		0x44	//
#define BTESH2_NMID_SWAPW		0x45	//
#define BTESH2_NMID_NEGC		0x46	//
#define BTESH2_NMID_NEG			0x47	//
#define BTESH2_NMID_EXTUB		0x48	//
#define BTESH2_NMID_EXTUW		0x49	//
#define BTESH2_NMID_EXTSB		0x4A	//
#define BTESH2_NMID_EXTSW		0x4B	//
#define BTESH2_NMID_TRAPA		0x4C	//
#define BTESH2_NMID_MOVA		0x4D	//
#define BTESH2_NMID_TSTB		0x4E	//
#define BTESH2_NMID_DIV0U		0x4F	//

#define BTESH2_NMID_LDC			0x50	//
#define BTESH2_NMID_LDCL		0x51	//
#define BTESH2_NMID_LDS			0x52	//
#define BTESH2_NMID_LDSL		0x53	//
#define BTESH2_NMID_STC			0x54	//
#define BTESH2_NMID_STCL		0x55	//
#define BTESH2_NMID_STS			0x56	//
#define BTESH2_NMID_STSL		0x57	//
#define BTESH2_NMID_CASL		0x58	//
#define BTESH2_NMID_TASB		0x59	//
#define BTESH2_NMID_XTRCT		0x5A	//
#define BTESH2_NMID_ANDB		0x5B	//
#define BTESH2_NMID_XORB		0x5C	//
#define BTESH2_NMID_ORB			0x5D	//
#define BTESH2_NMID_SHAD		0x5E	//
#define BTESH2_NMID_SHLD		0x5F	//
#define BTESH2_NMID_INVDLY		0x60	//
#define BTESH2_NMID_CLRS		0x61	//
#define BTESH2_NMID_SETS		0x62	//

#define BTESH2_NMID_FABS		0x80	//
#define BTESH2_NMID_FADD		0x81	//
#define BTESH2_NMID_FCMPEQ		0x82	//
#define BTESH2_NMID_FCMPGT		0x83	//
#define BTESH2_NMID_FCNVDS		0x84	//
#define BTESH2_NMID_FCNVSD		0x85	//
#define BTESH2_NMID_FDIV		0x86	//
#define BTESH2_NMID_FLDI0		0x87	//
#define BTESH2_NMID_FLDI1		0x88	//
#define BTESH2_NMID_FLDS		0x89	//
#define BTESH2_NMID_FLOAT		0x8A	//
#define BTESH2_NMID_FMAC		0x8B	//
#define BTESH2_NMID_FMOV		0x8C	//
#define BTESH2_NMID_FMOVS		0x8D	//
#define BTESH2_NMID_FMOVD		0x8E	//
#define BTESH2_NMID_FMUL		0x8F	//
#define BTESH2_NMID_FNEG		0x90	//
#define BTESH2_NMID_FSCHG		0x91	//
#define BTESH2_NMID_FSQRT		0x92	//
#define BTESH2_NMID_FSTS		0x93	//
#define BTESH2_NMID_FSUB		0x94	//
#define BTESH2_NMID_FTRC		0x95	//
#define BTESH2_NMID_MOVCAL		0x96	//
#define BTESH2_NMID_FIPR		0x97	//
#define BTESH2_NMID_FTRV		0x98	//

#define BTESH2_NMID_MOVI		0xC0	//
#define BTESH2_NMID_MOVIV		0xC1	//

#define BTESH2_NMID_PREF		0xC2	//
#define BTESH2_NMID_OCBI		0xC3	//
#define BTESH2_NMID_OCBP		0xC4	//
#define BTESH2_NMID_OCBWB		0xC5	//
#define BTESH2_NMID_ICBI		0xC6	//


#define BTESH2_FMID_REGREG		0x01	//Rm, Rn
#define BTESH2_FMID_REGRM		0x02	//Rm
#define BTESH2_FMID_REGRN		0x03	//Rn
#define BTESH2_FMID_REGIMM		0x04	//Imm, Rn
#define BTESH2_FMID_REGLDABS	0x05	//@(Abs), Rn
#define BTESH2_FMID_REGST		0x06	//Rm, @Rn
#define BTESH2_FMID_REGLD		0x07	//@Rm, Rn
#define BTESH2_FMID_REGDECST	0x08	//Rm, @-Rn
#define BTESH2_FMID_REGINCLD	0x09	//@Rm+, Rn
#define BTESH2_FMID_REGSTR0N	0x0A	//Rm, @(Ro+Rn)
#define BTESH2_FMID_REGLDR0M	0x0B	//@(Ro+Rm), Rn
#define BTESH2_FMID_REGSTDISP	0x0C	//Rm, @(Rn+Disp)
#define BTESH2_FMID_REGLDDISP	0x0D	//@(Rm+Disp), Rn
#define BTESH2_FMID_ABS			0x0E	//@(Abs)
#define BTESH2_FMID_NONE		0x0F	//-
#define BTESH2_FMID_IMM			0x10	//Imm

#define BTESH2_FMID_LDREG		0x11	//@Rm
#define BTESH2_FMID_STREG		0x12	//@Rn
#define BTESH2_FMID_IMMSTRMN	0x13	//Imm, @(Rm+Rn)
#define BTESH2_FMID_REGVIMM		0x14	//<Imm>, Rn

#define BTESH2_FMID_FREGREG		0x15	//FRm, FRn
#define BTESH2_FMID_FREGRM		0x16	//FRm
#define BTESH2_FMID_FREGRN		0x17	//FRn

#define BTESH2_FMID_DREGREG		0x18	//FRm, FRn
#define BTESH2_FMID_DREGRM		0x19	//FRm
#define BTESH2_FMID_DREGRN		0x1A	//FRn
#define BTESH2_FMID_IMMSTRN		0x1B	//Imm, @Rn
#define BTESH2_FMID_IMMDECSTRN	0x1C	//Imm, @-Rn

#define BTESH2_FMID_REGREGREG	0x1D	//Rn, Rm, Ro


typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#ifndef __cplusplus
#ifndef _BOOL_T
typedef char bool;
#define true 1
#define false 0
#endif
#endif

typedef struct BTESH2_PhysSpan_s BTESH2_PhysSpan;
typedef struct BTESH2_MemoryImage_s BTESH2_MemoryImage;

typedef struct BTESH2_Opcode_s BTESH2_Opcode;
typedef struct BTESH2_Trace_s BTESH2_Trace;
typedef struct BTESH2_CpuState_s BTESH2_CpuState;

/** Represents a physical memory span */
struct BTESH2_PhysSpan_s {
u32 base;
u32 limit;
u32 range_n3;
byte *data;
char *name;
u32 (*GetB)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, u32 reladdr);
u32 (*GetW)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, u32 reladdr);
u32 (*GetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, u32 reladdr);
int (*SetB)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, u32 reladdr, u32 val);
int (*SetW)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, u32 reladdr, u32 val);
int (*SetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, u32 reladdr, u32 val);

byte **dmdaddr;
int *dmdflag;
int flags;
};

/** Represents a physical memory image.
  * MMU stuff, if done, is to be handled by CPU image.
  */
struct BTESH2_MemoryImage_s {
BTESH2_PhysSpan **span;		//spans for image
// BTESH2_PhysSpan *pspan;		//predict span
// BTESH2_PhysSpan *pspanb;	//predict span (alt)
int nspan, mspan;
BTESH2_PhysSpan *t_span[32];
};

struct BTESH2_Opcode_s {
u16 opw;		//opcode word
byte rm;		//register Rm
byte rn;		//register Rn
byte ro;		//register for offsets
byte fl;		//opcode flags
byte nmid;		//name ID (disasm)
byte fmid;		//form ID (disasm)
u32 pc;
u32 imm;		//extended immediate or absolute value
void (*Run)(BTESH2_CpuState *cpu, BTESH2_Opcode *op);
};

//#define BTESH2_TR_MAXOPS	14
//#define BTESH2_TR_MAXOPS	22
//#define BTESH2_TR_MAXOPS	30
#define BTESH2_TR_MAXOPS	32

// #define BTESH2_TR_HASHSZ	4096
// #define BTESH2_TR_HASHAS	2

//#define BTESH2_TR_HASHSZ	64
//#define BTESH2_TR_HASHSZ	256
#define BTESH2_TR_HASHSZ	512
// #define BTESH2_TR_HASHSZ	1024
//#define BTESH2_TR_HASHSZ	2048
// #define BTESH2_TR_HASHSZ	4096

// #define BTESH2_TR_HASHPR	524287
// #define BTESH2_TR_HASHPR	262139
// #define BTESH2_TR_HASHSHR	19

#define BTESH2_TR_HASHPR	1048573
#define BTESH2_TR_HASHSHR	21

// #define BTESH2_TR_HASHPR	4194301
// #define BTESH2_TR_HASHSHR	23

// #define BTESH2_TR_HASHPR	8388593
// #define BTESH2_TR_HASHSHR	23

#define BTESH2_TR_HASHLVL	2

#ifdef BTESH2_HASJIT
#define BTESH2_TR_JHASHPR	262139
#define BTESH2_TR_JHASHSHR	19
#define BTESH2_TR_JHASHSZ	4096
#define BTESH2_TR_JHASHLVL	4
#endif

#define BTESH2_UAXJFL_PREJMP		1	//jump was handled early
#define BTESH2_UAXJFL_NOPREJMP		2	//disable pre-jump

#define BTESH2_TRJTFL_ICACHE		1
#define BTESH2_TRJTFL_JCACHE		2
#define BTESH2_TRJTFL_LINKED		4

#define BTESH2_TRJTFL_NOSTOMP_MASK	6
#define BTESH2_TRJTFL_NOFREE_MASK	7

struct BTESH2_Trace_s {
BTESH2_Opcode *ops[BTESH2_TR_MAXOPS];
u32 srcpc;		//source PC for trace
u32 nxtpc;		//next PC
u32 maxpc;		//max PC for trace
u32 jmppc;		//jump PC
byte nops;		//number of ops in trace
byte nwops;		//number of word-ops in trace
byte amiss;
byte csfl;		//control state flags
byte jtrig;		//jit trigger count
byte jtflag;	//JIT flags
byte lnkcnt;	//link ref-count
u32 excnt;		//execution count
BTESH2_Trace *(*Run)(BTESH2_CpuState *cpu, BTESH2_Trace *tr);
BTESH2_Trace *trnext;		//next trace to execute
BTESH2_Trace *trjmpnext;	//next trace to execute (on a jump)
BTESH2_Trace *lnknext;		//next trace in linked traces
};

// struct BTESH2_FastTLB_s {
// u32 vpn;	//virtual page number
// u32 ppn;	//physical page number
// };

struct BTESH2_CpuState_s {
u32 regs[64];
u32 trapregs[64];	//trapped registers

u32 fregs[32];
u32 trapfregs[32];

BTESH2_Trace *icache[BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL];
#ifdef BTESH2_TR_JHASHSZ
BTESH2_Trace *jcache[BTESH2_TR_JHASHSZ*BTESH2_TR_JHASHLVL];
#endif
u32 trpc[64];
int status;
byte trpc_rov;
int tr_dtot;
int tr_dcol;
int tr_runlim;	//run limit remaining
u64 tr_tops;	//total operations
s64 tr_tdt;		//total running time (ms)
byte csfl;		//control state flags
byte arch;		//arch ID
byte ctfl;		//control temp flags (don't effect control flow)
byte archfl;	//arch modifier flags
u32 ptcpc;

BTESH2_Trace *cur_trace;
u32 *smcdbm[128];		//SMC detection bitmap
u64 tlbe[128];			//TLB entries
byte jit_needflush;
int lsmc;

u32 dbg_lpc;
int dbg_ld;

BTESH2_MemoryImage *memory;
BTESH2_Opcode *free_opcode;
BTESH2_Trace *free_trace;
void *slabs;

BTESH2_PhysSpan *pspan;		//predict span
BTESH2_PhysSpan *pspanb;	//predict span (alt)
BTESH2_Trace *trnext;
BTESH2_Trace *trjmpnext;

u32 pspan_pbase;
u32 pspan_prng3;
byte *pspan_pdata;

BTESH2_Trace *trlinked[256];	//linked traces

int (*GetAddrByte)(BTESH2_CpuState *cpu, u32 addr);
int (*GetAddrWord)(BTESH2_CpuState *cpu, u32 addr);
u32 (*GetAddrDWord)(BTESH2_CpuState *cpu, u32 addr);
int (*SetAddrByte)(BTESH2_CpuState *cpu, u32 addr, int val);
int (*SetAddrWord)(BTESH2_CpuState *cpu, u32 addr, int val);
int (*SetAddrDWord)(BTESH2_CpuState *cpu, u32 addr, u32 val);

u32 *logpc;
u32 *logsp;
int nlogpc, mlogpc;

u32 *map_addr;
char **map_name;
int map_nsym, map_msym;
};

u32 BTESH2_GetAddrDWordFMMU(BTESH2_CpuState *cpu, u32 addr);
int BTESH2_SetAddrDWordFMMU(BTESH2_CpuState *cpu, u32 addr, u32 val);

u32 BTESH2_GetAddrDWord(BTESH2_CpuState *cpu, u32 addr);
int BTESH2_SetAddrDWord(BTESH2_CpuState *cpu, u32 addr, u32 val);

BTESH2_Opcode *BTESH2_AllocOpcode(BTESH2_CpuState *cpu);
void BTESH2_FreeOpcode(BTESH2_CpuState *cpu, BTESH2_Opcode *op);
BTESH2_Trace *BTESH2_AllocTrace(BTESH2_CpuState *cpu);
void BTESH2_FreeTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr);
void BTESH2_FlushTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr);

force_inline BTESH2_Trace *BTESH2_TraceForAddr(BTESH2_CpuState *cpu, u32 spc);

#endif
