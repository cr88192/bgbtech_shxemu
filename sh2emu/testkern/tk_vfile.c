// byte *tk_ird_imgbuf=NULL;

char kerninit[256];

int tk_ird_init()
{
	u32 reladdr;
	byte *ird;

	if(tk_ird_imgbuf)
		return(0);

	reladdr=P_INITRD_ADDR;
	if(!reladdr)
	{
		printf("tk_ird_init: no initrd\n");
		tk_ird_imgbuf=NULL;
		return(-1);
	}
	
	strcpy(kerninit, P_KINIT_ADDR);
	
#ifdef ARCH_SH4
//	ird=(byte *)(0x90000000+reladdr);
	ird=(byte *)(0x8C000000+reladdr);
#else
	ird=(byte *)(0x10000000+reladdr);
#endif
	tk_ird_imgbuf=ird;
	return(1);
}

TK_FILE *tk_alloc_file()
{
	TK_FILE *tmp;
	
	tmp=malloc(sizeof(TK_FILE));
	memset(tmp, 0, sizeof(TK_FILE));
	return(tmp);
}

int tk_ird_decoctal_n(byte *cs, int n)
{
	int i, v;

	v=0;
	for(i=0; i<n; i++)
	{
		v=(v*8)+(cs[i]-'0');
	}
	return(v);
}

TK_FILE *tk_ird_fopen(char *name, char *mode)
{
	TK_FILE *fd;
	u32 reladdr;
	byte *ird;
	byte *cs;
	int sz;
	
	if(mode[0]!='r')
	{
		printf("tk_ird_fopen: bad mode\n");
		return(NULL);
	}

#if 0	
	reladdr=P_INITRD_ADDR;
	if(!reladdr)
	{
		printf("tk_ird_fopen: no initrd\n");
		return(NULL);
	}
	
	ird=(byte *)(0x10000000+reladdr);
#endif

	ird=tk_ird_imgbuf;
	if(!ird)
	{
		printf("tk_ird_fopen: no initrd\n");
		return(NULL);
	}
	
	cs=ird;
	while(cs[0])
	{
		sz=tk_ird_decoctal_n(cs+124, 11);

		if(!strcmp(cs, name))
		{
			fd=tk_alloc_file();
			fd->ram_base=cs+0x200;
			fd->ram_end=fd->ram_base+sz;
			fd->ram_ofs=fd->ram_base;
			
			printf("tk_ird_fopen: %s %08X %08X\n", name,
				fd->ram_base, fd->ram_end);
			return(fd);
		}
		
		sz=(sz+511)&(~511);
		cs+=sz;
	}
	
	printf("tk_ird_fopen: can't find\n");
	return(NULL);
}

TK_FILE *tk_fopen(char *name, char *mode)
{
	TK_FILE *fd;

	fd=tk_ird_fopen(name, mode);
	if(fd)return(fd);

	return(NULL);
}

int tk_fseek(TK_FILE *fd, int ofs, int rel)
{
	if(fd->ram_base)
	{
		if(rel==0)
		{
			if(ofs<0)
				return(-1);
			if(ofs>(fd->ram_end-fd->ram_base))
				return(-1);
			fd->ram_ofs=fd->ram_base+ofs;
			return(0);
		}

		if(rel==1)
		{
			fd->ram_ofs=fd->ram_ofs+ofs;
			if(fd->ram_ofs>fd->ram_end)
				fd->ram_ofs=fd->ram_end;
			if(fd->ram_ofs<fd->ram_base)
				fd->ram_ofs=fd->ram_base;
			return(0);
		}

		if(rel==2)
		{
			if(ofs>0)
				return(-1);
			if((-ofs)>(fd->ram_end-fd->ram_base))
				return(-1);
			fd->ram_ofs=fd->ram_end+ofs;
			return(0);
		}

		return(-1);
	}

	return(-1);
}

int tk_fread(void *buf, int sz1, int sz2, TK_FILE *fd)
{
	int sz;

	if(fd->ram_base)
	{
		sz=sz1*sz2;
		if((fd->ram_ofs+sz)>fd->ram_end)
			{ sz=fd->ram_end-fd->ram_ofs; }
		if(sz<=0)return(0);
		memcpy(buf, fd->ram_ofs, sz);
		return(sz);
	}

	return(-1);
}

int tk_fgetc(TK_FILE *fd)
{
	int i;

	if(fd->ram_base)
	{
		if(fd->ram_ofs>=fd->ram_end)
			{ fd->ram_ofs=fd->ram_end; return(-1); }

		i=*fd->ram_ofs++;
		return(i);
	}

	return(-1);
}

int tk_ftell(TK_FILE *fd)
{
	int i;
	if(fd->ram_base)
	{
		i=fd->ram_ofs-fd->ram_base;
		return(i);
	}

	return(-1);
}
