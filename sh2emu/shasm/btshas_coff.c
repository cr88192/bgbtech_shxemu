int btshas_coff_getwordptr_le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
int btshas_coff_getwordptr_be(byte *ptr)
	{ return((ptr[0]<<8)|ptr[1]); }

u32 btshas_coff_getdwordptr_le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
u32 btshas_coff_getdwordptr_be(byte *ptr)
	{ return((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|ptr[3]); }

void btshas_coff_setwordptr_le(byte *ptr, int val)
	{ ptr[0]=val; ptr[1]=val>>8; }
void btshas_coff_setwordptr_be(byte *ptr, int val)
	{ ptr[0]=val>>8; ptr[1]=val; }

void btshas_coff_setdwordptr_le(byte *ptr, u32 val)
	{ ptr[0]=val; ptr[1]=val>>8; ptr[2]=val>>16; ptr[3]=val>>24; }
void btshas_coff_setdwordptr_be(byte *ptr, u32 val)
	{ ptr[0]=val>>24; ptr[1]=val>>16; ptr[2]=val>>8; ptr[3]=val; }

int BTSHAS_FlattenCOFF_SectionFlags(BTSHAS_Context *ctx, char *name)
{
	if(!strcmp(name, ".text"))
	{
		return(BTSHAS_COFF_SCNT_CODE|
			BTSHAS_COFF_SCNT_EXECUTE|
			BTSHAS_COFF_SCNT_READ);
	}
	if(!strcmp(name, ".data") ||
		!strcmp(name, ".idata") ||
		!strcmp(name, ".vsdata") ||
		!strcmp(name, ".tls") ||
		!strcmp(name, ".sdata"))
	{
		return(BTSHAS_COFF_SCNT_IDATA|
			BTSHAS_COFF_SCNT_READ|
			BTSHAS_COFF_SCNT_WRITE);
	}
	if(!strcmp(name, ".bss") ||
		!strcmp(name, ".sbss"))
	{
		return(BTSHAS_COFF_SCNT_UDATA|
			BTSHAS_COFF_SCNT_READ|
			BTSHAS_COFF_SCNT_WRITE);
	}
	if(!strcmp(name, ".rdata") ||
		!strcmp(name, ".edata") ||
		!strcmp(name, ".xdata") ||
		!strcmp(name, ".srdata") ||
		!strcmp(name, ".rsrc"))
	{
		return(BTSHAS_COFF_SCNT_IDATA|
			BTSHAS_COFF_SCNT_READ);
	}
	if(!strcmp(name, ".reloc"))
	{
		return(BTSHAS_COFF_SCNT_IDATA|
			BTSHAS_COFF_SCNT_READ|
			BTSHAS_COFF_SCNT_DISCARD);
	}

	return(BTSHAS_COFF_SCNT_IDATA|
		BTSHAS_COFF_SCNT_READ|
		BTSHAS_COFF_SCNT_WRITE);
//	return(0);
}

int BTSHAS_FlattenCOFF(BTSHAS_Context *ctx, byte *obuf, int obmsz)
{
	void (*setwordptr)(byte *ptr, int val);
	void (*setdwordptr)(byte *ptr, u32 val);
	BTSHAS_LabelInfo *lbl;
	BTSHAS_RelocInfo *rlc;
	byte *head, *secs;
	byte *ct, *ctt0, *ctt1;
	byte *strs, *stre;
	int ofd, szd, ofr, nr, szr, dfl;
	int ofst, szst, nst, attr;
	int i, j, k;
	
	attr=0;
	if(ctx->is_le)
	{
		setwordptr=btshas_coff_setwordptr_le;
		setdwordptr=btshas_coff_setdwordptr_le;
		attr|=0x80;
	}else
	{
		setwordptr=btshas_coff_setwordptr_be;
		setdwordptr=btshas_coff_setdwordptr_be;
		attr|=0x8000;
	}
	
	ct=obuf;
	head=obuf;
	setwordptr(head+0, BTSHAS_COFF_MACH_SH4);	//machine
	setwordptr(head+2, ctx->nseg);				//number of sections
	setdwordptr(head+4, 0);						//time/date
	setdwordptr(head+8, 0);						//pointer to symbol table
	setdwordptr(head+12, 0);					//number of symbols
	setwordptr(head+16, 0);						//size of optional header
	setwordptr(head+18, attr);					//flags
	
	secs=head+20;
	ct=secs+ctx->nseg*40;

	j=ct-obuf;
	ct=obuf+((j+3)&(~3));

	for(i=0; i<ctx->nseg; i++)
	{
		ctt0=secs+i*40;
		
		szd=ctx->seg[i]->ct-ctx->seg[i]->cts;
		ofd=ct-obuf;
		ct+=(szd+3)&(~3);

		nr=0;
		for(j=0; j<ctx->nrlc; j++)
		{
			rlc=ctx->rlc+j;
			if(!rlc->rlty || (rlc->sect!=i))
				continue;
			nr++;
		}
		szr=nr*10;
		ofr=ct-obuf;
		ct+=(szr+3)&(~3);
		
		dfl=BTSHAS_FlattenCOFF_SectionFlags(ctx, ctx->seg[i]->name);
		
		strncpy(ctt0+0, ctx->seg[i]->name, 8);
		setdwordptr(ctt0+8, 0);					//virtual size
		setdwordptr(ctt0+12, 0);				//virtual address
		setdwordptr(ctt0+16, szd);				//size of raw data
		setdwordptr(ctt0+20, ofd);				//pointer to raw data
		setdwordptr(ctt0+24, ofr);				//pointer to relocs
		setdwordptr(ctt0+28, 0);				//pointer to line numbers
		setwordptr(ctt0+32, nr);				//number of relocs
		setwordptr(ctt0+34, 0);					//number of line numbers
		setdwordptr(ctt0+36, dfl);				//flags
		
		memcpy(obuf+ofd, ctx->seg[i]->cts, szd);

		k=0;
		for(j=0; j<ctx->nrlc; j++)
		{
			rlc=ctx->rlc+j;
			if(!rlc->rlty || (rlc->sect!=i))
				continue;
			ctt1=obuf+ofr+((k++)*10);
			setdwordptr(ctt1+0, rlc->offs);		//virtual size
			setdwordptr(ctt1+4, rlc->sym);		//sym table index
			setwordptr(ctt1+8, rlc->rlty);		//reloc type
		}
	}

	j=ct-obuf;
	ct=obuf+((j+3)&(~3));
	
	nst=ctx->nlbl;
	szst=nst*18;
	ofst=ct-obuf;
//	ct+=(szst+3)&(~3);
	ct+=szst;
	strs=ct;
	stre=ct+5;

	setdwordptr(head+8, ofst);				//pointer to symbol table
	setdwordptr(head+12, nst);				//number of symbols
	for(i=0; i<ctx->nlbl; i++)
	{
		ctt0=obuf+ofst+(i*18);
		lbl=ctx->lbl+i;
	
		if(strlen(lbl->name)>8)
		{
			j=stre-strs;
			strcpy(stre, lbl->name);
			stre=stre+strlen(lbl->name)+1;
			
			setdwordptr(ctt0+0, 0);			//zero
			setdwordptr(ctt0+0, j);			//string table offset
		}else
		{
			strncpy(ctt0, lbl->name, 8);
		}
		
		setdwordptr(ctt0+8, lbl->offs);		//value (offset)
		setwordptr(ctt0+12, lbl->sect+1);	//section
		
		j=(lbl->ty_dty<<8)|lbl->ty_bty;
		setwordptr(ctt0+14, j);				//type
		ctt0[16]=lbl->scl;					//storage class
		ctt0[17]=0;
	}

	setdwordptr(strs, stre-strs);			//size of string table
	
	j=stre-obuf;
	ct=obuf+((j+3)&(~3));
	
	return(ct-obuf);
}

int BTSHAS_LoadBufferCOFF(BTSHAS_Context *ctx, byte *ibuf, int ibsz)
{
	int (*getwordptr)(byte *ptr);
	u32 (*getdwordptr)(byte *ptr);
	void (*setwordptr)(byte *ptr, int val);
	void (*setdwordptr)(byte *ptr, u32 val);
	char tbn[256];
	int bseg[64];
	int cseg[64];
	BTSHAS_Section *sec;
	BTSHAS_LabelInfo *lbl;
	BTSHAS_RelocInfo *rlc;
	byte *head, *secs, *syms;
	byte *strs, *stre;
	byte *cs, *cs0, *cs1;
	int mach, nseg;
	int ofd, szd, ofr, nr, szr, csg;
	int ofst, szst, nst, ofv;
	int bseg0, blbl0, en, attr;
	int i, j, k;

	attr=0;
	en=ctx->is_le;
	mach=btshas_coff_getwordptr_le(ibuf+0);
	if((mach==BTSHAS_COFF_MACH_SH4) ||
		(mach==BTSHAS_COFF_MACH_SH3))
			en=1;

	if(!ctx->nseg)
	{
//		ctx->is_le=0;
//		mach=btshas_coff_getwordptr_le(ibuf+0);
//		if((mach==BTSHAS_COFF_MACH_SH4) ||
//			(mach==BTSHAS_COFF_MACH_SH3))
//				ctx->is_le=1;
		ctx->is_le=en;
	}

//	if(ctx->is_le)
	if(en)
	{
		getwordptr=btshas_coff_getwordptr_le;
		getdwordptr=btshas_coff_getdwordptr_le;
		setwordptr=btshas_coff_setwordptr_le;
		setdwordptr=btshas_coff_setdwordptr_le;
	}else
	{
		getwordptr=btshas_coff_getwordptr_be;
		getdwordptr=btshas_coff_getdwordptr_be;
		setwordptr=btshas_coff_setwordptr_be;
		setdwordptr=btshas_coff_setdwordptr_be;
	}

	head=ibuf;
	secs=head+20;

	mach=getwordptr(head+0);					//machine
	nseg=getwordptr(head+2);					//number of sections
	i=getdwordptr(head+4);						//time/date
	ofst=getdwordptr(head+8);					//pointer to symbol table
	nst=getdwordptr(head+12);					//number of symbols
	i=getwordptr(head+16);						//size of optional header
	attr=getwordptr(head+18);					//flags

	if(!ctx->nseg)
	{
		if(attr&0x0080)
			ctx->is_le=1;
		if(attr&0x8000)
			ctx->is_le=0;
	}else
	{
		if(	((attr&0x0080) && !ctx->is_le) ||
			((attr&0x8000) &&  ctx->is_le))
		{
			printf("BTSHAS_LoadBufferCOFF: Endian Mismatch\n");
			return(-1);
		}
	}

	syms=ibuf+ofst;
	strs=syms+nst*18;

	if((mach!=BTSHAS_COFF_MACH_SH4) &&
		(mach!=BTSHAS_COFF_MACH_SH3))
	{
		printf("BTSHAS_LoadBufferCOFF: Bad Machine %04X\n", mach);
		return(-1);
	}
	
	blbl0=ctx->nlbl;
	
	for(i=0; i<nseg; i++)
	{
		cs=secs+i*40;
		memcpy(tbn, cs+0, 8);
		tbn[8]=0;
		BTSHAS_EmitSection(ctx, tbn);

		BTSHAS_EmitAlign(ctx, 4);

		sec=ctx->seg[ctx->cseg];
		bseg0=sec->ct-sec->cts;
		bseg[i]=bseg0;
		cseg[i]=ctx->cseg;

		szd=getdwordptr(cs+16);				//size of raw data
		ofd=getdwordptr(cs+20);				//pointer to raw data
		ofr=getdwordptr(cs+24);				//pointer to relocs
		nr=getwordptr(cs+32);				//number of relocs

		if(((ofd+szd)>ibsz) || ((ofd+szd)<ofd))
		{
			printf("BTSHAS_LoadBufferCOFF: Section Sanity Fail "
				"Offs=%08X Sz=%08X ImgSz=%08X\n", ofd, szd, ibsz);
		}

		BTSHAS_EmitRawBytes(ctx, ibuf+ofd, szd);
		
		for(j=0; j<nr; j++)
		{
			cs1=ibuf+ofr+j*10;

			BTSHAS_EmitCheckRelocExpand(ctx);

			k=ctx->nrlc++;
			rlc=ctx->rlc+k;

			rlc->offs=getdwordptr(cs1+0);		//virtual size
			rlc->sym=getdwordptr(cs1+4);		//sym table index
			rlc->rlty=getwordptr(cs1+8);		//reloc type
			rlc->offs+=bseg0;
			rlc->sym+=blbl0;
			rlc->sect=ctx->cseg;
		}
	}
	
	for(i=0; i<nst; i++)
	{
		BTSHAS_EmitCheckLabelExpand(ctx);
		k=ctx->nlbl++;
		lbl=ctx->lbl+k;

		cs=syms+i*18;
		
		if(cs[0])
		{
			memcpy(tbn, cs, 8);
			tbn[8]=0;
		}else
		{
			j=getdwordptr(cs+4);
			strncpy(tbn, strs+j, 254);
		}

		ofv=getdwordptr(cs+8);		//value (offset)
		csg=getwordptr(cs+12);		//section

		if(csg>0)
		{
			lbl->offs=ofv+bseg[csg-1];
			lbl->sect=cseg[csg-1];
		}else
		{
			lbl->offs=ofv;
			lbl->sect=csg-1;
		}

		lbl->name=btshas_strdup(tbn);
		lbl->scl=cs[16];			//storage class
		
		j=getwordptr(cs+14);		//type
		lbl->ty_dty=j>>8;
		lbl->ty_bty=j;
	}
	
	return(0);
}

int BTSHAS_LoadRebind(BTSHAS_Context *ctx)
{
	BTSHAS_Section *sec;
	BTSHAS_LabelInfo *lbl, *lbl2;
	BTSHAS_RelocInfo *rlc;
	int i, j, k;

	/* Merge symbols towards lower entries */
	for(i=0; i<ctx->nlbl; i++)
	{
		lbl=ctx->lbl+i;
		
		if((lbl->scl==BTSHAS_LBLSCL_EXTERNAL_DEF) ||
			(lbl->scl==BTSHAS_LBLSCL_UNDEF_LABEL) ||
			(lbl->sect<0))
		{
//			if(lbl->scl==BTSHAS_LBLSCL_EXTERN)
//				continue;
		
			j=BTSHAS_LookupDefinedLabel(ctx, lbl->name);
			if(j>=0)
			{
				lbl2=ctx->lbl+j;
				
				if(i>j)
				{
					lbl->offs=j;
					lbl->scl=BTSHAS_LBLSCL_REDIRECT;
					lbl->sect=lbl2->sect;
				}else
				{
					lbl->offs=lbl2->offs;
					lbl->sect=lbl2->sect;
					lbl->ty_bty=lbl2->ty_bty;
					lbl->ty_dty=lbl2->ty_dty;
					lbl->scl=lbl2->scl;

					lbl2->offs=i;
					lbl2->scl=BTSHAS_LBLSCL_REDIRECT;
				}
			}
		}
	}

	/* Fixup Relocs */
	for(i=0; i<ctx->nrlc; i++)
	{
		rlc=ctx->rlc+i;
		lbl=ctx->lbl+rlc->sym;

		k=16;
		while((lbl->scl==BTSHAS_LBLSCL_REDIRECT) && ((k--)>0))
		{
			rlc->sym=lbl->offs;
			lbl=ctx->lbl+rlc->sym;
		}
	}
	
	/* Eliminate tailing redirects */
	i=ctx->nlbl-1;
	lbl=ctx->lbl+i;
	while((i>0) && (lbl->scl==BTSHAS_LBLSCL_REDIRECT))
	{
		i--;
		lbl=ctx->lbl+i;
	}
	ctx->nlbl=i;
	
	BTSHAS_FixupLocal(ctx);
	
	return(0);
}

int BTSHAS_RebindComm(BTSHAS_Context *ctx)
{
	BTSHAS_Section *sec;
	BTSHAS_LabelInfo *lbl, *lbl2;
	BTSHAS_RelocInfo *rlc;
	int i, j, k, l;

	/* Merge symbols towards lower entries */
	for(i=0; i<ctx->nlbl; i++)
	{
		lbl=ctx->lbl+i;
		
		if((lbl->scl==BTSHAS_LBLSCL_EXTERN) && (lbl->sect<0))
		{
			j=BTSHAS_LookupDefinedLabel(ctx, lbl->name);
			if(j>=0)
			{
				lbl2=ctx->lbl+j;
				
				if(i>j)
				{
					lbl->offs=j;
					lbl->scl=BTSHAS_LBLSCL_REDIRECT;
					lbl->sect=lbl2->sect;
				}else
				{
					lbl->offs=lbl2->offs;
					lbl->sect=lbl2->sect;
					lbl->ty_bty=lbl2->ty_bty;
					lbl->ty_dty=lbl2->ty_dty;
					lbl->scl=lbl2->scl;
					lbl2->offs=i;
					lbl2->scl=BTSHAS_LBLSCL_REDIRECT;
				}
			}

			l=lbl->offs;
		
			j=ctx->cseg;
			BTSHAS_EmitSection(ctx, ".bss");
			sec=ctx->seg[ctx->cseg];
			BTSHAS_EmitAlign(ctx, 4);
			k=sec->ct-sec->cts;
//			BTSHAS_EmitLabel(ctx, tk0+1);
			BTSHAS_EmitRawBytes(ctx, NULL, l);

			lbl->offs=k;
			lbl->sect=ctx->cseg;

			ctx->cseg=j;
		}
	}
	
	BTSHAS_LoadRebind(ctx);
}



int BTSHAS_FlattenPECOFF(BTSHAS_Context *ctx, byte *obuf, int obmsz)
{
	void (*setwordptr)(byte *ptr, int val);
	void (*setdwordptr)(byte *ptr, u32 val);
//	int segrva[256];
	BTSHAS_Section *sec;
	BTSHAS_LabelInfo *lbl;
	BTSHAS_RelocInfo *rlc;
	byte *head, *secs, *opthead;
	byte *ct, *ctt0, *ctt1;
	byte *strs, *stre;
	u32 lrva, rrva, imgbase, imgsz, imghsz;
	int ofd, szd, ofr, nr, szr, szd2;
	int szrlc, rvarlc, lpg, dfl;
	int ofst, szst, nst, attr, mach;
	int i, j, k;
	
	attr=0;
//	if(ctx->is_le)
	if(1)
//	if(0)
	{
		setwordptr=btshas_coff_setwordptr_le;
		setdwordptr=btshas_coff_setdwordptr_le;
//		attr|=0x80;
	}else
	{
		setwordptr=btshas_coff_setwordptr_be;
		setdwordptr=btshas_coff_setdwordptr_be;
//		attr|=0x8000;
	}

	if(ctx->is_le)
	{
		attr|=0x80;
	}else
	{
		attr|=0x8000;
	}
	
	mach=BTSHAS_COFF_MACH_SH4;
	mach=0x14C;
	
	memset(obuf, 0, 256);
	
	obuf[0x00]='M';
	obuf[0x01]='Z';

//	obuf[0x3A]='M';
//	obuf[0x3B]='Z';
//	btshas_coff_setdwordptr_le(obuf+0x3C, 0x40);

//	obuf[0x3A]='M';
//	obuf[0x3B]='Z';
	setwordptr(obuf+0x3A, 0x5A4D);
	setdwordptr(obuf+0x3C, 0x40);
	
	obuf[0x40]='P';
	obuf[0x41]='E';

	ct=obuf+0x44;
	head=obuf+0x44;
	setwordptr(head+0, mach);					//machine
	setwordptr(head+2, ctx->nseg);				//number of sections
//	setdwordptr(head+4, 0);						//time/date
//	setdwordptr(head+4, 0xAA55AA55UL);			//time/date
	setdwordptr(head+4, 0x55AA55AAUL);			//time/date
	setdwordptr(head+8, 0);						//pointer to symbol table
	setdwordptr(head+12, 0);					//number of symbols
	setwordptr(head+16, 0x90);					//size of optional header
	setwordptr(head+18, attr);					//flags
	
	opthead=head+20;
	secs=head+20+0x90;
//	ct=secs+ctx->nseg*40;
	ct=secs+(ctx->nseg+4)*40;

//	imgbase=0x00010000;
	imgbase=0x00000000;
	imgsz=0;

	memset(opthead, 0, 0xF0);

	j=ct-obuf;
//	ct=obuf+((j+3)&(~3));
	ct=obuf+((j+4095)&(~4095));

	imghsz=ct-obuf;


	for(i=0; i<ctx->nseg; i++)
	{
		if(!strcmp(ctx->seg[i]->name, ".bss"))
			continue;
	
		szd=ctx->seg[i]->ct-ctx->seg[i]->cts;
		ofd=ct-obuf;
//		ct+=(szd+3)&(~3);
		ct+=(szd+4095)&(~4095);
		szd2=(szd+4095)&(~4095);
		
//		segrva[i]=ofd;
		
		ctx->seg[i]->rva_base=ofd;
		ctx->seg[i]->rva_size=szd2;

		if((ofd+szd2)>imgsz)
			imgsz=ofd+szd2;
	}

	BTSHAS_FixupImage(ctx);

	if(ctx->nrlc>0)
	{
		lpg=-1; szrlc=0;
		for(i=0; i<ctx->nrlc; i++)
		{
			rlc=ctx->rlc+i;
			lrva=rlc->offs+ctx->seg[rlc->sect]->rva_base;
			
			if((lrva>>12)!=lpg)
			{
				szrlc=(szrlc+3)&(~3);
				szrlc+=8;
				lpg=lrva>>12;
			}
			szrlc+=2;
		}

		BTSHAS_EmitSection(ctx, ".reloc");
		sec=ctx->seg[ctx->cseg];
		BTSHAS_EmitAlign(ctx, 4);
		k=sec->ct-sec->cts;
		ofr=k;
		BTSHAS_EmitRawBytes(ctx, NULL, szrlc+256);

		j=ctx->cseg;
		szd=szrlc;
		ofd=ct-obuf;
		szd2=(szd+4095)&(~4095);
		ct+=szd2;
		ctx->seg[j]->rva_base=ofd;
		ctx->seg[j]->rva_size=szd2;
		if((ofd+szd2)>imgsz)
			imgsz=ofd+szd2;

		rvarlc=ofd+ofr;

		ctt0=ctx->seg[j]->cts+ofr; ctt1=ctt0;
		lpg=-1;
		for(i=0; i<ctx->nrlc; i++)
		{
			rlc=ctx->rlc+i;
			lrva=rlc->offs+ctx->seg[rlc->sect]->rva_base;
			
			if((lrva>>12)!=lpg)
			{
//				j=ctt1-ctx->seg[ctx->cseg]->cts;
				j=ctt1-sec->cts;
				j=(j+3)&(~3);
				ctt1=sec->cts+j;
			
				setdwordptr(ctt0+4, ctt1-ctt0);

				ctt0=ctt1;
				ctt1=ctt0+8;
				lpg=lrva>>12;
				setdwordptr(ctt0+0, lpg<<12);
				setdwordptr(ctt0+4, 8);
			}

			if((rlc->rlty==BTSHAS_LBLRLC_SH_DIR32)||
				(rlc->rlty==BTSHAS_LBLRLC_SH_DIR32B))
			{
				setwordptr(ctt1, 0x3000|(lrva&0x0FFF));
				ctt1+=2;
			}

			if((rlc->rlty==BTSHAS_LBLRLC_SH_DIR16)||
				(rlc->rlty==BTSHAS_LBLRLC_SH_DIR16B))
			{
				setwordptr(ctt1, 0x2000|(lrva&0x0FFF));
				ctt1+=2;
			}
//			szrlc+=2;
		}

//		j=ctt1-ctx->seg[ctx->cseg]->cts;
		j=ctt1-sec->cts;
		j=(j+3)&(~3);	
		ctt1=sec->cts+j;
		setdwordptr(ctt0+4, ctt1-ctt0);
		
		sec->ct=ctt1;
	}

	setwordptr(head+2, ctx->nseg);				//number of sections

	for(i=0; i<ctx->nseg; i++)
	{
		ctt0=secs+i*40;

		szd=ctx->seg[i]->ct-ctx->seg[i]->cts;
		ofd=ctx->seg[i]->rva_base;
		szd2=ctx->seg[i]->rva_size;

		if(!strcmp(ctx->seg[i]->name, ".bss"))
		{
			szd=ctx->seg[i]->ct-ctx->seg[i]->cts;
			ofd=ct-obuf;
//			ct+=(szd+4095)&(~4095);
			szd2=(szd+4095)&(~4095);
			
			ctx->seg[i]->rva_base=ofd;
			ctx->seg[i]->rva_size=szd2;

			if((ofd+szd2)>imgsz)
				imgsz=ofd+szd2;
				
			szd=0;
		}
		
		printf("Sec %s ofd=%08X szd=%08X szd2=%08X\n",
			ctx->seg[i]->name, ofd, szd, szd2);

		dfl=BTSHAS_FlattenCOFF_SectionFlags(ctx, ctx->seg[i]->name);
		
		strncpy(ctt0+0, ctx->seg[i]->name, 8);
		setdwordptr(ctt0+ 8, szd2);				//virtual size
		setdwordptr(ctt0+12, ofd);				//virtual address
		setdwordptr(ctt0+16, szd);				//size of raw data
		setdwordptr(ctt0+20, ofd);				//pointer to raw data
		setdwordptr(ctt0+24, 0);				//pointer to relocs
		setdwordptr(ctt0+28, 0);				//pointer to line numbers
		setwordptr (ctt0+32, 0);				//number of relocs
		setwordptr (ctt0+34, 0);				//number of line numbers
		setdwordptr(ctt0+36, dfl);				//flags
		
		memcpy(obuf+ofd, ctx->seg[i]->cts, szd);
	}



	setwordptr(opthead+0x00, 0x10B);			//number of sections
	opthead[0x02]=0;
	opthead[0x03]=0;
	i=BTSHAS_LookupSection(ctx, ".text");
	j=(i>=0)?ctx->seg[i]->rva_size:0;
	setdwordptr(opthead+0x04, j);			//
	i=BTSHAS_LookupSection(ctx, ".data");
	j=(i>=0)?ctx->seg[i]->rva_size:0;
	setdwordptr(opthead+0x08, j);			//
	i=BTSHAS_LookupSection(ctx, ".bss");
	j=(i>=0)?ctx->seg[i]->rva_size:0;
	setdwordptr(opthead+0x0C, j);			//

	i=BTSHAS_LookupDefinedLabel(ctx, "___start");
	if(i<0)
		i=BTSHAS_LookupDefinedLabel(ctx, "__start");
	if(i<0)
		i=BTSHAS_LookupDefinedLabel(ctx, "_start");
	if(i<0)
		i=BTSHAS_LookupDefinedLabel(ctx, "start");
	if(i>=0)
	{
		lbl=ctx->lbl+i;
		j=lbl->offs+ctx->seg[lbl->sect]->rva_base;
		setdwordptr(opthead+0x10, j);			//EntryPoint
	}

	i=BTSHAS_LookupSection(ctx, ".text");
	j=(i>=0)?ctx->seg[i]->rva_base:0;
	setdwordptr(opthead+0x14, j);			//BaseOfCode

	i=BTSHAS_LookupSection(ctx, ".data");
	j=(i>=0)?ctx->seg[i]->rva_base:0;
	setdwordptr(opthead+0x18, j);			//BaseOfData

	setdwordptr(opthead+0x1C, imgbase);		//ImageBase
	setdwordptr(opthead+0x20, 4096);		//SectionAlignment
	setdwordptr(opthead+0x24, 4096);		//FileAlignment
	setwordptr (opthead+0x28, 0);			//MajorOsVersion
	setwordptr (opthead+0x2A, 0);			//MinorOsVersion
	setwordptr (opthead+0x2C, 0);			//MajorImageVersion
	setwordptr (opthead+0x2E, 0);			//MinorImageVersion
	setwordptr (opthead+0x30, 0);			//MajorSubsystemVersion
	setwordptr (opthead+0x32, 0);			//MinorSubsystemVersion
	setdwordptr(opthead+0x34, 0);			//Win32Version
	setdwordptr(opthead+0x38, imgsz);		//SizeOfImage
	setdwordptr(opthead+0x3C, imghsz);		//SizeOfHeaders
	setdwordptr(opthead+0x40, 0);			//CheckSum
	setwordptr (opthead+0x44, 0);			//Subsystem
	setwordptr (opthead+0x46, 0);			//DllCharacteristics

	setdwordptr(opthead+0x48, 32768);		//SizeOfStackReserve
	setdwordptr(opthead+0x4C, 32768);		//SizeOfStackCommit
	setdwordptr(opthead+0x50, 0);			//SizeOfHeapReserve
	setdwordptr(opthead+0x54, 0);			//SizeOfHeapCommit
	setdwordptr(opthead+0x58, 0);			//LoaderFlags
	setdwordptr(opthead+0x5C, 6);			//NumberOfRvaAndSizes

	setdwordptr(opthead+0x60, 0);			//Export Base
	setdwordptr(opthead+0x64, 0);			//Export Size
	setdwordptr(opthead+0x68, 0);			//Import Base
	setdwordptr(opthead+0x6C, 0);			//Import Size
	setdwordptr(opthead+0x70, 0);			//Resource Base
	setdwordptr(opthead+0x74, 0);			//Resource Size
	setdwordptr(opthead+0x78, 0);			//Except Base
	setdwordptr(opthead+0x7C, 0);			//Except Size
	setdwordptr(opthead+0x80, 0);			//Cert Base
	setdwordptr(opthead+0x84, 0);			//Cert Size
	setdwordptr(opthead+0x88, rvarlc);		//Reloc Base
	setdwordptr(opthead+0x8C, szrlc);		//Reloc Size

	i=BTSHAS_LookupSection(ctx, ".edata");
	if(i>=0)
	{
		j=ctx->seg[i]->rva_base;
		k=ctx->seg[i]->ct-ctx->seg[i]->cts;
		setdwordptr(opthead+0x60, j);			//Export Base
		setdwordptr(opthead+0x64, j);			//Export Size
	}

	i=BTSHAS_LookupSection(ctx, ".idata");
	if(i>=0)
	{
		j=ctx->seg[i]->rva_base;
		k=ctx->seg[i]->ct-ctx->seg[i]->cts;
		setdwordptr(opthead+0x68, j);			//Import Base
		setdwordptr(opthead+0x6C, j);			//Import Size
	}

	j=ct-obuf;
	ct=obuf+((j+3)&(~3));

	return(ct-obuf);
}
