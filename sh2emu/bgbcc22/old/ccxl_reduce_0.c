#include <bgbccc.h>

int BGBCC_CCXL_IsUnaryP(BGBCC_TransState *ctx, BCCX_Node *l, char *op)
{
	return(BCCX_TagAttrIsP(l, "unary", "op", op));
}

int BGBCC_CCXL_IsBinaryP(BGBCC_TransState *ctx, BCCX_Node *l, char *op)
{
	return(BCCX_TagAttrIsP(l, "binary", "op", op));
}

int BGBCC_CCXL_BoolExpr(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	double f;
	int i;

	if(BCCX_TagIsP(l, "int"))
	{
		i=BCCX_GetInt(l, "value");
		if(!i)return(0);
		return(1);
	}
	if(BCCX_TagIsP(l, "real"))
	{
		f=BCCX_GetFloat(l, "value");
		if(f==0)return(0);
		return(1);
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "value"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		return(i);
	}

	if(BCCX_TagIsP(l, "boolify"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "value"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		return(i);
	}

	if(BCCX_TagIsP(l, "string"))
		return(1);

	return(-1);
}

int BGBCC_CCXL_IsFixIntAssignRVP(
	BGBCC_TransState *ctx, BCCX_Node *l,
	char **rname, int *rval)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	ccxl_label l0, l1, l2;
	char *name;
	char *s0, *s1, *s2;
	s64 li;
	int i, j, k;

	if(!BCCX_TagIsP(l, "assign"))
		return(0);

	s0=BCCX_Get(l, "op");
	if(s0)
		return(0);

	ln=BCCX_Fetch(l, "left");
	rn=BCCX_Fetch(l, "right");
	ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
	rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

	name=NULL;
	if(BCCX_TagIsP(ln, "ref"))
		{ name=BCCX_Get(ln, "name"); }

	i=0;
	if(BCCX_TagIsP(rn, "int"))
	{
		li=BCCX_GetInt(rn, "value");
		if(((int)li)==li)
			i=1;
	}
	
	if(name && i)
	{
		*rname=name;
		*rval=li;
		return(1);
	}

	return(0);
}

int BGBCC_CCXL_IsFixIntCompareRVP(
	BGBCC_TransState *ctx, BCCX_Node *l,
	char **rname, char **rcmp, int *rval)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	ccxl_label l0, l1, l2;
	char *name, *op;
	char *s0, *s1, *s2;
	s64 li;
	int i, j, k;

	if(!BCCX_TagIsP(l, "binary"))
		return(0);

	s0=BCCX_Get(l, "op");
	if(!s0)
		return(0);
	
	op=NULL;
	if(!strcmp(s0, "=="))op=s0;
	if(!strcmp(s0, "!="))op=s0;
	if(!strcmp(s0, "<" ))op=s0;
	if(!strcmp(s0, ">" ))op=s0;
	if(!strcmp(s0, "<="))op=s0;
	if(!strcmp(s0, ">="))op=s0;

	ln=BCCX_Fetch(l, "left");
	rn=BCCX_Fetch(l, "right");
	ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
	rn=BGBCC_CCXL_ReduceExpr(ctx, rn);

	name=NULL;
	if(BCCX_TagIsP(ln, "ref"))
		{ name=BCCX_Get(ln, "name"); }

	i=0;
	if(BCCX_TagIsP(rn, "int"))
	{
		li=BCCX_GetInt(rn, "value");
		if(((int)li)==li)
			i=1;
	}
	
	if(name && op && i)
	{
		*rname=name;
		*rcmp=op;
		*rval=li;
		return(1);
	}

	return(0);
}

int BGBCC_CCXL_IsTagVarRVP(
	BGBCC_TransState *ctx, BCCX_Node *l,
	char *tag, char *name)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	ccxl_label l0, l1, l2;
	char *s0, *s1, *s2;
	s64 li;
	int i, j, k;

	if(!BCCX_TagIsP(l, tag))
		return(0);

	v=BCCX_Fetch(l, "value");
	v=BGBCC_CCXL_ReduceExpr(ctx, v);

	s0=NULL;
	if(BCCX_TagIsP(v, "ref"))
		{ s0=BCCX_Get(v, "name"); }
	
	if(s0 && name && !strcmp(s0, name))
		return(1);

	return(0);
}

BCCX_Node *BGBCC_CCXL_WrapIntSuf(s64 i, char *suf)
{
	BCCX_Node *t;
	t=BCCX_New("int");
	BCCX_SetInt(t, "value", i);
	if(suf)BCCX_Set(t, "tysuf", suf);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapRealSuf(double f, char *suf)
{
	BCCX_Node *t;
	t=BCCX_New("real");
	BCCX_SetFloat(t, "value", f);
	if(suf)BCCX_Set(t, "tysuf", suf);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapInt(s64 i)
{
	BCCX_Node *t;
	t=BCCX_New("int");
	BCCX_SetInt(t, "value", i);
	return(t);
}

BCCX_Node *BGBCC_CCXL_WrapReal(double f)
{
	BCCX_Node *t;
	t=BCCX_New("real");
	BCCX_SetFloat(t, "value", f);
	return(t);
}

int BGBCC_CCXL_IsIntP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "int"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsRealP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "int"))
		return(1);
	if(BCCX_TagIsP(l, "real"))
		return(1);
	return(0);
}

int BGBCC_CCXL_IsCharP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "charstring"))
		return(1);
	return(0);
}

BCCX_Node *BGBCC_CCXL_Boolify(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(BCCX_TagIsP(l, "boolify"))
		return(l);
	t=BCCX_New1("boolify", BCCX_New1("value", l));
	return(t);
}

//deal with number type suffixes
char *BGBCC_CCXL_BinaryTySuf(BGBCC_TransState *ctx, char *op,
	BCCX_Node *ln, BCCX_Node *rn)
{
	char *s0, *s1;

//	return(NULL);

	if(!ln || !rn)
		return(NULL);

	s0=BCCX_Get(ln, "tysuf");
	s1=BCCX_Get(rn, "tysuf");

	//throw out bad suffixes early
	if(s0 && BGBCC_CCXL_IsIntP(ctx, ln))
	{
		if(	!stricmp(s0, "F") ||
			!stricmp(s0, "D") ||
			!stricmp(s0, "M"))s0=NULL;
	}
	if(s1 && BGBCC_CCXL_IsIntP(ctx, rn))
	{
		if(	!stricmp(s1, "F") ||
			!stricmp(s1, "D") ||
			!stricmp(s1, "M"))s1=NULL;
	}

	if(s0 && BGBCC_CCXL_IsRealP(ctx, ln))
	{
		if(	!stricmp(s0, "L") ||
			!stricmp(s0, "LL") ||
			!stricmp(s0, "UL") ||
			!stricmp(s0, "ULL"))s0=NULL;
	}
	if(s1 && BGBCC_CCXL_IsRealP(ctx, rn))
	{
		if(	!stricmp(s1, "L") ||
			!stricmp(s1, "LL") ||
			!stricmp(s1, "UL") ||
			!stricmp(s1, "ULL"))s1=NULL;
	}

	//int+real, throw out int suffix
	if(BGBCC_CCXL_IsIntP(ctx, ln) && BGBCC_CCXL_IsRealP(ctx, rn)) s0=NULL;
	if(BGBCC_CCXL_IsRealP(ctx, ln) && BGBCC_CCXL_IsIntP(ctx, rn)) s1=NULL;
	
	if(!s0 && !s1) return(NULL);
	if(!s0) return(s1);
	if(!s1) return(s0);
	
	if(!strcmp(s0, s1))
		return(s0);

	//throw away 'F', as it is lesser
	if(!strcmp(s0, "F"))s0=NULL;
	if(!strcmp(s1, "F"))s1=NULL;
	if(!s0 && !s1) return(NULL);
	if(!s0) return(s1);
	if(!s1) return(s0);
	
	//ok, whatever for now...
	
	return(NULL);
}

BCCX_Node *BGBCC_CCXL_ReduceForm(BGBCC_TransState *ctx,
	BCCX_Node *l, int flag)
{
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s, *suf;
	double f, g;
	int i0, i1;
	s64 i, j;
//	int i, j;

	if(!l)return(l);

	if(	(BCCX_TagIsP(l, "value")) ||
		(BCCX_TagIsP(l, "left")) ||
		(BCCX_TagIsP(l, "right")) ||
		(BCCX_TagIsP(l, "cond")) ||
		(BCCX_TagIsP(l, "then")) ||
		(BCCX_TagIsP(l, "else")) ||
		(BCCX_TagIsP(l, "body")))
	{
		return(BGBCC_CCXL_ReduceForm(ctx,
			BCCX_Child(l), flag));
//		return(BCCX_Child(l));
	}

	if(BGBCC_CCXL_IsUnaryP(ctx, l, "!"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_Fetch(l, "value"), flag);

		if(BGBCC_CCXL_BoolExpr(ctx, t)==0)
		{
			BCCX_CheckDeleteUnlinked(t);
			return(BGBCP_Number(NULL, "1"));
		}
		if(BGBCC_CCXL_BoolExpr(ctx, t)==1)
		{
			BCCX_CheckDeleteUnlinked(t);
			return(BGBCP_Number(NULL, "0"));
		}

		ln=BCCX_Fetch(t, "left");
		rn=BCCX_Fetch(t, "right");

		ln=BGBCC_CCXL_ReduceForm(ctx, ln, flag);
		rn=BGBCC_CCXL_ReduceForm(ctx, rn, flag);

		x=NULL;
		if(BGBCC_CCXL_IsBinaryP(ctx, t, "=="))
			x=BGBCP_BinaryExpr("!=", ln, rn);
		if(BGBCC_CCXL_IsBinaryP(ctx, t, "!="))
			x=BGBCP_BinaryExpr("==", ln, rn);
		if(BGBCC_CCXL_IsBinaryP(ctx, t, "==="))
			x=BGBCP_BinaryExpr("!==", ln, rn);
		if(BGBCC_CCXL_IsBinaryP(ctx, t, "!=="))
			x=BGBCP_BinaryExpr("===", ln, rn);
		if(BGBCC_CCXL_IsBinaryP(ctx, t, "<"))
			x=BGBCP_BinaryExpr(">=", ln, rn);
		if(BGBCC_CCXL_IsBinaryP(ctx, t, ">"))
			x=BGBCP_BinaryExpr("<=", ln, rn);
		if(BGBCC_CCXL_IsBinaryP(ctx, t, "<="))
			x=BGBCP_BinaryExpr(">", ln, rn);
		if(BGBCC_CCXL_IsBinaryP(ctx, t, ">="))
			x=BGBCP_BinaryExpr("<", ln, rn);

		BCCX_CheckDeleteUnlinked(t);
		t=NULL;

		if(x)
		{
			v=BGBCC_CCXL_ReduceForm(ctx, x, flag);
			BCCX_CheckDeleteUnlinked(x);
			return(v);
		}
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "&&"))
	{
		ln=BGBCC_CCXL_ReduceForm(ctx, BCCX_Fetch(l, "left"), flag);
		rn=BGBCC_CCXL_ReduceForm(ctx, BCCX_Fetch(l, "right"), flag);

		if((BGBCC_CCXL_BoolExpr(ctx, ln)==0) ||
			(BGBCC_CCXL_BoolExpr(ctx, rn)==0))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "0"));
		}

		if(BGBCC_CCXL_BoolExpr(ctx, ln)==1)
		{
			BCCX_CheckDeleteUnlinked(ln);
			t=BGBCC_CCXL_Boolify(ctx, rn);
			return(t);
		}

		if(BGBCC_CCXL_BoolExpr(ctx, rn)==1)
		{
			BCCX_CheckDeleteUnlinked(rn);
			t=BGBCC_CCXL_Boolify(ctx, ln);
			return(t);
		}

		x=BGBCP_BinaryExpr("&&", ln, rn);
		return(x);
	}

	if(BGBCC_CCXL_IsBinaryP(ctx, l, "||"))
	{
		ln=BGBCC_CCXL_ReduceForm(ctx, BCCX_Fetch(l, "left"), flag);
		rn=BGBCC_CCXL_ReduceForm(ctx, BCCX_Fetch(l, "right"), flag);

		if((BGBCC_CCXL_BoolExpr(ctx, ln)==1) ||
			(BGBCC_CCXL_BoolExpr(ctx, rn)==1))
		{
			BCCX_CheckDeleteUnlinked(ln);
			BCCX_CheckDeleteUnlinked(rn);
			return(BGBCP_Number(NULL, "1"));
		}

		if(BGBCC_CCXL_BoolExpr(ctx, ln)==0)
		{
			BCCX_CheckDeleteUnlinked(ln);
			t=BGBCC_CCXL_Boolify(ctx, rn);
			return(t);
		}

		if(BGBCC_CCXL_BoolExpr(ctx, rn)==0)
		{
			BCCX_CheckDeleteUnlinked(rn);
			t=BGBCC_CCXL_Boolify(ctx, ln);
			return(t);
		}

		x=BGBCP_BinaryExpr("||", ln, rn);
		return(x);
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx, BCCX_Fetch(l, "value"), flag);

		if(BGBCC_CCXL_IsCharP(ctx, t))
		{
			s=BCCX_Get(t, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(t);
			t=BGBCC_CCXL_WrapInt(i);
		}

		s=BCCX_Get(l, "op");

		if(BGBCC_CCXL_IsIntP(ctx, t))
		{
			i=BCCX_GetInt(t, "value");
			suf=BCCX_Get(l, "tysuf");

			if(!strcmp(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapIntSuf(i, suf));
			}
			if(!strcmp(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapIntSuf(-i, suf));
			}
			if(!strcmp(s, "~"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapIntSuf(~i, suf));
			}
			if(!strcmp(s, "!"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapInt(!i));
			}
		}

		if(BGBCC_CCXL_IsRealP(ctx, t))
		{
			f=BCCX_GetFloat(t, "value");
			suf=BCCX_Get(l, "tysuf");
			if(!strcmp(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapRealSuf(f, suf));
			}
			if(!strcmp(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapRealSuf(-f, suf));
			}
			if(!strcmp(s, "!"))
			{
				BCCX_CheckDeleteUnlinked(t);
				return(BGBCC_CCXL_WrapInt(f!=0));
			}
		}

		x=BGBCP_UnaryExpr(s, t);
		return(x);
	}

	if(BCCX_TagIsP(l, "binary"))
	{
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");

		ln=BGBCC_CCXL_ReduceForm(ctx, ln, flag);
		rn=BGBCC_CCXL_ReduceForm(ctx, rn, flag);

		if(BGBCC_CCXL_IsCharP(ctx, ln))
		{
			s=BCCX_Get(ln, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(ln);
			ln=BGBCC_CCXL_WrapInt(i);
		}

		if(BGBCC_CCXL_IsCharP(ctx, rn))
		{
			s=BCCX_Get(rn, "value");
			if(s) { i=BGBCP_ParseChar(&s); }
				else i=0;
			BCCX_CheckDeleteUnlinked(rn);
			rn=BGBCC_CCXL_WrapInt(i);
		}

		s=BCCX_Get(l, "op");

		if(BGBCC_CCXL_IsIntP(ctx, ln) && BGBCC_CCXL_IsIntP(ctx, rn))
		{
			i=BCCX_GetInt(ln, "value");
			j=BCCX_GetInt(rn, "value");

			if(!strcmp(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i==j));
			}
			if(!strcmp(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i!=j));
			}
			if(!strcmp(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i==j));
			}
			if(!strcmp(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i!=j));
			}
			if(!strcmp(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i<=j));
			}
			if(!strcmp(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i>=j));
			}
			if(!strcmp(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i<j));
			}
			if(!strcmp(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i>j));
			}

			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);
			if(!strcmp(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i+j, suf));
			}
			if(!strcmp(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i-j, suf));
			}
			if(!strcmp(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i*j, suf));
			}
			if(!strcmp(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i/j, suf));
			}
			if(!strcmp(s, "%"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i%j, suf));
			}

			if(!strcmp(s, "&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i&j, suf));
			}
			if(!strcmp(s, "|"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i|j, suf));
			}
			if(!strcmp(s, "^"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i^j, suf));
			}
			if(!strcmp(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i/j, suf));
			}

			if(!strcmp(s, "&&"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i&&j));
			}
			if(!strcmp(s, "||"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(i||j));
			}

			if(!strcmp(s, "<<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i<<j, suf));
			}
			if(!strcmp(s, ">>"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapIntSuf(i>>j, suf));
			}
		}

		if(BGBCC_CCXL_IsRealP(ctx, ln) && BGBCC_CCXL_IsRealP(ctx, rn))
		{
			f=BCCX_GetFloat(ln, "value");
			g=BCCX_GetFloat(rn, "value");

			if(!strcmp(s, "=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f==g));
			}
			if(!strcmp(s, "!="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f!=g));
			}
			if(!strcmp(s, "==="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f==g));
			}
			if(!strcmp(s, "!=="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f!=g));
			}
			if(!strcmp(s, "<="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f<=g));
			}
			if(!strcmp(s, ">="))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f>=g));
			}
			if(!strcmp(s, "<"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f<g));
			}
			if(!strcmp(s, ">"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f>g));
			}

			suf=BGBCC_CCXL_BinaryTySuf(ctx, s, ln, rn);

			if(!strcmp(s, "+"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f+g, suf));
			}
			if(!strcmp(s, "-"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f-g, suf));
			}
			if(!strcmp(s, "*"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f*g, suf));
			}
			if(!strcmp(s, "/"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapRealSuf(f/g, suf));
			}
			if(!strcmp(s, "\\"))
			{
				BCCX_CheckDeleteUnlinked(ln);
				BCCX_CheckDeleteUnlinked(rn);
				return(BGBCC_CCXL_WrapInt(f/g));
			}
		}

		x=BGBCP_BinaryExpr(s, ln, rn);
		return(x);
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BGBCC_CCXL_ReduceForm(ctx,
				BCCX_Fetch(l, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			BCCX_CheckDeleteUnlinked(t);

			t=BCCX_Fetch(l, "else");
			if(t)t=BGBCC_CCXL_ReduceForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_Fetch(l, "then");
		rn=BCCX_Fetch(l, "else");

		ln=BGBCC_CCXL_ReduceForm(ctx, ln, flag);
		rn=BGBCC_CCXL_ReduceForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		return(x);
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		if(!BCCX_Child(l))
		{
			return(NULL);
		}

		if(!BCCX_Next(BCCX_Child(l)))
		{
			t=BGBCC_CCXL_ReduceForm(ctx, BCCX_Child(l), flag);
			return(t);
		}
		
		x=BCCX_New("begin");
		c=BCCX_Child(l);
		while(c)
		{
			t=BGBCC_CCXL_ReduceForm(ctx, c, flag);
			BCCX_Add(x, t);
			c=BCCX_Next(c);
		}
		return(x);
	}

	if(BCCX_TagIsP(l, "ref"))
	{
		if(!ctx)	//preprocessor
		{
			t=BGBCC_CCXL_WrapInt(0);
			return(t);
		}

		return(BCCX_Clone(l));
	}

	if(BCCX_TagIsP(l, "sizeof"))
	{
		t=BCCX_FindTag(l, "type");
		if(!t)
		{
			t=BCCX_Fetch(l, "tyexpr");
			if(t)t=BCCX_FindTag(t, "type");
		}

		if(ctx && t)
		{
			i=BGBCC_CCXL_TryGetSizeofType(ctx, t);
			if(i>0)return(BGBCC_CCXL_WrapInt(i));
			
			if(flag&1)
			{
				i=BGBCC_CCXL_GetMinMaxSizeofType(ctx, t,
					NULL, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
					return(BGBCC_CCXL_WrapInt(i0));
			}
		}
	}

	if(BCCX_TagIsP(l, "sizeof_expr"))
	{
		t=BCCX_Fetch(l, "value");

		if(BGBCC_CCXL_IsUnaryP(ctx, t, "&"))
		{
			if(flag&1)
				return(BGBCC_CCXL_WrapInt(8));
		}

		if(BCCX_TagIsP(t, "ref"))
		{
			s=BCCX_Get(t, "name");

			i=BGBCC_CCXL_TryGetSizeofName(ctx, s);
			if(i>0)return(BGBCC_CCXL_WrapInt(i));
			
			if(flag&1)
			{
				i=BGBCC_CCXL_GetMinMaxSizeofName(ctx, s,
					NULL, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
					return(BGBCC_CCXL_WrapInt(i0));
			}
		}

		if(BCCX_TagIsP(t, "loadindex"))
		{
			t=BCCX_Fetch(t, "array");
			if(BCCX_TagIsP(t, "ref"))
			{
				s=BCCX_Get(t, "name");

				i=BGBCC_CCXL_GetMinMaxSizeofDerefName(ctx, s,
					&i1, &i0, NULL, NULL);
				if((i>=0) && (i0>0))
				{
					if((i0==i1) || (flag&1))
						return(BGBCC_CCXL_WrapInt(i0));
				}
			}
		}

		if(BCCX_TagIsP(t, "string"))
		{
			s=BCCX_Get(t, "value");
			if(s)
			{
				i=strlen(s);
				return(BGBCC_CCXL_WrapInt(i));
			}
		}
	}

	return(BCCX_Clone(l));
}

int BGBCC_CCXL_SetLine(BGBCC_TransState *ctx, BCCX_Node *l,
	char *fn, int ln)
{
	if(fn)BCCX_Set(l, "fn", fn);
	if(ln>0)BCCX_SetInt(l, "ln", ln);
	return(0);
}

BCCX_Node *BGBCC_CCXL_ReduceStatementForm(BGBCC_TransState *ctx,
	BCCX_Node *l, int flag)
{
	BCCX_Node *c, *t, *v, *x, *ln, *rn;
	char *s, *fnam;
	double f, g;
	int i, j, lnum;

	if(!l)return(l);

	fnam=BCCX_Get(l, "fn");
	lnum=BCCX_GetInt(l, "ln");

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			t=BGBCC_CCXL_ReduceStatementForm(ctx,
				BCCX_Fetch(l, "then"), flag);
			return(t);
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)t=BGBCC_CCXL_ReduceStatementForm(ctx, t, flag);
			return(t);
		}

		ln=BCCX_Fetch(l, "then");
		rn=BCCX_Fetch(l, "else");

		ln=BGBCC_CCXL_ReduceStatementForm(ctx, ln, flag);
		rn=BGBCC_CCXL_ReduceStatementForm(ctx, rn, flag);
		x=BGBCP_IfExpr(t, ln, rn);
		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		if(!BCCX_Child(l))
		{
			return(NULL);
		}

		if(!BCCX_Next(BCCX_Child(l)))
		{
			t=BGBCC_CCXL_ReduceStatementForm(ctx, BCCX_Child(l), flag);
			return(t);
		}
		
		x=BCCX_New("begin");
		c=BCCX_Child(l);
		while(c)
		{
			t=BGBCC_CCXL_ReduceStatementForm(ctx, c, flag);
			BCCX_Add(x, t);
			c=BCCX_Next(c);
		}

		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsP(l, "while"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)
		{
			return(NULL);
		}

		v=BGBCC_CCXL_ReduceStatementForm(ctx,
			BCCX_Fetch(l, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_New2("while",
			BCCX_New1("cond", t),
			BCCX_New1("body", v));

		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);
		return(x);
	}

	if(BCCX_TagIsP(l, "do_while"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx,
			BCCX_Fetch(l, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)
		{
			v=BGBCC_CCXL_ReduceStatementForm(ctx,
				BCCX_Fetch(l, "body"), flag);
			return(v);
		}

		v=BGBCC_CCXL_ReduceStatementForm(ctx,
			BCCX_Fetch(l, "body"), flag);
		if(!v)return(NULL);

		x=BCCX_New2("do_while",
			BCCX_New1("cond", t),
			BCCX_New1("body", v));
		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	if(BCCX_TagIsP(l, "for"))
	{
		t=BGBCC_CCXL_ReduceForm(ctx, BCCX_Fetch(l, "cond"), flag);
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)
		{
			ln=BGBCC_CCXL_ReduceStatementForm(ctx,
				BCCX_Fetch(l, "init"), flag);
			return(ln);
		}

		ln=BGBCC_CCXL_ReduceStatementForm(ctx, BCCX_Fetch(l, "init"), flag);
		rn=BGBCC_CCXL_ReduceStatementForm(ctx, BCCX_Fetch(l, "step"), flag);

		v=BGBCC_CCXL_ReduceStatementForm(ctx, BCCX_Fetch(l, "body"), flag);

		x=BCCX_New4("for",
			BCCX_New1("init", ln),
			BCCX_New1("cond", t),
			BCCX_New1("step", rn),
			BCCX_New1("body", v));
		BGBCC_CCXL_SetLine(ctx, x, fnam, lnum);

		return(x);
	}

	t=BGBCC_CCXL_ReduceForm(ctx, l, flag);
	return(t);
}

BCCX_Node *BGBCC_CCXL_ReduceExpr(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCC_CCXL_ReduceForm(ctx, l, 0);

	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}

BCCX_Node *BGBCC_CCXL_ReduceStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCC_CCXL_ReduceStatementForm(ctx, l, 0);

	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}


BCCX_Node *BGBCC_CCXL_ReduceExprConst(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *t;
	if(!l)return(NULL);
	t=BGBCC_CCXL_ReduceForm(ctx, l, 1);
	
	if(t)
	{
		t->hnext=ctx->reduce_tmp;
		ctx->reduce_tmp=t;
	}

	return(t);
}
