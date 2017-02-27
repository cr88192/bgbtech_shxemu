#include <bgbccc.h>

extern char *bgbcc_lfn;
extern int bgbcc_lln;
extern int bgbcc_reqlfn;

extern int bgbcc_warn;
extern int bgbcc_err;


int BGBCC_BMC_CompileSwitch(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *u, *v;

	bgbcc_print(ctx, "switch(");
	BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "cond"));
	bgbcc_print(ctx, ") {\n");

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsP(c, "case"))
		{
			bgbcc_print(ctx, "case ");
			BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(c, "value"));
			bgbcc_print(ctx, ":\n");
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsP(c, "case_default"))
		{
			bgbcc_print(ctx, "default:\n");
			c=BCCX_Next(c);
			continue;
		}

		BGBCC_BMC_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}

	bgbcc_print(ctx, "}");
}

void BGBCC_BMC_CompileStatement(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	char *s0, *s1, *s2;
	int i, j, k;

	bgbcc_print(ctx, "\n");

	i=BCCX_GetInt(l, "ln");
	if((i>0) && (i!=bgbcc_lln))
	{
		s0=BCCX_Get(l, "fn");
		if(s0 && (s0!=bgbcc_lfn))
		{
			if(!bgbcc_reqlfn)
				bgbcc_print(ctx, " /*%s%d*/ ",
					BGBCC_CIfy(s0), i);
			bgbcc_lfn=s0;
			bgbcc_lln=i;
		}else
		{
			if(!bgbcc_reqlfn)bgbcc_print(ctx, " /*%d*/ ", i);
			bgbcc_lln=i;
		}
	}else
	{
		s0=BCCX_Get(l, "fn");
		if(s0 && (s0!=bgbcc_lfn))
		{
			if(!bgbcc_reqlfn)
				bgbcc_print(ctx, " /*%s1*/ ",
					BGBCC_CIfy(s0));
			bgbcc_lfn=s0;
			bgbcc_lln=1;
		}
	}

	if(bgbcc_reqlfn)
	{
		bgbcc_print(ctx, " /*%s%d*/ ",
			BGBCC_CIfy(bgbcc_lfn), bgbcc_lln);
		bgbcc_reqlfn=0;
	}

	l=BGBCC_ReduceExpr(ctx, l);

	if(BCCX_TagIsP(l, "switch"))
	{
		BGBCC_BMC_CompileSwitch(ctx, l);
		return;
	}

	if(BCCX_TagIsP(l, "goto"))
	{
		bgbcc_print(ctx, "goto %s;", BCCX_Get(l, "name"));
//		BGBCC_BMC_CompileJmp(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "label"))
	{
		bgbcc_print(ctx, "%s:", BCCX_Get(l, "name"));
//		BGBCC_BMC_EmitLabel(ctx, BCCX_Get(l, "name"));
		return;
	}

	if(BCCX_TagIsP(l, "continue"))
	{
		bgbcc_print(ctx, "continue;");

//		s0=ctx->contstack[ctx->contstackpos-1];
//		BGBCC_BMC_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "break"))
	{
		bgbcc_print(ctx, "break;");

//		s0=ctx->breakstack[ctx->breakstackpos-1];
//		BGBCC_BMC_CompileJmp(ctx, s0);
		return;
	}

	if(BCCX_TagIsP(l, "return"))
	{
		t=BCCX_Fetch(l, "value");
		if(!t)
		{
			bgbcc_print(ctx, "return;");
			return;
		}

		bgbcc_print(ctx, "return(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ");");
		return;
	}

	if(BCCX_TagIsP(l, "assign"))
	{
		s0=BCCX_Get(l, "op");

		if(s0)
		{
			BGBCC_BMC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
			BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "left"));
			bgbcc_print(ctx, "%s", s0);
			BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
			bgbcc_print(ctx, ";");
			return;
		}

		BGBCC_BMC_CompileAssign(ctx, BCCX_Fetch(l, "left"));
		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "right"));
		bgbcc_print(ctx, ";");
		return;
	}

	if(BCCX_TagIsP(l, "preinc") || BCCX_TagIsP(l, "postinc"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		bgbcc_print(ctx, "(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")++;");
		return;
	}

	if(BCCX_TagIsP(l, "predec") || BCCX_TagIsP(l, "postdec"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));
		bgbcc_print(ctx, "(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")--;");
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
			BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			if(t)BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "else"));
			return;
		}

		bgbcc_print(ctx, "if(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")");

		BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "then"));

		t=BCCX_Fetch(l, "else");
		if(t)
		{
			bgbcc_print(ctx, " else ");
			BGBCC_BMC_CompileStatement(ctx, t);
		}
		return;
	}

	if(BCCX_TagIsP(l, "begin"))
	{
		bgbcc_print(ctx, "{\n");
		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_BMC_CompileStatement(ctx, c);
			bgbcc_print(ctx, "\n");
			c=BCCX_Next(c);
		}
		bgbcc_print(ctx, "}\n");
		return;
	}

	if(BCCX_TagIsP(l, "for"))
	{
		bgbcc_print(ctx, "for(");

		BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "init"));
		bgbcc_print(ctx, " ");

		BGBCC_BMC_CompileExpr(ctx, BCCX_Fetch(l, "cond"));
		bgbcc_print(ctx, "; ");

		BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "body"));
		bgbcc_print(ctx, ")");

		BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "step"));
		return;
	}

	if(BCCX_TagIsP(l, "while"))
	{
		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);

		if(i==0)return;

		if(i==1)
		{
			bgbcc_print(ctx, "while(1)");
			BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "body"));
			return;
		}

		bgbcc_print(ctx, "while(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, ")");
		BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "body"));
		return;
	}


	if(BCCX_TagIsP(l, "do_while"))
	{

		bgbcc_print(ctx, "do ");

		BGBCC_BMC_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		t=BGBCC_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_BoolExpr(ctx, t);
		if(i==0)
		{
			bgbcc_print(ctx, " while(0);");
			return;
		}

		if(i==1)
		{
			bgbcc_print(ctx, " while(1);");
			return;
		}

		bgbcc_print(ctx, " while(");
		BGBCC_BMC_CompileExpr(ctx, t);
		bgbcc_print(ctx, " );");
		return;
	}


	BGBCC_BMC_CompileExpr(ctx, l);
	bgbcc_print(ctx, ";");
}


char *BGBCC_BMC_VarTypeString(BGBCC_State *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i;

	t=buf;

	if(!BCCX_TagIsP(ty, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)return(bgbcc_strdup("void"));

	if(!BCCX_TagIsP(ty, "type"))
		return(bgbcc_strdup("void"));

	if(ty)
	{
		i=BCCX_GetInt(ty, "flags");
		s=BCCX_Get(ty, "name");

		if(i&BGBCC_TYFL_PERSISTENT)
			{ sprintf(t, "__wide "); t+=strlen(t); }

		i=BCCX_GetInt(ty, "ind");
		while(i--)*t++='*';

		i=BCCX_GetInt(ty, "flags");
		s=BCCX_Get(ty, "name");

		if(!s)
			return(bgbcc_strdup("void"));

		t1=t;
		if(i&BGBCC_TYFL_UNSIGNED)
		{
			if(!strcmp(s, "void"))
				{ sprintf(t, "void"); t+=strlen(t); }
			if(!strcmp(s, "char"))
				{ sprintf(t, "__uint8"); t+=strlen(t); }
			if(!strcmp(s, "short"))
				{ sprintf(t, "__uint16"); t+=strlen(t); }
			if(!strcmp(s, "int"))
				{ sprintf(t, "__uint32"); t+=strlen(t); }
			if(!strcmp(s, "long"))
				{ sprintf(t, "long"); t+=strlen(t); }
			if(!strcmp(s, "llong"))
				{ sprintf(t, "__uint64"); t+=strlen(t); }
			if(!strcmp(s, "int128"))
				{ sprintf(t, "__uint128"); t+=strlen(t); }

			if(!strcmp(s, "float"))
				{ sprintf(t, "_Complex float"); t+=strlen(t); }
			if(!strcmp(s, "double"))
				{ sprintf(t, "_Complex double"); t+=strlen(t); }
			if(!strcmp(s, "float128"))
				{ sprintf(t, "_Complex __float128"); t+=strlen(t); }
			if(!strcmp(s, "float16"))
				{ sprintf(t, "_Complex __float16"); t+=strlen(t); }
		}else
		{
			if(!strcmp(s, "void"))
				{ sprintf(t, "void"); t+=strlen(t); }
			if(!strcmp(s, "char"))
				{ sprintf(t, "__int8"); t+=strlen(t); }
			if(!strcmp(s, "short"))
				{ sprintf(t, "__int16"); t+=strlen(t); }
			if(!strcmp(s, "int"))
				{ sprintf(t, "__int32"); t+=strlen(t); }
			if(!strcmp(s, "long"))
				{ sprintf(t, "long"); t+=strlen(t); }
			if(!strcmp(s, "llong"))
				{ sprintf(t, "__int64"); t+=strlen(t); }
			if(!strcmp(s, "int128"))
				{ sprintf(t, "__int128"); t+=strlen(t); }

			if(!strcmp(s, "float"))
				{ sprintf(t, "float"); t+=strlen(t); }
			if(!strcmp(s, "double"))
				{ sprintf(t, "double"); t+=strlen(t); }
			if(!strcmp(s, "float128"))
				{ sprintf(t, "__float128"); t+=strlen(t); }
			if(!strcmp(s, "float16"))
				{ sprintf(t, "__float16"); t+=strlen(t); }

			if(!strcmp(s, "variant"))
				{ sprintf(t, "__variant"); t+=strlen(t); }
		}

		if(ctx && (t==t1))
		{
			c=ctx->structs;
			while(c)
			{
				if(BCCX_AttrIsP(c, "name", s))
					break;
				c=BCCX_Next(c);
			}

			if(c && BCCX_TagIsP(c, "struct"))
			{
				sprintf(t, "struct %s", s);
				t+=strlen(t);
				break;
			}

			if(c && BCCX_TagIsP(c, "union"))
			{
				sprintf(t, "union %s", s);
				t+=strlen(t);
				break;
			}
		}

		if(t==t1)
		{
			sprintf(t, "U%s;", s);
			t+=strlen(t);
		}

		c=BCCX_Fetch(ty, "size");
		while(c)
		{
			n=BGBCC_ReduceForm(ctx, c);

			if(!BCCX_TagIsP(n, "int"))
			{
				BGBCC_Error("Invalid array size specifier");
				*t++='0'; c=BCCX_Next(c); continue;
			}

			i=BCCX_GetInt(n, "value");
			sprintf(t, "%d", i);
			t+=strlen(t);

			c=BCCX_Next(c);
			if(c)*t++=',';
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

void BGBCC_BMC_EmitVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1;
	int i;

	s=BGBCC_BMC_VarTypeString(ctx, ty);
	s1=name;

	if(!s1)s1="_";
	if(*s)bgbcc_print(ctx, "%s:%s ", s1, s);
		else bgbcc_print(ctx, "%s ", s1);
}

void BGBCC_BMC_EmitVar2(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	s=BGBCC_BMC_VarTypeString(ctx, ty);
	s1=name;

	i=BCCX_GetInt(ty, "flags");
	s2=(i&BGBCC_TYFL_STATIC)?"~":"";

	if(!s1)s1="_";
	if(*s)bgbcc_print(ctx, "%s%s:%s ", s1, s2, s);
		else bgbcc_print(ctx, "%s%s ", s1, s2);

	if(ctx->cur_struct)
	{
		BGBCC_BindStructInfoIdx(ctx, ctx->cur_struct,
			"field", ctx->cur_idx++, s1);

		BGBCC_BindFieldSig(ctx, ctx->cur_struct, s1, s);
		BGBCC_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
	}
}

void BGBCC_BMC_CompileVarStatement(BGBCC_State *ctx, BCCX_Node *l)
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

			BGBCC_BMC_EmitVar2(ctx, s, t, v);

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

//		BGBCC_BMC_CompileBlock(ctx, t, s, n, v);
		return;
	}
}

BCCX_Node *BGBCC_BMC_CompileBlock(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	char *cname[16], *bname[16];

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

	BGBCC_BMC_EmitSigProto(ctx, type, name, args);

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

	bgbcc_print(ctx, "FUNC ");
	BGBCC_BMC_EmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);

	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			bgbcc_print(ctx, "... ");
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_BMC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_STDCALL)bgbcc_print(ctx, "$stdcall ");
	if(i&BGBCC_TYFL_CDECL)bgbcc_print(ctx, "$cdecl ");
	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");
	if(i&BGBCC_TYFL_PROXY)bgbcc_print(ctx, "$proxy ");

	bgbcc_print(ctx, "VARS ");

	c=body;
	while(c)
	{
		BGBCC_BMC_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}


	bgbcc_print(ctx, "BODY ");

	bgbcc_reqlfn=1;

	c=body;
	while(c)
	{
		BGBCC_BMC_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}

	bgbcc_print(ctx, "ret_v ");
	bgbcc_print(ctx, "END\n\n");

	return(NULL);
}

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
			s=BGBCC_BMC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_BMC_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

	return(bgbcc_strdup(tb));
}

void BGBCC_BMC_EmitSigProto(BGBCC_State *ctx,
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
			s=BGBCC_BMC_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_BMC_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

	BGBCC_BindSig(ctx, name, tb);
}

BCCX_Node *BGBCC_BMC_CompileProto(BGBCC_State *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

	BGBCC_BMC_EmitSigProto(ctx, type, name, args);

	ctx->cf_n=name;
	ctx->cf_ty=type;

	bgbcc_print(ctx, "PROTO ");
	BGBCC_BMC_EmitVar(ctx, ctx->cf_n, ctx->cf_ty, NULL);

	c=args;
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
		{
			bgbcc_print(ctx, "... ");
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsP(c, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_BMC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	i=BCCX_GetInt(type, "flags");
	if(i&BGBCC_TYFL_STDCALL)bgbcc_print(ctx, "$stdcall ");
	if(i&BGBCC_TYFL_CDECL)bgbcc_print(ctx, "$cdecl ");
	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");
	if(i&BGBCC_TYFL_PROXY)bgbcc_print(ctx, "$proxy ");

	bgbcc_print(ctx, "END\n");

	return(NULL);
}

void BGBCC_BMC_CompileStruct(BGBCC_State *ctx, BCCX_Node *l)
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

	bgbcc_print(ctx, "STRUCT %s ", BCCX_Get(l, "name"));
	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_BMC_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	bgbcc_print(ctx, "END\n");

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_BMC_CompileUnion(BGBCC_State *ctx, BCCX_Node *l)
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

	bgbcc_print(ctx, "UNION %s ", BCCX_Get(l, "name"));
	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_BMC_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	bgbcc_print(ctx, "END\n");

	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

#if 0
void BGBCC_BMC_CompileEnum(BGBCC_State *ctx, BCCX_Node *l)
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

BCCX_Node *BGBCC_BMC_CompileSProto(BGBCC_State *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u;
	char *s, *s1;

	bgbcc_print(ctx, "S_PROTO ");

	t=BCCX_FindTag(l, "type");
	BGBCC_BMC_EmitVar(ctx, BCCX_Get(l, "name"), t, NULL);

	c=BCCX_Fetch(l, "args");
	while(c)
	{
		if(BCCX_TagIsP(c, "rest"))
			{ bgbcc_print(ctx, "... "); c=BCCX_Next(c); continue; }
		if(!BCCX_TagIsP(c, "var"))
			{ c=BCCX_Next(c); continue; }

		t=BCCX_FindTag(c, "type");
		BGBCC_BMC_EmitVar(ctx, BCCX_Get(c, "name"), t, NULL);
		c=BCCX_Next(c);
	}

	bgbcc_print(ctx, "END\n");


	s=BCCX_Get(l, "name");
	t=BCCX_FindTag(l, "type");
	u=BCCX_Fetch(l, "args");

	s1=BGBCC_GenProtoSig(ctx, t, u);
	BGBCC_BindStructSig(ctx, s, "func");
	BGBCC_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

void BGBCC_BMC_CompileTypedef(BGBCC_State *ctx, BCCX_Node *l)
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

		s1=BGBCC_BMC_VarTypeString(ctx, t);
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

void BGBCC_BMC_CompileStructs(BGBCC_State *ctx)
{
	BCCX_Node *c, *t, *n, *l;

	c=ctx->types;
	while(c)
	{
		BGBCC_BMC_CompileTypedef(ctx, c);
		c=BCCX_Next(c);
	}

	c=ctx->structs;
	while(c)
	{
		l=c;
		if(BCCX_TagIsP(c, "struct"))BGBCC_BMC_CompileStruct(ctx, c);
		if(BCCX_TagIsP(c, "union"))BGBCC_BMC_CompileUnion(ctx, c);
//		if(BCCX_TagIsP(c, "enum"))BGBCC_BMC_CompileEnum(ctx, c);
		if(BCCX_TagIsP(c, "s_proto"))BGBCC_BMC_CompileSProto(ctx, c);
		if(BCCX_TagIsP(c, "func"))BGBCC_BMC_CompileSProto(ctx, c);

		c=BCCX_Next(c);
	}
}

void BGBCC_BMC_EmitVarValue(BGBCC_State *ctx, BCCX_Node *v)
{
	if(BCCX_TagIsP(v, "int"))
	{
		bgbcc_print(ctx, "%s", BCCX_Get(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "real"))
	{
		bgbcc_print(ctx, "%s", BCCX_Get(v, "value"));
		return;
	}
	if(BCCX_TagIsP(v, "string"))
	{
		bgbcc_print(ctx, "%s", BGBCC_CIfy(BCCX_Get(v, "value")));
		return;
	}

	BGBCC_Error("BGBCC_BMC_EmitVarValue: Bad Value\n");
}

void BGBCC_BMC_EmitVarValueR(BGBCC_State *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsP(v, "list"))
	{
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_BMC_EmitVarValueR(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}
		return;
	}

	BGBCC_BMC_EmitVarValue(ctx, v);
}

void BGBCC_BMC_EmitTopArray(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int ord, cnt;
	int i, j, k;

	s=BGBCC_BMC_VarTypeString(ctx, ty);
	s1=name;

	i=BCCX_GetInt(ty, "flags");
	if(!(i&BGBCC_TYFL_STATIC))BGBCC_BindSig(ctx, s1, s);

	if(!s1)return;
	if(!s1)s1="_";

	if(v && BCCX_TagIsP(v, "list"))
	{
		bgbcc_print(ctx, "ARR ");
		i=BCCX_GetInt(ty, "flags");
		if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
		if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

		bgbcc_print(ctx, "%s:%s ", s1, s);

		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_BMC_EmitVarValueR(ctx, c);
			bgbcc_print(ctx, " ");
			c=BCCX_Next(c);
		}

		bgbcc_print(ctx, "END\n");

		return;
	}

	bgbcc_print(ctx, "VAR ");
	i=BCCX_GetInt(ty, "flags");
	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

	bgbcc_print(ctx, "%s:%s\n", s1, s);
}

void BGBCC_BMC_EmitTopVar(BGBCC_State *ctx, char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int i, j;

	i=BCCX_GetInt(ty, "flags");
	if(i&BGBCC_TYFL_EXTERN)
	{
		s=BGBCC_BMC_VarTypeString(ctx, ty);
		s1=name;

		BGBCC_BindSig(ctx, s1, s);

		if(!s1)return;
		if(!s1)s1="_";

		bgbcc_print(ctx, "EXTERN ");

		if(*s)bgbcc_print(ctx, "%s:%s", s1, s);
			else bgbcc_print(ctx, "%s", s1);

		bgbcc_print(ctx, "\n");
		return;
	}


	c=BCCX_Fetch(ty, "size"); i=0;
	while(c) { i++; c=BCCX_Next(c); }

	if(i)
	{
		BGBCC_BMC_EmitTopArray(ctx, name, ty, v);
		return;
	}

	s=BGBCC_BMC_VarTypeString(ctx, ty);
	s1=name;

	i=BCCX_GetInt(ty, "flags");
	if(!(i&BGBCC_TYFL_STATIC))BGBCC_BindSig(ctx, s1, s);

	if(!s1)return;
	if(!s1)s1="_";

	bgbcc_print(ctx, "VAR ");

	i=BCCX_GetInt(ty, "flags");
	if(i&BGBCC_TYFL_STATIC)bgbcc_print(ctx, "$static ");
	if(i&BGBCC_TYFL_INLINE)bgbcc_print(ctx, "$inline ");

	if(*s)bgbcc_print(ctx, "%s:%s", s1, s);
		else bgbcc_print(ctx, "%s", s1);

	if(v)
	{
		bgbcc_print(ctx, "=");
		BGBCC_BMC_EmitVarValue(ctx, v);
	}

	bgbcc_print(ctx, "\n");
}

void BGBCC_BMC_CompileTopStatement(BGBCC_State *ctx, BCCX_Node *l)
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

				BGBCC_BMC_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsP(c, "var"))
			{
				s=BCCX_Get(c, "name");
				t=BCCX_FindTag(c, "type");
				v=BCCX_Fetch(c, "value");

				BGBCC_BMC_EmitTopVar(ctx, s, t, v);
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

		BGBCC_BMC_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsP(l, "proto"))
	{
		s=BCCX_Get(l, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		BGBCC_BMC_CompileProto(ctx, t, s, n);
		return;
	}
}


char *BGBCC_BMC_CompileModule(char *name, BCCX_Node *l)
{
	BGBCC_State *ctx;
	BCCX_Node *c;


	ctx=bgbcc_malloc(sizeof(BGBCC_State));
	memset(ctx, 0, sizeof(BGBCC_State));

	bgbcc_warn=0;
	bgbcc_err=0;

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_BMC_CompileStructs(ctx);

	if(bgbcc_err)return(NULL);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_BMC_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(bgbcc_err)
	{
		printf("Error Count %d\n", bgbcc_err);
		return(NULL);
	}

	return(ctx->ips);
}
