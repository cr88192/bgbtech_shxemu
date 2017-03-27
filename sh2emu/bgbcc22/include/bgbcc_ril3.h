/*
 * RIL3: Naive Stack Bytecode
 * Intended mostly for static libraries.
 * Files are largely unstructured (Begin/End and Bytecode)
 *
 * Strings:
 * Start with a SVLI:
 *   0=NULL
 *   positive: Length of a raw string.
 *   (-1)..(-64): Repeat of recent string.
 *   (-65)..(...): LZ compressed string.
 * 
 * LZ String:
 *   SVLI csize;	//-(csize+64), compressed size
 *   SVLI usize;	//positive: uncompressed size
 *   tag-matches...
 *
 * tag-match: rrrr-llll
 *     rrrr=Raw Length, 0=none; 1-14=1-14 bytes, 15=longer
 *     llll=Match Length, 0=none; 1-14=3-16 bytes, 15=longer
 *   if(rrrr==15)
 *     UVLI rawlen;
 *   raw-bytes
 *   if(llll!=0)
 *     if(llll==15)UVLI matchlen;
 *     UVLI matchdist;
 */
 
#define BGBCC_RIL3OP_END		0x00
#define BGBCC_RIL3OP_ARGINT		0x01
#define BGBCC_RIL3OP_ARGSTR		0x02
#define BGBCC_RIL3OP_ASMBLOB	0x03
#define BGBCC_RIL3OP_BEGIN		0x04
#define BGBCC_RIL3OP_BEGINNAME	0x05
#define BGBCC_RIL3OP_MARKER		0x06

#define BGBCC_RIL3OP_ATTRINT	0x08
#define BGBCC_RIL3OP_ATTRLONG	0x09
#define BGBCC_RIL3OP_ATTRFLOAT	0x0A
#define BGBCC_RIL3OP_ATTRDOUBLE	0x0B
#define BGBCC_RIL3OP_ATTRSTR	0x0C

#define BGBCC_RIL3OP_LITINT		0x10
#define BGBCC_RIL3OP_LITLONG	0x11
#define BGBCC_RIL3OP_LITFLOAT	0x12
#define BGBCC_RIL3OP_LITDOUBLE	0x13
#define BGBCC_RIL3OP_LITSTR		0x14
#define BGBCC_RIL3OP_LITWSTR	0x15
#define BGBCC_RIL3OP_LITNAME	0x16

#define BGBCC_RIL3OP_MARK		0x20
#define BGBCC_RIL3OP_LOAD		0x21
#define BGBCC_RIL3OP_STORE		0x22
#define BGBCC_RIL3OP_CALLN		0x23
#define BGBCC_RIL3OP_CALLP		0x24
#define BGBCC_RIL3OP_LDIXC		0x25
#define BGBCC_RIL3OP_STIXC		0x26
#define BGBCC_RIL3OP_LDIXAC		0x27
#define BGBCC_RIL3OP_LDIX		0x28
#define BGBCC_RIL3OP_STIX		0x29
#define BGBCC_RIL3OP_LDIXA		0x2A
#define BGBCC_RIL3OP_DUP		0x2B
#define BGBCC_RIL3OP_DUPCLEAN	0x2C
#define BGBCC_RIL3OP_RETV		0x2D
#define BGBCC_RIL3OP_RET		0x2E
#define BGBCC_RIL3OP_CASTBOOL	0x2F
#define BGBCC_RIL3OP_CASTSIG	0x30
#define BGBCC_RIL3OP_BINOP		0x31
#define BGBCC_RIL3OP_CMPOP		0x32
#define BGBCC_RIL3OP_UNOP		0x33
#define BGBCC_RIL3OP_STBINOP	0x34
#define BGBCC_RIL3OP_STCMPOP	0x35
#define BGBCC_RIL3OP_LDUNOP		0x36
#define BGBCC_RIL3OP_LDCONSTI	0x37
#define BGBCC_RIL3OP_LDCONSTL	0x38
#define BGBCC_RIL3OP_LDCONSTF	0x39
#define BGBCC_RIL3OP_LDCONSTD	0x3A
#define BGBCC_RIL3OP_LDCONSTS	0x3B
#define BGBCC_RIL3OP_LDCONSTWS	0x3C

#define BGBCC_RIL3OP_POP		0x40
#define BGBCC_RIL3OP_LDA		0x41
#define BGBCC_RIL3OP_SIZEOFSG	0x42
#define BGBCC_RIL3OP_SIZEOFVAL	0x43
#define BGBCC_RIL3OP_OFFSETOF	0x44
#define BGBCC_RIL3OP_LOADSLOT	0x45
#define BGBCC_RIL3OP_STORESLOT	0x46
#define BGBCC_RIL3OP_LOADSLOTA	0x47
#define BGBCC_RIL3OP_BEGINU		0x48
#define BGBCC_RIL3OP_ENDU		0x4A
#define BGBCC_RIL3OP_SETU		0x4B
#define BGBCC_RIL3OP_INITVAR	0x4C
#define BGBCC_RIL3OP_INITVARVAL	0x4D
#define BGBCC_RIL3OP_JMP		0x4E
#define BGBCC_RIL3OP_JCMP		0x4F
#define BGBCC_RIL3OP_LABEL		0x50
#define BGBCC_RIL3OP_JMPT		0x51
#define BGBCC_RIL3OP_JMPF		0x52
#define BGBCC_RIL3OP_STKFN		0x53
#define BGBCC_RIL3OP_STKLN		0x54