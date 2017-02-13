u32 *btesh2_dcgfx_vram;
int btesh2_dcgfx_vramfl;

int btesh2_dcgfx_fbxs;
int btesh2_dcgfx_fbys;
int btesh2_dcgfx_fbsz;

u32 btesh2_dcgfx_borderclr;
u32 btesh2_dcgfx_displaycfg;
u32 btesh2_dcgfx_displaysize;
u32 btesh2_dcgfx_displaysync;

u32 btesh2_dcgfx_disp_hbor;
u32 btesh2_dcgfx_disp_sync;
u32 btesh2_dcgfx_disp_vbor;
u32 btesh2_dcgfx_disp_synw;
u32 btesh2_dcgfx_disp_tspc;
u32 btesh2_dcgfx_disp_vcfg;
u32 btesh2_dcgfx_disp_hpos;
u32 btesh2_dcgfx_disp_vpos;

u32 btesh2_dcgfx_displaybase_odd;
u32 btesh2_dcgfx_displaybase_even;

u32 btesh2_dcgfx_VregGetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	int ix, ix1;
	u32 v;

//	ix=reladdr>>2;
	
	switch(reladdr)
	{
	case 0x0000:	v=0x17FD11DB; break;
	case 0x0004:	v=0x11;	break;
	case 0x0008:	v=0; break;
	case 0x0014:	v=0; break;

	case 0x0040:	v=btesh2_dcgfx_borderclr; break;
	case 0x0044:	v=btesh2_dcgfx_displaycfg; break;

	case 0x0050:	v=btesh2_dcgfx_displaybase_odd; break;
	case 0x0054:	v=btesh2_dcgfx_displaybase_even; break;

	case 0x005C:	v=btesh2_dcgfx_displaysize; break;

	case 0x00D0:	v=btesh2_dcgfx_displaysync; break;
	case 0x00D4:	v=btesh2_dcgfx_disp_hbor; break;
	case 0x00D8:	v=btesh2_dcgfx_disp_sync; break;
	case 0x00DC:	v=btesh2_dcgfx_disp_vbor; break;
	case 0x00E0:	v=btesh2_dcgfx_disp_synw; break;
	case 0x00E4:	v=btesh2_dcgfx_disp_tspc; break;
	case 0x00E8:	v=btesh2_dcgfx_disp_vcfg; break;
	case 0x00EC:	v=btesh2_dcgfx_disp_hpos; break;
	case 0x00F0:	v=btesh2_dcgfx_disp_vpos; break;
	default:  break;
	}
	return(v);
}

int btesh2_dcgfx_VregSetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int ix, ix1;

//	ix=reladdr>>2;

	printf("btesh2_dcgfx_VregSetD: %04X=%08X\n", reladdr, val);

	switch(reladdr)
	{
//	case 0x0000:	v=0x17FD11DB; break;
//	case 0x0004:	v=0x11;	break;
//	case 0x0008:	v=0; break;
//	case 0x0014:	v=0; break;

	case 0x0040:	btesh2_dcgfx_borderclr=val; break;
	case 0x0044:	btesh2_dcgfx_displaycfg=val; break;

	case 0x0050:	btesh2_dcgfx_displaybase_odd=val; break;
	case 0x0054:	btesh2_dcgfx_displaybase_even=val; break;

	case 0x005C:	btesh2_dcgfx_displaysize=val; break;

	case 0x00D0:	btesh2_dcgfx_displaysync=val; break;
	case 0x00D4:	btesh2_dcgfx_disp_hbor=val; break;
	case 0x00D8:	btesh2_dcgfx_disp_sync=val; break;
	case 0x00DC:	btesh2_dcgfx_disp_vbor=val; break;
	case 0x00E0:	btesh2_dcgfx_disp_synw=val; break;
	case 0x00E4:	btesh2_dcgfx_disp_tspc=val; break;
	case 0x00E8:	btesh2_dcgfx_disp_vcfg=val; break;
	case 0x00EC:	btesh2_dcgfx_disp_hpos=val; break;
	case 0x00F0:	btesh2_dcgfx_disp_vpos=val; break;
	default:  break;
	}
	return(0);
}

int BTESH2_DCGFX_ImageRegister(BTESH2_MemoryImage *img)
{
	byte **rptr;
	
	rptr=(byte **)(&btesh2_dcgfx_vram);
	BTESH2_MemoryDefineSpanDemand(img, 0x05000000, 0x057FFFFF,
		rptr, &btesh2_dcgfx_vramfl, "DC_VRAM");
	
	BTESH2_MemoryDefineSpanRegs(img, 0x005F8000, 0x005F9FFF, "DC_VREG",
		btesh2_dcgfx_VregGetD, btesh2_dcgfx_VregSetD);

//	btesh2_dcgfx_displaycfg=0x000D;
	btesh2_dcgfx_displaycfg=0x000C;

	btesh2_dcgfx_displaysize=
		(btesh2_gfxcon_fbxs-1)|
		((btesh2_gfxcon_fbys-1)<<10)|
		((btesh2_gfxcon_fbxs+1)<<20);

	return(0);
}

u32 btesh2_dcgfx_redraw_tpix16_555le(byte *px)
{
	u16 pxa;
	u32 pxc;
	
	pxa=px[0]|(px[1]<<8);
//	pxc=0xFF000000|
//		((pxa&0x7C00)<<8)|
//		((pxa&0x03E0)>>2)|
//		((pxa&0x001F)<<3);

	pxc=0xFF000000|
		((pxa&0x7C00)>> 7)|
		((pxa&0x03E0)<< 6)|
		((pxa&0x001F)<<19);

	return(pxc);
}

u32 btesh2_dcgfx_redraw_tpix16_565le(byte *px)
{
	u16 pxa;
	u32 pxc;
	
	pxa=px[0]|(px[1]<<8);
//	pxc=0xFF000000|
//		((pxa&0xF800)<<8)|
//		((pxa&0x07E0)>>3)|
//		((pxa&0x001F)<<3);

	pxc=0xFF000000|
		((pxa&0xF800)>> 8)|
		((pxa&0x07E0)<< 5)|
		((pxa&0x001F)<<19);

	return(pxc);
}

static int btesh2_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}

#if 0
u32 btesh2_dcgfx_redraw_tpix16_yuv655le(byte *px)
{
	int cy, cu, cv, cu1, cv1;
	int cr, cg, cb;
	u16 pxa;
	u32 pxc;
	
	pxa=px[0]|(px[1]<<8);

	cy=(pxa>>8)&0xFC;
	cv=(pxa>>2)&0xF8;
	cu=(pxa<<3)&0xF8;
	cy=cy|(cy>>6);
	cu=cu|(cu>>5);
	cv=cv|(cv>>5);

	cu1=(cu-128)<<1; cv1=(cv-128)<<1;
//	cg=2*cy-cu1-cv1;
//	cg=(4*cy-cu1-cv1)>>1;
	cg=cy-((cu1+cv1)>>2);
	cb=cg+cu1;
	cr=cg+cv1;
	
	if((cr|cg|cb)>>8)
	{
		cr=btesh2_clamp255(cr);
		cg=btesh2_clamp255(cg);
		cb=btesh2_clamp255(cb);
	}
	
	pxc=0xFF000000|(cb<<16)|(cg<<8)|cr;

//	pxc=0xFF000000|
//		((pxa&0x7C00)>> 7)|
//		((pxa&0x03E0)<< 6)|
//		((pxa&0x001F)<<19);

	return(pxc);
}
#endif

#if 1
static u32 *yuv655_ttab=NULL;

void btesh2_dcgfx_init_yuv655le()
{
	int cy, cu, cv, cu1, cv1;
	int cr, cg, cb;
	int i, j, k;
	u16 pxa;
	u32 pxc;

//	yuv655_ttab=malloc(32768*4);
	yuv655_ttab=malloc(65536*4);
	
//	for(i=0; i<32768; i++)
	for(i=0; i<65536; i++)
	{
//		pxa=(i<<1)|(i&1);
		pxa=i;
	
		cy=(pxa>>8)&0xFC;
		cv=(pxa>>2)&0xF8;
		cu=(pxa<<3)&0xF8;
		cy=cy|(cy>>6);
		cu=cu|(cu>>5);
		cv=cv|(cv>>5);

		cu1=(cu-128)<<1; cv1=(cv-128)<<1;
	//	cg=2*cy-cu1-cv1;
	//	cg=(4*cy-cu1-cv1)>>1;
		cg=cy-((cu1+cv1)>>2);
		cb=cg+cu1;
		cr=cg+cv1;
		
		if((cr|cg|cb)>>8)
		{
			cr=btesh2_clamp255(cr);
			cg=btesh2_clamp255(cg);
			cb=btesh2_clamp255(cb);
		}
		
		pxc=0xFF000000|(cb<<16)|(cg<<8)|cr;
		
		yuv655_ttab[i]=pxc;
	}
}

u32 btesh2_dcgfx_redraw_tpix16_yuv655le(byte *px)
{
	int cy, cu, cv, cu1, cv1;
	int cr, cg, cb;
	int i, j, k;
	u16 pxa;
	u32 pxc;
	
	if(!yuv655_ttab)
	{
		btesh2_dcgfx_init_yuv655le();
	}

	pxa=*(u16 *)px;
	pxc=yuv655_ttab[pxa];
	
//	pxa=px[0]|(px[1]<<8);
//	pxc=ttab[pxa>>1];

	return(pxc);
}
#endif


u32 btesh2_dcgfx_redraw_tpix24_bgr(byte *px)
{
	u32 pxc;
	pxc=0xFF000000|
		(px[0]<<16)|(px[1]<<8)|px[2];
	return(pxc);
}

u32 btesh2_dcgfx_redraw_tpix32_rgba(u32 px)
{
	return(px);
}

u32 btesh2_dcgfx_redraw_tpix32_bgra(u32 px)
{
	return(
		(px&0xFF00FF00)|
		((px>>16)&0x000000FF)|
		((px<<16)&0x00FF0000));
}

int BTESH2_DCGFX_RedrawScreen()
{
	static int lastcfg=0;
	u32 (*tpix16)(byte *px);
	u32 (*tpix24)(byte *px);
	u32 (*tpix32)(u32 px);
	u32 *fb32, *ilb32, *olb32;
	byte *fbb, *ilb8;
	u32 px;
	int xstep, ystep, xcur, ycur;
	int vxs, vys, vxstr, vystr;
	int x, y, ix, iy;

	if(btesh2_dcgfx_displaycfg&1)
	{
		btesh2_gfxcon_con_disabled=1;
	}
	else
	{
		if(lastcfg&1)
		{
			btesh2_gfxcon_clearScreen(16);
		}
		lastcfg=btesh2_dcgfx_displaycfg;
		return(0);
	}

	if(!btesh2_dcgfx_vram || !btesh2_dcgfx_vramfl)
		return(0);

	lastcfg=btesh2_dcgfx_displaycfg;
	btesh2_dcgfx_vramfl&=~1;

	GfxDrv_PrepareFramebuf();
	
	vxs=(btesh2_dcgfx_displaysize&1023)+1;
	vys=((btesh2_dcgfx_displaysize>>10)&1023)+1;
	vystr=((btesh2_dcgfx_displaysize>>20)&1023)-1;

#if 0
	if(btesh2_dcgfx_displaycfg&0x0008)
	{
		vxstr=4;
//		vystr=vystr;
	}else
	{
		vxs*=2;
		vxstr=2;
//		vystr=vystr;
	}
#endif

	switch((btesh2_dcgfx_displaycfg>>2)&3)
	{
	case 0: case 1:
		vxs*=2; vxstr=2; break;
	case 2:
		vxs=(vxs*4)/3;
		vxstr=3;
		break;
	case 3:
		vxstr=4;
		break;
	}

#if 0
	if((vxstr==4) &&
		(vxs==btesh2_gfxcon_fbxs) &&
		(vys==btesh2_gfxcon_fbys))
	{
		fb32=btesh2_dcgfx_vram+(btesh2_dcgfx_displaybase_odd>>2);
//		memcpy(btesh2_gfxcon_framebuf, fb32, vxs*4);
		memcpy(btesh2_gfxcon_framebuf, fb32, vys*vystr*4);
		return(0);
	}
#endif
	
	if(vxstr==4)
	{
		fb32=btesh2_dcgfx_vram+(btesh2_dcgfx_displaybase_odd>>2);
		tpix32=btesh2_dcgfx_redraw_tpix32_bgra;
		
		xstep=(0x10000*vxs)/btesh2_gfxcon_fbxs;
		ystep=(0x10000*vys)/btesh2_gfxcon_fbys;

		ycur=0;
		for(y=0; y<btesh2_gfxcon_fbys; y++)
		{
			if((vxs*2)==btesh2_gfxcon_fbxs)
			{
				iy=ycur>>16;
				ilb32=fb32+(iy*vystr);
				olb32=btesh2_gfxcon_framebuf+(y*btesh2_gfxcon_fbxs);
				xcur=0;
				for(x=0; x<vxs; x++)
				{
//					ix=xcur>>16;
					px=ilb32[x];
					px=(((px    )&0xFF00FF00)|
						((px>>16)&0x000000FF)|
						((px<<16)&0x00FF0000));
					olb32[x*2+0]=px; olb32[x*2+1]=px;
//					xcur+=xstep<<1;
				}
				ycur+=ystep;
				continue;
			}
			
			iy=ycur>>16;
			ilb32=fb32+(iy*vystr);
			olb32=btesh2_gfxcon_framebuf+(y*btesh2_gfxcon_fbxs);
			xcur=0;
			for(x=0; x<btesh2_gfxcon_fbxs; x++)
			{
				ix=xcur>>16;
//				iy=ycur>>16;
//				px=tpix32(fb32[iy*vystr+ix]);
//				px=fb32[iy*vystr+ix];
				px=ilb32[ix];
				px=(((px    )&0xFF00FF00)|
					((px>>16)&0x000000FF)|
					((px<<16)&0x00FF0000));
				olb32[x]=px;
//				btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=px;
			
				xcur+=xstep;
			}
			ycur+=ystep;
		}
		return(0);
	}

	fbb=(byte *)(btesh2_dcgfx_vram+(btesh2_dcgfx_displaybase_odd>>2));
	xstep=(0x10000*vxs)/btesh2_gfxcon_fbxs;
	ystep=(0x10000*vys)/btesh2_gfxcon_fbys;

	if(vxstr==2)
	{
		tpix16=btesh2_dcgfx_redraw_tpix16_555le;

		if(btesh2_dcgfx_displaycfg&0x0004)
			tpix16=btesh2_dcgfx_redraw_tpix16_565le;

		if(btesh2_dcgfx_displaycfg&0x0080)
			tpix16=btesh2_dcgfx_redraw_tpix16_yuv655le;

		ycur=0;
		for(y=0; y<btesh2_gfxcon_fbys; y++)
		{
			xcur=0;

			olb32=btesh2_gfxcon_framebuf+(y*btesh2_gfxcon_fbxs);

			iy=ycur>>16;
			ilb8=fbb+(iy*vystr*4);

			if(tpix16==btesh2_dcgfx_redraw_tpix16_yuv655le)
			{
				for(x=0; x<btesh2_gfxcon_fbxs; x+=4)
				{
					ix=xcur>>16;	xcur+=xstep;
					px=btesh2_dcgfx_redraw_tpix16_yuv655le(ilb8+ix*2);
					olb32[x+0]=px;

					ix=xcur>>16;	xcur+=xstep;
					px=btesh2_dcgfx_redraw_tpix16_yuv655le(ilb8+ix*2);
					olb32[x+1]=px;

					ix=xcur>>16;	xcur+=xstep;
					px=btesh2_dcgfx_redraw_tpix16_yuv655le(ilb8+ix*2);
					olb32[x+2]=px;

					ix=xcur>>16;	xcur+=xstep;
					px=btesh2_dcgfx_redraw_tpix16_yuv655le(ilb8+ix*2);
					olb32[x+3]=px;
				}
			}else
			{
				for(x=0; x<btesh2_gfxcon_fbxs; x+=4)
				{
					ix=xcur>>16;	iy=ycur>>16;
					px=tpix16(fbb+(iy*vystr*4+ix*2));
					btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=px;
					xcur+=xstep;

					ix=xcur>>16;	iy=ycur>>16;
					px=tpix16(fbb+(iy*vystr*4+ix*2));
					btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=px;
					xcur+=xstep;

					ix=xcur>>16;	iy=ycur>>16;
					px=tpix16(fbb+(iy*vystr*4+ix*2));
					btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=px;
					xcur+=xstep;

					ix=xcur>>16;	iy=ycur>>16;
					px=tpix16(fbb+(iy*vystr*4+ix*2));
					btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=px;
					xcur+=xstep;
				}
			}
			ycur+=ystep;
		}
		return(0);
	}

	if(vxstr==3)
	{
		tpix24=btesh2_dcgfx_redraw_tpix24_bgr;
		ycur=0;
		for(y=0; y<btesh2_gfxcon_fbys; y++)
		{
			xcur=0;
			for(x=0; x<btesh2_gfxcon_fbxs; x++)
			{
				ix=xcur>>16;	iy=ycur>>16;
				px=tpix24(fbb+(iy*vystr*4+ix*3));
				btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=px;
				xcur+=xstep;
			}
			ycur+=ystep;
		}
		return(0);
	}

	return(0);
}
