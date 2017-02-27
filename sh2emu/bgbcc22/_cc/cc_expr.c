#include <bgbccc.h>


char *BGBCC_CIfy(char *s)
{
	static char buf[1024];
	char *t;
	int i;

	t=buf;
	*t++='\"';

	while(*s)
	{
		i=BGBCP_ParseChar(&s);

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
		*t++=i;
	}

	*t++='\"';
	*t=0;

	return(buf);
}

void BGBCC_CompileAssign(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t;
	char *s;
	int i, j;

	if(BCCX_TagIsP(l, "ref"))
	{
		bgbcc_print(ctx, "=%s ", BCCX_Get(l, "name"));
		return;
	}

	if(BGBCC_IsUnaryP(l, "*"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "0 storeindex ");
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "$%s storeslot ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex") || BCCX_TagIsP(l, "vector-ref"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		bgbcc_print(ctx, "storeindex ");
		return;
	}

	BGBCC_Error("bad lvalue in expression\n");
}

void BGBCC_CompileExprList(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_CompileExpr(ctx, cur);
		cur=BCCX_Next(cur);
	}
}

void BGBCC_CompileExprListReverse(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)BGBCC_CompileExpr(ctx, stk[i]);
}

void BGBCC_CompileArgsList(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--) { BGBCC_CompileExpr(ctx, stk[i]); bgbcc_print(ctx, ", "); }
}

void BGBCC_CompileFuncall(BGBCC_State *ctx, BCCX_Node *l)
{
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v;
	int i, j;
	
	if(BCCX_AttrIsP(l, "name", "__fvec") ||
		BCCX_AttrIsP(l, "name", "__fvec2") ||
		BCCX_AttrIsP(l, "name", "__fvec3") ||
		BCCX_AttrIsP(l, "name", "__fvec4"))
	{
		c=BCCX_Fetch(l, "args"); i=0;
		while(c)
		{
			BGBCC_CompileExpr(ctx, c);
			bgbcc_print(ctx, "$f cast ");
			c=BCCX_Next(c); i++;
		}

		if((i<2) || (i>4))
			BGBCC_Error("__fvec only valid with "
				"2, 3, or 4 args\n");
		bgbcc_print(ctx, "fvec%d ", i);
		return;
	}

	if(BCCX_AttrIsP(l, "name", "__qvec"))
	{
		c=BCCX_Fetch(l, "args"); i=0;
		while(c)
		{
			BGBCC_CompileExpr(ctx, c);
			bgbcc_print(ctx, "$f cast ");
			c=BCCX_Next(c); i++;
		}

		if(i!=4)BGBCC_Error("__qvec only valid with "
			"4 args\n");
		bgbcc_print(ctx, "quat ");
		return;
	}

	s0=BCCX_Get(l, "name");
	if(s0)
	{
		i=0;
		s1="__builtin_";
		if(!strncmp(s0, s1, strlen(s1)))i=1;
		if(!strcmp(s0, "sqrt"))i=1;
		if(!strcmp(s0, "sin"))i=1;
		if(!strcmp(s0, "cos"))i=1;
		if(!strcmp(s0, "tan"))i=1;

		if(!strcmp(s0, "creal"))i=1;
		if(!strcmp(s0, "cimag"))i=1;

		if(i)
		{
			bgbcc_print(ctx, "mark ");
			c=BCCX_Fetch(l, "args");
			BGBCC_CompileExprListReverse(ctx,
				BCCX_Fetch(l, "args"));
			bgbcc_print(ctx, "$%s builtin ", s0);
			return;
		}

		bgbcc_print(ctx, "%s( ", s0);
		BGBCC_CompileArgsList(ctx, BCCX_Fetch(l, "args"));
		bgbcc_print(ctx, ") ");
		return;
	}


	bgbcc_print(ctx, "mark ");

	BGBCC_CompileExprListReverse(ctx,
		BCCX_Fetch(l, "args"));
	BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "func"));
	bgbcc_print(ctx, "call ");
	return;
}

void BGBCC_CompileForm(BGBCC_State *ctx, BCCX_Node *l)
{
	byte *ips[64];
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn;
	int i, j;

	if(BCCX_TagIsP(l, "return"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "ret ");
		return;
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		s0=BGBCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "$%s cast ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "sizeof"))
	{
		t=BCCX_FindTag(l, "type");
		if(!t)
		{
			u=BCCX_FindTag(l, "tyexpr"); v=NULL;
			if(u)
			{
				v=BCCX_FindTag(u, "var");
				if(!v)v=BCCX_FindTag(u, "proto");
			}
			if(v)t=BCCX_FindTag(v, "type");
		}

		if(!t)
		{
			printf("BGBCC_CompileForm: sizeof: bad AST\n");
			bgbcc_print(ctx, "0 ");
			return;
		}

		s0=BGBCC_VarTypeString(ctx, t);
		bgbcc_print(ctx, "$%s sizeof_t ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "sizeof_expr"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "sizeof ");
		return;
	}
	if(BCCX_TagIsP(l, "offsetof"))
	{
		s0=BGBCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "$%s $%s offsetof ", s0, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		s0=BCCX_Get(l, "op");

		if(s0)
		{
			BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
			BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
			bgbcc_print(ctx, "%s dup ", s0);
			BGBCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
			return;
		}

		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, "dup ");
		BGBCC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		bgbcc_print(ctx, "loadindex ");
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "$%s loadslot ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CompileExpr(ctx, t);
		bgbcc_print(ctx, "inc dup ");
		BGBCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dec dup ");
		BGBCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dup inc ");
		BGBCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dup dec ");
		BGBCC_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_CompileExpr(ctx, t);
			return;
		}

		bgbcc_print(ctx, "beginu ");
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();
		BGBCC_CompileJCF(ctx, t, s0);

		BGBCC_CompileExprT(ctx, BCCX_Fetch(l, "then"));
		bgbcc_print(ctx, "setu ");

		BGBCC_CompileJmp(ctx, s1);
		BGBCC_EmitLabel(ctx, s0);

		t=BCCX_Fetch(l, "else");
		if(t)BGBCC_CompileExprT(ctx, t);
			else bgbcc_print(ctx, "push_null ");
		bgbcc_print(ctx, "setu ");

		BGBCC_EmitLabel(ctx, s1);

		bgbcc_print(ctx, "endu ");
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(1)
		{
			if(BCCX_Next(c))
			{
				BGBCC_CompileStatement(ctx, c);
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CompileExprT(ctx, c);
			break;
		}

		return;
	}

#if 0
	if((BCCX_TagIsP(l, "and")) || (BCCX_TagIsP(l, "or")))
	{
		if(BCCX_TagIsP(l, "and"))s1="false";
		if(BCCX_TagIsP(l, "or"))s1="true";

		s0=BGBCC_GenSym();

		i=0; c=BCCX_Child(l);
		while(1)
		{
			if(BCCX_Next(c))
			{
				BGBCC_CompileExpr(ctx, c);
				bgbcc_print(ctx, "dup ");
				bgbcc_print(ctx, "$%s jmp_%s ", s0, s1);
				bgbcc_print(ctx, "pop ");
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CompileExprT(ctx, c);

			bgbcc_print(ctx, "%s: boolify ", s0);
			break;
		}

		return;
	}
#endif

	if(BGBCC_IsBinaryP(l, "&&"))
	{
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();

		bgbcc_print(ctx, "beginu ");
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		bgbcc_print(ctx, "!%s ", s0);
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, "!%s ", s0);
		bgbcc_print(ctx, "1 setu *%s ", s1);
		bgbcc_print(ctx, "%s: 0 setu ", s0);
		bgbcc_print(ctx, "%s: endu ", s1);
		return;
	}

	if(BGBCC_IsBinaryP(l, "||"))
	{
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();

		bgbcc_print(ctx, "beginu ");
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		bgbcc_print(ctx, "?%s ", s0);
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, "?%s ", s0);
		bgbcc_print(ctx, "0 setu *%s ", s1);
		bgbcc_print(ctx, "%s: 1 setu ", s0);
		bgbcc_print(ctx, "%s: endu ", s1);
		return;
	}

	if(BCCX_TagIsP(l, "boolify"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "boolify ");
		return;
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		if(BCCX_AttrIsP(l, "op", "*"))
		{
			BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
			bgbcc_print(ctx, "0 loadindex ");
			return;
		}

		if(BCCX_AttrIsP(l, "op", "&"))
		{
			t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "value"));

			if(BCCX_TagIsP(t, "ref"))
			{
				s0=BCCX_Get(t, "name");
				bgbcc_print(ctx, "$%s loada ", s0);
				return;
			}

			if(BCCX_TagIsP(t, "objref"))
			{
				BGBCC_CompileExpr(ctx, BCCX_Fetch(t, "value"));
				bgbcc_print(ctx, "$%s loadslota ", BCCX_Get(t, "name"));
				return;
			}

			if(BCCX_TagIsP(t, "getindex") ||
				BCCX_TagIsP(t, "vector-ref"))
			{
				BGBCC_CompileExpr(ctx, BCCX_Fetch(t, "array"));
				BGBCC_CompileExpr(ctx, BCCX_Fetch(t, "index"));
				bgbcc_print(ctx, "loadindexa ");
				return;
			}

			BGBCC_Error("Bad expression type for '&'\n");
			return;
		}

		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));

		s0=NULL;
		if(BCCX_AttrIsP(l, "op", "+"))s0="";
		if(BCCX_AttrIsP(l, "op", "-"))s0="neg";
		if(BCCX_AttrIsP(l, "op", "~"))s0="not";
		if(BCCX_AttrIsP(l, "op", "!"))s0="lnot";
		if(BCCX_AttrIsP(l, "op", "*"))s0="defer";
		if(BCCX_AttrIsP(l, "op", "&"))s0="loada";

		bgbcc_print(ctx, "%s ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "binary"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "right"));

		s0="";

		if(BCCX_AttrIsP(l, "op", "+"))s0="+";
		if(BCCX_AttrIsP(l, "op", "-"))s0="-";
		if(BCCX_AttrIsP(l, "op", "*"))s0="*";
		if(BCCX_AttrIsP(l, "op", "/"))s0="/";
		if(BCCX_AttrIsP(l, "op", "%"))s0="%";
		if(BCCX_AttrIsP(l, "op", "&"))s0="&";
		if(BCCX_AttrIsP(l, "op", "|"))s0="|";
		if(BCCX_AttrIsP(l, "op", "^"))s0="^";

		if(BCCX_AttrIsP(l, "op", "~"))s0="nand";
		if(BCCX_AttrIsP(l, "op", "\\"))s0="idiv";
		if(BCCX_AttrIsP(l, "op", "**"))s0="exp";

		if(BCCX_AttrIsP(l, "op", "+`"))s0="add1";
		if(BCCX_AttrIsP(l, "op", "-`"))s0="sub1";
		if(BCCX_AttrIsP(l, "op", "*`"))s0="mul1";
		if(BCCX_AttrIsP(l, "op", "/`"))s0="div1";
		if(BCCX_AttrIsP(l, "op", "%`"))s0="mod1";
		if(BCCX_AttrIsP(l, "op", "&`"))s0="and1";
		if(BCCX_AttrIsP(l, "op", "|`"))s0="or1";
		if(BCCX_AttrIsP(l, "op", "^`"))s0="xor1";

		if(BCCX_AttrIsP(l, "op", "`+"))s0="add2";
		if(BCCX_AttrIsP(l, "op", "`-"))s0="sub2";
		if(BCCX_AttrIsP(l, "op", "`*"))s0="mul2";
		if(BCCX_AttrIsP(l, "op", "`/"))s0="div2";
		if(BCCX_AttrIsP(l, "op", "`%"))s0="mod2";
		if(BCCX_AttrIsP(l, "op", "`&"))s0="and2";
		if(BCCX_AttrIsP(l, "op", "`|"))s0="or2";
		if(BCCX_AttrIsP(l, "op", "`^"))s0="xor2";

		if(BCCX_AttrIsP(l, "op", "<<"))s0="shl";
		if(BCCX_AttrIsP(l, "op", ">>"))s0="shr";

		if(BCCX_AttrIsP(l, "op", "=="))s0="==";
		if(BCCX_AttrIsP(l, "op", "!="))s0="!=";
		if(BCCX_AttrIsP(l, "op", "==="))s0="==";
		if(BCCX_AttrIsP(l, "op", "!=="))s0="!=";
		if(BCCX_AttrIsP(l, "op", "<"))s0="<";
		if(BCCX_AttrIsP(l, "op", ">"))s0=">";
		if(BCCX_AttrIsP(l, "op", "<="))s0="<=";
		if(BCCX_AttrIsP(l, "op", ">="))s0=">=";

		bgbcc_print(ctx, "%s ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "funcall"))
	{
		BGBCC_CompileFuncall(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "methodcall"))
	{
		bgbcc_print(ctx, "mark ");
		c=BCCX_Fetch(l, "args");
		BGBCC_CompileExprListReverse(ctx, c);
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "$%s methodcall ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "new"))
	{
		bgbcc_print(ctx, "mark ");
		c=BCCX_Fetch(l, "args");
		BGBCC_CompileExprListReverse(ctx, c);
		
		s0=BGBCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "$%s new ", s0);
		
//		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
//		bgbcc_print(ctx, "$%s methodcall ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "throw"))
	{
		BGBCC_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "throw ");
		return;
	}

	BGBCC_Error("unhandled expr %s\n", BCCX_Tag(l));
	bgbcc_print(ctx, "push_null ");
}

void BGBCC_CompileExprT(BGBCC_State *ctx, BCCX_Node *l)
{
	long long li;
	float f;
	BCCX_Node *t, *c, *ct, *cv;
	char *s0, *s1, *s2;
	int i, j;

	l=BGBCC_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "ref"))
	{
		s0=BCCX_Get(l, "name");
		
		if((ctx->lang==BGBCC_LANG_JAVA) || (ctx->lang==BGBCC_LANG_CS))
		{
			if(!strcmp(s0, "true"))
				{ bgbcc_print(ctx, "push_true "); return; }
			if(!strcmp(s0, "false"))
				{ bgbcc_print(ctx, "push_false "); return; }
			if(!strcmp(s0, "null"))
				{ bgbcc_print(ctx, "push_null "); return; }
		}
		
		bgbcc_print(ctx, "%%%s ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "int"))
	{
		bgbcc_print(ctx, "%s ", BCCX_Get(l, "value"));
		return;
	}
	if(BCCX_TagIsP(l, "real"))
	{
		bgbcc_print(ctx, "%s ", BCCX_Get(l, "value"));
		return;
	}

	if(BCCX_TagIsP(l, "string"))
	{
		bgbcc_print(ctx, "%s ", BGBCC_CIfy(BCCX_Get(l, "value")));
		return;
	}

	if(BCCX_TagIsP(l, "complex"))
	{
		s0=BCCX_Get(l, "real"); s1=BCCX_Get(l, "imag");
		if(!s0)s0="0"; if(!s1)s1="0";
		bgbcc_print(ctx, "%s %s complex ", s0, s1);
		return;
	}

	BGBCC_CompileForm(ctx, l);
	return;
}

void BGBCC_CompileExpr(BGBCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=0;
	BGBCC_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_CompileExprTail(BGBCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=1;
	BGBCC_CompileExprT(ctx, l);
	ctx->tail=i;
}
