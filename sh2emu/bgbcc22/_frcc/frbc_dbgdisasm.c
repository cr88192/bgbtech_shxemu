#include <bgbccc.h>

FILE *bgbcc_disasmlog=NULL;

struct dbgdisop_s {
char *name;
char *pat;
};

/* 
 * 0-9, A-F, literal hex values.
 * a: Address
 * b: Binary Operator
 * c: Const
 * d: Dest Reg
 
 * g: Global Index
 
 * i: Index (Array/Generic)
 * j: Compare Operator
 * k: Const/Literal (Depends On Operator)
 * l: Literal Index
 * m: Literal Index 2
 
 * s: Source Reg A
 * t: Source Reg B
 * u: Unary Operator
 
 * v: ConvV.Z
 * w: ConvI.Z
 * z: Conv.Z
 
 * W: W-Factor
 * X: X (Placeholder)
 * Y: Y-Factor
 * Z: Z-Factor (Type)
 */

struct dbgdisop_s bgbcc_frbc_ops[]=
{
{"BINOP.I",			"00bdst"},
{"BINOP.L",			"01bdst"},
{"BINOP.F",			"02bdst"},
{"BINOP.D",			"03bdst"},
{"BINOP.IC",		"04bdsccc"},
{"BINOP.LC",		"05bdsccc"},
{"BINOP.FC",		"06bdslll"},
{"BINOP.DC",		"07bdslll"},
{"UNOP.I",			"08uYds"},
{"UNOP.L",			"09uYds"},
{"UNOP.F",			"0AuYds"},
{"UNOP.D",			"0BuYds"},
{"UNOP.IC",			"0CuYdkkk"},
{"UNOP.LC",			"0DuYdkkk"},
{"UNOP.FC",			"0EuYdkkk"},
{"UNOP.DC",			"0FuYdkkk"},
{"CMP.I",			"10jdst"},
{"CMP.L",			"11jdst"},
{"CMP.F",			"12jdst"},
{"CMP.D",			"13jdst"},
{"CMP.IC",			"14jdskkk"},
{"CMP.LC",			"15jdskkk"},
{"CMP.FC",			"16jdskkk"},
{"CMP.DC",			"17jdskkk"},
{"JCMP.I",			"18jYstaaaa"},
{"JCMP.L",			"19jYstaaaa"},
{"JCMP.F",			"1AjYstaaaa"},
{"JCMP.D",			"1BjYstaaaa"},
{"JCMP.IC",			"1CjYskkkaaaa"},
{"JCMP.LC",			"1DjYskkkaaaa"},
{"JCMP.FC",			"1EjYskkkaaaa"},
{"JCMP.DC",			"1FjYskkkaaaa"},
{"JMP",				"20aaaa"},
{"JMPW",			"21aaaaaaaa"},
{"LDVAR",			"22Wdiiii"},
{"STVAR",			"23Wsiiii"},
{"LDGVAR",			"24Wdgggg"},
{"STGVAR",			"25Wsgggg"},
{"LDCONST",			"26Wdllll"},
{"LDCONSTW",		"27ZYddllllllll"},
{"CONV",			"28zYds"},
{"LDAGVAR",			"29Wdgggg"},
{"CONV.I",			"2AwYds"},
{"CONV.L",			"2BwYds"},
{"SHRMASK.I",		"2CYdsccc"},
{"SHRMASK.L",		"2DYdsccc"},
{"SHLMASK.I",		"2EYdsccc"},
{"SHLMASK.L",		"2FYdsccc"},
{"JCMP.RZI",		"30jsaaaa"},
{"JCMP.RZL",		"31jsaaaa"},
{"JCMP.RZF",		"32jsaaaa"},
{"JCMP.RZD",		"33jsaaaa"},
{"JCMP.AZI",		"34jsaaaa"},
{"JCMP.AZL",		"35jsaaaa"},
{"JCMP.AZF",		"36jsaaaa"},
{"JCMP.AZD",		"37jsaaaa"},
{"JCMP.LZI",		"38jsaaaa"},
{"JCMP.LZL",		"39jsaaaa"},
{"JCMP.LZF",		"3Ajsaaaa"},
{"JCMP.LZD",		"3Bjsaaaa"},
{"JCMP.RZP",		"3Cjsaaaa"},
{"JCMP.AZP",		"3Djsaaaa"},
{"JCMP.LZP",		"3Ejsaaaa"},
{"JCMP.P",			"3FjYstaaaa"},
{"LOADINDEX.I",		"40Ydst"},
{"LOADINDEX.L",		"41Ydst"},
{"LOADINDEX.F",		"42Ydst"},
{"LOADINDEX.D",		"43Ydst"},
{"LOADINDEX.SB",	"44Ydst"},
{"LOADINDEX.UB",	"45Ydst"},
{"LOADINDEX.SS",	"46Ydst"},
{"LOADINDEX.US",	"47Ydst"},
{"LOADINDEX.P",		"48Ydst"},
{"LOADINDEX.IC",	"49Ydsccc"},
{"LOADINDEX.LC",	"4AYdsccc"},
{"LOADINDEX.FC",	"4BYdsccc"},
{"LOADINDEX.DC",	"4CYdsccc"},
{"LOADINDEX.SBC",	"4DYdsccc"},
{"LOADINDEX.SSC",	"4EYdsccc"},
{"LOADINDEX.PC",	"4FYdsccc"},
{"STOREINDEX.I",	"50Ydst"},
{"STOREINDEX.L",	"51Ydst"},
{"STOREINDEX.F",	"52Ydst"},
{"STOREINDEX.D",	"53Ydst"},
{"STOREINDEX.B",	"54Ydst"},
{"STOREINDEX.S",	"55Ydst"},
{"STOREINDEX.P",	"56Ydst"},
{"LOADINDEX.SBC",	"57Ydsccc"},
{"LOADINDEX.SSC",	"58Ydsccc"},
{"STOREINDEX.IC",	"59Ydsccc"},
{"STOREINDEX.LC",	"5AYdsccc"},
{"STOREINDEX.FC",	"5BYdsccc"},
{"STOREINDEX.DC",	"5CYdsccc"},
{"STOREINDEX.BC",	"5DYdsccc"},
{"STOREINDEX.SC",	"5EYdsccc"},
{"STOREINDEX.PC",	"5FYdsccc"},
{"LEA.I",			"60Ydst"},
{"LEA.L",			"61Ydst"},
{"LEA.F",			"62Ydst"},
{"LEA.D",			"63Ydst"},
{"LEA.B",			"64Ydst"},
{"LEA.S",			"65Ydst"},
{"LEA.P",			"66Ydst"},
{"LEALVAR.V",		"67Ydst"},
{"LEA.IC",			"68Ydsccc"},
{"LEA.LC",			"69Ydsccc"},
{"LEA.FC",			"6AYdsccc"},
{"LEA.DC",			"6BYdsccc"},
{"LEA.BC",			"6CYdsccc"},
{"LEA.SC",			"6DYdsccc"},
{"LEA.PC",			"6EYdsccc"},
{"LEALVAR.VC",		"6FYdsccc"},
{"CALL",			"70ggggggNN"},
{"CALLP",			"71YsNN"},
{"RET.V",			"72"},
{"RET",				"73Ws"},
{"RETW",			"74ZYss"},
{"LOADSLOT",		"75ZYdsllllii"},
{"STORESLOT",		"76ZYstllllii"},
{"INITOBJ",			"77Wdllll"},
{"CSRV.V",			"78"},
{"CSRV",			"79Ws"},
{"CSRVW",			"7AZYss"},
{"INITARR",			"7BZdcccc"},
{"LABEL",			"7C"},
{"LABEL2",			"7Dllllll"},
{"NOP2",			"7EPP"},
{"NOP",				"7F"},
{"BINOP2",			"80ZYbdst"},
{"BINOP2.C",		"81ZYbdslll"},
{"UNOP2",			"82ZYuds0"},
{"UNOP2.C",			"83ZYudllll"},
{"CMP2",			"84ZYjdst"},
{"CMP2.C",			"85ZYjdslll"},
{"LDAVAR",			"86Ydss"},
{"COPYOBJ",			"87Ydslll"},
{"DROPOBJ",			"88Wsllll"},
{"SIZEOF.I",		"89Ydllll"},
{"OFFSETOF.I",		"8AYdllllii"},
{"RET.C",			"8BZkkk"},
{"INITOBJARR",		"8CWdllllcccc"},
{"CALL2",			"8DNggg"},
{"MOV",				"8EZYds"},
{"LEAJ",			"8FZYdstccc"},
{"ADDMUL",			"90ZYdstccc"},
{"ADDOP_IC",		"91ZYdstccc"},
{"BINOP3.C",		"92ZYbdsccc"},
{"UNOP3.C",			"93ZYudcccc"},
{"CALLIH",			"94ggggggNN"},
{"CALLIH2",			"95Nggg"},
{"CALLOBJ",			"96Nsgggg"},
{"CALLVIRT",		"97Nsllllii"},
{"CONV.V",			"98Zvds"},
{"CHKCONV.V",		"99Zvds"},
{"JCMP2",			"9AZYjst0aaaa"},
{"JCMP2.C",			"9BZYjskkkkaaaa"},
{"LDINITARR",		"9CZdccccllll"},
{"LDINITOBJ",		"9DWdllllmmmm"},
{"LDINITOBJARR",	"9EWdllllccccmmmm"},

{"BINOPW.I",		"E100bYddsstt"},
{"BINOPW.L",		"E101bYddsstt"},
{"BINOPW.F",		"E102bYddsstt"},
{"BINOPW.D",		"E103bYddsstt"},
{"BINOPW.IC",		"E104bYddssllllll"},
{"BINOPW.LC",		"E105bYddssllllll"},
{"BINOPW.FC",		"E106bYddssllllll"},
{"BINOPW.DC",		"E107bYddssllllll"},
{"UNOPW.I",			"E108bYddss"},
{"UNOPW.L",			"E109bYddss"},
{"UNOPW.F",			"E10AbYddss"},
{"UNOPW.D",			"E10BbYddss"},
{"UNOPW.IC",		"E10CbYddllllll"},
{"UNOPW.LC",		"E10DbYddllllll"},
{"UNOPW.FC",		"E10EbYddllllll"},
{"UNOPW.DC",		"E10FbYddllllll"},
{"CMPW.I",			"E110jYddsstt"},
{"CMPW.L",			"E111jYddsstt"},
{"CMPW.F",			"E112jYddsstt"},
{"CMPW.D",			"E113jYddsstt"},
{"CMPW.IC",			"E114jYddssllllll"},
{"CMPW.LC",			"E115jYddssllllll"},
{"CMPW.FC",			"E116jYddssllllll"},
{"CMPW.DC",			"E117jYddssllllll"},
{"JCMPW.I",			"E118jYssttaaaaaaaa"},
{"JCMPW.L",			"E119jYssttaaaaaaaa"},
{"JCMPW.F",			"E11AjYssttaaaaaaaa"},
{"JCMPW.D",			"E11BjYssttaaaaaaaa"},
{"JCMPW.IC",		"E11CjYssllllllaaaaaaaa"},
{"JCMPW.LC",		"E11DjYssllllllaaaaaaaa"},
{"JCMPW.FC",		"E11EjYssllllllaaaaaaaa"},
{"JCMPW.DC",		"E11FjYssllllllaaaaaaaa"},
{"SIZEOFW.I",		"E120ddllllll"},
{"OFFSETOFW.I",		"E121ddlllllliiii"},
{"LOADSLOTW.I",		"E122ZYddsslllllliiii"},
{"STORESLOTW.I",	"E123ZYssttlllllliiii"},
{"LOADSLOTAW.I",	"E124ZYddsslllllliiii"},
{"INITOBJW",		"E125ZYddllllll"},
{"COPYOBJW",		"E126Y0ddssllllll"},
{"DROPOBJW",		"E127Y0ssllllll"},
{"LOADINDEXW",		"E128ZYddsstt"},
{"STOREINDEXW",		"E129ZYddsstt"},
{"LOADINDEXAW",		"E12AZYddsstt"},
{"LEAW",			"E12BZYddsstt"},
{"LOADINDEXW.C",	"E12CZYddssiiiiiiii"},
{"STOREINDEXW.C",	"E12DZYddssiiiiiiii"},
{"LOADINDEXAW.C",	"E12EZYddssiiiiiiii"},
{"LEAW.C",			"E12FZYddssiiiiiiii"},

{"LOADSLOTOW.I",	"E130ZYddsslllllliiiimmmmmm"},
{"STORESLOTOW.I",	"E131ZYssttlllllliiiimmmmmm"},

{"LOADINDEXOW",		"E132ZYddssttllllll"},
{"STOREINDEXOW",	"E133ZYddssttllllll"},
{"LEAOW",			"E134ZYddssttllllll"},
{"LOADINDEXOW.C",	"E135ZYddssiiiiiiiillllll"},
{"STOREINDEXOW.C",	"E136ZYddssiiiiiiiillllll"},
{"LEAOW.C",			"E137ZYddssiiiiiiiillllll"},

{"RETOBJ",			"E138Ysllll"},
{"CSRVOBJ",			"E139Ydllll"},
{"RETOBJW",			"E13AZYssllllll"},
{"CSRVOBJW",		"E13BZYddllllll"},
{"RETW.C",			"E13CZukkkkkk"},
{"CONVW",			"E13DZYZ0ddss"},
{"LOADVSF",			"E13EZYiiddss"},
{"STOREVSF",		"E13FZYiiddss"},

{"LOADXVAR",		"E140ZYddiicc"},
{"STOREXVAR",		"E141ZYssiicc"},
{"CLOSEFCN",		"E142Wdgggg"},
{"DELCXFCN",		"E143Ws"},

{"LEAJW",			"E144ZYddssttiiii"},
{"ADDMULW",			"E145ZYddssttllllll"},
{"DIFFPTR",			"E146ZYddsstt"},
{"DIFFPTRL",		"E147ZYddsstt"},
{"INITARRW",		"E148ZYddcccccccc"},
{"DIFFPTROBJ",		"E149ZYddssttllllll"},

{"LDGVARW",			"E14AZYddgggggg"},
{"STGVARW",			"E14BZYssgggggg"},
{"LDAGVARW",		"E14CZYddgggggg"},
{"CALLPW",			"E14DZYssNN"},

{"LLN",				"E14Ecccc"},
{"LFN",				"E14Fllllll"},

{"CMPW.P",			"E150jYddsstt"},
{"CMPW.PC",			"E151jYddssllllll"},
{"JCMPW.P",			"E152jYssttaaaaaaaa"},
{"JCMPW.PC",		"E153jYssllllllaaaaaaaa"},

{NULL,				NULL}
};

int bgbcc_frbc_oplenstats[256];

int BGBCC_DbgDisAsm_Printf(
	char *str, ...)
{
	char tb[1024];
	va_list lst;
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);
	
//	fputs(tb, stdout);
	
	if(bgbcc_disasmlog)
		fputs(tb, bgbcc_disasmlog);
}

int BGBCC_FrBC_DbgDisAsm_CheckMatch(
	BGBCC_FrCC_State *ctx, byte *css, char *pat)
{
	byte *cs;
	char *s;
	
	cs=css; s=pat;
	while(*s)
	{
		if((s[0]>='0') && (s[0]<='9'))
		{
			if((((*cs)>>4)&15)!=(s[0]-'0'))
				break;
		}
		if((s[0]>='A') && (s[0]<='F'))
		{
			if((((*cs)>>4)&15)!=(s[0]-'A'+10))
				break;
		}

		if(!s[1])
			break;

		if((s[1]>='0') && (s[1]<='9'))
		{
			if(((*cs)&15)!=(s[1]-'0'))
				break;
		}
		if((s[1]>='A') && (s[1]<='F'))
		{
			if(((*cs)&15)!=(s[1]-'A'+10))
				break;
		}
		
		cs++; s+=2;
	}
	
	if(!(*s))
		return(1);

	return(0);
}

int BGBCC_FrBC_DbgDisAsm_LookupMatch(
	BGBCC_FrCC_State *ctx, byte *cs)
{
	int i;
	
	for(i=0; bgbcc_frbc_ops[i].name; i++)
	{
		if(BGBCC_FrBC_DbgDisAsm_CheckMatch(
				ctx, cs, bgbcc_frbc_ops[i].pat))
			{ return(i); }
	}
	return(-1);
}

int BGBCC_FrBC_DbgDisAsm_GetArgsOpLen(
	BGBCC_FrCC_State *ctx, byte *css, int na)
{
	byte *cs;
	int i;

	cs=css;
	for(i=0; i<na; i++)
	{
		if((*cs>=0x00) && (*cs<0xA0))
			{ cs++; continue; }
		if((*cs>=0xA0) && (*cs<0xB0))
			{ cs+=2; continue; }
		if((*cs>=0xD0) && (*cs<=0xFF))
			{ cs++; continue; }

		switch(*cs)
		{
		case 0xB0: case 0xB1:
			cs+=2; break;
		case 0xB2: case 0xB3:
			cs+=3; break;
		case 0xB4: case 0xB5:
		case 0xB6: case 0xB7:
		case 0xB8: case 0xB9:
		case 0xBA: case 0xBB:
		case 0xBC: case 0xBD:
		case 0xBE:
			cs+=4; break;
		
		case 0xBF:
			cs+=3; break;

		case 0xCF:
			cs+=2; break;
		default:
			break;
		}
	}
	return(cs-css);
}

int BGBCC_FrBC_DbgDisAsm_GetPatOpLen(
	BGBCC_FrCC_State *ctx, byte *css, char *pat)
{
	byte *cs;
	char *s;
	int i, j, na;
	
	cs=css; s=pat; na=-1;
	while(*s)
	{
		if((s[0]=='P') && (s[1]=='P'))
		{
			cs=css+(*cs);
			s+=2;
			continue;
		}

		if((s[0]=='N') && (s[1]=='N'))
		{
			i=*cs++; s+=2;
			na=i;
//			j=BGBCC_FrBC_DbgDisAsm_GetArgsOpLen(ctx, cs, i);
//			cs+=j;
			continue;
		}

		if(s[0]=='N')
			{ na=(cs[0]>>4)&15; }

		cs++; s+=2;
	}
	
	if(na>=0)
	{
		j=BGBCC_FrBC_DbgDisAsm_GetArgsOpLen(ctx, cs, na);
		cs+=j;
	}
	
	if(*s)
		return(-1);
	
	return(cs-css);
}

int BGBCC_FrBC_DbgDisAsm_DecodeOpArgsList(
	BGBCC_FrCC_State *ctx, byte *css, byte **rcs, int na)
{
	static const char *ilpfd="ILFDPA";
	byte *cs;
	int i, j, k;

	if(na<=0)
		return(0);

	BGBCC_DbgDisAsm_Printf("\n\t[ ");

	cs=*rcs;
	for(i=0; i<na; i++)
	{
		if((*cs>=0x00) && (*cs<0x10))
			{ BGBCC_DbgDisAsm_Printf("R%dI ", (*cs++)&15); continue; }
		if((*cs>=0x10) && (*cs<0x20))
			{ BGBCC_DbgDisAsm_Printf("R%dL ", (*cs++)&15); continue; }
		if((*cs>=0x20) && (*cs<0x30))
			{ BGBCC_DbgDisAsm_Printf("R%dF ", (*cs++)&15); continue; }
		if((*cs>=0x30) && (*cs<0x40))
			{ BGBCC_DbgDisAsm_Printf("R%dD ", (*cs++)&15); continue; }
		if((*cs>=0x40) && (*cs<0x50))
			{ BGBCC_DbgDisAsm_Printf("R%dP ", (*cs++)&15); continue; }

		if((*cs>=0x50) && (*cs<0x60))
			{ BGBCC_DbgDisAsm_Printf("L%dI ", (*cs++)&15); continue; }
		if((*cs>=0x60) && (*cs<0x70))
			{ BGBCC_DbgDisAsm_Printf("L%dL ", (*cs++)&15); continue; }
		if((*cs>=0x70) && (*cs<0x80))
			{ BGBCC_DbgDisAsm_Printf("L%dF ", (*cs++)&15); continue; }
		if((*cs>=0x80) && (*cs<0x90))
			{ BGBCC_DbgDisAsm_Printf("L%dD ", (*cs++)&15); continue; }
		if((*cs>=0x90) && (*cs<0xA0))
			{ BGBCC_DbgDisAsm_Printf("L%dP ", (*cs++)&15); continue; }

		if((*cs>=0xD0) && (*cs<0xE0))
			{ BGBCC_DbgDisAsm_Printf("A%dI ", (*cs++)&15); continue; }
		if((*cs>=0xE0) && (*cs<0xF0))
			{ BGBCC_DbgDisAsm_Printf("A%dD ", (*cs++)&15); continue; }
		if((*cs>=0xF0) && (*cs<=0xFF))
			{ BGBCC_DbgDisAsm_Printf("A%dP ", (*cs++)&15); continue; }
			
		switch(*cs)
		{
		case 0xA0: BGBCC_DbgDisAsm_Printf("R%dI ", cs[1]); cs+=2; break;
		case 0xA1: BGBCC_DbgDisAsm_Printf("R%dL ", cs[1]); cs+=2; break;
		case 0xA2: BGBCC_DbgDisAsm_Printf("R%dF ", cs[1]); cs+=2; break;
		case 0xA3: BGBCC_DbgDisAsm_Printf("R%dD ", cs[1]); cs+=2; break;
		case 0xA4: BGBCC_DbgDisAsm_Printf("R%dP ", cs[1]); cs+=2; break;

		case 0xA5: BGBCC_DbgDisAsm_Printf("A%dI ", cs[1]); cs+=2; break;
		case 0xA6: BGBCC_DbgDisAsm_Printf("A%dL ", cs[1]); cs+=2; break;
		case 0xA7: BGBCC_DbgDisAsm_Printf("A%dF ", cs[1]); cs+=2; break;
		case 0xA8: BGBCC_DbgDisAsm_Printf("A%dD ", cs[1]); cs+=2; break;
		case 0xA9: BGBCC_DbgDisAsm_Printf("A%dP ", cs[1]); cs+=2; break;

		case 0xAA: BGBCC_DbgDisAsm_Printf("L%dI ", cs[1]); cs+=2; break;
		case 0xAB: BGBCC_DbgDisAsm_Printf("L%dL ", cs[1]); cs+=2; break;
		case 0xAC: BGBCC_DbgDisAsm_Printf("L%dF ", cs[1]); cs+=2; break;
		case 0xAD: BGBCC_DbgDisAsm_Printf("L%dD ", cs[1]); cs+=2; break;
		case 0xAE: BGBCC_DbgDisAsm_Printf("L%dP ", cs[1]); cs+=2; break;

		case 0xAF: BGBCC_DbgDisAsm_Printf("&L%d ", cs[1]); cs+=2; break;
		case 0xCF: BGBCC_DbgDisAsm_Printf("&A%d ", cs[1]); cs+=2; break;
		
		case 0xB0: case 0xB1:
			BGBCC_DbgDisAsm_Printf("C:%d ", (signed char)(cs[1]));
			cs+=2; break;
		case 0xB2: case 0xB3:
			BGBCC_DbgDisAsm_Printf("C:%d ", (signed short)((cs[1]<<8)|(cs[2])));
			cs+=3; break;

		case 0xB4: case 0xB5:
		case 0xB6: case 0xB7:
		case 0xB8:
			BGBCC_DbgDisAsm_Printf("L:%d%c ",
				(cs[1]<<16)|(cs[2]<<8)|(cs[3]),
				ilpfd[(*cs)-0xB4]);
			cs+=4; break;
		
		case 0xB9: case 0xBA: case 0xBB:
		case 0xBC: case 0xBD: case 0xBE:
			BGBCC_DbgDisAsm_Printf("G:%d%c ",
				(cs[1]<<16)|(cs[2]<<8)|(cs[3]),
				ilpfd[(*cs)-0xB9]);
			cs+=4; break;

		case 0xBF:
			j=(cs[1]<<8)|(cs[2]); cs+=3;
			k=(j>>12)&15; j=j&4095;
			switch(k)
			{
			case 4: case 5: case 6: case 7: case 8:
				BGBCC_DbgDisAsm_Printf("L:%d%c ", j, ilpfd[k-4]); break;
			case 9: case 10: case 11:
			case 12: case 13: case 14:
				BGBCC_DbgDisAsm_Printf("G:%d%c ", j, ilpfd[k-9]); break;
			default:
				BGBCC_DbgDisAsm_Printf("T%d:%d ", k, j);
				break;
			}
			break;
 
		default:
			BGBCC_DbgDisAsm_Printf("? ");
			break;
		}
	}

	BGBCC_DbgDisAsm_Printf("] ");
	
	*rcs=cs;
	return(0);
}

int BGBCC_FrBC_DbgDisAsm_DecodeOp(
	BGBCC_FrCC_State *ctx, byte *css, byte **rcs)
{
	static char *binop[16]={
		  "ADD",  "SUB",  "MUL",  "DIV",
		  "MOD",  "AND",   "OR",  "XOR",
		  "SHL",  "SHR", "SHRR", "MULH",
		"UMULH", "UDIV",    "-",    "-"};
	static char *unop[16]={
		"MOV1", "NEG1", "NOT1", "LNOT1",
		"INC1", "DEC1",   "-1",    "-1",
		"MOV2", "NEG2", "NOT2", "LNOT2",
		"INC2", "DEC2",   "-2",    "-2"};
	static char *jmpop[16]={
		"EQ1", "NE1", "LT1", "GT1",
		"LE1", "GE1", "AL1", "NV1",
		"EQ2", "NE2", "LT2", "GT2",
		"LE2", "GE2", "AL2", "NV2"};
	static char *yop[16]={
		"rRR", "rRA", "rRL", "rAR",
		"rAA", "rAL", "rLR", "rLA",
		"rLL", "ARR", "LRR", "LRA",
		"LAR", "LRL", "LLR", "LLL"};
	static char *zop[16]={
		"I", "L", "F", "D",
		"P", "V", "S", "NL",
		"SB", "UB", "SS", "US",
		"UI", "UL", "UNL", "T"};
	static char *wop[16]={
		"IR", "IA", "IL", "LR",
		"LA", "LL", "FR", "FA",
		"FL", "DR", "DA", "DL",
		"PR", "PA", "PL", "T"};

	static char *conviz[16]={
		 "SB",  "ZB",  "SS",  "ZS",
		 "SI",  "ZI", "SB2", "UB2",
		"SS2", "US2", "SB3", "UB3",
		"SS3", "US3", "SI2", "ZI2"};
	static char *convz[16]={
		"I2L", "I2F", "I2D", "I2P",
		"L2I", "L2F", "L2D", "L2P",
		"F2I", "F2L", "F2D", "D2I",
		"D2L", "D2F", "P2I", "P2L"};

	byte *cs;
	char *opn, *pat, *s;
	int idx, len, z, y, w, nr, na;
	int i, j, k;
	
	cs=*rcs;
	idx=BGBCC_FrBC_DbgDisAsm_LookupMatch(ctx, cs);
	if(idx<0)return(-1);
	
	opn=bgbcc_frbc_ops[idx].name;
	pat=bgbcc_frbc_ops[idx].pat;
	len=BGBCC_FrBC_DbgDisAsm_GetPatOpLen(ctx, cs, pat);
	if(len<=0)return(-2);
	
	bgbcc_frbc_oplenstats[cs[0]]+=len;
	
	BGBCC_DbgDisAsm_Printf("%04X: ", cs-css);
	for(i=0; i<len; i++)
		{ BGBCC_DbgDisAsm_Printf("%02X", cs[i]); }
	for(; i<12; i++)
		{ BGBCC_DbgDisAsm_Printf("  "); }
	BGBCC_DbgDisAsm_Printf(" %s ", opn);

	s=pat; na=-1;
	while(*s)
	{
		if(((s[0]>='0') && (s[0]<='9')) ||
			((s[0]>='A') && (s[0]<='F')))
		{
			if(((s[1]>='0') && (s[1]<='9')) ||
				((s[1]>='A') && (s[1]<='F')))
			{
				cs++; s+=2;
				continue;
			}
		}

		if((s[0]=='N') && (s[1]=='N'))
		{
			i=*cs++; s+=2;
			na=i;
//			BGBCC_FrBC_DbgDisAsm_DecodeOpArgsList(ctx, css, &cs, i);
			continue;
		}
		
		if((s[0]=='d') && (s[1]=='d'))
			{ BGBCC_DbgDisAsm_Printf("RdW:%d ", *cs++); s+=2; continue; }
		if((s[0]=='s') && (s[1]=='s'))
			{ BGBCC_DbgDisAsm_Printf("RsW:%d ", *cs++); s+=2; continue; }
		if((s[0]=='t') && (s[1]=='t'))
			{ BGBCC_DbgDisAsm_Printf("RtW:%d ", *cs++); s+=2; continue; }

		if(s[0]=='Y') { y=((*cs)>>4)&15; BGBCC_DbgDisAsm_Printf("Y:%d:%s ", y, yop[y]); }
		if(s[0]=='Z') { z=((*cs)>>4)&15; BGBCC_DbgDisAsm_Printf("Z:%d:%s ", z, zop[z]); }
		if(s[0]=='W') { w=((*cs)>>4)&15; BGBCC_DbgDisAsm_Printf("W:%d:%s ", w, wop[w]); }

		if(s[0]=='b')
			{ BGBCC_DbgDisAsm_Printf("Ob:%s ", binop[((*cs)>>4)&15]); }
		if(s[0]=='u')
			{ BGBCC_DbgDisAsm_Printf("Ou:%s ", unop[((*cs)>>4)&15]); }
		if(s[0]=='j')
			{ BGBCC_DbgDisAsm_Printf("Oj:%s ", jmpop[((*cs)>>4)&15]); }

//		if((s[0]=='d') || (s[0]=='s') || (s[0]=='t'))
//			{ BGBCC_DbgDisAsm_Printf("R:%d ", ((*cs)>>4)&15); }
		if(s[0]=='d')
			{ BGBCC_DbgDisAsm_Printf("Rd:%d ", ((*cs)>>4)&15); }
		if(s[0]=='s')
			{ BGBCC_DbgDisAsm_Printf("Rs:%d ", ((*cs)>>4)&15); }
		if(s[0]=='t')
			{ BGBCC_DbgDisAsm_Printf("Rt:%d ", ((*cs)>>4)&15); }

		if(s[0]=='w')
		{
			i=((*cs)>>4)&15;
			BGBCC_DbgDisAsm_Printf("Cw:%d:%s ", i, conviz[i]);
		}

		if(s[0]=='z')
		{
			i=((*cs)>>4)&15;
			BGBCC_DbgDisAsm_Printf("Cz:%d:%s ", i, convz[i]);
		}

		if(s[0]=='N')
			{ na=((*cs)>>4)&15; }

		if(s[0]=='a')
		{
			i=((*cs)&128)?-1:0;
			while(*s && (*s=='a'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("A:%04X ", (cs-css)+i);
			continue;
		}

		if(s[0]=='c')
		{
			i=((*cs)&128)?-1:0;
			while(*s && (*s=='c'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("C:%d", i);
			continue;
		}

		if(s[0]=='l')
		{
			i=0;
			while(*s && (*s=='l'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("L:%d ", i);
			continue;
		}

		if(s[0]=='i')
		{
			i=((*cs)&128)?-1:0;
			while(*s && (*s=='i'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("I:%d ", i);
			continue;
		}

		if(s[0]=='k')
		{
			i=((*cs)&128)?-1:0;
			while(*s && (*s=='k'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("K:%d ", i);
			continue;
		}

		if(s[0]=='g')
		{
//			i=((*cs)&128)?-1:0;
			i=0;
			while(*s && (*s=='g'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("G:%d ", i);
			continue;
		}

		if(s[1]=='Y') { y=(*cs)&15; BGBCC_DbgDisAsm_Printf("Y:%d:%s ", y, yop[y]); }
		if(s[1]=='Z') { z=(*cs)&15; BGBCC_DbgDisAsm_Printf("Z:%d:%s ", z, zop[z]); }
		if(s[1]=='W') { w=(*cs)&15; BGBCC_DbgDisAsm_Printf("W:%d:%s ", w, wop[w]); }

		if(s[1]=='b')
			{ BGBCC_DbgDisAsm_Printf("Ob:%s ", binop[(*cs)&15]); }
		if(s[1]=='u')
			{ BGBCC_DbgDisAsm_Printf("Ou:%s ", unop[(*cs)&15]); }
		if(s[1]=='j')
			{ BGBCC_DbgDisAsm_Printf("Oj:%s ", jmpop[(*cs)&15]); }

//		if((s[1]=='d') || (s[1]=='s') || (s[1]=='t'))
//			{ BGBCC_DbgDisAsm_Printf("R:%d ", (*cs)&15); }

		if(s[1]=='d')
			{ BGBCC_DbgDisAsm_Printf("Rd:%d ", (*cs)&15); }
		if(s[1]=='s')
			{ BGBCC_DbgDisAsm_Printf("Rs:%d ", (*cs)&15); }
		if(s[1]=='t')
			{ BGBCC_DbgDisAsm_Printf("Rt:%d ", (*cs)&15); }

		if(s[1]=='c')
		{
			i=(*cs++)&15; s+=2;
			i=(i<<28)>>28;
			while(*s && (*s=='c'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("C:%d ", i);
			continue;
		}

		if(s[1]=='l')
		{
			i=(*cs++)&15; s+=2;
//			i=(i<<28)>>28;
			while(*s && (*s=='l'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("L:%d ", i);
			continue;
		}

		if(s[1]=='i')
		{
			i=(*cs++)&15; s+=2;
			i=(i<<28)>>28;
			while(*s && (*s=='i'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("I:%d ", i);
			continue;
		}

		if(s[1]=='k')
		{
			i=(*cs++)&15; s+=2;
			i=(i<<28)>>28;
			while(*s && (*s=='k'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("K:%d ", i);
			continue;
		}

		if(s[1]=='g')
		{
			i=(*cs++)&15; s+=2;
//			i=(i<<28)>>28;
			while(*s && (*s=='g'))
				{ i=(i<<8)|(*cs++); s+=2; }
			BGBCC_DbgDisAsm_Printf("G:%d ", i);
			continue;
		}

		cs++; s+=2;
	}

	if(na>=0)
	{
		BGBCC_FrBC_DbgDisAsm_DecodeOpArgsList(ctx, css, &cs, na);
	}
	
	BGBCC_DbgDisAsm_Printf("\n");
	
//	*rcs=cs;
	*rcs=(*rcs)+len;
	return(0);
}

void BGBCC_FrBC_DbgDisAsmFunction(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_LiteralInfo *obj)
{
	int sortop[256];
	int sortlen[256];
	byte *cs, *css, *cse;
	int i, j, k, t;

	if(!bgbcc_disasmlog)
	{
		bgbcc_disasmlog=fopen("dump/frbc_dump.txt", "wt");
	}

	css=obj->decl->text;
	cse=css+obj->decl->sz_text;
	cs=css;
	
	if(cs>=cse)
		return;

	BGBCC_DbgDisAsm_Printf("DbgDisAsm %s%s\n", obj->decl->name, obj->decl->sig);

	while(cs<cse)
	{
		i=BGBCC_FrBC_DbgDisAsm_DecodeOp(ctx, css, &cs);
		if(i<0)
		{
			BGBCC_DbgDisAsm_Printf("Decode Break/Error %d\n", i);
			break;
		}
	}

#if 1
	BGBCC_DbgDisAsm_Printf("OpStats:\n");
	t=0;
	for(i=0; i<256; i++)
	{
		sortop[i]=i;
		sortlen[i]=bgbcc_frbc_oplenstats[i];
		t+=sortlen[i];
	}
	
	for(i=0; i<10; i++)
	{
		for(j=i+1; j<256; j++)
			if(sortlen[j]>sortlen[i])
		{
			k=sortop[i]; sortop[i]=sortop[j]; sortop[j]=k;
			k=sortlen[i]; sortlen[i]=sortlen[j]; sortlen[j]=k;
		}
		
		BGBCC_DbgDisAsm_Printf("%02X:%d ", sortop[i], (100*sortlen[i])/t);
	}
	BGBCC_DbgDisAsm_Printf("\n");
#endif

	BGBCC_DbgDisAsm_Printf("\n");
	if(bgbcc_disasmlog)
		fflush(bgbcc_disasmlog);

//	fgetc(stdin);
//	*(int *)-1=-1;
}
