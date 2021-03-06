#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

// #include <bgbgc.h>
// #include <bgbdy.h>

#ifndef BGBCCC_H
#define BGBCCC_H

#include <bgbcc_conf.h>

#ifdef WIN32
#ifdef BGBCC_DLL
#define BGBCC_API __declspec(dllexport)
#else
#define BGBCC_API __declspec(dllimport)
#endif
#else
#define BGBCC_API
#endif

#ifdef BGBCC_DLL
#ifdef linux
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif
#endif

#ifdef WIN32
#define BGBCC_EXCEPTION		0xEBADB0F0
#endif

#ifndef BGBCC_UNDEFINED
#define BGBCC_UNDEFINED ((void *)(-1))
#endif

#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

typedef struct _dyt_s *dyt;
typedef struct { u64 v; } dytf;

typedef u32 fourcc;

#ifndef UNDEFINED
#define UNDEFINED		((void *)(-1))
#endif

#define BGBDY_NULL		((void *)0)
#define BGBDY_TRUE		((void *)2)
#define BGBDY_FALSE		((void *)3)
#define BGBDY_AUXNULL	((void *)4)

#define DYTF_UNDEFINED	(dytfPtrF(UNDEFINED))
#define DYTF_NULL		(dytfPtrF((void *)0))
#define DYTF_TRUE		(dytfPtrF((void *)2))
#define DYTF_FALSE		(dytfPtrF((void *)3))
#define DYTF_AUXNULL	(dytfPtrF((void *)4))

#ifdef _MSC_VER
#define BGBCC_DBGBREAK	__debugbreak();
#else
#define BGBCC_DBGBREAK
#endif

#define BGBCC_CCXL_StubError(ctx)	\
	BGBCC_CCXL_StubErrorLLn(ctx, __FILE__, __LINE__)
#define BGBCC_CCXL_StubWarn(ctx)	\
	BGBCC_CCXL_StubWarnLLn(ctx, __FILE__, __LINE__)
#define BGBCC_CCXL_StubNote(ctx)	\
	BGBCC_CCXL_StubNoteLLn(ctx, __FILE__, __LINE__)

#define BGBCC_FrBC_StubError(ctx)	\
	BGBCC_FrBC_StubErrorLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrBC_StubWarn(ctx)	\
	BGBCC_FrBC_StubWarnLLn(ctx, __FILE__, __LINE__)
#define BGBCC_FrBC_StubNote(ctx)	\
	BGBCC_FrBC_StubNoteLLn(ctx, __FILE__, __LINE__)

#define BGBCC_CCXL_TagError(ctx, tag)	\
	BGBCC_CCXL_TagErrorLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_CCXL_TagWarn(ctx, tag)	\
	BGBCC_CCXL_TagWarnLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_CCXL_TagNote(ctx, tag)	\
	BGBCC_CCXL_TagNoteLLn(ctx, tag, __FILE__, __LINE__)

#define BGBCC_FrBC_TagError(ctx, tag)	\
	BGBCC_FrBC_TagErrorLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrBC_TagWarn(ctx, tag)	\
	BGBCC_FrBC_TagWarnLLn(ctx, tag, __FILE__, __LINE__)
#define BGBCC_FrBC_TagNote(ctx, tag)	\
	BGBCC_FrBC_TagNoteLLn(ctx, tag, __FILE__, __LINE__)

#define BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, rr)	\
	BGBCC_FrBC_RegisterAllocTemporaryLLn(ctx, bty, rr, __FILE__, __LINE__)

#ifdef __cplusplus
extern "C" {
#endif

#define BGBCC_TYFL_PUBLIC			0x0001
#define BGBCC_TYFL_PRIVATE			0x0002
#define BGBCC_TYFL_PROTECTED		0x0004
#define BGBCC_TYFL_STATIC			0x0008
#define BGBCC_TYFL_FINAL			0x0010
#define BGBCC_TYFL_SUPER			0x0020
#define BGBCC_TYFL_SYNCHRONIZED		BGBCC_TYFL_SUPER
#define BGBCC_TYFL_VOLATILE			0x0040
#define BGBCC_TYFL_TRANSIENT		0x0080
#define BGBCC_TYFL_NATIVE			0x0100
#define BGBCC_TYFL_INTERFACE		0x0200
#define BGBCC_TYFL_ABSTRACT			0x0400
#define BGBCC_TYFL_STRICT			0x0800

#define BGBCC_TYFL_BYREF			0x1000	//BS2: pass-by-reference
#define BGBCC_TYFL_DELEGATE			0x2000	//BS2: pass-by-reference

#define BGBCC_TYFL_GETTER			0x4000
#define BGBCC_TYFL_SETTER			0x8000

#define BGBCC_TYFL_SIGNED			0x00010000
#define BGBCC_TYFL_UNSIGNED			0x00020000
#define BGBCC_TYFL_EXTERN			0x00040000
#define BGBCC_TYFL_CONST			0x00080000
#define BGBCC_TYFL_INLINE			0x00100000

#define BGBCC_TYFL_EVENT			0x00200000

#define BGBCC_TYFL_WIDE				0x00400000
// #define BGBCC_TYFL_FUNCTION			0x00800000	//is a function
// #define BGBCC_TYFL_CLASS			0x01000000	//is a class
#define BGBCC_TYFL_THREAD			0x00800000
#define BGBCC_TYFL_XCALL			0x01000000

#define BGBCC_TYFL_STDCALL			0x02000000	//'__stdcall'
#define BGBCC_TYFL_CDECL			0x04000000	//'__cdecl'
#define BGBCC_TYFL_PROXY			0x08000000	//'__proxy'

#define BGBCC_TYFL_LTLENDIAN		0x10000000	//'__littleendian'
#define BGBCC_TYFL_BIGENDIAN		0x20000000	//'__bigendian'

#define BGBCC_TYFL_EXPNAME			0x40000000

// #define BGBCC_TYFL_XCALL			0x10000000

// #define BGBCC_TYFL_WIN64			0x10000000	//'__win64'
// #define BGBCC_TYFL_AMD64			0x20000000	//'__amd64'
// #define BGBCC_TYFL_XCALL			0x40000000	//'__xcall'

// #define BGBCC_TYFL_PERSISTENT	BGBCC_TYFL_WIDE
// #define BGBCC_TYFL_CLASS			BGBCC_TYFL_XCALL

#define BGBCC_TYFL_WIN64			BGBCC_TYFL_STDCALL
#define BGBCC_TYFL_AMD64			BGBCC_TYFL_CDECL

#define BGBCC_TYFL_SEALED			BGBCC_TYFL_FINAL
#define BGBCC_TYFL_READONLY			BGBCC_TYFL_STRICT
#define BGBCC_TYFL_INTERNAL			BGBCC_TYFL_SIGNED

#define BGBCC_TYFL_NEW				BGBCC_TYFL_CONST
#define BGBCC_TYFL_OVERRIDE			BGBCC_TYFL_STRICT
// #define BGBCC_TYFL_ALIASPTR			BGBCC_TYFL_STRICT
#define BGBCC_TYFL_RESTRICT			BGBCC_TYFL_STRICT

#define BGBCC_TYFL_CONST2			BGBCC_TYFL_FINAL

#define BGBCC_TYFL_PACKED			BGBCC_TYFL_SIGNED
// #define BGBCC_TYFL_GC			BGBCC_TYFL_UNSIGNED
// #define BGBCC_TYFL_NOGC			BGBCC_TYFL_INLINE

#define BGBCC_TYFL_GC				BGBCC_TYFL_XCALL
#define BGBCC_TYFL_NOGC				BGBCC_TYFL_NATIVE

// #define BGBCC_TYFL_EVENT			BGBCC_TYFL_TRANSIENT
#define BGBCC_TYFL_UNSAFE			BGBCC_TYFL_NATIVE

#define BGBCC_TYFL_VIRTUAL			BGBCC_TYFL_VOLATILE
#define BGBCC_TYFL_DYNAMIC			BGBCC_TYFL_XCALL

//#define BGBCC_TYFL_GETSET			(BGBCC_TYFL_EVENT|BGBCC_TYFL_BYREF)
//#define BGBCC_TYFL_GETTER			(BGBCC_TYFL_GETSET|BGBCC_TYFL_STRICT)
//#define BGBCC_TYFL_SETTER			BGBCC_TYFL_GETSET

#define BGBCC_TYFL_DLLEXPORT		(1LL<<32)
#define BGBCC_TYFL_DLLIMPORT		(1LL<<33)

#define BGBCC_CTXFL_SAVENAMES		0x0001
#define BGBCC_CTXFL_ENDOFTRACE		0x0002
#define BGBCC_CTXFL_LASTOPRET		0x0004

#define BGBCC_REGFL_ACCESSED		0x0001		//global: was used by code
#define BGBCC_REGFL_CULL			0x0002		//global: omit from image
#define BGBCC_REGFL_INITIALIZED		0x0004		//was initialized
	//field: structs were initialized with this field present
	//struct: struct was initialized with data in-code
	//register: was initialized with an object (destroy on release)

#define BGBCC_REGFL_RECTRACE		0x0008		//hit by reachability trace

#define BGBCC_REGFL_DEMANDLOAD		0x0010		//demand-load requested
#define BGBCC_REGFL_LOADED			0x0020		//already loaded
#define BGBCC_REGFL_ALIASPTR		0x0040		//may be aliased by a pointer

#ifndef BTK_NAME
#define BTK_NAME	1
#define BTK_NUMBER	2
#define BTK_STRING	3
#define BTK_CHARSTRING	4
#define BTK_SEPERATOR	5
#define BTK_OPERATOR	6
#define BTK_BRACE	7
#define BTK_MISC	8
#define BTK_NULL	9
#define BTK_RESV_	10	//special, don't use ('\n')
#define BTK_BREAK	11	//linebreak, ...
#define BTK_COMMENT	12	//comment, ...

#define BTK_INT			13
#define BTK_FLOAT		14
#define BTK_STRING_OVF	15	//overlong string

#endif

#if 0
#define BGBCC_LANG_C		1	//C
#define BGBCC_LANG_CPP		2	//C++
#define BGBCC_LANG_CS		3	//C#
#define BGBCC_LANG_JAVA		4	//Java
#define BGBCC_LANG_BS		5	//BGBScript / ECMAScript
#define BGBCC_LANG_CX		6	//C + extensions
#define BGBCC_LANG_MINIC	7	//Mini C
#define BGBCC_LANG_BS2		8	//BGBScript2

#define BGBCC_ARCH_DEFAULT	0	//any arch
#define BGBCC_ARCH_X86		1	//32-bit x86
#define BGBCC_ARCH_X64		2	//64-bit x86 / x86-64
#define BGBCC_ARCH_ARM		3	//ARM
#define BGBCC_ARCH_PPC		4	//PPC
#define BGBCC_ARCH_PPC64	5	//PPC64

#define BGBCC_ARCH_SH		6	//SH variants
#define BGBCC_ARCH_SH64		7	//64-bit SH variants

#define BGBCC_ARCH_SH_SH2	2	//SH2
#define BGBCC_ARCH_SH_SH4	4	//SH4
#endif

#if 1
#define BGBCC_FOURCC(a, b, c, d)	((a)|((b)<<8)|((c)<<16)|((d)<<24))

#define BGBCC_LANG_C		BGBCC_FOURCC('C', ' ', ' ', ' ')
#define BGBCC_LANG_CPP		BGBCC_FOURCC('C', 'P', 'P', ' ')
#define BGBCC_LANG_CS		BGBCC_FOURCC('C', 'S', 'R', 'P')
#define BGBCC_LANG_JAVA		BGBCC_FOURCC('J', 'A', 'V', 'A')
#define BGBCC_LANG_BS		BGBCC_FOURCC('B', 'S', ' ', ' ')
#define BGBCC_LANG_CX		BGBCC_FOURCC('C', 'A', 'U', 'X')
#define BGBCC_LANG_MINIC	BGBCC_FOURCC('M', 'I', 'N', 'C')
#define BGBCC_LANG_BS2		BGBCC_FOURCC('B', 'S', '2', ' ')

#define BGBCC_LANG_ASM		BGBCC_FOURCC('A', 'S', 'M', ' ')

#define BGBCC_ARCH_DEFAULT	0	//any arch
#define BGBCC_ARCH_X86		BGBCC_FOURCC('X', '8', '6', ' ')
#define BGBCC_ARCH_X64		BGBCC_FOURCC('X', '6', '4', ' ')
#define BGBCC_ARCH_ARM		BGBCC_FOURCC('A', 'R', 'M', ' ')
#define BGBCC_ARCH_PPC		BGBCC_FOURCC('P', 'P', 'C', ' ')
#define BGBCC_ARCH_PPC64	BGBCC_FOURCC('P', 'P', '6', '4')

#define BGBCC_ARCH_SH		BGBCC_FOURCC('S', 'H', ' ', ' ')
#define BGBCC_ARCH_SH64		BGBCC_FOURCC('S', 'H', '6', '4')

#define BGBCC_ARCH_SH_SH2	BGBCC_FOURCC('S', 'H', '2', ' ')
#define BGBCC_ARCH_SH_SH4	BGBCC_FOURCC('S', 'H', '4', ' ')
#define BGBCC_ARCH_SH_SH2L	BGBCC_FOURCC('S', 'H', '2', 'L')
#define BGBCC_ARCH_SH_SH4L	BGBCC_FOURCC('S', 'H', '4', 'L')
#define BGBCC_ARCH_SH_SH2B	BGBCC_FOURCC('S', 'H', '2', 'B')
#define BGBCC_ARCH_SH_SH4B	BGBCC_FOURCC('S', 'H', '4', 'B')

#define BGBCC_ARCH_SH_BX1L	BGBCC_FOURCC('B', 'X', '1', 'L')
#define BGBCC_ARCH_SH_BX1B	BGBCC_FOURCC('B', 'X', '1', 'B')
#define BGBCC_ARCH_SH_BX6L	BGBCC_FOURCC('B', 'X', '6', 'L')
#define BGBCC_ARCH_SH_BX6B	BGBCC_FOURCC('B', 'X', '6', 'B')

#define BGBCC_ARCH_SH_BX6M	BGBCC_FOURCC('B', 'X', '6', 'M')

#define BGBCC_ARCH_SH_BX6U	BGBCC_FOURCC('B', 'X', '6', 'U')
#define BGBCC_ARCH_SH_BX6V	BGBCC_FOURCC('B', 'X', '6', 'V')

#define BGBCC_ARCH_SH_BX6C	BGBCC_FOURCC('B', 'X', '6', 'C')

//output formats.
//format specifics depend on target.
#define BGBCC_IMGFMT_OBJ	BGBCC_FOURCC('O', 'B', 'J', ' ')
#define BGBCC_IMGFMT_EXE	BGBCC_FOURCC('E', 'X', 'E', ' ')
#define BGBCC_IMGFMT_DLL	BGBCC_FOURCC('D', 'L', 'L', ' ')
#define BGBCC_IMGFMT_ELXE	BGBCC_FOURCC('E', 'L', 'X', 'E')
#define BGBCC_IMGFMT_ELSO	BGBCC_FOURCC('E', 'L', 'S', 'O')
#define BGBCC_IMGFMT_ROM	BGBCC_FOURCC('R', 'O', 'M', ' ')

#define BGBCC_IMGFMT_RIL3	BGBCC_FOURCC('R', 'I', 'L', '3')

#endif

#define BGBCC_TYCSEEN_VARIANT	1
#define BGBCC_TYCSEEN_INT128	2
#define BGBCC_TYCSEEN_FLOAT128	4


#include <bgbcc_endian.h>
#include <bgbcc_thread.h>

#include <bgbcc_xml.h>

#ifdef BGBCC_BGBMETA
#include <bgbcc_meta.h>
#include <bgbcc_exwad.h>
#include <bgbcc_link.h>
#endif

#include <bgbcc_tokord.h>

#include <bgbcc_ccxl.h>
#include <bgbcc_ril3.h>
#include <bgbcc_shcc.h>

typedef struct BGBCP_ParseItem_s BGBCP_ParseItem;
typedef struct BGBCP_ParseState_s BGBCP_ParseState;
typedef struct BGBPP_Def_s BGBPP_Def;

typedef struct BGBCP_ParseState_s BGBJP_ParseState;
typedef struct BGBCC_TransState_s BGBCC_TransState;

struct BGBCP_ParseItem_s {
BGBCP_ParseItem *next;
char *name;

BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s);
};

struct BGBCP_ParseState_s {
int flags;

fourcc lang;
fourcc arch;
fourcc subarch;
int tuidx;				//translation unit index

BCCX_Node *structs;
BCCX_Node *types;

BCCX_Node *e_structs;
BCCX_Node *e_types;

BGBPP_Def *ppdef[256];

BCCX_Node *struct_hash[256];
BCCX_Node *type_hash[1024];

BCCX_Node *reduce_tmp;

char *cur_ns;
char **cur_nsi;
char *cur_class;

BGBCC_CCXL_BackendFuncs_vt *back_vt;
};

struct BGBPP_Def_s {
BGBPP_Def *next;
char *name;
char *value;
char **args;
char *t_args[8];
};

struct BGBCC_TransState_s {
byte lvl;	//block level, 0=toplevel, 1..=in function
byte olvl;	//object level, 0=module, 1..=in object
byte cap;	//set to indicate var capture
byte tail;	//this expression is in tail position
byte i_cx_p;	//infer: expression is clean (cleared to indicate dirty)
byte i_cap;	//infer: guess as to whether or not there is var capture

char *cf_n;
BCCX_Node *cf_ty;

//ccxl_label *goto_name;
//byte **goto_dest;
//ccxl_label *lbl_name;
//byte **lbl_dest;
//int n_goto, n_lbl;

// char **goto_name;
// ccxl_label *goto_lbl;
// int n_goto, m_goto;

int n_warn, n_error, n_note;

ccxl_label *contstack;
ccxl_label *breakstack;
int contstackpos;
int breakstackpos;

// char *ip;
// char *ips;
// char *ipe;

byte *ril_ip;
byte *ril_ips;
byte *ril_ipe;

byte *ril_txwin;
u16 *ril_txhash;
u16 ril_txrov;
u16 ril_txrh;
u16 ril_psidx[64];
byte ril_pslen[64];
byte ril_psrov;

BGBCC_CCXL_VirtOp **vop;
BGBCC_CCXL_VirtTr **vtr;
int n_vop, m_vop;
int n_vtr, m_vtr;
int s_vop;

int *markstack;
ccxl_register *regstack;
ccxl_register *uregstack;
int markstackpos;
int regstackpos;
int uregstackpos;

BCCX_Node *types;
BCCX_Node *structs;

char *cur_ns;
char *cur_struct;
int cur_idx;	//field
int cur_idx2;	//method
int cur_idx3;	//property
int cur_idx4;	//property

fourcc lang;
fourcc arch;			//major architecture
fourcc sub_arch;		//subset architecture
int tuidx;				//translation unit index

byte arch_sizeof_long;
byte arch_sizeof_ptr;
byte arch_sizeof_valist;
byte arch_demote_d2f;	//for arch, demote double to float
byte arch_align_max;	//don't use alignments larger than this
byte arch_align_objmin;	//minimum alignment for value-objects

byte ril3_norec;
byte cgif_no3ac;

int ccxl_tyc_seen;

int gs_seq;
char *lfn;
int lln;

int reqlfn;
// int reqlln;
int ctxflags;

BCCX_Node *cur_structdef;
BCCX_Node *reduce_tmp;


BGBCC_CCXL_RegisterInfo *cur_func;

// BGBCC_CCXL_RegisterInfo *reg_globals[4096];
// int idx_globals[4096];
BGBCC_CCXL_RegisterInfo *reg_globals[262144];
int idx_globals[262144];
int n_reg_globals;
int m_reg_globals;

BGBCC_CCXL_RegisterInfo *hash_globals[4096];	//global lookup hash
BGBCC_CCXL_RegisterInfo *usort_globals;			//unsorted globals

// BGBCC_CCXL_LiteralInfo *literals[4096];
// int idx_literals[4096];
BGBCC_CCXL_LiteralInfo *literals[65536];
int idx_literals[65536];
int n_literals, m_literals;

int hash_literals[4096];
int manif_literal;
int usort_literal;

// BGBCC_CCXL_TypeOverflow *tyovf[1024];
BGBCC_CCXL_TypeOverflow *tyovf[16384];
int n_tyovf, m_tyovf;

u32 *ctab_lvt4;
u64 *ctab_lvt8;
int n_ctab_lvt4, m_ctab_lvt4;
int n_ctab_lvt8, m_ctab_lvt8;

int *cntstrn;
char **cntstrs;
int n_cntstrs, m_cntstrs;

char *strtab;
int sz_strtab, msz_strtab;

char *wstrtab;
int sz_wstrtab, msz_wstrtab;

BGBCC_CCXL_RegisterInfo *reginfo_free;

BGBCC_CCXL_LiteralInfo *cur_objstack[64];
BGBCC_CCXL_LiteralInfo *cur_obj;
int cur_objstackpos;

BGBCC_CCXL_VirtOp *virtop_free;
BGBCC_CCXL_VirtTr *virttr_free;

char *imgbasename;

BGBCC_CCXL_BackendFuncs_vt *back_vt;
void *uctx;		//context for backend
};

struct BGBCC_CCXL_BackendFuncs_vt_s {
char *tgtname;
BGBCC_CCXL_BackendFuncs_vt *next;

ccxl_status (*SetupParserForArch)(BGBCP_ParseState *ctx);
ccxl_status (*SetupContextForArch)(BGBCC_TransState *ctx);
ccxl_status (*FlattenImage)(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt);
ccxl_status (*EndFunction)(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj);
ccxl_status (*LoadBufferDLL)(BGBCC_TransState *ctx,
	byte *buf, int sz);
};

#include <bgbcc_auto.h>

#ifdef __cplusplus
}
#endif

#endif

