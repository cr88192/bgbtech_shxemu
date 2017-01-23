char *BTSHASP_EatWhiteOnly(char *s)
{
	while(*s && (*s<=' '))
		s++;
	return(s);
}

char *BTSHASP_EatWhiteOnlyNoLinebreak(char *s)
{
	while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))
		s++;
	return(s);
}

char *BTSHASP_EatWhite(char *s)
{
	int i;
	while(*s && (*s<=' '))
		s++;
	if((s[0]=='/') && (s[1]=='/'))
	{
		s+=2;
		while(*s && (*s!='\r') && (*s!='\n'))
			s++;
		s=BTSHASP_EatWhite(s);
		return(s);
	}

	if((s[0]=='/') && (s[1]=='*'))
	{
		s+=2; i=1;
		while(*s && (i>0))
		{
			if((s[0]=='/') && (s[1]=='*'))
				{ i++; s+=2; continue; }
			if((s[0]=='*') && (s[1]=='/'))
				{ i--; s+=2; continue; }
			s++;
		}
		s=BTSHASP_EatWhite(s);
		return(s);
	}
	return(s);
}

char *BTSHASP_EatWhiteNoLinebreak(char *s)
{
	int i;
	while(*s && (*s<=' ') && (*s!='\r') && (*s!='\n'))
		s++;
	if((s[0]=='/') && (s[1]=='/'))
	{
		s+=2;
		while(*s && (*s!='\r') && (*s!='\n'))
			s++;
//		s=BTSHASP_EatWhiteNoLinebreak(s);
		return(s);
	}

	if((s[0]=='/') && (s[1]=='*'))
	{
		s+=2; i=1;
		while(*s && (i>0))
		{
			if((s[0]=='/') && (s[1]=='*'))
				{ i++; s+=2; continue; }
			if((s[0]=='*') && (s[1]=='/'))
				{ i--; s+=2; continue; }
			s++;
		}
		s=BTSHASP_EatWhiteNoLinebreak(s);
		return(s);
	}
	return(s);
}

int BTSHASP_ParseHexN(char *s, int n)
{
	int i, j, k;
	
	k=0;
	for(i=0; i<n; i++)
	{
		j=0;
		if((*s>='0') && (*s<='9'))
			j=*s-'0';
		if((*s>='A') && (*s<='F'))
			j=10+(*s-'A');
		if((*s>='a') && (*s<='f'))
			j=10+(*s-'a');
		s++;
		k=(k<<4)|j;
	}
	return(k);
}

char *BTSHASP_ReadUtf8(char *sb, int *rv)
{
	byte *s;
	int v;
	
	s=(byte *)sb;
	v=*s++;
	if(v<0x80)
		{ *rv=v; return((char *)s); }
	if(v<0xC0)
		{ *rv=-(v&63); return((char *)s); }
	if(v<0xE0)
	{
		v=((v&0x1F)<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xF0)
	{
		v=((v&0x0F)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xF8)
	{
		v=((v&0x07)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xFC)
	{
		v=((v&0x03)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	if(v<0xFE)
	{
		v=((v&0x01)<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		v=(v<<6)|((*s++)&63);
		*rv=v;
		return((char *)s);
	}
	
	return(NULL);
}

char *BTSHASP_EmitUtf8(char *t, int v)
{
	if(v<0x80)
	{
		*t++=v;
		return(t);
	}
	if(v<0x800)
	{
		*t++=0xC0|(v>>6);
		*t++=0x80|(v&0x3F);
		return(t);
	}
	if(v<0x10000)
	{
		*t++=0xE0|(v>>12);
		*t++=0x80|((v>>6)&0x3F);
		*t++=0x80|(v&0x3F);
		return(t);
	}
	if(v<0x200000)
	{
		*t++=0xF0|(v>>18);
		*t++=0x80|((v>>12)&0x3F);
		*t++=0x80|((v>>6)&0x3F);
		*t++=0x80|(v&0x3F);
		return(t);
	}
	
	return(NULL);
}

char *BTSHASP_ParseTokenBasic(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=BTSHASP_EatWhite(s);
	if(!(*s))
	{
		*t++=0; *t++=0;
		return(s);
	}
	
	if((*s=='_') || (*s=='$') || ((*s>='A') && (*s<='Z')) ||
		((*s>='a') && (*s<='z')))
	{
		*t++='I';
		while(*s && (t<te) &&
			((*s=='_') || (*s=='$') ||
			((*s>='A') && (*s<='Z')) ||
			((*s>='a') && (*s<='z')) ||
			((*s>='0') && (*s<='9'))))
				*t++=*s++;
		*t++=0;
		return(s);
	}
	
	if((*s>='0') && (*s<='9'))
	{
		*t++='#';

		*t++=*s++;
//		if(*s=='x')
		if((*s=='x') || (*s=='b') || (*s=='c') || (*s=='d'))
		{
			*tb='|';
			
			*t++=*s++;

			while(*s && (t<te) &&
				((*s=='_') ||
				((*s>='0') && (*s<='9')) ||
				((*s>='A') && (*s<='F')) ||
				((*s>='a') && (*s<='f'))))
					*t++=*s++;
			*t++=0;
			return(s);
		}

		while(*s && (t<te) && ((*s=='_') || ((*s>='0') && (*s<='9'))))
			*t++=*s++;
		if(*s=='.' && (*(s+1)!='.'))
		{
			*t++=*s++;
			while(*s && (t<te) && ((*s=='_') || ((*s>='0') && (*s<='9'))))
				*t++=*s++;
			if((*s=='e') || (*s=='E'))
			{
				*t++=*s++;
				if((*s=='+') || (*s=='-'))
					*t++=*s++;
				while(*s && (t<te) &&
					((*s=='_') || ((*s>='0') && (*s<='9'))))
						*t++=*s++;
			}
		}else
		{
			*tb='|';
		}
		*t++=0;
		return(s);
	}
	
	if(*s=='\"')
	{
		if((s[1]=='\"') && (s[2]=='\"'))
		{
			*t++='X';
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
			return(s);
		}
	
		s++;
		*t++='S';
		while(*s && (t<te))
		{
			if(*s=='\\')
			{
				s++;
				switch(*s)
				{
				case 'n': s++; *t++='\n'; break;
				case 'r': s++; *t++='\r'; break;
				case 'b': s++; *t++='\b'; break;
//				case 'e': s++; *t++='\e'; break;
				case 't': s++; *t++='\t'; break;
				case '\\': s++; *t++='\\'; break;
				case '\"': s++; *t++='\"'; break;
				case '\'': s++; *t++='\''; break;
				case 'x':
					s++;
					*t++=BTSHASP_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BTSHASP_ParseHexN(s, 4);
					t=BTSHASP_EmitUtf8(t, i);
					s+=4;
					break;
				}
				continue;
			}
			if(*s=='\"')
				break;
			*t++=*s++;
		}
		if(*s=='\"')
			s++;
		*t++=0;
		return(s);
	}

	if(*s=='\'')
	{
		s++;
		*t++='C';
		while(*s && (t<te))
		{
			if(*s=='\\')
			{
				s++;
				switch(*s)
				{
				case 'n': s++; *t++='\n'; break;
				case 'r': s++; *t++='\r'; break;
				case 'b': s++; *t++='\b'; break;
//				case 'e': s++; *t++='\e'; break;
				case 't': s++; *t++='\t'; break;
				case '\\': s++; *t++='\\'; break;
				case '\"': s++; *t++='\"'; break;
				case '\'': s++; *t++='\''; break;
				case 'x':
					s++;
					*t++=BTSHASP_ParseHexN(s, 2);
					s+=2;
					break;
				case 'u':
					s++;
					i=BTSHASP_ParseHexN(s, 4);
					t=BTSHASP_EmitUtf8(t, i);
					s+=4;
					break;
				}
				continue;
			}
			if(*s=='\'')
				break;
			*t++=*s++;
		}
		if(*s=='\'')
			s++;
		*t++=0;
		return(s);
	}
	
	*t++='X';
	switch(*s)
	{
	case '-':
		*t++=*s++;
		if((*s=='-') || (*s=='=') || (*s=='>'))
			{ *t++=*s++; }
		*t++=0;
		break;
	case '+':
		*t++=*s++;
		if((*s=='+') || (*s=='='))
			{ *t++=*s++; }
		*t++=0;
		break;

	case '=':	case '!':
	case '/':	case '*':
	case '%':	case '^':
		*t++=*s++;
		if(*s=='=')
			*t++=*s++;
		*t++=0;
		break;

	case '|':
		*t++=*s++;
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '|':
			*t++=*s++;
			switch(*s)
			{
			case '=': *t++=*s++; break;
			default: break;
			}
			break;
		default: break;
		}
		*t++=0;
		break;

	case '&':
		*t++=*s++;
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '&':
			*t++=*s++;
			switch(*s)
			{
			case '=': *t++=*s++; break;
			default: break;
			}
			break;
		default: break;
		}
		*t++=0;
		break;

	case '<':
		*t++=*s++;
//		if((*s=='=') || (*s=='<'))
//			{ *t++=*s++; }
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '<':
			*t++=*s++;
			switch(*s)
			{
			case '<':
			case '=': *t++=*s++; break;
			default: break;
			}
			break;
		default: break;
		}
		*t++=0;
		break;
	case '>':
		*t++=*s++;
		switch(*s)
		{
		case '=': *t++=*s++; break;
		case '>':
			*t++=*s++;
			switch(*s)
			{
			case '>':
			case '=': *t++=*s++; break;
			default: break;
			}
//			if(*s=='>')
//				*t++=*s++;
			break;
		default: break;
		}
//		if((*s=='=') || (*s=='>'))
//			{ *t++=*s++; }
		*t++=0;
		break;

	case '#':
		*t++=*s++;
		switch(*s)
		{
		case '#': *t++=*s++; break;
		case ':': *t++=*s++; break;
		case '(': *t++=*s++; break;
		case ')': *t++=*s++; break;
		case '[': *t++=*s++; break;
		case ']': *t++=*s++; break;
		case '{': *t++=*s++; break;
		case '}': *t++=*s++; break;
		default: break;
		}
		*t++=0;
		break;
	
	case '~': case '?':
	case '@':
	case '\\':
	case ';': case ':':
	case ',':
		*t++=*s++;
		*t++=0;
		break;
	
	case '.':
		if((s[1]=='.') && (s[2]=='.'))
		{
			*t++=*s++;
			*t++=*s++;
			*t++=*s++;
			*t++=0;
			break;
		}
		if(s[1]=='.')
		{
			*t++=*s++;
			*t++=*s++;
			*t++=0;
			break;
		}
		*t++=*s++;
		*t++=0;
		break;

	case '(': case ')':
	case '[': case ']':
	case '{': case '}':
		*t++=*s++;
		*t++=0;
		break;

	default:
		*t++=*s++;
		*t++=0;
		break;
	}
	return(s);
}

char *BTSHASP_ParseTokenBasicAltn(char *s, char *tb)
{
	char *t, *te;
	int i, j, k;

	t=tb; te=tb+252;

	s=BTSHASP_EatWhite(s);
	if(!(*s))
	{
		*t++=0; *t++=0;
		return(s);
	}
	
	if((*s=='_') || (*s=='$') ||
		(*s=='.') || (*s=='/') || 
		((*s>='A') && (*s<='Z')) ||
		((*s>='a') && (*s<='z')))
	{
		*t++='I';
		while(*s && (t<te) &&
			((*s=='_') || (*s=='$') ||
			 (*s=='.') || (*s=='/') ||
			((*s>='A') && (*s<='Z')) ||
			((*s>='a') && (*s<='z')) ||
			((*s>='0') && (*s<='9'))))
				*t++=*s++;
		*t++=0;
		return(s);
	}
	
	if((*s=='-') &&
		((s[1]>='0') && (s[1]<='9')))
	{
		*t++='|';
		*t++=*s++;
		while(*s && (t<te) && ((*s=='_') || ((*s>='0') && (*s<='9'))))
			*t++=*s++;
		*t++=0;
		return(s);
	}

	s=BTSHASP_ParseTokenBasic(s, tb);
	return(s);
}

char *BTSHASP_ParseToken(char *cs, char **rstr)
{
	char tb[256];
	cs=BTSHASP_ParseTokenBasic(cs, tb);
	*rstr=btshas_rstrdup(tb);
	return(cs);
}

char *BTSHASP_ParseTokenAlt(char *cs, char **rstr)
{
	char tb[256];
	cs=BTSHASP_ParseTokenBasicAltn(cs, tb);
	*rstr=btshas_rstrdup(tb);
	return(cs);
}

int BTSHASP_FlushToken()
{
	return(0);
}

int BTSHASP_CalcLinenum(char *ptr)
{
	return(0);
}

int BTSHASP_PushLinenum()
{
	return(0);
}

int BTSHASP_PopLinenum()
{
	return(0);
}

char *BTSHASP_GetFilename()
{
	return("");
}

int BTSHASP_SetLinenum(char *cfn, char *cbuf, int cln)
{
	return(0);
}

int BTSHASP_OpChar(int ch)
{
	if(ch<=' ')
		return(0);
	if((ch>='0') && (ch<='9'))
		return(0);
	if((ch>='A') && (ch<='Z'))
		return(0);
	if((ch>='a') && (ch<='z'))
		return(0);
	return(1);
}
