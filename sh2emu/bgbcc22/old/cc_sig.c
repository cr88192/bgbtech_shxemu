#include <bgbccc.h>
#include <bgbasm.h>
#include <bgbgc.h>

char **bgbcc_sig_name;
char **bgbcc_sig_value;
int bgbcc_sig_hash[4096];

int bgbcc_n_sig, bgbcc_m_sig;

void BGBCC_InitSig()
{
	static int init=0;
	char tb[256];
	VFILE *fd;
	char **a;

	if(init)return;
	init=1;

	bgbcc_sig_name=malloc(4096*sizeof(char *));
	bgbcc_sig_value=malloc(4096*sizeof(char *));
	bgbcc_n_sig=0; bgbcc_m_sig=4096;

	fd=vffopen("cc_usr/sigcache.txt", "rb");
	if(!fd)return;

	while(!vfeof(fd))
	{
		vfgets(tb, 255, fd);
		a=gcrsplit(tb);
		if(!a[0])continue;
		if(a[0][0]==';')continue;
		if(a[0][0]=='#')continue;
		if(a[0][0]=='/')continue;

		BGBCC_BindSig(a[0], a[1]);
	}
	vfclose(fd);
}

void BGBCC_SaveSig()
{
	VFILE *fd;
	int i;


//	printf("BGBCC_SaveSig: A %d\n", bgbcc_n_sig);

	if(!bgbcc_n_sig)return;

#if 1
	fd=vffopen("cc_usr/sigcache.txt", "wt");
	if(!fd)return;

//	printf("BGBCC_SaveSig: B\n");

	for(i=0; i<bgbcc_n_sig; i++)
	{
		vfprint(fd, "%s \"%s\"\n",
			bgbcc_sig_name[i], bgbcc_sig_value[i]);
	}
	vfclose(fd);
#endif
}

int BGBCC_HashSigName(char *str)
{
	char *s;
	int i;

	s=str; i=0;
	while(*s)i=(i*251)+(*s++);
	i=(i>>8)&4095;
	return(i);
}

char *BGBCC_LookupSig(char *name)
{
	int i, hi;

	BGBCC_InitSig();

	hi=BGBCC_HashSigName(name);
	i=bgbcc_sig_hash[hi];
	if((i>=0) && (i<bgbcc_n_sig))
		if(!strcmp(bgbcc_sig_name[i], name))
			return(bgbcc_sig_value[i]);

	for(i=0; i<bgbcc_n_sig; i++)
		if(!strcmp(bgbcc_sig_name[i], name))
			break;
	if(i<bgbcc_n_sig)
	{
		bgbcc_sig_hash[hi]=i;
		return(bgbcc_sig_value[i]);
	}

	return(NULL);
}

int BGBCC_BindSig(char *name, char *value)
{
	int i, hi;

	BGBCC_InitSig();

	if(!name || !value)
		return;

	hi=BGBCC_HashSigName(name);

#if 1
	i=bgbcc_sig_hash[hi];
	if((i>=0) && (i<bgbcc_n_sig))
		if(!strcmp(bgbcc_sig_name[i], name))
	{
		if(strcmp(bgbcc_sig_value[i], value))
		{
			printf("BGBCC_BindSig: Redefined %s as "
				"different type: %s->%s\n",
				name, bgbcc_sig_value[i], value);
			bgbcc_sig_value[i]=basm_strdup(value);
			return(i);
		}
		return(i);
	}
#endif

	for(i=0; i<bgbcc_n_sig; i++)
		if(!strcmp(bgbcc_sig_name[i], name))
			break;
	if(i<bgbcc_n_sig)
	{
		if(!strcmp(bgbcc_sig_value[i], value))
		{
			bgbcc_sig_hash[hi]=i;
			return(i);
		}

		printf("BGBCC_BindSig: Redefined %s as "
			"different type: %s->%s\n",
			name, bgbcc_sig_value[i], value);
		bgbcc_sig_value[i]=basm_strdup(value);
		bgbcc_sig_hash[hi]=i;
		return(i);
	}

	if(bgbcc_n_sig>=bgbcc_m_sig)
	{
		i=bgbcc_m_sig+(bgbcc_m_sig>>1);
		bgbcc_sig_name=realloc(bgbcc_sig_name, i*sizeof(char *));
		bgbcc_sig_value=realloc(bgbcc_sig_value, i*sizeof(char *));
		bgbcc_m_sig=i;
	}

	i=bgbcc_n_sig++;
	bgbcc_sig_name[i]=basm_strdup(name);
	bgbcc_sig_value[i]=basm_strdup(value);
	bgbcc_sig_hash[hi]=i;
	return(i);
}


dyt BGBCC_GetVarSig(void *p, char *sig)
{
	if(!strcmp(sig, "a"))return(dyint(*(signed char *)p));
	if(!strcmp(sig, "b"))return(dyint(*(char *)p));
	if(!strcmp(sig, "c"))return(dyint(*(char *)p));
	if(!strcmp(sig, "d"))return(dydouble(*(double *)p));
	if(!strcmp(sig, "e"))return(dydouble(*(long double *)p));
	if(!strcmp(sig, "f"))return(dyfloat(*(float *)p));
	//g=float128
	if(!strcmp(sig, "h"))return(dyint(*(unsigned char *)p));
	if(!strcmp(sig, "i"))return(dyint(*(int *)p));
	if(!strcmp(sig, "j"))return(dyint(*(unsigned int *)p));
	//k=float16
	if(!strcmp(sig, "l"))return(dylong(*(long *)p));
	if(!strcmp(sig, "m"))return(dylong(*(unsigned long *)p));
	//n=int128, o=uint128
	//p,q=reserved
	if(!strcmp(sig, "r"))return(*(dyt *)p);
	if(!strcmp(sig, "s"))return(dyint(*(short *)p));
	if(!strcmp(sig, "t"))return(dyint(*(unsigned short *)p));
	//u=bad
	if(!strcmp(sig, "v"))return(NULL);
	if(!strcmp(sig, "w"))return(dyint(*(short *)p));
	if(!strcmp(sig, "x"))return(dylong(*(long long *)p));
	if(!strcmp(sig, "y"))return(dylong(*(unsigned long long *)p));

	if(!strcmp(sig, "Cd"))return(dydcomplex(*(double _Complex *)p));
	if(!strcmp(sig, "Cf"))return(dyfcomplex(*(float _Complex *)p));

	if(*sig=='P')return((dyt)(*(void **)p));

	return(NULL);
}

void BGBCC_SetVarSig(void *var, char *sig, dyt val)
{
	if(!strcmp(sig, "a"))*(signed char *)var=dyintv(val);
	if(!strcmp(sig, "b"))*(signed char *)var=dyintv(val);
	if(!strcmp(sig, "c"))*(signed char *)var=dyintv(val);
	if(!strcmp(sig, "d"))*(double *)var=dydoublev(val);
	if(!strcmp(sig, "e"))*(long double *)var=dydoublev(val);
	if(!strcmp(sig, "f"))*(float *)var=dyfloatv(val);
	//g=float128
	if(!strcmp(sig, "h"))*(unsigned char *)var=dyintv(val);
	if(!strcmp(sig, "i"))*(int *)var=dyintv(val);
	if(!strcmp(sig, "j"))*(unsigned int *)var=dyintv(val);
	//k=float16
	if(!strcmp(sig, "l"))*(long *)var=dylongv(val);
	if(!strcmp(sig, "m"))*(unsigned long *)var=dylongv(val);
	//n=int128, o=uint128
	//p,q=reserved
	if(!strcmp(sig, "r"))*(dyt *)var=val;	
	if(!strcmp(sig, "s"))*(short *)var=dyintv(val);
	if(!strcmp(sig, "t"))*(unsigned short *)var=dyintv(val);
	//u=bad
	if(!strcmp(sig, "w"))*(short *)var=dyintv(val);
	if(!strcmp(sig, "x"))*(long long *)var=dylongv(val);
	if(!strcmp(sig, "y"))*(unsigned long long *)var=dylongv(val);

	if(!strcmp(sig, "Cd"))*(double _Complex *)var=dycomplexv(val);
	if(!strcmp(sig, "Cf"))*(float _Complex *)var=dycomplexv(val);

	if(*sig=='P')*(void **)var=(void *)val;

	return;
}

