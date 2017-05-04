/*
FELZ8: Byte based variant of FELZ

tag(0): normal match
  bits  0.. 7: ml, match length (0, 4..258 BYTEs)
  bits  8..13: rl, raw length (0..63 BYTEs)
  bits 14..15: al, special tag (0)
  bits 16..31: md, match distance (BYTEs)

tag(1): narrow match
  bits 0..8: md (dist, 0..511)
  bits 9..13: ml (len, 4..34)

tag(2): long raw
  bits  0..13: rl, raw length (BYTEs)
  bits 14..15: al, special tag (2)

tag(3): shallow match
  bits 0.. 3: md (dist, 0..15)
  bits 4.. 8: ml (len, 4..34)
  bits 9..11: rl (raw, 0..7)
  bit 12..13: 0

ml!=0:
  md=distance
  ml=length
  mr=raw
  al=special tag, reserved for now

ml==0, rl==0, al==0:
	md==0: EOB
	md!=0: long run of raw BYTEs


Standalone File Header:
	FOURCC magic0;	//'FeLZ'
	TWOCC magic1;	//'08'
	BYTE ver;		//1, format version
	BYTE resv;		//0, reserved
	DWORD csize;	//compressed size (includes header)
	DWORD dsize;	//decompressed size
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#endif

#if defined(__linux) || defined(__EMSCRIPTEN__)
#include <sys/time.h>
#endif


#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif

#ifndef ARCH_64BIT
#define ARCH_64BIT
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#endif

#ifdef __BIG_ENDIAN__
#define BIGENDIAN
#endif

#ifdef __LITTLE_ENDIAN__
#define LTLENDIAN
#endif

#if defined(X86) || defined(X86_64)
#ifndef LTLENDIAN
#define LTLENDIAN
#endif
#endif

#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifndef __EMSCRIPTEN__
#ifdef __GNUC__
#define force_inline inline
#define default_inline inline
#endif
#endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif


typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
#if defined(__linux) && defined(__x86_64__)
typedef unsigned long u64;
typedef signed long s64;
#else
typedef unsigned long long u64;
typedef signed long long s64;
#endif

typedef struct BGBDT_FeLzContext_s BGBDT_FeLzContext;

struct BGBDT_FeLzContext_s {
u32 mark1;

byte use_lvm;		//use multi-level matching (log2)
// byte use_lvb;	//enable byte matching

byte *css;
byte *cse;
//u32 lzhash[16384];
u32 lzhash[65536];
byte lzhrov[4096];
};

#if defined(X86) || defined(X86_64)

#define felz_getu32(ptr)			(*(u32 *)(ptr))
#define felz_getu32le(ptr)			(*(u32 *)(ptr))
#define felz_setu32le(ptr, val)		(*(u32 *)(ptr)=(val))

#define felz_getu32lea(ptr)			(*(u32 *)(ptr))
#define felz_setu32lea(ptr, val)	(*(u32 *)(ptr)=(val))

#define felz_getu16(ptr)			(*(u16 *)(ptr))
#define felz_getu16le(ptr)			(*(u16 *)(ptr))
#define felz_setu16le(ptr, val)		(*(u16 *)(ptr)=(val))

#define felz_getu16lea(ptr)			(*(u16 *)(ptr))
#define felz_setu16lea(ptr, val)	(*(u16 *)(ptr)=(val))

#else

u32 bgbdt_felz_getu32(byte *ptr)
{
	u32 v;
#if defined(BIGENDIAN)
	v=ptr[3]|(ptr[2]<<8)|(ptr[1]<<16)|(ptr[0]<<24);
#elif defined(LTLENDIAN)
	v=ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24);
#else
	static int tv=1;
	if(*(byte *)(&tv)==1)
		{ v=ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24); }
	else
		{ v=ptr[3]|(ptr[2]<<8)|(ptr[1]<<16)|(ptr[0]<<24); }
#endif
	return(v);
}

default_inline u32 bgbdt_felz_getu32le(byte *ptr)
{
	u32 v;
	v=ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24);
	return(v);
}

default_inline void bgbdt_felz_setu32le(byte *ptr, u32 val)
{	ptr[0]=val;		ptr[1]=val>>8;
	ptr[2]=val>>16;	ptr[3]=val>>24;		}

#define felz_getu32(ptr)			bgbdt_felz_getu32((byte *)(ptr))
#define felz_getu32le(ptr)			bgbdt_felz_getu32le((byte *)(ptr))
#define felz_setu32le(ptr, val)		bgbdt_felz_setu32le((byte *)(ptr), (val))

#define felz_getu32a(ptr)			(*(u32 *)(ptr))

#if defined(LTLENDIAN)
#define felz_getu32lea(ptr)			(*(u32 *)(ptr))
#define felz_setu32lea(ptr, val)	(*(u32 *)(ptr)=(val))
#elif defined(BIGENDIAN)
default_inline u32 bgbdt_felz_bswap32(u32 v)
{
	v=((v>> 8)&0x00FF00FF)|((v<< 8)&0xFF00FF00);
//	v=((v>>16)&0x0000FFFF)|((v<<16)&0xFFFF0000);
	v=((v>>16)           )|((v<<16)           );
	return(v);
}

#define felz_getu32lea(ptr)			bgbdt_felz_bswap32(*(u32 *)(ptr))
#define felz_setu32lea(ptr, val)	(*(u32 *)(ptr)=bgbdt_felz_bswap32(val))
#else
#define felz_getu32lea(ptr)			bgbdt_felz_getu32le((byte *)(ptr))
#define felz_setu32lea(ptr, val)	bgbdt_felz_setu32le((byte *)(ptr), (val))
#endif

#endif

int FRGL_TimeMS()
{
#ifdef _WIN32
	static unsigned int init;
	unsigned int t;

	t=timeGetTime();
	if(!init)init=t;

	return((unsigned int)(t-init));
#elif defined(__EMSCRIPTEN__) || defined(linux)
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#else
	static int init;
	int t;

	t=clock();
	t*=CLOCKS_PER_SEC/1000.0;
	if(!init)init=t;
	return((unsigned int)(t-init));
#endif
}


int BGBDT_FeLz8_LookupMatchB(BGBDT_FeLzContext *ctx,
	byte *cs, byte *css, byte *cse, int *rml, int *rmd);

int BGBDT_FeLz8_LookupMatchA(BGBDT_FeLzContext *ctx,
	byte *cs, byte *css, byte *cse, int *rml, int *rmd)
{
	byte *s, *se, *t, *te, *ct;
	byte *ctb, *tb;
	u32 v0, v, vs, vt, h, h1, o, o1, co;
	int hr, hm, sd;
	int l, d, rt, al, bl, bd, bal;
	int i, j, k;

	if(ctx->use_lvm)
	{
		v0=felz_getu32(cs);
		h=((v0*65521)>>16)&4095;
		hr=ctx->lzhrov[h];
		h1=h<<ctx->use_lvm;
		sd=1<<ctx->use_lvm;
		hm=sd-1;

		bl=0; bd=0;

		co=cs-css;

		for(i=0; i<sd; i++)
		{
			o=ctx->lzhash[h1|((hr+i)&hm)]^v0;

			if((o>=co) || ((co-o)>>16))
				continue;

			ct=css+o;

			s=cs; se=s+258;
			if(cse<se)se=cse;
			t=ct;

			while(s<se)
			{
				vs=felz_getu32(s);
				vt=felz_getu32(t);
				if(vs!=vt)
				{
					while(s<se)
					{
						if(*s!=*t)
							break;
						s++; t++;
					}
					break;
				}
				s+=4; t+=4;
			}
			
			l=s-cs;
			d=cs-ct;
			
			if(l>258)
				l=258;
			
			if((l>bl) && (d<65536))
				{ bl=l; bd=d; }
		}

		rt=(bl>=4) && (bd<65536);
		
		if(!rt)
		{
			hr=(hr-1)&hm;
			ctx->lzhrov[h]=hr;
			ctx->lzhash[h1|hr]=co^v0;
		}

		*rml=bl;
		*rmd=bd;
		return(rt);
	}
	
	return(BGBDT_FeLz8_LookupMatchB(ctx, cs, css, cse, rml, rmd));
}

int BGBDT_FeLz8_UpdateStringB(BGBDT_FeLzContext *ctx,
	byte *cs, byte *css, byte *cse, int len);

int BGBDT_FeLz8_UpdateStringA(BGBDT_FeLzContext *ctx,
	byte *cs, byte *css, byte *cse, int len)
{
	u32 v;
	int i, h, h1, hr, hm;

	if(ctx->use_lvm)
	{
		hm=(1<<ctx->use_lvm)-1;

		i=len;
		while(i--)
		{
			v=felz_getu32(cs);
			h=((v*65521)>>16)&4095;
			h1=h<<ctx->use_lvm;

			hr=ctx->lzhrov[h];
			hr=(hr-1)&hm;
			ctx->lzhrov[h]=hr;
			ctx->lzhash[h1|hr]=(cs-css)^v;
			cs++;
		}

		return(0);
	}

	return(BGBDT_FeLz8_UpdateStringB(ctx, cs, css, cse, len));
}


int BGBDT_FeLz8_LookupMatchB(BGBDT_FeLzContext *ctx,
	byte *cs, byte *css, byte *cse, int *rml, int *rmd)
{
	byte *s, *se, *t, *te, *ct;
	u32 v0, v, vs, vt, h, h1, o, co;
	int l, d;
	int i, j, k;

	v0=felz_getu32(cs);
	h=((v0*65521)>>16)&4095;

	o=ctx->lzhash[h]^v0;
	co=cs-css;

	ctx->lzhash[h]=co^v0;

	if((o>=co) || ((co-o)>>16))
	{
//		ctx->lzhash[h]=co^v0;
		*rml=0; *rmd=0;
		return(0);
	}

	ct=css+o;
	
	s=cs; se=s+258;
	if(cse<se)se=cse;
	t=ct;

#if 0
	while(s<se)
	{
		if(*s!=*t)
			break;
		s++; t++;
	}
#endif
	
#if 1
	while(s<se)
	{
		vs=felz_getu32(s);
		vt=felz_getu32(t);
		if(vs!=vt)
		{
			while(s<se)
			{
				if(*s!=*t)
					break;
				s++; t++;
			}
			break;
		}
		s+=4; t+=4;
	}
	if(s>se)s=se;
#endif

	l=s-cs;
	d=cs-ct;
	*rml=l;
	*rmd=d;
	return((l>=4) && (d<65536));
}

int BGBDT_FeLz8_UpdateStringB(BGBDT_FeLzContext *ctx,
	byte *cs, byte *css, byte *cse, int len)
{
	u32 v;
	int i, h, h1, hr, hm;

	i=len;
	while(i--)
	{
		v=felz_getu32(cs);
		h=((v*65521)>>16)&4095;
		ctx->lzhash[h]=(cs-css)^v;
		cs++;
	}
	return(0);
}

int BGBDT_FeLz8_EncodeBufferCtx(BGBDT_FeLzContext *ctx,
	byte *ibuf, int ibsz, byte *obuf, int obsz, int lvl)
{
	int (*LookupMatch)(BGBDT_FeLzContext *ctx,
		byte *cs, byte *css, byte *cse, int *rml, int *rmd);
	int (*UpdateString)(BGBDT_FeLzContext *ctx,
		byte *cs, byte *css, byte *cse, int len);

	byte *cs, *cse, *csrb;
	byte *ct, *cte;
	u32 tg;
	int ml, md, md1, rl, al, lmd;
	int i, j, k;

	LookupMatch=BGBDT_FeLz8_LookupMatchA;
	UpdateString=BGBDT_FeLz8_UpdateStringA;

	switch(lvl)
	{
	case 0:
	case 1:
		LookupMatch=BGBDT_FeLz8_LookupMatchB;
		UpdateString=BGBDT_FeLz8_UpdateStringB;
		ctx->use_lvm=0;
		break;
	case 2:
		ctx->use_lvm=1;
		break;
	case 3:
		ctx->use_lvm=2;
		break;
	case 4:
		ctx->use_lvm=3;
		break;
	case 5:
		ctx->use_lvm=4;
		break;

	case 6:
	case 7:
	case 8:
	case 9:
		ctx->use_lvm=4;
		break;
	}

	ml=4096<<ctx->use_lvm;

	for(i=0; i<ml; i++)
	{
		ctx->lzhash[i]=0;
	}

	for(i=0; i<4096; i++)
		ctx->lzhrov[i]=0;
		
	ctx->css=ibuf;
	ctx->cse=ibuf+ibsz;
	
	cs=ibuf; cse=cs+ibsz;
	ct=obuf; cte=ct+obsz;
	csrb=cs;
	lmd=-1;
	
	while(cs<cse)
	{	
		if(LookupMatch(ctx, cs, ibuf, cse, &ml, &md))
		{
//			if((ml<4) || (ml>258))
//				__debugbreak();
//			if((md<1) || (md>65535))
//				__debugbreak();
		
			rl=cs-csrb;
			while(rl>63)
			{
				if(rl>=16384)
					rl=16383;
				felz_setu16le(ct, rl|0x8000); ct+=2;
				i=rl;
				while(i--)
					{ *ct++=*csrb++; }
				rl=cs-csrb;
			}
			
			md1=md;
			if(md==lmd)md1=0;
			lmd=md;
			
			if((rl<8) && (md1<16) && (ml<35))
			{
				tg=0xC000|(rl<<9)|((ml-3)<<4)|md1;
				felz_setu16le(ct, tg); ct+=2;
			}else if(!rl && (md<512) && (ml<35))
			{
				tg=0x4000|((ml-3)<<9)|md;
				felz_setu16le(ct, tg); ct+=2;
			}else 
			{
				tg=(md<<16)|(rl<< 8)|(ml-3);
				felz_setu32le(ct, tg); ct+=4;
			}

			i=rl;
			while(i--)
				{ *ct++=*csrb++; }
			UpdateString(ctx, cs, ibuf, cse, ml);
			cs+=ml;
			csrb=cs;
		}else
		{
//			UpdateString(ctx, cs, ibuf, cse, 1);
			cs++;
		}
	}

	rl=cs-csrb;
	while(rl>0)
	{
		if(rl>=16384)
			rl=16383;
		felz_setu16le(ct, rl|0x8000); ct+=2;
		i=rl;
		while(i--)
			{ *ct++=*csrb++; }
		rl=cs-csrb;
	}
	
	felz_setu16le(ct, 0x8000); ct+=2;
	return(ct-obuf);
}

int BGBDT_FeLz8_EncodeBuffer(
	byte *ibuf, int ibsz, byte *obuf, int obsz, int lvl)
{
	BGBDT_FeLzContext tctx;
	return(BGBDT_FeLz8_EncodeBufferCtx(&tctx, ibuf, ibsz, obuf, obsz, lvl));
}

force_inline void felz_memcpy_u32b(byte *dst, byte *src, int num)
{
	u32 *cs, *ct, *cte;

//#if defined(X86_64) && !defined(__GNUC__)
#if 0
	cs=(u32 *)src; ct=(u32 *)dst; cte=ct+num;
	while(ct<cte)
	{
		((u64 *)ct)[0]=((u64 *)cs)[0];
		((u64 *)ct)[1]=((u64 *)cs)[1];
		ct+=4; cs+=4;
	}
#else
	cs=(u32 *)src; ct=(u32 *)dst; cte=ct+num;
	while(ct<cte)
	{
		ct[0]=cs[0];
		ct[1]=cs[1];
		ct[2]=cs[2];
		ct[3]=cs[3];
//		ct+=2; cs+=2;
		ct+=4; cs+=4;
	}
#endif
}

force_inline void felz_memcpy8(byte *dst, byte *src, int len)
{
//	byte *cs, *ct, *cte;
	u32 *cs, *ct, *cte;

#if 1
	cs=(u32 *)src; ct=(u32 *)dst; cte=(u32 *)(dst+len);
	while(ct<cte)
	{
		ct[0]=cs[0];
		ct[1]=cs[1];
		ct[2]=cs[2];
		ct[3]=cs[3];
//		ct+=2; cs+=2;
		ct+=4; cs+=4;
	}
#endif

#if 0
//	if(((dst-src)>=8) || (dst<src))
//	if(((dst-src)>=4) || (dst<src))
	if(1)
	{
		felz_memcpy_u32b(dst, src, (len+3)/4);
		return;
	}

	cs=src; ct=dst; cte=ct+len;
	while(ct<cte)
		{ *ct++=*cs++; }
#endif
}

force_inline void felz_memcpy8f(byte *dst, byte *src, int len)
{
//	byte *cs, *ct, *cte;

#if defined(X86_64) && !defined(__GNUC__)
	u64 *cs, *ct, *cte;
	cs=(u64 *)src; ct=(u64 *)dst; cte=(u64 *)(dst+len);
	while(ct<cte)
	{
		ct[0]=cs[0];
		ct[1]=cs[1];
		ct+=2; cs+=2;
	}
#else
	u32 *cs, *ct, *cte;
	cs=(u32 *)src; ct=(u32 *)dst; cte=(u32 *)(dst+len);
	while(ct<cte)
	{
		ct[0]=cs[0];
		ct[1]=cs[1];
		ct[2]=cs[2];
		ct[3]=cs[3];
//		ct+=2; cs+=2;
		ct+=4; cs+=4;
	}
#endif
}

force_inline void felz_memset_u32b(byte *dst, u32 val, int num)
{
	u32 *ct, *cte;
	
#if defined(X86_64) && !defined(__GNUC__)
	u64 lv;

	lv=(((u64)val)<<32)|val;
	ct=(u32 *)dst; cte=ct+num;
	while(ct<cte)
	{
		((u64 *)ct)[0]=lv;
//		((u64 *)ct)[1]=lv;
//		ct+=4;
		ct+=2;
	}
#else
	ct=(u32 *)dst; cte=ct+num;
	while(ct<cte)
	{
		ct[0]=val;	ct[1]=val;
		ct[2]=val;	ct[3]=val;
		ct+=4;
	}
#endif
}

force_inline void felz_memset3_u32b(byte *dst, u32 v0, u32 v1, u32 v2, int num)
{
	u32 *ct, *cte;
	
	ct=(u32 *)dst; cte=ct+num;
	while(ct<cte)
		{ ct[0]=v0; ct[1]=v1; ct[2]=v2; ct+=3; }
}

int BGBDT_FeLz8_DecodeBuffer(
	byte *ibuf, int ibsz, byte *obuf, int obsz)
{
	byte *cs, *cse, *cs1;
	byte *ct, *ct1, *cte;
	byte *cs1b;
	u64 lv;
	u32 tg, v, v0, v1, v2;
	int ml, md, rl, al, lmd;
	int i, j, k;

	cs=ibuf; cse=cs+ibsz;
	ct=obuf; cte=ct+obsz;
	lmd=1;

	while(cs<cse)
	{
		tg=felz_getu32le(cs); cs+=4;

		al=(tg>>14)& 3;
		if(al)
		{
			if(al==3)
			{
				cs-=2;
				md=(tg   )& 15;
				ml=(tg>>4)& 31;
				rl=(tg>>9)&  7;
				if(!md)md=lmd;
			}else
				if(al==1)
			{
				cs-=2;
				ml=(tg>>9)& 31;
				md=(tg   )&511;
				rl=0;
			}else
				if(al==2)
			{
				cs-=2;
				ml=0; md=0;
				rl=tg&16383;
			}else
			{
				__debugbreak();
			}
		}else
		{
			ml=(tg    )&  255;
			rl=(tg>> 8)&   63;
			md=(tg>>16)&65535;
		}

		if(ml)
		{
			if(rl)
			{
				felz_memcpy8f(ct, cs, rl);
				ct+=rl;		cs+=rl;
			}

//			if(!md)md=lmd;
			lmd=md;

			cs1=ct-md;
			ml+=3;
			if(md<8)
//			if(0)
			{
#if defined(LTLENDIAN) || defined(BIGENDIAN)
				switch(md)
				{
				case 1:
					v=cs1[0]; v=v|(v<<8); v=v|(v<<16);
					felz_memset_u32b(ct, v, (ml+3)/4);
					ct+=ml;
					break;
				case 2:
#ifdef LTLENDIAN
					v=cs1[0]|(cs1[1]<<8); v=v|(v<<16);
#else
					v=cs1[1]|(cs1[0]<<8); v=v|(v<<16);
#endif
					felz_memset_u32b(ct, v, (ml+3)/4);
					ct+=ml;
					break;
				case 3:
#ifdef LTLENDIAN
					v=cs1[0]|(cs1[1]<<8)|(cs1[2]<<16);
					v0=v|(v<<24);
					v1=(v>>8)|(v<<16);
					v2=(v>>16)|(v<<8);
#else
					v=cs1[2]|(cs1[1]<<8)|(cs1[0]<<16);
					v0=(v<< 8)|(v>>16);
					v1=(v<<16)|(v>> 8);
					v2=(v<<24)|(v    );
#endif
					felz_memset3_u32b(ct, v0, v1, v2, (ml+3)/4);
					ct+=ml;
					break;
				case 4:
					v=felz_getu32(cs1);
					felz_memset_u32b(ct, v, (ml+3)/4);
					ct+=ml;
					break;
				default:
					felz_memcpy8(ct, cs1, ml);
					ct+=ml;
					break;
				}
#else
				felz_memcpy8(ct, cs1, ml);
				ct+=ml;
#endif
			}else
			{
				felz_memcpy8f(ct, cs1, ml);
				ct+=ml;
			}
			continue;
		}else if(rl)
		{
			felz_memcpy8f(ct, cs, rl);
			ct+=rl;		cs+=rl;
			continue;
		}else
		{
			break;
		}
	}
	
	return(ct-obuf);
}

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

int felz_memcpy32(void *dst, void *src, int len)
{
	u32 *cs, *cse, *ct;
	
	cs=src; cse=cs+(len+3)/4;
	ct=dst;
	
	while(cs<cse)
	{
		ct[0]=cs[0];
		ct[1]=cs[1];
		ct[2]=cs[2];
		ct[3]=cs[3];
		ct+=4; cs+=4;
	}
	return(0);
}

int help(char *prg)
{
	printf("usage: %s opts* infile outfile\n", prg);
	printf("\t-d Decode\n");
	printf("\t-1 .. -9 Compression Level\n");
	return(0);
}

int main(int argc, char *argv[])
{
	byte *ibuf, *obuf, *tbuf;
	byte *cs, *cse;
	char *ifn, *ofn;
	byte dec, lvl;
	s64 li0;
	double f, g;
	int t0, t1, t2, t3;
	int bd0, bl0, bd1, bl1;
	int bl, bd;
	int sz, csz, dsz, h, rb;
	int i, j, k;
	
	ifn=NULL; ofn=NULL; dec=0; lvl=1;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-d"))
				{ dec=1; continue; }
			if(!strcmp(argv[i], "-t"))
				{ dec=2; continue; }

			if(!strcmp(argv[i], "-1"))
				{ lvl=1; continue; }
			if(!strcmp(argv[i], "-2"))
				{ lvl=2; continue; }
			if(!strcmp(argv[i], "-3"))
				{ lvl=3; continue; }
			if(!strcmp(argv[i], "-4"))
				{ lvl=4; continue; }
			if(!strcmp(argv[i], "-5"))
				{ lvl=5; continue; }
			if(!strcmp(argv[i], "-6"))
				{ lvl=6; continue; }
			if(!strcmp(argv[i], "-7"))
				{ lvl=7; continue; }
			if(!strcmp(argv[i], "-8"))
				{ lvl=8; continue; }
			if(!strcmp(argv[i], "-9"))
				{ lvl=9; continue; }
		
			continue;
		}
		if(!ifn)
			{ ifn=argv[i]; continue; }
		if(!ofn)
			{ ofn=argv[i]; continue; }
	}

	if(!ifn || !ofn)
	{
		help(argv[0]);
		return(-1);
	}
	
	if(dec==0)
	{
		ibuf=loadfile(ifn, &sz);
		obuf=malloc(sz*2);

		i=BGBDT_FeLz8_EncodeBuffer(
			ibuf, sz, obuf+16, 2*sz, lvl);

		csz=i+16;
		obuf[0]='F';	obuf[1]='e';
		obuf[2]='L';	obuf[3]='Z';
		obuf[4]='0';	obuf[5]='8';
		obuf[6]=1;		obuf[7]=0;
		felz_setu32le(obuf+ 8, csz);
		felz_setu32le(obuf+12, sz);
		
		printf("Enc %d -> %d (%.2f%%)\n", sz, csz, (100.0*csz)/(sz+1));
		
		if(ofn)
		{
			storefile(ofn, obuf, csz);
		}
		
		return(0);
	}

	if(dec==1)
	{
		ibuf=loadfile(ifn, &csz);

		if(memcmp(ibuf, "FeLZ08", 6) || (ibuf[6]!=1) || ibuf[7])
		{
			printf("FeLZ Magic Failed\n");
			return(-1);
		}

		csz=felz_getu32le(ibuf+ 8);
		dsz=felz_getu32le(ibuf+12);
		obuf=malloc(dsz+4096);
		
		BGBDT_FeLz8_DecodeBuffer(
			ibuf+16, csz-16, obuf, dsz);
		storefile(ofn, obuf, dsz);

		return(0);
	}

	if(dec==2)
	{
		ibuf=loadfile(ifn, &sz);
		obuf=malloc(sz*2);
		tbuf=malloc(sz*2);
		
		dsz=sz;

		i=BGBDT_FeLz8_EncodeBuffer(
			ibuf, sz, obuf+16, 2*sz, lvl);
		csz=i+16;

		obuf[0]='F';	obuf[1]='e';
		obuf[2]='L';	obuf[3]='Z';
		obuf[4]='0';	obuf[5]='8';
		obuf[6]=1;		obuf[7]=0;
		felz_setu32le(obuf+ 8, csz);
		felz_setu32le(obuf+12, sz);
		
		printf("Test %d -> %d (%.2f%%)\n", sz, csz, (100.0*csz)/(sz+1));
		
		if(ofn)
		{
			storefile(ofn, obuf, csz);
		}
		
//		printf("A0A\n");

		i=BGBDT_FeLz8_DecodeBuffer(
			obuf+16, csz-16, tbuf, dsz);

//		printf("A0B\n");
		
		if(i!=dsz)
		{
			printf("Size Mismatch %d -> %d\n", dsz, i);
		}
		
		if(memcmp(ibuf, tbuf, dsz))
		{
			printf("Data Mismatch\n");
			return(-1);
		}

		t0=FRGL_TimeMS();
		t2=t0+10000;
		t1=t0;

		printf("Encode:\n");
		li0=0;
		while((t1<t2) && (t1>=t0))
		{
			i=BGBDT_FeLz8_EncodeBuffer(
				ibuf, sz, obuf+16, 2*sz, lvl);
			li0+=sz;

			f=(t1-t0)/(1000.0);
			g=li0/(f*1000000.0);
			printf("%.2fs %.2fMB/s  \r", f, g);
			t1=FRGL_TimeMS();
		}
		printf("\n");
		
		t0=FRGL_TimeMS();
		t2=t0+10000;
		t1=t0;
		
		
		printf("Decode:\n");
		li0=0;
		while((t1<t2) && (t1>=t0))
		{
			i=BGBDT_FeLz8_DecodeBuffer(
				obuf+16, csz-16, tbuf, dsz);
			li0+=i;

			f=(t1-t0)/(1000.0);
			g=li0/(f*1000000.0);
			printf("%.2fs %.2fMB/s  \r", f, g);
			t1=FRGL_TimeMS();
		}
		printf("\n");

		
		t0=FRGL_TimeMS();
		t2=t0+1000;
		t1=t0;
		printf("Memcpy:\n");
		li0=0;
		while((t1<t2) && (t1>=t0))
		{
//			memcpy(tbuf, ibuf, dsz);
			felz_memcpy32(tbuf, ibuf, dsz);
			li0+=dsz;

			f=(t1-t0)/(1000.0);
			g=li0/(f*1000000.0);
			printf("%.2fs %.2fMB/s  \r", f, g);
			t1=FRGL_TimeMS();
		}
		printf("\n");

		return(0);
	}
}
