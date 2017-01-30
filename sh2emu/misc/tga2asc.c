#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

#include "bt1h_targa.c"

byte *loadfile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(path, "rb");
	if(!fd)
		return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

int storefile(char *name, byte *ibuf, int isz)
{
	byte *tbuf;
	FILE *fd;
	int sz;

	fd=fopen(name, "wb");
	if(!fd)
	{
		printf("Fail Open Write %s\n", name);
		return(-1);
	}

	fwrite(ibuf, 1, isz, fd);
	fclose(fd);
	return(0);
}

static const u32 clrtab[16]={
	0xFF000000,	0xFF0000AA,	0xFF00AA00,	0xFF00AAAA,
	0xFFAA0000,	0xFFAA00AA,	0xFFAA5500,	0xFFAAAAAA,
	0xFF555555,	0xFF5555FF,	0xFF55FF55,	0xFF55FFFF,
	0xFFFF5555,	0xFFFF55FF,	0xFFFFFF55,	0xFFFFFFFF};

int vqenc_rgb2p16(int cr, int cg, int cb)
{
	int dr, dg, db, d, bd, bi;
	int i, j, k;
	
	bd=999999999; bi=0;
	for(i=0; i<16; i++)
	{
//		dr=cr-((clrtab[i]>>16)&255);
		dr=cr-((clrtab[i]    )&255);
		dg=cg-((clrtab[i]>> 8)&255);
//		db=cb-((clrtab[i]    )&255);
		db=cb-((clrtab[i]>>16)&255);
		d=dr*dr+dg*dg+db*db;
		
		if(d<bd)
			{ bi=i; bd=d; }
	}
	return(bi);
}

int vqenc(byte *iblk, int ystr, u32 *oblk)
{
	short pxy[16];
	int min, max;

	int acr, acg, acb, acy;

	int cr, cg, cb, cy;
	int mr, mg, mb, my;
	int nr, ng, nb, ny;

	int mr2, mg2, mb2, my2;
	int nr2, ng2, nb2, ny2;

	int cy_c, cy_m, cy_y;
	int my_c, my_m, my_y;
	int ny_c, ny_m, ny_y;
	int ax, dy;

	int cya, cyb;
	int ia, ib, ix, px;
	int i, j, k;
	
	my=256; ny=-1;
	my_c=256; ny_c=-1;
	my_m=256; ny_m=-1;
	my_y=256; ny_y=-1;
	acr=0; acg=0; acb=0; acy=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
		cy=(cr+2*cg+cb)/4;

		cy_c=(1*cr+4*cg+3*cb)/8;
		cy_m=(4*cr+1*cg+3*cb)/8;
		cy_y=(3*cr+4*cg+1*cb)/8;

//		cy_c=cg+cb-cr;
//		cy_m=cb+cr-cg;
//		cy_y=cr+cg-cb;

		if(cy<my) { my=cy; }
		if(cy>ny) { ny=cy; }
		if(cy_c<my_c) { my_c=cy_c; }
		if(cy_c>ny_c) { ny_c=cy_c; }
		if(cy_m<my_m) { my_m=cy_m; }
		if(cy_m>ny_m) { ny_m=cy_m; }
		if(cy_y<my_y) { my_y=cy_y; }
		if(cy_y>ny_y) { ny_y=cy_y; }
		
		acr+=cr;	acg+=cg;
		acb+=cb;	acy+=cy;
	}
	
	acr=acr/16;	acg=acg/16;
	acb=acb/16;	acy=acy/16;

	ax=0; dy=ny-my;
//	i=ny_c-my_c; if(i>dy) { ax=1; dy=i; }
//	i=ny_m-my_m; if(i>dy) { ax=2; dy=i; }
//	i=ny_y-my_y; if(i>dy) { ax=3; dy=i; }

	mr=0; mg=0; mb=0; my=0; cya=0;
	nr=0; ng=0; nb=0; ny=0; cyb=0;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];
//		cy=(cr+2*cg+cb)/4;

		switch(ax)
		{
		case 0: cy=(cr+2*cg+cb)/4; break;
//		case 1: cy=cg+cb-cr; break;
//		case 2: cy=cb+cr-cg; break;
//		case 3: cy=cr+cg-cb; break;
		case 1: cy=(1*cr+4*cg+3*cb)/8; break;
		case 2: cy=(4*cr+1*cg+3*cb)/8; break;
		case 3: cy=(3*cr+4*cg+1*cb)/8; break;
		}

		pxy[i*4+j]=cy;
		
		if(cy<acy)
			{ mr+=cr; mg+=cg; mb+=cb; my+=cy; cya++; }
		else
			{ nr+=cr; ng+=cg; nb+=cb; ny+=cy; cyb++; }
	}
	
	if(!cya)cya++;
	if(!cyb)cyb++;

	mr=mr/cya; mg=mg/cya;
	mb=mb/cya; my=my/cya;

	nr=nr/cyb; ng=ng/cyb;
	nb=nb/cyb; ny=ny/cyb;

#if 0
	my=256; ny=-1;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];

//		cr=iblk[(3-i)*ystr+j*4+0];
//		cg=iblk[(3-i)*ystr+j*4+1];
//		cb=iblk[(3-i)*ystr+j*4+2];

		cy=(cr+2*cg+cb)/4;
		
		pxy[i*4+j]=cy;
		if(cy<my)
			{ my=cy; mr=cr; mg=cg; mb=cb; }
		if(cy>ny)
			{ ny=cy; nr=cr; ng=cg; nb=cb; }
	}
#endif

#if 1
	my2=256; ny2=-1;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
		cr=iblk[i*ystr+j*4+0];
		cg=iblk[i*ystr+j*4+1];
		cb=iblk[i*ystr+j*4+2];

//		cy=(cr+2*cg+cb)/4;

		switch(ax)
		{
		case 0: cy=(cr+2*cg+cb)/4; break;
//		case 1: cy=cg+cb-cr; break;
//		case 2: cy=cb+cr-cg; break;
//		case 3: cy=cr+cg-cb; break;
		case 1: cy=(1*cr+4*cg+3*cb)/8; break;
		case 2: cy=(4*cr+1*cg+3*cb)/8; break;
		case 3: cy=(3*cr+4*cg+1*cb)/8; break;
		}
		
		pxy[i*4+j]=cy;
		if(cy<my2)
			{ my2=cy; mr2=cr; mg2=cg; mb2=cb; }
		if(cy>ny2)
			{ ny2=cy; nr2=cr; ng2=cg; nb2=cb; }
	}
	
	mr=(mr+mr2)/2;
	mg=(mg+mg2)/2;
	mb=(mb+mb2)/2;
	my=(my+my2)/2;

	nr=(nr+nr2)/2;
	ng=(ng+ng2)/2;
	nb=(nb+nb2)/2;
	ny=(ny+ny2)/2;
#endif
	
	ia=vqenc_rgb2p16(mr, mg, mb);
	ib=vqenc_rgb2p16(nr, ng, nb);
	ix=(ia<<4)|ib;
	
	cya=(3*my+1*ny)/4;
	cyb=(1*my+3*ny)/4;

//	cya=(5*my+3*ny)/8;
//	cyb=(3*my+5*ny)/8;
	
	px=0;
	if(pxy[ 0]>cya)px|=0x0008;
	if(pxy[ 1]>cyb)px|=0x0004;
	if(pxy[ 2]>cya)px|=0x0002;
	if(pxy[ 3]>cyb)px|=0x0001;

	if(pxy[ 4]>cyb)px|=0x0080;
	if(pxy[ 5]>cya)px|=0x0040;
	if(pxy[ 6]>cyb)px|=0x0020;
	if(pxy[ 7]>cya)px|=0x0010;

	if(pxy[ 8]>cya)px|=0x0800;
	if(pxy[ 9]>cyb)px|=0x0400;
	if(pxy[10]>cya)px|=0x0200;
	if(pxy[11]>cyb)px|=0x0100;

	if(pxy[12]>cyb)px|=0x8000;
	if(pxy[13]>cya)px|=0x4000;
	if(pxy[14]>cyb)px|=0x2000;
	if(pxy[15]>cya)px|=0x1000;
	
	*oblk=px|(ix<<16);
	return(0);
}

int vqdec(byte *iblk, int ystr, u32 blk)
{
	u32 *ct0, *ct1, *ct2, *ct3;
	u32 clra, clrb;

	clra=clrtab[(blk>>20)&15];
	clrb=clrtab[(blk>>16)&15];
	
	ct0=(u32 *)(iblk+0*ystr);
	ct1=(u32 *)(iblk+1*ystr);
	ct2=(u32 *)(iblk+2*ystr);
	ct3=(u32 *)(iblk+3*ystr);
	
	ct0[0]=(blk&0x0008)?clrb:clra;
	ct0[1]=(blk&0x0004)?clrb:clra;
	ct0[2]=(blk&0x0002)?clrb:clra;
	ct0[3]=(blk&0x0001)?clrb:clra;
	ct1[0]=(blk&0x0080)?clrb:clra;
	ct1[1]=(blk&0x0040)?clrb:clra;
	ct1[2]=(blk&0x0020)?clrb:clra;
	ct1[3]=(blk&0x0010)?clrb:clra;
	ct2[0]=(blk&0x0800)?clrb:clra;
	ct2[1]=(blk&0x0400)?clrb:clra;
	ct2[2]=(blk&0x0200)?clrb:clra;
	ct2[3]=(blk&0x0100)?clrb:clra;
	ct3[0]=(blk&0x8000)?clrb:clra;
	ct3[1]=(blk&0x4000)?clrb:clra;
	ct3[2]=(blk&0x2000)?clrb:clra;
	ct3[3]=(blk&0x1000)?clrb:clra;
}

char *emit_chesc(char *t, int v)
{
	if(v=='\"')
	{
		*t++='w';
		return(t);
	}

	if(v=='\\')
	{
		*t++='x';
		return(t);
	}

//	if((v=='\"') || (v=='\''))
//	{
//		*t++='\\';
//		*t++=v;
//		return(t);
//	}

	*t++=v;
	return(t);
}

int main(int argc, char *argv[])
{
	byte *ibuf, *dbuf;
	char *obuf;
	u32 blk;
	char *ifn, *ofn;
	char *t, *t0;
	int xs, ys, cxs, cys;
	int ystr, x, y;
	int i, j, k, l;
	
	ifn=NULL;
	ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		if(!ifn)
			{ ifn=argv[i]; continue; }
		if(!ofn)
			{ ofn=argv[i]; continue; }
	}

	if(!ifn || !ofn)
	{
		return(-1);
	}

	ibuf=BTIC1H_Img_LoadTGA(ifn, &xs, &ys);

	cxs=xs/4;
	cys=ys/4;
	ystr=xs*4;

	dbuf=malloc(xs*ys*4);
	
	obuf=malloc(cxs*cys*8);
	t=obuf;
	
	*t++='\n';
	*t++='\n';
	
	for(i=0; i<cys; i++)
	{
		t0=t;
		*t++='\"';
//		*t++='\x1B';
		*t++='\\';	*t++='x';
		*t++='1';	*t++='B';
		*t++='!';
		for(j=0; j<cxs; j++)
		{
			k=((((cys-i-1)*4)*xs)+(j*4))*4;

//			k=(((i*4)*xs)+(j*4))*4;
			vqenc(ibuf+k, ystr, &blk);
			vqdec(dbuf+k, ystr, blk);
			
			blk=blk&0xFFFFFF;
			if((j+1)<cxs)
				blk|=0x1000000;
//			t=emit_chesc(t, (blk>>21)&127);
//			t=emit_chesc(t, (blk>>14)&127);
//			t=emit_chesc(t, (blk>> 7)&127);
//			t=emit_chesc(t, (blk    )&127);

			t=emit_chesc(t, '!'+(blk/614125)%85);
			t=emit_chesc(t, '!'+(blk/  7225)%85);
			t=emit_chesc(t, '!'+(blk/    85)%85);
			t=emit_chesc(t, '!'+(blk       )%85);
			
			if((t-t0)>=72)
			{
				*t++='\"';
				*t++='\n';
				t0=t;
				*t++='\"';
			}
		}
		
		*t++='\\';
		*t++='n';
		*t++='\"';
		*t++='\n';
	}

//	*t++=0;

	storefile(ofn, obuf, t-obuf);

	BTIC1H_Img_SaveTGA("tga2asc0.tga", dbuf, xs, ys);
}
