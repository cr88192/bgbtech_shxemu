s16 *btesh2_dspdev_vram;
int btesh2_dspdev_vramfl;

int btesh2_dspdev_rov;
int btesh2_dspdev_rate;
int btesh2_dspdev_lvol;
int btesh2_dspdev_rvol;

int SoundDev_WriteStereoSamples2(
	short *buffer, int cnt, int ovcnt);

double btesh2_ips;

u32 btesh2_dspdev_RegGetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr)
{
	int ix, ix1;
	u32 v;

	switch(reladdr)
	{
	case 0x0000:	v=btesh2_dspdev_rov; break;
	case 0x0004:	v=btesh2_dspdev_rate; break;
	case 0x0008:	v=btesh2_dspdev_lvol; break;
	case 0x000C:	v=btesh2_dspdev_rvol; break;

	case 0x0010:	v=FRGL_TimeMS(); break;
	case 0x0014:	v=cpu->tr_tops; break;
	case 0x0018:	v=cpu->tr_tops>>32; break;
	case 0x001C:	v=1024.0*(cpu->tr_tops/(cpu->tr_tdt*1000.0)); break;
//	case 0x001C:	v=btesh2_ips*(1024.0/1000000.0); break;

	default:  break;
	}
	return(v);
}

int btesh2_dspdev_RegSetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, btesh2_paddr reladdr, u32 val)
{
	int ix, ix1;

//	printf("btesh2_dspdev_RegSetD: %04X=%08X\n", reladdr, val);
	printf("btesh2_dspdev_RegSetD: %04llX=%08X\n", reladdr, val);

	switch(reladdr)
	{
	case 0x0000:	btesh2_dspdev_rov=val; break;
	case 0x0004:	btesh2_dspdev_rate=val; break;
	case 0x0008:	btesh2_dspdev_lvol=val; break;
	case 0x000C:	btesh2_dspdev_rvol=val; break;
	default:  break;
	}
	return(0);
}

int BTESH2_DspDev_Update(BTESH2_CpuState *cpu, float dt)
{
	short tsb[16384*2];
	int tl, tr;
	int ns, ns2, ns3;
	int rov, step;
	int i, j, k;
	
	if(!cpu || !btesh2_dspdev_vram)
		return(0);
	
	ns=btesh2_dspdev_rate*dt;
	ns2=44100*dt;
	if(!ns || !ns2)
		return(0);
	
	if(ns2>8192)
	{
		BTESH2_DspDev_Update(cpu, dt*0.5);
		BTESH2_DspDev_Update(cpu, dt*0.5);
		return(0);
	}
	
	ns3=1.75*ns2;
	
	step=(4096*btesh2_dspdev_rate)/44100;
	rov=btesh2_dspdev_rov<<12;
	for(i=0; i<ns3; i++)
	{
		j=(rov+2047)>>12;
		j=j&8191;
		rov+=step;
		
		tl=btesh2_dspdev_vram[0x0000|j];
		tr=btesh2_dspdev_vram[0x2000|j];
		
		tl=(tl*btesh2_dspdev_lvol)>>8;
		tr=(tr*btesh2_dspdev_rvol)>>8;
		
		tsb[i*2+0]=tl;
		tsb[i*2+1]=tr;
	}

	SoundDev_WriteStereoSamples2(tsb, ns2, ns3);

	rov=btesh2_dspdev_rov<<12;
	rov+=ns2*step;
	j=(rov+2047)>>12;
	btesh2_dspdev_rov=j&8191;

	return(0);
}

int BTESH2_DspDev_ImageRegister(BTESH2_MemoryImage *img)
{
	byte **rptr;
	
	rptr=(byte **)(&btesh2_dspdev_vram);
	BTESH2_MemoryDefineSpanDemand(img, 0x007F0000, 0x007F8000,
		rptr, &btesh2_dspdev_vramfl, "DSP_RAM");
	
	BTESH2_MemoryDefineSpanRegs(img, 0x007F8000, 0x007F9FFF, "DSP_REG",
		btesh2_dspdev_RegGetD, btesh2_dspdev_RegSetD);

	return(0);
}
