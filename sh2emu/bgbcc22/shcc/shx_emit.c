#include <bgbccc.h>

BGBCC_SHX_Context *BGBCC_SHX_AllocContext()
{
	BGBCC_SHX_Context *tmp;
	
	tmp=bgbcc_malloc(sizeof(BGBCC_SHX_Context));
	return(tmp);
}

int BGBCC_SHX_SetBeginSimPass(BGBCC_SHX_Context *ctx)
{
	int i;

	for(i=0; i<16; i++)
	{
		ctx->sec_vpos[i]=ctx->sec_pos[i];
	}
	ctx->nvlbl=ctx->nlbl;
	ctx->nvlbln=ctx->nlbln;

	ctx->is_simpass=1;
	return(0);
}

int BGBCC_SHX_SetEndSimPass(BGBCC_SHX_Context *ctx)
{
	ctx->is_simpass=0;
	ctx->sim_voffs=0;
	return(0);
}

int BGBCC_SHX_SetSectionName(BGBCC_SHX_Context *ctx, char *name)
{
	int sec;
	
	sec=-1;
	if(!strcmp(name, ".text"))		sec=BGBCC_SH_CSEG_TEXT;
	if(!strcmp(name, ".data"))		sec=BGBCC_SH_CSEG_DATA;
	if(!strcmp(name, ".bss"))		sec=BGBCC_SH_CSEG_BSS;
	if(!strcmp(name, ".got"))		sec=BGBCC_SH_CSEG_GOT;
	if(!strcmp(name, ".strtab"))	sec=BGBCC_SH_CSEG_STRTAB;
	if(!strcmp(name, ".reloc"))		sec=BGBCC_SH_CSEG_RELOC;
	
	if(sec>=0)
	{
		BGBCC_SHX_SetSection(ctx, sec);
		if(!ctx->sec_name[sec])
			ctx->sec_name[sec]=bgbcc_strdup(name);
		return(0);
	}

	for(sec=BGBCC_SH_CSEG_DYN; sec<ctx->nsec; sec++)
	{
		if(!strcmp(name, ctx->sec_name[sec]))	
		{
			BGBCC_SHX_SetSection(ctx, sec);
			return(0);
		}
	}

	sec=ctx->nsec++;
	if(sec<BGBCC_SH_CSEG_DYN)sec=BGBCC_SH_CSEG_DYN;
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
	int sz, ofs, vofs;

	if(!ctx->sec_buf[ctx->sec])
	{
//		sz=4096;
//		sz=1<<18;
		sz=1<<20;
		buf=bgbcc_malloc(sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf;
		ctx->sec_vpos[ctx->sec]=buf;
	}
	
//	if((ctx->sec_pos[ctx->sec]+1)>=ctx->sec_end[ctx->sec])
	if(((ctx->sec_pos[ctx->sec]+1)>=ctx->sec_end[ctx->sec]) ||
		((ctx->sec_vpos[ctx->sec]+1)>=ctx->sec_end[ctx->sec]))
	{
		buf=ctx->sec_buf[ctx->sec];
		sz=ctx->sec_end[ctx->sec]-buf;
		ofs=ctx->sec_pos[ctx->sec]-buf;
		vofs=ctx->sec_vpos[ctx->sec]-buf;
		sz=sz+(sz>>1);
		buf=bgbcc_realloc(buf, sz);
		ctx->sec_buf[ctx->sec]=buf;
		ctx->sec_end[ctx->sec]=buf+sz;
		ctx->sec_pos[ctx->sec]=buf+ofs;
		ctx->sec_vpos[ctx->sec]=buf+vofs;
	}

	if(ctx->is_simpass)
	{
		*ctx->sec_vpos[ctx->sec]++=val;
		return(0);
	}

	*ctx->sec_pos[ctx->sec]++=val;
	return(0);
}

int BGBCC_SHX_EmitWord(BGBCC_SHX_Context *ctx, int val)
{
//	if(!(val&0xFFFF) && (ctx->sec==BGBCC_SH_CSEG_TEXT))
//	{
//		BGBCC_DBGBREAK
//	}

	if(ctx->is_le)
	{
		BGBCC_SHX_EmitByte(ctx, val   );
		BGBCC_SHX_EmitByte(ctx, val>>8);
		return(0);
	}else
	{
		BGBCC_SHX_EmitByte(ctx, val>>8);
		BGBCC_SHX_EmitByte(ctx, val   );
		return(0);
	}
}

int BGBCC_SHX_EmitDWord(BGBCC_SHX_Context *ctx, u32 val)
{
	if(ctx->is_le)
	{
		BGBCC_SHX_EmitByte(ctx, val    );
		BGBCC_SHX_EmitByte(ctx, val>> 8);
		BGBCC_SHX_EmitByte(ctx, val>>16);
		BGBCC_SHX_EmitByte(ctx, val>>24);
		return(0);
	}else
	{
		BGBCC_SHX_EmitByte(ctx, val>>24);
		BGBCC_SHX_EmitByte(ctx, val>>16);
		BGBCC_SHX_EmitByte(ctx, val>> 8);
		BGBCC_SHX_EmitByte(ctx, val    );
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
	
	s=(byte *)str;
	while(*s)
		BGBCC_SHX_EmitByte(ctx, *s++);
	return(0);
}

int BGBCC_SHX_EmitStringUCS2(BGBCC_SHX_Context *ctx, u16 *str)
{
	u16 *s;
	
	s=str;
	while(*s)
		BGBCC_SHX_EmitWord(ctx, *s++);
	BGBCC_SHX_EmitWord(ctx, *s++);
	return(0);
}

int BGBCC_SHX_EmitGetStrtabLabel(BGBCC_SHX_Context *ctx, char *str)
{
	char *s0;
	int sn0;
	int i, j, k;
	
	if(!ctx->lblstr_ofs)
	{
		i=4096;
		ctx->lblstr_ofs=bgbcc_malloc(i*sizeof(u32));
		ctx->lblstr_id=bgbcc_malloc(i*sizeof(u32));
		ctx->nlblstr=0;
		ctx->mlblstr=i;
	}

#if 1
	for(i=0; i<ctx->nlblstr; i++)
	{
		s0=(char *)(ctx->sec_buf[BGBCC_SH_CSEG_STRTAB]+ctx->lblstr_ofs[i]);
		if(!strcmp(s0, str))
			{ return(ctx->lblstr_id[i]); }
	}
#endif

#if 0
	for(i=0; i<ctx->nlbl; i++)
	{
		if(ctx->lbl_sec[i]==BGBCC_SH_CSEG_STRTAB)
		{
			s0=(char *)(ctx->sec_buf[BGBCC_SH_CSEG_STRTAB]+ctx->lbl_ofs[i]);
			if(!strcmp(s0, str))
			{
				return(ctx->lbl_id[i]);
			}
		}
	}
#endif

	if(ctx->is_simpass)
	{
		/* Keep symbol labels aligned */
		ctx->nvlbl++;

#if 0
		sn0=ctx->sec;
		k=BGBCC_SHX_GenLabel(ctx);
		BGBCC_SHX_SetSectionName(ctx, ".strtab");
		i=BGBCC_SHX_EmitLabel(ctx, k);
		BGBCC_SHX_EmitString(ctx, str);
		ctx->sec=sn0;
#endif

		return(0);
	}

	sn0=ctx->sec;
	k=BGBCC_SHX_GenLabel(ctx);
	BGBCC_SHX_SetSectionName(ctx, ".strtab");
	i=BGBCC_SHX_EmitLabel(ctx, k);
	BGBCC_SHX_EmitString(ctx, str);
	ctx->sec=sn0;
	
	j=ctx->nlblstr++;
	ctx->lblstr_ofs[j]=ctx->lbl_ofs[i];
	ctx->lblstr_id[j]=k;
	
	return(k);
}

int BGBCC_SHX_EmitGetStrtabLabelUTF2ASCII(
	BGBCC_SHX_Context *ctx, char *str)
{
	char tb[4096];
	char *s, *t;
	int i, j, k;
	
	s=str;
	t=tb;
	while(*s)
	{
		k=BGBCP_ParseChar(&s);
		*t++=k;
	}
	*t++=0;
	
	i=BGBCC_SHX_EmitGetStrtabLabel(ctx, tb);
	return(i);
}

int BGBCC_SHX_EmitGetStrtabLabelUCS2(BGBCC_SHX_Context *ctx, u16 *str)
{
	u16 *s0;
	int sn0;
	int i, j, k;
	
	for(i=0; i<ctx->nlbl; i++)
	{
		if(ctx->lbl_sec[i]==BGBCC_SH_CSEG_STRTAB)
		{
			j=ctx->lbl_ofs[i];
			if(j&1)continue;
			s0=(u16 *)(ctx->sec_buf[BGBCC_SH_CSEG_STRTAB]+j);
			if(!BGBCP_StrcmpUCS2(s0, str))
			{
				return(ctx->lbl_id[i]);
			}
		}
	}
	
	sn0=ctx->sec;
	k=BGBCC_SHX_GenLabel(ctx);
	BGBCC_SHX_SetSectionName(ctx, ".strtab");
	BGBCC_SHX_EmitBAlign(ctx, 2);
	BGBCC_SHX_EmitLabel(ctx, k);
	BGBCC_SHX_EmitStringUCS2(ctx, str);
	ctx->sec=sn0;
	return(k);
}

int BGBCC_SHX_EmitGetStrtabLabelUTF2UCS2(
	BGBCC_SHX_Context *ctx, char *str)
{
	u16 tb[4096];
	char *s;
	u16 *t;
	int i, j, k;
	
	s=str;
	t=tb;
	while(*s)
	{
		k=BGBCP_ParseChar(&s);
		*t++=k;
	}
	*t++=0;
	
	i=BGBCC_SHX_EmitGetStrtabLabelUCS2(ctx, tb);
	return(i);
}


int BGBCC_SHX_EmitGetStrtabSecOfs(BGBCC_SHX_Context *ctx, char *str)
{
	int l0;
	int i;

	l0=BGBCC_SHX_EmitGetStrtabLabel(ctx, str);
	
	for(i=0; i<ctx->nlbl; i++)
		if(ctx->lbl_id[i]==l0)
			return(ctx->lbl_ofs[i]);
	return(-1);
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
		if((ctx->sec!=BGBCC_SH_CSEG_BSS) &&
				(ctx->sec!=BGBCC_SH_CSEG_DATA))
			{ n=-1; }
	
		n=sz;
		while(n--)
			BGBCC_SHX_EmitByte(ctx, 0);
	}
	return(0);
}

byte *BGBCC_SHX_EmitGetPos(BGBCC_SHX_Context *ctx)
{
	if(ctx->is_simpass)
	{
		return(ctx->sec_vpos[ctx->sec]);
	}

	return(ctx->sec_pos[ctx->sec]);
}

int BGBCC_SHX_EmitGetOffs(BGBCC_SHX_Context *ctx)
{
	if(ctx->is_simpass)
	{
		return(ctx->sec_vpos[ctx->sec]-ctx->sec_buf[ctx->sec]);
	}

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

int BGBCC_SHX_EmitGetOffsDWord(BGBCC_SHX_Context *ctx, int ofs)
{
	byte *ptr;
	ptr=BGBCC_SHX_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24));
	}else
	{
		return((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|ptr[3]);
	}
}

int BGBCC_SHX_EmitSetOffsWord(BGBCC_SHX_Context *ctx, int ofs, int val)
{
	byte *ptr;

//	if(!(val&0xFFFF) && (ctx->sec==BGBCC_SH_CSEG_TEXT))
//	{
//		BGBCC_DBGBREAK
//	}

	if(ctx->is_simpass)
		return(0);

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

int BGBCC_SHX_EmitSetOffsDWord(BGBCC_SHX_Context *ctx, int ofs, int val)
{
	byte *ptr;

	if(ctx->is_simpass)
		return(0);

	ptr=BGBCC_SHX_EmitGetPosForOffs(ctx, ofs);
	if(ctx->is_le)
	{
		ptr[0]=val    ;
		ptr[1]=val>> 8;
		ptr[2]=val>>16;
		ptr[3]=val>>24;
		return(0);
	}else
	{
		ptr[0]=val>>24;
		ptr[1]=val>>16;
		ptr[2]=val>> 8;
		ptr[3]=val    ;
		return(0);
	}
}

char *bgbcc_shx_srcidx[256];
int bgbcc_shx_nsrcidx=0;

int bgbcc_shx_indexSrcIdx(char *name)
{
	int i, j, k;

	for(i=0; i<bgbcc_shx_nsrcidx; i++)
	{
		if(!strcmp(name, bgbcc_shx_srcidx[i]))
			return(i);
	}
	
	i=bgbcc_shx_nsrcidx++;
	bgbcc_shx_srcidx[i]=name;
	return(i);
}

int BGBCC_SHX_GenLabelTempLLn(BGBCC_SHX_Context *ctx, char *file, int line)
{
	int i, j, k;

	i=ctx->lbltrov++;
	
	if(!ctx->genlabel_srcpos)
	{
		ctx->genlabel_srcpos=ctx->t_genlabel_srcpos;
		ctx->genlabel_limit=4096;
	}
	
	j=bgbcc_shx_indexSrcIdx(file);
	k=(j<<16)|line;
	if(ctx->is_simpass)
		k|=0x8000;
	if(i<ctx->genlabel_limit)
		ctx->genlabel_srcpos[i]=k;
	
	return(CCXL_LBL_GENSYM2BASE2+i);
}

int BGBCC_SHX_GenLabelLLn(BGBCC_SHX_Context *ctx, char *file, int line)
{
//	int i;
//	return(BGBCC_SHX_GenLabelTempLLn(ctx, file, line));

	int i, j, k;

	i=ctx->lblrov++;
	
	if(!ctx->genlabel_srcpos)
	{
		ctx->genlabel_srcpos=ctx->t_genlabel_srcpos;
		ctx->genlabel_limit=4096;
	}
	
	j=bgbcc_shx_indexSrcIdx(file);
	k=(j<<16)|line;
	if(ctx->is_simpass)
		k|=0x8000;
	if(i<ctx->genlabel_limit)
		ctx->genlabel_srcpos[i]=k;
	
	return(CCXL_LBL_GENSYM2BASE+i);
}

int BGBCC_SHX_EmitLabel(BGBCC_SHX_Context *ctx, int lblid)
{
	int h;
	int i;
	
	if(lblid<=0)
		return(0);

	if(!ctx->lbl_ofs)
	{
		for(i=0; i<1024; i++)
			ctx->lbl_hash[i]=(-1);
	
//		i=4096;
		i=65536;
		ctx->lbl_ofs=bgbcc_malloc(i*sizeof(u32));
		ctx->lbl_id =bgbcc_malloc(i*sizeof(u32));
		ctx->lbl_sec=bgbcc_malloc(i*sizeof(byte));
		ctx->lbl_chn=bgbcc_malloc(i*sizeof(s32));
		ctx->nlbl=1;
		ctx->nvlbl=1;
		ctx->mlbl=i;
	}
	
//	if((ctx->nlbl+1)>=ctx->mlbl)
	if(((ctx->nlbl+1)>=ctx->mlbl) ||
		((ctx->nvlbl+1)>=ctx->mlbl))
	{
		i=ctx->mlbl+(ctx->mlbl>>1);
		ctx->lbl_ofs=bgbcc_realloc(ctx->lbl_ofs, i*sizeof(u32));
		ctx->lbl_id =bgbcc_realloc(ctx->lbl_id , i*sizeof(u32));
		ctx->lbl_sec=bgbcc_realloc(ctx->lbl_sec, i*sizeof(byte));
		ctx->lbl_chn=bgbcc_realloc(ctx->lbl_chn, i*sizeof(s32));
		ctx->mlbl=i;
	}

	if(ctx->is_simpass)
	{
		i=ctx->nvlbl++;

		if(ctx->lbl_id[i]==lblid)
		{
			ctx->sim_voffs=BGBCC_SHX_EmitGetOffs(ctx)-ctx->lbl_ofs[i];
			if((ctx->sim_voffs!=0) && ctx->is_stable)
				ctx->is_stable|=2;
		}else
		{
			ctx->is_stable|=6;
		}

		ctx->lbl_id[i]=lblid;
		ctx->lbl_ofs[i]=BGBCC_SHX_EmitGetOffs(ctx);
		ctx->lbl_sec[i]=ctx->sec;
		return(i);
	}

	i=ctx->nlbl;
	if(ctx->lbl_id[i]==lblid)
	{
		ctx->sim_voffs=BGBCC_SHX_EmitGetOffs(ctx)-ctx->lbl_ofs[i];
		if((ctx->sim_voffs!=0) && ctx->is_stable)
			ctx->is_stable|=2;
	}

	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	i=ctx->nlbl++;
	ctx->lbl_id[i]=lblid;
	ctx->lbl_ofs[i]=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->lbl_sec[i]=ctx->sec;
	
	ctx->lbl_chn[i]=ctx->lbl_hash[h];
	ctx->lbl_hash[h]=i;
	
	if((ctx->lbl_ofs[i]>>20) && (ctx->sec!=BGBCC_SH_CSEG_BSS))
		{ BGBCC_DBGBREAK }
	return(i);
}

char *BGBCC_SHX_LookupNameForLabel(BGBCC_SHX_Context *ctx, int lblid)
{
	int i;
	
	for(i=0; i<ctx->nlbln; i++)
	{
		if(ctx->lbln_id[i]==lblid)
			return(ctx->lbln_name[i]);
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);
	}
	return(NULL);
}

int BGBCC_SHX_LookupNamedLabel(BGBCC_SHX_Context *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->nlbln; i++)
	{
		if(!strcmp(ctx->lbln_name[i], name))
			return(ctx->lbln_id[i]);
	}
	return(0);
}

int BGBCC_SHX_GetNamedLabel(BGBCC_SHX_Context *ctx, char *name)
{
	int lbl;
	int i;

	lbl=BGBCC_SHX_LookupNamedLabel(ctx, name);
	if(lbl>0)
		return(lbl);
	
	if(!ctx->lbln_name)
	{
//		i=1024;
		i=4096;
		ctx->lbln_name=bgbcc_malloc(i*sizeof(char *));
		ctx->lbln_id =bgbcc_malloc(i*sizeof(u32));
		ctx->nlbln=0;
		ctx->mlbln=i;
	}
	
	if((ctx->nlbln+1)>=ctx->mlbln)
	{
		i=ctx->mlbln+(ctx->mlbln>>1);
		ctx->lbln_name=bgbcc_realloc(ctx->lbln_name, i*sizeof(char *));
		ctx->lbln_id =bgbcc_realloc(ctx->lbln_id , i*sizeof(u32));
		ctx->mlbln=i;
	}

	if(ctx->is_simpass)
	{
		return(-1);
	}

	lbl=BGBCC_SHX_GenLabel(ctx);
	i=ctx->nlbln++;
	ctx->lbln_id[i]=lbl;
	ctx->lbln_name[i]=bgbcc_strdup(name);
	return(lbl);
}

int BGBCC_SHX_EmitNamedLabel(BGBCC_SHX_Context *ctx, char *name)
{
	int lbl;
	
	lbl=BGBCC_SHX_GetNamedLabel(ctx, name);
	if(lbl>0)
	{
		return(BGBCC_SHX_EmitLabel(ctx, lbl));
	}
	return(0);
}

int BGBCC_SHX_EmitNamedGlobal(BGBCC_SHX_Context *ctx, char *name)
{
	return(0);
}

int BGBCC_SHX_EmitCommSym(BGBCC_SHX_Context *ctx, int lblid, int sz)
{
	int i;

	for(i=0; i<ctx->nlbl; i++)
		if(ctx->lbl_id[i]==lblid)
			return(0);

	if(sz>>16)
		i=-1;

	i=ctx->sec;
	BGBCC_SHX_SetSectionName(ctx, ".bss");
	BGBCC_SHX_EmitBAlign(ctx, 4);
	BGBCC_SHX_EmitLabel(ctx, lblid);
	BGBCC_SHX_EmitRawBytes(ctx, NULL, sz);
	ctx->sec=i;
	return(1);
}

int BGBCC_SHX_EmitNamedCommSym(BGBCC_SHX_Context *ctx, char *name, int sz)
{
	int lbl;
	
	lbl=BGBCC_SHX_LookupNamedLabel(ctx, name);
	return(BGBCC_SHX_EmitCommSym(ctx, lbl, sz));

//	BGBCC_SHX_SetSectionName(sctx, ".bss");
//	BGBCC_SHX_EmitBAlign(sctx, 4);
//	BGBCC_SHX_EmitNamedLabel(sctx, name);
//	BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
//	return(1);
}

int BGBCC_SHX_EmitRelocTy(BGBCC_SHX_Context *ctx, int lblid, int ty)
{
	int i;
	
	if(lblid<=0)
		return(0);

	if(!ctx->rlc_ofs)
	{
//		i=4096;
		i=65536;
		ctx->rlc_ofs=bgbcc_malloc(i*sizeof(u32));
		ctx->rlc_id =bgbcc_malloc(i*sizeof(u32));
		ctx->rlc_sec=bgbcc_malloc(i*sizeof(byte));
		ctx->rlc_ty =bgbcc_malloc(i*sizeof(byte));
		ctx->nrlc=0;
		ctx->mrlc=i;
	}
	
	if((ctx->nrlc+1)>=ctx->mrlc)
	{
		i=ctx->mrlc+(ctx->mrlc>>1);
		ctx->rlc_ofs=bgbcc_realloc(ctx->rlc_ofs, i*sizeof(u32));
		ctx->rlc_id =bgbcc_realloc(ctx->rlc_id , i*sizeof(u32));
		ctx->rlc_sec=bgbcc_realloc(ctx->rlc_sec, i*sizeof(byte));
		ctx->rlc_ty =bgbcc_realloc(ctx->rlc_ty , i*sizeof(byte));
		ctx->mrlc=i;
	}

	if(ctx->is_simpass)
	{
		return(0);
	}

	i=ctx->nrlc++;
	ctx->rlc_id[i]=lblid;
	ctx->rlc_ofs[i]=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->rlc_sec[i]=ctx->sec;
	ctx->rlc_ty[i]=ty;
	return(1);
}

int BGBCC_SHX_EmitNamedReloc(BGBCC_SHX_Context *ctx, char *name, int ty)
{
	int lbl;
	
	lbl=BGBCC_SHX_GetNamedLabel(ctx, name);
	if(lbl>0)
	{
		return(BGBCC_SHX_EmitRelocTy(ctx, lbl, ty));
	}
	return(0);
}

int BGBCC_SHX_EmitRelocRel8(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL8)); }
int BGBCC_SHX_EmitRelocRel16(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL16)); }
int BGBCC_SHX_EmitRelocRel32(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_REL32)); }

int BGBCC_SHX_EmitRelocAbs8(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS8)); }
int BGBCC_SHX_EmitRelocAbs16(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS16)); }
int BGBCC_SHX_EmitRelocAbs32(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS32)); }
int BGBCC_SHX_EmitRelocAbs64(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_ABS64)); }

int BGBCC_SHX_EmitRelocRelW12(BGBCC_SHX_Context *ctx, int lbl)
	{ return(BGBCC_SHX_EmitRelocTy(ctx, lbl, BGBCC_SH_RLC_RELW12)); }

int BGBCC_SHX_EmitCheckRegSafe(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int vs;

	if(ctx->is_simpass || ctx->is_rawasm)
		return(0);

//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
	if(((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0) ||
		((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0))
	{
		if((reg&15)>=8)
		{
			if((reg&15)==15)
				return(0);
		
//			vs=ctx->reg_save|ctx->reg_vsave;
			vs=ctx->reg_save;
			if(!(vs&(1<<(reg&15))))
				{ BGBCC_DBGBREAK; }
			return(1);
		}
	}

	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
	{
		if((reg&15)>=12)
		{
//			vs=ctx->freg_save|ctx->freg_vsave;
			vs=ctx->freg_save;
			if(!(vs&(1<<(reg&15))))
				{ BGBCC_DBGBREAK; }
			return(1);
		}
	}

	return(0);
}

int BGBCC_SHX_EmitOpNone(BGBCC_SHX_Context *ctx, int nmid)
{
	if(!BGBCC_SHX_TryEmitOpNone(ctx, nmid))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpNone(BGBCC_SHX_Context *ctx, int nmid)
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
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

/** Registers which can be used as a normal GPR */
int BGBCC_SHX_EmitCheckRegGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	return(0);
}

/** Registers which can be used as an address */
int BGBCC_SHX_EmitCheckRegAddrGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);
//	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
//		return(1);
	return(0);
}

/** Registers which can be used as an extended GPR */
int BGBCC_SHX_EmitCheckRegExtGPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_R0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_LR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RH0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)
		return(1);
	return(0);
}

int BGBCC_SHX_EmitCheckRegFPR(BGBCC_SHX_Context *ctx, int reg)
{
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
		return(1);
	if((reg&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_DR0)
		return(1);
	return(0);
}

int BGBCC_SHX_EmitOpReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_SHX_TryEmitOpReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int opw, opw2;

//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ reg=BGBCC_SH_REG_FR0+(2*(reg&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

#if 1
	switch(nmid)
	{
	case BGBCC_SH_NMID_FSTS:	case BGBCC_SH_NMID_FLDS:
	case BGBCC_SH_NMID_FLOAT:	case BGBCC_SH_NMID_FTRC:
	case BGBCC_SH_NMID_FNEG:	case BGBCC_SH_NMID_FABS:
	case BGBCC_SH_NMID_FSQRT:	case BGBCC_SH_NMID_FSRRA:
	case BGBCC_SH_NMID_FCNVSD:	case BGBCC_SH_NMID_FCNVDS:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if(reg&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FMOV:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_SZ)
		{
			if(reg&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FLDI0:
	case BGBCC_SH_NMID_FLDI1:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
			{ BGBCC_DBGBREAK }
		break;
	}
#endif

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BSRF:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0003|((reg&15)<<8); break;
	case BGBCC_SH_NMID_BRAF:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0023|((reg&15)<<8); break;

	case BGBCC_SH_NMID_MOVT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x0029|((reg&15)<<8); break;
	case BGBCC_SH_NMID_MOVRT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x0039|((reg&15)<<8); break;

	case BGBCC_SH_NMID_CMPEQ:	/* pseudo-op */
	case BGBCC_SH_NMID_TST:		/* pseudo-op */
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x2008|((reg&15)<<8)|((reg&15)<<4); break;

	case BGBCC_SH_NMID_SHLL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4000|((reg&15)<<8); break;
	case BGBCC_SH_NMID_DT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4010|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHAL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4020|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4001|((reg&15)<<8); break;
	case BGBCC_SH_NMID_CMPPZ:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4011|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHAR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4021|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4004|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ROTCL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4024|((reg&15)<<8); break;

	case BGBCC_SH_NMID_ROTR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4005|((reg&15)<<8); break;
	case BGBCC_SH_NMID_CMPPL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4015|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ROTCR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4025|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLL2:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4008|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLL8:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4018|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLL16:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4028|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLR2:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4009|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLR8:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4019|((reg&15)<<8); break;
	case BGBCC_SH_NMID_SHLR16:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x4029|((reg&15)<<8); break;

	case BGBCC_SH_NMID_SHLL1:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x300C|((reg&15)<<8)|((reg&15)<<4);			break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x400B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_TASB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x401B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x402B|((reg&15)<<8); break;

	case BGBCC_SH_NMID_FSTS:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF00D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDS:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF01D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLOAT:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF02D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FTRC:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF03D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FNEG:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF04D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FABS:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF05D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FSQRT:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF06D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FSRRA:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF07D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDI0:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF08D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FLDI1:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF09D|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FCNVSD:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF0AD|((reg&15)<<8); break;
	case BGBCC_SH_NMID_FCNVDS:
//		if((reg&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)		break;
		opw=0xF0BD|((reg&15)<<8); break;

	case BGBCC_SH_NMID_PUSH:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(ctx->has_bjx1jmp)
			{ opw=0x4048|((reg&15)<<8); }
		else
			{ opw=0x2F06|((reg&15)<<4); }
		break;
	case BGBCC_SH_NMID_POP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		if(ctx->has_bjx1jmp)
			{ opw=0x4058|((reg&15)<<8); }
		else
			{ opw=0x60F6|((reg&15)<<8); }
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpImm(BGBCC_SHX_Context *ctx, int nmid, int imm)
{
	if(!BGBCC_SHX_TryEmitOpImm(ctx, nmid, imm))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpImm(BGBCC_SHX_Context *ctx, int nmid, int imm)
{
	int opw, opw2;

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_TRAPA:
		if(((byte)imm)!=imm)
			break;
		opw=0xC300|imm; break;

	case BGBCC_SH_NMID_PSETMD4:
		if(!ctx->has_bjx1ari)	break;
		if((imm&15)!=imm)		break;
		opw=0xF0DD|((imm&15)<<8);
		break;
	case BGBCC_SH_NMID_PSETMD12:
		if(!ctx->has_bjx1ari)	break;
		if((imm&4095)!=imm)		break;
		opw=0x8E00|((imm>>4)&255);
		opw2=0xF0DD|((imm&15)<<8);
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpMReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	if(!BGBCC_SHX_TryEmitOpMReg(ctx, nmid, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpMReg(BGBCC_SHX_Context *ctx, int nmid, int reg)
{
	int opw, opw2;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_PREF:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0083|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBI:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x0093|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x00A3|((reg&15)<<8); break;
	case BGBCC_SH_NMID_OCBWB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x00B3|((reg&15)<<8); break;
	case BGBCC_SH_NMID_ICBI:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x00E3|((reg&15)<<8); break;

	case BGBCC_SH_NMID_JSR:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x400B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_TASB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0x401B|((reg&15)<<8); break;
	case BGBCC_SH_NMID_JMP:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, reg))	break;
		opw=0x402B|((reg&15)<<8); break;

	default:
		break;
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLblReg(BGBCC_SHX_Context *ctx,
	int nmid, int lbl, int reg)
{
	if(!BGBCC_SHX_TryEmitOpLblReg(ctx, nmid, lbl, reg))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLblReg(BGBCC_SHX_Context *ctx,
	int nmid, int lbl, int reg)
{
	int opw, opw2, rlty;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	opw=-1; opw2=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		rlty=BGBCC_SH_RLC_RELW8L;
		opw=0xD000|((reg&15)<<8);
		break;
	case BGBCC_SH_NMID_MOVA:
		if(reg!=BGBCC_SH_REG_R0)
			break;
		rlty=BGBCC_SH_RLC_RELW8L;
		opw=0xC700;
		break;
	}

	if(opw>=0)
	{
		if(rlty>0)
			{ BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}

	return(0);
}

int BGBCC_SHX_EmitOpLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, rlty;

	opw=-1; opw2=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000; break;
	case BGBCC_SH_NMID_BSR:
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xB000; break;

	case BGBCC_SH_NMID_BT:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8900; break;
	case BGBCC_SH_NMID_BF:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8B00; break;
	case BGBCC_SH_NMID_BTS:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8D00; break;
	case BGBCC_SH_NMID_BFS:
		rlty=BGBCC_SH_RLC_RELW8;
		opw=0x8F00; break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(rlty>0)
			{ BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpNear12Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpNear12Label(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpNear12Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, opw3, rlty;

	opw=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000; break;
	case BGBCC_SH_NMID_BSR:
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xB000; break;

	case BGBCC_SH_NMID_BT:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8900;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8B01;
		opw3=0x0009;
		break;
	case BGBCC_SH_NMID_BF:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8B00;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8901;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BTS:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8D00;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8D00;
		break;
	case BGBCC_SH_NMID_BFS:
		if(ctx->has_bjx1jmp)
		{
			rlty=BGBCC_SH_RLC_RELW16_BJX;
			opw=0x8E00;
			opw3=0x8F00;
			break;
		}
		rlty=BGBCC_SH_RLC_RELW12;
		opw=0xA000;
		opw2=0x8900;
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);

		if(rlty>0)
			{ BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty); }
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw3>=0)
			BGBCC_SHX_EmitWord(ctx, opw3);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpFarLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpFarLabel(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpFarLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, opw3, rlty;
	int tr0;

	opw=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAF:
		opw=0x0023;
//		opw2=0xA002;
		break;
	case BGBCC_SH_NMID_BSR:
	case BGBCC_SH_NMID_BSRF:
		opw=0x0003;
		break;

	case BGBCC_SH_NMID_BT:
		opw=0x0023;
		opw2=0x8B02;
		opw3=0x0009;
		break;
	case BGBCC_SH_NMID_BF:
		opw=0x0023;
		opw2=0x8902;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BTS:
		opw=0x0023;
		opw2=0x8B01;
		break;
	case BGBCC_SH_NMID_BFS:
		opw=0x0023;
		opw2=0x8901;
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);

		tr0=BGBCC_SHXC_ScratchAllocReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
//		BGBCC_SHX_EmitLoadRegLabelRel(ctx, BGBCC_SH_REG_R0, lbl, 6);
		BGBCC_SHX_EmitLoadRegLabelRel(ctx, tr0, lbl, 6);
		opw|=(tr0<<8)&0x0F00;
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHXC_ScratchReleaseReg(NULL, ctx, tr0);

		if(opw3>=0)
			BGBCC_SHX_EmitWord(ctx, opw3);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpFar16Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	if(!BGBCC_SHX_TryEmitOpFar16Label(ctx, nmid, lbl))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpFar16Label(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int opw, opw2, opw3, rlty;
	int tr0;

	if(ctx->has_bjx1jmp)
	{
		if((nmid==BGBCC_SH_NMID_BT) ||
			(nmid==BGBCC_SH_NMID_BF) ||
			(nmid==BGBCC_SH_NMID_BTS) ||
			(nmid==BGBCC_SH_NMID_BFS))
		{
			return(BGBCC_SHX_TryEmitOpNear12Label(ctx, nmid, lbl));
		}
		
		opw=-1; opw2=-1; opw3=-1; rlty=-1;
		if(nmid==BGBCC_SH_NMID_BRA)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw=0x8E00;
			opw3=0xA000;
		}else if(nmid==BGBCC_SH_NMID_BSR)
		{
			rlty=BGBCC_SH_RLC_RELW20_BJX;
			opw=0x8E00;
			opw3=0xB000;
		}

		if(opw>=0)
		{
			if(opw2>=0)
				BGBCC_SHX_EmitWord(ctx, opw2);

			if(rlty>0)
				{ BGBCC_SHX_EmitRelocTy(ctx, lbl, rlty); }
			BGBCC_SHX_EmitWord(ctx, opw);
			if(opw3>=0)
				BGBCC_SHX_EmitWord(ctx, opw3);
			return(1);
		}
	}

	opw=-1; opw2=-1; opw3=-1; rlty=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_BRA:
	case BGBCC_SH_NMID_BRAF:
		opw=0x0023;
//		opw2=0xA002;
		break;
	case BGBCC_SH_NMID_BSR:
	case BGBCC_SH_NMID_BSRF:
		opw=0x0003;
		break;

	case BGBCC_SH_NMID_BT:
		opw=0x0023;
		opw2=0x8B02;
		opw3=0x0009;
		break;
	case BGBCC_SH_NMID_BF:
		opw=0x0023;
		opw2=0x8902;
		opw3=0x0009;
		break;

	case BGBCC_SH_NMID_BTS:
		opw=0x0023;
		opw2=0x8B01;
		break;
	case BGBCC_SH_NMID_BFS:
		opw=0x0023;
		opw2=0x8901;
		break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);

		tr0=BGBCC_SHXC_ScratchAllocReg(NULL, ctx, BGBCC_SH_REGCLS_GR);
//		BGBCC_SHX_EmitLoadRegLabelRel(ctx, BGBCC_SH_REG_R0, lbl, 6);
		BGBCC_SHX_EmitLoadRegLabelRel16(ctx, tr0, lbl, 6);
		opw|=(tr0<<8)&0x0F00;
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHXC_ScratchReleaseReg(NULL, ctx, tr0);

		if(opw3>=0)
			BGBCC_SHX_EmitWord(ctx, opw3);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_LookupLabelIndex(
	BGBCC_SHX_Context *sctx, int lblid)
{
	int c, h;
	int i, j, k;
	
	h=lblid*65521+31;
	h=h*65521+31;
	h=(h>>16)&1023;

	c=sctx->lbl_hash[h];
	while(c>=0)
	{
		if(sctx->lbl_id[c]==lblid)
			return(c);
		c=sctx->lbl_chn[c];
	}
	return(-1);

#if 0	
	for(i=0; i<sctx->nlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}
	return(-1);
#endif
}

int BGBCC_SHX_LookupSimLabelIndex(
	BGBCC_SHX_Context *sctx, int lblid)
{
	int i, j, k;
	
	for(i=sctx->nlbl; i<sctx->nvlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}

	i=BGBCC_SHX_LookupLabelIndex(sctx, lblid);
	return(i);
}

int BGBCC_SHX_EmitOpAutoLabel(BGBCC_SHX_Context *ctx, int nmid, int lbl)
{
	int i, j, k, rngb, rngw, rngw16, szrng;

	szrng=ctx->simfnnsz-ctx->simfnmsz;
	if(szrng<0)szrng=999999;

	i=BGBCC_SHX_LookupSimLabelIndex(ctx, lbl);
//	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec))
	if((i>=0) && (ctx->lbl_sec[i]==ctx->sec) && ctx->is_stable)
	{
		j=ctx->lbl_ofs[i];
		rngb=244;
//		rngw=2040;
		rngw=4080;
//		rngw16=32760;
//		rngw16=30720;
		rngw16=65280;
		if(i>=ctx->nlbl)
		{
//			j=j+ctx->sim_voffs;
//			j=j+ctx->sim_voffs+szrng;
			j=j+szrng;
//			rngb=44;
			rngb=88;
//			rngb=122;
//			rngb=144;
//			rngb=166;
//			rngw=480;
//			rngw=960;
			rngw=1536;
//			rngw=2040;
//			rngw16=24576;
			rngw16=65280-(szrng/2);
			
			rngb=244-(szrng/2);
//			rngw=2040-(szrng/2);
			rngw=4080-(szrng/2);
//			if(rngb<0)rngb=0;
//			if(rngw<0)rngw=0;
			
//			if(!ctx->need_n12jmp)
//				rngb=256;
//			if(!ctx->need_f16jmp)
//				rngw=4096;
//			if(!ctx->need_farjmp)
//				rngw16=65536;
			
//			if(!ctx->is_stable || (ctx->is_stable&2))
			if(!ctx->is_stable)
			{
				rngb=999999;
				rngw=999999999;
				rngw16=999999999;
				if(ctx->need_n12jmp)
					rngb=0;
				if(ctx->need_f16jmp)
					rngw=0;
				if(ctx->need_farjmp)
					rngw16=16;
			}
		}
			
//		k=ctx->sim_voffs; k=k^(k>>31);
//		rngb-=k;
//		rngw-=k;
		
//		if(k && ctx->is_stable)
//		if((k>8) && ctx->is_stable)
//			ctx->is_stable|=2;
		
//		if(ctx->is_simpass)
//			{ rngb=0; rngw=0; }

		k=BGBCC_SHX_EmitGetOffs(ctx);
//		j=k-j;
//		j=(k+4)-j;
		j=j-(k+4);
		if(j<0)j=-j;
		
//		if(j<104)
		if(j<rngb)
//		if(0)
		{
			return(BGBCC_SHX_EmitOpLabel(ctx, nmid, lbl));
		}

//		if(j<1984)
		if(j<rngw)
//		if(0)
		{
			return(BGBCC_SHX_EmitOpNear12Label(ctx, nmid, lbl));
//			return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
		}

		if(j<rngw16)
//		if(0)
		{
			return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
		}

		return(BGBCC_SHX_EmitOpFarLabel(ctx, nmid, lbl));
	}
	
//	return(BGBCC_SHX_EmitOpFarLabel(ctx, nmid, lbl));

	if(ctx->need_farjmp)
		return(BGBCC_SHX_EmitOpFarLabel(ctx, nmid, lbl));
	else if(ctx->need_f16jmp)
		return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
	else if(ctx->need_n12jmp)
	{
//		return(BGBCC_SHX_EmitOpFar16Label(ctx, nmid, lbl));
		return(BGBCC_SHX_EmitOpNear12Label(ctx, nmid, lbl));
	}
	else
	{
		i=BGBCC_SHX_EmitOpLabel(ctx, nmid, lbl);
//		if((nmid==BGBCC_SH_NMID_BRA) ||
//			(nmid==BGBCC_SH_NMID_BSR))
//				BGBCC_SHX_EmitWord(ctx, 0x0009);
		return(i);
	}
}

int BGBCC_SHX_EmitOpRegReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegReg(BGBCC_SHX_Context *ctx, int nmid, int rm, int rn)
{
	int opw, opw2;

	if((nmid==BGBCC_SH_NMID_MOV) && (rm==rn))
		return(1);

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }
//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

#if 1
	switch(nmid)
	{
	case BGBCC_SH_NMID_FADD:	case BGBCC_SH_NMID_FSUB:
	case BGBCC_SH_NMID_FMUL:	case BGBCC_SH_NMID_FDIV:
	case BGBCC_SH_NMID_FCMPEQ:	case BGBCC_SH_NMID_FCMPGT:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if((rm|rn)&1)
				{ BGBCC_DBGBREAK }
		}
		break;

//	case BGBCC_SH_NMID_FSTS:
	case BGBCC_SH_NMID_FLOAT:
	case BGBCC_SH_NMID_FCNVSD:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if(rn&1)
				{ BGBCC_DBGBREAK }
		}
		break;

//	case BGBCC_SH_NMID_FLDS:
	case BGBCC_SH_NMID_FTRC:
	case BGBCC_SH_NMID_FCNVDS:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
		{
			if(rm&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FMOV:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_SZ)
		{
			if((rm|rn)&1)
				{ BGBCC_DBGBREAK }
		}
		break;

	case BGBCC_SH_NMID_FMAC:
		if(ctx->cur_fpscr&BGBCC_SH_FPSCR_PR)
			{ BGBCC_DBGBREAK }
		break;
	}
#endif

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_STC:
	case BGBCC_SH_NMID_STS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((rm&0xF0)==0x10)
			{ opw=0x0002|((rn&15)<<8)|((rm&15)<<4); break; }
		if((rm&0xF0)==0x20)
			{ opw=0x000A|((rn&15)<<8)|((rm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_LDC:
	case BGBCC_SH_NMID_LDS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;

		if((rn&0xF0)==0x10)
			{ opw=0x400E|((rm&15)<<8)|((rn&15)<<4); break; }
		if((rn&0xF0)==0x20)
			{ opw=0x400A|((rm&15)<<8)|((rn&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MULL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_DIV0S:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x2007|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_TST:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x2008|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_AND:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x2009|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_XOR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x200A|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_OR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x200B|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_CMPSTR:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x200C|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_XTRCT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x200D|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MULUW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x200E|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MULSW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x200F|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_CMPEQ:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3000|((rn&15)<<8)|((rm&15)<<4); break;
	/* 3001=Esc32 */
	case BGBCC_SH_NMID_CMPHS:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_CMPGE:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3003|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_DIV1:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_DMULU:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_CMPHI:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_CMPGT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3007|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SUB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x3008|((rn&15)<<8)|((rm&15)<<4); break;
	/* 3009=Esc32 */
	case BGBCC_SH_NMID_SUBC:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x300A|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SUBV:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x300B|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_ADD:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x300C|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_DMULS:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x300D|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_ADDC:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x300E|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_ADDV:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x300F|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_SHAD:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x400C|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SHLD:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x400D|((rn&15)<<8)|((rm&15)<<4); break;
	/* 400E: LDC forms */
	case BGBCC_SH_NMID_MACW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x400F|((rn&15)<<8)|((rm&15)<<4); break;

	/* 6oo0..6oo2: MOV @Rm, Rn */
	case BGBCC_SH_NMID_MOV:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6003|((rn&15)<<8)|((rm&15)<<4); break;
	/* 6oo4..6oo6: MOV @Rm+, Rn */
	case BGBCC_SH_NMID_NOT:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6007|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SWAPB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6008|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SWAPW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6009|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_NEGC:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x600A|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_NEG:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x600B|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_EXTUB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x600C|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_EXTUW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x600D|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_EXTSB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x600E|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_EXTSW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x600F|((rn&15)<<8)|((rm&15)<<4); break;

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

	case BGBCC_SH_NMID_FSTS:
		if(rm!=BGBCC_SH_REG_FPUL)break;
		opw=0xF00D|((rn&15)<<8); break;
	case BGBCC_SH_NMID_FLDS:
		if(rn!=BGBCC_SH_REG_FPUL)break;
		opw=0xF01D|((rm&15)<<8); break;
	case BGBCC_SH_NMID_FLOAT:
		if(rm!=BGBCC_SH_REG_FPUL)break;
		opw=0xF02D|((rn&15)<<8); break;
	case BGBCC_SH_NMID_FTRC:
		if(rn!=BGBCC_SH_REG_FPUL)break;
		opw=0xF03D|((rm&15)<<8); break;
	case BGBCC_SH_NMID_FCNVSD:
		if(rm!=BGBCC_SH_REG_FPUL)break;
		opw=0xF0AD|((rn&15)<<8); break;
	case BGBCC_SH_NMID_FCNVDS:
		if(rn!=BGBCC_SH_REG_FPUL)break;
		opw=0xF0BD|((rm&15)<<8); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw2;

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0xF00A|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw2;

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6000|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6001|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6002|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SWAPB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6008|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_SWAPW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6009|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))	break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF008|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStDecReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStDecReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw2;

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_STSL:
	case BGBCC_SH_NMID_STCL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((rm&0xF0)==0x10)
			{ opw=0x4003|((rn&15)<<8)|((rm&15)<<4); break; }
		if((rm&0xF0)==0x20)
			{ opw=0x4002|((rn&15)<<8)|((rm&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0x2006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))	break;
		opw=0xF00B|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdIncRegReg(ctx, nmid, rm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdIncRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn)
{
	int opw, opw2;

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_LDSL:
	case BGBCC_SH_NMID_LDCL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if((rn&0xF0)==0x10)
			{ opw=0x4007|((rm&15)<<8)|((rn&15)<<4); break; }
		if((rn&0xF0)==0x20)
			{ opw=0x4006|((rm&15)<<8)|((rn&15)<<4); break; }
		break;

	case BGBCC_SH_NMID_MOVB:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVW:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_MOVL:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x6006|((rn&15)<<8)|((rm&15)<<4); break;
	case BGBCC_SH_NMID_FMOVS:
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF009|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


int BGBCC_SHX_EmitOpRegStRegDisp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int rn, int disp)
{
	if(!BGBCC_SHX_TryEmitOpRegStRegDisp(ctx, nmid, rm, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStRegDisp(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int rn, int disp)
{
	int opw, opw2;

	if(disp==0)
	{
		return(BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

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

		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		
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

		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

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

		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

		if((disp>>2)>=16)			break;
		opw=0x1000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_movi20)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(disp<0)					break;
			if(disp>=4096)				break;

			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x0000|((disp)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (disp<0))	break;
			if((disp>>1)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x1000|((disp>>1)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x2000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVS:
			if(BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x3000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVD:
//			if(!BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
//				break;
			if((disp&3) || (disp<0))	break;
			if((disp>>3)>=4096)			break;

			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;

			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x3000|((disp>>3)&0x0FFF);
			break;
		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(((sbyte)disp)!=disp)		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x2000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x2001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x2002|((rn&15)<<8)|((rm&15)<<4);
			break;
//		case BGBCC_SH_NMID_FMOVS:
//			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
//			opw2=0x7000|((disp>>2)&0x0FFF);
//			break;
		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdRegDispReg(ctx, nmid, rm, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdRegDispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int disp, int rn)
{
	int opw, opw2;

	if(disp==0)
	{
		return(BGBCC_SHX_TryEmitOpLdRegReg(ctx, nmid, rm, rn));
	}

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(rn!=BGBCC_SH_REG_R0)		break;
		if(disp<0)				break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if(disp>=256)			break;
			opw=0xC400|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		
		if(disp>=16)				break;
		opw=0x8400|((rm&15)<<4)|disp;
		break;
	case BGBCC_SH_NMID_MOVW:
		if(rn!=BGBCC_SH_REG_R0)		break;
		if((disp&1) || (disp<0))	break;

		if(rm==BGBCC_SH_REG_GBR)
		{
			if((disp>>1)>=256)		break;
			opw=0xC500|disp;		break;
		}

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;

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

		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;

		if((disp>>2)>=16)			break;
		opw=0x5000|((rn&15)<<8)|((rm&15)<<4)|(disp>>2);
		break;

	default:
		break;
	}

#if 1
	if((opw<0) && ctx->has_movi20)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(disp<0)					break;
			if(disp>=4096)				break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x4000|((disp)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (disp<0))	break;
			if((disp>>1)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x5000|((disp>>1)&0x0FFF);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x6000|((disp>>2)&0x0FFF);
			break;
		case BGBCC_SH_NMID_FMOVS:
			if((disp&3) || (disp<0))	break;
			if((disp>>2)>=4096)			break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
			opw2=0x7000|((disp>>2)&0x0FFF);
			break;
		default:
			break;
		}
	}
#endif

#if 1
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(((sbyte)disp)!=disp)		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((disp)&255);
			opw2=0x6000|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVW:
			if((disp&1) || (((sbyte)(disp>>1))!=(disp>>1)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>1)&255);
			opw2=0x6001|((rn&15)<<8)|((rm&15)<<4);
			break;
		case BGBCC_SH_NMID_MOVL:
			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((disp>>2)&255);
			opw2=0x6002|((rn&15)<<8)|((rm&15)<<4);
			break;
//		case BGBCC_SH_NMID_FMOVS:
//			if((disp&3) || (((sbyte)(disp>>2))!=(disp>>2)))		break;
//			opw=0x3001|((rn&15)<<8)|((rm&15)<<4);
//			opw2=0x7000|((disp>>2)&0x0FFF);
//			break;
		default:
			break;
		}
	}
#endif

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg2(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg2(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegStReg2(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw, opw2;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
		return(BGBCC_SHX_TryEmitOpRegStReg(ctx, nmid, rm, rn));
	}

//	if((rm&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rm=BGBCC_SH_REG_FR0+(2*(rm&7)); }

	if((ro!=BGBCC_SH_REG_R0) && (rn==BGBCC_SH_REG_R0))
		{ i=ro; ro=rn; rn=i; }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		if((rm&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0x0004|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVW:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		if((rm&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0x0005|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVL:
		if(ro!=BGBCC_SH_REG_R0)
			break;
		if((rm&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0x0006|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_FMOVS:
		if(ro!=BGBCC_SH_REG_R0)
			break;
//		if((rm&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
		opw=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}
	
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpLdReg2Reg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdReg2Reg(ctx, nmid, rm, ro, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpLdReg2Reg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn)
{
	int opw, opw2;
	int i;

	if(ro==BGBCC_SH_REG_ZZR)
	{
//		return(BGBCC_SHX_EmitOpRegLdReg(ctx, nmid, rm, rn));
		return(BGBCC_SHX_EmitOpLdRegReg(ctx, nmid, rm, rn));
	}

//	if((rn&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ rn=BGBCC_SH_REG_FR0+(2*(rn&7)); }

	if((ro!=BGBCC_SH_REG_R0) && (rm==BGBCC_SH_REG_R0))
		{ i=ro; ro=rm; rm=i; }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_MOVB:
		if(ro!=BGBCC_SH_REG_R0)
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x000C|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVW:
		if(ro!=BGBCC_SH_REG_R0)
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x000D|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_MOVL:
		if(ro!=BGBCC_SH_REG_R0)
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x000E|((rn&15)<<8)|((rm&15)<<4); break;

	case BGBCC_SH_NMID_FMOVS:
		if(ro!=BGBCC_SH_REG_R0)
			break;
//		if((rn&BGBCC_SH_REG_RTMASK)!=BGBCC_SH_REG_FR0)
//			{ BGBCC_DBGBREAK }
		if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegFPR(ctx, rn))		break;
		opw=0xF006|((rn&15)<<8)|((rm&15)<<4); break;

	default:
		break;
	}

	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		case BGBCC_SH_NMID_LEAB:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x6004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAW:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x6005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAL:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x6006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_LEAQ:
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4);
			opw2=0x6007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_EmitOpRegStReg2Disp(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int rn, int disp)
{
	if(!BGBCC_SHX_TryEmitOpRegStReg2Disp(ctx, nmid, rm, ro, rn, disp))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_EmitOpLdReg2DispReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int ro, int disp, int rn)
{
	if(!BGBCC_SHX_TryEmitOpLdReg2DispReg(ctx, nmid, rm, ro, disp, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

#if 1

int BGBCC_SHX_TryEmitOpRegStReg2Disp(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int rn, int disp)
{
	int opw, opw2;
	int i;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;
	
	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((disp<0) || (disp>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp);
			opw2=0x0004|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			if((disp<0) || (disp&1) || ((disp>>1)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp>>1);
			opw2=0x0005|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			if((disp<0) || (disp&3) || ((disp>>2)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp>>2);
			opw2=0x0006|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp<0) || (disp&7) || ((disp>>3)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp>>3);
			opw2=0x0007|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}
	
	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

int BGBCC_SHX_TryEmitOpLdReg2DispReg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int ro, int disp, int rn)
{
	int opw, opw2;
	int i, d;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, ro);

	opw=-1; opw2=-1;

	if((opw<0) && ctx->has_bjx1mov)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_MOVB:
			if((disp<0) || (disp>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp);
			opw2=0x000C|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVW:
			if((disp<0) || (disp&1) || ((disp>>1)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp>>1);
			opw2=0x000D|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVL:
			if((disp<0) || (disp&3) || ((disp>>2)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp>>2);
			opw2=0x000E|((rn&15)<<8)|((rm&15)<<4); break;
		case BGBCC_SH_NMID_MOVQ:
			if((disp<0) || (disp&7) || ((disp>>3)>=16))		break;
			if(!BGBCC_SHX_EmitCheckRegAddrGPR(ctx, rm))		break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
			opw=0x8E00|((ro&15)<<4)|(disp>>3);
			opw2=0x000F|((rn&15)<<8)|((rm&15)<<4); break;

//		case BGBCC_SH_NMID_FMOVS:
//			opw=0x8E00|((ro&15)<<4);
//			opw2=0xF007|((rn&15)<<8)|((rm&15)<<4); break;

		default:
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}


#endif

#if 1
int BGBCC_SHX_EmitOpRegRegReg(BGBCC_SHX_Context *ctx,
	int nmid, int rs, int rt, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegRegReg(ctx, nmid, rs, rt, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegRegReg(
	BGBCC_SHX_Context *ctx, int nmid, int rs, int rt, int rn)
{
	int opw, opw2;
	int i, e;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rs);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rt);

	opw=-1; opw2=-1;
	
//	if((opw<0) && ctx->has_bjx1ari)
	if((opw<0) && ctx->has_bjx1ari &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rs) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rt) &&
		BGBCC_SHX_EmitCheckRegExtGPR(ctx, rn))
	{
		e=0;

		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RQ0)	e|=8;
		if((rn&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RH0)	e|=4;
		if((rs&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RH0)	e|=2;
		if((rt&BGBCC_SH_REG_RTMASK)==BGBCC_SH_REG_RH0)	e|=1;
	
		if((e&8) && (e&7))
			{ BGBCC_DBGBREAK }
	
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:
			opw=0x8C30|e;
			opw2=0x0000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SUB:
			opw=0x8C30|e;
			opw2=0x1000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_MULL:
			opw=0x8C30|e;
			opw2=0x2000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_AND:
			opw=0x8C30|e;
			opw2=0x3000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_OR:
			opw=0x8C30|e;
			opw2=0x4000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_XOR:
			opw=0x8C30|e;
			opw2=0x5000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHLL:
			opw=0x8C30|e;
			opw2=0x6000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHLR:
			opw=0x8C30|e;
			opw2=0x7000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		case BGBCC_SH_NMID_SHAR:
			opw=0x8C30|e;
			opw2=0x8000|((rn&15)<<8)|((rs&15)<<4)|(rt&15);
			break;
		}
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}
#endif

int BGBCC_SHX_EmitOpRegImm(BGBCC_SHX_Context *ctx,
	int nmid, int reg, int imm)
{
	if(!BGBCC_SHX_TryEmitOpRegImm(ctx, nmid, reg, imm))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegImm(BGBCC_SHX_Context *ctx,
	int nmid, int reg, int imm)
{
	s32 imm1;
	int opw, opw2;

//	if((reg&BGBCC_SH_REG_RTMASK3)==BGBCC_SH_REG_DR0)
//		{ reg=BGBCC_SH_REG_FR0+(2*(reg&7)); }

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:	
		if(((sbyte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
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
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
		opw=0xE000|((reg&15)<<8)|(imm&255);
		break;

	default:
		break;
	}

	if((opw<0) && ctx->has_bjx1ari)
	{
		switch(nmid)
		{
		case BGBCC_SH_NMID_ADD:	
			if(((s16)imm)!=imm)
				break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>8)&255);
			opw2=0x7000|((reg&15)<<8)|(imm&255);
			break;

		case BGBCC_SH_NMID_TST:	
			if((imm&0xFFF)!=imm)
				break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xC800|((reg&15)<<4)|(imm&15);
			break;
		case BGBCC_SH_NMID_AND:	
			if((imm&0xFFF)!=imm)
				break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xC900|((reg&15)<<4)|(imm&15);
			break;
		case BGBCC_SH_NMID_XOR:	
			if((imm&0xFFF)!=imm)
				break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xCA00|((reg&15)<<4)|(imm&15);
			break;
		case BGBCC_SH_NMID_OR:	
			if((imm&0xFFF)!=imm)
				break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0xCB00|((reg&15)<<4)|(imm&15);
			break;

		case BGBCC_SH_NMID_CMPEQ:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3000|((reg&15)<<8)|((imm&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPHS:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3002|((reg&15)<<8)|((imm&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPGE:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3003|((reg&15)<<8)|((imm&15)<<4);
			break;

		case BGBCC_SH_NMID_CMPHI:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3006|((reg&15)<<8)|((imm&15)<<4);
			break;
		case BGBCC_SH_NMID_CMPGT:	
			imm1=(imm<<20)>>20;
			if(imm1!=imm)	break;
			if(!BGBCC_SHX_EmitCheckRegGPR(ctx, reg))		break;
			opw=0x8E00|((imm>>4)&255);
			opw2=0x3007|((reg&15)<<8)|((imm&15)<<4);
			break;

		default:
			break;
		}

	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		if(opw2>=0)
			BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}

#if 1
int BGBCC_SHX_EmitOpRegImmReg(BGBCC_SHX_Context *ctx,
	int nmid, int rm, int imm, int rn)
{
	if(!BGBCC_SHX_TryEmitOpRegImmReg(ctx, nmid, rm, imm, rn))
		{ BGBCC_DBGBREAK }
	return(1);
}

int BGBCC_SHX_TryEmitOpRegImmReg(
	BGBCC_SHX_Context *ctx, int nmid, int rm, int imm, int rn)
{
	int opw, opw2;

	if(!(ctx->has_bjx1ari))
		return(0);

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rn);
	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, rm);

	if((nmid==BGBCC_SH_NMID_ADD) && (imm<0) && ((-imm)!=imm))
		return(BGBCC_SHX_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_SUB, rm, -imm, rn));
	if((nmid==BGBCC_SH_NMID_SUB) && (imm<0) && ((-imm)!=imm))
		return(BGBCC_SHX_TryEmitOpRegImmReg(ctx,
			BGBCC_SH_NMID_ADD, rm, -imm, rn));

	opw=-1; opw2=-1;
	switch(nmid)
	{
	case BGBCC_SH_NMID_ADD:	
		if(((byte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x300C|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_SUB:	
		if(((byte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x3008|((rn&15)<<8)|((rm&15)<<4);
		break;

	case BGBCC_SH_NMID_AND:	
		if(((byte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x2009|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_XOR:	
		if(((byte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x200A|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_OR:	
		if(((byte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x200B|((rn&15)<<8)|((rm&15)<<4);
		break;

	case BGBCC_SH_NMID_MULL:	
		if(((sbyte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x200E|((rn&15)<<8)|((rm&15)<<4);
		break;

	case BGBCC_SH_NMID_SHAD:	
		if(((sbyte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x400C|((rn&15)<<8)|((rm&15)<<4);
		break;
	case BGBCC_SH_NMID_SHLD:	
		if(((sbyte)imm)!=imm)
			break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rm))		break;
		if(!BGBCC_SHX_EmitCheckRegGPR(ctx, rn))		break;
		opw=0x8E00|(imm&255);
		opw2=0x400D|((rn&15)<<8)|((rm&15)<<4);
		break;
	}

	if(opw>=0)
	{
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	return(0);
}
#endif

int BGBCC_SHX_EmitIndexAddImm16(BGBCC_SHX_Context *ctx, int imm)
{
	int i, n, o;

	for(i=0; i<ctx->const_ns16; i++)
	{
		if((ctx->const_s16tab[i]==imm) &&
			!(ctx->const_s16tab_rlc[i]))
		{
			n=ctx->nofs_s16tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s16tab[n]=o;
			ctx->idx_s16tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns16++;
	ctx->const_s16tab[i]=imm;
	ctx->const_s16tab_rlc[i]=0;

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
		if((ctx->const_s32tab[i]==imm) &&
			!(ctx->const_s32tab_rlc[i]))
		{
			n=ctx->nofs_s32tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s32tab[n]=o;
			ctx->idx_s32tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns32++;
	ctx->const_s32tab[i]=imm;
	ctx->const_s32tab_rlc[i]=0;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImmPair32(BGBCC_SHX_Context *ctx,
	s32 imma, s32 immb)
{
	int i, n, o;

	for(i=0; i<(ctx->const_ns32-1); i++)
	{
		if(	(ctx->const_s32tab[i+0]==imma) &&
			(ctx->const_s32tab[i+1]==immb) &&
			!(ctx->const_s32tab_rlc[i+0]) &&
			!(ctx->const_s32tab_rlc[i+1]))
		{
			n=ctx->nofs_s32tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s32tab[n]=o;
			ctx->idx_s32tab[n]=i;
			return(i);
		}
	}
	
	i=ctx->const_ns32;
	ctx->const_ns32+=2;
	ctx->const_s32tab[i+0]=imma;
	ctx->const_s32tab[i+1]=immb;
	ctx->const_s32tab_rlc[i+0]=0;
	ctx->const_s32tab_rlc[i+1]=0;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm32LblOfs(
	BGBCC_SHX_Context *ctx, int lbl, int ofs)
{
	int rlc;
	int i, n, o;

	rlc=lbl|(BGBCC_SH_RLC_REL32<<24);

	i=ctx->const_ns32++;
	ctx->const_s32tab[i]=ofs;
	ctx->const_s32tab_rlc[i]=rlc;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm32LblAbs(
	BGBCC_SHX_Context *ctx, int lbl)
{
	int rlc;
	int i, n, o;

	rlc=lbl|(BGBCC_SH_RLC_ABS32<<24);

	for(i=0; i<ctx->const_ns32; i++)
	{
		if(ctx->is_simpass==1)	//debug
			break;

		if((ctx->const_s32tab[i]==0) &&
			(ctx->const_s32tab_rlc[i]==rlc))
		{
			n=ctx->nofs_s32tab++;
			o=BGBCC_SHX_EmitGetOffs(ctx);
			ctx->ofs_s32tab[n]=o;
			ctx->idx_s32tab[n]=i;
			return(i);
		}
	}

	i=ctx->const_ns32++;
	ctx->const_s32tab[i]=0;
	ctx->const_s32tab_rlc[i]=rlc;

	n=ctx->nofs_s32tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s32tab[n]=o;
	ctx->idx_s32tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitIndexAddImm16LblOfs(
	BGBCC_SHX_Context *ctx, int lbl, int ofs)
{
	int rlc;
	int i, n, o;

	rlc=lbl|(BGBCC_SH_RLC_REL16<<24);

	i=ctx->const_ns16++;
	ctx->const_s16tab[i]=ofs;
	ctx->const_s16tab_rlc[i]=rlc;

	n=ctx->nofs_s16tab++;
	o=BGBCC_SHX_EmitGetOffs(ctx);
	ctx->ofs_s16tab[n]=o;
	ctx->idx_s16tab[n]=i;
	return(i);
}

int BGBCC_SHX_EmitLoadRegImm(
	BGBCC_SHX_Context *ctx, int nmid, int reg, s32 imm)
{
	u32 v, v1;
	int sg, ex, fr;
	int opw, opw1, opw2, opw3;

	BGBCC_SHX_EmitCheckRegSafe(ctx, nmid, reg);

	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg) &&
		!BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
	{
		if(imm==0x00000000)
		{
			opw=0xF08D|(((reg+0)&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			return(1);
		}

#if 1
		if(imm==0x3F800000)
		{
			opw=0xF09D|(((reg+0)&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			return(1);
		}
#endif
	
#if 0
		if(ctx->has_movi20)
		{
			if(((imm<<12)>>12)==imm)
			{
				opw=0x0000|((reg&15)<<8)|((imm>>12)&0xF0);
				opw2=imm&0xFFFF;
				BGBCC_SHX_EmitWord(ctx, opw);
				BGBCC_SHX_EmitWord(ctx, opw2);
				return(1);
			}
		}
#endif

		if(ctx->has_bjx1mov)
		{
			sg=(imm>>31)&1;
			ex=(imm>>23)&255;
			fr=imm&0x7FFFFF;

			if(ex>1)
			{
				ex=(ex-127)+15;
				if((ex<1) || (ex>30))
					{ ex=0; fr=0; sg=-1; }
				if(fr&0x1FFF)
					sg=-1;
				fr=fr>>13;
				v=(sg<<15)|(ex<<10)|fr;
			}else
			{
				v=0;
			}

			if(sg>=0)
			{
				opw1=0x8E00|(v>>8);
				opw2=0x8A00|(v&0x00FF);
				opw3=0xF00D|((reg&15)<<8);

				BGBCC_SHX_EmitWord(ctx, opw1);
				BGBCC_SHX_EmitWord(ctx, opw2);
				BGBCC_SHX_EmitWord(ctx, opw3);
				return(1);
			}
		}

		opw=0xC700;
		opw2=0xF009|(((reg+0)&15)<<8);

		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm32(ctx, imm);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsLpReg(NULL, ctx, reg))
		{ return(BGBCC_SHX_EmitLoadRegImm64P(ctx, reg, imm)); }

	if((((sbyte)imm)==imm) && ((reg&0xF0)==0x00))
	{
		opw=0xE000|((reg&15)<<8)|(imm&255);
		BGBCC_SHX_EmitWord(ctx, opw);
		return(1);
	}

#if 0
	if(ctx->has_movi20 || ctx->has_bjx1mov)
	{
		if((((s16)imm)==imm) && ((reg&0xF0)==0x00))
		{
			opw=0x9000|((reg&15)<<8);
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitIndexAddImm16(ctx, imm);
			return(1);
		}
	}
#endif

	if(ctx->has_movi20)
//	if(0)
	{
		if((((imm<<12)>>12)==imm) && ((reg&0xF0)==0x00))
		{
			opw=0x0000|((reg&15)<<8)|((imm>>12)&0xF0);
			opw2=imm&0xFFFF;
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}

		if(((((imm<<4)>>4)&(~255))==imm) && ((reg&0xF0)==0x00))
		{
			opw=0x0001|((reg&15)<<8)|((imm>>20)&0xF0);
			opw2=(imm>>8)&0xFFFF;
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
	}

	if(ctx->has_bjx1mov)
//	if(0)
	{
#if 1
		if((((imm<<8)>>8)==imm) && (reg==BGBCC_SH_REG_R0))
		{
			opw=0x8A00|((imm>>16)&0xFF);
			opw2=imm&0xFFFF;
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
#endif

#if 1
		if((((imm<<16)>>16)==imm) && ((reg&0xF0)==0x00))
		{
			opw=0x8E00|(imm&0xFF);
			opw2=0xE000|((reg&15)<<8)|((imm>>8)&0xFF);
			BGBCC_SHX_EmitWord(ctx, opw);
			BGBCC_SHX_EmitWord(ctx, opw2);
			return(1);
		}
#endif
	}

#if 0
	if(!(imm&3) && (((sbyte)(imm>>2))==(imm>>2)) && ((reg&0xF0)==0x00))
	{
		opw=0xE000|((reg&15)<<8)|((imm>>2)&255);
		opw2=0x4008|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
#endif

	if(!(imm&255) && (((sbyte)(imm>>8))==(imm>>8)) && ((reg&0xF0)==0x00))
	{
		opw=0xE000|((reg&15)<<8)|((imm>>8)&255);
		opw2=0x4018|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}

	if(!(imm&65535) && (((sbyte)(imm>>16))==(imm>>16)) && ((reg&0xF0)==0x00))
	{
		opw=0xE000|((reg&15)<<8)|((imm>>16)&255);
		opw2=0x4028|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
	
	if((((s16)imm)==imm) && ((reg&0xF0)==0x00))
	{
		opw=0x9000|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm16(ctx, imm);
		return(1);
	}

#if 1
	if(!(imm&255) && (((s16)(imm>>8))==(imm>>8)) && ((reg&0xF0)==0x00))
	{
		opw=0x9000|((reg&15)<<8);
		opw2=0x4018|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm16(ctx, imm>>8);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
#endif

#if 1
	if(!(imm&65535) && (((s16)(imm>>16))==(imm>>16)) && ((reg&0xF0)==0x00))
	{
		opw=0x9000|((reg&15)<<8);
		opw2=0x4028|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm16(ctx, imm>>16);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}
#endif

	if((((s32)imm)==imm) && ((reg&0xF0)==0x00))
	{
		opw=0xD000|((reg&15)<<8);
		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImm32(ctx, imm);
		return(1);
	}
	
	BGBCC_DBGBREAK
	
	return(0);
}

int BGBCC_SHX_EmitLoadRegImm64P(
	BGBCC_SHX_Context *ctx, int reg, s64 imm)
{
	s32 lo, hi;
	int i;

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	lo=imm; hi=imm>>32;
	i=BGBCC_SHX_EmitLoadRegImmPair32(ctx, reg, lo, hi);
	return(i);
}

int BGBCC_SHX_EmitLoadRegImmPair32(
	BGBCC_SHX_Context *ctx, int reg, s32 imma, s32 immb)
{
	int reg2;
	u32 v;
	int sg, ex, fr;
	int opw, opw1, opw2, opw3;

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	reg2=reg;
//	if(BGBCC_SHXC_EmitRegIsDpReg(NULL, ctx, reg))
//		{ reg2=BGBCC_SH_REG_FR0+((reg&7)*2); }
	if(BGBCC_SHXC_EmitRegIsDpReg(NULL, ctx, reg))
		{ reg2=BGBCC_SH_REG_FR0+(reg&15); }
	if(BGBCC_SHXC_EmitRegIsLpReg(NULL, ctx, reg))
		{ reg2=BGBCC_SH_REG_R0+(reg&15); }

#if 1
	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg2) &&
		BGBCC_SHXC_GetStatusFpscrPr(NULL, ctx))
	{
		if(ctx->has_bjx1mov)
		{
			sg=(immb>>31)&1;
			ex=(immb>>20)&2047;
			fr=immb&0x000FFFFF;
			if(imma || (fr&0x03FF))
				sg=-1;

			if(ex>1)
			{
				ex=(ex-1023)+15;
				if((ex<1) || (ex>30))
					{ ex=0; fr=0; sg=-1; }
				fr=fr>>10;
				v=(sg<<15)|(ex<<10)|fr;
			}else
			{
				v=0;
			}

			if(sg>=0)
			{
				opw1=0x8E00|((v>>8)&0x00FF);
				opw2=0x8A00|(v&0x00FF);
				opw3=0xF0AD|((reg2&15)<<8);

				BGBCC_SHX_EmitWord(ctx, opw1);
				BGBCC_SHX_EmitWord(ctx, opw2);
				BGBCC_SHX_EmitWord(ctx, opw3);
				return(1);
			}
		}
	}
#endif

	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg2) &&
		!BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
	{
		opw=0xC700;
		opw1=0xF009|(((reg2+1)&15)<<8);
		opw2=0xF009|(((reg2+0)&15)<<8);

		BGBCC_SHX_EmitWord(ctx, opw);
		BGBCC_SHX_EmitIndexAddImmPair32(ctx, imma, immb);
		BGBCC_SHX_EmitWord(ctx, opw1);
		BGBCC_SHX_EmitWord(ctx, opw2);
		return(1);
	}

	if(BGBCC_SHXC_EmitRegIsFpReg(NULL, ctx, reg2) &&
		BGBCC_SHXC_GetStatusFpscrSz(NULL, ctx))
	{
		opw=0xC700;
		opw1=0xF009|(((reg2+0)&15)<<8);

		BGBCC_SHX_EmitWord(ctx, opw);
		if(BGBCC_SHXC_GetStatusFpscrPr(NULL, ctx))
			BGBCC_SHX_EmitIndexAddImmPair32(ctx, imma, immb);
		else
			BGBCC_SHX_EmitIndexAddImmPair32(ctx, immb, imma);
		BGBCC_SHX_EmitWord(ctx, opw1);
		return(1);
	}
	
	BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+0, imma);
	BGBCC_SHX_EmitLoadRegImm(ctx, 0, reg2+1, immb);
	return(1);
}

int BGBCC_SHX_EmitLoadRegLabelRel(
	BGBCC_SHX_Context *ctx, int reg, int lbl, int ofs)
{
	int opw;

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	opw=0xD000|((reg&15)<<8);
	BGBCC_SHX_EmitWord(ctx, opw);
	BGBCC_SHX_EmitIndexAddImm32LblOfs(ctx, lbl, -ofs);
	return(1);
}

int BGBCC_SHX_EmitLoadRegLabelAbs(
	BGBCC_SHX_Context *ctx, int reg, int lbl)
{
	int opw;

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	opw=0xD000|((reg&15)<<8);
	BGBCC_SHX_EmitWord(ctx, opw);
	BGBCC_SHX_EmitIndexAddImm32LblAbs(ctx, lbl);
	return(1);
}

int BGBCC_SHX_EmitLoadRegLabelRel16(
	BGBCC_SHX_Context *ctx, int reg, int lbl, int ofs)
{
	int opw;

	BGBCC_SHX_EmitCheckRegSafe(ctx, 0, reg);

	opw=0x9000|((reg&15)<<8);
	BGBCC_SHX_EmitWord(ctx, opw);
	BGBCC_SHX_EmitIndexAddImm16LblOfs(ctx, lbl, -ofs);
	return(1);
}

int BGBCC_SHX_EmitFlushIndexImm16(BGBCC_SHX_Context *ctx)
{
	int ob, opc, opw, opd, oba, obd2;
	int i, j, k;
	
	if(!ctx->const_ns16)
		return(0);
	
	BGBCC_SHX_EmitBAlign(ctx, 2);
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	for(i=0; i<ctx->const_ns16; i++)
	{
		j=ctx->const_s16tab_rlc[i];
		if(j)
			{ BGBCC_SHX_EmitRelocTy(ctx, j&0x00FFFFFF, (j>>24)&255); }
		BGBCC_SHX_EmitWord(ctx, ctx->const_s16tab[i]);
	}

	for(i=0; i<ctx->nofs_s16tab; i++)
	{
		j=ctx->idx_s16tab[i];
		k=ctx->const_s16tab_rlc[j];

		opc=ctx->ofs_s16tab[i];
//		opd=((ob-(opc+4))/2)+(ctx->idx_s16tab[i]);
//		opd=((ob-(opc+2))/2)+(ctx->idx_s16tab[i]);
		opd=((ob-(opc+2))/2)+j;
//		opd=((ob-opc)/2)+(ctx->idx_s16tab[i]);
		opw=BGBCC_SHX_EmitGetOffsWord(ctx, opc-2);

		if((opw&0xF000)!=0x9000)
			{ BGBCC_DBGBREAK }
		if(opd>>8)
			{ BGBCC_DBGBREAK }

		opw+=opd;
		BGBCC_SHX_EmitSetOffsWord(ctx, opc-2, opw);

		if(((k>>24)&255)==BGBCC_SH_RLC_REL16)
		{
			oba=ob+j*2;
			obd2=BGBCC_SHX_EmitGetOffsWord(ctx, oba);
			obd2+=oba-(opc-2);
			BGBCC_SHX_EmitSetOffsWord(ctx, oba, obd2);
		}
	}
	
	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	return(1);
}

int BGBCC_SHX_EmitFlushIndexImm32(BGBCC_SHX_Context *ctx)
{
	int ob, opc, opw, opd, oba, obd2;
	int i, j, k;
	
	if(!ctx->const_ns32)
		return(0);

	BGBCC_SHX_EmitBAlign(ctx, 4);
	ob=BGBCC_SHX_EmitGetOffs(ctx);
	for(i=0; i<ctx->const_ns32; i++)
	{
		j=ctx->const_s32tab_rlc[i];
		if(j)
			{ BGBCC_SHX_EmitRelocTy(ctx, j&0x00FFFFFF, (j>>24)&255); }
		BGBCC_SHX_EmitDWord(ctx, ctx->const_s32tab[i]);
	}

	for(i=0; i<ctx->nofs_s32tab; i++)
	{
		j=ctx->idx_s32tab[i];
		k=ctx->const_s32tab_rlc[j];
		opc=ctx->ofs_s32tab[i];
//		opd=((ob-(opc+4))/4)+j;
//		opd=((ob-(opc+2))/4)+j;
		opd=((ob-opc)/4)+j;
		opw=BGBCC_SHX_EmitGetOffsWord(ctx, opc-2);

//		if((opw&0xF000)!=0xD000)
		if(((opw&0xF000)!=0xD000) && ((opw&0xFF00)!=0xC700))
			{ BGBCC_DBGBREAK }
		if(opd>>8)
			{ BGBCC_DBGBREAK }

		opw+=opd;
		BGBCC_SHX_EmitSetOffsWord(ctx, opc-2, opw);
		
		if(((k>>24)&255)==BGBCC_SH_RLC_REL32)
		{
			oba=ob+j*4;
			obd2=BGBCC_SHX_EmitGetOffsDWord(ctx, oba);
			obd2+=oba-(opc-2);
			BGBCC_SHX_EmitSetOffsDWord(ctx, oba, obd2);
		}
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
//		if((obe16-oo)>=480)
		if((obe16-oo)>=420)
			return(1);
	}

	if(ctx->nofs_s32tab)
	{
		oo=ctx->ofs_s32tab[0];
		if((obe32-oo)>=960)
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
	BGBCC_SHX_EmitWord(ctx, 0x0009);

	BGBCC_SHX_EmitFlushIndexImm16(ctx);
	BGBCC_SHX_EmitFlushIndexImm32(ctx);

	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;

	BGBCC_SHX_EmitLabel(ctx, l0);
	return(1);
}

int BGBCC_SHX_EmitForceFlushIndexImm(BGBCC_SHX_Context *ctx)
{
	int l0;

//	if(!BGBCC_SHX_EmitCheckFlushIndexImmP(ctx))
//		return(0);
	
	if(!ctx->const_ns16 && !ctx->const_ns32)
		return(0);
	
	l0=BGBCC_SHX_GenLabelTemp(ctx);

	BGBCC_SHX_EmitRelocRelW12(ctx, l0);
	BGBCC_SHX_EmitWord(ctx, 0xA000);
	BGBCC_SHX_EmitWord(ctx, 0x0009);

	BGBCC_SHX_EmitFlushIndexImm16(ctx);
	BGBCC_SHX_EmitFlushIndexImm32(ctx);

	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;

	BGBCC_SHX_EmitLabel(ctx, l0);
	return(1);
}

int BGBCC_SHX_EmitFlushIndexImmBasic(BGBCC_SHX_Context *ctx)
{
	BGBCC_SHX_EmitFlushIndexImm16(ctx);
	BGBCC_SHX_EmitFlushIndexImm32(ctx);
	ctx->const_ns16=0;
	ctx->nofs_s16tab=0;
	ctx->const_ns32=0;
	ctx->nofs_s32tab=0;
	return(1);
}
