#include <bgbccc.h>


void BGBCC_BS2IL_CompileAssign(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t;
	char *s;
	int i, j;

	if(BCCX_TagIsP(l, "ref"))
	{
		bgbcc_print(ctx, "store %s; ", BCCX_Get(l, "name"));
		return;
	}

	if(BGBCC_IsUnaryP(l, "*"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "storeindex_i 0; ");
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "storeslot %s; ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex") || BCCX_TagIsP(l, "vector-ref"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		bgbcc_print(ctx, "storeindex; ");
		return;
	}

	BGBCC_Error("bad lvalue in expression\n");
}

void BGBCC_BS2IL_CompileExprList(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *cur;

	cur=lst;
	while(cur)
	{
		BGBCC_BS2IL_CompileExpr(ctx, cur);
		cur=BCCX_Next(cur);
	}
}

void BGBCC_BS2IL_CompileExprListReverse(BGBCC_State *ctx, BCCX_Node *lst)
{
	BCCX_Node *stk[128];
	BCCX_Node *cur;
	int i;

	i=0; cur=lst;
	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
	while(i--)BGBCC_BS2IL_CompileExpr(ctx, stk[i]);
}

void BGBCC_BS2IL_CompileArgsList(BGBCC_State *ctx, BCCX_Node *lst)
{
//	BCCX_Node *stk[128];
//	BCCX_Node *cur;
//	int i;

//	i=0; cur=lst;
//	while(cur) { stk[i++]=cur; cur=BCCX_Next(cur); }
//	while(i--) { BGBCC_BS2IL_CompileExpr(ctx, stk[i]); bgbcc_print(ctx, ", "); }

	BGBCC_BS2IL_CompileExprList(ctx, lst);
}

void BGBCC_BS2IL_CompileFuncall(BGBCC_State *ctx, BCCX_Node *l)
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
			BGBCC_BS2IL_CompileExpr(ctx, c);
			bgbcc_print(ctx, "cast_s f; ");
			c=BCCX_Next(c); i++;
		}

		if((i<2) || (i>4))
			BGBCC_Error("__fvec only valid with "
				"2, 3, or 4 args\n");
		bgbcc_print(ctx, "fvec%d; ", i);
		return;
	}

	if(BCCX_AttrIsP(l, "name", "__qvec"))
	{
		c=BCCX_Fetch(l, "args"); i=0;
		while(c)
		{
			BGBCC_BS2IL_CompileExpr(ctx, c);
			bgbcc_print(ctx, "cast_s f; ");
			c=BCCX_Next(c); i++;
		}

		if(i!=4)BGBCC_Error("__qvec only valid with "
			"4 args\n");
		bgbcc_print(ctx, "quat; ");
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
			s1="__builtin_";
			if(!strncmp(s0, s1, strlen(s1)))
				s0+=strlen(s1);

			bgbcc_print(ctx, "mark; ");
			c=BCCX_Fetch(l, "args");
//			BGBCC_BS2IL_CompileExprListReverse(ctx,
//				BCCX_Fetch(l, "args"));
			BGBCC_BS2IL_CompileExprList(ctx,
				BCCX_Fetch(l, "args"));
			bgbcc_print(ctx, "builtin %s; ", s0);
			return;
		}

//		bgbcc_print(ctx, "%s( ", s0);
		bgbcc_print(ctx, "mark; ");
		BGBCC_BS2IL_CompileArgsList(ctx, BCCX_Fetch(l, "args"));
		bgbcc_print(ctx, "call_s %s; ", s0);
//		bgbcc_print(ctx, ") ");
		return;
	}


	bgbcc_print(ctx, "mark; ");

	BGBCC_BS2IL_CompileArgsList(ctx,
		BCCX_Fetch(l, "args"));
	BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "func"));
	bgbcc_print(ctx, "call; ");
	return;
}

void BGBCC_BS2IL_CompileForm(BGBCC_State *ctx, BCCX_Node *l)
{
	byte *ips[64];
	char *s0, *s1, *s2;
	BCCX_Node *c, *d, *t, *u, *v, *ln, *rn;
	int i, j;

	if(BCCX_TagIsP(l, "return"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "ret; ");
		return;
	}

	if(BCCX_TagIsP(l, "cast"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		s0=BGBCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "cast_s \"%s\"; ", s0);
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
			printf("BGBCC_BS2IL_CompileForm: sizeof: bad AST\n");
			bgbcc_print(ctx, "0 ");
			return;
		}

		s0=BGBCC_VarTypeString(ctx, t);
		bgbcc_print(ctx, "sizeof_s \"%s\"; ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "sizeof_expr"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "sizeof; ");
		return;
	}
	if(BCCX_TagIsP(l, "offsetof"))
	{
		s0=BGBCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "offsetof \"%s\", \"%s\"; ",
			s0, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		s0=BCCX_Get(l, "op");

		if(s0)
		{
			s1=NULL;
			if(!strcmp(s0, "+"))s1="add";
			if(!strcmp(s0, "-"))s1="sub";
			if(!strcmp(s0, "*"))s1="mul";
			if(!strcmp(s0, "/"))s1="div";
			if(!strcmp(s0, "%"))s1="mod";
			if(!strcmp(s0, "&"))s1="and";
			if(!strcmp(s0, "|"))s1="or";
			if(!strcmp(s0, "^"))s1="xor";

			if(!strcmp(s0, "~"))s1="nand";
			if(!strcmp(s0, "\\"))s1="idiv";
			if(!strcmp(s0, "**"))s1="exp";

			if(!strcmp(s0, "<<"))s1="shl";
			if(!strcmp(s0, ">>"))s1="shr";
			if(!strcmp(s0, ">>>"))s1="shrr";

			if(!s1)s1=s0;

			BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "left"));
			BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "right"));
			bgbcc_print(ctx, "%s; dup; ", s1);
			BGBCC_BS2IL_CompileAssign(ctx, BCCX_Fetch(l, "left"));
			return;
		}

		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, "dup; ");
		BGBCC_BS2IL_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		return;
	}

	if(BCCX_TagIsP(l, "getindex"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "array"));
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "index"));
		bgbcc_print(ctx, "loadindex; ");
		return;
	}

	if(BCCX_TagIsP(l, "objref"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "loadslot %s; ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_BS2IL_CompileExpr(ctx, t);
		bgbcc_print(ctx, "inc; dup; ");
		BGBCC_BS2IL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_BS2IL_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dec; dup; ");
		BGBCC_BS2IL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_BS2IL_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dup; inc; ");
		BGBCC_BS2IL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		BGBCC_BS2IL_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dup; dec; ");
		BGBCC_BS2IL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_BS2IL_CompileExpr(ctx, t);
			return;
		}

//		bgbcc_print(ctx, "beginu ");
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();
		BGBCC_BS2IL_CompileJCF(ctx, t, s0);

		BGBCC_BS2IL_CompileExprT(ctx, BCCX_Fetch(l, "then"));
//		bgbcc_print(ctx, "setu ");

		BGBCC_BS2IL_CompileJmp(ctx, s1);
		bgbcc_print(ctx, "pop; ");
		BGBCC_BS2IL_EmitLabel(ctx, s0);

		t=BCCX_Fetch(l, "else");
		if(t)BGBCC_BS2IL_CompileExprT(ctx, t);
			else bgbcc_print(ctx, "push_null ");
//		bgbcc_print(ctx, "setu ");

		BGBCC_BS2IL_EmitLabel(ctx, s1);

//		bgbcc_print(ctx, "endu ");
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		bgbcc_print(ctx, "\n");
		c=BCCX_Child(l);
		while(1)
		{
			if(BCCX_Next(c))
			{
				bgbcc_print(ctx, "\t");
				BGBCC_BS2IL_CompileStatement(ctx, c);
				bgbcc_print(ctx, "\n");
				c=BCCX_Next(c);
				continue;
			}

			bgbcc_print(ctx, "\t");
			BGBCC_BS2IL_CompileExprT(ctx, c);
			bgbcc_print(ctx, "\n");
			break;
		}

		return;
	}

	if(BGBCC_IsBinaryP(l, "&&"))
	{
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();

		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		bgbcc_print(ctx, "jmp_false %s; ", s0);
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, "jmp_false %s; ", s0);
		bgbcc_print(ctx, "push_i 1; jmp %s; ", s1);
		bgbcc_print(ctx, "pop; ");
		bgbcc_print(ctx, "%s: push_i 0; ", s0);
		bgbcc_print(ctx, "%s: ", s1);
		return;
	}

	if(BGBCC_IsBinaryP(l, "||"))
	{
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();

		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		bgbcc_print(ctx, "jmp_true %s; ", s0);
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, "jmp_true %s; ", s0);
		bgbcc_print(ctx, "push_i 0; jmp %s; ", s1);
		bgbcc_print(ctx, "pop; ");
		bgbcc_print(ctx, "%s: push_i 1; ", s0);
		bgbcc_print(ctx, "%s: ", s1);
		return;
	}

	if(BCCX_TagIsP(l, "boolify"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "boolify; ");
		return;
	}

	if(BCCX_TagIsP(l, "unary"))
	{
		if(BCCX_AttrIsP(l, "op", "*"))
		{
			BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
			bgbcc_print(ctx, "loadindex_i 0; ");
			return;
		}

		if(BCCX_AttrIsP(l, "op", "&"))
		{
			t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "value"));

			if(BCCX_TagIsP(t, "ref"))
			{
				s0=BCCX_Get(t, "name");
				bgbcc_print(ctx, "loada %s; ", s0);
				return;
			}

			if(BCCX_TagIsP(t, "objref"))
			{
				BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(t, "value"));
				bgbcc_print(ctx, "loadslota %s; ", BCCX_Get(t, "name"));
				return;
			}

			if(BCCX_TagIsP(t, "getindex") ||
				BCCX_TagIsP(t, "vector-ref"))
			{
				BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(t, "array"));
				BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(t, "index"));
				bgbcc_print(ctx, "loadindexa; ");
				return;
			}

			BGBCC_Error("Bad expression type for '&'\n");
			return;
		}

		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));

		s0=NULL;
		if(BCCX_AttrIsP(l, "op", "+"))s0="nop";
		if(BCCX_AttrIsP(l, "op", "-"))s0="neg";
		if(BCCX_AttrIsP(l, "op", "~"))s0="not";
		if(BCCX_AttrIsP(l, "op", "!"))s0="lnot";
		if(BCCX_AttrIsP(l, "op", "*"))s0="defer";
		if(BCCX_AttrIsP(l, "op", "&"))s0="loada";

		if(s0)
		{
			bgbcc_print(ctx, "%s; ", s0);
		}else
		{
			s0=BCCX_Get(l, "op");
			bgbcc_print(ctx, "unaryop \"%s\"; ", s0);
		}
		return;
	}

	if(BCCX_TagIsP(l, "binary"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "left"));
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "right"));

		s0=NULL;

		if(BCCX_AttrIsP(l, "op", "+"))s0="add";
		if(BCCX_AttrIsP(l, "op", "-"))s0="sub";
		if(BCCX_AttrIsP(l, "op", "*"))s0="mul";
		if(BCCX_AttrIsP(l, "op", "/"))s0="div";
		if(BCCX_AttrIsP(l, "op", "%"))s0="mod";
		if(BCCX_AttrIsP(l, "op", "&"))s0="and";
		if(BCCX_AttrIsP(l, "op", "|"))s0="or";
		if(BCCX_AttrIsP(l, "op", "^"))s0="xor";

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

		if(BCCX_AttrIsP(l, "op", "+."))s0="add1";
		if(BCCX_AttrIsP(l, "op", "-."))s0="sub1";
		if(BCCX_AttrIsP(l, "op", "*."))s0="mul1";
		if(BCCX_AttrIsP(l, "op", "/."))s0="div1";
		if(BCCX_AttrIsP(l, "op", "%."))s0="mod1";
		if(BCCX_AttrIsP(l, "op", "&."))s0="and1";
		if(BCCX_AttrIsP(l, "op", "|."))s0="or1";
		if(BCCX_AttrIsP(l, "op", "^."))s0="xor1";

		if(BCCX_AttrIsP(l, "op", ".+"))s0="add2";
		if(BCCX_AttrIsP(l, "op", ".-"))s0="sub2";
		if(BCCX_AttrIsP(l, "op", ".*"))s0="mul2";
		if(BCCX_AttrIsP(l, "op", "./"))s0="div2";
		if(BCCX_AttrIsP(l, "op", ".%"))s0="mod2";
		if(BCCX_AttrIsP(l, "op", ".&"))s0="and2";
		if(BCCX_AttrIsP(l, "op", ".|"))s0="or2";
		if(BCCX_AttrIsP(l, "op", ".^"))s0="xor2";

		if(BCCX_AttrIsP(l, "op", "<<"))s0="shl";
		if(BCCX_AttrIsP(l, "op", ">>"))s0="shr";
		if(BCCX_AttrIsP(l, "op", ">>>"))s0="shrr";

		if(BCCX_AttrIsP(l, "op", "=="))s0="cmp_eq";
		if(BCCX_AttrIsP(l, "op", "!="))s0="cmp_ne";
		if(BCCX_AttrIsP(l, "op", "==="))s0="cmp_eq";
		if(BCCX_AttrIsP(l, "op", "!=="))s0="cmp_ne";
		if(BCCX_AttrIsP(l, "op", "<"))s0="cmp_l";
		if(BCCX_AttrIsP(l, "op", ">"))s0="gmp_g";
		if(BCCX_AttrIsP(l, "op", "<="))s0="cmp_le";
		if(BCCX_AttrIsP(l, "op", ">="))s0="cmp_ge";

		if(s0)
		{
			bgbcc_print(ctx, "%s ", s0);
		}else
		{
			s0=BCCX_Get(l, "op");
			bgbcc_print(ctx, "binaryop \"%s\"; ", s0);
		}
		return;
	}

	if(BCCX_TagIsP(l, "funcall"))
	{
		BGBCC_BS2IL_CompileFuncall(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "methodcall"))
	{
		bgbcc_print(ctx, "mark; ");
		c=BCCX_Fetch(l, "args");
//		BGBCC_BS2IL_CompileExprListReverse(ctx, c);
		BGBCC_BS2IL_CompileExprList(ctx, c);
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "methodcall_s %s; ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "new"))
	{
		bgbcc_print(ctx, "mark; ");
		c=BCCX_Fetch(l, "args");
//		BGBCC_BS2IL_CompileExprListReverse(ctx, c);
		BGBCC_BS2IL_CompileExprList(ctx, c);
		
		s0=BGBCC_VarTypeString(ctx, BCCX_FindTag(l, "type"));
		bgbcc_print(ctx, "new_s \"%s\"; ", s0);
		
//		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
//		bgbcc_print(ctx, "$%s methodcall ", BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "throw"))
	{
		BGBCC_BS2IL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		bgbcc_print(ctx, "throw; ");
		return;
	}

	BGBCC_Error("unhandled expr %s\n", BCCX_Tag(l));
	bgbcc_print(ctx, "push_null; ");
}

void BGBCC_BS2IL_CompileExprT(BGBCC_State *ctx, BCCX_Node *l)
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
		
		if((ctx->lang==BGBCC_LANG_JAVA) ||
			(ctx->lang==BGBCC_LANG_CS) ||
			(ctx->lang==BGBCC_LANG_BS2))
		{
			if(!strcmp(s0, "true"))
				{ bgbcc_print(ctx, "push_true; "); return; }
			if(!strcmp(s0, "false"))
				{ bgbcc_print(ctx, "push_false; "); return; }
			if(!strcmp(s0, "null"))
				{ bgbcc_print(ctx, "push_null; "); return; }
//			if(!strcmp(s0, "undefined"))
//				{ bgbcc_print(ctx, "push_undef; "); return; }
		}
		
		bgbcc_print(ctx, "load %s; ", s0);
		return;
	}

	if(BCCX_TagIsP(l, "int"))
	{
		bgbcc_print(ctx, "push_i %s; ", BCCX_Get(l, "value"));
		return;
	}
	if(BCCX_TagIsP(l, "real"))
	{
		bgbcc_print(ctx, "push_f %s; ", BCCX_Get(l, "value"));
		return;
	}

	if(BCCX_TagIsP(l, "string"))
	{
		bgbcc_print(ctx, "push_rt %s; ", BGBCC_CIfy(BCCX_Get(l, "value")));
		return;
	}

	if(BCCX_TagIsP(l, "complex"))
	{
		s0=BCCX_Get(l, "real"); s1=BCCX_Get(l, "imag");
		if(!s0)s0="0"; if(!s1)s1="0";
		bgbcc_print(ctx, "%s %s complex; ", s0, s1);
		return;
	}

	BGBCC_BS2IL_CompileForm(ctx, l);
	return;
}

void BGBCC_BS2IL_CompileExpr(BGBCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=0;
	BGBCC_BS2IL_CompileExprT(ctx, l);
	ctx->tail=i;
}

void BGBCC_BS2IL_CompileExprTail(BGBCC_State *ctx, BCCX_Node *l)
{
	int i;

	i=ctx->tail;
	ctx->tail=1;
	BGBCC_BS2IL_CompileExprT(ctx, l);
	ctx->tail=i;
}
