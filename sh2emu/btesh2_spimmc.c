byte spimmc_imsgbuf[4096];
byte spimmc_omsgbuf[4096];
int spimmc_imsgrov, spimmc_imsgirov;
int spimmc_omsgrov, spimmc_omsgirov;

byte *spimmc_imgbuf=NULL;
int spimmc_szimgbuf;

int spimmc_rmultilba=-1;

int BTESH2_SPIMMC_SetImage(byte *buf, int sz)
{
	spimmc_imgbuf=buf;
	spimmc_szimgbuf=sz;
	return(0);
}

int btesh2_spimmc_SendByte(BTESH2_CpuState *cpu, int val)
{
	int ob;

	ob=spimmc_omsgrov;
	spimmc_omsgrov=(spimmc_omsgrov+1)&4095;
	spimmc_omsgbuf[ob]=val;
}

int btesh2_spimmc_XrCtl(BTESH2_CpuState *cpu, int val)
{
	static int init=0;
	int ni;
	
	ni=0;
	if(!init)
	{
		init=1;
		ni=1;
	}
	
//	printf("C%02X ", val);
//	val=val&(~0x02);
//	return(val|0xFF00);
	if(ni)
		return(0x1FF00);
	return(0);
}

int btesh2_spimmc_XrByte(BTESH2_CpuState *cpu, int val)
{
	int ib, ob, v, n;
	int i0, i1, i2, i3;
	int op, lba, offs;
	int i;
	
	if((spimmc_imsgrov!=spimmc_imsgirov) || (val!=0xFF))
	{
		ib=spimmc_imsgrov;
		spimmc_imsgrov=(spimmc_imsgrov+1)&4095;
		spimmc_imsgbuf[ib]=val;
	}

//	if(val!=0xFF)
//		printf("D%02X ", val);
	
	n=((spimmc_imsgrov|4096)-spimmc_imsgirov)&4095;
	if(n>=3)
	{
		ib=spimmc_imsgirov;
		op=spimmc_imsgbuf[ib];

		if(op==0xFF)
		{
			ib=spimmc_imsgirov;
			while(ib!=spimmc_imsgrov)
			{
				op=spimmc_imsgbuf[ib];
				if(op!=0xFF)
					break;
				ib=(ib+1)&4095;
			}
			spimmc_imsgirov=ib;
		}
	}

	n=((spimmc_imsgrov|4096)-spimmc_imsgirov)&4095;
	if(n>=6)
	{
		ib=spimmc_imsgirov;
		op=spimmc_imsgbuf[ib];

#if 0
		if(op==0xFF)
		{
			ib=spimmc_imsgirov;
			while(ib!=spimmc_imsgrov)
			{
				op=spimmc_imsgbuf[ib];
				if(op!=0xFF)
					break;
				ib=(ib+1)&4095;
			}
			spimmc_imsgirov=ib;
		}
#endif

		if((op&63)==0)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("GO_IDLE_STATE %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x01);
		}else
			if((op&63)==8)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("SEND_IF_COND %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);

//			btesh2_spimmc_SendByte(cpu, 0x00);
//			btesh2_spimmc_SendByte(cpu, 0x00);
//			btesh2_spimmc_SendByte(cpu, 0x01);
//			btesh2_spimmc_SendByte(cpu, 0xAA);
		}else
			if((op&63)==16)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("SET_BLOCKLEN %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);
		}else
			if((op&63)==17)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("Read Block %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);
			btesh2_spimmc_SendByte(cpu, 0xFE);
			
			offs=lba<<9;
			if(!spimmc_imgbuf || ((offs+512)>spimmc_szimgbuf))
			{
				for(i=0; i<512; i++)
				{
//					btesh2_spimmc_SendByte(cpu, spimmc_imgbuf[offs+i]);
					btesh2_spimmc_SendByte(cpu, i);
				}
			}else
			{
				for(i=0; i<512; i++)
					{ btesh2_spimmc_SendByte(cpu, spimmc_imgbuf[offs+i]); }
			}
		}else

			if((op&63)==18)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("Read Multi %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);
			btesh2_spimmc_SendByte(cpu, 0xFE);
			
			offs=lba<<9;
			if(!spimmc_imgbuf || ((offs+512)>spimmc_szimgbuf))
			{
				for(i=0; i<512; i++)
				{
//					btesh2_spimmc_SendByte(cpu, spimmc_imgbuf[offs+i]);
					btesh2_spimmc_SendByte(cpu, i);
				}
			}else
			{
				for(i=0; i<512; i++)
					{ btesh2_spimmc_SendByte(cpu, spimmc_imgbuf[offs+i]); }
			}
			
			spimmc_rmultilba=lba+1;
		}else

			if((op&63)==55)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("APP_CMD %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);
		}else

			if((op&63)==41)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("SEND_OP_COND %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);
		}else

			if((op&63)==52)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("SEND_OP_COND %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);
		}else

			if((op&63)==58)
		{
			i0=spimmc_imsgbuf[(ib+1)&4095];
			i1=spimmc_imsgbuf[(ib+2)&4095];
			i2=spimmc_imsgbuf[(ib+3)&4095];
			i3=spimmc_imsgbuf[(ib+4)&4095];
			lba=(i0<<24)|(i1<<16)|(i2<<8)|i3;
			
			spimmc_imsgirov=(ib+6)&4095;
			
//			printf("READ_OCR %08X\n", lba);

			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			btesh2_spimmc_SendByte(cpu, 0xFF);
			
			btesh2_spimmc_SendByte(cpu, 0x00);

			btesh2_spimmc_SendByte(cpu, 0x40);
			btesh2_spimmc_SendByte(cpu, 0x00);
			btesh2_spimmc_SendByte(cpu, 0x00);
			btesh2_spimmc_SendByte(cpu, 0x00);
		}else
		{
			printf("Unknown %02X %d\n", op, op&63);
		}
	}

	if((spimmc_omsgirov==spimmc_omsgrov) && (spimmc_rmultilba>=0))
	{
		lba=spimmc_rmultilba++;
		btesh2_spimmc_SendByte(cpu, 0xFE);
		
		offs=lba<<9;
		if(!spimmc_imgbuf || ((offs+512)>spimmc_szimgbuf))
		{
			for(i=0; i<512; i++)
				{ btesh2_spimmc_SendByte(cpu, i); }
		}else
		{
			for(i=0; i<512; i++)
				{ btesh2_spimmc_SendByte(cpu, spimmc_imgbuf[offs+i]); }
		}
	}
	
	if(spimmc_omsgirov!=spimmc_omsgrov)
	{
		ob=spimmc_omsgirov;
		spimmc_omsgirov=(spimmc_omsgirov+1)&4095;
		v=spimmc_omsgbuf[ob];
		
//		printf("R%02X ", v);
		
		return(v);
	}
	return(0xFF);
}
