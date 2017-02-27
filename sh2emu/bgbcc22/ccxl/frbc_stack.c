#include <bgbccc.h>

void BGBCC_FrBC_DebugPrintStackLLn(
	BGBCC_FrCC_State *ctx, char *op, char *file, int line)
{
	frbc_register reg;
	int i;

	return;

	printf("%16s:%4d(%10s) (RP=%d): ",
		file, line, op, ctx->cur_func->n_regs);

	for(i=0; i<ctx->regstackpos; i++)
	{
		reg=ctx->regstack[i];
		
		if(BGBCC_FrBC_IsRegRegExtP(ctx, reg))
		{
			printf("R%d ", BGBCC_FrBC_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegArgExtP(ctx, reg))
		{
			printf("A%d ", BGBCC_FrBC_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegLocalExtP(ctx, reg))
		{
			printf("L%d ", BGBCC_FrBC_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegGlobalP(ctx, reg))
		{
			printf("G%d ", BGBCC_FrBC_GetRegID(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegImmIntP(ctx, reg))
		{
			printf("I:%d ", BGBCC_FrBC_GetRegImmIntValue(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegImmLongP(ctx, reg))
		{
			printf("L:%d ", (int)BGBCC_FrBC_GetRegImmLongValue(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegImmFloatP(ctx, reg))
		{
			printf("F:%f ", BGBCC_FrBC_GetRegImmFloatValue(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegImmDoubleP(ctx, reg))
		{
			printf("D:%f ", BGBCC_FrBC_GetRegImmDoubleValue(ctx, reg));
			continue;
		}

		if(BGBCC_FrBC_IsRegImmStringP(ctx, reg))
		{
			printf("S:%d ", (int)(reg.val&FR2C_REGINT_MASK));
			continue;
		}
		
		printf("?(%08X%08X)", (u32)(reg.val>>32), (u32)reg.val);
	}
	
	printf("--\n");
}

frbc_status BGBCC_FrBC_CheckExpandStack(BGBCC_FrCC_State *ctx)
{
	int rt;
	int i;

	rt=FR2C_STATUS_NO;
	if(!ctx->markstack)
	{
		i=64;
		ctx->markstack=bgbcc_malloc(i*sizeof(int));
		ctx->markstackpos=0;
		rt=FR2C_STATUS_YES;
	}

	if(!ctx->regstack)
	{
		i=256;
		ctx->regstack=bgbcc_malloc(i*sizeof(frbc_register));
		ctx->regstackpos=0;
		rt=FR2C_STATUS_YES;
	}

	if(!ctx->uregstack)
	{
		i=16;
		ctx->uregstack=bgbcc_malloc(i*sizeof(frbc_register));
		ctx->uregstackpos=0;
		rt=FR2C_STATUS_YES;
	}
	
	return(rt);
}

frbc_status BGBCC_FrBC_PushRegister(BGBCC_FrCC_State *ctx, frbc_register reg)
{
	int i;

	BGBCC_FrBC_CheckExpandStack(ctx);
	
	i=ctx->regstackpos++;
	ctx->regstack[i]=reg;
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_PopRegister(BGBCC_FrCC_State *ctx,
	frbc_register *rreg)
{
	int i;

	if(!ctx->regstack || ctx->regstackpos<=0)
	{
		BGBCC_FrBC_TagError(ctx, FR2C_TERR_STACK_UNDERFLOW);
		return(FR2C_STATUS_NO);
	}

	i=--ctx->regstackpos;
	*rreg=ctx->regstack[i];
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_PushURegister(BGBCC_FrCC_State *ctx,
	frbc_register reg)
{
	int i;

	BGBCC_FrBC_CheckExpandStack(ctx);
	
	i=ctx->uregstackpos++;
	ctx->uregstack[i]=reg;
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_PopURegister(BGBCC_FrCC_State *ctx,
	frbc_register *rreg)
{
	int i;

	if(!ctx->uregstack || ctx->uregstackpos<=0)
	{
		BGBCC_FrBC_TagError(ctx, FR2C_TERR_STACK_UNDERFLOW);
		return(FR2C_STATUS_NO);
	}

	i=--ctx->uregstackpos;
	*rreg=ctx->uregstack[i];
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_PeekURegister(BGBCC_FrCC_State *ctx,
	frbc_register *rreg)
{
	int i;

	if(!ctx->uregstack || ctx->uregstackpos<=0)
	{
		BGBCC_FrBC_TagError(ctx, FR2C_TERR_STACK_UNDERFLOW);
		return(FR2C_STATUS_NO);
	}

	*rreg=ctx->uregstack[ctx->uregstackpos-1];
	return(FR2C_STATUS_YES);
}


frbc_status BGBCC_FrBC_PushMark(BGBCC_FrCC_State *ctx)
{
	int i;

	BGBCC_FrBC_CheckExpandStack(ctx);
	
	i=ctx->markstackpos++;
	ctx->markstack[i]=ctx->regstackpos;
	return(FR2C_STATUS_YES);
}

int BGBCC_FrBC_LookupArgIndex(BGBCC_FrCC_State *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->cur_func->n_args; i++)
		if(!strcmp(ctx->cur_func->args[i]->name, name))
			return(i);
	return(-1);
}

int BGBCC_FrBC_LookupLocalIndex(BGBCC_FrCC_State *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->cur_func->n_locals; i++)
	{
		if(!ctx->cur_func->locals[i])
			continue;
		if(!strcmp(ctx->cur_func->locals[i]->name, name))
			return(i);
	}
	return(-1);
}

int BGBCC_FrBC_LookupGlobalIndex(BGBCC_FrCC_State *ctx, char *name)
{
	int i;
	
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		if(!ctx->reg_globals[i])
			continue;

		if(ctx->reg_globals[i]->defp)
		{
			if(ctx->reg_globals[i]->defp!=ctx->cur_func)
				continue;
		}
		
		if(!strcmp(ctx->reg_globals[i]->name, name))
			return(i);
	}
	return(-1);
}

int BGBCC_FrBC_LookupAsRegister(BGBCC_FrCC_State *ctx,
	char *name, frbc_register *rreg)
{
	int i;

	i=BGBCC_FrBC_TryLookupAsRegister(ctx, name, rreg);
	if(i>0)return(i);

	BGBCC_FrCC_Error(ctx, "Undeclared Variable '%s'\n", name);

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_NO));

	return(FR2C_STATUS_NO);
}

int BGBCC_FrBC_TryLookupAsRegister(BGBCC_FrCC_State *ctx,
	char *name, frbc_register *rreg)
{
	frbc_register treg;
	frbc_type ty;
	int i;

	i=BGBCC_FrBC_LookupLocalIndex(ctx, name);
	if(i>=0)
	{
		ty=ctx->cur_func->locals[i]->type;
//		if(BGBCC_FrBC_TypeArrayP(ctx, ty))
//			{ BGBCC_FrBC_TypePointerType(ctx, ty, &ty); }
		treg.val=FR2C_REGTY_LOCAL|
			(((s64)ty.val)<<FR2C_REGID_TYPESHIFT)|i;
		BGBCC_FrBC_RegisterCheckAcquire(ctx, treg);
		*rreg=treg;
		return(FR2C_STATUS_YES);
	}

	i=BGBCC_FrBC_LookupArgIndex(ctx, name);
	if(i>=0)
	{
		ty=ctx->cur_func->args[i]->type;
//		if(BGBCC_FrBC_TypeArrayP(ctx, ty))
//			{ BGBCC_FrBC_TypePointerType(ctx, ty, &ty); }
		treg.val=FR2C_REGTY_ARG|
			(((s64)ty.val)<<FR2C_REGID_TYPESHIFT)|i;
		BGBCC_FrBC_RegisterCheckAcquire(ctx, treg);
		*rreg=treg;
		return(FR2C_STATUS_YES);
	}

	i=BGBCC_FrBC_LookupGlobalIndex(ctx, name);
	if(i>=0)
	{
		ty=ctx->reg_globals[i]->type;
//		if(BGBCC_FrBC_TypeArrayP(ctx, ty))
//			{ BGBCC_FrBC_TypePointerType(ctx, ty, &ty); }

		ctx->reg_globals[i]->regflags|=
			BGBCC_REGFL_ACCESSED;
		treg.val=FR2C_REGTY_GLOBAL|
			(((s64)ty.val)<<FR2C_REGID_TYPESHIFT)|i;
		BGBCC_FrBC_RegisterCheckAcquire(ctx, treg);
		*rreg=treg;
		return(FR2C_STATUS_YES);
	}

	return(FR2C_STATUS_NO);
}

BGBCC_FrCC_RegisterInfo *BGBCC_FrBC_LookupLocalInfo(
	BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_RegisterInfo *def;
	int i;
	
	for(i=0; i<ctx->cur_func->n_locals; i++)
	{
		if(!ctx->cur_func->locals[i])
			continue;
		if(!strcmp(ctx->cur_func->locals[i]->name, name))
			return(ctx->cur_func->locals[i]);
	}
	return(NULL);
}

BGBCC_FrCC_RegisterInfo *BGBCC_FrBC_LookupArgInfo(
	BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_RegisterInfo *def;
	int i;
	
	for(i=0; i<ctx->cur_func->n_args; i++)
	{
		if(!ctx->cur_func->args[i])
			continue;
		if(!strcmp(ctx->cur_func->args[i]->name, name))
			return(ctx->cur_func->args[i]);
	}
	return(NULL);
}

BGBCC_FrCC_RegisterInfo *BGBCC_FrBC_LookupGlobalInfo(
	BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_RegisterInfo *def;
	int i;
	
	i=BGBCC_FrBC_LookupGlobalIndex(ctx, name);
	def=ctx->reg_globals[i];
	return(def);
}

frbc_status BGBCC_FrBC_PushLoad(BGBCC_FrCC_State *ctx, char *name)
{
	frbc_register treg;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "Load", __FILE__, __LINE__);

	i=BGBCC_FrBC_LookupAsRegister(ctx, name, &treg);
	if(i<=0)
	{
		if(i==FR2C_STATUS_NO)
			return(FR2C_STATUS_ERR_LOOKUPFAIL);
		return(FR2C_STATUS_ERR_GENERIC);
	}
	
	BGBCC_FrBC_PushRegister(ctx, treg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_PopStore(BGBCC_FrCC_State *ctx, char *name)
{
	frbc_register treg, dreg;
	int i, j, k;
	
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "Store", __FILE__, __LINE__);

	i=BGBCC_FrBC_PopRegister(ctx, &treg);
	j=BGBCC_FrBC_LookupAsRegister(ctx, name, &dreg);
	if((i<0) || (j<=0))
	{
		if(!j)return(FR2C_STATUS_ERR_LOOKUPFAIL);
		if(i<0)return(i);
		if(j<0)return(j);
		return(FR2C_STATUS_ERR_GENERIC);
	}
	
	k=BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_GetRegType(ctx, dreg),
		dreg, treg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	return(k);
}

frbc_status BGBCC_FrBC_StackTransforCallArgs(BGBCC_FrCC_State *ctx)
{
	frbc_register treg, dreg;
	int ms;
	int i, j, k, n;
	
	ms=ctx->markstack[ctx->markstackpos-1];
	n=ctx->regstackpos-ms;
//	BGBCC_FrBC_EmitByte(ctx, n);

	while(ctx->regstackpos>ms)
	{
		i=BGBCC_FrBC_PopRegister(ctx, &treg);
		BGBCC_FrBC_EmitCallArg(ctx, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	}
	ctx->markstackpos--;
	return(FR2C_STATUS_YES);
}

int BGBCC_FrBC_StackGetCntCallArgs(BGBCC_FrCC_State *ctx)
{
	int ms, n;
	ms=ctx->markstack[ctx->markstackpos-1];
	n=ctx->regstackpos-ms;
	return(n);
}

frbc_status BGBCC_FrBC_StackCallName(BGBCC_FrCC_State *ctx, char *name)
{
	frbc_register treg, dreg;
	frbc_type bty;
	BGBCC_FrCC_RegisterInfo *ri;
	int i, j, k, n;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "CallName", __FILE__, __LINE__);

	i=BGBCC_FrBC_LookupLocalIndex(ctx, name);
	if(i<0)
		{ i=BGBCC_FrBC_LookupArgIndex(ctx, name); }
	if(i<0)
		{ i=BGBCC_FrBC_LookupGlobalIndex(ctx, name); }
//	i=BGBCC_FrBC_LookupAsRegister(ctx, name, &treg);
	if(i<0)
	{
		BGBCC_FrCC_Error(ctx, "Undeclared Function %s\n", name);
		BGBCC_FrBC_StackPushConstInt(ctx, 0);
//		ri=BGBCC_FrBC_GetGlobal(ctx, name);
//		ri->sig=bgbcc_strdup("(z)i");
		return(FR2C_STATUS_NO);
	}

	j=BGBCC_FrBC_LookupAsRegister(ctx, name, &treg);
	bty=BGBCC_FrBC_GetRegReturnType(ctx, treg);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, &dreg);
	n=BGBCC_FrBC_StackGetCntCallArgs(ctx);
	BGBCC_FrBC_EmitCallOp(ctx, bty, dreg, treg, n);
	BGBCC_FrBC_StackTransforCallArgs(ctx);
	BGBCC_FrBC_EmitCallCsrvOp(ctx, bty, dreg, treg);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackPopCall(BGBCC_FrCC_State *ctx)
{
	frbc_register treg, dreg;
	frbc_type bty;
	int i, j, k, n;
	
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "PopCall", __FILE__, __LINE__);

//	j=BGBCC_FrBC_LookupAsRegister(ctx, name, &treg);
	i=BGBCC_FrBC_PopRegister(ctx, &treg);

	bty=BGBCC_FrBC_GetRegReturnType(ctx, treg);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, &dreg);
	n=BGBCC_FrBC_StackGetCntCallArgs(ctx);
	BGBCC_FrBC_EmitCallOp(ctx, bty, dreg, treg, n);
	BGBCC_FrBC_StackTransforCallArgs(ctx);
	BGBCC_FrBC_EmitCallCsrvOp(ctx, bty, dreg, treg);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	return(FR2C_STATUS_YES);
}


frbc_status BGBCC_FrBC_StackLoadIndexConst(BGBCC_FrCC_State *ctx, int idx)
{
	frbc_register treg, dreg, sreg;
	frbc_type bty, bty2;
	int i, j, k;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "LoadIndexC", __FILE__, __LINE__);
	
	/* src -- src[idx] */
	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	bty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
	BGBCC_FrBC_TypeAutoPromoteType(ctx, bty, &bty2);
	BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
	BGBCC_FrBC_EmitLoadIndexImm(ctx, bty, dreg, sreg, idx);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackStoreIndexConst(BGBCC_FrCC_State *ctx, int idx)
{
	frbc_register treg, dreg, sreg;
	frbc_type bty;
	int i, j, k;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "StoreIndexC", __FILE__, __LINE__);

	/* value dest -- */
	i=BGBCC_FrBC_PopRegister(ctx, &dreg);
	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
//	bty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
	bty=BGBCC_FrBC_GetRegDerefType(ctx, dreg);
//	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_EmitStoreIndexImm(ctx, bty, dreg, sreg, idx);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackLoadIndexAddrConst(
	BGBCC_FrCC_State *ctx, int idx)
{
	frbc_register treg, dreg, sreg;
	frbc_type bty, vty;
	int i, j, k;
	
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "LoadIndexAC", __FILE__, __LINE__);
	
	/* src -- *src */
	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	vty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, &dreg);
	BGBCC_FrBC_EmitLeaImm(ctx, vty, dreg, sreg, idx);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackLoadIndex(BGBCC_FrCC_State *ctx)
{
	frbc_register dreg, sreg, treg;
	frbc_type bty, bty2;
	int i, j, k;
	
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "LoadIndex", __FILE__, __LINE__);
	
	/* src idx -- src[idx] */
	i=BGBCC_FrBC_PopRegister(ctx, &treg);
	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	bty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
	BGBCC_FrBC_TypeAutoPromoteType(ctx, bty, &bty2);
	BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty2, &dreg);

	bty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
//	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_EmitLoadIndex(ctx, bty, dreg, sreg, treg);
	BGBCC_FrBC_PushRegister(ctx, dreg);

	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackStoreIndex(BGBCC_FrCC_State *ctx)
{
	frbc_register dreg, sreg, treg;
	frbc_type bty;
	int i, j, k;
	
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "StoreIndex", __FILE__, __LINE__);
	
	/* value dest index -- */
	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	i=BGBCC_FrBC_PopRegister(ctx, &dreg);
	i=BGBCC_FrBC_PopRegister(ctx, &treg);
//	bty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
	bty=BGBCC_FrBC_GetRegDerefType(ctx, dreg);
//	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_EmitStoreIndex(ctx, bty, dreg, sreg, treg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackLoadIndexAddr(BGBCC_FrCC_State *ctx)
{
	frbc_register dreg, sreg, treg;
	frbc_type bty, vty;
	int i, j, k;
	
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "LoadIndexA", __FILE__, __LINE__);
	
	/* src idx -- src[idx] */
	i=BGBCC_FrBC_PopRegister(ctx, &treg);
	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	vty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, &dreg);

	bty=BGBCC_FrBC_GetRegDerefType(ctx, sreg);
//	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_EmitLea(ctx, vty, dreg, sreg, treg);
	BGBCC_FrBC_PushRegister(ctx, dreg);

	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	return(FR2C_STATUS_YES);
}


frbc_status BGBCC_FrBC_StackDup(BGBCC_FrCC_State *ctx)
{
	frbc_register sreg, trega, tregb;
	frbc_type bty;
	int i, j, k;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "Dup", __FILE__, __LINE__);
	
	/* src -- src(1) src(2) */
//	i=BGBCC_FrBC_PopRegister(ctx, &treg);
//	BGBCC_FrBC_RegisterCheckAcquire(ctx, treg);
//	BGBCC_FrBC_PushRegister(ctx, treg);
//	BGBCC_FrBC_PushRegister(ctx, treg);
//	return(FR2C_STATUS_YES);

	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	bty=BGBCC_FrBC_GetRegType(ctx, sreg);

	BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty, &trega);
	BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty, &tregb);
	BGBCC_FrBC_EmitMov(ctx, bty, trega, sreg);
	BGBCC_FrBC_EmitMov(ctx, bty, tregb, sreg);
	BGBCC_FrBC_PushRegister(ctx, trega);
	BGBCC_FrBC_PushRegister(ctx, tregb);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);
}
/** More efficient dup, only safe for clean expressions. */
frbc_status BGBCC_FrBC_StackDupClean(BGBCC_FrCC_State *ctx)
{
	frbc_register treg;
	frbc_type bty;
	int i, j, k;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "DupClean", __FILE__, __LINE__);
	
	/* src -- src src */
	i=BGBCC_FrBC_PopRegister(ctx, &treg);
	BGBCC_FrBC_RegisterCheckAcquire(ctx, treg);
	BGBCC_FrBC_PushRegister(ctx, treg);
	BGBCC_FrBC_PushRegister(ctx, treg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackRetV(BGBCC_FrCC_State *ctx)
{
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "RetV", __FILE__, __LINE__);

	BGBCC_FrBC_EmitCallRetV(ctx);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackRet(BGBCC_FrCC_State *ctx)
{
	frbc_register treg;
	frbc_type bty;
	int i, j, k;
	
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "Ret", __FILE__, __LINE__);

	/* src -- */
	i=BGBCC_FrBC_PopRegister(ctx, &treg);
	bty=BGBCC_FrBC_GetRegType(ctx, treg);
	BGBCC_FrBC_EmitCallRetOp(ctx, bty, treg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackCastBool(BGBCC_FrCC_State *ctx)
{
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "CastBool", __FILE__, __LINE__);

	return(BGBCC_FrBC_StackCastSig(ctx, "b"));
//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackCastSig(BGBCC_FrCC_State *ctx, char *sig)
{
	frbc_register sreg, dreg;
	frbc_type sty, dty;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "CastSig", __FILE__, __LINE__);

	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	sty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_TypeFromSig(ctx, &dty, sig);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &dreg);
	BGBCC_FrBC_EmitConv(ctx, dty, sty, dreg, sreg);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);

//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackBinaryOp(BGBCC_FrCC_State *ctx, char *op)
{
	frbc_register sreg, treg, dreg, sreg2, treg2, dreg2;
	frbc_type sty, tty, dty, bty, pty;
	int opr;
	int i, j, k;

	opr=-1;
	if(!strcmp(op, "+"))opr=FR2C_BINOP_ADD;
	if(!strcmp(op, "-"))opr=FR2C_BINOP_SUB;
	if(!strcmp(op, "*"))opr=FR2C_BINOP_MUL;
	if(!strcmp(op, "/"))opr=FR2C_BINOP_DIV;
	if(!strcmp(op, "%"))opr=FR2C_BINOP_MOD;
	if(!strcmp(op, "&"))opr=FR2C_BINOP_AND;
	if(!strcmp(op, "|"))opr=FR2C_BINOP_OR;
	if(!strcmp(op, "^"))opr=FR2C_BINOP_XOR;
	if(!strcmp(op, "<<"))opr=FR2C_BINOP_SHL;
	if(!strcmp(op, ">>"))opr=FR2C_BINOP_SHR;
	if(!strcmp(op, ">>>"))opr=FR2C_BINOP_SHRR;

	if(opr>=0)
	{
		BGBCC_FrBC_DebugPrintStackLLn(ctx, "BinaryOp", __FILE__, __LINE__);

		j=BGBCC_FrBC_PopRegister(ctx, &treg);
		i=BGBCC_FrBC_PopRegister(ctx, &sreg);
		sty=BGBCC_FrBC_GetRegType(ctx, sreg);
		tty=BGBCC_FrBC_GetRegType(ctx, treg);
		BGBCC_FrBC_GetTypeBinaryDest(ctx, opr, sty, tty, &dty);

		if(BGBCC_FrBC_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_FrBC_TypeArrayOrPointerP(ctx, tty) &&
			(opr==FR2C_BINOP_SUB))
		{
			pty=BGBCC_FrBC_MakeTypeID(ctx, FR2C_TY_I);
			BGBCC_FrBC_RegisterAllocTemporary(ctx, pty, &dreg);

			BGBCC_FrBC_EmitDiffPtr(ctx, sty, dreg, sreg, treg);

			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);

			BGBCC_FrBC_PushRegister(ctx, dreg);
			return(FR2C_STATUS_YES);
		}

		if(BGBCC_FrBC_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_FrBC_TypeIntP(ctx, tty))
		{
			if(BGBCC_FrBC_IsRegImmIntP(ctx, treg) &&
				(opr==FR2C_BINOP_ADD) ||
				(opr==FR2C_BINOP_SUB))
			{
				k=BGBCC_FrBC_GetRegImmIntValue(ctx, treg);
				if(opr==FR2C_BINOP_SUB)
					k=-k;

				if(BGBCC_FrBC_TypeArrayP(ctx, sty))
					{ BGBCC_FrBC_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }
				
				BGBCC_FrBC_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_FrBC_PushRegister(ctx, dreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				return(FR2C_STATUS_YES);
			}
			
			if(opr==FR2C_BINOP_ADD)
			{
				if(BGBCC_FrBC_TypeArrayP(ctx, sty))
					{ BGBCC_FrBC_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_FrBC_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_FrBC_PushRegister(ctx, dreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				return(FR2C_STATUS_YES);
			}

			if(opr==FR2C_BINOP_SUB)
			{
				if(BGBCC_FrBC_TypeArrayP(ctx, sty))
					{ BGBCC_FrBC_TypePointerType(ctx, sty, &pty); }
				else
					{ pty=sty; }

				BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_FrBC_EmitUnaryOp(ctx, tty,
					FR2C_UNOP_NEG, treg2, treg);

				BGBCC_FrBC_RegisterAllocTemporary(ctx, pty, &dreg);
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_FrBC_PushRegister(ctx, dreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg2);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				return(FR2C_STATUS_YES);
			}
		}

		if(BGBCC_FrBC_TypeEqualP(ctx, dty, sty) &&
			BGBCC_FrBC_TypeEqualP(ctx, dty, tty))
		{
			BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &dreg);
			BGBCC_FrBC_EmitBinaryOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_FrBC_PushRegister(ctx, dreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &dreg);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_FrBC_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_FrBC_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);

		BGBCC_FrBC_EmitBinaryOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg2);
		return(FR2C_STATUS_YES);
	}

	if(!strcmp(op, "=="))opr=FR2C_CMP_EQ;
	if(!strcmp(op, "!="))opr=FR2C_CMP_NE;
	if(!strcmp(op, "<"))opr=FR2C_CMP_LT;
	if(!strcmp(op, ">"))opr=FR2C_CMP_GT;
	if(!strcmp(op, "<="))opr=FR2C_CMP_LE;
	if(!strcmp(op, ">="))opr=FR2C_CMP_GE;

	if(opr>=0)
	{
		BGBCC_FrBC_DebugPrintStackLLn(ctx, "CompareOp", __FILE__, __LINE__);

		j=BGBCC_FrBC_PopRegister(ctx, &treg);
		i=BGBCC_FrBC_PopRegister(ctx, &sreg);
		sty=BGBCC_FrBC_GetRegType(ctx, sreg);
		tty=BGBCC_FrBC_GetRegType(ctx, treg);
		BGBCC_FrBC_GetTypeCompareBinaryDest(ctx, opr, sty, tty, &dty);

		if(BGBCC_FrBC_TypeEqualP(ctx, dty, sty) &&
			BGBCC_FrBC_TypeEqualP(ctx, dty, tty))
		{
			BGBCC_FrBC_RegisterAllocTemporaryInt(ctx, &dreg);
			BGBCC_FrBC_EmitCompareOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_FrBC_PushRegister(ctx, dreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_RegisterAllocTemporaryInt(ctx, &dreg);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_FrBC_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_FrBC_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);

		BGBCC_FrBC_EmitCompareOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg2);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackUnaryOp(BGBCC_FrCC_State *ctx, char *op)
{
	frbc_register sreg, dreg;
	frbc_type sty, dty, bty;
	int opr;
	int i, j, k;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "UnaryOp", __FILE__, __LINE__);

	opr=-1;
	if(!strcmp(op, "+"))opr=FR2C_UNOP_MOV;
	if(!strcmp(op, "-"))opr=FR2C_UNOP_NEG;
	if(!strcmp(op, "~"))opr=FR2C_UNOP_NOT;
	if(!strcmp(op, "!"))opr=FR2C_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=FR2C_UNOP_INC;
	if(!strcmp(op, "--"))opr=FR2C_UNOP_DEC;

	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	sty=BGBCC_FrBC_GetRegType(ctx, sreg);

	if(BGBCC_FrBC_TypeArrayOrPointerP(ctx, sty))
	{
		if((opr==FR2C_UNOP_INC) ||
			(opr==FR2C_UNOP_DEC))
		{
			BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
			k=(opr==FR2C_UNOP_INC)?(1):(-1);
			BGBCC_FrBC_RegisterAllocTemporary(ctx, sty, &dreg);
			BGBCC_FrBC_EmitLeaImm(ctx, bty, dreg, sreg, k);
			BGBCC_FrBC_PushRegister(ctx, dreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			return(FR2C_STATUS_YES);
		}
	}

//	BGBCC_FrBC_TypeFromSig(ctx, &dty, sig);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_FrBC_EmitConv(ctx, dty, sty, dreg, sreg);
	BGBCC_FrBC_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackBinaryOpStore(BGBCC_FrCC_State *ctx,
	char *op, char *name)
{
	frbc_register sreg, treg, dreg, sreg2, treg2;
	frbc_type sty, tty, dty, bty;
	int opr;
	int i, j, k;

//	BGBCC_FrBC_DebugPrintStackLLn(ctx, "BinaryOpStore", __FILE__, __LINE__);

//	i=BGBCC_FrBC_PopRegister(ctx, &treg);
	j=BGBCC_FrBC_LookupAsRegister(ctx, name, &dreg);
	if(j<=0)
	{
		if(!j)return(FR2C_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(FR2C_STATUS_ERR_GENERIC);
	}
	
	opr=-1;
	if(!strcmp(op, "+"))opr=FR2C_BINOP_ADD;
	if(!strcmp(op, "-"))opr=FR2C_BINOP_SUB;
	if(!strcmp(op, "*"))opr=FR2C_BINOP_MUL;
	if(!strcmp(op, "/"))opr=FR2C_BINOP_DIV;
	if(!strcmp(op, "%"))opr=FR2C_BINOP_MOD;
	if(!strcmp(op, "&"))opr=FR2C_BINOP_AND;
	if(!strcmp(op, "|"))opr=FR2C_BINOP_OR;
	if(!strcmp(op, "^"))opr=FR2C_BINOP_XOR;
	if(!strcmp(op, "<<"))opr=FR2C_BINOP_SHL;
	if(!strcmp(op, ">>"))opr=FR2C_BINOP_SHR;
	if(!strcmp(op, ">>>"))opr=FR2C_BINOP_SHRR;

	if(opr>=0)
	{
		BGBCC_FrBC_DebugPrintStackLLn(ctx,
			"BinaryOpStore", __FILE__, __LINE__);

		j=BGBCC_FrBC_PopRegister(ctx, &treg);
		i=BGBCC_FrBC_PopRegister(ctx, &sreg);
		sty=BGBCC_FrBC_GetRegType(ctx, sreg);
		tty=BGBCC_FrBC_GetRegType(ctx, treg);
		dty=BGBCC_FrBC_GetRegType(ctx, dreg);
//		BGBCC_FrBC_GetTypeBinaryDest(ctx, opr, sty, tty, &dty);

//		if(BGBCC_FrBC_TypeArrayOrPointerP(ctx, sty) &&
//			BGBCC_FrBC_TypeIntP(ctx, tty))
		if(BGBCC_FrBC_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_FrBC_TypeSmallIntP(ctx, tty))
		{
			if(BGBCC_FrBC_IsRegImmIntP(ctx, treg) &&
				(opr==FR2C_BINOP_ADD) ||
				(opr==FR2C_BINOP_SUB))
			{
				k=BGBCC_FrBC_GetRegImmIntValue(ctx, treg);
				if(opr==FR2C_BINOP_SUB)
					k=-k;
				
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLeaImm(ctx, bty, dreg, sreg, k);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
				return(FR2C_STATUS_YES);
			}
			
			if(opr==FR2C_BINOP_ADD)
			{
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
				return(FR2C_STATUS_YES);
			}

			if(opr==FR2C_BINOP_SUB)
			{
				BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_FrBC_EmitUnaryOp(ctx, tty,
					FR2C_UNOP_NEG, treg2, treg);
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg2);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
				return(FR2C_STATUS_YES);
			}
		}

		if(BGBCC_FrBC_TypeArrayOrPointerP(ctx, sty) &&
			BGBCC_FrBC_TypeSmallLongP(ctx, tty))
		{
			if(opr==FR2C_BINOP_ADD)
			{
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLea(ctx, bty, dreg, sreg, treg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
				return(FR2C_STATUS_YES);
			}

			if(opr==FR2C_BINOP_SUB)
			{
				BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg2);
				BGBCC_FrBC_EmitUnaryOp(ctx, tty,
					FR2C_UNOP_NEG, treg2, treg);
				BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
				BGBCC_FrBC_EmitLea(ctx, bty, dreg, sreg, treg2);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg2);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
				return(FR2C_STATUS_YES);
			}
		}

		if(opr==FR2C_BINOP_SHR)
		{
			if(BGBCC_FrBC_TypeUnsignedP(ctx, sty))
				opr=FR2C_BINOP_SHRR;
		}

		if(BGBCC_FrBC_TypeEqualP(ctx, dty, sty) &&
			BGBCC_FrBC_TypeEqualP(ctx, dty, tty))
		{
			BGBCC_FrBC_EmitBinaryOp(ctx, dty, opr, dreg, sreg, treg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			return(FR2C_STATUS_YES);
		}

//		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &dreg);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_FrBC_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_FrBC_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);

		BGBCC_FrBC_EmitBinaryOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
		return(FR2C_STATUS_YES);
	}

	if(!strcmp(op, "=="))opr=FR2C_CMP_EQ;
	if(!strcmp(op, "!="))opr=FR2C_CMP_NE;
	if(!strcmp(op, "<"))opr=FR2C_CMP_LT;
	if(!strcmp(op, ">"))opr=FR2C_CMP_GT;
	if(!strcmp(op, "<="))opr=FR2C_CMP_LE;
	if(!strcmp(op, ">="))opr=FR2C_CMP_GE;

	if(opr>=0)
	{
		BGBCC_FrBC_DebugPrintStackLLn(ctx,
			"CompareOpStore", __FILE__, __LINE__);

		j=BGBCC_FrBC_PopRegister(ctx, &treg);
		i=BGBCC_FrBC_PopRegister(ctx, &sreg);
		sty=BGBCC_FrBC_GetRegType(ctx, sreg);
		tty=BGBCC_FrBC_GetRegType(ctx, treg);
//		dty=BGBCC_FrBC_GetRegType(ctx, dreg);
		BGBCC_FrBC_GetTypeBinaryDest(ctx, opr, sty, tty, &dty);

		if(BGBCC_FrBC_TypeEqualP(ctx, dty, sty) &&
			BGBCC_FrBC_TypeEqualP(ctx, dty, tty))
		{
//			BGBCC_FrBC_RegisterAllocTemporaryInt(ctx, &dreg);
			BGBCC_FrBC_EmitCompareOp(ctx, dty, opr, dreg, sreg, treg);
//			BGBCC_FrBC_PushRegister(ctx, dreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			return(FR2C_STATUS_YES);
		}

//		BGBCC_FrBC_RegisterAllocTemporaryInt(ctx, &dreg);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &sreg2);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dty, &treg2);
		BGBCC_FrBC_EmitConv(ctx, dty, sty, sreg2, sreg);
		BGBCC_FrBC_EmitConv(ctx, dty, tty, treg2, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);

		BGBCC_FrBC_EmitCompareOp(ctx, dty, opr, dreg, sreg2, treg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg2);
		BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackUnaryOpName(BGBCC_FrCC_State *ctx,
	char *op, char *name)
{
	frbc_register sreg;
	frbc_type sty, dty, bty;
	int opr;
	int i, j, k;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "UnaryOp", __FILE__, __LINE__);

	j=BGBCC_FrBC_LookupAsRegister(ctx, name, &sreg);
	if(j<=0)
	{
		if(!j)return(FR2C_STATUS_ERR_LOOKUPFAIL);
//		if(i<0)return(i);
		if(j<0)return(j);
		return(FR2C_STATUS_ERR_GENERIC);
	}

	opr=-1;
	if(!strcmp(op, "+"))opr=FR2C_UNOP_MOV;
	if(!strcmp(op, "-"))opr=FR2C_UNOP_NEG;
	if(!strcmp(op, "~"))opr=FR2C_UNOP_NOT;
	if(!strcmp(op, "!"))opr=FR2C_UNOP_LNOT;
	if(!strcmp(op, "++"))opr=FR2C_UNOP_INC;
	if(!strcmp(op, "--"))opr=FR2C_UNOP_DEC;

	sty=BGBCC_FrBC_GetRegType(ctx, sreg);

	if(BGBCC_FrBC_TypePointerP(ctx, sty))
	{
		if((opr==FR2C_UNOP_INC) ||
			(opr==FR2C_UNOP_DEC))
		{
			BGBCC_FrBC_TypeDerefType(ctx, sty, &bty);
			k=(opr==FR2C_UNOP_INC)?(1):(-1);
//			BGBCC_FrBC_RegisterAllocTemporary(ctx, sty, &dreg);
//			BGBCC_FrBC_EmitLeaImm(ctx, bty, dreg, sreg, k);
//			BGBCC_FrBC_PushRegister(ctx, dreg);
			BGBCC_FrBC_EmitLeaImm(ctx, bty, sreg, sreg, k);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			return(FR2C_STATUS_YES);
		}
	}

	BGBCC_FrBC_EmitUnaryOp(ctx, sty, opr, sreg, sreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);

//	BGBCC_FrBC_TypeFromSig(ctx, &dty, sig);
//	BGBCC_FrBC_RegisterAllocTemporary(ctx, sty, &dreg);
//	BGBCC_FrBC_EmitConv(ctx, dty, sty, dreg, sreg);
//	BGBCC_FrBC_EmitUnaryOp(ctx, sty, opr, dreg, sreg);
//	BGBCC_FrBC_PushRegister(ctx, dreg);
//	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
//	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackPushConstInt(BGBCC_FrCC_State *ctx, s32 val)
{
	frbc_register sreg;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "PushConstI", __FILE__, __LINE__);

	BGBCC_FrBC_GetRegForIntValue(ctx, &sreg, val);
	BGBCC_FrBC_PushRegister(ctx, sreg);
	return(FR2C_STATUS_YES);
//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackPushConstLong(BGBCC_FrCC_State *ctx, s64 val)
{
	frbc_register sreg;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "PushConstL", __FILE__, __LINE__);

	BGBCC_FrBC_GetRegForLongValue(ctx, &sreg, val);
	BGBCC_FrBC_PushRegister(ctx, sreg);
	return(FR2C_STATUS_YES);
//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackPushConstFloat(
	BGBCC_FrCC_State *ctx, double val)
{
	frbc_register sreg;
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "PushConstF", __FILE__, __LINE__);
	BGBCC_FrBC_GetRegForFloatValue(ctx, &sreg, val);
	BGBCC_FrBC_PushRegister(ctx, sreg);
	return(FR2C_STATUS_YES);
//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackPushConstDouble(
	BGBCC_FrCC_State *ctx, double val)
{
	frbc_register sreg;
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "PushConstD", __FILE__, __LINE__);
	BGBCC_FrBC_GetRegForDoubleValue(ctx, &sreg, val);
	BGBCC_FrBC_PushRegister(ctx, sreg);
	return(FR2C_STATUS_YES);
//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackPushConstString(
	BGBCC_FrCC_State *ctx, char *val)
{
	frbc_register sreg;
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "PushConstS", __FILE__, __LINE__);
	BGBCC_FrBC_GetRegForStringValue(ctx, &sreg, val);
	BGBCC_FrBC_PushRegister(ctx, sreg);
	return(FR2C_STATUS_YES);
//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackPushConstWString(
	BGBCC_FrCC_State *ctx, char *val)
{
	frbc_register sreg;
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "PushConstWS", __FILE__, __LINE__);
	BGBCC_FrBC_GetRegForWStringValue(ctx, &sreg, val);
	BGBCC_FrBC_PushRegister(ctx, sreg);
	return(FR2C_STATUS_YES);
//	BGBCC_FrBC_StubError(ctx);
}

frbc_status BGBCC_FrBC_StackPop(BGBCC_FrCC_State *ctx)
{
	frbc_register sreg;
	int i;
	BGBCC_FrBC_DebugPrintStackLLn(ctx, "Pop", __FILE__, __LINE__);
	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackLoadAddr(BGBCC_FrCC_State *ctx, char *name)
{
	frbc_register sreg, dreg;
	frbc_type bty, pty;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "LoadA", __FILE__, __LINE__);

//	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	i=BGBCC_FrBC_LookupAsRegister(ctx, name, &sreg);

//	bty=BGBCC_FrBC_GetRegPointerToType(ctx, sreg);
	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_TypePointerType(ctx, bty, &pty);

	if(BGBCC_FrBC_TypeValueObjectP(ctx, bty) ||
		BGBCC_FrBC_TypeArrayP(ctx, bty))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, pty, &dreg);
		BGBCC_FrBC_EmitMov(ctx, pty, dreg, sreg);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_RegisterAllocTemporary(ctx, pty, &dreg);
	BGBCC_FrBC_EmitLdaVar(ctx, pty, dreg, sreg);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_StackSizeofSig(BGBCC_FrCC_State *ctx, char *sig)
{
	frbc_register sreg, dreg;
	frbc_type bty;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "SizeOfSig", __FILE__, __LINE__);

//	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
//	bty=BGBCC_FrBC_GetRegType(ctx, sreg);
	BGBCC_FrBC_TypeFromSig(ctx, &bty, sig);

	BGBCC_FrBC_RegisterAllocTemporaryInt(ctx, &dreg);
	BGBCC_FrBC_EmitSizeofVar(ctx, bty, dreg);
	BGBCC_FrBC_PushRegister(ctx, dreg);

//	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);

//	BGBCC_FrBC_StubError(ctx);
//	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackSizeofVal(BGBCC_FrCC_State *ctx)
{
	frbc_register sreg, dreg;
	frbc_type bty;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "SizeOfVal", __FILE__, __LINE__);

	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	bty=BGBCC_FrBC_GetRegType(ctx, sreg);

	BGBCC_FrBC_RegisterAllocTemporaryInt(ctx, &dreg);
	BGBCC_FrBC_EmitSizeofVar(ctx, bty, dreg);
	BGBCC_FrBC_PushRegister(ctx, dreg);

	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
	return(FR2C_STATUS_YES);

//	BGBCC_FrBC_StubError(ctx);
//	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackOffsetof(BGBCC_FrCC_State *ctx,
	char *sig, char *name)
{
	BGBCC_FrCC_LiteralInfo *st;
	frbc_register sreg, dreg;
	frbc_type bty;
	int i;

//	BGBCC_FrBC_LookupStructure()

	bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);
	st=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, &dreg);
	BGBCC_FrBC_EmitOffsetOf(ctx, bty, dreg, st, name);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	return(FR2C_STATUS_YES);
	
//	BGBCC_FrBC_StubError(ctx);
//	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackLoadSlot(BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_LiteralInfo *st;
	frbc_register sreg, dreg, treg;
	frbc_type bty, sty, bty2;
	int i, j;

//	BGBCC_FrBC_LookupStructure()

	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	sty=BGBCC_FrBC_GetRegType(ctx, sreg);

	st=BGBCC_FrBC_LookupStructureForType(ctx, sty);

//	st=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
//	bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);

	if(BGBCC_FrBC_LookupStructFieldType(ctx, st, name, &bty)>0)
	{
		BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty, &dreg);
		BGBCC_FrBC_EmitLoadSlot(ctx, bty, dreg, sreg, st, name);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		return(FR2C_STATUS_YES);
	}
	
	j=BGBCC_FrBC_LookupStructContainsFieldID(ctx, st, name);
	if(j>=0)
	{
		BGBCC_FrBC_LookupStructFieldIdType(ctx, st, j, &bty);
		BGBCC_FrBC_TypePointerType(ctx, bty, &bty2);

		BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
		BGBCC_FrBC_EmitLoadSlotAddrID(ctx, bty2, dreg, sreg, st, j);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		return(BGBCC_FrBC_StackLoadSlot(ctx, name));
	}
	
	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_StackStoreSlot(BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_LiteralInfo *st;
	frbc_register sreg, dreg, treg;
	frbc_type bty, sty, fty, bty2;
	int i, j;

	//value obj --

//	BGBCC_FrBC_LookupStructure()

	BGBCC_FrBC_PopRegister(ctx, &dreg);
	BGBCC_FrBC_PopRegister(ctx, &sreg);
	sty=BGBCC_FrBC_GetRegType(ctx, dreg);
	bty=BGBCC_FrBC_GetRegType(ctx, sreg);

	st=BGBCC_FrBC_LookupStructureForType(ctx, sty);

//	st=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
//	bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);
	if(BGBCC_FrBC_LookupStructFieldType(ctx, st, name, &fty)>0)
	{
		if(BGBCC_FrBC_TypeBaseSmallEqualP(ctx, fty, bty))
		{
//			BGBCC_FrBC_EmitStoreSlot(ctx, bty, dreg, sreg, st, name);
			BGBCC_FrBC_EmitStoreSlot(ctx, fty, dreg, sreg, st, name);
			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_RegisterAllocTemporary(ctx, fty, &treg);
//		BGBCC_FrBC_EmitConv(ctx, fty, sty, treg, sreg);
		BGBCC_FrBC_EmitConv(ctx, fty, bty, treg, sreg);
		BGBCC_FrBC_EmitStoreSlot(ctx, fty, dreg, sreg, st, name);
		BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(FR2C_STATUS_YES);
	}

	j=BGBCC_FrBC_LookupStructContainsFieldID(ctx, st, name);
	if(j>=0)
	{
		BGBCC_FrBC_LookupStructFieldIdType(ctx, st, j, &bty);
		BGBCC_FrBC_TypePointerType(ctx, bty, &bty2);

		BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
		BGBCC_FrBC_EmitLoadSlotAddrID(ctx, bty2, dreg, sreg, st, j);

		BGBCC_FrBC_PushRegister(ctx, sreg);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		return(BGBCC_FrBC_StackStoreSlot(ctx, name));
	}
	
	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_StackLoadSlotAddr(BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_LiteralInfo *st;
	frbc_register sreg, dreg, treg;
	frbc_type bty, sty, bty2;
	int i, j;

//	BGBCC_FrBC_LookupStructure()

	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	sty=BGBCC_FrBC_GetRegType(ctx, sreg);

	st=BGBCC_FrBC_LookupStructureForType(ctx, sty);

//	st=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
//	bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);

	if(BGBCC_FrBC_LookupStructFieldType(ctx, st, name, &bty)>0)
	{
		BGBCC_FrBC_TypePointerType(ctx, bty, &bty2);

		BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
		BGBCC_FrBC_EmitLoadSlotAddr(ctx, bty, dreg, sreg, st, name);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		return(FR2C_STATUS_YES);
	}
	
	j=BGBCC_FrBC_LookupStructContainsFieldID(ctx, st, name);
	if(j>=0)
	{
		BGBCC_FrBC_LookupStructFieldIdType(ctx, st, j, &bty);
		BGBCC_FrBC_TypePointerType(ctx, bty, &bty2);

		BGBCC_FrBC_RegisterAllocTemporaryInit(ctx, bty2, &dreg);
		BGBCC_FrBC_EmitLoadSlotAddrID(ctx, bty2, dreg, sreg, st, j);
		BGBCC_FrBC_PushRegister(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		return(BGBCC_FrBC_StackLoadSlotAddr(ctx, name));
	}
	
	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_StackFn(BGBCC_FrCC_State *ctx, char *fn)
{
	int i;
	
	i=BGBCC_FrBC_IndexString(ctx, fn);
	BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LFN);
	BGBCC_FrBC_EmitWord24(ctx, i);

//	BGBCC_FrBC_StubNote(ctx);
//	BGBCC_FrBC_StubError(ctx);
	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackLn(BGBCC_FrCC_State *ctx, int ln)
{
	BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LLN);
	BGBCC_FrBC_EmitWord(ctx, ln);

//	BGBCC_FrBC_StubNote(ctx);
//	BGBCC_FrBC_StubError(ctx);
	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackBeginU(BGBCC_FrCC_State *ctx, char *sig)
{
	frbc_register sreg, dreg;
	frbc_type bty;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "BeginU", __FILE__, __LINE__);

	BGBCC_FrBC_TypeFromSig(ctx, &bty, sig);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, &dreg);
	BGBCC_FrBC_PushURegister(ctx, dreg);
	return(FR2C_STATUS_YES);

//	BGBCC_FrBC_StubError(ctx);
//	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackEndU(BGBCC_FrCC_State *ctx)
{
	frbc_register reg;
	frbc_type bty;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "EndU", __FILE__, __LINE__);

	i=BGBCC_FrBC_PopURegister(ctx, &reg);
	BGBCC_FrBC_PushRegister(ctx, reg);
	return(FR2C_STATUS_YES);

//	BGBCC_FrBC_StubError(ctx);
//	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackSetU(BGBCC_FrCC_State *ctx)
{
	frbc_register sreg, dreg;
	frbc_type dty, sty;
	int i;

	BGBCC_FrBC_DebugPrintStackLLn(ctx, "SetU", __FILE__, __LINE__);

	i=BGBCC_FrBC_PopRegister(ctx, &sreg);
	i=BGBCC_FrBC_PeekURegister(ctx, &dreg);
	sty=BGBCC_FrBC_GetRegType(ctx, sreg);
	dty=BGBCC_FrBC_GetRegType(ctx, dreg);

	BGBCC_FrBC_EmitConv(ctx, dty, sty, dreg, sreg);
	BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);

	return(FR2C_STATUS_YES);

//	BGBCC_FrBC_StubError(ctx);
//	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackInitVar(BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_RegisterInfo *def;
	BGBCC_FrCC_LiteralInfo *st;
	frbc_register sreg, dreg;
	frbc_type bty;
	char *s;
	int i, j, k, l;

	def=BGBCC_FrBC_LookupLocalInfo(ctx, name);
	if(def)
	{
		i=BGBCC_FrBC_LookupAsRegister(ctx, name, &dreg);

		st=BGBCC_FrBC_LookupStructureForSig2(ctx, def->sig);
		if(st)
		{
			bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P);
			BGBCC_FrBC_EmitInitObj(ctx, bty, dreg, st);
			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			return(FR2C_STATUS_YES);
		}
		
		s=def->sig;
//		l=BGBCC_FrBC_GetArraySizeForSig2R(ctx, &s);
		l=BGBCC_FrBC_GetArraySizeForSig3R(ctx, &s);
		if(l>0)
		{
			st=BGBCC_FrBC_LookupStructureForSig2(ctx, s);
			if(st)
			{
				bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P);
//				BGBCC_FrBC_EmitInitArr(ctx, bty, dreg, l);
				BGBCC_FrBC_EmitInitObjArr(ctx, bty, dreg, st, l);
				BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
				return(FR2C_STATUS_YES);
			}

			BGBCC_FrBC_TypeFromSig(ctx, &bty, s);
			BGBCC_FrBC_EmitInitArr(ctx, bty, dreg, l);
			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			return(FR2C_STATUS_YES);
		}
	}

	def=BGBCC_FrBC_LookupArgInfo(ctx, name);
	if(def)
	{
		i=BGBCC_FrBC_LookupAsRegister(ctx, name, &dreg);

		st=BGBCC_FrBC_LookupStructureForSig2(ctx, def->sig);
		if(st)
		{
			bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P);
			BGBCC_FrBC_EmitInitObj(ctx, bty, dreg, st);
			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			return(FR2C_STATUS_YES);
		}
	}

//	BGBCC_FrBC_StubError(ctx);
	return(FR2C_STATUS_NO);

#if 0
	BGBCC_FrCC_LiteralInfo *st;
	frbc_register sreg, dreg;
	frbc_type bty;
	int i;

//	BGBCC_FrBC_LookupStructure()

	bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);
	st=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
	BGBCC_FrBC_RegisterAllocTemporary(ctx, bty, &dreg);
	BGBCC_FrBC_EmitOffsetOf(ctx, bty, dreg, st, name);
	BGBCC_FrBC_PushRegister(ctx, dreg);
	return(FR2C_STATUS_YES);
	
//	BGBCC_FrBC_StubError(ctx);
//	return(FR2C_STATUS_NO);
#endif

	BGBCC_FrBC_StubError(ctx);
	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_StackInitVarValue(BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_RegisterInfo *def;
	BGBCC_FrCC_LiteralInfo *st;
	frbc_register sreg, dreg;
	frbc_type bty;
	char *s;
	int i, j, k, l;

	def=BGBCC_FrBC_LookupLocalInfo(ctx, name);
	if(def)
	{
		BGBCC_FrBC_PopRegister(ctx, &sreg);

		i=BGBCC_FrBC_LookupAsRegister(ctx, name, &dreg);

		st=BGBCC_FrBC_LookupStructureForSig2(ctx, def->sig);
		if(st)
		{
			BGBCC_FrBC_StubError(ctx);

			bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P);
			BGBCC_FrBC_EmitInitObj(ctx, bty, dreg, st);

			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			return(FR2C_STATUS_YES);
		}
		
		s=def->sig;
//		l=BGBCC_FrBC_GetArraySizeForSig2R(ctx, &s);
		l=BGBCC_FrBC_GetArraySizeForSig3R(ctx, &s);
		if(l>0)
		{
//			BGBCC_FrBC_StubError(ctx);

			st=BGBCC_FrBC_LookupStructureForSig2(ctx, s);
			if(st)
			{
				bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P);
//				BGBCC_FrBC_EmitInitObjArr(ctx, bty, dreg, st, l);
				BGBCC_FrBC_EmitLoadInitObjArr(ctx,
					bty, dreg, st, def->value, l);

				BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
				BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
				return(FR2C_STATUS_YES);
			}

			BGBCC_FrBC_TypeFromSig(ctx, &bty, s);
			BGBCC_FrBC_EmitLoadInitArr(ctx, bty, dreg, def->value, l);

			BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
			BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_EmitMov(ctx,
			BGBCC_FrBC_GetRegType(ctx, dreg),
			dreg, sreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, dreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);

		return(FR2C_STATUS_YES);
	}

#if 0
	def=BGBCC_FrBC_LookupArgInfo(ctx, name);
	if(def)
	{
		i=BGBCC_FrBC_LookupAsRegister(ctx, name, &dreg);

		st=BGBCC_FrBC_LookupStructureForSig2(ctx, def->sig);
		if(st)
		{
			bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P);
			BGBCC_FrBC_EmitInitObj(ctx, bty, dreg, st);
			return(FR2C_STATUS_YES);
		}
	}
#endif

	def=BGBCC_FrBC_LookupGlobalInfo(ctx, name);
	if(def)
	{
		BGBCC_FrBC_PopRegister(ctx, &sreg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, sreg);
		return(FR2C_STATUS_NO);
	}

	BGBCC_FrBC_StubError(ctx);
	return(FR2C_STATUS_NO);
}
