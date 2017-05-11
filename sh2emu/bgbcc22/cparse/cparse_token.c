#include <bgbccc.h>

char *bgbcp_pbase;
char *bgbcp_fname;
int bgbcp_linenum=1;

char *bgbcp_pbasestack[256];
char *bgbcp_fnamestack[256];
int bgbcp_lnumstack[256];
int bgbcp_lnumstackpos;

char *bgbcp_csfn;
int bgbcp_csln;

char *bgbcp_lfn;
int bgbcp_lln;

int BGBCP_GetLinenum()
	{ return(bgbcp_linenum); }

char *BGBCP_GetFilename()
	{ return(bgbcp_fname); }

int BGBCP_CalcLinenum(char *se)
{
	char *s;
	int i;

	if(!bgbcp_pbase)
		return(0);

	if(bgbcp_csfn && (bgbcp_csfn<=se))
	{
		s=bgbcp_csfn;
		i=bgbcp_csln;
	}else
	{
		s=bgbcp_pbase;
		i=bgbcp_linenum;
	}

	while(s<se)
	{
		if(*s=='\n')i++;
		if((s[0]=='\r') && (s[1]!='\n'))i++;
		s++;
	}

	bgbcp_csfn=se;
	bgbcp_csln=i;

	return(i);
}

int BGBCP_SetLinenum(char *fname, char *base, int num)
{
	bgbcp_fname=bgbcc_strdup(fname);
	bgbcp_pbase=base;
	bgbcp_linenum=num;
	bgbcp_csfn=NULL;
	return(num);
}

int BGBCP_PushLinenum()
{
	if(bgbcp_lnumstackpos>=255)
		return(-1);

	bgbcp_csfn=NULL;

	bgbcp_lnumstack[bgbcp_lnumstackpos]=
		bgbcp_linenum;
	bgbcp_pbasestack[bgbcp_lnumstackpos]=
		bgbcp_pbase;
	bgbcp_fnamestack[bgbcp_lnumstackpos]=
		bgbcp_fname;
	bgbcp_lnumstackpos++;
	return(0);
}

int BGBCP_PopLinenum()
{
	if(bgbcp_lnumstackpos<=0)
		return(-1);

	bgbcp_csfn=NULL;

	bgbcp_lnumstackpos--;
	bgbcp_linenum=
		bgbcp_lnumstack[bgbcp_lnumstackpos];
	bgbcp_pbase=
		bgbcp_pbasestack[bgbcp_lnumstackpos];
	bgbcp_fname=
		bgbcp_fnamestack[bgbcp_lnumstackpos];
	return(bgbcp_linenum);
}

int BGBCP_SkimLinenum(char *s, char **rfn, int *rln)
{
	static char b[64];
	char *fn, *t;
	int ln;

	if(rfn)*rfn=NULL;
	if(rln)*rln=-1;

	fn=NULL; ln=-1;

	while(*s && (*s<=' '))s++;
	if((s[0]!='/') || (s[1]!='*'))
		return(0);

	s+=2;
	if(*s=='\"')
	{
		s++;
		t=b;
		while(*s && (*s!='\"'))*t++=*s++;
		*t=0;

		if(*s!='\"')return(0);
		s++;

		fn=bgbcc_strdup(b);
	}

	t=b;
	while(*s && (*s>='0') && (*s<='9'))
		*t++=*s++;
	*t=0;

	if(!b[0])return(0);

	ln=atoi(b);

	if((s[0]!='*') || (s[1]!='/'))
		return(0);

	if(rfn)*rfn=fn;
	if(rln)*rln=ln;
	return(1);
}

char *BGBCP_GetLastFileName()
	{ return(bgbcp_lfn); }

int BGBCP_GetLastLineNumber()
	{ return(bgbcp_lln); }


char *BGBCP_EatWhiteOnly(char *s)
{
	if(!s)
		return(s);

	while(*s && (*s<=' '))s++;
	return(s);
}

char *BGBCP_EatWhiteOnly2(char *s)
{
	if(!s)
		return(s);

	while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))s++;
	return(s);
}

char *BGBCP_EatWhite(char *s)
{
	char *fn;
	int ln;
	int i;

	if(!s)
		return(s);

	while(*s && (*s<=' '))s++;

	if(s[0]=='/')
	{
//		if((s[0]=='/') && (s[1]=='/'))
		if(s[1]=='/')
		{
			while(*s && (*s!='\n'))s++;
			s=BGBCP_EatWhite(s);
			return(s);
		}
//		if((s[0]=='/') && (s[1]=='*'))
		if(s[1]=='*')
		{
			i=BGBCP_SkimLinenum(s, &fn, &ln);
			if(i)
			{
				if(fn)bgbcp_lfn=fn;
				bgbcp_lln=ln;
			}

#if 1
			s+=2; i=1;
			while(*s)
			{
				if(*s=='*')
				{
					s++;
					if(*s=='/')
					{
						s++; i--;
						if(!i)break;
						continue;
					}
					continue;
				}else if(*s=='/')
				{
					s++;
					if(*s=='*')
					{
						s++; i++;
						continue;
					}
					continue;
				}
				s++;
//				else if(*s) { s++; continue; }
//				else break;
			}
#endif

#if 0
			s+=2;
			i=1;
			while(1)
			{
				if((s[0]=='*') && (s[1]=='/'))
				{
					s+=2;
					i--;
					if(!i)break;
					continue;
				}
				if((s[0]=='/') && (s[1]=='*'))
				{
					s+=2;
					i++;
					continue;
				}
				if(!*s)break;
				s++;
			}
#endif
			s=BGBCP_EatWhite(s);
			return(s);
		}
	}

#if 0
	if(s[0]=='#')
	{
		while(*s && (*s!='\n'))s++;
		s=BGBCP_EatWhite(s);
		return(s);
	}
#endif

	return(s);
}


char *BGBCP_EatWhiteNLb(char *s)
{
	char *fn;
	int ln;
	int i;

	if(!s)
		return(s);

	while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))s++;

	if(s[0]=='/')
	{
		if(s[1]=='/')
		{
			while(*s && (*s!='\n'))s++;
//			s=BGBCP_EatWhiteNLb(s);
			return(s);
		}
		if(s[1]=='*')
		{
			i=BGBCP_SkimLinenum(s, &fn, &ln);
			if(i)
			{
				if(fn)bgbcp_lfn=fn;
				bgbcp_lln=ln;
			}

#if 1
			s+=2; i=1;
			while(*s)
			{
				if(*s=='*')
				{
					s++;
					if(*s=='/')
					{
						s++; i--;
						if(!i)break;
						continue;
					}
					continue;
				}else if(*s=='/')
				{
					s++;
					if(*s=='*')
					{
						s++; i++;
						continue;
					}
					continue;
				}
				s++;
			}
#endif
			s=BGBCP_EatWhiteNLb(s);
			return(s);
		}
	}

	return(s);
}

int BGBCP_IsWhiteOnly(char *s)
{
	while((*s==' ') || (*s=='\t'))
		s++;

	if((s[0]=='/') && (s[1]=='/'))return(0);
	if((s[0]=='/') && (s[1]=='*'))return(0);
	if((s[0]=='*') && (s[1]=='/'))return(0);

	if(*s>' ')return(1);
	if(*s==' ')return(1);
	if(*s=='\t')return(1);

	return(0);
}

int BGBCP_IsLineBreak(char *se)
{
	char *s, *b, *t;
	int i, j;

	s=se;
	while(*s && (*s<=' '))
	{
		if(*s=='\r')return(1);
		if(*s=='\n')return(1);
		s++;
	}
	if(!*s)return(1);

	return(0);
}


int BGBCP_OpChar(int c)
{
	int i;

	switch(c)
	{
	case '~':
	case '!':
	case '@':
	case '#':
//	case '$':
	case '%':
	case '^':
	case '&':
	case '*':
	case '-':
	case '=':
	case '+':

	case '\\':
	case '|':

	case ':':

	case '<':
	case '>':
	case '?':
	case '/':

	case '`':
	case '.':
		i=1;
		break;

	default:
		i=0;
		break;
	}
	return(i);
}

int BGBCP_PeekChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(!(i&0x80))return(i);

	if(i>=0xFC)
	{
		i=(i&1)<<30;
		i+=((*s++)&63)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF8)
	{
		i=(i&3)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF0)
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

	return(i);
}

int BGBCP_ParseChar(char **str)
{
	unsigned char *s;
	int i;

	s=(unsigned char *)(*str);
	i=*s++;

	if(!(i&0x80))
	{
		*str=(char *)s;
		return(i);
	}

	if(i>=0xFC)
	{
		i=(i&1)<<30;
		i+=((*s++)&63)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF8)
	{
		i=(i&3)<<24;
		i+=((*s++)&63)<<18;
		i+=((*s++)&63)<<12;
		i+=((*s++)&63)<<6;
		i+=(*s++)&63;
	}else if(i>=0xF0)
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

int BGBCP_EmitChar(char **str, int j)
{
	unsigned char *t;
	int i;

	t=(unsigned char *)(*str);

	if(j<128)
	{
		if(j<=0) //Modified-UTF8
			{*t++=0xC0; *t++=0x80;}
			else *t++=j;
		*str=(char *)t;
		return(0);
	}

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

//	}else *t++=j;

	*str=(char *)t;
	return(0);
}

int BGBCP_NameInitChar(int c, int lang)
{
	static int vals[]={
	0x00AA, 0x00BA, 0x207F, 0x0386, 0x038C, 0x03DA, 0x03DC, 0x03DE,
	0x03E0, 0x1F59, 0x1F5B, 0x1F5D, 0x05BF, 0x09B2, 0x0A02, 0x0A5E,
	0x0A74, 0x0A8D, 0x0AD0, 0x0AE0,
	0x0B9C, 0x0CDE, 0x0E84, 0x0E8A, 0x0E8D, 0x0EA5, 0x0EA7, 0x0EC6,
	0x0F00, 0x0F35, 0x0F37, 0x0F39, 0x0F97, 0x0FB9, 0x00B5, 0x00B7,
	0x02BB, 0x037A, 0x0559, 0x093D, 0x0B3D, 0x1FBE, 0x2102, 0x2107,
	0x2115, 0x2124,	0x2126, 0x2128, 0}; 

	static int rngs[]={
	0x0388, 0x038A, 0x038E, 0x03A1, 0x03A3, 0x03CE, 0x03D0, 0x03D6,
	0x03E2, 0x03F3, 0x1F00, 0x1F15, 0x1F18, 0x1F1D, 0x1F20, 0x1F45,
	0x1F48, 0x1F4D, 0x1F50, 0x1F57, 0x1F5F, 0x1F7D, 0x1F80, 0x1FB4,
	0x1FB6, 0x1FBC, 0x1FC2, 0x1FC4, 0x1FC6, 0x1FCC, 0x1FD0, 0x1FD3,
	0x1FD6, 0x1FDB, 0x1FE0, 0x1FEC, 0x1FF2, 0x1FF4, 0x1FF6, 0x1FFC,
	0x0401, 0x040C, 0x040E, 0x044F, 0x0451, 0x045C, 0x045E, 0x0481,
	0x0490, 0x04C4, 0x04C7, 0x04C8, 0x04CB, 0x04CC, 0x04D0, 0x04EB,
	0x04EE, 0x04F5, 0x04F8, 0x04F9, 0x0531, 0x0556, 0x0561, 0x0587,
	0x05B0, 0x05B9, 0x05BB, 0x05BD, 0x05C1, 0x05C2, 0x05D0, 0x05EA,
	0x05F0, 0x05F2, 0x0621, 0x063A, 0x0640, 0x0652, 0x0670, 0x06B7,
	0x06BA, 0x06BE, 0x06C0, 0x06CE, 0x06D0, 0x06DC, 0x06E5, 0x06E8,
	0x06EA, 0x06ED, 0x0901, 0x0903, 0x0905, 0x0939, 0x093E, 0x094D,
	0x0950, 0x0952, 0x0958, 0x0963, 0x0981, 0x0983, 0x0985, 0x098C,
	0x098F, 0x0990, 0x0993, 0x09A8, 0x09AA, 0x09B0, 0x09B6, 0x09B9,
	0x09BE, 0x09C4, 0x09C7, 0x09C8, 0x09CB, 0x09CD, 0x09DC, 0x09DD,
	0x09DF, 0x09E3, 0x09F0, 0x09F1, 0x0A05, 0x0A0A, 0x0A0F, 0x0A10,
	0x0A13, 0x0A28, 0x0A2A, 0x0A30, 0x0A32, 0x0A33, 0x0A35, 0x0A36,
	0x0A38, 0x0A39, 0x0A3E, 0x0A42, 0x0A47, 0x0A48, 0x0A4B, 0x0A4D,
	0x0A59, 0x0A5C, 0x0A81, 0x0A83, 0x0A85, 0x0A8B, 0x0A8F, 0x0A91,
	0x0A93, 0x0AA8, 0x0AC7, 0x0AC9, 0x0ACB, 0x0ACD, 0x00C0, 0x00D6,
	0x00D8, 0x00F6, 0x00F8, 0x01F5, 0x01FA, 0x0217, 0x0250, 0x02A8,
	0x1E00, 0x1E9B, 0x1EA0, 0x1EF9, 0x0AAA, 0x0AB0, 0x0AB2, 0x0AB3,
	0x0AB5, 0x0AB9, 0x0ABD, 0x0AC5,
	0x0B01, 0x0B03, 0x0B05, 0x0B0C, 0x0B0F, 0x0B10, 0x0B13, 0x0B28,
	0x0B2A, 0x0B30, 0x0B32, 0x0B33, 0x0B36, 0x0B39, 0x0B3E, 0x0B43,
	0x0B47, 0x0B48, 0x0B4B, 0x0B4D, 0x0B5C, 0x0B5D, 0x0B5F, 0x0B61,
	0x0B82, 0x0B83, 0x0B85, 0x0B8A, 0x0B8E, 0x0B90, 0x0B92, 0x0B95,
	0x0B99, 0x0B9A, 0x0B9E, 0x0B9F, 0x0BA3, 0x0BA4, 0x0BA8, 0x0BAA,
	0x0BAE, 0x0BB5, 0x0BB7, 0x0BB9, 0x0BBE, 0x0BC2, 0x0BC6, 0x0BC8,
	0x0BCA, 0x0BCD, 0x0C01, 0x0C03, 0x0C05, 0x0C0C, 0x0C0E, 0x0C10,
	0x0C12, 0x0C28, 0x0C2A, 0x0C33, 0x0C35, 0x0C39, 0x0C3E, 0x0C44,
	0x0C46, 0x0C48, 0x0C4A, 0x0C4D, 0x0C60, 0x0C61, 0x0C82, 0x0C83,
	0x0C85, 0x0C8C, 0x0C8E, 0x0C90, 0x0C92, 0x0CA8, 0x0CAA, 0x0CB3,
	0x0CB5, 0x0CB9, 0x0CBE, 0x0CC4, 0x0CC6, 0x0CC8, 0x0CCA, 0x0CCD,
	0x0CE0, 0x0CE1, 0x0D02, 0x0D03, 0x0D05, 0x0D0C, 0x0D0E, 0x0D10,
	0x0D12, 0x0D28, 0x0D2A, 0x0D39, 0x0D3E, 0x0D43, 0x0D46, 0x0D48,
	0x0D4A, 0x0D4D, 0x0D60, 0x0D61, 0x0E01, 0x0E3A, 0x0E40, 0x0E5B,
	0x0E81, 0x0E82, 0x0E87, 0x0E88, 0x0E94, 0x0E97, 0x0E99, 0x0E9F,
	0x0EA1, 0x0EA3, 0x0EAA, 0x0EAB, 0x0EAD, 0x0EAE, 0x0EB0, 0x0EB9,
	0x0EBB, 0x0EBD, 0x0EC0, 0x0EC4, 0x0EC8, 0x0ECD, 0x0EDC, 0x0EDD,
	0x0F18, 0x0F19, 0x0F3E, 0x0F47, 0x0F49, 0x0F69, 0x0F71, 0x0F84,
	0x0F86, 0x0F8B, 0x0F90, 0x0F95, 0x0F99, 0x0FAD, 0x0FB1, 0x0FB7,
	0x10A0, 0x10C5, 0x10D0, 0x10F6, 0x3041, 0x3093, 0x309B, 0x309C,
	0x30A1, 0x30F6, 0x30FB, 0x30FC, 0x3105, 0x312C, 0x4E00, 0x9FA5,
	0xAC00, 0xD7A3, 0x0660, 0x0669, 0x06F0, 0x06F9, 0x0966, 0x096F,
	0x09E6, 0x09EF, 0x0A66, 0x0A6F, 0x0AE6, 0x0AEF, 0x0B66, 0x0B6F,
	0x0BE7, 0x0BEF, 0x0C66, 0x0C6F, 0x0CE6, 0x0CEF, 0x0D66, 0x0D6F,
	0x0E50, 0x0E59, 0x0ED0, 0x0ED9, 0x0F20, 0x0F33, 0x02B0, 0x02B8,
	0x02BD, 0x02C1, 0x02D0, 0x02D1, 0x02E0, 0x02E4, 0x203F, 0x2040,
	0x210A, 0x2113, 0x2118, 0x211D, 0x212A, 0x2131, 0x2133, 0x2138,
	0x2160, 0x2182, 0x3005, 0x3007, 0x3021, 0x3029, 0};

	int i;

	if((c>='a') && (c<='z'))return(1);
	if((c>='A') && (c<='Z'))return(1);
	if(c=='_')return(1);
	if(c=='$')return(1);
	
	if(lang==BGBCC_LANG_ASM)
	{
		if(c=='.')return(1);
//		if(c=='/')return(1);
		if(c=='@')return(1);
	}

//	if((c>='0') && (c<='9'))return(0);
	if(c<128)return(0);

	for(i=0; vals[i]; i++)
		if(c==vals[i])return(1);

	for(i=0; rngs[i*2+0]; i++)
		if((c>=rngs[i*2+0]) && (c<=rngs[i*2+1]))
			return(1);

	return(0);
}

int BGBCP_NameChar(int c, int lang)
{
	if((c>='0') && (c<='9'))return(1);
	if(BGBCP_NameInitChar(c, lang))return(1);

	if(lang==BGBCC_LANG_ASM)
	{
		if(c=='.')return(1);
		if(c=='/')return(1);
		if(c=='-')return(1);
		if(c=='@')return(1);
	}

	return(0);
}

char *BGBCP_TokenI(char *s, char *b, int *ty, int lang, int sz)
{
	char *t, *s1, *te;
	int i, j, k, sti;

	*b=0;
	s=BGBCP_EatWhite(s);
	if(!*s)
	{
		*ty=BTK_NULL;
		return(s);
	}
	t=b; te=t+sz-2;

	i=BGBCP_PeekChar(&s);
	if(BGBCP_NameInitChar(i, lang))
	{
		while(t<te)
		{
			s1=s;
			i=BGBCP_ParseChar(&s);
			if(!BGBCP_NameChar(i, lang))
				{ s=s1; break; }
			BGBCP_EmitChar(&t, i);
		}

		*t++=0;

		*ty=BTK_NAME;
		return(s);
	}

	if((*s>='0') && (*s<='9'))
	{
		if((s[0]=='0') && (s[1]=='x'))
		{
			*t++=*s++;
			*t++=*s++;
			while(((*s>='0') && (*s<='9')) ||
				((*s>='A') && (*s<='F')) ||
				((*s>='a') && (*s<='f')) ||
				(*s=='_') || (*s=='`'))
			{
				if((*s=='_') || (*s=='`'))
					{ s++; continue; }
				*t++=*s++;
			}
			*t++=0;

			*ty=BTK_NUMBER;
			return(s);
		}

#if 0
		if(*s=='-')*t++=*s++;
		while(((*s>='0') && (*s<='9')) || (*s=='.') || (*s=='e'))
		{
			if(*s=='e')
			{
				*t++=*s++;
				if((*s=='+') || (*s=='-'))
					*t++=*s++;
				continue;
			}
			*t++=*s++;
		}
		*t++=0;
#endif

		while(((*s>='0') && (*s<='9')) || (*s=='_') || (*s=='`'))
		{
			if((*s=='_') || (*s=='`'))
				{ s++; continue; }
			*t++=*s++;
		}
		if(*s=='.')*t++=*s++;
		while(((*s>='0') && (*s<='9')) || (*s=='_') || (*s=='`'))
		{
			if((*s=='_') || (*s=='`'))
				{ s++; continue; }
			*t++=*s++;
		}
		if(*s=='e')
		{
			*t++=*s++;
			if((*s=='+') || (*s=='-'))*t++=*s++;
			while((*s>='0') && (*s<='9'))
				*t++=*s++;
		}

		*t++=0;

		*ty=BTK_NUMBER;
		return(s);
	}

	if((s[0]=='.') && (s[1]>='0') && (s[1]<='9'))
	{
		*t++='0';
		*t++='.';

		while((*s>='0') && (*s<='9'))
			*t++=*s++;
		if(*s=='e')
		{
			*t++=*s++;
			if((*s=='+') || (*s=='-'))*t++=*s++;
			while((*s>='0') && (*s<='9'))
				*t++=*s++;
		}

		*t++=0;

		*ty=BTK_NUMBER;
		return(s);
	}

	if((*s=='"') || (*s=='\'')) /* quoted string */
	{
		if(*s=='\'')sti=1;
			else sti=0;
		s++;
		while(*s && (t<te))
		{
			if(!sti)if(*s=='"')break;
			if(sti)if(*s=='\'')break;

//			if((s[0]=='\\') && ((s[1]=='x') ||
//				(s[1]=='u') || (s[1]=='U')))
			if((s[0]=='\\') && (s[1]=='x'))
			{
#if 0
				k=2;
				if(s[1]=='u')k=4;

				s+=2; j=0;
				while(k--)
				{
					j<<=4;
					if((*s>='0') && (*s<='9'))j+=*s-'0';
					if((*s>='A') && (*s<='F'))j+=*s-'A'+10;
					if((*s>='a') && (*s<='f'))j+=*s-'a'+10;
					s++;
				}
#endif

				s+=2; j=0;
				while(*s)
				{
					k=-1;
					if((*s>='0') && (*s<='9'))k=*s-'0';
					if((*s>='A') && (*s<='F'))k=*s-'A'+10;
					if((*s>='a') && (*s<='f'))k=*s-'a'+10;
					if(k<0)break;
					j=(j<<4)|k;
					s++;
				}

				BGBCP_EmitChar(&t, j);
				continue;
			}

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
				BGBCP_EmitChar(&t, j);
				continue;
			}

			if(*s=='\\')
			{
				s++;
				switch(*s++)
				{
				case 'a':
					*t++='\b';
					break;
				case 'b':
					*t++='\b';
					break;
				case 't':
					*t++='\t';
					break;
				case 'n':
					*t++='\n';
					break;
				case 'v':
					*t++='\v';
					break;
				case 'f':
					*t++='\f';
					break;
				case 'r':
					*t++='\r';
					break;
				case 'e':
					*t++='\x1B';
					break;
				case '\\':
					*t++='\\';
					break;
				case '"':
					*t++='"';
					break;
				case '\'':
					*t++='\'';
					break;
				case '?':
					*t++='?';
					break;
				default:
					break;
				}
				continue;
			}

//			*t++=*s++;
			i=BGBCP_ParseChar(&s);
			BGBCP_EmitChar(&t, i);
		}
		*t++=0;
		if(*s)s++;

		*ty=BTK_STRING;
		if(sti)*ty=BTK_CHARSTRING;
		return(s);
	}

	if(!strncmp(s, "...", 3))
	{
		*t++=*s++;
		*t++=*s++;
		*t++=*s++;
		*t++=0;

		*ty=BTK_MISC;
		return(s);
	}

	if((*s==',') || (*s==';') || ((*s=='.') && (s[1]!='.')))
	{
		*t++=*s++;
		*t++=0;

//		kprint("token: %p '%s'\n", s, b);
		*ty=BTK_SEPERATOR;
		return(s);
	}

	if(	!strncmp(s, "#<`", 3) || !strncmp(s, "#{`", 3) ||
		!strncmp(s, "#[`", 3) || !strncmp(s, "#(`", 3)
		)
	{
		*t++=*s++; *t++=*s++; *t++=*s++; *t++=0;
		*ty=BTK_BRACE;
		return(s);
	}

	if(	!strncmp(s, "<:", 2) || !strncmp(s, ":>", 2) ||
		!strncmp(s, "<%", 2) || !strncmp(s, "%>", 2) ||
		!strncmp(s, "<`", 2) || !strncmp(s, "`>", 2) ||
		!strncmp(s, "{`", 2) || !strncmp(s, "`}", 2) ||
		!strncmp(s, "[`", 2) || !strncmp(s, "`]", 2) ||
		!strncmp(s, "(`", 2) || !strncmp(s, "`)", 2) ||
		!strncmp(s, "#<", 2) || !strncmp(s, "#{", 2) ||
		!strncmp(s, "#[", 2) || !strncmp(s, "#(", 2)
		)
	{
		*t++=*s++; *t++=*s++; *t++=0;
		*ty=BTK_BRACE;
		return(s);
	}
		

	if(	(*s=='{') || (*s=='}') ||
		(*s=='[') || (*s==']') ||
		(*s=='(') || (*s==')'))
	{
		*t++=*s++;
		*t++=0;

//		kprint("token: %p '%s'\n", s, b);
		*ty=BTK_BRACE;
		return(s);
	}

	if(BGBCP_OpChar(*s))
	{
		*ty=BTK_OPERATOR;

//		if(lang==BGBCC_LANG_ASM)
		if(0)
		{
			*t++=*s++;
			*t++=0;
			if(*b==':')*ty=BTK_SEPERATOR;
			return(s);
		}

		if(lang==BGBCC_LANG_JAVA)
		{
			if( !strncmp(s, "<<<=", 4) ||
				!strncmp(s, ">>>=", 4) )
			{
				*t++=*s++;
				*t++=*s++;
				*t++=*s++;
				*t++=*s++;
				*t++=0;
				return(s);
			}else if(	!strncmp(s, "<<<", 3) ||
						!strncmp(s, ">>>", 3) )
			{
				*t++=*s++;
				*t++=*s++;
				*t++=*s++;
				*t++=0;
				return(s);
			}
		}

		if( !strncmp(s, "%:%:", 4) )
		{
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
		}else if(	!strncmp(s, "<<=", 3) ||
			!strncmp(s, ">>=", 3)
			)
		{
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
		}else if(
			!strncmp(s, "==", 2) ||
			!strncmp(s, "<=", 2) ||
			!strncmp(s, ">=", 2) ||
			!strncmp(s, "!=", 2) ||
			!strncmp(s, "<<", 2) ||
			!strncmp(s, ">>", 2) ||
			!strncmp(s, "&&", 2) ||
			!strncmp(s, "||", 2) ||

			!strncmp(s, "++", 2) ||
			!strncmp(s, "--", 2) ||

			!strncmp(s, "+=", 2) ||
			!strncmp(s, "-=", 2) ||
			!strncmp(s, "*=", 2) ||
			!strncmp(s, "/=", 2) ||
			!strncmp(s, "%=", 2) ||
			!strncmp(s, "&=", 2) ||
			!strncmp(s, "|=", 2) ||
			!strncmp(s, "^=", 2) ||

			!strncmp(s, "%:", 2) ||

			!strncmp(s, "+`", 2) ||
			!strncmp(s, "-`", 2) ||
			!strncmp(s, "*`", 2) ||
			!strncmp(s, "/`", 2) ||
			!strncmp(s, "%`", 2) ||
			!strncmp(s, "&`", 2) ||
			!strncmp(s, "|`", 2) ||
			!strncmp(s, "^`", 2) ||

			!strncmp(s, "`+", 2) ||
			!strncmp(s, "`-", 2) ||
			!strncmp(s, "`*", 2) ||
			!strncmp(s, "`/", 2) ||
			!strncmp(s, "`%", 2) ||
			!strncmp(s, "`&", 2) ||
			!strncmp(s, "`|", 2) ||
			!strncmp(s, "`^", 2) ||

			!strncmp(s, "!`", 2) ||
			!strncmp(s, "`!", 2) ||
			!strncmp(s, "~`", 2) ||
			!strncmp(s, "`~", 2) ||

			!strncmp(s, "``", 2) ||

			!strncmp(s, "##", 2) ||
			!strncmp(s, "->", 2)
			)
		{
			*t++=*s++;
			*t++=*s++;
			*t++=0;
		}else
		{
			*t++=*s++;
			*t++=0;

			if(*b==':')*ty=BTK_SEPERATOR;
		}

//		kprint("token: %p '%s'\n", s, b);
//		*ty=BTK_OPERATOR;
//		if(!strcmp(b, ":"))*ty=BTK_SEPERATOR;

		return(s);
	}

	BGBCP_Error(s, "BGBCP_Token: Parse Error Char '%c'\n", *s);
	*ty=BTK_NULL;
	return(NULL);
}

#if 1

int bgbcp_token_cnt;

#define BGBCP_TKHASHSZ 256
char *BGBCP_TokenLang2(char *s, char *b, int *ty, int lang)
{
	static char ltb[BGBCP_TKHASHSZ][256];
	static char *ls[BGBCP_TKHASHSZ];
	static char *ls1[BGBCP_TKHASHSZ];
	static int lty[BGBCP_TKHASHSZ];
	static int flush=0;
	char *tb;
	int i;
	
	if(!s)
	{
		if(b && ty)
		{
			*b=0;
			*ty=BTK_NULL;
		}

		if(!flush)
		{
			for(i=0; i<BGBCP_TKHASHSZ; i++)
				ls[i]=NULL;
		}
		bgbcp_token_cnt=0;
		flush=64;
		return(NULL);
	}

	if(flush)
	{
		flush--;
		bgbcp_token_cnt++;
		return(BGBCP_TokenI(s, b, ty, lang, 256));
	}

//	i=(((nlint)s)*7)&63;
//	i=((((nlint)s)*31)>>6)&(BGBCP_TKHASHSZ-1);
	i=((nlint)s)&(BGBCP_TKHASHSZ-1);
	tb=ltb[i];

	if(ls[i]==s)
	{
		tb[255]=0;

		strcpy(b, tb);
		*ty=lty[i];
		return(ls1[i]);
	}
	
	bgbcp_token_cnt++;
	ls[i]=s;
	ls1[i]=BGBCP_TokenI(s, tb, &(lty[i]), lang, 256);
	strcpy(b, tb);
	*ty=lty[i];
	return(ls1[i]);
}

char *BGBCP_Token(char *s, char *b, int *ty)
{
	return(BGBCP_TokenLang2(s, b, ty, BGBCC_LANG_C));
}

void BGBCP_FlushToken(char *s)
{
	BGBCP_Token(NULL, NULL, NULL);
}

int BGBCP_GetTokenCount(void)
{
	return(bgbcp_token_cnt);
}
#else

char *BGBCP_Token(char *s, char *b, int *ty)
	{ return(BGBCP_TokenI(s, b, ty, BGBCC_LANG_C, 256)); }
void BGBCP_FlushToken(char *s)
	{ }

#endif

char *BGBCP_TokenGen(char *s, char *b, int *ty)
{
	char *t;
	int i, j, k, sti;

	*b=0;
	s=BGBCP_EatWhite(s);
	if(!*s)
	{
		*ty=BTK_NULL;
		return(s);
	}
	t=b;

	if(((*s)=='<') || ((*s)=='>'))
	{
		*t++=*s++;
		*t++=0;
		*ty=BTK_BRACE;
		return(s);
	}

	return(BGBCP_Token(s, b, ty));
}

char *BGBCP_TokenLang(char *s, char *b, int *ty, int lang)
{
	return(BGBCP_TokenI(s, b, ty, lang, 256));
}

char *BGBCP_TokenCtx(BGBCP_ParseState *ctx, char *s, char *b, int *ty)
{
	return(BGBCP_TokenI(s, b, ty, ctx->lang, 256));
}

#if 1
#define BGBCP_TKHASHSZ 256

char *BGBCP_ReadToken(BGBCP_ParseState *ctx, char **str, int *ty)
{
	static char *lts[BGBCP_TKHASHSZ];
	static char *ls[BGBCP_TKHASHSZ];
	static char *ls1[BGBCP_TKHASHSZ];
	static int lty[BGBCP_TKHASHSZ];
	static int flush=0;
	static char *tb=NULL;
	char *s, *t;
	int i;
	
	if(!str)
	{
		if(!flush)
		{
			for(i=0; i<BGBCP_TKHASHSZ; i++)
				ls[i]=NULL;
		}
		flush=64;
		return(NULL);
	}

	if(!tb)tb=malloc(65536);

	if(flush)
	{
		flush--;
		*str=BGBCP_TokenI(*str, tb, ty, ctx->lang, 4096);
		return(bgbcc_strdup(tb));
	}

	s=*str;

//	i=(((nlint)s)*7)&63;
//	i=((((nlint)s)*31)>>6)&(BGBCP_TKHASHSZ-1);
	i=((nlint)s)&(BGBCP_TKHASHSZ-1);
	t=lts[i];

	if(ls[i]==s)
	{
		*str=ls1[i];
		*ty=lty[i];
		return(t);
	}
	
	ls[i]=s;
	ls1[i]=BGBCP_TokenI(s, tb, &(lty[i]), ctx->lang, 4096);
	lts[i]=bgbcc_strdup(tb);
	*str=ls1[i];
	*ty=lty[i];
	return(lts[i]);
}
#endif
