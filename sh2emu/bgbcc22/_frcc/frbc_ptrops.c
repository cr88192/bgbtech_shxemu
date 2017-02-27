#include <bgbccc.h>

frbc_status BGBCC_FrBC_EmitLoadIndexImm(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register dst,
	frbc_register src, int idx)
{
	frbc_register treg;
	int z, y, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		rt=BGBCC_FrBC_EmitLoadIndexImm(ctx, type, treg, src, idx);
//		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, src, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, src);
		rt=BGBCC_FrBC_EmitLoadIndexImm(ctx, type, dst, treg, idx);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

//	if(BGBCC_FrBC_IsRegImmZeroP(ctx, srcb))
//	{
//		rt=BGBCC_FrBC_EmitJumpRegZero(ctx, type, opr, srca, name);
//		if(rt>=0)return(rt);
//	}
	
	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=FR2C_TY_S;
//		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADSLOTOW);
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADINDEXOW_C);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitDWord(ctx, idx);
		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(idx>=(-2048)) && (idx<2048) &&
		(y>=0) && (y<16))
	{
		op=-1;
		if(BGBCC_FrBC_TypeIntP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_IC; }
		if(BGBCC_FrBC_TypeLongP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_LC; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_FC; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_DC; }
		if(BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_PC; }

		if(BGBCC_FrBC_TypeUnsignedIntP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_IC; }
		if(BGBCC_FrBC_TypeUnsignedLongP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_LC; }

		if(BGBCC_FrBC_TypeSignedByteP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_SBC; }
		if(BGBCC_FrBC_TypeUnsignedByteP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_UBC; }
		if(BGBCC_FrBC_TypeSignedShortP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_SSC; }
		if(BGBCC_FrBC_TypeUnsignedShortP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_USC; }

		if(op>=0)
		{
			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, src)<<4)|(idx>>8)&15);
			BGBCC_FrBC_EmitByte(ctx, idx&255);

			return(FR2C_STATUS_YES);
		}
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		(y>=0) && (y<16) && (z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADINDEXW_C);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitDWord(ctx, idx);

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitStoreIndexImm(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register dst,
	frbc_register src, int idx)
{
	frbc_register treg;
	int z, y, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		rt=BGBCC_FrBC_EmitStoreIndexImm(ctx, type, treg, src, idx);
//		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, src, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, src);
		rt=BGBCC_FrBC_EmitStoreIndexImm(ctx, type, dst, treg, idx);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

//	if(BGBCC_FrBC_IsRegImmZeroP(ctx, srcb))
//	{
//		rt=BGBCC_FrBC_EmitJumpRegZero(ctx, type, opr, srca, name);
//		if(rt>=0)return(rt);
//	}

	if(BGBCC_FrBC_IsRegImmP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		rt=BGBCC_FrBC_EmitStoreIndexImm(ctx, type, dst, treg, idx);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);

		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}
	
	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=FR2C_TY_S;
//		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADSLOTOW);
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STOREINDEXOW_C);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitDWord(ctx, idx);
		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(idx>=(-2048)) && (idx<2048) &&
		(y>=0) && (y<16))
	{
		op=-1;
		if(BGBCC_FrBC_TypeIntP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_IC; }
		if(BGBCC_FrBC_TypeLongP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_LC; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_FC; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_DC; }
		if(BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_PC; }

		if(BGBCC_FrBC_TypeUnsignedIntP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_IC; }
		if(BGBCC_FrBC_TypeUnsignedLongP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_LC; }

		if(BGBCC_FrBC_TypeSignedByteP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_BC; }
		if(BGBCC_FrBC_TypeUnsignedByteP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_BC; }
		if(BGBCC_FrBC_TypeSignedShortP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_SC; }
		if(BGBCC_FrBC_TypeUnsignedShortP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_SC; }

		if(op>=0)
		{
			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, src)<<4)|(idx>>8)&15);
			BGBCC_FrBC_EmitByte(ctx, idx&255);

			return(FR2C_STATUS_YES);
		}
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		(y>=0) && (y<16) && (z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STOREINDEXW_C);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitDWord(ctx, idx);

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitLoadIndex(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register dst,
	frbc_register srca, frbc_register srcb)
{
	frbc_register treg;
	frbc_type tty;
	int z, y, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		rt=BGBCC_FrBC_EmitLoadIndex(ctx, type, treg, srca, srcb);
//		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srca))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srca, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srca);
		rt=BGBCC_FrBC_EmitLoadIndex(ctx, type, dst, treg, srcb);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srcb, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srcb);
		rt=BGBCC_FrBC_EmitLoadIndex(ctx, type, dst, srca, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

//	if(BGBCC_FrBC_IsRegImmZeroP(ctx, srcb))
//	{
//		rt=BGBCC_FrBC_EmitJumpRegZero(ctx, type, opr, srca, name);
//		if(rt>=0)return(rt);
//	}
	
	if(BGBCC_FrBC_IsRegImmIntP(ctx, srcb))
	{
		i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);
		rt=BGBCC_FrBC_EmitLoadIndexImm(ctx, type, dst, srca, i);
		return(rt);
	}
	
	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode3(ctx, dst, srca, srcb);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y<0))
	{
		tty=BGBCC_FrBC_GetRegType(ctx, dst);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		BGBCC_FrBC_EmitLoadIndex(ctx, type, treg, srca, srcb);
		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADINDEXOW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));

		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srcb) &&
		(y>=0) && (y<16))
	{
		op=-1;
		if(BGBCC_FrBC_TypeIntP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_I; }
		if(BGBCC_FrBC_TypeLongP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_L; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_F; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_D; }
		if(BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_P; }

		if(BGBCC_FrBC_TypeUnsignedIntP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_I; }
		if(BGBCC_FrBC_TypeUnsignedLongP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_L; }

		if(BGBCC_FrBC_TypeSignedByteP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_SB; }
		if(BGBCC_FrBC_TypeUnsignedByteP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_UB; }
		if(BGBCC_FrBC_TypeSignedShortP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_SS; }
		if(BGBCC_FrBC_TypeUnsignedShortP(ctx, type))
			{ op=FR2C_OP_LOADINDEX_US; }

		if(op>=0)
		{
			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
				BGBCC_FrBC_GetRegID(ctx, srcb));

			return(FR2C_STATUS_YES);
		}
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	
	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		(z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LOADINDEXW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitStoreIndex(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register dst,
	frbc_register srca, frbc_register srcb)
{
	frbc_register treg;
	frbc_type tty;
	int z, y, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, dst);
		rt=BGBCC_FrBC_EmitStoreIndex(ctx, type, treg, srca, srcb);
//		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srca))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srca, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srca);
		rt=BGBCC_FrBC_EmitStoreIndex(ctx, type, dst, treg, srcb);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srcb, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srcb);
		rt=BGBCC_FrBC_EmitStoreIndex(ctx, type, dst, srca, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

//	if(BGBCC_FrBC_IsRegImmZeroP(ctx, srcb))
//	{
//		rt=BGBCC_FrBC_EmitJumpRegZero(ctx, type, opr, srca, name);
//		if(rt>=0)return(rt);
//	}
	
	if(BGBCC_FrBC_IsRegImmP(ctx, srcb))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
		rt=BGBCC_FrBC_EmitStoreIndex(ctx, type, dst, srca, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);

		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	if(BGBCC_FrBC_IsRegImmIntP(ctx, srca))
	{
		i=BGBCC_FrBC_GetRegImmIntValue(ctx, srca);
		rt=BGBCC_FrBC_EmitStoreIndexImm(ctx, type, dst, srcb, i);
		return(rt);
	}
	
	y=BGBCC_FrBC_GetRegOprMode3(ctx, dst, srca, srcb);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y<0))
	{
//		tty=BGBCC_FrBC_GetRegType(ctx, dst);
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, dst);
		BGBCC_FrBC_EmitStoreIndex(ctx, type, treg, srca, srcb);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16))
	{
		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STOREINDEXOW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));

		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srcb) &&
		(y>=0) && (y<16))
	{
		op=-1;
		if(BGBCC_FrBC_TypeIntP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_I; }
		if(BGBCC_FrBC_TypeLongP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_L; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_F; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_D; }
		if(BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_P; }

		if(BGBCC_FrBC_TypeUnsignedIntP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_I; }
		if(BGBCC_FrBC_TypeUnsignedLongP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_L; }

		if(BGBCC_FrBC_TypeSignedByteP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_B; }
		if(BGBCC_FrBC_TypeUnsignedByteP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_B; }
		if(BGBCC_FrBC_TypeSignedShortP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_S; }
		if(BGBCC_FrBC_TypeUnsignedShortP(ctx, type))
			{ op=FR2C_OP_STOREINDEX_S; }

		if(op>=0)
		{
			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
				BGBCC_FrBC_GetRegID(ctx, srcb));

			return(FR2C_STATUS_YES);
		}
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	
	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		(z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_STOREINDEXW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitLeaImm(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register dst,
	frbc_register src, int idx)
{
	frbc_register treg;
	frbc_type tty;
	int z, y, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		rt=BGBCC_FrBC_EmitLeaImm(ctx, type, treg, src, idx);
//		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, src, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, src);
		rt=BGBCC_FrBC_EmitLeaImm(ctx, type, dst, treg, idx);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}
	
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LEAOW_C);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitDWord(ctx, idx);
		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(idx>=(-2048)) && (idx<2048) &&
		(y>=0) && (y<16))
	{
		op=-1;
		if(BGBCC_FrBC_TypeIntP(ctx, type))
			{ op=FR2C_OP_LEA_IC; }
		if(BGBCC_FrBC_TypeLongP(ctx, type))
			{ op=FR2C_OP_LEA_LC; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_LEA_FC; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_LEA_DC; }
		if(BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_LEA_PC; }

		if(BGBCC_FrBC_TypeUnsignedIntP(ctx, type))
			{ op=FR2C_OP_LEA_IC; }
		if(BGBCC_FrBC_TypeUnsignedLongP(ctx, type))
			{ op=FR2C_OP_LEA_LC; }

		if(BGBCC_FrBC_TypeSignedByteP(ctx, type))
			{ op=FR2C_OP_LEA_BC; }
		if(BGBCC_FrBC_TypeUnsignedByteP(ctx, type))
			{ op=FR2C_OP_LEA_BC; }
		if(BGBCC_FrBC_TypeSignedShortP(ctx, type))
			{ op=FR2C_OP_LEA_SC; }
		if(BGBCC_FrBC_TypeUnsignedShortP(ctx, type))
			{ op=FR2C_OP_LEA_SC; }

		if(op>=0)
		{
			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, src)<<4)|(idx>>8)&15);
			BGBCC_FrBC_EmitByte(ctx, idx&255);

			return(FR2C_STATUS_YES);
		}
	}

	z=BGBCC_FrBC_TypeAsOprBasic(ctx, type);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		(z>=0) && (z<16) && (y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LEAW_C);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitDWord(ctx, idx);

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitLea(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register dst,
	frbc_register srca, frbc_register srcb)
{
	frbc_register treg;
	frbc_type tty;
	int z, y, rt, op;
	int i, j, k;
	
	if(BGBCC_FrBC_IsRegImmIntP(ctx, srcb))
	{
		i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);
		rt=BGBCC_FrBC_EmitLeaImm(ctx, type, dst, srca, i);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		rt=BGBCC_FrBC_EmitLea(ctx, type, treg, srca, srcb);
//		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srca))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srca, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srca);
		rt=BGBCC_FrBC_EmitLea(ctx, type, dst, treg, srcb);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb))
	{
//		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srcb, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srcb);
		rt=BGBCC_FrBC_EmitLea(ctx, type, dst, srca, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	y=BGBCC_FrBC_GetRegOprMode3(ctx, dst, srca, srcb);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y<0))
	{
		tty=BGBCC_FrBC_GetRegType(ctx, dst);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		BGBCC_FrBC_EmitLea(ctx, type, treg, srca, srcb);
//		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LEAOW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));

		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srcb) &&
		(y>=0) && (y<16))
	{
		op=-1;
		if(BGBCC_FrBC_TypeIntP(ctx, type))
			{ op=FR2C_OP_LEA_I; }
		if(BGBCC_FrBC_TypeLongP(ctx, type))
			{ op=FR2C_OP_LEA_L; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_LEA_F; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_LEA_D; }
		if(BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_LEA_P; }

		if(BGBCC_FrBC_TypeUnsignedIntP(ctx, type))
			{ op=FR2C_OP_LEA_I; }
		if(BGBCC_FrBC_TypeUnsignedLongP(ctx, type))
			{ op=FR2C_OP_LEA_L; }

		if(BGBCC_FrBC_TypeSignedByteP(ctx, type))
			{ op=FR2C_OP_LEA_B; }
		if(BGBCC_FrBC_TypeUnsignedByteP(ctx, type))
			{ op=FR2C_OP_LEA_B; }
		if(BGBCC_FrBC_TypeSignedShortP(ctx, type))
			{ op=FR2C_OP_LEA_S; }
		if(BGBCC_FrBC_TypeUnsignedShortP(ctx, type))
			{ op=FR2C_OP_LEA_S; }

#if 0
		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}
#endif

		if(op>=0)
		{
			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
				BGBCC_FrBC_GetRegID(ctx, srcb));

			return(FR2C_STATUS_YES);
		}
	}

	z=BGBCC_FrBC_TypeAsOprBasic(ctx, type);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) && (z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LEAW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}


frbc_status BGBCC_FrBC_EmitLdaVar(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src)
{
	frbc_register treg;
	frbc_type tty;
	int y, w, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
		w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			(w>=0) && (w<16))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDAGVAR);
			BGBCC_FrBC_EmitByte(ctx, (w<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitWord(ctx, BGBCC_FrBC_GetRegID(ctx, src));

			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}
	
	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		(y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDAVAR);
		BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitSizeofVar(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst)
{
	BGBCC_FrCC_LiteralInfo *linf;
	frbc_register treg;
	frbc_type tty;
	char *s;
	int y;
	int i, j, k;

	y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);

	BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_SIZEOF_I);
	BGBCC_FrBC_EmitByte(ctx, (y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));

	s=BGBCC_FrBC_TypeGetSig(ctx, type);
	linf=BGBCC_FrBC_GetTypedef(ctx, NULL, s);
	i=linf->litid;
	
	BGBCC_FrBC_EmitWord(ctx, i);
	BGBCC_FrBC_EmitRelocLiteral16(ctx);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_EmitDiffPtr(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register srca, frbc_register srcb)
{
	frbc_register treg;
	frbc_type tty;
	int z, y, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, dst, &treg);
		rt=BGBCC_FrBC_EmitDiffPtr(ctx, type, treg, srca, srcb);
		BGBCC_FrBC_EmitMovMatchDst(ctx, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srca))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srca, &treg);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srca);
		rt=BGBCC_FrBC_EmitDiffPtr(ctx, type, dst, treg, srcb);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb))
	{
		BGBCC_FrBC_RegisterAllocTemporaryMatch(ctx, srcb, &treg);
		BGBCC_FrBC_EmitMovMatchSrc(ctx, treg, srcb);
		rt=BGBCC_FrBC_EmitDiffPtr(ctx, type, dst, srca, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	z=BGBCC_FrBC_TypeAsOprBasic(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode3(ctx, dst, srca, srcb);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) && (z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_DIFFPTR);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));

		return(FR2C_STATUS_YES);
	}

	k=BGBCC_FrBC_TypeObjectLiteralIndex(ctx, type);
	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) && (k>=0) && (k<16))
	{
		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_DIFFPTROBJ);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));
		BGBCC_FrBC_EmitWord24(ctx, k);

		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}
