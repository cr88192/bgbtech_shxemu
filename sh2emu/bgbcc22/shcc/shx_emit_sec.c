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

int BGBCC_SHX_LookupSectionID(BGBCC_SHX_Context *ctx, char *name)
{
	int sec;
	
	sec=-1;
	if(!strcmp(name, ".text"))		sec=BGBCC_SH_CSEG_TEXT;
	if(!strcmp(name, ".data"))		sec=BGBCC_SH_CSEG_DATA;
	if(!strcmp(name, ".bss"))		sec=BGBCC_SH_CSEG_BSS;
	if(!strcmp(name, ".got"))		sec=BGBCC_SH_CSEG_GOT;
	if(!strcmp(name, ".strtab"))	sec=BGBCC_SH_CSEG_STRTAB;
	if(!strcmp(name, ".reloc"))		sec=BGBCC_SH_CSEG_RELOC;

	if(!strcmp(name, ".rdata"))		sec=BGBCC_SH_CSEG_RODATA;
	if(!strcmp(name, ".rodata"))	sec=BGBCC_SH_CSEG_RODATA;
	
	if(sec>=0)
		return(sec);

	for(sec=BGBCC_SH_CSEG_DYN; sec<ctx->nsec; sec++)
	{
		if(!strcmp(name, ctx->sec_name[sec]))	
		{
			return(sec);
		}
	}
	
	return(-1);
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

	if(!strcmp(name, ".rdata"))		sec=BGBCC_SH_CSEG_RODATA;
	if(!strcmp(name, ".rodata"))	sec=BGBCC_SH_CSEG_RODATA;

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

int BGBCC_SHX_IsSectionReadOnly(BGBCC_SHX_Context *ctx, int sec)
{
	if(sec==BGBCC_SH_CSEG_TEXT)
		return(1);
	if(sec==BGBCC_SH_CSEG_STRTAB)
		return(1);
	if(sec==BGBCC_SH_CSEG_RODATA)
		return(1);
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

int BGBCC_SHX_EmitStatWord(BGBCC_SHX_Context *ctx, int val)
{
	if((ctx->sec!=BGBCC_SH_CSEG_TEXT) ||
		ctx->is_simpass)
	{
		ctx->stat_opc_issfx=0;
		return(0);
	}

	if(ctx->stat_opc_issfx)
	{
		ctx->stat_opc_issfx=0;
		return(0);
	}
	
	ctx->stat_opc_tot++;
	
	if((val&0xFF00)==0x8A00)
	{
		ctx->stat_opc_ext8a++;
		ctx->stat_opc_issfx=5;
		return(0);
	}

	if((val&0xFF00)==0x8E00)
	{
		ctx->stat_opc_ext8e++;
		ctx->stat_opc_issfx=1;
		return(0);
	}

	if(ctx->is_addr64)
	{
		if((val&0xFF00)==0xCE00)
		{
			ctx->stat_opc_extCe++;
			ctx->stat_opc_issfx=2;
			return(0);
		}

		if((val&0xFFF0)==0xCC30)
		{
			ctx->stat_opc_extCC3++;
			ctx->stat_opc_issfx=3;
			return(0);
		}

		if((val&0xFFF0)==0xCC00)
		{
			ctx->stat_opc_extCC0++;
			ctx->stat_opc_issfx=4;
			return(0);
		}
	}

	ctx->stat_opc_base16++;
	ctx->stat_opc_issfx=0;
	return(0);

}

int BGBCC_SHX_EmitWord(BGBCC_SHX_Context *ctx, int val)
{
	int i, j, k;

//	if(!(val&0xFFFF) && (ctx->sec==BGBCC_SH_CSEG_TEXT))
//	{
//		BGBCC_DBGBREAK
//	}

	BGBCC_SHX_EmitStatWord(ctx, val);

	if(!val && (ctx->sec==BGBCC_SH_CSEG_TEXT))
	{
		k=-1;
	}

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
	int i, j, k;

	if(!val && (ctx->sec==BGBCC_SH_CSEG_TEXT))
	{
		k=-1;
	}

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
	int i, j, k;

	if(!val && (ctx->sec==BGBCC_SH_CSEG_TEXT))
	{
		k=-1;
	}

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
				(ctx->sec!=BGBCC_SH_CSEG_DATA) &&
				(ctx->sec!=BGBCC_SH_CSEG_RELOC))
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
