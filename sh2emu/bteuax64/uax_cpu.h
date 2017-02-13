#define UAX_REG_EAX		0x00
#define UAX_REG_ECX		0x01
#define UAX_REG_EDX		0x02
#define UAX_REG_EBX		0x03
#define UAX_REG_ESP		0x04
#define UAX_REG_EBP		0x05
#define UAX_REG_ESI		0x06
#define UAX_REG_EDI		0x07

#define UAX_REG_RAX		0x10
#define UAX_REG_RCX		0x11
#define UAX_REG_RDX		0x12
#define UAX_REG_RBX		0x13
#define UAX_REG_RSP		0x14
#define UAX_REG_RBP		0x15
#define UAX_REG_RSI		0x16
#define UAX_REG_RDI		0x17

#define UAX_REG_AX		0x20
#define UAX_REG_CX		0x21
#define UAX_REG_DX		0x22
#define UAX_REG_BX		0x23
#define UAX_REG_SP		0x24
#define UAX_REG_BP		0x25
#define UAX_REG_SI		0x26
#define UAX_REG_DI		0x27

#define UAX_REG_AL		0x30
#define UAX_REG_CL		0x31
#define UAX_REG_DL		0x32
#define UAX_REG_BL		0x33
#define UAX_REG_SPL		0x34
#define UAX_REG_BPL		0x35
#define UAX_REG_SIL		0x36
#define UAX_REG_DIL		0x37

#define UAX_REG_R0D		0x00
#define UAX_REG_R1D		0x01
#define UAX_REG_R2D		0x02
#define UAX_REG_R3D		0x03
#define UAX_REG_R4D		0x04
#define UAX_REG_R5D		0x05
#define UAX_REG_R6D		0x06
#define UAX_REG_R7D		0x07
#define UAX_REG_R8D		0x08
#define UAX_REG_R9D		0x09
#define UAX_REG_R10D	0x0A
#define UAX_REG_R11D	0x0B
#define UAX_REG_R12D	0x0C
#define UAX_REG_R13D	0x0D
#define UAX_REG_R14D	0x0E
#define UAX_REG_R15D	0x0F

#define UAX_REG_R0Q		0x10
#define UAX_REG_R1Q		0x11
#define UAX_REG_R2Q		0x12
#define UAX_REG_R3Q		0x13
#define UAX_REG_R4Q		0x14
#define UAX_REG_R5Q		0x15
#define UAX_REG_R6Q		0x16
#define UAX_REG_R7Q		0x17
#define UAX_REG_R8Q		0x18
#define UAX_REG_R9Q		0x19
#define UAX_REG_R10Q	0x1A
#define UAX_REG_R11Q	0x1B
#define UAX_REG_R12Q	0x1C
#define UAX_REG_R13Q	0x1D
#define UAX_REG_R14Q	0x1E
#define UAX_REG_R15Q	0x1F

#define UAX_REG_R0W		0x20
#define UAX_REG_R1W		0x21
#define UAX_REG_R2W		0x22
#define UAX_REG_R3W		0x23
#define UAX_REG_R4W		0x24
#define UAX_REG_R5W		0x25
#define UAX_REG_R6W		0x26
#define UAX_REG_R7W		0x27
#define UAX_REG_R8W		0x28
#define UAX_REG_R9W		0x29
#define UAX_REG_R10W	0x2A
#define UAX_REG_R11W	0x2B
#define UAX_REG_R12W	0x2C
#define UAX_REG_R13W	0x2D
#define UAX_REG_R14W	0x2E
#define UAX_REG_R15W	0x2F

#define UAX_REG_R0B		0x30
#define UAX_REG_R1B		0x31
#define UAX_REG_R2B		0x32
#define UAX_REG_R3B		0x33
#define UAX_REG_R4B		0x34
#define UAX_REG_R5B		0x35
#define UAX_REG_R6B		0x36
#define UAX_REG_R7B		0x37
#define UAX_REG_R8B		0x38
#define UAX_REG_R9B		0x39
#define UAX_REG_R10B	0x3A
#define UAX_REG_R11B	0x3B
#define UAX_REG_R12B	0x3C
#define UAX_REG_R13B	0x3D
#define UAX_REG_R14B	0x3E
#define UAX_REG_R15B	0x3F

#define UAX_REG_RIP		0xF0		//no register
#define UAX_REG_Z		0xFF		//no register

#define UAX_RLC_REL8	0x01		//
#define UAX_RLC_REL16	0x02		//
#define UAX_RLC_REL32	0x03		//
#define UAX_RLC_ABS8	0x04		//
#define UAX_RLC_ABS16	0x05		//
#define UAX_RLC_ABS32	0x06		//
#define UAX_RLC_ABS64	0x07		//

#define UAX_CSEG_TEXT	0x00		//.text section
#define UAX_CSEG_DATA	0x01		//.data section
#define UAX_CSEG_BSS	0x02		//.bss section

#define UAX_OP_ADC					4
#define UAX_OP_ADD					5
#define UAX_OP_AND					6
#define UAX_OP_BSF					7
#define UAX_OP_BSR					8
#define UAX_OP_BSWAP				9
#define UAX_OP_BT					10
#define UAX_OP_BTC					11
#define UAX_OP_BTR					12
#define UAX_OP_BTS					13
#define UAX_OP_CALL					14
#define UAX_OP_CALL_W				15
#define UAX_OP_CBW					16
#define UAX_OP_CWDE					17
#define UAX_OP_CDQE					18
#define UAX_OP_CLC					19
#define UAX_OP_CLD					20
#define UAX_OP_CLI					21
#define UAX_OP_CLTS					22
#define UAX_OP_CMC					23
#define UAX_OP_CMOVO				24
#define UAX_OP_CMOVNO				25
#define UAX_OP_CMOVNAE				26
#define UAX_OP_CMOVB				27
#define UAX_OP_CMOVNB				28
#define UAX_OP_CMOVNC				29
#define UAX_OP_CMOVAE				30
#define UAX_OP_CMOVZ				31
#define UAX_OP_CMOVE				32
#define UAX_OP_CMOVNZ				33
#define UAX_OP_CMOVNE				34
#define UAX_OP_CMOVNA				35
#define UAX_OP_CMOVBE				36
#define UAX_OP_CMOVNBE				37
#define UAX_OP_CMOVA				38
#define UAX_OP_CMOVS				39
#define UAX_OP_CMOVNS				40
#define UAX_OP_CMOVPE				41
#define UAX_OP_CMOVP				42
#define UAX_OP_CMOVNP				43
#define UAX_OP_CMOVPO				44
#define UAX_OP_CMOVNGE				45
#define UAX_OP_CMOVL				46
#define UAX_OP_CMOVNL				47
#define UAX_OP_CMOVGE				48
#define UAX_OP_CMOVNG				49
#define UAX_OP_CMOVLE				50
#define UAX_OP_CMOVNLE				51
#define UAX_OP_CMOVG				52
#define UAX_OP_CMP					53
#define UAX_OP_CMPSB				54
#define UAX_OP_CMPSW				55
#define UAX_OP_CMPSD				56
#define UAX_OP_CMPXCHG				57
#define UAX_OP_CPUID				58
#define UAX_OP_CWD					59
#define UAX_OP_CDQ					60
#define UAX_OP_CQO					61
#define UAX_OP_DAA					62
#define UAX_OP_DAS					63
#define UAX_OP_DEC					64
#define UAX_OP_DIV					65
#define UAX_OP_EMMS					66
#define UAX_OP_ENTER				67
#define UAX_OP_HLT					68
#define UAX_OP_IDIV					69
#define UAX_OP_IMUL					70
#define UAX_OP_IN					71
#define UAX_OP_INC					72
#define UAX_OP_INSB					73
#define UAX_OP_INSW					74
#define UAX_OP_INSD					75
#define UAX_OP_INSQ					76
#define UAX_OP_INT					77
#define UAX_OP_INTO					78
#define UAX_OP_INTXT				79
#define UAX_OP_INVLD				80
#define UAX_OP_INVPLG				81
#define UAX_OP_IRET					82
#define UAX_OP_IRETD				83
#define UAX_OP_JA					84
#define UAX_OP_JAE					85
#define UAX_OP_JB					86
#define UAX_OP_JBE					87
#define UAX_OP_JC					88
#define UAX_OP_JE					89
#define UAX_OP_JG					90
#define UAX_OP_JGE					91
#define UAX_OP_JL					92
#define UAX_OP_JLE					93
#define UAX_OP_JNA					94
#define UAX_OP_JNAE					95
#define UAX_OP_JNB					96
#define UAX_OP_JNBE					97
#define UAX_OP_JNC					98
#define UAX_OP_JNE					99
#define UAX_OP_JNG					100
#define UAX_OP_JNGE					101
#define UAX_OP_JNL					102
#define UAX_OP_JNLE					103
#define UAX_OP_JNO					104
#define UAX_OP_JNP					105
#define UAX_OP_JNS					106
#define UAX_OP_JNZ					107
#define UAX_OP_JO					108
#define UAX_OP_JP					109
#define UAX_OP_JPE					110
#define UAX_OP_JPO					111
#define UAX_OP_JS					112
#define UAX_OP_JZ					113
#define UAX_OP_JMP					114
#define UAX_OP_LAHF					115
#define UAX_OP_LAR					116
#define UAX_OP_LEA					117
#define UAX_OP_LEAVE				118
#define UAX_OP_LGDT					119
#define UAX_OP_LIDT					120
#define UAX_OP_LLDT					121
#define UAX_OP_LMSW					122
#define UAX_OP_LMTR					123
#define UAX_OP_LODSB				124
#define UAX_OP_LODSW				125
#define UAX_OP_LODSD				126
#define UAX_OP_LODSQ				127
#define UAX_OP_LOOP					128
#define UAX_OP_LOOPE				129
#define UAX_OP_LOOPZ				130
#define UAX_OP_LOOPNE				131
#define UAX_OP_LOOPNZ				132
#define UAX_OP_LSL					133
#define UAX_OP_MOV					134
#define UAX_OP_MOVSB				135
#define UAX_OP_MOVSW				136
#define UAX_OP_MOVSD				137
#define UAX_OP_MOVSQ				138
#define UAX_OP_MOVSX				139
#define UAX_OP_MOVSXB				140
#define UAX_OP_MOVSXW				141
#define UAX_OP_MOVSXD				142
#define UAX_OP_MOVZX				143
#define UAX_OP_MOVZXB				144
#define UAX_OP_MOVZXW				145
#define UAX_OP_MOVZXD				146
#define UAX_OP_MUL					147
#define UAX_OP_NEG					148
#define UAX_OP_NOP					149
#define UAX_OP_NOP2					150
#define UAX_OP_NOT					151
#define UAX_OP_OR					152
#define UAX_OP_OUT					153
#define UAX_OP_POP					154
#define UAX_OP_POPA					155
#define UAX_OP_POPAD				156
#define UAX_OP_POPF					157
#define UAX_OP_POPFD				158
#define UAX_OP_POPFQ				159
#define UAX_OP_PUSH					160
#define UAX_OP_PUSH_CS				161
#define UAX_OP_PUSH_SS				162
#define UAX_OP_PUSH_DS				163
#define UAX_OP_PUSH_ES				164
#define UAX_OP_PUSH_FS				165
#define UAX_OP_PUSH_GS				166
#define UAX_OP_PUSHA				167
#define UAX_OP_PUSHAW				168
#define UAX_OP_PUSHAD				169
#define UAX_OP_PUSHF				170
#define UAX_OP_PUSHFW				171
#define UAX_OP_PUSHFD				172
#define UAX_OP_PUSHFQ				173
#define UAX_OP_RCL					174
#define UAX_OP_RCR					175
#define UAX_OP_ROL					176
#define UAX_OP_ROR					177
#define UAX_OP_RDTSC				178
#define UAX_OP_RET					179
#define UAX_OP_RETF					180
#define UAX_OP_SAHF					181
#define UAX_OP_SAL					182
#define UAX_OP_SAR					183
#define UAX_OP_SBB					184
#define UAX_OP_SCAS					185
#define UAX_OP_SCASB				186
#define UAX_OP_SCASW				187
#define UAX_OP_SCASD				188
#define UAX_OP_SCASQ				189
#define UAX_OP_SETA					190
#define UAX_OP_SETAE				191
#define UAX_OP_SETB					192
#define UAX_OP_SETBE				193
#define UAX_OP_SETC					194
#define UAX_OP_SETE					195
#define UAX_OP_SETG					196
#define UAX_OP_SETGE				197
#define UAX_OP_SETL					198
#define UAX_OP_SETLE				199
#define UAX_OP_SETNA				200
#define UAX_OP_SETNAE				201
#define UAX_OP_SETNB				202
#define UAX_OP_SETNBE				203
#define UAX_OP_SETNC				204
#define UAX_OP_SETNE				205
#define UAX_OP_SETNG				206
#define UAX_OP_SETNGE				207
#define UAX_OP_SETNL				208
#define UAX_OP_SETNLE				209
#define UAX_OP_SETNO				210
#define UAX_OP_SETNP				211
#define UAX_OP_SETNS				212
#define UAX_OP_SETNZ				213
#define UAX_OP_SETO					214
#define UAX_OP_SETP					215
#define UAX_OP_SETPE				216
#define UAX_OP_SETPO				217
#define UAX_OP_SETS					218
#define UAX_OP_SETZ					219
#define UAX_OP_SFENCE				220
#define UAX_OP_SGDT					221
#define UAX_OP_SHL					222
#define UAX_OP_SHLD					223
#define UAX_OP_SHR					224
#define UAX_OP_SHRD					225
#define UAX_OP_SIDT					226
#define UAX_OP_SLDT					227
#define UAX_OP_SMSW					228
#define UAX_OP_STC					229
#define UAX_OP_STD					230
#define UAX_OP_STI					231
#define UAX_OP_STOSB				232
#define UAX_OP_STOSW				233
#define UAX_OP_STOSD				234
#define UAX_OP_STR					235
#define UAX_OP_SUB					236
#define UAX_OP_TEST					237
#define UAX_OP_UD2					238
#define UAX_OP_WAIT					239
#define UAX_OP_FWAIT				240
#define UAX_OP_WBINVD				241
#define UAX_OP_WRMSR				242
#define UAX_OP_XADD					243
#define UAX_OP_XCHG					244
#define UAX_OP_XOR					245

typedef struct UAX_Context_s UAX_Context;
typedef struct UAX_OpcodeArg_s UAX_OpcodeArg;

typedef struct UAX_OpcodeDesc_s UAX_OpcodeDesc;

struct UAX_Context_s {
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

UAX_Context *next;
};

/*
sc==0: register, immediate, or label
	breg!=Z: register
	breg==Z: immediate (disp)
sc!=0: memory reference
 */
struct UAX_OpcodeArg_s {
byte sc;		//scale for Reg/RM forms (1,2,3,4 for mem-ref)
byte ireg;		//index for Reg/RM forms
byte breg;		//base for Reg/RM forms, or register
s64 disp;		//displacement for Reg/RM forms, or immed
int lbl;		//label
};

struct UAX_OpcodeDesc_s {
int id;
char *opstr;
char *args;
int next;
};