#include <bgbccc.h>

int BGBCC_SHX_SetSectionName(BGBCC_SHX_Context *ctx, char *name)
{
	int sec;
	
	sec=-1;
	if(!strcmp(name, ".text"))	sec=BGBCC_SH_CSEG_TEXT;
	if(!strcmp(name, ".data"))	sec=BGBCC_SH_CSEG_DATA;
	if(!strcmp(name, ".bss"))	sec=BGBCC_SH_CSEG_BSS;
	
	if(sec>=0)
	{
		BGBCC_SHX_SetSection(ctx, sec);
		if(!ctx->sec_name[sec])
			ctx->sec_name[sec]=bgbcc_strdup(name);
		return(0);
	}

	for(sec=BGBCC_SHX_CSEG_DYN; sec<ctx->nsec; sec++)
	{
		if(!strcmp(name, ctx->sec_name[sec]))	
		{
			BGBCC_SHX_SetSection(ctx, sec);
			return(0);
		}
	}

	sec=ctx->nsec++;
	if(sec<BGBCC_SHX_CSEG_DYN)sec=BGBCC_SHX_CSEG_DYN;
	ctx->sec_name[sec]=bgbcc_strdup(name);	
	ctx->sec=sec;
	if((sec+1)>ctx->nsec)
		ctx->nsec=sec+1;
	return(0);
}

int BGBCC_SHX_SetSection(BGBCC_SHX_Context *ctx, int sec)
{
	ctx->sec=sec;
	if((sec+1)>ctx->nsec)
		ctx->nsec=sec+1;
	return(0);
}

int BGBCC_SHX_EmitByte(BGBCC_SHX_Context *ctx, int val)
{
	byte *buf;
	int sz, ofs;

	if(!ctx->sec_buf[ctx->sec])
	{
		sz=4096;	buf=malloc(sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf;
	}
	
	if((ctx->sec_pos[ctx->sec]+1)>=ctx->sec_end[ctx->sec])
	{
		buf=ctx->sec_buf[ctx->sec];
		sz=ctx->sec_end[ctx->sec]-buf;
		ofs=ctx->sec_pos[ctx->sec]-buf;
		sz=sz+(sz>>1);
		buf=realloc(buf, sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf+ofs;
	}

	*ctx->sec_pos[ctx->sec]++=val;
	return(0);
}

int BGBCC_SHX_EmitWord(BGBCC_SHX_Context *ctx, int val)
{
	if(ctx->is_le)
	{
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val   );
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>>8);
		return(0);
	}else
	{
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>>8);
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val   );
		return(0);
	}
}

int BGBCC_SHX_EmitDWord(BGBCC_SHX_Context *ctx, u32 val)
{
	if(ctx->is_le)
	{
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val    );
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>> 8);
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>>16);
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>>24);
		return(0);
	}else
	{
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>>24);
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>>16);
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val>> 8);
		BGBCC_SHX_SecEmitByte(ctx->seg[ctx->cseg], val    );
		return(0);
	}
}

int BGBCC_SHX_EmitQWord(BGBCC_SHX_Context *ctx, s64 val)
{
	if(ctx->is_le)
	{
		BGBCC_SHX_EmitByte(ctx, (val    )&255);
		BGBCC_SHX_EmitByte(ctx, (val>> 8)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>16)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>24)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>32)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>40)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>48)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>56)&255);
		return(0);
	}else
	{
		BGBCC_SHX_EmitByte(ctx, (val>>56)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>48)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>40)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>32)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>24)&255);
		BGBCC_SHX_EmitByte(ctx, (val>>16)&255);
		BGBCC_SHX_EmitByte(ctx, (val>> 8)&255);
		BGBCC_SHX_EmitByte(ctx, (val    )&255);
		return(0);
	}
}

int BGBCC_SHX_EmitString(BGBCC_SHX_Context *ctx, char *str)
{
	byte *s;
	
	s=str;
	while(*s)
		BGBCC_SHX_EmitByte(ctx, *s++);
	BGBCC_SHX_EmitByte(ctx, *s++);
	return(0);
}

int BGBCC_SHX_EmitAscii(BGBCC_SHX_Context *ctx, char *str)
{
	byte *s;
	
	s=str;
	while(*s)
		BGBCC_SHX_EmitByte(ctx, *s++);
	return(0);
}

int BGBCC_SHX_EmitRawBytes(BGBCC_SHX_Context *ctx, byte *buf, int sz)
{
	byte *s;
	int n;
	
	if(buf)
	{
		s=buf; n=sz;
		while(n--)
			BGBCC_SHX_EmitByte(ctx, *s++);
	}else
	{
		n=sz;
		while(n--)
			BGBCC_SHX_EmitByte(ctx, 0);
	}
	return(0);
}

byte *BGBCC_SHX_EmitGetPos(BGBCC_SHX_Context *ctx)
{
	return(ctx->sec_pos[ctx->sec]);
}

int BGBCC_SHX_EmitGetOffs(BGBCC_SHX_Context *ctx)
{
	return(ctx->sec_pos[ctx->sec]-ctx->sec_buf[ctx->sec]);
}

int BGBCC_SHX_EmitBAlign(BGBCC_SHX_Context *ctx, int al)
{
	int i, j, k;
	
	i=BGBCC_SHX_EmitGetOffs(ctx);
	if(!(i&(al-1)))
		return(0);
	j=al-(i&(al-1));

	if(ctx->sec==BGBCC_SH_CSEG_TEXT)
	{
		if(j&1)
			{ BGBCC_SHX_EmitByte(ctx, 0); j--; }
		while(j>0)
			{ BGBCC_SHX_EmitWord(ctx, 0x003B); j-=2; }
	}else
	{
		while(j--)
			BGBCC_SHX_EmitByte(ctx, 0);
	}

//	while(j--)
//		BGBCC_SHX_EmitByte(ctx, 0);
	return(1);
}

byte *BGBCC_SHX_EmitGetPosForOffs(BGBCC_SHX_Context *ctx, int ofs)
{
	return(ctx->sec_buf[ctx->sec]+ofs);
}

int BGBCC_SHX_EmitGetOffsWord(BGBCC_SHX_Context *ctx, int ofs)
{
	byte *ptr;
	ptr=BGBCC_SHX_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		return(ptr[0]|(ptr[1]<<8));
	}else
	{
		return((ptr[0]<<8)|ptr[1]);
	}
}

int BGBCC_SHX_EmitSetOffsWord(BGBCC_SHX_Context *ctx, int ofs, int val)
{
	byte *ptr;
	ptr=BGBCC_SHX_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		ptr[0]=val   ;
		ptr[1]=val>>8;
		return(0);
	}else
	{
		ptr[0]=val>>8;
		ptr[1]=val   ;
		return(0);
	}
}

int BGBCC_SHX_GenLabelTemp(BGBCC_SHX_Context *ctx)
{
	int i;
	i=ctx->lblrov++;
	return(i);
}

int BGBCC_SHX_EmitLabel(BGBCC_SHX_Context *ctx, int lblid)
{
	int i;
	
	if(lblid>0)
	{
		i=ctx->nlbl++;
		ctx->lbl_id[i]=lblid;
		ctx->lbl_ofs[i]=BGBCC_SHX_EmitGetOffs(ctx);
		ctx->lbl_sec[i]=ctx->sec;
//		ctx->lbl_ty[i]=ty;
	}
	return(0);
}


int BGBCC_SHX_EmitRelocTy(BGBCC_SHX_Context *ctx, int lblid, int ty)
{
	int i;
	
	if(lblid>0)
	{
		i=ctx->nrlc++;
		ctx->rlc_id[i]=lblid;
		ctx->rlc_ofs[i]=BGBCC_SHX_EmitGetOffs(ctx);
		ctx->rlc_sec[i]=ctx->sec;
		ctx->rlc_ty[i]=ty;
	}
	return(0);
}

int BGBCC_SHX_EmitRelocRel8(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_REL8)); }
int BGBCC_SHX_EmitRelocRel16(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_REL16)); }
int BGBCC_SHX_EmitRelocRel32(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_REL32)); }

int BGBCC_SHX_EmitRelocAbs8(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_ABS8)); }
int BGBCC_SHX_EmitRelocAbs16(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_ABS16)); }
int BGBCC_SHX_EmitRelocAbs32(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_ABS32)); }
int BGBCC_SHX_EmitRelocAbs64(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_ABS64)); }

int BGBCC_SHX_EmitRelocRelW12(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SHX_RLC_RELW12)); }

int BGBCC_SHX_EmitOpNone(BGBCC_SHX_Context *ctx, int nmid)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_CLRT:	opw=0x0008; break;
	case BGBCC_SH_NMID_SETT:	opw=0x0018; break;
	case BGBCC_SH_NMID_CLRMAC:	opw=0x0028; break;
	case BGBCC_SH_NMID_LDTLB:	opw=0x0038; break;
	case BGBCC_SH_NMID_CLRS:	opw=0x0048; break;
	case BGBCC_SH_NMID_SETS:	opw=0x0058; break;
	case BGBCC_SH_NMID_NOTT:	opw=0x0068; break;

	case BGBCC_SH_NMID_NOP:		opw=0x0009; break;
	case BGBCC_SH_NMID_DIV0U:	opw=0x0019; break;

	case BGBCC_SH_NMID_RTS:		opw=0x000B; break;
	case BGBCC_SH_NMID_SLEEP:	opw=0x001B; break;
	case BGBCC_SH_NMID_RTE:		opw=0x002B; break;
	case BGBCC_SH_NMID_BRK:		opw=0x003B; break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BSRF:	opw=0x0003|((reg&15)<<8); break;
	case BGBCC_SH_NMID_BRAF:	opw=0x0023|((reg&15)<<8); break;

	case BGBCC_SH_NMID_MOVT:	opw=0x0029|((reg&15)<<8); break;
	case BGBCC_SH_NMID_MOVRT:	opw=0x0039|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLL:	opw=0x4000|((reg&15)<<8); break;
	case BGBCC_SH_NMID_DT:		opw=0x4010|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHAL:	opw=0x4020|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLR:	opw=0x4001|((reg&15)<<8); break;
	case BGBCC_SH_NMID_CMPPZ:	opw=0x4011|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHAR:	opw=0x4021|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTL:	opw=0x4004|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ROTCL:	opw=0x4024|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTR:	opw=0x4005|((reg&15)<<8); break;
	case BGBCC_SH_NMID_CMPPL:	opw=0x4015|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ROTCR:	opw=0x4025|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLL2:	opw=0x4008|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLL8:	opw=0x4018|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLL16:	opw=0x4028|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLR2:	opw=0x4009|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLR8:	opw=0x4019|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLR16:	opw=0x4029|((reg&15)<<8); break;

	case BGBCC_SH_NMID_JSR:		opw=0x400B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_TASB:	opw=0x401B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_JMP:		opw=0x402B|((reg&15)<<8); break;

	case BGBCC_SH_NMID_FSTS:	opw=0xF00D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDS:	opw=0xF01D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLOAT:	opw=0xF02D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FTRC:	opw=0xF03D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FNEG:	opw=0xF04D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FABS:	opw=0xF05D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FSQRT:	opw=0xF06D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FSRRA:	opw=0xF07D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDI0:	opw=0xF08D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDI1:	opw=0xF09D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FCNVSD:	opw=0xF0AD|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FCNVDS:	opw=0xF0BD|((reg&15)<<8); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpImm(BGBCC_SHX_Context *ctx, int nmid, int imm)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_TRAPA:
		if(((byte)imm)!=imm)
			break;
		opw=0xC300|imm; break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpMReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_PREF:		opw=0x0083|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBI:		opw=0x0093|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBP:		opw=0x00A3|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBWB:		opw=0x00B3|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ICBI:		opw=0x00E3|((reg&15)<<8); break;

	default:
		break;
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_STC:
	case BGBCC_SH_NMID_STS:
		if((rm&0xF0)==0x10)
			{ opw=0x0002|(rn<<8)|((rm&15)<<4); break; }
		if((rm&0xF0)==0x20)
			{ opw=0x000A|(rn<<8)|((rm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_LDC:
	case BGBCC_SH_NMID_LDS:
		if((rn&0xF0)==0x10)
			{ opw=0x400E|(rm<<8)|((rn&15)<<4); break; }
		if((rn&0xF0)==0x20)
			{ opw=0x400A|(rm<<8)|((rn&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MULL:	opw=0x0007|(rn<<8)|(rm<<4); break;

	case BGBCC_SH_NMID_DIV0S:	opw=0x2007|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_TST:		opw=0x2008|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_AND:		opw=0x2009|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_OR:		opw=0x200A|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_XOR:		opw=0x200B|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_CMPSTR:	opw=0x200C|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_XTRCT:	opw=0x200D|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_MULUW:	opw=0x200E|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_MULSW:	opw=0x200F|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_CMPEQ:	opw=0x3000|(rn<<8)|(rm<<4); break;
	/* 3001=Esc32 */
	case BGBCC_SH_NMID_CMPHS:	opw=0x3002|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_CMPGE:	opw=0x3003|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_CMPHI:	opw=0x3006|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_CMPGT:	opw=0x3007|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_SUB:		opw=0x3008|(rn<<8)|(rm<<4); break;
	/* 3009=Esc32 */
	case BGBCC_SH_NMID_SUBC:	opw=0x300A|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_SUBV:	opw=0x300B|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_ADD:		opw=0x300C|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_ADDC:	opw=0x300E|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_ADDV:	opw=0x300F|(rn<<8)|(rm<<4); break;

	case BGBCC_SH_NMID_SHAD:	opw=0x400C|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_SHLD:	opw=0x400D|(rn<<8)|(rm<<4); break;
	/* 400E: LDC forms */
	case BGBCC_SH_NMID_MACW:	opw=0x400F|(rn<<8)|(rm<<4); break;

	/* 6oo0..6oo2: MOV @Rm, Rn */
	case BGBCC_SH_NMID_MOV:		opw=0x6003|(rn<<8)|(rm<<4); break;
	/* 6oo4..6oo6: MOV @Rm+, Rn */
	case BGBCC_SH_NMID_NOT:		opw=0x6007|(rn<<8)|(rm<<4); break;

	case BGBCC_SH_NMID_NEGC:	opw=0x600A|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_NEG:		opw=0x600B|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_EXTUB:	opw=0x600C|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_EXTUW:	opw=0x600D|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_EXTSB:	opw=0x600E|(rn<<8)|(rm<<4); break;
	case BGBCC_SH_NMID_EXTSW:	opw=0x600F|(rn<<8)|(rm<<4); break;

	case BGBCC_SH_NMID_FADD:	opw=0xF000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FSUB:	opw=0xF001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMUL:	opw=0xF002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FDIV:	opw=0xF003|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FCMPEQ:	opw=0xF004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FCMPGT:	opw=0xF005|((rn&15)<<8)|((rm&15)<<4); break;
	/* Foo6..FooB: FMOV.S forms */
	case BGBCC_SH_NMID_FMOV:	opw=0xF00C|((rn&15)<<8)|((rm&15)<<4); break;
	/* FooD: Single Arg Forms */
	case BGBCC_SH_NMID_FMAC:	opw=0xF00E|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	opw=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:	opw=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:	opw=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:	opw=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdRegReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:	opw=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:	opw=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:	opw=0x6002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SWAPB:	opw=0x6008|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SWAPW:	opw=0x6009|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:	opw=0xF008|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_STSL:
	case BGBCC_SH_NMID_STCL:
		if((rm&0xF0)==0x10)
			{ opw=0x4003|((rn&15)<<8)|((rm&15)<<4); break; }
		if((rm&0xF0)==0x20)
			{ opw=0x4002|((rn&15)<<8)|((rm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:	opw=0x2004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:	opw=0x2005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:	opw=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:	opw=0xF00B|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_LDSL:
	case BGBCC_SH_NMID_LDCL:
		if((rn&0xF0)==0x10)
			{ opw=0x4007|((rm&15)<<8)|((rn&15)<<4); break; }
		if((rn&0xF0)==0x20)
			{ opw=0x4006|((rm&15)<<8)|((rn&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:	opw=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:	opw=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:	opw=0x6006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:	opw=0xF009|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegStRegDisp(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int rn, int disp)
{
	int opw, opw2;

	if(disp==0)
	{
		return(BGBCC_SHX_EmitOpRegStReg(ctx, nmid, rm, rn));
	}

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(rm!=BGBCC_SH_REG_R0)		break;
		if(disp<0)				break;

		if(rn==BGBCC_SH_REG_GBR)
		{
			if(disp>=256)			break;
			opw=0xC000|disp;		break;
		}
		
		if(disp>=16)				break;
		opw=0x8000|((rn&15)<<4)|disp;
		break;
	case BGBCC_SH_NMID_MOVW:
		if(rm!=BGBCC_SH_REG_R0)		break;
		if((disp&1) || (disp<0))	break;

		if(rn==BGBCC_SH_REG_GBR)
		{
			if((disp>>1)>=256)		break;
			opw=0xC100|disp;		break;
		}

		if((disp>>1)>=16)			break;
		opw=0x8100|((rn&15)<<4)|(disp>>1);
		break;

	case BGBCC_SH_NMID_MOVL:
		if((disp&3) || (disp<0))	break;
		if(rn==BGBCC_SH_REG_GBR)
		{
			if(rm!=BGBCC_SH_REG_R0)	break;
			if((disp>>2)>=256)		break;
			opw=0xC200|disp;		break;
		}
		if((disp>>2)>=16)			break;
		opw=0x1000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int opw, opw2;

	if(disp==0)
	{
		return(BGBCC_SHX_EmitOpLdRegReg(ctx, nmid, rm, rn));
	}

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(rm!=BGBCC_SH_REG_R0)		break;
		if(disp<0)				break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if(disp>=256)			break;
			opw=0xC400|disp;		break;
		}
		
		if(disp>=16)				break;
		opw=0x8400|((rm&15)<<4)|disp;
		break;
	case BGBCC_SH_NMID_MOVW:
		if(rm!=BGBCC_SH_REG_R0)		break;
		if((disp&1) || (disp<0))	break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if((disp>>1)>=256)		break;
			opw=0xC500|disp;		break;
		}

		if((disp>>1)>=16)			break;
		opw=0x8500|((rm&15)<<4)|(disp>>1);
		break;

	case BGBCC_SH_NMID_MOVL:
		if((disp&3) || (disp<0))	break;
		if(rm==BGBCC_SH_REG_GBR)
		{
			if(rm!=BGBCC_SH_REG_R0)	break;
			if((disp>>2)>=256)		break;
			opw=0xC600|disp;		break;
		}
		if((disp>>2)>=16)			break;
		opw=0x5000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg2(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw, opw2;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
		return(BGBCC_SHX_EmitOpRegStReg(ctx, nmid, rm, rn));
	}

	if((ro!=BGBCC_SH_REG_R0) && (rn==BGBCC_SH_REG_R0))
		{ i=ro; ro=rn; rn=i; }

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdReg2Reg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw, opw2;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
		return(BGBCC_SHX_EmitOpRegStReg(ctx, nmid, rm, rn));
	}

	if((ro!=BGBCC_SH_REG_R0) && (rm==BGBCC_SH_REG_R0))
		{ i=ro; ro=rm; rn=i; }

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0x000C|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0x000D|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		opw=0xF006|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegImm(BGBCC_SHX_Context *ctx, int nmid, int reg, int imm)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:	
		if(((sbyte)imm)!=imm)
			break;
		opw=0x7000|((reg&15)<<8)|(imm&255);
		break;

	case BGBCC_SH_NMID_CMPEQ:	
		if(((sbyte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		opw=0x8800|((reg&15)<<8)|(imm&255);
		break;

	case BGBCC_SH_NMID_TST:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		opw=0xC800|((reg&15)<<8)|(imm&255);
		break;
	case BGBCC_SH_NMID_AND:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		opw=0xC900|((reg&15)<<8)|(imm&255);
		break;
	case BGBCC_SH_NMID_OR:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		opw=0xCA00|((reg&15)<<8)|(imm&255);
		break;
	case BGBCC_SH_NMID_XOR:	
		if(((byte)imm)!=imm)		break;
		if(reg!=BGBCC_SH_REG_R0)	break;
		opw=0xCB00|((reg&15)<<8)|(imm&255);
		break;

	case BGBCC_SH_NMID_MOV:	
		if(((sbyte)imm)!=imm)
			break;
		opw=0xE000|((reg&15)<<8)|(imm&255);
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitIndexAddImm16(BGBCC_SHX_Context *ctx, int imm)
{
	int i, n, o;

	for(i=0; i<ctx->const_ns16; i++)
	{
		if(ctx->const_s16tab[i]==imm)
		{
			n=ctx->nofs_s16tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s16tab[n]=o;
			ctx->idx_s16tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns16++;
	n=ctx->nofs_s16tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s16tab[n]=o;
	ctx->idx_s16tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm32(BGBCC_SHX_Context *ctx, int imm)
{
	int i, n, o;

	for(i=0; i<ctx->const_ns32; i++)
	{
		if(ctx->const_s32tab[i]==imm)
		{
			n=ctx->nofs_s32tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s32tab[n]=o;
			ctx->idx_s32tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns32++;
	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitLoadRegImm(
	BGBCC_SHX_Context *ctx, int nmid, int reg, int imm)
{
	int opw, opw2;

	if((((sbyte)imm)==imm) && ((reg&0xF0)==0x00))
	{
		opw=0xE000|((reg&15)<<8)|(imm&255);
		BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}
	
	if((((s16)imm)==imm) && ((reg&0xF0)==0x00))
	{
		opw=0x9000|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm16(ctx, imm);
		return(1);
	}

	if((((s32)imm)==imm) && ((reg&0xF0)==0x00))
	{
		opw=0xD000|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm32(ctx, imm);
		return(1);
	}
}

int BGBCC_SHX_EmitFlushIndexImm16(BGBCC_SHX_Context *ctx)
{
	int ob, opc, opw;
	int i;
	
	if(!ctx->const_ns16)
		return(0);
	
	BGBCC_SHX_EmitBAlign(ctx, 2);
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	for(i=0; i<ctx->const_ns16; i++)
		{ BGBCC_SHX_EmitWord(ctx, const_s16tab[i]); }

	for(i=0; i<ctx->nofs_s16tab; i++)
	{
		opc=ctx->ofs_s16tab[i];
		opd=((ob-(opc+4))/2)+(ctx->idx_s16tab[i]);
		opw=BGBCC_SHX_EmitGetOffsWord(ctx, opc);
		opw+=opd;
		BGBCC_SHX_EmitSetOffsWord(ctx, opc, opw);
	}
	
	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	return(1);
}

int BGBCC_SHX_EmitFlushIndexImm32(BGBCC_SHX_Context *ctx)
{
	int ob, opc, opw;
	int i;
	
	if(!ctx->const_ns32)
		return(0);

	BGBCC_SHX_EmitBAlign(ctx, 4);
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	for(i=0; i<ctx->const_ns32; i++)
		{ BGBCC_SHX_EmitDWord(ctx, const_s32tab[i]); }

	for(i=0; i<ctx->nofs_s32tab; i++)
	{
		opc=ctx->ofs_s32tab[i];
		opd=((ob-(opc+4))/4)+(ctx->idx_s32tab[i]);
		opw=BGBCC_SHX_EmitGetOffsWord(ctx, opc);
		opw+=opd;
		BGBCC_SHX_EmitSetOffsWord(ctx, opc, opw);
	}
	
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;
	return(1);
}

int BGBCC_SHX_EmitCheckFlushIndexImmP(BGBCC_SHX_Context *ctx)
{
	int ob, obe16, obe32;
	int oo;

	if(!ctx->const_ns16 && !ctx->const_ns32)
		return(0);
	
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	ob=(ob+3)&(~3);
	obe16=ob+(((ctx->const_ns16+1)&(~1))*2);
	obe32=obe16+((ctx->const_ns32)*4);
	
	if(ctx->nofs_s16tab)
	{
		oo=ctx->ofs_s16tab[0];
		if((obe16-oo)>=480)
			return(1);
	}

	if(ctx->nofs_s32tab)
	{
		oo=ctx->ofs_s32tab[0];
		if((obe16-oo)>=960)
			return(1);
	}
	
	return(0);
}

int BGBCC_SHX_EmitCheckFlushIndexImm(BGBCC_SHX_Context *ctx)
{
	int l0;

	if(!BGBCC_SHX_EmitCheckFlushIndexImmP(ctx))
		return(0);
	
	l0=BGBCC_SHX_GenLabelTemp(ctx);
	BGBCC_SHX_EmitRelocRelW12(ctx, l0);
	BGBCC_SHX_EmitWord(ctx, 0xA000);
	BGBCC_SHX_EmitFlushIndexImm16(ctx);
	BGBCC_SHX_EmitFlushIndexImm32(ctx);
	BGBCC_SHX_EmitLabel(ctx, l0);
	return(1);
}
