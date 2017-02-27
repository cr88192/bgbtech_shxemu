#include <bgbccc.h>

frbc_status BGBCC_FrBC_EmitCallOp(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src, int na)
{
	int z, y, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
		k=BGBCC_FrBC_GetRegID(ctx, src);
		
		if((na<16) && (k<4096))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CALL2);
			BGBCC_FrBC_EmitByte(ctx,
				(na<<4)|((k>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, (k   )&255);
			BGBCC_FrBC_EmitRelocGlobal12(ctx);
			return(FR2C_STATUS_YES);
		}
		
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CALL);
		BGBCC_FrBC_EmitByte(ctx, (k>>16)&255);
		BGBCC_FrBC_EmitByte(ctx, (k>> 8)&255);
		BGBCC_FrBC_EmitByte(ctx, (k    )&255);
		BGBCC_FrBC_EmitRelocGlobal24(ctx);
		BGBCC_FrBC_EmitByte(ctx, na);
		return(FR2C_STATUS_YES);
	}

	y=BGBCC_FrBC_GetRegOprMode1(ctx, src);
	if(BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		BGBCC_FrBC_IsRegPointerP(ctx, src) &&
		(y>=0) && (y<16))
	{
		k=BGBCC_FrBC_GetRegID(ctx, src);
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CALLP);
		BGBCC_FrBC_EmitByte(ctx, (y<<4)|k);
		BGBCC_FrBC_EmitByte(ctx, na);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		BGBCC_FrBC_IsRegPointerP(ctx, src) &&
		(y>=0) && (y<16))
	{
		z=FR2C_TY_P;
		k=BGBCC_FrBC_GetRegID(ctx, src);
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CALLPW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, k);
		BGBCC_FrBC_EmitByte(ctx, na);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitCallCsrvOp(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src)
{
	int z, y, w, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_TypeVoidP(ctx, type))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CSRV_V);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
		y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);
		j=BGBCC_FrBC_GetRegID(ctx, dst);
		k=BGBCC_FrBC_LookupStructureIDForType(ctx, type);

		if((y>=0) && (y<16) && (j>=0) && (j<16) &&
			(k<65536))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CSRVOBJ);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|j);
			BGBCC_FrBC_EmitWord(ctx, k);
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CSRVOBJW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, j);
		BGBCC_FrBC_EmitWord24(ctx, k);
		return(FR2C_STATUS_YES);
	}

	w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);
	j=BGBCC_FrBC_GetRegID(ctx, dst);
//	k=BGBCC_FrBC_GetRegID(ctx, src);

	if((w>=0) && (w<16) && (j>=0) && (j<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CSRV);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|(j&15));
		return(FR2C_STATUS_YES);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);
	if((z>=0) && (z<16) && (y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CSRVW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|(y&15));
		BGBCC_FrBC_EmitByte(ctx, j&255);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitCallArg(
	BGBCC_FrCC_State *ctx, frbc_register reg)
{
	frbc_type ty;
	s64 li;
	double lf;
	int i, j, k;

	ty=BGBCC_FrBC_GetRegType(ctx, reg);
	
	if(BGBCC_FrBC_IsRegBasicP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegID(ctx, reg);
		
		if(BGBCC_FrBC_TypeSmallIntP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x00|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeSmallLongP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x10|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeFloatP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x20|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeDoubleP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x30|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypePointerP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x40|i); return(FR2C_STATUS_YES); }

		if(BGBCC_FrBC_TypeArrayP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x40|i); return(FR2C_STATUS_YES); }

		if(BGBCC_FrBC_TypeValueObjectP(ctx, ty))
		{
			j=BGBCC_FrBC_GetTypeBaseType(ctx, ty);
			if(j>=256)
			{
				BGBCC_FrBC_EmitByte(ctx, 0xCE);
				BGBCC_FrBC_EmitByte(ctx, 0x00|i);
				BGBCC_FrBC_EmitWord(ctx, j-256);
				return(FR2C_STATUS_YES);
			}
		}
	}

	if(BGBCC_FrBC_IsRegLocalBasicP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegID(ctx, reg);
		
		if(BGBCC_FrBC_TypeSmallIntP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x50|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeSmallLongP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x60|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeFloatP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x70|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeDoubleP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x80|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypePointerP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x90|i); return(FR2C_STATUS_YES); }

		if(BGBCC_FrBC_TypeArrayP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0x90|i); return(FR2C_STATUS_YES); }

		if(BGBCC_FrBC_TypeValueObjectP(ctx, ty))
		{
			j=BGBCC_FrBC_GetTypeBaseType(ctx, ty);
			if(j>=256)
			{
				BGBCC_FrBC_EmitByte(ctx, 0xCE);
				BGBCC_FrBC_EmitByte(ctx, 0x20|i);
				BGBCC_FrBC_EmitWord(ctx, j-256);
				return(FR2C_STATUS_YES);
			}
		}
	}

	if(BGBCC_FrBC_IsRegArgBasicP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegID(ctx, reg);
		
		if(BGBCC_FrBC_TypeSmallIntP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0xD0|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeDoubleP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0xE0|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypePointerP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0xF0|i); return(FR2C_STATUS_YES); }
		if(BGBCC_FrBC_TypeArrayP(ctx, ty))
			{ BGBCC_FrBC_EmitByte(ctx, 0xF0|i); return(FR2C_STATUS_YES); }

		if(BGBCC_FrBC_TypeValueObjectP(ctx, ty))
		{
			j=BGBCC_FrBC_GetTypeBaseType(ctx, ty);
			if(j>=256)
			{
				BGBCC_FrBC_EmitByte(ctx, 0xCE);
				BGBCC_FrBC_EmitByte(ctx, 0x10|i);
				BGBCC_FrBC_EmitWord(ctx, j-256);
				return(FR2C_STATUS_YES);
			}
		}
	}
	
	if(BGBCC_FrBC_IsRegRegExtP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegID(ctx, reg);

		j=-1;
		if((j<0) && BGBCC_FrBC_TypeSmallIntP(ctx, ty))	j=0;
		if((j<0) && BGBCC_FrBC_TypeSmallLongP(ctx, ty))	j=1;
		if((j<0) && BGBCC_FrBC_TypeFloatP(ctx, ty))		j=2;
		if((j<0) && BGBCC_FrBC_TypeDoubleP(ctx, ty))	j=3;
		if((j<0) && BGBCC_FrBC_TypePointerP(ctx, ty))	j=4;
		if((j<0) && BGBCC_FrBC_TypeArrayP(ctx, ty))		j=4;

		if(j>=0)
		{
			BGBCC_FrBC_EmitByte(ctx, 0xA0+j);
			BGBCC_FrBC_EmitByte(ctx, i);
			return(FR2C_STATUS_YES);
		}
	}

	if(BGBCC_FrBC_IsRegArgExtP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegID(ctx, reg);

		j=-1;
		if((j<0) && BGBCC_FrBC_TypeSmallIntP(ctx, ty))	j=0;
		if((j<0) && BGBCC_FrBC_TypeSmallLongP(ctx, ty))	j=1;
		if((j<0) && BGBCC_FrBC_TypeFloatP(ctx, ty))		j=2;
		if((j<0) && BGBCC_FrBC_TypeDoubleP(ctx, ty))	j=3;
		if((j<0) && BGBCC_FrBC_TypePointerP(ctx, ty))	j=4;
		if((j<0) && BGBCC_FrBC_TypeArrayP(ctx, ty))		j=4;

		if(j>=0)
		{
			BGBCC_FrBC_EmitByte(ctx, 0xA5+j);
			BGBCC_FrBC_EmitByte(ctx, i);
			return(FR2C_STATUS_YES);
		}
	}

	if(BGBCC_FrBC_IsRegLocalExtP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegID(ctx, reg);

		j=-1;
		if((j<0) && BGBCC_FrBC_TypeSmallIntP(ctx, ty))	j=0;
		if((j<0) && BGBCC_FrBC_TypeSmallLongP(ctx, ty))	j=1;
		if((j<0) && BGBCC_FrBC_TypeFloatP(ctx, ty))		j=2;
		if((j<0) && BGBCC_FrBC_TypeDoubleP(ctx, ty))	j=3;
		if((j<0) && BGBCC_FrBC_TypePointerP(ctx, ty))	j=4;
		if((j<0) && BGBCC_FrBC_TypeArrayP(ctx, ty))		j=4;

		if(j>=0)
		{
			BGBCC_FrBC_EmitByte(ctx, 0xAA+j);
			BGBCC_FrBC_EmitByte(ctx, i);
			return(FR2C_STATUS_YES);
		}
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegID(ctx, reg);

		j=-1;
		if((j<0) && BGBCC_FrBC_TypeSmallIntP(ctx, ty))	j=0;
		if((j<0) && BGBCC_FrBC_TypeSmallLongP(ctx, ty))	j=1;
		if((j<0) && BGBCC_FrBC_TypeFloatP(ctx, ty))		j=2;
		if((j<0) && BGBCC_FrBC_TypeDoubleP(ctx, ty))	j=3;
		if((j<0) && BGBCC_FrBC_TypePointerP(ctx, ty))	j=4;
		if((j<0) && BGBCC_FrBC_TypeArrayP(ctx, ty))		j=4;

		if(j>=0)
		{
			if((i>=0)&&(i<4096))
			{
				BGBCC_FrBC_EmitByte(ctx, 0xBF);
				BGBCC_FrBC_EmitByte(ctx,
					(0x90+(j<<4))|
					((i>>8)&15));
				BGBCC_FrBC_EmitByte(ctx, i&255);
				return(FR2C_STATUS_YES);
			}

			BGBCC_FrBC_EmitByte(ctx, 0xB9+j);
			BGBCC_FrBC_EmitByte(ctx, (i>>16)&255);
			BGBCC_FrBC_EmitByte(ctx, (i>> 8)&255);
			BGBCC_FrBC_EmitByte(ctx, (i    )&255);
			return(FR2C_STATUS_YES);
		}
	}
	
	if(BGBCC_FrBC_IsRegImmIntP(ctx, reg))
	{
		i=BGBCC_FrBC_GetRegImmIntValue(ctx, reg);
		if((i>=(-128)) && (i<128))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xB0);
			BGBCC_FrBC_EmitByte(ctx, i&255);
			return(FR2C_STATUS_YES);
		}

		if((i>=(-32768)) && (i<32768))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xB2);
			BGBCC_FrBC_EmitByte(ctx, (i>> 8)&255);
			BGBCC_FrBC_EmitByte(ctx, (i    )&255);
			return(FR2C_STATUS_YES);
		}
		
		j=BGBCC_FrBC_IndexLitS32(ctx, i);
		if((j>=0)&&(j<4096))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xBF);
			BGBCC_FrBC_EmitByte(ctx, 0x40|((j>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, j&255);
			return(FR2C_STATUS_YES);
		}
		BGBCC_FrBC_EmitByte(ctx, 0xB4);
		BGBCC_FrBC_EmitByte(ctx, (j>>16)&255);
		BGBCC_FrBC_EmitByte(ctx, (j>> 8)&255);
		BGBCC_FrBC_EmitByte(ctx, (j    )&255);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmLongP(ctx, reg))
	{
		li=BGBCC_FrBC_GetRegImmLongValue(ctx, reg);
		if((li>=(-128)) && (li<128))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xB1);
			BGBCC_FrBC_EmitByte(ctx, li&255);
			return(FR2C_STATUS_YES);
		}

		if((i>=(-32768)) && (li<32768))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xB3);
			BGBCC_FrBC_EmitByte(ctx, (li>> 8)&255);
			BGBCC_FrBC_EmitByte(ctx, (li    )&255);
			return(FR2C_STATUS_YES);
		}
		
		i=BGBCC_FrBC_IndexLitS64(ctx, li);
		if((i>=0)&&(i<4096))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xBF);
			BGBCC_FrBC_EmitByte(ctx, 0x50|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);
			return(FR2C_STATUS_YES);
		}
		BGBCC_FrBC_EmitByte(ctx, 0xB5);
		BGBCC_FrBC_EmitByte(ctx, (i>>16)&255);
		BGBCC_FrBC_EmitByte(ctx, (i>> 8)&255);
		BGBCC_FrBC_EmitByte(ctx, (i    )&255);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmFloatP(ctx, reg))
	{
		lf=BGBCC_FrBC_GetRegImmFloatValue(ctx, reg);
		i=BGBCC_FrBC_IndexLitF32(ctx, lf);
		if((i>=0)&&(i<4096))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xBF);
			BGBCC_FrBC_EmitByte(ctx, 0x60|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);
			return(FR2C_STATUS_YES);
		}
		BGBCC_FrBC_EmitByte(ctx, 0xB6);
		BGBCC_FrBC_EmitByte(ctx, (i>>16)&255);
		BGBCC_FrBC_EmitByte(ctx, (i>> 8)&255);
		BGBCC_FrBC_EmitByte(ctx, (i    )&255);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmDoubleP(ctx, reg))
	{
		lf=BGBCC_FrBC_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_FrBC_IndexLitF64(ctx, lf);
		if((i>=0)&&(i<4096))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xBF);
			BGBCC_FrBC_EmitByte(ctx, 0x70|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);
			return(FR2C_STATUS_YES);
		}
		BGBCC_FrBC_EmitByte(ctx, 0xB7);
		BGBCC_FrBC_EmitByte(ctx, (i>>16)&255);
		BGBCC_FrBC_EmitByte(ctx, (i>> 8)&255);
		BGBCC_FrBC_EmitByte(ctx, (i    )&255);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmStringP(ctx, reg) ||
		BGBCC_FrBC_IsRegImmWStringP(ctx, reg))
	{
//		lf=BGBCC_FrBC_GetRegImmDoubleValue(ctx, reg);
		i=BGBCC_FrBC_GetRegImmLit24(ctx, reg,
			BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P));
		if((i>=0)&&(i<4096))
		{
			BGBCC_FrBC_EmitByte(ctx, 0xBF);
			BGBCC_FrBC_EmitByte(ctx, 0x80|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);
			return(FR2C_STATUS_YES);
		}
		BGBCC_FrBC_EmitByte(ctx, 0xB8);
		BGBCC_FrBC_EmitByte(ctx, (i>>16)&255);
		BGBCC_FrBC_EmitByte(ctx, (i>> 8)&255);
		BGBCC_FrBC_EmitByte(ctx, (i    )&255);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_NO);
}

frbc_status BGBCC_FrBC_EmitCallRetDefault(
	BGBCC_FrCC_State *ctx)
{
	if((ctx->ip-ctx->ips)<=0)
		return(FR2C_STATUS_NO);
	if(ctx->ctxflags&BGBCC_CTXFL_LASTOPRET)
		return(FR2C_STATUS_NO);

	return(BGBCC_FrBC_EmitCallRetV(ctx));
}

frbc_status BGBCC_FrBC_EmitCallRetV(
	BGBCC_FrCC_State *ctx)
{
	BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RET_V);
	BGBCC_FrBC_EmitMarkEndTrace(ctx);
	BGBCC_FrBC_EmitMarkLastOpRet(ctx);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_EmitCallRetOp(
	BGBCC_FrCC_State *ctx, frbc_type type, frbc_register src)
{
	frbc_register treg;
	int z, y, w, rt, op;
	int i, j, k;

	if(BGBCC_FrBC_TypeVoidP(ctx, type))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RET_V);
		BGBCC_FrBC_EmitMarkEndTrace(ctx);
		BGBCC_FrBC_EmitMarkLastOpRet(ctx);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
		y=BGBCC_FrBC_GetRegOprMode1(ctx, src);
		j=BGBCC_FrBC_GetRegID(ctx, src);
		k=BGBCC_FrBC_LookupStructureIDForType(ctx, type);

		if((y>=0) && (y<16) && (j>=0) && (j<16) &&
			(k<65536))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RETOBJ);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|j);
			BGBCC_FrBC_EmitWord(ctx, k);
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RETOBJW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, j);
		BGBCC_FrBC_EmitWord24(ctx, k);
		return(FR2C_STATUS_YES);
	}

	w=BGBCC_FrBC_GetRegOprModeW(ctx, src, type);
	j=BGBCC_FrBC_GetRegID(ctx, src);

	if((w>=0) && (w<16) && (j>=0) && (j<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RET);
		BGBCC_FrBC_EmitByte(ctx, (w<<4)|(j&15));
		BGBCC_FrBC_EmitMarkEndTrace(ctx);
		BGBCC_FrBC_EmitMarkLastOpRet(ctx);
		return(FR2C_STATUS_YES);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
//	y=BGBCC_FrBC_GetRegOprModeW(ctx, src, type);
	y=BGBCC_FrBC_GetRegOprMode1(ctx, src);

	if((z>=0) && (z<16) && (y>=0) && (y<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RETW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|(y&15));
		BGBCC_FrBC_EmitByte(ctx, j&255);
		BGBCC_FrBC_EmitMarkEndTrace(ctx);
		BGBCC_FrBC_EmitMarkLastOpRet(ctx);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImm12P(ctx, src, type) &&
		(z>=0) && (z<8))
	{
		k=BGBCC_FrBC_GetRegImm12Value(ctx, src, type);

		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RET_C);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|(k&15));
		BGBCC_FrBC_EmitByte(ctx, k&255);
		BGBCC_FrBC_EmitMarkEndTrace(ctx);
		BGBCC_FrBC_EmitMarkLastOpRet(ctx);
		return(FR2C_STATUS_YES);
	}

	k=BGBCC_FrBC_GetRegImmLit12(ctx, src, type);
	if((k>=0) && (z>=0) && (z<8))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_RET_C);
		BGBCC_FrBC_EmitByte(ctx, 0x80|(z<<4)|(k&15));
		BGBCC_FrBC_EmitByte(ctx, k&255);
		BGBCC_FrBC_EmitMarkEndTrace(ctx);
		BGBCC_FrBC_EmitMarkLastOpRet(ctx);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src) ||
		BGBCC_FrBC_IsRegImmP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		rt=BGBCC_FrBC_EmitCallRetOp(ctx, type, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}
