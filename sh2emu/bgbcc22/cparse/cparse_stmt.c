#include <bgbccc.h>

#ifdef WIN32
#include <windows.h>
// #define CATCH_BLOCK_SEH
#endif

BGBCP_ParseItem *bgbcp_stmts=NULL;
BGBCP_ParseItem *bgbcp_blkstmts=NULL;

char *bgbcp_curfilename=NULL;

int bgbcp_warn, bgbcp_err;

void BGBCP_Warn(char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_warn++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: warning: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCP_Error(char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_err++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: error: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
	fflush(stdout);

//	*(int *)-1=-1;
	BGBCC_DieError();
}

void BGBCP_WarnCtx(BGBCP_ParseState *ctx, char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_warn++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: warning: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
	fflush(stdout);

//	*(int *)-1=-1;
//	BGBCC_DieError();
}

void BGBCP_ErrorCtx(BGBCP_ParseState *ctx, char *se, char *str, ...)
{
	va_list lst;
	char *fn;
	int ln;

	va_start(lst, str);

	bgbcp_err++;
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

	printf("%s:%d: error: ", fn, ln);
	vprintf(str, lst);

	va_end(lst);
	fflush(stdout);

//	*(int *)-1=-1;
	BGBCC_DieError();
}

int BGBCP_AddStatement(char *name,
	BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s))
{
	BGBCP_ParseItem *tmp;

	tmp=malloc(sizeof(BGBCP_ParseItem));
	tmp->name=strdup(name);
	tmp->func=func;

	tmp->next=bgbcp_stmts;
	bgbcp_stmts=tmp;

	return(0);
}

BCCX_Node *BGBCP_ParseStatementName(
	BGBCP_ParseState *ctx, char *name, char **s)
{
	BGBCP_ParseItem *cur;

	cur=bgbcp_stmts;
	while(cur)
	{
		if(!strcmp(name, cur->name))
			return(cur->func(ctx, s));
		cur=cur->next;
	}
	return(NULL);
}

int BGBCP_AddBlockStatement(char *name,
	BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s))
{
	BGBCP_ParseItem *tmp;

	tmp=malloc(sizeof(BGBCP_ParseItem));
	tmp->name=strdup(name);
	tmp->func=func;

	tmp->next=bgbcp_blkstmts;
	bgbcp_blkstmts=tmp;

	return(0);
}

BCCX_Node *BGBCP_ParseBlockStatementName(
	BGBCP_ParseState *ctx, char *name, char **s)
{
	BGBCP_ParseItem *cur;

	cur=bgbcp_blkstmts;
	while(cur)
	{
		if(!strcmp(name, cur->name))
			return(cur->func(ctx, s));
		cur=cur->next;
	}
	return(NULL);
}

char *BGBCP_EatSemicolon(char *s)
{
	if(!s)
		return(s);
	s=BGBCP_EatWhite(s);
	if(*s==';')s++;
	s=BGBCP_EatWhite(s);
	return(s);
}

char *BGBCP_EatSemicolonRequired(BGBCP_ParseState *ctx, char *s)
{
	if(!s)
		return(s);
	s=BGBCP_EatWhite(s);
	if(*s==';')
	{
		s++;
	}else
	{
		BGBCP_ErrorCtx(ctx, s, "Missing expected ';'\n");
	}
	s=BGBCP_EatWhite(s);
	return(s);
}

BCCX_Node *BGBCP_Statement(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2;

	s=*str;
	s=BGBCP_Token(s, b, &ty);
	BGBCP_Token(s, b2, &ty2);
	if(!*s)
	{
		*str=s;
		return(NULL);
	}

	if(ty!=BTK_NAME)
	{
		s=*str;
//		n=BGBCP_Expression(ctx, &s);
		n=BGBCP_Expression2(ctx, &s);
		*str=s;
		return(n);
	}

	n=BGBCP_ParseStatementName(ctx, b, &s);
	if(n!=NULL)
	{
		*str=s;
		return(n);
	}

	if(!strcmp(b, "break"))
	{
		n=BCCX_New("break");
		*str=s;
		return(n);
	}

	if(!strcmp(b, "continue"))
	{
		n=BCCX_New("continue");
		*str=s;
		return(n);
	}

	if(!strcmp(b, "goto"))
	{
		s=BGBCP_Token(s, b2, &ty2);
//		n=BCCX_New1("goto", BCCX_NewText(b2));
		n=BCCX_New("goto");
		BCCX_Set(n, "name", b2);
		*str=s;
		return(n);
	}

	if(!strcmp(b, "return"))
	{
		BGBCP_Token(s, b2, &ty2);
		if(b2[0]!=';')
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_New1("return", BCCX_New1V("value", n1));
		}else n=BCCX_New("return");

		*str=s;
		return(n);
	}


	s=*str;
	s=BGBCP_EatWhite(s);
	if(!s || !*s)return(NULL);

//	n=BGBCP_Expression(ctx, &s);
	n=BGBCP_Expression2(ctx, &s);
	if(n!=NULL)
	{
		*str=s;
		return(n);
	}

	BGBCP_Error(s, "PDSCR_CParse_Statement: Unrecognizable "
		"statement type\n");
	*str=NULL;
	return(NULL);
}

BCCX_Node *BGBCP_ParseComment(BGBCP_ParseState *ctx, char **str)
{
	char b[65536];
	char *s, *t, *s0, *s1, *s2;
	BCCX_Node *n, *n1;
	int i;

	s=*str;

#if 0
	s1=s;
	while(*s1 && (*s1<=' '))s1++;
	if((s1[0]=='/') && (s1[1]=='/'))
	{
		s=s1+2; t=b;
		while(*s && (*s!='\n')) *t++=*s++;
		*t++=0;

		n=BCCX_New("linecomment");
		BCCX_Set(n, "value", b);
		*str=s;
		return(n);
	}

	if((s1[0]=='/') && (s1[1]=='*'))
	{
		s=s1+2; t=b;
		while(*s)
		{
			if((s[0]=='*') && (s[1]=='/'))
				{ s+=2; break; }
			*t++=*s++;
		}
		*t++=0;

		if(BGBCP_SkimLinenum(s1, NULL, NULL))
		{
			//n=BGBCP_BlockStatementInner(ctx, &s);
			*str=s;
			return(NULL);
		}

		n=BCCX_New("blockcomment");
		BCCX_Set(n, "value", b);
		*str=s;
		return(n);
	}
#endif

	return(NULL);
}

BCCX_Node *BGBCP_ParseDocComment(BGBCP_ParseState *ctx, char **str)
{
	char b[65536];
	char *s, *t, *s0, *s1, *s2;
	BCCX_Node *n, *n1;
	int i;

	s=*str;

	s1=s;
	while(*s1 && (*s1<=' '))s1++;
	if((s1[0]=='/') && (s1[1]=='*') && (s1[2]=='*') && (s1[3]!='*'))
	{
		s=s1+3; t=b;
		while(*s)
		{
			if((s[0]=='*') && (s[1]=='/'))
				{ s+=2; break; }
			*t++=*s++;
		}
		*t++=0;

		n=BCCX_New("doc_comment");
		BCCX_Set(n, "value", b);
		*str=s;
		return(n);
	}

	return(NULL);
}

BCCX_Node *BGBCP_BlockStatementInner(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *ab[16];
	char **a;
	char *s, *t, *s0, *s1, *s2;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3, *n4;
	int i;

	s=*str;

	n=BGBCP_ParseComment(ctx, &s);
	if(n)
	{
		*str=s;
		return(n);
	}

	s=BGBCP_Token(s, b, &ty);
	BGBCP_Token(s, b2, &ty2);
	if(!*s)
	{
		*str=s;
		return(NULL);
	}

	if(ty==BTK_SEPERATOR)
	{
		*str=s;
		return(NULL);
	}

	if((ty==BTK_NAME) && !strcmp(b2, ":"))
	{
		s=BGBCP_Token(s, b2, &ty2);	//:

		if(!strcmp(b, "default"))
		{
			n=BCCX_New("case_default");
			*str=s;
			return(n);
		}

		n=BCCX_New("label");
		BCCX_Set(n, "name", b);

		*str=s;
		return(n);
	}

	if(ty==BTK_NAME)
	{

		if(!strcmp(b, "typedef") && (ctx->lang==BGBCC_LANG_C))
		{
			n=BGBCP_ForceDefinition(ctx, &s);
			BGBCP_HandleTypedef(ctx, n);

			*str=s;
			return(NULL);
		}

		if(!strcmp(b, "case"))
		{
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b2, &ty2);	//:
			n=BCCX_New1("case", BCCX_New1V("value", n1));

			*str=s;
			return(n);
		}

		if(!strcmp(b, "if"))
		{
			s=BGBCP_Token(s, b, &ty);	//(
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty);	//)
			n2=BGBCP_BlockStatement3(ctx, &s);

			BGBCP_Token(s, b, &ty);
			if(strcmp(b, "else"))
			{
				n=BCCX_New2("if",
					BCCX_New1V("cond", n1),
					BCCX_New1V("then", n2));
				*str=s;
				return(n);
			}

			s=BGBCP_Token(s, b, &ty);	//else
			n3=BGBCP_BlockStatement3(ctx, &s);
			n=BCCX_New3("if",
				BCCX_New1V("cond", n1),
				BCCX_New1V("then", n2),
				BCCX_New1V("else", n3));

			*str=s;
			return(n);
		}

		if(!strcmp(b, "while"))
		{
			s=BGBCP_Token(s, b, &ty);	//'('
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty);	//')'

			n2=BGBCP_BlockStatement3(ctx, &s);
			n=BCCX_New2("while",
				BCCX_New1V("cond", n1),
				BCCX_New1V("body", n2));

			*str=s;
			return(n);
		}

		if(!strcmp(b, "for"))
		{
			s=BGBCP_Token(s, b, &ty);		//(
			s2=BGBCP_Token(s, b, &ty);		//
			s2=BGBCP_Token(s2, b2, &ty);	//

			if(!strcmp(b, ";"))
				{ n1=NULL; }
			else
				{ n1=BGBCP_Expression2(ctx, &s); }
			s=BGBCP_EatSemicolonRequired(ctx, s);

			s2=BGBCP_Token(s, b, &ty);
			if(!strcmp(b, ";"))
				{ n2=NULL; }
			else
				{ n2=BGBCP_Expression2(ctx, &s); }
			s=BGBCP_EatSemicolonRequired(ctx, s);

			s2=BGBCP_Token(s, b, &ty);
			if(!strcmp(b, ")"))
				{ n3=NULL; }
			else
				{ n3=BGBCP_Expression2(ctx, &s); }

			s=BGBCP_Token(s, b, &ty);	//)
			n4=BGBCP_BlockStatement3(ctx, &s);

			n=BCCX_New4("for",
				BCCX_New1V("init", n1),
				BCCX_New1V("cond", n2),
				BCCX_New1V("step", n3),
				BCCX_New1V("body", n4));

			*str=s;
			return(n);
		}

		if(!strcmp(b, "do"))
		{
			n2=BGBCP_BlockStatement3(ctx, &s);

			BGBCP_Token(s, b, &ty);	//'while'
			if(!strcmp(b, "while"))
			{
				s=BGBCP_Token(s, b, &ty);	//'while'

				s=BGBCP_Token(s, b, &ty);	//'('
				n1=BGBCP_Expression(ctx, &s);
				s=BGBCP_Token(s, b, &ty);	//')'

				n=BCCX_New2("do_while",
					BCCX_New1V("cond", n1),
					BCCX_New1V("body", n2));
			}else
			{
				n=n2;
			}

			*str=s;
			return(n);
		}

		if(!strcmp(b, "switch"))
		{
			s=BGBCP_Token(s, b, &ty);	//(
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty);	//)

			s=BGBCP_Token(s, b, &ty); //{
			n2=BGBCP_Block(ctx, &s);

			n=BCCX_New2("switch",
				BCCX_New1V("cond", n1),
				BCCX_New1V("body", n2));

			*str=s;
			return(n);
		}

		if(!strcmp(b, "__asm__"))
		{
//			s=BGBCP_Token(s, b, &ty);	//(
//			n1=BGBCP_Expression(ctx, &s);
//			s=BGBCP_Token(s, b, &ty);	//)

//			s=BGBCP_Token(s, b, &ty);	//(

			i=0;
			while(1)
			{
				s=BGBCP_Token(s, b, &ty);
				if(!strcmp(b, "("))i++;
				if(!strcmp(b, ")"))
				{
					i--;
					if(!i)break;
				}
			}

			n=BCCX_New("gcc_asm");

			*str=s;
			return(n);
		}

		if(!strcmp(b, "__asm"))
		{
			i=0;
			while(1)
			{
				s=BGBCP_Token(s, b, &ty);
				if(!strcmp(b, "{"))i++;
				if(!strcmp(b, "}"))
				{
					i--;
					if(!i)break;
				}
			}

			n=BCCX_New("msvc_asm");

			*str=s;
			return(n);
		}

		if(	!strcmp(b, "__extern_c") ||
			!strcmp(b, "__extern_cpp") ||
			!strcmp(b, "__extern_java") ||
			!strcmp(b, "__extern_cs") ||
			!strcmp(b, "__extern_bs") ||
			!strcmp(b, "__extern_bs2"))
		{
			i=ctx->lang;
			ctx->lang=BGBCP_LangForName(b+strlen("__extern_"));
			n1=BGBCP_BlockStatement2(ctx, &s);
			n=BCCX_New1("extern2_lang", n1);
			BCCX_Set(n, "lang", BGBCP_NameForLang(ctx->lang));

			ctx->lang=i;
			return(n);
		}

		if((ctx->lang==BGBCC_LANG_CPP) ||
			(ctx->lang==BGBCC_LANG_CS) ||
			(ctx->lang==BGBCC_LANG_CX))
		{
			if(!strcmp(b, "using"))
			{
				s=BGBCP_Token(s, b, &ty);	//'name'
				BGBCP_Token(s, b2, &ty2);

				while(!strcmp(b2, "."))
				{
					s=BGBCP_Token(s, b2, &ty2);	//.
					s=BGBCP_Token(s, b2, &ty2);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token(s, b2, &ty2);
				}

				s1=bgbcc_strdup(b);

				for(i=0; ctx->cur_nsi[i]; i++);
				ctx->cur_nsi[i++]=s1;
				ctx->cur_nsi[i]=NULL;

				n=BCCX_New("using");
				BCCX_Set(n, "name", s1);

//				s=BGBCP_EatSemicolon(s);
				s=BGBCP_EatSemicolonRequired(ctx, s);

				*str=s;
				return(n);
			}

			if(!strcmp(b, "namespace"))
			{
				s=BGBCP_Token(s, b, &ty);	//'name'
				BGBCP_Token(s, b2, &ty2);

				while(!strcmp(b2, "."))
				{
					s=BGBCP_Token(s, b2, &ty2);	//.
					s=BGBCP_Token(s, b2, &ty2);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token(s, b2, &ty2);
				}

				s0=ctx->cur_ns;
				s1=bgbcc_strdup(b);
				ctx->cur_ns=s1;

				a=ctx->cur_nsi;
				ctx->cur_nsi=ab;
				ctx->cur_nsi[0]=NULL;

				s=BGBCP_Token(s, b, &ty); //{
				n1=BGBCP_Block(ctx, &s);
				n=BCCX_New1("namespace", n1);
				BCCX_Set(n, "name", s1);

				ctx->cur_ns=s0;
				ctx->cur_nsi=a;

				*str=s;
				return(n);
			}
		}

		if(	(ctx->lang==BGBCC_LANG_JAVA) ||
			(ctx->lang==BGBCC_LANG_CX))
		{
			if(!strcmp(b, "import"))
			{
				s=BGBCP_Token(s, b, &ty);	//'name'
				BGBCP_Token(s, b2, &ty2);

				while(!strcmp(b2, "."))
				{
					s=BGBCP_Token(s, b2, &ty2);	//.
					s=BGBCP_Token(s, b2, &ty2);
					strcat(b, "/");
					strcat(b, b2);

					BGBCP_Token(s, b2, &ty2);
				}

				s1=bgbcc_strdup(b);

				for(i=0; ctx->cur_nsi[i]; i++);
				ctx->cur_nsi[i++]=s1;
				ctx->cur_nsi[i]=NULL;

				n=BCCX_New("import");
				BCCX_Set(n, "name", s1);

//				s=BGBCP_EatSemicolon(s);
				s=BGBCP_EatSemicolonRequired(ctx, s);

				*str=s;
				return(n);
			}
		}

		if(!strcmp(b, "__pragma"))
		{
			s=*str;
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_New1("pragma", n1);
			*str=s;
			return(n);
		}
	}

	if(!strcmp(b, "{"))
	{
		s=*str;
		n=BGBCP_BlockStatement2(ctx, &s);
		*str=s;
		return(n);
	}

	n=BGBCP_ParseBlockStatementName(ctx, b, &s);
	if(n)
	{
		*str=s;
		return(n);
	}


	s=*str;

#if 0
	n=BGBCP_Definition(ctx, &s);
	if(n!=NULL)
	{
		s=BGBCP_EatSemicolon(s);
		*str=s;
		return(n);
	}
#endif

	n=BGBCP_Statement(ctx, &s);
//	s=BGBCP_EatSemicolon(s);
	s=BGBCP_EatSemicolonRequired(ctx, s);
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_BlockStatementI(BGBCP_ParseState *ctx, char **str, int flag)
{
	char b[256];
	int ty, ln;
	BCCX_Node *n, *n1;
	char *s, *s1, *fn, *t;
	char **a;
	int i;

	s=*str;

#if 1
	BGBCP_Token(s, b, &ty);
	while(!strcmp(b, "#"))
	{
		s=BGBCP_Token(s, b, &ty);
		while(*s && (*s!='\n'))s++;

		b[0]=0;
		BGBCP_Token(s, b, &ty);
	}
#endif

//	s=BGBCP_EatWhite(s);
	BGBCP_EatWhite(s);
	fn=BGBCP_GetLastFileName();
	ln=BGBCP_GetLastLineNumber();

//	printf("@ %s:%d\n", fn, ln);

#if 1
	n=BGBCP_Definition(ctx, &s);

	if(!n && (flag&1))
	{
		n=BGBCP_DefinitionOldStyle(ctx, &s);
	}

	if(n!=NULL)
	{
		BCCX_Set(n, "fn", fn);
		BCCX_SetInt(n, "ln", ln);

		s=BGBCP_EatSemicolon(s);
//		s=BGBCP_EatSemicolonRequired(ctx, s);
		*str=s;
		return(n);
	}
#endif

	s1=ctx->cur_ns; a=ctx->cur_nsi;
	ctx->cur_ns=NULL; ctx->cur_nsi=NULL;

	n=BGBCP_BlockStatementInner(ctx, &s);

	ctx->cur_ns=s1; ctx->cur_nsi=a;

	if(n)
	{
		BCCX_Set(n, "fn", fn);
		BCCX_SetInt(n, "ln", ln);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_BlockStatement(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	n=BGBCP_BlockStatementI(ctx, str, 0);
	return(n);
}

BCCX_Node *BGBCP_BlockI(BGBCP_ParseState *ctx, char **str, int flag)
{
	char b[256];
	char *s, *s1;
	int ty, ty2, i;
	BCCX_Node *n, *lst, *lste;

	s=*str; s1=s;
	lst=NULL; lste=NULL;
	while(1)
	{
		if(!s)
			break;
		BGBCP_Token(s, b, &ty);
		if(!*s || (*b=='}'))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		s1=BGBCP_EatWhite(s);
		if(!*s1) { s=s1; break; }

//#if 0
#ifdef CATCH_BLOCK_SEH
		__try {
#endif

		n=BGBCP_BlockStatementI(ctx, &s, flag);
		if(n==NULL)
		{
			if(!s || s==s1)
			{
				BGBCP_Error(s, "PDSCR_CParse_Block: "
					"Unknown Parse Error\n");
				break;
			}
			continue;
		}
//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

//#if 0
#ifdef CATCH_BLOCK_SEH
		} __except(EXCEPTION_EXECUTE_HANDLER)
		{
			printf("RECAUGHT\n");
			i=1;
			while(s && *s)
			{
				if(*s=='(') { i++; s++; continue; }
				if(*s==')') { i--; s++; continue; }
				if(*s=='[') { i++; s++; continue; }
				if(*s==']') { i--; s++; continue; }

				if(*s=='{') { i++; s++; continue; }
				if(*s=='}')
				{
					i--; s++;
					if(!i)break;
					continue;
				}
				if((i==1) && (*s==';'))
					{ s++; break; }
				s++;
			}
		}
#endif

	}

	*str=s;
	return(lst);
}

BCCX_Node *BGBCP_Block(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	
	n=BGBCP_BlockI(ctx, str, 0);
	return(n);
}

BCCX_Node *BGBCP_Toplevel(BGBCP_ParseState *ctx, char **str)
{
	BCCX_Node *n;
	
	n=BGBCP_BlockI(ctx, str, 1);
	return(n);
}

BCCX_Node *BGBCP_BlockStatement2(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s;
	int tk0, tk1, tk2;
	int ty;
	BCCX_Node *n;

//	if(	(ctx->lang==BGBCC_LANG_JAVA) ||
//		(ctx->lang==BGBCC_LANG_CS) ||
//		(ctx->lang==BGBCC_LANG_BS) ||
//		(ctx->lang==BGBCC_LANG_BS2))
//			return(BGBJP_BlockStatement2(ctx, str));

	s=*str;

	BGBCP_Token(s, b, &ty);
	if(!strcmp(b, "{"))
	{
		s=BGBCP_Token(s, b, &ty);

		tk0=BGBCP_GetTokenCount();
		n=BGBCP_Block(ctx, &s);
		tk1=BGBCP_GetTokenCount();

		n=BCCX_New1("begin", n);

		tk2=tk1-tk0;
		if(tk2>0)
			BCCX_SetInt(n, "tokens", tk2);

		*str=s;
		return(n);
	}

	n=BGBCP_BlockStatement(ctx, str);
	return(n);
}

BCCX_Node *BGBCP_BlockStatement3(BGBCP_ParseState *ctx, char **str)
{
	char *s;
	BCCX_Node *n;

	s=*str;
	s=BGBCP_EatWhite(s);
	n=BGBCP_BlockStatement2(ctx, &s);
	*str=s;
	return(n);
}
