#include <bgbccc.h>

int BGBCC_FrCC_BindSig(BGBCC_FrCC_State *ctx, char *name, char *sig)
{
	char tb[256];

//	DYLL_MetaPath_BindKey(name, "func");
	dyllMetaBindKey(name, "func");
	sprintf(tb, "%s:sig", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);

//	sprintf(tb, "FN:%s", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

int BGBCC_FrCC_BindTypeSig(BGBCC_FrCC_State *ctx, char *name, char *sig)
{
	char tb[256];

//	DYLL_MetaPath_BindKey(name, "type");
	dyllMetaBindKey(name, "type");

	sprintf(tb, "%s:sig", name);

//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);

//	sprintf(tb, "TY:%s", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

int BGBCC_FrCC_BindVarSig(BGBCC_FrCC_State *ctx, char *name, char *sig)
{
	char tb[256];
	char *ts;

	ts="var";
	if(ctx->cur_struct)ts="field";

	dyllMetaBindKey(name, ts);
	sprintf(tb, "%s:sig", name);
	dyllMetaBindKey(tb, sig);
	return(0);
}

int BGBCC_FrCC_BindVarInfo(BGBCC_FrCC_State *ctx, char *name, char *key, char *val)
{
	char tb[256];

	sprintf(tb, "%s:%s", name, key);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_FrCC_BindTypeInfo(BGBCC_FrCC_State *ctx, char *name, char *key, char *val)
	{ return(BGBCC_FrCC_BindVarInfo(ctx, name, key, val)); }

int BGBCC_FrCC_BindDyTypeSig(BGBCC_FrCC_State *ctx, char *name, char *sig)
{
	char tb[256];

	sprintf(tb, "bgbdy/%s", name);
	dyllMetaBindKey(tb, "dytype");
	sprintf(tb, "bgbdy/%s:sig", name);
	dyllMetaBindKey(tb, sig);
	return(0);
}

int BGBCC_FrCC_BindStructSig(BGBCC_FrCC_State *ctx, char *name, char *sig)
{
	char tb[256];

//	DYLL_MetaPath_BindKey(name, sig);
	dyllMetaBindKey(name, sig);

//	sprintf(tb, "S:%s", name);
//	DYLL_MetaPath_BindKey(tb, sig);
	return(0);
}

int BGBCC_FrCC_BindStructInfo(BGBCC_FrCC_State *ctx,
	char *name, char *key, char *val)
{
	char tb[256];

//	sprintf(tb, "SI:%s:%s", name, key);
	sprintf(tb, "%s:%s", name, key);
//	DYLL_MetaPath_BindKey(tb, val);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_FrCC_BindStructInfoIdx(BGBCC_FrCC_State *ctx,
	char *name, char *key, int idx, char *val)
{
	char tb[256];

//	sprintf(tb, "SI:%s:%s.%d", name, key, idx);
	sprintf(tb, "%s:%s.%d", name, key, idx);
//	DYLL_MetaPath_BindKey(tb, val);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_FrCC_BindFieldSig(BGBCC_FrCC_State *ctx,
	char *cls, char *name, char *sig)
{
	char tb[256];

//	sprintf(tb, "F:%s:%s", cls, name);
//	sprintf(tb, "%s:%s", cls, name);

	sprintf(tb, "%s/%s", cls, name);
//	DYLL_MetaPath_BindKey(tb, "field");
	dyllMetaBindKey(tb, "field");

	sprintf(tb, "%s/%s:sig", cls, name);
//	DYLL_MetaPath_BindKey(tb, sig);
	dyllMetaBindKey(tb, sig);
	return(0);
}

int BGBCC_FrCC_BindFieldInfo(BGBCC_FrCC_State *ctx,
	char *cls, char *name, char *key, char *val)
{
	char tb[256];

//	sprintf(tb, "FI:%s/%s:%s", cls, name, key);
	sprintf(tb, "%s/%s:%s", cls, name, key);
//	DYLL_MetaPath_BindKey(tb, val);
	dyllMetaBindKey(tb, val);
	return(0);
}

int BGBCC_FrCC_BindStructInfoI(BGBCC_FrCC_State *ctx,
	char *name, char *key, int val)
{
	char tb[64];

	sprintf(tb, "%d", val);
	BGBCC_FrCC_BindStructInfo(ctx, name, key, tb);
	return(0);
}

int BGBCC_FrCC_BindFieldInfoI(BGBCC_FrCC_State *ctx,
	char *cls, char *name, char *key, int val)
{
	char tb[64];

	sprintf(tb, "%d", val);
	BGBCC_FrCC_BindFieldInfo(ctx, cls, name, key, tb);
	return(0);
}
