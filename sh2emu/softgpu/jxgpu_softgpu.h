#define JXGPU_XY_SHR	16
#define JXGPU_ST_SHR	16
#define JXGPU_Z_SHR		16

#define JXGPU_PRTY_TRIANGLE			0x01

#define JXGPU_PRMD_TEX2D			0x10
#define JXGPU_PRMD_DEPTH			0x20
#define JXGPU_PRMD_COLOR			0x40

#define JXGPU_PRIMTY_TEXZTRI		1
#define JXGPU_PRIMTY_TEXZCLRTRI		2

typedef struct JXGPU_RasterVertex_s JXGPU_RasterVertex;

typedef struct JXGPU_RasterSpan_s JXGPU_RasterSpan;
typedef struct JXGPU_RasterPrim_s JXGPU_RasterPrim;
typedef struct JXGPU_RasterTexi_s JXGPU_RasterTexi;
typedef struct JXGPU_RasterState_s JXGPU_RasterState;

struct JXGPU_RasterVertex_s {
s32 x, y, z;
s32 s, t, c;
};

struct JXGPU_RasterSpan_s {
JXGPU_RasterSpan *next;
s32 sposx, eposx;		//start/end raster X
s32 posy;				//raster Y
u32 stexs, stext;		//start ST
u32 etexs, etext;		//end ST
u32 sposz, eposz;		//start/end Z
u64 sclr, eclr;
};

struct JXGPU_RasterPrim_s {
JXGPU_RasterPrim *next;
JXGPU_RasterTexi *texi;
JXGPU_RasterSpan *span;

int primty, texid;
JXGPU_RasterVertex vtx[4];
};

struct JXGPU_RasterTexi_s {
void *pix[16];			//pixel data
int xs, ys;				//size (power-of-2)
};

struct JXGPU_RasterState_s {
void *pxbuf;		//pixel data
void *zbuf;			//zbuffer
int xs, ys;			//size of raster buffer
int ystr;			//stride between scanlines
int cstepmask;		//color step mask

JXGPU_RasterTexi *texi[4096];

JXGPU_RasterSpan *freespan;
JXGPU_RasterPrim *freeprim;

JXGPU_RasterPrim *prims;
};
