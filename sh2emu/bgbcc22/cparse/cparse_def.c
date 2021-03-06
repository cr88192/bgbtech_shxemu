#include <bgbccc.h>

// extern BCCX_Node *bgbcp_structs;
// extern BCCX_Node *bgbcp_types;

BCCX_Node *BGBCP_VarsList(BGBCP_ParseState *ctx, char **str, BCCX_Node *tn)
{
	char b[256], b2[256];
	char *s;
	int ty, ty2;
	BCCX_Node *n, *n1, *lst, *lste;

	s=*str;
	lst=NULL; lste=NULL;

//	printf("VL %p %s\n", s, s);

	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(!bgbcp_strcmp1(b, ";"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		n=BGBCP_VarDefinition(ctx, &s, tn);

		BGBCP_Token(s, b, &ty);
		if(!bgbcp_strcmp1(b, "="))
		{
			s=BGBCP_Token(s, b, &ty);
			n1=BGBCP_Expression(ctx, &s);
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_value, "value", n1));

			BGBCP_Token(s, b, &ty);
		}

//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

		s=BGBCP_Token(s, b, &ty);
		if(!bgbcp_strcmp1(b, ";"))break;

		if(bgbcp_strcmp1(b, ","))
		{
			BGBCP_ErrorCtx(ctx, s, "PDSCR_CParse_VarsList: "
				"Invalid token '%s' in vars list\n", b);

//			printf("%p %s\n", s, s);
//			*(int *)-1=-1;

			*str=NULL;
			return(NULL);
		}
	}

	*str=s;
	return(lst);
}

BCCX_Node *BGBCP_FunVarsList(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *lst, *lste;

	s=*str;
	lst=NULL; lste=NULL;

	while(1)
	{
		s1=BGBCP_Token(s, b, &ty);
		BGBCP_Token(s1, b2, &ty2);
		if(!bgbcp_strcmp1(b, ")"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if(!bgbcp_strcmp3(b, "..."))
		{
			s=BGBCP_Token(s, b, &ty);

			n=BCCX_NewCst(&bgbcc_rcst_rest, "rest");
//			lst=BCCX_AddEnd(lst, n);
			lst=BCCX_AddEnd2(lst, &lste, n);
			continue;
		}

		if(!bgbcp_strcmp4(b, "void") && !bgbcp_strcmp1(b2, ")"))
		{
			s=BGBCP_Token(s, b, &ty);
			s=BGBCP_Token(s, b, &ty);
			break;
		}


		n=BGBCP_ArgDefinition(ctx, &s);
//		lst=BCCX_AddEnd(lst, n);
		lst=BCCX_AddEnd2(lst, &lste, n);

		BGBCP_Token(s, b, &ty);

		if(!bgbcp_strcmp1(b, ")"))
		{
			s=BGBCP_Token(s, b, &ty);
			break;
		}

		if(bgbcp_strcmp1(b, ","))
		{
			BGBCP_ErrorCtx(ctx, s, "PDSCR_CParse_FunVarsList: "
				"Invalid token '%s' in vars list\n", b);
//			*(int *)-1=-1;

			*str=NULL;
			return(NULL);
		}
		s=BGBCP_Token(s, b, &ty);
	}

	*str=s;
	return(lst);
}

BCCX_Node *BGBCP_DefName(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s;
	int i, ty, fl, ind;
	BCCX_Node *n, *n1, *nl, *anl, *nty;

	s=*str;

	BGBCP_Token(s, b, &ty);

	if(!bgbcp_strcmp1(b, ";") || !bgbcp_strcmp1(b, ","))
		return(NULL);

	fl=0; ind=0;

	anl=NULL;
	while(1)
	{
//		BGBCP_Token(s, b, &ty);

		if(!bgbcp_strcmp1(b, "*"))
		{
			s=BGBCP_Token(s, b, &ty);
			BGBCP_Token(s, b, &ty);
			ind++;
			continue;
		}

		i=BGBCP_DefTypeFlag(ctx, b);
		if(i)
		{
			s=BGBCP_Token(s, b, &ty);
			BGBCP_Token(s, b, &ty);
			fl|=i;
			continue;
		}

		n1=BGBCP_DeclAttributeC(ctx, &s);
		if(n1)
		{
			BGBCP_Token(s, b, &ty);
			anl=BCCX_AddEnd(anl, n1);
			continue;
		}
		
		break;
	}

	BGBCP_Token(s, b, &ty);

	if(!bgbcp_strcmp1(b, "("))
	{
		s=BGBCP_Token(s, b, &ty);	//(
		n1=BGBCP_DefName2(ctx, &s);
		s=BGBCP_Token(s, b, &ty);	//)

		n=BCCX_NewCst1(&bgbcc_rcst_fproto, "fproto", n1);
	}else if(!bgbcp_strcmp1(b, ";") || !bgbcp_strcmp1(b, ",") ||
		!bgbcp_strcmp1(b, ")") ||
		!bgbcp_strcmp1(b, "[") || !bgbcp_strcmp1(b, ":"))
	{
		n=BCCX_NewCst(&bgbcc_rcst_var, "var");
	}else if(ty==BTK_NAME)
	{
		s=BGBCP_Token(s, b, &ty);
		n=BCCX_NewCst(&bgbcc_rcst_var, "var");
		BCCX_SetCst(n, &bgbcc_rcst_name, "name", b);
	}else
	{
		BGBCP_ErrorCtx(ctx, s, "BGBCP_DefName: "
			"Invalid Token '%s'\n", b);
//		*(int *)(-1)=-1;
	}

	nty=BCCX_NewCst(&bgbcc_rcst_type, "type");
	BCCX_Add(n, nty);

	nl=NULL;
	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(!bgbcp_strcmp1(b, "["))
		{
			s=BGBCP_Token(s, b, &ty);
			BGBCP_Token(s, b, &ty);
			if(!bgbcp_strcmp1(b, "]"))
			{
				s=BGBCP_Token(s, b, &ty);
				n1=BGBCP_Number(ctx, "0");
				nl=BCCX_AddEnd(nl, n1);
				continue;
			}

			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty);

			nl=BCCX_AddEnd(nl, n1);
			continue;
		}

		if(!bgbcp_strcmp1(b, ":"))
		{
			s=BGBCP_Token(s, b, &ty);	//':'
			n1=BGBCP_Expression(ctx, &s);
//			s=BGBCP_Token(s, b, &ty);

			n1=BCCX_NewCst1V(&bgbcc_rcst_bits, "bits", n1);
			BCCX_AddV(nty, n1);
			continue;
		}

		n1=BGBCP_DeclAttributeC(ctx, &s);
		if(n1)
		{
			anl=BCCX_AddEnd(anl, n1);
			continue;
		}

		break;
	}

//	if(nl)BCCX_AddV(nty, BCCX_NewCst1V(&bgbcc_rcst_size, "size", nl));
	if(nl)BCCX_Add(nty, BCCX_NewCst1(&bgbcc_rcst_size, "size", nl));
	if(anl)BCCX_Add(nty, anl);
	BCCX_SetIntCst(nty, &bgbcc_rcst_flags, "flags", fl);
	BCCX_SetIntCst(nty, &bgbcc_rcst_ind, "ind", ind);

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_DefName2(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s;
	int i, ty;
	BCCX_Node *n, *n1;

	n=BGBCP_DefName(ctx, str);
	if(n==NULL)return(n);

	s=*str;

	BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp1(b, ";") || !bgbcp_strcmp1(b, ","))
		return(n);

	if(!bgbcp_strcmp1(b, "("))
	{
		s=BGBCP_Token(s, b, &ty);	//(
		n1=BGBCP_FunVarsList(ctx, &s);

		BCCX_SetTagCst(n, &bgbcc_rcst_iproto, "iproto");
		BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", n1));

		while(1)
		{
			n1=BGBCP_DeclAttributeC(ctx, &s);
			if(!n1)break;
			BCCX_Add(n, n1);
		}

		*str=s;
		return(n);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_VarDefinition(BGBCP_ParseState *ctx,
	char **str, BCCX_Node *tn)
{
	char b[256], b2[256];
	char *s, *s2;
	char *fn, *tdn;
	int ty, ty2, fl, ind, fl1, ind1;
	BCCX_Node *n, *n1, *n2, *n3, *n4;
	BCCX_Node *nl;
	int i, j;

	s=*str;

	fl=BCCX_GetIntCst(tn, &bgbcc_rcst_flags, "flags");
	ind=BCCX_GetIntCst(tn, &bgbcc_rcst_ind, "ind");

	BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp(b, "__stdcall"))
		{ s=BGBCP_Token(s, b, &ty); fl|=BGBCC_TYFL_STDCALL; }

	n1=BGBCP_DefName(ctx, &s);

	if(!n1)
	{
		n=BCCX_NewCst(&bgbcc_rcst_var, "var");

		n1=BCCX_Clone(tn);
		BCCX_Add(n, n1);

		BCCX_SetIntCst(n1, &bgbcc_rcst_flags, "flags", fl);
		BCCX_SetIntCst(n1, &bgbcc_rcst_ind, "ind", ind);

		*str=s;
		return(n);
	}

	BGBCP_Token(s, b, &ty);
	if(!bgbcp_strcmp(b, "__stdcall"))
		{ s=BGBCP_Token(s, b, &ty); fl|=BGBCC_TYFL_STDCALL; }

	BGBCP_Token(s, b, &ty);

	if(!bgbcp_strcmp1(b, "(") && (ty==BTK_BRACE))
	{
		s=BGBCP_Token(s, b, &ty);	//(
		n2=BGBCP_FunVarsList(ctx, &s);

		if(BCCX_TagIsP(n1, "fproto"))
		{
//			s2=BGBCC_GenSym2();

			n3=BCCX_FindTag(n1, "type");
			if(n3)
			{
				BCCX_SetIntCst(tn, &bgbcc_rcst_flags, "flags",
					fl|BCCX_GetIntCst(n3, &bgbcc_rcst_flags, "flags"));
				BCCX_SetIntCst(tn, &bgbcc_rcst_ind, "ind",
					ind+BCCX_GetIntCst(n3, &bgbcc_rcst_ind, "ind"));
			}
	
			s2=BGBCC_GenSym();
//			s2=BGBCC_GenProtoSig(NULL, tn, n2);
//			i=0; while(*s2)i=(i*127)+(*s2++);
//			sprintf(b2, "HSG%08X", i);
//			s2=bgbcc_strdup(b2);

			n=BCCX_NewCst(&bgbcc_rcst_func, "func");
			BCCX_SetCst(n, &bgbcc_rcst_name, "name", s2);
//			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags",
//				fl|BCCX_GetInt(n, "flags"));
//			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind",
//				ind+BCCX_GetInt(n, "ind"));
			BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags",
				fl|BCCX_GetIntCst(n1, &bgbcc_rcst_flags, "flags"));
			BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind",
				ind+BCCX_GetIntCst(n1, &bgbcc_rcst_ind, "ind"));

			BCCX_Add(n, BCCX_Clone(tn));
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", n2));
//			ctx->structs=BCCX_AddEnd(ctx->structs, n);
			ctx->structs=BCCX_AddEnd2(ctx->structs, &ctx->e_structs, n);


			n=BCCX_Child(n1);

			n1=BCCX_FindTag(n, "type");
			if(!n1)
			{
				n1=BCCX_NewCst(&bgbcc_rcst_type, "type");
				BCCX_Add(n, n1);
			}
			BCCX_SetCst(n1, &bgbcc_rcst_name, "name", s2);

			while(1)
			{
				n2=BGBCP_DeclAttributeC(ctx, &s);
				if(!n2)break;
				BCCX_Add(n1, n2);
			}

			*str=s;
			return(n);
		}

		n=n1;
		BCCX_SetTagCst(n, &bgbcc_rcst_proto, "proto");

//		n2=BCCX_Clone(n2);

		BCCX_Add(n, BCCX_NewCst1V(&bgbcc_rcst_args, "args", n2));

		n2=BCCX_FindTag(n, "type");
		BCCX_SetCst(n2, &bgbcc_rcst_name, "name",
			BCCX_GetCst(tn, &bgbcc_rcst_name, "name"));
		BCCX_SetIntCst(n2, &bgbcc_rcst_flags, "flags",
			fl|BCCX_GetIntCst(n2, &bgbcc_rcst_flags, "flags"));
		BCCX_SetIntCst(n2, &bgbcc_rcst_ind, "ind",
			ind+BCCX_GetIntCst(n2, &bgbcc_rcst_ind, "ind"));

		while(1)
		{
			n1=BGBCP_DeclAttributeC(ctx, &s);
			if(!n1)break;
			BCCX_Add(n2, n1);
		}

#if 1
		n1=BCCX_Child(tn);
		while(n1)
		{
			if(BCCX_TagIsP(n1, "declspec") ||
				BCCX_TagIsP(n1, "attribute"))
					BCCX_Add(n2, BCCX_Clone(n1));
			n1=BCCX_Next(n1);
		}
#endif

		*str=s;
		return(n);
	}

	n=n1;
//	BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags",
//		fl|BCCX_GetInt(n, "flags"));
//	BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind",
//		ind+BCCX_GetInt(n, "ind"));
//	BCCX_Add(n, BCCX_Clone(tn));

	tdn=BCCX_GetCst(n, &bgbcc_rcst_name, "name");
	n4=BCCX_FindTag(n, "type");
	fn=BCCX_GetCst(tn, &bgbcc_rcst_name, "name");
	fl1=fl|BCCX_GetIntCst(n4, &bgbcc_rcst_flags, "flags");
	ind1=ind+BCCX_GetIntCst(n4, &bgbcc_rcst_ind, "ind");

	BCCX_SetCst(n4, &bgbcc_rcst_name, "name", fn);
	BCCX_SetIntCst(n4, &bgbcc_rcst_flags, "flags", fl1);
	BCCX_SetIntCst(n4, &bgbcc_rcst_ind, "ind", ind1);

	n1=BCCX_FindTag(n4, "size");
	n2=BCCX_FindTag(tn, "size");
	if(n1 && n2)
	{
		nl=NULL;

		n2=BCCX_Child(n2);
		while(n2)
		{
			nl=BCCX_AddEnd(nl, BCCX_Clone(n2));
			n2=BCCX_Next(n2);
		}

		n2=BCCX_Child(n1);
		while(n2)
		{
			nl=BCCX_AddEnd(nl, BCCX_Clone(n2));
			n2=BCCX_Next(n2);
		}

		BCCX_Unlink(n1);
		BCCX_AddV(n4, BCCX_NewCst1V(&bgbcc_rcst_size, "size", nl));
	}else if(n2)
	{
		BCCX_Add(n4, BCCX_Clone(n2));
	}

#if 1
	n1=BCCX_Child(tn);
	while(n1)
	{
		if(BCCX_TagIsP(n1, "declspec") ||
			BCCX_TagIsP(n1, "attribute"))
				BCCX_Add(n4, BCCX_Clone(n1));
		n1=BCCX_Next(n1);
	}
#endif

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ArgDefinition(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s, *s2;
	BCCX_Node *n, *n1;
	int i, ty;

	s=*str;
	n1=BGBCP_DefType(ctx, &s);
	if(!n1)
	{
//		BGBCC_DBGBREAK
	
#if 1
		//Old Style
		BGBCP_Token(s, b, &ty);
		if(ty==BTK_NAME)
		{
			n1=BCCX_NewCst(&bgbcc_rcst_type, "type");
			BCCX_SetCst(n1, &bgbcc_rcst_name, "name", "int");
			BCCX_SetIntCst(n1, &bgbcc_rcst_flags, "flags", 0);
			BCCX_SetIntCst(n1, &bgbcc_rcst_ind, "ind", 0);

			n=BGBCP_VarDefinition(ctx, &s, n1);
			BCCX_CheckDeleteUnlinked(n1);

			*str=s;
			return(n);
		}
#endif

		return(NULL);
	}

	n=BGBCP_VarDefinition(ctx, &s, n1);
	BCCX_CheckDeleteUnlinked(n1);

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ArgDefinition2(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s, *s2;
	BCCX_Node *n, *n1;
	int i, ty;

	s=*str;
	n1=BGBCP_DefType(ctx, &s);
	if(!n1)
	{
		return(NULL);
	}

	n=BGBCP_VarDefinition(ctx, &s, n1);
	BCCX_CheckDeleteUnlinked(n1);

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_Definition(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s, *s2;
	BCCX_Node *n, *n1, *n2, *ntl;
	int tk0, tk1, tk2;
	int i, ty;

//	if(	(ctx->lang==BGBCC_LANG_JAVA) ||
//		(ctx->lang==BGBCC_LANG_CS) ||
//		(ctx->lang==BGBCC_LANG_BS) ||
//		(ctx->lang==BGBCC_LANG_MINIC) ||
//		(ctx->lang==BGBCC_LANG_BS2))
//			return(BGBJP_Definition(ctx, str));

	s=*str;

#if 1
	s=BGBCP_EatWhiteOnly(s);
	if(!strncmp(s, "/*AHPRX:*/", 10))
	{
		s+=10;
		n=BGBCP_Definition(ctx, &s);
		if(n)
		{
			n1=BCCX_FindTag(n, "type");
			if(n1)
			{
				i=BCCX_GetIntCst(n1, &bgbcc_rcst_flags, "flags");
				i|=BGBCC_TYFL_VIRTUAL|BGBCC_TYFL_PROXY;
				BCCX_SetIntCst(n1, &bgbcc_rcst_flags, "flags", i);
			}
			*str=s;
			return(n);
		}
		return(NULL);
	}
	
	n1=BGBCP_ParseDocComment(ctx, &s);
	n2=BGBCP_ParseComment(ctx, &s);
	if(n1 && !n2)
	{
		n=BGBCP_Definition(ctx, &s);
		if(n)
		{
			BCCX_Add(n, n1);
			*str=s;
			return(n);
		}
		return(NULL);
	}

	BCCX_CheckDeleteUnlinked(n1);
	BCCX_CheckDeleteUnlinked(n2);

#endif

//	printf("\n");
//	printf("DEF %p %s\n", s, s);

	n=BGBCP_DefType(ctx, &s);
	if(n)
	{
//		BGBCP_Warn(s, "BGBCP_Definition: Note: definition\n");

#if 0
		if(BCCX_TagIsP(n, "classdef") ||
			BCCX_TagIsP(n, "enumdef"))
		{
			*str=s2;
			return(n);
		}
#endif


		s2=s;

		n1=BGBCP_VarDefinition(ctx, &s2, n);
		if(BCCX_TagIsP(n1, "proto"))
		{
			ntl=NULL;
			BGBCP_Token(s2, b, &ty);
			if(bgbcp_strcmp1(b, "{") &&
				bgbcp_strcmp1(b, ",") &&
				bgbcp_strcmp1(b, ";"))
			{
				ntl=BCCX_NewCst(&bgbcc_rcst_argdecls, "argdecls");
				while(s2 && bgbcp_strcmp1(b, "{"))
				{
					n2=BGBCP_Definition(ctx, &s2);
					if(n2)
					{
						BCCX_Add(ntl, n2);
					}else
					{
						BGBCP_ErrorCtx(ctx, s, "Parse ArgDecl Fail");
						break;
					}

					BGBCP_Token(s2, b, &ty);
				}
			}

			BGBCP_Token(s2, b, &ty);
			if(!bgbcp_strcmp1(b, "{"))
			{
				BCCX_CheckDeleteUnlinked(n);

				n=BCCX_Clone(n1);
				BCCX_CheckDeleteUnlinked(n1);

				tk0=BGBCP_GetTokenCount();
				n1=BGBCP_BlockStatement2(ctx, &s2);
				tk1=BGBCP_GetTokenCount();

				BCCX_SetTagCst(n, &bgbcc_rcst_defun, "defun");
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1));

				if(ntl)BCCX_Add(n, ntl);

				tk2=tk1-tk0;
				if(tk2>0)
					BCCX_SetIntCst(n, &bgbcc_rcst_tokens, "tokens", tk2);

				*str=s2;
				return(n);
			}
		}

		n1=BGBCP_VarsList(ctx, &s, n);
		BCCX_CheckDeleteUnlinked(n);
//		n1=BCCX_Clone(n1);
		n=BCCX_NewCst1(&bgbcc_rcst_vars, "vars", n1);

		*str=s;
		return(n);
	}

	return(NULL);
}

BCCX_Node *BGBCP_DefinitionOldStyle(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *s2;
	BCCX_Node *n, *n1, *n2, *ntl;
	int tk0, tk1, tk2;
	int i, ty, ty2;

	s=*str;

	s2=BGBCP_Token(s, b, &ty);
	BGBCP_Token(s2, b2, &ty2);
	
	if((ty!=BTK_NAME) || bgbcp_strcmp1(b2, "("))
		return(NULL);

	BGBCP_WarnCtx(ctx, s, "Old-style declaration\n");

	n=BCCX_NewCst(&bgbcc_rcst_type, "type");
	BCCX_SetCst(n, &bgbcc_rcst_name, "name", "int");
	BCCX_SetIntCst(n, &bgbcc_rcst_flags, "flags", 0);
	BCCX_SetIntCst(n, &bgbcc_rcst_ind, "ind", 0);

	s2=s;
	n1=BGBCP_VarDefinition(ctx, &s2, n);
	if(BCCX_TagIsP(n1, "proto"))
	{
		ntl=NULL;
		BGBCP_Token(s2, b, &ty);
		if(bgbcp_strcmp1(b, "{") &&
			bgbcp_strcmp1(b, ",") &&
			bgbcp_strcmp1(b, ";"))
		{
			ntl=BCCX_NewCst(&bgbcc_rcst_argdecls, "argdecls");
			while(s2 && bgbcp_strcmp1(b, "{"))
			{
				n2=BGBCP_Definition(ctx, &s2);
				if(n2)
				{
					BCCX_Add(ntl, n2);
				}else
				{
					BGBCP_ErrorCtx(ctx, s, "Parse ArgDecl Fail");
					break;
				}

				BGBCP_Token(s2, b, &ty);
			}
		}

		BGBCP_Token(s2, b, &ty);
		if(!bgbcp_strcmp1(b, "{"))
		{
			BCCX_CheckDeleteUnlinked(n);

			n=BCCX_Clone(n1);
			BCCX_CheckDeleteUnlinked(n1);

			tk0=BGBCP_GetTokenCount();
			n1=BGBCP_BlockStatement2(ctx, &s2);
			tk1=BGBCP_GetTokenCount();

			BCCX_SetTagCst(n, &bgbcc_rcst_defun, "defun");
			BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1));

			if(ntl)BCCX_Add(n, ntl);

			tk2=tk1-tk0;
			if(tk2>0)
				BCCX_SetIntCst(n, &bgbcc_rcst_tokens, "tokens", tk2);

			*str=s2;
			return(n);
		}
	}

	n1=BGBCP_VarsList(ctx, &s, n);
	BCCX_CheckDeleteUnlinked(n);
//		n1=BCCX_Clone(n1);
	n=BCCX_NewCst1(&bgbcc_rcst_vars, "vars", n1);

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ForceDefinition(BGBCP_ParseState *ctx, char **str)
{
	char b[256];
	char *s, *s2;
	BCCX_Node *n, *n1, *n2;
	int tk0, tk1, tk2;
	int i, ty;

//	if(	(ctx->lang==BGBCC_LANG_JAVA) ||
//		(ctx->lang==BGBCC_LANG_CS) ||
//		(ctx->lang==BGBCC_LANG_BS) ||
//		(ctx->lang==BGBCC_LANG_MINIC) ||
//		(ctx->lang==BGBCC_LANG_BS2))
//			return(BGBJP_Definition(ctx, str));

	s=*str;

#if 1
	s=BGBCP_EatWhiteOnly(s);
	if(!strncmp(s, "/*AHPRX:*/", 10))
	{
		s+=10;
		n=BGBCP_ForceDefinition(ctx, &s);
		if(n)
		{
			n1=BCCX_FindTag(n, "type");
			if(n1)
			{
				i=BCCX_GetIntCst(n1, &bgbcc_rcst_flags, "flags");
				i|=BGBCC_TYFL_VIRTUAL|BGBCC_TYFL_PROXY;
				BCCX_SetIntCst(n1, &bgbcc_rcst_flags, "flags", i);
			}
			*str=s;
			return(n);
		}
		return(NULL);
	}
	
	n1=BGBCP_ParseDocComment(ctx, &s);
	n2=BGBCP_ParseComment(ctx, &s);
	if(n1 && !n2)
	{
		n=BGBCP_ForceDefinition(ctx, &s);
		if(n)
		{
			BCCX_Add(n, n1);
			*str=s;
			return(n);
		}
		return(NULL);
	}
	BCCX_CheckDeleteUnlinked(n1);
	BCCX_CheckDeleteUnlinked(n2);
#endif

	n=BGBCP_DefType(ctx, &s);
	if(n)
	{
		s2=s;

		n1=BGBCP_VarDefinition(ctx, &s2, n);
		if(BCCX_TagIsP(n1, "proto"))
		{
			BGBCP_Token(s2, b, &ty);
			if(!bgbcp_strcmp1(b, "{"))
			{
				BCCX_CheckDeleteUnlinked(n);

				n=BCCX_Clone(n1);
				BCCX_CheckDeleteUnlinked(n1);

				tk0=BGBCP_GetTokenCount();
				n1=BGBCP_BlockStatement2(ctx, &s2);
				tk1=BGBCP_GetTokenCount();

				BCCX_SetTagCst(n, &bgbcc_rcst_defun, "defun");
				BCCX_AddV(n, BCCX_NewCst1V(&bgbcc_rcst_body, "body", n1));

				tk2=tk1-tk0;
				if(tk2>0)
					BCCX_SetIntCst(n, &bgbcc_rcst_tokens, "tokens", tk2);

				*str=s2;
				return(n);
			}
		}

		n1=BGBCP_VarsList(ctx, &s, n);

		BCCX_CheckDeleteUnlinked(n);

//		n1=BCCX_Clone(n1);
		n=BCCX_NewCst1(&bgbcc_rcst_vars, "vars", n1);

		*str=s;
		return(n);
	}

	BGBCP_Token(s, b, &ty);

	BGBCP_ErrorCtx(ctx, s, "BGBCP_ForceDefinition: "
		"Failed Parse Type '%s'\n", b);

	return(NULL);
}

