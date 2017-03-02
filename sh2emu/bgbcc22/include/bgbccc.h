#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

#include <bgbcc_xml.h>

#ifdef BGBCC_BGBMETA
#include <bgbcc_meta.h>
#include <bgbcc_exwad.h>
#include <bgbcc_link.h>
#endif

#include <bgbcc_tokord.h>

#include <bgbcc_ccxl.h>

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
int flags, lang, arch;
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

ccxl_label *goto_name;
byte **goto_dest;
ccxl_label *lbl_name;
byte **lbl_dest;
int n_goto, n_lbl;
int n_warn, n_error, n_note;

ccxl_label *contstack;
ccxl_label *breakstack;
int contstackpos;
int breakstackpos;

char *ip;
char *ips;
char *ipe;

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

int lang, arch;

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

BGBCC_CCXL_BackendFuncs_vt *back_vt;
};

struct BGBCC_CCXL_BackendFuncs_vt_s {
ccxl_status (*StackFn)(BGBCC_TransState *ctx, char *name);
ccxl_status (*StackLn)(BGBCC_TransState *ctx, int line);
ccxl_status (*Begin)(BGBCC_TransState *ctx, int tag);
ccxl_status (*BeginName)(BGBCC_TransState *ctx, int tag, char *name);
ccxl_status (*AttribStr)(BGBCC_TransState *ctx, int attr, char *str);
ccxl_status (*AttribLong)(BGBCC_TransState *ctx, int attr, s64 val);
ccxl_status (*LiteralInt)(BGBCC_TransState *ctx, int tag, s64 val);
ccxl_status (*LiteralLong)(BGBCC_TransState *ctx, int tag, s64 val);
ccxl_status (*LiteralFloat)(BGBCC_TransState *ctx, int tag, f64 val);
ccxl_status (*LiteralDouble)(BGBCC_TransState *ctx, int tag, f64 val);
ccxl_status (*LiteralStr)(BGBCC_TransState *ctx, int tag, char *str);
ccxl_status (*LiteralGlobalAddr)(BGBCC_TransState *ctx, int tag, int gblid);
ccxl_status (*CheckDefinedContextName)(BGBCC_TransState *ctx,
	int tag, char *str);
ccxl_status (*Marker)(BGBCC_TransState *ctx, int tag);
ccxl_status (*End)(BGBCC_TransState *ctx);
ccxl_status (*EmitMarkEndTrace)(BGBCC_TransState *ctx);
ccxl_status (*EmitLabel)(BGBCC_TransState *ctx, ccxl_label id);
ccxl_status (*EmitJump)(BGBCC_TransState *ctx, ccxl_label id);
ccxl_status (*EmitJumpRegZero)(BGBCC_TransState *ctx,
	ccxl_type ty, int cmpop, ccxl_register reg, ccxl_label lbl);
ccxl_status (*EmitJumpRegCmp)(BGBCC_TransState *ctx,
	ccxl_type ty, int cmpop, ccxl_register sreg, ccxl_register treg,
	ccxl_label lbl);
ccxl_status (*EmitMov)(BGBCC_TransState *ctx,
	ccxl_type ty, ccxl_register dreg, ccxl_register sreg);
ccxl_status (*EmitCallOp)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int na);
ccxl_status (*EmitCallCsrvOp)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src);
ccxl_status (*EmitCallArg)(BGBCC_TransState *ctx,
	ccxl_register reg);
ccxl_status (*EmitCallRetDefault)(BGBCC_TransState *ctx);
ccxl_status (*EmitCallRetV)(BGBCC_TransState *ctx);
ccxl_status (*EmitCallRetOp)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register src);

#if 0
ccxl_status (*EmitMovInt)(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitMovLong)(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitMovFloat)(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitMovDouble)(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitMovPointer)(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitMovMatchDst)(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitMovMatchSrc)(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src);
#endif

ccxl_status (*EmitConv)(BGBCC_TransState *ctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitUnaryOp)(BGBCC_TransState *ctx,
	ccxl_type type, int opr,
	ccxl_register dst, ccxl_register src);
ccxl_status (*EmitBinaryOp)(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb);
ccxl_status (*EmitCompareOp)(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb);
ccxl_status (*EmitLoadIndexImm)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx);
ccxl_status (*EmitStoreIndexImm)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx);
ccxl_status (*EmitLoadIndex)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb);
ccxl_status (*EmitStoreIndex)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb);
ccxl_status (*EmitLeaImm)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx);
ccxl_status (*EmitLea)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb);
ccxl_status (*EmitLdaVar)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src);
ccxl_status (*EmitSizeofVar)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst);
ccxl_status (*EmitDiffPtr)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb);
ccxl_status (*EmitOffsetOf)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, char *name);
ccxl_status (*EmitLoadSlot)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name);
ccxl_status (*EmitStoreSlot)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name);
ccxl_status (*EmitLoadSlotAddr)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name);
ccxl_status (*EmitLoadSlotAddrID)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, int fn);
ccxl_status (*EmitInitObj)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st);
ccxl_status (*EmitDropObj)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st);
ccxl_status (*EmitInitArr)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, int sz);
ccxl_status (*EmitInitObjArr)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, int sz);
ccxl_status (*EmitLoadInitArr)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register val, int sz);
ccxl_status (*EmitLoadInitObjArr)(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, ccxl_register val, int sz);
};

#include <bgbcc_auto.h>

#ifdef __cplusplus
}
#endif

#endif

