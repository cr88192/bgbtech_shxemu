void btesh2_tkfat_setChs(byte *chs, int lba)
{
	int c, h, s, ts;
	
	s=lba&63;
	h=(lba>>6)&255;
	c=lba>>10;
	ts=(c<<6)|s;
	
	chs[0]=h;
	chs[1]=ts;
	chs[2]=ts>>8;
}

void btesh2_tkfat_setWord(byte *ptr, u16 val)
	{	ptr[0]=val;	ptr[1]=val>>8;	}
void btesh2_tkfat_setDWord(byte *ptr, u32 val)
	{	ptr[0]=val;	ptr[1]=val>>8;	ptr[2]=val>>16;	ptr[3]=val>>24;	}

u16 btesh2_tkfat_getWord(byte *ptr)
	{	return(ptr[0]|(ptr[1]<<8));	}
u32 btesh2_tkfat_getDWord(byte *ptr)
	{	return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24));	}

int BTESH2_TKFAT_ReadSectors(BTESH2_TKFAT_ImageInfo *img,
	byte *buf, s64 lba, int num)
{
	fseek(img->fdImgData, (int)(lba<<9), 0);
	fread(buf, 1, 512*num, img->fdImgData);
	return(0);
}

int BTESH2_TKFAT_WriteSectors(BTESH2_TKFAT_ImageInfo *img,
	byte *buf, s64 lba, int num)
{
	fseek(img->fdImgData, (int)(lba<<9), 0);
	fwrite(buf, 1, 512*num, img->fdImgData);
	return(0);
}

/** Get sectors as a temporary buffer.
  * Given LBA and a number of sectors (max=128).
  * If DIRTY flag is set, mark the sector as dirty.
  * The buffer will be released implicitly following the call.
  */
byte *BTESH2_TKFAT_GetSectorTempBuffer(BTESH2_TKFAT_ImageInfo *img,
	s64 lba, int num)
{
	u32 tba;
	void *tbd;
	int n, tbn;
	int i, j, k;

//	if(img->pImgData)
//		return(img->pImgData+(lba<<9));

	n=num&255;
	for(i=0; i<img->tbc_num; i++)
	{
		if((img->tbc_lba[i]==lba) &&
			((img->tbc_lbn[i]&255)==n))
		{
			j=(i*7)>>3;
			tbd=img->tbc_buf[i];
			tba=img->tbc_lba[i];
			tbn=img->tbc_lbn[i];
			img->tbc_buf[i]=img->tbc_buf[j];
			img->tbc_lba[i]=img->tbc_lba[j];
			img->tbc_lbn[i]=img->tbc_lbn[j];
			img->tbc_buf[i]=tbd;
			img->tbc_lba[i]=tba;
			img->tbc_lbn[i]=tbn;
			return(tbd);
		}
	}

	if(img->tbc_num<256)
	{
		i=img->tbc_num++;
		tbd=malloc(n*512);
		img->tbc_buf[i]=tbd;
		img->tbc_lba[i]=lba;
		img->tbc_lbn[i]=num;
	}else
	{
		i=255;
		
		if(img->tbc_lbn[i]&TKFAT_SFL_DIRTY)
		{
			BTESH2_TKFAT_WriteSectors(img,
				img->tbc_buf[i],
				img->tbc_lba[i],
				img->tbc_lbn[i]&255);
		}
		
		if(n!=(img->tbc_lbn[i]&255))
		{
			free(img->tbc_buf[i]);
			img->tbc_buf[i]=malloc(n*512);
		}
		img->tbc_lba[i]=lba;
		img->tbc_lbn[i]=num;
		tbd=img->tbc_buf[i];
	}

	BTESH2_TKFAT_ReadSectors(img, tbd, lba, n);
	return(tbd);
}

/** Get sectors as a static buffer.
  * Given LBA and a number of sectors (max=128).
  * If DIRTY flag is set, mark the sector as dirty.
  * Static buffers will need to be released explicitly.
  */
byte *BTESH2_TKFAT_GetSectorStaticBuffer(BTESH2_TKFAT_ImageInfo *img,
	s64 lba, int num)
{
	int i;

//	if(img->pImgData)
//		return(img->pImgData+(lba<<9));

	for(i=0; i<img->sbc_num; i++)
		if(img->sbc_lba[i]==lba)
	{
		if(num&TKFAT_SFL_DIRTY)
			img->sbc_lbn[i]|=TKFAT_SFL_DIRTY;
		return(img->sbc_buf[i]);
	}

	i=img->sbc_num++;
	img->sbc_buf[i]=malloc((num&255)*512);
	img->sbc_lba[i]=lba;
	img->sbc_lbn[i]=num;

	BTESH2_TKFAT_ReadSectors(img, img->sbc_buf[i], lba, num&255);
//	fseek(img->fdImgData, lba<<9, 0);
//	fread(tbd, 1, 512*n, img->fdImgData);
	return(img->sbc_buf[i]);
//	return(img->pImgData+(lba<<9));
}

void BTESH2_TKFAT_FlushBuffers(BTESH2_TKFAT_ImageInfo *img)
{
	int i;
	
	for(i=0; i<img->sbc_num; i++)
	{
		if(img->sbc_lbn[i]&TKFAT_SFL_DIRTY)
		{
			BTESH2_TKFAT_WriteSectors(img,
				img->sbc_buf[i],
				img->sbc_lba[i],
				img->sbc_lbn[i]&255);
		}
		
		free(img->sbc_buf[i]);
		img->sbc_buf[i]=NULL;
	}

	for(i=0; i<img->tbc_num; i++)
	{
		if(img->tbc_lbn[i]&TKFAT_SFL_DIRTY)
		{
			BTESH2_TKFAT_WriteSectors(img,
				img->tbc_buf[i],
				img->tbc_lba[i],
				img->tbc_lbn[i]&255);
		}

		free(img->tbc_buf[i]);
		img->tbc_buf[i]=NULL;
	}
}

/** Partition image with a simple MBR holding a FAT volume.
  */
void BTESH2_TKFAT_SetupImageMBR(BTESH2_TKFAT_ImageInfo *img)
{
	int fsty;
	
	if(!img->fsty)
	{
		fsty=0x06;
//		if(img->nImgBlks>32768)
//		if(img->nImgBlks>65536)
//		if(img->nImgBlks>131072)
		if(img->nImgBlks>130048)
		{
			fsty=0x0C;
		}
	}else
	{
		fsty=img->fsty;
	}
	
	img->fsty=fsty;
	img->isfat16=(fsty==0x06);
	img->lba_start=8;
	img->lba_count=img->nImgBlks-8;
	
//	img->mbr=(BTESH2_TKFAT_MBR *)img->pImgData;
	img->mbr=(BTESH2_TKFAT_MBR *)
		BTESH2_TKFAT_GetSectorStaticBuffer(
			img, 0, 1|TKFAT_SFL_DIRTY);
	memset(img->mbr, 0, 512);
	img->mbr->entry[0].flag=0x00;
	img->mbr->entry[0].fstype=fsty;
	
	btesh2_tkfat_setChs(&(img->mbr->entry[0].shead), img->lba_start);
	btesh2_tkfat_setChs(&(img->mbr->entry[0].ehead),
		img->lba_start+img->lba_count);

	btesh2_tkfat_setDWord(img->mbr->entry[0].lba_start, img->lba_start);
	btesh2_tkfat_setDWord(img->mbr->entry[0].lba_count, img->lba_count);

	img->mbr->aa55[0]=0x55;
	img->mbr->aa55[1]=0xAA;
}

char *btesh2_tkfat_fstnameforfsty(int fsty)
{
	char *s0;
	
	switch(fsty)
	{
	case 0x00: s0="(none)"; break;
	case 0x06: s0="FAT16"; break;
	case 0x0B: s0="FAT32"; break;
	case 0x0C: s0="FAT32"; break;
	default: s0="?"; break;
	}
	return(s0);
}

void BTESH2_TKFAT_ReadImageMBR(BTESH2_TKFAT_ImageInfo *img)
{
	char *s0;
	int lba, lbn, fsty;
	int i;

	printf("TKFAT_ReadImageMBR\n");

	img->mbr=(BTESH2_TKFAT_MBR *)
		BTESH2_TKFAT_GetSectorStaticBuffer(
			img, 0, 1);

	for(i=0; i<4; i++)
	{
		fsty=img->mbr->entry[i].fstype;
		lba=btesh2_tkfat_getDWord(img->mbr->entry[i].lba_start);
		lbn=btesh2_tkfat_getDWord(img->mbr->entry[i].lba_count);
		
		s0=btesh2_tkfat_fstnameforfsty(fsty);
		printf("  %08X %08X %02X %s\n", lba, lbn, fsty, s0);
	}

	img->fsty=img->mbr->entry[0].fstype;
	img->lba_start=btesh2_tkfat_getDWord(img->mbr->entry[0].lba_start);
	img->lba_count=btesh2_tkfat_getDWord(img->mbr->entry[0].lba_count);

	img->isfat16=(img->fsty==0x06);

	s0=btesh2_tkfat_fstnameforfsty(img->fsty);
	printf("TKFAT_ReadImageMBR: %08X %08X %02X %s\n",
		img->lba_start, img->lba_count, img->fsty, s0);
}

int BTESH2_TKFAT_GetFatEntry(BTESH2_TKFAT_ImageInfo *img, int clid)
{
	byte *ofs;
	int lba;
	int i;

	if(img->isfat16)
	{
//		ofs=BTESH2_TKFAT_GetSectorStaticBuffer(img, 0, 1);
//		ofs=img->pImgData+(img->lba_fat1*512)+(clid*2);

		lba=img->lba_fat1+(clid>>8);
		ofs=BTESH2_TKFAT_GetSectorTempBuffer(img, lba, 1);
		ofs+=(clid&255)*2;

		i=ofs[0]+(ofs[1]<<8);
		if(i>=0xFFF0)
			i=(i<<16)>>16;
		return(i);
	}

//	ofs=img->pImgData+(img->lba_fat1*512)+(clid*4);
	lba=img->lba_fat1+(clid>>7);
	ofs=BTESH2_TKFAT_GetSectorTempBuffer(img, lba, 1);
	ofs+=(clid&127)*4;

	i=ofs[0]+(ofs[1]<<8)+(ofs[2]<<16)+(ofs[3]<<24);
	if(i>=0x0FFFFFF0)
		i=(i<<4)>>4;
	return(i);
}

int BTESH2_TKFAT_SetFatEntry(BTESH2_TKFAT_ImageInfo *img,
	int clid, int val)
{
	int lba1, lba2;
	byte *ofs1, *ofs2;
	int i;

	if(img->isfat16)
	{
		lba1=img->lba_fat1+(clid>>8);
		lba2=img->lba_fat2+(clid>>8);
		ofs1=BTESH2_TKFAT_GetSectorTempBuffer(img,
			lba1, 1|TKFAT_SFL_DIRTY);
		ofs1+=(clid&255)*2;
		ofs2=BTESH2_TKFAT_GetSectorTempBuffer(img,
			lba2, 1|TKFAT_SFL_DIRTY);
		ofs2+=(clid&255)*2;

//		ofs1=img->pImgData+(img->lba_fat1*512)+(clid*2);
//		ofs2=img->pImgData+(img->lba_fat2*512)+(clid*2);
		ofs1[0]=val; ofs1[1]=val>>8;
		ofs2[0]=val; ofs2[1]=val>>8;
		return(0);
	}

	if(!img->isfat32e)
		val&=0x0FFFFFFF;

	lba1=img->lba_fat1+(clid>>7);
	lba2=img->lba_fat2+(clid>>7);
	ofs1=BTESH2_TKFAT_GetSectorTempBuffer(img,
		lba1, 1|TKFAT_SFL_DIRTY);
	ofs1+=(clid&127)*4;
	ofs2=BTESH2_TKFAT_GetSectorTempBuffer(img,
		lba2, 1|TKFAT_SFL_DIRTY);
	ofs2+=(clid&127)*4;

//	ofs1=img->pImgData+(img->lba_fat1*512)+(clid*4);
//	ofs2=img->pImgData+(img->lba_fat2*512)+(clid*4);
	ofs1[0]=val; ofs1[1]=val>>8; ofs1[2]=val>>16; ofs1[3]=val>>24;
	ofs2[0]=val; ofs2[1]=val>>8; ofs2[2]=val>>16; ofs2[3]=val>>24;
	return(0);
}

/** Get Disk LBA offset for a given cluster number. */
int BTESH2_TKFAT_GetClusterLBA(BTESH2_TKFAT_ImageInfo *img, int clid)
{
	return(img->lba_data+((clid-2)*img->szclust));
}

void BTESH2_TKFAT_SetupImageFAT(BTESH2_TKFAT_ImageInfo *img)
{
	int fatsz, rootsz;
	s64 cln;
	int clsz, clsh;
	int i, j, k;

	if(img->lba_count<128)
	{
		printf("BTESH2_TKFAT_SetupImageFAT: Image LBA Size Bad\n");
		return;
	}

	img->boot16=(BTESH2_TKFAT_FAT16_Boot *)
		BTESH2_TKFAT_GetSectorStaticBuffer(
			img, img->lba_start, 1|TKFAT_SFL_DIRTY);
	img->boot32=(BTESH2_TKFAT_FAT32_Boot *)
		img->boot16;

//	img->boot16=(BTESH2_TKFAT_FAT16_Boot *)
//		(img->pImgData+(img->lba_start*512));
//	img->boot32=(BTESH2_TKFAT_FAT32_Boot *)
//		(img->pImgData+(img->lba_start*512));

	img->boot16->pad_jmp[0]=0xEB;
	img->boot16->pad_jmp[1]=0x7F;
	img->boot16->pad_jmp[2]=0x90;
	img->boot16->aa55[0]=0x55;
	img->boot16->aa55[1]=0xAA;

	if(img->isfat16)
	{
		strcpy(img->boot16->oem_name, "BTESH2  ");
		strcpy(img->boot16->vol_label, "DEFAULT    ");
		strcpy(img->boot16->fs_tyname, "FAT16   ");
		
		clsz=1; clsh=0;
		cln=img->lba_count;
		while(cln>65525)
		{
			clsh++; clsz=1<<clsh;
			cln=img->lba_count>>clsh;
		}
		
//		fatsz=(img->lba_count*2+511)/512;
		fatsz=(cln*2+511)/512;
//		rootsz=512/32;
		rootsz=(512*32+511)>>9;
		
		btesh2_tkfat_setWord(img->boot16->bytes_sector, 512);
		btesh2_tkfat_setWord(img->boot16->reserved_sectors, 2);
		btesh2_tkfat_setWord(img->boot16->root_dirents, 512);
		btesh2_tkfat_setWord(img->boot16->lba_count16, 0);
		btesh2_tkfat_setWord(img->boot16->sectors_fat, fatsz);
		btesh2_tkfat_setWord(img->boot16->sectors_track, 64);
		btesh2_tkfat_setWord(img->boot16->heads, 256);

		btesh2_tkfat_setDWord(img->boot16->hidden_sectors, 0);
		btesh2_tkfat_setDWord(img->boot16->lba_count, img->lba_count);
		btesh2_tkfat_setDWord(img->boot16->vol_sn, rand()*rand());
		
		img->boot16->sectors_cluster=clsz;
		img->boot16->num_fats=2;
		img->boot16->media_type=0xF8;
		img->boot16->drive_id=0x80;
		img->boot16->drive_flag=0x00;
		img->boot16->ebsig=0x29;
		
		img->lba_fat1=img->lba_start+2;
		img->lba_fat2=img->lba_start+2+fatsz;
		img->lba_root=img->lba_start+2+2*fatsz;
		img->lba_data=img->lba_root+rootsz;
		img->szclust=clsz;
		img->shclust=9+clsh;

		cln=((img->lba_start+img->lba_count)-img->lba_data)/clsz;
		img->tot_clust=cln;
		img->clid_root=1;
		
		BTESH2_TKFAT_SetFatEntry(img, 0, 0xFFFF);
		BTESH2_TKFAT_SetFatEntry(img, 1, 0xFFFF);
		
		printf("BTESH2_TKFAT_SetupImageFAT: Created FAT16\n");
		printf("  LBA FAT1=%08X (Offs=%08X)\n",
			img->lba_fat1, img->lba_fat1<<9);
		printf("  LBA FAT2=%08X (Offs=%08X)\n",
			img->lba_fat2, img->lba_fat2<<9);
		printf("  LBA Root=%08X (Offs=%08X)\n",
			img->lba_root, img->lba_root<<9);
		printf("  LBA Data=%08X (Offs=%08X)\n",
			img->lba_data, img->lba_data<<9);
		printf("  %d Sectors/Cluster, %d bytes\n", clsz, 512*clsz);
		printf("  %d total clusters\n", img->tot_clust);
	}else
	{
		strcpy(img->boot32->oem_name, "BTESH2  ");
		strcpy(img->boot32->vol_label, "DEFAULT    ");
		strcpy(img->boot32->fs_tyname, "FAT32   ");

//		if(img->lba_count>=((1LL<<32)-11))
		if(img->lba_count>=(1LL<<32))
			img->isfat32e=true;

		if(img->isfat32e)
		{
			strcpy(img->boot32->oem_name, "FAT32   ");

			clsz=1; clsh=0;
			cln=img->lba_count;
			while(cln>4294967285)
			{
				clsh++; clsz=1<<clsh;
				cln=img->lba_count>>clsh;
			}
		}else
		{
			clsz=1; clsh=0;
			cln=img->lba_count;
			while(cln>268435445)
			{
				clsh++; clsz=1<<clsh;
				cln=img->lba_count>>clsh;
			}
		}
		
//		fatsz=(img->lba_count*4+511)/512;
		fatsz=(cln*4+511)/512;
		
		btesh2_tkfat_setWord(img->boot32->bytes_sector, 512);
		btesh2_tkfat_setWord(img->boot32->reserved_sectors, 8);
		btesh2_tkfat_setWord(img->boot32->root_dirents, 0);
		btesh2_tkfat_setWord(img->boot32->lba_count16, 0);
		btesh2_tkfat_setWord(img->boot32->sectors_fat, 0);
		btesh2_tkfat_setWord(img->boot32->sectors_track, 64);
		btesh2_tkfat_setWord(img->boot32->heads, 256);

		btesh2_tkfat_setWord(img->boot32->fsis_sector, 0xFFFF);
		btesh2_tkfat_setWord(img->boot32->fsaltboot_sector, 0xFFFF);

		btesh2_tkfat_setDWord(img->boot32->hidden_sectors, 0);
		btesh2_tkfat_setDWord(img->boot32->lba_count, img->lba_count);
		btesh2_tkfat_setDWord(img->boot32->sectors_fat32, fatsz);
		btesh2_tkfat_setDWord(img->boot32->root_cluster, 2);
		btesh2_tkfat_setDWord(img->boot32->vol_sn, rand()*rand());

		if(img->isfat32e)
		{
			btesh2_tkfat_setWord(img->boot32->lba_count16, 0);
			btesh2_tkfat_setDWord(img->boot32->lba_count, 0);

			btesh2_tkfat_setDWord(img->boot32->fs_tyname,
				img->lba_count);
			btesh2_tkfat_setDWord(img->boot32->fs_tyname+4,
				img->lba_count>>32);
		}
		
		img->boot32->sectors_cluster=clsz;
		img->boot32->num_fats=2;
		img->boot32->media_type=0xF8;
		img->boot32->drive_id=0x80;
		img->boot32->drive_flag=0x00;
		img->boot32->ebsig=0x29;

		img->lba_fat1=img->lba_start+8;
		img->lba_fat2=img->lba_start+8+fatsz;
		img->lba_data=img->lba_start+8+2*fatsz;
		img->lba_root=img->lba_data;
		img->szclust=clsz;
		img->shclust=9+clsh;

		cln=((img->lba_start+img->lba_count)-img->lba_data)/clsz;
		img->tot_clust=cln;
		img->clid_root=2;

		BTESH2_TKFAT_SetFatEntry(img, 0, 0xFFFFFFFF);
		BTESH2_TKFAT_SetFatEntry(img, 1, 0xFFFFFFFF);

		//pre-allocate a FAT32 root directory
		for(i=0; i<32; i++)
			BTESH2_TKFAT_SetFatEntry(img, i+2, i+2+1);
		BTESH2_TKFAT_SetFatEntry(img, 31+2, 0xFFFFFFFF);

		printf("BTESH2_TKFAT_SetupImageFAT: Created FAT32\n");

		printf("  FAT1 LBA=%08X Offs=%08X\n",
			img->lba_fat1, img->lba_fat1<<9);
		printf("  FAT2 LBA=%08X Offs=%08X\n",
			img->lba_fat2, img->lba_fat2<<9);
		printf("  Root LBA=%08X Offs=%08X\n",
			img->lba_root, img->lba_root<<9);
		printf("  Data LBA=%08X Offs=%08X\n",
			img->lba_data, img->lba_data<<9);
		printf("  %d Sectors/Cluster, %d bytes\n", clsz, 512*clsz);
		printf("  %d total clusters\n", img->tot_clust);
	}
}

void BTESH2_TKFAT_ReadImageFAT(BTESH2_TKFAT_ImageInfo *img)
{
	s64 lban, fatsz, cln;
	int rootsz, rootnde, rootcl;
	int clsz, clsh;
	u32 i0, i1, i2, i3;
	int i;

	img->boot16=(BTESH2_TKFAT_FAT16_Boot *)
		BTESH2_TKFAT_GetSectorStaticBuffer(
			img, img->lba_start, 1);
	img->boot32=(BTESH2_TKFAT_FAT32_Boot *)
		img->boot16;


	i0=btesh2_tkfat_getWord(img->boot32->lba_count16);
	i1=btesh2_tkfat_getDWord(img->boot32->lba_count);

	i2=btesh2_tkfat_getWord(img->boot32->sectors_fat);
	i3=btesh2_tkfat_getDWord(img->boot32->sectors_fat32);

	rootnde=btesh2_tkfat_getWord(img->boot32->root_dirents);
	clsz=img->boot32->sectors_cluster;

	rootcl=btesh2_tkfat_getDWord(img->boot32->root_cluster);

	if((i0!=0) && (i1!=0) && (img->boot32->ebsig==0x29))
	{
		lban=(((s64)i0)<<32)|i1;
		fatsz=(((s64)i2)<<32)|i3;
	}else
	{
		lban=i0?i0:i1;
		fatsz=i2?i2:i3;
		if(!lban)
		{
			i0=btesh2_tkfat_getDWord(img->boot32->fs_tyname+4);
			i1=btesh2_tkfat_getDWord(img->boot32->fs_tyname);
			lban=(((s64)i0)<<32)|i1;
		}
	}
	
	rootsz=0;
	if(rootnde)
		{ rootsz=((rootnde*32)+511)>>9; }

	if(!clsz || (clsz&(clsz-1)))
	{
		printf("TKFAT_ReadImageFAT: Bad Sectors/Cluster %d\n", clsz);
		return;
	}else
	{
		i=clsz; clsh=0;
		while(i>1)
			{ i=i>>1; clsh++; }
	}
	
	if((img->boot32->ebsig!=0x29) || !rootcl)
	{
		rootcl=1;
	}

	img->lba_fat1=img->lba_start+2;
	img->lba_fat2=img->lba_start+2+fatsz;
	img->lba_root=img->lba_start+2+2*fatsz;
	img->lba_data=img->lba_root+rootsz;
	img->szclust=clsz;
	img->shclust=9+clsh;

//	cln=((img->lba_start+img->lba_count)-img->lba_data)/clsz;
//	cln=((img->lba_start+img->lba_count)-img->lba_data)>>clsh;
	cln=((img->lba_start+lban)-img->lba_data)>>clsh;
	img->tot_clust=cln;
	img->clid_root=rootcl;
	
	img->isfat16=false;
	if(cln<=65525)
		img->isfat16=true;
	
	printf("TKFAT_ReadImageFAT: Read FAT%d\n", img->isfat16?16:32);
	printf("  LBA FAT1=%08X (Offs=%08X)\n",
		img->lba_fat1, img->lba_fat1<<9);
	printf("  LBA FAT2=%08X (Offs=%08X)\n",
		img->lba_fat2, img->lba_fat2<<9);
	printf("  LBA Root=%08X (Offs=%08X)\n",
		img->lba_root, img->lba_root<<9);
	printf("  LBA Data=%08X (Offs=%08X)\n",
		img->lba_data, img->lba_data<<9);
	printf("  %d Sectors/Cluster, %d bytes\n", clsz, 512*clsz);
	printf("  %d total clusters\n", img->tot_clust);
}

/** Allocate a cluster from the FAT. Implicitly marks as EOF. */
int BTESH2_TKFAT_AllocFreeCluster(BTESH2_TKFAT_ImageInfo *img)
{
	int n;
	int i, j, k;
	
	n=(img->lba_count-img->lba_data)/img->szclust;
	
	for(i=2; i<n; i++)
	{
		j=BTESH2_TKFAT_GetFatEntry(img, i);
		if(!j)
		{
			BTESH2_TKFAT_SetFatEntry(img, i, 0xFFFFFFFF);
			return(i);
		}
	}
	return(-1);
}

/** Get cluster ID at a specific position within a chain starting at clid.
  * Negative values means an error was encountered.
  * If expand is true, chain is expanded as-needed.
  */
int BTESH2_TKFAT_GetWalkCluster(
	BTESH2_TKFAT_ImageInfo *img,
	int clid, int cloffs, bool expand)
{
	int i, j, n;

	if(!clid)
		return(-1);

	i=clid; n=cloffs;
	while(n>0)
	{
		j=BTESH2_TKFAT_GetFatEntry(img, i);
		if(j<0)
		{
			if(!expand)
				return(-1);

			j=BTESH2_TKFAT_AllocFreeCluster(img);
			if(j<=0)
				return(-1);
			BTESH2_TKFAT_SetFatEntry(img, i, j);
//			BTESH2_TKFAT_SetFatEntry(img, j, 0x0FFFFFFF);
		}
		i=j;
		n--;
	}
	return(i);
}

int BTESH2_TKFAT_GetClusterFileOffs(BTESH2_TKFAT_ImageInfo *img,
	int clid, int foffs, bool expand,
	int *rclid, int *rclfrac)
{
	int cloffs, clfrac;
	int cl2;
	
	cloffs=foffs>>img->shclust;
	clfrac=foffs&((1<<img->shclust)-1);
	cl2=BTESH2_TKFAT_GetWalkCluster(img, clid, cloffs, expand);
	if(cl2<0)return(-1);
	
	*rclid=cl2;
	*rclfrac=clfrac;
	return(0);
}

int BTESH2_TKFAT_ReadWriteSector(BTESH2_TKFAT_ImageInfo *img,
	int lba, int offs, bool iswrite, byte *data, int size)
{
	byte *clbuf;
//	lba=BTESH2_TKFAT_GetClusterLBA(img, clid);

	if(iswrite)
	{
		clbuf=BTESH2_TKFAT_GetSectorTempBuffer(img,
			lba, 1|TKFAT_SFL_DIRTY);
		memcpy(clbuf+offs, data, size);
	}
	else
	{
		clbuf=BTESH2_TKFAT_GetSectorTempBuffer(img,
			lba, 1);
		memcpy(data, clbuf+offs, size);
	}
	return(0);
}

int BTESH2_TKFAT_ReadWriteCluster(BTESH2_TKFAT_ImageInfo *img,
	int clid, int offs, bool iswrite, byte *data, int size)
{
	byte *clbuf;
	int lba;
	
	lba=BTESH2_TKFAT_GetClusterLBA(img, clid);

	if(iswrite)
	{
		clbuf=BTESH2_TKFAT_GetSectorTempBuffer(img,
			lba, img->szclust|TKFAT_SFL_DIRTY);
		memcpy(clbuf+offs, data, size);
	}
	else
	{
		clbuf=BTESH2_TKFAT_GetSectorTempBuffer(img,
			lba, img->szclust);
		memcpy(data, clbuf+offs, size);
	}
	return(0);
}

int BTESH2_TKFAT_ReadWriteClusterOffset(BTESH2_TKFAT_ImageInfo *img,
	int clid, int foffs, bool iswrite,
	byte *data, int size)
{
	byte *ct, *cte;
	int clid1, clid2, clidt;
	int offs1, offs2, szcl;
	int i, j, k;
	
	/* First, check if access is within a single cluster. */
	offs2=foffs+(size-1);
	if((foffs>>img->shclust)==(offs2>>img->shclust))
	{
		i=BTESH2_TKFAT_GetClusterFileOffs(img, clid, foffs, iswrite,
			&clid1, &offs1);
		if(i<0)return(i);
		i=BTESH2_TKFAT_ReadWriteCluster(img,
			clid1, offs1, iswrite, data, size);
		return(i);
	}

	i=BTESH2_TKFAT_GetClusterFileOffs(img, clid,
		foffs, iswrite, &clid1, &offs1);
	j=BTESH2_TKFAT_GetClusterFileOffs(img, clid,
		foffs+(size-1), iswrite, &clid2, &offs2);
	if(i<0)return(i);
	if(j<0)return(j);
	
	szcl=(1<<img->shclust);
	ct=data; cte=data+size;
	clidt=clid1;
	BTESH2_TKFAT_ReadWriteCluster(img,
		clidt, offs1, iswrite, ct, szcl-offs1);
	clidt=BTESH2_TKFAT_GetWalkCluster(img, clidt, 1, iswrite);
	ct+=szcl-offs1;
	while((ct+szcl)<=cte)
	{
		BTESH2_TKFAT_ReadWriteCluster(img,
			clidt, 0, iswrite, ct, szcl);
		clidt=BTESH2_TKFAT_GetWalkCluster(img, clidt, 1, iswrite);
		if(clidt<0)
			return(-1);
		ct+=szcl;
	}
	if(ct<cte)
	{
		BTESH2_TKFAT_ReadWriteCluster(img,
			clidt, 0, iswrite, ct, cte-ct);
	}
	return(0);
}

int BTESH2_TKFAT_ReadWriteDirEntOffset(BTESH2_TKFAT_ImageInfo *img,
	int clid, int idx, bool iswrite, void *de)
{
	int i, n;

	if(clid<=0)
		return(-1);

	if(clid==1)
	{
		if(idx<0)
			return(-1);
		n=btesh2_tkfat_getWord(img->boot16->root_dirents);
		if(idx>=n)
			return(-1);
		BTESH2_TKFAT_ReadWriteSector(img,
			img->lba_root+(idx>>4), (idx&15)<<5, iswrite, de, 32);
		return(0);
	}
	
	i=BTESH2_TKFAT_ReadWriteClusterOffset(img,
		clid, idx<<5, iswrite, de, 32);
	return(i);
}

int btesh2_tkfat_name2sfn(char *name, char *sfn)
{
	char *s, *t;
	int i, j, k;

	for(i=0; i<11; i++)
		sfn[i]=' ';

	s=name; t=sfn;
	for(i=0; i<8; i++)
	{
		if(!(*s) || (*s=='.'))
			break;
		j=*s++;
		if((j>='a') && (j<='z'))
			j='A'+(j-'a');
		*t++=j;
	}
	if(!(*s))
		return(0);
	if(*s!='.')
		return(-1);
	s++;

	t=sfn+8;
	for(i=0; i<3; i++)
	{
		if(!(*s) || (*s=='.'))
			break;
		j=*s++;
		if((j>='a') && (j<='z'))
			j='A'+(j-'a');
		*t++=j;
	}
	if(*s)
		return(-1);
	return(0);
}

int btesh2_tkfat_name2lfn(char *name, u16 *lfn)
{
	char *s;
	u16 *t, *te;
	int i, j, k;
	
	for(i=0; i<256; i++)
		lfn[i]=0xFFFF;
	
	s=name; t=lfn; te=t+256;
	while(*s && (t<te))
	{
		i=*s++;
		if(!(i&0x80))
			{ *t++=i; continue; }
		if((i&0xE0)==0xC0)
		{
			j=((i&0x1F)<<6)|((*s++)&0x3F);
		}else if((i&0xF0)==0xE0)
		{
			j=((i&0x0F)<<12)|(((s[0])&0x3F)<<6)|((s[1])&0x3F);
			s+=2;
		}else if((i&0xF8)==0xF0)
		{
			j=((i&0x07)<<18)|(((s[0])&0x3F)<<12)|
				(((s[1])&0x3F)<<6)|((s[2])&0x3F);
			s+=3;
		}
		if(j>=0x10000)
		{
			*t++=0xD800+((j>>10)&1023);
			*t++=0xDC00+((j    )&1023);
		}else
		{
			*t++=j;
		}
	}
	*t=0;
	return(0);
}

int btesh2_tkfat_matchlfn(u16 *lfn1, u16 *lfn2)
{
	u16 *s1, *s2;
	int i1, i2;
	
	s1=lfn1; s2=lfn2;
	i1=*s1; i2=*s2;
	while(*s1 && *s2)
	{
		i1=*s1++;
		i2=*s2++;
		if((i1>='a') && (i1<='z'))
			i1='A'+(i1-'a');
		if((i2>='a') && (i2<='z'))
			i2='A'+(i2-'a');
		if(i1!=i2)
			break;
	}
	if(i1>i2)return( 1);
	if(i2>i1)return(-1);
	return(0);
}

int btesh2_tkfat_lfnchecksum(char *name)
{
	byte *s;
	int i;
	byte h;

	s=(byte *)name; h=0;
	for(i=0; i<11; i++)
		h=((h&1)<<7)+(h>>1)+(*s++);
	return(h&255);
}

static const int tkfat_tuc52[32]={
	0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021,
	0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0xFFFF,
	0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
	0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009C, 0x017E, 0x0178};

int btesh2_tkfat_asc2ucs(int v)
{
	if(!(v&0x80))
		return(v);
	if(v&0x60)
		return(v);
	return(tkfat_tuc52[v&0x1F]);
}

int btesh2_tkfat_ucs2asc(int v)
{
	int i;
	if(!(v&0x80))
		return(v);
	if(v&0x60)
		return(v);
	for(i=0; i<32; i++)
		if(v==tkfat_tuc52[i])
			return(0x80+i);
	return(v&255);
}

byte *btesh2_tkfat_emitUtf8(byte *ct, int v)
{
	if(v<0x80)
		{ *ct++=v; }
	else if(v<0x0800)
	{
		*ct++=0xC0|((v>>6)&0x1F);
		*ct++=0x80|((v   )&0x3F);
	}
	else if(v<0x10000)
	{
		*ct++=0xE0|((v>>12)&0x0F);
		*ct++=0x80|((v>> 6)&0x3F);
		*ct++=0x80|((v    )&0x3F);
	}
	else if(v<0x200000)
	{
		*ct++=0xF0|((v>>18)&0x07);
		*ct++=0x80|((v>>12)&0x3F);
		*ct++=0x80|((v>> 6)&0x3F);
		*ct++=0x80|((v    )&0x3F);
	}
	return(ct);
}

int btesh2_tkfat_lfn2utf8(u16 *lfn, byte *dst)
{
	u16 *cs;
	byte *ct;
	int i;
	
	cs=lfn; ct=dst;
	while(*cs)
	{
		i=*cs++;
		ct=btesh2_tkfat_emitUtf8(ct, i);
	}
	*ct++=0;
	return(0);
}

int btesh2_tkfat_sfn2utf8(byte *sfn, int lcase, byte *dst)
{
	byte *cs, *ct;
	int i, j, k;

	cs=sfn; ct=dst;
	
	if(*cs<=' ')
		return(-1);
	if(lcase&0x20)
		return(-1);
	
	for(i=0; i<8; i++)
	{
		j=*cs++;

		if((j<' ') || (j==0x7F))
			return(-1);
		
		if(j==' ')
		{
			for(k=i+1; k<8; k++)
				if(sfn[k]>' ')
					break;
			if(k>=8)
				break;
		}
		
		if(lcase&0x08)
		{
			if((j>='A') && j<='Z')
				j='a'+(j-'A');
		}
		
		j=btesh2_tkfat_asc2ucs(j);
		ct=btesh2_tkfat_emitUtf8(ct, j);
//		*ct++=j;
	}
	
	cs=sfn+8;
	if(*cs<' ')
		return(-1);
	if(*cs==' ')
	{
		*ct++=0;
		return(1);
	}
	
	*ct++='.';

	for(i=0; i<3; i++)
	{
		j=*cs++;

		if((j<' ') || (j==0x7F))
			return(-1);
		
		if(j==' ')
		{
			break;
		}
		
		if(lcase&0x10)
		{
			if((j>='A') && j<='Z')
				j='a'+(j-'A');
		}
//		*ct++=j;

		j=btesh2_tkfat_asc2ucs(j);
		ct=btesh2_tkfat_emitUtf8(ct, i);
	}
	*ct++=0;

	return(1);
}

int btesh2_tkfat_lfn2bytes(u16 *lfn, int sz, byte *dst)
{
	u16 *cs;
	byte *ct;
	int i, j, k;
	
	cs=lfn; ct=dst;
	for(i=0; i<sz; i++)
	{
		j=*cs++;
		*ct++=btesh2_tkfat_ucs2asc(j);
	}
	*ct++=0;
	return(0);
}

int btesh2_tkfat_stricmp(byte *str1, byte *str2)
{
	byte *cs1, *cs2;
	int i1, i2;

	cs1=str1;
	cs2=str2;
	i1=*cs1; i2=*cs2;
	while(*cs1 && *cs2)
	{
		i1=*cs1++;
		i2=*cs2++;
		
		if((i1>='a') && (i1<='z'))
			i1='A'+(i1-'a');
		if((i2>='a') && (i2<='z'))
			i2='A'+(i2-'a');
		if(i1!=i2)
			break;
	}
	if(i1>i2)return(1);
	if(i2>i1)return(-1);
	return(0);
}

int BTESH2_TKFAT_WalkDirEntNext(BTESH2_TKFAT_ImageInfo *img,
	BTESH2_TKFAT_FAT_DirEntExt *dee)
{
	BTESH2_TKFAT_FAT_DirEnt tdeb;
	BTESH2_TKFAT_FAT_DirEnt *deb;
	BTESH2_TKFAT_FAT_DirLfnEnt *del;
	u16 bln[288];	//buffer for longname
	u16 bln2[420];	//buffer for longname (alt)
	u16 tln[288];	//temp longname
	char tsn[12];	//temp shortname
	char *s;
	int h0, h1, lh, clid, sidx;
	int i, j, k;
	
	if(!img)
		img=dee->img;
	
	clid=dee->clid;
	sidx=(dee->idx>=0)?(dee->idx+1):0;

	deb=&tdeb;
	del=(BTESH2_TKFAT_FAT_DirLfnEnt *)(&tdeb);

	for(k=0; k<288; k++)
	{
		bln[k]=0xFFFF;
		bln2[k]=0xFFFF;
	}
	
	bln[0]=0;
	bln2[0]=0;

	for(i=sidx; i<65536; i++)
	{
		j=BTESH2_TKFAT_ReadWriteDirEntOffset(img, clid, i, 0, deb);
		if(j<0)break;
		
		if(deb->name[0]==0x00)
			continue;
		if(deb->name[0]==0xE5)
			continue;
		
		if(deb->attrib==0x0F)
		{
//			j=(del->seq)&0x1F;
//			j=del->seq;
			j=(del->seq)&0x3F;
			if((j>=0x01) && j<=0x14)
			{
				h1=del->csum;
				if(h1!=lh)
				{
					for(k=0; k<288; k++)
						bln[k]=0xFFFF;
					lh=h1;
				}

				j=(del->seq)&0x1F;
//				j=((del->seq)&0x1F)*13;
				j=(j-1)*13;
				if(del->type==0x20)
				{
					for(k=0; k<10; k++)
						{ bln[j+0+k]=btesh2_tkfat_asc2ucs(
							del->name1[k]); }
					for(k=0; k<12; k++)
						{ bln[j+5+k]=btesh2_tkfat_asc2ucs(
							del->name2[k]); }
					for(k=0; k<4; k++)
						{ bln[j+11+k]=btesh2_tkfat_asc2ucs(
							del->name3[k]); }
					if((del->seq)&0x40)
						bln[j+26]=0;
				}else if(del->type==0x00)
				{
					for(k=0; k<5; k++)
						{ bln[j+0+k]=btesh2_tkfat_getWord(
							del->name1+k*2); }
					for(k=0; k<6; k++)
						{ bln[j+5+k]=btesh2_tkfat_getWord(
							del->name2+k*2); }
					for(k=0; k<2; k++)
						{ bln[j+11+k]=btesh2_tkfat_getWord(
							del->name3+k*2); }
					if((del->seq)&0x40)
						bln[j+13]=0;
				}
			}

//			j=del->seq;
			j=(del->seq)&0x3F;
			if((j>=0x21) && j<=0x3D)
			{
				h1=del->csum;
				if(h1!=lh)
				{
					for(k=0; k<288; k++)
						bln2[k]=0xFFFF;
					lh=h1;
				}

				j=(del->seq)&0x1F;
//				j=((del->seq)&0x1F)*13;
				j=(j-1)*13;
				if(del->type==0x20)
				{
					for(k=0; k<10; k++)
						{ bln2[j+0+k]=btesh2_tkfat_asc2ucs(
							del->name1[k]); }
					for(k=0; k<12; k++)
						{ bln2[j+5+k]=btesh2_tkfat_asc2ucs(
							del->name2[k]); }
					for(k=0; k<4; k++)
						{ bln2[j+11+k]=btesh2_tkfat_asc2ucs(
							del->name3[k]); }
					if((del->seq)&0x40)
						bln2[j+26]=0;
				}else if(del->type==0x00)
				{
					for(k=0; k<5; k++)
						{ bln2[j+0+k]=btesh2_tkfat_getWord(
							del->name1+k*2); }
					for(k=0; k<6; k++)
						{ bln2[j+5+k]=btesh2_tkfat_getWord(
							del->name2+k*2); }
					for(k=0; k<2; k++)
						{ bln2[j+11+k]=btesh2_tkfat_getWord(
							del->name3+k*2); }
					if((del->seq)&0x40)
						bln2[j+13]=0;
				}
			}
			
			continue;
		}
		
		h0=btesh2_tkfat_lfnchecksum(deb->name);
//		if(!memcmp(deb.name, tsn, 11))
//		if((h0==h1) && !tkfat_matchlfn(bln, tln))
//		if(h0==h1)
		if(1)
		{
			if(h0==h1)
			{
				btesh2_tkfat_lfn2utf8(bln, dee->de_name);
				
				dee->de_aname[0]=0;
				if(bln2[0] && (bln2[0]!=0xFFFF))
					btesh2_tkfat_lfn2utf8(bln2, dee->de_aname);
				if(deb->attrib==0x28)
					btesh2_tkfat_lfn2bytes(bln2, 384, dee->de_aname);
			}else
			{
				j=btesh2_tkfat_sfn2utf8(
					deb->name, deb->lncase, dee->de_name);
				if(j<0)
				{
					printf("TKFAT_WalkDirEntNext: Reject DirEnt\n");
					continue;
				}
				dee->de_aname[0]=0;
			}

			dee->deb=*deb;
//			dee->img=img;
//			dee->clid=clid;
			dee->idx=i;
//			break;
//			strcpy(dee->de_name, name);
			return(i);
		}
	}

	return(-1);
}

int BTESH2_TKFAT_LookupDirEntName(BTESH2_TKFAT_ImageInfo *img,
	int clid, BTESH2_TKFAT_FAT_DirEntExt *dee, char *name)
{
	int i, j;

	dee->img=img;
	dee->clid=clid;
	dee->idx=-1;
	
	while(1)
	{
		i=BTESH2_TKFAT_WalkDirEntNext(img, dee);
		if(i<0)
			break;
		if(!btesh2_tkfat_stricmp(dee->de_name, name))
			return(i);
	}
	return(-1);
}

#if 0
int BTESH2_TKFAT_LookupDirEntName(BTESH2_TKFAT_ImageInfo *img,
	int clid, BTESH2_TKFAT_FAT_DirEntExt *dee, char *name)
{
	BTESH2_TKFAT_FAT_DirEnt tdeb;
	BTESH2_TKFAT_FAT_DirEnt *deb;
	BTESH2_TKFAT_FAT_DirLfnEnt *del;
	u16 bln[288];	//buffer for longname
	u16 bln2[420];	//buffer for longname (alt)
	u16 tln[288];	//temp longname
	char tsn[12];	//temp shortname
	char *s;
	int h0, h1, lh;
	int i, j, k;
	
	deb=&tdeb;
	del=(BTESH2_TKFAT_FAT_DirLfnEnt *)(&tdeb);
	
	lh=-1;
	i=btesh2_tkfat_name2sfn(name, tsn);
	if(i>=0)
	{
		for(i=0; i<65536; i++)
		{
			j=BTESH2_TKFAT_ReadWriteDirEntOffset(img, clid, i, 0, deb);
			if(j<0)break;

			if(deb->name[0]==0x00)
				continue;
			if(deb->name[0]==0xE5)
				continue;

			if(!memcmp(deb->name, tsn, 11))
			{
				dee->deb=tdeb;
				dee->img=img;
				dee->clid=clid;
				dee->idx=i;
				strcpy(dee->de_name, name);
				return(i);
			}
		}
		return(-1);
	}

	btesh2_tkfat_name2lfn(name, tln);

	for(i=0; i<65536; i++)
	{
		j=BTESH2_TKFAT_ReadWriteDirEntOffset(img, clid, i, 0, deb);
		if(j<0)break;
		
		if(deb->name[0]==0x00)
			continue;
		if(deb->name[0]==0xE5)
			continue;
		
		if(deb->attrib==0x0F)
		{
//			j=(del->seq)&0x1F;
//			j=del->seq;
			j=(del->seq)&0x3F;
			if((j>=0x01) && j<=0x14)
			{
				h1=del->csum;
				if(h1!=lh)
				{
					for(k=0; k<288; k++)
						bln[k]=0xFFFF;
					lh=h1;
				}

				j=(del->seq)&0x1F;
//				j=((del->seq)&0x1F)*13;
				j=(j-1)*13;
				if(del->type==0x20)
				{
					for(k=0; k<10; k++)
						{ bln[j+0+k]=btesh2_tkfat_asc2ucs(del->name1[k]); }
					for(k=0; k<12; k++)
						{ bln[j+5+k]=btesh2_tkfat_asc2ucs(del->name2[k]); }
					for(k=0; k<4; k++)
						{ bln[j+11+k]=btesh2_tkfat_asc2ucs(del->name3[k]); }
					if((del->seq)&0x40)
						bln[j+26]=0;
				}else if(del->type==0x00)
				{
					for(k=0; k<5; k++)
						{ bln[j+0+k]=btesh2_tkfat_getWord(
							del->name1+k*2); }
					for(k=0; k<6; k++)
						{ bln[j+5+k]=btesh2_tkfat_getWord(
							del->name2+k*2); }
					for(k=0; k<2; k++)
						{ bln[j+11+k]=btesh2_tkfat_getWord(
							del->name3+k*2); }
					if((del->seq)&0x40)
						bln[j+13]=0;
				}
			}

//			j=del->seq;
			j=(del->seq)&0x3F;
			if((j>=0x21) && j<=0x3D)
			{
				h1=del->csum;
				if(h1!=lh)
				{
					for(k=0; k<288; k++)
						bln2[k]=0xFFFF;
					lh=h1;
				}

				j=(del->seq)&0x1F;
//				j=((del->seq)&0x1F)*13;
				j=(j-1)*13;
				if(del->type==0x20)
				{
					for(k=0; k<10; k++)
						{ bln2[j+0+k]=btesh2_tkfat_asc2ucs(del->name1[k]); }
					for(k=0; k<12; k++)
						{ bln2[j+5+k]=btesh2_tkfat_asc2ucs(del->name2[k]); }
					for(k=0; k<4; k++)
						{ bln2[j+11+k]=btesh2_tkfat_asc2ucs(del->name3[k]); }
					if((del->seq)&0x40)
						bln2[j+26]=0;
				}else if(del->type==0x00)
				{
					for(k=0; k<5; k++)
						{ bln2[j+0+k]=btesh2_tkfat_getWord(
							del->name1+k*2); }
					for(k=0; k<6; k++)
						{ bln2[j+5+k]=btesh2_tkfat_getWord(
							del->name2+k*2); }
					for(k=0; k<2; k++)
						{ bln2[j+11+k]=btesh2_tkfat_getWord(
							del->name3+k*2); }
					if((del->seq)&0x40)
						bln2[j+13]=0;
				}
			}
			
			continue;
		}
		
		h0=btesh2_tkfat_lfnchecksum(deb->name);
//		if(!memcmp(deb.name, tsn, 11))
		if((h0==h1) && !btesh2_tkfat_matchlfn(bln, tln))
		{
			dee->deb=*deb;
			dee->img=img;
			dee->clid=clid;
			dee->idx=i;
			strcpy(dee->de_name, name);
			return(i);
		}
	}

	return(-1);
}
#endif

int BTESH2_TKFAT_CreateDirEntName(BTESH2_TKFAT_ImageInfo *img,
	int clid, bool create, BTESH2_TKFAT_FAT_DirEntExt *dee, char *name)
{
	BTESH2_TKFAT_FAT_DirEnt tdeb;
	BTESH2_TKFAT_FAT_DirEnt *deb;
	BTESH2_TKFAT_FAT_DirLfnEnt *del;
//	u16 bln[256];
	u16 tln[258];
	char tsn[12];
	char *s;
	int h0, h1, lh, li, n;
	int i, j, k, l;
	
	i=BTESH2_TKFAT_LookupDirEntName(img, clid, dee, name);
	if(i>=0)
		return(i);
	
	if(!create)
		return(-1);

	deb=&tdeb;
	del=(BTESH2_TKFAT_FAT_DirLfnEnt *)(&tdeb);
	
	lh=-1;
	i=btesh2_tkfat_name2sfn(name, tsn);
	if(i>=0)
	{
		for(i=0; i<65536; i++)
		{
			j=BTESH2_TKFAT_ReadWriteDirEntOffset(img, clid, i, 0, deb);
			if(j<0)
			{
				memset(deb, 0, 32);
				j=BTESH2_TKFAT_ReadWriteDirEntOffset(
					img, clid, i, 1, deb);
				if(j<0)
					break;
			}

			if((deb->name[0]==0x00) || (deb->name[0]==0xE5))
			{
				memset(deb, 0, 32);
				memcpy(deb->name, tsn, 11);
				BTESH2_TKFAT_ReadWriteDirEntOffset(
					img, clid, i, 1, deb);
				
				dee->deb=tdeb;
				dee->img=img;
				dee->clid=clid;
				dee->idx=i;
				strcpy(dee->de_name, name);
				return(i);
			}
		}
		return(-1);
	}

//	for(i=0; i<258; i++)
//		tln[i]=0xFFFF;

	btesh2_tkfat_name2lfn(name, tln);
	for(l=0; tln[l]; l++);
	lh=1+((l+13)/13);

	h0=0;
	for(i=0; tln[i]; i++)
	{
//		h0=(h0*65521)+tln[i]+1;
		h0=(h0*4093)+tln[i]+1;
	}
	memset(tsn, 0, 11);
	tsn[0]=' ';
	tsn[1]=0;
	btesh2_tkfat_setDWord(tsn+2, h0);
	tsn[5]=':';
	h1=btesh2_tkfat_lfnchecksum(tsn);

	li=0;
	for(i=0; i<65536; i++)
	{
		j=BTESH2_TKFAT_ReadWriteDirEntOffset(img, clid, i, 0, deb);
		if(j<0)
		{
			memset(deb, 0, 32);
			j=BTESH2_TKFAT_ReadWriteDirEntOffset(
				img, clid, i, 1, deb);
			if(j<0)
				break;
		}
		
		if((deb->name[0]==0x00) || (deb->name[0]==0xE5))
		{
			if((i-li)>=lh)
				break;
			continue;
		}
		li=i+1;
		continue;
	}

	if((i<65536) && (j>=0) && (i-li)>=lh)
	{
		n=lh-1;
		for(i=0; i<n; i++)
		{
			memset(del, 0, 32);
//			del->seq=lh-i;
			del->seq=n-i;
			if(!i)del->seq|=0x40;
			del->attrib=0x0F;
			del->csum=h1;
			
			j=(n-i-1)*13;

			for(k=0; k<5; k++)
				{ btesh2_tkfat_setWord(del->name1+k*2, tln[j+0+k]); }
			for(k=0; k<6; k++)
				{ btesh2_tkfat_setWord(del->name2+k*2, tln[j+5+k]); }
			for(k=0; k<2; k++)
				{ btesh2_tkfat_setWord(del->name3+k*2, tln[j+11+k]); }
			
			BTESH2_TKFAT_ReadWriteDirEntOffset(
				img, clid, li+i, 1, del);
		}

		memset(deb, 0, 32);

		memcpy(deb->name, tsn, 11);
		BTESH2_TKFAT_ReadWriteDirEntOffset(
			img, clid, li+i, 1, deb);
		
		dee->deb=tdeb;
		dee->img=img;
		dee->clid=clid;
		dee->idx=li+i;
		strcpy(dee->de_name, name);
		return(i);
	}

	return(-1);
}

int BTESH2_TKFAT_CreateDirEntPathR(BTESH2_TKFAT_ImageInfo *img,
	int clid, bool create, BTESH2_TKFAT_FAT_DirEntExt *dee, char *name)
{
	BTESH2_TKFAT_FAT_DirEntExt tdee;
	char tb[256];
	char *s, *t;
	bool mkd;
	int i;
	
	mkd=false;
	s=name;
	while(*s=='/')s++;
	t=tb;
	while(*s && (*s=='/'))s++;
	while(*s && (*s!='/'))
		{ *t++=*s++; }
	*t++=0;
	if(*s=='/')
		{ s++; mkd=true; }

//	if(*s)
	if(mkd)
	{
		i=BTESH2_TKFAT_CreateDirEntName(img, clid, create, &tdee, tb);
		if(i<0)
		{
			printf("BTESH2_TKFAT_CreateDirEntPathR: "
				"Failed Recurse %s\n", tb);
			return(i);
		}
		BTESH2_TKFAT_SetupDirEntNewDirectory(&tdee);
		
		i=BTESH2_TKFAT_CreateDirEntPathR(img, tdee.clid, create, dee, s);
		return(i);
	}

	i=BTESH2_TKFAT_CreateDirEntName(img, clid, create, dee, tb);
	return(i);
}

int BTESH2_TKFAT_CreateDirEntPath(
	BTESH2_TKFAT_ImageInfo *img,
	BTESH2_TKFAT_FAT_DirEntExt *dee,
	char *name)
{
	int clid;
	int i;
	
//	clid=img->isfat16?1:2;
	clid=img->clid_root;
	
	i=BTESH2_TKFAT_CreateDirEntPathR(img, clid, true, dee, name);
	return(i);
}

int BTESH2_TKFAT_LookupDirEntPath(
	BTESH2_TKFAT_ImageInfo *img,
	BTESH2_TKFAT_FAT_DirEntExt *dee,
	char *name)
{
	int clid;
	int i;
	
//	clid=img->isfat16?1:2;
	clid=img->clid_root;
	
	i=BTESH2_TKFAT_CreateDirEntPathR(img, clid, false, dee, name);
	return(i);
}

int BTESH2_TKFAT_UpdateDirEnt(
	BTESH2_TKFAT_FAT_DirEntExt *dee)
{
	int i;
	i=BTESH2_TKFAT_ReadWriteDirEntOffset(
		dee->img, dee->clid, dee->idx, 1, &(dee->deb));
	return(i);
}

u32 BTESH2_TKFAT_GetDirEntCluster(
	BTESH2_TKFAT_FAT_DirEntExt *dee)
{
	int i, j;
	i=btesh2_tkfat_getWord(dee->deb.cluster_lo);
	j=btesh2_tkfat_getWord(dee->deb.cluster_hi);
	return(i|(j<<16));
}

int BTESH2_TKFAT_SetDirEntCluster(
	BTESH2_TKFAT_FAT_DirEntExt *dee,
	u32 clid)
{
	btesh2_tkfat_setWord(dee->deb.cluster_lo, clid);
	btesh2_tkfat_setWord(dee->deb.cluster_hi, clid>>16);
	return(0);
}

s64 BTESH2_TKFAT_GetDirEntSize(
	BTESH2_TKFAT_FAT_DirEntExt *dee)
{
	s64 i;
	int j, k;
	
	i=btesh2_tkfat_getDWord(dee->deb.filesize);
	if((dee->deb.lncase&0x20) &&
		(dee->deb.name[0]==' ') &&
		(dee->deb.name[1]==0))
	{
		if(dee->deb.lncase&0x08)
		{
			j=(dee->deb.ctime_ms);
			k=(dee->deb.lncase&7);
			i=i|(((s64)j)<<32)|(((s64)k)<<40);
		}else
		{
			i=i|(((s64)(dee->deb.lncase&7))<<32);
		}
	}else
	{
		j=(dee->deb.lncase&7);
		k=((dee->deb.lncase>>5)&7);
//		i=i|(((s64)(dee->deb.lncase&7))<<32);
		i=i|(((s64)j)<<32)|(((s64)k)<<35);
	}
	return(i);
}

int BTESH2_TKFAT_SetDirEntSize(
	BTESH2_TKFAT_FAT_DirEntExt *dee, s64 sz)
{
	btesh2_tkfat_setDWord(dee->deb.filesize, sz);

	if(	(dee->deb.name[0]==' ') &&
		(dee->deb.name[1]==0))
	{
		dee->deb.lncase=0;
		if(((int)(sz>>35))!=0)
		{
			dee->deb.lncase=0x28;
			dee->deb.ctime_ms=sz>>32;
			dee->deb.lncase|=(byte)((sz>>40)&7);
		}else
		{
			dee->deb.lncase=0x20;
			dee->deb.lncase|=(byte)((sz>>32)&7);
		}
	}else
	{
		dee->deb.lncase=dee->deb.lncase&0x18;
		dee->deb.lncase|=(byte)((sz>>32)&7);
		dee->deb.lncase|=(byte)(((sz>>35)&7)<<5);
	}

	return(0);
}

int BTESH2_TKFAT_ReadWriteDirEntFile(
	BTESH2_TKFAT_FAT_DirEntExt *dee,
	u32 offs, bool iswrite,
	byte *data, int size)
{
	u32 dcli, dsz;
	bool upd;
	int i, sz;

	if(iswrite)
	{
		upd=false;
		dcli=BTESH2_TKFAT_GetDirEntCluster(dee);
		dsz=BTESH2_TKFAT_GetDirEntSize(dee);
		if(!dcli)
		{
			dcli=BTESH2_TKFAT_AllocFreeCluster(dee->img);
			BTESH2_TKFAT_SetDirEntCluster(dee, dcli);
			upd=true;
		}
		
		if((offs+size)>dsz)
		{
			dsz=offs+size;
			BTESH2_TKFAT_SetDirEntSize(dee, dsz);
			upd=true;
		}

		if(upd)
			BTESH2_TKFAT_UpdateDirEnt(dee);
		
		i=BTESH2_TKFAT_ReadWriteClusterOffset(dee->img,
			dcli, offs, 1, data, size);
		return(i);
	}

	dcli=BTESH2_TKFAT_GetDirEntCluster(dee);
	dsz=BTESH2_TKFAT_GetDirEntSize(dee);

	if(offs>=dsz)
		return(0);
	
	sz=size;
	if((offs+size)>dsz)
		{ sz=dsz-offs; }

	i=BTESH2_TKFAT_ReadWriteClusterOffset(dee->img,
		dcli, offs, 0, data, sz);
	if(i<0)
		return(i);
	return(sz);
}

int BTESH2_TKFAT_SetupDirEntNewDirectory(
	BTESH2_TKFAT_FAT_DirEntExt *dee)
{
	BTESH2_TKFAT_FAT_DirEnt tdeb;
	BTESH2_TKFAT_FAT_DirEnt *deb;
	u32 dcli, pcli;

	dcli=BTESH2_TKFAT_GetDirEntCluster(dee);
	if(dcli)
	{
		if(dee->deb.attrib&0x10)
			return(0);
		return(-1);
	}

	dee->deb.attrib|=0x10;
	if(!dcli)
	{
		dcli=BTESH2_TKFAT_AllocFreeCluster(dee->img);
		BTESH2_TKFAT_SetDirEntCluster(dee, dcli);
	}

	BTESH2_TKFAT_UpdateDirEnt(dee);
	
	deb=&tdeb;
	memset(deb, 0, sizeof(BTESH2_TKFAT_FAT_DirEnt));

	strcpy(deb->name, ".          ");
	deb->attrib|=0x10;
	btesh2_tkfat_setWord(deb->cluster_lo, dcli);
	btesh2_tkfat_setWord(deb->cluster_hi, dcli>>16);

	BTESH2_TKFAT_ReadWriteDirEntOffset(
		dee->img, dcli, 0, 1, deb);

	pcli=dee->clid;
	if(pcli<2)pcli=0;
	strcpy(deb->name, "..         ");
	deb->attrib|=0x10;
	btesh2_tkfat_setWord(deb->cluster_lo, pcli);
	btesh2_tkfat_setWord(deb->cluster_hi, pcli>>16);

	BTESH2_TKFAT_ReadWriteDirEntOffset(
		dee->img, dcli, 1, 1, deb);
	return(1);
}

//int TKFAT_WalkDirEntNext(TKFAT_ImageInfo *img,
//	TKFAT_FAT_DirEntExt *dee)
//{
//}

int TKFAT_ListDir(BTESH2_TKFAT_ImageInfo *img, int clid)
{
	BTESH2_TKFAT_FAT_DirEntExt tdee;
	BTESH2_TKFAT_FAT_DirEntExt *dee;
	int i, j;
	
	dee=&tdee;
	memset(dee, 0, sizeof(BTESH2_TKFAT_FAT_DirEntExt));
	
	dee->img=img;
	dee->clid=clid;
	dee->idx=-1;
	
	while(1)
	{
		i=BTESH2_TKFAT_WalkDirEntNext(img, dee);
		if(i<0)break;
		
		printf("%s\n", dee->de_name);
	}

	return(0);
}



char *BTESH2_BufGetLine(char *tbuf, char *str, char *stre)
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

char **BTESH2_SplitLine(char *buf)
{
	static char *ta[64];
	static char tb[512];
	char **a;
	char *s, *t, *t0;
	int i;
	
	for(i=0; i<64; i++)
		ta[i]=NULL;
	
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

byte *BTESH2_LoadFile(char *path, int *rsz)
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

int BTESH2_StoreFile(char *path, byte *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(path, "wb");
	if(!fd)
		return(-1);
	fwrite(buf, 1, sz, fd);
	fclose(fd);
	return(0);
}

int BTESH2_SetUseImage(char *name)
{
	BTESH2_TKFAT_ImageInfo *img;
	FILE *imgfd;
	char *imgfn;
	int imgsz;

	imgfn=strdup(name);
	imgfd=fopen(imgfn, "r+b");
	if(!imgfd)
	{
#if 0
		imgfd=fopen(imgfn, "w+b");
		if(imgfd)
		{
			memset(tb, 0, 1024);
			n=imgsz>>10;
			for(i=0; i<n; i++)
				fwrite(tb, 1, 1024, imgfd);
		}
#endif
	}
	
	if(!imgfd)
	{
		printf("BTESH2_SetUseImage: Fail Open %s\n", name);
		return(-1);
	}
	
	fseek(imgfd, 0, 2);
	imgsz=ftell(imgfd);
	fseek(imgfd, 0, 0);
	
	img=malloc(sizeof(BTESH2_TKFAT_ImageInfo));
	memset(img, 0, sizeof(BTESH2_TKFAT_ImageInfo));
	
//	img->pImgData=imgbuf;
	img->nImgBlks=imgsz;
	img->fdImgData=imgfd;
	
	spimmc_img=img;
	return(0);
}

int BTESH2_ProcessSDCL(
	byte *ibuf, int szibuf)
{
	char tb[1024];
	BTESH2_TKFAT_FAT_DirEntExt tdee;
	BTESH2_TKFAT_ImageInfo *img;
	FILE *imgfd;
	char *imgfn;
	char *fn1, *fn2;
	byte *imgbuf, *tbuf;
	char *cs, *cse;
	char **a;
	bool useimg, imgisnew;
	int imgsz, fsz, fty;
	int n;
	int i;

	imgfn=NULL;
	imgbuf=NULL;
	img=NULL;
	useimg=false;

	cs=ibuf; cse=cs+szibuf;
	while((cs<cse) && *cs)
	{
		cs=BTESH2_BufGetLine(tb, cs, cse);
		a=BTESH2_SplitLine(tb);
		
		if(!a[0])
			continue;
		if((a[0][0]=='#') || (a[0][0]==';') || (a[0][0]=='/'))
			continue;
		
		if(!strcmp(a[0], "mkimage"))
		{
			imgsz=atoi(a[2]);
			if(a[2][strlen(a[2])-1]=='k')
				imgsz*=2;
			if(a[2][strlen(a[2])-1]=='M')
				imgsz*=2*1024;
			
			fty=0;
			for(i=3; a[i]; i++)
			{
				if(!strcmp(a[i], "-F16"))
					fty=0x06;
				if(!strcmp(a[i], "-F32"))
					fty=0x0B;
			}
			
			printf("Make Image %dKiB\n", imgsz/2);
			
//			imgbuf=malloc(imgsz*512);
			imgfn=strdup(a[1]);
			imgfd=fopen(imgfn, "r+b");
			if(!imgfd)
			{
				imgfd=fopen(imgfn, "w+b");
				
				if(imgfd)
				{
					memset(tb, 0, 1024);
					n=imgsz>>10;
					for(i=0; i<n; i++)
						fwrite(tb, 1, 1024, imgfd);
				}
			}
			
			img=malloc(sizeof(BTESH2_TKFAT_ImageInfo));
			memset(img, 0, sizeof(BTESH2_TKFAT_ImageInfo));
			
			if(fty)
				img->fsty=fty;
			
//			img->pImgData=imgbuf;
			img->nImgBlks=imgsz;
			img->fdImgData=imgfd;
			
			BTESH2_TKFAT_SetupImageMBR(img);
			BTESH2_TKFAT_SetupImageFAT(img);
			
			continue;
		}

		if(!strcmp(a[0], "openimage"))
		{
			imgsz=atoi(a[2]);
			if(a[2][strlen(a[2])-1]=='k')
				imgsz*=2;
			if(a[2][strlen(a[2])-1]=='M')
				imgsz*=2*1024;
			
			fty=0;
			for(i=3; a[i]; i++)
			{
				if(!strcmp(a[i], "-F16"))
					fty=0x06;
				if(!strcmp(a[i], "-F32"))
					fty=0x0B;
			}
			
			printf("Open Image %dKiB\n", imgsz/2);
			
			imgisnew=false;
//			imgbuf=malloc(imgsz*512);
			imgfn=strdup(a[1]);
			imgfd=fopen(imgfn, "r+b");
			if(!imgfd)
			{
				imgfd=fopen(imgfn, "w+b");
				
				if(imgfd)
				{
					memset(tb, 0, 1024);
					n=imgsz>>10;
					for(i=0; i<n; i++)
						fwrite(tb, 1, 1024, imgfd);
					imgisnew=true;
				}
			}else
			{
				fseek(imgfd, 0, 2);
				imgsz=ftell(imgfd);
			}
			
			img=malloc(sizeof(BTESH2_TKFAT_ImageInfo));
			memset(img, 0, sizeof(BTESH2_TKFAT_ImageInfo));
			
//			img->pImgData=imgbuf;
			img->nImgBlks=imgsz;
			img->fdImgData=imgfd;
			
			if(imgisnew)
			{
				if(fty)
					img->fsty=fty;
				BTESH2_TKFAT_SetupImageMBR(img);
				BTESH2_TKFAT_SetupImageFAT(img);
			}else
			{
				BTESH2_TKFAT_ReadImageMBR(img);
				BTESH2_TKFAT_ReadImageFAT(img);
			}
			
			continue;
		}

		if(!strcmp(a[0], "addfile"))
		{
			fn1=NULL;
			fn2=NULL;
			
			for(i=1; a[i]; i++)
			{
				if(a[i][0]=='-')
				{
					printf("addfile: unknown option %s\n", a[i]);
					continue;
				}
				if(!fn1)
					{fn1=a[i]; continue; }
				if(!fn2)
					{fn2=a[i]; continue; }
			}
			
			if(!fn2)
				fn2=fn1;
			
//			fn1=a[1];
//			fn2=a[2];
			
			printf("Add File %s\n", fn1);

			tbuf=BTESH2_LoadFile(fn2, &fsz);
			if(!tbuf)
			{
				printf("Read %s fail\n", fn2);
				continue;
			}
			
			i=BTESH2_TKFAT_CreateDirEntPath(img, &tdee, fn1);
			if(i<0)
			{
				printf("Create %s fail\n", fn1);
				continue;
			}
			
			BTESH2_TKFAT_ReadWriteDirEntFile(&tdee, 0, true, tbuf, fsz);
			printf("OK\n");
			continue;
		}

		if(!strcmp(a[0], "addexport"))
		{
			fn1=NULL;	fn2=NULL;
			
			for(i=1; a[i]; i++)
			{
				if(a[i][0]=='-')
				{
					printf("addfile: unknown option %s\n", a[i]);
					continue;
				}
				if(!fn1)
					{fn1=a[i]; continue; }
				if(!fn2)
					{fn2=a[i]; continue; }
			}
			
			if(!fn2)
				fn2=fn1;
			
			i=img->exp_cnt++;
			img->exp_iname[i]=strdup(fn1);
			img->exp_ename[i]=strdup(fn2);
			continue;
		}
		
		if(!strcmp(a[0], "list"))
		{
			TKFAT_ListDir(img, img->clid_root);
			continue;
		}

		if(!strcmp(a[0], "useimage"))
		{
			spimmc_img=img;
			useimg=true;
			continue;
		}
	}

	BTESH2_TKFAT_FlushBuffers(img);
	
//	if(imgfn)
//	{
//		BTESH2_StoreFile(imgfn, imgbuf, imgsz*512);
//	}
	
	if(!useimg)
	{
		if(imgfd)
		{
			fclose(imgfd);
		}
		if(img)
		{
			free(img);
		}
//		if(imgbuf)
//		{
//			free(imgbuf);
//		}
	}
	return(0);
}

int BTESH2_TKFAT_SyncExports(void)
{
	BTESH2_TKFAT_FAT_DirEntExt tdee;
	BTESH2_TKFAT_ImageInfo *img;
	byte *tbuf;
	s64 tsz;
	int i, j, k;
	
	img=spimmc_img;
	if(!img)
		return(0);
	if(!img->exp_cnt)
		return(0);

	for(i=0; i<img->exp_cnt; i++)
	{
		j=BTESH2_TKFAT_LookupDirEntPath(img, &tdee, img->exp_iname[i]);
		if(j<0)
			{ continue; }
		
		tsz=BTESH2_TKFAT_GetDirEntSize(&tdee);
		if((tsz<0) || tsz>=(1LL<<31))
			continue;
		
		tbuf=malloc(tsz);
		
		BTESH2_TKFAT_ReadWriteDirEntFile(&tdee, 0, false, tbuf, tsz);
		BTESH2_StoreFile(img->exp_ename[i], tbuf, tsz);
		free(tbuf);
	}
	
	return(1);
}
