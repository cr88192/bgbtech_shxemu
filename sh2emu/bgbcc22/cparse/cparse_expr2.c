#include <bgbccc.h>

//Lit:		Literal values/builtin expressions
//PE:		(<expr>) <expr>[<expr>] <expr>.<name>
//IncDec:	++<name> --<name> + - ! ~ ... <name>++ <name>-- <expr>(<args>)       
//E:		<expr>**<expr>
//MD:		* / % \ &	*. /. %. \. &.
//AS:		+ - | ^		+. -. |. ^.
//SHLR:		<< >>
//RCmp:		< > <= >= == === != <=> <<== >>==
//Lop:		&&
//Lop2:		|| ^^
//TCond:	<expr>?<expr>:<expr>
//Attr:		:= :!= :< :> :<= :>= :<< :>> :<<= :>>=
//Equals:	= += -= *= /= \= %= &= |= ^= >>= <<=

BGBCP_ParseItem *bgbcp_exprs=NULL;

int BGBCP_AddExpression(char *name,
	BCCX_Node *(*func)(BGBCP_ParseState *ctx, char **s))
{
	BGBCP_ParseItem *tmp;

	tmp=malloc(sizeof(BGBCP_ParseItem));
	tmp->name=strdup(name);
	tmp->func=func;

	tmp->next=bgbcp_exprs;
	bgbcp_exprs=tmp;

	return(0);
}

BCCX_Node *BGBCP_ParseExpressionName(
	BGBCP_ParseState *ctx, char *name, char **s)
{
	BGBCP_ParseItem *cur;

	cur=bgbcp_exprs;
	while(cur)
	{
		if(!strcmp(name, cur->name))
			return(cur->func(ctx, s));
		cur=cur->next;
	}
	return(NULL);
}

BCCX_Node *BGBCP_Number(BGBCP_ParseState *ctx, char *str)
{
	BCCX_Node *n;
	char *s;
	s64 li, lj;
	int i;

	s=str;
//	while(*s && (*s!='.') && (*s!='e') && (*s!='E')) s++;
	while(*s && (*s!='.')) s++;

	if(!*s)
	{
//		li=bgbcc_atoi(str);

		bgbcc_atoxl(str, (u64 *)(&li), (u64 *)(&lj));
		
		if((lj!=0) && (lj!=(-1)))
		{
			n=BCCX_New("int128");
			BCCX_SetInt(n, "value_lo", li);
			BCCX_SetInt(n, "value_hi", lj);
			return(n);
		}

		n=BCCX_New("int");
//		BCCX_Set(n, "value", str);
		BCCX_SetInt(n, "value", li);
		return(n);
	}

	n=BCCX_New("real");
//	BCCX_Set(n, "value", str);
	BCCX_SetFloat(n, "value", BGBCC_ParseNumber(str));
	return(n);
}

BCCX_Node *BGBCP_ExpressionLit(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	char *suf;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2;
	int i;

	s=*str;

	BGBCP_Token(s, b, &ty);
	if(!strcmp(b, "(") && (ty==BTK_BRACE))
	{
		s=BGBCP_Token(s, b, &ty);	//(
		n=BGBCP_Expression2(ctx, &s);
		s=BGBCP_Token(s, b, &ty);	//)

		if(strcmp(b, ")"))
		{
			BGBCP_ErrorCtx(ctx, s, "parser, token '%s'\n", b);
//			printf("parser, token '%s'\n", b);
		}

//		n=BGBCP_FunArgs(ctx, &s);
//		if(CDR(n))n=CONS(SYM("begin"), n);

		*str=s;
		return(n);
	}

	s=*str;

	s=BGBCP_Token(s, b, &ty);
	if(!s)return(NULL);

	if(!strcmp(b, "{") && (ty==BTK_BRACE))
	{
		n1=BGBCP_FunArgs(ctx, &s);
		n=BCCX_New1("list", n1);
		*str=s;
		return(n);
	}

	if(ty==BTK_NAME)
	{
		BGBCP_Token(s, b2, &ty2);

		if(!strcmp(b, "__offsetof"))
		{
			s=BGBCP_Token(s, b2, &ty2);
			if(strcmp(b2, "("))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

//			n1=BGBCP_DefType(ctx, &s);
//			n2=BGBCP_DefName2(ctx, &s);
//			n1=BGBCP_ExpandDefinition(n1, n2);
//			n1=CDR(n1);

			n1=BGBCP_ArgDefinition(ctx, &s);

			s=BGBCP_Token(s, b2, &ty2);
			if(strcmp(b2, ","))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

			s=BGBCP_Token(s, b, &ty);
//			n2=BCCX_New("symbol"); BCCX_Set(n2, "name", b2);

			s=BGBCP_Token(s, b2, &ty2);
			if(strcmp(b2, ")"))
				BGBCP_Error(s, 
					"offsetof token '%s'\n", b2);

			n=BCCX_New1("offsetof", BCCX_New1V("tyexpr", n1));
			BCCX_Set(n, "name", b);

			*str=s;
			return(n);
		}

		if(!strcmp(b, "new") && (ctx->lang!=BGBCC_LANG_C))
		{
			n=BCCX_New("new");

			n1=BGBCP_DefType(ctx, &s);

			if(!n1)
			{
				BGBCP_Error(s, "invalid type for new\n");
				*str=NULL;
				return(NULL);
			}

			BCCX_Add(n, n1);

			BGBCP_Token(s, b, &ty);
			if(!strcmp(b, "("))
			{
				s=BGBCP_Token(s, b, &ty); //'('
				n1=BGBCP_FunArgs(ctx, &s);
				BCCX_Add(n, BCCX_New1V("args", n1));
			}

			*str=s;
			return(n);
		}

		if(!strcmp(b, "L") && (ty2==BTK_STRING))
		{
			n=BGBCP_ExpressionLit(ctx, &s);
			BCCX_Set(n, "tysuf", "L");
			*str=s;
			return(n);
		}

		n=BGBCP_ParseExpressionName(ctx, b, &s);
		if(n)
		{
			*str=s;
			return(n);
		}

		n=BCCX_New("ref");
		BCCX_Set(n, "name", b);
		*str=s;
		return(n);
	}

	BGBCP_Token(s, b2, &ty2);

	if(ty==BTK_NUMBER)
	{
		BGBCP_Token(s, b2, &ty2);
		if((!strcmp(b2, "i") || !strcmp(b2, "iF") ||
			!strcmp(b2, "if")) && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
			if(!strcmp(b2, "i"))
			{
				n=BCCX_New("complex");
				BCCX_Set(n, "imag", b);
			}
			*str=s;
			return(n);

		}

		suf=NULL;
		if(!strcmp(b2, "F") || !strcmp(b2, "f"))suf="F";
		if(!strcmp(b2, "L") || !strcmp(b2, "l"))suf="L";
		if(!strcmp(b2, "LL") || !strcmp(b2, "ll"))suf="LL";
		if(!strcmp(b2, "U") || !strcmp(b2, "u"))suf="U";
		if(!strcmp(b2, "UL") || !strcmp(b2, "ul"))suf="UL";
		if(!strcmp(b2, "ULL") || !strcmp(b2, "ull"))suf="ULL";

		if(!strcmp(b2, "XL") || !strcmp(b2, "xl"))suf="XL";
		if(!strcmp(b2, "UXL") || !strcmp(b2, "uXl"))suf="UXL";

		if(suf && (ty2==BTK_NAME))
		{
			s=BGBCP_Token(s, b2, &ty2);
			*str=s;
			n=BGBCP_Number(ctx, b);
			BCCX_Set(n, "tysuf", suf);
			return(n);
		}

		n=BGBCP_Number(ctx, b);
		*str=s;
		return(n);
	}

	if(ty==BTK_STRING)
	{
		BGBCP_Token(s, b2, &ty2);
		while(ty2==BTK_STRING)
		{
			s=BGBCP_Token(s, b2, &ty2);
			strcat(b, b2);
			BGBCP_Token(s, b2, &ty2);
		}

		n=BCCX_New("string");
		BCCX_Set(n, "value", b);

		*str=s;
		return(n);
	}

	if(ty==BTK_CHARSTRING)
	{
		BGBCP_Token(s, b2, &ty2);
		while(ty2==BTK_CHARSTRING)
		{
			s=BGBCP_Token(s, b2, &ty2);
			strcat(b, b2);
			BGBCP_Token(s, b2, &ty2);
		}

		n=BCCX_New("charstring");
		BCCX_Set(n, "value", b);

		*str=s;
		return(n);
	}

	BGBCP_Error(s, "parse error before '%s'\n", b);

	*str=NULL;
	return(NULL);
}

BCCX_Node *BGBCP_ExpressionPostfix(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t, *s0;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;
	int i;

	s=*str;

	n=BGBCP_ExpressionLit(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(!strcmp(b, "[") && (ty==BTK_BRACE))
		{
			s=BGBCP_Token(s, b, &ty); //[
			n1=BGBCP_Expression(ctx, &s);
			s=BGBCP_Token(s, b, &ty); //]
			n=BCCX_New2("getindex",
				BCCX_New1V("array", n),
				BCCX_New1V("index", n1));
			continue;
		}

		if(!strcmp(b, ".") && (ty==BTK_SEPERATOR))
		{
			s=BGBCP_Token(s, b, &ty); //.
			s=BGBCP_Token(s, b, &ty); //name
			n=BCCX_New1("objref", BCCX_New1V("value", n));
			BCCX_Set(n, "name", b);
			continue;
		}

		if(!strcmp(b, "->") && (ty==BTK_OPERATOR))
		{
			s=BGBCP_Token(s, b, &ty); //.
			s=BGBCP_Token(s, b, &ty); //name
			n=BCCX_New1("objref", BCCX_New1V("value", n));
			BCCX_Set(n, "name", b);
			continue;
		}

#if 1
		if(!strcmp(b, "(") && (ty==BTK_BRACE))
		{
			if(BCCX_TagIsP(n, "objref"))
			{
				s=BGBCP_Token(s, b, &ty); //'('
				n1=BGBCP_FunArgs(ctx, &s);
//				n=CONS3(SYM("methodcall"),
//					CADR(n), CADDR(n), n1);

				BCCX_SetTag(n, "methodcall");
				BCCX_AddV(n, BCCX_New1V("args", n1));
				continue;
			}

			s=BGBCP_Token(s, b, &ty); //'('
			n1=BGBCP_FunArgs(ctx, &s);
			if(!s)
			{
				*str=s;
				return(n);
			}

//			n=CONS2(SYM("funcall"), n, n1);
//			n->tag=bgbcc_strdup("funcall");
//			BCCX_Add(n, BCCX_New1("args", n1));

			if(BCCX_TagIsP(n, "ref"))
			{
				s0=BCCX_Get(n, "name");
				n=BCCX_New1("funcall",
					BCCX_New1V("args", n1));
				BCCX_Set(n, "name", s0);
				continue;
			}

			n=BCCX_New2("funcall",
				BCCX_New1V("value", n),
				BCCX_New1V("args", n1));
			continue;
		}

		if(!strcmp(b, "++") && (ty==BTK_OPERATOR))
		{
			s=BGBCP_Token(s, b, &ty);
			n=BCCX_New1("postinc", BCCX_New1V("expr", n));
			continue;
		}
		if(!strcmp(b, "--") && (ty==BTK_OPERATOR))
		{
			s=BGBCP_Token(s, b, &ty);
			n=BCCX_New1("postdec", BCCX_New1V("expr", n));
			continue;
		}
#endif

		break;
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionUnary(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	t=BGBCP_Token(s, b, &ty);
	BGBCP_Token(t, b2, &ty2);

	if(!strcmp(b, "++") && (ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionUnary(ctx, &s);
		n=BCCX_New1("preinc", BCCX_New1V("expr", n1));
		*str=s;
		return(n);
	}
	if(!strcmp(b, "--") && (ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionUnary(ctx, &s);
		n=BCCX_New1("predec", BCCX_New1V("expr", n1));
		*str=s;
		return(n);
	}

	if((!strcmp(b, "+") || !strcmp(b, "-") || !strcmp(b, "!") ||
		!strcmp(b, "~") || !strcmp(b, "&") || !strcmp(b, "*")) &&
			(ty==BTK_OPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionCast(ctx, &s);
		n1=BCCX_New1V("value", n1);
		n=BCCX_New1("unary", n1);
		BCCX_Set(n, "op", b);
		*str=s;
		return(n);
	}

#if 1
	if(!strcmp(b, "sizeof"))
	{
		s=BGBCP_Token(s, b, &ty);

		if(!strcmp(b2, "(") && (ty2==BTK_BRACE))
		{
			s=BGBCP_Token(s, b2, &ty2);

//			n=BGBCP_DefType(ctx, &s);
//			n1=BGBCP_DefName2(ctx, &s);
//			n=BGBCP_ExpandDefinition(n, n1);
//			n=CDR(n);

			n=BGBCP_ArgDefinition2(ctx, &s);
			if(n)
			{
				n=BCCX_New1("sizeof", BCCX_New1("tyexpr", n));
			}else
			{
				n1=BGBCP_Expression(ctx, &s);
				n=BCCX_New1("sizeof_expr", BCCX_New1("value", n1));
			}

			s=BGBCP_Token(s, b2, &ty2);
			if(strcmp(b2, ")"))
				BGBCP_Error(s,
					"sizeof token '%s'\n", b2);

			*str=s;
			return(n);
		}

		n=BGBCP_ArgDefinition2(ctx, &s);
		if(n)
		{
			n=BCCX_New1("sizeof", BCCX_New1("tyexpr", n));
		}else
		{
			n1=BGBCP_Expression(ctx, &s);
			n=BCCX_New1("sizeof_expr", BCCX_New1("value", n1));
		}

//		n1=BGBCP_Expression(ctx, &s);
//		n=BCCX_New1("sizeof_expr", BCCX_New1V("value", n1));
		*str=s;
		return(n);
	}
#endif

	s=*str;
	n=BGBCP_ExpressionPostfix(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionCast(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t, *s1;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2;

	s=*str;
	s1=BGBCP_Token(s, b, &ty);
	if(!strcmp(b, "(") && (ty==BTK_BRACE))
	{
//		n=BGBCP_DefType(ctx, &s1);
		n=BGBCP_ArgDefinition2(ctx, &s1);
		if(n)
		{
//			n1=BGBCP_DefName2(ctx, &s1);
//			n=BGBCP_ExpandDefinition(n, n1);
//			n=CDR(n);

			n2=BCCX_FindTag(n, "type");
			n2=BCCX_Clone(n2);

			s=s1;
			s=BGBCP_Token(s, b, &ty);
			if(strcmp(b, ")"))
			{
//				printf("parser, cast token '%s'\n", b);
				BGBCP_ErrorCtx(ctx, s, "parser, cast token '%s'\n", b);
			}

			n1=BGBCP_ExpressionCast(ctx, &s);

			BCCX_CheckDeleteUnlinked(n);

			n=BCCX_New2("cast", n2, BCCX_New1V("value", n1));

			*str=s;
			return(n);
		}
	}

	n=BGBCP_ExpressionUnary(ctx, str);
	return(n);
}

BCCX_Node *BGBCP_BinaryExpr(char *op, BCCX_Node *l, BCCX_Node *r)
{
	BCCX_Node *n, *n1, *n2;

	n1=BCCX_New1V("left", l);
	n2=BCCX_New1V("right", r);
	n=BCCX_New2("binary", n1, n2);
	BCCX_Set(n, "op", op);

	return(n);
}

BCCX_Node *BGBCP_UnaryExpr(char *op, BCCX_Node *v)
{
	BCCX_Node *n, *n1, *n2;

	n1=BCCX_New1V("value", v);
	n=BCCX_New1("unary", n1);
	BCCX_Set(n, "op", op);

	return(n);
}

BCCX_Node *BGBCP_IfExpr(BCCX_Node *cond, BCCX_Node *ln, BCCX_Node *rn)
{
	BCCX_Node *n, *n1, *n2, *n3;

	if(ln && rn)
	{
		n1=BCCX_New1V("then", ln);
		n2=BCCX_New1V("else", rn);
		n3=BCCX_New1V("cond", cond);
		n=BCCX_New3("if", n3, n1, n2);
	}else if(ln)
	{
		n1=BCCX_New1V("then", ln);
		n3=BCCX_New1V("cond", cond);
		n=BCCX_New2("if", n3, n1);
	}else if(rn)
	{
		n1=BCCX_New1V("then", rn);
		n2=BGBCP_UnaryExpr("!", cond);
		n3=BCCX_New1V("cond", n2);
		n=BCCX_New2("if", n3, n1);
	}else
	{
		n=NULL;
	}

	return(n);
}

BCCX_Node *BGBCP_ExpressionExp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionCast(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(strcmp(b, "`") && strcmp(b, "``") &&
			strcmp(b, "&") && strcmp(b, "&`") && strcmp(b, "`&") &&
			strcmp(b, "|") && strcmp(b, "|`") && strcmp(b, "`|") &&
			strcmp(b, "^") && strcmp(b, "^`") && strcmp(b, "`^")
			)
			break;

		s=BGBCP_Token(s, b, &ty);
		if(!strcmp(b, "`"))strcpy(b, "**");

		n1=BGBCP_ExpressionCast(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionMD(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionExp(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	strcmp(b, "*") && strcmp(b, "/") && strcmp(b, "%") &&
			strcmp(b, "*`") && strcmp(b, "/`") && strcmp(b, "%`") &&
			strcmp(b, "`*") && strcmp(b, "`/") && strcmp(b, "`%") &&
			strcmp(b, "~") && strcmp(b, "~`") && strcmp(b, "`~"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionExp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionAS(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionMD(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;

		if(	strcmp(b, "+") && strcmp(b, "-") &&
			strcmp(b, "+`") && strcmp(b, "-`") &&
			strcmp(b, "`+") && strcmp(b, "`-"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionMD(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionSHLR(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionAS(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)
			break;
		if(strcmp(b, "<<") && strcmp(b, ">>"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionAS(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionRCmp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionSHLR(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;

		if(strcmp(b, "<") && strcmp(b, ">") &&
			strcmp(b, "<=") && strcmp(b, ">="))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionSHLR(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionECmp(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionRCmp(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;
		if(strcmp(b, "==") && strcmp(b, "!="))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionRCmp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitAnd(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionECmp(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(strcmp(b, "&"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionECmp(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitXor(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitAnd(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(strcmp(b, "^"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitAnd(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionBitOr(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitXor(ctx, &s);
	if(!n) { *str=NULL; return(n); }
	while(1)
	{
		t=BGBCP_Token(s, b, &ty);
		if(ty!=BTK_OPERATOR)break;
		if(strcmp(b, "|"))break;
		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitXor(ctx, &s);
		n=BGBCP_BinaryExpr(b, n, n1);
	}
	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLop(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionBitOr(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)break;
		if(strcmp(b, "&&"))break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionBitOr(ctx, &s);

		n=BGBCP_BinaryExpr("&&", n, n1);
//		n=BCCX_New2("l_and",
//			BCCX_New1V("left", n),
//			BCCX_New1V("right", n1));

#if 0
		if(BCCX_TagIsP(n, "and"))
		{
			BCCX_Add(n, BCCX_New1V("value", n1));
			continue;
		}

		n=BCCX_New2("and",
			BCCX_New1V("value", n),
			BCCX_New1V("value", n1));
#endif
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionLop2(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionLop(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;
		if(strcmp(b, "||"))
			break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionLop(ctx, &s);

		n=BGBCP_BinaryExpr("||", n, n1);
//		n=BCCX_New2("l_or",
//			BCCX_New1V("left", n),
//			BCCX_New1V("right", n1));

#if 0
		if(BCCX_TagIsP(n, "or"))
		{
			BCCX_Add(n, BCCX_New1("value", n1));
			continue;
		}

		n=BCCX_New2("or",
			BCCX_New1("value", n),
			BCCX_New1("value", n1));
#endif
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionTCond(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n1=BGBCP_ExpressionLop2(ctx, &s);
	BGBCP_Token(s, b, &ty);
	if(strcmp(b, "?") || (ty!=BTK_OPERATOR))
	{
		*str=s;
		return(n1);
	}

//	s=BGBCP_Token(s, b, &ty); //?
	s=BGBCP_EatExpectToken(ctx, s, "?");
//	n2=BGBCP_ExpressionLop2(ctx, &s);
	n2=BGBCP_Expression(ctx, &s);
//	s=BGBCP_Token(s, b, &ty); //:
	s=BGBCP_EatExpectToken(ctx, s, ":");
//	n3=BGBCP_ExpressionTCond(ctx, &s);
	n3=BGBCP_Expression(ctx, &s);

	n=BCCX_New3("if",
		BCCX_New1V("cond", n1),
		BCCX_New1V("then", n2),
		BCCX_New1V("else", n3));

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionEquals(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionTCond(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);

		if(ty!=BTK_OPERATOR)
			break;

		if(strcmp(b, "=") && strcmp(b, "+=") && strcmp(b, "-=") &&
		strcmp(b, "*=") && strcmp(b, "/=")  && strcmp(b, "%=")  &&
		strcmp(b, "&=") && strcmp(b, "|=") &&
		strcmp(b, "^=") && strcmp(b, "<<=") && strcmp(b, ">>="))
			break;

		t=NULL;
		if(!strcmp(b, "+="))t="+";
		if(!strcmp(b, "-="))t="-";
		if(!strcmp(b, "*="))t="*";
		if(!strcmp(b, "/="))t="/";
		if(!strcmp(b, "%="))t="%";
		if(!strcmp(b, "&="))t="&";
		if(!strcmp(b, "|="))t="|";
		if(!strcmp(b, "^="))t="^";
		if(!strcmp(b, "<<="))t="<<";
		if(!strcmp(b, ">>="))t=">>";

		s=BGBCP_Token(s, b, &ty);

#if 0
		if(BCCX_TagIsP(n, "ref"))
		{
			n1=BGBCP_ExpressionEquals(ctx, &s);
			n2=n;

			n=BCCX_New1("assign", BCCX_New1("value", n1));
			if(t)BCCX_Set(n, "op", t);

			t=BCCX_Get(n2, "name");
			BCCX_Set(n, "name", t);

			continue;
		}

		if(BCCX_TagIsP(n, "objref") || BCCX_TagIsP(n, "getindex"))
		{
			if(BCCX_TagIsP(n, "objref"))
			{
				n1=BGBCP_ExpressionEquals(ctx, &s);
				if(t)n1=BGBCP_BinaryExpr(t,
					BCCX_Clone(n), n1);

				n2=BCCX_Find(n, "value");
				BCCX_SetTag(n2, "object");

				BCCX_SetTag(n, "objset");
				BCCX_AddV(n, BCCX_New1V("value", n1));
				continue;
			}
			if(BCCX_TagIsP(n, "getindex"))
			{
				n1=BGBCP_ExpressionEquals(ctx, &s);
				if(t)n1=BGBCP_BinaryExpr(t,
					BCCX_Clone(n), n1);
				BCCX_SetTag(n, "setindex");
				BCCX_Add(n, BCCX_New1("value", n1));
				continue;
			}
		}
#endif

		n1=BGBCP_ExpressionEquals(ctx, &s);
		n=BCCX_New2("assign",
			BCCX_New1V("left", n),
			BCCX_New1V("right", n1));
		if(t)BCCX_Set(n, "op", t);

		continue;
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_ExpressionComma(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	char *s, *t;
	int ty, ty2;
	BCCX_Node *n, *n1, *n2, *n3;

	s=*str;
	n=BGBCP_ExpressionEquals(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	while(1)
	{
		BGBCP_Token(s, b, &ty);
		if(ty!=BTK_SEPERATOR)break;
		if(strcmp(b, ","))break;

		s=BGBCP_Token(s, b, &ty);
		n1=BGBCP_ExpressionEquals(ctx, &s);
		n=BCCX_New2("comma",
			BCCX_New1V("left", n),
			BCCX_New1V("right", n1));
	}

	*str=s;
	return(n);
}

BCCX_Node *BGBCP_Expression(BGBCP_ParseState *ctx, char **str)
{
	return(BGBCP_ExpressionEquals(ctx, str));
}

BCCX_Node *BGBCP_Expression2(BGBCP_ParseState *ctx, char **str)
{
	return(BGBCP_ExpressionComma(ctx, str));
}

#if 0
// BCCX_Node *BGBCP_Expression3(BGBCP_ParseState *ctx, char **str)
{
	char b[256], b2[256];
	int ty, ty2;
	char *s;
	BCCX_Node *n;

	s=*str;
	n=BGBCP_ExpressionEquals(ctx, &s);
	if(!n) { *str=NULL; return(n); }

	BGBCP_Token(s, b, &ty);
	if(!strcmp(b, ":") && (ty==BTK_SEPERATOR))
	{
		s=BGBCP_Token(s, b, &ty);
		if(BS1_SYMBOLP(n))n=KEYSYM(TOSYM(n));
			else n=LIST2S("colon", n);
	}

	*str=s;
	return(n);
}
#endif

