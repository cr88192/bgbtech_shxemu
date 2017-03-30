#include <bgbccc.h>

ccxl_label ril3_lbltab[4096];
int ril3_nlbl;

int BGBCC_CCXLR3_LabelToIndex(BGBCC_TransState *ctx, ccxl_label lbl)
{
	int i;
	
	for(i=0; i<ril3_nlbl; i++)
	{
		if(ril3_lbltab[i].id==lbl.id)
			return(i);
	}
	
	i=ril3_nlbl++;
	ril3_lbltab[i].id=lbl.id;
	return(i);
}

ccxl_label BGBCC_CCXLR3_IndexToLabel(BGBCC_TransState *ctx, int ix)
{
	if(ril3_lbltab[ix].id)
		return(ril3_lbltab[ix]);
	
	if(ix>=ril3_nlbl)
		ril3_nlbl=ix+1;
	
	ril3_lbltab[ix]=BGBCC_CCXL_GenSym(ctx);
	return(ril3_lbltab[ix]);
}

void BGBCC_CCXLR3_CheckLzWindow(
	BGBCC_TransState *ctx)
{
	if(!ctx->ril_txwin)
	{
		ctx->ril_txwin=bgbcc_malloc(16384);
		ctx->ril_txhash=bgbcc_malloc(256*4*2);
		ctx->ril_txrov=0;
		ctx->ril_psrov=0;
	}
}

void BGBCC_CCXLR3_CheckExpandOutput(
	BGBCC_TransState *ctx, int sz)
{
	int i, j, k;

	if(!(ctx->ril_ip))
	{
		i=sz;
		j=65536;
		while(i>=j)
			j=j+(j>>1);
		ctx->ril_ips=bgbcc_malloc(j);
		ctx->ril_ipe=ctx->ril_ips+j;
		ctx->ril_ip=ctx->ril_ips;
		BGBCC_CCXLR3_CheckLzWindow(ctx);
	}
	
	if((ctx->ril_ip+sz)>=(ctx->ril_ipe))
	{
		k=ctx->ril_ip-ctx->ril_ips;
		i=k+sz;
		j=ctx->ril_ipe-ctx->ril_ips;

		while(i>=j)
			j=j+(j>>1);

		ctx->ril_ips=bgbcc_realloc(ctx->ril_ips, j);
		ctx->ril_ipe=ctx->ril_ips+j;
		ctx->ril_ip=ctx->ril_ips+k;
	}
}

void BGBCC_CCXLR3_EmitByte(
	BGBCC_TransState *ctx, int val)
{
	BGBCC_CCXLR3_CheckExpandOutput(ctx, 1);
	*ctx->ril_ip++=val;
}

void BGBCC_CCXLR3_EmitUVLI(
	BGBCC_TransState *ctx, u64 val)
{
	if(val<0x80)
	{
		BGBCC_CCXLR3_EmitByte(ctx, val);
	}else if(val<0x4000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0x80|((val>>8)&63));
		BGBCC_CCXLR3_EmitByte(ctx, val&255);
	}else if(val<0x200000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xC0|((val>>16)&31));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val   )&255);
	}else if(val<0x10000000)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xE0|((val>>24)&15));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x800000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xF0|((val>>32)&7));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x40000000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xF8|((val>>40)&3));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x2000000000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xFC|((val>>48)&1));
		BGBCC_CCXLR3_EmitByte(ctx, (val>>40)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else if(val<0x1000000000000ULL)
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xFE);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>48)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>40)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}else
	{
		BGBCC_CCXLR3_EmitByte(ctx, 0xFF);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>56)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>48)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>40)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>32)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>24)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>>16)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val>> 8)&255);
		BGBCC_CCXLR3_EmitByte(ctx, (val    )&255);
	}
}

void BGBCC_CCXLR3_EmitSVLI(
	BGBCC_TransState *ctx, s64 val)
{
	u64 val2;
	
	val2=(val<<1)^(val>>63);
	BGBCC_CCXLR3_EmitUVLI(ctx, val2);
}

void BGBCC_CCXLR3_UpdateTextByte(
	BGBCC_TransState *ctx, int val)
{
	int h;
	
//	ctx->ril_txrh=(ctx->ril_txrh<<8)|(val&255);
//	h=(((ctx->ril_txrh&0x00FFFFFF)*16777213)>>24)&255;
	h=(((ctx->ril_txrh&0x0000FFFF)*65521)>>16)&255;
//	ctx->ril_txhash[h]=(ctx->ril_txrov-3)&16383;
//	ctx->ril_txhash[h]=(ctx->ril_txrov-2)&16383;

	ctx->ril_txhash[h*4+3]=ctx->ril_txhash[h*4+2];
	ctx->ril_txhash[h*4+2]=ctx->ril_txhash[h*4+1];
	ctx->ril_txhash[h*4+1]=ctx->ril_txhash[h*4+0];
	ctx->ril_txhash[h*4+0]=(ctx->ril_txrov-2)&16383;
//	ctx->ril_txhash[h*4+0]=(ctx->ril_txrov-3)&16383;

	ctx->ril_txwin[ctx->ril_txrov]=val;
	ctx->ril_txrov=(ctx->ril_txrov+1)&16383;
	ctx->ril_txrh=(ctx->ril_txrh<<8)|(val&255);
}

void BGBCC_CCXLR3_UpdateTextString(
	BGBCC_TransState *ctx, byte *str, int sz)
{
	int i;
	for(i=0; i<sz; i++)
		BGBCC_CCXLR3_UpdateTextByte(ctx, str[i]);
}

int BGBCC_CCXLR3_TextLookupMatch(
	BGBCC_TransState *ctx, byte *str, byte *stre, int *rl, int *rd)
{
	byte *s, *se;
	int h, p, l, d, bl, bd;
	int c0, c1;
	int i;
	
	if((str+3)>=stre)
		return(0);
	
//	h=(((ctx->ril_txrh&0x00FFFFFF)*16777213)>>24)&255;
//	h=(((ctx->ril_txrh&0x0000FFFF)*65521)>>16)&255;

//	p=(str[0]<<16)|(str[1]<<8)|str[2];
//	h=(((p&0x00FFFFFF)*16777213)>>24)&255;

	p=(str[0]<<8)|str[1];
	h=(((p&0x0000FFFF)*65521)>>16)&255;
	
	bl=0; bd=0;
	for(i=0; i<4; i++)
	{
		p=ctx->ril_txhash[h*4+i];
		d=(ctx->ril_txrov-p)&16383;
		
		s=str;
//		se=str+252;
		se=str+4088;
		if(se>stre)se=stre;
		while(s<se)
		{
			c0=*s; c1=ctx->ril_txwin[p];
			if(c0!=c1)break;
			s++; p=(p+1)&16383;
		}

		l=s-str;
		if(l>bl)
			{ bl=l; bd=d; }
	}
	
	if((bd>=16128) || ((bl+bd)>=16384))
		bl=0;
	
	if(bl>=3)
	{
		*rl=bl;
		*rd=bd;
		return(1);
	}
	
	*rl=0;
	*rd=0;
	return(0);
}

void BGBCC_CCXLR3_EmitTextByte(
	BGBCC_TransState *ctx, int val)
{
	BGBCC_CCXLR3_EmitByte(ctx, val);
	BGBCC_CCXLR3_UpdateTextByte(ctx, val);
}

void BGBCC_CCXLR3_EmitRawString(
	BGBCC_TransState *ctx, char *str)
{
	char *s;
	
	s=str;
	while(*s)
		BGBCC_CCXLR3_EmitTextByte(ctx, *s++);
	BGBCC_CCXLR3_EmitTextByte(ctx, *s++);
}

void BGBCC_CCXLR3_EmitOp(
	BGBCC_TransState *ctx, int op)
{
	if(!(ctx->ril_ip))
		return;

	BGBCC_CCXLR3_EmitUVLI(ctx, op);
}

void BGBCC_CCXLR3_EmitArgInt(
	BGBCC_TransState *ctx, s64 val)
{
	if(!(ctx->ril_ip))
		return;

//	BGBCC_CCXLR3_EmitUVLI(ctx, BGBCC_RIL3OP_ARGINT);
	BGBCC_CCXLR3_EmitSVLI(ctx, val);
}

void BGBCC_CCXLR3_EmitArgTag(
	BGBCC_TransState *ctx, s64 val)
{
	if(!(ctx->ril_ip))
		return;

	if((val>0x8000) && (val<0x8020))
		val=val&0x1F;
	if((val>0x9000) && (val<0x9020))
		val=0x20|(val&0x1F);

//	BGBCC_CCXLR3_EmitUVLI(ctx, BGBCC_RIL3OP_ARGINT);
	BGBCC_CCXLR3_EmitSVLI(ctx, val);
}

void BGBCC_CCXLR3_EmitArgFloat(
	BGBCC_TransState *ctx, f64 val)
{
	u64 fv;
	s64 fm;
	int fe, fs;

	if(!(ctx->ril_ip))
		return;

	fv=*(u64 *)(&val);
	fm=(fv&0x000FFFFFFFFFFFFFULL)|0x0010000000000000ULL;
	fe=(fv>>52)&2047;
	fs=(fv>>63)&1;
	fe-=(1023+52);
	
	while(!(fm&1))
		{ fm=fm>>1; fe++; }
	if(fs)
		{ fm=-fm; }

//	BGBCC_CCXLR3_EmitUVLI(ctx, BGBCC_RIL3OP_ARGINT);
	BGBCC_CCXLR3_EmitSVLI(ctx, fe);
	BGBCC_CCXLR3_EmitSVLI(ctx, fm);
}

void BGBCC_CCXLR3_EmitBufUVLI(
	byte **rct, u64 val)
{
	byte *ct;
	
	ct=*rct;
	if(val<0x80)
	{
		*ct++=val;
	}else if(val<0x4000)
	{
		*ct++=(0x80|((val>>8)&63));		*ct++=(val&255);
	}else if(val<0x200000)
	{
		*ct++=(0xC0|((val>>16)&31));
		*ct++=((val>>8)&255);			*ct++=((val   )&255);
	}else if(val<0x10000000)
	{
		*ct++=(0xE0|((val>>24)&15));	*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x800000000ULL)
	{
		*ct++=(0xF0|((val>>32)&7));
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x40000000000ULL)
	{
		*ct++=(0xF8|((val>>40)&3));		*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x2000000000000ULL)
	{
		*ct++=(0xFC|((val>>48)&1));
		*ct++=((val>>40)&255);			*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else if(val<0x1000000000000ULL)
	{
		*ct++=(0xFE);					*ct++=((val>>48)&255);
		*ct++=((val>>40)&255);			*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}else
	{
		*ct++=(0xFF);
		*ct++=((val>>56)&255);			*ct++=((val>>48)&255);
		*ct++=((val>>40)&255);			*ct++=((val>>32)&255);
		*ct++=((val>>24)&255);			*ct++=((val>>16)&255);
		*ct++=((val>> 8)&255);			*ct++=((val    )&255);
	}
	*rct=ct;
}

/*
 * 0: NULL
 * >0: String with N chars.
 * -1 .. -64: Index for recently seen string.
 * < (-65): LZ Compressed String
 */
void BGBCC_CCXLR3_EmitArgString(
	BGBCC_TransState *ctx, char *str)
{
	byte ttb[65536];
	byte *ct;
	byte *s, *se, *rs;
	int c0, c1;
	int l, r, t, p, ml, md;
	int i, j, k;

	if(!(ctx->ril_ip))
		return;

	if(!str)
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, 0);
		return;
	}

	l=strlen(str);
	if(!l)l=1;

	for(i=0; i<64; i++)
	{
		j=(ctx->ril_psrov-i-1)&63;
		if(ctx->ril_pslen[j]!=l)
			continue;
		s=(byte *)str;
		p=ctx->ril_psidx[j];
		for(k=0; k<l; k++)
		{
			c0=*s; c1=ctx->ril_txwin[p];
			if(c0!=c1)break;
			s++; p=(p+1)&16383;
		}
		if(k>=l)
			break;
	}
	if(i<64)
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, -(i+1));
		return;
	}

	i=ctx->ril_psrov;
	ctx->ril_psidx[i]=ctx->ril_txrov;
	ctx->ril_pslen[i]=l;
	ctx->ril_psrov=(i+1)&63;

	if(l<8)
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, l);
		s=(byte *)str;
//		while(*s)
		for(i=0; i<l; i++)
			BGBCC_CCXLR3_EmitTextByte(ctx, *s++);
		return;
	}
	
	ct=ttb;
	s=(byte *)str;
	se=s+l;
	
	rs=s;
	while(s<se)
	{
		if(BGBCC_CCXLR3_TextLookupMatch(ctx, s, se, &ml, &md)>0)
		{
			t=0x00;
			r=s-rs;
			if(r>=15)
				{ t|=0xF0; }
			else
				{ t|=(r<<4); }
			if(ml>=17)
				{ t|=0x0F; }
			else
				{ t|=(ml-2); }
			
			*ct++=t;
			if(r>=15)
				{ BGBCC_CCXLR3_EmitBufUVLI(&ct, r); }
			while(rs<s)
				{ *ct++=*rs++; }
			if(ml>=17)
				{ BGBCC_CCXLR3_EmitBufUVLI(&ct, ml); }
			BGBCC_CCXLR3_EmitBufUVLI(&ct, md);
			for(i=0; i<ml; i++)
				BGBCC_CCXLR3_UpdateTextByte(ctx, *s++);
			rs=s;
		}else
		{
			BGBCC_CCXLR3_UpdateTextByte(ctx, *s++);
		}
	}
	r=s-rs;
	if(r>0)
	{
		t=0x00;
		if(r>=15)
			{ t|=0xF0; }
		else
			{ t|=(r<<4); }
		*ct++=t;
		if(r>=15)
			{ BGBCC_CCXLR3_EmitBufUVLI(&ct, r); }
		while(rs<s)
			{ *ct++=*rs++; }
	}

	k=ct-ttb;

	if((k+3)>=(l+1))
	{
		BGBCC_CCXLR3_EmitSVLI(ctx, l);
		s=(byte *)str;
		for(i=0; i<l; i++)
			BGBCC_CCXLR3_EmitByte(ctx, *s++);
		return;
	}
	
	BGBCC_CCXLR3_EmitSVLI(ctx, -(k+64));
	BGBCC_CCXLR3_EmitSVLI(ctx, l);
	s=ttb;
	while(s<ct)
		BGBCC_CCXLR3_EmitByte(ctx, *s++);
	return;
}

void BGBCC_CCXLR3_EmitArgLabel(
	BGBCC_TransState *ctx, ccxl_label lbl)
{
	int i;
	i=BGBCC_CCXLR3_LabelToIndex(ctx, lbl);
	BGBCC_CCXLR3_EmitArgInt(ctx, i);
}

void BGBCC_CCXLR3_EmitArgTextBlob(
	BGBCC_TransState *ctx, char *str)
{
	BGBCC_CCXLR3_EmitArgString(ctx, str);
}

void BGBCC_CCXLR3_BeginRecRIL(
	BGBCC_TransState *ctx)
{
	ctx->ril_ip=ctx->ril_ips;
	ctx->ril_txrov=0;

	BGBCC_CCXLR3_EmitByte(ctx, 'R');
	BGBCC_CCXLR3_EmitByte(ctx, 'I');
	BGBCC_CCXLR3_EmitByte(ctx, 'L');
	BGBCC_CCXLR3_EmitByte(ctx, '3');
	BGBCC_CCXLR3_EmitByte(ctx, 1);	//format major.minor
	BGBCC_CCXLR3_EmitByte(ctx, 0);	//resv
	BGBCC_CCXLR3_EmitByte(ctx, 0);	//resv
	BGBCC_CCXLR3_EmitByte(ctx, 0);	//resv
}

u64 BGBCC_CCXLR3_ReadUVLI(BGBCC_TransState *ctx, byte **rcs)
{
	byte *cs;
	u64 tv;
	int i, j, k;
	
	cs=*rcs;
	i=*cs++;
	if(!(i&0x80))
		{ *rcs=cs; return(i); }
	if(!(i&0x40))
	{
		tv=((i&63)<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x20))
	{
		tv=((i&31)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x10))
	{
		tv=((i&15)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x08))
	{
		tv=((i&7)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x04))
	{
		tv=((i&3)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x02))
	{
		tv=((i&1)<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
	if(!(i&0x01))
	{
		tv=(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}

	if(1)
	{
		tv=(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		tv=(tv<<8)|(*cs++);
		*rcs=cs; return(tv);
	}
}

s64 BGBCC_CCXLR3_ReadSVLI(BGBCC_TransState *ctx, byte **rcs)
{
	u64 tv;
	s64 sv;

	tv=BGBCC_CCXLR3_ReadUVLI(ctx, rcs);
	sv=(tv>>1)^((((s64)tv)<<63)>>63);
	return(sv);
}

f64 BGBCC_CCXLR3_ReadFVLI(BGBCC_TransState *ctx, byte **rcs)
{
	int fe;
	s64 fm;
	f64 v;
	
	fe=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	fm=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	v=fm*pow(2.0, fe);
	return(v);
}

int BGBCC_CCXLR3_ReadTag(BGBCC_TransState *ctx, byte **rcs)
{
	int i;
	i=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if((i>=0x00) && (i<=0x1F))
		i=0x8000+(i&0x1F);
	if((i>=0x20) && (i<=0x3F))
		i=0x9000+(i&0x1F);
	return(i);
}

int BGBCC_CCXLR3_ReadTextBlob(BGBCC_TransState *ctx, byte **rcs,
	byte **rbuf, int *rsz)
{
	byte *tbuf, *cs, *cse, *ct, *cte;
	int sz, csz, tsz, p, t, ml, md, mr;
	int i, j, k;

	sz=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if(!sz)
	{
		*rbuf=NULL;
		*rsz=0;
		return(0);
	}

	tbuf=*rbuf;
	tsz=*rsz;
	
	if(sz>0)
	{
		if(!tbuf || (sz>=tsz))
		{
			tbuf=bgbcc_malloc(sz+1);
			*rbuf=tbuf;
		}

		memcpy(tbuf, *rcs, sz);
		tbuf[sz]=0;
		*rsz=sz;
		*rcs=(*rcs)+sz;
		
		i=ctx->ril_psrov;
		ctx->ril_psidx[i]=ctx->ril_txrov;
		ctx->ril_pslen[i]=sz;
		ctx->ril_psrov=(i+1)&63;
		BGBCC_CCXLR3_UpdateTextString(ctx, tbuf, sz);
		
		return(0);
	}
	
	i=(-sz)-1;
	if(i<64)
	{
		j=(ctx->ril_psrov-i-1)&63;
		sz=ctx->ril_pslen[j];
		p=ctx->ril_psidx[j];

		if(!tbuf || (sz>=tsz))
		{
			tbuf=bgbcc_malloc(sz+1);
			*rbuf=tbuf;
		}
		*rsz=sz;
		
		for(k=0; k<sz; k++)
			{ tbuf[k]=ctx->ril_txwin[(p+k)&16383]; }
		tbuf[sz]=0;
		return(0);
	}
	
	csz=(-sz)-64;
	sz=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if(sz<=0)
		__debugbreak();

	i=ctx->ril_psrov;
	ctx->ril_psidx[i]=ctx->ril_txrov;
	ctx->ril_pslen[i]=sz;
	ctx->ril_psrov=(i+1)&63;

	if(!tbuf || (sz>=tsz))
	{
		tbuf=bgbcc_malloc(sz+1);
		*rbuf=tbuf;
	}

	*rsz=sz;

	cs=*rcs; cse=cs+csz;
	ct=tbuf; cte=tbuf+sz;
	
	while((cs<cse) && (ct<cte))
	{
		t=*cs++;
		
		if(t&0xF0)
		{
			mr=(t>>4)&15;
			if(mr>=15)
				{ mr=BGBCC_CCXLR3_ReadUVLI(ctx, &cs); }

			BGBCC_CCXLR3_UpdateTextString(ctx, cs, mr);
			for(i=0; i<mr; i++)
				*ct++=*cs++;
		}
		
		if(t&0x0F)
		{
			ml=(t&0x0F)+2;
			if(ml>=17)
				{ ml=BGBCC_CCXLR3_ReadUVLI(ctx, &cs); }
			md=BGBCC_CCXLR3_ReadUVLI(ctx, &cs);
			
			for(i=0; i<ml; i++)
			{
				j=ctx->ril_txwin[(ctx->ril_txrov-md)&16383];
				*ct++=j;
				BGBCC_CCXLR3_UpdateTextByte(ctx, j);
			}
		}
	}
	
	*ct=0;
	
	*rcs=cse;
	return(1);
	
//	__debugbreak();
//	return(-1);
}

char *BGBCC_CCXLR3_ReadString(BGBCC_TransState *ctx, byte **rcs)
{
	byte tb[4096];
	byte *s0, *s1;
	int sz;

	s0=tb; sz=4096;
	BGBCC_CCXLR3_ReadTextBlob(ctx, rcs, &s0, &sz);
	
	s1=(byte *)bgbcc_rstrdup((char *)s0);
	if(s0!=tb)
		bgbcc_free(s0);
	return(s1);

#if 0
	sz=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	if(!sz)
		{ return(NULL); }
	
	if(sz>0)
	{
		s0=bgbcc_ralloc(sz+1);
		memcpy(s0, *rcs, sz);
		*rcs=(*rcs)+sz;
		return(s0);
	}
	
	__debugbreak();
	return(NULL);
#endif
}

ccxl_label BGBCC_CCXLR3_ReadLabel(BGBCC_TransState *ctx, byte **rcs)
{
	ccxl_label lbl;
	int i0;
	
	i0=BGBCC_CCXLR3_ReadSVLI(ctx, rcs);
	lbl=BGBCC_CCXLR3_IndexToLabel(ctx, i0);
	return(lbl);
}

void BGBCC_CCXLR3_DecodeBufCmd(
	BGBCC_TransState *ctx, byte **rcs)
{
	char tb[4096];
	BGBCC_CCXL_RegisterInfo *decl;
	byte *cs, *tbuf;
	char *s0, *s1, *s2;
	ccxl_label lbl;
	s64 li0, li1;
	f64 f0, f1;
	int i0, i1, i2, i3;
	int op, tsz;
	
	cs=*rcs;
	op=*cs++;
	switch(op)
	{
	case BGBCC_RIL3OP_END:
		BGBCC_CCXL_End(ctx);
		break;
	case BGBCC_RIL3OP_ASMBLOB:
		tbuf=tb; tsz=4096;
		BGBCC_CCXLR3_ReadTextBlob(ctx, &cs, &tbuf, &tsz);
		BGBCC_CCXL_AddAsmBlob(ctx, tbuf);
		if(tbuf!=tb)
			bgbcc_free(tbuf);
		break;
	case BGBCC_RIL3OP_BEGIN:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		BGBCC_CCXL_Begin(ctx, i0);
		break;
	case BGBCC_RIL3OP_BEGINNAME:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_BeginName(ctx, i0, s0);
		break;
	case BGBCC_RIL3OP_MARKER:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		BGBCC_CCXL_Marker(ctx, i0);
		break;

	case BGBCC_RIL3OP_ATTRINT:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_AttribInt(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_ATTRLONG:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_AttribLong(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_ATTRSTR:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_AttribStr(ctx, i0, s1);
		break;

	case BGBCC_RIL3OP_LITINT:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_LiteralInt(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_LITLONG:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		li1=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_LiteralLong(ctx, i0, li1);
		break;
	case BGBCC_RIL3OP_LITFLOAT:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		f1=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_LiteralFloat(ctx, i0, f1);
		break;
	case BGBCC_RIL3OP_LITDOUBLE:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		f1=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_LiteralDouble(ctx, i0, f1);
		break;
	case BGBCC_RIL3OP_LITSTR:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_LiteralStr(ctx, i0, s1);
		break;
	case BGBCC_RIL3OP_LITWSTR:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_LiteralWStr(ctx, i0, s1);
		break;
	case BGBCC_RIL3OP_LITNAME:
		i0=BGBCC_CCXLR3_ReadTag(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
//		decl=BGBCC_CCXL_LookupGlobal(ctx, s1);
		decl=BGBCC_CCXL_GetGlobal(ctx, s1);
		BGBCC_CCXL_LiteralGlobalAddr(ctx, i0, decl->gblid);
		break;

	case BGBCC_RIL3OP_MARK:
		BGBCC_CCXL_PushMark(ctx);
		break;
	case BGBCC_RIL3OP_LOAD:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_PushLoad(ctx, s0);
		break;
	case BGBCC_RIL3OP_STORE:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_PopStore(ctx, s0);
		break;
	case BGBCC_RIL3OP_CALLN:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackCallName(ctx, s0);
		break;
	case BGBCC_RIL3OP_CALLP:
		BGBCC_CCXL_StackPopCall(ctx);
		break;

	case BGBCC_RIL3OP_LDIXC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackLoadIndexConst(ctx, i0);
		break;
	case BGBCC_RIL3OP_STIXC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackStoreIndexConst(ctx, i0);
		break;
	case BGBCC_RIL3OP_LDIXAC:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackLoadIndexAddrConst(ctx, i0);
		break;
	case BGBCC_RIL3OP_LDIX:
		BGBCC_CCXL_StackLoadIndex(ctx);
		break;
	case BGBCC_RIL3OP_STIX:
		BGBCC_CCXL_StackStoreIndex(ctx);
		break;
	case BGBCC_RIL3OP_LDIXA:
		BGBCC_CCXL_StackLoadIndexAddr(ctx);
		break;
	case BGBCC_RIL3OP_DUP:
		BGBCC_CCXL_StackDup(ctx);
		break;
	case BGBCC_RIL3OP_DUPCLEAN:
		BGBCC_CCXL_StackDupClean(ctx);
		break;
	case BGBCC_RIL3OP_RETV:
		BGBCC_CCXL_StackRetV(ctx);
		break;
	case BGBCC_RIL3OP_RET:
		BGBCC_CCXL_StackRet(ctx);
		break;
	case BGBCC_RIL3OP_CASTBOOL:
		BGBCC_CCXL_StackCastBool(ctx);
		break;
	case BGBCC_RIL3OP_CASTSIG:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackCastSig(ctx, s0);
		break;

	case BGBCC_RIL3OP_BINOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_BINOP_ADD: s0="+"; break;
		case CCXL_BINOP_SUB: s0="-"; break;
		case CCXL_BINOP_MUL: s0="*"; break;
		case CCXL_BINOP_DIV: s0="/"; break;
		case CCXL_BINOP_MOD: s0="%"; break;
		case CCXL_BINOP_AND: s0="&"; break;
		case CCXL_BINOP_OR: s0="|"; break;
		case CCXL_BINOP_XOR: s0="^"; break;
		case CCXL_BINOP_SHL: s0="<<"; break;
		case CCXL_BINOP_SHR: s0=">>"; break;
		case CCXL_BINOP_SHRR: s0=">>>"; break;
		}
		BGBCC_CCXL_StackBinaryOp(ctx, s0);
		break;

	case BGBCC_RIL3OP_CMPOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_CMP_EQ: s0="=="; break;
		case CCXL_CMP_NE: s0="!="; break;
		case CCXL_CMP_LT: s0="<"; break;
		case CCXL_CMP_GT: s0=">"; break;
		case CCXL_CMP_LE: s0="<="; break;
		case CCXL_CMP_GE: s0=">="; break;
		}
		BGBCC_CCXL_StackBinaryOp(ctx, s0);
		break;

	case BGBCC_RIL3OP_UNOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		switch(i0)
		{
		case CCXL_UNOP_MOV: s0="+"; break;
		case CCXL_UNOP_NEG: s0="-"; break;
		case CCXL_UNOP_NOT: s0="~"; break;
		case CCXL_UNOP_LNOT: s0="!"; break;
		case CCXL_UNOP_INC: s0="++"; break;
		case CCXL_UNOP_DEC: s0="--"; break;
		}
		BGBCC_CCXL_StackUnaryOp(ctx, s0);
		break;

	case BGBCC_RIL3OP_STBINOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		switch(i0)
		{
		case CCXL_BINOP_ADD: s0="+"; break;
		case CCXL_BINOP_SUB: s0="-"; break;
		case CCXL_BINOP_MUL: s0="*"; break;
		case CCXL_BINOP_DIV: s0="/"; break;
		case CCXL_BINOP_MOD: s0="%"; break;
		case CCXL_BINOP_AND: s0="&"; break;
		case CCXL_BINOP_OR: s0="|"; break;
		case CCXL_BINOP_XOR: s0="^"; break;
		case CCXL_BINOP_SHL: s0="<<"; break;
		case CCXL_BINOP_SHR: s0=">>"; break;
		case CCXL_BINOP_SHRR: s0=">>>"; break;
		}
		BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
		break;

	case BGBCC_RIL3OP_STCMPOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		switch(i0)
		{
		case CCXL_CMP_EQ: s0="=="; break;
		case CCXL_CMP_NE: s0="!="; break;
		case CCXL_CMP_LT: s0="<"; break;
		case CCXL_CMP_GT: s0=">"; break;
		case CCXL_CMP_LE: s0="<="; break;
		case CCXL_CMP_GE: s0=">="; break;
		}
		BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
		break;
	case BGBCC_RIL3OP_LDUNOP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		switch(i0)
		{
		case CCXL_UNOP_MOV: s0="+"; break;
		case CCXL_UNOP_NEG: s0="-"; break;
		case CCXL_UNOP_NOT: s0="~"; break;
		case CCXL_UNOP_LNOT: s0="!"; break;
		case CCXL_UNOP_INC: s0="++"; break;
		case CCXL_UNOP_DEC: s0="--"; break;
		}
		BGBCC_CCXL_StackUnaryOpName(ctx, s0, s1);
		break;
		
	case BGBCC_RIL3OP_LDCONSTI:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstInt(ctx, li0);
		break;
	case BGBCC_RIL3OP_LDCONSTL:
		li0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstLong(ctx, li0);
		break;
	case BGBCC_RIL3OP_LDCONSTF:
		f0=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstFloat(ctx, f0);
		break;
	case BGBCC_RIL3OP_LDCONSTD:
		f0=BGBCC_CCXLR3_ReadFVLI(ctx, &cs);
		BGBCC_CCXL_StackPushConstDouble(ctx, f0);
		break;
	case BGBCC_RIL3OP_LDCONSTS:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackPushConstString(ctx, s0);
		break;
	case BGBCC_RIL3OP_LDCONSTWS:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackPushConstWString(ctx, s0);
		break;

	case BGBCC_RIL3OP_POP:
		BGBCC_CCXL_StackPop(ctx);
		break;
	case BGBCC_RIL3OP_LDA:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackLoadAddr(ctx, s0);
		break;
	case BGBCC_RIL3OP_SIZEOFSG:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackSizeofSig(ctx, s0);
		break;
	case BGBCC_RIL3OP_SIZEOFVAL:
		BGBCC_CCXL_StackSizeofVal(ctx);
		break;
	case BGBCC_RIL3OP_OFFSETOF:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		s1=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackOffsetof(ctx, s0, s1);
		break;
	case BGBCC_RIL3OP_LOADSLOT:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackLoadSlot(ctx, s0);
		break;
	case BGBCC_RIL3OP_STORESLOT:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackStoreSlot(ctx, s0);
		break;
	case BGBCC_RIL3OP_LOADSLOTA:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackLoadSlotAddr(ctx, s0);
		break;

	case BGBCC_RIL3OP_BEGINU:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackBeginU(ctx, s0);
		break;
	case BGBCC_RIL3OP_ENDU:
		BGBCC_CCXL_StackEndU(ctx);
		break;
	case BGBCC_RIL3OP_SETU:
		BGBCC_CCXL_StackSetU(ctx);
		break;
	case BGBCC_RIL3OP_INITVAR:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackInitVar(ctx, s0);
		break;
	case BGBCC_RIL3OP_INITVARVAL:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackInitVarValue(ctx, s0);
		break;

	case BGBCC_RIL3OP_JMP:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_CompileJmp(ctx, lbl);
		break;
	case BGBCC_RIL3OP_JMPT:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_CompileJmpTrue(ctx, lbl);
		break;
	case BGBCC_RIL3OP_JMPF:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_CompileJmpFalse(ctx, lbl);
		break;
	case BGBCC_RIL3OP_JCMP:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		switch(i0)
		{
		case CCXL_CMP_EQ: s0="=="; break;
		case CCXL_CMP_NE: s0="!="; break;
		case CCXL_CMP_LT: s0="<"; break;
		case CCXL_CMP_GT: s0=">"; break;
		case CCXL_CMP_LE: s0="<="; break;
		case CCXL_CMP_GE: s0=">="; break;
		}

		BGBCC_CCXL_CompileJmpCond(ctx, s0, lbl);
		break;
	case BGBCC_RIL3OP_LABEL:
		lbl=BGBCC_CCXLR3_ReadLabel(ctx, &cs);
		BGBCC_CCXL_EmitLabel(ctx, lbl);
		break;

	case BGBCC_RIL3OP_STKFN:
		s0=BGBCC_CCXLR3_ReadString(ctx, &cs);
		BGBCC_CCXL_StackFn(ctx, s0);
		break;
	case BGBCC_RIL3OP_STKLN:
		i0=BGBCC_CCXLR3_ReadSVLI(ctx, &cs);
		BGBCC_CCXL_StackLn(ctx, i0);
		break;

	case BGBCC_RIL3OP_ADD:	BGBCC_CCXL_StackBinaryOp(ctx, "+"); break;
	case BGBCC_RIL3OP_SUB:	BGBCC_CCXL_StackBinaryOp(ctx, "-"); break;
	case BGBCC_RIL3OP_MUL:	BGBCC_CCXL_StackBinaryOp(ctx, "*"); break;
	case BGBCC_RIL3OP_AND:	BGBCC_CCXL_StackBinaryOp(ctx, "&"); break;
	case BGBCC_RIL3OP_OR:	BGBCC_CCXL_StackBinaryOp(ctx, "|"); break;
	case BGBCC_RIL3OP_XOR:	BGBCC_CCXL_StackBinaryOp(ctx, "^"); break;
	case BGBCC_RIL3OP_SHL:	BGBCC_CCXL_StackBinaryOp(ctx, "<<"); break;
	case BGBCC_RIL3OP_SAR:	BGBCC_CCXL_StackBinaryOp(ctx, ">>"); break;

	default:
		__debugbreak();
		break;
	}
	
	*rcs=cs;
}

void BGBCC_CCXLR3_LoadBufferRIL(
	BGBCC_TransState *ctx, byte *buf, int bufsz)
{
	byte *cs;
	byte *cse;

	if(!buf)
		return;
	
	BGBCC_CCXLR3_CheckLzWindow(ctx);
	
	cs=buf; cse=buf+bufsz;
	cs+=8;
	while(cs<cse)
		{ BGBCC_CCXLR3_DecodeBufCmd(ctx, &cs); }
}
