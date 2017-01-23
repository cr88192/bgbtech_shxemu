#define BTSHAS_OPVTY_NONE	0x00	//None
#define BTSHAS_OPVTY_IMM	0x01	//#Imm
#define BTSHAS_OPVTY_REG	0x02	//Reg
#define BTSHAS_OPVTY_NAME	0x03	//Name

#define BTSHAS_OPVTY_DRREG	0x04	//@Reg
#define BTSHAS_OPVTY_DRREGI	0x05	//@Reg+
#define BTSHAS_OPVTY_DRREGD	0x06	//@-Reg

#define BTSHAS_OPVTY_RRMEM	0x08	//@(Reg,Reg)
#define BTSHAS_OPVTY_RDMEM	0x09	//@(Reg,Disp)

#define BTSHAS_OPVTY_RM		0x10	//Rm
#define BTSHAS_OPVTY_RN		0x11	//Rn
#define BTSHAS_OPVTY_DRM	0x12	//@Rm
#define BTSHAS_OPVTY_DRN	0x13	//@Rn
#define BTSHAS_OPVTY_DRMI	0x14	//@Rm+
#define BTSHAS_OPVTY_DRNI	0x15	//@Rn+
#define BTSHAS_OPVTY_DRMD	0x16	//@-Rm
#define BTSHAS_OPVTY_DRND	0x17	//@-Rn
#define BTSHAS_OPVTY_IMM8	0x18	//#imm8 (signed)
#define BTSHAS_OPVTY_IMM8U	0x19	//#imm8 (unsigned)
#define BTSHAS_OPVTY_IMM4U	0x1A	//#imm4 (unsigned)
#define BTSHAS_OPVTY_IMM3L	0x1B	//#imm3 (0-7 -> 0-7)
#define BTSHAS_OPVTY_IMM3H	0x1C	//#imm3 (0-7 -> 8-F)
#define BTSHAS_OPVTY_LABEL	0x1D	//label (relative addr)

#define BTSHAS_OPVTY_FRM	0x1E	//FRm
#define BTSHAS_OPVTY_FRN	0x1F	//FRn

#define BTSHAS_OPVTY_R0		0x20	//R0
#define BTSHAS_OPVTY_DR0	0x21	//@R0
#define BTSHAS_OPVTY_DR15I	0x22	//@R15+
#define BTSHAS_OPVTY_DR15D	0x23	//@-R15

#define BTSHAS_OPVTY_DRMO	0x24	//@(Rm,disp)
#define BTSHAS_OPVTY_DRNO	0x25	//@(Rn,disp)
#define BTSHAS_OPVTY_DR0RM	0x26	//@(R0,Rm)
#define BTSHAS_OPVTY_DR0RN	0x27	//@(R0,Rn)
#define BTSHAS_OPVTY_DGBRO	0x28	//@(GBR,disp)
#define BTSHAS_OPVTY_DGBRR0	0x29	//@(GBR,R0)
#define BTSHAS_OPVTY_DPCO	0x2A	//@(PC,disp)

#define BTSHAS_OPVTY_SR		0x30	//
#define BTSHAS_OPVTY_GBR	0x31	//
#define BTSHAS_OPVTY_VBR	0x32	//
#define BTSHAS_OPVTY_SSR	0x33	//
#define BTSHAS_OPVTY_SPC	0x34	//
#define BTSHAS_OPVTY_DBR	0x35	//

#define BTSHAS_OPVTY_MACH	0x38	//
#define BTSHAS_OPVTY_MACL	0x39	//
#define BTSHAS_OPVTY_PR		0x3A	//
#define BTSHAS_OPVTY_TBR	0x3B	//
#define BTSHAS_OPVTY_FPUL	0x3C	//
#define BTSHAS_OPVTY_FPSCR	0x3D	//

#define BTSHAS_OPVTM_TY		0x03000000	//
#define BTSHAS_OPVTM_B		0x01000000	//
#define BTSHAS_OPVTM_W		0x02000000	//
#define BTSHAS_OPVTM_L		0x03000000	//

#define BTSHAS_OPVA2(a, b)		((a)|((b)<<8))
#define BTSHAS_OPVA3(a, b, c)	((a)|((b)<<8)|((c)<<16))

#define BTSHAS_OPVTY_RM_RN		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_RN)
#define BTSHAS_OPVTY_RM_DRN		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_DRN)
#define BTSHAS_OPVTY_DRM_RN		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRM, BTSHAS_OPVTY_RN)
#define BTSHAS_OPVTY_DRM_DRN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRM, BTSHAS_OPVTY_DRN)

#define BTSHAS_OPVTY_FRM_FRN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_FRM, BTSHAS_OPVTY_FRN)
#define BTSHAS_OPVTY_FRM_DRN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_FRM, BTSHAS_OPVTY_DRN)
#define BTSHAS_OPVTY_DRM_FRN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRM, BTSHAS_OPVTY_FRN)

#define BTSHAS_OPVTY_FRM_DRND	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_FRM, BTSHAS_OPVTY_DRND)
#define BTSHAS_OPVTY_DRMI_FRN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRMI, BTSHAS_OPVTY_FRN)

#define BTSHAS_OPVTY_RM_DRND		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_DRND)

#define BTSHAS_OPVTY_R0_RN		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_R0, BTSHAS_OPVTY_RN)
#define BTSHAS_OPVTY_R0_DRN		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_R0, BTSHAS_OPVTY_DRN)
#define BTSHAS_OPVTY_R0_DRNI		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_R0, BTSHAS_OPVTY_DRNI)
#define BTSHAS_OPVTY_R0_DRNO		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_R0, BTSHAS_OPVTY_DRNO)

#define BTSHAS_OPVTY_R0_DGBRO		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_R0, BTSHAS_OPVTY_DGBRO)

#define BTSHAS_OPVTY_DRM_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRM, BTSHAS_OPVTY_R0)
#define BTSHAS_OPVTY_DRMD_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRMD, BTSHAS_OPVTY_R0)
#define BTSHAS_OPVTY_DRND_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRND, BTSHAS_OPVTY_R0)
#define BTSHAS_OPVTY_DRMO_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRMO, BTSHAS_OPVTY_R0)
#define BTSHAS_OPVTY_DRNO_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRNO, BTSHAS_OPVTY_R0)

#define BTSHAS_OPVTY_DGBRO_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DGBRO, BTSHAS_OPVTY_R0)
#define BTSHAS_OPVTY_DPCO_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DPCO, BTSHAS_OPVTY_R0)

#define BTSHAS_OPVTY_RM_DR0RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_DR0RN)
#define BTSHAS_OPVTY_DR0RM_RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DR0RM, BTSHAS_OPVTY_RN)

#define BTSHAS_OPVTY_FRM_DR0RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_FRM, BTSHAS_OPVTY_DR0RN)
#define BTSHAS_OPVTY_DR0RM_FRN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DR0RM, BTSHAS_OPVTY_FRN)

#define BTSHAS_OPVTY_RM_DRNI	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_DRNI)

#define BTSHAS_OPVTY_DRMI_RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRMI, BTSHAS_OPVTY_RN)

#define BTSHAS_OPVTY_RM_DRNO	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_DRNO)
#define BTSHAS_OPVTY_DRMO_RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DRMO, BTSHAS_OPVTY_RN)
#define BTSHAS_OPVTY_DPCO_RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DPCO, BTSHAS_OPVTY_RN)

#define BTSHAS_OPVTY_RM_RN_DR0		\
	BTSHAS_OPVA3(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_RN, BTSHAS_OPVTY_DR0)
#define BTSHAS_OPVTY_RM_DR15D		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_RM, BTSHAS_OPVTY_DR15D)
#define BTSHAS_OPVTY_DR15I_RN		\
	BTSHAS_OPVA2(BTSHAS_OPVTY_DR15I, BTSHAS_OPVTY_RN)

#define BTSHAS_OPVTY_IMM8_RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_IMM8, BTSHAS_OPVTY_RN)
#define BTSHAS_OPVTY_IMM8_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_IMM8, BTSHAS_OPVTY_R0)
#define BTSHAS_OPVTY_IMM8U_R0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_IMM8U, BTSHAS_OPVTY_R0)

#define BTSHAS_OPVTY_IMM8U_DGBRR0	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_IMM8U, BTSHAS_OPVTY_DGBRR0)

#define BTSHAS_OPVTY_IMM3L_RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_IMM3L, BTSHAS_OPVTY_RN)
#define BTSHAS_OPVTY_IMM3H_RN	\
	BTSHAS_OPVA2(BTSHAS_OPVTY_IMM3H, BTSHAS_OPVTY_RN)


#define BTSHAS_OPREG_R0		0x00
#define BTSHAS_OPREG_R15	0x0F
#define BTSHAS_OPREG_SP		0x0F

#define BTSHAS_OPREG_FR0	0x10
#define BTSHAS_OPREG_DR0	0x20

#define BTSHAS_OPREG_SR		0x30
#define BTSHAS_OPREG_GBR	0x31
#define BTSHAS_OPREG_VBR	0x32
#define BTSHAS_OPREG_TBR	0x33
#define BTSHAS_OPREG_MACH	0x34
#define BTSHAS_OPREG_MACL	0x35
#define BTSHAS_OPREG_PR		0x36
#define BTSHAS_OPREG_PC		0x37
#define BTSHAS_OPREG_FPSCR	0x38
#define BTSHAS_OPREG_FPUL	0x39

#define BTSHAS_OPREG_SSR	0x3A
#define BTSHAS_OPREG_SPC	0x3B
#define BTSHAS_OPREG_DBR	0x3C

#define BTSHAS_LBLSCL_NONE			0x00	//no class
#define BTSHAS_LBLSCL_AUTO			0x01	//
#define BTSHAS_LBLSCL_EXTERN		0x02	//
#define BTSHAS_LBLSCL_STATIC		0x03	//
#define BTSHAS_LBLSCL_REGISTER		0x04	//
#define BTSHAS_LBLSCL_EXTERNAL_DEF	0x05	//
#define BTSHAS_LBLSCL_LABEL			0x06	//
#define BTSHAS_LBLSCL_UNDEF_LABEL	0x07	//
#define BTSHAS_LBLSCL_STRUCT_MBR	0x08	//
#define BTSHAS_LBLSCL_ARGUMENT		0x09	//
#define BTSHAS_LBLSCL_STRUCT_TAG	0x0A	//
#define BTSHAS_LBLSCL_UNION_MBR		0x0B	//
#define BTSHAS_LBLSCL_UNION_TAG		0x0C	//
#define BTSHAS_LBLSCL_TYPEDEF		0x0D	//
#define BTSHAS_LBLSCL_UNDEF_STATIC	0x0E	//
#define BTSHAS_LBLSCL_ENUM_TAG		0x0F	//
#define BTSHAS_LBLSCL_ENUM_MBR		0x10	//
#define BTSHAS_LBLSCL_REG_PARAM		0x11	//
#define BTSHAS_LBLSCL_BITFIELD		0x12	//
#define BTSHAS_LBLSCL_CLS_BLK		0x64	//
#define BTSHAS_LBLSCL_CLS_FUNC		0x65	//
#define BTSHAS_LBLSCL_CLS_END		0x66	//
#define BTSHAS_LBLSCL_CLS_FILE		0x67	//
#define BTSHAS_LBLSCL_CLS_SECT		0x68	//
#define BTSHAS_LBLSCL_CLS_WEAKEXT	0x69	//
#define BTSHAS_LBLSCL_CLS_CLRTOKEN	0x71	//

#define BTSHAS_LBLSCL_REDIRECT		0x7E	//

#define BTSHAS_LBLRLC_SH_NONE		0x00	//no reloc
#define BTSHAS_LBLRLC_SH_DIR16		0x01	//target=16 bits of VA
#define BTSHAS_LBLRLC_SH_DIR32		0x02	//target=32 bit VA
#define BTSHAS_LBLRLC_SH_DIR8		0x03	//op with 8-bit disp to byte
#define BTSHAS_LBLRLC_SH_DIR8W		0x04	//op with 8-bit disp to word
#define BTSHAS_LBLRLC_SH_DIR8L		0x05	//op with 8-bit disp to long
#define BTSHAS_LBLRLC_SH_DIR4		0x06	//op with 4-bit disp to byte
#define BTSHAS_LBLRLC_SH_DIR4W		0x07	//op with 4-bit disp to word
#define BTSHAS_LBLRLC_SH_DIR4L		0x08	//op with 4-bit disp to long
#define BTSHAS_LBLRLC_SH_PC8W		0x09	//op with 8-bit PC disp to word
#define BTSHAS_LBLRLC_SH_PC8L		0x0A	//op with 8-bit PC disp to long
#define BTSHAS_LBLRLC_SH_PC12W		0x0B	//op with 12-bit PC disp to word
#define BTSHAS_LBLRLC_SH_RVASECT	0x0C	//32-bit address to section
#define BTSHAS_LBLRLC_SH_SZSECT		0x0D	//32-bit size of section
#define BTSHAS_LBLRLC_SH_SECTID		0x0E	//16-bit section ID
#define BTSHAS_LBLRLC_SH_SECREL		0x0F	//32-bit section-relative offset
#define BTSHAS_LBLRLC_SH_DIR32_NB	0x10	//target=32 bit RVA
#define BTSHAS_LBLRLC_SH_GPREL4L	0x11	//GP relative
#define BTSHAS_LBLRLC_SH_TOKEN		0x12
#define BTSHAS_LBLRLC_SH_PCRELPT	0x13
#define BTSHAS_LBLRLC_SH_REFLO		0x14	//low 16 bits of address
#define BTSHAS_LBLRLC_SH_REFHI		0x15	//high 16 bits of address
#define BTSHAS_LBLRLC_SH_RELLO		0x16	//low 16 bits of relative addr
#define BTSHAS_LBLRLC_SH_RELHI		0x17	//high 16 bits of relative addr
#define BTSHAS_LBLRLC_SH_SHM_PAIR	0x18	//paired with RELLO or RELHI

#define BTSHAS_LBLRLC_SH_REL32		0x1C	//ext: Rel32 Reloc (A+=T-A)
#define BTSHAS_LBLRLC_SH_REL32B		0x1D	//ext: Rel32 Reloc (A+=A-T)

#define BTSHAS_LBLRLC_SH_DIR16B		0x1E	//DIR16, Resolved
#define BTSHAS_LBLRLC_SH_DIR32B		0x1F	//DIR32, Resolved

#define BTSHAS_LBLRLC_SH_NOMODE		0x8000

#define BTSHAS_COFF_MACH_SH3		0x01A2
#define BTSHAS_COFF_MACH_SH3DSP		0x01A3
#define BTSHAS_COFF_MACH_SH4		0x01A6

#define BTSHAS_COFF_SCNT_NOPAD		0x00000008
#define BTSHAS_COFF_SCNT_CODE		0x00000020
#define BTSHAS_COFF_SCNT_IDATA		0x00000040
#define BTSHAS_COFF_SCNT_UDATA		0x00000080

#define BTSHAS_COFF_SCNT_GPREL		0x00008000
#define BTSHAS_COFF_SCNT_MEM6BIT	0x00020000

#define BTSHAS_COFF_SCNT_ALGN1B		0x00100000
#define BTSHAS_COFF_SCNT_ALGN2B		0x00200000
#define BTSHAS_COFF_SCNT_ALGN4B		0x00300000
#define BTSHAS_COFF_SCNT_ALGN8B		0x00400000
#define BTSHAS_COFF_SCNT_ALGN16B	0x00500000
#define BTSHAS_COFF_SCNT_ALGN32B	0x00600000
#define BTSHAS_COFF_SCNT_ALGN64B	0x00700000
#define BTSHAS_COFF_SCNT_ALGN128B	0x00800000
#define BTSHAS_COFF_SCNT_ALGN256B	0x00900000
#define BTSHAS_COFF_SCNT_ALGN512B	0x00A00000
#define BTSHAS_COFF_SCNT_ALGN1KB	0x00B00000
#define BTSHAS_COFF_SCNT_ALGN2KB	0x00C00000
#define BTSHAS_COFF_SCNT_ALGN4KB	0x00D00000
#define BTSHAS_COFF_SCNT_ALGN8KB	0x00E00000

#define BTSHAS_COFF_SCNT_NRLCOVF	0x01000000
#define BTSHAS_COFF_SCNT_DISCARD	0x02000000
#define BTSHAS_COFF_SCNT_NOCACHE	0x04000000
#define BTSHAS_COFF_SCNT_NOPAGE		0x08000000
#define BTSHAS_COFF_SCNT_SHARED		0x10000000
#define BTSHAS_COFF_SCNT_EXECUTE	0x20000000
#define BTSHAS_COFF_SCNT_READ		0x40000000
#define BTSHAS_COFF_SCNT_WRITE		0x80000000

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

typedef struct BTSHAS_OprVal_s BTSHAS_OprVal;

typedef struct BTSHAS_LabelInfo_s BTSHAS_LabelInfo;
typedef struct BTSHAS_RelocInfo_s BTSHAS_RelocInfo;

typedef struct BTSHAS_Section_s BTSHAS_Section;
typedef struct BTSHAS_Context_s BTSHAS_Context;

typedef struct BTSHAS_ListingOp_s BTSHAS_ListingOp;

typedef struct BTSHAS_PpDef_s BTSHAS_PpDef;

struct BTSHAS_OprVal_s {
byte ty;		//operand type
byte rb;		//reg base
byte ri;		//reg index
s64 imm;		//immediate or offset
char *name;
};

struct BTSHAS_LabelInfo_s {
char *name;		//name of symbol
int offs;		//offset of symbol
sbyte sect;		//section ID
byte ty_bty;	//base type of symbol
byte ty_dty;	//derived type of symbol
byte scl;		//storage class
// byte rlty;		//reloc type
};

struct BTSHAS_RelocInfo_s {
u32 offs;		//offset of address
u16 sym;		//symbol index
u16 rlty;		//reloc type
byte sect;
};

#define BTSHAS_SEC_IDATA	4096
struct BTSHAS_Section_s {
char *name;
byte *ct;
byte *cts;
byte *cte;

u32 rva_base;
u32 rva_size;

// BTSHAS_LabelInfo *lbl;
// BTSHAS_LabelInfo *rlc;
// int nlbl, mlbl;
// int nrlc, mrlc;

char t_name[16];
byte t_data[BTSHAS_SEC_IDATA];
// BTSHAS_LabelInfo t_lbl[256];
// BTSHAS_LabelInfo t_rlc[256];
};

#define BTSHAS_CTX_IMAXLBL	4096
#define BTSHAS_CTX_IMAXRLC	4096

struct BTSHAS_Context_s {
BTSHAS_Section *seg[16];
int nseg;
int cseg;
bool is_le;

BTSHAS_LabelInfo *lbl;
BTSHAS_RelocInfo *rlc;
int nlbl, mlbl;
int nrlc, mrlc;

BTSHAS_PpDef *ppdef[256];

BTSHAS_LabelInfo t_lbl[BTSHAS_CTX_IMAXLBL];
BTSHAS_RelocInfo t_rlc[BTSHAS_CTX_IMAXRLC];
};


struct BTSHAS_ListingOp_s {
char *form;
char *name;
int aty;
};

struct BTSHAS_PpDef_s {
BTSHAS_PpDef *next;
char *name;
char *value;
char **args;
char *t_args[8];
};
