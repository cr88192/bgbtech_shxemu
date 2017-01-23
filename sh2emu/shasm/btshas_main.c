#include "btshas_multi.c"

char *testbuf=
".section .text\n"
"test1:\n"
"mov r1, r2\n"
"mov #4, r3\n"

"__ashrsi3_r0:\n"
"	mov.l r4, @-sp\n"
"	not r0, r0\n"
"	and	#31,r0\n"
"	shal r0\n"
"	mov r0, r4\n"
"	mova shar_list, r0\n"
"	add r0, r4\n"
"	mov.l @sp+, r0\n"
"	jmp @r4\n"
"	nop\n"
".align 2\n"
"shar_list:\n"
"	shar	r0	/* 31 */\n"
"	shar	r0	/* 30 */\n"
"	shar	r0	/* 29 */\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0	/* 25 */\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0	/* 21 */\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0	/* 17 */\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0	/* 13 */\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0	/* 9 */\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0	/* 5 */\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0\n"
"	shar	r0	/* 1 */\n"
"	rts\n"
"	nop\n"
;

int main(int argc, char *argv[])
{
	char *ifna[256];
	BTSHAS_Context *ctx;
	byte *ibuf, *tbuf, *obuf;
	char *ifn, *ofn, *ife;
	char *cs;
	int i, j, k, sz, sz1, nifn;
	int ofmt, en;

//	ifn=NULL;
	en=1; ofmt=0;
	nifn=0; ofn=NULL;
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-o"))
				{ ofn=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "-D"))
				{ BTSHASP_AddStaticArgDefine(NULL, argv[i]+2); continue; }

			if(!strcmp(argv[i], "-I"))
				{ BTSHASP_AddIncludePathFront(argv[i]+2); continue; }

			if(!strcmp(argv[i], "-EL"))
				{ en=1; continue; }
			if(!strcmp(argv[i], "-EB"))
				{ en=0; continue; }

			if(!strcmp(argv[i], "-OfPE"))
				{ ofmt=1; continue; }
			if(!strcmp(argv[i], "-OfLZPE"))
				{ ofmt=2; continue; }

			continue;
		}
		
//		if(!ifn)
//			ifn=argv[i];
		ifna[nifn++]=argv[i];
	}
		
	ctx=BTSHASP_AllocContext();
	ctx->is_le=en;
	
	BTSHAS_EmitSection(ctx, ".text");
	
//	cs=testbuf;
//	cs=ibuf;
//	if(!ibuf)
//		cs=testbuf;
//	ibuf=BTSHAS_LoadFile(ifn, &sz);
//	BTSHASP_ParseBuffer(ctx, &cs);

	for(i=0; i<nifn; i++)
	{
		ifn=ifna[i];
		ibuf=BTSHAS_LoadFile(ifn, &sz);
		if(!ibuf)
		{
			continue;
		}

		ife=btshas_getext(ifn);

		if(!btshasp_stricmp(ife, "s") ||
			!btshasp_stricmp(ife, "as") ||
			!btshasp_stricmp(ife, "asm"))
		{
			cs=ibuf;
			
			tbuf=NULL;
			if(!strcmp(ife, "S"))
			{
				tbuf=btshas_mm_malloc(1<<20);
				BTSHASP_Filter(ctx, ibuf, tbuf, 1<<20);
				cs=tbuf;
			}
			
			BTSHASP_ParseBuffer(ctx, &cs);
			btshas_mm_free(ibuf);
			if(tbuf)
				btshas_mm_free(tbuf);
			continue;
		}

		if(!btshasp_stricmp(ife, "o") ||
			!btshasp_stricmp(ife, "obj"))
		{
			BTSHAS_LoadBufferCOFF(ctx, ibuf, sz);
			btshas_mm_free(ibuf);
			continue;
		}
	}

//	BTSHAS_FixupLocal(ctx);
	BTSHAS_LoadRebind(ctx);
	
	for(i=0; i<ctx->nseg; i++)
	{
		printf("%s\n", ctx->seg[i]->name);
		j=ctx->seg[i]->ct-ctx->seg[i]->cts;
		BTSHAS_PrintHex(ctx->seg[i]->cts, j, 0);
	}
	
	obuf=malloc(1<<20);
	
	if(ofmt==2)
	{
		tbuf=btshas_mm_malloc(1<<20);
		sz1=BTSHAS_FlattenPECOFF(ctx, tbuf, 1<<20);
		sz=btshas_blzst0_CompressBasic(tbuf, obuf, sz1, 1<<20);
	}else
		if(ofmt==1)
	{
		sz=BTSHAS_FlattenPECOFF(ctx, obuf, 1<<20);
	}else
	{
		sz=BTSHAS_FlattenCOFF(ctx, obuf, 1<<20);
	}

	printf("obuf:\n");
	BTSHAS_PrintHex(obuf, sz, 0);
	
	if(ofn)
	{
		BTSHAS_StoreFile(ofn, obuf, sz);
	}
}
