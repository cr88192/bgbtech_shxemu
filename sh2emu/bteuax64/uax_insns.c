UAX_OpcodeDesc uax_opx86[]={
{UAX_OP_ADC,	NULL,			NULL},
{0,				"14,ib",		"al,i8"},
{0,				"X80/2,ib",		"rm8,i8"},
{0,				"WX83/2,ib",	"rm16,i8"},
{0,				"TX83/2,ib",	"rm32,i8"},
{0,				"X83/2,ib",		"rm64,i8"},
{0,				"X12/r",		"r8,rm8"},
{0,				"X10/R",		"rm8,r8"},
{0,				"W15,iw",		"ax,i16"},
{0,				"WX81/2,iw",	"rm16,i16"},
{0,				"WX13/r",		"r16,rm16"},
{0,				"WX11/R",		"rm16,r16"},
{0,				"T15,id",		"eax,i32"},
{0,				"TX13/r",		"r32,rm32"},
{0,				"TX81/2,id",	"rm32,i32"},
{0,				"TX11/R",		"rm32,r32"},
{0,				"X15,id",		"rax,i32"},
{0,				"X13/r",		"r64,rm64"},
{0,				"X81/2,id",		"rm64,i32"},
{0,				"X11/R",		"rm64,r64"},
{UAX_OP_ADD,	NULL,			NULL},
{0,				"04,ib",		"al,i8"},
{0,				"X80/0,ib",		"rm8,i8"},
{0,				"WX83/0,ib",	"rm16,i8"},
{0,				"TX83/0,ib",	"rm32,i8"},
{0,				"X83/0,ib",		"rm64,i8"},
{0,				"X02/r",		"r8,rm8"},
{0,				"X00/R",		"rm8,r8"},
{0,				"W05,iw",		"ax,i16"},
{0,				"WX81/0,iw",	"rm16,i16"},
{0,				"WX03/r",		"r16,rm16"},
{0,				"WX01/R",		"rm16,r16"},
{0,				"T05,id",		"eax,i32"},
{0,				"TX81/0,id",	"rm32,i32"},
{0,				"TX01/R",		"rm32,r32"},
{0,				"TX03/r",		"r32,rm32"},
{0,				"X05,id",		"rax,i32"},
{0,				"X81/0,id",		"rm64,i32"},
{0,				"X03/r",		"r64,rm64"},
{0,				"X01/R",		"rm64,r64"},
{UAX_OP_AND,	NULL,			NULL},
{0,				"24,ib",		"al,i8"},
{0,				"X80/4,ib",		"rm8,i8"},
{0,				"WX83/4,ib",	"rm16,i8"},
{0,				"TX83/4,ib",	"rm32,i8"},
{0,				"X83/4,ib",		"rm64,i8"},
{0,				"X20/R",		"rm8,r8"},
{0,				"X22/r",		"r8,rm8"},
{0,				"WX25,iw",		"ax,i16"},
{0,				"WX81/4,iw",	"rm16,i16"},
{0,				"WX23/r",		"r16,rm16"},
{0,				"WX21/R",		"rm16,r16"},
{0,				"T25,id",		"eax,i32"},
{0,				"TX81/4,id",	"rm32,i32"},
{0,				"TX23/r",		"r32,rm32"},
{0,				"TX21/R",		"rm32,r32"},
{0,				"X25,id",		"rax,i32"},
{0,				"X81/4,id",		"rm64,i32"},
{0,				"X23/r",		"r64,rm64"},
{0,				"X21/R",		"rm64,r64"},
{UAX_OP_BSF,	NULL,			NULL},
{0,				"WX0FBC/r",		"r16,rm16"},
{0,				"TX0FBC/r",		"r32,rm32"},
{0,				"X0FBC/r",		"r64,rm64"},
{UAX_OP_BSR,	NULL,			NULL},
{0,				"WX0FBD/r",		"r16,rm16"},
{0,				"TX0FBD/r",		"r32,rm32"},
{0,				"X0FBD/r",		"r64,rm64"},
{UAX_OP_BSWAP,	NULL,			NULL},
{0,				"X0FC8|r",		"r32"},
{0,				"X0FC8|r",		"r64"},
{UAX_OP_BT,		NULL,			NULL},
{0,				"WX0FA3/R",		"rm16,r16"},
{0,				"TX0FA3/R",		"rm32,r32"},
{0,				"X0FA3/R",		"rm64,r32"},
{0,				"WX0FBA/4,ib",	"rm16,i8"},
{0,				"TX0FBA/4,ib",	"rm32,i8"},
{0,				"X0FBA/4,ib",	"rm64,i8"},
{UAX_OP_BTC,	NULL,			NULL},
{0,				"WX0FBB/R",		"rm16,r16"},
{0,				"TX0FBB/R",		"rm32,r32"},
{0,				"X0FBB/R",		"rm64,r64"},
{0,				"WX0FBA/7,ib",	"rm16,i8"},
{0,				"TX0FBA/7,ib",	"rm32,i8"},
{0,				"X0FBA/7,ib",	"rm64,i8"},
{UAX_OP_BTR,	NULL,			NULL},
{0,				"WX0FB3/R",		"rm16,r16"},
{0,				"TX0FB3/R",		"rm32,r32"},
{0,				"X0FB3/R",		"rm64,r64"},
{0,				"WX0FBA/6,ib",	"rm16,i8"},
{0,				"TX0FBA/6,ib",	"rm32,i8"},
{0,				"X0FBA/6,ib",	"rm64,i8"},
{UAX_OP_BTS,	NULL,			NULL},
{0,				"WX0FAB/R",		"rm16,r16"},
{0,				"TX0FAB/R",		"rm32,r32"},
{0,				"X0FAB/R",		"rm64,r64"},
{0,				"WX0FBA/5,ib",	"rm16,i8"},
{0,				"TX0FBA/5,ib",	"rm32,i8"},
{0,				"X0FBA/5,ib",	"rm64,i8"},
{UAX_OP_CALL,	NULL,			NULL},
{0,				"TE8,rd",		"ar32"},
{0,				"WXFF/2",		"rm16"},
{0,				"TXFF/2",		"rm32"},
{0,				"XFF/2",		"rm64"},
{UAX_OP_CBW,	"W98",			NULL},
{UAX_OP_CWDE,	"T98",			NULL},
{UAX_OP_CDQE,	"4898",			NULL},
{UAX_OP_CLC,	"F8",			NULL},
{UAX_OP_CLD,	"FC",			NULL},
{UAX_OP_CLI,	"FA",			NULL},
{UAX_OP_CLTS,	"0F06",			NULL},
{UAX_OP_CMC,	"F5",			NULL},
{UAX_OP_CMOVO,	NULL,			NULL},
{0,				"WX0F40/r",		"r16,rm16"},
{0,				"TX0F40/r",		"r32,rm32"},
{0,				"X0F40/r",		"r64,rm64"},
{UAX_OP_CMOVNO,	NULL,			NULL},
{0,				"WX0F41/r",		"r16,rm16"},
{0,				"TX0F41/r",		"r32,rm32"},
{0,				"X0F41/r",		"r64,rm64"},
{UAX_OP_CMOVB,	NULL,			NULL},
{0,				"WX0F42/r",		"r16,rm16"},
{0,				"TX0F42/r",		"r32,rm32"},
{0,				"X0F42/r",		"r64,rm64"},
{UAX_OP_CMOVAE,	NULL,			NULL},
{0,				"WX0F43/r",		"r16,rm16"},
{0,				"TX0F43/r",		"r32,rm32"},
{0,				"X0F43/r",		"r64,rm64"},
{UAX_OP_CMOVZ,	NULL,			NULL},
{UAX_OP_CMOVE,	NULL,			NULL},
{0,				"WX0F44/r",		"r16,rm16"},
{0,				"TX0F44/r",		"r32,rm32"},
{0,				"X0F44/r",		"r64,rm64"},
{UAX_OP_CMOVNZ,	NULL,			NULL},
{UAX_OP_CMOVNE,	NULL,			NULL},
{0,				"WX0F45/r",		"r16,rm16"},
{0,				"TX0F45/r",		"r32,rm32"},
{0,				"X0F45/r",		"r64,rm64"},
{UAX_OP_CMOVNA,	NULL,			NULL},
{UAX_OP_CMOVBE,	NULL,			NULL},
{0,				"WX0F46/r",		"r16,rm16"},
{0,				"TX0F46/r",		"r32,rm32"},
{0,				"X0F46/r",		"r64,rm64"},
{UAX_OP_CMOVA,	NULL,			NULL},
{0,				"WX0F47/r",		"r16,rm16"},
{0,				"TX0F47/r",		"r32,rm32"},
{0,				"X0F47/r",		"r64,rm64"},
{UAX_OP_CMOVS,	NULL,			NULL},
{0,				"WX0F48/r",		"r16,rm16"},
{0,				"TX0F48/r",		"r32,rm32"},
{0,				"X0F48/r",		"r64,rm64"},
{UAX_OP_CMOVNS,	NULL,			NULL},
{0,				"WX0F49/r",		"r16,rm16"},
{0,				"TX0F49/r",		"r32,rm32"},
{0,				"X0F49/r",		"r64,rm64"},
{UAX_OP_CMOVPE,	NULL,			NULL},
{UAX_OP_CMOVP,	NULL,			NULL},
{0,				"WX0F4A/r",		"r16,rm16"},
{0,				"TX0F4A/r",		"r32,rm32"},
{0,				"X0F4A/r",		"r64,rm64"},
{UAX_OP_CMOVNP,	NULL,			NULL},
{UAX_OP_CMOVPO,	NULL,			NULL},
{0,				"WX0F4B/r",		"r16,rm16"},
{0,				"TX0F4B/r",		"r32,rm32"},
{0,				"X0F4B/r",		"r64,rm64"},
{UAX_OP_CMOVNGE, NULL,			NULL},
{UAX_OP_CMOVL,	NULL,			NULL},
{0,				"WX0F4C/r",		"r16,rm16"},
{0,				"TX0F4C/r",		"r32,rm32"},
{0,				"X0F4C/r",		"r64,rm64"},
{UAX_OP_CMOVNL,	NULL,			NULL},
{UAX_OP_CMOVGE,	NULL,			NULL},
{0,				"WX0F4D/r",		"r16,rm16"},
{0,				"TX0F4D/r",		"r32,rm32"},
{0,				"X0F4D/r",		"r64,rm64"},
{UAX_OP_CMOVNG,	NULL,			NULL},
{UAX_OP_CMOVLE,	NULL,			NULL},
{0,				"WX0F4E/r",		"r16,rm16"},
{0,				"TX0F4E/r",		"r32,rm32"},
{0,				"X0F4E/r",		"r64,rm64"},
{UAX_OP_CMOVNLE, NULL,			NULL},
{UAX_OP_CMOVG,	NULL,			NULL},
{0,				"WX0F4F/r",		"r16,rm16"},
{0,				"TX0F4F/r",		"r32,rm32"},
{0,				"X0F4F/r",		"r64,rm64"},
{UAX_OP_CMP,	NULL,			NULL},
{0,				"3C,ib",		"al,i8"},
{0,				"X80/7,ib",		"rm8,i8"},
{0,				"WX83/7,ib",	"rm16,i8"},
{0,				"TX83/7,ib",	"rm32,i8"},
{0,				"X83/7,ib",		"rm64,i8"},
{0,				"X3A/r",		"r8,rm8"},
{0,				"X38/R",		"rm8,r8"},
{0,				"W3D,iw",		"ax,i16"},
{0,				"WX81/7,iw",	"rm16,i16"},
{0,				"WX3B/r",		"r16,rm16"},
{0,				"WX39/R",		"rm16,r16"},
{0,				"T3D,id",		"eax,i32"},
{0,				"TX81/7,id",	"rm32,i32"},
{0,				"TX3B/r",		"r32,rm32"},
{0,				"TX39/R",		"rm32,r32"},
{0,				"X3D,id",		"rax,i32"},
{0,				"X81/7,id",		"rm64,i32"},
{0,				"X3B/r",		"r64,rm64"},
{0,				"X39/R",		"rm64,r64"},
{UAX_OP_CMPSB,	"A6",			NULL},
{UAX_OP_CMPSW,	"WA7",			NULL},
{UAX_OP_CMPSD,	"TA7",			NULL},
{0,				"F2X0FC2/r,ib",	"xr,xrm,u8"},
{UAX_OP_CMPXCHG, NULL,			NULL},
{0,				"X0FB0/R",		"rm8,r8"},
{0,				"WX0FB1/R",		"rm16,r16"},
{0,				"TX0FB1/R",		"rm32,r32"},
{0,				"X0FB1/R",		"rm64,r64"},
{UAX_OP_CPUID,	"0FA2",			NULL},
{UAX_OP_CWD,	"W99",			NULL},
{UAX_OP_CDQ,	"T99",			NULL},
{UAX_OP_CQO,	"4899",			NULL},
{UAX_OP_DAA,	"27",			NULL},
{UAX_OP_DAS,	"2F",			NULL},
{UAX_OP_DEC,	NULL,			NULL},
{0,				"XFE/1",		"rm8"},
{0,				"WXFF/1",		"rm16"},
{0,				"TXFF/1",		"rm32"},
{0,				"XFF/1",		"rm64"},
{UAX_OP_DIV,	NULL,			NULL},
{0,				"XF6/6",		"r8"},
{0,				"WXF7/6",		"r16"},
{0,				"TXF7/6",		"r32"},
{0,				"XF7/6",		"r64"},
{UAX_OP_EMMS,	"0F77",			NULL},
{UAX_OP_ENTER,	"C8,iw,ib",		"i16,i8"},
{UAX_OP_HLT,	"F4",			NULL},
{UAX_OP_IDIV,	NULL,			NULL},
{0,				"XF6/7",		"rm8"},
{0,				"WXF7/7",		"rm16"},
{0,				"TXF7/7",		"rm32"},
{0,				"XF7/7",		"rm64"},
{UAX_OP_IMUL,	NULL,			NULL},
{0,				"XF6/5",		"rm8"},
{0,				"WXF7/5",		"rm16"},
{0,				"TXF7/5",		"rm32"},
{0,				"XF7/5",		"rm64"},
{0,				"WX0FAF/r",		"r16,rm16"},
{0,				"TX0FAF/r",		"r32,rm32"},
{0,				"X0FAF/r",		"r64,rm64"},
{0,				"WX6B/r,ib",	"r16,i8"},
{0,				"TX6B/r,ib",	"r32,i8"},
{0,				"X6B/r,ib",		"r64,i8"},
{0,				"WX69/r,iw",	"r16,i16"},
{0,				"TX69/r,id",	"r32,i32"},
{0,				"X69/r,id",		"r64,i32"},
{0,				"WX6B/r,ib",	"r16,rm16,i8"},
{0,				"TX6B/r,ib",	"r32,rm32,i8"},
{0,				"X6B/r,ib",		"r64,rm64,i8"},
{0,				"WX69/r,iw",	"r16,rm16,i16"},
{0,				"TX69/r,id",	"r32,rm32,i32"},
{0,				"X69/r,id",		"r64,rm64,i32"},
{UAX_OP_IN,		NULL,			NULL},
{0,				"E4,ib",		"al,u8"},
{0,				"WE5,ib",		"ax,u8"},
{0,				"TE5,ib",		"eax,u8"},
{0,				"XE5,ib",		"rax,u8"},
{0,				"EC",			"al,dx"},
{0,				"WED",			"ax,dx"},
{0,				"TED",			"eax,dx"},
{0,				"48ED",			"rax,dx"},
{UAX_OP_INC,	NULL,			NULL},
{0,				"XFE/0",		"rm8"},
{0,				"WXFF/0",		"rm16"},
{0,				"TXFF/0",		"rm32"},
{0,				"XFF/0",		"rm64"},
{UAX_OP_INT,	"CC",			"3"},
{0,				"CD,ib",		"u8"},
{UAX_OP_INTO,	"CE",			NULL},
{UAX_OP_IRET,	"CF",			NULL},
{UAX_OP_JA,		NULL,			NULL},
{0,				"77,rb",		"ar8"},
{0,				"W0F87,rw",		"ar16"},
{0,				"T0F87,rd",		"ar32"},
{UAX_OP_JAE,	NULL,			NULL},
{0,				"73,rb",		"ar8"},
{0,				"W0F83,rw",		"ar16"},
{0,				"T0F83,rd",		"ar32"},
{UAX_OP_JB,		NULL,			NULL},
{0,				"72,rb",		"ar8"},
{0,				"W0F82,rw",		"ar16"},
{0,				"T0F82,rd",		"ar32"},
{UAX_OP_JBE,	NULL,			NULL},
{0,				"76,rb",		"ar8"},
{0,				"W0F86,rw",		"ar16"},
{0,				"T0F86,rd",		"ar32"},
{UAX_OP_JC,		NULL,			NULL},
{0,				"72,rb",		"ar8"},
{0,				"W0F82,rw",		"ar16"},
{0,				"T0F82,rd",		"ar32"},
{UAX_OP_JE,		NULL,			NULL},
{0,				"74,rb",		"ar8"},
{0,				"W0F84,rw",		"ar16"},
{0,				"T0F84,rd",		"ar32"},
{UAX_OP_JG,		NULL,			NULL},
{0,				"7F,rb",		"ar8"},
{0,				"W0F8F,rw",		"ar16"},
{0,				"T0F8F,rd",		"ar32"},
{UAX_OP_JGE,	NULL,			NULL},
{0,				"7D,rb",		"ar8"},
{0,				"W0F8D,rw",		"ar16"},
{0,				"T0F8D,rd",		"ar32"},
{UAX_OP_JL,		NULL,			NULL},
{0,				"7C,rb",		"ar8"},
{0,				"W0F8C,rw",		"ar16"},
{0,				"T0F8C,rd",		"ar32"},
{UAX_OP_JLE,	NULL,			NULL},
{0,				"7E,rb",		"ar8"},
{0,				"W0F8E,rw",		"ar16"},
{0,				"T0F8E,rd",		"ar32"},
{UAX_OP_JNA,	NULL,			NULL},
{0,				"76,rb",		"ar8"},
{0,				"W0F86,rw",		"ar16"},
{0,				"T0F86,rd",		"ar32"},
{UAX_OP_JNAE,	NULL,			NULL},
{0,				"72,rb",		"ar8"},
{0,				"W0F82,rw",		"ar16"},
{0,				"T0F82,rd",		"ar32"},
{UAX_OP_JNB,	NULL,			NULL},
{0,				"73,rb",		"ar8"},
{0,				"W0F83,rw",		"ar16"},
{0,				"T0F83,rd",		"ar32"},
{UAX_OP_JNBE,	NULL,			NULL},
{0,				"77,rb",		"ar8"},
{0,				"W0F87,rw",		"ar16"},
{0,				"T0F87,rd",		"ar32"},
{UAX_OP_JNC,	NULL,			NULL},
{0,				"73,rb",		"ar8"},
{0,				"W0F83,rw",		"ar16"},
{0,				"T0F83,rd",		"ar32"},
{UAX_OP_JNE,	NULL,			NULL},
{0,				"75,rb",		"ar8"},
{0,				"W0F85,rw",		"ar16"},
{0,				"T0F85,rd",		"ar32"},
{UAX_OP_JNG,	NULL,			NULL},
{0,				"7E,rb",		"ar8"},
{0,				"W0F8E,rw",		"ar16"},
{0,				"T0F8E,rd",		"ar32"},
{UAX_OP_JNGE,	NULL,			NULL},
{0,				"7C,rb",		"ar8"},
{0,				"W0F8C,rw",		"ar16"},
{0,				"T0F8C,rd",		"ar32"},
{UAX_OP_JNL,	NULL,			NULL},
{0,				"7D,rb",		"ar8"},
{0,				"W0F8D,rw",		"ar16"},
{0,				"T0F8D,rd",		"ar32"},
{UAX_OP_JNLE,	NULL,			NULL},
{0,				"7F,rb",		"ar8"},
{0,				"W0F8F,rw",		"ar16"},
{0,				"T0F8F,rd",		"ar32"},
{UAX_OP_JNO,	NULL,			NULL},
{0,				"71,rb",		"ar8"},
{0,				"W0F81,rw",		"ar16"},
{0,				"T0F81,rd",		"ar32"},
{UAX_OP_JNP,	NULL,			NULL},
{0,				"7B,rb",		"ar8"},
{0,				"W0F8B,rw",		"ar16"},
{0,				"T0F8B,rd",		"ar32"},
{UAX_OP_JNS,	NULL,			NULL},
{0,				"79,rb",		"ar8"},
{0,				"W0F89,rw",		"ar16"},
{0,				"T0F89,rd",		"ar32"},
{UAX_OP_JNZ,	NULL,			NULL},
{0,				"75,rb",		"ar8"},
{0,				"W0F85,rw",		"ar16"},
{0,				"T0F85,rd",		"ar32"},
{UAX_OP_JO,		NULL,			NULL},
{0,				"70,rb",		"ar8"},
{0,				"W0F80,rw",		"ar16"},
{0,				"T0F80,rd",		"ar32"},
{UAX_OP_JP,		NULL,			NULL},
{0,				"7A,rb",		"ar8"},
{0,				"W0F8A,rw",		"ar16"},
{0,				"T0F8A,rd",		"ar32"},
{UAX_OP_JPE,	NULL,			NULL},
{0,				"7A,rb",		"ar8"},
{0,				"W0F8A,rw",		"ar16"},
{0,				"T0F8A,rd",		"ar32"},
{UAX_OP_JPO,	NULL,			NULL},
{0,				"7B,rb",		"ar8"},
{0,				"W0F8B,rw",		"ar16"},
{0,				"T0F8B,rd",		"ar32"},
{UAX_OP_JS,		NULL,			NULL},
{0,				"78,rb",		"ar8"},
{0,				"W0F88,rw",		"ar16"},
{0,				"T0F88,rd",		"ar32"},
{UAX_OP_JZ,		NULL,			NULL},
{0,				"74,rb",		"ar8"},
{0,				"W0F84,rw",		"ar16"},
{0,				"T0F84,rd",		"ar32"},
{UAX_OP_JMP,	NULL,			NULL},
{0,				"EB,rb",		"ar8"},
{0,				"WE9,rw",		"ar16"},
{0,				"TE9,rd",		"ar32"},
{0,				"WXFF/4",		"rm16"},
{0,				"TXFF/4",		"rm32"},
{0,				"XFF/4",		"rm64"},
{UAX_OP_LAR,	NULL,			NULL},
{0,				"WX0F02/r",		"r16,rm16"},
{0,				"TX0F02/r",		"r32,rm32"},
{0,				"X0F02/r",		"r64,rm64"},
{UAX_OP_LEA,	NULL,			NULL},
{0,				"WX8D/r",		"r16,rm16"},
{0,				"TX8D/r",		"r32,rm32"},
{0,				"X8D/r",		"r64,rm64"},
{UAX_OP_LODSB,	"AC",			NULL},
{UAX_OP_LODSW,	"WAD",			NULL},
{UAX_OP_LODSD,	"TAD",			NULL},
{UAX_OP_LODSQ,	"48AD",			NULL},
{UAX_OP_LOOP,	"E2,rb",		"ar8"},
{UAX_OP_LOOPE,	"E1,rb",		"ar8"},
{UAX_OP_LOOPZ,	"E1,rb",		"ar8"},
{UAX_OP_LOOPNE,	"E0,rb",		"ar8"},
{UAX_OP_LOOPNZ,	"E0,rb",		"ar8"},
{UAX_OP_MOV,	NULL,			NULL},
{0,				"X8A/r",		"r8,rm8"},
{0,				"WX8B/r",		"r16,rm16"},
{0,				"TX8B/r",		"r32,rm32"},
{0,				"X8B/r",		"r64,rm64"},
{0,				"X88/R",		"rm8,r8"},
{0,				"WX89/R",		"rm16,r16"},
{0,				"TX89/R",		"rm32,r32"},
{0,				"X89/R",		"rm64,r64"},
{0,				"XB0|r,ib",		"r8,i8"},
{0,				"WXB8|r,iw",	"r16,i16"},
{0,				"TXB8|r,id",	"r32,i32"},
{0,				"XC6/0,ib",		"rm8,i8"},
{0,				"WXC7/0,iw",	"rm16,i16"},
{0,				"TXC7/0,id",	"rm32,i32"},
{0,				"XC7/0,id",		"rm64,i32"},
{0,				"XB8|r,iq",		"r64,i64"},

#if 0
{0,				"VA0,mw",		"al,mo16"},
{0,				"VWA1,mw",		"ax,mo16"},
{0,				"VTA1,mw",		"eax,mo16"},
{0,				"VTXA1,mw",		"rax,mo16"},
{0,				"VA2,mw",		"mo16,al"},
{0,				"VWA3,mw",		"mo16,ax"},
{0,				"VTA3,mw",		"mo16,eax"},
{0,				"VTXA3,mw",		"mo16,rax"},
#endif

#if 0
{0,				"X8C/R",		"rm16,sr"},
{0,				"X8E/r",		"sr,rm16"},
{0,				"X0F22/r",		"cr,r32"},
{0,				"X0F22/r",		"cr,r64"},
{0,				"X0F20/r",		"r32,cr"},
{0,				"X0F20/r",		"r64,cr"},
{0,				"X0F23/r",		"dr,r32"},
{0,				"X0F23/r",		"dr,r64"},
{0,				"X0F21/r",		"r32,dr"},
{0,				"X0F21/r",		"r64,dr"},
#endif
{UAX_OP_MOVSB,	"A4",			NULL},
{UAX_OP_MOVSW,	"WA5",			NULL},
{UAX_OP_MOVSD,	"TA5",			NULL},
{0,				"F2X0F10/r",	"xr,xrm"},
{0,				"F2X0F11/r",	"xrm,xr"},
{UAX_OP_MOVSQ,	"48A5",			NULL},
{UAX_OP_MOVSX,	NULL,			NULL},
{0,				"WX0FBE/r",		"r16,rm8"},
{0,				"TX0FBE/r",		"r32,rm8"},
{0,				"X0FBE/r",		"r64,rm8"},
{0,				"X0FBF/r",		"r32,rm16"},
{0,				"X0FBF/r",		"r64,rm16"},
{0,				"X63/r",		"r64,rm32"},
{UAX_OP_MOVSXB,	NULL,			NULL},
{0,				"WX0FBE/r",		"r16,rm8"},
{0,				"TX0FBE/r",		"r32,rm8"},
{0,				"X0FBE/r",		"r64,rm8"},
{UAX_OP_MOVSXW,	NULL,			NULL},
{0,				"X0FBF/r",		"r32,rm16"},
{0,				"X0FBF/r",		"r64,rm32"},
{UAX_OP_MOVSXD,	"X63/r",		"r64,rm32"},
{UAX_OP_MOVZX,	NULL,			NULL},
{0,				"WX0FB6/r",		"r16,rm8"},
{0,				"TX0FB6/r",		"r32,rm8"},
{0,				"X0FB6/r",		"r64,rm8"},
{0,				"X0FB7/r",		"r32,rm16"},
{0,				"X0FB7/r",		"r64,rm32"},
{UAX_OP_MOVZXB,	NULL,			NULL},
{0,				"WX0FB6/r",		"r16,rm8"},
{0,				"TX0FB6/r",		"r32,rm8"},
{0,				"X0FB6/r",		"r64,rm8"},
{UAX_OP_MOVZXW,	NULL,			NULL},
{0,				"X0FB7/r",		"r32,rm16"},
{0,				"X0FB7/r",		"r64,rm32"},
{UAX_OP_MOVZXD,	"X0FB7/r",		"r64,rm32"},
{UAX_OP_MUL,	NULL,			NULL},
{0,				"XF6/4",		"rm8"},
{0,				"WXF7/4",		"rm16"},
{0,				"TXF7/4",		"rm32"},
{0,				"XF7/4",		"rm64"},
{UAX_OP_NEG,	NULL,			NULL},
{0,				"XF6/3",		"rm8"},
{0,				"WXF7/3",		"rm16"},
{0,				"TXF7/3",		"rm32"},
{0,				"XF7/3",		"rm64"},
{UAX_OP_NOP,	NULL,			NULL},
{0,				"90",			NULL},
{0,				"0F1F/0",		"rm16"},
{0,				"0F1F/0",		"rm32"},
{UAX_OP_NOP2,	"6690",			NULL},
{UAX_OP_NOT,	NULL,			NULL},
{0,				"XF6/2",		"rm8"},
{0,				"WXF7/2",		"rm16"},
{0,				"TXF7/2",		"rm32"},
{0,				"XF7/2",		"rm64"},
{UAX_OP_OR,		NULL,			NULL},
{0,				"0C,ib",		"al,i8"},
{0,				"X80/1,ib",		"rm8,i8"},
{0,				"WX83/1,ib",	"rm16,i8"},
{0,				"TX83/1,ib",	"rm32,i8"},
{0,				"X83/1,ib",		"rm64,i8"},
{0,				"X0A/r",		"r8,rm8"},
{0,				"X08/R",		"rm8,r8"},
{0,				"WX0D,iw",		"ax,i16"},
{0,				"WX81/1,iw",	"rm16,i16"},
{0,				"WX0B/r",		"r16,rm16"},
{0,				"WX09/R",		"rm16,r16"},
{0,				"T0D,id",		"eax,i32"},
{0,				"TX81/1,id",	"rm32,i32"},
{0,				"TX0B/r",		"r32,rm32"},
{0,				"TX09/R",		"rm32,r32"},
{0,				"480D,id",		"rax,i32"},
{0,				"X81/1,id",		"rm64,i32"},
{0,				"X0B/r",		"r64,rm64"},
{0,				"X09/R",		"rm64,r64"},
{UAX_OP_OUT,	NULL,			NULL},
{0,				"E6,ib",		"u8,al"},
{0,				"WE7,ib",		"u8,ax"},
{0,				"TE7,ib",		"u8,eax"},
{0,				"EE",			"dx,al"},
{0,				"WEF",			"dx,ax"},
{0,				"TEF",			"dx,eax"},
{UAX_OP_POP,	NULL,			NULL},
{0,				"WX58|r",		"r16"},
{0,				"TX58|r",		"r32"},
{0,				"X58|r",		"r64"},
{0,				"WX8F/0",		"rm16"},
// {0,				"TX8F/0",		"rm32"},
{0,				"X8F/0",		"rm64"},

{UAX_OP_POPA,	"W61",			NULL},
{UAX_OP_POPAD,	"T61",			NULL},
{UAX_OP_POPF,	"W9D",			NULL},
{UAX_OP_POPFD,	"T9D",			NULL},
{UAX_OP_POPFQ,	"489D",			NULL},
{UAX_OP_PUSH,	NULL,			NULL},
{0,				"WX50|r",		"r16"},
{0,				"TX50|r",		"r32"},
{0,				"X50|r",		"r64"},
{0,				"6A,ib",		"i8"},
{0,				"W68,iw",		"i16"},
{0,				"T68,id",		"i32"},
{0,				"WXFF/6",		"rm16"},
{0,				"TXFF/6",		"rm32"},
{0,				"XFF/6",		"rm64"},
{UAX_OP_PUSHA,	"60",			NULL},
{UAX_OP_PUSHAW,	"W60",			NULL},
{UAX_OP_PUSHAD,	"T60",			NULL},
{UAX_OP_PUSHF,	"9C",			NULL},
{UAX_OP_PUSHFW,	"W9C",			NULL},
{UAX_OP_PUSHFD,	"T9C",			NULL},
{UAX_OP_PUSHFQ,	"9C",			NULL},
{UAX_OP_RCL,	NULL,			NULL},
{0,				"XD0/2",		"rm8"},
{0,				"XD0/2",		"rm8,1"},
{0,				"XD2/2",		"rm8,r8"},
{0,				"XC0/2,ib",		"rm8,i8"},
{0,				"WXD1/2",		"rm16"},
{0,				"WXD1/2",		"rm16,1"},
{0,				"WXD3/2",		"rm16,r8"},
{0,				"WXC1/2,ib",	"rm16,i8"},
{0,				"TXD1/2",		"rm32"},
{0,				"TXD1/2",		"rm32,1"},
{0,				"TXD3/2",		"rm32,r8"},
{0,				"TXC1/2,ib",	"rm32,i8"},
{0,				"XD1/2",		"rm64"},
{0,				"XD1/2",		"rm64,1"},
{0,				"XD3/2",		"rm64,r8"},
{0,				"XC1/2,ib",		"rm64,i8"},
{UAX_OP_RCR,	NULL,			NULL},
{0,				"XD0/3",		"rm8"},
{0,				"XD0/3",		"rm8,1"},
{0,				"XD2/3",		"rm8,r8"},
{0,				"XC0/3,ib",		"rm8,i8"},
{0,				"WXD1/3",		"rm16"},
{0,				"WXD1/3",		"rm16,1"},
{0,				"WXD3/3",		"rm16,r8"},
{0,				"WXC1/3,ib",	"rm16,i8"},
{0,				"TXD1/3",		"rm32"},
{0,				"TXD1/3",		"rm32,1"},
{0,				"TXD3/3",		"rm32,r8"},
{0,				"TXC1/3,ib",	"rm32,i8"},
{0,				"XD1/3",		"rm64"},
{0,				"XD1/3",		"rm64,1"},
{0,				"XD3/3",		"rm64,r8"},
{0,				"XC1/3,ib",		"rm64,i8"},
{UAX_OP_ROL,	NULL,			NULL},
{0,				"XD0/0",		"rm8"},
{0,				"XD0/0",		"rm8,1"},
{0,				"XD2/0",		"rm8,r8"},
{0,				"XC0/0,ib",		"rm8,i8"},
{0,				"WXD1/0",		"rm16"},
{0,				"WXD1/0",		"rm16,1"},
{0,				"WXD3/0",		"rm16,r8"},
{0,				"WXC1/0,ib",	"rm16,i8"},
{0,				"TXD1/0",		"rm32"},
{0,				"TXD1/0",		"rm32,1"},
{0,				"TXD3/0",		"rm32,r8"},
{0,				"TXC1/0,ib",	"rm32,i8"},
{0,				"XD1/0",		"rm64"},
{0,				"XD1/0",		"rm64,1"},
{0,				"XD3/0",		"rm64,r8"},
{0,				"XC1/0,ib",		"rm64,i8"},
{UAX_OP_ROR,	NULL,			NULL},
{0,				"XD0/1",		"rm8"},
{0,				"XD0/1",		"rm8,1"},
{0,				"XD2/1",		"rm8,r8"},
{0,				"XC0/1,ib",		"rm8,i8"},
{0,				"WXD1/1",		"rm16"},
{0,				"WXD1/1",		"rm16,1"},
{0,				"WXD3/1",		"rm16,r8"},
{0,				"WXC1/1,ib",	"rm16,i8"},
{0,				"TXD1/1",		"rm32"},
{0,				"TXD1/1",		"rm32,1"},
{0,				"TXD3/1",		"rm32,r8"},
{0,				"TXC1/1,ib",	"rm32,i8"},
{0,				"XD1/1",		"rm64"},
{0,				"XD1/1",		"rm64,1"},
{0,				"XD3/1",		"rm64,r8"},
{0,				"XC1/1,ib",		"rm64,i8"},
{UAX_OP_RDTSC,	"0F31",			NULL},
{UAX_OP_RET,	NULL,			NULL},
{0,				"C3",			NULL},
{0,				"C2,iw",		"i16"},
{UAX_OP_RETF,	NULL,			NULL},
{0,				"CB",			NULL},
{0,				"CA,iw",		"i16"},
{UAX_OP_SAL,	NULL,			NULL},
{0,				"XD0/4",		"rm8"},
{0,				"XD0/4",		"rm8,1"},
{0,				"XD2/4",		"rm8,r8"},
{0,				"XC0/4,ib",		"rm8,i8"},
{0,				"WXD1/4",		"rm16"},
{0,				"WXD1/4",		"rm16,1"},
{0,				"WXD3/4",		"rm16,r8"},
{0,				"WXC1/4,ib",	"rm16,i8"},
{0,				"TXD1/4",		"rm32"},
{0,				"TXD1/4",		"rm32,1"},
{0,				"TXD3/4",		"rm32,r8"},
{0,				"TXC1/4,ib",	"rm32,i8"},
{0,				"XD1/4",		"rm64"},
{0,				"XD1/4",		"rm64,1"},
{0,				"XD3/4",		"rm64,r8"},
{0,				"XC1/4,ib",		"rm64,i8"},
{UAX_OP_SAR,	NULL,			NULL},
{0,				"XD0/7",		"rm8"},
{0,				"XD0/7",		"rm8,1"},
{0,				"XD2/7",		"rm8,r8"},
{0,				"XC0/7,ib",		"rm8,i8"},
{0,				"WXD1/7",		"rm16"},
{0,				"WXD1/7",		"rm16,1"},
{0,				"WXD3/7",		"rm16,r8"},
{0,				"WXC1/7,ib",	"rm16,i8"},
{0,				"TXD1/7",		"rm32"},
{0,				"TXD1/7",		"rm32,1"},
{0,				"TXD3/7",		"rm32,r8"},
{0,				"TXC1/7,ib",	"rm32,i8"},
{0,				"XD1/7",		"rm64"},
{0,				"XD1/7",		"rm64,1"},
{0,				"XD3/7",		"rm64,r8"},
{0,				"XC1/7,ib",		"rm64,i8"},
{UAX_OP_SBB,	NULL,			NULL},
{0,				"1C,ib",		"al,i8"},
{0,				"X80/3,ib",		"rm8,i8"},
{0,				"WX83/3,ib",	"rm16,i8"},
{0,				"TX83/3,ib",	"rm32,i8"},
{0,				"X83/3,ib",		"rm64,i8"},
{0,				"X1A/r",		"r8,rm8"},
{0,				"X18/R",		"rm8,r8"},
{0,				"WX1D,iw",		"ax,i16"},
{0,				"WX81/3,iw",	"rm16,i16"},
{0,				"WX1B/r",		"r16,rm16"},
{0,				"WX19/R",		"rm16,r16"},
{0,				"T1D,id",		"eax,i32"},
{0,				"TX81/3,id",	"rm32,i32"},
{0,				"TX1B/r",		"r32,rm32"},
{0,				"TX19/R",		"rm32,r32"},
{0,				"481D,id",		"rax,i32"},
{0,				"X81/3,id",		"rm64,i32"},
{0,				"X1B/r",		"r64,rm64"},
{0,				"X19/R",		"rm64,r64"},
{UAX_OP_SCAS,	NULL,			NULL},
{0,				"AE",			"rm8"},
{0,				"WAF",			"rm16"},
{0,				"TAF",			"rm32"},
{0,				"48AF",			"rm64"},
{UAX_OP_SCASB,	"AE",			NULL},
{UAX_OP_SCASW,	"WAF",			NULL},
{UAX_OP_SCASD,	"TAF",			NULL},
{UAX_OP_SCASQ,	"48AF",			NULL},
{UAX_OP_SETA,	"X0F97/0",		"rm8"},
{UAX_OP_SETAE,	"X0F93/0",		"rm8"},
{UAX_OP_SETB,	"X0F92/0",		"rm8"},
{UAX_OP_SETBE,	"X0F96/0",		"rm8"},
{UAX_OP_SETC,	"X0F92/0",		"rm8"},
{UAX_OP_SETE,	"X0F94/0",		"rm8"},
{UAX_OP_SETG,	"X0F9F/0",		"rm8"},
{UAX_OP_SETGE,	"X0F9D/0",		"rm8"},
{UAX_OP_SETL,	"X0F9C/0",		"rm8"},
{UAX_OP_SETLE,	"X0F9E/0",		"rm8"},
{UAX_OP_SETNA,	"X0F96/0",		"rm8"},
{UAX_OP_SETNAE,	"X0F92/0",		"rm8"},
{UAX_OP_SETNB,	"X0F93/0",		"rm8"},
{UAX_OP_SETNBE,	"X0F97/0",		"rm8"},
{UAX_OP_SETNC,	"X0F93/0",		"rm8"},
{UAX_OP_SETNE,	"X0F95/0",		"rm8"},
{UAX_OP_SETNG,	"X0F9E/0",		"rm8"},
{UAX_OP_SETNGE,	"X0F9C/0",		"rm8"},
{UAX_OP_SETNL,	"X0F9D/0",		"rm8"},
{UAX_OP_SETNLE,	"X0F9F/0",		"rm8"},
{UAX_OP_SETNO,	"X0F91/0",		"rm8"},
{UAX_OP_SETNP,	"X0F9B/0",		"rm8"},
{UAX_OP_SETNS,	"X0F99/0",		"rm8"},
{UAX_OP_SETNZ,	"X0F95/0",		"rm8"},
{UAX_OP_SETO,	"X0F90/0",		"rm8"},
{UAX_OP_SETP,	"X0F9A/0",		"rm8"},
{UAX_OP_SETPE,	"X0F9A/0",		"rm8"},
{UAX_OP_SETPO,	"X0F9B/0",		"rm8"},
{UAX_OP_SETS,	"X0F98/0",		"rm8"},
{UAX_OP_SETZ,	"X0F94/0",		"rm8"},
{UAX_OP_SHL,	NULL,			NULL},
{0,				"XD0/4",		"rm8"},
{0,				"XD0/4",		"rm8,1"},
{0,				"XD2/4",		"rm8,cl"},
{0,				"XC0/4,ib",		"rm8,i8"},
{0,				"WXD1/4",		"rm16"},
{0,				"WXD1/4",		"rm16,1"},
{0,				"WXD3/4",		"rm16,cl"},
{0,				"WXC1/4,ib",	"rm16,i8"},
{0,				"TXD1/4",		"rm32"},
{0,				"TXD1/4",		"rm32,1"},
{0,				"TXD3/4",		"rm32,cl"},
{0,				"TXC1/4,ib",	"rm32,i8"},
{0,				"XD1/4",		"rm64"},
{0,				"XD1/4",		"rm64,1"},
{0,				"XD3/4",		"rm64,cl"},
{0,				"XC1/4,ib",		"rm64,i8"},
{UAX_OP_SHLD,	NULL,			NULL},
{0,				"WX0FA5/r,ib",	"rm16,r16"},
{0,				"TX0FA5/r,ib",	"rm32,r32"},
{0,				"X0FA5/r,ib",	"rm64,r64"},
{0,				"WX0FA5/r,ib",	"rm16,r16,1"},
{0,				"TX0FA5/r,ib",	"rm32,r32,1"},
{0,				"X0FA5/r,ib",	"rm64,r64,1"},
{0,				"WX0FA4/r,ib",	"rm16,r16,i8"},
{0,				"WX0FA5/r,ib",	"rm16,r16,cl"},
{0,				"TX0FA4/r,ib",	"rm32,r32,i8"},
{0,				"TX0FA5/r,ib",	"rm32,r32,cl"},
{0,				"X0FA4/r,ib",	"rm64,r64,i8"},
{0,				"X0FA5/r,ib",	"rm64,r64,cl"},
{UAX_OP_SHR,	NULL,			NULL},
{0,				"XD0/5",		"rm8"},
{0,				"XD0/5",		"rm8,1"},
{0,				"XD2/5",		"rm8,cl"},
{0,				"XC0/5,ib",		"rm8,i8"},
{0,				"WXD1/5",		"rm16"},
{0,				"WXD1/5",		"rm16,1"},
{0,				"WXD3/5",		"rm16,cl"},
{0,				"WXC1/5,ib",	"rm16,i8"},
{0,				"TXD1/5",		"rm32"},
{0,				"TXD1/5",		"rm32,1"},
{0,				"TXD3/5",		"rm32,cl"},
{0,				"TXC1/5,ib",	"rm32,i8"},
{0,				"XD1/5",		"rm64"},
{0,				"XD1/5",		"rm64,1"},
{0,				"XD3/5",		"rm64,cl"},
{0,				"XC1/5,ib",		"rm64,i8"},
{UAX_OP_SHRD,	NULL,			NULL},
{0,				"WX0FAD/r,ib",	"rm16,r16"},
{0,				"TX0FAD/r,ib",	"rm32,r32"},
{0,				"X0FAD/r,ib",	"rm64,r64"},
{0,				"WX0FAD/r,ib",	"rm16,r16,1"},
{0,				"TX0FAD/r,ib",	"rm32,r32,1"},
{0,				"X0FAD/r,ib",	"rm64,r64,1"},
{0,				"WX0FAC/r,ib",	"rm16,r16,i8"},
{0,				"WX0FAD/r,ib",	"rm16,r16,cl"},
{0,				"TX0FAC/r,ib",	"rm32,r32,i8"},
{0,				"TX0FAD/r,ib",	"rm32,r32,cl"},
{0,				"X0FAC/r,ib",	"rm64,r64,i8"},
{0,				"X0FAD/r,ib",	"rm64,r64,cl"},
{UAX_OP_STC,	"F9",			NULL},
{UAX_OP_STD,	"FD",			NULL},
{UAX_OP_STI,	"FB",			NULL},
{UAX_OP_STOSB,	"AA",			NULL},
{UAX_OP_STOSW,	"WAB",			NULL},
{UAX_OP_STOSD,	"TAB",			NULL},
{UAX_OP_STR,	"0F00/1",		"rm16"},
{UAX_OP_SUB,	NULL,			NULL},
{0,				"2C,ib",		"al,i8"},
{0,				"X80/5,ib",		"rm8,i8"},
{0,				"WX83/5,ib",	"rm16,i8"},
{0,				"TX83/5,ib",	"rm32,i8"},
{0,				"X83/5,ib",		"rm64,i8"},
{0,				"X2A/r",		"r8,rm8"},
{0,				"X28/R",		"rm8,r8"},
{0,				"W2D,iw",		"ax,i16"},
{0,				"WX81/5,iw",	"rm16,i16"},
{0,				"WX2B/r",		"r16,rm16"},
{0,				"WX29/R",		"rm16,r16"},
{0,				"T2D,id",		"eax,i32"},
{0,				"TX81/5,id",	"rm32,i32"},
{0,				"TX2B/r",		"r32,rm32"},
{0,				"TX29/R",		"rm32,r32"},
{0,				"482D,id",		"rax,i32"},
{0,				"X81/5,id",		"rm64,i32"},
{0,				"X2B/r",		"r64,rm64"},
{0,				"X29/R",		"rm64,r64"},
{UAX_OP_TEST,	NULL,			NULL},
{0,				"A8,ib",		"al,i8"},
{0,				"WA9,iw",		"ax,i16"},
{0,				"TA9,id",		"eax,i32"},
{0,				"48A9,id",		"rax,i32"},
{0,				"XF6/0,ib",		"rm8,i8"},
{0,				"WXF7/0,iw",	"rm16,i16"},
{0,				"TXF7/0,id",	"rm32,i32"},
{0,				"XF7/0,id",		"rm64,i32"},
{0,				"X84/R",		"rm8,r8"},
{0,				"WX85/R",		"rm16,r16"},
{0,				"TX85/R",		"rm32,r32"},
{0,				"X85/R",		"rm64,r64"},
{UAX_OP_XADD,	NULL,			NULL},
{0,				"X0FC0/R",		"rm8,r8"},
{0,				"WX0FC1/R",		"rm16,r16"},
{0,				"TX0FC1/R",		"rm32,r32"},
{0,				"X0FC1/R",		"rm64,r64"},
{UAX_OP_XCHG,	NULL,			NULL},
{0,				"WX87C0",		"ax,ax"},
{0,				"TX87C0",		"eax,eax"},
{0,				"WX90|r",		"ax,r16"},
{0,				"WX90|r",		"r16,ax"},
{0,				"TX90|r",		"eax,r32"},
{0,				"TX90|r",		"r32,eax"},
{0,				"X90|r",		"rax,r64"},
{0,				"X90|r",		"r64,rax"},
{0,				"X86/R",		"rm8,r8"},
{0,				"X86/r",		"r8,rm8"},
{0,				"WX87/R",		"rm16,r16"},
{0,				"WX87/r",		"r16,rm16"},
{0,				"TX87/R",		"rm32,r32"},
{0,				"TX87/r",		"r32,rm32"},
{0,				"X87/R",		"rm64,r64"},
{0,				"X87/r",		"r64,rm64"},
{UAX_OP_XOR,	NULL,			NULL},
{0,				"34,ib",		"al,i8"},
{0,				"X80/6,ib",		"rm8,i8"},
{0,				"WX83/6,ib",	"rm16,i8"},
{0,				"TX83/6,ib",	"rm32,i8"},
{0,				"X83/6,ib",		"rm64,i8"},
{0,				"X32/r",		"r8,rm8"},
{0,				"X30/R",		"rm8,r8"},
{0,				"W35,iw",		"ax,i16"},
{0,				"WX81/6,iw",	"rm16,i16"},
{0,				"WX33/r",		"r16,rm16"},
{0,				"WX31/R",		"rm16,r16"},
{0,				"T35,id",		"eax,i32"},
{0,				"TX81/6,id",	"rm32,i32"},
{0,				"TX33/r",		"r32,rm32"},
{0,				"TX31/R",		"rm32,r32"},
{0,				"4835,id",		"rax,i32"},
{0,				"X81/6,id",		"rm64,i32"},
{0,				"X33/r",		"r64,rm64"},
{0,				"X31/R",		"rm64,r64"},

{0,				NULL,			NULL},
};