#include <bgbccc.h>


// extern BCCX_Node *bgbcp_structs;
// extern BCCX_Node *bgbcp_types;

char *bgbcc_lfn;
int bgbcc_lln;
int bgbcc_reqlfn;
// static int bgbcc_seq=1;

int bgbcc_warn;
int bgbcc_err;

int BGBCC_BSVM_CompileSwitch(BGBCC_State *ctx, BCCX_Node *l)
{
	char **cl, *s;
	int i, j, ncl;
	BCCX_Node *c, *t, *u, *v;

	cl=bgbcc_malloc(4096*sizeof(char *));

	ncl=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case") ||
			BCCX_TagIsP(c, "case_default"))
		{
			cl[ncl++]=BGBCC_GenSym();
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}

	BGBCC_BSVM_CompileExpr(ctx, BCCX_Fetch(l, "cond"));

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case"))
		{
			bgbcc_print(ctx, "dup ");
			BGBCC_BSVM_CompileExpr(ctx, BCCX_Fetch(c, "value"));
			BGBCC_BSVM_CompileJmpCond(ctx, "==", cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_BSVM_CompileJmp(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}


	s=BGBCC_GenSym();
	ctx->breakstack[ctx->breakstackpos++]=s;
	BGBCC_BSVM_CompileBreak(ctx);

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case") ||
			BCCX_TagIsP(c, "case_default"))
		{
			BGBCC_BSVM_EmitLabel(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		BGBCC_BSVM_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_BSVM_EmitLabel(ctx, s);
	bgbcc_print(ctx, "pop ");

	bgbcc_free(cl);
	ctx->breakstackpos--;
	return(0);
}

void BGBCC_BSVM_CompileStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	char *s0, *s1, *s2;
	int i, j, k;

//	bgbcc_print(ctx, "\n");

	s0=BCCX_Get(l, "fn");
	if(s0 && (s0!=bgbcc_lfn))
	{
		if(!bgbcc_reqlfn)
			bgbcc_print(ctx, "fn %s; ", BGBCC_CIfy(s0));
		bgbcc_lfn=s0;
	}

	i=BCCX_GetInt(l, "ln");
	if((i>0) && (i!=bgbcc_lln))
	{
		if(!bgbcc_reqlfn)bgbcc_print(ctx, "ln %d; ", i);
		bgbcc_lln=i;
	}

	if(bgbcc_reqlfn)
	{
		bgbcc_print(ctx, "fn %s; ", BGBCC_CIfy(bgbcc_lfn));
		bgbcc_print(ctx, "ln %d; ", bgbcc_lln);
		bgbcc_reqlfn=0;
	}

	l=BGBCC_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "switch"))
	{
		BGBCC_BSVM_CompileSwitch(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "goto"))
	{
		BGBCC_BSVM_CompileJmp(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "label"))
	{
		BGBCC_BSVM_EmitLabel(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "continue"))
	{
		s0=ctx->contstack[ctx->contstackpos-1];
		BGBCC_BSVM_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "break"))
	{
		s0=ctx->breakstack[ctx->breakstackpos-1];
		BGBCC_BSVM_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "return"))
	{
		t=BCCX_Fetch(l, "value");
		if(!t)
		{
			bgbcc_print(ctx, "ret_v; ");
			return;
		}

		BGBCC_BSVM_CompileExpr(ctx, t);
		bgbcc_print(ctx, "ret; ");
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		s0=BCCX_Get(l, "op");

		if(s0)
		{
			BGBCC_BSVM_CompileExpr(ctx, BCCX_Fetch(l, "left"));
			BGBCC_BSVM_CompileExpr(ctx, BCCX_Fetch(l, "right"));
			bgbcc_print(ctx, "%s ", s0);
			BGBCC_BSVM_CompileAssign(ctx, BCCX_Fetch(l, "left"));
			return;
		}

		BGBCC_BSVM_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		BGBCC_BSVM_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		return;
	}

	if(BCCX_TagIsP(l, "preinc") || BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			bgbcc_print(ctx, "inc_s %s; ", s0);
			return;
		}

		BGBCC_BSVM_CompileExpr(ctx, t);
		bgbcc_print(ctx, "inc ");
		BGBCC_BSVM_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "predec") || BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsP(t, "ref"))
		{
			s0=BCCX_Get(t, "name");
			bgbcc_print(ctx, "dec_s %s; ", s0);
			return;
		}

		BGBCC_BSVM_CompileExpr(ctx, t);
		bgbcc_print(ctx, "dec; ");
		BGBCC_BSVM_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsP(l, "defun"))	{ return; }
	if(BCCX_TagIsP(l, "vars"))	{ return; }

	if(BCCX_TagIsP(l, "if"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_BSVM_CompileStatement(ctx, t);
			return;
		}

		s0=BGBCC_GenSym();
		BGBCC_BSVM_CompileJCF(ctx, t, s0);

		t=BCCX_Fetch(l, "else");
		BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "then"));
		if(t)
		{
			s1=BGBCC_GenSym();
			BGBCC_BSVM_CompileJmp(ctx, s1);
			BGBCC_BSVM_EmitLabel(ctx, s0);
			BGBCC_BSVM_CompileStatement(ctx, t);
			s0=s1;
		}
		BGBCC_BSVM_EmitLabel(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_BSVM_CompileStatement(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "for"))
	{
		s0=BGBCC_GenSym();
		s1=BGBCC_GenSym();
		s2=BGBCC_GenSym();

		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "init"));

//		BGBCC_BSVM_EmitLabel(ctx, s1);
		BGBCC_BSVM_EmitLabel(ctx, s0);
		BGBCC_BSVM_CompileJCF(ctx, BCCX_Fetch(l, "cond"), s2);

		BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		BGBCC_BSVM_EmitLabel(ctx, s1);
		BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "step"));
//		BGBCC_BSVM_CompileContinue(ctx);
		BGBCC_BSVM_CompileJmp(ctx, s0);

		BGBCC_BSVM_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "while"))
	{
		s1=BGBCC_GenSym();
		s2=BGBCC_GenSym();

		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_BSVM_EmitLabel(ctx, s1);

		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);

		if(i==0)return;

		if(i==1)
		{
			BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "body"));
			BGBCC_BSVM_CompileContinue(ctx);
			BGBCC_BSVM_EmitLabel(ctx, s2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_BSVM_CompileJCF(ctx, t, s2);
		BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "body"));
		BGBCC_BSVM_CompileContinue(ctx);
		BGBCC_BSVM_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}


	if(BCCX_TagIsP(l, "do_while"))
	{
		s1=BGBCC_GenSym();
		s2=BGBCC_GenSym();
		ctx->contstack[ctx->contstackpos++]=s1;
		ctx->breakstack[ctx->breakstackpos++]=s2;

		BGBCC_BSVM_EmitLabel(ctx, s1);
		BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==0)return;

		if(i==1)
		{
			BGBCC_BSVM_CompileContinue(ctx);
			BGBCC_BSVM_EmitLabel(ctx, s2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_BSVM_CompileJCT(ctx, t, s1);
		BGBCC_BSVM_EmitLabel(ctx, s2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsP(l, "try_catch"))
	{
//		s1=BGBCC_GenSym();
//		s2=BGBCC_GenSym();
//		ctx->contstack[ctx->contstackpos++]=s1;
//		ctx->breakstack[ctx->breakstackpos++]=s2;

		//lame: for now just spit out try block...
		BGBCC_BSVM_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		return;
	}

	if(BCCX_TagIsP(l, "msvc_asm"))
	{
		BGBCC_Warn("inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "gcc_asm"))
	{
		BGBCC_Warn("inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsP(l, "linecomment")) { return; }
	if(BCCX_TagIsP(l, "blockcomment")) { return; }

	BGBCC_BSVM_CompileExpr(ctx, l);
	bgbcc_print(ctx, "pop; ");
}

void BGBCC_BSVM_EmitVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;
	s2=BGBCC_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";
	if(!s1)s1="_";

	bgbcc_print(ctx, "var { name='%s' type='%s' ", s1, s);

	if(s2 && *s2)
		bgbcc_print(ctx, "flags='%s' ", s2);

	if(v)
	{
		bgbcc_print(ctx, "value= ");
		BGBCC_BSVM_EmitVarValueR(ctx, v);
	}

	bgbcc_print(ctx, "} ");

}

void BGBCC_BSVM_EmitVar2(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	BGBCC_BSVM_EmitVar(ctx, name, ty, v);

#if 0
	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;

	if(!s)s="v";

	i=BCCX_GetInt(ty, "flags");
	s2=((i&BGBCC_TYFL_STATIC) && !ctx->cur_struct)?"~":"";

	if(!s1)s1="_";
	if(*s)bgbcc_print(ctx, "%s%s:%s ", s1, s2, s);
		else bgbcc_print(ctx, "%s%s ", s1, s2);

	if(v)
	{
		bgbcc_print(ctx, "= ");
		BGBCC_BSVM_EmitVarValueR(ctx, v);
	}

	if(ctx->cur_struct)
	{
		s2=BGBCC_VarTypeFlagsString(ctx, ty);

		//BGBCC_BindStructInfoIdx(ctx, ctx->cur_struct,
		//	"field", ctx->cur_idx++, s1);

		//BGBCC_BindFieldSig(ctx, ctx->cur_struct, s1, s);
		//BGBCC_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
		//if(s2 && (*s2))
			//BGBCC_BindFieldInfo(ctx, ctx->cur_struct, s1, "flagstr", s2);
	}
#endif
}

void BGBCC_BSVM_EmitVar3(BGBCC_State *ctx, char *name, char *ty)
{
	char *s, *s1;
	int i;

	s=ty;
	s1=name;

	if(!s)s="v";
	if(!s1)s1="_";
	
	bgbcc_print(ctx, "name='%s' type='%s' ", s1, s);
}

void BGBCC_BSVM_EmitVar4(BGBCC_State *ctx, char *name, BCCX_Node *ty)
{
	char *s, *s1, *s2;
	int i;

	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;
	s2=BGBCC_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";
	if(!s1)s1="_";

	bgbcc_print(ctx, "name='%s' type='%s' ", s1, s);

	if(s2 && *s2)
		bgbcc_print(ctx, "flags='%s' ", s2);
}

void BGBCC_BSVM_EmitVarProperty(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");

	n=BCCX_New1("var", t);
	BCCX_Set(n, "name", "value");

	v=BCCX_FindTag(l, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_GenProtoSig(ctx, t, NULL);
		BGBCC_BSVM_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_GenProtoSig(ctx, NULL, n);
		BGBCC_BSVM_EmitVar3(ctx, s2, s1);
	}

//	BGBCC_BSVM_EmitVar3(ctx, "new", s1);
//	BGBCC_BSVM_CompileBlock(ctx, t, s, n, v);

	if(ctx->cur_struct)
	{
		s1=BGBCC_VarTypeString(ctx, t);
		s2=BGBCC_VarTypeFlagsString(ctx, t);
		i=BCCX_GetInt(t, "flags");

		//BGBCC_BindStructInfoIdx(ctx, ctx->cur_struct,
		//	"property", ctx->cur_idx3++, s);

		//BGBCC_BindFieldSig(ctx, ctx->cur_struct, s, s1);
		//BGBCC_BindFieldInfoI(ctx, ctx->cur_struct, s, "flags", i);
		//if(s2 && (*s2))
			//BGBCC_BindFieldInfo(ctx, ctx->cur_struct, s, "flagstr", s2);
	}
}

void BGBCC_BSVM_CompileVarProperty(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

#if 1
	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");

	n=BCCX_New1("var", t);
	BCCX_Set(n, "name", "value");

	v=BCCX_FindTag(l, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_BSVM_CompileBlock(ctx, t, s2, NULL, v);

//		s1=BGBCC_GenProtoSig(ctx, t, NULL);
//		BGBCC_BSVM_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_BSVM_CompileBlock(ctx, NULL, s2, n, v);

//		s1=BGBCC_GenProtoSig(ctx, NULL, n);
//		BGBCC_BSVM_EmitVar3(ctx, s2, s1);
	}

//	BGBCC_BSVM_EmitVar3(ctx, "new", s1);
//	BGBCC_BSVM_CompileBlock(ctx, t, s, n, v);
#endif

}

int BGBCC_BSVM_CompileVarDummyP(BGBCC_State *ctx, BCCX_Node *l)
{
	if(BCCX_TagIsP(l, "assign") ||
		BCCX_TagIsP(l, "if") ||
		BCCX_TagIsP(l, "do_while") ||
		BCCX_TagIsP(l, "for") ||
		BCCX_TagIsP(l, "methodcall") ||
		BCCX_TagIsP(l, "while") ||
		BCCX_TagIsP(l, "return") ||
		BCCX_TagIsP(l, "switch") ||
		BCCX_TagIsP(l, "funcall"))
		{ return(1); }

	if(BCCX_TagIsP(l, "linecomment")) { return(1); }
	if(BCCX_TagIsP(l, "blockcomment")) { return(1); }

	return(0);
}

void BGBCC_BSVM_CompileVarStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1;
	int i;

	s=BCCX_Get(l, "fn");
	if(s)bgbcc_lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsP(c, "var"))
				{ c=BCCX_Next(c); continue; }

			s=BCCX_Get(c, "name");
			t=BCCX_FindTag(c, "type");
			//v=BCCX_Fetch(c, "value");
			v=NULL;

			BGBCC_BSVM_EmitVar2(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_GenProtoSig(ctx, t, n);
		BGBCC_BSVM_EmitVar3(ctx, s, s1);
//		BGBCC_BSVM_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_GenProtoSig(ctx, NULL, n);
		BGBCC_BSVM_EmitVar3(ctx, "new", s1);
//		BGBCC_BSVM_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_BSVM_EmitVarProperty(ctx, l);
		return;
	}

	if(BGBCC_BSVM_CompileVarDummyP(ctx, l))
		return;

	BGBCC_Warn("unhandled var statement %s\n", BCCX_Tag(l));
}

void BGBCC_BSVM_CompileVarStatement2(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s;
	int i;

	s=BCCX_Get(l, "fn");
	if(s)bgbcc_lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsP(c, "var"))
				{ c=BCCX_Next(c); continue; }

			s=BCCX_Get(c, "name");
			t=BCCX_FindTag(c, "type");
			v=BCCX_Fetch(c, "value");

//			BGBCC_BSVM_EmitVar2(ctx, s, t, v);
			BGBCC_BSVM_EmitTopVar(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_BSVM_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "constructor"))
	{
		s="new";
		t=NULL;
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_BSVM_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "property"))
	{
		BGBCC_BSVM_CompileVarProperty(ctx, l);
		return;
	}

	if(BGBCC_BSVM_CompileVarDummyP(ctx, l))
		return;

	BGBCC_Warn("unhandled var statement %s\n", BCCX_Tag(l));
}

BCCX_Node *BGBCC_BSVM_CompileBlock(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	char *cname[16], *bname[16];

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

//	name=BGBCC_QualifyNameNS(ctx, name);

//	BGBCC_BSVM_EmitSigProto(ctx, type, name, args);

	ctx->contstack=cname;
	ctx->breakstack=bname;

	ctx->cf_n=name;
	ctx->cf_ty=type;

	i=0;

	//hack: disable extern inline bastards...
	if(type)i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_EXTERN)
		return(NULL);

	if(BCCX_TagIsP(body, "begin"))
		body=BCCX_Child(body);

	if(ctx->cur_struct)
		bgbcc_print(ctx, "method { ");
	else bgbcc_print(ctx, "function { ");

	BGBCC_BSVM_EmitVar4(ctx, ctx->cf_n, ctx->cf_ty);

	bgbcc_print(ctx, "\n");

	bgbcc_print(ctx, "\targs {\n");

	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			bgbcc_print(ctx, "\trest{}\n");
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		bgbcc_print(ctx, "\t");
		t=BCCX_FindTag(c, "type");
		BGBCC_BSVM_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
		bgbcc_print(ctx, "\n");
	}

	bgbcc_print(ctx, "\t}\n");

	bgbcc_print(ctx, "\tvars {\n");
	c=body;
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_BSVM_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
		bgbcc_print(ctx, "\n");
	}

	bgbcc_print(ctx, "\t}\n");


//	bgbcc_print(ctx, "BODY ");
//	bgbcc_print(ctx, "\n");
	bgbcc_print(ctx, "\tbody {\n");

	bgbcc_reqlfn=1;

	c=body;
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_BSVM_CompileStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}

	bgbcc_print(ctx, "ret_v ");
//	bgbcc_print(ctx, "END\n\n");

	bgbcc_print(ctx, "\t}\n");
	bgbcc_print(ctx, "}\n");

	return(NULL);
}

void BGBCC_BSVM_EmitSigProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
		{ BGBCC_EmitSigProto(ctx, type, name, args); }

BCCX_Node *BGBCC_BSVM_CompileProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

//	name=BGBCC_QualifyNameNS(ctx, name);
//	BGBCC_BSVM_EmitSigProto(ctx, type, name, args);

	ctx->cf_n=name;
	ctx->cf_ty=type;

	bgbcc_print(ctx, "proto { ");
	BGBCC_BSVM_EmitVar4(ctx, ctx->cf_n, ctx->cf_ty);
	bgbcc_print(ctx, "\n");

	bgbcc_print(ctx, "\targs {\n");
	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			bgbcc_print(ctx, "\trest{}\n");
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		bgbcc_print(ctx, "\t");

		t=BCCX_FindTag(c, "type");
		BGBCC_BSVM_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
		bgbcc_print(ctx, "\n");
	}

	bgbcc_print(ctx, "\t}\n");

#if 0
	i=BCCX_GetInt(type, "flags");
	if(ctx->arch==BGBCC_ARCH_X86)
	{
		if(i&BGBCC_TYFL_STDCALL)bgbcc_print(ctx, "$stdcall ");
		if(i&BGBCC_TYFL_CDECL)bgbcc_print(ctx, "$cdecl ");
	}
	if(ctx->arch==BGBCC_ARCH_X64)
	{	
		if(i&BGBCC_TYFL_WIN64)bgbcc_print(ctx, "$win64 ");
		if(i&BGBCC_TYFL_AMD64)bgbcc_print(ctx, "$amd64 ");
	}
	if(i&BGBCC_TYFL_XCALL)bgbcc_print(ctx, "$xcall ");

	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");
	if(i&BGBCC_TYFL_PROXY)bgbcc_print(ctx, "$proxy ");
#endif

//	bgbcc_print(ctx, "END\n");
	bgbcc_print(ctx, "}\n ");

	return(NULL);
}

void BGBCC_BSVM_CompileStruct(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *t, *n;
	char *s, *os0, *s1;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
	//BGBCC_BindStructSig(ctx, s, "struct");
	//BGBCC_BindStructInfoI(ctx, s, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

//	bgbcc_print(ctx, "STRUCT %s ", BCCX_Get(l, "name"));
//	bgbcc_print(ctx, "\n");

	bgbcc_print(ctx, "struct { ");
	bgbcc_print(ctx, "name='%s'\n", BCCX_Get(l, "name"));

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_BSVM_CompileVarStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
//	bgbcc_print(ctx, "\tEND\n");
	bgbcc_print(ctx, "}\n");

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_BSVM_CompileUnion(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	s=BGBCC_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
	//BGBCC_BindStructSig(ctx, s, "union");
	//BGBCC_BindStructInfoI(ctx, s, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

//	bgbcc_print(ctx, "UNION %s ", BCCX_Get(l, "name"));
//	bgbcc_print(ctx, "\n");

	bgbcc_print(ctx, "union { ");
	bgbcc_print(ctx, "name='%s'\n", BCCX_Get(l, "name"));

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		bgbcc_print(ctx, "\t");
		BGBCC_BSVM_CompileVarStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}

//	bgbcc_print(ctx, "\tEND\n");
	bgbcc_print(ctx, "}\n");

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_BSVM_CompileClass(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *t, *n;
	char *s, *os0, *qn;
	int i, j, j1, j2;

	s=BCCX_Get(l, "name");
	qn=BGBCC_QualifyNameNS(ctx, s);

	i=BCCX_GetInt(l, "flags");
	//BGBCC_BindStructSig(ctx, qn, "class");
	//BGBCC_BindStructInfoI(ctx, qn, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=qn;
	j=ctx->cur_idx;
	j1=ctx->cur_idx2;
	j2=ctx->cur_idx3;

	ctx->cur_idx=0;
	ctx->cur_idx2=0;
	ctx->cur_idx3=0;

//	bgbcc_print(ctx, "CLASS %s ", qn);
	bgbcc_print(ctx, "class {\n");
	bgbcc_print(ctx, "name='%s' ", qn);

	c=BCCX_Fetch(l, "super");
	if(c)
	{
		s=BGBCC_VarTypeString(ctx, c);
		if(s)bgbcc_print(ctx, "extends='%s' ", s);

#if 0
		bgbcc_print(ctx, "EXTENDS ");
		while(c)
		{
			s=BGBCC_VarTypeString(ctx, c);
			if(s)bgbcc_print(ctx, "%s", s);
			c=BCCX_Next(c);
		}
		bgbcc_print(ctx, " ");
#endif
	}

	c=BCCX_Fetch(l, "impl");
	if(c)
	{
		bgbcc_print(ctx, "IMPLEMENTS ");
		while(c)
		{
			s=BGBCC_VarTypeString(ctx, c);
			if(s)bgbcc_print(ctx, "%s", s);
			c=BCCX_Next(c);
		}
		bgbcc_print(ctx, " ");
	}

	bgbcc_print(ctx, "\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		bgbcc_print(ctx, "\t");
//		BGBCC_BSVM_CompileVarStatement(ctx, c);
		BGBCC_BSVM_CompileStatement(ctx, c);
		bgbcc_print(ctx, "\n");
		c=BCCX_Next(c);
	}
	bgbcc_print(ctx, "}\n");

#if 0
	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_BSVM_CompileVarStatement2(ctx, c);
		c=BCCX_Next(c);
	}
#endif

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
	ctx->cur_idx2=j1;
	ctx->cur_idx3=j2;
}

BCCX_Node *BGBCC_BSVM_CompileSProto(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u;
	char *s, *s1;

	bgbcc_print(ctx, "s_proto { ");

	t=BCCX_FindTag(l, "type");
	BGBCC_BSVM_EmitVar4(ctx, BCCX_Get(l, "name"), t);
	bgbcc_print(ctx, "\n");

	bgbcc_print(ctx, "\targs {\n");
	c=BCCX_Fetch(l, "args");
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
			{ bgbcc_print(ctx, "\trest{}\n"); c=BCCX_Next(c); continue; }
		if(!BCCX_TagIsP(c, "var"))
			{ c=BCCX_Next(c); continue; }

		bgbcc_print(ctx, "\t");
		t=BCCX_FindTag(c, "type");
		BGBCC_BSVM_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
		bgbcc_print(ctx, "\n");
	}

	bgbcc_print(ctx, "\t}\n");
	bgbcc_print(ctx, "}\n");


//	s=BCCX_Get(l, "name");
//	t=BCCX_FindTag(l, "type");
//	u=BCCX_Fetch(l, "args");

//	s1=BGBCC_GenProtoSig(ctx, t, u);
	//BGBCC_BindStructSig(ctx, s, "func");
	//BGBCC_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

void BGBCC_BSVM_CompileTypedef(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

//	s=BCCX_Get(l, "fn");
//	if(s)bgbcc_lfn=s;
//	i=BCCX_GetInt(l, "ln");
//	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "var"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
//		v=BCCX_Fetch(l, "value");

		s1=BGBCC_VarTypeString(ctx, t);
		s2=BGBCC_VarTypeFlagsString(ctx, t);
		//BGBCC_BindTypeSig(ctx, s, s1);
		//if(s2 && (*s2))
			//BGBCC_BindTypeInfo(ctx, s, "flagstr", s2);

		bgbcc_print(ctx, "s_var { name='%s' type='%s' ",
			s, s1);
		if(s2 && *s2)
			bgbcc_print(ctx, "flags='%s' ", s2);
		bgbcc_print(ctx, "}\n");

//		s2=BGBCC_GetNodeAttributeString(ctx, t, "dytname");
//		if(!s2)s2=BGBCC_GetNodeAttributeString(ctx, t, "dytname");
		if(s2)
		{
			//BGBCC_BindDyTypeSig(ctx, s2, s1);
		}

//		c=BCCX_Next(c);
	}

#if 0
	if(BCCX_TagIsP(l, "func"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		v=BCCX_FindTag(l, "args");

		s1=BGBCC_GenProtoSig(ctx, t, v);
		//BGBCC_BindTypeSig(ctx, s, s1);
	}
#endif

	return;
}

void BGBCC_BSVM_CompileStructs(BGBCC_State *ctx)
{
	BCCX_Node *c, *t, *n, *l;

	c=ctx->types;
	while(c)
	{
		BGBCC_BSVM_CompileTypedef(ctx, c);
		c=BCCX_Next(c);
	}

	c=ctx->structs;
	while(c)
	{
		l=c;
		if(BCCX_TagIsP(c, "struct"))BGBCC_BSVM_CompileStruct(ctx, c);
		if(BCCX_TagIsP(c, "union"))BGBCC_BSVM_CompileUnion(ctx, c);
//		if(BCCX_TagIsP(c, "enum"))BGBCC_BSVM_CompileEnum(ctx, c);
		if(BCCX_TagIsP(c, "s_proto"))BGBCC_BSVM_CompileSProto(ctx, c);
		if(BCCX_TagIsP(c, "func"))BGBCC_BSVM_CompileSProto(ctx, c);

		c=BCCX_Next(c);
	}
}

void BGBCC_BSVM_EmitVarValue(BGBCC_State *ctx, BCCX_Node *v)
{
	if(BCCX_TagIsP(v, "int"))
	{
		bgbcc_print(ctx, "int{value=%s} ", BCCX_Get(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "real"))
	{
		bgbcc_print(ctx, "real{value=%s}", BCCX_Get(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "string"))
	{
		bgbcc_print(ctx, "string{value=%s}",
			BGBCC_CIfy(BCCX_Get(v, "value")));
		return;
	}

	BGBCC_Error("BGBCC_BSVM_EmitVarValue: Bad Value %s\n", v->tag);
}

void BGBCC_BSVM_EmitVarValueR(BGBCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_BSVM_EmitVarValueR(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}

		return;
	}

	BGBCC_BSVM_EmitVarValue(ctx, v);
}

void BGBCC_BSVM_EmitVarValueR2(BGBCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		bgbcc_print(ctx, "list { ");

		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_BSVM_EmitVarValueR2(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}

		bgbcc_print(ctx, "} ");

		return;
	}

	BGBCC_BSVM_EmitVarValue(ctx, v);
}

void BGBCC_BSVM_EmitTopArray(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int ord, cnt;
	int i, j, k;

	s=BGBCC_VarTypeString(ctx, ty);
	//s1=name;
	s1=BGBCC_QualifyNameNS(ctx, name);

	i=BCCX_GetInt(ty, "flags");
	//if(!(i&BGBCC_TYFL_STATIC))//BGBCC_BindSig(ctx, s1, s);

	if(!s1)return;
	if(!s1)s1="_";

	if(v && BCCX_TagIsP(v, "list"))
	{
		bgbcc_print(ctx, "var { ");
//		i=BCCX_GetInt(ty, "flags");
//		if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
//		if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

		bgbcc_print(ctx, "name='%s' type='%s' ", s1, s);

		bgbcc_print(ctx, "values { ");
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_BSVM_EmitVarValueR2(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}
		bgbcc_print(ctx, "}\n");

		bgbcc_print(ctx, "}\n");

		return;
	}

//	bgbcc_print(ctx, "VAR ");
//	i=BCCX_GetInt(ty, "flags");
//	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
//	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

	bgbcc_print(ctx, "var { name='%s' type='%s' }\n", s1, s);
}

void BGBCC_BSVM_EmitTopVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int i, j;

#if 0
	i=BCCX_GetInt(ty, "flags");
	if(!ctx->cur_struct && (i&BGBCC_TYFL_EXTERN))
	{
		s=BGBCC_VarTypeString(ctx, ty);
//		s1=name;
		s1=BGBCC_QualifyNameNS(ctx, name);
		s2=BGBCC_VarTypeFlagsString(ctx, ty);

		//BGBCC_BindVarSig(ctx, s1, s);
		//if(s2 && (*s2))
			//BGBCC_BindVarInfo(ctx, s1, "flagstr", s2);

		if(!s1)return;
		if(!s1)s1="_";

		bgbcc_print(ctx, "EXTERN ");

		if(*s)bgbcc_print(ctx, "%s:%s", s1, s);
			else bgbcc_print(ctx, "%s", s1);

		bgbcc_print(ctx, "\n");
		return;
	}
#endif

	if(!ctx->cur_struct)
	{
		c=BCCX_Fetch(ty, "size"); i=0;
		while(c) { i++; c=BCCX_Next(c); }

		if(i)
		{
			BGBCC_BSVM_EmitTopArray(ctx, name, ty, v);
			return;
		}
	}

	s=BGBCC_VarTypeString(ctx, ty);
	s2=BGBCC_VarTypeFlagsString(ctx, ty);
//	s1=name;
	s1=BGBCC_QualifyNameNS(ctx, name);

	i=BCCX_GetInt(ty, "flags");
//	if(!(i&BGBCC_TYFL_STATIC))
	//BGBCC_BindVarSig(ctx, s1, s);
//	if(s2 && (*s2))
			//BGBCC_BindVarInfo(ctx, s1, "flagstr", s2);

	if(!s1)return;
	if(!s1)s1="_";

#if 0
	if(!ctx->cur_struct)
	{
		bgbcc_print(ctx, "VAR ");

		i=BCCX_GetInt(ty, "flags");
		if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
		if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

		if(i&BGBCC_TYFL_CONST)bgbcc_print(ctx, "$const ");
		if(i&BGBCC_TYFL_FINAL)bgbcc_print(ctx, "$final ");
		if(i&BGBCC_TYFL_VOLATILE)bgbcc_print(ctx, "$volatile ");
		if(i&BGBCC_TYFL_NATIVE)bgbcc_print(ctx, "$native ");
	}else
	{
		bgbcc_print(ctx, "FIELD ");
	}
#endif

	bgbcc_print(ctx, "var { name='%s' type='%s' ", s1, s);

	if(v)
	{
		bgbcc_print(ctx, "values { ");
		BGBCC_BSVM_EmitVarValueR2(ctx, v);
		bgbcc_print(ctx, "} ");
	}

	bgbcc_print(ctx, "}\n");
}

void BGBCC_BSVM_CompileTopStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1;
	int i;

	s=BCCX_Get(l, "fn");
	if(s)bgbcc_lfn=s;
	i=BCCX_GetInt(l, "ln");
	if(i)bgbcc_lln=i;

	if(BCCX_TagIsP(l, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(BCCX_TagIsP(c, "proto"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				n=BCCX_Fetch(c, "args");

				BGBCC_BSVM_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsP(c, "var"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				v=BCCX_Fetch(c, "value");

				BGBCC_BSVM_EmitTopVar(ctx, s, t, v);
			}

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "defun"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_BSVM_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "proto"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		BGBCC_BSVM_CompileProto(ctx, t, s, n);
		return;
	}

	if(BCCX_TagIsP(l, "struct"))
	{
		BGBCC_BSVM_CompileStruct(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "union"))
	{
		BGBCC_BSVM_CompileUnion(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "class"))
	{
		BGBCC_BSVM_CompileClass(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "import"))
	{
		s=BCCX_Get(l, "name");
		bgbcc_print(ctx, "import {name='%s'}\n", s);
		return;
	}

	if(BCCX_TagIsP(l, "using"))
	{
		s=BCCX_Get(l, "name");
		bgbcc_print(ctx, "import{name='%s'}\n", s);
		return;
	}

	if(BCCX_TagIsP(l, "package"))
	{
		s=BCCX_Get(l, "name");
		n=BCCX_FindTag(l, "body");
//		c=BCCX_Fetch(l, "body");

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

		//BGBCC_BindStructSig(ctx, s, "namespace");
		bgbcc_print(ctx, "package { name='%s'\n", s);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;

#if 1
		if(n)
		{
			c=BCCX_Child(n);
			while(c)
			{
				BGBCC_BSVM_CompileTopStatement(ctx, c);
				c=BCCX_Next(c);
			}
			ctx->cur_ns=s1;
		}
#endif
		bgbcc_print(ctx, "}\n ");
		return;
	}

	if(BCCX_TagIsP(l, "namespace"))
	{
		s=BCCX_Get(l, "name");
//		n=BCCX_FindTag(l, "body");
//		c=BCCX_Fetch(l, "body");

		c=BCCX_Child(l);

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

		//BGBCC_BindStructSig(ctx, s, "namespace");
		bgbcc_print(ctx, "package { name='%s'\n", s);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;
		
		while(c)
		{
			BGBCC_BSVM_CompileTopStatement(ctx, c);
			c=BCCX_Next(c);
		}
		ctx->cur_ns=s1;

		bgbcc_print(ctx, "}\n");

		return;
	}

	if(BCCX_TagIsP(l, "null_statement"))
		{ return; }

	if(BCCX_TagIsP(l, "funcall"))
		{ return; }

	if(BCCX_TagIsP(l, "linecomment")) { return; }
	if(BCCX_TagIsP(l, "blockcomment")) { return; }

	BGBCC_Warn("unhandled top statement %s\n", BCCX_Tag(l));
}


char *BGBCC_BSVM_CompileModule(char *name, BCCX_Node *l)
{
	BGBCC_State *ctx;
	BCCX_Node *c;
	char *s;


	ctx=bgbcc_malloc(sizeof(BGBCC_State));
	memset(ctx, 0, sizeof(BGBCC_State));

	bgbcc_warn=0;
	bgbcc_err=0;

	s=BCCX_Get(l, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_Get(l, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_BSVM_CompileStructs(ctx);

	if(bgbcc_err)return(NULL);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_BSVM_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(bgbcc_err)

	{
		printf("Error Count %d\n", bgbcc_err);
		return(NULL);
	}

	return(ctx->ips);
}
