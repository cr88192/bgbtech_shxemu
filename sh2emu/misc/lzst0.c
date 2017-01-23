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

#define LZST0_WINSZ		(1<<20)
#define LZST0_WINMSK	(LZST0_WINSZ-1)

int lzst0_hash[4096];
int lzst0_hchain[LZST0_WINSZ];
byte lzst0_hwin[LZST0_WINSZ];
int lzst0_hrov;

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

int lzst0_lookup(byte *cs, byte *cse,
	int *rbi, int *rbl)
{
	int c, d, bi, bl;
	int h, n, lim;
	int i;

	bi=-1; bl=-1;

	d=cse-cs;
	if(d<4)
	{
		*rbi=-1;
		*rbl=-1;
		return(0);
	}
	
//	lim=258;
	lim=4096;
	if(d<lim)
		lim=d;

	h=(cs[0]*4093+cs[1])*4093;
	h=(h>>12)&4095;
	
	c=lzst0_hash[h]; n=256;
	while((c>=0) && ((n--)>0))
	{
		for(i=0; i<lim; i++)
		{
			if(cs[i]!=lzst0_hwin[(c+i)&LZST0_WINMSK])
				break;
		}
		
		d=lzst0_hrov-c;
		if(d<=0)
			d+=LZST0_WINSZ;
		if(d>(LZST0_WINSZ-lim))
			break;
		
		if(d>=65536)
			break;

//		if(d>=16384)
//			break;
		
		if(i && i<4)
		{
			if((d>256) && (i<3))
				i=0;
			if((d>4096) && (i<4))
				i=0;
		}
			
		if(i>bl)
			{ bi=d; bl=i; }

		c=lzst0_hchain[c];
	}
	
	*rbi=bi;
	*rbl=bl;
	return(bl>2);
}

int lzst0_update(byte *cs, int len)
{
	int c, h, n;
	
	n=len;
	while((n--)>0)
	{
		h=(cs[0]*4093+cs[1])*4093;
		h=(h>>12)&4095;
		
		c=lzst0_hrov;
		lzst0_hrov=(lzst0_hrov+1)&LZST0_WINMSK;
		
		lzst0_hwin[c]=*cs++;
		lzst0_hchain[c]=lzst0_hash[h];
		lzst0_hash[h]=c;
	}
	return(0);
}

int lzst0_log2up(int v)
{
	int i, j;
	
	i=0; j=v;
	while(j>1)
		{ i++; j=(j+1)>>1; }
	return(i);
}

int lzst0_log128(int v)
{
	int i, j;
	
	i=0; j=v;
	while(j>=128)
		{ i++; j=j>>7; }
	return(i);
}

int lzst0_log240(int v)
{
	int i, j;
	
	i=0; j=v;
	while(j>=240)
		{ i++; j=j>>8; }
	return(i);
}

int lzst0_log255(int v)
{
	int i, j;
	
	i=0; j=v;
	while(j>=255)
		{ i++; j-=255; }
	return(i);
}

int main(int argc, char *argv[])
{
	int lst[32], dst[32], rst[32];
	int rtst[32];
	byte *ibuf;
	byte *cs, *cse;
	char *ifn;
	int t0, t1, t2, t3;
	int bd0, bl0, bd1, bl1;
	int bl, bd;
	int sz, h, rb;
	int i, j, k;
	
	ifn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			continue;
		}
		ifn=argv[i];
	}
	if(!ifn)
	{
		return(-1);
	}
	
	for(i=0; i<4096; i++)
		lzst0_hash[i]=-1;
	
	for(i=0; i<32; i++)
	{
		lst[i]=0;
		dst[i]=0;
		rst[i]=0;
		
		rtst[i]=0;
	}
	
	ibuf=loadfile(ifn, &sz);
	
	lzst0_hrov=0; rb=0;
	cs=ibuf; cse=ibuf+sz-2;
	while(cs<cse)
	{
		j=lzst0_lookup(cs, cse, &bd0, &bl0);
		
		if(j)
		{
			k=lzst0_lookup(cs+1, cse, &bd1, &bl1);
			if(k)
			{
				if(bl1>(bl0+2))
					j=0;
			}
		}
		
		if(j)
		{
			bl=bl0;
			bd=bd0;
		
			if(bl<2)
			{
				printf("Bad Len %d\n", bl);
				bl=2;
			}
			if(bd<1)
			{
				printf("Bad Dist %d\n", bd);
				bd=1;
			}
		
			dst[lzst0_log2up(bd)]++;
			lst[lzst0_log2up(bl)]++;
			rst[lzst0_log2up(rb)]++;
		
			rtst[16]+=rb;

			if(1)
			{
				if(bd>=65536)
//				if(0)
				{
					rtst[11]+=bl;
				}else
				{
					rtst[12]++;
//					rtst[13]++;
//					rtst[13]+=1+(bd>>8);
					rtst[13]+=2;
					if(bl>14)
					{
						rtst[14]+=1+lzst0_log255(bl);
//						rtst[14]+=1+(bl>>8);
					}
					if(rb>14)
					{
						rtst[15]+=1+lzst0_log255(rb);
//						rtst[15]+=1+(rb>>8);
					}
				}
			}

			if(1)
			{
				rtst[24]++;
				rtst[25]+=1+lzst0_log128(bd);
//				rtst[25]+=2;
				if(bl>14)
				{
//					rtst[26]+=1+lzst0_log255(bl);
					rtst[26]+=1+lzst0_log240(bl);
//					rtst[26]+=1+lzst0_log128(bl);
				}
				if(rb>14)
				{
//					rtst[27]+=1+lzst0_log255(rb);
					rtst[27]+=1+lzst0_log240(rb);
//					rtst[27]+=1+lzst0_log128(rb);
				}
			}

			if(rb>=4)
			{
				rtst[1]++;
				rb=0;
			}

			if((bl<18) && (bd<257))
			{
				rtst[2]++;
//				rtst[16]+=rb;
			}else if(bl<3)
			{
				rtst[16]+=bl;
			}else
			{
				if((bl<258) && (bd<4097))
				{
					rtst[3]++;
				}else
				{
					rtst[5]++;
					if(rb!=0)
						rtst[7]++;
				}
				if((bl<258) && (bd<16385))
				{
					rtst[4]++;
					if(rb!=0)
						rtst[8]++;
				}else
				{
					rtst[6]++;
					if(rb!=0)
						rtst[8]++;
				}
			}
		
			lzst0_update(cs, bl);
			cs+=bl;
			rb=0;
		}else
		{
			lzst0_update(cs, 1);
			cs++;
			rb++;
		}
	}
	
	printf("dist:\n");
	for(i=0; i<4; i++)
	{
		for(j=0; j<8; j++)
		{
			printf("%8d", dst[i*8+j]);
		}
		printf("\n");
	}

	printf("len:\n");
	for(i=0; i<4; i++)
	{
		for(j=0; j<8; j++)
		{
			printf("%8d", lst[i*8+j]);
		}
		printf("\n");
	}
	
	rst[0]=0;
	printf("raw:\n");
	for(i=0; i<4; i++)
	{
		for(j=0; j<8; j++)
		{
			printf("%8d", rst[i*8+j]);
		}
		printf("\n");
	}

	printf("runs:\n");
	for(i=0; i<4; i++)
	{
		for(j=0; j<8; j++)
		{
			printf("%8d", rtst[i*8+j]);
		}
		printf("\n");
	}
	
	t0=rtst[1]+(rtst[2]*2)+(rtst[3]*3)+(rtst[5]*4)+rtst[7]+rtst[16];
	t1=rtst[1]+(rtst[2]*2)+(rtst[4]*3)+(rtst[6]*4)+rtst[8]+rtst[16];

	t2=rtst[12]+rtst[13]+rtst[14]+rtst[15]+ rtst[11]+rtst[16];
	t3=rtst[24]+rtst[25]+rtst[26]+rtst[27]+ rtst[16];
	
	printf("Scen Cost A=%d, B=%d LZ4=%d D=%d\n", t0, t1, t2, t3);
}
