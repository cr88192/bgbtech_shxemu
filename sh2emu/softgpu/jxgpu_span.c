byte *jxgpu_membuf=NULL;
byte *jxgpu_memend=NULL;
byte *jxgpu_memrov=NULL;

void *jxgpu_malloc(int size)
{
	byte *ptr;
	int sz;
	if(!jxgpu_membuf)
	{
		jxgpu_membuf=malloc(1<<24);
		jxgpu_memend=jxgpu_membuf+(1<<24);
		jxgpu_memrov=jxgpu_membuf;
	}
	
	sz=(size+15)&(~15);
	ptr=jxgpu_memrov;
	jxgpu_memrov+=sz;

	if(jxgpu_memrov>jxgpu_memend)
		{ __debugbreak(); }

	memset(ptr, 0, sz);

	return(ptr);
}

JXGPU_RasterState *JXGPU_AllocContext()
{
	JXGPU_RasterState *tmp;
	
	tmp=jxgpu_malloc(sizeof(JXGPU_RasterState));
	memset(tmp, 0, sizeof(JXGPU_RasterState));
	return(tmp);
}

JXGPU_RasterSpan *JXGPU_AllocSpan(JXGPU_RasterState *ctx)
{
	JXGPU_RasterSpan *tmp, *tta;
	int i;
	
	tmp=ctx->freespan;
	if(tmp)
	{
		ctx->freespan=tmp->next;
		return(tmp);
	}
	
	tta=jxgpu_malloc(256*sizeof(JXGPU_RasterSpan));
	for(i=0; i<256; i++)
	{
		tta->next=ctx->freespan;
		ctx->freespan=tta;
		tta++;
	}

	tmp=ctx->freespan;
	ctx->freespan=tmp->next;
	return(tmp);
}

JXGPU_RasterPrim *JXGPU_AllocPrim(JXGPU_RasterState *ctx)
{
	JXGPU_RasterPrim *tmp, *tta;
	int i;
	
	tmp=ctx->freeprim;
	if(tmp)
	{
		ctx->freeprim=tmp->next;
		return(tmp);
	}
	
	tta=jxgpu_malloc(256*sizeof(JXGPU_RasterPrim));
	for(i=0; i<256; i++)
	{
		tta->next=ctx->freeprim;
		ctx->freeprim=tta;
		tta++;
	}

	tmp=ctx->freeprim;
	ctx->freeprim=tmp->next;
	return(tmp);
}

JXGPU_RasterTexi *JXGPU_AllocTexi(JXGPU_RasterState *ctx)
{
	JXGPU_RasterTexi *tmp, *tta;
	int i;
	
	tmp=jxgpu_malloc(sizeof(JXGPU_RasterTexi));
	memset(tmp, 0, sizeof(JXGPU_RasterTexi));
	return(tmp);
}

void JXGPU_FlushFreePrim(JXGPU_RasterState *ctx, JXGPU_RasterPrim *prim)
{
	JXGPU_RasterSpan *spcur, *spnxt;
	
	spcur=prim->span;
	while(spcur)
	{
		spnxt=spcur->next;
		spcur->next=ctx->freespan;
		ctx->freespan=spcur;
		spcur=spnxt;
	}
	
	prim->next=ctx->freeprim;
	ctx->freeprim=prim;
}

u64 JXGPU_DrawSpans_ColorStep(
	JXGPU_RasterState *ctx,
	u64 eclr, u64 sclr, int n)
{
	u64 cstp;
//	cstp=
//		((((eclr&0xFC00)-(sclr&0xFC00))/n)&0xFC00)|
//		((((eclr&0x03E0)-(sclr&0x03E0))/n)&0x03E0)|
//		((((eclr&0x001F)-(sclr&0x001F))/n)&0x001F);

	cstp=
		((((eclr&0xFFFF00000000)-(sclr&0xFFFF00000000))/n)&0xFFFF00000000)|
		((((eclr&0x0000FFFF0000)-(sclr&0x0000FFFF0000))/n)&0x0000FFFF0000)|
		((((eclr&0x00000000FFFF)-(sclr&0x00000000FFFF))/n)&0x00000000FFFF);

	return(cstp);
}

u64 JXGPU_DrawSpans_ColorSplit(
	JXGPU_RasterState *ctx,	u32 clr)
{
	u64 clr1;
	
	clr1=
		((u64)(clr&0xFC00)<<32)|
		((u64)(clr&0x03E0)<<22)|
		((u64)(clr&0x001F)<<11);
	return(clr1);
}

u32 JXGPU_DrawSpans_ColorMerge(
	JXGPU_RasterState *ctx,	u64 clr)
{
	u32 clr1;
	
	clr1=
		((clr>>32)&0xFC00)|
		((clr>>22)&0x03E0)|
		((clr>>11)&0x001F);
	return(clr1);
}

u32 JXGPU_DrawSpans_ColorModulate(
	JXGPU_RasterState *ctx,	u64 clr, u32 bclr)
{
	u32 clr1;
	
//	clr1=
//		((clr>>32)&0xFC00)|
//		((clr>>22)&0x03E0)|
//		((clr>>11)&0x001F);

	clr1=
		(((((u16)(clr>>32))*(bclr&0xFC00))>>16)&0xFC00)|
		(((((u16)(clr>>16))*(bclr&0x03E0))>>16)&0x03E0)|
		(((((u16)(clr    ))*(bclr&0x001F))>>16)&0x001F);

	return(clr1);
}

void JXGPU_DrawSpans_ZFlatBasic(
	JXGPU_RasterState *ctx,
	JXGPU_RasterPrim *prim)
{
	JXGPU_RasterSpan *spcur;
	JXGPU_RasterSpan *span;
	u16 *px, *pxe, *pz, *pt;
	int xpos, ypos;
	int spos, tpos, zpos;
	int sstp, tstp, zstp;
	int n, ts, tt, tz;
	int txm, tym, tys, mip;
//	u32 cpos, cstp, csm;
	u64 cpos, cstp, csm;
	int i, j, k;

//	texi=prim->texi;
	span=prim->span;
//	clr=prim->vtx[0].c;

//	csm=ctx->cstepmask;
	csm=0xFFFEFFFEFFFEFFFEULL;

	spcur=span;
	while(spcur)
	{
		n=(spcur->eposx-spcur->sposx)>>JXGPU_XY_SHR;
		if(n<1)
		{
			spcur=spcur->next;
			continue;
		}

		xpos=spcur->sposx>>JXGPU_XY_SHR;
		ypos=spcur->posy>>JXGPU_XY_SHR;
		
		if((ypos<0)|(ypos>=ctx->ys)|((xpos+n)<0)|(xpos>=ctx->xs))
		{
			spcur=spcur->next;
			continue;
		}

		spos=spcur->stexs;
		tpos=spcur->stext;
		zpos=spcur->sposz;
		cpos=spcur->sclr;
		
		if(n>1)
		{
			sstp=(spcur->etexs-spos)/n;
			tstp=(spcur->etext-tpos)/n;
			zstp=(spcur->eposz-zpos)/n;
//			cstp=(spcur->eclr-cpos)/n;
			cstp=JXGPU_DrawSpans_ColorStep(ctx, spcur->eclr, cpos, n);
		}else
		{
			sstp=0; tstp=0; zstp=0;
		}
		
		if(xpos<0)
		{
			i=-xpos;
			spos+=i*sstp;
			tpos+=i*tstp;
			zpos+=i*zstp;
			xpos=0;
		}
		
		if((xpos+n)>ctx->xs)
			n=ctx->xs-xpos;
		
		px=((u16 *)ctx->pxbuf)+ypos*ctx->ystr*2+xpos;
		pxe=px+n;
		pz=((u16 *)ctx->zbuf)+ypos*ctx->ystr*2+xpos;
		
		while(px<pxe)
		{
			ts=(spos>>JXGPU_ST_SHR);
			tt=(tpos>>JXGPU_ST_SHR);
			tz=(zpos>>JXGPU_Z_SHR);
			
			if(tz<=(*pz))
			{
//				*px=cpos;
				*px=JXGPU_DrawSpans_ColorMerge(ctx, cpos);
				*pz=tz;
			}
			
			spos+=sstp;
			tpos+=tstp;
			zpos+=zstp;
//			cpos+=cstp;
			cpos=(cpos&csm)+cstp;
			px++; pz++;
		}
		spcur=spcur->next;
	}
}

void JXGPU_DrawSpans_ZTexBasic(
	JXGPU_RasterState *ctx,
	JXGPU_RasterPrim *prim)
{
	JXGPU_RasterSpan *spcur;
	JXGPU_RasterTexi *texi;
	JXGPU_RasterSpan *span;
	u16 *px, *pxe, *pz, *pt;
	int xpos, ypos;
	int spos, tpos, zpos;
	int sstp, tstp, zstp;
	int n, ts, tt, tz;
	int txm, tym, tys, mip;
	int i, j, k;

	texi=prim->texi;
	span=prim->span;

	mip=0;
	pt=texi->pix[mip];
	if(!pt)
		{ mip=0; pt=texi->pix[mip]; }
	txm=(texi->xs>>mip)-1;
	tym=(texi->ys>>mip)-1;
	tys=texi->xs>>mip;

	if(txm<0)txm=0;
	if(tym<0)tym=0;

	spcur=span;
	while(spcur)
	{	
		n=(spcur->eposx-spcur->sposx)>>JXGPU_XY_SHR;
		if(n<1)
		{
			spcur=spcur->next;
			continue;
		}

		xpos=spcur->sposx>>JXGPU_XY_SHR;
		ypos=spcur->posy>>JXGPU_XY_SHR;
		
		if((ypos<0)|(ypos>=ctx->ys)|((xpos+n)<0)|(xpos>=ctx->xs))
		{
			spcur=spcur->next;
			continue;
		}

//		spos=spcur->stexs;
//		tpos=spcur->stext;
		spos=spcur->stexs>>mip;
		tpos=spcur->stext>>mip;
		zpos=spcur->sposz;
		
		if(n>1)
		{
			sstp=(spcur->etexs-spos)/n;
			tstp=(spcur->etext-tpos)/n;
			zstp=(spcur->eposz-zpos)/n;
//			cstp=JXGPU_DrawSpans_ColorStep(ctx, spcur->eclr, cpos, n);
		}else
		{
			sstp=0; tstp=0; zstp=0;
		}
		
		if(xpos<0)
		{
			i=-xpos;
			spos+=i*sstp;
			tpos+=i*tstp;
			zpos+=i*zstp;
			xpos=0;
		}
		
		if((xpos+n)>ctx->xs)
			n=ctx->xs-xpos;
		
		px=((u16 *)ctx->pxbuf)+ypos*ctx->ystr*2+xpos;
		pxe=px+n;
		pz=((u16 *)ctx->zbuf)+ypos*ctx->ystr*2+xpos;
		
		while(px<pxe)
		{
			ts=(spos>>JXGPU_ST_SHR);
			tt=(tpos>>JXGPU_ST_SHR);
			tz=(zpos>>JXGPU_Z_SHR);
			
			if(tz<=(*pz))
			{
				*px=pt[(tt&tym)*tys+(ts&txm)];
				*pz=tz;
			}
			
			spos+=sstp;
			tpos+=tstp;
			zpos+=zstp;
			px++; pz++;
		}
		spcur=spcur->next;
	}
}

void JXGPU_DrawSpans_ZTexColorBasic(
	JXGPU_RasterState *ctx,
	JXGPU_RasterPrim *prim)
{
	JXGPU_RasterSpan *spcur;
	JXGPU_RasterTexi *texi;
	JXGPU_RasterSpan *span;
	u16 *px, *pxe, *pz, *pt;
	int xpos, ypos;
	int spos, tpos, zpos;
	int sstp, tstp, zstp;
	int n, ts, tt, tz;
	int txm, tym, tys, mip;
	u64 cpos, cstp, csm;
	int i, j, k;

	texi=prim->texi;
	span=prim->span;

	csm=0xFFFEFFFEFFFEFFFEULL;

	mip=0;
	pt=texi->pix[mip];
	if(!pt)
		{ mip=0; pt=texi->pix[mip]; }
	txm=(texi->xs>>mip)-1;
	tym=(texi->ys>>mip)-1;
	tys=texi->xs>>mip;

	if(txm<0)txm=0;
	if(tym<0)tym=0;

	spcur=span;
	while(spcur)
	{	
		n=(spcur->eposx-spcur->sposx)>>JXGPU_XY_SHR;
		if(n<1)
		{
			spcur=spcur->next;
			continue;
		}

		xpos=spcur->sposx>>JXGPU_XY_SHR;
		ypos=spcur->posy>>JXGPU_XY_SHR;
		
		if((ypos<0)|(ypos>=ctx->ys)|((xpos+n)<0)|(xpos>=ctx->xs))
		{
			spcur=spcur->next;
			continue;
		}

//		spos=spcur->stexs;
//		tpos=spcur->stext;
		spos=spcur->stexs>>mip;
		tpos=spcur->stext>>mip;
		zpos=spcur->sposz;
		cpos=spcur->sclr;
		
		if(n>1)
		{
			sstp=(spcur->etexs-spos)/n;
			tstp=(spcur->etext-tpos)/n;
			zstp=(spcur->eposz-zpos)/n;
			cstp=JXGPU_DrawSpans_ColorStep(ctx, spcur->eclr, cpos, n);
		}else
		{
			sstp=0; tstp=0; zstp=0; cstp=0;
		}
		
		if(xpos<0)
		{
			i=-xpos;
			spos+=i*sstp;
			tpos+=i*tstp;
			zpos+=i*zstp;
			cpos+=i*cstp;
			xpos=0;
		}
		
		if((xpos+n)>ctx->xs)
			n=ctx->xs-xpos;
		
		px=((u16 *)ctx->pxbuf)+ypos*ctx->ystr*2+xpos;
		pxe=px+n;
		pz=((u16 *)ctx->zbuf)+ypos*ctx->ystr*2+xpos;
		
		while(px<pxe)
		{
			ts=(spos>>JXGPU_ST_SHR);
			tt=(tpos>>JXGPU_ST_SHR);
			tz=(zpos>>JXGPU_Z_SHR);
			
			if(tz<=(*pz))
			{
				*px=JXGPU_DrawSpans_ColorModulate(ctx, cpos,
					pt[(tt&tym)*tys+(ts&txm)]);
//				*px=pt[(tt&tym)*tys+(ts&txm)];
				*pz=tz;
			}
			
			spos+=sstp;
			tpos+=tstp;
			zpos+=zstp;
			cpos=(cpos&csm)+cstp;
			px++; pz++;
		}
		spcur=spcur->next;
	}
}

void JXGPU_DrawSpans_Basic(
	JXGPU_RasterState *ctx,
	JXGPU_RasterPrim *prim)
{
	if(prim->texi)
	{
		if(prim->primty&JXGPU_PRMD_COLOR)
		{
			JXGPU_DrawSpans_ZTexColorBasic(ctx, prim);
			return;
		}
	
		JXGPU_DrawSpans_ZTexBasic(ctx, prim);
		return;
	}

	JXGPU_DrawSpans_ZFlatBasic(ctx, prim);
	return;
}

void JXGPU_TriangleGenerateSpansBasic(
	JXGPU_RasterState *ctx,
	JXGPU_RasterPrim *prim)
{
	JXGPU_RasterVertex v[3];
	JXGPU_RasterSpan *sp;
	int x0, x1, x2, y0, y1, y2;
	int s0, s1, s2, t0, t1, t2;
	int z0, z1, z2;
	u64 c0, c1, c2;
	int xb0, xb1, xb2, yb0, yb1, yb2, yd0, yd1;
	int xv0, xv1, xc0, xc1, yc;
	int sv0, sv1, sc0, sc1;
	int tv0, tv1, tc0, tc1;
	int zv0, zv1, zc0, zc1;
	u64 cv0, cv1, cc0, cc1;
	u64 csm;
	int i, j, k;
	
	prim->span=NULL;
	
	y0=prim->vtx[0].y;
	y1=prim->vtx[1].y;
	y2=prim->vtx[2].y;
	
	if(y1<y0)
	{
		if(y2<y1)
			{ v[0]=prim->vtx[2]; v[1]=prim->vtx[1]; v[2]=prim->vtx[0]; }
		else if(y2<y0)
			{ v[0]=prim->vtx[1]; v[1]=prim->vtx[2]; v[2]=prim->vtx[0]; }
		else
			{ v[0]=prim->vtx[1]; v[1]=prim->vtx[0]; v[2]=prim->vtx[2]; }
	}else
	{
		if(y2<y0)
			{ v[0]=prim->vtx[2]; v[1]=prim->vtx[0]; v[2]=prim->vtx[1]; }
		else if(y2<y1)
			{ v[0]=prim->vtx[0]; v[1]=prim->vtx[2]; v[2]=prim->vtx[1]; }
		else
			{ v[0]=prim->vtx[0]; v[1]=prim->vtx[1]; v[2]=prim->vtx[2]; }
	}

	x0=v[0].x;	x1=v[1].x;	x2=v[2].x;
	y0=v[0].y;	y1=v[1].y;	y2=v[2].y;
	z0=v[0].z;	z1=v[1].z;	z2=v[2].z;
	s0=v[0].s;	s1=v[1].s;	s2=v[2].s;
	t0=v[0].t;	t1=v[1].t;	t2=v[2].t;
//	c0=v[0].c;	c1=v[1].c;	c2=v[2].c;

	c0=JXGPU_DrawSpans_ColorSplit(ctx, v[0].c);
	c1=JXGPU_DrawSpans_ColorSplit(ctx, v[1].c);
	c2=JXGPU_DrawSpans_ColorSplit(ctx, v[2].c);
	
	xb0=x0>>JXGPU_XY_SHR;
	yb0=y0>>JXGPU_XY_SHR;
	yb1=y1>>JXGPU_XY_SHR;
	yb2=y2>>JXGPU_XY_SHR;

//	csm=ctx->cstepmask;
	csm=0xFFFEFFFEFFFEFFFEULL;

	if(yb1>yb0)
	{
		yd0=yb1-yb0;		yd1=yb2-yb0;
		xv0=(x1-x0)/yd0;	xv1=(x2-x0)/yd1;
		zv0=(z1-z0)/yd0;	zv1=(z2-z0)/yd1;
		sv0=(s1-s0)/yd0;	sv1=(s2-s0)/yd1;
		tv0=(t1-t0)/yd0;	tv1=(t2-t0)/yd1;
//		cv0=(c1-c0)/yd0;	cv1=(c2-c0)/yd1;
		cv0=JXGPU_DrawSpans_ColorStep(ctx, c1, c0, yd0);
		cv1=JXGPU_DrawSpans_ColorStep(ctx, c2, c0, yd1);
		
		if(xv1<xv0)
		{
			i=xv0; xv0=xv1; xv1=i;
			i=zv0; zv0=zv1; zv1=i;
			i=sv0; sv0=sv1; sv1=i;
			i=tv0; tv0=tv1; tv1=i;
			i=cv0; cv0=cv1; cv1=i;
		}

		sc0=s0; sc1=s0;
		tc0=t0; tc1=t0;
		zc0=z0; zc1=z0;
		cc0=c0; cc1=c0;

		xc0=xb0<<JXGPU_XY_SHR; xc1=xc0; yc=yb0;
		while(yc<yb1)
		{
			sp=JXGPU_AllocSpan(ctx);
			sp->sposx=xc0;	sp->eposx=xc1;
			sp->posy=yc<<JXGPU_XY_SHR;
			sp->stexs=sc0;	sp->etexs=sc1;
			sp->stext=tc0;	sp->etext=tc1;
			sp->sposz=zc0;	sp->eposz=zc1;
			sp->sclr=cc0;	sp->eclr=cc1;
		
			sp->next=prim->span;
			prim->span=sp;
		
			xc0+=xv0;		xc1+=xv1;
			zc0+=zv0;		zc1+=zv1;
			sc0+=sv0;		sc1+=sv1;
			tc0+=tv0;		tc1+=tv1;
			cc0=(cc0&csm)+cv0;
			cc1=(cc1&csm)+cv1;
			yc++;
		}
	}else
	{
		sc0=s0; sc1=s0;
		tc0=t0; tc1=t0;
		zc0=z0; zc1=z0;
		cc0=c0; cc1=c0;

		xc0=x0;
		xc1=x0;
		if(x1<x0) { xc0=x1; sc0=s1; tc0=t1; zc0=z1; cc0=c1; }
		if(x1>x0) { xc1=x1; sc1=s1; tc1=t1; zc1=z1; cc1=c1; }
	}

	if(yb2>yb1)
	{
		yd0=yb2-yb1;
		xv0=(x2-xc0)/yd0;	xv1=(x2-xc1)/yd0;
		zv0=(z2-zc0)/yd0;	zv1=(z2-zc1)/yd0;
		sv0=(s2-sc0)/yd0;	sv1=(s2-sc1)/yd0;
		tv0=(t2-tc0)/yd0;	tv1=(t2-tc1)/yd0;
//		cv0=(c2-cc0)/yd0;	cv1=(c2-cc1)/yd0;
		cv0=JXGPU_DrawSpans_ColorStep(ctx, c2, cc0, yd0);
		cv1=JXGPU_DrawSpans_ColorStep(ctx, c2, cc1, yd0);
		
		yc=yb1;
		while(yc<yb2)
		{
			sp=JXGPU_AllocSpan(ctx);
			sp->sposx=xc0;	sp->eposx=xc1;
			sp->posy=yc<<JXGPU_XY_SHR;
			sp->stexs=sc0;	sp->etexs=sc1;
			sp->stext=tc0;	sp->etext=tc1;
			sp->sposz=zc0;	sp->eposz=zc1;
			sp->sclr=cc0;	sp->eclr=cc1;

			sp->next=prim->span;
			prim->span=sp;
		
			xc0+=xv0;		xc1+=xv1;
			zc0+=zv0;		zc1+=zv1;
			sc0+=sv0;		sc1+=sv1;
			tc0+=tv0;		tc1+=tv1;
//			cc0+=cv0;		cc1+=cv1;
			cc0=(cc0&csm)+cv0;
			cc1=(cc1&csm)+cv1;
			yc++;
		}
	}
}

void JXGPU_ContextRenderPrims(JXGPU_RasterState *ctx)
{
	JXGPU_RasterPrim *pcur, *pnxt;
	
	pcur=ctx->prims;
	ctx->prims=NULL;
	
	while(pcur)
	{
		pnxt=pcur->next;

		JXGPU_TriangleGenerateSpansBasic(ctx, pcur);
		JXGPU_DrawSpans_Basic(ctx, pcur);
		JXGPU_FlushFreePrim(ctx, pcur);
		
		pcur=pnxt;
	}
}

