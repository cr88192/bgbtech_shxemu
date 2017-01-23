BTSHAS_Section *BTSHAS_AllocSection(char *name)
{
	BTSHAS_Section *tmp;
	
	tmp=btshas_mm_malloc(sizeof(BTSHAS_Section));
	tmp->cts=tmp->t_data;
	tmp->cte=tmp->cts+BTSHAS_SEC_IDATA;
	tmp->ct=tmp->cts;
	
//	tmp->lbl=tmp->t_lbl;
//	tmp->rlc=tmp->t_rlc;
//	tmp->mlbl=256;
//	tmp->mrlc=256;
	
	tmp->name=tmp->t_name;
	strcpy(tmp->name, name);
	
	return(tmp);
}

int BTSHAS_SecCheckExpand(BTSHAS_Section *sec, int cnt)
{
	int i, j, k;

	if((sec->ct+cnt)<=sec->cte)
		return(0);
	
	if(sec->cts==sec->t_data)
	{
		i=sec->cte-sec->cts;
		j=sec->ct-sec->cts;
		k=i;
		while((j+cnt)>=k)
			k=k+(k>>1);
		sec->cts=btshas_mm_malloc(k);
		memcpy(sec->cts, sec->t_data, i);
		sec->cte=sec->cts+k;
		sec->ct=sec->cts+j;
		return(1);
	}

	i=sec->cte-sec->cts;
	j=sec->ct-sec->cts;
	k=i;
	while((j+cnt)>=k)
		k=k+(k>>1);
	sec->cts=btshas_mm_realloc(sec->cts, k);
	sec->cte=sec->cts+k;
	sec->ct=sec->cts+j;
	return(1);
}

int BTSHAS_SecEmitByte(BTSHAS_Section *sec, int val)
{
	BTSHAS_SecCheckExpand(sec, 1);
	*sec->ct++=val;
	return(0);
}

int BTSHAS_SecEmitAlign(BTSHAS_Section *sec, int val)
{
	int i, j, k;
	
	i=sec->ct-sec->cts;
	j=(i+(val-1))&(~(val-1));
	k=j-i;
	
	if(k)
	{
		BTSHAS_SecCheckExpand(sec, k);
		while(k--)
			*sec->ct++=0;
	}
	return(0);
}

int BTSHAS_EmitAlign(BTSHAS_Context *ctx, int val)
{
	return(BTSHAS_SecEmitAlign(ctx->seg[ctx->cseg], val));
}

int BTSHAS_EmitByte(BTSHAS_Context *ctx, int val)
{
	return(BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val));
}

int BTSHAS_EmitWord(BTSHAS_Context *ctx, int val)
{
	if(ctx->is_le)
	{
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val);
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>>8);
		return(0);
	}else
	{
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>>8);
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val);
		return(0);
	}
}

int BTSHAS_EmitDWord(BTSHAS_Context *ctx, u32 val)
{
	if(ctx->is_le)
	{
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val    );
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>> 8);
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>>16);
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>>24);
		return(0);
	}else
	{
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>>24);
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>>16);
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val>> 8);
		BTSHAS_SecEmitByte(ctx->seg[ctx->cseg], val    );
		return(0);
	}
}

int BTSHAS_EmitString(BTSHAS_Context *ctx, char *str)
{
	byte *s;
	
	s=str;
	while(*s)
		BTSHAS_EmitByte(ctx, *s++);
	BTSHAS_EmitByte(ctx, *s++);
	return(0);
}

int BTSHAS_EmitAscii(BTSHAS_Context *ctx, char *str)
{
	byte *s;
	
	s=str;
	while(*s)
		BTSHAS_EmitByte(ctx, *s++);
	return(0);
}

int BTSHAS_EmitRawBytes(BTSHAS_Context *ctx, byte *buf, int sz)
{
	byte *s;
	int n;
	
	if(buf)
	{
		s=buf; n=sz;
		while(n--)
			BTSHAS_EmitByte(ctx, *s++);
	}else
	{
		n=sz;
		while(n--)
			BTSHAS_EmitByte(ctx, 0);
	}
	return(0);
}

#if 0
int BTSHAS_EmitCheckLabelExpand(BTSHAS_Context *ctx)
{
	BTSHAS_Section *sec;
	int i, j, k;

	sec=ctx->seg[ctx->cseg];

	if((sec->nlbl+1)<sec->mlbl)
		return(0);

	if(sec->lbl==sec->t_lbl)
	{
		j=sec->mlbl+(sec->mlbl>>1);
		sec->lbl=btshas_mm_malloc(j*sizeof(BTSHAS_LabelInfo));
		memcpy(sec->lbl, sec->t_lbl,
			sec->mlbl*sizeof(BTSHAS_LabelInfo));
		sec->mlbl=j;
		return(1);
	}
	
	j=sec->mlbl+(sec->mlbl>>1);
	sec->lbl=btshas_mm_realloc(sec->lbl, j*sizeof(BTSHAS_LabelInfo));
	sec->mlbl=j;
	return(1);
}
#endif

#if 1
int BTSHAS_EmitCheckLabelExpand(BTSHAS_Context *ctx)
{
	int i, j, k;

	if((ctx->nlbl+1)<ctx->mlbl)
		return(0);

	if(ctx->lbl==ctx->t_lbl)
	{
		j=ctx->mlbl+(ctx->mlbl>>1);
		ctx->lbl=btshas_mm_malloc(j*sizeof(BTSHAS_LabelInfo));
		memcpy(ctx->lbl, ctx->t_lbl,
			ctx->mlbl*sizeof(BTSHAS_LabelInfo));
		ctx->mlbl=j;
		return(1);
	}
	
	j=ctx->mlbl+(ctx->mlbl>>1);
	ctx->lbl=btshas_mm_realloc(ctx->lbl, j*sizeof(BTSHAS_LabelInfo));
	ctx->mlbl=j;
	return(1);
}
#endif

int BTSHAS_LookupLabel(BTSHAS_Context *ctx, char *name)
{
	BTSHAS_LabelInfo *lbl;
	int i;
	
	for(i=0; i<ctx->nlbl; i++)
	{
		lbl=ctx->lbl+i;
		
//		if(lbl->rlty!=BTSHAS_LBLRLC_SH_NONE)
//			continue;
		
		if(!strcmp(lbl->name, name))
			return(i);
	}
	return(-1);
}

int BTSHAS_LookupDefinedLabel(BTSHAS_Context *ctx, char *name)
{
	BTSHAS_LabelInfo *lbl;
	int i;
	
	for(i=0; i<ctx->nlbl; i++)
	{
		lbl=ctx->lbl+i;
		
		if((lbl->scl==BTSHAS_LBLSCL_EXTERNAL_DEF) ||
				(lbl->scl==BTSHAS_LBLSCL_UNDEF_LABEL))
			continue;
		if(lbl->sect<0)
			continue;
		
		if(!strcmp(lbl->name, name))
			return(i);
	}
	return(-1);
}

int BTSHAS_EmitLabel(BTSHAS_Context *ctx, char *name)
{
	BTSHAS_Section *sec;
	BTSHAS_LabelInfo *lbl;
	int i, j;

	sec=ctx->seg[ctx->cseg];
//	i=sec->nlbl++;
//	lbl=sec->lbl+i;

	i=BTSHAS_LookupLabel(ctx, name);
	if(i<0)
	{
		BTSHAS_EmitCheckLabelExpand(ctx);
		i=ctx->nlbl++;
	}

	lbl=ctx->lbl+i;

	lbl->name=btshas_strdup(name);
	lbl->offs=sec->ct-sec->cts;
	lbl->sect=ctx->cseg;
	lbl->ty_bty=0;
	lbl->ty_dty=0;
	if((lbl->scl==BTSHAS_LBLSCL_EXTERNAL_DEF) ||
		(lbl->scl==BTSHAS_LBLSCL_EXTERN))
		{ lbl->scl=BTSHAS_LBLSCL_EXTERN; }
	else
		{ lbl->scl=BTSHAS_LBLSCL_LABEL; }
//	lbl->rlty=BTSHAS_LBLRLC_SH_NONE;
	return(i);
}

int BTSHAS_EmitGlobal(BTSHAS_Context *ctx, char *name)
{
	BTSHAS_LabelInfo *lbl;
	int i, j;

	i=BTSHAS_LookupLabel(ctx, name);

	if(i<0)
	{
		BTSHAS_EmitCheckLabelExpand(ctx);
		i=ctx->nlbl++;

		lbl=ctx->lbl+i;

		lbl->name=btshas_strdup(name);
		lbl->offs=0;
//		lbl->sect=ctx->cseg;
		lbl->sect=-1;
		lbl->ty_bty=0;
		lbl->ty_dty=0;
		lbl->scl=BTSHAS_LBLSCL_EXTERNAL_DEF;
//		lbl->rlty=BTSHAS_LBLRLC_SH_NONE;
		return(i);
	}else
	{
		lbl=ctx->lbl+i;
		if(lbl->scl==BTSHAS_LBLSCL_LABEL)
			{ lbl->scl=BTSHAS_LBLSCL_EXTERN; }
		else if(lbl->scl==BTSHAS_LBLSCL_UNDEF_LABEL)
			{ lbl->scl=BTSHAS_LBLSCL_EXTERNAL_DEF; }
	}
	return(0);
}

int BTSHAS_EmitUndefSym(BTSHAS_Context *ctx, char *name)
{
	BTSHAS_Section *sec;
	BTSHAS_LabelInfo *lbl;
	int i, j;

	i=BTSHAS_LookupLabel(ctx, name);

	if(i>=0)
		return(i);

	BTSHAS_EmitCheckLabelExpand(ctx);

	sec=ctx->seg[ctx->cseg];
	i=ctx->nlbl++;
	lbl=ctx->lbl+i;

	lbl->name=btshas_strdup(name);
//	lbl->offs=sec->ct-sec->cts;
	lbl->offs=0;
	lbl->sect=ctx->cseg;
	lbl->ty_bty=0;
	lbl->ty_dty=0;
	lbl->scl=BTSHAS_LBLSCL_UNDEF_LABEL;
//	lbl->rlty=rlty;
	return(i);
}

int BTSHAS_EmitCommSym(BTSHAS_Context *ctx, char *name, int sz)
{
	BTSHAS_Section *sec;
	BTSHAS_LabelInfo *lbl;
	int i, j;

	i=BTSHAS_LookupLabel(ctx, name);

	if(i>=0)
		return(i);

	BTSHAS_EmitCheckLabelExpand(ctx);

	sec=ctx->seg[ctx->cseg];
	i=ctx->nlbl++;
	lbl=ctx->lbl+i;

	lbl->name=btshas_strdup(name);
	lbl->offs=sz;
	lbl->sect=-1;
	lbl->ty_bty=0;
	lbl->ty_dty=0;
	lbl->scl=BTSHAS_LBLSCL_EXTERN;
	return(i);
}

int BTSHAS_EmitCheckRelocExpand(BTSHAS_Context *ctx)
{
	int i, j, k;

	if((ctx->nrlc+1)<ctx->mrlc)
		return(0);

	if(ctx->rlc==ctx->t_rlc)
	{
		j=ctx->mrlc+(ctx->mrlc>>1);
		ctx->rlc=btshas_mm_malloc(j*sizeof(BTSHAS_RelocInfo));
		memcpy(ctx->rlc, ctx->t_rlc,
			ctx->mrlc*sizeof(BTSHAS_RelocInfo));
		ctx->mrlc=j;
		return(1);
	}
	
	j=ctx->mrlc+(ctx->mrlc>>1);
	ctx->rlc=btshas_mm_realloc(ctx->rlc, j*sizeof(BTSHAS_RelocInfo));
	ctx->mrlc=j;
	return(1);
}

int BTSHAS_EmitReloc(BTSHAS_Context *ctx, char *name, int rlty)
{
	BTSHAS_Section *sec;
	BTSHAS_RelocInfo *rlc;
	int sy;
	int i, j, k;

	BTSHAS_EmitCheckRelocExpand(ctx);

	sy=BTSHAS_EmitUndefSym(ctx, name);
	
	sec=ctx->seg[ctx->cseg];
	i=ctx->nrlc++;
	rlc=ctx->rlc+i;

	rlc->offs=sec->ct-sec->cts;
	rlc->sym=sy;
	rlc->rlty=rlty;
	rlc->sect=ctx->cseg;
	return(i);
}

int BTSHAS_LookupSection(BTSHAS_Context *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->nseg; i++)
	{
		if(!strcmp(ctx->seg[i]->name, name))
		{
			ctx->cseg=i;
			return(i);
		}
	}
	return(-1);
}	

int BTSHAS_EmitSection(BTSHAS_Context *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->nseg; i++)
	{
		if(!strcmp(ctx->seg[i]->name, name))
		{
			ctx->cseg=i;
			return(i);
		}
	}
	
	i=ctx->nseg++;
	ctx->seg[i]=BTSHAS_AllocSection(name);
	ctx->cseg=i;
	return(i);
}


int BTSHAS_EmitListOp(BTSHAS_Context *ctx, BTSHAS_ListingOp *opc,
	BTSHAS_OprVal *arg0, BTSHAS_OprVal *arg1, BTSHAS_OprVal *arg2)
{
	char *s;
	int i, j, k;
	
	s=opc->form;
	while(*s)
	{
		i=0;
		for(j=0; j<4; j++)
		{
			if((s[j]>='0') && (s[j]<='9'))
				{ i|=(s[j]-'0')<<(12-j*4); }
			if((s[j]>='A') && (s[j]<='F'))
				{ i|=(10+s[j]-'A')<<(12-j*4); }
		}

		if(arg0->ty==BTSHAS_OPVTY_IMM)
		{
			if(s[1]=='n')
				{ i|=(arg1->rb&15)<<8; }
			if(s[1]=='m')
				{ i|=(arg1->rb&15)<<8; }
			if(s[1]=='N')
				{ i|=(8|(arg1->rb&7))<<8; }
			if(s[1]=='M')
				{ i|=(8|(arg1->rb&7))<<8; }
			if(s[2]=='n')
				{ i|=(arg1->rb&15)<<4; }
			if(s[2]=='m')
				{ i|=(arg1->rb&15)<<4; }
			if(s[2]=='N')
				{ i|=(8|(arg1->rb&7))<<4; }
			if(s[2]=='M')
				{ i|=(8|(arg1->rb&7))<<4; }
				
			if((s[2]=='i') || (s[2]=='j'))
			{
				if((s[3]=='i') || (s[3]=='j'))
				{
					i|=(arg0->imm)&255;
				}else
				{
					i|=((arg0->imm)&15)<<4;
				}
			}else
			{
				if((s[3]=='i') || (s[3]=='j'))
				{
					i|=(arg0->imm)&15;
				}

				if(s[3]=='J')
				{
					i|=8|((arg0->imm)&7);
				}
			}
		}else
		{
			k=0;
		
			if(((s[1]=='n') || (s[1]=='m') ||
					(s[1]=='N') || (s[1]=='M')) &&
				((s[2]=='n') || (s[2]=='m') ||
					(s[2]=='N') || (s[2]=='M')))
			{
				if(s[1]=='n')
					{ i|=(arg1->rb&15)<<8; }
				if(s[1]=='m')
					{ i|=(arg1->rb&15)<<8; }
				if(s[1]=='N')
					{ i|=(8|(arg1->rb&7))<<8; }
				if(s[1]=='M')
					{ i|=(8|(arg1->rb&7))<<8; }

				if(s[2]=='n')
					{ i|=(arg0->rb&15)<<4; }
				if(s[2]=='m')
					{ i|=(arg0->rb&15)<<4; }
				if(s[2]=='N')
					{ i|=(8|(arg0->rb&7))<<4; }
				if(s[2]=='M')
					{ i|=(8|(arg0->rb&7))<<4; }
			}else
				if(((s[1]=='n') || (s[1]=='m') ||
					(s[1]=='N') || (s[1]=='M')))
			{
				if(s[1]=='n')
					{ i|=(arg0->rb&15)<<8; }
				if(s[1]=='m')
					{ i|=(arg0->rb&15)<<8; }
				if(s[1]=='N')
					{ i|=(8|(arg0->rb&7))<<8; }
				if(s[1]=='M')
					{ i|=(8|(arg0->rb&7))<<8; }
			}else
			{
				if(s[2]=='n')
					{ i|=(arg0->rb&15)<<4; }
				if(s[2]=='m')
					{ i|=(arg0->rb&15)<<4; }
				if(s[2]=='N')
					{ i|=(8|(arg0->rb&7))<<4; }
				if(s[2]=='M')
					{ i|=(8|(arg0->rb&7))<<4; }
			}
		}
		
		if(arg0->ty==BTSHAS_OPVTY_RDMEM)
		{
			if((s[2]=='d') && (s[3]=='d'))
			{
				i|=(arg0->imm)&255;
			}else if(s[3]=='d')
			{
				i|=(arg0->imm)&15;
			}
		}else if(arg1->ty==BTSHAS_OPVTY_RDMEM)
		{
			if((s[2]=='d') && (s[3]=='d'))
			{
				i|=(arg1->imm)&255;
			}else if(s[3]=='d')
			{
				i|=(arg1->imm)&15;
			}
		}

		if(arg0->ty==BTSHAS_OPVTY_NAME)
		{
			k=0;
			if((opc->aty&BTSHAS_OPVTM_TY)==BTSHAS_OPVTM_B)
				k=0;
			if((opc->aty&BTSHAS_OPVTM_TY)==BTSHAS_OPVTM_W)
				k=1;
			if((opc->aty&BTSHAS_OPVTM_TY)==BTSHAS_OPVTM_L)
				k=2;
		
			if((s[1]=='d') && (s[2]=='d') && (s[3]=='d'))
			{
				BTSHAS_EmitReloc(ctx, arg0->name,
					BTSHAS_LBLRLC_SH_PC12W);
			}else if((s[2]=='d') && (s[3]=='d'))
			{
				BTSHAS_EmitReloc(ctx, arg0->name,
					(k==2)?BTSHAS_LBLRLC_SH_PC8L:BTSHAS_LBLRLC_SH_PC8W);
			}else if(s[3]=='d')
			{
			}
		}

		BTSHAS_EmitWord(ctx, i);
		s+=4;
	}
	return(0);
}

int BTSHAS_GetSectionWord(BTSHAS_Context *ctx, int seg, int offs)
{
	int i;

	if(ctx->is_le)
	{
		i=	(ctx->seg[seg]->cts[offs+0]   )|
			(ctx->seg[seg]->cts[offs+1]<<8);
		return(i);
	}else
	{
		i=	(ctx->seg[seg]->cts[offs+0]<<8)|
			(ctx->seg[seg]->cts[offs+1]   );
		return(i);
	}
}

int BTSHAS_SetSectionWord(BTSHAS_Context *ctx,
	int seg, int offs, int val)
{
	if(ctx->is_le)
	{
		ctx->seg[seg]->cts[offs+0]=val   ;
		ctx->seg[seg]->cts[offs+1]=val>>8;
	}else
	{
		ctx->seg[seg]->cts[offs+0]=val>>8;
		ctx->seg[seg]->cts[offs+1]=val   ;
	}
	return(0);
}

u32 BTSHAS_GetSectionDWord(BTSHAS_Context *ctx, int seg, int offs)
{
	int i;

	if(ctx->is_le)
	{
		i=	(ctx->seg[seg]->cts[offs+0]    ) |
			(ctx->seg[seg]->cts[offs+1]<< 8) |
			(ctx->seg[seg]->cts[offs+2]<<16) |
			(ctx->seg[seg]->cts[offs+3]<<24);
		return(i);
	}else
	{
		i=	(ctx->seg[seg]->cts[offs+0]<<24)|
			(ctx->seg[seg]->cts[offs+1]<<16)|
			(ctx->seg[seg]->cts[offs+2]<< 8)|
			(ctx->seg[seg]->cts[offs+3]    );
		return(i);
	}
}

int BTSHAS_SetSectionDWord(BTSHAS_Context *ctx,
	int seg, int offs, u32 val)
{
	if(ctx->is_le)
	{
		ctx->seg[seg]->cts[offs+0]=val    ;
		ctx->seg[seg]->cts[offs+1]=val>> 8;
		ctx->seg[seg]->cts[offs+2]=val>>16;
		ctx->seg[seg]->cts[offs+3]=val>>24;
	}else
	{
		ctx->seg[seg]->cts[offs+0]=val>>24;
		ctx->seg[seg]->cts[offs+0]=val>>16;
		ctx->seg[seg]->cts[offs+0]=val>> 8;
		ctx->seg[seg]->cts[offs+1]=val    ;
	}
	return(0);
}

int BTSHAS_FixupLocal(BTSHAS_Context *ctx)
{
	BTSHAS_LabelInfo *lbl;
	BTSHAS_RelocInfo *rlc, *rlc2;
	int i, j, k;

	for(i=0; i<ctx->nrlc; i++)
	{
		rlc=ctx->rlc+i;
		lbl=ctx->lbl+rlc->sym;

		if(lbl->scl==BTSHAS_LBLSCL_REDIRECT)
		{
			lbl=ctx->lbl+lbl->offs;
		}

		if(lbl->scl==BTSHAS_LBLSCL_EXTERNAL_DEF)
			continue;
		if(lbl->scl==BTSHAS_LBLSCL_UNDEF_LABEL)
		{
			printf("BTSHAS_FixupLocal: Undeclared '%s'\n",
				lbl->name);
			continue;
		}
		
		switch(rlc->rlty)
		{
		case BTSHAS_LBLRLC_SH_PC8W:
		case BTSHAS_LBLRLC_SH_PC12W:
			if(lbl->sect==rlc->sect)
			{
				j=lbl->offs-(rlc->offs+4);
				j=j>>1;
				k=BTSHAS_GetSectionWord(ctx, rlc->sect, rlc->offs);
				if(rlc->rlty==BTSHAS_LBLRLC_SH_PC12W)
					k=(k&0xF000)|(j&0x0FFF);
				else
					k=(k&0xFF00)|(j&0x00FF);
				BTSHAS_SetSectionWord(ctx, rlc->sect, rlc->offs, k);
				rlc->rlty=BTSHAS_LBLRLC_SH_NONE;
			}
			break;
		case BTSHAS_LBLRLC_SH_PC8L:
			if(lbl->sect==rlc->sect)
			{
				j=lbl->offs-((rlc->offs+4)&(~3));
				j=j>>2;
				k=((rlc->offs+4)&(~3))+(j*4);
				if(k!=lbl->offs)
				{
					printf("BTSHAS_FixupLocal:"
						"Sanity Fail %02X, %04X!=%04X\n",
						rlc->rlty, k, lbl->offs);
				}
				k=BTSHAS_GetSectionWord(ctx, rlc->sect, rlc->offs);
				k=(k&0xFF00)|(j&0x00FF);
				BTSHAS_SetSectionWord(ctx, rlc->sect, rlc->offs, k);
				rlc->rlty=BTSHAS_LBLRLC_SH_NONE;
			}
			break;

		case BTSHAS_LBLRLC_SH_REL32:
			if(lbl->sect==rlc->sect)
			{
				j=lbl->offs-rlc->offs;
				k=BTSHAS_GetSectionDWord(ctx, rlc->sect, rlc->offs);
				k=k+j;
				BTSHAS_SetSectionDWord(ctx, rlc->sect, rlc->offs, k);
				rlc->rlty=BTSHAS_LBLRLC_SH_NONE;
			}
			break;
		case BTSHAS_LBLRLC_SH_REL32B:
			if(lbl->sect==rlc->sect)
			{
				j=lbl->offs-rlc->offs;
				k=BTSHAS_GetSectionDWord(ctx, rlc->sect, rlc->offs);
				k=k-j;
				BTSHAS_SetSectionDWord(ctx, rlc->sect, rlc->offs, k);
				rlc->rlty=BTSHAS_LBLRLC_SH_NONE;
			}
			break;

		case BTSHAS_LBLRLC_SH_DIR16:
		case BTSHAS_LBLRLC_SH_DIR32:
			break;

		default:
			printf("BTSHAS_FixupLocal: Unhandled Reloc %02X\n", rlc->rlty);
			break;
		}
	}

	/* Collapse relocs */
	k=0;
	for(i=0; i<ctx->nrlc; i++)
	{
		rlc=ctx->rlc+i;
		rlc2=ctx->rlc+k;
		if(rlc->rlty==BTSHAS_LBLRLC_SH_NONE)
			continue;
		*rlc2=*rlc;
		k++;
	}
	ctx->nrlc=k;

	return(0);
}


int BTSHAS_FixupImage(BTSHAS_Context *ctx)
{
	BTSHAS_LabelInfo *lbl;
	BTSHAS_RelocInfo t_rlc;
	BTSHAS_RelocInfo *rlc, *rlc2, *trlc;
	u32 lrva, rrva;
	int i, j, k;

	for(i=0; i<ctx->nrlc; i++)
	{
		rlc=ctx->rlc+i;
		lbl=ctx->lbl+rlc->sym;

		if(!rlc->rlty)
			continue;

		if((lbl->scl==BTSHAS_LBLSCL_UNDEF_LABEL) ||
			(lbl->scl==BTSHAS_LBLSCL_EXTERNAL_DEF))
		{
			printf("BTSHAS_FixupImage: Undeclared '%s'\n",
				lbl->name);
			continue;
		}

		lrva=lbl->offs+ctx->seg[lbl->sect]->rva_base;
		rrva=rlc->offs+ctx->seg[rlc->sect]->rva_base;

		switch(rlc->rlty)
		{
		case BTSHAS_LBLRLC_SH_DIR32:
		case BTSHAS_LBLRLC_SH_DIR32_NB:
			j=lrva;
			k=BTSHAS_GetSectionDWord(ctx, rlc->sect, rlc->offs);
			k=k+j;
			BTSHAS_SetSectionDWord(ctx, rlc->sect, rlc->offs, k);
			rlc->rlty=BTSHAS_LBLRLC_SH_DIR32B;
			break;
		case BTSHAS_LBLRLC_SH_DIR16:
			j=lrva;
			k=BTSHAS_GetSectionWord(ctx, rlc->sect, rlc->offs);
			k=k+j;
			BTSHAS_SetSectionWord(ctx, rlc->sect, rlc->offs, k);
			rlc->rlty=BTSHAS_LBLRLC_SH_DIR16B;
			break;

		case BTSHAS_LBLRLC_SH_REL32:
			j=lrva-rrva;
			k=BTSHAS_GetSectionDWord(ctx, rlc->sect, rlc->offs);
			k=k+j;
			BTSHAS_SetSectionDWord(ctx, rlc->sect, rlc->offs, k);
			rlc->rlty=BTSHAS_LBLRLC_SH_NONE;
			break;
		case BTSHAS_LBLRLC_SH_REL32B:
			j=lrva-rrva;
			k=BTSHAS_GetSectionDWord(ctx, rlc->sect, rlc->offs);
			k=k-j;
			BTSHAS_SetSectionDWord(ctx, rlc->sect, rlc->offs, k);
			rlc->rlty=BTSHAS_LBLRLC_SH_NONE;
			break;

		default:
			printf("BTSHAS_FixupLocal: Unhandled Reloc %02X\n", rlc->rlty);
			break;
		}
	}

	/* Collapse relocs */
	k=0;
	for(i=0; i<ctx->nrlc; i++)
	{
		rlc=ctx->rlc+i;
		rlc2=ctx->rlc+k;
		if(rlc->rlty==BTSHAS_LBLRLC_SH_NONE)
			continue;
		*rlc2=*rlc;
		k++;
	}
	ctx->nrlc=k;

	/* Sort remaining relocs by RVA */
	trlc=&t_rlc;
	for(i=0; i<ctx->nrlc; i++)
	{
		rlc=ctx->rlc+i;
		for(j=i+1; j<ctx->nrlc; j++)
		{
			rlc2=ctx->rlc+j;

			lrva=rlc->offs+ctx->seg[rlc->sect]->rva_base;
			rrva=rlc2->offs+ctx->seg[rlc2->sect]->rva_base;
			
			if(rrva<lrva)
			{
				*trlc=*rlc;
				*rlc=*rlc2;
				*rlc2=*trlc;
			}
		}
	}

	return(0);
}
