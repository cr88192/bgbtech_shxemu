// #include <bgbccc.h>

#define BTK_NAME		'I'
#define BTK_NUMBER		'|'
#define BTK_STRING		'S'
#define BTK_CHARSTRING	'C'
#define BTK_OPERATOR	'X'

char *btshas_ibuf;
char *btshas_obuf;
char *btshas_obufe;

BTSHAS_PpDef *btshas_sdef;	//static defines

int btshas_warn, btshas_err;

int btshas_iflvl;	//nested ifdef-level (true branches)
int btshas_iflvl2;	//nested ifdef-level (false branch)
int btshas_ifmark;	//set if 'true' branch already taken
int btshas_ifflag[256];	//ifdef level flags

char *btshas_ifdname[256];	//ifdef level flags
char *btshas_ifdname2[256];	//ifdef level flags

char *btshas_spos;	//temp source position
char *btshas_lfn;	//last file name

char *btshas_cfn;	//current file name
int btshas_cln;	//current file name


//includes
char *btshas_sinc[256];	//static include paths
int btshas_nsinc=0;	//num static include paths

char *btshas_inc[256];	//include paths
int btshas_ninc;	//num include paths

int btshas_noinclude_p=0;	//_noinclude set

char *btshas_cache_incname[4096];
byte *btshas_cache_incbuf[4096];
int btshas_cache_incsz[4096];
int btshas_cache_ninc=0;

BTSHAS_PpDef *btshas_freedef=NULL;

char *BTSHASP_PrintToken(char *t, char *s, int ty);

int BTSHASP_CheckIncludeHashName(char *str)
{
	int h;
	char *s;
	
	s=str; h=0;
	while(*s)h=(h*251)+(*s++);
	h=h*251;
	return((h>>8)&4095);
}

int BTSHASP_CheckIncludeHashNext(int hash)
{
	return(((hash*251)>>8)&4095);
}

int BTSHASP_CheckIncludeCache(char *str, byte **rbuf, int *rsz)
{
	int hi, n;
	
	hi=BTSHASP_CheckIncludeHashName(str); n=64;
	while(btshas_cache_incname[hi] && ((n--)>0))
	{
		if(!strcmp(btshas_cache_incname[hi], str))
		{
			if(rbuf)*rbuf=btshas_cache_incbuf[hi];
			if(rsz)*rsz=btshas_cache_incsz[hi];
			return(1);
		}
		hi=BTSHASP_CheckIncludeHashNext(hi);
	}

	if(rbuf)*rbuf=NULL;
	if(rsz)*rsz=0;
	return(0);
}

int BTSHASP_AddIncludeCache(char *str, byte *buf, int sz)
{
	int hi, n;
	
	hi=BTSHASP_CheckIncludeHashName(str); n=64;
	while(btshas_cache_incname[hi] && ((n--)>0))
	{
		if(!strcmp(btshas_cache_incname[hi], str))
		{
			btshas_cache_incbuf[hi]=buf;
			btshas_cache_incsz[hi]=sz;
			return(1);
		}
		hi=BTSHASP_CheckIncludeHashNext(hi);
	}
	
	if(n>0)
	{
		btshas_cache_incname[hi]=btshas_strdup(str);
		btshas_cache_incbuf[hi]=buf;
		btshas_cache_incsz[hi]=sz;
		return(1);
	}
	
	return(0);
}

int BTSHASP_AddIncludePathFront(char *str)
{
	int i;

	if(!str)return(-1);

	for(i=0; i<btshas_nsinc; i++)
		if(!strcmp(btshas_sinc[i], str))
			return(i);
	i=btshas_nsinc++;
	btshas_sinc[i]=strdup(str);
	return(i);
}

int BTSHASP_AddIncludePathBack(char *str)
{
	int i;

	if(!str)return(-1);

	for(i=0; i<btshas_nsinc; i++)
		if(!strcmp(btshas_sinc[i], str))
			return(i);

	for(i=btshas_nsinc; i>0; i++)
		btshas_sinc[i]=btshas_sinc[i-1];
	btshas_nsinc++;
	btshas_sinc[0]=strdup(str);
	return(i);
}

int BTSHASP_ParseChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(i>=0xF0)
	{
		i=(i&7)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xE0)
	{
		i=(i&15)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xC0)
	{
		i=(i&31)<<6;
		i+=(*s++)&63;
	}else if(i>=0x80)
	{
//		continue;
	}

	*str=(char *)s;
	return(i);
}

int BTSHASP_EmitChar(char **str, int j)
{
	unsigned char *t;
	int i;

	t=(unsigned char *)(*str);

	if(j>0x3FFFFFF)
	{
		*t++=0xFC+(j>>30);
		*t++=0x80+((j>>24)&0x3F);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x1FFFFF)
	{
		*t++=0xF8+(j>>24);
		*t++=0x80+((j>>18)&0x3F);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0xFFFF)
	{
		*t++=0xF0+(j>>18);
		*t++=0x80+((j>>12)&0x3F);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if(j>0x7FF)
	{
		*t++=0xE0+(j>>12);
		*t++=0x80+((j>>6)&0x3F);
		*t++=0x80+(j&0x3F);
	}else if((j>0x7F) || !j)
	{
		*t++=0xC0+(j>>6);
		*t++=0x80+(j&0x3F);
	}else
	{
		if(!j) //Modified-UTF8
			{ *t++=0xC0; *t++=0x80; }
			else *t++=j;
	}

	*str=(char *)t;
	return(0);
}

char *BTSHASP_EmitString(char *t, char *s)
{
	int i;

	if((*(t-1)=='\"') || (*(t-1)=='\''))
		*t++=' ';

	*t++='\"';
	while(*s)
	{
		i=BTSHASP_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		BTSHASP_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(t);
}

char *BTSHASP_EmitCharString(char *t, char *s)
{
	int i;

	if((*(t-1)=='\"') || (*(t-1)=='\''))
		*t++=' ';

	*t++='\'';
	while(*s)
	{
		i=BTSHASP_ParseChar(&s);

		if((i<' ') || (i>='~'))
		{
			if(i>0xFF)
			{
				sprintf(t, "\\u%04X", i); t+=6;
				continue;
			}

			switch(i)
			{
			case '\r': *t++='\\'; *t++='r'; break;
			case '\n': *t++='\\'; *t++='n'; break;
			case '\t': *t++='\\'; *t++='t'; break;
			case '\x1B': *t++='\\'; *t++='e'; break;
			default: sprintf(t, "\\x%02X", i); t+=4; break;
			}
			continue;
		}

		if(i=='\"')*t++='\\';
		if(i=='\'')*t++='\\';
		if(i=='\\')*t++='\\';
		BTSHASP_EmitChar(&t, i);
//		*t++=i;
	}

	*t++='\'';
	*t=0;

	return(t);
}

void BTSHASP_UniEscape(BTSHAS_Context *ctx, char *str)
{
	char *s, *t;
	int i, j, k;

	s=str; t=str;
	while(*s)
	{
		if((s[0]=='\\') && ((s[1]=='u') || (s[1]=='U')))
		{
			k=2;
			if(s[1]=='u')k=4;
			if(s[1]=='U')k=8;

			s+=2; j=0;
			while(k--)
			{
				j<<=4;
				if((*s>='0') && (*s<='9'))j+=*s-'0';
				if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
				if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
				s++;
			}
			BTSHASP_EmitChar(&t, j);
			continue;
		}
		
		*t++=*s++;
		continue;
	}
	*t++=0;
}

int BTSHASP_Error(BTSHAS_Context *ctx, char *str, ...)
{
	char tb[1024];
	va_list lst;
	int l;
	
	va_start(lst, str);
	vsprintf(tb, str, lst);
	va_end(lst);
	
	l=BTSHASP_CalcLinenum(btshas_spos);
	printf("pp-error: %s:%d: %s\n", btshas_lfn, l, tb);
	btshas_err++;

	return(0);
}

BTSHAS_PpDef *BTSHASP_AllocDefine(BTSHAS_Context *ctx)
{
	BTSHAS_PpDef *tmp;
	
#if 1
	if(btshas_freedef)
	{
		tmp=btshas_freedef;
		btshas_freedef=tmp->next;
		
		memset(tmp, 0, sizeof(BTSHAS_PpDef));
		return(tmp);
	}
#endif

	tmp=btshas_mm_malloc(sizeof(BTSHAS_PpDef));
	memset(tmp, 0, sizeof(BTSHAS_PpDef));
	return(tmp);
}

void BTSHASP_FreeDefine(BTSHAS_Context *ctx, BTSHAS_PpDef *def)
{
	def->next=btshas_freedef;
	btshas_freedef=def;
}

BTSHAS_PpDef *BTSHASP_LookupDefine(BTSHAS_Context *ctx, char *name)
{
	char *args[64];
	char buf[4096];
	BTSHAS_PpDef *cur;
	char *s;
	int i;

	s=name; i=0; while(*s)i=(i*251)+(*s++);
	i&=255;

	cur=ctx->ppdef[i];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	cur=btshas_sdef;
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->next;
	}

	if(!btshas_noinclude_p)
		return(NULL);

	return(NULL);
}

void BTSHASP_DeleteDefine(BTSHAS_Context *ctx, char *name)
{
	BTSHAS_PpDef *cur, *prv;
	char *s;
	int i;

	s=name; i=0; while(*s)i=(i*251)+(*s++);
	i&=255;

	cur=ctx->ppdef[i]; prv=NULL;
	while(cur)
	{
		if(!strcmp(cur->name, name))
		{
			if(prv)
			{
				prv->next=cur->next;
			}else
			{
				ctx->ppdef[i]=cur->next;
			}
			BTSHASP_FreeDefine(ctx, cur);
			return;
		}
		prv=cur; cur=cur->next;
	}
}

void BTSHASP_CleanupDefines(BTSHAS_Context *ctx)
{
	BTSHAS_PpDef *cur, *nxt;
	int i;

	for(i=0; i<256; i++)
	{
		cur=ctx->ppdef[i];
		ctx->ppdef[i]=NULL;

		while(cur)
		{
			nxt=cur->next;
			BTSHASP_FreeDefine(ctx, cur);
			cur=nxt; 
		}
	}
}

void BTSHASP_AddDefine(BTSHAS_Context *ctx, char *name, char *str)
{
	BTSHAS_PpDef *cur;
	char *s;
	int i;

	s=name; i=0;
	while(*s)i=(i*251)+(*s++);
	i&=255;

	cur=BTSHASP_AllocDefine(ctx);

	cur->name=btshas_strdup(name);
	cur->value=btshas_strdup(str);

	cur->next=ctx->ppdef[i];
	ctx->ppdef[i]=cur;
}

void BTSHASP_AddDefineArgs(BTSHAS_Context *ctx,
	char *name, char **args, char *str)
{
	BTSHAS_PpDef *cur;
	char *s;
	int i, hi;

	s=name; hi=0; while(*s)hi=(hi*251)+(*s++);
	hi&=255;

	cur=BTSHASP_AllocDefine(ctx);

	cur->name=btshas_strdup(name);
	cur->value=btshas_strdup(str);

	for(i=0; args[i]; i++);

	if(i>=8)
		{ cur->args=btshas_mm_malloc((i+1)*sizeof(char *)); }
	else
		{ cur->args=cur->t_args; }

	for(i=0; args[i]; i++)cur->args[i]=btshas_strdup(args[i]);
	cur->args[i]=NULL;

	cur->next=ctx->ppdef[hi];
	ctx->ppdef[hi]=cur;
}


void BTSHASP_AddStaticDefine(BTSHAS_Context *ctx,
	char *name, char *str)
{
	BTSHAS_PpDef *cur;

	cur=btshas_mm_malloc(sizeof(BTSHAS_PpDef));
	memset(cur, 0, sizeof(BTSHAS_PpDef));

	cur->name=btshas_strdup(name);
	cur->value=btshas_strdup(str);

	cur->next=btshas_sdef;
	btshas_sdef=cur;
}

void BTSHASP_AddStaticArgDefine(BTSHAS_Context *ctx,
	char *val)
{
	char tbn[64];
	char *s, *t, *te;
	
	s=val; t=tbn; te=t+62;
	while(*s && (*s!='=') && (t<te))
		*t++=*s++;
	*t++=0;
	
	if(*s=='=')
	{
		BTSHASP_AddStaticDefine(ctx, tbn, s+1);
	}else
	{
		BTSHASP_AddStaticDefine(ctx, val, NULL);
	}
}

void BTSHASP_AddStaticDefineArgs(BTSHAS_Context *ctx,
	char *name, char **args, char *str)
{
	BTSHAS_PpDef *cur;
	int i;

	cur=btshas_mm_malloc(sizeof(BTSHAS_PpDef));
	memset(cur, 0, sizeof(BTSHAS_PpDef));

	cur->name=btshas_strdup(name);
	cur->value=btshas_strdup(str);

	for(i=0; args[i]; i++);
	if(i>=8)
		{ cur->args=btshas_mm_malloc((i+1)*sizeof(char *)); }
	else
		{ cur->args=cur->t_args; }

	for(i=0; args[i]; i++)
		cur->args[i]=btshas_strdup(args[i]);
	cur->args[i]=NULL;

	cur->next=btshas_sdef;
	btshas_sdef=cur;
}

char *BTSHASP_LoadInclude(BTSHAS_Context *ctx, char *name, int *rsz)
{
	char b[4096];
	char *buf;
	int i, sz;

	if(BTSHASP_CheckIncludeCache(name, &buf, &sz)>0)
	{
		if(rsz)*rsz=sz;
		return(buf);
	}

	buf=NULL;
	for(i=(btshas_ninc-1); i>=0; i--)
	{
		sprintf(b, "%s/%s", btshas_inc[i], name);
		buf=BTSHAS_LoadFile(b, &sz);
		if(buf)break;
	}

	if(!buf)
	{
		for(i=(btshas_nsinc-1); i>=0; i--)
		{
			sprintf(b, "%s/%s", btshas_sinc[i], name);
			buf=BTSHAS_LoadFile(b, &sz);
			if(buf)break;
		}
	}

	if(buf)
	{
		BTSHASP_AddIncludeCache(name, buf, sz);
		if(rsz)*rsz=sz;
		return(buf);
	}

	return(NULL);
}

void BTSHASP_Include(BTSHAS_Context *ctx, char *str)
{
	char b[4096];
	char *buf;
	char *s;
	int l0, l1, l2, l3;
	int i;

	buf=BTSHASP_LoadInclude(ctx, str, NULL);

	if(!buf)
	{
		printf("BTSHASP_Include: fail open include"
			" '%s'\n", str);
		btshas_err++;
		return;
	}

	l0=btshas_iflvl;
	l1=btshas_iflvl2;

	BTSHASP_PushLinenum();
	BTSHASP_SetLinenum(str, buf, 1);

	BTSHASP_Buffer(ctx, buf);
//	free(buf);

	BTSHASP_PopLinenum();
	
	l2=btshas_iflvl;
	l3=btshas_iflvl2;
	if((l0!=l2) || (l1!=l3))
	{
		printf("BTSHASP_Include: PP Mismatched Nesting Levels"
			" '%s': %d->%d, %d->%d\n", str, l0, l2, l1, l3);

		for(i=l0-2; i<l2+2; i++)
		{
			if(i<0)continue;
			s=btshas_ifdname[i];
			if(!s)s="--";
			printf("    %d: %s\n", i, s);
		}
		for(i=l1-2; i<l3+2; i++)
		{
			if(i<0)continue;
			s=btshas_ifdname2[i];
			if(!s)s="--";
			printf("    %d: %s\n", i, s);
		}

		btshas_iflvl=l0;
		btshas_iflvl2=l1;
	}
}

char *BTSHASP_TokenCtx(BTSHAS_Context *ctx,
	char *str, char *buf, int *rty)
{
	char tb[256];
	char *s;
	
//	s=BTSHASP_ParseTokenBasic(str, tb);
	s=BTSHASP_ParseTokenBasicAltn(str, tb);
	strcpy(buf, tb+1);
	*rty=tb[0];
	return(s);
}

void BTSHASP_Directive2(BTSHAS_Context *ctx, char *str)
{
	char b[256], b2[4096];
	int ty, ty2;
	char *s, *t, *s1;
	int i;

	s=str;
	s=BTSHASP_EatWhiteOnly(s);
	s++;

	s=BTSHASP_TokenCtx(ctx, s, b, &ty);
	BTSHASP_TokenCtx(ctx, s, b2, &ty2);

	if(!strcmp(b, "if"))
	{
		btshas_ifdname2[btshas_iflvl2]=btshas_strdup(str);
		btshas_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifdef"))
	{
		btshas_ifdname2[btshas_iflvl2]=btshas_strdup(b2);
		btshas_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifndef"))
	{
		btshas_ifdname2[btshas_iflvl2]=btshas_strdup(b2);
		btshas_iflvl2++;
		return;
	}

	if(!strcmp(b, "endif"))
	{
		btshas_iflvl2--;
		return;
	}

	if(!strcmp(b, "else"))
	{
		if((btshas_iflvl2==1) && !btshas_ifmark)
		{
			btshas_ifdname[btshas_iflvl]=
				btshas_ifdname2[btshas_iflvl2-1];
			btshas_iflvl2--;
			btshas_iflvl++;
			return;
		}
		return;
	}

	if(!strcmp(b, "elif"))
	{
		if((btshas_iflvl2==1) && !btshas_ifmark)
		{
			strcpy(b2, s);
			i=1; while(i)i=BTSHASP_Line(ctx, b2);

			s1=b2;
//			n0=BTSHASP_Expression(ctx, &s1);
//			n0=BGBCC_ReduceExpr(NULL, n0);
//			i=BGBCC_BoolExpr(NULL, n0);
			i=0;

			if(i>0)
			{
				btshas_ifdname[btshas_iflvl]=
					btshas_ifdname2[btshas_iflvl2-1];
				btshas_iflvl2--;
				btshas_iflvl++;
				return;
			}else if(i<0)
			{
				BTSHASP_Error(ctx, "#elif pp-expr couldn't be evaluated\n");
				//error
			}

			return;
		}

		return;
	}
}

void BTSHASP_Directive(BTSHAS_Context *ctx, char *str)
{
	char b[4096], b2[4096], b3[4096];
	char *ab[64];
	BTSHAS_PpDef *def;
	char *s, *t, *s1, *s2, *tn;
	int ty, ty2;
	int i, j, n;

	if(btshas_iflvl2)
	{
		BTSHASP_Directive2(ctx, str);
		return;
	}

	s=str;
	s=BTSHASP_EatWhiteOnly(s);

	s++;

	s=BTSHASP_TokenCtx(ctx, s, b, &ty);
	s1=BTSHASP_TokenCtx(ctx, s, b2, &ty2);

	if(!strcmp(b, "pragma"))
	{
		return;
	}

	if(!strcmp(b, "error"))
	{
		s=BTSHASP_EatWhiteOnly(s);
		printf("BTSHASP_Directive: %s:%d: #error: %s\n",
			BTSHASP_GetFilename(),
			BTSHASP_CalcLinenum(btshas_spos), s);
		btshas_err++;

		return;
	}

	if(!strcmp(b, "warning"))
	{
		s=BTSHASP_EatWhiteOnly(s);
		printf("BTSHASP_Directive: %s:%d: #warning: \"%s\"\n",
			BTSHASP_GetFilename(),
			BTSHASP_CalcLinenum(btshas_spos), s);
		btshas_warn++;

		return;
	}

	if(!strcmp(b, "include"))
	{
		i=1; strcpy(b, s);
		while(i)i=BTSHASP_Line(ctx, b);

		s1=BTSHASP_TokenCtx(ctx, b, b2, &ty2);

		if(!strcmp(b2, "<"))
		{
			t=b2;
			while(*s1 && (*s1!='>'))*t++=*s1++;
			*t++=0;

			BTSHASP_Include(ctx, b2);
			return;
		}

		t=b2; t+=strlen(t);
		while(*s1)
		{
			s1=BTSHASP_TokenCtx(ctx, s1, t, &ty2);
			t+=strlen(t);
		}
		*t=0;

		BTSHASP_Include(ctx, b2);
		return;
	}

	if(!strcmp(b, "define"))
	{
		if(*s1!='(')
		{
			if(!strcmp(b2, "_noinclude"))
				{ btshas_noinclude_p=1; return; }

			while(*s1 && (*s1<=' '))s1++;
			BTSHASP_AddDefine(ctx, b2, s1);
			return;
		}

		tn=btshas_strdup(b2);

		s1++;
		n=0;
		while(1)
		{
			s1=BTSHASP_TokenCtx(ctx, s1, b, &ty);
			if(!strcmp(b, ")"))break;
			if(!strcmp(b, ","))continue;

			ab[n++]=btshas_strdup(b);
		}
		ab[n]=NULL;

		while(*s1 && (*s1<=' '))s1++;

		BTSHASP_AddDefineArgs(ctx, tn, ab, s1);
		return;
	}

	if(!strcmp(b, "define_arch"))
	{
		if(*s1!='(')
		{
			while(*s1 && (*s1<=' '))s1++;
			if(*s1)
			{
				sprintf(b3, "#ARCH:%s", s1);
			}else
			{
				sprintf(b3, "#ARCH:%s", b2);
			}
			BTSHASP_AddDefine(ctx, b2, b3);
			return;
		}

		return;
	}

	if(!strcmp(b, "endif"))
	{
		if(btshas_ifflag[btshas_iflvl]&1)
		{
			sprintf(btshas_obuf, "/*%d*/ }\n",
				BTSHASP_CalcLinenum(btshas_spos));
		}
		btshas_iflvl--;
		return;
	}

	if(!strcmp(b, "else"))
	{
		if(btshas_ifflag[btshas_iflvl]&1)
		{
			sprintf(btshas_obuf, "/*%d*/ }else{\n",
				BTSHASP_CalcLinenum(btshas_spos));
			return;
		}

		btshas_ifdname2[btshas_iflvl2]=
			btshas_ifdname[btshas_iflvl-1];
		btshas_iflvl--;
		btshas_iflvl2++;
		return;
	}

	if(!strcmp(b, "elif"))
	{
		btshas_ifdname2[btshas_iflvl2]=
			btshas_ifdname[btshas_iflvl-1];
		btshas_iflvl--;
		btshas_iflvl2++;
		return;
	}

	if(!strcmp(b, "ifdef"))
	{
		btshas_ifmark=0;
		btshas_ifflag[btshas_iflvl]=0;
		def=BTSHASP_LookupDefine(ctx, b2);
		if(!def)
		{
			btshas_ifdname2[btshas_iflvl2]=btshas_strdup(b2);
			btshas_iflvl2++;
		}else
		{
			if(def->value && !strncmp(def->value, "#ARCH:", 6))
			{
				btshas_ifflag[btshas_iflvl]|=1;
				sprintf(btshas_obuf, "/*%d*/ __ifdef(%s) {\n",
					BTSHASP_CalcLinenum(btshas_spos), def->value+6);
			}
			btshas_ifdname[btshas_iflvl]=btshas_strdup(b2);
			btshas_iflvl++;
		}
		if(!btshas_iflvl2)btshas_ifmark=1;
		return;
	}
	if(!strcmp(b, "ifndef"))
	{
		btshas_ifmark=0;
		btshas_ifflag[btshas_iflvl]=0;
		def=BTSHASP_LookupDefine(ctx, b2);
		if(!def)
		{
			btshas_ifdname[btshas_iflvl]=btshas_strdup(b2);
			btshas_iflvl++;
		}else
		{
			if(def->value && !strncmp(def->value, "#ARCH:", 6))
			{
				btshas_ifflag[btshas_iflvl]|=1;
				sprintf(btshas_obuf, "/*%d*/ __ifdef(%s) {\n",
					BTSHASP_CalcLinenum(btshas_spos), def->value+6);
				btshas_ifdname[btshas_iflvl]=btshas_strdup(b2);
				btshas_iflvl++;
			}else
			{
				btshas_ifdname2[btshas_iflvl2]=btshas_strdup(b2);
				btshas_iflvl2++;
			}
		}
		if(!btshas_iflvl2)btshas_ifmark=1;
		return;
	}

	if(!strcmp(b, "undef"))
	{
		BTSHASP_DeleteDefine(ctx, b2);
		return;
	}

	if(!strcmp(b, "if"))
	{
		i=1;
		strcpy(b2, s);
		while(i)i=BTSHASP_Line(ctx, b2);

		s2=b2;
//		exp=BTSHASP_Expression(ctx, &s2);
//		exp=BGBCC_ReduceExpr(NULL, exp);
//		i=BGBCC_BoolExpr(NULL, exp);
		i=-1;

		btshas_ifmark=0;
		if(i>0)
		{
			btshas_ifdname[btshas_iflvl]=btshas_strdup(b2);
			btshas_iflvl++;
		} else {
			btshas_ifdname2[btshas_iflvl2]=btshas_strdup(b2);
			btshas_iflvl2++;
		}
		if(!btshas_iflvl2)btshas_ifmark=1;

		if(i<0)
		{
			BTSHASP_Error(ctx, "#if pp-expr couldn't be evaluated\n");
//			BCCX_Print(exp);
		}
		return;
	}

	BTSHASP_Error(ctx, "Unsupported preproc directive\n");

//	printf("BTSHASP_Directive: %s:%d: Unsupported preproc directive %s\n",
//		BTSHASP_GetFilename(),
//		BTSHASP_CalcLinenum(btshas_spos), b);
	btshas_err++;
}


char *BTSHASP_ParseArg(BTSHAS_Context *ctx, char *s, char *b)
{
	char b2[4096];
	int i, ty2;

	i=0;
	while(*s)
	{
		if((*s=='\"') || (*s=='\''))
		{
			s=BTSHASP_TokenCtx(ctx, s, b2, &ty2);
			b=BTSHASP_PrintToken(b, b2, ty2);
			continue;
		}

		if(*s=='(') { i++; *b++=*s++; continue; }
		if(*s==')') { if(!i)break; i--; *b++=*s++; continue; }
		if(*s==',') { if(!i)break; *b++=*s++; continue; }
		*b++=*s++;
	}

	*b++=0;
	return(s);
}

char *BTSHASP_PrintName(char *t, char *s)
{
	int i;

	i=*(t-1);
	if((i=='_') || ((i>='0') && (i<='9')) ||
		((i>='A') && (i<='Z')) || ((i>='a') && (i<='z')))
			*t++=' ';
	strcpy(t, s);
	t+=strlen(s);
	return(t);
}

char *BTSHASP_PrintToken(char *t, char *s, int ty)
{
	if(ty==BTK_NAME)
	{
		t=BTSHASP_PrintName(t, s);
		return(t);
	}

	if(ty==BTK_NUMBER)
	{
		t=BTSHASP_PrintName(t, s);
		return(t);
	}

	if(ty==BTK_STRING)
	{
		t=BTSHASP_EmitString(t, s);
		return(t);
	}

	if(ty==BTK_CHARSTRING)
	{
		t=BTSHASP_EmitCharString(t, s);
		return(t);
	}

	if(ty==BTK_OPERATOR)
	{
		if(BTSHASP_OpChar(*(t-1)) || (*s=='.') || (*s=='@'))
			*t++=' ';
		sprintf(t, "%s", s);
		t+=strlen(t);
		return(t);
	}

	if((*s=='.') || (*s=='@'))
		*t++=' ';

	sprintf(t, "%s", s);
	t+=strlen(t);
	return(t);
}

int BTSHASP_Expand(BTSHAS_Context *ctx,
	char **src, char **dst, char *op, char *dste)
{
	char b[4096], b2[4096];
	char *ab[64];
	BTSHAS_PpDef *def;
	char *s, *s1, *t, *te;
	int i, j, ty, ty2;

	s=*src; t=*dst; te=dste;

	if((t+16)>=te)
	{
		BTSHASP_Error(ctx, "BTSHASP_Expand: Buffer Overflow\n");
		return(-1);
	}

	if(!strcmp(op, "defined"))
	{
		s=BTSHASP_TokenCtx(ctx, s, b, &ty);	//<name> | '('
		if(!strcmp(b, "("))
		{
			s=BTSHASP_TokenCtx(ctx, s, b, &ty); //<name>
			def=BTSHASP_LookupDefine(ctx, b);
			s=BTSHASP_TokenCtx(ctx, s, b, &ty); //')'

			sprintf(t, "%d ", def?1:0);
			t+=strlen(t);

			*src=s; *dst=t;
			return(1);
		}

		def=BTSHASP_LookupDefine(ctx, b);
		sprintf(t, "%d ", def!=NULL);
		t+=strlen(t);

		*src=s; *dst=t;
		return(1);
	}

	def=BTSHASP_LookupDefine(ctx, op);
	if(!def)return(0);

	if(!def->args)
	{
		if((t+strlen(def->value)+16)>=te)
		{
			BTSHASP_Error(ctx, "BTSHASP_Expand: Buffer Overflow\n");
			return(-1);
		}

		t=BTSHASP_PrintName(t, def->value);
		*src=s; *dst=t;
		return(1);
	}

	s=BTSHASP_EatWhiteOnly(s);

	if(*s!='(')return(0);

	s++; j=0;
	while(def->args[j])
	{
		while(*s && (*s<=' '))s++;

		if(*s==',')
		{
			ab[j++]=btshas_strdup("");
			s++; continue;
		}
		if(*s==')') break;
		while(*s && (*s<=' '))s++;

		s=BTSHASP_ParseArg(ctx, s, b2);
		ab[j++]=btshas_strdup(b2);

		if(*s==',')
		{
			s++;
			while(*s && (*s<=' '))s++;
			if(*s==')')
			{
				ab[j++]=btshas_strdup("");
				break;
			}
			continue;
		}
	}
	ab[j]=NULL;

	if(def->args[j] || (*s!=')'))
	{
		BTSHASP_Error(ctx, "BTSHASP_Expand: macro parse problem (%s)\n", op);
		btshas_err++;
		return(0);
	}
	s++;

	*src=s;

	s=def->value;
	while(*s)
	{
		s=BTSHASP_TokenCtx(ctx, s, b, &ty);

		if(ty==BTK_NAME)
		{
			for(i=0; def->args[i]; i++)
				if(!strcmp(def->args[i], b))
					break;

			if(def->args[i])
			{
				if((t+strlen(ab[i])+16)>=te)
				{
					BTSHASP_Error(ctx, "BTSHASP_Expand: Buffer Overflow\n");
					return(-1);
				}

				t=BTSHASP_PrintName(t, ab[i]);
				continue;
			}

			if((t+strlen(b)+16)>=te)
			{
				BTSHASP_Error(ctx, "BTSHASP_Expand: Buffer Overflow\n");
				return(-1);
			}

			t=BTSHASP_PrintName(t, b);
			continue;
		}

		if((t+strlen(b)+16)>=te)
		{
			BTSHASP_Error(ctx, "BTSHASP_Expand: Buffer Overflow\n");
			return(-1);
		}

		if(ty==BTK_NUMBER)
		{
			t=BTSHASP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_STRING)
		{
			t=BTSHASP_EmitString(t, b);
			continue;
		}

		if(ty==BTK_CHARSTRING)
		{
			t=BTSHASP_EmitCharString(t, b);
			continue;
		}

		if(ty==BTK_OPERATOR)
		{
			if(BTSHASP_OpChar(*(t-1)) || (b[0]=='.') || (b[0]=='@'))
				*t++=' ';
			sprintf(t, "%s", b);
			t+=strlen(t);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}

	*t=0;

	*dst=t;
	return(1);
}

int BTSHASP_Line(BTSHAS_Context *ctx, char *str)
{
	static char lbuf[1<<20];
	char b[4096], b2[4096];
	char *s, *t, *t1, *te;
	int i, ty, ty2, ni;

	BTSHASP_FlushToken(str);

	ni=0;
	s=str; t=lbuf; te=lbuf+((1<<20)-4096);
	while(*s)
	{
		s=BTSHASP_TokenCtx(ctx, s, b, &ty);
		BTSHASP_TokenCtx(ctx, s, b2, &ty2);

		if((ty2==BTK_OPERATOR) &&
			(!strcmp(b2, "##") || !strcmp(b, "%:%:")))
		{
			if(ty==BTK_NAME)
			{
				t1=t+1;
				i=BTSHASP_Expand(ctx, &s, &t1, b, te);
				if(i>0) { strcpy(b, t+1); ni++; }
			}

			s=BTSHASP_TokenCtx(ctx, s, b2, &ty2);	//'##'

			s=BTSHASP_TokenCtx(ctx, s, b2, &ty2);

			if(ty2==BTK_NAME)
			{
				t1=t+1;
				i=BTSHASP_Expand(ctx, &s, &t1, b2, te);
				if(i>0) { strcpy(b2, t+1); ni++; }
			}

//			printf("## operator: '%s' ## '%s' ('%s')\n", b, b2, str);

			*t=0;
			strcat(b, b2);
			
			if((t+strlen(b)+16)>=te)
			{
				BTSHASP_Error(ctx, "BTSHASP_Line: Buffer Overflow\n");
				return(-1);
			}
			
			if((ty==BTK_STRING)||(ty2==BTK_STRING))
			{
				t=BTSHASP_EmitString(t, b);
			}else if((ty==BTK_CHARSTRING)||(ty2==BTK_CHARSTRING))
			{
				t=BTSHASP_EmitCharString(t, b);
			}else
			{
				t=BTSHASP_PrintName(t, b);
			}
			
//			printf("## operator: '%s'\n", b);
			
			ni++;
			continue;
		}

		if((t+strlen(b)+16)>=te)
		{
			BTSHASP_Error(ctx, "BTSHASP_Line: Buffer Overflow\n");
			return(-1);
		}

		if(ty==BTK_NAME)
		{
			i=BTSHASP_Expand(ctx, &s, &t, b, te);
			if(i>0)
			{
				ni++;
				continue;
			}

			t=BTSHASP_PrintName(t, b);
			continue;
		}

		if((ty==BTK_OPERATOR) &&
			(
//			!strcmp(b, "#") ||
			!strcmp(b, "%:")))
		{
			if(ty2==BTK_NAME)
			{
				s=BTSHASP_TokenCtx(ctx, s, b2, &ty2);

				t1=t;
				i=BTSHASP_Expand(ctx, &s, &t1, b2, te);
				if(i>0) { strcpy(b2, t); ni++; }

				t=BTSHASP_PrintToken(t, b2, BTK_STRING);
				ni++;
				continue;
			}

			s=BTSHASP_TokenCtx(ctx, s, b2, &ty2);
			t=BTSHASP_PrintToken(t, b2, BTK_STRING);
			ni++;
			continue;
		}

		if(ty==BTK_NUMBER)
		{
			t=BTSHASP_PrintName(t, b);
			continue;
		}

		if(ty==BTK_STRING)
		{
			t=BTSHASP_EmitString(t, b);
			continue;
		}

		if(ty==BTK_CHARSTRING)
		{
			t=BTSHASP_EmitCharString(t, b);
			continue;
		}

		if(ty==BTK_OPERATOR)
		{
			if(BTSHASP_OpChar(*(t-1)) || (b[0]=='.') || (b[0]=='@'))
				*t++=' ';
			sprintf(t, "%s", b);
			t+=strlen(t);
			continue;
		}

		sprintf(t, "%s", b);
		t+=strlen(t);
	}
	*t=0;


	BTSHASP_FlushToken(str);

	memset(str, 0, 256);
	strcpy(str, lbuf);
	return(ni);
}

int BTSHASP_LinePostFilter(BTSHAS_Context *ctx, char *str)
{
//	BTSHASP_LineDigraph(ctx, str);
	return(0);
}


char *BTSHASP_ParseLine(BTSHAS_Context *ctx, char *s, char *b)
{
	static int is=0, is2=0;
	char *t, *s1;
	int nplvl;

	t=b; nplvl=0;
	while(*s)
	{
		if(is)
		{
			if(*s=='\"')
			{
				is=0;
				*t++=*s++;
				continue;
			}

			if((s[0]=='\\') && (s[1]=='\''))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }

			*t++=*s++;
			continue;
		}

		if(is2)
		{
			if(*s=='\'')
			{
				is2=0;
				*t++=*s++;
				continue;
			}

			if((s[0]=='\\') && (s[1]=='\''))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\"'))
				{ *t++=*s++; *t++=*s++; continue; }
			if((s[0]=='\\') && (s[1]=='\\'))
				{ *t++=*s++; *t++=*s++; continue; }

			*t++=*s++;
			continue;
		}

		if(*s=='\"')
		{
			is=1;
			*t++=*s++;
			continue;
		}

		if(*s=='\'')
		{
			is2=1;
			*t++=*s++;
			continue;
		}

		if((*s=='\n') || (*s=='\r'))
		{
			if((t-b)>8192)
				break;
				
			s1=s;
			while(*s1 && (*s1<=' '))s1++;
			if(*s1=='#')
				break;

			if(nplvl>0)
			{
				*t++=*s++;
				continue;
			}
			break;
		}

		if((s[0]=='/') && (s[1]=='/'))
		{
			if(nplvl>0)
			{
				*t++=' ';
				while(*s && (*s!='\n') && (*s!='\r'))
					s++;
				continue;
			}
			break;
		}

		if((s[0]=='/') && (s[1]=='*'))
		{
			s+=2;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
				{
					s+=2;
					break;
				}
				s++;
			}
			*t++=' ';
			continue;
		}

		if((*s=='\\') && ((s[1]=='\r') || (s[1]=='\n')))
		{
			s++;
			if(*s=='\r')s++;
			if(*s=='\n')s++;
			while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))
				s++;
			*t++=' ';
			continue;
		}
		
		if(*s=='(')nplvl++;
		if(*s==')')nplvl--;
		
		*t++=*s++;
	}
	*t++=0;
	if(*s=='\r')s++;
	if(*s=='\n')s++;

	return(s);
}

int BTSHASP_BufferLine(BTSHAS_Context *ctx, char *b)
{
	char *s, *t, *s1;
	int i;

	t=b; while(*t && (*t<=' '))t++;
	if(!*t)
	{
		*b=0;
		return(0);
	}

//	BTSHASP_Trigraphs(ctx, b);
	BTSHASP_UniEscape(ctx, b);

	s1=b; while(*s1 && (*s1<=' '))s1++;
	if(*s1=='#')
	{
		BTSHASP_FlushToken(b);
		BTSHASP_Directive(ctx, b);
		return(0);
	}

	if(btshas_iflvl2)
		{ return(0); }

	i=1;
	while(i)
	{
		BTSHASP_FlushToken(b);
		i=BTSHASP_Line(ctx, b);
	}

	BTSHASP_FlushToken(b);
	BTSHASP_LinePostFilter(ctx, b);

	if((btshas_obuf+strlen(b)+256)>=btshas_obufe)
	{
		BTSHASP_Error(ctx, "BTSHASP_BufferLine: Output Buffer Overflow\n");
		return(-1);
	}

#if 0
	s1=BTSHASP_GetFilename();
	if(s1!=btshas_lfn)
	{
		sprintf(btshas_obuf, "/*\"%s\"%d*/ %s\n",
			s1, BTSHASP_CalcLinenum(btshas_spos), b);
		btshas_lfn=s1;
	}else
	{
		sprintf(btshas_obuf, "/*%d*/ %s\n",
			BTSHASP_CalcLinenum(btshas_spos), b);
	}
#endif

	sprintf(btshas_obuf, "%s\n", b);

	return(0);
}

int BTSHASP_Buffer(BTSHAS_Context *ctx, char *ibuf)
{
	static char b[1<<20];
	char *s, *t, *s1;
	int i;

	s=ibuf;
	while(*s)
	{
		if((btshas_obuf+4096)>=btshas_obufe)
		{
			BTSHASP_Error(ctx, "BTSHASP_Buffer: Output Buffer Overflow\n");
			return(-1);
		}
	
		btshas_spos=s;
		
#if 1
		s1=s;
		while(*s1 && (*s1<=' '))s1++;
		if((s1[0]=='/') && (s1[1]=='/'))
		{
			s=s1; t=btshas_obuf;
			while(*s && (*s!='\r') && (*s!='\n')) *t++=*s++;
			if(*s=='\r')s++; if(*s=='\n')*t++=*s++;
			btshas_obuf=t; *t=0;
			continue;
		}

		if((s1[0]=='/') && (s1[1]=='*') && (s1[2]=='*'))
		{
			s=s1; t=btshas_obuf;
			*t++=*s++; *t++=*s++; *t++=*s++;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
					{ *t++=*s++; *t++=*s++; break; }
				if(*s=='\r') { s++; continue; }
				*t++=*s++;
			}
			while(*s && ((*s==' ')||(*s=='\t'))) *t++=*s++;
			if(*s=='\r')s++; if(*s=='\n')*t++=*s++;
			btshas_obuf=t; *t=0;
			continue;
		}

		if((s1[0]=='/') && (s1[1]=='*'))
		{
			s=s1; t=btshas_obuf;
			s+=2;
			while(*s)
			{
				if((s[0]=='*') && (s[1]=='/'))
					{ s+=2; break; }
				s++;
			}
			while(*s && (*s<=' '))
				{ *s++; }
			btshas_obuf=t; *t=0;
			continue;
		}
#endif
		
		s=BTSHASP_ParseLine(ctx, s, b);
		BTSHASP_BufferLine(ctx, b);
		btshas_obuf+=strlen(btshas_obuf);
	}
	
	return(0);
}


int BTSHASP_Filter(BTSHAS_Context *ctx,
	char *ibuf, char *obuf, int osz)
{
	char tb[4096];
	char *s, *t;
	int i;


	for(i=0; i<btshas_nsinc; i++)
		btshas_inc[i]=btshas_sinc[i];
	btshas_ninc=btshas_nsinc;

	s=BTSHASP_GetFilename();
	if(*s)
	{
		strcpy(tb, s);
		s=tb+strlen(tb);
		while((s>tb) && (*s!='/'))s--;

		if(*s=='/')
		{
			*s=0;

			i=btshas_ninc++;
			btshas_inc[i]=btshas_strdup(tb);
		}
	}

	btshas_ibuf=ibuf;
	btshas_obuf=obuf;
	btshas_obufe=obuf+osz;
	btshas_noinclude_p=0;

	for(i=0; i<256; i++)
		ctx->ppdef[i]=NULL;

	btshas_warn=0;
	btshas_err=0;

	btshas_iflvl=0;
	btshas_iflvl2=0;

	BTSHASP_FlushToken(ibuf);
	BTSHASP_Buffer(ctx, ibuf);

//	BTSHASP_SendDefines(ctx);
	BTSHASP_FlushToken(obuf);
	
	BTSHASP_CleanupDefines(ctx);
	
	if(btshas_err>0)
		return(-1);
	return(0);
}
