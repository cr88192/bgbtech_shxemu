/** Attempt to load PE/COFF image into VM.
  * Image is given as buffer, with optional address hint.
  */
int BTESH2_BootLoadPeCoff(
	BTESH2_CpuState *cpu,
	byte *ibuf, int szibuf,
	u32 addrhint)
{
	byte tbn[9];
	byte *cs_hdr, *cs_opt, *cs_sec, *cs_strs;
	byte *cs;
	char *sn;
	u32 msva, mssz, msfsz, msofs, msflag;
	int ofs_pe;
	int mach, nsec, szopt, mflag;
	int imgbase, rva_strt, entry;
	int i, j, k;

	if((ibuf[0]!='M') || (ibuf[1]!='Z'))
	{
		printf("No MZ sig\n");
		return(-1);
	}

	ofs_pe=btsh2_ptrGetSD(ibuf+0x3C, 1);
	if((ofs_pe<0) || (ofs_pe>=szibuf))
	{
		printf("PE Header Address Invalid\n");
		return(-1);
	}

	if(	(ibuf[ofs_pe+0]!='P')	||	(ibuf[ofs_pe+1]!='E') ||
		(ibuf[ofs_pe+2]!=0)		||	(ibuf[ofs_pe+3]!=0))
	{
		printf("No PE sig\n");
		return(-1);
	}
	
	cs_hdr=ibuf+ofs_pe;
	mach=btsh2_ptrGetUW(cs_hdr+0x04, 1);
	nsec=btsh2_ptrGetUW(cs_hdr+0x06, 1);
	szopt=btsh2_ptrGetUW(cs_hdr+0x14, 1);
	mflag=btsh2_ptrGetUW(cs_hdr+0x16, 1);
	cs_opt=cs_hdr+0x18;
	cs_sec=cs_opt+szopt;
	cs_strs=cs_sec+(nsec*0x28);

	imgbase=btsh2_ptrGetUD(cs_hdr+0x34, 1);
	rva_strt=btsh2_ptrGetUD(cs_hdr+0x28, 1);
	entry=imgbase+rva_strt;
	
	cs=cs_sec;
	for(i=0; i<nsec; i++)
	{
		if(cs[0])
		{
			strncpy(tbn, cs, 8);
			tbn[8]=0;
			sn=tbn;
		}else
		{
			j=btsh2_ptrGetUD(cs+0x04, 1);
			sn=cs_strs+j;
		}
		
		mssz=btsh2_ptrGetUD(cs+0x08, 1);
		msva=btsh2_ptrGetUD(cs+0x0C, 1);
		msfsz=btsh2_ptrGetUD(cs+0x10, 1);
		msofs=btsh2_ptrGetUD(cs+0x14, 1);
		msflag=btsh2_ptrGetUD(cs+0x24, 1);
		
		printf("%-.8s\t%08X %08X %08X %08X %08X\n",
			sn, mssz, msva, msfsz, msofs, msflag);

		if((msofs+msfsz)>szibuf)
		{
			msfsz=szibuf-msofs;
		}
		
		if(msfsz>0)
		{
			j=BTESH2_MemCpyIn(cpu, msva&0x1FFFFFFF, ibuf+msofs, msfsz);
		}
		
		cs+=40;
	}

	cpu->csfl|=1;
	cpu->regs[BTESH2_REG_PC]=entry;
	return(0);
}

int BTESH2_BootLoadImage(
	BTESH2_CpuState *cpu,
	byte *ibuf, int szibuf,
	u32 addrhint)
{
	if((ibuf[0]=='M') && (ibuf[1]=='Z'))
	{
		return(BTESH2_BootLoadPeCoff(cpu, ibuf, szibuf, addrhint));
	}

	if(	(ibuf[0]==0x7F) && (ibuf[1]=='E') &&
		(ibuf[2]=='L') && (ibuf[3]=='F'))
	{
		return(BTESH2_BootLoadElf(cpu, ibuf, szibuf, addrhint));
	}
	
	return(-1);
}
