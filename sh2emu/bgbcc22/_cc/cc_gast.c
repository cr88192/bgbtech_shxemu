/*
Generalized AST compiler.
Basically, the goal will be to reduce forms, and to rewrite types (replace
with a version using a signature string instead).
 */

#include <bgbccc.h>

extern char *bgbcc_lfn;
extern int bgbcc_lln;
extern int bgbcc_reqlfn;

extern int bgbcc_warn;
extern int bgbcc_err;


BCCX_Node *BGBCC_GAstCompileStatementList(BGBCC_State *ctx, char *tag,
	BCCX_Node *lst)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	char *s0, *s1, *s2;
	int i, j, k;

	n=BCCX_New(tag);
	c=lst;
	while(c)
	{
		t=BGBCC_GAstCompileStatement(ctx, c);
		BCCX_Add(n, t);
		c=BCCX_Next(c);
	}
	return(n);
}

BCCX_Node *BGBCC_GAstCompileStatementBody(BGBCC_State *ctx, BCCX_Node *l)
	{ return(BGBCC_GAstCompileStatementList(ctx, l->tag, BCCX_Child(l))); }

BCCX_Node *BGBCC_GAstCompileStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	char *s, *s0, *s1, *s2;
	int i, j, k;

	if(!l)return(l);

	l=BGBCC_ReduceStatement(ctx, l);
	if(!l)return(l);

	if(BCCX_TagIsP(l, "vars"))
		{ return(BGBCC_GAstCompileStatementBody(ctx, l)); }
	if(BCCX_TagIsP(l, "body"))
		{ return(BGBCC_GAstCompileStatementBody(ctx, l)); }
	if(BCCX_TagIsP(l, "args"))
		{ return(BGBCC_GAstCompileStatementBody(ctx, l)); }

	if(BCCX_TagIsP(l, "var"))
		{ return(BGBCC_GAstCompileVar(ctx, l)); }
	if(BCCX_TagIsP(l, "type"))
		{ return(BGBCC_GAstCompileType(ctx, l)); }

	if(BCCX_TagIsP(l, "defun"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		n=BGBCC_GAstCompileBlock(ctx, t, s, n, v);
		return(n);
	}

	if(BCCX_TagIsP(l, "proto"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		n=BGBCC_GAstCompileProto(ctx, t, s, n);
		return(n);
	}

	n=BGBCC_ReduceStatement(ctx, l);
	return(n);
}

BCCX_Node *BGBCC_GAstCompileType(BGBCC_State *ctx, BCCX_Node *ty)
{
	BCCX_Node *t, *n;
	char *s, *s1;
	int i;

	s=BGBCC_VarTypeString(ctx, ty);
	n=BCCX_New("type");
	BCCX_Set(n, "sig", s);
	
	t=BCCX_FindTag(ty, "expr");
	if(t)BCCX_Add(n, BCCX_Clone(t));

	t=BCCX_FindTag(ty, "attribute");
	while(t)
	{
		BCCX_Add(n, BCCX_Clone(t));
		t=BCCX_FindNextTag(BCCX_Next(t), "attribute");
	}

	t=BCCX_FindTag(ty, "declspec");
	while(t)
	{
		BCCX_Add(n, BCCX_Clone(t));
		t=BCCX_FindNextTag(BCCX_Next(t), "declspec");
	}


	return(n);
}

BCCX_Node *BGBCC_GAstCompileVar(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *n0, *n1, *v, *t;
	char *s;
	int i;

	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");
	v=BCCX_Fetch(l, "value");

	t=BGBCC_GAstCompileType(ctx, t);
	v=BGBCC_ReduceExpr(ctx, v);

	n=BCCX_New("var");
	BCCX_Set(n, "name", s);
	if(t)BCCX_Add(n, t);
	if(v)BCCX_Add(n, BCCX_New1("value", v));
	
	return(n);
}

BCCX_Node *BGBCC_GAstCompileBlock(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	BCCX_Node *c, *t, *n, *u, *ta, *tb, *tt;
	int i, j, k;

	//hack: disable extern inline bastards...
	i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_EXTERN)
		return(NULL);

	if(BCCX_TagIsP(body, "begin"))
		body=BCCX_Child(body);

	ta=BGBCC_GAstCompileStatementList(ctx, "args", args);
	tb=BGBCC_GAstCompileStatementList(ctx, "body", body);
	tt=BGBCC_GAstCompileType(ctx, type);

	n=BCCX_New3("defun", tt, ta, tb);
	BCCX_Set(n, "name", name);
	return(n);
}

BCCX_Node *BGBCC_GAstCompileProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u, *tt, *ta;
	int i, j, k;

	ta=BGBCC_GAstCompileStatementList(ctx, "args", args);
	tt=BGBCC_GAstCompileType(ctx, type);
	n=BCCX_New2("proto", tt, ta);
	BCCX_Set(n, "name", name);
	return(n);
}

BCCX_Node *BGBCC_GAstCompileStruct(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	os0=ctx->cur_struct; ctx->cur_struct=s; j=ctx->cur_idx; ctx->cur_idx=0;
	t=BGBCC_GAstCompileStatementList(ctx, "body", BCCX_Fetch(l, "body"));
	ctx->cur_struct=os0; ctx->cur_idx=j;
	n=BCCX_New1("struct", t);
	BCCX_Set(n, "name", s);
	return(n);
}

BCCX_Node *BGBCC_GAstCompileUnion(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n;
	char *s, *os0;
	int i, j;

	s=BCCX_Get(l, "name");
	os0=ctx->cur_struct; ctx->cur_struct=s; j=ctx->cur_idx; ctx->cur_idx=0;
	t=BGBCC_GAstCompileStatementList(ctx, "body", BCCX_Fetch(l, "body"));
	ctx->cur_struct=os0; ctx->cur_idx=j;
	n=BCCX_New1("union", t);
	BCCX_Set(n, "name", s);
	return(n);
}

BCCX_Node *BGBCC_GAstCompileSProto(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u, *tt, *ta;
	char *s;
	int i, j, k;

	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");
	n=BCCX_Fetch(l, "args");

	ta=BGBCC_GAstCompileStatementList(ctx, "args", n);
	tt=BGBCC_GAstCompileType(ctx, t);
	n=BCCX_New2("s_proto", tt, ta);
	BCCX_Set(n, "name", s);
	return(n);
}

BCCX_Node *BGBCC_GAstCompileTypedef(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *t;

	BGBCC_MetaCompileTypedef(ctx, l);

	if(BCCX_TagIsP(l, "struct"))return(BGBCC_GAstCompileStruct(ctx, l));
	if(BCCX_TagIsP(l, "union"))return(BGBCC_GAstCompileUnion(ctx, l));
//	if(BCCX_TagIsP(l, "enum"))return(BGBCC_GAstCompileEnum(ctx, l));
	if(BCCX_TagIsP(l, "s_proto"))return(BGBCC_GAstCompileSProto(ctx, l));
	if(BCCX_TagIsP(l, "func"))return(BGBCC_GAstCompileSProto(ctx, l));

	t=BGBCC_GAstCompileStatement(ctx, l);
	return(t);
}

void BGBCC_GAstCompileStructs(BGBCC_State *ctx)
{
	BCCX_Node *c, *t, *n, *l;

	c=ctx->types; l=NULL;
	while(c)
	{
		t=BGBCC_GAstCompileTypedef(ctx, c);
		l=BCCX_AddEnd(l, t);
		c=BCCX_Next(c);
	}
	ctx->types=l;

	c=ctx->structs; l=NULL;
	while(c)
	{
		t=BGBCC_GAstCompileTypedef(ctx, c);
		l=BCCX_AddEnd(l, t);
		c=BCCX_Next(c);
	}
	ctx->structs=l;
}

BCCX_Node *BGBCC_GAstCompileTopStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BGBCC_MetaCompileTopStatement(ctx, l);
	return(BGBCC_GAstCompileStatement(ctx, l));
}


byte *BGBCC_GAstCompileModule(char *name, BCCX_Node *l, int *rsz)
{
	char tb[256];
	BGBCC_State *ctx;
	BCCX_Node *c, *t, *n;
	FILE *fd;
	byte *buf;
	int i, sz;

	ctx=bgbcc_malloc(sizeof(BGBCC_State));
	memset(ctx, 0, sizeof(BGBCC_State));

	bgbcc_warn=0;
	bgbcc_err=0;

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_GAstCompileStructs(ctx);

	if(bgbcc_err)return(NULL);

	n=BCCX_New("body");
	c=BCCX_Fetch(l, "body");
	while(c)
	{
		t=BGBCC_GAstCompileTopStatement(ctx, c);
		BCCX_Add(n, t);
		c=BCCX_Next(c);
	}

	if(bgbcc_err)
	{
		printf("GAST: Error Count %d\n", bgbcc_err);
		return(NULL);
	}

	n=BCCX_New3("module",
		BCCX_New1("types", ctx->types),
		BCCX_New1("structs", ctx->structs),
		n);

	buf=malloc(1<<24);
	sz=SBXE_WriteNodeList(buf, n);
	if(sz<=0) { free(buf); return(NULL); }

	printf("SBXE Encoded %d bytes\n", sz);
	buf=realloc(buf, sz);
	if(rsz)*rsz=sz;

#if 1
	if(name)
	{
		sprintf(tb, "dump/%s_gast.txt", name);
		fd=fopen(tb, "wt");
		if(fd)
		{
			BCCX_PrintFD(fd, n);
			fclose(fd);
		}
	
		sprintf(tb, "dump/%s_gast.sbxe", name);
		fd=fopen(tb, "wb");
		if(fd)
		{
			fwrite(buf, 1, sz, fd);
			fclose(fd);
		}
	}
#endif

	return(buf);
}
