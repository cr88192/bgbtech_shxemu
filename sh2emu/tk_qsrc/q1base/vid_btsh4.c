/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// vid_null.c -- null video driver to aid porting efforts

#include "quakedef.h"
#include "d_local.h"

viddef_t	vid;				// global video state

#define	BASEWIDTH	320
// #define	BASEHEIGHT	200
#define	BASEHEIGHT	240

// #define	BASEWIDTH	480
// #define	BASEHEIGHT	360

//byte	vid_buffer[BASEWIDTH*(BASEHEIGHT+4)];
// byte	vid_buffer[BASEWIDTH*BASEHEIGHT*2];
// short	zbuffer[BASEWIDTH*(BASEHEIGHT+4)];
// byte	surfcache[256*1024];

byte	*vid_buffer;
short	*zbuffer;
byte	*surfcache;

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];
unsigned	d2d_8to24table[256];

u32 *vid_vram;
u32 *vid_vreg;

int vid_clamp255(int v)
{
	if(v<0)return(0);
	if(v>255)return(255);
	return(v);
}

int VID_BlendEven16(int pixa, int pixb)
{
	int pix;
	pix=((pixa&0xFBDF)+(pixb&0xFBDF))>>1;
	return(pix);
}

int VID_Blend66_16(int pixa, int pixb)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
#if 0
	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(11*y0+5*y1+0x02000)>>4;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(11*u0+5*u1+0x00008)>>4;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(11*v0+5*v1+0x00100)>>4;
	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));
//	return(y2|u2|v2);
#endif

	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_Blend75_16(int pixa, int pixb)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
//	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(12*y0+4*y1+0x01FFF)>>4;
//	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(12*u0+4*u1+0x00007)>>4;
//	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(12*v0+4*v1+0x000FF)>>4;

	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(12*y0+4*y1+0x02000)>>4;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(12*u0+4*u1+0x00008)>>4;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(12*v0+4*v1+0x00100)>>4;

	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));

//	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_Blend88_16(int pixa, int pixb)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);	 y2=(14*y0+2*y1+0x02000)>>4;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);	 u2=(14*u0+2*u1+0x00008)>>4;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);	 v2=(14*v0+2*v1+0x00100)>>4;
	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));

//	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_BlendRatio16(int pixa, int pixb, int rat)
{
	int y0, y1, y2, u0, u1, u2, v0, v1, v2;
	
	y0=(pixa&0xFC00);	y1=(pixb&0xFC00);
	y2=((255-rat)*y0+rat*y1+0x02000)>>8;
	u0=(pixa&0x001F);	u1=(pixb&0x001F);
	u2=((255-rat)*u0+rat*u1+0x00008)>>8;
	v0=(pixa&0x03E0);	v1=(pixb&0x03E0);
	v2=((255-rat)*v0+rat*v1+0x00100)>>8;
	return((y2&0xFC00)|(u2&0x001F)|(v2&0x03E0));

//	return(VID_BlendEven16(pixa, VID_BlendEven16(pixa, pixb)));
}

int VID_ColorMap16(int pix, int light)
{
	int d, sc, scuv;
	int y1, u1, v1;

#if 1
//	d = ((l+(l>>2)) & 0xFC00);
	d = (light & 0xFC00)-(8<<10);

	pix= pix - d;
	if(pix<0x0000)pix=0x01EF;
	if(pix>0xFFFF)pix=0xFDEF;
#endif

#if 0
//	sc=(256+64)-((light>>8)&255)*4;
//	sc=(256+128)-((light>>8)&255)*8;
//	sc=(256+256)-((light>>8)&255)*8;

	sc=(512+128)-((light>>8)&255)*10;
//	scuv=(256+64)-((light>>8)&255)*5;
	scuv=256-((light>>8)&255)*4;
//	scuv=256-((light>>8)&255)*5;

	if(sc<0)sc=0;
	y1=(pix&0xFC00);
	u1=(pix&0x001F);
	v1=(pix&0x03E0);
	y1=(y1*sc+0x01FFFF)>>8;
	u1=(((u1-0x0010)*scuv+0x00007F)>>8)+0x0010;
	v1=(((v1-0x0200)*scuv+0x000FFF)>>8)+0x0200;
	if((y1>>16) || (v1>>10) || (u1>>5))
//	if(1)
	{
		if(y1<0x0000)y1=0x0000;
		if(y1>0xFFFF)y1=0xFC00;
		if(u1<0x0000)u1=0x0000;
		if(u1>0x001F)u1=0x001F;
		if(v1<0x0000)v1=0x0000;
		if(v1>0x03EF)v1=0x03E0;
	}
	
	pix=(y1&0xFC00)|(u1&0x001F)|(v1&0x03E0);
#endif

	return(pix);
}

void	VID_SetPalette (unsigned char *palette)
{
	int cr, cg, cb;
	int cy, cu, cv;
	int i, j, k;
	
	for(i=0; i<256; i++)
	{
		cr=palette[i*3+0];
		cg=palette[i*3+1];
		cb=palette[i*3+2];
		
//		cr=cr*1.5;
//		cg=cg*1.5;
//		cb=cb*1.5;
		
		cy=(2*cg+cr+cb)/4;
		cu=((cb-cg)/2)+128;
		cv=((cr-cg)/2)+128;

//		cy+=16;
		
		cy=vid_clamp255(cy+1)>>2;
		cu=vid_clamp255(cu+3)>>3;
		cv=vid_clamp255(cv+3)>>3;
		d_8to16table[i]=(cy<<10)|(cv<<5)|cu;

//		d_8to16table[i]=((cy>>2)<<10)|((cv>>3)<<5)|(cu>>3);
		
//		d_8to16table[i]=((cr>>3)<<10)|((cg>>3)<<5)|(cb>>3);
//		d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
		d2d_8to24table[i]=0xFF000000|(cr<<16)|(cg<<8)|cb;
	}
	
	d_8to16table[255]=0x7FFF;
}

void	VID_ShiftPalette (unsigned char *palette)
{
//	VID_SetPalette(palette);
}

u16 vid_blendv;
int vid_blendp;

void	VID_ShiftPaletteVec (int dr, int dg, int db, int dpcnt)
{
	int cy, cu, cv;
	
	cy=(dr+2*dg+db)/4;
	cu=(db-dg)/2+128;
	cv=(dr-dg)/2+128;
	
	cy=vid_clamp255(cy);
	cu=vid_clamp255(cu);
	cv=vid_clamp255(cv);
	
	cy>>=2; cu>>=3; cv>>=3;
	
	vid_blendv=(cy<<10)|(cv<<5)|cu;
	vid_blendp=dpcnt;
}

void	VID_Init (unsigned char *palette)
{
	u32 *ict;
	int i, j, k;

	vid_buffer=malloc(BASEWIDTH*BASEHEIGHT*2);
	zbuffer=malloc(BASEWIDTH*BASEHEIGHT*2);
	surfcache=malloc(BASEWIDTH*BASEHEIGHT*3*2);
//	surfcache=malloc(512*1024);

	printf("VID_Init:\n");

	vid.maxwarpwidth = vid.width = vid.conwidth = BASEWIDTH;
	vid.maxwarpheight = vid.height = vid.conheight = BASEHEIGHT;
	vid.aspect = 1.0;
	vid.numpages = 1;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
//	vid.buffer = vid.conbuffer = vid_buffer;
	vid.buffer = vid.conbuffer = vid_buffer + (2*BASEWIDTH);
	vid.rowbytes = vid.conrowbytes = BASEWIDTH;
	
	d_pzbuffer = zbuffer;
//	D_InitCaches (surfcache, sizeof(surfcache));
//	D_InitCaches (surfcache, 512*1024);
	D_InitCaches (surfcache, BASEWIDTH*BASEHEIGHT*3*2);

	vid_vreg=(u32 *)0xA05F8000;
	vid_vreg[(0x44/4)]=0x000D;
//	vid_vreg[(0x5C/4)]=320|(240<<10)|(320<<20);
//	vid_vreg[(0x5C/4)]=319|(239<<10)|(321<<20);
//	vid_vreg[(0x5C/4)]=319|(199<<10)|(321<<20);

	vid_vreg[(0x5C/4)]=
		(BASEWIDTH-1)|
		((BASEHEIGHT-1)<<10)|
		((BASEWIDTH+1)<<20);

	vid_vram=(u32 *)0xA5000000;

#if 1
//	vid.buffer = vid.conbuffer = (byte *) vid_vram;
	vid.buffer = vid.conbuffer = vid_buffer;
	vid.rowbytes = vid.conrowbytes = 2*BASEWIDTH;
	r_pixbytes=2;

//	host_colormap16 = Hunk_AllocName (131072, "colormap16");
	host_colormap16 = malloc (131072);

	vid.colormap16 = host_colormap16;
#endif

#if 1
//	vid_vreg[(0x44/4)]=0x0005;
//	vid_vreg[(0x44/4)]=0x0001;
	vid_vreg[(0x44/4)]=0x0081;

	vid_vreg[(0x5C/4)]=
		(BASEWIDTH/2-1)|
		((BASEHEIGHT-1)<<10)|
		((BASEWIDTH/2+1)<<20);
#endif

	printf("VID_Init: B\n");

	VID_SetPalette(palette);

	printf("VID_Init: C\n");

	if(host_colormap16)
	{
		for(i=0; i<65536; i++)
		{
			host_colormap16[i]=d_8to16table[host_colormap[i]];
		}
	}

#if 0
	ict=vid_vram;
	for(i=0; i<240; i++)
	{
		for(j=0; j<320; j++)
		{
//			ict[j]=d_8to24table[ics[j]];
			*ict++=0xFFFFFFFF;
		}
	}

	for(i=0; i<100; i++)
	{
		ict=vid_vram+i*320;
		for(j=0; j<256; j++)
		{
//			ict[j]=d_8to24table[ics[j]];
			*ict++=d_8to24table[j];
		}
	}
#endif
}

void	VID_Shutdown (void)
{
}

void	VID_Update (vrect_t *rects)
{
	byte *ics;
	u16 *ict16, *ics16;
	u32 *ict;
	int pix;
	int i, j, k;

	if(host_colormap16)
	{
		r_pixbytes=2;

		if(vid_blendp)
		{
			ics16=(u16 *)vid.buffer;
			ict16=(u16 *)vid_vram;
			for(i=0; i<BASEHEIGHT; i++)
			{
				for(j=0; j<BASEWIDTH; j++)
				{
					pix=*ics16++;
					pix=VID_BlendRatio16(pix, vid_blendv, vid_blendp);
					*ict16++=pix;
				}
			}
		}else
		{
			memcpy(vid_vram, vid.buffer, BASEWIDTH*BASEHEIGHT*2);
		}
		return;
	}
	
//	return;

#if 1
	ics=vid.buffer;
	ict16=(u16 *)vid_vram;
	for(i=0; i<BASEHEIGHT; i++)
	{
		for(j=0; j<BASEWIDTH; j+=8)
		{
			ict16[0]=d_8to16table[ics[0]];
			ict16[1]=d_8to16table[ics[1]];
			ict16[2]=d_8to16table[ics[2]];
			ict16[3]=d_8to16table[ics[3]];
			ict16[4]=d_8to16table[ics[4]];
			ict16[5]=d_8to16table[ics[5]];
			ict16[6]=d_8to16table[ics[6]];
			ict16[7]=d_8to16table[ics[7]];
			ict16+=8; ics+=8;
		}
	}

#endif

#if 0
	ics=vid.buffer;
	ict=vid_vram;
	for(i=0; i<BASEHEIGHT; i++)
	{
//		for(j=0; j<BASEWIDTH; j++)
		for(j=0; j<BASEWIDTH; j+=8)
		{
//			ict[j]=d_8to24table[ics[j]];
//			*ict++=d_8to24table[*ics++];

			ict[0]=d2d_8to24table[ics[0]];
			ict[1]=d2d_8to24table[ics[1]];
			ict[2]=d2d_8to24table[ics[2]];
			ict[3]=d2d_8to24table[ics[3]];
			ict[4]=d2d_8to24table[ics[4]];
			ict[5]=d2d_8to24table[ics[5]];
			ict[6]=d2d_8to24table[ics[6]];
			ict[7]=d2d_8to24table[ics[7]];
			ict+=8; ics+=8;
		}
	}

#endif

//	for(j=0; j<256; j++)
//	{
//		*ict++=d_8to24table[j];
//	}
}

/*
================
D_BeginDirectRect
================
*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
}


/*
================
D_EndDirectRect
================
*/
void D_EndDirectRect (int x, int y, int width, int height)
{
}


