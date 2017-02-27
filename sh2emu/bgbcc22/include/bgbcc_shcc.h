/*
 * SH-ISA
 */

#define BGBCC_SH_REG_R0			0x00

#define BGBCC_SH_REG_SP			0x0F

#define BGBCC_SH_REG_SR			0x10
#define BGBCC_SH_REG_GBR		0x11
#define BGBCC_SH_REG_VBR		0x12
#define BGBCC_SH_REG_SSR		0x13
#define BGBCC_SH_REG_SPC		0x14

#define BGBCC_SH_REG_R0B		0x18
#define BGBCC_SH_REG_R1B		0x19
#define BGBCC_SH_REG_R2B		0x1A
#define BGBCC_SH_REG_R3B		0x1B
#define BGBCC_SH_REG_R4B		0x1C
#define BGBCC_SH_REG_R5B		0x1D
#define BGBCC_SH_REG_R6B		0x1E
#define BGBCC_SH_REG_R7B		0x1F

#define BGBCC_SH_REG_MACH		0x20	//S
#define BGBCC_SH_REG_MACL		0x21	//S
#define BGBCC_SH_REG_PR			0x22	//S
#define BGBCC_SH_REG_SGR		0x23	//C

#define BGBCC_SH_REG_FPUL		0x25
#define BGBCC_SH_REG_FPSCR		0x26

#define BGBCC_SH_REG_DBR		0x2F

#define BGBCC_SH_REG_PC			0x3E
#define BGBCC_SH_REG_ZZR		0xFF		//Null Register (Placeholder Reg)

#define BGBCC_SH_NMID_UNK			0x00	//Unknown
#define BGBCC_SH_NMID_MOV			0x01	//MOV
#define BGBCC_SH_NMID_MOVB			0x02	//MOV.B
#define BGBCC_SH_NMID_MOVW			0x03	//MOV.W
#define BGBCC_SH_NMID_MOVL			0x04	//MOV.L
#define BGBCC_SH_NMID_ADD			0x05	//ADD
#define BGBCC_SH_NMID_ADDC			0x06	//ADDC
#define BGBCC_SH_NMID_ADDV			0x07	//ADDV
#define BGBCC_SH_NMID_SUB			0x08	//SUB
#define BGBCC_SH_NMID_SUBC			0x09	//SUBC
#define BGBCC_SH_NMID_SUBV			0x0A	//SUBV
#define BGBCC_SH_NMID_CMPHS			0x0B	//CMPHS
#define BGBCC_SH_NMID_CMPHI			0x0C	//CMPHI
#define BGBCC_SH_NMID_CMPEQ			0x0D	//CMPEQ
#define BGBCC_SH_NMID_CMPGE			0x0E	//CMPGE
#define BGBCC_SH_NMID_CMPGT			0x0F	//CMPGT
#define BGBCC_SH_NMID_JMP			0x10	//JMP
#define BGBCC_SH_NMID_JSR			0x11	//JSR
#define BGBCC_SH_NMID_BRA			0x12	//BRA
#define BGBCC_SH_NMID_BSR			0x13	//BSR
#define BGBCC_SH_NMID_BT			0x14	//BT
#define BGBCC_SH_NMID_BF			0x15	//BF
#define BGBCC_SH_NMID_BTS			0x16	//BTS
#define BGBCC_SH_NMID_BFS			0x17	//BFS
#define BGBCC_SH_NMID_DIV1			0x18	//DIV1
#define BGBCC_SH_NMID_DMULU			0x19	//DMULU
#define BGBCC_SH_NMID_DMULS			0x1A	//DMULS
#define BGBCC_SH_NMID_TST			0x1B	//TST
#define BGBCC_SH_NMID_AND			0x1C	//AND
#define BGBCC_SH_NMID_XOR			0x1D	//XOR
#define BGBCC_SH_NMID_OR			0x1E	//OR
#define BGBCC_SH_NMID_INVALID		0x1F	//Invalid
#define BGBCC_SH_NMID_DIV0S			0x20	//
#define BGBCC_SH_NMID_CMPSTR		0x21	//
#define BGBCC_SH_NMID_MULUW			0x22	//
#define BGBCC_SH_NMID_MULSW			0x23	//
#define BGBCC_SH_NMID_MACL			0x24	//
#define BGBCC_SH_NMID_BSRF			0x25	//
#define BGBCC_SH_NMID_BRAF			0x26	//
#define BGBCC_SH_NMID_MULL			0x27	//
#define BGBCC_SH_NMID_CLRT			0x28	//
#define BGBCC_SH_NMID_SETT			0x29	//
#define BGBCC_SH_NMID_CLRMAC		0x2A	//
#define BGBCC_SH_NMID_NOP			0x2B	//
#define BGBCC_SH_NMID_MOVT			0x2C	//
#define BGBCC_SH_NMID_RTS			0x2D	//
#define BGBCC_SH_NMID_SLEEP			0x2E	//
#define BGBCC_SH_NMID_RTE			0x2F	//
#define BGBCC_SH_NMID_SHLL			0x30	//
#define BGBCC_SH_NMID_DT			0x31	//
#define BGBCC_SH_NMID_SHAL			0x32	//
#define BGBCC_SH_NMID_SHLR			0x33	//
#define BGBCC_SH_NMID_CMPPZ			0x34	//
#define BGBCC_SH_NMID_SHAR			0x35	//
#define BGBCC_SH_NMID_ROTL			0x36	//
#define BGBCC_SH_NMID_ROTCL			0x37	//
#define BGBCC_SH_NMID_ROTT			0x38	//
#define BGBCC_SH_NMID_ROTCR			0x39	//
#define BGBCC_SH_NMID_SHLL2			0x3A	//
#define BGBCC_SH_NMID_ROTR			0x3B	//
#define BGBCC_SH_NMID_CMPPL			0x3C	//
#define BGBCC_SH_NMID_SHLL8			0x3D	//
#define BGBCC_SH_NMID_SHLL16		0x3E	//
#define BGBCC_SH_NMID_SHLR2			0x3F	//
#define BGBCC_SH_NMID_SHLR8			0x40	//
#define BGBCC_SH_NMID_SHLR16		0x41	//
#define BGBCC_SH_NMID_MACW			0x42	//
#define BGBCC_SH_NMID_NOT			0x43	//
#define BGBCC_SH_NMID_SWAPB			0x44	//
#define BGBCC_SH_NMID_SWAPW			0x45	//
#define BGBCC_SH_NMID_NEGC			0x46	//
#define BGBCC_SH_NMID_NEG			0x47	//
#define BGBCC_SH_NMID_EXTUB			0x48	//
#define BGBCC_SH_NMID_EXTUW			0x49	//
#define BGBCC_SH_NMID_EXTSB			0x4A	//
#define BGBCC_SH_NMID_EXTSW			0x4B	//
#define BGBCC_SH_NMID_TRAPA			0x4C	//
#define BGBCC_SH_NMID_MOVA			0x4D	//
#define BGBCC_SH_NMID_TSTB			0x4E	//
#define BGBCC_SH_NMID_DIV0U			0x4F	//

#define BGBCC_SH_NMID_LDC			0x50	//
#define BGBCC_SH_NMID_LDCL			0x51	//
#define BGBCC_SH_NMID_LDS			0x52	//
#define BGBCC_SH_NMID_LDSL			0x53	//
#define BGBCC_SH_NMID_STC			0x54	//
#define BGBCC_SH_NMID_STCL			0x55	//
#define BGBCC_SH_NMID_STS			0x56	//
#define BGBCC_SH_NMID_STSL			0x57	//
#define BGBCC_SH_NMID_CASL			0x58	//
#define BGBCC_SH_NMID_TASB			0x59	//
#define BGBCC_SH_NMID_XTRCT			0x5A	//
#define BGBCC_SH_NMID_ANDB			0x5B	//
#define BGBCC_SH_NMID_XORB			0x5C	//
#define BGBCC_SH_NMID_ORB			0x5D	//
#define BGBCC_SH_NMID_SHAD			0x5E	//
#define BGBCC_SH_NMID_SHLD			0x5F	//
#define BGBCC_SH_NMID_INVDLY		0x60	//
#define BGBCC_SH_NMID_CLRS			0x61	//
#define BGBCC_SH_NMID_SETS			0x62	//

#define BGBCC_SH_NMID_FABS			0x80	//
#define BGBCC_SH_NMID_FADD			0x81	//
#define BGBCC_SH_NMID_FCMPEQ		0x82	//
#define BGBCC_SH_NMID_FCMPGT		0x83	//
#define BGBCC_SH_NMID_FCNVDS		0x84	//
#define BGBCC_SH_NMID_FCNVSD		0x85	//
#define BGBCC_SH_NMID_FDIV			0x86	//
#define BGBCC_SH_NMID_FLDI0			0x87	//
#define BGBCC_SH_NMID_FLDI1			0x88	//
#define BGBCC_SH_NMID_FLDS			0x89	//
#define BGBCC_SH_NMID_FLOAT			0x8A	//
#define BGBCC_SH_NMID_FMAC			0x8B	//
#define BGBCC_SH_NMID_FMOV			0x8C	//
#define BGBCC_SH_NMID_FMOVS			0x8D	//
#define BGBCC_SH_NMID_FMOVD			0x8E	//
#define BGBCC_SH_NMID_FMUL			0x8F	//
#define BGBCC_SH_NMID_FNEG			0x90	//
#define BGBCC_SH_NMID_FSCHG			0x91	//
#define BGBCC_SH_NMID_FSQRT			0x92	//
#define BGBCC_SH_NMID_FSTS			0x93	//
#define BGBCC_SH_NMID_FSUB			0x94	//
#define BGBCC_SH_NMID_FTRC			0x95	//
#define BGBCC_SH_NMID_MOVCAL		0x96	//

#define BGBCC_SH_NMID_MOVI			0xC0	//
#define BGBCC_SH_NMID_MOVIV			0xC1	//

#define BGBCC_SH_NMID_PREF			0xC2	//
#define BGBCC_SH_NMID_OCBI			0xC3	//
#define BGBCC_SH_NMID_OCBP			0xC4	//
#define BGBCC_SH_NMID_OCBWB			0xC5	//
#define BGBCC_SH_NMID_ICBI			0xC6	//


#define BGBCC_SH_FMID_REGREG		0x01	//Rm, Rn
#define BGBCC_SH_FMID_REGRM			0x02	//Rm
#define BGBCC_SH_FMID_REGRN			0x03	//Rn
#define BGBCC_SH_FMID_REGIMM		0x04	//Imm, Rn
#define BGBCC_SH_FMID_REGLDABS		0x05	//@(Abs), Rn
#define BGBCC_SH_FMID_REGST			0x06	//Rm, @Rn
#define BGBCC_SH_FMID_REGLD			0x07	//@Rm, Rn
#define BGBCC_SH_FMID_REGDECST		0x08	//Rm, @-Rn
#define BGBCC_SH_FMID_REGINCLD		0x09	//@Rm+, Rn
#define BGBCC_SH_FMID_REGSTR0N		0x0A	//Rm, @(Ro+Rn)
#define BGBCC_SH_FMID_REGLDR0M		0x0B	//@(Ro+Rm), Rn
#define BGBCC_SH_FMID_REGSTDISP		0x0C	//Rm, @(Rn+Disp)
#define BGBCC_SH_FMID_REGLDDISP		0x0D	//@(Rm+Disp), Rn
#define BGBCC_SH_FMID_ABS			0x0E	//@(Abs)
#define BGBCC_SH_FMID_NONE			0x0F	//-
#define BGBCC_SH_FMID_IMM			0x10	//Imm

#define BGBCC_SH_FMID_LDREG			0x11	//@Rm
#define BGBCC_SH_FMID_STREG			0x12	//@Rn
#define BGBCC_SH_FMID_IMMSTRMN		0x13	//Imm, @(Rm+Rn)
#define BGBCC_SH_FMID_REGVIMM		0x14	//<Imm>, Rn

#define BGBCC_SH_FMID_FREGREG		0x15	//FRm, FRn
#define BGBCC_SH_FMID_FREGRM		0x16	//FRm
#define BGBCC_SH_FMID_FREGRN		0x17	//FRn

#define BGBCC_SH_FMID_DREGREG		0x18	//FRm, FRn
#define BGBCC_SH_FMID_DREGRM		0x19	//FRm
#define BGBCC_SH_FMID_DREGRN		0x1A	//FRn


#define BGBCC_SH_RLC_REL8			0x01	//
#define BGBCC_SH_RLC_REL16			0x02	//
#define BGBCC_SH_RLC_REL32			0x03	//
#define BGBCC_SH_RLC_ABS8			0x04	//
#define BGBCC_SH_RLC_ABS16			0x05	//
#define BGBCC_SH_RLC_ABS32			0x06	//
#define BGBCC_SH_RLC_ABS64			0x07	//
#define BGBCC_SH_RLC_RELW12			0x08	//
#define BGBCC_SH_RLC_RELW8			0x09	//

#define BGBCC_SH_CSEG_TEXT		0x00		//.text section
#define BGBCC_SH_CSEG_DATA		0x01		//.data section
#define BGBCC_SH_CSEG_BSS		0x02		//.bss section

#define BGBCC_SH_CSEG_DYN		0x04		//.bss section


typedef struct BGBCC_SHX_Context_s BGBCC_SHX_Context;
typedef struct BGBCC_SHX_OpcodeArg_s BGBCC_SHX_OpcodeArg;

struct BGBCC_SHX_Context_s {
char *sec_name[8];
byte *sec_buf[8];
byte *sec_end[8];
byte *sec_pos[8];
byte sec;
byte nsec;

int lbl_ofs[256];	//label offsets
int rlc_ofs[256];	//reloc offsets
u16 lbl_id[256];	//label IDs
u16 rlc_id[256];	//reloc label IDs
byte lbl_sec[256];	//label section
byte rlc_sec[256];	//reloc section
byte rlc_ty[256];	//reloc type
int nlbl;
int nrlc;
u16 lblrov;			//labels (local/temp)

byte reg_idx[BGBCC_SH_MAX_CACHEVAR];
byte reg_reg[BGBCC_SH_MAX_CACHEVAR];
int reg_live;
int reg_resv;
int reg_save;

int ofs_s16tab[256];
int ofs_s32tab[256];
byte idx_s16tab[256];
byte idx_s32tab[256];
s16 const_s16tab[256];
s32 const_s32tab[256];
byte const_ns16;
byte const_ns32;
byte nofs_s16tab;
byte nofs_s32tab;

int jitfl;

BGBCC_SHX_Context *next;
};

/*
sc==0: register, immediate, or label
	breg!=Z: register
	breg==Z: immediate (disp)
sc!=0: memory reference
 */
struct BGBCC_SHX_OpcodeArg_s {
byte sc;		//scale for Reg/RM forms (1,2,3,4 for mem-ref)
byte ireg;		//index for Reg/RM forms
byte breg;		//base for Reg/RM forms, or register
s64 disp;		//displacement for Reg/RM forms, or immed
int lbl;		//label
};
