char *btshas_ralloc_buf=NULL;
int btshas_ralloc_pos;

static char *btshas_mm_strtab=NULL;
static char *btshas_mm_strtabe=NULL;
static char *btshas_mm_estrtab=NULL;
static char **btshas_mm_strhash;


void *btshas_mm_malloc(int sz)
{
	void *p;
	p=malloc(sz);
	memset(p, 0, sz);
	return(p);
}

void *btshas_mm_realloc(void *ptr, int sz)
{
	return(realloc(ptr, sz));
}

void btshas_mm_free(void *ptr)
{
	free(ptr);
}

char *btshas_ralloc(int sz)
{
	char *ptr;

	if(!btshas_ralloc_buf)
	{
		btshas_ralloc_buf=malloc(1<<16);
		btshas_ralloc_pos=0;
	}
	
	if((btshas_ralloc_pos+sz)>(1<<16))
		btshas_ralloc_pos=0;

	ptr=btshas_ralloc_buf+btshas_ralloc_pos;
	btshas_ralloc_pos=(btshas_ralloc_pos+sz+7)&(~7);
	return(ptr);
}

char *btshas_rstrdup(char *str)
{
	char *s1;
	int l;
	
	l=strlen(str);
	s1=btshas_ralloc(l+1);
	memcpy(s1, str, l+1);
	return(s1);
}

s64 btshas_strtol(char *str, int rdx)
{
	char *s;
	s64 t;
	int i, j;
	
	s=str; t=0;
	while(*s)
	{
		if(*s=='_')
			{ s++; continue; }

		i=*s++; j=-1;
		if((i>='0') && (i<='9'))
			j=0+(i-'0');
		if((i>='A') && (i<='Z'))
			j=10+(i-'A');
		if((i>='a') && (i<='z'))
			j=10+(i-'a');
		if((j<0) || (j>=rdx))
			break;
		t=t*rdx+j;
	}
	return(t);
}

s64 btshas_atol(char *str)
{
	s64 li;
	char *s;
	int sg;
	
	if(*str=='0')
	{
		if(str[1]=='x')
			return(btshas_strtol(str+2, 16));
		if(str[1]=='d')
			return(btshas_strtol(str+2, 10));
		if(str[1]=='b')
			return(btshas_strtol(str+2, 2));
		return(btshas_strtol(str+1, 8));
	}
	
	s=str; sg=0;
	if(*s=='-')
		{ s++; sg=1; }
	li=btshas_strtol(s, 10);
	if(sg)li=-li;
	return(li);
}

char *btshas_strdup(char *str)
{
	char *s, *t;
	int i;

	if(!str)
	{
//		*(int *)-1=-1;
		return(NULL);
	}
	if(!*str)return("");

	if(!btshas_mm_strhash)
	{
		btshas_mm_strtab=btshas_mm_malloc(1<<20);
		btshas_mm_strtabe=btshas_mm_strtab;
		btshas_mm_estrtab=btshas_mm_strtab+(1<<20);

		i=16384*sizeof(char *);
		btshas_mm_strhash=btshas_mm_malloc(i);
		memset(btshas_mm_strhash, 0, i);
	}
	
	if((btshas_mm_strtabe+(strlen(str)+2))>=btshas_mm_estrtab)
	{
		btshas_mm_strtab=btshas_mm_malloc(1<<20);
		btshas_mm_strtabe=btshas_mm_strtab;
		btshas_mm_estrtab=btshas_mm_strtab+(1<<20);
	}

	i=0; s=str;
	while(*s)i=i*251+(*s++);
//	i=((i*251)>>8)&0xFFF;
	i=((i*251)>>8)&0x3FFF;

	t=btshas_mm_strhash[i];
	while(t)
	{
		s=(char *)(((char **)t)+1);
		t=*(char **)t;
		if(!strcmp(s, str))return(s);
	}

	t=btshas_mm_strtabe;
	btshas_mm_strtabe=t+strlen(str)+1+sizeof(char *);

	s=(char *)(((char **)t)+1);
	strcpy(s, str);

	*(char **)t=btshas_mm_strhash[i];
	btshas_mm_strhash[i]=t;
	return(s);
}

int btshasp_stricmp(char *s1, char *s2)
{
	int c1, c2;

	c1=*s1; c2=*s2;
	while(*s1 && *s2)
	{
		c1=*s1++; c2=*s2++;
		if((c1>='a') && (c1<='z'))
			c1='A'+(c1-'a');
		if((c2>='a') && (c2<='z'))
			c2='A'+(c2-'a');
		if(c1!=c2)
			break;
	}
	if(c1>c2)return( 1);
	if(c2>c1)return(-1);
	return(0);
}

int btshasp_strntoupper(char *dst, char *src, int len)
{
	char *s, *t, *te;
	int i;
	
	s=src; t=dst; te=dst+len;
	while(*s && (t<te))
	{
		i=*s++;
		if((i>='a') && (i<='z'))
			i='A'+(i-'a');
		*t++=i;
	}
	*t++=0;
	return(0);
}

int btshasp_strtoupper(char *dst, char *src)
{
	return(btshasp_strntoupper(dst, src, 1<<16));
}

char *BTSHAS_BufGetLine(char *tbuf, char *str, char *stre)
{
	char *s, *t;
	
	s=str; t=tbuf;
	while(*s && (s<stre))
	{
		if(*s=='\r')
			break;
		if(*s=='\n')
			break;
		*t++=*s++;
	}
	*t=0;
	
	if(*s=='\r')s++;
	if(*s=='\n')s++;
	return(s);
}

char **BTSHAS_SplitLine(char *buf)
{
	static char *ta[64];
	static char tb[512];
	char **a;
	char *s, *t, *t0;
	
	a=ta; s=buf; t=tb;
	while(*s)
	{
		if(*s<=' ')
		{
			while(*s && *s<=' ')
				s++;
			continue;
		}
		
		if(*s=='\"')
		{
			s++;
			t0=t;
			while(*s && (*s!='\"'))
			{
				if(*s=='\\')
				{
					s++;
					switch(*s)
					{
					case 'b': s++; *t++='\b'; break;
//					case 'e': s++; *t++='\e'; break;
					case 'n': s++; *t++='\n'; break;
					case 'r': s++; *t++='\r'; break;
					case 't': s++; *t++='\t'; break;
					case '\\': s++; *t++='\\'; break;
					default:
						*t++=*s++;
						break;
					}
					continue;
				}
				*t++=*s++;
			}
			*t++=0;
			*a++=t0;
			continue;
		}
		
		t0=t;
		while(*s && (*s>' '))
			{ *t++=*s++; }
		*t++=0;
		*a++=t0;
	}
	*a++=NULL;
	return(ta);
}

int BTSHAS_PrintHex(byte *buf, int sz, u32 offs)
{
	int i, j, k, nl;
	
	nl=(sz+15)>>4;
	for(i=0; i<nl; i++)
	{
		printf("%06X  ", offs+i*16);
		for(j=0; j<16; j++)
		{
//			if(j && !(j&7))
//				printf(" ");
			if(j && !(j&3))
				printf(" ");
//			if(j && !(j&1))
//				printf(" ");
			k=i*16+j;
			if(k<sz)
				{ printf("%02X ", buf[k]); }
			else
				{ printf("   "); }
		}

		printf("  ");

		for(j=0; j<16; j++)
		{
			k=i*16+j;
			k=(k<sz)?buf[k]:' ';
			if(k<' ')k='.';
			if(k>'~')k='.';
			fputc(k, stdout);
		}
		fputc('\n', stdout);
	}
}

byte *BTSHAS_LoadFile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	if(!path)
		return(NULL);
	
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

int BTSHAS_StoreFile(char *path, byte *buf, int sz)
{
	FILE *fd;
	
	if(!path)
		return(-1);
	
	fd=fopen(path, "wb");
	if(!fd)
		return(-1);
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

char *btshas_getext(char *fn)
{
	char *s;
	s=fn+strlen(fn);
	while((s>fn) && s[-1]!='.')s--;
	return(s);
}

