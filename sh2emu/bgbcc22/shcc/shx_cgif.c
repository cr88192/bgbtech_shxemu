BGBCC_CCXL_BackendFuncs_vt bgbgc_shx_vt;
static int bgbgc_shx_init=0;

	
int BGBCC_SHX_InitIface()
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
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->decl->args[i]->type);
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
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->decl->locals[i]->type);
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
		j=BGBCC_SHXC_TypeGetRegClassP(ctx, obj->decl->regs[i]->type);
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

int BGBCC_SHXC_EmitFrameProlog(BGBCC_TransState *ctx,
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

	BGBCC_SHX_EmitWord(sctx, 0xC700);
	BGBCC_SHX_EmitLoadRegLabelRel(sctx, BGBCC_SH_REG_R12, sctx->lbl_got, 0);
	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_ADD,
		BGBCC_SH_REG_R0, BGBCC_SH_REG_R12);

	sctx->regalc_save=0;
	sctx->regalc_live=0;
	sctx->regalc_dirty=0;
	
	sctx->lbl_ret=BGBCC_SHX_GenLabelTemp(sctx);
	
	return(0);
}

int BGBCC_SHXC_EmitFrameEpilog(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	int p0;

//	BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, - 4, BGBCC_SH_REG_R3);
//	BGBCC_SHX_EmitOpRegReg(sctx, BGBCC_SH_NMID_LDS,
//		BGBCC_SH_REG_R3, BGBCC_SH_REG_PR);

	if(sctx->lbl_ret)
	{
		BGBCC_SHX_EmitLabel(sctx, sctx->lbl_ret);
	}

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

ccxl_status BGBCC_SHXC_CompileVirtOp(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_LiteralInfo *obj, BGBCC_CCXL_VirtOp *op)
{
	switch(op->opn)
	{
	case CCXL_VOP_NONE:
		break;
	case CCXL_VOP_DBGFN:
		break;
	case CCXL_VOP_DBGLN:
		break;
	case CCXL_VOP_LABEL:
		BGBCC_SHX_EmitLabel(sctx,
			op->imm.si);
		break;
	case CCXL_VOP_JMP:
//		BGBCC_SHX_EmitOpLabel(sctx, BGBCC_SH_NMID_BRA,
//			op->imm.si);
		BGBCC_SHX_EmitOpAutoLabel(sctx, BGBCC_SH_NMID_BRA,
			op->imm.si);
		break;
	case CCXL_VOP_MOV:
		BGBCC_SHXC_EmitMovVRegVReg(ctx, sctx, op->type,
			op->dst, op->srca);
		break;
	case CCXL_VOP_JCMP_ZERO:
		BGBCC_SHXC_EmitJCmpVRegZero(ctx, sctx, op->type,
			op->srca, op->opr, op->imm.si);
		break;
	case CCXL_VOP_JCMP:
		BGBCC_SHXC_EmitJCmpVRegVReg(ctx, sctx, op->type,
			op->srca, op->srcb, op->opr, op->imm.si);
		break;
	case CCXL_VOP_CALL:
		BGBCC_SHXC_EmitCallVReg(ctx, sctx,
			op->type, op->srca,
			op->imm.call.na, op->imm.call.args);
		break;
	case CCXL_VOP_CSRV:
		BGBCC_SHXC_EmitCsrvVReg(ctx, sctx,
			op->type, op->dst);
		break;
	case CCXL_VOP_RETDFL:
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		break;
	case CCXL_VOP_RETV:
		BGBCC_SHXC_EmitReturnVoid(ctx, sctx);
		break;
	case CCXL_VOP_RET:
		BGBCC_SHXC_EmitReturnVReg(ctx, sctx,
			op->type, op->srca);
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

	default:
		BGBCC_CCXL_StubError(ctx);
		break;
	}
	return(1);
}

ccxl_status BGBCC_SHXC_CompileVirtTr(BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	BGBCC_CCXL_LiteralInfo *obj, BGBCC_CCXL_VirtTr *tr)
{
	int i, j, k;
	
	for(i=0; i<tr->n_ops; i++)
	{
		BGBCC_SHXC_CompileVirtOp(ctx, sctx, obj, obj->decl->vop[tr->b_ops+i]);
	}

	return(0);
}

ccxl_status BGBCC_SHXC_EndFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	return(0);
}

ccxl_status BGBCC_SHXC_BuildFunction(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	int l0;
	int i, j, k;

	sctx=ctx->uctx;
	
	l0=BGBCC_SHX_GenLabel(sctx);
	obj->decl->fxoffs=l0;
	
	sctx->need_farjmp=0;
	sctx->need_n12jmp=0;
	if((obj->decl->srctok*4)>=128)
		sctx->need_n12jmp=1;
	if((obj->decl->srctok*4)>=2048)
		sctx->need_farjmp=1;
	
	BGBCC_SHXC_SetupFrameLayout(ctx, sctx, obj);

	BGBCC_SHX_SetSectionName(sctx, ".text");
	BGBCC_SHX_EmitBAlign(sctx, 4);

	BGBCC_SHX_EmitLabel(sctx, l0);
	BGBCC_SHXC_EmitFrameProlog(ctx, sctx, obj);
	
	for(i=0; i<obj->decl->n_vtr; i++)
	{
		BGBCC_SHXC_CompileVirtTr(ctx, sctx, obj, obj->decl->vtr[i]);
	}
	
	BGBCC_SHXC_EmitFrameEpilog(ctx, sctx, obj);

	return(0);
}

ccxl_status BGBCC_SHXC_BuildGlobal(BGBCC_TransState *ctx,
	BGBCC_CCXL_LiteralInfo *obj)
{
	BGBCC_SHX_Context *sctx;
	int l0, sz;
	int i, j, k;

	sctx=ctx->uctx;
	
	l0=BGBCC_SHX_GenLabel(sctx);
	obj->decl->fxoffs=l0;
	
	if(!obj->decl->value.val)
	{
		sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->decl->type);

		BGBCC_SHX_SetSectionName(sctx, ".bss");
		BGBCC_SHX_EmitBAlign(sctx, 4);
		BGBCC_SHX_EmitLabel(sctx, l0);
		BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
		return(1);
	}
	
	sz=BGBCC_CCXL_TypeGetLogicalSize(ctx, obj->decl->type);

	BGBCC_SHX_SetSectionName(sctx, ".data");
	BGBCC_SHX_EmitBAlign(sctx, 4);
	BGBCC_SHX_EmitLabel(sctx, l0);
//	BGBCC_SHX_EmitRawBytes(sctx, NULL, sz);
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

ccxl_status BGBCC_SHXC_ApplyImageRelocs(
	BGBCC_TransState *ctx, BGBCC_SHX_Context *sctx,
	byte *imgbase)
{
	byte *ctl, *ctr;
	int en;
	s32 b, d, b1, d1;
	int i, j, k;

	en=sctx->is_le;

	for(i=0; i<sctx->nrlc; i++)
	{
		j=BGBCC_SHXC_LookupLabelIndex(ctx, sctx, sctx->rlc_id[i]);
		if(j<0)
			{ __debugbreak(); }
		ctl=imgbase+sctx->sec_rva[sctx->lbl_sec[j]]+sctx->lbl_ofs[j];
		ctr=imgbase+sctx->sec_rva[sctx->rlc_sec[i]]+sctx->rlc_ofs[i];
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

		case BGBCC_SH_RLC_ABS32:
			d1=0x0C000000+(ctl-imgbase);
			b=bgbcc_gets32en(ctr, en);
			bgbcc_sets32en(ctr, en, b+d);
			break;

		case BGBCC_SH_RLC_RELW12:
			b=bgbcc_getu16en(ctr, en);
			b1=(b<<20)>>20;
			d1=b1+(d-4);
			if(((d1<<20)>>20)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xF000)|(d1&0x0FFF));
			break;
		case BGBCC_SH_RLC_RELW8:
			b=bgbcc_getu16en(ctr, en);
			b1=(b<<24)>>24;
			d1=b1+(d-4);
			if(((d1<<24)>>24)!=d1)
				__debugbreak();
			bgbcc_setu16en(ctr, en, (b&0xFF00)|(d1&0x00FF));
			break;
		}
	}
	
	return(0);
}

ccxl_status BGBCC_SHXC_FlattenImageELF(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_SHX_Context *sctx;
	byte *ct;
	int en, ofs, ofs_sdat, ofs_iend, ofs_mend;
	int of_phdr, ne_phdr;
	int of_shdr, ne_shdr;
	int i, j, k;

	sctx=ctx->uctx;

	en=sctx->is_le;
	ne_phdr=1;
	ne_shdr=sctx->nsec;
	
	of_phdr=64;
	
	of_shdr=of_phdr+(ne_phdr*32);

	ofs_sdat=of_shdr+(ne_shdr*40);
	ofs_sdat=(ofs_sdat+255)&(~255);

	k=ofs_sdat;
	for(i=0; i<sctx->nsec; i++)
	{
		if(i==BGBCC_SH_CSEG_BSS)
			continue;
		j=sctx->sec_pos[i]-sctx->sec_buf[i];
		sctx->sec_rva[i]=k;
		sctx->sec_lva[i]=0x0C000000+k;
		memcpy(obuf+k, sctx->sec_buf[i], j);
		k+=j;
		k=(k+255)&(~255);
	}
	ofs_iend=k;

	i=BGBCC_SH_CSEG_BSS;
	j=sctx->sec_pos[i]-sctx->sec_buf[i];
	sctx->sec_rva[i]=k;
	k+=j;
	k=(k+255)&(~255);

	ofs_mend=k;

	ct=obuf;
	ct[ 0]=0x7F;	ct[ 1]='E';			//magic
	ct[ 2]='L';		ct[ 3]='F';			//magic
	ct[ 4]=1;		ct[ 5]=en?1:2;		//width, endian
	ct[ 6]=1;		ct[ 7]=0;			//ELF version, OS ABI
	ct[ 8]=0;		ct[ 9]=0;			//pad
	ct[10]=0;		ct[11]=0;			//pad
	ct[12]=0;		ct[13]=0;			//pad
	ct[14]=0;		ct[15]=0;			//pad

	bgbcc_setu16en(ct+16, en, 3);		//image type
	bgbcc_setu16en(ct+18, en, 0x2A);	//arch
	bgbcc_setu32en(ct+20, en, 1);		//ELF version
	bgbcc_setu32en(ct+24, en, 0);		//entry point
	bgbcc_setu32en(ct+28, en, of_phdr);	//program headers offset
	bgbcc_setu32en(ct+32, en, of_shdr);	//section headers offset
	bgbcc_setu32en(ct+36, en, 0);		//arch flags
	bgbcc_setu16en(ct+40, en, 52);		//header size
	bgbcc_setu16en(ct+42, en, 32);		//size of program header entry
	bgbcc_setu16en(ct+44, en, ne_phdr);	//number of program header entries
	bgbcc_setu16en(ct+46, en, 40);		//section header entry size
	bgbcc_setu16en(ct+48, en, ne_shdr);	//number of entries in section header
	bgbcc_setu16en(ct+50, en, 0);		//index of section-header names

	ct=obuf+of_phdr;
	bgbcc_setu32en(ct+ 0, en, 1);			//segment type
	bgbcc_setu32en(ct+ 4, en, 0);			//segment image offset
	bgbcc_setu32en(ct+ 8, en, 0x0C000000);	//segment load address
	bgbcc_setu32en(ct+12, en, 0);			//undefined (phys address)
	bgbcc_setu32en(ct+16, en, ofs_iend);	//size of segment in file
	bgbcc_setu32en(ct+20, en, ofs_mend);	//size of segment in memory
	bgbcc_setu32en(ct+24, en, 7);			//access flags
	bgbcc_setu32en(ct+28, en, 4096);		//alignment

	return(0);
}

ccxl_status BGBCC_SHXC_FlattenImage(BGBCC_TransState *ctx,
	byte *obuf, int *rosz, fourcc imgfmt)
{
	BGBCC_SHX_Context *sctx;
	BGBCC_CCXL_LiteralInfo *obj;
	int l0;
	u32 addr;
	int i, j, k;

	sctx=ctx->uctx;
	
	sctx->lbl_got=BGBCC_SHX_GenLabel(sctx);

	for(i=0; i<ctx->n_literals; i++)
	{
		obj=ctx->literals[i];
		if(!obj)
			continue;

		if((obj->littype==CCXL_LITID_FUNCTION) && (obj->decl->vtr))
		{
			BGBCC_SHXC_BuildFunction(ctx, obj);
			continue;
		}

		if(obj->littype==CCXL_LITID_GLOBALVAR)
		{
			BGBCC_SHXC_BuildGlobal(ctx, obj);
			continue;
		}
	}

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

	BGBCC_SHXC_FlattenImageELF(ctx, obuf, rosz, imgfmt);
	return(1);
}
