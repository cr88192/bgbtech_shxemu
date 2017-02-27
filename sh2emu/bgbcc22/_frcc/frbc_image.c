#include <bgbccc.h>

BGBCC_FrCC_LiteralInfo *bgbcc_frcc_freelits=NULL;

/** Stub Errors which indicate Error conditions.
  * These mean the output code will not work.
  * Or for fully unimplemented functionality.
  */
void BGBCC_FrBC_StubErrorLLn(BGBCC_FrCC_State *ctx,
	char *file, int line)
{
	BGBCC_FrCC_Error(ctx, "StubError %s:%d (CC=%s:%d)\n",
		file, line, ctx->lfn, ctx->lln);
	*(int *)-1=-1;
}

/** Stub Warnings which indicate Warning conditions.
  * These are for functionality which may produce unintended behavior.
  * This may also apply to hacked / incomplete functionality.
  */
void BGBCC_FrBC_StubWarnLLn(BGBCC_FrCC_State *ctx,
	char *file, int line)
{
	BGBCC_FrCC_Warn(ctx, "StubWarning %s:%d (CC=%s:%d)\n",
		file, line, ctx->lfn, ctx->lln);
}

/** Stub Warnings which indicate Note conditions.
  * These are for unimplemented / incomplete functionality, 
  * Which does not otherwise effect output code.
  * These are to be queitly logged.
  */
void BGBCC_FrBC_StubNoteLLn(BGBCC_FrCC_State *ctx,
	char *file, int line)
{
	BGBCC_FrCC_Note(ctx, "StubNote %s:%d (CC=%s:%d)\n",
		file, line, ctx->lfn, ctx->lln);
}


/** Stub Errors which indicate Error conditions.
  * These mean the output code will not work.
  * Or for fully unimplemented functionality.
  */
void BGBCC_FrBC_TagErrorLLn(BGBCC_FrCC_State *ctx, int tag,
	char *file, int line)
{
	BGBCC_FrCC_Error(ctx, "TagError %s:%d(%04X) (CC=%s:%d)\n",
		file, line, tag, ctx->lfn, ctx->lln);
	*(int *)-1=-1;
}

/** Stub Warnings which indicate Warning conditions.
  * These are for functionality which may produce unintended behavior.
  * This may also apply to hacked / incomplete functionality.
  */
void BGBCC_FrBC_TagWarnLLn(BGBCC_FrCC_State *ctx, int tag,
	char *file, int line)
{
	BGBCC_FrCC_Warn(ctx, "TagWarning %s:%d(%04X) (CC=%s:%d)\n",
		file, line, tag, ctx->lfn, ctx->lln);
}

/** Stub Warnings which indicate Note conditions.
  * These are for unimplemented / incomplete functionality, 
  * Which does not otherwise effect output code.
  * These are to be queitly logged.
  */
void BGBCC_FrBC_TagNoteLLn(BGBCC_FrCC_State *ctx, int tag,
	char *file, int line)
{
	BGBCC_FrCC_Note(ctx, "TagNote %s:%d(%04X) (CC=%s:%d)\n",
		file, line, tag, ctx->lfn, ctx->lln);
}


void BGBCC_FrBC_AddGlobalDecl(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_RegisterInfo *decl)
{
	if(!ctx->n_reg_globals)
		ctx->n_reg_globals++;
	decl->gblid=ctx->n_reg_globals;
	ctx->reg_globals[ctx->n_reg_globals++]=decl;
	
	decl->hashnext=ctx->usort_globals;
	ctx->usort_globals=decl;
}

void BGBCC_FrBC_AddFrameArg(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_RegisterInfo *frame,
	BGBCC_FrCC_RegisterInfo *decl)
{
	int i;

	if(!frame->args)
	{
		frame->args=bgbcc_malloc(64*sizeof(BGBCC_FrCC_RegisterInfo *));
		frame->n_args=0;
		frame->m_args=64;
	}

	if(frame->n_args>=frame->m_args)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_FORMAT_SANITY);
	}

	if(frame->n_eargs>0)
	{
		i=frame->n_vargs++;
		if(decl->name)
		{
			frame->args[i]->name=
				decl->name;
		}
		return;
	}

	decl->gblid=frame->n_args;
	frame->args[frame->n_args++]=decl;
}

void BGBCC_FrBC_AddFrameLocal(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_RegisterInfo *frame,
	BGBCC_FrCC_RegisterInfo *decl)
{
	if(!frame->locals)
	{
		frame->locals=bgbcc_malloc(256*sizeof(BGBCC_FrCC_RegisterInfo *));
		frame->n_locals=0;
		frame->m_locals=256;
	}

	if(frame->n_locals>=frame->m_locals)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_FORMAT_SANITY);
	}

	decl->gblid=frame->n_locals;
	frame->locals[frame->n_locals++]=decl;
}

void BGBCC_FrBC_AddFrameField(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_RegisterInfo *frame,
	BGBCC_FrCC_RegisterInfo *decl)
{
	if(!frame->fields)
	{
		frame->fields=bgbcc_malloc(256*sizeof(BGBCC_FrCC_RegisterInfo *));
		frame->n_fields=0;
		frame->m_fields=256;
	}

	if(frame->n_fields>=frame->m_fields)
	{
		BGBCC_FrBC_TagError(ctx,
			FR2C_TERR_FORMAT_SANITY);
	}

	decl->gblid=frame->n_fields;
	frame->fields[frame->n_fields++]=decl;
}

BGBCC_FrCC_LiteralInfo *BGBCC_FrBC_AllocLiteral(BGBCC_FrCC_State *ctx)
{
	BGBCC_FrCC_LiteralInfo *obj;
	
	if(bgbcc_frcc_freelits)
	{
		obj=bgbcc_frcc_freelits;
		bgbcc_frcc_freelits=obj->parent;
		memset(obj, 0, sizeof(BGBCC_FrCC_LiteralInfo));
		return(obj);
	}

	obj=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
	return(obj);
}

void BGBCC_FrBC_CheckFreeLiteral(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_LiteralInfo *obj)
{
	if(!obj)return;

	if(obj->litid>0)
		return;
		
	obj->parent=bgbcc_frcc_freelits;
	bgbcc_frcc_freelits=obj;
}

void BGBCC_FrBC_AddLiteral(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_LiteralInfo *obj)
{
	if(!ctx->n_literals)
		ctx->n_literals++;
		
	obj->litid=ctx->n_literals;
	ctx->literals[ctx->n_literals++]=obj;
}

char *BGBCC_FrBC_GetParentLiteralSig(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *obj)
{
	char *sig;

	if(!obj->parent)
		return(NULL);

	sig=NULL;
	switch(obj->parent->littype)
	{
	case FR2C_LITID_ARGS:
		break;
	case FR2C_LITID_LOCALS:
		break;
	case FR2C_LITID_FUNCTION:
		sig=obj->parent->decl->sig;
		break;
	case FR2C_LITID_VAR:
	case FR2C_LITID_GLOBALVAR:
		sig=obj->parent->decl->sig;
		break;
	case FR2C_LITID_STRUCT:
	case FR2C_LITID_UNION:
		break;
	case FR2C_LITID_LIST:
		sig=obj->parent->decl->sig;
		break;
	case FR2C_LITID_VALUE:
		sig=BGBCC_FrBC_GetParentLiteralSig(ctx, obj->parent);
		break;
	}
	return(sig);
}

char *BGBCC_FrBC_GetParentLiteralSigDeref(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *obj)
{
	char tb[256];
	char *sig, *s, *t;
	
	sig=BGBCC_FrBC_GetParentLiteralSig(ctx, obj);
	if(!sig)
		return(sig);
	
	if(*sig=='P')
		return(sig+1);

	if(*sig=='A')
	{
		s=sig+1;
		while((*s) && (*s>='0') && (*s<='9'))
			s++;
		if(*s==';')
			return(s+1);
		if((*s>='A') && *s<='Z')
			return(s);
		if((*s>='a') && *s<='z')
			return(s);
			
		if(*s==',')
		{
			t=tb; s++;
			*t++='A';
			while(*s)
				*t++=*s++;
			*t++=0;
			
			return(bgbcc_strdup(tb));
		}
	}

	if(*sig=='X')
	{
			s=sig;
			t=tb; s++;
			*t++='Y';
			while(*s)
				*t++=*s++;
			*t++=0;
			return(bgbcc_strdup(tb));
	}

	return(sig);
}

char *BGBCC_FrBC_GetObjQNameR_I(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *obj, char *vt)
{
	char *t;

	if(obj->parent)
	{
		t=BGBCC_FrBC_GetObjQNameR_I(ctx, obj->parent, vt);
		if(obj->decl && obj->decl->name)
		{
			if(t!=vt) { *t++='/'; }
			strcpy(t, obj->decl->name);
			t=t+strlen(t);
		}
		return(t);
	}

	if(obj->decl && obj->decl->name)
	{
		t=vt;
		strcpy(t, obj->decl->name);
		t=t+strlen(t);
		return(t);
	}
	
	return(vt);
}

char *BGBCC_FrBC_GetObjQName(
	BGBCC_FrCC_State *ctx, BGBCC_FrCC_LiteralInfo *obj)
{
	char tb[256];
	
	if(!obj)
		return(NULL);
	
	memset(tb, 0, 256);
	BGBCC_FrBC_GetObjQNameR_I(ctx, obj, tb);
	if(!tb[0])
		return(NULL);
	return(bgbcc_strdup(tb));
}

void BGBCC_FrBC_Begin(BGBCC_FrCC_State *ctx, int tag)
{
	BGBCC_FrBC_BeginName(ctx, tag, NULL);
}

int BGBCC_FrBC_CheckDefinedContextName(
	BGBCC_FrCC_State *ctx, int tag, char *name)
{
	BGBCC_FrCC_RegisterInfo *decl;

	if((tag==FR2C_CMD_VARDECL) && !ctx->cur_obj)
	{
		decl=BGBCC_FrBC_LookupGlobal(ctx, name);
		if(decl)
			return(1);
	}
	return(0);
}

void BGBCC_FrBC_BeginName(BGBCC_FrCC_State *ctx, int tag, char *name)
{
	BGBCC_FrCC_LiteralInfo *obj;
	BGBCC_FrCC_RegisterInfo *decl;

	if(name)
	{
		if(tag==FR2C_CMD_STRUCT)
		{
			obj=BGBCC_FrBC_LookupStructure(ctx, name);
			if(obj)
			{
				if(!obj->decl)
				{
					obj->littype=FR2C_LITID_STRUCT;
					obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
					obj->decl->regtype=FR2C_LITID_STRUCT;
				}
			
				obj->decl->n_fields=0;
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				return;
			}
		}

		if(tag==FR2C_CMD_UNION)
		{
			obj=BGBCC_FrBC_LookupStructure(ctx, name);
			if(obj)
			{
				if(!obj->decl)
				{
					obj->littype=FR2C_LITID_UNION;
					obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
					obj->decl->regtype=FR2C_LITID_UNION;
				}
			
				obj->decl->n_fields=0;
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				return;
			}
		}

		if(tag==FR2C_CMD_FUNCTION)
		{
			decl=BGBCC_FrBC_LookupGlobal(ctx, name);
			if(decl)
			{
				obj=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
				obj->parent=ctx->cur_obj;
	
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				
				obj->decl=decl;

				obj->littype=FR2C_LITID_FUNCTION;
				obj->decl->regtype=FR2C_LITID_FUNCTION;

				obj->decl->n_fields=0;
//				obj->decl->n_args=0;
				obj->decl->n_locals=0;
				obj->decl->n_vargs=0;

				ctx->regstackpos=0;
				ctx->uregstackpos=0;
				ctx->markstackpos=0;
				ctx->ip=ctx->ips;
				ctx->n_goto=0;
				ctx->n_lbl=0;
				return;
			}
		}

		if(tag==FR2C_CMD_PROTOTYPE)
		{
			decl=BGBCC_FrBC_LookupGlobal(ctx, name);
			if(decl)
			{
				obj=BGBCC_FrBC_AllocLiteral(ctx);
//				obj=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
				obj->parent=ctx->cur_obj;
	
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;

				obj->decl->n_fields=0;
//				obj->decl->n_args=0;
				obj->decl->n_vargs=0;

				ctx->ip=ctx->ips;
				ctx->n_goto=0;
				ctx->n_lbl=0;

				obj->decl=decl;
				obj->littype=FR2C_LITID_FUNCTION;
				obj->decl->regtype=FR2C_LITID_FUNCTION;
				return;
			}
		}

		if(tag==FR2C_CMD_S_PROTOTYPE)
		{
			obj=BGBCC_FrBC_LookupStructure(ctx, name);
			if(obj)
			{
				if(!obj->decl)
				{
					obj->littype=FR2C_LITID_FUNCTION;
					obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
					obj->decl->regtype=FR2C_LITID_FUNCTION;
				}

				obj->decl->n_fields=0;
//				obj->decl->n_args=0;
				obj->decl->n_vargs=0;

				ctx->ip=ctx->ips;
				ctx->n_goto=0;
				ctx->n_lbl=0;

				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;
				return;
			}
		}

		if((tag==FR2C_CMD_VARDECL) && !ctx->cur_obj)
		{
			decl=BGBCC_FrBC_LookupGlobal(ctx, name);
			if(decl)
			{
				obj=BGBCC_FrBC_AllocLiteral(ctx);
//				obj=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
				obj->parent=ctx->cur_obj;
	
				ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
				ctx->cur_obj=obj;

				obj->decl=decl;
				obj->littype=FR2C_LITID_GLOBALVAR;
				obj->decl->regtype=FR2C_LITID_GLOBALVAR;
				return;
			}
		}
	}

	obj=BGBCC_FrBC_AllocLiteral(ctx);
//	obj=bgbcc_malloc(sizeof(BGBCC_FrCC_LiteralInfo));
	obj->parent=ctx->cur_obj;
	
	ctx->cur_objstack[ctx->cur_objstackpos++]=obj;
	ctx->cur_obj=obj;
	
	switch(tag)
	{
	case FR2C_CMD_VARDECL:
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_VAR;

#if 1
		if(obj->parent)
		{
			obj->littype=FR2C_LITID_VAR;
			obj->decl->regtype=FR2C_LITID_VAR;
		}else
		{
			obj->littype=FR2C_LITID_GLOBALVAR;
			obj->decl->regtype=FR2C_LITID_GLOBALVAR;
		}
#endif

#if 1
		if(obj->parent)
		{
			obj->littype=FR2C_LITID_VAR;
			switch(obj->parent->littype)
			{
			case FR2C_LITID_ARGS:
//				BGBCC_FrBC_AddFrameArg(ctx,
//					obj->parent->parent->decl,
//					obj->decl);
				break;
			case FR2C_LITID_LOCALS:
				BGBCC_FrBC_AddFrameLocal(ctx,
					obj->parent->parent->decl,
					obj->decl);
				break;
			case FR2C_LITID_FUNCTION:
				obj->parent->decl->defv=obj->decl;
				break;
			case FR2C_LITID_STRUCT:
			case FR2C_LITID_UNION:
				BGBCC_FrBC_AddFrameField(ctx,
					obj->parent->decl,
					obj->decl);
				break;
			}
		}else
		{
			obj->littype=FR2C_LITID_GLOBALVAR;
			obj->decl->regtype=FR2C_LITID_GLOBALVAR;
			BGBCC_FrBC_AddGlobalDecl(ctx, obj->decl);
		}
#endif
		break;
	case FR2C_CMD_STATICVARDECL:
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_STATICVAR;
		obj->littype=FR2C_LITID_GLOBALVAR;
		BGBCC_FrBC_AddGlobalDecl(ctx, obj->decl);

		if(obj->parent)
		{
			switch(obj->parent->littype)
			{
			case FR2C_LITID_ARGS:
//				BGBCC_FrBC_AddFrameArg(ctx,
//					obj->parent->parent->decl,
//					obj->decl);
				break;
			case FR2C_LITID_LOCALS:
//				BGBCC_FrBC_AddFrameLocal(ctx,
//					obj->parent->parent->decl,
//					obj->decl);
				obj->decl->defp=obj->parent->parent->decl;
				break;
			case FR2C_LITID_FUNCTION:
//				obj->parent->decl->defv=obj->decl;
				break;
			case FR2C_LITID_STRUCT:
			case FR2C_LITID_UNION:
//				BGBCC_FrBC_AddFrameField(ctx,
//					obj->parent->decl,
//					obj->decl);
				break;
			}
		}
		break;

	case FR2C_CMD_FUNCTION:
		obj->littype=FR2C_LITID_FUNCTION;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_FUNCTION;
		BGBCC_FrBC_AddGlobalDecl(ctx, obj->decl);

		ctx->regstackpos=0;
		ctx->uregstackpos=0;
		ctx->markstackpos=0;
		ctx->ip=ctx->ips;
		ctx->n_goto=0;
		ctx->n_lbl=0;
		break;

	case FR2C_CMD_ARGS:
		obj->littype=FR2C_LITID_ARGS;
//		obj->parent->decl->n_args=0;
		break;
	case FR2C_CMD_LOCALS:
		obj->littype=FR2C_LITID_LOCALS;
//		obj->parent->decl->n_locals=0;
		break;
	case FR2C_CMD_VARVALUE:
		obj->littype=FR2C_LITID_VALUE;
		break;

	case FR2C_CMD_PROTOTYPE:
		obj->littype=FR2C_LITID_FUNCTION;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_FUNCTION;
		BGBCC_FrBC_AddGlobalDecl(ctx, obj->decl);

		ctx->ip=ctx->ips;
		ctx->n_goto=0;
		ctx->n_lbl=0;
		break;

	case FR2C_CMD_S_PROTOTYPE:
		obj->littype=FR2C_LITID_FUNCTION;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_FUNCTION;
		BGBCC_FrBC_AddLiteral(ctx, obj);
		break;

	case FR2C_CMD_STRUCT:
		obj->littype=FR2C_LITID_STRUCT;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_STRUCT;
		BGBCC_FrBC_AddLiteral(ctx, obj);
		break;
	case FR2C_CMD_UNION:
		obj->littype=FR2C_LITID_UNION;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_UNION;
		BGBCC_FrBC_AddLiteral(ctx, obj);
		break;

	case FR2C_CMD_LIST:
		obj->littype=FR2C_LITID_LIST;
		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
		obj->decl->regtype=FR2C_LITID_LIST;
		obj->decl->sig=BGBCC_FrBC_GetParentLiteralSigDeref(ctx, obj);
		obj->decl->defp=obj->parent->decl;
		BGBCC_FrBC_AddLiteral(ctx, obj);
		break;

	case FR2C_CMD_BODY:
		ctx->cur_func=obj->parent->decl;
		ctx->regstackpos=0;
		ctx->markstackpos=0;
		ctx->ip=ctx->ips;
		ctx->n_goto=0;
		ctx->n_lbl=0;
		break;

	default:
		break;
	}

//	BGBCC_FrBC_StubError(ctx);
}

int BGBCC_FrBC_LookupCurFunctionLabel(BGBCC_FrCC_State *ctx, char *name)
{
	int i, j, k;
	
	for(i=0; i<ctx->n_lbl; i++)
	{
		if(!strcmp(ctx->lbl_name[i], name))
			return(ctx->lbl_dest[i]);
	}
	return(-1);
}

void BGBCC_FrBC_EndFunction(BGBCC_FrCC_State *ctx,
	BGBCC_FrCC_LiteralInfo *obj)
{
	byte *rla, *rld;
	int i, j, k;

	for(i=0; i<ctx->n_goto; i++)
	{
		switch(ctx->goto_type[i])
		{
		case FR2C_JMPMODE_A16:
			j=BGBCC_FrBC_LookupCurFunctionLabel(ctx,
				ctx->goto_name[i]);
			rla=ctx->ips+ctx->goto_dest[i];
			rld=ctx->ips+j;
			k=(rla[-2]<<8)|(rla[-1]);
			k+=rld-rla;
			rla[-2]=(k>>8)&255;
			rla[-1]=(k   )&255;
			break;
		case FR2C_JMPMODE_A32:
			j=BGBCC_FrBC_LookupCurFunctionLabel(ctx,
				ctx->goto_name[i]);
			rla=ctx->ips+ctx->goto_dest[i];
			rld=ctx->ips+j;
			k=(rla[-4]<<24)|(rla[-3]<<16)|(rla[-2]<<8)|(rla[-1]);
			k+=rld-rla;
			rla[-4]=(k>>24)&255;
			rla[-3]=(k>>16)&255;
			rla[-2]=(k>> 8)&255;
			rla[-1]=(k    )&255;
			break;
		default:
			break;
		}
	}

	if(obj->decl->defv)
	{
		obj->decl->name=obj->decl->defv->name;
		obj->decl->sig=obj->decl->defv->sig;
		obj->decl->flagstr=obj->decl->defv->flagstr;
	}

	if(ctx->ip!=ctx->ips)
	{
		i=ctx->ip-ctx->ips;
		obj->decl->text=bgbcc_malloc(i);
		obj->decl->sz_text=i;
		memcpy(obj->decl->text, ctx->ips, i);
	
		BGBCC_FrBC_DbgDisAsmFunction(ctx, obj);
	}
}

void BGBCC_FrBC_End(BGBCC_FrCC_State *ctx)
{
	int sza[2], ala[2];
	BGBCC_FrCC_LiteralInfo *obj, *obj2;
	frbc_register reg;
	int msz, nsz, msz2, nsz2;
	int mal, nal, mal2, nal2;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_FUNCTION:
		BGBCC_FrBC_EmitCallRetDefault(ctx);
		BGBCC_FrBC_EndFunction(ctx, obj);
		break;
	case FR2C_LITID_VALUE:
		break;
	case FR2C_LITID_GLOBALVAR:
		if(!obj->decl)
			break;

		obj->decl->pbname=BGBCC_FrBC_GetObjQName(ctx, obj->parent);
		obj->decl->qname=BGBCC_FrBC_GetObjQName(ctx, obj);

		if(obj->decl->sig)
		{
			BGBCC_FrBC_TypeFromSig(ctx,
				&(obj->decl->type), obj->decl->sig);

			BGBCC_FrBC_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}
		if(obj->decl->value.val)
		{
			i=BGBCC_FrBC_GetRegImmLitIndex(ctx,
				obj->decl->value, obj->decl->type);
			obj->decl->validx=i;
		}else
		{
			obj->decl->validx=0;
		}

		if(BGBCC_FrBC_TypeArrayP(ctx, obj->decl->type) &&
			(obj->decl->validx>0))
		{
			i=BGBCC_FrBC_TypeGetArraySize(ctx, obj->decl->type);
			obj2=ctx->literals[obj->decl->validx>>2];
			if((i<=0) && (obj2->decl->n_listdata>0))
			{
				BGBCC_FrBC_TypeArrayAdjustSize(ctx, 
					obj->decl->type, &(obj->decl->type),
					obj2->decl->n_listdata);
				obj->decl->sig=
					BGBCC_FrBC_TypeGetSig(ctx, obj->decl->type);

				BGBCC_FrBC_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
				obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
				obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
			}
		}

		if(BGBCC_FrBC_TypeIsObjectP(ctx, obj->decl->type))
		{
			obj->decl->sig=	BGBCC_FrBC_TypeGetSig(ctx, obj->decl->type);
			BGBCC_FrBC_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}
		break;
	case FR2C_LITID_VAR:
		if(obj->decl && obj->decl->sig)
		{
			BGBCC_FrBC_TypeFromSig(ctx,
				&(obj->decl->type), obj->decl->sig);

			BGBCC_FrBC_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}
		if(obj->decl->value.val)
		{
			i=BGBCC_FrBC_GetRegImmLitIndex(ctx,
				obj->decl->value, obj->decl->type);
			obj->decl->validx=i;
		}

		if(BGBCC_FrBC_TypeArrayP(ctx, obj->decl->type) &&
			(obj->decl->validx>0))
		{
			i=BGBCC_FrBC_TypeGetArraySize(ctx, obj->decl->type);
			obj2=ctx->literals[obj->decl->validx>>2];
			if((i<=0) && (obj2->decl->n_listdata>0))
			{
				BGBCC_FrBC_TypeArrayAdjustSize(ctx, 
					obj->decl->type, &(obj->decl->type),
					obj2->decl->n_listdata);
				obj->decl->sig=
					BGBCC_FrBC_TypeGetSig(ctx, obj->decl->type);

				BGBCC_FrBC_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
				obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
				obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
			}
		}

		if(BGBCC_FrBC_TypeIsObjectP(ctx, obj->decl->type))
		{
			obj->decl->sig=	BGBCC_FrBC_TypeGetSig(ctx, obj->decl->type);

			BGBCC_FrBC_GetSigMinMaxSize(ctx, obj->decl->sig, sza, ala);
			obj->decl->fxmsize=sza[0];	obj->decl->fxnsize=sza[1];
			obj->decl->fxmalgn=ala[0];	obj->decl->fxnalgn=ala[1];
		}

#if 1
		if(obj->parent && (obj->parent->littype==FR2C_LITID_ARGS))
		{
			BGBCC_FrBC_AddFrameArg(ctx,
				obj->parent->parent->decl,
				obj->decl);
		}
#endif

#if 0
		if(obj->parent)
		{
			obj->littype=FR2C_LITID_VAR;
			switch(obj->parent->littype)
			{
			case FR2C_LITID_ARGS:
				BGBCC_FrBC_AddFrameArg(ctx,
					obj->parent->parent->decl,
					obj->decl);
				break;
			case FR2C_LITID_LOCALS:
				BGBCC_FrBC_AddFrameLocal(ctx,
					obj->parent->parent->decl,
					obj->decl);
				break;
			case FR2C_LITID_FUNCTION:
				obj->parent->decl->defv=obj->decl;
				break;
			case FR2C_LITID_STRUCT:
			case FR2C_LITID_UNION:
				BGBCC_FrBC_AddFrameField(ctx,
					obj->parent->decl,
					obj->decl);
				break;
			}
		}else
		{
			obj->littype=FR2C_LITID_GLOBALVAR;
			obj->decl->regtype=FR2C_LITID_GLOBALVAR;
			BGBCC_FrBC_AddGlobalDecl(ctx, obj->decl);
		}
#endif
		break;

	case FR2C_LITID_STRUCT:
	case FR2C_LITID_CLASS:
		msz=0; nsz=0; mal=1; nal=1;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxmsize;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			nal2=obj->decl->fields[i]->fxnalgn;
			msz=(msz+(mal2-1))&(~(mal2-1));
			nsz=(nsz+(nal2-1))&(~(nal2-1));
			msz+=msz2;	nsz+=nsz2;
			if(mal2>mal)mal=mal2;
			if(nal2>nal)nal=nal2;
		}
		msz=(msz+(mal-1))&(~(mal-1));
		nsz=(nsz+(nal-1))&(~(nal-1));
		obj->decl->fxmsize=msz;	obj->decl->fxnsize=nsz;
		obj->decl->fxmalgn=mal;	obj->decl->fxnalgn=nal;
		break;

	case FR2C_LITID_UNION:
		msz=0; nsz=0; mal=1; nal=1;
		for(i=0; i<obj->decl->n_fields; i++)
		{
			msz2=obj->decl->fields[i]->fxmsize;
			nsz2=obj->decl->fields[i]->fxnsize;
			mal2=obj->decl->fields[i]->fxmalgn;
			nal2=obj->decl->fields[i]->fxnalgn;
			if(msz2>msz)msz=msz2;
			if(nsz2>nsz)nsz=nsz2;
			if(mal2>mal)mal=mal2;
			if(nal2>nal)nal=nal2;
		}
		msz=(msz+(mal-1))&(~(mal-1));
		nsz=(nsz+(nal-1))&(~(nal-1));
		obj->decl->fxmsize=msz;	obj->decl->fxnsize=nsz;
		obj->decl->fxmalgn=mal;	obj->decl->fxnalgn=nal;
		break;

	case FR2C_LITID_LIST:
		if(obj->decl && obj->decl->sig)
		{
			BGBCC_FrBC_TypeFromSig(ctx,
				&(obj->decl->type), obj->decl->sig);
		}

		switch(obj->parent->littype)
		{
		case FR2C_LITID_LIST:
			BGBCC_FrBC_GetRegForLiteralValue(ctx, &reg, obj->litid);
			BGBCC_FrBC_ListAddLiteral(ctx, obj->parent, reg);
			break;
		case FR2C_LITID_VALUE:
			BGBCC_FrBC_GetRegForLiteralValue(ctx, &reg, obj->litid);
			obj->parent->parent->decl->value=reg;
			break;
		default:
			break;
		}
		break;
	case FR2C_LITID_ARGS:
		obj->parent->decl->n_eargs=
			obj->parent->decl->n_args;
		break;
	}

	BGBCC_FrBC_CheckFreeLiteral(ctx, obj);

	ctx->cur_objstackpos--;
	if(ctx->cur_objstackpos>0)
	{
		ctx->cur_obj=ctx->cur_objstack[ctx->cur_objstackpos-1];
	}else
	{
		ctx->cur_obj=NULL;
	}

//	BGBCC_FrBC_StubError(ctx);
}

void BGBCC_FrBC_AttribStr(BGBCC_FrCC_State *ctx, int attr, char *str)
{
	BGBCC_FrCC_LiteralInfo *obj;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_VAR:
	case FR2C_LITID_FUNCTION:
	case FR2C_LITID_GLOBALVAR:
	case FR2C_LITID_STRUCT:
	case FR2C_LITID_UNION:
	case FR2C_LITID_CLASS:
		switch(attr)
		{
		case FR2C_ATTR_NAME:
			obj->decl->name=bgbcc_strdup(str);
			if(!obj->name)obj->name=obj->decl->name;
			break;
		case FR2C_ATTR_SIG:
			obj->decl->sig=bgbcc_strdup(str);
			if(!obj->sig)obj->sig=obj->decl->sig;
			break;
		case FR2C_ATTR_FLAGS:
			if(str && (*str))
				{ obj->decl->flagstr=bgbcc_strdup(str); }
			else
				{ obj->decl->flagstr=NULL; }
			break;
		default:
			break;
		}
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_AttribInt(BGBCC_FrCC_State *ctx, int attr, int val)
{
	BGBCC_FrCC_LiteralInfo *obj;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_VAR:
	case FR2C_LITID_FUNCTION:
	case FR2C_LITID_GLOBALVAR:
	case FR2C_LITID_STRUCT:
	case FR2C_LITID_UNION:
	case FR2C_LITID_CLASS:
		switch(attr)
		{
		case FR2C_ATTR_FLAGS:
			obj->decl->flagsint=val;
			break;
		default:
			break;
		}
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_AttribLong(BGBCC_FrCC_State *ctx, int attr, s64 val)
{
	BGBCC_FrCC_LiteralInfo *obj;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_VAR:
	case FR2C_LITID_FUNCTION:
	case FR2C_LITID_GLOBALVAR:
	case FR2C_LITID_STRUCT:
	case FR2C_LITID_UNION:
	case FR2C_LITID_CLASS:
		switch(attr)
		{
		case FR2C_ATTR_FLAGS:
			obj->decl->flagsint=val;
			break;
		default:
			break;
		}
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}


void BGBCC_FrBC_Marker(BGBCC_FrCC_State *ctx, int tag)
{
	switch(tag)
	{
	case FR2C_CMD_VARARGS:
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
	}
}

void BGBCC_FrBC_ListAddLiteral(BGBCC_FrCC_State *ctx, 
	BGBCC_FrCC_LiteralInfo *list, frbc_register val)
{
	BGBCC_FrCC_RegisterInfo *def;
	BGBCC_FrCC_LiteralInfo *str;
	int i;

	def=list->decl;
	if(!def->listdata)
	{
		def->listdata=bgbcc_malloc(64*sizeof(frbc_register));
		def->n_listdata=0;
		def->m_listdata=64;
	}

	if((def->n_listdata+1)>=def->m_listdata)
	{
		i=def->m_listdata;
		i=i+(i>>1);
		def->listdata=bgbcc_realloc(def->listdata,
			i*sizeof(frbc_register));
		def->m_listdata=i;
	}

	i=def->n_listdata++;
	def->listdata[i]=val;

	if(def->sig && (def->sig[0]=='Y'))
	{
		str=BGBCC_FrBC_LookupStructureForSig2(ctx, def->sig);
		if(str && str->decl)
		{
			if(str->decl->fields[i])
				str->decl->fields[i]->regflags|=
					BGBCC_REGFL_INITIALIZED;
			str->decl->regflags|=BGBCC_REGFL_INITIALIZED;
		}
	}

//	BGBCC_FrBC_StubError(ctx);
}

void BGBCC_FrBC_LiteralInt(BGBCC_FrCC_State *ctx, int attr, s32 val)
{
	BGBCC_FrCC_LiteralInfo *obj;
	frbc_register reg;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_LIST:
		BGBCC_FrBC_GetRegForIntValue(ctx, &reg, val);
		BGBCC_FrBC_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case FR2C_LITID_VALUE:
		BGBCC_FrBC_GetRegForIntValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_LiteralLong(BGBCC_FrCC_State *ctx, int attr, s64 val)
{
	BGBCC_FrCC_LiteralInfo *obj;
	frbc_register reg;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_LIST:
		BGBCC_FrBC_GetRegForLongValue(ctx, &reg, val);
		BGBCC_FrBC_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case FR2C_LITID_VALUE:
		BGBCC_FrBC_GetRegForLongValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_LiteralFloat(BGBCC_FrCC_State *ctx, int attr, double val)
{
	BGBCC_FrCC_LiteralInfo *obj;
	frbc_register reg;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_LIST:
		BGBCC_FrBC_GetRegForFloatValue(ctx, &reg, val);
		BGBCC_FrBC_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case FR2C_LITID_VALUE:
		BGBCC_FrBC_GetRegForFloatValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_LiteralDouble(BGBCC_FrCC_State *ctx, int attr, double val)
{
	BGBCC_FrCC_LiteralInfo *obj;
	frbc_register reg;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_LIST:
		BGBCC_FrBC_GetRegForDoubleValue(ctx, &reg, val);
		BGBCC_FrBC_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case FR2C_LITID_VALUE:
		BGBCC_FrBC_GetRegForDoubleValue(ctx, &reg, val);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_LiteralStr(BGBCC_FrCC_State *ctx, int attr, char *str)
{
	BGBCC_FrCC_LiteralInfo *obj;
	frbc_register reg;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_LIST:
		BGBCC_FrBC_GetRegForStringValue(ctx, &reg, str);
		BGBCC_FrBC_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case FR2C_LITID_VALUE:
		BGBCC_FrBC_GetRegForStringValue(ctx, &reg, str);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_LiteralWStr(BGBCC_FrCC_State *ctx, int attr, char *str)
{
	BGBCC_FrCC_LiteralInfo *obj;
	frbc_register reg;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_LIST:
		BGBCC_FrBC_GetRegForWStringValue(ctx, &reg, str);
		BGBCC_FrBC_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case FR2C_LITID_VALUE:
		BGBCC_FrBC_GetRegForWStringValue(ctx, &reg, str);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

void BGBCC_FrBC_LiteralGlobalAddr(BGBCC_FrCC_State *ctx,
	int attr, s32 gblid)
{
	BGBCC_FrCC_LiteralInfo *obj;
	frbc_register reg;
	int i, j, k;

	obj=ctx->cur_obj;
	switch(obj->littype)
	{
	case FR2C_LITID_LIST:
		BGBCC_FrBC_GetRegForGlobalAddrValue(ctx, &reg, gblid);
		BGBCC_FrBC_ListAddLiteral(ctx, ctx->cur_obj, reg);
		break;
	case FR2C_LITID_VALUE:
		BGBCC_FrBC_GetRegForGlobalAddrValue(ctx, &reg, gblid);
		obj->parent->decl->value=reg;
		break;
	default:
		BGBCC_FrBC_StubError(ctx);
		break;
	}
}

byte *BGBCC_FrBC_EmitBufVLIE(byte *ct, s64 vli)
{
	if(vli<0)
	{
		*ct++=0x80|((-vli)&63);
		return(ct);
	}
	
	if(vli<0x80)
	{
		*ct++=vli;
	}else if(vli<0x2000)
	{
		*ct++=0xC0|((vli>>8)&31);
		*ct++=vli&255;
	}else if(vli<0x100000)
	{
		*ct++=0xE0|((vli>>16)&15);
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}else if(vli<0x8000000)
	{
		*ct++=0xF0|((vli>>24)&7);
		*ct++=(vli>>16)&255;
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}else if(vli<0x400000000)
	{
		*ct++=0xF8|((vli>>32)&3);
		*ct++=(vli>>24)&255;
		*ct++=(vli>>16)&255;
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}else if(vli<0x20000000000)
	{
		*ct++=0xFC|((vli>>40)&1);
		*ct++=(vli>>32)&255;
		*ct++=(vli>>24)&255;
		*ct++=(vli>>16)&255;
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}else if(vli<0x1000000000000)
	{
		*ct++=0xFE;
		*ct++=(vli>>40)&255;
		*ct++=(vli>>32)&255;
		*ct++=(vli>>24)&255;
		*ct++=(vli>>16)&255;
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}else if(vli<0x80000000000000)
	{
		*ct++=0xFF;
		*ct++=0x80|((vli>>48)&63);
		*ct++=(vli>>40)&255;
		*ct++=(vli>>32)&255;
		*ct++=(vli>>24)&255;
		*ct++=(vli>>16)&255;
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}else if(vli<0x4000000000000000)
	{
		*ct++=0xFF;
		*ct++=0x80|((vli>>56)&31);
		*ct++=(vli>>48)&255;
		*ct++=(vli>>40)&255;
		*ct++=(vli>>32)&255;
		*ct++=(vli>>24)&255;
		*ct++=(vli>>16)&255;
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}else
	{
		*ct++=0xFF;
		*ct++=0xC0;
		*ct++=(vli>>56)&255;
		*ct++=(vli>>48)&255;
		*ct++=(vli>>40)&255;
		*ct++=(vli>>32)&255;
		*ct++=(vli>>24)&255;
		*ct++=(vli>>16)&255;
		*ct++=(vli>> 8)&255;
		*ct++=(vli    )&255;
	}

	return(ct);
}

void BGBCC_FrBC_FlattenImage_EmitGlobalVarDecl(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_RegisterInfo *gbl)
{
	byte *ct, *ct1, *ctm0;
	int sz;
	int i, j, k;
	
	if(!obuf)
	{
		if(ctx->ctxflags&BGBCC_CTXFL_SAVENAMES)
			{ gbl->flagsint|=BGBCC_TYFL_EXPNAME; }

		if(!(gbl->flagsint&BGBCC_TYFL_DLLEXPORT) &&
			!(gbl->regflags&BGBCC_REGFL_ACCESSED))
		{
			gbl->regflags|=BGBCC_REGFL_CULL;
			return;
		}

		if(gbl->qname && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->qname); }
		else if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->name); }

		if(gbl->sig)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->sig); }
		if(gbl->flagstr)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->flagstr); }

		BGBCC_FrBC_MarkTypeAccessed(ctx, gbl->type);

		return;
	}

	ct=*rct;

	ctm0=ct;
//	*ct++=0xE4;		*ct++=0;
//	*ct++=0;		*ct++=0;
//	*ct++='D';		*ct++='V';
//	*ct++='A';		*ct++='R';

//	*ct++=0xE5;		*ct++=0;
//	*ct++='D';		*ct++='V';

	*ct++=FR2C_TAG_DEFVAR;		*ct++=0;
	
	if(gbl->qname && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->qname);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}else if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->name);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->sig)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->sig);
		if(i>0)
		{
			*ct++=FR2C_TAG_SIG;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->flagstr)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->flagstr);
		if(i>0)
		{
			*ct++=FR2C_TAG_FLAGS;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->validx>0)
	{
		*ct++=FR2C_TAG_VALUE;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, gbl->validx);
	}

	i=ct-ctm0;
	
	if(i>127)
	{
		printf("BGBCC_FrBC_FlattenImage_EmitGlobalVarDecl: "
			"Size Sanity Fail, %d>127\n", i);
		BGBCC_FrBC_TagError(ctx, FR2C_TERR_FORMAT_SANITY);
	}
	i-=2;
	
//	ctm0[1]=(i>>16)&255;
//	ctm0[2]=(i>> 8)&255;
//	ctm0[3]=(i    )&255;
	ctm0[1]=i&255;

	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitGlobalFuncDecl(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_RegisterInfo *gbl)
{
	byte *ct, *ct1, *ctm0;
	int sz;
	int i, j, k;

	if(!obuf)
	{
		if(ctx->ctxflags&BGBCC_CTXFL_SAVENAMES)
			{ gbl->flagsint|=BGBCC_TYFL_EXPNAME; }

		if(!(gbl->flagsint&BGBCC_TYFL_DLLEXPORT) &&
			!(gbl->regflags&BGBCC_REGFL_ACCESSED))
		{
			gbl->regflags|=BGBCC_REGFL_CULL;
			return;
		}
		
		if(gbl->sz_text<=0)
		{
			gbl->flagsint|=BGBCC_TYFL_EXPNAME;
		}

		if(gbl->qname && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->qname); }
		else if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->name); }

		if(gbl->sig)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->sig); }
		if(gbl->flagstr)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->flagstr); }

		for(i=0; i<gbl->n_args; i++)
			{ BGBCC_FrBC_MarkTypeAccessed(ctx, gbl->args[i]->type); }
		for(i=0; i<gbl->n_locals; i++)
			{ BGBCC_FrBC_MarkTypeAccessed(ctx, gbl->locals[i]->type); }

		return;
	}

	sz=64;
	if(gbl->text)
		{ sz+=gbl->sz_text; }

	ct=*rct;

	ctm0=ct;
//	if(sz>=256)
	if(sz>=128)
	{
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='F';		*ct++='U';
		*ct++='N';		*ct++='C';
	}else
	{
//		*ct++=0xE5;		*ct++=0;
//		*ct++='F';		*ct++='N';

		*ct++=FR2C_TAG_FUNC;		*ct++=0;
	}
	
	if(gbl->qname && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->qname);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}else if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->name);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->sig)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->sig);
		if(i>0)
		{
			*ct++=FR2C_TAG_SIG;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->flagstr)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->flagstr);
		if(i>0)
		{
			*ct++=FR2C_TAG_FLAGS;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}
	
	if(gbl->n_args>0)
	{
		i=gbl->n_args;
		*ct++=FR2C_TAG_NARGS;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
	}

	if(gbl->n_locals>0)
	{
		i=gbl->n_locals;
		*ct++=FR2C_TAG_NLOCALS;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
	}

	if(gbl->n_regs>0)
	{
		i=gbl->n_regs;
		*ct++=FR2C_TAG_NREGS;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
	}

	if(gbl->n_cargs>0)
	{
		i=gbl->n_cargs;
		*ct++=FR2C_TAG_NCARGS;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
	}
	
	if(gbl->text && (gbl->sz_text>0))
	{
		if(gbl->sz_text>=248)
		{
			i=gbl->sz_text+4; ct1=ct+i;
			*ct++=0xE1;			*ct++=(i>>16)&255;
			*ct++=(i>>8)&255;	*ct++=(i    )&255;
			memcpy(ct, gbl->text, gbl->sz_text);
			ct=ct1;
		}else
		{
			i=gbl->sz_text+2; ct1=ct+i;
			*ct++=0xE2;			*ct++=i&255;
			memcpy(ct, gbl->text, gbl->sz_text);
			ct=ct1;
		}
	}
	
//	if(gbl->validx>0)
//	{
//		i=gbl->validx;
//		*ct++=FR2C_TAG_VALUE;
//		ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
//	}

//	if(sz>=256)
	if(sz>=128)
	{
		i=ct-ctm0;
		if((i<0) || (i>=16777216))
			{ BGBCC_FrBC_TagError(ctx, FR2C_TERR_FORMAT_SANITY); }
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;
	}else
	{
		i=ct-ctm0;
		i-=2;
//		if((i<0) || (i>=256))
		if((i<0) || (i>=128))
			{ BGBCC_FrBC_TagError(ctx, FR2C_TERR_FORMAT_SANITY); }
		ctm0[1]=i&255;
	}

	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitGlobalNullDecl(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_RegisterInfo *gbl)
{
	byte *ct;
	
	if(!obuf)
		return;
	
	ct=*rct;
	*ct++=0xE4;		*ct++=0;
	*ct++=0;		*ct++=8;
	*ct++='N';		*ct++='U';
	*ct++='L';		*ct++='L';
	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitLiteralNullDecl(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_LiteralInfo *lit)
{
	byte *ct;	

	if(!obuf)
		return;

	ct=*rct;
	*ct++=0xE4;		*ct++=0;
	*ct++=0;		*ct++=8;
	*ct++='N';		*ct++='U';
	*ct++='L';		*ct++='L';
	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitGlobal(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, int gidx)
{
	BGBCC_FrCC_RegisterInfo *gbl;
	
	gbl=ctx->reg_globals[gidx];
	if(!gbl)
	{
		BGBCC_FrBC_FlattenImage_EmitGlobalNullDecl(
			ctx, obuf, omsz, rct, gbl);
		return;
	}
	
	switch(gbl->regtype)
	{
	case FR2C_LITID_GLOBALVAR:
	case FR2C_LITID_STATICVAR:
		BGBCC_FrBC_FlattenImage_EmitGlobalVarDecl(
			ctx, obuf, omsz, rct, gbl);
		break;
	case FR2C_LITID_FUNCTION:
		BGBCC_FrBC_FlattenImage_EmitGlobalFuncDecl(
			ctx, obuf, omsz, rct, gbl);
		break;
	default:
		printf("BGBCC_FrBC_FlattenImage_EmitGlobal: Unknown %d %d\n",
			gidx, gbl->regtype);
		BGBCC_FrBC_FlattenImage_EmitGlobalNullDecl(
			ctx, obuf, omsz, rct, gbl);
		break;
	}
}

void BGBCC_FrBC_FlattenImage_EmitStructDefField(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_RegisterInfo *gbl)
{
	byte *ct, *ct1, *ctm0;
	int sza[2], ala[2];
	int sz, svn;
	int i, j, k;
	
	if(!obuf)
	{
		if(ctx->ctxflags&BGBCC_CTXFL_SAVENAMES)
			{ gbl->flagsint|=BGBCC_TYFL_EXPNAME; }

		if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->name); }
		if(gbl->sig)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->sig); }
		if(gbl->flagstr)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->flagstr); }

		if(gbl->regflags&BGBCC_REGFL_INITIALIZED)
		{
			BGBCC_FrBC_GetSigFixedSize(ctx, gbl->sig, &k);
			gbl->fxsize=k;
		}else
		{
			gbl->fxsize=0;
		}

#if 0
		BGBCC_FrBC_GetSigMinMaxSize(ctx, gbl->sig, sza, ala);
		gbl->fxmsize=sza[0];
		gbl->fxnsize=sza[1];
		gbl->fxmalgn=ala[0];
		gbl->fxnalgn=ala[1];
#endif

		return;
	}

	ct=*rct;

	ctm0=ct;
//	*ct++=0xE5;		*ct++=0;
//	*ct++='D';		*ct++='F';

	*ct++=FR2C_TAG_DEFFIELD;		*ct++=0;
	
	if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->name);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->sig)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->sig);
		if(i>0)
		{
			*ct++=FR2C_TAG_SIG;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->flagstr)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->flagstr);
		if(i>0)
		{
			*ct++=FR2C_TAG_FLAGS;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->validx>0)
	{
		*ct++=FR2C_TAG_VALUE;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, gbl->validx);
	}

	BGBCC_FrBC_GetSigFixedSize(ctx, gbl->sig, &k);
	if(gbl->fxsize!=k)
	{
		*ct++=FR2C_TAG_FXSIZE;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, gbl->fxsize);
	}

	i=ct-ctm0;
	
	if(i>127)
	{
		printf("BGBCC_FrBC_FlattenImage_EmitStructDefField: "
			"Size Sanity Fail, %d>127\n", i);
		BGBCC_FrBC_TagError(ctx, FR2C_TERR_FORMAT_SANITY);
	}
	i-=2;

	ctm0[1]=i&255;

	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitStructDef(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_LiteralInfo *obj)
{
	BGBCC_FrCC_RegisterInfo *gbl;
	byte *ct, *ct1, *ctm0;
	int sz;
	int i, j, k;

	gbl=obj->decl;

	if(!gbl)
	{
		if(!obuf)
			return;

		ct=*rct;
		*ct++=0xE5;		*ct++=4;
		*ct++='S';		*ct++='T';
		*rct=ct;
		return;

//		obj->decl=bgbcc_malloc(sizeof(BGBCC_FrCC_RegisterInfo));
//		gbl=obj->decl;
	}

	if(!obuf)
	{
		if(ctx->ctxflags&BGBCC_CTXFL_SAVENAMES)
			{ gbl->flagsint|=BGBCC_TYFL_EXPNAME; }

		if(!(gbl->flagsint&BGBCC_TYFL_DLLEXPORT) &&
			!(gbl->regflags&BGBCC_REGFL_ACCESSED))
		{
			gbl->regflags|=BGBCC_REGFL_CULL;
			return;
		}
		
		if(gbl->qname && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->qname); }
		else if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->name); }

		if(gbl->sig)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->sig); }
		if(gbl->flagstr)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->flagstr); }

		if(gbl->regflags&BGBCC_REGFL_INITIALIZED)
		{
			k=0;
			for(i=0; i<gbl->n_fields; i++)
			{
				BGBCC_FrBC_FlattenImage_EmitStructDefField(
					ctx, obuf, omsz, rct, gbl->fields[i]);

				j=gbl->fields[i]->fxsize;
				if(j>0)k+=j;
			}
			gbl->fxsize=k;
		}else
		{
			for(i=0; i<gbl->n_fields; i++)
			{
				BGBCC_FrBC_FlattenImage_EmitStructDefField(
					ctx, obuf, omsz, rct, gbl->fields[i]);
			}
			gbl->fxsize=0;
		}

		return;
	}

	ct=*rct;

	ctm0=ct;
	*ct++=0xE4;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='D';		*ct++='S';
	*ct++='T';		*ct++='R';
	
	if(obj->littype==FR2C_LITID_UNION)
	{
		ct[-1]='U';
	}

	if(gbl->qname && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->qname);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}else if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->name);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->sig)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->sig);
		if(i>0)
		{
			*ct++=FR2C_TAG_SIG;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl->flagstr)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->flagstr);
		if(i>0)
		{
			*ct++=FR2C_TAG_FLAGS;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(!gbl->fxsize)
	{
		*ct++=FR2C_TAG_FXSIZE;
		ct=BGBCC_FrBC_EmitBufVLIE(ct, gbl->fxsize);
	}

	for(i=0; i<gbl->n_fields; i++)
	{
		BGBCC_FrBC_FlattenImage_EmitStructDefField(
			ctx, obuf, omsz, &ct, gbl->fields[i]);
	}
	
	i=ct-ctm0;
	ctm0[1]=(i>>16)&255;
	ctm0[2]=(i>> 8)&255;
	ctm0[3]=(i    )&255;

	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitTypeDef(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_LiteralInfo *obj)
{
	BGBCC_FrCC_RegisterInfo *gbl;
	byte *ct, *ct1, *ctm0;
	int sz;
	int i, j, k;

	gbl=obj->decl;

//	if(!gbl)
//	{
//		if(!obuf)
//			return;
//	}

	if(!obuf)
	{
//		if(ctx->ctxflags&BGBCC_CTXFL_SAVENAMES)
//			{ gbl->flagsint|=BGBCC_TYFL_EXPNAME; }

//		if(obj->name)
//		if(obj->qname && (ctx->ctxflags&BGBCC_CTXFL_SAVENAMES))
//			{ BGBCC_FrBC_IndexCountString(ctx, obj->qname); }
//		else
		if(obj->name && (ctx->ctxflags&BGBCC_CTXFL_SAVENAMES))
			{ BGBCC_FrBC_IndexCountString(ctx, obj->name); }
		if(obj->sig)
			{ BGBCC_FrBC_IndexCountString(ctx, obj->sig); }

		if(gbl && gbl->flagstr)
			{ BGBCC_FrBC_IndexCountString(ctx, gbl->flagstr); }

		if(gbl)
		{
			for(i=0; i<gbl->n_fields; i++)
			{
				BGBCC_FrBC_FlattenImage_EmitStructDefField(
					ctx, obuf, omsz, rct, gbl->fields[i]);
			}
		}

		return;
	}

	ct=*rct;

	ctm0=ct;
	*ct++=0xE4;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='D';		*ct++='T';
	*ct++='Y';		*ct++='D';
	
//	if(gbl->name && (gbl->flagsint&BGBCC_TYFL_EXPNAME))
//	if(cur->name)
//	if(obj->qname && (ctx->ctxflags&BGBCC_CTXFL_SAVENAMES))
//	{
//		i=BGBCC_FrBC_LookupString(ctx, obj->qname);
//		if(i>0)
//		{
//			*ct++=FR2C_TAG_NAME;
//			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
//		}
//	}else
	if(obj->name && (ctx->ctxflags&BGBCC_CTXFL_SAVENAMES))
	{
		i=BGBCC_FrBC_LookupString(ctx, obj->name);
		if(i>0)
		{
			*ct++=FR2C_TAG_NAME;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(obj->sig)
	{
		i=BGBCC_FrBC_LookupString(ctx, obj->sig);
		if(i>0)
		{
			*ct++=FR2C_TAG_SIG;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}

	if(gbl && gbl->flagstr)
	{
		i=BGBCC_FrBC_LookupString(ctx, gbl->flagstr);
		if(i>0)
		{
			*ct++=FR2C_TAG_FLAGS;
			ct=BGBCC_FrBC_EmitBufVLIE(ct, i);
		}
	}
	
	if(gbl)
	{
		for(i=0; i<gbl->n_fields; i++)
		{
			BGBCC_FrBC_FlattenImage_EmitStructDefField(
				ctx, obuf, omsz, &ct, gbl->fields[i]);
		}
	}
	
	i=ct-ctm0;
	ctm0[1]=(i>>16)&255;
	ctm0[2]=(i>> 8)&255;
	ctm0[3]=(i    )&255;

	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitFixedFieldData(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_LiteralInfo *str,
	BGBCC_FrCC_RegisterInfo *gbl,
	frbc_register val)
{
	char *sig;
	byte *ct;
	s64 li;
	int i, j, k;

	if(!gbl->fxsize)
		return;

	ct=*rct;
	if(!val.val)
	{
		memset(ct, 0, gbl->fxsize);
		ct+=gbl->fxsize;
		*rct=ct;
		return;
	}
	
	sig=gbl->sig;

	if((*sig=='c') || (*sig=='h') || (*sig=='a') || (*sig=='b'))
	{
		j=BGBCC_FrBC_GetRegImmIntValue(ctx, val);
		*ct=j;
	}

	if((*sig=='s') || (*sig=='t') || (*sig=='w') || (*sig=='k'))
	{
		j=BGBCC_FrBC_GetRegImmIntValue(ctx, val);
		ct[0]=(j>>8)&255;
		ct[1]=(j   )&255;
	}

	if((*sig=='i') || (*sig=='j'))
	{
		j=BGBCC_FrBC_GetRegImmIntValue(ctx, val);
		ct[0]=(j>>24)&255;
		ct[1]=(j>>16)&255;
		ct[2]=(j>> 8)&255;
		ct[3]=(j    )&255;
	}

	if((*sig=='l') || (*sig=='m') || (*sig=='x') || (*sig=='y'))
	{
		li=BGBCC_FrBC_GetRegImmLongValue(ctx, val);
		ct[0]=(li>>56)&255;
		ct[1]=(li>>48)&255;
		ct[2]=(li>>40)&255;
		ct[3]=(li>>32)&255;
		ct[4]=(li>>24)&255;
		ct[5]=(li>>16)&255;
		ct[6]=(li>> 8)&255;
		ct[7]=(li    )&255;
	}

	if(*sig=='f')
	{
		*(f32 *)(&j)=BGBCC_FrBC_GetRegImmFloatValue(
			ctx, val);
		ct[0]=(j>>24)&255;
		ct[1]=(j>>16)&255;
		ct[2]=(j>> 8)&255;
		ct[3]=(j    )&255;
	}

	if(*sig=='d')
	{
		*(f64 *)(&li)=BGBCC_FrBC_GetRegImmDoubleValue(
			ctx, val);
		ct[0]=(li>>56)&255;
		ct[1]=(li>>48)&255;
		ct[2]=(li>>40)&255;
		ct[3]=(li>>32)&255;
		ct[4]=(li>>24)&255;
		ct[5]=(li>>16)&255;
		ct[6]=(li>> 8)&255;
		ct[7]=(li    )&255;
	}

	if(*sig=='P')
	{
		j=BGBCC_FrBC_GetRegImmLitIndex(ctx,
			val, gbl->type);
		ct[0]=(j>>24)&255;
		ct[1]=(j>>16)&255;
		ct[2]=(j>> 8)&255;
		ct[3]=(j    )&255;
	}

	ct+=gbl->fxsize;
	*rct=ct;
	return;
}

void BGBCC_FrBC_FlattenImage_EmitFixedStructData(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz, byte **rct,
	BGBCC_FrCC_LiteralInfo *str, BGBCC_FrCC_LiteralInfo *lst)
{
	byte *ct, *ct1, *ctm0;
	char *sig;
	s64 li;
	int sz;
	int i, j, k;

//	gbl=str->decl;

	ct=*rct;
	for(i=0; i<str->decl->n_fields; i++)
	{
		BGBCC_FrBC_FlattenImage_EmitFixedFieldData(
			ctx, obuf, omsz, &ct,
			str, str->decl->fields[i], lst->decl->listdata[i]);
	}
	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_EmitListDef(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, BGBCC_FrCC_LiteralInfo *obj)
{
	BGBCC_FrCC_RegisterInfo *gbl;
	BGBCC_FrCC_LiteralInfo *str, *lst;
	byte *ct, *ct1, *ctm0;
	char *sig;
	s64 li;
	int sz, n;
	int i, j, k;

	gbl=obj->decl;

	if(!obuf)
	{
//		if(ctx->ctxflags&BGBCC_CTXFL_SAVENAMES)
//			{ gbl->flagsint|=BGBCC_TYFL_EXPNAME; }

		return;
	}

	sig=gbl->sig;

	if((*sig=='c') || (*sig=='h') || (*sig=='a') || (*sig=='b'))
	{
		ct=*rct;

		n=gbl->n_listdata;
		if(n<252)
		{
			ctm0=ct;
			*ct++=0xE5;		*ct++=(n*1)+4;
			*ct++='A';		*ct++='1';
	
			for(i=0; i<n; i++)
			{
				j=BGBCC_FrBC_GetRegImmIntValue(ctx, gbl->listdata[i]);
				*ct++=j;
			}

			*rct=ct;
			return;
		}

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='R';
		*ct++='R';		*ct++='1';
	
		for(i=0; i<gbl->n_listdata; i++)
		{
			j=BGBCC_FrBC_GetRegImmIntValue(ctx, gbl->listdata[i]);
			*ct++=j;
		}
	
		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	if((*sig=='s') || (*sig=='t') || (*sig=='w') || (*sig=='k'))
	{
		ct=*rct;

		n=gbl->n_listdata;
		if(n<126)
		{
			ctm0=ct;
			*ct++=0xE5;		*ct++=(n*2)+4;
			*ct++='A';		*ct++='2';
	
			for(i=0; i<n; i++)
			{
				j=BGBCC_FrBC_GetRegImmIntValue(ctx, gbl->listdata[i]);
				*ct++=(j>>8)&255;
				*ct++=(j   )&255;
			}

			*rct=ct;
			return;
		}

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='R';
		*ct++='R';		*ct++='2';
	
		for(i=0; i<gbl->n_listdata; i++)
		{
			j=BGBCC_FrBC_GetRegImmIntValue(ctx, gbl->listdata[i]);
			*ct++=(j>>8)&255;
			*ct++=(j   )&255;
		}
	
		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	if((*sig=='i') || (*sig=='j'))
	{
		ct=*rct;

		n=gbl->n_listdata;
		if(n<63)
		{
			ctm0=ct;
			*ct++=0xE5;		*ct++=(n*4)+4;
			*ct++='A';		*ct++='4';
	
			for(i=0; i<n; i++)
			{
				j=BGBCC_FrBC_GetRegImmIntValue(ctx, gbl->listdata[i]);
				*ct++=(j>>24)&255;
				*ct++=(j>>16)&255;
				*ct++=(j>> 8)&255;
				*ct++=(j    )&255;
			}

			*rct=ct;
			return;
		}

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='R';
		*ct++='R';		*ct++='4';
	
		for(i=0; i<gbl->n_listdata; i++)
		{
			j=BGBCC_FrBC_GetRegImmIntValue(ctx, gbl->listdata[i]);
			*ct++=(j>>24)&255;
			*ct++=(j>>16)&255;
			*ct++=(j>> 8)&255;
			*ct++=(j    )&255;
		}
	
		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	if((*sig=='l') || (*sig=='m') || (*sig=='x') || (*sig=='y'))
	{
		ct=*rct;

		n=gbl->n_listdata;
		if(n<31)
		{
			ctm0=ct;
			*ct++=0xE5;		*ct++=(n*8)+4;
			*ct++='A';		*ct++='8';
	
			for(i=0; i<n; i++)
			{
				li=BGBCC_FrBC_GetRegImmLongValue(ctx, gbl->listdata[i]);
				*ct++=(li>>56)&255;
				*ct++=(li>>48)&255;
				*ct++=(li>>40)&255;
				*ct++=(li>>32)&255;
				*ct++=(li>>24)&255;
				*ct++=(li>>16)&255;
				*ct++=(li>> 8)&255;
				*ct++=(li    )&255;
			}

			*rct=ct;
			return;
		}

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='R';
		*ct++='R';		*ct++='8';
	
		for(i=0; i<gbl->n_listdata; i++)
		{
			li=BGBCC_FrBC_GetRegImmLongValue(ctx, gbl->listdata[i]);
			*ct++=(li>>56)&255;
			*ct++=(li>>48)&255;
			*ct++=(li>>40)&255;
			*ct++=(li>>32)&255;
			*ct++=(li>>24)&255;
			*ct++=(li>>16)&255;
			*ct++=(li>> 8)&255;
			*ct++=(li    )&255;
		}
	
		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	if(*sig=='f')
	{
		ct=*rct;

		n=gbl->n_listdata;
		if(n<63)
		{
			ctm0=ct;
			*ct++=0xE5;		*ct++=(n*4)+4;
			*ct++='A';		*ct++='4';
	
			for(i=0; i<n; i++)
			{
				*(f32 *)(&j)=BGBCC_FrBC_GetRegImmFloatValue(
					ctx, gbl->listdata[i]);
				*ct++=(j>>24)&255;
				*ct++=(j>>16)&255;
				*ct++=(j>> 8)&255;
				*ct++=(j    )&255;
			}

			*rct=ct;
			return;
		}

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='R';
		*ct++='R';		*ct++='4';
	
		for(i=0; i<gbl->n_listdata; i++)
		{
			*(f32 *)(&j)=BGBCC_FrBC_GetRegImmFloatValue(
				ctx, gbl->listdata[i]);
			*ct++=(j>>24)&255;
			*ct++=(j>>16)&255;
			*ct++=(j>> 8)&255;
			*ct++=(j    )&255;
		}
	
		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	if(*sig=='d')
	{
		ct=*rct;

		n=gbl->n_listdata;
		if(n<31)
		{
			ctm0=ct;
			*ct++=0xE5;		*ct++=(n*8)+4;
			*ct++='A';		*ct++='8';
	
			for(i=0; i<n; i++)
			{
				*(f64 *)(&li)=BGBCC_FrBC_GetRegImmDoubleValue(
					ctx, gbl->listdata[i]);
				*ct++=(li>>56)&255;
				*ct++=(li>>48)&255;
				*ct++=(li>>40)&255;
				*ct++=(li>>32)&255;
				*ct++=(li>>24)&255;
				*ct++=(li>>16)&255;
				*ct++=(li>> 8)&255;
				*ct++=(li    )&255;
			}

			*rct=ct;
			return;
		}

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='R';
		*ct++='R';		*ct++='8';
	
		for(i=0; i<gbl->n_listdata; i++)
		{
			*(f64 *)(&li)=BGBCC_FrBC_GetRegImmDoubleValue(
				ctx, gbl->listdata[i]);
			*ct++=(li>>56)&255;
			*ct++=(li>>48)&255;
			*ct++=(li>>40)&255;
			*ct++=(li>>32)&255;
			*ct++=(li>>24)&255;
			*ct++=(li>>16)&255;
			*ct++=(li>> 8)&255;
			*ct++=(li    )&255;
		}
	
		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	if((*sig=='P') || (*sig=='A'))
	{
		ct=*rct;

		n=gbl->n_listdata;
		if(n<63)
		{
			ctm0=ct;
			*ct++=0xE5;		*ct++=(n*4)+4;
			*ct++='A';		*ct++='P';
	
			for(i=0; i<n; i++)
			{
				j=BGBCC_FrBC_GetRegImmLitIndex(ctx,
					gbl->listdata[i], gbl->type);
				*ct++=(j>>24)&255;
				*ct++=(j>>16)&255;
				*ct++=(j>> 8)&255;
				*ct++=(j    )&255;
			}

			*rct=ct;
			return;
		}

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='R';
		*ct++='R';		*ct++='P';
	
		for(i=0; i<gbl->n_listdata; i++)
		{
			j=BGBCC_FrBC_GetRegImmLitIndex(ctx,
				gbl->listdata[i], gbl->type);
			*ct++=(j>>24)&255;
			*ct++=(j>>16)&255;
			*ct++=(j>> 8)&255;
			*ct++=(j    )&255;
		}
	
		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	//literal struct
	if(*sig=='Y')
	{
		ct=*rct;

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='S';
		*ct++='T';		*ct++='D';
		
		str=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
		j=str->litid;

		*ct++=(j>>24)&255;
		*ct++=(j>>16)&255;
		*ct++=(j>> 8)&255;
		*ct++=(j    )&255;

		for(i=0; i<str->decl->n_fields; i++)
		{
			BGBCC_FrBC_FlattenImage_EmitFixedFieldData(
				ctx, obuf, omsz, &ct,
				str, str->decl->fields[i], gbl->listdata[i]);
		}

		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	//literal struct
	if(*sig=='X')
	{
		ct=*rct;

		ctm0=ct;
		*ct++=0xE4;		*ct++=0;
		*ct++=0;		*ct++=0;
		*ct++='A';		*ct++='S';
		*ct++='T';		*ct++='D';
		
		str=BGBCC_FrBC_LookupStructureForSig(ctx, sig);
		j=str->litid;

		*ct++=(j>>24)&255;
		*ct++=(j>>16)&255;
		*ct++=(j>> 8)&255;
		*ct++=(j    )&255;

		for(i=0; i<gbl->n_listdata; i++)
		{
			j=(gbl->listdata[i].val)&FR2C_REGID_REGMASK;
			lst=ctx->literals[j];
			BGBCC_FrBC_FlattenImage_EmitFixedStructData(
				ctx, obuf, omsz, &ct,
				str, lst);
		}

		i=ct-ctm0;
		ctm0[1]=(i>>16)&255;
		ctm0[2]=(i>> 8)&255;
		ctm0[3]=(i    )&255;

		*rct=ct;
		return;
	}

	printf("BGBCC_FrBC_FlattenImage_EmitListDef: "
		"Unhandled Sig '%s'\n", sig);
	BGBCC_FrBC_TagError(ctx, FR2C_TERR_FORMAT_SANITY);
}

void BGBCC_FrBC_FlattenImage_EmitLiteral(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz,
	byte **rct, int idx)
{
	BGBCC_FrCC_LiteralInfo *lit;
	
	lit=ctx->literals[idx];
	if(!lit)
	{
		BGBCC_FrBC_FlattenImage_EmitLiteralNullDecl(
			ctx, obuf, omsz, rct, lit);
		return;
	}
	
	switch(lit->littype)
	{
//	case FR2C_LITID_GLOBALVAR:
//		BGBCC_FrBC_FlattenImage_EmitGlobalVarDecl(
//			ctx, obuf, omsz, rct, gbl);
//		break;
//	case FR2C_LITID_FUNCTION:
//		BGBCC_FrBC_FlattenImage_EmitGlobalFuncDecl(
//			ctx, obuf, omsz, rct, gbl);
//		break;

	case FR2C_LITID_STRUCT:
	case FR2C_LITID_UNION:
		BGBCC_FrBC_FlattenImage_EmitStructDef(
			ctx, obuf, omsz, rct, lit);
		break;
	case FR2C_LITID_TYPEDEF:
		BGBCC_FrBC_FlattenImage_EmitTypeDef(
			ctx, obuf, omsz, rct, lit);
		break;
	case FR2C_LITID_LIST:
		BGBCC_FrBC_FlattenImage_EmitListDef(
			ctx, obuf, omsz, rct, lit);
		break;
	case FR2C_LITID_FUNCTION:
		BGBCC_FrBC_FlattenImage_EmitGlobalFuncDecl(
			ctx, obuf, omsz, rct, lit->decl);
		break;
	default:
		printf("BGBCC_FrBC_FlattenImage_EmitLiteral: Unknown %d %d\n",
			idx, lit->littype);
		BGBCC_FrBC_FlattenImage_EmitLiteralNullDecl(
			ctx, obuf, omsz, rct, lit);
		break;
	}
}

void BGBCC_FrBC_FlattenImage_Globals(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz, byte **rct)
{
	BGBCC_FrCC_RegisterInfo *gbl;
	byte *ct, *ct1, *ctm0;
	int sz;
	int i, j, k;
	
	if(ctx->n_reg_globals<=0)
		return;
	
	ct=*rct;

	ctm0=ct;
	*ct++=0xE4;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='G';		*ct++='D';
	*ct++='E';		*ct++='F';
	
	for(i=0; i<ctx->n_reg_globals; i++)
	{
		gbl=ctx->reg_globals[i];
		if(gbl && gbl->regflags&BGBCC_REGFL_CULL)
			{ ctx->idx_globals[i]=0; continue; }
	
		ctx->idx_globals[i]=ct-(ctm0+8);
		BGBCC_FrBC_FlattenImage_EmitGlobal(ctx, obuf, omsz, &ct, i);
	}

	i=ct-ctm0;
	ctm0[1]=(i>>16)&255;
	ctm0[2]=(i>> 8)&255;
	ctm0[3]=(i    )&255;
	sz=i;

	if(sz>=16777216)
	{
		i=ctx->n_reg_globals*4+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='G';	*ct++='I';	*ct++='D';	*ct++='X';
		
		for(i=0; i<ctx->n_reg_globals; i++)
		{
			ct[i*4+0]=(ctx->idx_globals[i]>>24)&255;
			ct[i*4+1]=(ctx->idx_globals[i]>>16)&255;
			ct[i*4+2]=(ctx->idx_globals[i]>> 8)&255;
			ct[i*4+3]=(ctx->idx_globals[i]    )&255;
		}
		ct=ct1;
	}else if(sz>=65536)
	{
		i=ctx->n_reg_globals*3+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='G';	*ct++='I';	*ct++='X';	*ct++='3';
		
		for(i=0; i<ctx->n_reg_globals; i++)
		{
			ct[i*3+0]=(ctx->idx_globals[i]>>16)&255;
			ct[i*3+1]=(ctx->idx_globals[i]>> 8)&255;
			ct[i*3+2]=(ctx->idx_globals[i]    )&255;
		}
		ct=ct1;
	}else
	{
		i=ctx->n_reg_globals*2+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='G';	*ct++='I';	*ct++='X';	*ct++='2';
		
		for(i=0; i<ctx->n_reg_globals; i++)
		{
			ct[i*2+0]=(ctx->idx_globals[i]>> 8)&255;
			ct[i*2+1]=(ctx->idx_globals[i]    )&255;
		}
		ct=ct1;
	}

	*rct=ct;
}

void BGBCC_FrBC_FlattenImage_Literals(
	BGBCC_FrCC_State *ctx, byte *obuf, int omsz, byte **rct)
{
	BGBCC_FrCC_LiteralInfo *lit;
	byte *ct, *ct1, *ctm0;
	int sz;
	int i, j, k;
	
	if(ctx->n_reg_globals<=0)
		return;
	
	ct=*rct;

	ctm0=ct;
	*ct++=0xE4;		*ct++=0;
	*ct++=0;		*ct++=0;
	*ct++='L';		*ct++='D';
	*ct++='E';		*ct++='F';
	
	for(i=0; i<ctx->n_literals; i++)
	{
		lit=ctx->literals[i];

		if(lit && lit->decl && lit->decl->regflags&BGBCC_REGFL_CULL)
			{ ctx->idx_literals[i]=0; continue; }

		ctx->idx_literals[i]=ct-(ctm0+8);
		BGBCC_FrBC_FlattenImage_EmitLiteral(ctx, obuf, omsz, &ct, i);
	}

	i=ct-ctm0;
	ctm0[1]=(i>>16)&255;
	ctm0[2]=(i>> 8)&255;
	ctm0[3]=(i    )&255;
	sz=i;

	if(sz>=16777216)
	{
		i=ctx->n_literals*4+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='L';	*ct++='I';	*ct++='D';	*ct++='X';
		
		for(i=0; i<ctx->n_literals; i++)
		{
			ct[i*4+0]=(ctx->idx_literals[i]>>24)&255;
			ct[i*4+1]=(ctx->idx_literals[i]>>16)&255;
			ct[i*4+2]=(ctx->idx_literals[i]>> 8)&255;
			ct[i*4+3]=(ctx->idx_literals[i]    )&255;
		}
		ct=ct1;
	}else if(sz>=65536)
	{
		i=ctx->n_literals*3+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='L';	*ct++='I';	*ct++='X';	*ct++='3';
		
		for(i=0; i<ctx->n_literals; i++)
		{
			ct[i*3+0]=(ctx->idx_literals[i]>>16)&255;
			ct[i*3+1]=(ctx->idx_literals[i]>> 8)&255;
			ct[i*3+2]=(ctx->idx_literals[i]    )&255;
		}
		ct=ct1;
	}else
	{
		i=ctx->n_literals*2+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='L';	*ct++='I';	*ct++='X';	*ct++='2';
		
		for(i=0; i<ctx->n_literals; i++)
		{
			ct[i*2+0]=(ctx->idx_literals[i]>>8)&255;
			ct[i*2+1]=(ctx->idx_literals[i]   )&255;
		}
		ct=ct1;
	}

	*rct=ct;
}

void BGBCC_FrBC_PreFlattenImage_Globals(BGBCC_FrCC_State *ctx)
{
	int i;

	for(i=0; i<ctx->n_reg_globals; i++)
	{
		BGBCC_FrBC_FlattenImage_EmitGlobal(ctx, NULL, 0, NULL, i);
	}
}

void BGBCC_FrBC_PreFlattenImage_Literals(BGBCC_FrCC_State *ctx)
{
	int i;

	for(i=0; i<ctx->n_literals; i++)
	{
		BGBCC_FrBC_FlattenImage_EmitLiteral(ctx, NULL, 0, NULL, i);
	}
}

int BGBCC_FrBC_FlattenImage(BGBCC_FrCC_State *ctx, byte *obuf, int omsz)
{
	byte *ct, *ct1;
	int sz;
	int i, j, k;
	
	BGBCC_FrBC_PreFlattenImage_Globals(ctx);
	BGBCC_FrBC_PreFlattenImage_Literals(ctx);
	BGBCC_FrBC_FlattenCountStrings(ctx);

	ct=obuf;
	*ct++='F';	*ct++='R';	*ct++='B';	*ct++='C';
	*ct++='2';	*ct++='C';	*ct++='0';	*ct++='0';
	*ct++=0;	*ct++=0;	*ct++=0;	*ct++=0;

	if(ctx->strtab)
	{
		i=ctx->sz_strtab+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='S';	*ct++='T';	*ct++='R';	*ct++='S';
		memcpy(ct, ctx->strtab, ctx->sz_strtab);
		ct=ct1;
	}

	if(ctx->wstrtab)
	{
		i=ctx->sz_wstrtab+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='W';	*ct++='S';	*ct++='T';	*ct++='R';
		memcpy(ct, ctx->wstrtab, ctx->sz_wstrtab);
		ct=ct1;
	}

	if(ctx->ctab_lvt4)
	{
		i=ctx->n_ctab_lvt4*4+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='L';	*ct++='V';	*ct++='T';	*ct++='4';
		
		for(i=0; i<ctx->n_ctab_lvt4; i++)
		{
			ct[i*4+0]=(ctx->ctab_lvt4[i]>>24)&255;
			ct[i*4+1]=(ctx->ctab_lvt4[i]>>16)&255;
			ct[i*4+2]=(ctx->ctab_lvt4[i]>> 8)&255;
			ct[i*4+3]=(ctx->ctab_lvt4[i]    )&255;
		}

		ct=ct1;
	}

	if(ctx->ctab_lvt8)
	{
		i=ctx->n_ctab_lvt8*8+8; ct1=ct+i;
		*ct++=0xE4;			*ct++=(i>>16)&255;
		*ct++=(i>>8)&255;	*ct++=(i    )&255;
		*ct++='L';	*ct++='V';	*ct++='T';	*ct++='8';
		
		for(i=0; i<ctx->n_ctab_lvt8; i++)
		{
			ct[i*8+0]=(ctx->ctab_lvt8[i]>>56)&255;
			ct[i*8+1]=(ctx->ctab_lvt8[i]>>48)&255;
			ct[i*8+2]=(ctx->ctab_lvt8[i]>>40)&255;
			ct[i*8+3]=(ctx->ctab_lvt8[i]>>32)&255;
			ct[i*8+4]=(ctx->ctab_lvt8[i]>>24)&255;
			ct[i*8+5]=(ctx->ctab_lvt8[i]>>16)&255;
			ct[i*8+6]=(ctx->ctab_lvt8[i]>> 8)&255;
			ct[i*8+7]=(ctx->ctab_lvt8[i]    )&255;
		}

		ct=ct1;
	}

	BGBCC_FrBC_FlattenImage_Globals(ctx, obuf, omsz, &ct);
	BGBCC_FrBC_FlattenImage_Literals(ctx, obuf, omsz, &ct);

	sz=ct-obuf;
	obuf[ 8]=(sz>>24)&255;
	obuf[ 9]=(sz>>16)&255;
	obuf[10]=(sz>> 8)&255;
	obuf[11]=(sz    )&255;
	return(sz);
}
