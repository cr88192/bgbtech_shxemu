int strcmp(char *s1, char *s2)
{
	while(*s1 && *s2 && (*s1==*s2))
		{ s1++; s2++; }
	if(*s1>*s2)return(1);
//	if(*s2>*s1)return(-1);
	if(*s1<*s2)return(-1);
	return(0);
}

void *memset(void *dest, int val, size_t size)
{
	byte *ct, *cte;
	int i;
	
	ct=dest; cte=ct+size;

	i=(int)ct;
	switch(i&3)
	{
	case 1: *ct++=val;
	case 2: *ct++=val;
	case 3: *ct++=val;
	case 0: break;
	default: break;
	}
	
	i=val|(val<<8); i=i|(i<<16);
	while((ct+8)<=cte)
		{ ((u32 *)ct)[0]=i; ((u32 *)ct)[1]=i; ct+=8; }
	if((ct+4)<=cte)
		{ (*(u32 *)ct)=i; ct+=4; }
	while(ct<cte)
		{ *ct++=val; }
	
	return(dest);
}

void *memcpy(void *dest, const void *src, size_t size)
{
	byte *ct, *cs, *cte;
	int i, j;
	
	ct=dest; cte=ct+size;
	cs=(byte *)src;

	i=(int)ct;
	j=(int)cs;

	if((i&3)==(j&3))
	{
		switch(i&3)
		{
		case 1: *ct++=*cs++;
		case 2: *ct++=*cs++;
		case 3: *ct++=*cs++;
		case 0: break;
		default: break;
		}
		
		while((ct+16)<=cte)
		{
			((u32 *)ct)[0]=((u32 *)cs)[0];
			((u32 *)ct)[1]=((u32 *)cs)[1];
			((u32 *)ct)[2]=((u32 *)cs)[2];
			((u32 *)ct)[3]=((u32 *)cs)[3];
			ct+=16; cs+=16;
		}
		while((ct+8)<=cte)
		{
			((u32 *)ct)[0]=((u32 *)cs)[0];
			((u32 *)ct)[1]=((u32 *)cs)[1];
			ct+=8; cs+=8;
		}
		if((ct+4)<=cte)
			{ (*(u32 *)ct)=(*(u32 *)cs); ct+=4; cs+=4; }
		while(ct<cte)
			{ *ct++=*cs++; }
		return(dest);
	}

	while((ct+8)<=cte)
	{
		ct[0]=cs[0];	ct[1]=cs[1];
		ct[2]=cs[2];	ct[3]=cs[3];
		ct[4]=cs[4];	ct[5]=cs[5];
		ct[6]=cs[6];	ct[7]=cs[7];
		ct+=8; cs+=8;
	}
	if((ct+4)<=cte)
	{
		ct[0]=cs[0];	ct[1]=cs[1];
		ct[2]=cs[2];	ct[3]=cs[3];
		ct+=4; cs+=4;
	}
	while(ct<cte)
		{ *ct++=*cs++; }
	return(dest);
}

int memcmp(byte *s1, byte *s2, size_t size)
{
	int n;
	
	n=size;
	while((*s1==*s2) && (n>0))
		{ s1++; s2++; }
	if(n<=0)
		return(0);
	if(*s1>*s2)return(1);
//	if(*s2>*s1)return(-1);
	if(*s1<*s2)return(-1);
	return(0);
}

char *strcpy(char *dest, const char *src)
{
	char *cs, *ct;
	ct=dest; cs=(char *)src;
	while(*cs)
		{ *ct++=*cs++; }
	*ct++=*cs++;
	return(dest);
}


u64 rand_seed=0x6969696969696969ULL;

int rand(void)
{
	rand_seed=rand_seed*281474976710597ULL+1;
	return(rand_seed>>48);
}

void srand(unsigned int seed)
{
	rand_seed=seed;
	rand_seed=rand_seed*281474976710597ULL+1;
	rand_seed=rand_seed*281474976710597ULL+1;
	rand_seed=rand_seed*281474976710597ULL+1;
}

int rand_r(unsigned int *seedp)
{
	unsigned int s;
	s=(*seedp)*65521;
	*seedp=s;
	return(s>>16);
}
