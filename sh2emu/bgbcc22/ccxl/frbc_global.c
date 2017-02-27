#include <bgbccc.h>

void BGBCC_FrBC_CheckExpandGlobals(
	BGBCC_FrCC_State *ctx)
{
}

int BGBCC_FrBC_HashName(char *name)
{
	char *s;
	int hi;
	
	s=name; hi=0;
	while(*s)hi=(hi*251)+(*s++);
	hi=((hi*251)>>8)&4095;
	return(hi);
}

BGBCC_FrCC_RegisterInfo *BGBCC_FrBC_LookupGlobal(
	BGBCC_FrCC_State *ctx, char *name)
{
//	BGBCC_FrCC_RegisterInfo *cur, *nxt, *lst2, *sel;
	BGBCC_FrCC_RegisterInfo *cur, *nxt;
	BGBCC_FrCC_RegisterInfo *llst;
	BGBCC_FrCC_RegisterInfo *sel;
	int i;

	i=BGBCC_FrBC_HashName(name);
	cur=ctx->hash_globals[i];
	while(cur)
	{
		if(!strcmp(cur->name, name))
			return(cur);
		cur=cur->hashnext;
	}

	/* deal with unsorted globals */
	if(ctx->usort_globals)
	{
		cur=ctx->usort_globals; llst=NULL; sel=NULL;
		while(cur)
		{
			nxt=cur->hashnext;
			if(!cur->name)
			{
				cur->hashnext=llst;
				llst=cur;
				cur=nxt;
				continue;
			}
		
			if(!sel && !strcmp(cur->name, name))
				sel=cur;

			i=BGBCC_FrBC_HashName(cur->name);
			cur->hashnext=ctx->hash_globals[i];
			ctx->hash_globals[i]=cur;
			cur=nxt;
		}

		ctx->usort_globals=llst;
		
		if(sel)
			return(sel);
	}

	return(NULL);

#if 0
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		cur=ctx->reg_globals[i];
		if(!cur)
			continue;
		if(!strcmp(cur->name, name))
			return(cur);
	}
	return(NULL);
#endif
}

BGBCC_FrCC_RegisterInfo *BGBCC_FrBC_GetGlobal(
	BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrCC_RegisterInfo *cur;
	int i, h;
	
	cur=BGBCC_FrBC_LookupGlobal(ctx, name);
	if(cur)return(cur);
	
	BGBCC_FrBC_CheckExpandGlobals(ctx);
	
	cur=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
	cur->name=bgbcc_strdup(name);
	
	i=ctx->n_reg_globals++;
	ctx->reg_globals[i]=cur;
	cur->regid=i;

	h=BGBCC_FrBC_HashName(name);
	cur->hashnext=ctx->hash_globals[h];
	ctx->hash_globals[h]=cur;
	
	return(cur);
}

frbc_status BGBCC_FrBC_EmitLoadGlobal(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src)
{
	int z, y, w, g, rt;

	w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);
	g=BGBCC_FrBC_GetRegID(ctx, src);

	if(BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		(w>=0) && (g<65536))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDGVAR);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|
			BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitWord(ctx, g);
		return(FR2C_STATUS_YES);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);

	if(BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		(w>=0) && (g<16777216))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDGVARW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitWord24(ctx, g);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitStoreGlobal(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src)
{
	frbc_register treg;
	int z, y, w, g, rt;

	w=BGBCC_FrBC_GetRegOprModeW(ctx, src, type);
	g=BGBCC_FrBC_GetRegID(ctx, dst);

	if(BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(w>=0) && (g<65536))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STGVAR);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|
			BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord(ctx, g);
		return(FR2C_STATUS_YES);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode1(ctx, src);

	if(BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		(w>=0) && (g<16777216))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STGVARW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord24(ctx, g);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src) ||
		BGBCC_FrBC_IsRegImmP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		rt=BGBCC_FrBC_EmitStoreGlobal(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}
