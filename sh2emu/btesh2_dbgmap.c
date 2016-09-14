char *BTESH2_ReadNextLine(char *str, char *stre)
{
	char *s;
	
	s=str;
	while(*s && (s<stre))
	{
		if(*s=='\r')
			break;
		if(*s=='\n')
			break;
		s++;
	}
	if(*s=='\r')s++;
	if(*s=='\n')s++;
	return(s);
}

int BTESH2_BootLoadMap(
	BTESH2_CpuState *cpu,
	byte *ibuf, int szibuf,
	u32 addrhint)
{
//	u32 t_addr[4096];
//	char *t_name[4096];

	u32 *t_addr;
	char **t_name;

	char tb1[256], tb2[256];
	u32 addr;
	char *cs, *cse;
	int n, m;
	int i, j, k;
	
	m=4096;
	t_addr=malloc(m*sizeof(u32));
	t_name=malloc(m*sizeof(char *));
	
	cs=ibuf; cse=cs+szibuf; n=0;
	while((cs<cse) && *cs)
	{
		sscanf(cs, "%08X %s %s", &addr, tb1, tb2);
		cs=BTESH2_ReadNextLine(cs, cse);
		
		if((n+1)>=m)
		{
			m=m+(m>>1);
			t_addr=realloc(t_addr, m*sizeof(u32));
			t_name=realloc(t_name, m*sizeof(char *));
		}
		
		t_addr[n]=addr;
		t_name[n]=strdup(tb2);
		n++;
	}

	cpu->map_addr=t_addr;
	cpu->map_name=t_name;
	cpu->map_nsym=n;
	cpu->map_msym=m;

#if 0	
	cpu->map_addr=malloc(n*sizeof(u32));
	cpu->map_name=malloc(n*sizeof(char *));
	for(i=0; i<n; i++)
	{
		printf("%08X %s\n", t_addr[i], t_name[i]);
		cpu->map_addr[i]=t_addr[i];
		cpu->map_name[i]=t_name[i];
	}
	cpu->map_nsym=n;
	cpu->map_msym=n;
#endif
	return(0);
}

int BTESH2_LookupNameForAddr(
	BTESH2_CpuState *cpu, u32 addr,
	char **rname, u32 *raddr)
{
	int l, h, m, r;
	int i;
	
	if(!cpu->map_name)
		return(-1);
	
	l=0; h=cpu->map_nsym;
	r=h-l; m=l+(r>>1);
	while(r>1)
	{
		if(addr<cpu->map_addr[m])
		{
			h=m;
			r=h-l; m=l+(r>>1);
		}else
		{
			l=m;
			r=h-l; m=l+(r>>1);
		}
	}
	
	*rname=cpu->map_name[m];
	*raddr=cpu->map_addr[m];
	return(1);
}