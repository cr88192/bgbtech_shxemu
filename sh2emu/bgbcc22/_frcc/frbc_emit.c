#include <bgbccc.h>

void BGBCC_FrBC_CheckExpandLabel(BGBCC_FrCC_State *ctx)
{
	int i;

	if(!ctx->lbl_name)
	{
		i=1024;
		ctx->lbl_name=bgbcc_malloc(i*sizeof(char *));
		ctx->lbl_dest=bgbcc_malloc(i*sizeof(int));
		ctx->n_lbl=0;
		ctx->m_lbl=i;
		return;
	}
	
	if((ctx->n_lbl+1)>=ctx->m_lbl)
	{
		i=ctx->m_lbl+(ctx->m_lbl>>1);
		ctx->lbl_name=bgbcc_realloc(ctx->lbl_name, i*sizeof(char *));
		ctx->lbl_dest=bgbcc_realloc(ctx->lbl_dest, i*sizeof(int));
		ctx->m_lbl=i;
	}
}

void BGBCC_FrBC_CheckExpandGoto(BGBCC_FrCC_State *ctx)
{
	int i;

	if(!ctx->goto_name)
	{
		i=1024;
		ctx->goto_name=bgbcc_malloc(i*sizeof(char *));
		ctx->goto_dest=bgbcc_malloc(i*sizeof(int));
		ctx->goto_type=bgbcc_malloc(i*sizeof(int));
		ctx->n_goto=0;
		ctx->m_goto=i;
		return;
	}
	
	if((ctx->n_goto+1)>=ctx->m_goto)
	{
		i=ctx->m_goto+(ctx->m_goto>>1);
		ctx->goto_name=bgbcc_realloc(ctx->goto_name, i*sizeof(char *));
		ctx->goto_dest=bgbcc_realloc(ctx->goto_dest, i*sizeof(int));
		ctx->goto_type=bgbcc_realloc(ctx->goto_type, i*sizeof(int));
		ctx->m_goto=i;
	}
}

void BGBCC_FrBC_CheckExpandText(BGBCC_FrCC_State *ctx, int sz)
{
	int i, j;

	if(!ctx->ips)
	{
		i=4096;
		if(i<sz)i+=sz;
		ctx->ips=bgbcc_malloc(i);
		ctx->ipe=ctx->ips+i;
		ctx->ip=ctx->ips;
		return;
	}
	
	if((ctx->ip+sz)>=ctx->ipe)
	{
		i=ctx->ipe-ctx->ips;
		j=ctx->ip-ctx->ips;
		while(i<=(j+sz))
			i=i+(i>>1);
		ctx->ips=bgbcc_realloc(ctx->ips, i);
		ctx->ipe=ctx->ips+i;
		ctx->ip=ctx->ips+j;
		return;
	}
}

void BGBCC_FrBC_EmitLabel(BGBCC_FrCC_State *ctx, char *name)
{
	int i;

	BGBCC_FrBC_CheckExpandLabel(ctx);

	if(!(ctx->ctxflags&BGBCC_CTXFL_ENDOFTRACE))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LABEL);
	}

	ctx->ctxflags&=~BGBCC_CTXFL_LASTOPRET;

	i=ctx->n_lbl++;
	ctx->lbl_name[i]=bgbcc_strdup(name);
	ctx->lbl_dest[i]=(ctx->ip)-(ctx->ips);
}

void BGBCC_FrBC_EmitGotoAddr16(BGBCC_FrCC_State *ctx, char *name)
{
	int i;

	BGBCC_FrBC_CheckExpandGoto(ctx);

	i=ctx->n_goto++;
	ctx->goto_name[i]=bgbcc_strdup(name);
	ctx->goto_dest[i]=(ctx->ip)-(ctx->ips);
	ctx->goto_type[i]=FR2C_JMPMODE_A16;
}

void BGBCC_FrBC_EmitGotoAddr32(BGBCC_FrCC_State *ctx, char *name)
{
	int i;

	BGBCC_FrBC_CheckExpandGoto(ctx);

	i=ctx->n_goto++;
	ctx->goto_name[i]=bgbcc_strdup(name);
	ctx->goto_dest[i]=(ctx->ip)-(ctx->ips);
	ctx->goto_type[i]=FR2C_JMPMODE_A32;
}

void BGBCC_FrBC_EmitRelocTag(BGBCC_FrCC_State *ctx, int tag)
{
	int i;

	BGBCC_FrBC_CheckExpandGoto(ctx);

	i=ctx->n_goto++;
	ctx->goto_name[i]=NULL;
	ctx->goto_dest[i]=(ctx->ip)-(ctx->ips);
	ctx->goto_type[i]=tag;
}

void BGBCC_FrBC_EmitLitInt12(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LI12); }
void BGBCC_FrBC_EmitLitLong12(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LJ12); }
void BGBCC_FrBC_EmitLitFloat12(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LF12); }
void BGBCC_FrBC_EmitLitDouble12(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LD12); }

void BGBCC_FrBC_EmitLitZ12(BGBCC_FrCC_State *ctx, int z)
{
	int t;
	switch(z)
	{
	case FR2C_TY_I:		case FR2C_TY_UI:
	case FR2C_TY_SB:	case FR2C_TY_UB:
	case FR2C_TY_SS:	case FR2C_TY_US:
		t=FR2C_JMPMODE_LI12; break;
	case FR2C_TY_L:		case FR2C_TY_UL:
		t=FR2C_JMPMODE_LJ12; break;
	case FR2C_TY_F:
		t=FR2C_JMPMODE_LF12; break;
	case FR2C_TY_D:
		t=FR2C_JMPMODE_LD12; break;
	default:
		t=FR2C_JMPMODE_LL12; break;
	}
	BGBCC_FrBC_EmitRelocTag(ctx, t);
}

void BGBCC_FrBC_EmitLitZ24(BGBCC_FrCC_State *ctx, int z)
{
	int t;
	switch(z)
	{
	case FR2C_TY_I:		case FR2C_TY_UI:
	case FR2C_TY_SB:	case FR2C_TY_UB:
	case FR2C_TY_SS:	case FR2C_TY_US:
		t=FR2C_JMPMODE_LI24; break;
	case FR2C_TY_L:		case FR2C_TY_UL:
		t=FR2C_JMPMODE_LJ24; break;
	case FR2C_TY_F:
		t=FR2C_JMPMODE_LF24; break;
	case FR2C_TY_D:
		t=FR2C_JMPMODE_LD24; break;
	default:
		t=FR2C_JMPMODE_LL24; break;
	}
	BGBCC_FrBC_EmitRelocTag(ctx, t);
}

void BGBCC_FrBC_EmitRelocLiteral12(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LL12); }
void BGBCC_FrBC_EmitRelocGlobal12(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LG12); }

void BGBCC_FrBC_EmitRelocLiteral16(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LL16); }
void BGBCC_FrBC_EmitRelocGlobal16(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LG16); }

void BGBCC_FrBC_EmitRelocLiteral24(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LL24); }
void BGBCC_FrBC_EmitRelocGlobal24(BGBCC_FrCC_State *ctx)
	{ BGBCC_FrBC_EmitRelocTag(ctx, FR2C_JMPMODE_LG24); }

void BGBCC_FrBC_EmitMarkEndTrace(BGBCC_FrCC_State *ctx)
{
	ctx->ctxflags|=BGBCC_CTXFL_ENDOFTRACE;
}

void BGBCC_FrBC_EmitMarkLastOpRet(BGBCC_FrCC_State *ctx)
{
	ctx->ctxflags|=BGBCC_CTXFL_LASTOPRET;
}

void BGBCC_FrBC_EmitByte(BGBCC_FrCC_State *ctx, int val)
{
	BGBCC_FrBC_CheckExpandText(ctx, 1);
	*ctx->ip++=val;
	
	ctx->ctxflags&=~(BGBCC_CTXFL_ENDOFTRACE|BGBCC_CTXFL_LASTOPRET);
}

void BGBCC_FrBC_EmitWord(BGBCC_FrCC_State *ctx, int val)
{
	BGBCC_FrBC_CheckExpandText(ctx, 2);
	*ctx->ip++=(val>>8);
	*ctx->ip++=val;

	ctx->ctxflags&=~(BGBCC_CTXFL_ENDOFTRACE|BGBCC_CTXFL_LASTOPRET);
}

void BGBCC_FrBC_EmitWord24(BGBCC_FrCC_State *ctx, int val)
{
	BGBCC_FrBC_CheckExpandText(ctx, 3);
	*ctx->ip++=(val>>16);
	*ctx->ip++=(val>>8);
	*ctx->ip++=val;

	ctx->ctxflags&=~(BGBCC_CTXFL_ENDOFTRACE|BGBCC_CTXFL_LASTOPRET);
}

void BGBCC_FrBC_EmitDWord(BGBCC_FrCC_State *ctx, int val)
{
	BGBCC_FrBC_CheckExpandText(ctx, 4);
	*ctx->ip++=(val>>24);
	*ctx->ip++=(val>>16);
	*ctx->ip++=(val>>8);
	*ctx->ip++=val;

	ctx->ctxflags&=~(BGBCC_CTXFL_ENDOFTRACE|BGBCC_CTXFL_LASTOPRET);
}

void BGBCC_FrBC_EmitWord32(BGBCC_FrCC_State *ctx, int val)
{
	BGBCC_FrBC_EmitDWord(ctx, val);
}

frbc_status BGBCC_FrBC_EmitOpcode(BGBCC_FrCC_State *ctx, int val)
{
	if(val<0)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPCODE));
		return(FR2C_STATUS_ERR_BADOPCODE);
	}

	if(val<224)
	{
		BGBCC_FrBC_EmitByte(ctx, val);
		return(FR2C_STATUS_YES);
	}
	if(val<4096)
	{
		BGBCC_FrBC_EmitByte(ctx, 0xE0+((val>>8)&15));
		BGBCC_FrBC_EmitByte(ctx, val&255);
		return(FR2C_STATUS_YES);
	}

	if(val<0x0007FFFF)
	{
		BGBCC_FrBC_EmitByte(ctx, 0xF0+((val>>16)&7));
		BGBCC_FrBC_EmitByte(ctx, (val>>8)&255);
		BGBCC_FrBC_EmitByte(ctx, val&255);
		return(FR2C_STATUS_YES);
	}

	if(val<0x03FFFFFF)
	{
		BGBCC_FrBC_EmitByte(ctx, 0xF8+((val>>24)&3));
		BGBCC_FrBC_EmitByte(ctx, (val>>16)&255);
		BGBCC_FrBC_EmitByte(ctx, (val>>8)&255);
		BGBCC_FrBC_EmitByte(ctx, val&255);
		return(FR2C_STATUS_YES);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPCODE));
	return(FR2C_STATUS_ERR_BADOPCODE);
}

frbc_status BGBCC_FrBC_EmitMovInt(
	BGBCC_FrCC_State *ctx,
	frbc_register dst, frbc_register src)
{
	return(BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I), dst, src));
}

frbc_status BGBCC_FrBC_EmitMovLong(
	BGBCC_FrCC_State *ctx,
	frbc_register dst, frbc_register src)
{
	return(BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_L), dst, src));
}

frbc_status BGBCC_FrBC_EmitMovFloat(
	BGBCC_FrCC_State *ctx,
	frbc_register dst, frbc_register src)
{
	return(BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_F), dst, src));
}

frbc_status BGBCC_FrBC_EmitMovDouble(
	BGBCC_FrCC_State *ctx,
	frbc_register dst, frbc_register src)
{
	return(BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_D), dst, src));
}

frbc_status BGBCC_FrBC_EmitMovPointer(
	BGBCC_FrCC_State *ctx,
	frbc_register dst, frbc_register src)
{
	return(BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_P), dst, src));
}

frbc_status BGBCC_FrBC_EmitMovMatchDst(
	BGBCC_FrCC_State *ctx,
	frbc_register dst, frbc_register src)
{
	return(BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_GetRegType(ctx, dst), dst, src));
}

frbc_status BGBCC_FrBC_EmitMovMatchSrc(
	BGBCC_FrCC_State *ctx,
	frbc_register dst, frbc_register src)
{
	return(BGBCC_FrBC_EmitMov(ctx,
		BGBCC_FrBC_GetRegType(ctx, src), dst, src));
}


frbc_status BGBCC_FrBC_EmitMov(
	BGBCC_FrCC_State *ctx, frbc_type type,
	frbc_register dst, frbc_register src)
{
	int z, y, w;
	int i, j, k;

	if(dst.val==src.val)
		return(FR2C_STATUS_NO);
	
	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
		{ return(BGBCC_FrBC_EmitStoreGlobal(ctx, type, dst, src)); }

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
		{ return(BGBCC_FrBC_EmitLoadGlobal(ctx, type, dst, src)); }

	if(BGBCC_FrBC_IsRegImmP(ctx, src))
	{
		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegImmInt12P(ctx, src) &&
			BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
		{
			return(BGBCC_FrBC_EmitUnaryOp(ctx, type, FR2C_UNOP_MOV,
				dst, src));
		}

		w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, type);
		j=BGBCC_FrBC_GetRegID(ctx, dst);
		k=BGBCC_FrBC_GetRegImmLit16(ctx, src, type);

		if((w>=0) && (w<16) && (j>=0) && (j<16) && (k>=0))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDCONST);
			BGBCC_FrBC_EmitByte(ctx, (w<<4)|(j&15));
			BGBCC_FrBC_EmitWord(ctx, k);
			return(FR2C_STATUS_YES);
		}

		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
		y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);
		k=BGBCC_FrBC_GetRegImmLitIndex(ctx, src, type);

		if((z>=0) && (z<16) && (y>=0) && (y<16) &&
			(j>=0) && (j<256) && (k>=0))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDCONSTW);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|(y&15));
			BGBCC_FrBC_EmitByte(ctx, j);
			BGBCC_FrBC_EmitDWord(ctx, k);
			return(FR2C_STATUS_YES);
		}
	}

	if(BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
		k=BGBCC_FrBC_TypeObjectLiteralIndex(ctx, type);

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseP(ctx, src) &&
			(y>=0) && (y<16) &&
			(k>=0) && (k<4096))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_COPYOBJ);
			BGBCC_FrBC_EmitByte(ctx, (y<<4)|
				BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, src)<<4)|((k>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, k&255);
			return(FR2C_STATUS_YES);
		}

		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_COPYOBJW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord24(ctx, k);
		return(FR2C_STATUS_YES);
	}

	return(BGBCC_FrBC_EmitUnaryOp(ctx, type, FR2C_UNOP_MOV,
		dst, src));
}

frbc_status BGBCC_FrBC_EmitConv(
	BGBCC_FrCC_State *ctx, frbc_type dtype, frbc_type stype,
	frbc_register dst, frbc_register src)
{
	frbc_register treg;
	frbc_type ttype;
	int y, z, z2, w, rt, op;
	int i, j, k;

//	if(dtype.val==stype.val)
	if(BGBCC_FrBC_TypeBaseEqualP(ctx, dtype, stype))
	{
		rt=BGBCC_FrBC_EmitMov(ctx, dtype, dst, src);
		return(rt);
//		return(FR2C_STATUS_NO);
	}

	if(BGBCC_FrBC_TypePointerP(ctx, dtype) &&
		BGBCC_FrBC_TypeArrayOrPointerP(ctx, stype))
	{
		rt=BGBCC_FrBC_EmitMov(ctx, dtype, dst, src);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, dtype, &treg);
		rt=BGBCC_FrBC_EmitConv(ctx, dtype, stype, treg, src);
		BGBCC_FrBC_EmitMov(ctx, dtype, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, stype, &treg);
		BGBCC_FrBC_EmitMov(ctx, stype, treg, src);
		rt=BGBCC_FrBC_EmitConv(ctx, dtype, stype, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
	z=BGBCC_FrBC_GetTypeConvMode(ctx, dtype, stype);
	z2=BGBCC_FrBC_GetTypeConvIMode(ctx, dtype, stype);
//	if(y<0)return(-1);

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(y>=0) && (y<16) &&
		(z>=0) && (z<16))
	{
		op=FR2C_OP_CONV;

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, src));
		return(FR2C_STATUS_YES);
	}

	z2=BGBCC_FrBC_GetTypeConvIMode(ctx, dtype, stype);
	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(y>=0) && (y<16) &&
		(z2>=0) && (z2<16))
	{
		op=FR2C_OP_CONV_I;
		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (z2<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, src));
		return(FR2C_STATUS_YES);
	}

	z2=BGBCC_FrBC_GetTypeConvLMode(ctx, dtype, stype);
	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		(y>=0) && (y<16) &&
		(z2>=0) && (z2<16))
	{
		op=FR2C_OP_CONV_L;
		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (z2<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, src));
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmP(ctx, src))
	{
//		if(BGBCC_FrBC_TypePointerP(ctx, dtype))
		if(1)
		{
			w=BGBCC_FrBC_GetRegOprModeW(ctx, dst, dtype);
			i=BGBCC_FrBC_GetRegImmLit16(ctx, src, dtype);
			if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
				(w>=0) && (w<16) && (i>=0))
			{
				BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDCONST);
				BGBCC_FrBC_EmitByte(ctx, (w<<4)|
					BGBCC_FrBC_GetRegID(ctx, dst));
				BGBCC_FrBC_EmitWord(ctx, i);
				return(FR2C_STATUS_YES);
			}

			y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);
			z=BGBCC_FrBC_TypeAsOprBasic(ctx, dtype);
			i=BGBCC_FrBC_GetRegImmLitIndex(ctx, src, dtype);
			if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
				(z>=0) && (z<16) && (y>=0) && (y<16) && (i>=0))
			{
				BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_LDCONSTW);
				BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
				BGBCC_FrBC_EmitByte(ctx,
					BGBCC_FrBC_GetRegID(ctx, dst));
				BGBCC_FrBC_EmitDWord(ctx, i);
				return(FR2C_STATUS_YES);
			}
		}
	
		BGBCC_FrBC_RegisterAllocTemporary(ctx, stype, &treg);
		BGBCC_FrBC_EmitMov(ctx, stype, treg, src);
		rt=BGBCC_FrBC_EmitConv(ctx, dtype, stype, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	if(	BGBCC_FrBC_TypeSmallIntP(ctx, stype) &&
		BGBCC_FrBC_TypeSmallIntP(ctx, dtype))
	{
		rt=BGBCC_FrBC_EmitMov(ctx, dtype, dst, src);
		return(rt);
	}

	if(	BGBCC_FrBC_TypeSmallLongP(ctx, stype) &&
		BGBCC_FrBC_TypeSmallLongP(ctx, dtype))
	{
		rt=BGBCC_FrBC_EmitMov(ctx, dtype, dst, src);
		return(rt);
	}

	/* Detour through int */
	if(	(	BGBCC_FrBC_TypeBaseILFD_P(ctx, dtype) &&
			BGBCC_FrBC_TypeSmallIntP(ctx, stype)) ||
		(	BGBCC_FrBC_TypeBaseILFD_P(ctx, stype) &&
			BGBCC_FrBC_TypeSmallIntP(ctx, dtype)))
	{
		ttype=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_I);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, ttype, &treg);
		BGBCC_FrBC_EmitConv(ctx, ttype, stype, treg, src);
		rt=BGBCC_FrBC_EmitConv(ctx, dtype, ttype, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	/* Detour through long */
	if(	(	BGBCC_FrBC_TypeBaseILFD_P(ctx, dtype) &&
			BGBCC_FrBC_TypeSmallLongP(ctx, stype)) ||
		(	BGBCC_FrBC_TypeBaseILFD_P(ctx, stype) &&
			BGBCC_FrBC_TypeSmallLongP(ctx, dtype)))
	{
		ttype=BGBCC_FrBC_TypeWrapBasicType(FR2C_TY_L);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, ttype, &treg);
		BGBCC_FrBC_EmitConv(ctx, ttype, stype, treg, src);
		rt=BGBCC_FrBC_EmitConv(ctx, dtype, ttype, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitUnaryOp(
	BGBCC_FrCC_State *ctx, frbc_type type, int opr,
	frbc_register dst, frbc_register src)
{
	frbc_register treg;
	int y, z, rt, op, im12, im16;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		rt=BGBCC_FrBC_EmitUnaryOp(ctx, type, opr, treg, src);
		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		rt=BGBCC_FrBC_EmitUnaryOp(ctx, type, opr, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, src);
//	if(y<0)return(-1);

	if((opr==0) && BGBCC_FrBC_TypeValueObjectP(ctx, type))
	{
		j=BGBCC_FrBC_LookupStructureIDForType(ctx, type);

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseP(ctx, src) &&
			(j<4096) &&
			(y>=0) && (y<16))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_COPYOBJ);
			BGBCC_FrBC_EmitByte(ctx,
				(y<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, src)<<4)|
				((j>>8)&15));
			BGBCC_FrBC_EmitByte(ctx,
				j&255);
			return(FR2C_STATUS_YES);
		}

		z=FR2C_TY_S;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_COPYOBJW);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitWord24(ctx, j);
		return(FR2C_STATUS_YES);
	}

	if((opr==0) && BGBCC_FrBC_TypeArrayOrPointerP(ctx, type) &&
		(y>=0) && (y<16))
	{
		z=FR2C_TY_P;
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_MOV);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
			BGBCC_FrBC_GetRegID(ctx, src));
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type) &&
		(y>=0) && (y<16))
	{
		if(opr==0)
		{
			z=BGBCC_FrBC_TypeAsOprBasic(ctx, type);
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_MOV);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
				BGBCC_FrBC_GetRegID(ctx, src));
			return(FR2C_STATUS_YES);
		}

		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_UNOP_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_UNOP_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_UNOP_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_UNOP_D; }

		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, src));

		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, src) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type) &&
		(y>=0) && (y<16))
	{
		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_UNOPW_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_UNOPW_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_UNOPW_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_UNOPW_D; }

		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, src));
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		BGBCC_FrBC_TypeOprBasicP(ctx, type) &&
		(y>=0) && (y<16))
	{
		z=BGBCC_FrBC_TypeAsOprBasic(ctx, type);
		if(z<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		if(opr==0)
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_MOV);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|
				BGBCC_FrBC_GetRegID(ctx, src));
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_UNOP2);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, src)<<4)|
			 0);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmP(ctx, src))
	{
		y=BGBCC_FrBC_GetRegOprMode1(ctx, dst);

#if 0
		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegImmInt12P(ctx, src) &&
			BGBCC_FrBC_TypeBaseSmallIL_P(ctx, type) &&
			(y>=0) && (y<16) && (opr>=0) && (opr<8))
		{
			op=-1;
			if(BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_UNOP_IC; }
			if(BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_UNOP_LC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			i=BGBCC_FrBC_GetRegImmIntValue(ctx, src);

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);

			return(FR2C_STATUS_YES);
		}
#endif

#if 1
		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegImm12P(ctx, src, type) &&
			BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type) &&
			(y>=0) && (y<16) && (opr>=0) && (opr<8))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_UNOP_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_UNOP_LC; }
			if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_UNOP_FC; }
			if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_UNOP_DC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			i=BGBCC_FrBC_GetRegImm12Value(ctx, src, type);

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);

			return(FR2C_STATUS_YES);
		}
#endif

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegImmInt16P(ctx, src) &&
			BGBCC_FrBC_TypeOprBasicP(ctx, type) &&
			(y>=0) && (y<16))
		{
			z=BGBCC_FrBC_TypeAsOprBasic(ctx, type);
			if(z<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			i=BGBCC_FrBC_GetRegImmIntValue(ctx, src);

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_UNOP3_C);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitWord(ctx, i);
			return(FR2C_STATUS_YES);
		}

		im12=BGBCC_FrBC_GetRegImmLit12(ctx, src, type);

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type) &&
			(im12>=0) &&
			(y>=0) && (y<16) && (opr>=0) && (opr<16))
		{
			op=-1;
			if(BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_UNOP_IC; z=FR2C_TY_I; }
			if(BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_UNOP_LC; z=FR2C_TY_L; }
			if(BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_UNOP_FC; z=FR2C_TY_F; }
			if(BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_UNOP_DC; z=FR2C_TY_D; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, ((opr+8)<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, dst)<<4)|((im12>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, im12&255);
			BGBCC_FrBC_EmitLitZ12(ctx, z);

			return(FR2C_STATUS_YES);
		}

		im16=im12;
		if(im16<0)
			{ im16=BGBCC_FrBC_GetRegImmLit16(ctx, src, type); }

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_TypeOprBasicP(ctx, type) &&
			(im16>=0) &&
			(y>=0) && (y<16))
		{
			z=BGBCC_FrBC_TypeAsOprBasic(ctx, type);
			if(z<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_UNOP2_C);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitWord(ctx, im16);
			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitBinaryOp(
	BGBCC_FrCC_State *ctx, frbc_type type, int opr,
	frbc_register dst, frbc_register srca, frbc_register srcb)
{
	frbc_register treg;
	frbc_type bty, tty;
	double f0, f1, f2;
	s64 l0, l1, l2;
	int y, z, z2, rt, op, im12, im24;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		rt=BGBCC_FrBC_EmitBinaryOp(ctx, type, opr, treg, srca, srcb);
		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srca))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
		rt=BGBCC_FrBC_EmitBinaryOp(ctx, type, opr, dst, treg, srcb);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
		rt=BGBCC_FrBC_EmitBinaryOp(ctx, type, opr, dst, srca, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode3(ctx, dst, srca, srcb);
//	if(y<0)return(-1);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y<0))
	{
		tty=BGBCC_FrBC_GetRegType(ctx, dst);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		BGBCC_FrBC_EmitBinaryOp(ctx, type, opr, treg, srca, srcb);
		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
		BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
		BGBCC_FrBC_IsRegBasicP(ctx, srcb) &&
		(y==FR2C_OPMODE_RRR) && (opr>=0) && (opr<16) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
	{
		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_BINOP_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_BINOP_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_BINOP_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_BINOP_D; }

		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, srcb));

		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srcb) &&
		(y>=0) && (y<16) && (opr>=0) && (opr<16) &&
		(z>=0) && (z<16))
	{
//		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_BINOP2);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, srcb));
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
	{
		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_BINOPW_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_BINOPW_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_BINOPW_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_BINOPW_D; }

		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));
		return(FR2C_STATUS_YES);
	}

	z2=BGBCC_FrBC_GetTypeOperationExtZ(ctx, type);
	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) && (opr>=0) && (opr<16) &&
		(z2>=0) && (z2<256))
	{
//		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_BINOP2W);
		BGBCC_FrBC_EmitByte(ctx, z);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmP(ctx, srcb))
	{
		if(BGBCC_FrBC_IsRegImmP(ctx, srca))
		{
			if(BGBCC_FrBC_IsRegImmILP(ctx, srca) &&
				BGBCC_FrBC_IsRegImmILP(ctx, srcb))
			{
				l0=BGBCC_FrBC_GetRegImmLongValue(ctx, srca);
				l1=BGBCC_FrBC_GetRegImmLongValue(ctx, srcb);
				
				switch(opr)
				{
				case FR2C_BINOP_ADD: l2=l0+l1; break;
				case FR2C_BINOP_SUB: l2=l0-l1; break;
				case FR2C_BINOP_MUL: l2=l0*l1; break;
				case FR2C_BINOP_DIV: l2=l0/l1; break;
				case FR2C_BINOP_MOD: l2=l0%l1; break;
				case FR2C_BINOP_AND: l2=l0&l1; break;
				case FR2C_BINOP_OR: l2=l0|l1; break;
				case FR2C_BINOP_XOR: l2=l0^l1; break;
				case FR2C_BINOP_SHL: l2=l0<<(int)l1; break;
				case FR2C_BINOP_SHR: l2=l0>>(int)l1; break;
				case FR2C_BINOP_SHRR: l2=((u64)l0)>>(int)l1; break;
				case FR2C_BINOP_UDIV: l2=((u64)l0)/((u64)l1); break;
				default:
					BGBCC_FrBC_TagError(ctx,
						FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
					break;
				}
				
				if(BGBCC_FrBC_IsRegImmLongP(ctx, srca) ||
					BGBCC_FrBC_IsRegImmLongP(ctx, srcb))
					{ BGBCC_FrBC_GetRegForLongValue(ctx, &treg, l2); }
				else
					{ BGBCC_FrBC_GetRegForIntValue(ctx, &treg, l2); }

				k=BGBCC_FrBC_EmitMov(ctx,
					type, dst, treg);
				return(k);
			}

			if(BGBCC_FrBC_IsRegImmILFDP(ctx, srca) &&
				BGBCC_FrBC_IsRegImmILFDP(ctx, srcb))
			{
				f0=BGBCC_FrBC_GetRegImmDoubleValue(ctx, srca);
				f1=BGBCC_FrBC_GetRegImmDoubleValue(ctx, srcb);
				
				switch(opr)
				{
				case FR2C_BINOP_ADD: f2=f0+f1; break;
				case FR2C_BINOP_SUB: f2=f0-f1; break;
				case FR2C_BINOP_MUL: f2=f0*f1; break;
				case FR2C_BINOP_DIV: f2=f0/f1; break;
				default:
					BGBCC_FrBC_TagError(ctx,
						FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
					break;
				}
				
				if(BGBCC_FrBC_IsRegImmFloatP(ctx, srca) &&
					BGBCC_FrBC_IsRegImmFloatP(ctx, srcb))
					{ BGBCC_FrBC_GetRegForFloatValue(ctx, &treg, f2); }
				else
					{ BGBCC_FrBC_GetRegForDoubleValue(ctx, &treg, f2); }

				k=BGBCC_FrBC_EmitMov(ctx,
					type, dst, treg);
				return(k);
			}
		}

		y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, srca);
		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
		z2=BGBCC_FrBC_GetTypeOperationExtZ(ctx, type);

#if 0
		if(	BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
			BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
			BGBCC_FrBC_IsRegImmInt12P(ctx, srcb) &&
			(	BGBCC_FrBC_TypeSmallIntP(ctx, type) ||
					BGBCC_FrBC_TypeSmallLongP(ctx, type)) &&
			(y==FR2C_OPMODE_RRR) && (opr>=0) && (opr<16))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_BINOP_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_BINOP_LC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);

			return(FR2C_STATUS_YES);
		}
#endif

		if(	BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
			BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
			BGBCC_FrBC_IsRegImm12P(ctx, srcb, type) &&
			BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type) &&
//			(	BGBCC_FrBC_TypeFloatP(ctx, type) ||
//					BGBCC_FrBC_TypeDoubleP(ctx, type)) &&
			(y==FR2C_OPMODE_RRR) && (opr>=0) && (opr<16))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_BINOP_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_BINOP_LC; }
			if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_BINOP_FC; }
			if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_BINOP_DC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			i=BGBCC_FrBC_GetRegImm12Value(ctx, srcb, type);

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);

			return(FR2C_STATUS_YES);
		}

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
//			BGBCC_FrBC_IsRegImmInt12P(ctx, srcb) &&
			BGBCC_FrBC_IsRegImm12P(ctx, srcb, type) &&
			(y>=0) && (y<16) && (z>=0) && (z<16))
		{
//			i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);
			i=BGBCC_FrBC_GetRegImm12Value(ctx, srcb, type);

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_BINOP3_C);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);
			return(FR2C_STATUS_YES);
		}

		if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
			BGBCC_FrBC_IsRegImm32P(ctx, srcb, type) &&
			(y>=0) && (y<16) && (z2>=0) && (z2<256))
		{
//			i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);
			i=BGBCC_FrBC_GetRegImm32Value(ctx, srcb, type);

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_BINOP3W_C);
			BGBCC_FrBC_EmitByte(ctx, z2);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
			BGBCC_FrBC_EmitWord32(ctx, i);
			return(FR2C_STATUS_YES);
		}

		im12=BGBCC_FrBC_GetRegImmLit12(ctx, srcb, type);

		if(	BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
			BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
			(	BGBCC_FrBC_TypeFloatP(ctx, type) ||
				BGBCC_FrBC_TypeDoubleP(ctx, type)) &&
			(im12>=0) && (y==FR2C_OPMODE_RRR) &&
			(opr>=0) && (opr<16))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_BINOP_FC; }
			if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_BINOP_DC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx,
				(opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((im12>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, im12&255);

			return(FR2C_STATUS_YES);
		}

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
			(im12>=0) && (y>=0) && (y<16) && (z>=0) && (z<16))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_BINOP2_C);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((im12>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, im12&255);
			BGBCC_FrBC_EmitLitZ12(ctx, z);

			return(FR2C_STATUS_YES);
		}

		im24=BGBCC_FrBC_GetRegImmLit24(ctx, srcb, type);

		if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
			(y>=0) && (y<16) && (im24>=0) && 
			BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_BINOPW_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_BINOPW_LC; }
			if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_BINOPW_FC; }
			if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_BINOPW_DC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
			BGBCC_FrBC_EmitWord24(ctx, im24);
			BGBCC_FrBC_EmitLitZ24(ctx, z);
			return(FR2C_STATUS_YES);
		}

		if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
			(im24>=0) && (y>=0) && (y<16) && (z2>=0) && (z2<256))
		{
			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_BINOP2W_C);

			BGBCC_FrBC_EmitByte(ctx, z2);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
			BGBCC_FrBC_EmitWord24(ctx, im24);
			BGBCC_FrBC_EmitLitZ24(ctx, z);

			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegImmP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb))
	{
		if((opr==FR2C_BINOP_ADD) ||
			(opr==FR2C_BINOP_MUL))
		{
			rt=BGBCC_FrBC_EmitBinaryOp(ctx, type, opr, dst, srcb, srca);
			if(rt>=0)return(rt);
		}else
		{
			BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
			BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
			rt=BGBCC_FrBC_EmitBinaryOp(ctx, type, opr, dst, treg, srcb);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
			if(rt>=0)return(rt);
		}
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

//Binary Compare Operations
frbc_status BGBCC_FrBC_EmitCompareOp(
	BGBCC_FrCC_State *ctx, frbc_type type, int opr,
	frbc_register dst, frbc_register srca, frbc_register srcb)
{
	frbc_register treg;
	frbc_type bty, tty;
	int y, z, z2, rt, op, im12, im24;
	int i, j, k;

	if(BGBCC_FrBC_IsRegGlobalP(ctx, dst))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		rt=BGBCC_FrBC_EmitCompareOp(ctx, type, opr, treg, srca, srcb);
		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srca))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
		rt=BGBCC_FrBC_EmitCompareOp(ctx, type, opr, dst, treg, srcb);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
		rt=BGBCC_FrBC_EmitCompareOp(ctx, type, opr, dst, srca, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(rt);
	}

	y=BGBCC_FrBC_GetRegOprMode3(ctx, dst, srca, srcb);
//	if(y<0)return(-1);

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y<0))
	{
		tty=BGBCC_FrBC_GetRegType(ctx, dst);
		BGBCC_FrBC_RegisterAllocTemporary(ctx, tty, &treg);
//		BGBCC_FrBC_EmitMov(ctx, type, treg, dst);
		BGBCC_FrBC_EmitCompareOp(ctx, type, opr, treg, srca, srcb);
		BGBCC_FrBC_EmitMov(ctx, type, dst, treg);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
		BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
		BGBCC_FrBC_IsRegBasicP(ctx, srcb) &&
		(y==FR2C_OPMODE_RRR) && (opr>=0) && (opr<16) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
	{
		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_CMP_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_CMP_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_CMP_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_CMP_D; }

		if(op<0)return(FR2C_STATUS_ERR_UNHANDLEDTYPE);

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, srcb));

		return(FR2C_STATUS_YES);
	}

	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	z2=BGBCC_FrBC_GetTypeOperationExtZ(ctx, type);

#if 1
	if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srcb) &&
		(z>=0) && (z<16) && 
		(y>=0) && (y<16) &&
		(opr>=0) && (opr<16))
	{
//		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CMP2);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, srcb));
		return(FR2C_STATUS_YES);
	}
#endif

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		BGBCC_FrBC_TypeBaseSmallILFDP_P(ctx, type))
	{
		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_CMPW_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_CMPW_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_CMPW_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_CMPW_D; }
		if((op<0) && BGBCC_FrBC_TypeArrayOrPointerP(ctx, type))
			{ op=FR2C_OP_CMPW_P; }

		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, dst));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmP(ctx, srcb))
	{
		y=BGBCC_FrBC_GetRegOprMode2(ctx, dst, srca);

		if(	BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
			BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
			BGBCC_FrBC_IsRegImmInt12P(ctx, srcb) &&
			(	BGBCC_FrBC_TypeSmallIntP(ctx, type) ||
					BGBCC_FrBC_TypeSmallLongP(ctx, type)) &&
			(y==FR2C_OPMODE_RRR) && (opr>=0) && (opr<8))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_CMP_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_CMP_LC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);

			return(FR2C_STATUS_YES);
		}

#if 1
//		z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);

		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
			BGBCC_FrBC_IsRegImmInt12P(ctx, srcb) &&
			(y>=0) && (y<16) &&
			(z>=0) && (z<16))
		{
			i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CMP2_C);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|
				BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);
			return(FR2C_STATUS_YES);
		}
#endif

		im12=BGBCC_FrBC_GetRegImmLit12(ctx, srcb, type);

		if(	BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
			BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
			(	BGBCC_FrBC_TypeSmallIntP(ctx, type) ||
				BGBCC_FrBC_TypeSmallLongP(ctx, type) ||
				BGBCC_FrBC_TypeFloatP(ctx, type) ||
				BGBCC_FrBC_TypeDoubleP(ctx, type)) &&
			(im12>=0) && (y==FR2C_OPMODE_RRR) &&
			(opr>=0) && (opr<8))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_CMP_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_CMP_LC; }
			if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_CMP_FC; }
			if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_CMP_DC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx,
				((opr+8)<<4)|BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((im12>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, im12&255);

			return(FR2C_STATUS_YES);
		}

#if 1
		if(	BGBCC_FrBC_IsRegBaseP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
			(im12>=0) && (y>=0) && (y<16) &&
			(z>=0) && (z<16))
		{
//			z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_CMP2_C);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, ((opr+8)<<4)|
				BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((im12>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, im12&255);
			BGBCC_FrBC_EmitLitZ12(ctx, z);

			return(FR2C_STATUS_YES);
		}
#endif

		im24=BGBCC_FrBC_GetRegImmLit24(ctx, srcb, type);

		if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
			BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
			(	BGBCC_FrBC_TypeSmallIntP(ctx, type) ||
				BGBCC_FrBC_TypeSmallLongP(ctx, type) ||
				BGBCC_FrBC_TypeFloatP(ctx, type) ||
				BGBCC_FrBC_TypeDoubleP(ctx, type)) &&
			(im24>=0) && (y>=0) && (y<16) &&
			(opr>=0) && (opr<16))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_CMPW_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_CMPW_LC; }
			if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_CMPW_FC; }
			if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_CMPW_DC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				BGBCC_FrBC_GetRegID(ctx, dst));
			BGBCC_FrBC_EmitByte(ctx,
				BGBCC_FrBC_GetRegID(ctx, srca));
			BGBCC_FrBC_EmitWord24(ctx, im24);

			return(FR2C_STATUS_YES);
		}

		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, dst) &&
		BGBCC_FrBC_IsRegImmP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb))
	{
		switch(opr)
		{
		case FR2C_CMP_EQ: op=opr; break;
		case FR2C_CMP_NE: op=opr; break;
		case FR2C_CMP_AL: op=opr; break;
		case FR2C_CMP_NV: op=opr; break;
		case FR2C_CMP_LT: op=FR2C_CMP_GE; break;
		case FR2C_CMP_GT: op=FR2C_CMP_LE; break;
		case FR2C_CMP_LE: op=FR2C_CMP_GT; break;
		case FR2C_CMP_GE: op=FR2C_CMP_LT; break;
		default: op=FR2C_CMP_NV; break;
		}
		rt=BGBCC_FrBC_EmitCompareOp(ctx, type, op, dst, srcb, srca);
		if(rt>=0)return(rt);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitJump(
	BGBCC_FrCC_State *ctx, char *name)
{
	BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_JMP);
	BGBCC_FrBC_EmitByte(ctx, 0);
	BGBCC_FrBC_EmitByte(ctx, 0);
	BGBCC_FrBC_EmitGotoAddr16(ctx, name);
	return(FR2C_STATUS_YES);
}

frbc_status BGBCC_FrBC_EmitJumpRegZero(
	BGBCC_FrCC_State *ctx, frbc_type type, int opr,
	frbc_register src, char *name)
{
	frbc_register treg;
	int op, rt;

	if(opr==FR2C_CMP_AL)
		{ return(BGBCC_FrBC_EmitJump(ctx, name)); }
	if(opr==FR2C_CMP_NV)
		{ return(FR2C_STATUS_NO); }

	if(BGBCC_FrBC_IsRegBasicP(ctx, src) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
	{
		op=-1;
		if(BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_JCMP_RZI; }
		if(BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_JCMP_RZL; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_JCMP_RZF; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_JCMP_RZD; }
	
		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr16(ctx, name);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegArgBasicP(ctx, src) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
	{
		op=-1;
		if(BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_JCMP_AZI; }
		if(BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_JCMP_AZL; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_JCMP_AZF; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_JCMP_AZD; }

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr16(ctx, name);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegLocalBasicP(ctx, src) &&
		BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type))
	{
		op=-1;
		if(BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_JCMP_LZI; }
		if(BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_JCMP_LZL; }
		if(BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_JCMP_LZF; }
		if(BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_JCMP_LZD; }
	
		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr16(ctx, name);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegBaseP(ctx, src) &&
		BGBCC_FrBC_TypePointerP(ctx, type))
	{
		op=-1;
		if(BGBCC_FrBC_IsRegBasicP(ctx, src))
			{ op=FR2C_OP_JCMP_RZP; }
		if(BGBCC_FrBC_IsRegArgBasicP(ctx, src))
			{ op=FR2C_OP_JCMP_AZP; }
		if(BGBCC_FrBC_IsRegLocalBasicP(ctx, src))
			{ op=FR2C_OP_JCMP_LZP; }
	
		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|BGBCC_FrBC_GetRegID(ctx, src));
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitByte(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr16(ctx, name);
		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, src) ||
		BGBCC_FrBC_IsRegImmP(ctx, src))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, src);
		rt=BGBCC_FrBC_EmitJumpRegZero(ctx, type, opr, treg, name);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}

frbc_status BGBCC_FrBC_EmitJumpRegCmp(
	BGBCC_FrCC_State *ctx, frbc_type type, int opr,
	frbc_register srca, frbc_register srcb, char *name)
{
	frbc_register treg;
	int z, z2, y, rt, op, im12, im24;
	int i, j, k;

	if(opr==FR2C_CMP_AL)
		{ return(BGBCC_FrBC_EmitJump(ctx, name)); }
	if(opr==FR2C_CMP_NV)
		{ return(FR2C_STATUS_NO); }
	
	if(BGBCC_FrBC_IsRegImmZeroP(ctx, srcb))
	{
		rt=BGBCC_FrBC_EmitJumpRegZero(ctx, type, opr, srca, name);
		if(rt>=0)return(rt);
	}
	
	z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);
	z2=BGBCC_FrBC_GetTypeOperationExtZ(ctx, type);
	y=BGBCC_FrBC_GetRegOprMode2(ctx, srca, srcb);

	if(	BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		BGBCC_FrBC_TypeBaseSmallILFDP_P(ctx, type))
	{
		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_JCMP_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_JCMP_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_JCMP_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_JCMP_D; }
		if((op<0) && BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_JCMP_P; }
		if((op<0) && BGBCC_FrBC_TypeArrayP(ctx, type))
			{ op=FR2C_OP_JCMP_P; }

		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, srcb));
		BGBCC_FrBC_EmitWord(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr16(ctx, name);

		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		(z>=0) && (z<16))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_JCMP2);
		BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);

		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|0);
		BGBCC_FrBC_EmitByte(ctx,
			(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
			 BGBCC_FrBC_GetRegID(ctx, srcb));
		BGBCC_FrBC_EmitWord(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr16(ctx, name);

		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		BGBCC_FrBC_TypeBaseSmallILFDP_P(ctx, type))
	{
		op=-1;
		if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
			{ op=FR2C_OP_JCMPW_I; }
		if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
			{ op=FR2C_OP_JCMPW_L; }
		if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
			{ op=FR2C_OP_JCMPW_F; }
		if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
			{ op=FR2C_OP_JCMPW_D; }

		if((op<0) && BGBCC_FrBC_TypePointerP(ctx, type))
			{ op=FR2C_OP_JCMPW_P; }
		if((op<0) && BGBCC_FrBC_TypeArrayP(ctx, type))
			{ op=FR2C_OP_JCMPW_P; }

		if(op<0)
		{
			BGBCC_FrBC_TagError(ctx,
				FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
			return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
		}

		BGBCC_FrBC_EmitOpcode(ctx, op);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));
		BGBCC_FrBC_EmitDWord(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr32(ctx, name);
		return(FR2C_STATUS_YES);
	}

	if(	BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb) &&
		(y>=0) && (y<16) &&
		(z2>=0) && (z2<256))
	{
		BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_JCMP2W);

		BGBCC_FrBC_EmitByte(ctx, z2);
		BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
		BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srcb));
		BGBCC_FrBC_EmitDWord(ctx, 0);
		BGBCC_FrBC_EmitGotoAddr32(ctx, name);

		return(FR2C_STATUS_YES);
	}

	if(BGBCC_FrBC_IsRegImmP(ctx, srcb))
	{
		y=BGBCC_FrBC_GetRegOprMode1(ctx, srca);
		im12=BGBCC_FrBC_GetRegImmLit12(ctx, srcb, type);
		im24=BGBCC_FrBC_GetRegImmLit24(ctx, srcb, type);

		if(	BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
			BGBCC_FrBC_IsRegImmInt12P(ctx, srcb) &&
			(	BGBCC_FrBC_TypeSmallIntP(ctx, type) ||
				BGBCC_FrBC_TypeSmallLongP(ctx, type)) &&
			(y>=0) && (y<16))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_JCMP_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_JCMP_LC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			i=BGBCC_FrBC_GetRegImmIntValue(ctx, srcb);

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, (opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|
				((i>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, i&255);
			BGBCC_FrBC_EmitWord(ctx, 0);
			BGBCC_FrBC_EmitGotoAddr16(ctx, name);

			return(FR2C_STATUS_YES);
		}

		if(	BGBCC_FrBC_IsRegBaseP(ctx, srca) &&
			BGBCC_FrBC_TypeBaseSmallILFD_P(ctx, type) &&
//			(	BGBCC_FrBC_TypeFloatP(ctx, type) ||
//				BGBCC_FrBC_TypeDoubleP(ctx, type)) &&
			(im12>=0) && (y>=0) && (y<16) &&
			(opr>=0) && (opr<16))
		{
			op=-1;
			if((op<0) && BGBCC_FrBC_TypeSmallIntP(ctx, type))
				{ op=FR2C_OP_JCMP_IC; }
			if((op<0) && BGBCC_FrBC_TypeSmallLongP(ctx, type))
				{ op=FR2C_OP_JCMP_LC; }
			if((op<0) && BGBCC_FrBC_TypeFloatP(ctx, type))
				{ op=FR2C_OP_JCMP_FC; }
			if((op<0) && BGBCC_FrBC_TypeDoubleP(ctx, type))
				{ op=FR2C_OP_JCMP_DC; }

			if(op<0)
			{
				BGBCC_FrBC_TagError(ctx,
					FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
				return(FR2C_STATUS_ERR_UNHANDLEDTYPE);
			}

			BGBCC_FrBC_EmitOpcode(ctx, op);
			BGBCC_FrBC_EmitByte(ctx, 0x80|(opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx,
				(BGBCC_FrBC_GetRegID(ctx, srca)<<4)|((im12>>8)&15));
			BGBCC_FrBC_EmitByte(ctx, im12&255);
			BGBCC_FrBC_EmitWord(ctx, 0);
			BGBCC_FrBC_EmitGotoAddr16(ctx, name);

			return(FR2C_STATUS_YES);
		}

#if 1
		if(	
//			BGBCC_FrBC_IsRegBasicP(ctx, dst) &&
			BGBCC_FrBC_IsRegBasicP(ctx, srca) &&
			(im12>=0) && (y>=0) && (y<16) &&
			(z>=0) && (z<16))
		{
//			z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_JCMP2_C);
			BGBCC_FrBC_EmitByte(ctx, (z<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, 0x80|
				(opr<<4)|BGBCC_FrBC_GetRegID(ctx, srca));
			BGBCC_FrBC_EmitWord(ctx, im12);
			BGBCC_FrBC_EmitLitZ12(ctx, z);
			BGBCC_FrBC_EmitWord(ctx, 0);
			BGBCC_FrBC_EmitGotoAddr16(ctx, name);

			return(FR2C_STATUS_YES);
		}
#endif

#if 1
		if(
//			BGBCC_FrBC_IsRegExtBasicP(ctx, dst) &&
//			BGBCC_FrBC_IsRegExtBasicP(ctx, srca) &&
			BGBCC_FrBC_IsRegBaseExtP(ctx, srca) &&
			(im24>=0) && (y>=0) && (y<16) &&
			(z2>=0) && (z2<256))
		{
//			z=BGBCC_FrBC_GetTypeOperationZ(ctx, type);

			BGBCC_FrBC_EmitOpcode(ctx, FR2C_OP_JCMP2W_C);
			BGBCC_FrBC_EmitByte(ctx, z2);

			BGBCC_FrBC_EmitByte(ctx, 0x80|(opr<<4)|y);
			BGBCC_FrBC_EmitByte(ctx, BGBCC_FrBC_GetRegID(ctx, srca));
			BGBCC_FrBC_EmitWord24(ctx, im24);
			BGBCC_FrBC_EmitLitZ24(ctx, z2);
			BGBCC_FrBC_EmitDWord(ctx, 0);
			BGBCC_FrBC_EmitGotoAddr32(ctx, name);

			return(FR2C_STATUS_YES);
		}
#endif

		if(BGBCC_FrBC_IsRegGlobalP(ctx, srca) ||
			BGBCC_FrBC_IsRegImmP(ctx, srca))
		{
			BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
			BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
			rt=BGBCC_FrBC_EmitJumpRegCmp(ctx, type, opr, treg, srcb, name);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
			if(rt>=0)return(rt);
		}

		if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb) ||
			BGBCC_FrBC_IsRegImmP(ctx, srcb))
		{
			BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
			BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
			rt=BGBCC_FrBC_EmitJumpRegCmp(ctx, type, opr, srca, treg, name);
			BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
			if(rt>=0)return(rt);
		}

		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
		return(FR2C_STATUS_ERR_BADOPARGS);
	}

	if(	BGBCC_FrBC_IsRegImmP(ctx, srca) &&
		BGBCC_FrBC_IsRegBaseExtP(ctx, srcb))
	{
		switch(opr)
		{
		case FR2C_CMP_EQ: op=opr; break;
		case FR2C_CMP_NE: op=opr; break;
		case FR2C_CMP_AL: op=opr; break;
		case FR2C_CMP_NV: op=opr; break;
		case FR2C_CMP_LT: op=FR2C_CMP_GE; break;
		case FR2C_CMP_GT: op=FR2C_CMP_LE; break;
		case FR2C_CMP_LE: op=FR2C_CMP_GT; break;
		case FR2C_CMP_GE: op=FR2C_CMP_LT; break;
		default: op=FR2C_CMP_NV; break;
		}
		rt=BGBCC_FrBC_EmitJumpRegCmp(ctx, type, op, srcb, srca, name);
		if(rt>=0)return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srca) ||
		BGBCC_FrBC_IsRegImmP(ctx, srca))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, srca);
		rt=BGBCC_FrBC_EmitJumpRegCmp(ctx, type, opr, treg, srcb, name);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	if(BGBCC_FrBC_IsRegGlobalP(ctx, srcb) ||
		BGBCC_FrBC_IsRegImmP(ctx, srcb))
	{
		BGBCC_FrBC_RegisterAllocTemporary(ctx, type, &treg);
		BGBCC_FrBC_EmitMov(ctx, type, treg, srcb);
		rt=BGBCC_FrBC_EmitJumpRegCmp(ctx, type, opr, srca, treg, name);
		BGBCC_FrBC_RegisterCheckRelease(ctx, treg);
		if(rt>=0)return(rt);
	}

	BGBCC_FrBC_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_BADOPARGS));
	return(FR2C_STATUS_ERR_BADOPARGS);
}
