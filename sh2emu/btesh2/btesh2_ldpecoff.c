/** Attempt to load PE/COFF image into VM.
  * Image is given as buffer, with optional address hint.
  */
int BTESH2_BootLoadPeCoff(
	BTESH2_CpuState *cpu,
	byte *ibuf, int szibuf,
	u32 addrhint)
{
	byte tbn[9];
	byte *cs_hdr, *cs_opt, *cs_sec, *cs_strs, *cs_reloc;
	byte *cs, *cse, *cs1, *cs1e, *cs2;
	char *sn;
	u32 msva, mssz, msfsz, msofs, msflag;
	u32 it_ilt, it_iat, it_dll;
	u32 rva_reloc, rsz_reloc;
	u32 rva_exptab, rsz_exptab;
	u32 rva_imptab, rsz_imptab;
	byte is64;
	int ofs_pe;
	int mach, nsec, szopt, mflag;
	int imgbase, imgbase2, imgsize, rva_strt, entry;
	int adj_rebase;
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

	cpu->subarch=BTESH2_SUBARCH_BJX1_32;

	is64=0;
	k=btsh2_ptrGetUD(cs_hdr+0x18, 1);
	if(k==0x020B)
	{
		cpu->subarch=BTESH2_SUBARCH_BJX1_64;
		is64=1;
	}
	if(mach==0xB64C)
	{
		cpu->subarch=BTESH2_SUBARCH_BJX1_64C;
		is64=2;
	}

	if(is64)
		{ imgbase=btsh2_ptrGetUD(cs_hdr+0x30, 1); }
	else
		{ imgbase=btsh2_ptrGetUD(cs_hdr+0x34, 1); }
	rva_strt=btsh2_ptrGetUD(cs_hdr+0x28, 1);
	entry=imgbase+rva_strt;
	
	imgsize=btsh2_ptrGetUD(cs_hdr+0x50, 1);

	if(is64)
	{
		rva_exptab=btsh2_ptrGetUD(cs_hdr+0x88, 1);
		rsz_exptab=btsh2_ptrGetUD(cs_hdr+0x8C, 1);
		rva_imptab=btsh2_ptrGetUD(cs_hdr+0x90, 1);
		rsz_imptab=btsh2_ptrGetUD(cs_hdr+0x94, 1);

		rva_reloc=btsh2_ptrGetUD(cs_hdr+0xB0, 1);
		rsz_reloc=btsh2_ptrGetUD(cs_hdr+0xB4, 1);
	}else
	{
		rva_exptab=btsh2_ptrGetUD(cs_hdr+0x78, 1);
		rsz_exptab=btsh2_ptrGetUD(cs_hdr+0x7C, 1);
		rva_imptab=btsh2_ptrGetUD(cs_hdr+0x80, 1);
		rsz_imptab=btsh2_ptrGetUD(cs_hdr+0x84, 1);

		rva_reloc=btsh2_ptrGetUD(cs_hdr+0xA0, 1);
		rsz_reloc=btsh2_ptrGetUD(cs_hdr+0xA4, 1);
	}
	
	imgbase2=imgbase;
	adj_rebase=0; cs_reloc=NULL;
	if(addrhint && rva_reloc)
	{
		adj_rebase=addrhint-imgbase;
		cs_reloc=ibuf+rva_reloc;
		imgbase2=addrhint;
	}
	
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
		
		msva+=adj_rebase;
		
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
	
	if(adj_rebase)
	{
		cs=cs_reloc; cse=cs+rsz_reloc;
		while(cs<cse)
		{
			msva=btsh2_ptrGetUD(cs+0x00, 1);
			mssz=btsh2_ptrGetUD(cs+0x04, 1);

			if(!mssz)
				break;
			
			if(mssz<=0)
			{
				printf("PE Bad Base Relocation Block Size\n");
				break;
			}

			cs2=cs+mssz;
			if((cs2<cs)|| (cs2>cse))
			{
				printf("PE Bad/Truncated Base Relocation Block\n");
				break;
			}

			cs1=cs+8; cs1e=cs2;
			while(cs1<cs1e)
			{
				i=btsh2_ptrGetUW(cs1, 1);
				cs1+=2;
				
				if(!(i&0xF000))
					continue;
				
				if((i&0xF000)==0x3000)
				{
					j=msva+(i&0x0FFF);
					k=BTESH2_GetAddrDWordPhy(cpu, imgbase2+j);
					k+=adj_rebase;
					BTESH2_SetAddrDWordPhy2(cpu, imgbase2+j, k);
					continue;
				}

				printf("PE Unhandled Base Reloc Type %d\n", (i>>12)&15);
			}
			
			cs=cs2;
		}
	}
	
	if(rva_imptab)
	{
		cs=ibuf+rva_imptab; cse=cs+rsz_imptab;
		while(cs<cse)
		{
			it_ilt=btsh2_ptrGetUD(cs+0x00, 1);
			it_dll=btsh2_ptrGetUD(cs+0x0C, 1);
			it_iat=btsh2_ptrGetUD(cs+0x10, 1);
			if(!it_ilt)
				break;
				
			sn=((char *)ibuf)+it_dll;
		}
	}

	cpu->csfl|=1;
	if(is64)
	{
		BTESH2_SetRegQWord(cpu, BTESH2_REG_SR,
			BTESH2_GetRegQWord(cpu, BTESH2_REG_SR)|
				BTESH2_SRFL_JQ |
				(((u64)BTESH2_SRFL_JQ)<<32));

//		cpu->regs[BTESH2_REG_SR+BTESH2_REG_RLO]|=BTESH2_SRFL_JQ;
//		cpu->regs[BTESH2_REG_SR+BTESH2_REG_RHI]|=BTESH2_SRFL_JQ;
		cpu->csfl|=BTESH2_CSFL_SRJQ;
	}
//	cpu->regs[BTESH2_REG_PC]=entry;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, entry);
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
