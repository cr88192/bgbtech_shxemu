#include <bgbccc.h>


void BGBCC_CCXL_Warn(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_warn++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: warning: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_Error(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_error++;
//	bgbcc_frcc_err++;
	printf("%s:%d: error: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_Note(BGBCC_TransState *ctx, char *str, ...)
{
	va_list lst;

	va_start(lst, str);

	ctx->n_note++;
//	bgbcc_frcc_warn++;
	printf("%s:%d: note: ", ctx->lfn, ctx->lln);
	vprintf(str, lst);

	va_end(lst);
}

void BGBCC_CCXL_StubWarnLLn(BGBCC_TransState *ctx,
	char *file, int line)
{
	BGBCC_CCXL_Warn(ctx, "StubWarn %s:%d\n", file, line);
}

void BGBCC_CCXL_StubErrorLLn(BGBCC_TransState *ctx,
	char *file, int line)
{
	BGBCC_CCXL_Error(ctx, "StubError %s:%d\n", file, line);
}

void BGBCC_CCXL_TagErrorLLn(BGBCC_TransState *ctx, int tag,
	char *file, int line)
{
	BGBCC_CCXL_Error(ctx, "TagError(%04X) %s:%d\n", tag, file, line);
}

void BGBCC_CCXL_TagWarnLLn(BGBCC_TransState *ctx, int tag,
	char *file, int line)
{
	BGBCC_CCXL_Warn(ctx, "TagWarn(%04X) %s:%d\n", tag, file, line);
}

ccxl_label BGBCC_CCXL_GenSym(BGBCC_TransState *ctx)
{
//	char buf[32];
	ccxl_label l;
//	int id;

	l.id=CCXL_LBL_GENSYMBASE+(ctx->gs_seq++);
	return(l);

//	sprintf(buf, "GS%d", ctx->gs_seq++);
//	return(bgbcc_strdup(buf));
}

ccxl_label BGBCC_CCXL_GenSym2(BGBCC_TransState *ctx)
{
	return(BGBCC_CCXL_GenSym(ctx));
//	return(BS1_RStrDup(BASM_GenSym()));
}

int BGBCC_CCXL_CompileSwitchJmpR(
	BGBCC_TransState *ctx,
	ccxl_label *cl, s64 *clv,
	int clm, int cln,
	ccxl_label dfl)
{
	ccxl_label lclc, dflz;
	int ncl, clc;
	int i, j, k;
	
	/* Stack: Cond -- */
	
	ncl=cln-clm;
	
	if(ncl<4)
	{
		for(i=0; i<ncl; i++)
		{
//			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_StackPushConstInt(ctx, clv[clm+i]);
			BGBCC_CCXL_CompileJmpCond(ctx, "==", cl[clm+i]);
		}

//		BGBCC_CCXL_StackPop(ctx);
		if(dfl.id)
		{
			BGBCC_CCXL_CompileJmp(ctx, dfl);
		}
		return(0);
	}else
	{
		clc=(clm+cln)>>1;
		lclc=BGBCC_CCXL_GenSym(ctx);
		dflz=dfl;
		if(clv[clc]==(clv[clc-1]+1))
			dflz.id=0;

//		BGBCC_CCXL_StackDup(ctx);
		BGBCC_CCXL_StackDupClean(ctx);
		BGBCC_CCXL_StackPushConstInt(ctx, clv[clc]);
		BGBCC_CCXL_CompileJmpCond(ctx, ">=", lclc);
		
		BGBCC_CCXL_CompileSwitchJmpR(ctx, cl, clv, clm, clc, dflz);
		BGBCC_CCXL_EmitLabel(ctx, lclc);
		BGBCC_CCXL_CompileSwitchJmpR(ctx, cl, clv, clc, cln, dfl);
		return(0);
	}
}

int BGBCC_CCXL_CompileSwitch(BGBCC_TransState *ctx, BCCX_Node *l)
{
	ccxl_label t_cl[512];
	s64 t_clv[512];
	ccxl_label t_cl2[512];
	s64 t_clv2[512];

	BGBCC_CCXL_RegisterInfo *ri;
	ccxl_label *cl, *cl2;
	s64 *clv, *clv2;
	char *s;
	ccxl_label l0, dfl, lbrk, fxdfl;
	s64 li;
	int i, j, ncl, mcl;
	BCCX_Node *c, *t, *u, *v;

//	cl=bgbcc_malloc(4096*sizeof(ccxl_label));
//	clv=bgbcc_malloc(4096*sizeof(s64));
//	mcl=4096;

	cl=t_cl;
	clv=t_clv;
	mcl=512;
	
	cl2=NULL;
	clv2=NULL;

	lbrk=BGBCC_CCXL_GenSym(ctx);

#if 1
	dfl.id=0;
	fxdfl.id=0;
	ncl=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if((ncl+1)>=mcl)
		{
			if(cl==t_cl)
			{
				i=4096;
				cl2=bgbcc_malloc(i*sizeof(ccxl_label));
				clv2=bgbcc_malloc(i*sizeof(s64));
				memcpy(cl2, cl, mcl*sizeof(ccxl_label));
				memcpy(clv2, clv, mcl*sizeof(s64));
				
				cl=cl2;
				clv=clv2;
				mcl=i;
				continue;
			}
			
			i=mcl+(mcl>>1);
			cl=bgbcc_realloc(cl, i*sizeof(ccxl_label));
			clv=bgbcc_realloc(clv, i*sizeof(s64));
			mcl=i;
			continue;
		}
	
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case"))
		{
			v=BCCX_Fetch(c, "value");
			v=BGBCC_CCXL_ReduceExprConst(ctx, v);

			if(BGBCC_CCXL_IsIntP(ctx, v))
			{
				li=BCCX_GetIntCst(v, &bgbcc_rcst_value, "value");
			}else if(BGBCC_CCXL_IsCharP(ctx, v))
			{
				s=BCCX_GetCst(v, &bgbcc_rcst_value, "value");
				if(s) { li=BGBCP_ParseChar(&s); }
			}else if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
			{
				s=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
				ri=BGBCC_CCXL_LookupGlobal(ctx, s);
				if(ri && BGBCC_CCXL_IsRegImmIntP(ctx, ri->value))
				{
					li=BGBCC_CCXL_GetRegImmIntValue(ctx, ri->value);
				}else
				{
					BGBCC_CCXL_StubError(ctx);
				}
			}else
			{
				BGBCC_CCXL_StubError(ctx);
			}

			i=ncl++;
			cl[i]=BGBCC_CCXL_GenSym(ctx);
			clv[i]=li;
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			dfl=BGBCC_CCXL_GenSym(ctx);
//			BGBCC_CCXL_CompileJmp(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "cond"));
	
	/* small switch, use if/else */
	if(ncl<8)
//	if(1)
	{
		for(i=0; i<ncl; i++)
		{
//			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_StackPushConstInt(ctx, clv[i]);
			BGBCC_CCXL_CompileJmpCond(ctx, "==", cl[i]);
		}

		if(dfl.id)
		{
			BGBCC_CCXL_CompileJmp(ctx, dfl);
			BGBCC_CCXL_StackPop(ctx);
		}else
		{
			BGBCC_CCXL_StackPop(ctx);
			BGBCC_CCXL_CompileJmp(ctx, lbrk);
		}
		
		cl2=NULL;
		clv2=NULL;
	}else
	{
		/* larger switch, use binary dispatch */
		if(ncl<512)
		{
			cl2=t_cl2;
			clv2=t_clv2;
		}else
		{
			cl2=bgbcc_malloc(mcl*sizeof(ccxl_label));
			clv2=bgbcc_malloc(mcl*sizeof(s64));
		}

		/* generate sorted list of cases */
		for(i=0; i<ncl; i++)
			{ cl2[i]=cl[i]; clv2[i]=clv[i]; }
		for(i=0; i<ncl; i++)
			for(j=i+1; j<ncl; j++)
		{
			if(clv2[j]<clv2[i])
			{
				li=clv2[j]; clv2[j]=clv2[i]; clv2[i]=li;
				l0=cl2[j]; cl2[j]=cl2[i]; cl2[i]=l0;
			}
		}
		
		if(!dfl.id)
		{
			fxdfl=BGBCC_CCXL_GenSym(ctx);
			dfl=fxdfl;
		}
		BGBCC_CCXL_CompileSwitchJmpR(ctx, cl2, clv2, 0, ncl, dfl);
	}

#endif

#if 0
	ncl=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case") ||
			BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			cl[ncl++]=BGBCC_CCXL_GenSym(ctx);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "cond"));

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case"))
		{
//			BGBCC_CCXL_StackDup(ctx);
			BGBCC_CCXL_StackDupClean(ctx);
			BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(c, "value"));
			BGBCC_CCXL_CompileJmpCond(ctx, "==", cl[i++]);
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			BGBCC_CCXL_CompileJmp(ctx, cl[i++]);
			c=BCCX_Next(c);
			continue;
		}
		c=BCCX_Next(c);
	}
#endif


//	lbrk=BGBCC_CCXL_GenSym(ctx);
	ctx->breakstack[ctx->breakstackpos++]=lbrk;

//	l0=BGBCC_CCXL_GenSym(ctx);
//	ctx->breakstack[ctx->breakstackpos++]=l0;
//	BGBCC_CCXL_CompileBreak(ctx);

	i=0; c=BCCX_Fetch(l, "body");
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case, "case"))
		{
			/* case labels: Cond -- */
			BGBCC_CCXL_StackPushVoid(ctx);
			BGBCC_CCXL_EmitLabel(ctx, cl[i++]);
			BGBCC_CCXL_StackPop(ctx);
			c=BCCX_Next(c);
			continue;
		}

		if(BCCX_TagIsCstP(c, &bgbcc_rcst_case_default, "case_default"))
		{
			/* default label: Cond -- */
			BGBCC_CCXL_StackPushVoid(ctx);
			BGBCC_CCXL_EmitLabel(ctx, dfl);
			BGBCC_CCXL_StackPop(ctx);
			c=BCCX_Next(c);
			continue;
		}

		BGBCC_CCXL_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}
	
	if(fxdfl.id)
	{
		BGBCC_CCXL_StackPushVoid(ctx);
		BGBCC_CCXL_EmitLabel(ctx, fxdfl);
		BGBCC_CCXL_StackPop(ctx);
	}

	BGBCC_CCXL_EmitLabel(ctx, lbrk);
//	BGBCC_CCXL_StackPop(ctx);

	if(cl!=t_cl)
	{
		bgbcc_free(cl);
		bgbcc_free(clv);
	}

	if(cl2 && (cl2!=t_cl2))
	{
		bgbcc_free(cl2);
		bgbcc_free(clv2);
	}

	ctx->breakstackpos--;
	return(0);
}

int BGBCC_CCXL_CompileInitArrayMultiR(BGBCC_TransState *ctx,
	char *name, ccxl_type ty, BCCX_Node *l,
	int *cidx, int nidx)
{
	BCCX_Node *cur;
	ccxl_type bty;
	char *s1;
	int i, idx;

	BGBCC_CCXL_TypeDerefType(ctx, ty, &bty);
	
	s1=NULL;
	if(!BGBCC_CCXL_TypeArrayP(ctx, bty))
		s1=BGBCC_CCXL_TypeGetSig(ctx, bty);

	cur=BCCX_Child(l); idx=0;
	while(cur)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, bty))
		{
			cidx[nidx]=idx;
			BGBCC_CCXL_CompileInitArrayMultiR(ctx,
				name, bty, cur, cidx, nidx+1);
		}else
		{
			BGBCC_CCXL_CompileExpr(ctx, cur);
			BGBCC_CCXL_StackCastSig(ctx, s1);

			BGBCC_CCXL_PushLoad(ctx, name);
			for(i=0; i<nidx; i++)
				BGBCC_CCXL_StackLoadIndexConst(ctx, cidx[i]);
			BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
		}

		idx++;
		cur=BCCX_Next(cur);
	}

	return(0);
}

int BGBCC_CCXL_CompileInitArrayMulti(BGBCC_TransState *ctx,
	char *name, char *sig, BCCX_Node *l)
{
	int cidx[16];
	BCCX_Node *cur;
	ccxl_type ty, bty;
	char *s1;
	int idx;

	BGBCC_CCXL_TypeFromSig(ctx, &ty, sig);

	BGBCC_CCXL_CompileInitArrayMultiR(ctx, name, ty, l, cidx, 0);
	return(0);

#if 0
	BGBCC_CCXL_TypeDerefType(ctx, ty, &bty);

	s1=BGBCC_CCXL_TypeGetSig(ctx, bty);
	
	cur=BCCX_Child(l); idx=0;
	while(cur)
	{
		BGBCC_CCXL_CompileExpr(ctx, cur);
		BGBCC_CCXL_StackCastSig(ctx, s1);

		BGBCC_CCXL_PushLoad(ctx, name);
		BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
		idx++;

		cur=BCCX_Next(cur);
	}
#endif

	return(0);
}

int BGBCC_CCXL_CompileInitArray(BGBCC_TransState *ctx,
	char *name, char *sig, BCCX_Node *l)
{
	BCCX_Node *cur;
	ccxl_type ty, bty;
	char *s1;
	int idx;

	BGBCC_CCXL_TypeFromSig(ctx, &ty, sig);
	BGBCC_CCXL_TypeDerefType(ctx, ty, &bty);

	if(BGBCC_CCXL_TypeArrayP(ctx, bty))
	{
		BGBCC_CCXL_CompileInitArrayMulti(ctx, name, sig, l);
		return(false);
	}

	s1=BGBCC_CCXL_TypeGetSig(ctx, bty);
	
	cur=BCCX_Child(l); idx=0;
	while(cur)
	{
		BGBCC_CCXL_CompileExpr(ctx, cur);
		BGBCC_CCXL_StackCastSig(ctx, s1);

		BGBCC_CCXL_PushLoad(ctx, name);
		BGBCC_CCXL_StackStoreIndexConst(ctx, idx);
		idx++;

		cur=BCCX_Next(cur);
	}

	return(0);
}

void BGBCC_CCXL_CompileInitVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *type, BCCX_Node *value)
{
	char *s0;
	BCCX_Node *t;
	s64 li;
	int i, j, k;

	li=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");
	if(li&BGBCC_TYFL_STATIC)
		return;

	if(value)
	{
		s0=BGBCC_CCXL_VarTypeString(ctx, type);
		if(*s0=='A')
		{
			BGBCC_CCXL_StackInitVar(ctx, name);
			BGBCC_CCXL_CompileInitArray(ctx, name, s0, value);
			return;
		}

		t=BGBCC_CCXL_ReduceExpr(ctx, value);

//		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_CompileExprAsType(ctx, type, t);
		BGBCC_CCXL_StackInitVarValue(ctx, name);
	}else
	{
		BGBCC_CCXL_StackInitVar(ctx, name);
	}
}

void BGBCC_CCXL_CompileStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *ct, *cv, *t, *n, *u, *v, *n1;
	BCCX_Node *ln, *rn, *ln2, *rn2;
	BCCX_Node *ni, *nc, *ns, *nb;
	ccxl_label l0, l1, l2;
	ccxl_type bty, dty, sty, tty, lty, rty;
	char *s0, *s1, *s2;
	int i0, i1, i2, i3;
	int i, j, k;

	if(!l)
	{
		return;
	}

	s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s0 && (s0!=ctx->lfn))
	{
		if(!ctx->reqlfn)
			{ BGBCC_CCXL_StackFn(ctx, s0); }
		ctx->lfn=s0;
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if((i>0) && (i!=ctx->lln))
	{
		if(!ctx->reqlfn)
			{ BGBCC_CCXL_StackLn(ctx, i); }
		ctx->lln=i;
	}

	if(ctx->reqlfn)
	{
		BGBCC_CCXL_StackFn(ctx, ctx->lfn);
		BGBCC_CCXL_StackLn(ctx, ctx->lln);
		ctx->reqlfn=0;
	}

	l=BGBCC_CCXL_ReduceExpr(ctx, l);
	if(!l)
	{
		printf("BGBCC_CCXL_CompileStatement: Statement Reduced Away\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_switch, "switch"))
	{
		BGBCC_CCXL_CompileSwitch(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_goto, "goto"))
	{
		BGBCC_CCXL_CompileJmp(ctx,
			BGBCC_CCXL_LabelFromName(ctx,
				BCCX_GetCst(l, &bgbcc_rcst_name, "name")));
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_label, "label"))
	{
		BGBCC_CCXL_EmitLabel(ctx,
			BGBCC_CCXL_LabelFromName(ctx,
				BCCX_GetCst(l, &bgbcc_rcst_name, "name")));
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_continue, "continue"))
	{
		l0=ctx->contstack[ctx->contstackpos-1];
		BGBCC_CCXL_CompileJmp(ctx, l0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_break, "break"))
	{
		l0=ctx->breakstack[ctx->breakstackpos-1];
		BGBCC_CCXL_CompileJmp(ctx, l0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return"))
	{
		t=BCCX_Fetch(l, "value");
		if(!t)
		{
			BGBCC_CCXL_StackRetV(ctx);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackRet(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign"))
	{
		ln=BCCX_Fetch(l, "left");
		rn=BCCX_Fetch(l, "right");
		ln=BGBCC_CCXL_ReduceExpr(ctx, ln);
		rn=BGBCC_CCXL_ReduceExpr(ctx, rn);
		BGBCC_CCXL_InferExpr(ctx, ln, &lty);
		BGBCC_CCXL_InferExpr(ctx, rn, &rty);

		s0=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(s0)
		{
			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref"))
			{
				s1=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_CompileExpr(ctx, ln);
				BGBCC_CCXL_CompileExpr(ctx, rn);
				BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
				return;
			}

			BGBCC_CCXL_CompileExpr(ctx, ln);
			BGBCC_CCXL_CompileExpr(ctx, rn);
			BGBCC_CCXL_StackBinaryOp(ctx, s0);
			BGBCC_CCXL_CompileAssign(ctx, ln);
			return;
		}

		if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_binary, "binary"))
		{
			s0=BCCX_GetCst(rn, &bgbcc_rcst_op, "op");
			s1=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");

			if(strcmp(s0, "&&") && strcmp(s0, "||"))
			{
				ln2=BCCX_Fetch(rn, "left");
				rn2=BCCX_Fetch(rn, "right");
				ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
				rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);
				BGBCC_CCXL_CompileExpr(ctx, ln2);
				BGBCC_CCXL_CompileExpr(ctx, rn2);
				BGBCC_CCXL_StackBinaryOpStore(ctx, s0, s1);
				return;
			}
		}

//		if(BCCX_TagIsCstP(rn, &bgbcc_rcst_ref, "ref"))
//		{
//			s0=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");
//			if(!strcmp(s0, "cshift_empty"))
//			{
//				k=0;
//			}
//		}

		if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
			s1=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_MovLoadStore(ctx, s0, s1);
			return;
		}

#if 1
		if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_funcall, "funcall"))
		{
			s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_CompileFuncallStore(ctx, rn, s0);
			return;
		}
#endif

#if 1
		if(	BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
			BCCX_TagIsCstP(rn, &bgbcc_rcst_unary, "unary") &&
			!(	BCCX_AttrIsCstP(rn, &bgbcc_rcst_op, "op", "*") ||
				BCCX_AttrIsCstP(rn, &bgbcc_rcst_op, "op", "&")))
		{
			s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
			s1=BCCX_GetCst(rn, &bgbcc_rcst_op, "op");
			v=BCCX_Fetch(rn, "value");
			
			if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
			{
				s2=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
				if(!strcmp(s2, s0))
				{
					BGBCC_CCXL_StackUnaryOpName(ctx, s1, s0);
					return;
				}
			}
			
			BGBCC_CCXL_CompileExpr(ctx, v);
			BGBCC_CCXL_StackUnaryOpStore(ctx, s1, s0);
			return;
		}
#endif

		if(BGBCC_CCXL_TypeCompatibleP(ctx, lty, rty))
		{
			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_objref, "objref"))
			{
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(rn, "value"));
				s1=BCCX_GetCst(rn, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_StackLoadSlotStore(ctx, s1, s0);
				return;
			}

#if 1
			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_getindex, "getindex"))
			{
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				ln2=BCCX_Fetch(rn, "array");
				rn2=BCCX_Fetch(rn, "index");
//				ln2=BGBCC_CCXL_ReduceExpr(ctx, ln2);
				rn2=BGBCC_CCXL_ReduceExpr(ctx, rn2);

				if(BGBCC_CCXL_IsIntP(ctx, rn2))
				{
					i=BCCX_GetIntCst(rn2, &bgbcc_rcst_value, "value");
					BGBCC_CCXL_CompileExpr(ctx, ln2);
					BGBCC_CCXL_StackLoadIndexConstStore(ctx, i, s0);
				}else
				{
					BGBCC_CCXL_CompileExpr(ctx, ln2);
					BGBCC_CCXL_CompileExpr(ctx, rn2);
					BGBCC_CCXL_StackLoadIndexStore(ctx, s0);
				}
				return;
			}
#endif

			if(BCCX_TagIsCstP(ln, &bgbcc_rcst_ref, "ref") &&
				BCCX_TagIsCstP(rn, &bgbcc_rcst_cast, "cast"))
			{
				s0=BCCX_GetCst(ln, &bgbcc_rcst_name, "name");
				BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(rn, "value"));
				s1=BGBCC_CCXL_VarTypeString(ctx, BCCX_FindTag(rn, "type"));
				BGBCC_CCXL_StackCastSigStore(ctx, s1, s0);
				return;
			}
		}

		BGBCC_CCXL_CompileExpr(ctx, rn);
		BGBCC_CCXL_CompileAssign(ctx, ln);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_preinc, "preinc") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postinc, "postinc"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpName(ctx, "++", s0);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "++");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_predec, "predec") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postdec, "postdec"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "expr"));

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s0=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			BGBCC_CCXL_StackUnaryOpName(ctx, "--", s0);
			return;
		}

		BGBCC_CCXL_CompileExpr(ctx, t);
		BGBCC_CCXL_StackUnaryOp(ctx, "--");
		BGBCC_CCXL_CompileAssign(ctx, t);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))	{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
				{ c=BCCX_Next(c); continue; }

			s0=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			t=BCCX_FindTag(c, "type");
			v=BCCX_Fetch(c, "value");

			BGBCC_CCXL_CompileInitVar(ctx, s0, t, v);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==1)
		{
			BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "then"));
			return;
		}
		if(i==0)
		{
			t=BCCX_Fetch(l, "else");
			if(t)BGBCC_CCXL_CompileStatement(ctx, t);
			return;
		}

		l0=BGBCC_CCXL_GenSym(ctx);
		BGBCC_CCXL_CompileJCF(ctx, t, l0);

		t=BCCX_Fetch(l, "else");
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "then"));
		if(t)
		{
			l1=BGBCC_CCXL_GenSym(ctx);
			BGBCC_CCXL_CompileJmp(ctx, l1);
			BGBCC_CCXL_EmitLabel(ctx, l0);
			BGBCC_CCXL_CompileStatement(ctx, t);
			l0=l1;
		}
		BGBCC_CCXL_EmitLabel(ctx, l0);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			BGBCC_CCXL_CompileStatement(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for"))
	{
		ni=BCCX_Fetch(l, "init");
		nc=BCCX_Fetch(l, "cond");
		nb=BCCX_Fetch(l, "body");
		ns=BCCX_Fetch(l, "step");

#if 0
		i=1;
		i=i&&BGBCC_CCXL_IsFixIntAssignRVP(ctx, ni, &s0, &i0);
		i=i&&BGBCC_CCXL_IsFixIntCompareRVP(ctx, nc, &s1, &s2, &i1);
		i=i&&(	BGBCC_CCXL_IsTagVarRVP(ctx, ns, "postinc", s0) ||
				BGBCC_CCXL_IsTagVarRVP(ctx, ns, "preinc", s0));
		i=i&&(!strcmp(s0, s1));
		i=i&&(!strcmp(s2, "<") || !strcmp(s2, "<="));
		i=i&&(	BCCX_TagIsCstP(nb, &bgbcc_rcst_assign, "assign") ||
				BCCX_TagIsCstP(nb, &bgbcc_rcst_funcall, "funcall") ||
				BCCX_TagIsCstP(nb, &bgbcc_rcst_setindex, "setindex"));
		
		if(i && ((i1-i0)<=8))
		{
			if(!strcmp(s2, "<="))i1++;
			for(i=i0; i<i1; i++)
			{
				BGBCC_CCXL_StackPushConstInt(ctx, i);
				BGBCC_CCXL_PopStore(ctx, s0);
				BGBCC_CCXL_CompileStatement(ctx, nb);
			}

			return;
		}
#endif

		l0=BGBCC_CCXL_GenSym(ctx);
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);

		ctx->contstack[ctx->contstackpos++]=l1;
		ctx->breakstack[ctx->breakstackpos++]=l2;

		if(ni)
			{ BGBCC_CCXL_CompileStatement(ctx, ni); }

		BGBCC_CCXL_EmitLabel(ctx, l0);
		if(nc)
			{ BGBCC_CCXL_CompileJCF(ctx, nc, l2); }

		BGBCC_CCXL_CompileStatement(ctx, nb);

		BGBCC_CCXL_EmitLabel(ctx, l1);
		if(ns)
			{ BGBCC_CCXL_CompileStatement(ctx, ns); }
		BGBCC_CCXL_CompileJmp(ctx, l0);

		BGBCC_CCXL_EmitLabel(ctx, l2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);

		ctx->contstack[ctx->contstackpos++]=l1;
		ctx->breakstack[ctx->breakstackpos++]=l2;

		BGBCC_CCXL_EmitLabel(ctx, l1);

		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);

		if(i==0)return;

		if(i==1)
		{
			BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));
			BGBCC_CCXL_CompileContinue(ctx);
			BGBCC_CCXL_EmitLabel(ctx, l2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_CCXL_CompileJCF(ctx, t, l2);
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));
		BGBCC_CCXL_CompileContinue(ctx);
		BGBCC_CCXL_EmitLabel(ctx, l2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}


	if(BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while"))
	{
		l1=BGBCC_CCXL_GenSym(ctx);
		l2=BGBCC_CCXL_GenSym(ctx);
		ctx->contstack[ctx->contstackpos++]=l1;
		ctx->breakstack[ctx->breakstackpos++]=l2;

		BGBCC_CCXL_EmitLabel(ctx, l1);
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		t=BGBCC_CCXL_ReduceExpr(ctx, BCCX_Fetch(l, "cond"));
		i=BGBCC_CCXL_BoolExpr(ctx, t);
		if(i==0)return;

		if(i==1)
		{
			BGBCC_CCXL_CompileContinue(ctx);
			BGBCC_CCXL_EmitLabel(ctx, l2);

			ctx->contstackpos--;
			ctx->breakstackpos--;
			return;
		}

		BGBCC_CCXL_CompileJCT(ctx, t, l1);
		BGBCC_CCXL_EmitLabel(ctx, l2);

		ctx->contstackpos--;
		ctx->breakstackpos--;
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
	{
		BGBCC_CCXL_CompileFuncallStmt(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_try_catch, "try_catch"))
	{
		//lame: for now just spit out try block...
		BGBCC_CCXL_CompileStatement(ctx, BCCX_Fetch(l, "body"));

		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_msvc_asm, "msvc_asm"))
	{
		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_gcc_asm, "gcc_asm"))
	{
		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast"))
	{
		BGBCC_CCXL_CompileExpr(ctx, BCCX_Fetch(l, "value"));
		BGBCC_CCXL_StackPop(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_asm_blob, "asm_blob"))
	{
		c=BCCX_Child(l);
		s0=BCCX_Text(c);

		BGBCC_CCXL_AddAsmBlob(ctx, s0);
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_linecomment, "linecomment")) { return; }
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_blockcomment, "blockcomment")) { return; }

	BGBCC_CCXL_CompileExpr(ctx, l);
	BGBCC_CCXL_StackPop(ctx);
}


char *BGBCC_CCXL_VarTypeString_FlattenExpr(BGBCC_TransState *ctx,
	char *dst, BCCX_Node *l, int fl)
{
	BCCX_Node *c, *n;
	char *s, *t;
	
	t=dst;
	
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BGBCC_CCXL_VarTypeString_FlattenName(ctx, dst, s, fl);
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_emptyindex, "emptyindex"))
	{
		*t++='Q';
		n=BCCX_Fetch(l, "array");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_sharpcall, "sharpcall"))
	{
		*t++='<';
		c=BCCX_Fetch(l, "args");
		while(c)
		{
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, c, fl);
			c=BCCX_Next(c);
		}
		*t++='>';
		n=BCCX_Fetch(l, "value");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_anytype, "anytype"))
	{
		*t++='C';
		*t++='z';
		return(t);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_unary, "unary"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_op, "op");

		if(!strcmp(s, "..."))
		{
			*t++='C';
			*t++='z';
			return(t);
		}

		if(!strcmp(s, "*"))*t++='P';
		if(!strcmp(s, "&"))*t++='R';
		if(!strcmp(s, "^"))*t++='P';

		n=BCCX_Fetch(l, "value");
		t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, fl);
		return(t);
	}
	
	return(t);
}

char *BGBCC_CCXL_VarTypeString_FlattenName(BGBCC_TransState *ctx,
	char *t, char *s, int fl)
{
	BCCX_Node *c, *n;
	char *t1;
	int lang;
	int i, j, k;

	t1=t;

	if(ctx)lang=ctx->lang;
		else lang=BGBCC_LANG_C;

	if((lang==BGBCC_LANG_C) || (lang==BGBCC_LANG_CPP))
	{
		if(fl&BGBCC_TYFL_UNSIGNED)
		{
			if(!strcmp(s, "void"))*t++='v';
			if(!strcmp(s, "char"))*t++='h';
			if(!strcmp(s, "short"))*t++='t';
			if(!strcmp(s, "int"))*t++='j';
			if(!strcmp(s, "long"))*t++='m';
			if(!strcmp(s, "llong"))*t++='y';
			if(!strcmp(s, "int128"))*t++='o';

			if(!strcmp(s, "float"))
				{ *t++='C'; *t++='f'; }
			if(!strcmp(s, "double"))
				{ *t++='C'; *t++='d'; }
			if(!strcmp(s, "float128"))
				{ *t++='C'; *t++='n'; }
			if(!strcmp(s, "float16"))
				{ *t++='C'; *t++='k'; }
		}
	}

	if(t==t1)
	{
		if(!strcmp(s, "void"))*t++='v';
		if(!strcmp(s, "short"))*t++='s';
		if(!strcmp(s, "int"))*t++='i';
		if(!strcmp(s, "float"))*t++='f';
		if(!strcmp(s, "double"))*t++='d';

		if((lang==BGBCC_LANG_C) || (lang==BGBCC_LANG_CPP))
		{
			if(!strcmp(s, "char"))*t++='c';
			if(!strcmp(s, "long"))*t++='l';
			if(!strcmp(s, "llong"))*t++='x';
			if(!strcmp(s, "bool"))*t++='b';
		}else if((lang==BGBCC_LANG_CS) ||
			(lang==BGBCC_LANG_BS2))
		{
			if(!strcmp(s, "char"))*t++='w';
			if(!strcmp(s, "object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "string"))
				{ *t++='C'; *t++='s'; }

			if(!strcmp(s, "bool"))*t++='b';

			if(!strcmp(s, "sbyte"))*t++='c';
			if(!strcmp(s, "byte"))*t++='h';
			if(!strcmp(s, "ushort"))*t++='t';
			if(!strcmp(s, "uint"))*t++='j';

			if(!strcmp(s, "long"))*t++='x';
			if(!strcmp(s, "ulong"))*t++='y';

			if(!strcmp(s, "decimal"))*t++='g';
		}else if(lang==BGBCC_LANG_JAVA)
		{
			if(!strcmp(s, "char"))*t++='w';
			if(!strcmp(s, "long"))*t++='l';

			if(!strcmp(s, "byte"))*t++='c';
			if(!strcmp(s, "boolean"))*t++='b';

			if(!strcmp(s, "Object"))
				{ *t++='C'; *t++='o'; }
			if(!strcmp(s, "String"))
				{ *t++='C'; *t++='t'; }
		}else
		{
			if(!strcmp(s, "char"))*t++='c';
			if(!strcmp(s, "long"))*t++='l';
			if(!strcmp(s, "llong"))*t++='x';
		}

		if(!strcmp(s, "_Bool"))*t++='b';

		if(!strcmp(s, "int64"))*t++='x';
		if(!strcmp(s, "uint64"))*t++='y';

		if(!strcmp(s, "int128"))*t++='n';
		if(!strcmp(s, "float128"))*t++='g';
		if(!strcmp(s, "float16"))*t++='k';

		if(!strcmp(s, "uint128"))*t++='o';

		if(!strcmp(s, "variant"))*t++='r';
		if(!strcmp(s, "variantf"))
			{ *t++='C'; *t++='r'; }

		if(!strcmp(s, "variant_this"))
			{ *t++='C'; *t++='x'; }
		if(!strcmp(s, "variant_cls"))
			{ *t++='C'; *t++='y'; }
		if(!strcmp(s, "variant_va"))
			{ *t++='C'; *t++='z'; }

		if(!strcmp(s, "builtin_va_list") ||
				!strcmp(s, "gnuc_va_list"))
			{ *t++='D'; *t++='z'; }

		if(!strcmp(s, "vec2"))
			{ *t++='C'; *t++='a'; }
		if(!strcmp(s, "vec3"))
			{ *t++='C'; *t++='b'; }
		if(!strcmp(s, "vec4"))
			{ *t++='C'; *t++='c'; }
		if(!strcmp(s, "quat"))
			{ *t++='C'; *t++='q'; }

		if(!strcmp(s, "vec2d"))
			{ *t++='D'; *t++='a'; }
		if(!strcmp(s, "vec3d"))
			{ *t++='D'; *t++='b'; }
		if(!strcmp(s, "vec4d"))
			{ *t++='D'; *t++='c'; }
		if(!strcmp(s, "quatd"))
			{ *t++='D'; *t++='q'; }
		*t=0;
	}

	if(ctx && (t==t1))
	{
		c=ctx->structs;
		while(c)
		{
			if(BCCX_AttrIsCstP(c, &bgbcc_rcst_name, "name", s))
			{
				if(BCCX_TagIsCstP(c, &bgbcc_rcst_enum, "enum"))
				{
					*t++='i';
					*t=0;
					break;
				}

				if((ctx->ctxflags&BGBCC_CTXFL_SAVENAMES) ||
					(fl&BGBCC_TYFL_EXPNAME) || ctx->ril_ip)
				{
					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
					}

					sprintf(t, "X%s;", s);
					t+=strlen(t);
				}else
				{
					i=BGBCC_CCXL_LookupStructureID(ctx, s);
					if(i<0)
					{
						i=BGBCC_CCXL_GetStructID(ctx, s);
					}
					if(i>0)
					{
						sprintf(t, "X%d", i);
						t+=strlen(t);
						break;
					}

					sprintf(t, "X%s;", s);
					t+=strlen(t);
				}
				break;
			}
			c=BCCX_Next(c);
		}
	}

	if(t==t1)
	{
		sprintf(t, "U%s;", s);
		t+=strlen(t);
	}
		
	return(t);
}

int BGBCC_CCXL_VarTypeString_ModifierChar(BGBCC_TransState *ctx, s64 i)
{
	int c;

	if(!i)return(0);

	if(ctx->arch==BGBCC_ARCH_X86)
	{
		switch(i)
		{
		case BGBCC_TYFL_CDECL: c='c'; break;
		case BGBCC_TYFL_STDCALL: c='w'; break;
		default: c=0; break;
		}
		if(c)return(c);
	}
	if(ctx->arch==BGBCC_ARCH_X64)
	{
		switch(i)
		{
		case BGBCC_TYFL_WIN64: c='w'; break;
		case BGBCC_TYFL_AMD64: c='f'; break;
		default: c=0; break;
		}
		if(c)return(c);
	}

	switch(i)
	{
	case BGBCC_TYFL_ABSTRACT: c='a'; break;
	case BGBCC_TYFL_BIGENDIAN: c='b'; break;
	case BGBCC_TYFL_CDECL: c='c'; break;
	case BGBCC_TYFL_DELEGATE: c='d'; break;
//	case BGBCC_TYFL_EVENT: c='e'; break;
	case BGBCC_TYFL_EXTERN: c='e'; break;
	//f (fastcall/amd64)
	case BGBCC_TYFL_GETTER: c='g'; break;
	case BGBCC_TYFL_SETTER: c='h'; break;
	case BGBCC_TYFL_INLINE: c='i'; break;
	case BGBCC_TYFL_FINAL: c='j'; break;
	case BGBCC_TYFL_CONST: c='k'; break;
	case BGBCC_TYFL_LTLENDIAN: c='l'; break;
	case BGBCC_TYFL_PACKED: c='m'; break;
	case BGBCC_TYFL_NATIVE: c='n'; break;
	case BGBCC_TYFL_OVERRIDE: c='o'; break;
	case BGBCC_TYFL_PUBLIC: c='p'; break;
	case BGBCC_TYFL_PRIVATE: c='q'; break;
	case BGBCC_TYFL_PROTECTED: c='r'; break;
	case BGBCC_TYFL_STATIC: c='s'; break;
	//t (thiscall)
	//u (special)
	case BGBCC_TYFL_VIRTUAL: c='v'; break;
	case BGBCC_TYFL_STDCALL: c='w'; break;
	case BGBCC_TYFL_XCALL: c='x'; break;
	case BGBCC_TYFL_TRANSIENT: c='y'; break;
	//z
//	case BGBCC_TYFL_VOLATILE: c='z'; break;

//	case BGBCC_TYFL_TRANSIENT: c=('C'<<8)|'t'; break;

//	case BGBCC_TYFL_ASYNC: c=('C'<<8)|'a'; break;
	case BGBCC_TYFL_BYREF: c=('C'<<8)|'b'; break;

	case BGBCC_TYFL_EVENT: c=('C'<<8)|'e'; break;

	case BGBCC_TYFL_INTERFACE: c=('C'<<8)|'i'; break;

	case BGBCC_TYFL_SYNCHRONIZED: c=('C'<<8)|'s'; break;

	case BGBCC_TYFL_DLLEXPORT: c=('D'<<8)|'e'; break;
	case BGBCC_TYFL_DLLIMPORT: c=('D'<<8)|'i'; break;

	case BGBCC_TYFL_THREAD: c=('D'<<8)|'t'; break;
	default: c=0; break;
	}
	return(c);
}

char *BGBCC_CCXL_VarTypeString_FlattenModifiers(BGBCC_TransState *ctx,
	char *t, s64 fl)
{
	int i, j, k;

	k=0;
//	for(i=0; i<30; i++)
	for(i=0; i<60; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)k++;
	}
	
	if(k>2)
	{
		*t++='M';
//		for(i=0; i<31; i++)
		for(i=0; i<60; i++)
		{
			j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
			if(j)
			{
				if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
				if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
				if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
				if(j&0xFF)*t++=j&0xFF;
			}
		}
		*t++=';';
		
		return(t);
	}

//	for(i=0; i<31; i++)
	for(i=0; i<60; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)
		{
			*t++='F';
			if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
			if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
			if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
			if(j&0xFF)*t++=j&0xFF;
		}
	}
	return(t);
}

char *BGBCC_CCXL_VarTypeString_FlattenModifiers2(BGBCC_TransState *ctx,
	char *t, s64 fl)
{
	int i, j, k;

//	for(i=0; i<31; i++)
	for(i=0; i<60; i++)
	{
		j=BGBCC_CCXL_VarTypeString_ModifierChar(ctx, fl&(1<<i));
		if(j)
		{
			if((j>>24)&0xFF)*t++=(j>>24)&0xFF;
			if((j>>16)&0xFF)*t++=(j>>16)&0xFF;
			if((j>> 8)&0xFF)*t++=(j>> 8)&0xFF;
			if(j&0xFF)*t++=j&0xFF;
		}
	}
	return(t);
}

char *BGBCC_CCXL_VarTypeString(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i, j, k;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)
		{ return(bgbcc_strdup("v")); }

	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		{ return(bgbcc_strdup("v")); }

	if(ty)
	{
#if 1
		c=BCCX_Fetch(ty, "size");
		if(c)
		{
			*t++='A';
			while(c)
			{
				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
				{
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_if, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
					BGBCC_DBGBREAK
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
				if(i<0)
				{
					BGBCC_CCXL_Warn(ctx,
						"Negative Size Array\n");
					*t++='1'; c=BCCX_Next(c); continue;
				}
				
//				if(ctx->cur_func && ctx->cur_func->name &&
//					!strcmp(ctx->cur_func->name, "D_PolysetDraw"))
//						{ k=-1; BGBCC_DBGBREAK }
				
				sprintf(t, "%d", i);
				t+=strlen(t);

				c=BCCX_Next(c);
				if(c)*t++=',';
			}
			*t++=';';
		}
#endif

		i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		s=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		if(i&BGBCC_TYFL_WIDE)
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='W';
		}else
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='P';
		}

		i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");

		n=BCCX_Fetch(ty, "expr");
		if(n)
		{
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(
				ctx, t, n, i|BGBCC_TYFL_EXPNAME);
		}else
		{
			if(s)
			{
				t=BGBCC_CCXL_VarTypeString_FlattenName(
					ctx, t, s, i|BGBCC_TYFL_EXPNAME);
			}else
			{
				*t++='v';
			}
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_CCXL_VarImageTypeString(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)
		{ return(bgbcc_strdup("v")); }

	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		{ return(bgbcc_strdup("v")); }

	if(ty)
	{
#if 1
		c=BCCX_Fetch(ty, "size");
		if(c)
		{
			*t++='A';
			while(c)
			{
				n=BGBCC_CCXL_ReduceExprConst(ctx, c);

				if(!BCCX_TagIsCstP(n, &bgbcc_rcst_int, "int"))
				{
					if(BCCX_TagIsCstP(n, &bgbcc_rcst_if, "if"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Conditional array-size Hack\n");
						*t++='0'; c=BCCX_Next(c); continue;
					}

					if(BCCX_TagIsCstP(n, &bgbcc_rcst_sizeof, "sizeof"))
					{
						BGBCC_CCXL_Warn(ctx,
							"Sizeof array-size Hack\n");
						*t++='1024'; c=BCCX_Next(c); continue;
					}

					BGBCC_CCXL_Error(ctx, "Invalid array size specifier\n");
					*t++='0'; c=BCCX_Next(c); continue;
				}

				i=BCCX_GetIntCst(n, &bgbcc_rcst_value, "value");
				sprintf(t, "%d", i);
				t+=strlen(t);

				c=BCCX_Next(c);
				if(c)*t++=',';
			}
		}
#endif

		i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		s=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		if(i&BGBCC_TYFL_WIDE)
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='W';
		}else
		{
			i=BCCX_GetIntCst(ty, &bgbcc_rcst_ind, "ind");
			while(i--)*t++='P';
		}

		i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");

		n=BCCX_Fetch(ty, "expr");
		if(n)
		{
			t=BGBCC_CCXL_VarTypeString_FlattenExpr(ctx, t, n, i);
		}else
		{
			if(s)
			{
				t=BGBCC_CCXL_VarTypeString_FlattenName(ctx, t, s, i);
			}else
			{
				*t++='v';
			}
		}
	}

	*t=0;

	return(bgbcc_strdup(buf));
}

char *BGBCC_CCXL_VarTypeFlagsString(BGBCC_TransState *ctx, BCCX_Node *ty)
{
	char buf[256];
	char *s, *t, *t1;
	BCCX_Node *c, *n;
	int i;

	if(!ty)return(NULL);

	t=buf;
	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))ty=BCCX_Fetch(ty, "type");
	if(!ty)return(bgbcc_strdup(""));

	if(!BCCX_TagIsCstP(ty, &bgbcc_rcst_type, "type"))
		return(bgbcc_strdup(""));

	if(ty)
	{
		i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
		s=BCCX_GetCst(ty, &bgbcc_rcst_name, "name");

		t=BGBCC_CCXL_VarTypeString_FlattenModifiers2(ctx, t, i);
	}
	*t=0;

	return(bgbcc_strdup(buf));
}

void BGBCC_CCXL_EmitVarFunc(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *args)
{
	char *s, *s1, *s2;
	s64 li;
	int i;

//	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	s=BGBCC_CCXL_VarSigProto(ctx, ty, name, args);
	s1=name;
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";

	if(!s1)s1="_";

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	li=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
	
	if(BGBCC_CCXL_GetNodeAttribute(ctx, ty, "dllexport") ||
		!strcmp(s1, "main") || !strcmp(s1, "WinMain"))
	{
		li|=BGBCC_TYFL_EXPNAME|BGBCC_TYFL_DLLEXPORT;
	}
	
	BGBCC_CCXL_AttribLong(ctx, CCXL_ATTR_FLAGS, li);
}

void BGBCC_CCXL_EmitVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	int i;

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s=BGBCC_CCXL_VarImageTypeString(ctx, ty);
	s1=name;
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

	if(!s)s="v";

	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	if(v)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_EmitVarValueR2(ctx, v);
		BGBCC_CCXL_End(ctx);
	}

	BGBCC_CCXL_End(ctx);
}

void BGBCC_CCXL_EmitVar2(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char tb[256];
	char *s, *s1, *s2;
	int i, op;

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s=BGBCC_CCXL_VarImageTypeString(ctx, ty);
	s1=name;

	if(!s)s="v";

	i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
	if(!s1)s1="_";

	if(i&BGBCC_TYFL_EXTERN)
		return;

	op=CCXL_CMD_VARDECL;
	if(i&BGBCC_TYFL_STATIC)
		op=CCXL_CMD_STATICVARDECL;

	BGBCC_CCXL_BeginName(ctx, op, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);

	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	if(ctx->cur_structdef &&
		BCCX_TagIsCstP(ctx->cur_structdef,
			&bgbcc_rcst_enum, "enum"))
	{
		if(v)
		{
			ctx->cur_idx4=BCCX_GetIntCst(v, &bgbcc_rcst_value, "value");
		}

		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
			ctx->cur_idx4++);
		BGBCC_CCXL_End(ctx);
	}else
	{
		if(v)
		{
			BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
			BGBCC_CCXL_EmitVarValueR2(ctx, v);
			BGBCC_CCXL_End(ctx);
		}
	}

	BGBCC_CCXL_End(ctx);

	if(ctx->cur_struct)
	{
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);

//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"field", ctx->cur_idx++, s1);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s1, s);
//		BGBCC_CCXL_BindFieldInfoI(ctx, ctx->cur_struct, s1, "flags", i);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindFieldInfo(ctx, ctx->cur_struct, s1, "flagstr", s2);
	}
}

void BGBCC_CCXL_EmitVar3(BGBCC_TransState *ctx, char *name, char *ty)
{
	char *s, *s1;
	int i;

	if(BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, name))
	{
		return;
	}

	s=ty;
	s1=name;

	if(!s)s="v";
	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_End(ctx);

	if(ctx->cur_struct)
	{
//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"method", ctx->cur_idx2++, s1);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s1, s);
	}
}

void BGBCC_CCXL_EmitVarProperty(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	t=BCCX_FindTag(l, "type");

	n=BCCX_New1("var", t);
	BCCX_Set(n, "name", "value");

	v=BCCX_FindTag(l, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_CCXL_GenProtoSig(ctx, t, NULL);
		BGBCC_CCXL_EmitVar3(ctx, s2, s1);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);
		s1=BGBCC_CCXL_GenProtoSig(ctx, NULL, n);
		BGBCC_CCXL_EmitVar3(ctx, s2, s1);
	}

	if(ctx->cur_struct)
	{
		s1=BGBCC_CCXL_VarTypeString(ctx, t);
		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);
		i=BCCX_GetIntCst(t, &bgbcc_rcst_flags, "flags");

//		BGBCC_CCXL_BindStructInfoIdx(ctx, ctx->cur_struct,
//			"property", ctx->cur_idx3++, s);

//		BGBCC_CCXL_BindFieldSig(ctx, ctx->cur_struct, s, s1);
//		BGBCC_CCXL_BindFieldInfoI(ctx, ctx->cur_struct, s, "flags", i);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindFieldInfo(ctx, ctx->cur_struct, s, "flagstr", s2);
	}
}

void BGBCC_CCXL_CompileVarProperty(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t;
	char *s, *s1, *s2;
	int i;

#if 1
	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	t=BCCX_FindTag(l, "type");

	n=BCCX_New1("var", t);
	BCCX_Set(n, "name", "value");

	v=BCCX_FindTag(l, "get");
	if(v)
	{
		sprintf(tb, "get_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_CCXL_CompileBlock(ctx, t, s2, NULL, v);
	}

	v=BCCX_FindTag(l, "set");
	if(v)
	{
		sprintf(tb, "set_%s", s);
		s2=bgbcc_strdup(tb);

		v=BCCX_Fetch(v, "body");
		BGBCC_CCXL_CompileBlock(ctx, NULL, s2, n, v);
	}
#endif

}

int BGBCC_CCXL_CompileVarDummyP(BGBCC_TransState *ctx, BCCX_Node *l)
{
	if(	BCCX_TagIsCstP(l, &bgbcc_rcst_assign, "assign") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_break, "break") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_case, "case") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_case_default, "case_default") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_cast, "cast") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_comma, "comma") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_continue, "continue") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_goto, "goto") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_label, "label") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_methodcall, "methodcall") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_msvc_asm, "msvc_asm") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postdec, "postdec") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_postinc, "postinc") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_predec, "predec") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_preinc, "preinc") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_ref, "ref") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_return, "return") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_switch, "switch") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
		{ return(1); }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_linecomment, "linecomment"))
		{ return(1); }
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_blockcomment, "blockcomment"))
		{ return(1); }

	return(0);
}

void BGBCC_CCXL_CompileVarStatementBlock(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	
	c=l;
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
}

int BGBCC_CCXL_CompileVarStatementCompound(
	BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_begin, "begin"))
	{
		c=BCCX_Child(l);
		BGBCC_CCXL_CompileVarStatementBlock(ctx, c);
 		return(1);
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_if, "if"))
	{
		c=BCCX_Fetch(l, "then");
		BGBCC_CCXL_CompileVarStatement(ctx, c);

		c=BCCX_Fetch(l, "else");
		if(c)
		{
			BGBCC_CCXL_CompileVarStatement(ctx, c);
		}

 		return(1);
	}

	if(
		BCCX_TagIsCstP(l, &bgbcc_rcst_do_while, "do_while") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_for, "for") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_switch, "switch") ||
		BCCX_TagIsCstP(l, &bgbcc_rcst_while, "while"))
	{
		c=BCCX_Fetch(l, "body");
		BGBCC_CCXL_CompileVarStatementBlock(ctx, c);
 		return(1);
	}
	return(0);
}

void BGBCC_CCXL_CompileVarStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t;
	char *s, *s0, *s1;
	int i;

	s=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if(i)ctx->lln=i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
				{ c=BCCX_Next(c); continue; }

			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			t=BCCX_FindTag(c, "type");
			v=BCCX_Fetch(c, "value");
//			v=NULL;

			BGBCC_CCXL_EmitVar2(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
//		s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_CCXL_GenProtoSig(ctx, t, n);
		BGBCC_CCXL_EmitVar3(ctx, s, s1);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_constructor, "constructor"))
	{
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		s1=BGBCC_CCXL_GenProtoSig(ctx, NULL, n);
		BGBCC_CCXL_EmitVar3(ctx, "new", s1);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_property, "property"))
	{
		BGBCC_CCXL_EmitVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CCXL_CompileVarStatementCompound(ctx, l))
		return;

	if(BGBCC_CCXL_CompileVarDummyP(ctx, l))
		return;

	BGBCC_CCXL_Error(ctx, "Unhandled var statement %s\n", BCCX_Tag(l));
}

void BGBCC_CCXL_CompileVarStatement2(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t, *ntl;
	char *s, *s0;
	int i;

	s=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if(i)ctx->lln=i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
				{ c=BCCX_Next(c); continue; }

			s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			t=BCCX_FindTag(c, "type");
			v=BCCX_Fetch(c, "value");

			BGBCC_CCXL_EmitTopVar(ctx, s, t, v);

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
//		s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");
		ntl=BCCX_Fetch(l, "argdecls");

		BGBCC_CCXL_CompileBlock2(ctx, t, s, n, v, ntl);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_constructor, "constructor"))
	{
		s="new";
		t=NULL;
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");

		BGBCC_CCXL_CompileBlock(ctx, t, s, n, v);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_property, "property"))
	{
		BGBCC_CCXL_CompileVarProperty(ctx, l);
		return;
	}

	if(BGBCC_CCXL_CompileVarDummyP(ctx, l))
		return;

	BGBCC_CCXL_Error(ctx, "Unhandled var statement %s\n", BCCX_Tag(l));
}

BCCX_Node *BGBCC_CCXL_ArgDeclsTypeForName(BGBCC_TransState *ctx,
	BCCX_Node *adecl, char *name)
{
	BCCX_Node *c, *t, *n, *u, *v, *c2;
	char *s0, *s1;
	
	if(!adecl || !name)
		return(NULL);
	
	c=adecl;
	while(c)
	{
		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_vars, "vars"))
		{
			c=BCCX_Next(c);
			continue;
		}
		
		c2=BCCX_Child(c);
		while(c2)
		{
			if(!BCCX_TagIsCstP(c2, &bgbcc_rcst_var, "var"))
				{ c2=BCCX_Next(c2); continue; }

			s0=BCCX_Get(c2, "name");
			t=BCCX_FindTag(c2, "type");
//			v=BCCX_Fetch(c2, "value");

			if(!strcmp(s0, name))
				return(t);

//			BGBCC_CCXL_CompileInitVar(ctx, s0, t, v);
			c2=BCCX_Next(c2);
		}

		c=BCCX_Next(c);
	}
	
	return(NULL);
}

BCCX_Node *BGBCC_CCXL_CompileBlock(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body)
{
	return(BGBCC_CCXL_CompileBlock2(ctx, type, name, args, body, NULL));
}

BCCX_Node *BGBCC_CCXL_CompileBlock2(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args, BCCX_Node *body,
	BCCX_Node *adecl)
{
	ccxl_label cname[16], bname[16];

	BCCX_Node *blkn;
	BCCX_Node *c, *t, *n, *u, *t2;
	BCCX_Node *ocf_ty;
	char *ocf_n;
	char *s0, *s1;
	int tk;
	int i, j, k;

//	if(name && !strcmp(name, "D_PolysetDraw"))
//	{
//		k=-1;
//		BGBCC_DBGBREAK
//	}

	i=0;
	if(type)i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");

	name=BGBCC_CCXL_QualifyNameNS(ctx, name);
//	name=BGBCC_CCXL_QualifyNameNSFl(ctx, name, i);

	BGBCC_CCXL_EmitSigProto(ctx, type, name, args);

	if(i&BGBCC_TYFL_EXTERN)
		return(NULL);

	ctx->contstack=cname;
	ctx->breakstack=bname;
	ctx->contstackpos=0;
	ctx->breakstackpos=0;

	ocf_n=ctx->cf_n;
	ocf_ty=ctx->cf_ty;
	ctx->cf_n=name;
	ctx->cf_ty=type;

	tk=0;
	if(BCCX_TagIsCstP(body, &bgbcc_rcst_begin, "begin"))
	{
		tk=BCCX_GetIntCst(body, &bgbcc_rcst_tokens, "tokens");
		body=BCCX_Child(body);
	}

	BGBCC_CCXL_BeginName(ctx, (ctx->cur_struct)?
		CCXL_CMD_METHOD:CCXL_CMD_FUNCTION, ctx->cf_n);

	BGBCC_CCXL_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_SRCTOK, tk);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
	c=args;
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		s0=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		
		if(adecl)
		{
			t2=BGBCC_CCXL_ArgDeclsTypeForName(ctx, adecl, s0);
			if(t2)t=t2;
		}
		
		BGBCC_CCXL_EmitVar(ctx, s0, t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_LOCALS);
	c=body;
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_BODY);
	ctx->reqlfn=1;

//	BGBCC_CCXL_StackFn(ctx, ctx->lfn);
//	BGBCC_CCXL_StackLn(ctx, ctx->lln);

	c=body;
	while(c)
	{
		BGBCC_CCXL_CompileStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);
	BGBCC_CCXL_End(ctx);

	ctx->cf_n=ocf_n;
	ctx->cf_ty=ocf_ty;

	return(NULL);
}

char *BGBCC_CCXL_GenProtoSig(BGBCC_TransState *ctx,
	BCCX_Node *type, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

	c=args; t=tb;
	*t++='(';
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			*t++='z';
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTag(c, "type");
		if(n)
		{
			s=BGBCC_CCXL_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	if(type)
	{
		s=BGBCC_CCXL_VarTypeString(ctx, type);
		if(s) { strcpy(t, s); t+=strlen(t); }
	}else
	{
		*t++='Pv';
	}

	*t++=0;

	return(bgbcc_strdup(tb));
}

void BGBCC_CCXL_EmitSigProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

	if(!type)return;

	i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");
	if(i&BGBCC_TYFL_STATIC)return;

	c=args; t=tb;
	*t++='(';
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			*t++='z';
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTag(c, "type");
		if(n)
		{
			s=BGBCC_CCXL_VarTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_CCXL_VarTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;

//	BGBCC_CCXL_BindSig(ctx, name, tb);

//	s=BGBCC_CCXL_VarTypeFlagsString(ctx, type);
//	if(s && (*s))
//			BGBCC_CCXL_BindVarInfo(ctx, name, "flagstr", s);
}

char *BGBCC_CCXL_VarSigProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	char tb[256];
	char *s, *t;

	BCCX_Node *c, *n;
	int i, j, k;

	if(!type)
		return(NULL);

	i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");

	c=args; t=tb;
	*t++='(';
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			*t++='z';
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		n=BCCX_FindTag(c, "type");
		if(n)
		{
			s=BGBCC_CCXL_VarImageTypeString(ctx, n);
			if(s) { strcpy(t, s); t+=strlen(t); }
		}

		c=BCCX_Next(c);
	}
	*t++=')';

	s=BGBCC_CCXL_VarImageTypeString(ctx, type);
	if(s) { strcpy(t, s); t+=strlen(t); }

	*t++=0;
	
	return(bgbcc_strdup(tb));
}

BCCX_Node *BGBCC_CCXL_CompileProto(BGBCC_TransState *ctx,
	BCCX_Node *type, char *name, BCCX_Node *args)
{
	BCCX_Node *c, *t, *n, *u;
	int i, j, k;

//	i=0;
//	if(type)i=BCCX_GetIntCst(type, &bgbcc_rcst_flags, "flags");

	name=BGBCC_CCXL_QualifyNameNS(ctx, name);
//	name=BGBCC_CCXL_QualifyNameNSFl(ctx, name, i);

	/* avoid redefining the same stuff */
	if(BGBCC_CCXL_LookupGlobal(ctx, name))
		{ return(NULL); }

	BGBCC_CCXL_EmitSigProto(ctx, type, name, args);

	ctx->cf_n=name;
	ctx->cf_ty=type;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_PROTOTYPE, ctx->cf_n);
	BGBCC_CCXL_EmitVarFunc(ctx, ctx->cf_n, ctx->cf_ty, args);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
	c=args;
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}

		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
		{
			c=BCCX_Next(c);
			continue;
		}

		t=BCCX_FindTag(c, "type");
		BGBCC_CCXL_EmitVar(ctx,
			BCCX_GetCst(c, &bgbcc_rcst_name, "name"), t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	BGBCC_CCXL_End(ctx);

	return(NULL);
}

void BGBCC_CCXL_CompileStruct(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s0, *os0, *s1;
	int i, j;

	s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	s=BGBCC_CCXL_QualifyNameNS(ctx, s0);

	/* avoid redefining the same stuff */
	cur=BGBCC_CCXL_LookupStructure(ctx, s);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "struct");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_STRUCT, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}

	BGBCC_CCXL_End(ctx);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CCXL_CompileUnion(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s0, *os0;
	int i, j;

	s0=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	s=BGBCC_CCXL_QualifyNameNS(ctx, s0);

	/* avoid redefining the same stuff */
	cur=BGBCC_CCXL_LookupStructure(ctx, s);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "union");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_UNION, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

void BGBCC_CCXL_CompileClass(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	BCCX_Node *c, *t, *n, *osn;
	char *s, *os0, *qn;
	int i, j, j1, j2;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	qn=BGBCC_CCXL_QualifyNameNS(ctx, s);

	/* avoid redefining the same stuff */
	cur=BGBCC_CCXL_LookupStructure(ctx, qn);
	if(cur)
	{
		if(cur->decl && cur->decl->n_fields)
			return;
	}

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, qn, "class");
//	BGBCC_CCXL_BindStructInfoI(ctx, qn, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=qn;
	j=ctx->cur_idx;
	j1=ctx->cur_idx2;
	j2=ctx->cur_idx3;

	ctx->cur_idx=0;
	ctx->cur_idx2=0;
	ctx->cur_idx3=0;

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_CLASS, qn);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, qn);

	c=BCCX_Fetch(l, "super");
	if(c)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_EXTENDS);
		while(c)
		{
			s=BGBCC_CCXL_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
				BGBCC_CCXL_End(ctx);
			}
			c=BCCX_Next(c);
		}
		BGBCC_CCXL_End(ctx);
	}

	c=BCCX_Fetch(l, "impl");
	if(c)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_IMPLEMENTS);
		while(c)
		{
			s=BGBCC_CCXL_VarTypeString(ctx, c);
			if(s)
			{
				BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s);
				BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
				BGBCC_CCXL_End(ctx);
			}
			c=BCCX_Next(c);
		}
		BGBCC_CCXL_End(ctx);
	}

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement(ctx, c);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileVarStatement2(ctx, c);
		c=BCCX_Next(c);
	}

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
	ctx->cur_idx2=j1;
	ctx->cur_idx3=j2;
}

void BGBCC_CCXL_CompileEnum(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *osn;
	char *s, *s1, *s2, *os0;
	int op;
	int i, j;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	s=BGBCC_CCXL_QualifyNameNS(ctx, s);

	i=BCCX_GetIntCst(l, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindStructSig(ctx, s, "enum");
//	BGBCC_CCXL_BindStructInfoI(ctx, s, "flags", i);

	osn=ctx->cur_structdef;
	os0=ctx->cur_struct;
	ctx->cur_structdef=l;
	ctx->cur_struct=s;
	j=ctx->cur_idx;
	ctx->cur_idx=0;
	ctx->cur_idx4=0;

	c=BCCX_Fetch(l, "defs");
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_def, "def"))
		{
			op=CCXL_CMD_VARDECL;
			j=BCCX_GetIntCst(c, &bgbcc_rcst_value, "value");
			s1=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
			s2="i";

			if(BGBCC_CCXL_CheckDefinedContextName(ctx,
				CCXL_CMD_VARDECL, s1))
			{
				c=BCCX_Next(c);
				continue;
			}

			BGBCC_CCXL_BeginName(ctx, op, s1);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s2);

			s2="k";
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

			BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
			BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, j++);
			BGBCC_CCXL_End(ctx);

			BGBCC_CCXL_End(ctx);
		}
		c=BCCX_Next(c);
	}

	ctx->cur_structdef=osn;
	ctx->cur_struct=os0;
	ctx->cur_idx=j;
}

BCCX_Node *BGBCC_CCXL_CompileSProto(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *t, *n, *u, *a;
	char *s, *s1;

	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_S_PROTOTYPE, s);

	t=BCCX_FindTag(l, "type");
	a=BCCX_Fetch(l, "args");
	BGBCC_CCXL_EmitVarFunc(ctx, s, t, a);

	BGBCC_CCXL_Begin(ctx, CCXL_CMD_ARGS);
	c=a;
	while(c)
	{
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_rest, "rest"))
		{
			BGBCC_CCXL_Marker(ctx, CCXL_CMD_VARARGS);
			c=BCCX_Next(c);
			continue;
		}
		if(!BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{ c=BCCX_Next(c); continue; }

		t=BCCX_FindTag(c, "type");
		s1=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_EmitVar(ctx, s1, t, NULL);
		c=BCCX_Next(c);
	}
	BGBCC_CCXL_End(ctx);
	BGBCC_CCXL_End(ctx);


	s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
	t=BCCX_FindTag(l, "type");
	u=BCCX_Fetch(l, "args");

//	s1=BGBCC_CCXL_GenProtoSig(ctx, t, u);
//	BGBCC_CCXL_BindStructSig(ctx, s, "func");
//	BGBCC_CCXL_BindStructInfo(ctx, s, "sig", s1);

	return(NULL);
}

BCCX_Node *BGBCC_CCXL_GetNodeAttribute(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *cn, *cn1;
	char *s;

	cn=BCCX_Child(l);
	while(cn)
	{
		if(!BCCX_TagIsCstP(cn, &bgbcc_rcst_declspec, "declspec") &&
			!BCCX_TagIsCstP(cn, &bgbcc_rcst_attribute, "attribute"))
				{ cn=BCCX_Next(cn); continue; }

		cn1=BCCX_Child(cn);
		while(cn1)
		{
			if(!BCCX_TagIsCstP(cn1, &bgbcc_rcst_attr, "attr"))
				{ cn1=BCCX_Next(cn1); continue; }

			s=BCCX_Get(cn1, "name");
			if(s && !strcmp(s, name))
				return(cn1);

			cn1=BCCX_Next(cn1);
		}
		cn=BCCX_Next(cn);
	}
	
	return(NULL);
}

char *BGBCC_CCXL_GetNodeAttributeString(BGBCC_TransState *ctx,
	BCCX_Node *l, char *name)
{
	BCCX_Node *attr, *cn;
	char *s;
	
	attr=BGBCC_CCXL_GetNodeAttribute(ctx, l, name);
	if(!attr)return(NULL);

	s=BCCX_Get(attr, "name");

	cn=BCCX_Fetch(attr, "args");
	if(!cn)return(NULL);

	if(!BCCX_TagIsCstP(cn, &bgbcc_rcst_string, "string"))
		return(NULL);
	s=BCCX_Get(cn, "value");
	return(s);
}

void BGBCC_CCXL_CompileTypedef(BGBCC_TransState *ctx, BCCX_Node *l)
{
	BCCX_Node *c, *n, *v, *t, *n1;
	char *s, *s1, *s2;
	int i;

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_var, "var"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTag(l, "type");
		
		n1=BCCX_FindTag(t, "size");
		if(n1)
		{
			s1=BGBCC_CCXL_VarTypeString(ctx, t);
			s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);

			BGBCC_CCXL_BeginName(ctx, CCXL_CMD_TYPEDEF, s);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s1);
			BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);
			BGBCC_CCXL_End(ctx);
		}

#if 0
//		s1=BGBCC_CCXL_VarTypeString(ctx, t);
//		s2=BGBCC_CCXL_VarTypeFlagsString(ctx, t);
//		BGBCC_CCXL_BindTypeSig(ctx, s, s1);
//		if(s2 && (*s2))
//			BGBCC_CCXL_BindTypeInfo(ctx, s, "flagstr", s2);

		s2=BGBCC_CCXL_GetNodeAttributeString(ctx, t, "dyt_name");
		if(s2)
		{
			printf("BGBCC_CCXL_CompileTypedef: DyType %s->%s\n", s, s2);
		
//			BGBCC_CCXL_BindTypeInfo(ctx, s, "dytype", s2);
//			BGBCC_CCXL_BindDyTypeSig(ctx, s2, s1);
		}
#endif
	}

	return;
}

void BGBCC_CCXL_CompileStructs(BGBCC_TransState *ctx)
{
	BCCX_Node *c, *t, *n, *l;
	BGBCC_CCXL_LiteralInfo *litobj;
	int i;

	c=ctx->types;
	while(c)
	{
		BGBCC_CCXL_CompileTypedef(ctx, c);
		c=BCCX_Next(c);
	}

	c=ctx->structs;
	while(c)
	{
		l=c;
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_struct, "struct"))
			BGBCC_CCXL_CompileStruct(ctx, c);
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_union, "union"))
			BGBCC_CCXL_CompileUnion(ctx, c);
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_enum, "enum"))
			BGBCC_CCXL_CompileEnum(ctx, c);
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_s_proto, "s_proto"))
			BGBCC_CCXL_CompileSProto(ctx, c);
		if(BCCX_TagIsCstP(c, &bgbcc_rcst_func, "func"))
			BGBCC_CCXL_CompileSProto(ctx, c);

		c=BCCX_Next(c);
	}

#if 1
	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 0);
	}
#endif
}

void BGBCC_CCXL_EmitVarValue(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BGBCC_CCXL_RegisterInfo *decl;
	BCCX_Node *t;
	ccxl_register rval;
	char *s;
	int i;

	v=BGBCC_CCXL_ReduceExprConst(ctx, v);

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_int, "int"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_tysuf, "tysuf");
		if(s &&
			(!strcmp(s, "L") || !strcmp(s, "LL") ||
			 !strcmp(s, "UL") || !strcmp(s, "ULL") ||
			 !strcmp(s, "LX") || !strcmp(s, "ULX")))
		{
			BGBCC_CCXL_LiteralLong(ctx, CCXL_ATTR_VALUE,
				BCCX_GetIntCst(v, &bgbcc_rcst_value, "value"));
			return;
		}
	
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
			BCCX_GetIntCst(v, &bgbcc_rcst_value, "value"));
		return;
	}
	if(BCCX_TagIsCstP(v, &bgbcc_rcst_real, "real"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_tysuf, "tysuf");
		if(s && !strcmp(s, "F"))
		{
			BGBCC_CCXL_LiteralFloat(ctx, CCXL_ATTR_VALUE,
				BCCX_GetFloatCst(v, &bgbcc_rcst_value, "value"));
			return;
		}

		BGBCC_CCXL_LiteralDouble(ctx, CCXL_ATTR_VALUE,
			BCCX_GetFloatCst(v, &bgbcc_rcst_value, "value"));
		return;
	}
	if(BCCX_TagIsCstP(v, &bgbcc_rcst_string, "string"))
	{
		BGBCC_CCXL_LiteralStr(ctx, CCXL_ATTR_VALUE,
			BCCX_GetCst(v, &bgbcc_rcst_value, "value"));
		return;
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_charstring, "charstring"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_value, "value");
		if(s) { i=BGBCP_ParseChar(&s); }
			else i=0;
		BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE, i);
		return;
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_ref, "ref"))
	{
		s=BCCX_GetCst(v, &bgbcc_rcst_name, "name");
		decl=BGBCC_CCXL_LookupGlobal(ctx, s);
		if(decl)
		{
			rval=decl->value;
			if(BGBCC_CCXL_IsRegImmIntP(ctx, rval))
			{
				BGBCC_CCXL_LiteralInt(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmIntValue(ctx, rval));
				return;
			}
			if(BGBCC_CCXL_IsRegImmLongP(ctx, rval))
			{
				BGBCC_CCXL_LiteralLong(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmLongValue(ctx, rval));
				return;
			}

			if(BGBCC_CCXL_IsRegImmFloatP(ctx, rval))
			{
				BGBCC_CCXL_LiteralFloat(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmFloatValue(ctx, rval));
				return;
			}
			if(BGBCC_CCXL_IsRegImmDoubleP(ctx, rval))
			{
				BGBCC_CCXL_LiteralDouble(ctx, CCXL_ATTR_VALUE,
					BGBCC_CCXL_GetRegImmDoubleValue(ctx, rval));
				return;
			}

			if(decl->regtype==CCXL_LITID_FUNCTION)
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}

			if(decl->sig && (*decl->sig=='A'))
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}

			if(ctx->cf_n)
				return;

			BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: "
				"Bad Reg Literal Type %08X\n", rval.val);
			return;
		}

		if(ctx->cf_n)
			return;

		BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: "
			"Undeclared Var %s\n", s);
		return;
	}

	if(BGBCC_CCXL_IsUnaryP(ctx, v, "&"))
	{
		t=BCCX_Fetch(v, "value");

		if(BCCX_TagIsCstP(t, &bgbcc_rcst_ref, "ref"))
		{
			s=BCCX_GetCst(t, &bgbcc_rcst_name, "name");
			decl=BGBCC_CCXL_LookupGlobal(ctx, s);
			if(decl)
			{
				BGBCC_CCXL_LiteralGlobalAddr(ctx, CCXL_ATTR_VALUE,
					decl->gblid);
				return;
			}
		}
	}

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_cast, "cast"))
	{
		t=BCCX_Fetch(v, "value");
		BGBCC_CCXL_EmitVarValue(ctx, t);
		return;
	}

	if(ctx->cf_n)
		return;

	s=BCCX_Tag(v);
	BGBCC_CCXL_Error(ctx, "BGBCC_CCXL_EmitVarValue: Bad Value %s\n", s);
	return;
}

void BGBCC_CCXL_EmitVarValueR(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_list, "list"))
	{
		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_CCXL_EmitVarValueR(ctx, c);
			c=BCCX_Next(c);
		}
		return;
	}

	BGBCC_CCXL_EmitVarValue(ctx, v);
}

void BGBCC_CCXL_EmitVarValueR2(BGBCC_TransState *ctx, BCCX_Node *v)
{
	BCCX_Node *c;

	if(BCCX_TagIsCstP(v, &bgbcc_rcst_list, "list"))
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_LIST);

		c=BCCX_Child(v);
		while(c)
		{
			BGBCC_CCXL_EmitVarValueR2(ctx, c);
			c=BCCX_Next(c);
		}

		BGBCC_CCXL_End(ctx);
		return;
	}

	BGBCC_CCXL_EmitVarValue(ctx, v);
}

char *BGBCC_CCXL_QualifyNameNS(BGBCC_TransState *ctx, char *name)
{
	return(BGBCC_CCXL_QualifyNameNSFl(ctx, name, 0));
}

char *BGBCC_CCXL_QualifyNameNSFl(BGBCC_TransState *ctx, char *name, s64 fl)
{
	char tb[256];
	char *s;

	if(fl&BGBCC_TYFL_STATIC)
	{
		if(ctx->cur_struct)
		{
			sprintf(tb, "TU%08X/%s/%s", ctx->tuidx, ctx->cur_struct, name);
			s=bgbcc_strdup(tb);
			return(s);
		}

		if(ctx->cur_ns)
		{
			sprintf(tb, "TU%08X/%s/%s", ctx->tuidx, ctx->cur_ns, name);
			s=bgbcc_strdup(tb);
			return(s);
		}

		sprintf(tb, "TU%08X/%s", ctx->tuidx, name);
		s=bgbcc_strdup(tb);
		return(s);
	}

	if(ctx->cur_struct)
	{
		sprintf(tb, "%s/%s", ctx->cur_struct, name);
		s=bgbcc_strdup(tb);
		return(s);
	}

	if(ctx->cur_ns)
	{
		sprintf(tb, "%s/%s", ctx->cur_ns, name);
		s=bgbcc_strdup(tb);
		return(s);
	}

	return(name);
}

void BGBCC_CCXL_EmitTopVar(BGBCC_TransState *ctx,
	char *name, BCCX_Node *ty, BCCX_Node *v)
{
	char *s, *s1, *s2;
	BCCX_Node *c;
	int i, j;

	i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");

	s=BGBCC_CCXL_VarTypeString(ctx, ty);
	s2=BGBCC_CCXL_VarTypeFlagsString(ctx, ty);
//	s1=BGBCC_CCXL_QualifyNameNS(ctx, name);
	s1=BGBCC_CCXL_QualifyNameNSFl(ctx, name, i);

	if(!v && BGBCC_CCXL_CheckDefinedContextName(ctx,
		CCXL_CMD_VARDECL, s1))
	{
		return;
	}

	i=BCCX_GetIntCst(ty, &bgbcc_rcst_flags, "flags");
//	BGBCC_CCXL_BindVarSig(ctx, s1, s);
//	if(s2 && (*s2))
//			BGBCC_CCXL_BindVarInfo(ctx, s1, "flagstr", s2);

	if(!s1)return;
	if(!s1)s1="_";

	BGBCC_CCXL_BeginName(ctx, CCXL_CMD_VARDECL, s1);

	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_NAME, s1);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_SIG, s);
	BGBCC_CCXL_AttribStr(ctx, CCXL_ATTR_FLAGS, s2);

	if(v)
	{
		BGBCC_CCXL_Begin(ctx, CCXL_CMD_VARVALUE);
		BGBCC_CCXL_EmitVarValueR2(ctx, v);
		BGBCC_CCXL_End(ctx);
	}

	BGBCC_CCXL_End(ctx);
}

void BGBCC_CCXL_CompileTopStatement(BGBCC_TransState *ctx, BCCX_Node *l)
{
	char tb[256];
	BCCX_Node *c, *n, *v, *t, *ntl;
	char *s, *s1;
	int i;

	s=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
	if(s)ctx->lfn=s;
	i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
	if(i)ctx->lln=i;

//	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_vars, "vars"))
	{
		c=BCCX_Child(l);
		while(c)
		{
			if(BCCX_TagIsCstP(c, &bgbcc_rcst_proto, "proto"))
			{
				s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				t=BCCX_FindTag(c, "type");
				n=BCCX_Fetch(c, "args");

				BGBCC_CCXL_CompileProto(ctx, t, s, n);
			}

			if(BCCX_TagIsCstP(c, &bgbcc_rcst_var, "var"))
			{
				s=BCCX_GetCst(c, &bgbcc_rcst_name, "name");
				t=BCCX_FindTag(c, "type");
				v=BCCX_Fetch(c, "value");

				BGBCC_CCXL_EmitTopVar(ctx, s, t, v);
			}

			c=BCCX_Next(c);
		}
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_defun, "defun"))
	{
//		s0=BCCX_GetCst(l, &bgbcc_rcst_fn, "fn");
//		if(s0)ctx->lfn=s0;
//		i=BCCX_GetIntCst(l, &bgbcc_rcst_ln, "ln");
//		if(i>0)ctx->lln=i;

		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");
		v=BCCX_Fetch(l, "body");
//		tk=BCCX_GetIntCst(l, &bgbcc_rcst_tokens, "tokens");
		ntl=BCCX_Fetch(l, "argdecls");

		BGBCC_CCXL_CompileBlock2(ctx, t, s, n, v, ntl);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_proto, "proto"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		t=BCCX_FindTag(l, "type");
		n=BCCX_Fetch(l, "args");

		BGBCC_CCXL_CompileProto(ctx, t, s, n);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_struct, "struct"))
	{
		BGBCC_CCXL_CompileStruct(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_union, "union"))
	{
		BGBCC_CCXL_CompileUnion(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_class, "class"))
	{
		BGBCC_CCXL_CompileClass(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_enum, "enum"))
	{
		BGBCC_CCXL_CompileEnum(ctx, l);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_import, "import"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_using, "using"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		BGBCC_CCXL_StubError(ctx);
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_package, "package"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		n=BCCX_FindTag(l, "body");

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

//		BGBCC_CCXL_BindStructSig(ctx, s, "namespace");
		BGBCC_CCXL_StubError(ctx);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;

#if 1
		if(n)
		{
			c=BCCX_Child(n);
			while(c)
			{
				BGBCC_CCXL_CompileTopStatement(ctx, c);
				c=BCCX_Next(c);
			}
			ctx->cur_ns=s1;
		}
#endif
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_namespace, "namespace"))
	{
		s=BCCX_GetCst(l, &bgbcc_rcst_name, "name");
		c=BCCX_Child(l);

		if(ctx->cur_ns)
		{
			sprintf(tb, "%s/%s", ctx->cur_ns, s);
			s=bgbcc_strdup(tb);
		}

//		BGBCC_CCXL_BindStructSig(ctx, s, "namespace");
		BGBCC_CCXL_StubError(ctx);

		s1=ctx->cur_ns;
		ctx->cur_ns=s;
		
		while(c)
		{
			BGBCC_CCXL_CompileTopStatement(ctx, c);
			c=BCCX_Next(c);
		}
		ctx->cur_ns=s1;
		
		return;
	}

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_null_statement, "null_statement"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_funcall, "funcall"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_linecomment, "linecomment"))
		{ return; }
	if(BCCX_TagIsCstP(l, &bgbcc_rcst_blockcomment, "blockcomment"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_pragma, "pragma"))
		{ return; }

	if(BCCX_TagIsCstP(l, &bgbcc_rcst_asm_blob, "asm_blob"))
	{
		c=BCCX_Child(l);
		BGBCC_CCXL_AddAsmBlob(ctx, BCCX_Text(c));
//		BGBCC_CCXL_Warn(ctx, "inline ASM is unsupported\n");
		return;
	}

	BGBCC_CCXL_Error(ctx, "unhandled top statement %s\n", BCCX_Tag(l));
}


int BGBCC_CCXL_CompileModuleCTX(
	BGBCC_TransState *ctx, char *name, BCCX_Node *l)
{
	BCCX_Node *c;
	char *s;
	int i;

	ctx->n_warn=0;
	ctx->n_error=0;

	s=BCCX_GetCst(l, &bgbcc_rcst_lang, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_GetCst(l, &bgbcc_rcst_arch, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	i=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
	ctx->tuidx=i;
	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_TUIDX, i);

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_CCXL_CompileStructs(ctx);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(-1);
	}

	return(0);
}

char *BGBCC_CCXL_CompileModule(char *name, BCCX_Node *l)
{
	BGBCC_TransState *ctx;
	BCCX_Node *c;
	char *s;
	int i;

	ctx=bgbcc_malloc(sizeof(BGBCC_TransState));
	memset(ctx, 0, sizeof(BGBCC_TransState));

	ctx->n_warn=0;
	ctx->n_error=0;

	s=BCCX_GetCst(l, &bgbcc_rcst_lang, "lang");
	ctx->lang=BGBCP_LangForName(s);

	s=BCCX_GetCst(l, &bgbcc_rcst_arch, "arch");
	ctx->arch=BGBCP_ArchForName(s);

	i=BCCX_GetIntCst(l, &bgbcc_rcst_index, "index");
	ctx->tuidx=i;
	BGBCC_CCXL_AttribInt(ctx, CCXL_ATTR_TUIDX, i);

	ctx->types=BCCX_Fetch(l, "types");
	ctx->structs=BCCX_Fetch(l, "structs");

	BGBCC_CCXL_CompileStructs(ctx);

	c=BCCX_Fetch(l, "body");
	while(c)
	{
		BGBCC_CCXL_CompileTopStatement(ctx, c);
		c=BCCX_Next(c);
	}

	if(ctx->n_error)
	{
		printf("Error Count %d\n", ctx->n_error);
		return(NULL);
	}

//	return(ctx->ips);
	return(NULL);
}
