ccxl_status BGBCC_SHX_StackFn(BGBCC_TransState *ctx, char *name)
{
	return(0);
}

ccxl_status BGBCC_SHX_StackLn(BGBCC_TransState *ctx, int line)
{
	return(0);
}


ccxl_status BGBCC_SHX_EmitMarkEndTrace(BGBCC_TransState *ctx)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLabel(BGBCC_TransState *ctx, ccxl_label id)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitJump(BGBCC_TransState *ctx, ccxl_label id)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitJumpRegZero(BGBCC_TransState *ctx,
	ccxl_type ty, int cmpop, ccxl_register reg, ccxl_label lbl)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitJumpRegCmp(BGBCC_TransState *ctx,
	ccxl_type ty, int cmpop, ccxl_register sreg, ccxl_register treg,
	ccxl_label lbl)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitMov(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	return(0);
}


ccxl_status BGBCC_SHX_EmitCallOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int na)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitCallCsrvOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitCallArg(BGBCC_TransState *ctx,
	ccxl_register reg)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitCallRetDefault(BGBCC_TransState *ctx)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitCallRetV(BGBCC_TransState *ctx)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitCallRetOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register src)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitConv(BGBCC_TransState *ctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitUnaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitBinaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitCompareOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLoadIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitStoreIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLoadIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitStoreIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLeaImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLea(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLdaVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitSizeofVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitDiffPtr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	return(0);
}


ccxl_status BGBCC_SHX_EmitOffsetOf(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_SHX_LiteralInfo *st, char *name)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLoadSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_SHX_LiteralInfo *st, char *name)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitStoreSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_SHX_LiteralInfo *st, char *name)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLoadSlotAddr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_SHX_LiteralInfo *st, char *name)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLoadSlotAddrID(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_SHX_LiteralInfo *st, int fn)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitInitObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_SHX_LiteralInfo *st)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitDropObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_SHX_LiteralInfo *st)
{
	return(0);
}


ccxl_status BGBCC_SHX_EmitInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, int sz)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_SHX_LiteralInfo *st, int sz)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLoadInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register val, int sz)
{
	return(0);
}

ccxl_status BGBCC_SHX_EmitLoadInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_SHX_LiteralInfo *st, ccxl_register val, int sz)
{
	return(0);
}
