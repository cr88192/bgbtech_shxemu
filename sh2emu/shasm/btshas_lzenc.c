#define LZST0_WINSZ		(1<<15)
#define LZST0_WINMSK	(LZST0_WINSZ-1)

int btshas_blzst0_hash[4096];
int btshas_blzst0_hchain[LZST0_WINSZ];
byte btshas_blzst0_hwin[LZST0_WINSZ];
int btshas_blzst0_hrov;

int btshas_blzst0_lookup(byte *cs, byte *cse,
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
	lim=272;
//	lim=4096;
	if(d<lim)
		lim=d;

	h=(((cs[0]*4093+cs[1])*4093+cs[2])*4093+cs[3])*4093;
	h=(h>>12)&4095;
	
	c=btshas_blzst0_hash[h]; n=256;
	while((c>=0) && ((n--)>0))
	{
		for(i=0; i<lim; i++)
		{
			if(cs[i]!=btshas_blzst0_hwin[(c+i)&LZST0_WINMSK])
				break;
		}
		
		d=btshas_blzst0_hrov-c;
		if(d<=0)
			d+=LZST0_WINSZ;
		if(d>(LZST0_WINSZ-lim))
			break;
			
		if(i>bl)
			{ bi=d; bl=i; }

		c=btshas_blzst0_hchain[c];
	}
	
	*rbi=bi;
	*rbl=bl;
	return(bl>=4);
}

int btshas_blzst0_update(byte *cs, int len)
{
	int c, h, n;
	
	n=len;
	while((n--)>0)
	{
//		h=(cs[0]*4093+cs[1])*4093;
		h=(((cs[0]*4093+cs[1])*4093+cs[2])*4093+cs[3])*4093;
		h=(h>>12)&4095;
		
		c=btshas_blzst0_hrov;
		btshas_blzst0_hrov=(btshas_blzst0_hrov+1)&LZST0_WINMSK;
		
		btshas_blzst0_hwin[c]=*cs++;
		btshas_blzst0_hchain[c]=btshas_blzst0_hash[h];
		btshas_blzst0_hash[h]=c;
	}
	return(0);
}

int btshas_blzst0_log2up(int v)
{
	int i, j;
	
	i=0; j=v;
	while(j>1)
		{ i++; j=(j+1)>>1; }
	return(i);
}

int btshas_blzst0_clear()
{
	int i, j, k;

	for(i=0; i<4096; i++)
		btshas_blzst0_hash[i]=-1;

	btshas_blzst0_hrov=0;
}


int btshas_blzst0_PackBuffer(byte **rcs, byte **rct, byte *cse, byte *cte)
{
	byte *cs, *ct, *cs0;
	int t0, t1, t2, t3;
	int bd0, bl0, bd1, bl1;
	int bl, bd;
	int sz, h, rb;
	int i, j, k;

	cs=*rcs; ct=*rct;
	cs0=cs; rb=0;
	while((cs<cse) && (ct<cte))
	{
		j=btshas_blzst0_lookup(cs, cse, &bd0, &bl0);
		
		if(j)
		{
			k=btshas_blzst0_lookup(cs+1, cse, &bd1, &bl1);
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

			k=1;
			if(bl>=18)k++;
			if(bd>=128)k++;
			if((ct+rb+2+k)>=cte)
			{
				if(rb>=15)
				{
					*ct++=0xF0;
					*ct++=rb-15;
					memcpy(ct, cs0, rb);
					ct+=rb;
					if(ct<cte)
						*ct++=0x00;
					break;
				}
				*ct++=rb<<4;
				memcpy(ct, cs0, rb);
				ct+=rb;
				if(ct<cte)
					*ct++=0x00;
				break;
			}

			*ct++=((rb>=15)?0xF0:(rb<<4)) |
				((bl>=18)?0x0F:(bl-3));
			if(rb>=15)
				{ *ct++=rb-15; }
			memcpy(ct, cs0, rb);
			ct+=rb;
			
			if(bl>=18)
				*ct++=bl-18;
			if(bd>=0x80)
				{ *ct++=0x80|(bd>>8); }
			*ct++=bd;
			btshas_blzst0_update(cs, bl);
			cs+=bl;
			rb=0;
			cs0=cs;
		}else
		{
			btshas_blzst0_update(cs, 1);
			cs++;
			rb++;

			if(rb>=269)
			{
				*ct++=0xF0;
				*ct++=rb-15;
				memcpy(ct, cs0, rb);
				ct+=rb;
				continue;
			}

			if((ct+rb+2)>=cte)
			{
				if(rb>=15)
				{
					*ct++=0xF0;
					*ct++=rb-15;
					memcpy(ct, cs0, rb);
					ct+=rb;
					if(ct<cte)
						*ct++=0x00;
					break;
				}
				*ct++=rb<<4;
				memcpy(ct, cs0, rb);
				ct+=rb;
				if(ct<cte)
					*ct++=0x00;
				break;
			}
		}
	}
	*rcs=cs; *rct=ct;
}


int btshas_blzst0_CompressBasic(byte *ibuf, byte *obuf, int ibsz, int obsz)
{
	byte *cs, *ct, *ct1, *cse, *cte;
	
	cs=ibuf; cse=cs+ibsz;
	ct=obuf; cte=ct+obsz;
	
	memset(ct, 0, 512);
	ct[0x80]='C';
	ct[0x81]='B';
	ct[0x82]='E';
	ct[0x83]='X';

	/* ImageBase */
	ct[0x84]=ibuf[0x44+20+0x1C];
	ct[0x85]=ibuf[0x44+20+0x1D];
	ct[0x86]=ibuf[0x44+20+0x1E];
	ct[0x87]=ibuf[0x44+20+0x1F];

	/* ImageSize */
	ct[0x88]=ibuf[0x44+20+0x38];
	ct[0x89]=ibuf[0x44+20+0x39];
	ct[0x8A]=ibuf[0x44+20+0x3A];
	ct[0x8B]=ibuf[0x44+20+0x3B];

	/* EntryPoint */
	ct[0x8C]=ibuf[0x44+20+0x10];
	ct[0x8D]=ibuf[0x44+20+0x11];
	ct[0x8E]=ibuf[0x44+20+0x12];
	ct[0x8F]=ibuf[0x44+20+0x13];

	ct+=512;
	
	while(cs<cse)
	{
		memset(ct, 0, 512);
		ct[0]='D';
		ct[1]='C';
		
		ct1=ct+2;
		btshas_blzst0_PackBuffer(&cs, &ct1, cse, ct+512);
		ct+=512;
	}

	return(ct-obuf);
}


byte *btshas_blzst0_DecodeBuffer(byte *ct, byte *cs, byte *cse)
{
    byte *cs1;
    int i, j, k;
    while(cs<cse)
    {
        i=*cs++;
        if(!i)break;
        j=i>>4;
        if(j==15)
            j+=(*cs++);
        while(j--)
            *ct++=*cs++;
        j=i&15;
        if(!j)
            continue;
        if(j==15)
            j+=(*cs++);
        j+=3;
        k=*cs++;
        if(k&0x80)
            k=((k&127)<<8)|(*cs++);
        cs1=ct-k;
        while(j--)
            *ct++=*cs1++;
    }
    return(ct);
} 
