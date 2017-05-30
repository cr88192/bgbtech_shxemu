#include <bgbccc.h>

BGBCC_CCXL_VirtOp *BGBCC_CCXL_AllocVirtOp(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtOp *tmp, *tmpa;
	int i;
	
	tmp=ctx->virtop_free;
	if(tmp)
	{
		ctx->virtop_free=*(BGBCC_CCXL_VirtOp **)tmp;
		memset(tmp, 0, sizeof(BGBCC_CCXL_VirtOp));
		return(tmp);
	}
	
	tmp=bgbcc_malloc(256*sizeof(BGBCC_CCXL_VirtOp));
	for(i=0; i<255; i++)
	{
		*(BGBCC_CCXL_VirtOp **)tmp=ctx->virtop_free;
		ctx->virtop_free=tmp++;
	}

	memset(tmp, 0, sizeof(BGBCC_CCXL_VirtOp));
	return(tmp);
}

BGBCC_CCXL_VirtTr *BGBCC_CCXL_AllocVirtTr(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtTr *tmp, *tmpa;
	int i;
	
	tmp=ctx->virttr_free;
	if(tmp)
	{
		ctx->virttr_free=*(BGBCC_CCXL_VirtTr **)tmp;
		memset(tmp, 0, sizeof(BGBCC_CCXL_VirtTr));
		return(tmp);
	}
	
	tmp=bgbcc_malloc(256*sizeof(BGBCC_CCXL_VirtTr));
	for(i=0; i<255; i++)
	{
		*(BGBCC_CCXL_VirtTr **)tmp=ctx->virttr_free;
		ctx->virttr_free=tmp++;
	}

	memset(tmp, 0, sizeof(BGBCC_CCXL_VirtTr));
	return(tmp);
}

int BGBCC_CCXL_AddVirtOp(BGBCC_TransState *ctx, BGBCC_CCXL_VirtOp *op)
{
	int i;

	if(!ctx->vop)
	{
		ctx->vop=bgbcc_malloc(4096*sizeof(BGBCC_CCXL_VirtOp *));
		ctx->n_vop=0;
		ctx->m_vop=4096;
	}

	if((ctx->n_vop+1)>(ctx->m_vop))
	{
		i=ctx->m_vop+(ctx->m_vop>>1);
		ctx->vop=bgbcc_realloc(ctx->vop,
			i*sizeof(BGBCC_CCXL_VirtOp *));
//		ctx->n_vop=0;
		ctx->m_vop=i;
	}

	i=ctx->n_vop++;
	ctx->vop[i]=op;
	return(i);
}

int BGBCC_CCXL_AddVirtTr(BGBCC_TransState *ctx, BGBCC_CCXL_VirtTr *tr)
{
	int i;

	if(!ctx->vtr)
	{
		ctx->vtr=bgbcc_malloc(256*sizeof(BGBCC_CCXL_VirtOp *));
		ctx->n_vtr=0;
		ctx->m_vtr=256;
	}

	if((ctx->n_vtr+1)>(ctx->m_vtr))
	{
		i=ctx->m_vtr+(ctx->m_vtr>>1);
		ctx->vtr=bgbcc_realloc(ctx->vtr,
			i*sizeof(BGBCC_CCXL_VirtTr *));
//		ctx->n_vtr=0;
		ctx->m_vtr=i;
	}

	i=ctx->n_vtr++;
	ctx->vtr[i]=tr;
	return(i);
}

int BGBCC_CCXL_EmitMarkEndTrace(BGBCC_TransState *ctx)
{
	BGBCC_CCXL_VirtTr *tr;
	int i, n;

	n=ctx->n_vop-ctx->s_vop;
	if(!n)
		return(-1);

	if(n<0)
		{ BGBCC_DBGBREAK }
	
	tr=BGBCC_CCXL_AllocVirtTr(ctx);

	tr->b_ops=ctx->s_vop;
	tr->n_ops=ctx->n_vop-tr->b_ops;
	ctx->s_vop=ctx->n_vop;

	i=BGBCC_CCXL_AddVirtTr(ctx, tr);
	return(i);
}
