/*
Dummy Compiler code mostly intended for producing metadata...
 */

#include <bgbccc.h>

extern char *bgbcc_lfn;
extern int bgbcc_lln;
extern int bgbcc_reqlfn;

extern int bgbcc_warn;
extern int bgbcc_err;


void BGBCC_MetaCompileStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	char *s0, *s1, *s2;
	int i, j, k;

	s0=BCCX_Get(l, "fn");
	if(s0 && (s0!=bgbcc_lfn))
		bgbcc_lfn=s0;

	i=BCCX_GetInt(l, "ln");
	if((i>0) && (i!=bgbcc_lln))
		bgbcc_lln=i;

	l=BGBCC_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "switch"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "goto"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "label"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "continue"))
	{
		s0=ctx->contstack[ctx->contstackpos-1];
		return;
	}

	if(BCCX_TagIsP(l, "break"))
	{
		s0=ctx->breakstack[ctx->breakstackpos-1];
		return;
	}

	if(BCCX_TagIsP(l, "return"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "preinc") || BCCX_TagIsP(l, "postinc"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "predec") || BCCX_TagIsP(l, "postdec"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "defun"))	{ return; }
	if(BCCX_TagIsP(l, "vars"))	{ return; }


	if(BCCX_TagIsP(l, "if"))
	{
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_MetaCompileStatement(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsP(l, "for"))
	{
		BGBCC_MetaCompileStatement(ctx, BCCX_Fetch(l, "init"));
		BGBCC_MetaCompileStatement(ctx, BCCX_Fetch(l, "body"));
		BGBCC_MetaCompileStatement(ctx, BCCX_Fetch(l, "step"));
		return;
	}

	if(BCCX_TagIsP(l, "while"))
	{
//		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
//		i=BGBCC_BoolExpr(ctx, t);

		BGBCC_MetaCompileStatement(ctx, BCCX_Fetch(l, "body"));
		return;
	}


	if(BCCX_TagIsP(l, "do_while"))
	{
		BGBCC_MetaCompileStatement(ctx, BCCX_Fetch(l, "body"));
//
//		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
//		i=BGBCC_BoolExpr(ctx, t);
		return;
	}

//	BGBCC_MetaCompileExpr(ctx, l);
}

void BGBCC_MetaEmitVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1;
	int i;

//	s=BGBCC_VarTypeString(ctx, ty);
//	s1=name;
}

void BGBCC_MetaEmitVar2(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;

	i=BCCX_GetInt(ty, "flags");
	s2=(i&BGBCC_TYFL_STATIC)?"~":"";

	if(!s1)s1="_";

	if(ctx->cur_struct)
	{
		BGBCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"field", ctx->cur_idx++, s1);

		BGBCC_BindFieldSig(ctx, ctx->cur_struct, s1, s);
		BGBCC_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
	}
}

void BGBCC_MetaCompileVarStatement(BGBCC_State *ctx, BCCX_Node *l)
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

			BGBCC_MetaEmitVar2(ctx, s, t, v);

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

//		BGBCC_MetaCompileBlock(ctx, t, s, n, v);
		return;
	}
}

BCCX_Node *BGBCC_MetaCompileBlock(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	char *cname[16], *bname[16];

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

	BGBCC_MetaEmitSigProto(ctx, type, name, args);

	ctx->contstack=cname;
	ctx->breakstack=bname;

	ctx->cf_n=name;
	ctx->cf_ty=type;

	//hack: disable extern inline bastards...
	i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_EXTERN)
		return(NULL);

	if(BCCX_TagIsP(body, "begin"))
		body=BCCX_Child(body);

	BGBCC_MetaEmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);

	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_MetaEmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	i=BCCX_GetInt(type, "flags");

	c=body;
	while(c)
	{
		BGBCC_MetaCompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}


	bgbcc_reqlfn=1;

	c=body;
	while(c)
	{
		BGBCC_MetaCompileStatement(ctx, c);
		c=BCCX_Next(c);
	}

	return(NULL);
}

#if 0
char *BGBCC_GenProtoSig(BGBCC_State *ctx,
	BCCX_Node *type, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

	if(!type)return(NULL);

	c=args; t=tb;
	*t++='(';
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			*t++='z';
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTag(c, "type");
		if(n)
		{
			s=BGBCC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

	return(bgbcc_strdup(tb));
}
#endif

void BGBCC_MetaEmitSigProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

//	return;

	if(!type)return;

	i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_STATIC)return;

	c=args; t=tb;
	*t++='(';
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			*t++='z';
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTag(c, "type");
		if(n)
		{
			s=BGBCC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

	BGBCC_BindSig(ctx, name, tb);
}

BCCX_Node *BGBCC_MetaCompileProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

	BGBCC_MetaEmitSigProto(ctx, type, name, args);

	ctx->cf_n=name;
	ctx->cf_ty=type;

	BGBCC_MetaEmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);

	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_MetaEmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	i=BCCX_GetInt(type, "flags");
	return(NULL);
}

void BGBCC_MetaCompileStruct(BGBCC_State *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *t, *n;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	i=BCCX_GetInt(l, "flags");
	BGBCC_BindStructSig(ctx, s, "struct");
	BGBCC_BindStructInfoI(ctx, s, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_MetaCompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_MetaCompileUnion(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	i=BCCX_GetInt(l, "flags");
	BGBCC_BindStructSig(ctx, s, "union");
	BGBCC_BindStructInfoI(ctx, s, "flags", i);

	os0=ctx->cur_struct;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_MetaCompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

#if 0
void BGBCC_MetaCompileEnum(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *v;

	c=BCCX_Fetch(l, "body");
	while(BS1_CONSP(c))
	{
		n=CAAR(c);
		v=CADAR(c);
		t=BS1_MM_NULL;

		SET(ctx->mlenv, CONS(n, ctx->mlenv));
		SET(ctx->mtenv, CONS(t, ctx->mtenv));
		SET(ctx->mvenv, CONS(v, ctx->mvenv));

		c=CDR(c);
	}
}
#endif

BCCX_Node *BGBCC_MetaCompileSProto(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u;
	char *s, *s1;


	t=BCCX_FindTag(l, "type");
	BGBCC_MetaEmitVar(ctx, BCCX_Get(l, "name"), t, NULL);

	c=BCCX_Fetch(l, "args");
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
			{ c=BCCX_Next(c); continue; }
		if(!BCCX_TagIsP(c, "var"))
			{ c=BCCX_Next(c); continue; }

		t=BCCX_FindTag(c, "type");
		BGBCC_MetaEmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");
	u=BCCX_Fetch(l, "args");

	s1=BGBCC_GenProtoSig(ctx, t, u);
	BGBCC_BindStructSig(ctx, s, "func");
	BGBCC_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

void BGBCC_MetaCompileTypedef(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1;
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
		BGBCC_BindTypeSig(ctx, s, s1);

//		c=BCCX_Next(c);
	}

#if 0
	if(BCCX_TagIsP(l, "func"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		v=BCCX_FindTag(l, "args");

		s1=BGBCC_GenProtoSig(ctx, t, v);
		BGBCC_BindTypeSig(ctx, s, s1);
	}
#endif

	return;
}

void BGBCC_MetaCompileStructs(BGBCC_State *ctx)
{
	BCCX_Node *c, *t, *n, *l;

	c=ctx->types;
	while(c)
	{
		BGBCC_MetaCompileTypedef(ctx, c);
		c=BCCX_Next(c);
	}

	c=ctx->structs;
	while(c)
	{
		l=c;
		if(BCCX_TagIsP(c, "struct"))BGBCC_MetaCompileStruct(ctx, c);
		if(BCCX_TagIsP(c, "union"))BGBCC_MetaCompileUnion(ctx, c);
//		if(BCCX_TagIsP(c, "enum"))BGBCC_MetaCompileEnum(ctx, c);
		if(BCCX_TagIsP(c, "s_proto"))BGBCC_MetaCompileSProto(ctx, c);
		if(BCCX_TagIsP(c, "func"))BGBCC_MetaCompileSProto(ctx, c);

		c=BCCX_Next(c);
	}
}

void BGBCC_MetaEmitVarValue(BGBCC_State *ctx, BCCX_Node *v)
{
	if(BCCX_TagIsP(v, "int"))
	{
		return;
	}
	if(BCCX_TagIsP(v, "real"))
	{
		return;
	}
	if(BCCX_TagIsP(v, "string"))
	{
		return;
	}

	BGBCC_Error("BGBCC_MetaEmitVarValue: Bad Value\n");
}

void BGBCC_MetaEmitVarValueR(BGBCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_MetaEmitVarValueR(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	BGBCC_MetaEmitVarValue(ctx, v);
}

void BGBCC_MetaEmitTopArray(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int ord, cnt;
	int i, j, k;

	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;

	i=BCCX_GetInt(ty, "flags");
	if(!(i&BGBCC_TYFL_STATIC))BGBCC_BindSig(ctx, s1, s);

	if(!s1)return;
	if(!s1)s1="_";

	if(v && BCCX_TagIsP(v, "list"))
	{
		i=BCCX_GetInt(ty, "flags");
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_MetaEmitVarValueR(ctx, c);
			c=BCCX_Next(c);
		}

		return;
	}

	i=BCCX_GetInt(ty, "flags");
}

void BGBCC_MetaEmitTopVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int i, j;

	i=BCCX_GetInt(ty, "flags");
	if(i&BGBCC_TYFL_EXTERN)
	{
		s=BGBCC_VarTypeString(ctx, ty);
		s1=name;

		BGBCC_BindSig(ctx, s1, s);

		if(!s1)return;
		if(!s1)s1="_";

		return;
	}


	c=BCCX_Fetch(ty, "size"); i=0;
	while(c) { i++; c=BCCX_Next(c); }

	if(i)
	{
		BGBCC_MetaEmitTopArray(ctx, name, ty, v);
		return;
	}

	s=BGBCC_VarTypeString(ctx, ty);
	s1=name;

	i=BCCX_GetInt(ty, "flags");
	if(!(i&BGBCC_TYFL_STATIC))BGBCC_BindSig(ctx, s1, s);

	if(!s1)return;
	if(!s1)s1="_";

	i=BCCX_GetInt(ty, "flags");

	if(v)
	{
		BGBCC_MetaEmitVarValue(ctx, v);
	}
}

void BGBCC_MetaCompileTopStatement(BGBCC_State *ctx, BCCX_Node *l)
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
			if(BCCX_TagIsP(c, "proto"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				n=BCCX_Fetch(c, "args");

				BGBCC_MetaCompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsP(c, "var"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				v=BCCX_Fetch(c, "value");

				BGBCC_MetaEmitTopVar(ctx, s, t, v);
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

		BGBCC_MetaCompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "proto"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		BGBCC_MetaCompileProto(ctx, t, s, n);
		return;
	}
}


char *BGBCC_MetaCompileModule(char *name, BCCX_Node *l)
{
	BGBCC_State *ctx;
	BCCX_Node *c;


	ctx=bgbcc_malloc(sizeof(BGBCC_State));
	memset(ctx, 0, sizeof(BGBCC_State));

	bgbcc_warn=0;
	bgbcc_err=0;

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_MetaCompileStructs(ctx);

	if(bgbcc_err)return(NULL);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_MetaCompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(bgbcc_err)
	{
		printf("Error Count %d\n", bgbcc_err);
		return(NULL);
	}

	return(ctx->ips);
}
