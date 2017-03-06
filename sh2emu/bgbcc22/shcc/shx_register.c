/*
 * R0/R1/R2/R3: Return Value, Scratch
 *   R2=Struct Return Pointer
 * R4/R5/R6/R7: Arg Passing/Scratch
 * R8..R11: Var Cache (Callee Save)
 * R12: Context Pointer (GOT)
 * R13: Var Cache (Callee Save)
 * R14: Frame Pointer
 * R15: Stack Pointer
 * FR5,FR4,FR7,FR6,FR9,FR8,FR11,FR10: Float/Double Args, Scratch
 * FR12..FR15: Var Cache (Callee Save)
 *
 * Frame (Frame-Ref Point)
 *   (- 4) Saved PR
 *   (- 8) Saved R14
 *   (-12) Saved R13
 *   (-16) Saved R12
 *   (-20) Saved R11
 *   (-24) Saved R10
 *   (-28) Saved R9
 *   (-32) Saved R8
 *   ...
 *   SP (Stack Ref Point)
 */


const byte bgbcc_shx_cachereg[6]={ 13, 11, 10, 9, 8, 255 };

int BGBCC_SHXC_EmitLoadFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int ofs1;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);

	p0=BGBCC_SHX_EmitOpLdRegDispReg(sctx, BGBCC_SH_NMID_MOVL,
		BGBCC_SH_REG_SP, ofs1, dreg);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpLdReg2Reg(sctx, BGBCC_SH_NMID_MOVL,
			BGBCC_SH_REG_SP, BGBCC_SH_REG_R0, dreg);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitStoreFrameOfsReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	int ofs, int dreg)
{
	int p0, p1;
	int ofs1;
	int i, j, k;

	ofs1=ofs+(sctx->frm_size);

	p0=BGBCC_SHX_EmitOpRegStRegDisp(sctx, BGBCC_SH_NMID_MOVL,
		dreg, BGBCC_SH_REG_SP, ofs1);
	if(p0>0)
		return(1);

	p0=BGBCC_SHX_EmitLoadRegImm(sctx, BGBCC_SH_NMID_MOV,
		BGBCC_SH_REG_R0, ofs1);
	if(p0>0)
	{
		p1=BGBCC_SHX_EmitOpRegStReg2(sctx, BGBCC_SH_NMID_MOVL,
			dreg, BGBCC_SH_REG_R0, BGBCC_SH_REG_SP);
		if(p1>0)
			return(1);
	}

	BGBCC_CCXL_StubError(ctx);
	return(0);
}

int BGBCC_SHXC_EmitLoadVRegReg(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register sreg, int dreg)
{
	int p0, p1;
	int i, j, k;

	if(BGBCC_CCXL_IsRegLocalP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_REGBASE;
//		k=sctx->frm_offs_lcl+j*4;
		k=ctx->cur_func->locals[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegTempP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_REGBASE;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->regs[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}

	if(BGBCC_CCXL_IsRegArgP(ctx, sreg))
	{
		j=sreg.val&CCXL_REGID_REGBASE;
//		k=sctx->frm_offs_tmp+j*4;
		k=ctx->cur_func->args[j]->fxoffs;
		i=BGBCC_SHXC_EmitLoadFrameOfsReg(ctx, sctx, k, dreg);
		return(i);
	}
	
	BGBCC_CCXL_StubError(ctx);
	return(0);
}


/* Get Variable as register.
 * If no registers are free, evict least-recently-used.
 */
int BGBCC_SHXC_EmitGetRegister(
	BGBCC_TransState *ctx,
	BGBCC_SHX_Context *sctx,
	ccxl_register reg)
{
	int i;

	/* value already in a register? */
	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
		if(reg.val==sctx->regalc_map[i].val)
		{
			sctx->regalc_live|=1<<i;
			if(sctx->regalc_ltcnt[i]>0)
				sctx->regalc_ltcnt[i]--;
			sctx->regalc_utcnt[i]++;
			return(bgbcc_shx_cachereg[i]);
		}
	}

	for(i=0; i<5; i++)
		if(sctx->regalc_ltcnt[i]<255)
			sctx->regalc_ltcnt[i]++;

	/* Check for registers not holding a live value. */
	for(i=0; i<5; i++)
	{
		if(!((sctx->regalc_save)&(1<<i)))
			continue;
		if(!((sctx->regalc_live)&(1<<i)))
		{
			sctx->regalc_map[i]=reg;
			sctx->regalc_live|=1<<i;
			return(bgbcc_shx_cachereg[i]);
		}
	}

	/* Check for unallocated registers. */
	for(i=0; i<5; i++)
	{
		if((sctx->regalc_save)&(1<<i))
			continue;
		
		sctx->regalc_map[i]=reg;
		sctx->regalc_save|=1<<i;
		sctx->regalc_live|=1<<i;
		return(bgbcc_shx_cachereg[i]);
	}
}
