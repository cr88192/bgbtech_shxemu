#include <bgbccc.h>

int BGBCC_FrBC_CheckExpandLvt4(BGBCC_FrCC_State *ctx)
{
	int i;

	if(!ctx->ctab_lvt4)
	{
		i=1024;
		ctx->ctab_lvt4=bgbcc_malloc(i*sizeof(u32));
		ctx->ctab_lvt4[0]=0;
		ctx->n_ctab_lvt4=1;
		ctx->m_ctab_lvt4=i;
	}
	
	if((ctx->n_ctab_lvt4+1)>=ctx->m_ctab_lvt4)
	{
		i=ctx->m_ctab_lvt4;
		i=i+(i>>1);
		ctx->ctab_lvt4=bgbcc_realloc(ctx->ctab_lvt4, i*sizeof(u32));
		ctx->m_ctab_lvt4=i;
	}
	
	return(0);
}

int BGBCC_FrBC_CheckExpandLvt8(BGBCC_FrCC_State *ctx)
{
	int i;

	if(!ctx->ctab_lvt8)
	{
		i=1024;
		ctx->ctab_lvt8=bgbcc_malloc(i*sizeof(u64));
		ctx->ctab_lvt8[0]=0;
		ctx->n_ctab_lvt8=1;
		ctx->m_ctab_lvt8=i;
	}
	
	if((ctx->n_ctab_lvt8+1)>=ctx->m_ctab_lvt8)
	{
		i=ctx->m_ctab_lvt8;
		i=i+(i>>1);
		ctx->ctab_lvt8=bgbcc_realloc(ctx->ctab_lvt8, i*sizeof(u64));
		ctx->m_ctab_lvt8=i;
	}

	return(0);
}

int BGBCC_FrBC_IndexLitU32(BGBCC_FrCC_State *ctx, u32 val)
{
	int i;

	for(i=0; i<ctx->n_ctab_lvt4; i++)
	{
		if(ctx->ctab_lvt4[i]==val)
			return(i);
	}

	BGBCC_FrBC_CheckExpandLvt4(ctx);
	
	i=ctx->n_ctab_lvt4++;
	ctx->ctab_lvt4[i]=val;
	return(i);
}

int BGBCC_FrBC_IndexLitU64(BGBCC_FrCC_State *ctx, u64 val)
{
	int i;

	for(i=0; i<ctx->n_ctab_lvt8; i++)
	{
		if(ctx->ctab_lvt8[i]==val)
			return(i);
	}

	BGBCC_FrBC_CheckExpandLvt8(ctx);
	
	i=ctx->n_ctab_lvt8++;
	ctx->ctab_lvt8[i]=val;
	return(i);
}

int BGBCC_FrBC_IndexLitS32(BGBCC_FrCC_State *ctx, s32 val)
{
	return(BGBCC_FrBC_IndexLitU32(ctx, (u32)val));
}

int BGBCC_FrBC_IndexLitF32(BGBCC_FrCC_State *ctx, float val)
{
	return(BGBCC_FrBC_IndexLitU32(ctx, *(u32 *)(&val)));
}

int BGBCC_FrBC_IndexLitS64(BGBCC_FrCC_State *ctx, s64 val)
{
	return(BGBCC_FrBC_IndexLitU64(ctx, (u64)val));
}

int BGBCC_FrBC_IndexLitF64(BGBCC_FrCC_State *ctx, double val)
{
	return(BGBCC_FrBC_IndexLitU64(ctx, *(u64 *)(&val)));
}

int BGBCC_FrBC_LookupString(BGBCC_FrCC_State *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!ctx->strtab)
		return(-1);

	if(!str)return(0);
	if(!(*str))return(1);

	s=ctx->strtab; se=s+ctx->sz_strtab; se2=s+ctx->msz_strtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->strtab);
		s+=strlen(s)+1;
	}

	return(-1);
}

int BGBCC_FrBC_LookupWString(BGBCC_FrCC_State *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!ctx->wstrtab)
		return(-1);

	if(!str)return(0);
	if(!(*str))return(1);

	s=ctx->wstrtab; se=s+ctx->sz_wstrtab; se2=s+ctx->msz_wstrtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->wstrtab);
		s+=strlen(s)+1;
	}

	return(-1);
}

int BGBCC_FrBC_IndexString(BGBCC_FrCC_State *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!str)return(0);
	if(!(*str))return(1);

	if(!ctx->strtab)
	{
		ctx->strtab=bgbcc_malloc(1<<18);
		ctx->sz_strtab=2;
		ctx->msz_strtab=1<<18;
		ctx->strtab[0]=0;
		ctx->strtab[1]=0;
	}
	
	s=ctx->strtab; se=s+ctx->sz_strtab; se2=s+ctx->msz_strtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->strtab);
		s+=strlen(s)+1;
	}
	
	if((s+strlen(str)+1)>=se2)
	{
		i=ctx->msz_strtab;
		j=(s+strlen(str)+1)-ctx->strtab;
		k=s-ctx->strtab;
		
		while(i<=j)i=i+(i>>1);
		ctx->strtab=bgbcc_realloc(ctx->strtab, i);
		ctx->msz_strtab=i;
		s=ctx->strtab+k;
		se=ctx->strtab+ctx->sz_strtab;
		se2=ctx->strtab+ctx->msz_strtab;
	}

	i=s-ctx->strtab;
	strcpy(s, str);
	s+=strlen(s)+1;
	ctx->sz_strtab=s-ctx->strtab;
	return(i);
}

int BGBCC_FrBC_IndexWString(BGBCC_FrCC_State *ctx, char *str)
{
	char *s, *se, *se2;
	int i, j, k;

	if(!str)return(0);
	if(!(*str))return(1);

	if(!ctx->wstrtab)
	{
		ctx->wstrtab=bgbcc_malloc(1<<18);
		ctx->sz_wstrtab=2;
		ctx->msz_wstrtab=1<<18;
		ctx->wstrtab[0]=0;
		ctx->wstrtab[1]=0;
	}
	
	s=ctx->wstrtab; se=s+ctx->sz_wstrtab; se2=s+ctx->msz_wstrtab;

	while(s<se)
	{
		if(!strcmp(s, str))
			return(s-ctx->wstrtab);
		s+=strlen(s)+1;
	}
	
	if((s+strlen(str)+1)>=se2)
	{
		i=ctx->msz_wstrtab;
		j=(s+strlen(str)+1)-ctx->wstrtab;
		k=s-ctx->wstrtab;
		
		while(i<=j)i=i+(i>>1);
		ctx->wstrtab=bgbcc_realloc(ctx->wstrtab, i);
		ctx->msz_wstrtab=i;
		s=ctx->wstrtab+k;
		se=ctx->wstrtab+ctx->sz_wstrtab;
		se2=ctx->wstrtab+ctx->msz_wstrtab;
	}

	i=s-ctx->wstrtab;
	strcpy(s, str);
	s+=strlen(s)+1;
	ctx->sz_wstrtab=s-ctx->wstrtab;
	return(i);
}


int BGBCC_FrBC_IndexCountString(BGBCC_FrCC_State *ctx, char *str)
{
	char *s, *se, *se2, *s0, *s1;
	int n0, n1;
	int i, j, k;

	if(!str)return(0);
	if(!(*str))return(1);

	if(!ctx->cntstrs)
	{
		i=4096;
		ctx->cntstrn=bgbcc_malloc(i*sizeof(int));
		ctx->cntstrs=bgbcc_malloc(i*sizeof(char *));
		ctx->n_cntstrs=0;
		ctx->m_cntstrs=i;
	}

	for(i=0; i<ctx->n_cntstrs; i++)
	{
		if(!strcmp(ctx->cntstrs[i], str))
			break;
	}
	
	if(i<ctx->n_cntstrs)
	{
		ctx->cntstrn[i]++;
		while((i>0) && (ctx->cntstrn[i]>ctx->cntstrn[i-1]))
		{
			s0=ctx->cntstrs[i  ];	n0=ctx->cntstrn[i  ];
			s1=ctx->cntstrs[i-1];	n1=ctx->cntstrn[i-1];
			ctx->cntstrs[i  ]=s1;	ctx->cntstrn[i  ]=n1;
			ctx->cntstrs[i-1]=s0;	ctx->cntstrn[i-1]=n0;
			i--;
		}
		return(i);
	}
	
	if((ctx->n_cntstrs+1)>=ctx->m_cntstrs)
	{
		i=ctx->m_cntstrs;
		i=i+(i>>1);
		ctx->cntstrn=bgbcc_realloc(ctx->cntstrn, i*sizeof(int));
		ctx->cntstrs=bgbcc_realloc(ctx->cntstrs, i*sizeof(char *));
		ctx->m_cntstrs=i;
	}
	
	i=ctx->n_cntstrs++;
	ctx->cntstrn[i]=1;
	ctx->cntstrs[i]=bgbcc_strdup(str);
	return(i);
}

int BGBCC_FrBC_FlattenCountStrings(BGBCC_FrCC_State *ctx)
{
	int i;

	if(!ctx->cntstrs)
		return(0);

	for(i=0; i<ctx->n_cntstrs; i++)
	{
		BGBCC_FrBC_IndexString(ctx, ctx->cntstrs[i]);
	}
	
	ctx->n_cntstrs=0;
	return(1);
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_LookupStruct(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=FR2C_LITID_STRUCT)
			continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
	return(NULL);
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_LookupUnion(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=FR2C_LITID_UNION)
			continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
	return(NULL);
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_LookupStructure(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if((cur->littype!=FR2C_LITID_STRUCT) &&
			(cur->littype!=FR2C_LITID_UNION) &&
			(cur->littype!=FR2C_LITID_CLASS) &&
			(cur->littype!=FR2C_LITID_FUNCTION))
				continue;
		if(!cur->name)
			continue;
		if(!strcmp(cur->name, str))
			return(cur);
	}
	return(NULL);
}

int BGBCC_FrBC_LookupStructureID(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	cur=BGBCC_FrBC_LookupStructure(ctx, str);
	if(cur)
		return(cur->litid);
	return(-1);
}

void BGBCC_FrBC_CheckExpandLiterals(
	BGBCC_FrCC_State *ctx)
{
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_GetStruct(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	int i, j, k;
	
	cur=BGBCC_FrBC_LookupStruct(ctx, str);
	if(cur)return(cur);
	
	BGBCC_FrBC_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
	cur->littype=FR2C_LITID_STRUCT;
	cur->name=bgbcc_strdup(str);

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;
	
	return(cur);
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_GetUnion(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	int i, j, k;
	
	cur=BGBCC_FrBC_LookupUnion(ctx, str);
	if(cur)return(cur);
	
	BGBCC_FrBC_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
	cur->littype=FR2C_LITID_UNION;
	cur->name=bgbcc_strdup(str);

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;
	
	return(cur);
}

int BGBCC_FrBC_GetStructID(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	cur=BGBCC_FrBC_GetStruct(ctx, str);
	if(cur)
		return(cur->litid);
	return(-1);
}

int BGBCC_FrBC_GetUnionID(
	BGBCC_FrCC_State *ctx, char *str)
{
	BGBCC_FrCC_LiteralInfo *cur;
	cur=BGBCC_FrBC_GetUnion(ctx, str);
	if(cur)
		return(cur->litid);
	return(-1);
}


BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_LookupTypedef(
	BGBCC_FrCC_State *ctx, char *name, char *sig)
{
	BGBCC_FrCC_LiteralInfo *cur;
	int i, j, k;
	
	for(i=0; i<ctx->n_literals; i++)
	{
		cur=ctx->literals[i];
		if(!cur)
			continue;
		if(cur->littype!=FR2C_LITID_TYPEDEF)
			continue;

		if(name)
		{
			if(!cur->name)
				continue;
			if(strcmp(cur->name, name))
				continue;
		}else
		{
			if(cur->name)
				continue;
		}

		if(sig)
		{
			if(!cur->sig)
				continue;
			if(strcmp(cur->sig, sig))
				continue;
		}
		
		return(cur);
	}
	return(NULL);
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_GetTypedef(
	BGBCC_FrCC_State *ctx, char *name, char *sig)
{
	BGBCC_FrCC_LiteralInfo *cur;
	int i, j, k;
	
	cur=BGBCC_FrBC_LookupTypedef(ctx, name, sig);
	if(cur)return(cur);
	
	BGBCC_FrBC_CheckExpandLiterals(ctx);

	cur=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
	cur->littype=FR2C_LITID_TYPEDEF;
	cur->name=bgbcc_strdup(name);
	cur->sig=bgbcc_strdup(sig);

	i=ctx->n_literals++;
	ctx->literals[i]=cur;
	cur->litid=i;
	
	return(cur);
}
