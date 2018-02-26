/*
 * Stuff to attempt to load a bootable ELF image into VM image.
 */

typedef struct { byte v[2]; }btsh2_half;
typedef struct { byte v[4]; }btsh2_word;
typedef struct { byte v[4]; }btsh2_addr;
typedef struct { byte v[4]; }btsh2_off;

typedef struct btsh2_elf32_hdr_s btsh2_elf32_hdr;

struct btsh2_elf32_hdr_s
{
  byte			e_ident[16];
  btsh2_half	e_type;		//filt type
  btsh2_half	e_machine;	//target machine
  btsh2_word	e_version;	//file version
  btsh2_addr	e_entry;	/* Entry point */
  btsh2_off		e_phoff;	//program head table's file offset 
  btsh2_off		e_shoff;	//section head table's file offset
  btsh2_word	e_flags;	//
  btsh2_half	e_ehsize;	//elf head's size
  btsh2_half	e_phentsize;//the size of the entry in the program head table
  btsh2_half	e_phnum;	//the number of the entry in the program head table
  btsh2_half	e_shentsize;//the size of the entry in the program head table
  btsh2_half	e_shnum;	//the number of the entry in the section head table
  btsh2_half	e_shstrndx;	//the section head table index of the entry associate with the section name string table
};

struct btsh2_elf32_phdr_s
{
  btsh2_word	p_type;		
  btsh2_off		p_offset;
  btsh2_addr	p_vaddr;
  btsh2_addr	p_paddr;
  btsh2_word	p_filesz;
  btsh2_word	p_memsz;
  btsh2_word	p_flags;
  btsh2_word	p_align;
};

int btsh2_ptrGetSW(byte *ptr, byte en)
{
	if(en)
		return((s16)((ptr[1]<<8)|(ptr[0])));
	return((s16)((ptr[0]<<8)|(ptr[1])));
}

int btsh2_ptrGetUW(byte *ptr, byte en)
{
	if(en)
		return((u16)((ptr[1]<<8)|(ptr[0])));
	return((u16)((ptr[0]<<8)|(ptr[1])));
}

s32 btsh2_ptrGetSD(byte *ptr, byte en)
{
	if(en)
		return((s32)((ptr[3]<<24)|(ptr[2]<<16)|(ptr[1]<<8)|(ptr[0])));
	return((s32)((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3])));
}

u32 btsh2_ptrGetUD(byte *ptr, byte en)
{
	if(en)
		return((u32)((ptr[3]<<24)|(ptr[2]<<16)|(ptr[1]<<8)|(ptr[0])));
	return((u32)((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3])));
}

/** Attempt to load ELF image into VM.
  * ELF given as buffer, with address hint in case of PIC.
  */
int BTESH2_BootLoadElf(
	BTESH2_CpuState *cpu,
	byte *ibuf, int szibuf,
	u32 addrhint)
{
	btsh2_elf32_hdr *ehdr;
	struct btsh2_elf32_phdr_s *phdr;
	u32 entry, phoff, shoff;
	u32 paddr, pmsz, poff;
	byte en;
	int phentsz, phnum;
	int i, j, k;

	if((ibuf[0]!=0x7F) || (ibuf[1]!='E') ||
		(ibuf[2]!='L') || (ibuf[3]!='F'))
	{
		printf("No ELF sig\n");
		return(-1);
	}
	
	ehdr=(btsh2_elf32_hdr *)ibuf;
	en=0;
	if(btsh2_ptrGetUW(ehdr->e_type.v, 1)==2)
		en=1;
	
	if(btsh2_ptrGetUW(ehdr->e_type.v, en)!=2)
	{
		printf("Not Exec %d\n",
			btsh2_ptrGetUW(ehdr->e_type.v, en));
		return(-1);
	}
	
	entry=btsh2_ptrGetUD(ehdr->e_entry.v, en);
	phoff=btsh2_ptrGetUD(ehdr->e_phoff.v, en);
//	shoff=btsh2_ptrGetUD(ehdr->e_eshoff.v, en);
	
	phentsz=btsh2_ptrGetUW(ehdr->e_phentsize.v, en);
	phnum=btsh2_ptrGetUW(ehdr->e_phnum.v, en);

	for(i=0; i<phnum; i++)
	{
		phdr=(struct btsh2_elf32_phdr_s *)(ibuf+(phoff+(i*phentsz)));
		if(btsh2_ptrGetUD(phdr->p_type.v, en)!=1)
		{
//			paddr=btsh2_ptrGetUD(phdr->p_paddr.v);
//			pmsz=btsh2_ptrGetUD(phdr->p_memsz.v);
//			poff=btsh2_ptrGetUD(phdr->p_offset.v);		
//			printf("Non-Load %08X -> %08X %08X\n", poff, paddr, pmsz);

			continue;
		}

		paddr=btsh2_ptrGetUD(phdr->p_paddr.v, en);
		pmsz=btsh2_ptrGetUD(phdr->p_memsz.v, en);
		poff=btsh2_ptrGetUD(phdr->p_offset.v, en);
		
		printf("%08X -> %08X %08X\n", poff, paddr, pmsz);

		if((poff+pmsz)>szibuf)
		{
			printf("BTESH2_BootLoadElf: Copy exeeds image by %d bytes\n",
				(poff+pmsz)-szibuf);
			printf("Truncating: %d -> %d\n", pmsz, szibuf-poff);
			pmsz=szibuf-poff;
		}

		j=BTESH2_MemCpyIn(cpu, paddr&0x1FFFFFFF, ibuf+poff, pmsz);
		
//		j=BTESH2_MemCpyIn(cpu, paddr, ibuf+poff, pmsz);
		if(j<0)
		{
			printf("BTESH2_BootLoadElf: MemCpy Failed\n");
		}
	}

	if(en)cpu->csfl|=1;
	BTESH2_SetRegQWord(cpu, BTESH2_REG_PC, entry);
//	cpu->regs[BTESH2_REG_PC]=entry;
	return(0);
}
