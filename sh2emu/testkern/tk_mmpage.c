u32 tkmm_pagebase, tkmm_pageend;
//static int tkmm_initvar=0;

byte tkmm_pagebmp[4096];
int tkmm_maxpage;
int tkmm_pagerov;

int TKMM_FindFreePages(int n)
{
	int i0, i1;
	int i, j, m;
	
	if(n<=0)
		return(-1);
	
	i=tkmm_pagerov;
	m=tkmm_maxpage;
	while(i<m)
	{
		if(tkmm_pagebmp[i>>3]&(1<<(i&7)))
		{
			while((tkmm_pagebmp[i>>3]==0xFF) && (i<m))
			{
				i=(i+8)&(~7);
				continue;
			}
			while((tkmm_pagebmp[i>>3]&(1<<(i&7))) && (i<m))
				i++;
			continue;
		}
		i0=i; i1=i0+n;
		if(i1>m)break;
		while(!(tkmm_pagebmp[i>>3]&(1<<(i&7))) && (i<i1))
			i++;
		if(i>=i1)
		{
			tkmm_pagerov=i1;
			return(i0);
		}
	}
	
	if(tkmm_pagerov!=0)
	{
		tkmm_pagerov=0;
		i=TKMM_FindFreePages(n);
		return(i);
	}

	return(-1);
}

int TKMM_AllocPages(int n)
{
	int i, j, k;
	
	i=TKMM_FindFreePages(n);
	if(i<0)return(-1);
	
	j=i; k=j+n;
	while(j<k)
	{
		tkmm_pagebmp[j>>3]|=(1<<(j&7));
		j++;
	}
	return(i);
}

void *TKMM_PageToPointer(int n)
{
	byte *ptr;
	ptr=((byte *)TKMM_PAGEBASE)+(n<<12);
	return(ptr);
}

int TKMM_PointerToPage(void *ptr)
{
	int d;
	
	d=((byte *)ptr)-((byte *)TKMM_PAGEBASE);
	return(d>>12);
}

void *TKMM_PageAlloc(int sz)
{
	void *p;
	int pg;
	
	pg=TKMM_AllocPages((sz+4095)>>12);
	if(pg<0)return(NULL);
	p=TKMM_PageToPointer(pg);
	return(p);
}

/** Convert size into a 5.3 minifloat, Round Up */
int TKMM_SizeToFxiU(int sz)
{
	int fr, ex;
	int i;
	
	fr=sz;
	ex=0;
	while((fr>>3)>1)
		{ fr=(fr+1)>>1; ex++; }
	i=(fr&7)|(ex<<3);
	return(i);
}

/** Convert size into a 5.3 minifloat, Round Down */
int TKMM_SizeToFxiD(int sz)
{
	int fr, ex;
	int i;
	
	fr=sz;
	ex=0;
	while((fr>>3)>1)
		{ fr=fr>>1; ex++; }
	i=(fr&7)|(ex<<3);
	return(i);
}

/** Convert Fxi to size */
int TKMM_FxiToSize(int ix)
{
	int ex, fr;
	int sz;
	
	if(ix<8)return(ix);
	fr=(ix&7)|8; ex=ix>>3;
	sz=fr<<ex;
	return(sz);
}

TKMM_MemLnkObj *tkmm_freelist[256];

void *TKMM_Malloc(int sz)
{
	TKMM_MemLnkObj *obj, *obj1;
	byte *p1, *p2;
	int ix, ix1;
	int sz1, sz2;

	if(sz<=0)return(NULL);
	if(sz<256)sz=256;
	
//	putc('<');
	sz=(sz+7)&(~7);
	ix=TKMM_SizeToFxiU(sz);
//	printf("%d", ix);
	obj=tkmm_freelist[ix];
	if(obj)
	{
//		putc('A');
		tkmm_freelist[ix]=*(TKMM_MemLnkObj **)(obj->data);
//		obj->ix=0;
		obj->fl&=~1;
//		putc('>');
		return((byte *)obj->data);
	}

//	putc('B');

	ix1=ix;
	while(!obj && (ix1<255))
		{ ix1++; obj=tkmm_freelist[ix1]; }

	if(!obj)
	{
//		putc('C');

		sz1=TKMM_FxiToSize(ix);
		sz1=(sz1+65535)&(~65535);
		
		p1=TKMM_PageAlloc(sz1);
		if(!p1)
		{
//			putc('!');
//			putc('>');
			return(NULL);
		}
		p2=p1+sz1;
		obj=(TKMM_MemLnkObj *)p1;
		sz2=p2-((byte *)obj1->data);
		obj->ix=TKMM_SizeToFxiD(sz2);
		obj->fl=3;
		obj->cnext=(TKMM_MemLnkObj *)p2;
		obj->cprev=NULL;
	}

//	putc('D');

	sz1=TKMM_FxiToSize(ix);
//	printf("%d", ix);
	sz1=(sz1+7)&(~7);
	p1=((byte *)obj->data)+sz1;
	p2=(byte *)obj->cnext;
	if((p2-p1)>256)
	{
//		putc('E');

		obj1=(TKMM_MemLnkObj *)p1;
		if(!(obj->fl&2))
			{ obj->cnext->cprev=obj1; }
		obj1->cnext=obj->cnext;
		obj1->cprev=obj;
		obj->cnext=obj1;
		obj1->fl=obj->fl;
		obj->fl&=~2;
		obj->ix=ix;
		
		sz2=p2-((byte *)obj1->data);
		obj1->ix=TKMM_SizeToFxiD(sz2);
		
		*(TKMM_MemLnkObj **)(obj1->data)=tkmm_freelist[obj1->ix];
		tkmm_freelist[obj1->ix]=obj1;
	}

//	putc('F');

	obj->fl&=~1;
//	putc('>');
	return((byte *)obj->data);
}

int TKMM_Free(void *ptr)
{
	TKMM_MemLnkObj *obj;

	if(!ptr)return(-1);
	obj=(TKMM_MemLnkObj *)(((byte *)ptr)-TKMM_OFFS_DATA);
	obj->fl|=1;

	*(TKMM_MemLnkObj **)(obj->data)=tkmm_freelist[obj->ix];
	tkmm_freelist[obj->ix]=obj;
	return(0);
}

void tkmm_init()
{
	static int init=0;
	int i;

	if(init)return;
	init=1;
		
	tkmm_pagebase=TKMM_PAGEBASE;
	tkmm_pageend=TKMM_PAGEEND;
	tkmm_maxpage=(tkmm_pageend-tkmm_pagebase)>>12;
	
	printf("tkmm_init: heap=%dMB\n",
		(tkmm_pageend-tkmm_pagebase)>>20);

	for(i=0; i<256; i++)
		tkmm_freelist[i]=NULL;
}
