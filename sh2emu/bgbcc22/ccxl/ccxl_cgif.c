ccxl_status BGBCC_CCXL_StackFn(BGBCC_TransState *ctx, char *name)
{
	if(ctx->back_vt && ctx->back_vt->StackFn)
		return(ctx->back_vt->StackFn(ctx, name));
	return(0);
}

ccxl_status BGBCC_CCXL_StackLn(BGBCC_TransState *ctx, int line)
{
	if(ctx->back_vt && ctx->back_vt->StackLn)
		return(ctx->back_vt->StackLn(ctx, line));
	return(0);
}

#if 0
// ccxl_status BGBCC_CCXL_Begin(BGBCC_TransState *ctx, int tag)
{
	if(ctx->back_vt && ctx->back_vt->Begin)
		return(ctx->back_vt->Begin(ctx, tag));
	return(0);
}

// ccxl_status BGBCC_CCXL_BeginName(BGBCC_TransState *ctx, int tag, char *name)
{
	if(ctx->back_vt && ctx->back_vt->BeginName)
		return(ctx->back_vt->BeginName(ctx, tag, name));
	return(0);
}

// ccxl_status BGBCC_CCXL_AttribStr(BGBCC_TransState *ctx, int attr, char *str)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_AttribLong(BGBCC_TransState *ctx, int attr, s64 val)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_LiteralInt(BGBCC_TransState *ctx, int tag, s64 val)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_LiteralLong(BGBCC_TransState *ctx, int tag, s64 val)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_LiteralFloat(BGBCC_TransState *ctx, int tag, f64 val)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_LiteralDouble(BGBCC_TransState *ctx, int tag, f64 val)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_LiteralStr(BGBCC_TransState *ctx, int tag, char *str)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_LiteralGlobalAddr(BGBCC_TransState *ctx,
	int tag, int gblid)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_MarkTypeAccessed(BGBCC_TransState *ctx, ccxl_type bty)
//{
//	return(0);
//}

// ccxl_status BGBCC_CCXL_CheckDefinedContextName(BGBCC_TransState *ctx,
	int tag, char *str)
{
	return(0);
}


// ccxl_status BGBCC_CCXL_Marker(BGBCC_TransState *ctx, int tag)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_End(BGBCC_TransState *ctx)
{
	return(0);
}
#endif


ccxl_status BGBCC_CCXL_EmitMarkEndTrace(BGBCC_TransState *ctx)
{
	if(ctx->back_vt && ctx->back_vt->EmitMarkEndTrace)
		return(ctx->back_vt->EmitMarkEndTrace(ctx));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLabel(BGBCC_TransState *ctx, ccxl_label id)
{
	if(ctx->back_vt && ctx->back_vt->EmitLabel)
		return(ctx->back_vt->EmitLabel(ctx, id));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJump(BGBCC_TransState *ctx, ccxl_label id)
{
	if(ctx->back_vt && ctx->back_vt->EmitJump)
		return(ctx->back_vt->EmitJump(ctx, id));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJumpRegZero(BGBCC_TransState *ctx,
	ccxl_type ty, int cmpop, ccxl_register reg, ccxl_label lbl)
{
	if(ctx->back_vt && ctx->back_vt->EmitJumpRegZero)
		return(ctx->back_vt->EmitJumpRegZero(ctx, ty, cmpop, reg, lbl));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitJumpRegCmp(BGBCC_TransState *ctx,
	ccxl_type ty, int cmpop, ccxl_register sreg, ccxl_register treg,
	ccxl_label lbl)
{
	if(ctx->back_vt && ctx->back_vt->EmitJumpRegCmp)
		return(ctx->back_vt->EmitJumpRegCmp(ctx, ty, cmpop, sreg, treg, lbl));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitMov(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dreg, ccxl_register sreg)
{
	if(ctx->back_vt && ctx->back_vt->EmitMov)
		return(ctx->back_vt->EmitMov(ctx, type, dreg, sreg));
	return(0);
}


ccxl_status BGBCC_CCXL_EmitCallOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int na)
{
	if(ctx->back_vt && ctx->back_vt->EmitCallOp)
		return(ctx->back_vt->EmitCallOp(ctx, type, dst, src, na));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallCsrvOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	if(ctx->back_vt && ctx->back_vt->EmitCallCsrvOp)
		return(ctx->back_vt->EmitCallCsrvOp(ctx, type, dst, src));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallArg(BGBCC_TransState *ctx,
	ccxl_register reg)
{
	if(ctx->back_vt && ctx->back_vt->EmitCallArg)
		return(ctx->back_vt->EmitCallArg(ctx, reg));
	return(0);
}

#if 1
ccxl_status BGBCC_CCXL_EmitCallRetDefault(BGBCC_TransState *ctx)
{
	if(ctx->back_vt && ctx->back_vt->EmitCallRetDefault)
		return(ctx->back_vt->EmitCallRetDefault(ctx));
	return(0);
}
#endif

ccxl_status BGBCC_CCXL_EmitCallRetV(BGBCC_TransState *ctx)
{
	if(ctx->back_vt && ctx->back_vt->EmitCallRetV)
		return(ctx->back_vt->EmitCallRetV(ctx));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCallRetOp(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register src)
{
	if(ctx->back_vt && ctx->back_vt->EmitCallRetOp)
		return(ctx->back_vt->EmitCallRetOp(ctx, type, src));
	return(0);
}

#if 0
// ccxl_status BGBCC_CCXL_EmitMovInt(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_EmitMovLong(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_EmitMovFloat(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_EmitMovDouble(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_EmitMovPointer(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_EmitMovMatchDst(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}

// ccxl_status BGBCC_CCXL_EmitMovMatchSrc(BGBCC_TransState *ctx,
	ccxl_register dst, ccxl_register src)
{
	return(0);
}
#endif

ccxl_status BGBCC_CCXL_EmitConv(BGBCC_TransState *ctx,
	ccxl_type dtype, ccxl_type stype,
	ccxl_register dst, ccxl_register src)
{
	if(ctx->back_vt && ctx->back_vt->EmitConv)
		return(ctx->back_vt->EmitConv(ctx, dtype, stype, dst, src));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitUnaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr,
	ccxl_register dst, ccxl_register src)
{
	if(ctx->back_vt && ctx->back_vt->EmitUnaryOp)
		return(ctx->back_vt->EmitUnaryOp(ctx, type, opr, dst, src));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitBinaryOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	if(ctx->back_vt && ctx->back_vt->EmitBinaryOp)
		return(ctx->back_vt->EmitBinaryOp(ctx, type, opr, dst, srca, srcb));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitCompareOp(BGBCC_TransState *ctx,
	ccxl_type type, int opr, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	if(ctx->back_vt && ctx->back_vt->EmitCompareOp)
		return(ctx->back_vt->EmitCompareOp(ctx, type, opr, dst, srca, srcb));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	if(ctx->back_vt && ctx->back_vt->EmitLoadIndexImm)
		return(ctx->back_vt->EmitLoadIndexImm(ctx, type, dst, src, idx));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreIndexImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	if(ctx->back_vt && ctx->back_vt->EmitStoreIndexImm)
		return(ctx->back_vt->EmitStoreIndexImm(ctx, type, dst, src, idx));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	if(ctx->back_vt && ctx->back_vt->EmitLoadIndex)
		return(ctx->back_vt->EmitLoadIndex(ctx, type, dst, srca, srcb));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreIndex(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	if(ctx->back_vt && ctx->back_vt->EmitStoreIndex)
		return(ctx->back_vt->EmitStoreIndex(ctx, type, dst, srca, srcb));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLeaImm(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src, int idx)
{
	if(ctx->back_vt && ctx->back_vt->EmitLeaImm)
		return(ctx->back_vt->EmitLeaImm(ctx, type, dst, src, idx));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLea(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	if(ctx->back_vt && ctx->back_vt->EmitLea)
		return(ctx->back_vt->EmitLea(ctx, type, dst, srca, srcb));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLdaVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src)
{
	if(ctx->back_vt && ctx->back_vt->EmitLdaVar)
		return(ctx->back_vt->EmitLdaVar(ctx, type, dst, src));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitSizeofVar(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst)
{
	if(ctx->back_vt && ctx->back_vt->EmitSizeofVar)
		return(ctx->back_vt->EmitSizeofVar(ctx, type, dst));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitDiffPtr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	ccxl_register srca, ccxl_register srcb)
{
	if(ctx->back_vt && ctx->back_vt->EmitDiffPtr)
		return(ctx->back_vt->EmitDiffPtr(ctx, type, dst, srca, srcb));
	return(0);
}


ccxl_status BGBCC_CCXL_EmitOffsetOf(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	if(ctx->back_vt && ctx->back_vt->EmitOffsetOf)
		return(ctx->back_vt->EmitOffsetOf(ctx, type, dst, st, name));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	if(ctx->back_vt && ctx->back_vt->EmitLoadSlot)
		return(ctx->back_vt->EmitLoadSlot(ctx, type, dst, src, st, name));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitStoreSlot(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	if(ctx->back_vt && ctx->back_vt->EmitStoreSlot)
		return(ctx->back_vt->EmitStoreSlot(ctx, type, dst, src, st, name));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadSlotAddr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, char *name)
{
	if(ctx->back_vt && ctx->back_vt->EmitLoadSlotAddr)
		return(ctx->back_vt->EmitLoadSlotAddr(ctx,
			type, dst, src, st, name));
	return(0);
}

#if 1
ccxl_status BGBCC_CCXL_EmitLoadSlotAddrID(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register src,
	BGBCC_CCXL_LiteralInfo *st, int fn)
{
	if(ctx->back_vt && ctx->back_vt->EmitLoadSlotAddrID)
		return(ctx->back_vt->EmitLoadSlotAddrID(ctx,
			type, dst, src, st, fn));
	return(0);
}
#endif

ccxl_status BGBCC_CCXL_EmitInitObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st)
{
	if(ctx->back_vt && ctx->back_vt->EmitInitObj)
		return(ctx->back_vt->EmitInitObj(ctx,
			type, dst, st));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitDropObj(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st)
{
	if(ctx->back_vt && ctx->back_vt->EmitDropObj)
		return(ctx->back_vt->EmitDropObj(ctx,
			type, dst, st));
	return(0);
}


ccxl_status BGBCC_CCXL_EmitInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, int sz)
{
	if(ctx->back_vt && ctx->back_vt->EmitInitArr)
		return(ctx->back_vt->EmitInitArr(ctx,
			type, dst, sz));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, int sz)
{
	if(ctx->back_vt && ctx->back_vt->EmitInitObjArr)
		return(ctx->back_vt->EmitInitObjArr(ctx,
			type, dst, st, sz));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadInitArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst, ccxl_register val, int sz)
{
	if(ctx->back_vt && ctx->back_vt->EmitLoadInitArr)
		return(ctx->back_vt->EmitLoadInitArr(ctx,
			type, dst, val, sz));
	return(0);
}

ccxl_status BGBCC_CCXL_EmitLoadInitObjArr(BGBCC_TransState *ctx,
	ccxl_type type, ccxl_register dst,
	BGBCC_CCXL_LiteralInfo *st, ccxl_register val, int sz)
{
	if(ctx->back_vt && ctx->back_vt->EmitLoadInitObjArr)
		return(ctx->back_vt->EmitLoadInitObjArr(ctx,
			type, dst, st, val, sz));
	return(0);
}
