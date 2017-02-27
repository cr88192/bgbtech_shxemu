#include <bgbccc.h>

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureForSig(
	BGBCC_TransState *ctx, char *sig)
{
	char *s, *t;

	s=sig;
	while(*s && (*s=='P'))
		{ s++; }

	return(BGBCC_CCXL_LookupStructureForSig2(ctx, s));
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureForSig2(
	BGBCC_TransState *ctx, char *sig)
{
	char tb[256];
	BGBCC_CCXL_LiteralInfo *cur;
	char *s, *t;
	int i;
	
	s=sig;
//	while(*s && (*s=='P'))
//		{ s++; }
	if((*s!='X') && (*s!='Y'))
		{ return(NULL); }
	s++;
		
	if((*s>='0') && (*s<='9'))
	{
		i=atoi(s);
		cur=ctx->literals[i];
		return(cur);
	}

	t=tb;
	while(*s && (*s!=';'))
		*t++=*s++;
	*t++=0;
	
	cur=BGBCC_CCXL_LookupStructure(ctx, tb);
	return(cur);
}

int BGBCC_CCXL_LookupStructureIDForSig(
	BGBCC_TransState *ctx, char *sig)
{
	BGBCC_CCXL_LiteralInfo *cur;
	
	cur=BGBCC_CCXL_LookupStructureForSig(ctx, sig);
	if(!cur)return(-1);
	return(cur->litid);
}

BGBCC_CCXL_LiteralInfo *BGBCC_CCXL_LookupStructureForType(
	BGBCC_TransState *ctx, ccxl_type type)
{
	BGBCC_CCXL_TypeOverflow ovf;
	BGBCC_CCXL_LiteralInfo *st;
	int bt;

	if((type.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_BASIC)
	{
		bt=type.val&FR2C_TY_BASEMASK;
		st=ctx->literals[bt-256];
		return(st);
	}

	if((type.val&FR2C_TY_TYTY_MASK)==FR2C_TY_TYTY_OVF1)
	{
		ovf=*(ctx->tyovf[type.val&FR2C_TYOVF_IDXMASK]);
//		bt=type.val&FR2C_TYB2_BASEMASK;
		bt=ovf.base;
		st=ctx->literals[bt-256];
		return(st);
	}
	
	BGBCC_CCXL_TagError(ctx,
		FR2C_TERR_STATUS(FR2C_STATUS_ERR_UNHANDLEDTYPE));
	return(NULL);
}

int BGBCC_CCXL_LookupStructureIDForType(
	BGBCC_TransState *ctx, ccxl_type type)
{
	BGBCC_CCXL_LiteralInfo *cur;
	
	cur=BGBCC_CCXL_LookupStructureForType(ctx, type);
	if(!cur)return(-1);
	return(cur->litid);
}

int BGBCC_CCXL_LookupStructContainsFieldID(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, char *name)
{
	BGBCC_CCXL_LiteralInfo *st2;
	int i, j;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
		{
			st2=BGBCC_CCXL_LookupStructureForType(ctx,
				st->decl->fields[i]->type);
			if(!st2)
				continue;
			
			j=BGBCC_CCXL_LookupStructContainsFieldID(ctx, st2, name);
			if(j>=0)return(i);

			continue;
		}

		if(!strcmp(st->decl->fields[i]->name, name))
			return(i);
	}
	return(-1);
}

int BGBCC_CCXL_LookupStructFieldID(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, char *name)
{
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
			continue;

		if(!strcmp(st->decl->fields[i]->name, name))
			return(i);
	}
	return(-1);
}

ccxl_status BGBCC_CCXL_LookupStructFieldType(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, char *name,
	ccxl_type *rty)
{
	ccxl_type bty;
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	for(i=0; i<st->decl->n_fields; i++)
	{
		if(!st->decl->fields[i]->name)
			continue;

		if(!strcmp(st->decl->fields[i]->name, name))
		{
			BGBCC_CCXL_TypeFromSig(ctx, &bty,
				st->decl->fields[i]->sig);
			*rty=bty;
			return(1);
		}
	}
	
//	printf("BGBCC_CCXL_LookupStructFieldType: No Field '%s'\n", name);
	bty=BGBCC_CCXL_TypeWrapBasicType(FR2C_TY_I);
//	*rty=bty;	
	return(0);
}

ccxl_status BGBCC_CCXL_LookupStructFieldIdType(
	BGBCC_TransState *ctx, BGBCC_CCXL_LiteralInfo *st, int idx,
	ccxl_type *rty)
{
	ccxl_type bty;
	int i;
	
	if(!st)
		return(-1);
	if(!st->decl)
		return(-1);
	
	if((idx>=0) && (idx<st->decl->n_fields))
	{
		BGBCC_CCXL_TypeFromSig(ctx, &bty,
			st->decl->fields[idx]->sig);
		*rty=bty;
		return(1);
	}
	
	bty=BGBCC_CCXL_TypeWrapBasicType(FR2C_TY_I);
	return(-1);
}
