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

bool BGBCC_SHXC_TypeIntRegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeSmallIntP(ctx, ty))
		return(true);
	if(BGBCC_CCXL_TypePointerP(ctx, ty))
		return(true);
	return(false);
}

bool BGBCC_SHXC_TypeInt2RegP(BGBCC_TransState *ctx, ccxl_type ty)
{
	if(BGBCC_CCXL_TypeSmallLongP(ctx, ty))
		return(true);
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
	if(BGBCC_SHXC_TypeIntRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR);
	if(BGBCC_SHXC_TypeInt2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_GR2);
	if(BGBCC_SHXC_TypeFloatRegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR);
	if(BGBCC_SHXC_TypeFloat2RegP(ctx, ty))
		return(BGBCC_SH_REGCLS_FR2);
	return(BGBCC_SH_REGCLS_NONE);
}

int BGBCC_SHXC_SetupFrameLayout(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	ccxl_type tty;
	int ni, nf;
	int i, j, k, ka, kf;
	
	ctx->cur_func=obj;
	sctx->use_bp=0;
	sctx->use_fpr=0;
	sctx->use_dbr=0;
	sctx->is_vararg=0;
	sctx->frm_offs_retstr=0;
	
	ni=0; nf=0;
	k=0; ka=0; kf=0;
	k-=2*4;		//saved PR, R14
	k-=6*4;		//saved R8/9/10/11/13, R2

	for(i=0; i<obj->n_locals; i++)
	{
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->locals[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_FR:	case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;	break;
		default:		break;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, obj->locals[i]->type) ||
			BGBCC_CCXL_TypeValueObjectP(ctx, obj->locals[i]->type))
		{
//			k-=4; obj->locals[i]->fxoffs=k;
			obj->locals[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->locals[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}
	}

	for(i=0; i<obj->n_regs; i++)
	{
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->regs[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_FR:	case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;	break;
		default:		break;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, obj->regs[i]->type) ||
			BGBCC_CCXL_TypeValueObjectP(ctx, obj->regs[i]->type))
		{
//			k-=4; obj->regs[i]->fxoffs=k;
			obj->regs[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}
	}

	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ sctx->is_vararg=1; }

		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_FR:
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;	break;
		default:		break;
		}

		if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->args[i]->type))
		{
//			k-=4; obj->args[i]->fxoffs=k;
			obj->args[i]->fxmoffs=kf;
			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->args[i]->type);
			j=(j+3)&(~3);
			kf+=j;
		}
	}

	if(sctx->use_fpr)
		{ k-=4*4; }		//saved FR12/13/14/15

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);
	if(BGBCC_CCXL_TypeValueObjectP(ctx, tty))
	{
		k-=4;
		sctx->frm_offs_retstr=k;
	}

	k-=kf;
	sctx->frm_offs_fix=k;

	for(i=0; i<obj->n_args; i++)
	{
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
			if(ni<4)	{ k-=4; obj->args[i]->fxoffs=k; ni++; }
			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			break;
		case BGBCC_SH_REGCLS_GR2:
			if(ni<3)	{ k-=8; obj->args[i]->fxoffs=k; ni+=2; }
			else		{ obj->args[i]->fxoffs=ka; ka+=8; }
			break;
		case BGBCC_SH_REGCLS_FR:
			if(nf<8)	{ k-=4; obj->args[i]->fxoffs=k; nf++; }
			else		{ obj->args[i]->fxoffs=ka; ka+=4; }
			sctx->use_fpr=1;
			break;
		case BGBCC_SH_REGCLS_FR2:
			nf+=nf&1;
			if(nf<7)	{ k-=8; obj->args[i]->fxoffs=k; nf+=2; }
			else		{ obj->args[i]->fxoffs=ka; ka+=8; }
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			break;
		default:
			break;
		}

		if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->args[i]->type))
		{
			k-=4; obj->args[i]->fxoffs=k;
//			obj->args[i]->fxmoffs=kf;
//			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->args[i]->type);
//			j=(j+3)&(~3);
//			kf+=j;
		}
	}

	for(i=0; i<obj->n_locals; i++)
	{
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->locals[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
			k-=4; obj->locals[i]->fxoffs=k;	break;
		case BGBCC_SH_REGCLS_GR2:
			k-=8; obj->locals[i]->fxoffs=k;	break;

		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k-=4; obj->locals[i]->fxoffs=k;	break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k-=8; obj->locals[i]->fxoffs=k;	break;

		default:	break;
		}
		
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->locals[i]->type) ||
			BGBCC_CCXL_TypeValueObjectP(ctx, obj->locals[i]->type))
		{
			k-=4; obj->locals[i]->fxoffs=k;
//			obj->locals[i]->fxmoffs=kf;
//			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->locals[i]->type);
//			j=(j+3)&(~3);
//			kf+=j;
		}
	}
	sctx->frm_offs_lcl=k;

	for(i=0; i<obj->n_regs; i++)
	{
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->regs[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
			k-=4; obj->regs[i]->fxoffs=k;		break;
		case BGBCC_SH_REGCLS_GR2:
			k-=8; obj->regs[i]->fxoffs=k;		break;

		case BGBCC_SH_REGCLS_FR:
			sctx->use_fpr=1;
			k-=4; obj->regs[i]->fxoffs=k;		break;
		case BGBCC_SH_REGCLS_FR2:
			sctx->use_fpr=1;
			sctx->use_dbr=1;
			k-=8; obj->regs[i]->fxoffs=k;		break;
		default:	break;
		}

		if(BGBCC_CCXL_TypeArrayP(ctx, obj->regs[i]->type) ||
			BGBCC_CCXL_TypeValueObjectP(ctx, obj->regs[i]->type))
		{
			k-=4; obj->regs[i]->fxoffs=k;
//			obj->regs[i]->fxmoffs=kf;
//			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->regs[i]->type);
//			j=(j+3)&(~3);
//			kf+=j;
		}
	}
	sctx->frm_offs_tmp=k;
	
	k-=obj->n_cargs*4;
//	k-=obj->n_cargs*8;
//	k&=~15;
//	k&=~3;

	if((-k)>=60)
	{
		j=-1;
	}
	
//	if((-k)>=960)
	if((-k)>=1012)
		{ k&=~255; sctx->use_bp=1; }
	
	sctx->frm_size=-k;
	
	return(0);
}

int BGBCC_SHXC_EmitFrameProlog(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	int p0, vaix;
	int ni, nf;
	int i, j, k;

	sctx->cur_fpscr=0;
	sctx->dfl_fpscr=0;

	sctx->reg_save=0;
	sctx->freg_save=0;
	
	sctx->sreg_held=0x00F4;
	sctx->sfreg_held=0x0FF0;

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	if(sctx->is_vararg)
	{
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_SP, BGBCC_SH_REG_R1);
	}

	BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_STSL,
		BGBCC_SH_REG_PR, BGBCC_SH_REG_SP);
//	BGBCC_SHX_EmitOpRegStDecReg(sctx, BGBCC_SH_NMID_MOVL,
//		BGBCC_SH_REG_BP, BGBCC_SH_REG_SP);

	p0=BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
		BGBCC_SH_REG_SP, -(sctx->frm_size-4));
	if(p0<=0)
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, sctx->frm_size-4);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_SUB,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
	}

//	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_STS,
//		BGBCC_SH_REG_PR, BGBCC_SH_REG_R0);
//	BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, - 4, BGBCC_SH_REG_R0);
//	BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, - 8, BGBCC_SH_REG_R14);

	if(sctx->is_pic)
	{
//		BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, -16, BGBCC_SH_REG_R12);
		BGBCC_SHXC_EmitSaveFrameReg(ctx, sctx, BGBCC_SH_REG_R12);

		BGBCC_SHX_EmitWord(sctx, 0xC700);
		BGBCC_SHX_EmitLoadRegLabelRel(sctx, BGBCC_SH_REG_R12, sctx->lbl_got, 0);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R12);
	}

	k=sctx->frm_offs_retstr;
	if(k)
	{
		BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R2);
	}

	ni=0; nf=0; vaix=-1;
	for(i=0; i<obj->n_args; i++)
	{
		if(BGBCC_CCXL_TypeVarArgsP(ctx, obj->args[i]->type))
			{ vaix=i; continue; }
	
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->args[i]->type);
		switch(j)
		{
		case BGBCC_SH_REGCLS_GR:
			if(ni>=4)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R4+ni);
			ni++;
			break;
		case BGBCC_SH_REGCLS_GR2:
			if(ni>=3)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx,
				k, BGBCC_SH_REG_R4+ni);
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_R4+ni+1);
			ni+=2;
			break;
		case BGBCC_SH_REGCLS_FR:
			if(nf>=8)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_FR4+nf);
			nf++;
			break;
		case BGBCC_SH_REGCLS_FR2:
			nf+=nf&1;
			if(nf>=7)
				break;
			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx,
				k+4, BGBCC_SH_REG_FR4+nf+0);
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx,
				k+0, BGBCC_SH_REG_FR4+nf+1);
			nf+=2;
			break;
		default:
			break;
		}

		if(BGBCC_CCXL_TypeValueObjectP(ctx, obj->args[i]->type))
		{
			if(ni>=4)
				continue;
			k=obj->args[i]->fxoffs;
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R4+ni);
			ni++;
			continue;

//			obj->args[i]->fxmoffs=kf;
//			j=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->args[i]->type);
//			j=(j+3)&(~3);
//			kf+=j;
		}
	}
	if(vaix<0)
		vaix=obj->n_args;

	for(i=0; i<obj->n_locals; i++)
	{
		if(BGBCC_CCXL_TypeArrayP(ctx, obj->locals[i]->type) ||
			BGBCC_CCXL_TypeValueObjectP(ctx, obj->locals[i]->type))
		{
			j=obj->locals[i]->fxmoffs+(sctx->frm_offs_fix);
			k=obj->locals[i]->fxoffs;
			BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, j, BGBCC_SH_REG_R3);
			BGBCC_SHXC_EmitStoreFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R3);
		}
		
		if(BGBCC_CCXL_TypeVaListP(ctx, obj->locals[i]->type) && (vaix>0))
		{
//			j=obj->locals[i]->fxmoffs+(sctx->frm_offs_fix);
//			BGBCC_SHXC_EmitLdaFrameOfsReg(ctx, sctx, j, BGBCC_SH_REG_R3);
//			BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R3);

//			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
//				BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 0);

			for(j=0; j<4; j++)
			{
				BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
					BGBCC_SH_REG_R4+j, BGBCC_SH_REG_R3, j*4);
			}

			for(j=0; j<8; j++)
			{
				BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_FMOVS,
					BGBCC_SH_REG_FR4+j, BGBCC_SH_REG_R3, 16+j*4);
			}

			BGBCC_SHX_EmitLoadRegImm(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, ni*4);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 48);

			BGBCC_SHX_EmitLoadRegImm(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, nf*4);
			BGBCC_SHX_EmitOpRegStRegDisp(sctx,
				BGBCC_SH_NMID_MOVL, BGBCC_SH_REG_R0, BGBCC_SH_REG_R3, 52);

			BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
				BGBCC_SH_REG_R1, BGBCC_SH_REG_R3, 56);
		}
	}

	sctx->regalc_save=0;
	sctx->regalc_live=0;
	sctx->regalc_dirty=0;

	sctx->fregalc_save=0;
	sctx->fregalc_live=0;
	sctx->fregalc_dirty=0;

	sctx->sreg_held=0x0000;
	sctx->sfreg_held=0x0000;
	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	sctx->lbl_ret=BGBCC_SHX_GenLabelTemp(sctx);
	
	return(0);
}

int BGBCC_SHXC_EmitFrameEpilog(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	ccxl_type tty;
	int p0, tsz;
	int i, j, k;

//	BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, - 4, BGBCC_SH_REG_R3);
//	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
//		BGBCC_SH_REG_R3, BGBCC_SH_REG_PR);

	if(sctx->lbl_ret)
	{
		BGBCC_SHX_EmitLabel(sctx, sctx->lbl_ret);
	}

	sctx->sreg_held=0x0003;
	sctx->sfreg_held=0x0003;

	tty=BGBCC_CCXL_GetTypeReturnType(ctx, obj->type);

	if(BGBCC_CCXL_TypeVoidP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeSmallIntP(ctx, tty) ||
		BGBCC_CCXL_TypePointerP(ctx, tty) ||
		BGBCC_CCXL_TypeValueObjectP(ctx, tty))
		{ sctx->sreg_held=0x0001; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeSgLongP(ctx, tty))
		{ sctx->sreg_held=0x0003; sctx->sfreg_held=0x0000; }
	if(BGBCC_CCXL_TypeFloatP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0001; }
	if(BGBCC_CCXL_TypeDoubleP(ctx, tty))
		{ sctx->sreg_held=0x0000; sctx->sfreg_held=0x0003; }

	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

	k=sctx->frm_offs_retstr;
	if(k)
	{
		tsz=BGBCC_CCXL_TypeGetLogicalSize(ctx, tty);

		BGBCC_SHXC_ScratchSafeStompReg(ctx, sctx, BGBCC_SH_REG_R2);
		BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, BGBCC_SH_REG_R2);

		BGBCC_SHXC_EmitValueCopyRegRegSz(ctx, sctx,
			BGBCC_SH_REG_R2, BGBCC_SH_REG_R0, tsz, 4);

		BGBCC_SHXC_ScratchReleaseReg(ctx, sctx, BGBCC_SH_REG_R2);
	}

	BGBCC_SHXC_ResetFpscrDefaults(ctx, sctx);
	
	for(i=0; i<16; i++)
	{
		BGBCC_SHXC_EmitReloadSavedFrameReg(ctx, sctx,
			BGBCC_SH_REG_R0+i);
	}

	for(i=0; i<16; i++)
	{
		BGBCC_SHXC_EmitReloadSavedFrameReg(ctx, sctx,
			BGBCC_SH_REG_FR0+i);
	}

	p0=BGBCC_SHX_EmitOpRegImm(sctx, BGBCC_SH_NMID_ADD,
		BGBCC_SH_REG_SP, sctx->frm_size-4);
	if(p0<=0)
	{
		BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
			BGBCC_SH_REG_R0, sctx->frm_size-4);
		BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
	}

//	BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_MOVL,
//		BGBCC_SH_REG_SP, BGBCC_SH_REG_BP);
	BGBCC_SHX_EmitOpLdIncRegReg(sctx, BGBCC_SH_NMID_LDSL,
		BGBCC_SH_REG_SP, BGBCC_SH_REG_PR);

	BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_RTS);
	BGBCC_SHX_EmitOpNone(sctx, BGBCC_SH_NMID_NOP);
	BGBCC_SHX_EmitFlushIndexImm16(sctx);
	BGBCC_SHX_EmitFlushIndexImm32(sctx);
	
	return(0);
}

int BGBCC_SHXC_EmitVaArg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_type type,
	ccxl_register dreg,
	ccxl_register sreg)
{
	int i, j, k;

	sctx->sreg_live|=0x0010;
	sctx->sfreg_live|=0x0000;

	BGBCC_SHXC_EmitLoadFrameVRegReg(ctx, sctx, sreg, BGBCC_SH_REG_R4);
//	BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_i");
//	BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R0);

	j=BGBCC_SHXC_TypeGetRegClassP(ctx, type);
	switch(j)
	{
	case BGBCC_SH_REGCLS_GR:
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_i");
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R3);
		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_R3);
		break;
	case BGBCC_SH_REGCLS_GR2:
		BGBCC_SHXC_EmitCallName(ctx, sctx, "__va_arg_l");
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R6);
		BGBCC_SHXC_EmitMovRegReg(ctx, sctx,
			BGBCC_SH_REG_R0, BGBCC_SH_REG_R7);
		BGBCC_SHXC_EmitStoreFrameVRegReg(ctx, sctx, dreg, BGBCC_SH_REG_LR6);
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


ccxl_status BGBCC_SHXC_CompileVirtOp(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	sctx->sreg_live=sctx->sreg_held;
	sctx->sfreg_live=sctx->sfreg_held;

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
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHX_EmitLabel(sctx,
			op->imm.si);
		break;
	case CCXL_VOP_JMP:
//		BGBCC_SHX_EmitOpLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_ResetFpscrLocal(ctx, sctx);
		BGBCC_SHX_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRA,
			op->imm.si);
		BGBCC_SHX_EmitOpNone(sctx,
			BGBCC_SH_NMID_NOP);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_MOV:
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;
	case CCXL_VOP_JCMP_ZERO:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitJCmpVRegZero(ctx, sctx, op->type,
			op->srca, op->opr, op->imm.si);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_JCMP:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr, op->imm.si);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CALL:
		sctx->sreg_live|=0x00F2;
		sctx->sfreg_live|=0x0FF2;
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitCallVReg(ctx, sctx,
			op->type, op->srca,
			op->imm.call.na, op->imm.call.args);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_CSRV:
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		BGBCC_SHXC_EmitCsrvVReg(ctx, sctx,
			op->type, op->dst);
		break;
	case CCXL_VOP_RETDFL:
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_RETV:
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
		break;
	case CCXL_VOP_RET:
		BGBCC_SHXC_EmitSyncRegisters(ctx, sctx);
		BGBCC_SHXC_EmitReturnVReg(ctx, sctx,
			op->type, op->srca);
		BGBCC_SHXC_EmitLabelFlushRegisters(ctx, sctx);
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
		BGBCC_SHXC_EmitVaStart(ctx, sctx, op->dst, op->srca);
		break;
	case CCXL_VOP_VA_END:
		BGBCC_SHXC_EmitVaEnd(ctx, sctx, op->srca);
		break;
	case CCXL_VOP_VA_ARG:
		BGBCC_SHXC_EmitVaArg(ctx, sctx, op->type, op->dst, op->srca);
		break;

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
	return(1);
}

ccxl_status BGBCC_SHXC_CompileVirtTr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_RegisterInfo *obj, BGBCC_CCXL_VirtTr *tr)
{
	int i, j, k;
	
	for(i=0; i<tr->n_ops; i++)
	{
		BGBCC_SHXC_CompileVirtOp(ctx, sctx, obj, obj->vop[tr->b_ops+i]);
	}

	return(0);
}

ccxl_status BGBCC_SHXC_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	return(0);
}

ccxl_status BGBCC_SHXC_BuildFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	int l0;
	int i, j, k;

	sctx=ctx->uctx;
	
	l0=obj->fxoffs;
	if(l0<=0)
	{
//		l0=BGBCC_SHX_GenLabel(sctx);
		l0=BGBCC_SHX_GetNamedLabel(sctx, obj->name);
		obj->fxoffs=l0;
	}
	
	sctx->need_farjmp=0;
	sctx->need_n12jmp=0;
//	if((obj->srctok*4)>=128)
	if((obj->n_vop*6)>=128)
		sctx->need_n12jmp=1;
//	if((obj->srctok*4)>=2048)
	if((obj->n_vop*6)>=2048)
		sctx->need_farjmp=1;
	
	BGBCC_SHXC_SetupFrameLayout(ctx, sctx, obj);

	BGBCC_SHX_SetSectionName(sctx, ".text");
//	BGBCC_SHX_EmitBAlign(sctx, 4);
	BGBCC_SHX_EmitBAlign(sctx, 16);

	BGBCC_SHX_EmitLabel(sctx, l0);
	BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj);
	
	for(i=0; i<obj->n_vtr; i++)
	{
		BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->vtr[i]);
	}
	
	BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);

	return(0);
}

ccxl_status BGBCC_SHXC_BuildGlobal(BGBCC_TransState *ctx,
	BGBCC_CCXL_RegisterInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	char *s0;
	int l0, sz;
	s64 li;
	int i, j, k;

	sctx=ctx->uctx;
	
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
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->type);

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
	
	if(BGBCC_CCXL_IsRegImmIntP(ctx, obj->value))
	{
		if(BGBCC_CCXL_TypeSmallIntP(ctx, obj->type) ||
			BGBCC_CCXL_TypePointerP(ctx, obj->type))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_SHX_EmitDWord(sctx, i);
			return(1);
		}

		if(BGBCC_CCXL_TypeSgLongP(ctx, obj->type))
		{
			i=BGBCC_CCXL_GetRegImmIntValue(ctx, obj->value);
			BGBCC_SHX_EmitQWord(sctx, i);
			return(1);
		}
	}

	if(BGBCC_CCXL_IsRegImmLongP(ctx, obj->value))
	{
		if(BGBCC_CCXL_TypeSgLongP(ctx, obj->type))
		{
			li=BGBCC_CCXL_GetRegImmLongValue(ctx, obj->value);
			BGBCC_SHX_EmitQWord(sctx, li);
			return(1);
		}
	}

	if(BGBCC_CCXL_IsRegImmStringP(ctx, obj->value))
	{
		if(BGBCC_CCXL_TypePointerP(ctx, obj->type))
		{
			s0=BGBCC_CCXL_GetRegImmStringValue(ctx, obj->value);
			k=BGBCC_SHX_EmitGetStrtabLabel(sctx, s0);
			BGBCC_SHX_EmitRelocTy(sctx, k, BGBCC_SH_RLC_ABS32);
			BGBCC_SHX_EmitDWord(sctx, 0);
			return(1);
		}
	}
	
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
	int i, j, k;
	
	for(i=0; i<sctx->nlbl; i++)
	{
		if(sctx->lbl_id[i]==lblid)
			return(i);
	}
	return(-1);
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

ccxl_status BGBCC_SHXC_ApplyImageRelocs(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	byte *imgbase)
{
	byte *ctl, *ctr;
	char *s0;
	int en;
	s32 b, d, b1, d1;
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
				printf("BGBCC_SHXC_ApplyImageRelocs: Missing Label %s\n", s0);
			}else
			{
				printf("BGBCC_SHXC_ApplyImageRelocs: Missing Label %08X\n", k);
			}
//			__debugbreak();
		}
		ctl=imgbase+sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
		ctr=imgbase+sctx->sec_rva[sctx->rlc_sec[i]]+sctx->rlc_ofs[i];
		
		if((ctl<=imgbase) || (ctl>(imgbase+0x1000000)))
			__debugbreak();
		if((ctr<=imgbase) || (ctr>(imgbase+0x1000000)))
			__debugbreak();

		
		d=ctl-ctr;
		switch(sctx->rlc_ty[i])
		{
		case BGBCC_SH_RLC_REL8:
			b=bgbcc_gets8en(ctr, en);
			bgbcc_sets8en(ctr, en, b+d);
			break;
		case BGBCC_SH_RLC_REL16:
			b=bgbcc_gets16en(ctr, en);
			bgbcc_sets16en(ctr, en, b+d);
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
	}
	ofs_iend=k;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	sctx->sec_rva[i]=k;
	sctx->sec_lva[i]=0x0C000000+k;
	sctx->sec_lsz[i]=j;
	k+=j;
	k=(k+63)&(~63);

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

ccxl_status BGBCC_SHXC_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_SHX_Context *sctx;
	BGBCC_CCXL_RegisterInfo *obj;
	int l0;
	u32 addr;
	int i, j, k;

	sctx=ctx->uctx;

	BGBCC_SHX_SetSectionName(sctx, ".bss");
	BGBCC_SHX_EmitNamedLabel(sctx, "__bss_start");
	
	sctx->lbl_got=BGBCC_SHX_GenLabel(sctx);

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

	BGBCC_SHXC_FlattenImageELF(ctx, obuf, rosz, imgfmt);
	return(1);
}
