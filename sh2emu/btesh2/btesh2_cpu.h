#ifndef BTESH2_CPU_H
#define BTESH2_CPU_H

#define BTESH2_ARCH_SH2				0x01
#define BTESH2_ARCH_SH4				0x02
#define BTESH2_ARCH_SH4_NOLINK		0x12
#define BTESH2_ARCH_NOLINK			0x10

#define BTESH2_ARFL_NOLINK			0x01
#define BTESH2_ARFL_NOJIT			0x02

#define BTESH2_SUBARCH_SH4			0x02
#define BTESH2_SUBARCH_BJX1_32		0x03
#define BTESH2_SUBARCH_BJX1_64		0x04
#define BTESH2_SUBARCH_BJX1_64C		0x05

#if 0

#define BTESH2_REG_R0		0

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

#define BTESH2_REG_RBANK	32		//R0B..R7B

#define BTESH2_REG_SSR		40
#define BTESH2_REG_SPC		41
#define BTESH2_REG_SGR		42
#define BTESH2_REG_DBR		43

#define BTESH2_REG_TRAEVT	44		//Trap Event
#define BTESH2_REG_EXPEVT	45		//Exception Event
#define BTESH2_REG_INTEVT	46		//Interrupt Event

#define BTESH2_REG_ZZR		47		//Null Register (Placeholder Reg)

#define BTESH2_REG_RBANK2	48		//

#define BTESH2_REG_RLO		0		//Register Low Halves
#define BTESH2_REG_RHI		64		//Register High Halves

#define BTESH2_REG_R0HI		64
#define BTESH2_REG_R15HI	79
#define BTESH2_REG_RBANK_HI	(BTESH2_REG_R0HI|BTESH2_REG_RBANK)

#endif


#if 1

#define BTESH2_REG_R0		0
#define BTESH2_REG_R1		1
#define BTESH2_REG_R2		2
#define BTESH2_REG_R3		3
#define BTESH2_REG_R4		4
#define BTESH2_REG_R5		5
#define BTESH2_REG_R6		6
#define BTESH2_REG_R7		7
#define BTESH2_REG_R8		8
#define BTESH2_REG_R9		9
#define BTESH2_REG_R10		10
#define BTESH2_REG_R11		11
#define BTESH2_REG_R12		12
#define BTESH2_REG_R13		13
#define BTESH2_REG_R14		14
#define BTESH2_REG_R15		15

#define BTESH2_REG_SP		15

#define BTESH2_REG_R16		16
#define BTESH2_REG_R31		31

#define BTESH2_REG_RBANK	32		//R0B..R7B
#define BTESH2_REG_RBANK2	40		//R16B..R23B


#define BTESH2_REG_MACH		48
#define BTESH2_REG_MACL		49
#define BTESH2_REG_PR		50
#define BTESH2_REG_SGR		51
#define BTESH2_REG_FLA		52		//FLA (internal)
#define BTESH2_REG_FPUL		53
#define BTESH2_REG_FPSCR	54
#define BTESH2_REG_FLB		55		//FLB (internal)

#define BTESH2_REG_DBR		63

#define BTESH2_REG_SR		64
#define BTESH2_REG_GBR		65
#define BTESH2_REG_VBR		66
#define BTESH2_REG_SSR		67
#define BTESH2_REG_SPC		68

#define BTESH2_REG_PC		71


#define BTESH2_REG_MMUCR	72
#define BTESH2_REG_PTEH		73
#define BTESH2_REG_PTEL		74
#define BTESH2_REG_TTB		75
#define BTESH2_REG_TEA		76

#define BTESH2_REG_TRAEVT	77		//Trap Event
#define BTESH2_REG_EXPEVT	78		//Exception Event
#define BTESH2_REG_INTEVT	79		//Interrupt Event
#define BTESH2_REG_ZZR		127		//Null Register (Placeholder Reg)

// #define BTESH2_REG_RLO		0		//Register Low Halves
// #define BTESH2_REG_RHI		128		//Register High Halves

//#define BTESH2_REG_R0HI		128
//#define BTESH2_REG_R15HI	(128+15)
//#define BTESH2_REG_RBANK_HI	(BTESH2_REG_R0HI|BTESH2_REG_RBANK)

#endif



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

#define BTESH2_EXC_FPUERR		0x20		//FPU Operation Error

#define BTESH2_EXC_TRAPSMC		0x1000		//Trapped Self-Modifying Code
#define BTESH2_EXC_TRAPSLEEP	0x1001		//Sleep
#define BTESH2_EXC_TLBMISS		0x1002		//TLB Miss
#define BTESH2_EXC_BREAK		0x1003		//Breakpoint

#define BTESH2_SRFL_T			0x00000001	//
#define BTESH2_SRFL_S			0x00000002	//

#define BTESH2_SRFL_DQ			0x00001000	//64-bit mode

#define BTESH2_SRFL_BL			0x10000000	//Block Ints
#define BTESH2_SRFL_RB			0x20000000	//Register Bank
#define BTESH2_SRFL_MD			0x40000000	//Mode (User/Super)
#define BTESH2_SRFL_JQ			0x80000000	//64-bit mode

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

#define BTESH2_FPSCR_VE			0x01000000	//

#define BTESH2_MMUCR_AT			0x00000001	//
#define BTESH2_MMUCR_TI			0x00000004	//
#define BTESH2_MMUCR_SV			0x00000100	//
#define BTESH2_MMUCR_SQMD		0x00000200	//

#define BTESH2_CSFL_LE			0x01	//Little Endian
#define BTESH2_CSFL_FPPR		0x02	//FPSCR.PR
#define BTESH2_CSFL_FPSZ		0x04	//FPSCR.SZ
#define BTESH2_CSFL_FPFR		0x08	//FPSCR.FR
#define BTESH2_CSFL_FPVE		0x10	//FPSCR.VE
#define BTESH2_CSFL_SRJQ		0x20	//SR.JQ
#define BTESH2_CSFL_SRDQ		0x40	//SR.DQ

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
#define BTESH2_NMID_MOVI20		0x63	//MOVI20
#define BTESH2_NMID_MOVI20S		0x64	//MOVI20S
#define BTESH2_NMID_MOVUB		0x65	//MOVU.B
#define BTESH2_NMID_MOVUW		0x66	//MOVU.W
#define BTESH2_NMID_MOVUL		0x67	//MOVU.L
#define BTESH2_NMID_LEAB		0x68	//LEA.B
#define BTESH2_NMID_LEAW		0x69	//LEA.W
#define BTESH2_NMID_LEAL		0x6A	//LEA.L
#define BTESH2_NMID_LEAQ		0x6B	//LEA.Q
#define BTESH2_NMID_LEAO		0x6C	//LEA.O
#define BTESH2_NMID_NOTT		0x6D	//
#define BTESH2_NMID_LDTLB		0x6E	//
#define BTESH2_NMID_BRK			0x6F	//
#define BTESH2_NMID_MOVRT		0x70	//
#define BTESH2_NMID_MOVCAL		0x71	//
#define BTESH2_NMID_PUSH		0x72	//
#define BTESH2_NMID_POP			0x73	//
#define BTESH2_NMID_ICLRMD		0x74	//
#define BTESH2_NMID_ISETMD		0x75	//
#define BTESH2_NMID_SHLL1		0x76	//
#define BTESH2_NMID_LDHF16		0x77	//
#define BTESH2_NMID_BRAN		0x78	//
#define BTESH2_NMID_BSRN		0x79	//
#define BTESH2_NMID_RTSN		0x7A	//
#define BTESH2_NMID_LDIF16		0x7B	//
#define BTESH2_NMID_STHF16		0x7C	//
#define BTESH2_NMID_CSELT		0x7D	//
#define BTESH2_NMID_EXTUL		0x7E	//
#define BTESH2_NMID_EXTSL		0x7F	//
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
#define BTESH2_NMID_FSRRA		0x96	//
#define BTESH2_NMID_FIPR		0x97	//
#define BTESH2_NMID_FTRV		0x98	//
#define BTESH2_NMID_PSETMD4		0x99	//
#define BTESH2_NMID_PSETMD12	0x9A	//
#define BTESH2_NMID_FMOVX		0x9B	//
#define BTESH2_NMID_FMOVIS		0x9C	//
#define BTESH2_NMID_FMOVID		0x9D	//
#define BTESH2_NMID_FMOVSI		0x9E	//
#define BTESH2_NMID_FMOVDI		0x9F	//

#define BTESH2_NMID_CMOVTB		0xA0	//CMOVT.B
#define BTESH2_NMID_CMOVTW		0xA1	//CMOVT.W
#define BTESH2_NMID_CMOVTL		0xA2	//CMOVT.L
#define BTESH2_NMID_CMOVTQ		0xA3	//CMOVT.Q
#define BTESH2_NMID_CMOVFB		0xA4	//CMOVF.B
#define BTESH2_NMID_CMOVFW		0xA5	//CMOVF.W
#define BTESH2_NMID_CMOVFL		0xA6	//CMOVF.L
#define BTESH2_NMID_CMOVFQ		0xA7	//CMOVF.Q

#define BTESH2_NMID_MOVI		0xC0	//
#define BTESH2_NMID_MOVIV		0xC1	//
#define BTESH2_NMID_PREF		0xC2	//
#define BTESH2_NMID_OCBI		0xC3	//
#define BTESH2_NMID_OCBP		0xC4	//
#define BTESH2_NMID_OCBWB		0xC5	//
#define BTESH2_NMID_ICBI		0xC6	//
#define BTESH2_NMID_MOVQ		0xC7	//SUB
#define BTESH2_NMID_ADDQ		0xC8	//ADD
#define BTESH2_NMID_SUBQ		0xC9	//SUB
#define BTESH2_NMID_MULQ		0xCA	//SUB
#define BTESH2_NMID_ANDQ		0xCB	//SUB
#define BTESH2_NMID_ORQ			0xCC	//SUB
#define BTESH2_NMID_XORQ		0xCD	//SUB
#define BTESH2_NMID_SHLLQ		0xCE	//SUB
#define BTESH2_NMID_SHLRQ		0xCF	//SUB
#define BTESH2_NMID_SHALQ		0xD0	//SUB
#define BTESH2_NMID_SHARQ		0xD1	//SUB
#define BTESH2_NMID_LDSH16		0xD2	//SUB
#define BTESH2_NMID_TSTQ		0xD3	//SUB
#define BTESH2_NMID_SHADQ		0xD4	//
#define BTESH2_NMID_SHLDQ		0xD5	//
#define BTESH2_NMID_SHLL4		0xD6	//
#define BTESH2_NMID_SHLR4		0xD7	//
#define BTESH2_NMID_BREQ		0xD8	//
#define BTESH2_NMID_BRNE		0xD9	//
#define BTESH2_NMID_BRGT		0xDA	//
#define BTESH2_NMID_BRLE		0xDB	//
#define BTESH2_NMID_BRGE		0xDC	//
#define BTESH2_NMID_BRLT		0xDD	//
#define BTESH2_NMID_ICLRMD_DQ	0xDE	//?
#define BTESH2_NMID_ISETMD_DQ	0xDF	//?
#define BTESH2_NMID_CMPQHS		0xE0	//CMPQ/HS
#define BTESH2_NMID_CMPQHI		0xE1	//CMPQ/HI
#define BTESH2_NMID_CMPQEQ		0xE2	//CMPQ/EQ
#define BTESH2_NMID_CMPQGE		0xE3	//CMPQ/GE
#define BTESH2_NMID_CMPQGT		0xE4	//CMPQ/GT
#define BTESH2_NMID_CMPQPZ		0xE5	//CMPQ/PZ
#define BTESH2_NMID_CMPQPL		0xE6	//CMPQ/PL


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
#define BTESH2_FMID_DREGREG		0x18	//DRm, DRn
#define BTESH2_FMID_DREGRM		0x19	//DRm
#define BTESH2_FMID_DREGRN		0x1A	//DRn
#define BTESH2_FMID_IMMSTRN		0x1B	//Imm, @Rn
#define BTESH2_FMID_IMMDECSTRN	0x1C	//Imm, @-Rn
#define BTESH2_FMID_REGREGREG	0x1D	//Rn, Rm, Ro
#define BTESH2_FMID_REGINCST	0x1E	//Rm, @+Rn
#define BTESH2_FMID_REGDECLD	0x1F	//@Rm-, Rn

#define BTESH2_FMID_REGSTRODISP		0x20	//Rm, @(Rn,Ro,Disp)
#define BTESH2_FMID_REGLDRODISP		0x21	//@(Rm,Ro,Disp), Rn
#define BTESH2_FMID_REGIMMREG		0x22	//Rm, Imm, Rn
#define BTESH2_FMID_FREGREGREG		0x23	//FRm, FRo, FRn
#define BTESH2_FMID_DREGREGREG		0x24	//DRm, DRo, DRn

#define BTESH2_FMID_FREGST			0x26	//FRm, @Rn
#define BTESH2_FMID_FREGLD			0x27	//@Rm, FRn
#define BTESH2_FMID_FREGDECST		0x28	//FRm, @-Rn
#define BTESH2_FMID_FREGINCLD		0x29	//@Rm+, FRn
#define BTESH2_FMID_FREGSTR0N		0x2A	//FRm, @(Ro+Rn)
#define BTESH2_FMID_FREGLDR0M		0x2B	//@(Ro+Rm), FRn
#define BTESH2_FMID_FREGSTDISP		0x2C	//FRm, @(Rn+Disp)
#define BTESH2_FMID_FREGLDDISP		0x2D	//@(Rm+Disp), FRn
#define BTESH2_FMID_FREGSTRODISP	0x2E	//FRm, @(Rn,Ro,Disp)
#define BTESH2_FMID_FREGLDRODISP	0x2F	//@(Rm,Ro,Disp), FRn

#define BTESH2_FMID_REG1ABS			0x30	//Rn, @(Abs)
#define BTESH2_FMID_REG2ABS			0x31	//Rm, Rn, @(Abs)

#define BTESH2_FMID_DREGST			0x36	//DRm, @Rn
#define BTESH2_FMID_DREGLD			0x37	//@Rm, DRn
#define BTESH2_FMID_DREGDECST		0x38	//DRm, @-Rn
#define BTESH2_FMID_DREGINCLD		0x39	//@Rm+, DRn
#define BTESH2_FMID_DREGSTR0N		0x3A	//DRm, @(Ro+Rn)
#define BTESH2_FMID_DREGLDR0M		0x3B	//@(Ro+Rm), DRn
#define BTESH2_FMID_DREGSTDISP		0x3C	//DRm, @(Rn+Disp)
#define BTESH2_FMID_DREGLDDISP		0x3D	//@(Rm+Disp), DRn
#define BTESH2_FMID_DREGSTRODISP	0x3E	//DRm, @(Rn,Ro,Disp)
#define BTESH2_FMID_DREGLDRODISP	0x3F	//@(Rm,Ro,Disp), DRn


typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

// typedef u32 btesh2_vaddr;
// typedef u32 btesh2_paddr;

typedef u64 btesh2_vaddr;
typedef u64 btesh2_paddr;

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

typedef struct BTESH2_LoaderImage_s BTESH2_LoaderImage;

/** Represents a physical memory span */
struct BTESH2_PhysSpan_s {
btesh2_paddr base;
btesh2_paddr limit;
btesh2_paddr range_n3;
byte *data;
char *name;
u32 (*GetB)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, btesh2_paddr reladdr);
u32 (*GetW)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, btesh2_paddr reladdr);
u32 (*GetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, btesh2_paddr reladdr);
u64 (*GetQ)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu, btesh2_paddr reladdr);

int (*SetB)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu,
	btesh2_paddr reladdr, u32 val);
int (*SetW)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu,
	btesh2_paddr reladdr, u32 val);
int (*SetD)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu,
	btesh2_paddr reladdr, u32 val);
int (*SetQ)(BTESH2_PhysSpan *sp, BTESH2_CpuState *cpu,
	btesh2_paddr reladdr, u64 val);

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
BTESH2_PhysSpan *t_span[128];
};

struct BTESH2_Opcode_s {
u16 opw;		//opcode word
u16 opw2;		//opcode word 2
byte rm;		//register Rm
byte rn;		//register Rn
byte ro;		//register for offsets
byte fl;		//opcode flags
byte nmid;		//name ID (disasm)
byte fmid;		//form ID (disasm)
btesh2_vaddr pc;
btesh2_vaddr imm;		//extended immediate or absolute value
void (*Run)(BTESH2_CpuState *cpu, BTESH2_Opcode *op);
};

//#define BTESH2_TR_MAXOPS	14
//#define BTESH2_TR_MAXOPS	22
//#define BTESH2_TR_MAXOPS	30
#define BTESH2_TR_MAXOPS	32

#define BTESH2_TR_MAXOPSH	24

// #define BTESH2_TR_HASHSZ	4096
// #define BTESH2_TR_HASHAS	2

//#define BTESH2_TR_HASHSZ	64
//#define BTESH2_TR_HASHSZ	256
//#define BTESH2_TR_HASHSZ	512
// #define BTESH2_TR_HASHSZ	1024
// #define BTESH2_TR_HASHSZ	2048
#define BTESH2_TR_HASHSZ	4096

// #define BTESH2_TR_HASHPR	524287
#define BTESH2_TR_HASHPR	262139
#define BTESH2_TR_HASHSHR	19

// #define BTESH2_TR_HASHPR	1048573
// #define BTESH2_TR_HASHSHR	21

// #define BTESH2_TR_HASHPR	4194301
// #define BTESH2_TR_HASHSHR	23

// #define BTESH2_TR_HASHPR	8388593
// #define BTESH2_TR_HASHSHR	23

#define BTESH2_TR_HASHLVL	2

#ifdef BTESH2_HASJIT
#define BTESH2_TR_JHASHPR	262139
#define BTESH2_TR_JHASHSHR	19
// #define BTESH2_TR_JHASHSZ	4096
#define BTESH2_TR_JHASHSZ	8192
#define BTESH2_TR_JHASHLVL	4
#endif

#define BTESH2_UAXJFL_PREJMP		1	//jump was handled early
#define BTESH2_UAXJFL_NOPREJMP		2	//disable pre-jump

#define BTESH2_TRJTFL_ICACHE		1
#define BTESH2_TRJTFL_JCACHE		2
#define BTESH2_TRJTFL_LINKED		4

#define BTESH2_TRJTFL_NOSTOMP_MASK	6
#define BTESH2_TRJTFL_NOFREE_MASK	7


#define BTESH2_FTOPFL_SEEN_DIV0S	1
#define BTESH2_FTOPFL_SEEN_DIV0U	2
#define BTESH2_FTOPFL_SEEN_DIV1		4

struct BTESH2_Trace_s {
u32 dbgpad3;
BTESH2_Opcode *ops[BTESH2_TR_MAXOPS];
u32 dbgpad4;
btesh2_vaddr srcpc;		//source PC for trace
btesh2_vaddr nxtpc;		//next PC
btesh2_vaddr minpc;		//max PC for trace
btesh2_vaddr maxpc;		//max PC for trace
btesh2_vaddr jmppc;		//jump PC
byte nops;		//number of ops in trace
byte nwops;		//number of word-ops in trace
byte amiss;
byte csfl;		//control state flags
byte jtrig;		//jit trigger count
byte jtflag;	//JIT flags
byte lnkcnt;	//link ref-count
u32 excnt;		//execution count
u32 dbgpad1;
BTESH2_Trace *(*Run)(BTESH2_CpuState *cpu, BTESH2_Trace *tr);
BTESH2_Trace *trnext;		//next trace to execute
BTESH2_Trace *trjmpnext;	//next trace to execute (on a jump)
BTESH2_Trace *lnknext;		//next trace in linked traces
u32 dbgpad2;
};

// struct BTESH2_FastTLB_s {
// u32 vpn;	//virtual page number
// u32 ppn;	//physical page number
// };

struct BTESH2_LoaderImage_s {
BTESH2_LoaderImage *next;
char *dllname;
btesh2_vaddr imgbase;		//image base address
int imgsize;				//image size
};

#if 0
typedef union btesh2_reggpr_u btesh2_reggpr;

union btesh2_reggpr_u {
u64 q;
struct {
u32 lo;
u32 hi;
};
};
#endif

#if 1
typedef struct btesh2_reggpr_u btesh2_reggpr;

struct btesh2_reggpr_u {
u64 q;
};
#endif

struct BTESH2_CpuState_s {
// u32 regs[64];
// u32 trapregs[64];	//trapped registers

// u32 regs[256];
// u32 trapregs[256];	//trapped registers

u32 dbgpad8;

btesh2_reggpr qregs[128];
btesh2_reggpr trapqregs[128];		//trapped registers

//u32 fregs[32];
//u32 trapfregs[32];

u32 fregs[64];
u32 trapfregs[64];

u32 dbgpad7;


BTESH2_Trace *icache[BTESH2_TR_HASHSZ*BTESH2_TR_HASHLVL];
#ifdef BTESH2_TR_JHASHSZ
BTESH2_Trace *jcache[BTESH2_TR_JHASHSZ*BTESH2_TR_JHASHLVL];
#endif

u32 trpc[64];
u32 trpc_hi[64];
byte trpc_csfl[64];

u32 dbgpad6;

int status;
byte trpc_rov;
int tr_dtot;
int tr_dcol;
int tr_runlim;	//run limit remaining
u64 tr_tops;	//total operations
s64 tr_tdt;		//total running time (ms)
byte csfl;		//control state flags
byte arch;		//arch ID
byte subarch;	//sub-arch ID
byte ctfl;		//control temp flags (don't effect control flow)
byte archfl;	//arch modifier flags
btesh2_vaddr ptcpc;		//trap PC

u32 ftopfl;		//feature option flags

BTESH2_Trace *cur_trace;
BTESH2_Trace *tr_next;
u32 *smcdbm[128];		//SMC detection bitmap
u64 tlbe[128];			//TLB entries
byte jit_needflush;
byte mmu_usetmmu;
int lsmc;

u64 tmmu_tlb[64][4];	//TLB for TLBMMU

u32 dbgpad5;

u32 dbg_lpc;
int dbg_ld;

BTESH2_MemoryImage *memory;
BTESH2_Opcode *free_opcode;
BTESH2_Trace *free_trace;
void *slabs;

BTESH2_PhysSpan *pspan;		//predict span
BTESH2_PhysSpan *pspanb;	//predict span (alt)
u32 dbgpad1;
BTESH2_Trace *trnext;
BTESH2_Trace *trjmpnext;

u32 dbgpad2;

u32 pspan_pbase;
u32 pspan_prng3;
byte *pspan_pdata;

u32 dbgpad3;

BTESH2_Trace *trlinked[256];	//linked traces

u32 dbgpad4;

int (*GetAddrByte)(BTESH2_CpuState *cpu, btesh2_vaddr addr);
int (*GetAddrWord)(BTESH2_CpuState *cpu, btesh2_vaddr addr);
u32 (*GetAddrDWord)(BTESH2_CpuState *cpu, btesh2_vaddr addr);
u64 (*GetAddrQWord)(BTESH2_CpuState *cpu, btesh2_vaddr addr);
int (*SetAddrByte)(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val);
int (*SetAddrWord)(BTESH2_CpuState *cpu, btesh2_vaddr addr, int val);
int (*SetAddrDWord)(BTESH2_CpuState *cpu, btesh2_vaddr addr, u32 val);
int (*SetAddrQWord)(BTESH2_CpuState *cpu, btesh2_vaddr addr, u64 val);
int (*OpLdTlb)(BTESH2_CpuState *cpu);

u32 *logpc;
u32 *logsp;
int nlogpc, mlogpc;

u32 *map_addr;
char **map_name;
int map_nsym, map_msym;

BTESH2_LoaderImage *loadimg;
};

u32 BTESH2_GetAddrDWordFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr);
int BTESH2_SetAddrDWordFMMU(BTESH2_CpuState *cpu, btesh2_vaddr addr, u32 val);

u32 BTESH2_GetAddrDWord(BTESH2_CpuState *cpu, btesh2_vaddr addr);
int BTESH2_SetAddrDWord(BTESH2_CpuState *cpu, btesh2_vaddr addr, u32 val);

BTESH2_Opcode *BTESH2_AllocOpcode(BTESH2_CpuState *cpu);
void BTESH2_FreeOpcode(BTESH2_CpuState *cpu, BTESH2_Opcode *op);
BTESH2_Trace *BTESH2_AllocTrace(BTESH2_CpuState *cpu);
void BTESH2_FreeTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr);
void BTESH2_FlushTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr);

force_inline BTESH2_Trace *BTESH2_TraceForAddr(BTESH2_CpuState *cpu, 
	btesh2_vaddr spc);

int BTESH2_DecodeOpcode_2A(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2);
int BTESH2_DecodeOpcode_BJX1(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2);
int BTESH2_DecodeOpcode_BJX1_8E(BTESH2_CpuState *cpu, BTESH2_Opcode *op,
	btesh2_vaddr pc, u16 opw, u16 opw2);

void BTESH2_ValidateTrace(BTESH2_CpuState *cpu, BTESH2_Trace *tr);

// #define BTESH2_GetRegDWord(cpu, rm)			((s32)(cpu->qregs[rm].q))
// #define BTESH2_SetRegDWord(cpu, rm, val)	((cpu->qregs[rm].q)=((s32)(val)))

#define BTESH2_GetRegDWordSx(cpu, rm)		((s32)(cpu->qregs[rm].q))
#define BTESH2_SetRegDWordSx(cpu, rm, val)	((cpu->qregs[rm].q)=((s32)(val)))

#define BTESH2_GetRegDWordZx(cpu, rm)		((u32)(cpu->qregs[rm].q))
#define BTESH2_SetRegDWordZx(cpu, rm, val)	((cpu->qregs[rm].q)=((u32)(val)))

#define BTESH2_GetRegQWord(cpu, rm)			((u64)(cpu->qregs[rm].q))
#define BTESH2_SetRegQWord(cpu, rm, val)	((cpu->qregs[rm].q)=((u64)(val)))

// #define BTESH2_GetRegDWordL(cpu, rm)		((s32)(cpu->qregs[rm].q))
// #define BTESH2_GetRegDWordH(cpu, rm)		((s32)((cpu->qregs[rm].q)>>32))

#define BTESH2_GetRegDWordL(cpu, rm)		(((u32 *)(cpu->qregs+(rm)))[0])
#define BTESH2_GetRegDWordH(cpu, rm)		(((u32 *)(cpu->qregs+(rm)))[1])

#define BTESH2_SetRegDWordL(cpu, rm, val)	\
	(((u32 *)(cpu->qregs+(rm)))[0]=((s32)(val)))
#define BTESH2_SetRegDWordH(cpu, rm, val)	\
	(((u32 *)(cpu->qregs+(rm)))[1]=((s32)(val)))

#define BTESH2_GetRegDWord(cpu, rm)			\
	(((u32 *)(cpu->qregs+(rm)))[0])
#define BTESH2_SetRegDWord(cpu, rm, val)	\
	(((u32 *)(cpu->qregs+(rm)))[0]=((s32)(val)))

#endif
