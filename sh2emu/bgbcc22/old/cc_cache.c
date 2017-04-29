#include <bgbccc.h>
#include <bgbgc.h>
#include <bgbdy.h>

typedef union {
char pad[1024];
int tag;
struct {
int tag;
char name[252];
int hash;	//file hash
int rev;	//file revision
int deps[64];	//dependencies
int drev[64];	//dependencies
}file;
}BGBCC_CacheNode;


BGBCC_CacheNode *bgbcc_cache_node=NULL;
int bgbcc_cache_node_cnt, bgbcc_cache_node_max;

int bgbcc_cache_stk[256];
int bgbcc_cache_stk_pos;

void BGBCC_LoadCache()
{
	VFILE *fd;
	int i, sz;

	fd=vffopen("cc_usr/obj_cache.db", "rb");
	if(fd)
	{
		vfseek(fd, 0, 2);
		sz=vftell(fd);

		bgbcc_cache_node_cnt=sz/sizeof(BGBCC_CacheNode);
		bgbcc_cache_node_max=
			bgbcc_cache_node_cnt+(bgbcc_cache_node_cnt>>1);
		if(bgbcc_cache_node_max<256)
			bgbcc_cache_node_max=256;

		bgbcc_cache_node=malloc(
			bgbcc_cache_node_max*sizeof(BGBCC_CacheNode));

		i=bgbcc_cache_node_cnt*sizeof(BGBCC_CacheNode);
		vfseek(fd, 0, 0);
		vfread(bgbcc_cache_node, 1, i, fd);
		vfclose(fd);
	}else
	{
		bgbcc_cache_node_cnt=0;
		bgbcc_cache_node_max=256;
		i=bgbcc_cache_node_max*sizeof(BGBCC_CacheNode);
		bgbcc_cache_node=malloc(i);
	}
}

void BGBCC_StoreCache()
{
	VFILE *fd;
	int i;

	if(!bgbcc_cache_node)return;

	fd=vffopen("cc_usr/obj_cache.db", "wb");
	if(fd)
	{
		i=bgbcc_cache_node_cnt*sizeof(BGBCC_CacheNode);
		vfwrite(bgbcc_cache_node, 1, i, fd);
		vfclose(fd);
	}
}

int BGBCC_CacheAllocNode()
{
	int i;

	for(i=0; i<bgbcc_cache_node_cnt; i++)
	{
		if(bgbcc_cache_node[i].tag==0)
		{
			memset(&(bgbcc_cache_node[i]), 0,
				sizeof(BGBCC_CacheNode));
			return(i);
		}
	}

	if(bgbcc_cache_node_cnt<bgbcc_cache_node_max)
	{
		i=bgbcc_cache_node_cnt++;
		memset(&(bgbcc_cache_node[i]), 0, sizeof(BGBCC_CacheNode));
		bgbcc_cache_node[i].tag=-1;
		return(i);
	}

	bgbcc_cache_node_max+=bgbcc_cache_node_max>>1;
	i=bgbcc_cache_node_max*sizeof(BGBCC_CacheNode);
	bgbcc_cache_node=realloc(bgbcc_cache_node, i);

	i=bgbcc_cache_node_cnt++;
	memset(&(bgbcc_cache_node[i]), 0, sizeof(BGBCC_CacheNode));
	bgbcc_cache_node[i].tag=-1;
	return(i);
}

int BGBCC_CacheLookupFile(char *name)
{
	int i;

	if(!bgbcc_cache_node)BGBCC_LoadCache();

	for(i=0; i<bgbcc_cache_node_cnt; i++)
	{
		if(bgbcc_cache_node[i].tag!=1)continue;
		if(!strcmp(name, bgbcc_cache_node[i].file.name))
			return(i);
	}

	return(-1);
}

int BGBCC_CacheGetFile(char *name)
{
	int i, j;

	i=BGBCC_CacheLookupFile(name);
	if(i>=0)return(i);

	i=BGBCC_CacheAllocNode();
	bgbcc_cache_node[i].tag=1;
	strcpy(bgbcc_cache_node[i].file.name, name);
	bgbcc_cache_node[i].file.hash=-1;
	bgbcc_cache_node[i].file.rev=0;

	for(j=0; j<64; j++)
		bgbcc_cache_node[i].file.deps[j]=-1;

	return(i);
}

int BGBCC_CacheCheckDepNodes(int node)
{
	char *buf, *s;
	int i, j, k, sz, nd;

	if(bgbcc_cache_node[node].tag!=1)
		return(-1);

	for(i=0; i<bgbcc_cache_stk_pos; i++)
		if(bgbcc_cache_stk[i]==node)
			return(1);	//just ignore this
	bgbcc_cache_stk[bgbcc_cache_stk_pos++]=node;

	for(i=0; i<64; i++)
	{
		j=bgbcc_cache_node[node].file.deps[i];
		if(j<0)break;

		k=BGBCC_CacheCheckDepNodes(j);
		if(k<=0)
		{
			bgbcc_cache_stk_pos--;
			return(k);	//fail or out of date
		}

		k=bgbcc_cache_node[j].file.rev;
		if(k>bgbcc_cache_node[node].file.drev[i])
		{
			bgbcc_cache_stk_pos--;
			return(0);	//out of date
		}
	}

	//deps OK, verify file has not changed
	buf=BGBCC_ReadFile(bgbcc_cache_node[node].file.name, &sz);
	if(!buf)
	{
		bgbcc_cache_stk_pos--;
		return(1);	//no file?...
	}

	j=0; s=buf; for(i=0; i<sz; i++)j=j*251+(*s++);
	j=(j>>8)&0xFFFFFF;

//	free(buf);

	if(j==bgbcc_cache_node[node].file.hash)
	{
		bgbcc_cache_stk_pos--;
		return(1);	//ok, this file is up to date
	}

	bgbcc_cache_stk_pos--;
	return(0); //out of date
}

int BGBCC_CacheUpdateDepNodes(int node)
{
	char b[16384];
	char b1[256], b2[256];

	char *buf, *s, *t, *se, *s1;
	int i, j, k, sz, nd;
	int ty1, ty2;

	if(bgbcc_cache_node[node].tag!=1)
		return(-1);

	for(i=0; i<bgbcc_cache_stk_pos; i++)
		if(bgbcc_cache_stk[i]==node)
			return(1);	//just ignore this

	i=BGBCC_CacheCheckDepNodes(node);
	if(i==1)return(1);	//up to date
	if(i<0)return(i);	//error

	//deps OK, verify file has not changed
	buf=BGBCC_ReadFile(bgbcc_cache_node[node].file.name, &sz);
	if(!buf)return(1);	//no file?...

	j=0; s=buf; for(i=0; i<sz; i++)j=j*251+(*s++);
	j=(j>>8)&0xFFFFFF;

	bgbcc_cache_node[node].file.hash=j;	//update hash
	bgbcc_cache_node[node].file.rev++;

	bgbcc_cache_stk[bgbcc_cache_stk_pos++]=node;

	//process buffer for dependencies
	se=buf+sz; s=buf; nd=0;
	while((*s) && (s<se))
	{
		if(nd>=62)break;

		s=BGBCP_EatWhite(s);
		s=BGBPP_ParseLine(NULL, s, b);
		if(b[0]!='#')continue;

		s1=b+1;
		s1=BGBCP_Token(s1, b1, &ty1);
		s1=BGBCP_Token(s1, b2, &ty2);
		if(strcmp(b1, "include"))
			continue;

		if(!strcmp(b2, "<"))
		{
			t=b2;
			while(*s1 && (*s1!='>'))*t++=*s1++;
			*t++=0;

			bgbcc_cache_node[node].file.deps[nd++]=
				BGBCC_CacheGetFile(b2);
			continue;
		}

		t=b2; t+=strlen(t);
		while(*s1)
		{
			s1=BGBCP_Token(s1, t, &ty2);
			t+=strlen(t);
		}
		*t=0;

		i=BGBCC_CacheGetFile(b2);

		//avoid circular dependencies
//		for(j=0; j<bgbcc_cache_stk_pos; j++)
//			if(bgbcc_cache_stk[j]==i)break;
//		if(j<bgbcc_cache_stk_pos)continue;

		bgbcc_cache_node[node].file.deps[nd++]=i;
		continue;
	}

//	free(buf);

	for(i=nd; i<64; i++)
		bgbcc_cache_node[node].file.deps[i]=-1;

	for(i=0; i<64; i++)
	{
		j=bgbcc_cache_node[node].file.deps[i];
		if(j<0)break;

		k=BGBCC_CacheUpdateDepNodes(j);
		if(k<0)
		{
			bgbcc_cache_stk_pos--;
			return(k);	//error
		}

		k=bgbcc_cache_node[j].file.rev;
		bgbcc_cache_node[node].file.drev[i]=k;
	}

	bgbcc_cache_stk_pos--;
	return(0);
}

int BGBCC_CacheCheckFile(char *name)
{
	int i, j;

	i=BGBCC_CacheLookupFile(name);
	if(i<0)return(0);
	j=BGBCC_CacheCheckDepNodes(i);
	if(j==1)return(1);
	return(0);
}

int BGBCC_UpdateCheckFile(char *name)
{
	int i, j;

	i=BGBCC_CacheGetFile(name);
	if(i<0)return(j);

	bgbcc_cache_stk_pos=0;
	j=BGBCC_CacheUpdateDepNodes(i);
	BGBCC_StoreCache();

	return(j);
}

