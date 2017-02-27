#define JXGPU_PRTY_TRIANGLE			0x01

#define JXGPU_PRMD_TEX2D			0x10
#define JXGPU_PRMD_DEPTH			0x20
#define JXGPU_PRMD_COLOR			0x40

#define JXGPU_PRTY_CLRZTRI			\
	(JXGPU_PRTY_TRIANGLE|JXGPU_PRMD_DEPTH|JXGPU_PRMD_COLOR)
#define JXGPU_PRTY_TEXZTRI			\
	(JXGPU_PRTY_TRIANGLE|JXGPU_PRMD_DEPTH|JXGPU_PRMD_TEX2D)
#define JXGPU_PRTY_CLRTEXZTRI			\
	(JXGPU_PRTY_TRIANGLE|JXGPU_PRMD_DEPTH|JXGPU_PRMD_TEX2D|JXGPU_PRMD_COLOR)

u32 *tk_jxgpu_vreg;
u32 *tk_jxgpu_vram;

u16 *tk_jxgpu_fbuf;
u16 *tk_jxgpu_zbuf;
u32 *tk_jxgpu_cbuf;
u32 *tk_jxgpu_tbuf;
u32 *tk_jxgpu_trov;

int tk_jxgpu_cbuf_mask;

void sleep(int ms);

void tk_jxgpu_clear()
{
	int i;

//	memset(tk_jxgpu_fbuf, 0, 640*480*2);
	memset(tk_jxgpu_zbuf, 255, 640*480*2);
	
	for(i=0; i<640*480; i++)
	{
		tk_jxgpu_fbuf[i]=0x0210;
	}
}

void tk_jxgpu_setup()
{
//	u32 *vreg;
//	u32 *vram;
	u32 px;
	int i, j, k;

	tk_jxgpu_vreg=(u32 *)0xA05F8000;
	tk_jxgpu_vreg[(0x44/4)]=0x00000085;
//	tk_jxgpu_vreg[(0x5C/4)]=320|(240<<10)|(320<<20);
//	tk_jxgpu_vreg[(0x5C/4)]=319|(239<<10)|((320/2+1)<<20);
	tk_jxgpu_vreg[(0x5C/4)]=319|(479<<10)|((640/2+1)<<20);

//	sleep(1);
	
	tk_jxgpu_vram=(u32 *)0xA5000000;
	tk_jxgpu_fbuf=(u16 *)tk_jxgpu_vram;
	tk_jxgpu_zbuf=tk_jxgpu_fbuf+640*480;
	tk_jxgpu_cbuf=(u32 *)(tk_jxgpu_zbuf+640*480);
	tk_jxgpu_tbuf=(u32 *)(tk_jxgpu_cbuf+4096);
	tk_jxgpu_trov=(u32 *)tk_jxgpu_tbuf;
	
	tk_jxgpu_cbuf_mask=4095;

	tk_jxgpu_vreg[(0x200/4)]=((byte *)tk_jxgpu_fbuf)-((byte *)tk_jxgpu_vram);
	tk_jxgpu_vreg[(0x204/4)]=((byte *)tk_jxgpu_zbuf)-((byte *)tk_jxgpu_vram);
	tk_jxgpu_vreg[(0x208/4)]=((byte *)tk_jxgpu_cbuf)-((byte *)tk_jxgpu_vram);
	tk_jxgpu_vreg[(0x20C/4)]=4096*4;
	tk_jxgpu_vreg[(0x210/4)]=0;
	tk_jxgpu_vreg[(0x214/4)]=0;

	tk_jxgpu_vreg[(0x44/4)]=0x01000085;

//	sleep(1000);
//	vreg[0x11]=0x0000;
}

void tk_jxgpu_submit_tri(int mode, int tex, float *xyz, float *st, u32 *clr)
{
	int srov, erov, msk;
	
	srov=tk_jxgpu_vreg[(0x214/4)]>>2;
	erov=tk_jxgpu_vreg[(0x214/4)]>>2;
	msk=tk_jxgpu_cbuf_mask;
	tk_jxgpu_cbuf[(erov+0)&msk]=(mode<<16)|0x01;
	tk_jxgpu_cbuf[(erov+1)&msk]=tex;
	erov+=2;

	((f32 *)tk_jxgpu_cbuf)[(erov+0)&msk]=xyz[0];
	((f32 *)tk_jxgpu_cbuf)[(erov+1)&msk]=xyz[1];
	((f32 *)tk_jxgpu_cbuf)[(erov+2)&msk]=xyz[4];
	((f32 *)tk_jxgpu_cbuf)[(erov+3)&msk]=xyz[5];
	((f32 *)tk_jxgpu_cbuf)[(erov+4)&msk]=xyz[8];
	((f32 *)tk_jxgpu_cbuf)[(erov+5)&msk]=xyz[9];
	erov+=6;
	
	if(mode&0x10)
	{
		((f32 *)tk_jxgpu_cbuf)[(erov+0)&msk]=st[0];
		((f32 *)tk_jxgpu_cbuf)[(erov+1)&msk]=st[1];
		((f32 *)tk_jxgpu_cbuf)[(erov+2)&msk]=st[2];
		((f32 *)tk_jxgpu_cbuf)[(erov+3)&msk]=st[3];
		((f32 *)tk_jxgpu_cbuf)[(erov+4)&msk]=st[4];
		((f32 *)tk_jxgpu_cbuf)[(erov+5)&msk]=st[5];
		erov+=6;
	}

	if(mode&0x20)
	{
		((f32 *)tk_jxgpu_cbuf)[(erov+0)&msk]=xyz[2];
		((f32 *)tk_jxgpu_cbuf)[(erov+1)&msk]=xyz[6];
		((f32 *)tk_jxgpu_cbuf)[(erov+2)&msk]=xyz[10];
		erov+=3;
	}

	if(mode&0x40)
	{
		tk_jxgpu_cbuf[(erov+0)&msk]=clr[0];
		tk_jxgpu_cbuf[(erov+1)&msk]=clr[1];
		tk_jxgpu_cbuf[(erov+2)&msk]=clr[2];
		erov+=3;
	}

	tk_jxgpu_vreg[(0x214/4)]=erov<<2;
}

void tk_jxgpu_submit_tex(int mip, int tex, int xs, int ys, void *addr)
{
	int srov, erov, msk;
	
	srov=tk_jxgpu_vreg[(0x214/4)]>>2;
	erov=tk_jxgpu_vreg[(0x214/4)]>>2;
	msk=tk_jxgpu_cbuf_mask;
	tk_jxgpu_cbuf[(erov+0)&msk]=(mip<<8)|0x02;
	tk_jxgpu_cbuf[(erov+1)&msk]=tex;
	tk_jxgpu_cbuf[(erov+2)&msk]=((byte *)addr)-((byte *)tk_jxgpu_vram);
	erov+=3;
	
	if(!mip)
	{
		tk_jxgpu_cbuf[(erov+0)&msk]=(ys<<16)|xs;
		tk_jxgpu_cbuf[(erov+1)&msk]=0;
		erov+=2;
	}

	tk_jxgpu_vreg[(0x214/4)]=erov<<2;
}


void tk_jxgpu_test()
{
	float xyz[16];
	float st[8];
	u32 clr[4];
	u16 *tptr;
	int i, j, k;

	tk_jxgpu_setup();
	tk_jxgpu_clear();
	
	tptr=(u16 *)tk_jxgpu_trov;
	tk_jxgpu_trov+=16*16/2;
	
	for(i=0; i<16; i++)
		for(j=0; j<16; j++)
	{
		k=((i^j)&1)?0x7FFFF:0x7C00;
		tptr[i*16+j]=k;
	}

	tk_jxgpu_submit_tex(0, 1, 16, 16, tptr);

	xyz[0]=128;	xyz[1]=128;	xyz[2]=0;
	st[0]=0;	st[1]=0;	clr[0]=0xFE10;
	xyz[4]=128;	xyz[5]=256;	xyz[6]=0;
	st[2]=0;	st[3]=1;	clr[1]=0x0210;
//	xyz[8]=256;	xyz[9]=256;	xyz[10]=0;
	xyz[8]=256;	xyz[9]=192;	xyz[10]=0;
	st[4]=1;	st[5]=1;	clr[2]=0x7FFF;

//	tk_jxgpu_submit_tri(JXGPU_PRTY_CLRZTRI, 0, xyz, st, clr);
//	tk_jxgpu_submit_tri(JXGPU_PRTY_TEXZTRI, 1, xyz, st, clr);
	tk_jxgpu_submit_tri(JXGPU_PRTY_CLRTEXZTRI, 1, xyz, st, clr);

	sleep(30000);
	tk_jxgpu_vreg[0x11]=0x0000;
}
