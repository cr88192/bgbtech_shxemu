BGBCC_CCXL_BackendFuncs_vt bgbgc_shx_vt;
static int bgbgc_shx_init=0;

	
int BGBCC_SHX_InitIface()
{
	if(bgbgc_shx_init)
		return(0);
	bgbgc_shx_init=1;

	bgbgc_shx_vt.tgtname="SHx";
	
	bgbgc_shx_vt.SetupContextForArch=	BGBCC_SHX_SetupContextForArch;
	bgbgc_shx_vt.FlattenImage=			BGBCC_SHX_FlattenImage;
	bgbgc_shx_vt.EndFunction=			BGBCC_SHX_EndFunction;

	BGBCC_CCXL_RegisterBackend(&bgbgc_shx_vt);

	return(1);
}


ccxl_status BGBCC_SHX_SetupContextForArch(BGBCC_TransState *ctx)
{
	BGBCC_SHX_Context *shctx;

	if(ctx->arch!=BGBCC_ARCH_SH)
		return(CCXL_STATUS_ERR_UNSUPPORTED);

	shctx=BGBCC_SHX_AllocContext();

	ctx->arch_sizeof_long=4;
	ctx->arch_sizeof_ptr=4;
	ctx->uctx=shctx;

	shctx->is_le=0;

	switch(ctx->sub_arch)
	{
	case BGBCC_ARCH_SH_SH2:
	case BGBCC_ARCH_SH_SH2B:
	case BGBCC_ARCH_SH_SH4B:
		shctx->is_le=0;
		break;
	case BGBCC_ARCH_SH_SH4:
	case BGBCC_ARCH_SH_SH2L:
	case BGBCC_ARCH_SH_SH4L:
		shctx->is_le=1;
		break;
	default:
		break;
	}

	return(0);
}

ccxl_status BGBCC_SHX_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
}

bool BGBCC_SHX_TypeIntRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);
	return(false);
}

bool BGBCC_SHX_TypeInt2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_SHX_TypeFloatRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeFloatP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

bool BGBCC_SHX_TypeFloat2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeDoubleP(ctx, ty))
		return(true);
//	if(BGBCC_CCXL_TypePointerP(ctx, ty))
//		return(true);
	return(false);
}

int BGBCC_SHX_TypeGetRegClassP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_SHX_TypeIntRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR);
	if(BGBCC_SHX_TypeInt2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR2);
	if(BGBCC_SHX_TypeFloatRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR);
	if(BGBCC_SHX_TypeFloat2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR2);
	return(BGBCC_SH_REGCLS_NONE);
}

int BGBCC_SHX_SetupFrameLayout(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	int ni, nf;
	int i, j, k, ka;
	
	sctx->use_bp=0;
	
	ni=0; nf=0;
	k=0; ka=0;
	k-=2*4;		//saved PR, R14
	k-=6*4;		//saved R8/9/10/11/13, R2
	for(i=0; i<obj->decl->n_args; i++)
	{
		j=BGBCC_SHX_TypeGetRegClassP(ctx, obj->decl->args[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
			if(ni<4)	{ k-=4; obj->decl->args[i]->fxoffs=k; ni++; }
			else		{ obj->decl->args[i]->fxoffs=ka; ka+=4; }
			break;
		case BGBCC_SH_REGCLS_GR2:
			if(ni<3)	{ k-=8; obj->decl->args[i]->fxoffs=k; ni++; }
			else		{ obj->decl->args[i]->fxoffs=ka; ka+=8; }
			break;
		case BGBCC_SH_REGCLS_FR:
			if(nf<8)	{ k-=4; obj->decl->args[i]->fxoffs=k; nf++; }
			else		{ obj->decl->args[i]->fxoffs=ka; ka+=4; }
			break;
		case BGBCC_SH_REGCLS_FR2:
			nf+=nf&1;
			if(nf<7)	{ k-=8; obj->decl->args[i]->fxoffs=k; nf++; }
			else		{ obj->decl->args[i]->fxoffs=ka; ka+=8; }
			break;
		default:
			break;
		}
	}

	for(i=0; i<obj->decl->n_locals; i++)
	{
		j=BGBCC_SHX_TypeGetRegClassP(ctx, obj->decl->locals[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:		case BGBCC_SH_REGCLS_FR:
			k-=4; obj->decl->locals[i]->fxoffs=k;	break;
		case BGBCC_SH_REGCLS_GR2:		case BGBCC_SH_REGCLS_FR2:
			k-=8; obj->decl->locals[i]->fxoffs=k;	break;
		default:	break;
		}
	}
	sctx->frm_offs_lcl=k;

	for(i=0; i<obj->decl->n_regs; i++)
	{
		j=BGBCC_SHX_TypeGetRegClassP(ctx, obj->decl->regs[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:	case BGBCC_SH_REGCLS_FR:
			k-=4; obj->decl->regs[i]->fxoffs=k;		break;
		case BGBCC_SH_REGCLS_GR2:	case BGBCC_SH_REGCLS_FR2:
			k-=8; obj->decl->regs[i]->fxoffs=k;		break;
		default:	break;
		}
	}
	sctx->frm_offs_tmp=k;
	
	k-=obj->decl->n_cargs*8;
	k&=~15;
	
//	if((-k)>=960)
	if((-k)>=1012)
		{ k&=~255; sctx->use_bp=1; }
	
	sctx->frm_size=-k;
	
	return(0);
}

int BGBCC_SHX_EmitFrameProlog(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	int p0;

	BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
		BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
	BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_MOVL,
		BGBCC_SH_REG_BP, BGBCC_SH_REG_SP);

	p0=BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
		BGBCC_SH_REG_SP, -(sctx->frm_size-8));
	if(p0<=0)
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, sctx->frm_size-8);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
	}

//	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
//		BGBCC_SH_REG_PR, BGBCC_SH_REG_R0);
//	BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, - 4, BGBCC_SH_REG_R0);
//	BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, - 8, BGBCC_SH_REG_R14);
	BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, -16, BGBCC_SH_REG_R12);

	sctx->regalc_save=0;
	sctx->regalc_live=0;
	
	return(0);
}

int BGBCC_SHX_EmitFrameEpilog(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	int p0;

//	BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, - 4, BGBCC_SH_REG_R3);
//	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
//		BGBCC_SH_REG_R3, BGBCC_SH_REG_PR);

	p0=BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
		BGBCC_SH_REG_SP, -(sctx->frm_size-8));
	if(p0<=0)
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, sctx->frm_size-8);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
	}

	BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_MOVL,
		BGBCC_SH_REG_SP, BGBCC_SH_REG_BP);
	BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_LDSL,
		BGBCC_SH_REG_SP, BGBCC_SH_REG_PR);

	BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
	
	return(0);
}

ccxl_status BGBCC_SHX_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	BGBCC_SHX_Context *sctx;

	sctx=ctx->uctx;
	
	BGBCC_SHX_SetupFrameLayout(ctx, sctx, obj);

	BGBCC_SHX_EmitFrameProlog(ctx, sctx, obj);
	
	
	
	BGBCC_SHX_EmitFrameEpilog(ctx, sctx, obj);
}
