BGBCC_CCXL_BackendFuncs_vt bgbgc_shx_vt;
static int bgbgc_shx_init=0;

	
int BGBCC_SHXC_InitIface()
{
	if(bgbgc_shx_init)
		return(0);
	bgbgc_shx_init=1;

	bgbgc_shx_vt.tgtname="SHx";
	
	bgbgc_shx_vt.SetupContextForArch=	BGBCC_SHXC_SetupContextForArch;
	bgbgc_shx_vt.FlattenImage=			BGBCC_SHXC_FlattenImage;
	bgbgc_shx_vt.EndFunction=			BGBCC_SHXC_EndFunction;

	BGBCC_CCXL_RegisterBackend(&bgbgc_shx_vt);

	return(1);
}


ccxl_status BGBCC_SHXC_SetupContextForArch(BGBCC_TransState *ctx)
{
	BGBCC_SHX_Context *shctx;

	if(ctx->arch!=BGBCC_ARCH_SH)
		return(CCXL_STATUS_ERR_UNSUPPORTED);

	shctx=BGBCC_SHX_AllocContext();

	ctx->arch_sizeof_long=4;
	ctx->arch_sizeof_ptr=4;
	ctx->arch_sizeof_valist=64;
	ctx->arch_align_max=4;
	ctx->arch_align_objmin=4;
	ctx->uctx=shctx;

	shctx->tctx=ctx;
	shctx->is_le=0;

	switch(ctx->sub_arch)
	{
	case BGBCC_ARCH_SH_BX6B:
	case BGBCC_ARCH_SH_BX6L:
		ctx->arch_sizeof_long=8;
		ctx->arch_sizeof_ptr=8;
		ctx->arch_sizeof_valist=128;
		break;
	default:
		break;
	}

	switch(ctx->sub_arch)
	{
	case BGBCC_ARCH_SH_SH2:
	case BGBCC_ARCH_SH_SH2B:
	case BGBCC_ARCH_SH_SH4B:
	case BGBCC_ARCH_SH_BX1B:
	case BGBCC_ARCH_SH_BX6B:
		shctx->is_le=0;
		break;
	case BGBCC_ARCH_SH_SH4:
	case BGBCC_ARCH_SH_SH2L:
	case BGBCC_ARCH_SH_SH4L:
	case BGBCC_ARCH_SH_BX1L:
	case BGBCC_ARCH_SH_BX6L:
		shctx->is_le=1;
		break;
	default:
		break;
	}

	shctx->has_shad=0;
	shctx->has_movi20=0;
	shctx->has_bjx1jmp=0;
	shctx->has_movi20=0;
	shctx->has_misalgn=0;
	shctx->has_bjx1mov=0;
	shctx->has_bjx1ari=0;

	switch(ctx->sub_arch)
	{
	case BGBCC_ARCH_SH_SH2:
	case BGBCC_ARCH_SH_SH2L:
	case BGBCC_ARCH_SH_SH2B:
		break;
	case BGBCC_ARCH_SH_SH4:
	case BGBCC_ARCH_SH_SH4L:
	case BGBCC_ARCH_SH_SH4B:
		shctx->has_shad=1;
		break;

	case BGBCC_ARCH_SH_BX1L:
	case BGBCC_ARCH_SH_BX1B:
	case BGBCC_ARCH_SH_BX6L:
	case BGBCC_ARCH_SH_BX6B:
		shctx->has_shad=1;
//		shctx->has_movi20=0;
//		shctx->has_bjx1jmp=0;
		shctx->has_movi20=1;
		shctx->has_misalgn=1;
		shctx->has_bjx1mov=1;
		shctx->has_bjx1jmp=1;
		shctx->has_bjx1ari=1;
		break;

	default:
		break;
	}

	return(0);
}

bool BGBCC_SHXC_TypeIntRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypeFunctionP(ctx, ty))
		return(true);

	return(false);
}

bool BGBCC_SHXC_TypeInt2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
		return(true);

	if(BGBCC_CCXL_TypeVoidP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypeVariantP(ctx, ty))
//		return(true);

//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_SHXC_TypeFloatRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeFloatP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_SHXC_TypeFloat2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

int BGBCC_SHXC_TypeGetRegClassP(BGBCC_TransState *ctx, ccxl_type ty)
{
	int sz;

	if(BGBCC_CCXL_TypeArrayP(ctx, ty))
	{
		return(BGBCC_SH_REGCLS_AR_REF);
	}

	if(BGBCC_SHXC_TypeIntRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR);
	if(BGBCC_SHXC_TypeInt2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR2);
	if(BGBCC_SHXC_TypeFloatRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR);
	if(BGBCC_SHXC_TypeFloat2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR2);
		
	if(BGBCC_CCXL_TypeValueObjectP(ctx, ty))
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, ty);
		if(sz<=4)
			return(BGBCC_SH_REGCLS_VO_GR);
		if(sz<=8)
			return(BGBCC_SH_REGCLS_VO_GR2);
		return(BGBCC_SH_REGCLS_VO_REF);
	}

	return(BGBCC_SH_REGCLS_NONE);
}


int BGBCC_SHXC_EmitVaArg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dreg,
	ccxl_register sreg)
{
	int i, j, k;

//	sctx->sreg_live|=0x0010;
//	sctx->sfreg_live|=0x0000;

	BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R4);

	BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
//	BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_i");
//	BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);

	j=BGBCC_SHXC_TypeGetRegClassP(ctx, type);
	switch(j)
	{
	case BGBCC_SH_REGCLS_GR:
	case BGBCC_SH_REGCLS_VO_GR:
	case BGBCC_SH_REGCLS_VO_REF:
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_i");
//		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
//		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R0);

		break;
	case BGBCC_SH_REGCLS_GR2:
	case BGBCC_SH_REGCLS_VO_GR2:
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_l");
//		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_R6);
//		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
//			BGBCC_SH_REG_R0, BGBCC_SH_REG_R7);
//		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR6);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_EmitStoreVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR0);
		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_LR0);

		break;
	case BGBCC_SH_REGCLS_FR:
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_f");
		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_FR0);
		break;
	case BGBCC_SH_REGCLS_FR2:
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_d");
		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_DR0);
		break;
	default:
		break;
	}

	BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R4);

	return(1);
}

int BGBCC_SHXC_EmitVaEnd(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg)
{
	return(0);
}

int BGBCC_SHXC_EmitVaStart(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg,
	ccxl_register treg)
{
	BGBCC_CCXL_RegisterInfo *obj;
	int ni, nf, sofs;
	int ix, kx;
	int i, j, k;
	
	ix=treg.val&4095;
	sofs=999999;
	
	obj=ctx->cur_func;

#if 0
	BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R7);
//	BGBCC_SHXC_EmitLdaFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R7);

	ni=0; nf=0; kx=1;
	for(i=0; i<obj->n_args; i++)
	{
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
			if(ni>=4)
			{
				if(obj->args[i]->fxoffs<sofs)
					sofs=obj->args[i]->fxoffs;
				break;
			}
			
			if(i<ix)
				{ ni++; break; }
			
			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R0);
//			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
//				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R7, ni*4);

			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R7, kx*4);
			kx++;

			ni++;
			break;
		case BGBCC_SH_REGCLS_GR2:
			if(ni>=3)
			{
				if(obj->args[i]->fxoffs<sofs)
					sofs=obj->args[i]->fxoffs;
				break;
			}

			if(i<ix)
				{ ni+=2; break; }

			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx,
				k+0, BGBCC_SH_REG_R0);
			BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_R1);

//			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
//				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R7, (ni+0)*4);
//			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
//				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, BGBCC_SH_REG_R7, (ni+1)*4);

			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R7, (kx+0)*4);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R1, BGBCC_SH_REG_R7, (kx+1)*4);
			kx+=2;

			ni+=2;
			break;
		case BGBCC_SH_REGCLS_FR:
			if(nf>=8)
			{
				if(obj->args[i]->fxoffs<sofs)
					sofs=obj->args[i]->fxoffs;
				break;
			}

			if(i<ix)
				{ nf++; break; }

			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_FR0);

//			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
//				BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR0,
//				BGBCC_SH_REG_R7, 16+nf*4);

			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR0, BGBCC_SH_REG_R7, kx*4);
			kx++;

			nf++;
			break;
		case BGBCC_SH_REGCLS_FR2:
			nf+=nf&1;
			if(nf>=7)
			{
				if(obj->args[i]->fxoffs<sofs)
					sofs=obj->args[i]->fxoffs;
				break;
			}

			if(i<ix)
				{ nf+=2; break; }

			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_FR0);
			BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx,
				k+0, BGBCC_SH_REG_FR1);

//			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
//				BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR0,
//				BGBCC_SH_REG_R7, 16+(nf+1)*4);
//			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
//				BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR1,
//				BGBCC_SH_REG_R7, 16+(nf+0)*4);

			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR0,
				BGBCC_SH_REG_R7, (kx+0)*4);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_FMOVS, BGBCC_SH_REG_FR1,
				BGBCC_SH_REG_R7, (kx+1)*4);
			kx+=2;

			nf+=2;
			break;
		default:
			break;
		}

//		if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->args[i]->type))
//		{
//		}
	}

	BGBCC_SHX_EmitLoadRegImm(sctx,
		BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, sofs);
	BGBCC_SHX_EmitOpRegStRegDisp(sctx,
		BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R7, 0);

	BGBCC_SHX_EmitLoadRegImm(sctx,
		BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, 4);
	BGBCC_SHX_EmitOpRegStRegDisp(sctx,
		BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R7, 48);

	BGBCC_SHX_EmitLoadRegImm(sctx,
		BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, kx*4);
	BGBCC_SHX_EmitOpRegStRegDisp(sctx,
		BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R7, 52);
#endif

//	BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_start_1");
//	BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);
	return(0);
}

char *BGBCC_SHXC_DebugRegToStr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register reg)
{
	char tb[256];
	char *s0;
	s64 li;
	int i;
	
	if(reg.val==CCXL_REGID_REG_Z)
	{
		sprintf(tb, "ZZ");
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_LONG)
	{
		li=reg.val<<3; li=li>>3;
		sprintf(tb, "I64(%lld)", li);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY2_TYMASK)==CCXL_REGTY2_IMM_DOUBLE)
	{
		li=reg.val<<3;
		sprintf(tb, "F64(%g)", *(double *)(&li));
		return(bgbcc_rstrdup(tb));
	}
	
	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_TEMP)
	{
		sprintf(tb, "T%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_ARG)
	{
		sprintf(tb, "A%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_LOCAL)
	{
		sprintf(tb, "L%d.%d",
			(int)(reg.val&CCXL_REGID_BASEMASK),
			(int)((reg.val&CCXL_REGID_SEQMASK)>>12));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_GLOBAL)
	{
		i=(int)(reg.val&CCXL_REGID_REGMASK);
//		i=(int)(reg.val);
		s0=ctx->reg_globals[i]->name;
		if(!s0)s0="?";
		sprintf(tb, "G%d(%s)", i, s0);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_INT)
	{
		sprintf(tb, "I32(%d)", (int)(reg.val));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_LONG)
	{
		li=reg.val<<8; li=li>>8;
		sprintf(tb, "I64(%lld)", li);
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_FLOAT)
	{
		i=(int)(reg.val);
		sprintf(tb, "F32(%g)", *(float *)(&i));
		return(bgbcc_rstrdup(tb));
	}

	if((reg.val&CCXL_REGTY_REGMASK)==CCXL_REGTY_IMM_DOUBLE)
	{
		li=reg.val<<8;
		sprintf(tb, "F64(%g)", *(double *)(&li));
		return(bgbcc_rstrdup(tb));
	}

	sprintf(tb, "%016llX", reg.val);
	return(bgbcc_rstrdup(tb));
}

ccxl_status BGBCC_SHXC_PrintVirtOp(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0;

#if 1
	if(sctx->cgen_log)
	{
//		fprintf(sctx->cgen_log,
//			"\t%04X ty=%08X",
//			op->opn, op->type.val);

		s0=NULL;
		switch(op->opn)
		{
			case CCXL_VOP_NONE:				s0="NONE"; break;
			case CCXL_VOP_DBGFN:			s0="DBGFN"; break;
			case CCXL_VOP_DBGLN:			s0="DBGLN"; break;
			case CCXL_VOP_LABEL:			s0="LABEL"; break;
			case CCXL_VOP_JMP:				s0="JMP"; break;
			case CCXL_VOP_MOV:				s0="MOV"; break;
			case CCXL_VOP_JCMP_ZERO:		s0="JCMP_Z"; break;
			case CCXL_VOP_JCMP:				s0="JCMP"; break;
			case CCXL_VOP_CALL:				s0="CALL"; break;
			case CCXL_VOP_CSRV:				s0="CSRV"; break;
			case CCXL_VOP_RETDFL:			s0="RETDFL"; break;
			case CCXL_VOP_RETV:				s0="RETV"; break;
			case CCXL_VOP_RET:				s0="RET"; break;
			case CCXL_VOP_CONV:				s0="CONV"; break;
			case CCXL_VOP_UNARY:			s0="UNARY"; break;
			case CCXL_VOP_BINARY:			s0="BINARY"; break;
			case CCXL_VOP_COMPARE:			s0="COMPARE"; break;
			case CCXL_VOP_LDIXIMM:			s0="LDIXIMM"; break;
			case CCXL_VOP_STIXIMM:			s0="STIXIMM"; break;
			case CCXL_VOP_LDIX:				s0="LDIX"; break;
			case CCXL_VOP_STIX:				s0="STIX"; break;
			case CCXL_VOP_LEAIMM:			s0="LEAIMM"; break;
			case CCXL_VOP_LEA:				s0="LEA"; break;
			case CCXL_VOP_LDAVAR:			s0="LDAVAR"; break;
			case CCXL_VOP_SIZEOFVAR:		s0="SIZEOFVAR"; break;
			case CCXL_VOP_DIFFPTR:			s0="DIFFPTR"; break;
			case CCXL_VOP_OFFSETOF:			s0="OFFSETOF"; break;
			case CCXL_VOP_LOADSLOT:			s0="LOADSLOT"; break;
			case CCXL_VOP_STORESLOT:		s0="STORESLOT"; break;
			case CCXL_VOP_LOADSLOTADDR:		s0="LOADSLOTA"; break;
			case CCXL_VOP_LOADSLOTADDRID:	s0="LOADSLOTA"; break;
			case CCXL_VOP_INITOBJ:			s0="INITOBJ"; break;
			case CCXL_VOP_DROPOBJ:			s0="DROPOBJ"; break;
			case CCXL_VOP_INITARR:			s0="INITARR"; break;
			case CCXL_VOP_INITOBJARR:		s0="INITOBJARR"; break;
			case CCXL_VOP_LOADINITARR:		s0="LDINITARR"; break;
			case CCXL_VOP_LOADINITOBJARR:	s0="LDINITOBJARR"; break;
			case CCXL_VOP_VA_START:			s0="VA_START"; break;
			case CCXL_VOP_VA_END:			s0="VA_END"; break;
			case CCXL_VOP_VA_ARG:			s0="VA_ARG"; break;
		
		}

		if(s0)
		{
			fprintf(sctx->cgen_log,
				"\t%s",
				s0);
		}else
		{
			fprintf(sctx->cgen_log,
				"\t%04X",
				op->opn);
		}

		if(op->type.val<256)
		{
			fprintf(sctx->cgen_log,
				" ty=%02X", op->type.val);
		}else if(op->type.val<65536)
		{
			fprintf(sctx->cgen_log,
				" ty=%04X", op->type.val);
		}else
		{
			fprintf(sctx->cgen_log,
				" ty=%08X", op->type.val);
		}
		
		if(op->stype.val)
		{
			if(op->stype.val<256)
			{
				fprintf(sctx->cgen_log,
					" sty=%02X", op->stype.val);
			}else if(op->stype.val<65536)
			{
				fprintf(sctx->cgen_log,
					" sty=%04X", op->stype.val);
			}else
			{
				fprintf(sctx->cgen_log,
					" sty=%08X", op->stype.val);
			}
		}

		if(op->opr)
		{
			fprintf(sctx->cgen_log,
				" opr=%02X", op->opr);
		}

		if(op->dst.val)
		{
//			fprintf(sctx->cgen_log, " dst=%016llX", op->dst.val);
			fprintf(sctx->cgen_log, " dst=%s", 
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->dst));
		}
		if(op->srca.val)
		{
//			fprintf(sctx->cgen_log, " srca=%016llX", op->srca.val);
			fprintf(sctx->cgen_log, " srca=%s", 
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->srca));
		}
		if(op->srcb.val)
		{
//			fprintf(sctx->cgen_log, " srcb=%016llX", op->srcb.val);
			fprintf(sctx->cgen_log, " srcb=%s", 
				BGBCC_SHXC_DebugRegToStr(ctx, sctx, op->type, op->srcb));
		}

		if(op->imm.ul)
		{
			if((op->opn==CCXL_VOP_LOADSLOT) ||
				(op->opn==CCXL_VOP_STORESLOT) ||
				(op->opn==CCXL_VOP_LOADSLOTADDR))
			{
				fprintf(sctx->cgen_log, " obj=%03X.%02X",
					op->imm.obj.gid, op->imm.obj.fid);
			}else if(op->imm.ul==op->imm.ui)
				{ fprintf(sctx->cgen_log, " imm=%08X", op->imm.ui); }
			else 
				{ fprintf(sctx->cgen_log, " imm=%016llX", op->imm.ul); }
		}

		fprintf(sctx->cgen_log, "\n");
		fflush(sctx->cgen_log);
	}
#endif

	return(0);
}

ccxl_status BGBCC_SHXC_CompileVirtOp(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	char *s0;

//	BGBCC_SHXC_CompilePrintVirtOp(ctx, sctx, obj, op);

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;
	BGBCC_SHX_EmitCheckFlushIndexImm(sctx);

	switch(op->opn)
	{
	case CCXL_VOP_NONE:
		break;
	case CCXL_VOP_DBGFN:
		ctx->lfn=op->imm.str;
		break;
	case CCXL_VOP_DBGLN:
		ctx->lln=op->imm.si;
		break;
	case CCXL_VOP_LABEL:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);
		BGBCC_SHX_EmitLabel(sctx,
			op->imm.si);
		break;
	case CCXL_VOP_JMP:
//		BGBCC_SHX_EmitOpLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrUnknown(ctx, sctx);
		BGBCC_SHX_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRA,
			op->imm.si);
		BGBCC_SHX_EmitOpNone(sctx,
			BGBCC_SH_NMID_NOP);
//		BGBCC_SHX_EmitFlushIndexImm16(sctx);
//		BGBCC_SHX_EmitFlushIndexImm32(sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_MOV:
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;
	case CCXL_VOP_JCMP_ZERO:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);
		BGBCC_SHXC_EmitJCmpVRegZero(ctx, sctx, op->type,
			op->srca, op->opr, op->imm.si);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_JCMP:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrUnknown(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);
		BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr, op->imm.si);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CALL:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
//		BGBCC_SHXC_ResetFpscrDefaults(ctx, sctx);
//		BGBCC_SHX_EmitForceFlushIndexImm(sctx);

//		sctx->sreg_live|=0x00F4;
		sctx->sreg_live|=0x00F0;
		sctx->sfreg_live|=0x0FF0;
		BGBCC_SHXC_EmitCallVReg(ctx, sctx,
			op->type, op->dst, op->srca,
			op->imm.call.na, op->imm.call.args);

		sctx->sreg_live&=~0x00F0;
		sctx->sfreg_live&=~0x0FF0;

//		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
//		BGBCC_SHXC_EmitLabelFlushFpRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CSRV:
//		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
//		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHXC_EmitCsrvVReg(ctx, sctx,
			op->type, op->dst);
		break;
	case CCXL_VOP_RETDFL:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RETV:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_RET:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHXC_EmitReturnVReg(ctx, sctx,
			op->type, op->srca);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHX_EmitFlushIndexImmBasic(sctx);
		break;
	case CCXL_VOP_CONV:
		BGBCC_SHXC_EmitConvVRegVReg(ctx, sctx,
			op->type, op->stype,
			op->dst, op->srca);
		break;
	case CCXL_VOP_UNARY:
		BGBCC_SHXC_EmitUnaryVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca);
		break;
	case CCXL_VOP_BINARY:
		BGBCC_SHXC_EmitBinaryVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_COMPARE:
		BGBCC_SHXC_EmitCompareVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->opr, op->srca, op->srcb);
		break;
	case CCXL_VOP_LDIXIMM:
		BGBCC_SHXC_EmitLdixVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_STIXIMM:
		BGBCC_SHXC_EmitStixVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LDIX:
		BGBCC_SHXC_EmitLdixVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_STIX:
		BGBCC_SHXC_EmitStixVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_LEAIMM:
		BGBCC_SHXC_EmitLeaVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.si);
		break;
	case CCXL_VOP_LEA:
		BGBCC_SHXC_EmitLeaVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;
	case CCXL_VOP_LDAVAR:
		BGBCC_SHXC_EmitLdaVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;

	case CCXL_VOP_DIFFPTR:
		BGBCC_SHXC_EmitDiffPtrVRegVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca, op->srcb);
		break;

	case CCXL_VOP_LOADSLOT:
		BGBCC_SHXC_EmitLoadSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	case CCXL_VOP_STORESLOT:
		BGBCC_SHXC_EmitStoreSlotVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	case CCXL_VOP_LOADSLOTADDR:
		BGBCC_SHXC_EmitLoadSlotAddrVRegVRegImm(ctx, sctx, op->type,
			op->dst, op->srca, op->imm.obj.gid, op->imm.obj.fid);
		break;
	
	case CCXL_VOP_INITOBJ:
		break;
	case CCXL_VOP_DROPOBJ:
		break;
	case CCXL_VOP_INITARR:
		break;
	case CCXL_VOP_INITOBJARR:
		break;

	case CCXL_VOP_VA_START:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitVaStart(ctx, sctx, op->dst, op->srca);
		break;
	case CCXL_VOP_VA_END:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitVaEnd(ctx, sctx, op->srca);
		break;
	case CCXL_VOP_VA_ARG:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitVaArg(ctx, sctx, op->type, op->dst, op->srca);
		break;

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}

	BGBCC_SHX_EmitCheckFlushIndexImm(sctx);
	
	if(sctx->regalc_live || sctx->fregalc_live)
	{
		BGBCC_DBGBREAK
	}

	if(sctx->sreg_live || sctx->sfreg_live)
	{
		BGBCC_DBGBREAK
	}

#if 1
//	BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
//	BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);

//	BGBCC_SHXC_EmitSyncFpRegisters(ctx, sctx);
//	BGBCC_SHXC_EmitLabelFlushFpRegisters(ctx, sctx);
#endif
	
	return(1);
}

ccxl_status BGBCC_SHXC_PrintVirtTr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr, int idx)
{
	int i, j, k;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log,
			"   Tr: idx=%d base=%d num=%d\n",
			idx, tr->b_ops, tr->n_ops);
		fflush(sctx->cgen_log);
	}

	for(i=0; i<tr->n_ops; i++)
	{
		BGBCC_SHXC_PrintVirtOp(ctx, sctx, obj, obj->vop[tr->b_ops+i]);
	}

	return(0);
}

ccxl_status BGBCC_SHXC_CompileVirtTr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr, int idx)
{
	int i, j, k;

#if 0
	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log,
			"   Tr: idx=%d base=%d num=%d\n",
			idx, tr->b_ops, tr->n_ops);
		fflush(sctx->cgen_log);
	}
#endif

	for(i=0; i<tr->n_ops; i++)
	{
		sctx->tr_opnum=tr->b_ops+i;
		BGBCC_SHXC_CompileVirtOp(ctx, sctx, obj, obj->vop[tr->b_ops+i]);
	}

	BGBCC_SHX_EmitCheckFlushIndexImm(sctx);

	return(0);
}

ccxl_status BGBCC_SHXC_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	return(0);
}

ccxl_status BGBCC_SHXC_BuildFunctionBody(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	int bo, co, bo1, co1;
	int i, j, k;

	BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj);
	
	bo=BGBCC_SHX_EmitGetOffs(sctx);

	for(i=0; i<obj->n_vtr; i++)
	{
		bo1=BGBCC_SHX_EmitGetOffs(sctx);
		sctx->tr_trnum=i;
		BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		co1=BGBCC_SHX_EmitGetOffs(sctx);
		
		if(i<16)
		{
			sctx->fnsz_lvtr[i]=sctx->fnsz_vtr[i];
			sctx->fnsz_vtr[i]=co1-bo1;
			sctx->fnsz_dvtr[i]=sctx->fnsz_vtr[i]-sctx->fnsz_lvtr[i];
		}
	}

	co=BGBCC_SHX_EmitGetOffs(sctx);
	sctx->fnsz_bod=co-bo;

	BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);
}

ccxl_status BGBCC_SHXC_BuildFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	int fnsz[8];
	int fnsz_pro[8];
	int fnsz_epi[8];
	int fnsz_bod[8];
	int otrov, issta;
	int l0, np, co, lo, bo, bo1, co1, sz, sz1, msz, nsz;
	int i, j, k;

	sctx=ctx->uctx;

	if(obj->regflags&BGBCC_REGFL_CULL)
		return(0);

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: Begin %s\n",
			obj->name);
		fflush(sctx->cgen_log);

		for(i=0; i<obj->n_vtr; i++)
		{
			BGBCC_SHXC_PrintVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
	}

	
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		l0=BGBCC_SHX_GenLabel(sctx);
		l0=BGBCC_SHX_GetNamedLabel(sctx, obj->name);
		obj->fxoffs=l0;
	}
	
#if 1
	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: BegSim %s\n",
			obj->name);
		fflush(sctx->cgen_log);
	}

	sctx->cnt_set_fp32=0;
	sctx->cnt_set_fp64=0;

	otrov=sctx->lbltrov;
	sctx->reg_vsave=0;
	sctx->freg_vsave=0;
	lo=-1;
//	bo=BGBCC_SHX_EmitGetOffs(sctx);
	sctx->simfnsz=9999999;
	nsz=0;
	msz=999999999;

	sctx->need_farjmp=1;
	sctx->need_f16jmp=1;
	sctx->need_n12jmp=1;

	for(np=0; np<6; np++)
	{
		sctx->lbltrov=otrov;

		sctx->is_stable=1;		//pretend stable
		if(np==0)
			sctx->is_stable=0;

		BGBCC_SHX_SetBeginSimPass(sctx);
		sctx->is_simpass=np+1;

		BGBCC_SHXC_SetupFrameLayout(ctx, sctx, obj);

		BGBCC_SHX_SetSectionName(sctx, ".text");
		BGBCC_SHX_EmitBAlign(sctx, 4);
//		BGBCC_SHX_EmitBAlign(sctx, 16);

		bo=BGBCC_SHX_EmitGetOffs(sctx);

		BGBCC_SHX_EmitLabel(sctx, l0);
		BGBCC_SHXC_BuildFunctionBody(ctx, sctx, obj);

#if 0
		BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj);
		
		for(i=0; i<obj->n_vtr; i++)
		{
			sctx->tr_trnum=i;
			BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
		}
		
		BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);
#endif

		co=BGBCC_SHX_EmitGetOffs(sctx);

		BGBCC_SHX_SetEndSimPass(sctx);
		
//		co=BGBCC_SHX_EmitGetOffs(sctx);
		sz=co-bo;
		if(sz<msz)msz=sz;
		if(sz>nsz)nsz=sz;
		sctx->simfnsz=sz;
		sctx->simfnmsz=msz;
		sctx->simfnnsz=nsz;
		
		fnsz[np]=sz;
		fnsz_pro[np]=sctx->fnsz_pro;
		fnsz_epi[np]=sctx->fnsz_epi;
		fnsz_bod[np]=sctx->fnsz_bod;
		
		issta=1;
		if(sctx->reg_vsave!=sctx->reg_save)
			issta=0;
		if(sctx->freg_vsave!=sctx->freg_save)
			issta=0;
		if(co!=lo)
			issta=0;
		if(sctx->is_stable&2)
			issta=0;
		
		sctx->reg_vsave=sctx->reg_save;
		sctx->freg_vsave=sctx->freg_save;
		lo=co;

//		if(issta)
		if(issta && (np>2))
			break;

		sctx->need_farjmp=0;
		sctx->need_f16jmp=0;
		sctx->need_n12jmp=0;
		if(sctx->simfnsz>244)
			sctx->need_n12jmp=1;
		if(sctx->simfnnsz>3840)
			sctx->need_f16jmp=1;
		if(sctx->simfnnsz>30720)
			sctx->need_farjmp=1;
	}
	
	sctx->is_stable=issta;
	sctx->simfnsz=sz;
	sctx->simfnmsz=msz;
	sctx->simfnnsz=nsz;

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: EndSim %s np=%d\n",
			obj->name, np);
		fflush(sctx->cgen_log);
	}
#endif

	sctx->lbltrov=otrov;

	sz1=sctx->simfnnsz+((sctx->simfnnsz-sctx->simfnmsz)*2);
	sctx->simfnsz=sz1;
	
	sctx->need_farjmp=0;
	sctx->need_f16jmp=0;
	sctx->need_n12jmp=0;
//	if((obj->srctok*4)>=128)
//	if((obj->n_vop*6)>=128)
	if(sctx->simfnsz>244)
//	if(sctx->simfnnsz>120)
//	if(sctx->simfnnsz>60)
		sctx->need_n12jmp=1;
//	if((obj->srctok*4)>=2048)
//	if((obj->n_vop*6)>=2048)
	if(sctx->simfnsz>3840)
		sctx->need_f16jmp=1;
	if(sctx->simfnsz>30720)
		sctx->need_farjmp=1;
	
	BGBCC_SHXC_SetupFrameLayout(ctx, sctx, obj);

	BGBCC_SHX_SetSectionName(sctx, ".text");
	BGBCC_SHX_EmitBAlign(sctx, 4);
//	BGBCC_SHX_EmitBAlign(sctx, 16);

	bo1=BGBCC_SHX_EmitGetOffs(sctx);

	BGBCC_SHX_EmitLabel(sctx, l0);

	BGBCC_SHXC_BuildFunctionBody(ctx, sctx, obj);

#if 0
	BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj);
	
	for(i=0; i<obj->n_vtr; i++)
	{
		sctx->tr_trnum=i;
		BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->vtr[i], i);
	}
	
	BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);
#endif

	co1=BGBCC_SHX_EmitGetOffs(sctx);
	sz1=co1-bo1;

	fnsz[np]=sz1;
	fnsz_pro[np]=sctx->fnsz_pro;
	fnsz_epi[np]=sctx->fnsz_epi;
	fnsz_bod[np]=sctx->fnsz_bod;

	if(sctx->reg_vsave!=sctx->reg_save)
		{ BGBCC_DBGBREAK }
	if(sctx->freg_vsave!=sctx->freg_save)
		{ BGBCC_DBGBREAK }

//	if(sctx->is_stable&2)
//		{ BGBCC_DBGBREAK }

	if(bo1!=bo)
		{ BGBCC_DBGBREAK }
		
	k=sz1-sz;
	if(k<0)k=-k;
//	if(k>64)
//	if(sz1<msz)
//		{ BGBCC_DBGBREAK }
//	if(sz1>nsz)
	if(sz1>sctx->simfnsz)
		{ BGBCC_DBGBREAK }

	if(sctx->cgen_log)
	{
		fprintf(sctx->cgen_log, "BGBCC_SHXC_BuildFunction: End %s\n",
			obj->name);
		fflush(sctx->cgen_log);
	}

	return(0);
}

ccxl_status BGBCC_SHXC_BuildGlobal_EmitLitAsType(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type, ccxl_register value)
{
	BGBCC_CCXL_LiteralInfo *litobj;
	BGBCC_CCXL_LiteralInfo *littyobj;
	BGBCC_CCXL_RegisterInfo *gblobj;
	ccxl_register treg;
	ccxl_type tty;
	s64 li, lj;
	double f, g;
	char *s0;
	int n, sz, asz, bty;
	int i, j, k;

	bty=BGBCC_CCXL_GetTypeBaseType(ctx, type);
	
	if(BGBCC_CCXL_TypeArrayP(ctx, type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, type, &tty);

		if(BGBCC_CCXL_IsRegImmLiteralP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			litobj=ctx->literals[i];
//			tty=litobj->decl->type;
//			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);
			asz=BGBCC_CCXL_TypeGetArrayDimSize(ctx, type);

			n=litobj->decl->n_listdata;
			if((asz>0) && (asz<n))
			{
				n=asz;
			}
				
			for(i=0; i<n; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, litobj->decl->listdata[i]);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=n; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
//			tty=litobj->decl->type;
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(i=0; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, type);

//			j=strlen(s0);
			j=BGBCP_StrlenUTF8(s0);
			
			if(asz<j)j=asz;
			for(i=0; i<j; i++)
			{
				k=BGBCP_ParseChar(&s0);
				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
//				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, s0[i]);
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}

			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			if(asz<=0)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, type))
	{
		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmStringP(ctx, value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, value);

			if(type.val==CCXL_VTY_PCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			if(type.val==CCXL_VTY_PWCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2UCS2(sctx, s0);				
			else
				k=BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);

			BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
			BGBCC_SHX_EmitDWord(sctx, 0);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}
		
		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
//			gblobj=ctx->reg_globals[i];
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, i);
			BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
			BGBCC_SHX_EmitDWord(sctx, 0);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_I) || (bty==CCXL_TY_UI) ||
		(bty==CCXL_TY_NL) || (bty==CCXL_TY_UNL))
	{
		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_L) || (bty==CCXL_TY_UL))
	{
		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			BGBCC_SHX_EmitQWord(sctx, li);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_F)
	{
		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(float *)(&j))=li;
			BGBCC_SHX_EmitDWord(sctx, j);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(float *)(&j))=f;
			BGBCC_SHX_EmitDWord(sctx, j);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(bty==CCXL_TY_D)
	{
		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) ||
			BGBCC_CCXL_IsRegImmLongP(ctx, value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, value);
			(*(double *)(&lj))=li;
			BGBCC_SHX_EmitQWord(sctx, lj);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmFloatP(ctx, value) ||
			BGBCC_CCXL_IsRegImmDoubleP(ctx, value))
		{
			f=BGBCC_CCXL_GetRegImmDoubleValue(ctx, value);
			(*(double *)(&lj))=f;
			BGBCC_SHX_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SB) || (bty==CCXL_TY_UB))
	{
//		BGBCC_SHX_EmitBAlign(sctx, 4);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_SHX_EmitByte(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if((bty==CCXL_TY_SS) || (bty==CCXL_TY_US))
	{
		BGBCC_SHX_EmitBAlign(sctx, 2);

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, value);
			BGBCC_SHX_EmitWord(sctx, i);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeSgInt128P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmInt128P(ctx, value) ||
			BGBCC_CCXL_IsRegImmILFDP(ctx, value))
		{
			BGBCC_CCXL_GetRegImmInt128Value(ctx, value, &li, &lj);
			BGBCC_SHX_EmitQWord(sctx, li);
			BGBCC_SHX_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeFloat128P(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmFloat128P(ctx, value) ||
			BGBCC_CCXL_IsRegImmILFDP(ctx, value))
		{
			BGBCC_CCXL_GetRegImmFloat128Value(ctx, value, &li, &lj);
			BGBCC_SHX_EmitQWord(sctx, li);
			BGBCC_SHX_EmitQWord(sctx, lj);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	if(BGBCC_CCXL_TypeValueObjectP(ctx, type))
	{
		if(BGBCC_CCXL_IsRegImmLiteralP(ctx, value))
		{
			i=value.val&CCXL_REGINT_MASK;
			litobj=ctx->literals[i];
			tty=litobj->decl->type;

			littyobj=NULL;
			i=BGBCC_CCXL_GetTypeBaseType(ctx, tty);
			if(i>=256)
				{ littyobj=ctx->literals[i-256]; }

			if(littyobj && (littyobj->littype==CCXL_LITID_STRUCT))
			{
				n=littyobj->decl->n_fields;
				if(litobj->decl->n_listdata<n)
					n=litobj->decl->n_listdata;
				
				for(i=0; i<n; i++)
				{
					BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type,
						litobj->decl->listdata[i]);
				}

				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
				for(i=n; i<littyobj->decl->n_fields; i++)
				{
					BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
						littyobj->decl->fields[i]->type, treg);
				}
				return(1);
			}

			BGBCC_CCXL_StubError(ctx);
			return(0);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, value) &&
			BGBCC_CCXL_IsRegImmZeroP(ctx, value))
		{
			sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, type);
			BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
			return(1);
		}

		BGBCC_CCXL_StubError(ctx);
		return(0);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

ccxl_status BGBCC_SHXC_BuildGlobal(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	BGBCC_CCXL_LiteralInfo *litobj;
	BGBCC_CCXL_LiteralInfo *littyobj;
	ccxl_register treg;
	ccxl_type tty;
	char *s0;
	int l0, sz, asz;
	s64 li;
	int n;
	int i, j, k;

	sctx=ctx->uctx;
	
	if(obj->regflags&BGBCC_REGFL_CULL)
		return(0);
	
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		lbl=BGBCC_SHX_LookupNamedLabel(ctx, name);
//		l0=BGBCC_SHX_GenLabel(sctx);
		l0=BGBCC_SHX_GetNamedLabel(sctx, obj->name);
		obj->fxoffs=l0;
	}
	
	if(!obj->value.val)
	{
//		if(!strcmp(obj->name, "snd_scaletable"))
//			i=-1;
		
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);
		if(sz<1)sz=1;
		sz=(sz+3)&(~3);

		BGBCC_SHX_EmitCommSym(sctx, l0, sz);

//		BGBCC_SHX_SetSectionName(sctx, ".bss");
//		BGBCC_SHX_EmitBAlign(sctx, 4);
//		BGBCC_SHX_EmitLabel(sctx, l0);
//		BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
		return(1);
	}
	
	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);

	BGBCC_SHX_SetSectionName(sctx, ".data");
	BGBCC_SHX_EmitBAlign(sctx, 4);
	BGBCC_SHX_EmitLabel(sctx, l0);
	
	if(BGBCC_CCXL_TypeSmallIntP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeSgLongP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_SHX_EmitQWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_SHX_EmitQWord(sctx, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_TypeFloatP(ctx, obj->type) ||
		BGBCC_CCXL_TypeDoubleP(ctx, obj->type))
	{
		BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypePointerP(ctx, obj->type))
	{
		if(BGBCC_CCXL_IsRegImmStringP(ctx, obj->value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, obj->value);
			if(obj->type.val==CCXL_VTY_PCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2ASCII(sctx, s0);
			if(obj->type.val==CCXL_VTY_PWCHAR)
				k=BGBCC_SHX_EmitGetStrtabLabelUTF2UCS2(sctx, s0);				
			else
				k=BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
			BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
			BGBCC_SHX_EmitDWord(sctx, 0);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmGlobalAddrP(ctx, obj->value))
		{
			i=obj->value.val&CCXL_REGINT_MASK;
			k=BGBCC_SHXC_GetGblIndexLabel(ctx, sctx, i);
			BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
			BGBCC_SHX_EmitDWord(sctx, 0);
			return(1);
		}

		if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}
	}

	if(BGBCC_CCXL_IsRegImmLiteralP(ctx, obj->value))
	{
//		if(!strcmp(obj->name, "keynames"))
//		{
//			k=-1;
//		}
	
		BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
			obj->type, obj->value);
		return(1);
	}

	if(BGBCC_CCXL_TypeArrayP(ctx, obj->type))
	{
		BGBCC_CCXL_TypeDerefType(ctx, obj->type, &tty);

		if(BGBCC_CCXL_IsRegImmStringP(ctx, obj->value))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, obj->value);
			asz=BGBCC_CCXL_TypeGetArraySize(ctx, obj->type);
			
			if(asz<=0)
			{
				asz=strlen(s0)+1;
				asz=(asz+3)&(~3);
			}

//			j=strlen(s0);
			j=BGBCP_StrlenUTF8(s0);
			if((asz>0) && (asz<j))j=asz;
			for(i=0; i<j; i++)
			{
				k=BGBCP_ParseChar(&s0);
				BGBCC_CCXL_GetRegForIntValue(ctx, &treg, k);
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			
			BGBCC_CCXL_GetRegForIntValue(ctx, &treg, 0);
			for(; i<asz; i++)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}

			if(asz<=0)
			{
				BGBCC_SHXC_BuildGlobal_EmitLitAsType(ctx, sctx,
					tty, treg);
			}
			return(1);
		}
	}
	
	BGBCC_CCXL_StubError(ctx);
	BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
	return(1);
}

ccxl_status BGBCC_SHXC_BuildAsmBlob(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	int l0, sz;
	int i, j, k;

	sctx=ctx->uctx;
	
	BGBCC_SHXC_AssembleBuffer(ctx, sctx, (char *)(obj->text));
	return(1);
}

int BGBCC_SHXC_LookupLabelIndex(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	int lblid)
{

	return(BGBCC_SHX_LookupLabelIndex(sctx, lblid));

#if 0
	int i, j, k;
	
	for(i=0; i<sctx->nlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}
	return(-1);
#endif
}

int BGBCC_SHXC_LookupLabelImgOffs(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	j=BGBCC_SHXC_LookupLabelIndex(ctx, sctx, lblid);
	k=sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

int BGBCC_SHXC_LookupLabelImgVA(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	int lblid)
{
	int i, j, k;
	
	j=BGBCC_SHXC_LookupLabelIndex(ctx, sctx, lblid);
	k=sctx->sec_lva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
	return(k);
}

extern char *bgbcc_shx_srcidx[256];
extern int bgbcc_shx_nsrcidx;

ccxl_status BGBCC_SHXC_ApplyImageRelocs(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	byte *imgbase)
{
	byte *ctl, *ctr;
	char *s0;
	int en;
	s32 b, d, b1, d1, w0, w1;
	int i, j, k;

	en=(sctx->is_le==0);

	for(i=0; i<sctx->nrlc; i++)
	{
		j=BGBCC_SHXC_LookupLabelIndex(ctx, sctx, sctx->rlc_id[i]);
		if(j<0)
		{
			k=sctx->rlc_id[i];
			s0=BGBCC_SHX_LookupNameForLabel(sctx, k);
			if(s0)
			{
				printf("BGBCC_SHXC_ApplyImageRelocs: "
					"Missing Label N=%s\n", s0);
			}else
			{
				printf("BGBCC_SHXC_ApplyImageRelocs: "
					"Missing Label ID=%08X\n", k);
				if((k&(~65535))==CCXL_LBL_GENSYM2BASE)
				{
					b=k&65535;
					b1=sctx->genlabel_srcpos[b];
					printf("\t%s:%d\n",
						bgbcc_shx_srcidx[b1>>16], (u16)b1);
				}
			}
//			__debugbreak();
			continue;
		}
		ctl=imgbase+sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
		ctr=imgbase+sctx->sec_rva[sctx->rlc_sec[i]]+sctx->rlc_ofs[i];
		
		if((ctl<=imgbase) || (ctl>(imgbase+0x1000000)))
			__debugbreak();
		if((ctr<=imgbase) || (ctr>(imgbase+0x1000000)))
			__debugbreak();

		if(		(sctx->lbl_ofs[j]<0) ||
				(sctx->lbl_ofs[j] > sctx->sec_lsz[sctx->lbl_sec[j]]))
			__debugbreak();
		
		d=ctl-ctr;
		switch(sctx->rlc_ty[i])
		{
		case BGBCC_SH_RLC_REL8:
			b=bgbcc_gets8en(ctr, en);
			d1=b+d;
			if(((sbyte)d1)!=d1)
				__debugbreak();
			bgbcc_sets8en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL16:
			b=bgbcc_gets16en(ctr, en);
			d1=b+d;
			if(((s16)d1)!=d1)
				__debugbreak();
			bgbcc_sets16en(ctr, en, d1);
			break;
		case BGBCC_SH_RLC_REL32:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d);
			break;
		case BGBCC_SH_RLC_REL32B:
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b-d);
			break;

		case BGBCC_SH_RLC_ABS32:
			d1=0x0C000000+(ctl-imgbase);
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d1);
			break;

		case BGBCC_SH_RLC_RELW12:
			b=bgbcc_getu16en(ctr, en);
			b1=(b<<20)>>20;
//			d1=b1+(d-4);
			d1=b1+((d-4)>>1);
			if(((d1<<20)>>20)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8:
			b=bgbcc_getu16en(ctr, en);
			b1=(b<<24)>>24;
//			d1=b1+(d-4);
			d1=b1+((d-4)>>1);
			if(((d1<<24)>>24)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;

		case BGBCC_SH_RLC_RELW12L:
			b=bgbcc_getu16en(ctr, en);
			b1=(b<<20)>>20;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<20)>>20)!=d1)
			if((d1&4095)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8L:
			b=bgbcc_getu16en(ctr, en);
			b1=(b<<24)>>24;
//			d1=b1+(d-4);
			d1=b1+((d-2)>>2);
//			if(((d1<<24)>>24)!=d1)
			if((d1&255)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;

		case BGBCC_SH_RLC_RELW16_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
			b=((w0&255)<<8)|(w1&255);
			b1=(b<<16)>>16;

			d1=b1+((d-6)>>1);
			if(((d1<<16)>>16)!=d1)
				__debugbreak();
			w0=(w0&0xFF00)|((d1>>8)&0x00FF);
			w1=(w1&0xFF00)|((d1   )&0x00FF);
			bgbcc_setu16en(ctr+0, en, w0);
			bgbcc_setu16en(ctr+2, en, w1);
			break;

		case BGBCC_SH_RLC_RELW20_BJX:
			w0=bgbcc_getu16en(ctr+0, en);
			w1=bgbcc_getu16en(ctr+2, en);
			
//			b=(w0&255)|((w1&255)<<8);
			b=((w0&255)<<8)|(w1&255);
			b1=(b<<12)>>12;

			d1=b1+((d-6)>>1);
			if(((d1<<12)>>12)!=d1)
				__debugbreak();
			w0=(w0&0xFF00)|((d1>>12)&0x00FF);
			w1=(w1&0xF000)|((d1    )&0x0FFF);
			bgbcc_setu16en(ctr+0, en, w0);
			bgbcc_setu16en(ctr+2, en, w1);
			break;

		default:
			__debugbreak();
		}
	}
	
	return(0);
}

ccxl_status BGBCC_SHXC_FlattenImageELF(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_SHX_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct;
	int en, ofs, ofs_sdat, ofs_iend, ofs_mend;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int lb_strt, va_strt;
	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int i, j, k;

	sctx=ctx->uctx;

	BGBCC_SHX_EmitGetStrtabLabel(sctx, "");

	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
			BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
		}else
		{
			switch(i)
			{
			case BGBCC_SH_CSEG_TEXT: s0=".text"; break;
			case BGBCC_SH_CSEG_STRTAB: s0=".strtab"; break;
			case BGBCC_SH_CSEG_GOT: s0=".got"; break;
			case BGBCC_SH_CSEG_DATA: s0=".data"; break;
			case BGBCC_SH_CSEG_BSS: s0=".bss"; break;
			default: s0=".unknown"; break;
			}

			BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
			sctx->sec_name[i]=s0;
		}
	}

	en=(sctx->is_le==0);
	ne_phdr=1;
	ne_shdr=sctx->nsec+1;
	
	of_phdr=64;
	
	of_shdr=of_phdr+(ne_phdr*32);
	of_shdr=(of_shdr+15)&(~15);

	ofs_sdat=of_shdr+(ne_shdr*40);
	ofs_sdat=(ofs_sdat+63)&(~63);

	k=ofs_sdat;
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=0x0C000000+k;
		sctx->sec_lsz[i]=j;
		memcpy(obuf+k, sctx->sec_buf[i], j);
		k+=j;
		k=(k+63)&(~63);

		s0=sctx->sec_name[i];
		printf("%d: %s %08X..%08X %d\n", i, s0,
			sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
			sctx->sec_lsz[i]);

	}
	ofs_iend=k;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	sctx->sec_rva[i]=k;
	sctx->sec_lva[i]=0x0C000000+k;
	sctx->sec_lsz[i]=j;
	k+=j;
	k=(k+63)&(~63);

	s0=sctx->sec_name[i];
	printf("%d: %s %08X..%08X %d\n", i, s0,
		sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
		sctx->sec_lsz[i]);

	ofs_mend=k;
	
	lb_strt=0;
//	lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "__start");
//	if(lb_strt<=0)
//		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "___start");
	if(lb_strt<=0)
		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "_start");
	if(lb_strt<=0)
		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "__start");
	
	if(lb_strt>0)
		{ va_strt=BGBCC_SHXC_LookupLabelImgVA(ctx, sctx, lb_strt); }
	else
	{
		printf("BGBCC_SHXC_FlattenImageELF: No Entry Point Found\n");
		va_strt=0;
	}

	ct=obuf;
	ct[ 0]=0x7F;	ct[ 1]='E';			//magic
	ct[ 2]='L';		ct[ 3]='F';			//magic
	ct[ 4]=1;		ct[ 5]=en?2:1;		//width, endian
	ct[ 6]=1;		ct[ 7]=0;			//ELF version, OS ABI
	ct[ 8]=0;		ct[ 9]=0;			//pad
	ct[10]=0;		ct[11]=0;			//pad
	ct[12]=0;		ct[13]=0;			//pad
	ct[14]=0;		ct[15]=0;			//pad

	sn_strs=BGBCC_SH_CSEG_STRTAB+1;

	imty=2;
//	if(sctx->is_pic)
//		imty=3;

	bgbcc_setu16en(ct+16, en, imty);	//image type
	bgbcc_setu16en(ct+18, en, 0x2A);	//arch
	bgbcc_setu32en(ct+20, en, 1);		//ELF version
	bgbcc_setu32en(ct+24, en, va_strt);	//entry point
	bgbcc_setu32en(ct+28, en, of_phdr);	//program headers offset
	bgbcc_setu32en(ct+32, en, of_shdr);	//section headers offset
	bgbcc_setu32en(ct+36, en, 0);		//arch flags
	bgbcc_setu16en(ct+40, en, 52);		//header size
	bgbcc_setu16en(ct+42, en, 32);		//size of program header entry
	bgbcc_setu16en(ct+44, en, ne_phdr);	//number of program header entries
	bgbcc_setu16en(ct+46, en, 40);		//section header entry size
	bgbcc_setu16en(ct+48, en, ne_shdr);	//number of entries in section header
	bgbcc_setu16en(ct+50, en, sn_strs);	//index of section-header names

	ct=obuf+of_phdr;
	bgbcc_setu32en(ct+ 0, en, 1);			//segment type
	bgbcc_setu32en(ct+ 4, en, 0);			//segment image offset
	bgbcc_setu32en(ct+ 8, en, 0x0C000000);	//segment load address
	bgbcc_setu32en(ct+12, en, 0x0C000000);	//undefined (phys address)
	bgbcc_setu32en(ct+16, en, ofs_iend);	//size of segment in file
	bgbcc_setu32en(ct+20, en, ofs_mend);	//size of segment in memory
	bgbcc_setu32en(ct+24, en, 7);			//access flags
	bgbcc_setu32en(ct+28, en, 4096);		//alignment

	ct=obuf+of_shdr;
	ct+=40;
	for(i=0; i<sctx->nsec; i++)
	{
		s0=sctx->sec_name[i];
		if(s0)
		{
			nm=BGBCC_SHX_EmitGetStrtabSecOfs(sctx, s0);
		}

		fl=3;
		if(i==BGBCC_SH_CSEG_TEXT)fl|=4;

		lva=sctx->sec_lva[i];
		rva=sctx->sec_rva[i];
		lsz=sctx->sec_lsz[i];

		bgbcc_setu32en(ct+ 0, en, nm);		//name
		bgbcc_setu32en(ct+ 4, en, 1);		//type
		bgbcc_setu32en(ct+ 8, en, fl);		//flags
		bgbcc_setu32en(ct+12, en, lva);		//addr
		bgbcc_setu32en(ct+16, en, rva);		//offset
		bgbcc_setu32en(ct+20, en, lsz);		//size
		bgbcc_setu32en(ct+24, en, 0);		//link
		bgbcc_setu32en(ct+28, en, 0);		//info
		bgbcc_setu32en(ct+32, en, 64);		//addralign
		bgbcc_setu32en(ct+36, en, 0);		//entsize
		ct+=40;
	}

	BGBCC_SHXC_ApplyImageRelocs(ctx, sctx, obuf);

#if 1
	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_SHXC_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
	}
	fclose(mapfd);
#endif

	*rosz=ofs_iend;

	return(0);
}



int BGBCC_SHXC_CoffSectionFlags(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	char *name)
{
	if(!strcmp(name, ".text"))
	{
		return(BGBCC_COFF_SCNT_CODE|
			BGBCC_COFF_SCNT_EXECUTE|
			BGBCC_COFF_SCNT_READ);
	}
	if(!strcmp(name, ".data") ||
		!strcmp(name, ".idata") ||
		!strcmp(name, ".vsdata") ||
		!strcmp(name, ".tls") ||
		!strcmp(name, ".sdata"))
	{
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_WRITE);
	}
	if(!strcmp(name, ".bss") ||
		!strcmp(name, ".sbss"))
	{
		return(BGBCC_COFF_SCNT_UDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_WRITE);
	}
	if(!strcmp(name, ".rdata") ||
		!strcmp(name, ".edata") ||
		!strcmp(name, ".xdata") ||
		!strcmp(name, ".srdata") ||
		!strcmp(name, ".rsrc") ||
		!strcmp(name, ".strtab"))
	{
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ);
	}
	if(!strcmp(name, ".reloc"))
	{
		return(BGBCC_COFF_SCNT_IDATA|
			BGBCC_COFF_SCNT_READ|
			BGBCC_COFF_SCNT_DISCARD);
	}

	return(BGBCC_COFF_SCNT_IDATA|
		BGBCC_COFF_SCNT_READ|
		BGBCC_COFF_SCNT_WRITE);
//	return(0);
}

int bgbcc_shxc_qrsort(u32 *arr, int cnt, int rd)
{
	int lo, hi, cn, pvt;
	u32 v, v0, v1, v2;
	int i, j, k;
	
	if((cnt<8) || (rd>=20))
	{
		for(i=0; i<cnt; i++)
			for(j=i+1; j<cnt; j++)
		{
			if(arr[j]<arr[i])
				{ v=arr[i]; arr[i]=arr[j]; arr[j]=v; }
		}
		return(0);
	}
	
	cn=cnt>>1;
	v0=arr[cn>>1];
	v1=arr[cn];
	v2=arr[cn+(cn>>1)];
	if(v0>v1) { v=v0; v0=v1; v1=v; }
	if(v0>v2) { v=v0; v0=v2; v2=v; }
	if(v1>v2) { v=v1; v1=v2; v2=v; }
	pvt=v1;
	
	lo=0; hi=cnt;
	
	while(lo<hi)
	{
		v=arr[lo];
		if(v<=pvt)
			{ lo++; continue; }
		hi--;		v1=arr[hi];
		arr[hi]=v;	arr[lo]=v1;
	}
	
	bgbcc_shxc_qrsort(arr, lo, rd+1);
	bgbcc_shxc_qrsort(arr+lo, cnt-lo, rd+1);
	return(0);
}

ccxl_status BGBCC_SHXC_FlattenImagePECOFF(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
//	int rlc_rva_page[4096];
//	int rlc_sz_page[4096];
	u32 rlctab[65536];

	BGBCC_SHX_Context *sctx;
	FILE *mapfd;
	char *s0;
	byte *ct, *ct0, *ct1, *ctb;
	int en, ofs, ofs_sdat, ofs_iend, ofs_mend;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int lb_strt, va_strt;
	int img_base;
	int nm, fl, lva, rva, lsz, sn_strs, imty;
	int lpg, szrlc, ofsrlc, nrlce;
	int i, j, k;

	sctx=ctx->uctx;

	img_base=0x0C000000;
	k=0x400;
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=img_base+k;
		sctx->sec_lsz[i]=j;
		k+=j;
		k=(k+63)&(~63);
	}

	lpg=-1; szrlc=0; nrlce=0;
	for(i=0; i<sctx->nrlc; i++)
	{
		if(	(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS16) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS32) &&
			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS64))
				continue;
			
		j=sctx->rlc_sec[i];
		rva=sctx->sec_rva[j]+
			sctx->rlc_ofs[i];

		rlctab[nrlce++]=rva;

		if((rva>>12)!=lpg)
		{
			szrlc=(szrlc+3)&(~3);
			szrlc+=8;
			lpg=rva>>12;
		}
		szrlc+=2;
	}
	
#if 0
	for(i=0; i<nrlce; i++)
		for(j=i+1; j<nrlce; j++)
	{
		if(rlctab[j]<rlctab[i])
		{
			k=rlctab[i];
			rlctab[i]=rlctab[j];
			rlctab[j]=k;
		}
	}
#endif

	bgbcc_shxc_qrsort(rlctab, nrlce, 0);

	BGBCC_SHX_SetSectionName(sctx, ".reloc");
	BGBCC_SHX_EmitBAlign(sctx, 4);
	ofsrlc=BGBCC_SHX_EmitGetOffs(sctx);
	BGBCC_SHX_EmitRawBytes(sctx, NULL, szrlc+256);

	BGBCC_SHX_EmitGetStrtabLabel(sctx, "");

	ne_shdr=0;
	for(i=0; i<sctx->nsec; i++)
	{
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		if(j>0)
			ne_shdr++;

		s0=sctx->sec_name[i];
		if(s0)
		{
			BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
		}else
		{
			switch(i)
			{
			case BGBCC_SH_CSEG_TEXT: s0=".text"; break;
			case BGBCC_SH_CSEG_STRTAB: s0=".strtab"; break;
			case BGBCC_SH_CSEG_GOT: s0=".got"; break;
			case BGBCC_SH_CSEG_DATA: s0=".data"; break;
			case BGBCC_SH_CSEG_BSS: s0=".bss"; break;
			default: s0=".unknown"; break;
			}

			BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
			sctx->sec_name[i]=s0;
		}
	}

	en=(sctx->is_le==0);
//	ne_phdr=1;
//	ne_shdr=sctx->nsec;
	
//	of_phdr=64;
	
//	of_shdr=of_phdr+(ne_phdr*32);
//	of_shdr=(of_shdr+15)&(~15);

	of_shdr=0x0138;
//	of_shdr=0x00E8;

	ofs_sdat=of_shdr+(ne_shdr*40);
	ofs_sdat=(ofs_sdat+63)&(~63);

//	k=ofs_sdat;
	k=0x400;
	for(i=0; i<sctx->nsec; i++)
	{
//		if(sctx->sec_lsz[i]<=0)
//			continue;

		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=img_base+k;
		sctx->sec_lsz[i]=j;
		memcpy(obuf+k, sctx->sec_buf[i], j);
		k+=j;
		k=(k+63)&(~63);

		s0=sctx->sec_name[i];
		printf("%d: %s %08X..%08X %d\n", i, s0,
			sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
			sctx->sec_lsz[i]);

	}
	ofs_iend=k;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	sctx->sec_rva[i]=k;
	sctx->sec_lva[i]=0x0C000000+k;
	sctx->sec_lsz[i]=j;
	k+=j;
	k=(k+63)&(~63);

	s0=sctx->sec_name[i];
	printf("%d: %s %08X..%08X %d\n", i, s0,
		sctx->sec_lva[i], sctx->sec_lva[i]+sctx->sec_lsz[i],
		sctx->sec_lsz[i]);

	ofs_mend=k;
	
	lb_strt=0;
//	lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "__start");
//	if(lb_strt<=0)
//		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "___start");
	if(lb_strt<=0)
		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "_start");
	if(lb_strt<=0)
		lb_strt=BGBCC_SHX_LookupNamedLabel(sctx, "__start");
	
	if(lb_strt>0)
		{ va_strt=BGBCC_SHXC_LookupLabelImgVA(ctx, sctx, lb_strt); }
	else
	{
		printf("BGBCC_SHXC_FlattenImageELF: No Entry Point Found\n");
		va_strt=0;
	}

	ofsrlc=sctx->sec_rva[BGBCC_SH_CSEG_RELOC]+ofsrlc;
	ctb=obuf+ofsrlc;
	szrlc=0; ct=ctb; ct0=ctb; lpg=-1;
//	for(i=0; i<sctx->nrlc; i++)
	for(i=0; i<nrlce; i++)
	{
//		if(	(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS16) &&
//			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS32) &&
//			(sctx->rlc_ty[i]!=BGBCC_SH_RLC_ABS64))
//				continue;
			
//		j=sctx->rlc_sec[i];
//		rva=sctx->sec_rva[j]+
//			sctx->rlc_ofs[i];

		k=rlctab[i];
		rva=k&0x0FFFFFFF;

		if((rva>>12)!=lpg)
		{
			j=ct-ctb;
			j=(j+3)&(~3);
			ct=ctb+j;
			j=ct-ct0;
			bgbcc_setu32en(ct0+4, en, j);

			ct0=ct;
			ct+=8;
			lpg=rva>>12;
			bgbcc_setu32en(ct0+0, en, (lpg<<12));
			bgbcc_setu32en(ct0+4, en, 0);
		}

#if 1
		switch(k>>28)
		{
		case 0:		j=0x3000|(rva&0xFFF);	break;
		case 1:		j=0xA000|(rva&0xFFF);	break;
		case 2:		j=0x2000|(rva&0xFFF);	break;
		default:	BGBCC_DBGBREAK			break;
		}
#endif

		bgbcc_setu16en(ct, en, j);
		ct+=2;
	}
	if(1)
	{
		j=ct-ctb;
		j=(j+3)&(~3);
		ct=ctb+j;
		j=ct-ct0;
		bgbcc_setu32en(ct0+4, en, j);
	}
	szrlc=ct-ctb;

	ct=obuf;
	
//	memset(ct, 0, 0x400);
	ct[0x00]='M';	ct[0x01]='Z';	ct[0x02]=0;		ct[0x03]=0;
	ct[0x3C]=0x40;	ct[0x3D]=0;		ct[0x3E]=0;		ct[0x3F]=0;
	ct[0x40]='P';	ct[0x41]='E';	ct[0x42]=0;		ct[0x43]=0;

	k=sctx->is_le?0x0080:0x8000;
	k|=0x0302;

//	bgbcc_setu16en(ct+0x44, en, 0x14C);		//mMachine
	bgbcc_setu16en(ct+0x44, en, 0x1A6);		//mMachine
	bgbcc_setu16en(ct+0x46, en, ne_shdr);	//mNumberOfSections
	bgbcc_setu32en(ct+0x48, en, 0);			//mTimeDateStamp
	bgbcc_setu32en(ct+0x4C, en, 0);			//mPointerToSymbolTable
	bgbcc_setu32en(ct+0x50, en, 0);			//mNumberOfSymbols
	bgbcc_setu16en(ct+0x54, en, 224);		//mSizeOfOptionalHeader
//	bgbcc_setu16en(ct+0x54, en, 144);		//mSizeOfOptionalHeader
	bgbcc_setu16en(ct+0x56, en, k);			//mCharacteristics

	bgbcc_setu16en(ct+0x58, en, 0x010B);	//mMagic
	bgbcc_setu16en(ct+0x5A, en, 0x0000);	//mLinkerVersion

	j=sctx->sec_lsz[BGBCC_SH_CSEG_TEXT];
	bgbcc_setu32en(ct+0x5C, en, j);			//mSizeOfCode
	j=sctx->sec_lsz[BGBCC_SH_CSEG_DATA];
	bgbcc_setu32en(ct+0x60, en, j);			//mSizeOfInitializedData
	j=sctx->sec_lsz[BGBCC_SH_CSEG_BSS];
	bgbcc_setu32en(ct+0x64, en, j);			//mSizeOfUninitializedData
	j=va_strt-img_base;
	bgbcc_setu32en(ct+0x68, en, j);			//rvaAddressOfEntryPoint
	j=sctx->sec_rva[BGBCC_SH_CSEG_TEXT];
	bgbcc_setu32en(ct+0x6C, en, j);			//mBaseOfCode
	j=sctx->sec_rva[BGBCC_SH_CSEG_DATA];
	bgbcc_setu32en(ct+0x70, en, j);			//mBaseOfData
	bgbcc_setu32en(ct+0x74, en, img_base);	//mImageBase
	bgbcc_setu32en(ct+0x78, en, 64);		//mSectionAlignment
	bgbcc_setu32en(ct+0x7C, en, 64);		//mFileAlignment

	bgbcc_setu32en(ct+0x90, en, ofs_mend);	//mSizeOfImage
	bgbcc_setu32en(ct+0x94, en, ofs_sdat);	//mSizeOfHeaders

	bgbcc_setu32en(ct+0xA0, en, 0x100000);	//mSizeOfHeapReserve
	bgbcc_setu32en(ct+0xA4, en, 0x10000);	//mSizeOfHeapCommit
	bgbcc_setu32en(ct+0xA8, en, 0);			//mSizeOfHeapReserve
	bgbcc_setu32en(ct+0xAC, en, 0);			//mSizeOfHeapCommit
	bgbcc_setu32en(ct+0xB0, en, 0);			//mLoaderFlags
	bgbcc_setu32en(ct+0xB4, en, 6);			//mNumberOfRvaAndSizes

	bgbcc_setu32en(ct+0xE0, en, ofsrlc);	//rvaBaseRelocTable
	bgbcc_setu32en(ct+0xE4, en, szrlc);		//szBaseRelocTable


	ct=obuf+of_shdr;
	for(i=0; i<sctx->nsec; i++)
	{
		if(sctx->sec_lsz[i]<=0)
			continue;

		s0=sctx->sec_name[i];
		if(s0)
		{
			nm=BGBCC_SHX_EmitGetStrtabSecOfs(sctx, s0);
			nm+=sctx->sec_rva[BGBCC_SH_CSEG_STRTAB];
		}

		fl=3;
		if(i==BGBCC_SH_CSEG_TEXT)fl|=4;

		lva=sctx->sec_lva[i];
		rva=sctx->sec_rva[i];
		lsz=sctx->sec_lsz[i];

		if(strlen(s0)<=8)
			{ strncpy(ct+0, s0, 8); }
		else
			{ bgbcc_setu32en(ct+0x04, en, nm); }
		bgbcc_setu32en(ct+0x08, en, lsz);	//mVirtualSize
		bgbcc_setu32en(ct+0x0C, en, lva);	//mVirtualAddress
		k=(i==BGBCC_SH_CSEG_BSS)?0:lsz;
		bgbcc_setu32en(ct+0x10, en, k);		//mSizeOfRawData
		bgbcc_setu32en(ct+0x14, en, rva);	//mPointerToRawData
		bgbcc_setu32en(ct+0x18, en, 0);		//mPointerToRealocations
		bgbcc_setu32en(ct+0x1C, en, 0);		//mPointerToLinenumbers
		bgbcc_setu16en(ct+0x20, en, 0);		//mNumberOfRealocations
		bgbcc_setu16en(ct+0x22, en, 0);		//mNumberOfLinenumbers

		k=BGBCC_SHXC_CoffSectionFlags(ctx, sctx, s0);
		bgbcc_setu32en(ct+0x24, en, k);		//mCharacteristics
		ct+=40;
	}

//	sn_strs=BGBCC_SH_CSEG_STRTAB+1;

	imty=2;
//	if(sctx->is_pic)
//		imty=3;

	BGBCC_SHXC_ApplyImageRelocs(ctx, sctx, obuf);

#if 1
	mapfd=fopen("aout.map", "wt");
	for(i=0; i<sctx->nlbln; i++)
	{
//		if(!strcmp(ctx->lbln_name[i], name))
//			return(ctx->lbln_id[i]);

		lva=BGBCC_SHXC_LookupLabelImgVA(ctx, sctx, sctx->lbln_id[i]);
		fprintf(mapfd, "%08X T %s\n", lva, sctx->lbln_name[i]);
	}
	fclose(mapfd);
#endif

	*rosz=ofs_iend;

	return(0);
}

ccxl_status BGBCC_SHXC_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_SHX_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj;
	BGBCC_CCXL_LiteralInfo *litobj;
	int l0;
	u32 addr;
	int i, j, k;

	sctx=ctx->uctx;

	if(!sctx->cgen_log)
		sctx->cgen_log=fopen("bgbcc_shxlog.txt", "wt");

	BGBCC_SHX_SetSectionName(sctx, ".bss");
	BGBCC_SHX_EmitNamedLabel(sctx, "__bss_start");
	
	sctx->lbl_got=BGBCC_SHX_GenLabel(sctx);

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 0);
	}

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_FixupObjSize(ctx, litobj, 1);
	}

	for(i=0; i<ctx->n_literals; i++)
	{
		litobj=ctx->literals[i];
		BGBCC_CCXL_SanityObjSize(ctx, litobj, 1);
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;
		
		if(obj->flagsint&BGBCC_TYFL_DLLEXPORT)
		{
			BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
			continue;
		}

		if(obj->name)
		{
			if(!strncmp(obj->name, "__", 2))
			{
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}

			if(!strcmp(obj->name, "main"))
			{
				BGBCC_CCXL_GlobalMarkReachable(ctx, obj);
				continue;
			}
		}
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if(obj->regtype==CCXL_LITID_FUNCTION)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			if(!(obj->regflags&BGBCC_REGFL_ACCESSED))
				{ obj->regflags|=BGBCC_REGFL_CULL; }
			continue;
		}
	}

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		obj=ctx->reg_globals[i];
		if(!obj)
			continue;

		if((obj->regtype==CCXL_LITID_FUNCTION) && (obj->vtr))
		{
			BGBCC_SHXC_BuildFunction(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_SHXC_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_STATICVAR)
		{
			BGBCC_SHXC_BuildGlobal(ctx, obj);
			continue;
		}

		if(obj->regtype==CCXL_LITID_ASMBLOB)
		{
			BGBCC_SHXC_BuildAsmBlob(ctx, obj);
			continue;
		}
	}

	if(sctx->lvt16_n_idx>0)
	{
		BGBCC_SHX_SetSectionName(sctx, ".data");
		BGBCC_SHX_EmitBAlign(sctx, 16);
		for(i=0; i<sctx->lvt16_n_idx; i++)
		{
			BGBCC_SHX_EmitLabel(sctx, sctx->lvt16_lbl[i]);
			if(sctx->is_le)
			{
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
			}else
			{
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+1]);
				BGBCC_SHX_EmitQWord(sctx, sctx->lvt16_val[i*2+0]);
			}
		}
	}

	if(sctx->got_n_gblidx>0)
	{
		BGBCC_SHX_SetSectionName(sctx, ".got");
		BGBCC_SHX_EmitBAlign(sctx, 4);
		BGBCC_SHX_EmitLabel(sctx, sctx->lbl_got);
		
		addr=0;
		for(i=0; i<sctx->got_n_gblidx; i++)
		{
			j=sctx->got_gblidx[i];
			l0=ctx->reg_globals[j]->fxoffs;
			BGBCC_SHX_EmitRelocAbs32(sctx, l0);
			BGBCC_SHX_EmitDWord(sctx, 0);
		}
	}

	BGBCC_SHX_SetSectionName(sctx, ".bss");
	BGBCC_SHX_EmitNamedLabel(sctx, "_end");

	if((imgfmt==BGBCC_IMGFMT_EXE) ||
		(imgfmt==BGBCC_IMGFMT_DLL))
	{
		BGBCC_SHXC_FlattenImagePECOFF(ctx, obuf, rosz, imgfmt);
		return(1);
	}

	if((imgfmt==BGBCC_IMGFMT_ELXE) ||
		(imgfmt==BGBCC_IMGFMT_ELSO))
	{
		BGBCC_SHXC_FlattenImageELF(ctx, obuf, rosz, imgfmt);
		return(1);
	}
}
