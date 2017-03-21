BGBCC_CCXL_BackendFuncs_vt *bgbcc_ccxl_backends=NULL;

ccxl_status BGBCC_CCXL_RegisterBackend(BGBCC_CCXL_BackendFuncs_vt *ivt)
{
	ivt->next=bgbcc_ccxl_backends;
	bgbcc_ccxl_backends=ivt;
	return(0);
}

#if 1
ccxl_status BGBCC_CCXL_StackFn(BGBCC_TransState *ctx, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DBGFN;
	op->imm.str=bgbcc_strdup(name);
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_StackLn(BGBCC_TransState *ctx, int line)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DBGLN;
	op->imm.si=line;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLabel(BGBCC_TransState *ctx, ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LABEL;
	op->imm.ui=lbl.id;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJump(BGBCC_TransState *ctx, ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_JMP;
	op->imm.ui=lbl.id;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJumpRegZero(BGBCC_TransState *ctx,
	ccxl_type type, int cmpop, ccxl_register reg, ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_JCMP_ZERO;
	op->opr=cmpop;
	op->type=type;
	op->srca=reg;
	op->imm.ui=lbl.id;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJumpRegCmp(BGBCC_TransState *ctx,
	ccxl_type type, int cmpop, ccxl_register sreg, ccxl_register treg,
	ccxl_label lbl)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_JCMP;
	op->opr=cmpop;
	op->type=type;
	op->srca=sreg;
	op->srcb=treg;
	op->imm.ui=lbl.id;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitMov(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_MOV;
	op->type=type;
	op->srca=sreg;
	op->dst=dreg;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}


ccxl_status BGBCC_CCXL_EmitCallOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int na)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CALL;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->imm.call.na=na;
	op->imm.call.ca=0;
	op->imm.call.args=bgbcc_malloc(na*sizeof(ccxl_register));
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallCsrvOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CSRV;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallArg(BGBCC_TransState *ctx,
	ccxl_register reg)
{
	BGBCC_CCXL_VirtOp *op;
	int i;
	op=ctx->vop[ctx->n_vop-1];
	i=op->imm.call.ca++;
	op->imm.call.args[i]=reg;
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallRetDefault(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_RETDFL;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallRetV(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_RETV;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallRetOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_RET;
	op->type=type;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	BGBCC_CCXL_EmitMarkEndTrace(ctx);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitConv(BGBCC_TransState *ctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_CONV;
	op->type=dtype;
	op->stype=stype;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitUnaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr,
	ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_UNARY;
	op->opr=opr;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitBinaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_BINARY;
	op->opr=opr;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCompareOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_COMPARE;
	op->opr=opr;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LDIXIMM;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->imm.si=idx;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_STIXIMM;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->imm.si=idx;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LDIX;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_STIX;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLeaImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LEAIMM;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->imm.si=idx;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLea(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LEA;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLdaVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LDAVAR;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitSizeofVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_SIZEOFVAR;
	op->type=type;
	op->dst=dst;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitDiffPtr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DIFFPTR;
	op->type=type;
	op->dst=dst;
	op->srca=srca;
	op->srcb=srcb;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}


ccxl_status BGBCC_CCXL_EmitOffsetOf(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	int fn;

	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_OFFSETOF;
	op->type=type;
	op->dst=dst;
//	op->srca=srca;
//	op->srcb=srcb;
	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	int fn;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADSLOT;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	int fn;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_STORESLOT;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlotAddr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_VirtOp *op;
	int fn;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADSLOTADDR;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	fn=BGBCC_CCXL_LookupStructFieldID(ctx, st, name);
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlotAddrID(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, int fn)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADSLOTADDR;
	op->type=type;
	op->dst=dst;
	op->srca=src;
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=fn;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitInitObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_INITOBJ;
	op->type=type;
	op->dst=dst;
	op->imm.obj.gid=st->litid;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitDropObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_DROPOBJ;
	op->type=type;
	op->dst=dst;
	op->imm.si=st->litid;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, int sz)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_INITARR;
	op->type=type;
	op->dst=dst;
	op->imm.si=sz;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, int sz)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_INITOBJARR;
	op->type=type;
	op->dst=dst;
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=sz;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register val, int sz)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADINITARR;
	op->type=type;
	op->dst=dst;
	op->srca=val;
	op->imm.si=sz;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, ccxl_register val, int sz)
{
	BGBCC_CCXL_VirtOp *op;
	op=BGBCC_CCXL_AllocVirtOp(ctx);
	op->opn=CCXL_VOP_LOADINITOBJARR;
	op->type=type;
	op->dst=dst;
	op->srca=val;
	op->imm.obj.gid=st->litid;
	op->imm.obj.fid=sz;
	BGBCC_CCXL_AddVirtOp(ctx, op);
	return(0);
}

#endif


ccxl_status BGBCC_CCXL_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	if(ctx->back_vt && ctx->back_vt->FlattenImage)
		return(ctx->back_vt->FlattenImage(ctx, obuf, rosz, imgfmt));
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}


/** Setup context for the current target architecture.
  * Returns UNSUPPORTED error status if unsupported.
  */
ccxl_status BGBCC_CCXL_SetupContextForArch(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_BackendFuncs_vt *cur;
	int i;

	if(ctx->back_vt && ctx->back_vt->SetupContextForArch)
		return(ctx->back_vt->SetupContextForArch(ctx));
		
	cur=bgbcc_ccxl_backends;
	while(cur)
	{
		if(cur->SetupContextForArch)
		{
			i=cur->SetupContextForArch(ctx);
			if(i>=0)
			{
				ctx->back_vt=cur;
				return(i);
			}
		}
		cur=cur->next;
	}
	
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}


/** Setup parser context for the current target architecture.
  * Returns UNSUPPORTED error status if unsupported.
  */
ccxl_status BGBCC_CCXL_SetupParserForArch(BGBCP_ParseState *ctx)
{
	BGBCC_CCXL_BackendFuncs_vt *cur;
	int i;

	if(ctx->back_vt && ctx->back_vt->SetupParserForArch)
		return(ctx->back_vt->SetupParserForArch(ctx));
		
	cur=bgbcc_ccxl_backends;
	while(cur)
	{
		if(cur->SetupParserForArch)
		{
			i=cur->SetupParserForArch(ctx);
			if(i>=0)
			{
				ctx->back_vt=cur;
				return(i);
			}
		}
		cur=cur->next;
	}
	
	return(CCXL_STATUS_ERR_UNIMPLEMENTED);
}


ccxl_status BGBCC_CCXL_InitTargets(void)
{
	static int init=0;
	
	if(init)
		return(0);
	init=1;

	return(1);
}