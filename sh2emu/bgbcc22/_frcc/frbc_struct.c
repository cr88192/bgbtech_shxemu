#include <bgbccc.h>

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_LookupStructureForSig(
	BGBCC_FrCC_State *ctx, char *sig)
{
	char *s, *t;

	s=sig;
	while(*s && (*s=='P'))
		{ s++; }

	return(BGBCC_FrBC_LookupStructureForSig2(ctx, s));
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_LookupStructureForSig2(
	BGBCC_FrCC_State *ctx, char *sig)
{
	char tb[256];
	BGBCC_FrCC_LiteralInfo *cur;
	char *s, *t;
	int i;
	
	s=sig;
//	while(*s && (*s=='P'))
//		{ s++; }
	if((*s!='X') && (*s!='Y'))
		{ return(NULL); }
	s++;
		
	if((*s>='0') && (*s<='9'))
	{
		i=atoi(s);
		cur=ctx->literals[i];
		return(cur);
	}

	t=tb;
	while(*s && (*s!=';'))
		*t++=*s++;
	*t++=0;
	
	cur=BGBCC_FrBC_LookupStructure(ctx, tb);
	return(cur);
}

int BGBCC_FrBC_LookupStructureIDForSig(
	BGBCC_FrCC_State *ctx, char *sig)
{
	BGBCC_FrCC_LiteralInfo *cur;
	
	cur=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
	if(!cur)return(-1);
	return(cur->litid);
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_LookupStructureForType(
	BGBCC_FrCC_State *ctx, frbc_type type)
{
	BGBCC_FrCC_TypeOverflow ovf;
	BGBCC_FrCC_LiteralInfo *st;
	int bt;

	if((type.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		bt=type.val&FR2C_TY_BASEMASK;
		st=ctx->literals[bt-256];
		return(st);
	}

	if((type.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[type.val&FR2C_TYOVF_IDXMASK]);
//		bt=type.val&FR2C_TYB2_BASEMASK;
		bt=ovf.base;
		st=ctx->literals[bt-256];
		return(st);
	}
	
	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(NULL);
}

int BGBCC_FrBC_LookupStructureIDForType(
	BGBCC_FrCC_State *ctx, frbc_type type)
{
	BGBCC_FrCC_LiteralInfo *cur;
	
	cur=BGBCC_FrBC_LookupStructureForType(ctx, type);
	if(!cur)return(-1);
	return(cur->litid);
}

int BGBCC_FrBC_LookupStructContainsFieldID(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *st, char *name)
{
	BGBCC_FrCC_LiteralInfo *st2;
	int i, j;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
		{
			st2=BGBCC_FrBC_LookupStructureForType(ctx,
				st->decl->fields[i]->type);
			if(!st2)
				continue;
			
			j=BGBCC_FrBC_LookupStructContainsFieldID(ctx, st2, name);
			if(j>=0)return(i);

			continue;
		}

		if(!strcmp(st->decl->fields[i]->name, name))
			return(i);
	}
	return(-1);
}

int BGBCC_FrBC_LookupStructFieldID(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *st, char *name)
{
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
			continue;

		if(!strcmp(st->decl->fields[i]->name, name))
			return(i);
	}
	return(-1);
}

frbc_status BGBCC_FrBC_LookupStructFieldType(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *st, char *name,
	frbc_type *rty)
{
	frbc_type bty;
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
			continue;

		if(!strcmp(st->decl->fields[i]->name, name))
		{
			BGBCC_FrBC_TypeFromSig(ctx, &bty,
				st->decl->fields[i]->sig);
			*rty=bty;
			return(1);
		}
	}
	
//	printf("BGBCC_FrBC_LookupStructFieldType: No Field '%s'\n", name);
	bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);
//	*rty=bty;	
	return(0);
}

frbc_status BGBCC_FrBC_LookupStructFieldIdType(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *st, int idx,
	frbc_type *rty)
{
	frbc_type bty;
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	if((idx>=0) && (idx<st->decl->n_fields))
	{
		BGBCC_FrBC_TypeFromSig(ctx, &bty,
			st->decl->fields[idx]->sig);
		*rty=bty;
		return(1);
	}
	
	bty=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);
	return(-1);
}

frbc_status BGBCC_FrBC_EmitOffsetOf(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register dst,
	BGBCC_FrCC_LiteralInfo *st, char *name)
{
	int y, rt, op, fn;
	int i, j, k;
	
	fn=BGBCC_FrBC_LookupStructFieldID(ctx, st, name);
	if(fn<0)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		(y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_OFFSETOF_I);
		BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitWord(ctx, st->litid);
		BGBCC_FrBC_EmitByte(ctx, fn);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitLoadSlot(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src,
	BGBCC_FrCC_LiteralInfo *st, char *name)
{
	frbc_register treg;
	int z, y, rt, op, fn;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		rt=BGBCC_FrBC_EmitLoadSlot(ctx, type, treg, src, st, name);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, src, &treg);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, src);
		rt=BGBCC_FrBC_EmitLoadSlot(ctx, type, dst, treg, st, name);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	fn=BGBCC_FrBC_LookupStructFieldID(ctx, st, name);
	if(fn<0)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADSLOTOW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord24(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, fn);
		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(z>=0) && (z<16) && (y>=0) && (y<16) && (fn<256))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADSLOT);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, src));
//		BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitWord(ctx, st->litid);
		BGBCC_FrBC_EmitByte(ctx, fn);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		(z>=0) && (z<16) && (y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADSLOTW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx,
			 BGBCC_FrBC_GetRegID(ctx, src));
//		BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitWord24(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, fn);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitStoreSlot(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src,
	BGBCC_FrCC_LiteralInfo *st, char *name)
{
	frbc_register treg;
	int z, y, rt, op, fn;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		BGBCC_FrBC_EmitMovMatchDst(ctx, treg, dst);
		rt=BGBCC_FrBC_EmitStoreSlot(ctx, type, treg, src, st, name);
//		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}
	
	fn=BGBCC_FrBC_LookupStructFieldID(ctx, st, name);
	if(fn<0)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STORESLOTOW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord24(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, fn);
		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(z>=0) && (z<16) && (y>=0) && (y<16) && (fn<256))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STORESLOT);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, src));
//		BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitWord(ctx, st->litid);
		BGBCC_FrBC_EmitByte(ctx, fn);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		(z>=0) && (z<16) && (y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STORESLOTW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx,
			 BGBCC_FrBC_GetRegID(ctx, src));
//		BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitWord24(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, fn);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src) ||
		BGBCC_FrBC_IsRegImmP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		rt=BGBCC_FrBC_EmitStoreSlot(ctx, type, dst, treg, st, name);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}


frbc_status BGBCC_FrBC_EmitLoadSlotAddr(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src,
	BGBCC_FrCC_LiteralInfo *st, char *name)
{
	frbc_register treg;
	int z, y, rt, op, fn;
	int i, j, k;
	
	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		rt=BGBCC_FrBC_EmitLoadSlotAddr(ctx, type, treg, src, st, name);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, src, &treg);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, src);
		rt=BGBCC_FrBC_EmitLoadSlotAddr(ctx, type, dst, treg, st, name);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	fn=BGBCC_FrBC_LookupStructFieldID(ctx, st, name);
	if(fn<0)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(z>=0) && (z<16) && (y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADSLOTAW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);

		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord24(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, fn);

//		BGBCC_FrBC_EmitByte(ctx,
//			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
//			 BGBCC_FrBC_GetRegID(ctx, src));
//		BGBCC_FrBC_EmitWord(ctx, st->litid);
//		BGBCC_FrBC_EmitByte(ctx, fn);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitLoadSlotAddrID(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src,
	BGBCC_FrCC_LiteralInfo *st, int fn)
{
	frbc_register treg;
	int z, y, rt, op;
	int i, j, k;
	
	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		rt=BGBCC_FrBC_EmitLoadSlotAddrID(ctx, type, treg, src, st, fn);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, src, &treg);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, src);
		rt=BGBCC_FrBC_EmitLoadSlotAddrID(ctx, type, dst, treg, st, fn);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(z>=0) && (z<16) && (y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADSLOTAW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);

		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord24(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, fn);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}


frbc_status BGBCC_FrBC_EmitInitObj(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, BGBCC_FrCC_LiteralInfo *st)
{
	int z, y, w, rt, op, fn;
	int i, j, k;
	
//	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
//	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
	w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		(w>=0) && (w<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_INITOBJ);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|
			(BGBCC_FrBC_GetRegID(ctx, dst)&15));
		BGBCC_FrBC_EmitWord(ctx, st->litid);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitDropObj(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, BGBCC_FrCC_LiteralInfo *st)
{
	int z, y, w, rt, op, fn;
	int i, j, k;
	
//	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
//	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
	w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		(w>=0) && (w<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_DROPOBJ);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|
			(BGBCC_FrBC_GetRegID(ctx, dst)&15));
		BGBCC_FrBC_EmitWord(ctx, st->litid);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

int BGBCC_FrBC_GetArraySizeForSig(
	BGBCC_FrCC_State *ctx, char *sig)
{
	char *s, *t;
	s=sig;
	while(*s && (*s=='P'))
		{ s++; }
	return(BGBCC_FrBC_GetArraySizeForSig2(ctx, s));
}

int BGBCC_FrBC_GetArraySizeForSig2(
	BGBCC_FrCC_State *ctx, char *sig)
{
	return(BGBCC_FrBC_GetArraySizeForSig2R(ctx, &sig));
}

int BGBCC_FrBC_GetArraySizeForSig2R(
	BGBCC_FrCC_State *ctx, char **rsig)
{
	char *s, *t;
	int i;

	s=*rsig;
	if(*s=='A')
	{
		s++; i=0;
		while(*s && (*s>='0') && (*s<='9'))
			{ i=(i*10)+((*s++)-'0'); }
		if(*s==',')
			return(-1);		
		if(*s==';')
			s++;
		*rsig=s;
		return(i);
	}
	
	return(-1);
}

int BGBCC_FrBC_GetArraySizeForSig3R(
	BGBCC_FrCC_State *ctx, char **rsig)
{
	char *s, *t;
	int i, j;

	s=*rsig;
	if(*s=='A')
	{
		s++; i=1;
		while(*s)
		{
			j=0;
			while(*s && (*s>='0') && (*s<='9'))
				{ j=(j*10)+((*s++)-'0'); }
			if(*s==',')
			{
				i=i*j;
				s++;
				continue;
			}
			if(*s==';')
			{
				i=i*j;
				s++;
				break;
			}
			i=i*j;
			break;
		}
		
		*rsig=s;
		return(i);
	}
	
	return(-1);
}

frbc_status BGBCC_FrBC_GetStructSigFixedSize(
	BGBCC_FrCC_State *ctx, char *sig, int *rsz)
{
	BGBCC_FrCC_LiteralInfo *str;

	str=BGBCC_FrBC_LookupStructureForSig2(ctx, sig);
	if(str)
	{
		*rsz=str->decl->fxsize;
		return(0);
	}

	*rsz=0;
	return(-1);
}

frbc_status BGBCC_FrBC_GetSigFixedSize(
	BGBCC_FrCC_State *ctx, char *sig, int *rsz)
{
	int asz[16];
	char *s;
	int sz, na;
	int i, j, k;

	switch(*sig)
	{
	case 'a':	case 'b':
	case 'c':	case 'h':
		sz=1;	break;
	case 'i':	case 'j':
		sz=4;	break;
	case 'l':	case 'm':
		sz=4;	break;
	case 's':	case 't':
	case 'w':	case 'k':
		sz=2;	break;
	case 'x':	case 'y':
		sz=8;	break;
	case 'f':
		sz=4;	break;
	case 'd':
		sz=8;	break;
	case 'P':
		sz=4;	break;
	case 'A':
		s=sig+1; na=0; i=0;
		while(*s)
		{
			if((*s>='0') && (*s<='9'))
				{ i=i*10+((*s++)-'0'); continue; }
			if(*s==',')
				{ asz[na++]=i; i=0; s++; continue; }
			if(*s==';')
				{ asz[na++]=i; i=0; s++; break; }
			asz[na++]=i;
			break;
		}
		BGBCC_FrBC_GetSigFixedSize(ctx, s, &sz);
		j=1;
		for(i=0; i<na; i++)
			j*=asz[i];
		sz=j*sz;
		break;
	case 'X':
		BGBCC_FrBC_GetStructSigFixedSize(ctx, sig, &sz);
		break;
	default:
		sz=4;	break;
	}
	
	*rsz=sz;
	return(0);
}

frbc_status BGBCC_FrBC_EmitInitArr(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, int sz)
{
	int z, y, w, rt, op, fn;
	int i, j, k;
	
	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
//	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
//	w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);

	if(	BGBCC_FrBC_IsRegLocalBasicP(ctx, dst) &&
		(z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_INITARR);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|
			(BGBCC_FrBC_GetRegID(ctx, dst)&15));
		BGBCC_FrBC_EmitWord(ctx, sz);
		return(FR2C_STATUS_YES);
	}

	y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);
	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		(y>=0) && (y<16) && (z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_INITARRW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|(y&15));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitDWord(ctx, sz);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitInitObjArr(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, BGBCC_FrCC_LiteralInfo *st, int sz)
{
	int z, y, w, rt, op, fn;
	int i, j, k;
	
//	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
//	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
	w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		(w>=0) && (w<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_INITOBJARR);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|
			(BGBCC_FrBC_GetRegID(ctx, dst)&15));
		BGBCC_FrBC_EmitWord(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, sz);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitLoadInitArr(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register val, int sz)
{
	int z, y, w, rt, op, fn;
	int i, j, k;
	
	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);

	if(	BGBCC_FrBC_IsRegLocalBasicP(ctx, dst) &&
		(z>=0) && (z<16))
	{
		k=BGBCC_FrBC_GetRegID(ctx, val);
	
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDINITARR);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|
			(BGBCC_FrBC_GetRegID(ctx, dst)&15));
		BGBCC_FrBC_EmitWord(ctx, sz);
		BGBCC_FrBC_EmitWord(ctx, k);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitLoadInitObjArr(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, BGBCC_FrCC_LiteralInfo *st,
	frbc_register val, int sz)
{
	int z, y, w, rt, op, fn;
	int i, j, k;
	
//	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
//	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
	w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		(w>=0) && (w<16))
	{
		k=BGBCC_FrBC_GetRegID(ctx, val);

		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDINITOBJARR);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|
			(BGBCC_FrBC_GetRegID(ctx, dst)&15));
		BGBCC_FrBC_EmitWord(ctx, st->litid);
		BGBCC_FrBC_EmitWord(ctx, sz);
		BGBCC_FrBC_EmitWord(ctx, k);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

char *BGBCC_FrBC_SigGetReturnSig(
	BGBCC_FrCC_State *ctx, char *sig)
{
	char *s;
	int i;
	
	s=sig;
	while(*s=='P')s++;
	if(*s=='(')
	{
		i=0;
		while(*s)
		{
			if(*s=='(') { i++; s++; continue; }
			if(*s==')')
			{
				i--; s++;
				if(!i)break;
				continue;
			}
			s++;
		}
		return(s);
	}
	
	return(sig);
}


frbc_status BGBCC_FrBC_GetStructSigMinMaxSize(
	BGBCC_FrCC_State *ctx, char *sig, int *rsz, int *ral)
{
	BGBCC_FrCC_LiteralInfo *str;

	str=BGBCC_FrBC_LookupStructureForSig2(ctx, sig);
	if(str && str->decl)
	{
		rsz[0]=str->decl->fxmsize;
		rsz[1]=str->decl->fxnsize;
		ral[0]=str->decl->fxmalgn;
		ral[1]=str->decl->fxnalgn;
		return(0);
	}

	rsz[0]=0;	rsz[1]=0;
	ral[0]=1;	ral[1]=1;
	return(-1);
}

frbc_status BGBCC_FrBC_GetSigMinMaxSize(
	BGBCC_FrCC_State *ctx, char *sig, int *rsz, int *ral)
{
	int asz[16];
	char *s;
	int sza[2], ala[2];
	int sz, na, ret;
	int i, j, k;

	ret=0;
	switch(*sig)
	{
	case 'a':	case 'b':
	case 'c':	case 'h':
		sza[0]=1; sza[1]=1;
		ala[0]=1; ala[1]=1;
		break;
	case 'i':	case 'j':
		sza[0]=4; sza[1]=4;
		ala[0]=4; ala[1]=4;
		break;
	case 'l':	case 'm':
		sza[0]=4; sza[1]=8;
		ala[0]=4; ala[1]=8;
		break;
	case 's':	case 't':
	case 'w':	case 'k':
		sza[0]=2; sza[1]=2;
		ala[0]=2; ala[1]=2;
		break;
	case 'x':	case 'y':
		sza[0]=8; sza[1]=8;
		ala[0]=8; ala[1]=8;
		break;
	case 'f':
		sza[0]=4; sza[1]=4;
		ala[0]=4; ala[1]=4;
		break;
	case 'd':
		sza[0]=8; sza[1]=8;
		ala[0]=8; ala[1]=8;
		break;
	case 'P':
		sza[0]=4; sza[1]=8;
		ala[0]=4; ala[1]=8;
		break;
	case 'A':
		s=sig+1; na=0; i=0;
		while(*s)
		{
			if((*s>='0') && (*s<='9'))
				{ i=i*10+((*s++)-'0'); continue; }
			if(*s==',')
				{ asz[na++]=i; i=0; s++; continue; }
			if(*s==';')
				{ asz[na++]=i; i=0; s++; break; }
			asz[na++]=i;
			break;
		}
		BGBCC_FrBC_GetSigMinMaxSize(ctx, s, sza, ala);
		j=1;
		for(i=0; i<na; i++)
			j*=asz[i];
		sza[0]=j*sza[0];
		sza[1]=j*sza[1];
		break;
	case 'X':
		ret=BGBCC_FrBC_GetStructSigMinMaxSize(ctx, sig, sza, ala);
		break;
	default:
		sza[0]=0; sza[1]=0;
		ala[0]=1; ala[1]=1;
		ret=-1;
		break;
	}
	
	rsz[0]=sza[0];
	rsz[1]=sza[1];
	ral[0]=ala[0];
	ral[1]=ala[1];
	return(ret);
}

frbc_status BGBCC_FrBC_MarkTypeAccessed(
	BGBCC_FrCC_State *ctx, frbc_type type)
{
	BGBCC_FrCC_LiteralInfo *obj;
	int i, j, k, n;

	i=BGBCC_FrBC_TypeObjectLiteralIndex(ctx, type);
	if(i<0)return(0);
	obj=ctx->literals[i];
	if(!obj || !obj->decl)
		return(0);

	if(obj->decl->regflags&BGBCC_REGFL_ACCESSED)
		return(0);

	obj->decl->regflags|=BGBCC_REGFL_ACCESSED;

	for(i=0; i<obj->decl->n_args; i++)
		{ BGBCC_FrBC_MarkTypeAccessed(ctx, obj->decl->args[i]->type); }
	for(i=0; i<obj->decl->n_locals; i++)
		{ BGBCC_FrBC_MarkTypeAccessed(ctx, obj->decl->locals[i]->type); }

	n=obj->decl->n_fields;
	for(i=0; i<n; i++)
	{
		BGBCC_FrBC_MarkTypeAccessed(ctx,
			obj->decl->fields[i]->type);
	}

	return(1);
}
