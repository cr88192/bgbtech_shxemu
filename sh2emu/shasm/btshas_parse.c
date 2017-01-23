
int BTSHASP_GetRegId(char *str)
{
	int i, t;
	
	switch(str[0])
	{
	case 'F':	case 'f':
	case 'D':	case 'd':
		t=BTSHAS_OPREG_FR0;
		if((str[0]=='D') || (str[0]=='d'))
			t=BTSHAS_OPREG_DR0;
		if(str[2]=='0')
		{
			if((str[3]>='0') && (str[3]<='9') && !str[4])
				{ return(t+(str[3]-'0')); }
			if(!str[3])
			return(t);
		}else if(str[2]=='1')
		{
			if((str[3]>='0') && (str[3]<='5') && !str[4])
				{ return(t+10+(str[3]-'0')); }
			if(!str[3])
				return(t+1);
		}else if((str[2]>='0') && (str[2]<='9') && !str[3])
			{ return(t+(str[2]-'0')); }
		if(!btshasp_stricmp(str, "fpscr"))
			return(BTSHAS_OPREG_FPSCR);
		if(!btshasp_stricmp(str, "fpul"))
			return(BTSHAS_OPREG_FPUL);
		break;
	
	case 'R':	case 'r':
		t=BTSHAS_OPREG_R0;
		if(str[1]=='0')
		{
			if((str[2]>='0') && (str[2]<='9') && !str[3])
				{ return(t+(str[2]-'0')); }
			if(!str[2])
				return(t);
		}
		if(str[1]=='1')
		{
			if((str[2]>='0') && (str[2]<='5') && !str[3])
				{ return(t+10+(str[2]-'0')); }
			if(!str[2])
				{ return(t+1); }
		}else if((str[1]>='0') && (str[1]<='9') && !str[2])
			{ return(t+(str[1]-'0')); }
		break;

	case 'M':	case 'm':
		if(!btshasp_stricmp(str, "mach"))
			return(BTSHAS_OPREG_MACH);
		if(!btshasp_stricmp(str, "macl"))
			return(BTSHAS_OPREG_MACL);
		break;
	case 'P':	case 'p':
		if(!btshasp_stricmp(str, "pr"))
			return(BTSHAS_OPREG_PR);
		if(!btshasp_stricmp(str, "pc"))
			return(BTSHAS_OPREG_PC);
		break;

	default:
		if(!btshasp_stricmp(str, "sp"))
			return(BTSHAS_OPREG_SP);

		if(!btshasp_stricmp(str, "sr"))
			return(BTSHAS_OPREG_SR);
		if(!btshasp_stricmp(str, "gbr"))
			return(BTSHAS_OPREG_GBR);
		if(!btshasp_stricmp(str, "vbr"))
			return(BTSHAS_OPREG_VBR);
		if(!btshasp_stricmp(str, "tbr"))
			return(BTSHAS_OPREG_TBR);
		break;
	}
	
	return(-1);
}

int BTSHASP_ParseOperand(char **rcs, BTSHAS_OprVal *opv)
{
	char *tk0, *tk1;
	char *cs, *cs1, *cs2;
	int i;
	
	cs=*rcs;
	cs=BTSHASP_EatWhiteNoLinebreak(cs);
	if((*cs=='\r') || (*cs=='\n'))
	{
		cs=BTSHASP_EatWhite(cs);
		*rcs=cs;
		return(0);
	}
	
	if(*cs==';')
	{
		cs++;
		cs=BTSHASP_EatWhite(cs);
		*rcs=cs;
		return(0);
	}
	
	if(*cs=='#')
	{
		cs++;
		cs1=BTSHASP_ParseTokenAlt(cs, &tk0);

		opv->ty=BTSHAS_OPVTY_IMM;
		opv->imm=btshas_atol(tk0+1);
		*rcs=cs1;
		return(1);
	}

	if(*cs=='@')
	{
		cs++;
		while(*cs==' ')cs++;
		
		if(*cs=='-')
		{
			cs++;

			cs1=BTSHASP_ParseToken(cs, &tk0);

			if(*tk0=='I')
			{
				opv->ty=BTSHAS_OPVTY_DRREGD;
				opv->rb=BTSHASP_GetRegId(tk0+1);
				*rcs=cs1;
				return(1);
			}
			return(-1);
		}
		
		if(*cs=='(')
		{
			cs++;
			cs=BTSHASP_ParseToken(cs, &tk0);
			tk1=NULL;
			cs=BTSHASP_EatWhiteNoLinebreak(cs);
			if(*cs==',')
			{
				cs++;
				cs=BTSHASP_ParseToken(cs, &tk1);
			}
			cs=BTSHASP_EatWhiteNoLinebreak(cs);
			if(*cs==')')
				cs++;

			if(tk1)
			{
				if((*tk0=='|') && (*tk1=='I'))
				{
					opv->ty=BTSHAS_OPVTY_RDMEM;
					opv->rb=BTSHASP_GetRegId(tk1+1);
					opv->imm=btshas_atol(tk0+1);
					*rcs=cs;
					return(1);
				}

				if(*tk1=='I')
				{
					opv->ty=BTSHAS_OPVTY_RRMEM;
//					opv->rb=BTSHASP_GetRegId(tk0+1);
//					opv->ri=BTSHASP_GetRegId(tk1+1);

					opv->ri=BTSHASP_GetRegId(tk0+1);
					opv->rb=BTSHASP_GetRegId(tk1+1);
					*rcs=cs;
					return(1);
				}
				if(*tk1=='|')
				{
					opv->ty=BTSHAS_OPVTY_RDMEM;
					opv->rb=BTSHASP_GetRegId(tk0+1);
					opv->imm=btshas_atol(tk1+1);
					*rcs=cs;
					return(1);
				}
				return(-1);
			}

			opv->ty=BTSHAS_OPVTY_DRREG;
			opv->rb=BTSHASP_GetRegId(tk0+1);
			if(*cs=='+')
			{
				cs++;
				if(*cs==')')
					cs++;
				opv->ty=BTSHAS_OPVTY_DRREGI;
			}
			*rcs=cs;
			return(1);
		}
		
		cs1=BTSHASP_ParseToken(cs, &tk0);

		if(*tk0=='I')
		{
			opv->ty=BTSHAS_OPVTY_DRREG;
			opv->rb=BTSHASP_GetRegId(tk0+1);
			if(*cs1=='+')
			{
				cs1++;
				opv->ty=BTSHAS_OPVTY_DRREGI;
			}
			*rcs=cs1;
			return(1);
		}
		
		opv->ty=BTSHAS_OPVTY_IMM;
		opv->imm=btshas_atol(tk0+1);
		*rcs=cs1;
		return(1);
	}
	
	cs1=BTSHASP_ParseTokenAlt(cs, &tk0);
	cs2=BTSHASP_ParseToken(cs1, &tk1);

	if(*tk0=='|')
	{
		opv->ty=BTSHAS_OPVTY_IMM;
		opv->imm=btshas_atol(tk0+1);
		*rcs=cs1;
		return(1);
	}
	
	if(*tk0=='I')
	{
		i=BTSHASP_GetRegId(tk0+1);
		if(i>=0)
		{
			opv->ty=BTSHAS_OPVTY_REG;
			opv->rb=i;
			*rcs=cs1;
			return(1);
		}

		opv->ty=BTSHAS_OPVTY_NAME;
		if(strlen(tk0+1)<8)
		{
			opv->name=(char *)(&opv->imm);
			strncpy(opv->name, tk0+1, 7);
		}else
		{
			opv->name=btshas_strdup(tk0+1);
		}
		*rcs=cs1;
		return(1);
	}

	return(0);
}

int BTSHASP_ParseOpcode(BTSHAS_Context *ctx, char **rcs)
{
	BTSHAS_OprVal arg[4];
	BTSHAS_ListingOp *op;
	char *tk0, *tk1;
	char *cs, *cs1, *cs2;
	int i, j, k;
	
	cs=*rcs;
	cs1=BTSHASP_ParseTokenAlt(cs, &tk0);

	if(*tk0=='I')
	{
		cs1=BTSHASP_EatWhiteOnlyNoLinebreak(cs1);
		if(*cs1==':')
		{
			BTSHAS_EmitLabel(ctx, tk0+1);
			*rcs=cs1+1;
			return(1);
		}
		
		cs2=cs1;
		arg[0].ty=0; arg[1].ty=0; arg[2].ty=0;
		for(i=0; i<3; i++)
		{
			if(BTSHASP_ParseOperand(&cs2, &arg[i])<=0)
				break;
			cs2=BTSHASP_EatWhiteNoLinebreak(cs2);
			if(*cs2==',')
				cs2++;
		}
//		if(BTSHASP_ParseOperand(&cs2, arg[0])>0)
//			if(BTSHASP_ParseOperand(&cs2, arg[1])>0)
//				if(BTSHASP_ParseOperand(&cs2, arg[2])>0)
//		{
//		}
		
		op=BTSHAS_SHxOpLookup(tk0+1, &arg[0], &arg[1], &arg[2]);
		if(op)
		{
			BTSHAS_EmitListOp(ctx, op, &arg[0], &arg[1], &arg[2]);
			*rcs=cs2;
			return(1);
		}
		
		if(!strcmp(tk0, "I.align"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseToken(cs2, &tk0);
			BTSHAS_EmitAlign(ctx, btshas_atol(tk0+1)*2);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.long"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
			cs2=BTSHASP_EatWhiteNoLinebreak(cs2);
			
			if(*tk0=='I')
			{
				if(*cs2=='@')
				{
					while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
						cs2++;
				}
			
				k=0;
				if(*cs2=='-')
				{
					cs2++;
					cs2=BTSHASP_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						BTSHAS_EmitReloc(ctx, tk0+1,
							BTSHAS_LBLRLC_SH_REL32);
						BTSHAS_EmitReloc(ctx, tk1+1,
							BTSHAS_LBLRLC_SH_REL32B);
					}else if(tk1[0]=='|')
					{
						BTSHAS_EmitReloc(ctx, tk0+1,
							BTSHAS_LBLRLC_SH_DIR32);
						k=-btshas_atol(tk1+1);
					}
				}else if(*cs2=='+')
				{
					cs2++;
					cs2=BTSHASP_ParseTokenAlt(cs2, &tk1);
					if(tk1[0]=='I')
					{
						BTSHAS_EmitReloc(ctx, tk0+1,
							BTSHAS_LBLRLC_SH_REL32);
						BTSHAS_EmitReloc(ctx, tk1+1,
							BTSHAS_LBLRLC_SH_REL32);
					}else if(tk1[0]=='|')
					{
						BTSHAS_EmitReloc(ctx, tk0+1,
							BTSHAS_LBLRLC_SH_DIR32);
						k=btshas_atol(tk1+1);
					}
				}else
				{
					BTSHAS_EmitReloc(ctx, tk0+1,
						BTSHAS_LBLRLC_SH_DIR32);
				}
				
				BTSHAS_EmitDWord(ctx, k);
				*rcs=cs2;
				return(1);
			}
			
			BTSHAS_EmitDWord(ctx, btshas_atol(tk0+1));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
				BTSHAS_EmitDWord(ctx, btshas_atol(tk0+1));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.word") ||
			!strcmp(tk0, "I.short"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
			BTSHAS_EmitDWord(ctx, btshas_atol(tk0+1));
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
				BTSHAS_EmitWord(ctx, btshas_atol(tk0+1));
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.byte"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseToken(cs2, &tk0);
			if(*tk0=='I')
				i=btshas_atol(tk0+1);
			if(*tk0=='C')
				i=tk0[1];
			BTSHAS_EmitByte(ctx, i);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BTSHASP_ParseToken(cs2, &tk0);
				if(*tk0=='I')
					i=btshas_atol(tk0+1);
				if(*tk0=='C')
					i=tk0[1];
				BTSHAS_EmitByte(ctx, i);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.string"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseToken(cs2, &tk0);
			BTSHAS_EmitString(ctx, tk0+1);
			while(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BTSHASP_ParseToken(cs2, &tk0);
				BTSHAS_EmitString(ctx, tk0+1);
			}

			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.section"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
			BTSHAS_EmitSection(ctx, tk0+1);
			
			if(*cs2==',')
			{
				while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
					cs2++;
			}
			
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.text") ||
			!strcmp(tk0, "I.data") ||
			!strcmp(tk0, "I.bss"))
		{
			BTSHAS_EmitSection(ctx, tk0+1);
			*rcs=cs1;
			return(1);
		}

		if(!strcmp(tk0, "I.global") ||
			!strcmp(tk0, "I.extern") ||
			!strcmp(tk0, "I.weak"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
			BTSHAS_EmitGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.local"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
			BTSHAS_EmitGlobal(ctx, tk0+1);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.rel32"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);

			BTSHAS_EmitReloc(ctx, tk0+1,
				BTSHAS_LBLRLC_SH_REL32);

			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);
				BTSHAS_EmitReloc(ctx, tk0+1,
					BTSHAS_LBLRLC_SH_REL32B);
			}

			BTSHAS_EmitDWord(ctx, 0);
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.comm"))
		{
			cs2=cs1;
			cs2=BTSHASP_ParseTokenAlt(cs2, &tk0);

			j=1; k=1;
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BTSHASP_ParseTokenAlt(cs2, &tk1);
				j=atoi(tk1+1);
			}
			if(*cs2 && *cs2==',')
			{
				if(*cs2==',')
					cs2++;
				cs2=BTSHASP_ParseTokenAlt(cs2, &tk1);
				k=atoi(tk1+1);
			}
			
			j=j*k;

			BTSHAS_EmitCommSym(ctx, tk0+1, j);

//			i=ctx->cseg;
//			BTSHAS_EmitSection(ctx, ".bss");
//			BTSHAS_EmitLabel(ctx, tk0+1);
//			BTSHAS_EmitRawBytes(ctx, NULL, j);
//			ctx->cseg=i;
			*rcs=cs2;
			return(1);
		}

		if(!strcmp(tk0, "I.type") ||
			!strcmp(tk0, "I.size") ||
			!strcmp(tk0, "I.file") ||
			!strcmp(tk0, "I.end") ||
			!strcmp(tk0, "I.ident"))
		{
			cs2=cs1;
			while(*cs2 && (*cs2!='\r') && (*cs2!='\n'))
				cs2++;
			*rcs=cs2;
			return(1);
		}

		printf("unexpected token '%s'\n", tk0+1);
		*rcs=cs;
		return(-1);
	}

	printf("unexpected token '%s'\n", tk0+1);
	return(-1);
}

int BTSHASP_ParseBuffer(BTSHAS_Context *ctx, char **rcs)
{
	char tb[256];
	byte *cs, *t;
	int i;
	
	cs=*rcs;
	while(*cs)
	{
		cs=BTSHASP_EatWhite(cs);
		if(!(*cs))
			break;
		i=BTSHASP_ParseOpcode(ctx, &cs);
		if(i<0)
		{
			t=tb;
			while(*cs && (*cs!='\r') && (*cs!='\n'))
				*t++=*cs++;
			*t++=0;
			printf("skip line \"%s\"\n", tb);
		}
	}

	*rcs=cs;
	return(0);
}

BTSHAS_Context *BTSHASP_AllocContext(void)
{
	BTSHAS_Context *ctx;
	
	ctx=btshas_mm_malloc(sizeof(BTSHAS_Context));
	
	ctx->lbl=ctx->t_lbl;
	ctx->mlbl=BTSHAS_CTX_IMAXLBL;

	ctx->rlc=ctx->t_rlc;
	ctx->mrlc=BTSHAS_CTX_IMAXRLC;
	
	return(ctx);
}
