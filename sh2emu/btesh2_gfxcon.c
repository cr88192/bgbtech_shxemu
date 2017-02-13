/*
0080_0000	Framebuffer 	(0x000000)
00E0_0000	TileBuffer  	(0x180000)
00FF_0000	Console Buffer	(0x1FC000)
00FF_8000	Registers		(0x1FE000)
00FF_8800	ASCII Font		(0x1FE200)
 */

#if 0
#define BTESH2_GFXCON_DEFAULT		0x0700
#define BTESH2_GFXCON_BRIGHT		0x0800
#define BTESH2_GFXCON_BLINK			0x8000

#define BTESH2_GFXCON_ITALIC		0x00010000
#define BTESH2_GFXCON_UNDERLINE		0x00020000
#define BTESH2_GFXCON_STRIKEOUT		0x00040000
#define BTESH2_GFXCON_REVERSE		0x00080000
#define BTESH2_GFXCON_DIRTY			0x10000000

#define BTESH2_GFXCON_GFXCELL		0x000E0000

#define BTESH2_GFXCON_ASCSHIFT		0
#define BTESH2_GFXCON_CLRFGSHIFT	8
#define BTESH2_GFXCON_CLRBGSHIFT	12
#define BTESH2_GFXCON_FONTSHIFT		20

#define BTESH2_GFXCON_ASCMASK		0x000000FF
#define BTESH2_GFXCON_CLRFGMASK		0x00000700
#define BTESH2_GFXCON_CLRBGMASK		0x00007000
#define BTESH2_GFXCON_FONTMASK		0x0FF00000
#endif

#if 1
#define BTESH2_GFXCON_DEFAULT		0x00070000
#define BTESH2_GFXCON_BRIGHT		0x00080000
#define BTESH2_GFXCON_BLINK			0x00800000
#define BTESH2_GFXCON_ITALIC		0x01000000
#define BTESH2_GFXCON_UNDERLINE		0x02000000
#define BTESH2_GFXCON_STRIKEOUT		0x04000000
#define BTESH2_GFXCON_REVERSE		0x08000000
#define BTESH2_GFXCON_DIRTY			0x10000000
#define BTESH2_GFXCON_GFXCELL		0x20000000

//#define BTESH2_GFXCON_GFXCELL		0x0E000000

#define BTESH2_GFXCON_ASCSHIFT		0
#define BTESH2_GFXCON_FONTSHIFT		8

#define BTESH2_GFXCON_CLRFGSHIFT	16
#define BTESH2_GFXCON_CLRBGSHIFT	20

#define BTESH2_GFXCON_ASCMASK		0x000000FF
#define BTESH2_GFXCON_FONTMASK		0x0000FF00
#define BTESH2_GFXCON_CLRFGMASK		0x00070000
#define BTESH2_GFXCON_CLRBGMASK		0x00700000

#endif


u64 btesh2_gfxcon_glyphs[256]=
{
0x0000000000000000,	//0x00
0x7c82aa82aaba827c,	//0x01
0x007effdbffdbc37e,	//0x02
0x00367f7f7f3e1c08,	//0x03
0x00081c3e3e3e1c08,	//0x04
0x1c1c087f6b08087f,	//0x05
0x081c3e7f7f7f087f,	//0x06
0x00001c3e3e1c0000,	//0x07
0x3f3f332121333f3f,	//0x08
0x000e1111110e0000,	//0x09
0x3f3f332d2d333f3f,	//0x0A
0x000f030539484830,	//0x0B
0x1c2222221c083e08,	//0x0C
0x00040605040c1c18,	//0x0D
0x0c0e0b0909393307,	//0x0E
0x08492a1c7f1c2a49,	//0x0F
0x0000303c3e3c3000,	//0x10
0x00000c3c7c3c0c00,	//0x11
0x183c7e18187e3c18,	//0x12
0x0000121212001200,	//0x13
0x00071d3d1d050505,	//0x14
0x0007080e0907010e,	//0x15
0x0000000f0f000000,	//0x16
0x081c08081c08003e,	//0x17
0x0000081c2a080808,	//0x18
0x00000808082a1c08,	//0x19
0x000004023f020400,	//0x1A
0x000008103f100800,	//0x1B
0x0000000808080f00,	//0x1C
0x00002442ff422400,	//0x1D
0x0000081c1c3e3e00,	//0x1E
0x00003e3e1c1c0800,	//0x1F

0x0000000000000000,	//0x20 " "
0x0010101010001000,	//0x21 "!"
0x0044444400000000,	//0x22 "\""
0x0044FF4444FF4400,	//0x23 "#"
0x103C403C047C1000,	//0x24 "$"
0x0044481020444400,	//0x25 "%"
0x1C241C384F443A00,	//0x26 "&"
0x0010101000000000,	//0x27 "'"
0x0008101010100800,	//0x28 "("
0x0020101010102000,	//0x29 ")"
0x00443CFF3C440000,	//0x2A "*"
0x0010107E10100000,	//0x2B "+"
0x0000000000101020,	//0x2C ","
0x0000007E00000000,	//0x2D "-"
0x0000000000101000,	//0x2E "."
0x0002040810204000,	//0x2F "/"
0x0038444C74443800,	//0x20 "0"
0x0010301010107E00,	//0x31 "1"
0x003C440810207E00,	//0x32 "2"
0x007C081008443C00,	//0x33 "3"
0x0008183C487E0800,	//0x34 "4"
0x007E407C04443C00,	//0x35 "5"
0x003C407C44443800,	//0x36 "6"
0x007E040810202000,	//0x37 "7"
0x003C443C44443800,	//0x38 "8"
0x003C443E04083000,	//0x39 "9"
0x0000101000101000,	//0x3A ":"
0x0000101000101020,	//0x3B ";"
0x0408102010080400,	//0x3C "<"
0x00007E00007E0000,	//0x3D "="
0x4020100810204000,	//0x3E ">"
0x003C440810001000,	//0x3F "?"
0x003C446E6E403E00,	//0x40 "@"
0x001038447C444400,	//0x41 "A"
0x0078447844447800,	//0x42 "B"
0x0038444040443800,	//0x43 "C"
0x0070484444487000,	//0x44 "D"
0x007E407C40407E00,	//0x45 "E"
0x007E407C40404000,	//0x46 "F"
0x003E40404E443C00,	//0x47 "G"
0x0044447C44444400,	//0x48 "H"
0x007E101010107E00,	//0x49 "I"
0x0004040404443800,	//0x4A "J"
0x0044487870484400,	//0x4B "K"
0x0040404040407E00,	//0x4C "L"
0x0042667E5A424200,	//0x4D "M"
0x00446454544C4400,	//0x4E "N"
0x0038444444443800,	//0x4F "O"
0x0078444478404000,	//0x50 "P"
0x003C444444483400,	//0x51 "Q"
0x0078444478484400,	//0x52 "R"
0x003C403C04047800,	//0x53 "S"
0x007E101010101000,	//0x54 "T"
0x0044444444443800,	//0x55 "U"
0x0044444444381000,	//0x56 "V"
0x0042425A5A664200,	//0x57 "W"
0x0044443838444400,	//0x58 "X"
0x0044443810101000,	//0x59 "Y"
0x007C081020407C00,	//0x5A "Z"
0x001E101010101E00,	//0x5B "["
0x0040201008040200,	//0x5C "\\"
0x0078080808087800,	//0x5D "]"
0x0008142200000000,	//0x5E "^"
0x000000000000FF00,	//0x5F "_"
0x0020201000000000,	//0x40 "`"
0x000038043C443E00,	//0x61 "a"
0x0040407844447800,	//0x62 "b"
0x0000384440443800,	//0x42 "c"
0x0004043C44443C00,	//0x64 "d"
0x000038447E403C00,	//0x65 "e"
0x000E103E10101000,	//0x44 "f"
0x00003844443C0478,	//0x67 "g"
0x0040407844444400,	//0x68 "h"
0x0010003010103800,	//0x69 "i"
0x0004000404040438,	//0x6A "j"
0x0040444870484400,	//0x6B "k"
0x0030101010103800,	//0x48 "l"
0x00007C5252524200,	//0x6D "m"
0x0000784444444400,	//0x6E "n"
0x0000384444443800,	//0x6F "o"
0x0000784444784040,	//0x70 "p"
0x00003C44443C0404,	//0x71 "q"
0x00406E7040404000,	//0x72 "r"
0x00003C4038047800,	//0x73 "s"
0x00107E1010100E00,	//0x74 "t"
0x0000444444443800,	//0x75 "u"
0x0000444444381000,	//0x76 "v"
0x00004252523C2400,	//0x77 "w"
0x0000442810284400,	//0x78 "x"
0x0000444444380870,	//0x79 "y"
0x00007C0810207C00,	//0x7A "z"
0x000E107010100E00,	//0x7B "{"
0x0010100010101000,	//0x7C "|"
0x0070100E10107000,	//0x7D "}"
0x0077DC0000000000,	//0x7E "~"
0x000000081C36427F,	//0x7F "\x7F"

0x000f0908090f020e,	//0x80 "?"
0x000a001212120c00,	//0x81 "?"
0x0618000e111f100f,	//0x82 "?"
0x0609000e010f110f,	//0x83 "?"
0x000a000e010f110f,	//0x84 "?"
0x0c03000e010f110f,	//0x85 "?"
0x0606000e010f110f,	//0x86 "?"
0x0000070808070206,	//0x87 "?"
0x040a000e111f100f,	//0x88 "?"
0x000a000e111f100f,	//0x89 "?"
0x0c03000e111f100f,	//0x8A "?"
0x000a000602020207,	//0x8B "?"
0x0205000602020207,	//0x8C "?"
0x0c03000602020207,	//0x8D "?"
0x09000e1b111f1111,	//0x8E "?"
0x0e000e1b111f1111,	//0x8F "?"
0x00000f080e080f00,	//0x90 "?"
0x00003c0a3e50522c,	//0x91 "?"
0x00003e487e484e00,	//0x92 "?"
0x040a000609090600,	//0x93 "?"
0x000a000609090600,	//0x94 "?"
0x0c03000609090600,	//0x95 "?"
0x040a000909090600,	//0x96 "?"
0x0c03000909090600,	//0x97 "?"
0x000a000909070106,	//0x98 "?"
0x0a00060909090906,	//0x99 "?"
0x0500090909090600,	//0x9A "?"
0x00040e1514150e04,	//0x9B "?"
0x000007040e040f00,	//0x9C "?"
0x110a041f041f0404,	//0x9D "?"
0x0000302b342a2906,	//0x9E "?"
0x0002040e04040418,	//0x9F "?"
0x030c000e010f110f,	//0xA0 "?"
0x0618000c04040e00,	//0xA1 "?"
0x030c000609090600,	//0xA2 "?"
0x030c000909090600,	//0xA3 "?"
0x050a000609090900,	//0xA4 "?"
0x050a00090d0f0b09,	//0xA5 "?"
0x18041c2418003e00,	//0xA6 "?"
0x0018242418003c00,	//0xA7 "?"
0x0008000810221c00,	//0xA8 "?"
0x000000000f080808,	//0xA9 "?"
0x00000000f8080808,	//0xAA "?"
0x4244485721470407,	//0xAB "?"
0x4244485525470101,	//0xAC "?"
0x0000020002020200,	//0xAD "?"
0x0014142828141400,	//0xAE "?"
0x0014140a0a141400,	//0xAF "?"
0xaa00aa00aa00aa00,	//0xB0 "?"
0xaa55aa55aa55aa55,	//0xB1 "?"
0xaaffaaffaaffaaff,	//0xB2 "?"
0x0808080808080808,	//0xB3 "?"
0x080808f808080808,	//0xB4 "?"
0x0808f808f8080808,	//0xB5 "?"
0x14141414f4141414,	//0xB6 "?"
0x00000000fc141414,	//0xB7 "?"
0x000000f808f80808,	//0xB8 "?"
0x141414f404f41414,	//0xB9 "?"
0x1414141414141414,	//0xBA "?"
0x000000fc04f41414,	//0xBB "?"
0x141414f404fc0000,	//0xBC "?"
0x14141414fc000000,	//0xBD "?"
0x0808083808380000,	//0xBE "?"
0x0000001c04040404,	//0xBF "?"
0x080808080f000000,	//0xC0 "?"
0x10101010ff000000,	//0xC1 "?"
0x00000000ff080808,	//0xC2 "?"
0x080808080f080808,	//0xC3 "?"
0x00000000ff000000,	//0xC4 "?"
0x08080808ff080808,	//0xC5 "?"
0x0808080f080f0808,	//0xC6 "?"
0x1414141417141414,	//0xC7 "?"
0x0808080f080f0000,	//0xC8 "?"
0x0000001f10171414,	//0xC9 "?"
0x141414f404fc0000,	//0xCA "?"
0x000000ff00f71414,	//0xCB "?"
0x1414141710171414,	//0xCC "?"
0x000000ff00ff0000,	//0xCD "?"
0x141414f700f71414,	//0xCE "?"
0x040404ff00ff0000,	//0xCF "?"
0x141414143e000000,	//0xD0 "?"
0x0000003e003e0808,	//0xD1 "?"
0x000000001f141414,	//0xD2 "?"
0x141414141f000000,	//0xD3 "?"
0x0808080f080f0000,	//0xD4 "?"
0x0000000f080f0808,	//0xD5 "?"
0x000000001f141414,	//0xD6 "?"
0x14141414f7141414,	//0xD7 "?"
0x080808ff08ff0808,	//0xD8 "?"
0x08080808ff000000,	//0xD9 "?"
0x000000000f080808,	//0xDA "?"
0xffffffffffffffff,	//0xDB "?"
0x00000000ffffffff,	//0xDC "?"
0xf0f0f0f0f0f0f0f0,	//0xDD "?"
0x0f0f0f0f0f0f0f0f,	//0xDE "?"
0xffffffff00000000,	//0xDF "?"
0x00023a4444443a02,	//0xE0 "?"
0x00382c243e22362c,	//0xE1 "?"
0x003c202020202000,	//0xE2 "?"
0x00003e1414141400,	//0xE3 "?"
0x003e10080408103e,	//0xE4 "?"
0x00001f2222221c00,	//0xE5 "?"
0x00000012121e1010,	//0xE6 "?"
0x0000003e08080806,	//0xE7 "?"
0x7f083e49493e087f,	//0xE8 "?"
0x001c22223e22221c,	//0xE9 "?"
0x001c222222221422,	//0xEA "?"
0x000704040e11110e,	//0xEB "?"
0x0000003649360000,	//0xEC "?"
0x0026494949493e08,	//0xED "?"
0x000609101e100906,	//0xEE "?"
0x00001c2222222222,	//0xEF "?"
0x00003e003e003e00,	//0xF0 "?"
0x0008083e0808003e,	//0xF1 "?"
0x0804020408001f00,	//0xF2 "?"
0x0204080402001f00,	//0xF3 "?"
0x0000030202020202,	//0xF4 "?"
0x0202020202060000,	//0xF5 "?"
0x000008003e000800,	//0xF6 "?"
0x0000324c00324c00,	//0xF7 "?"
0x0008141408000000,	//0xF8 "?"
0x000000000c0c0000,	//0xF9 "?"
0x000000000c000000,	//0xFA "?"
0x0007040424140c04,	//0xFB "?"
0x00000e0909090000,	//0xFC "?"
0x0007010704070000,	//0xFD "?"
0x0000001e1e1e1e00,	//0xFE "?"
};

u32 *btesh2_gfxcon_framebuf;
int btesh2_gfxcon_fbxs;
int btesh2_gfxcon_fbys;
int btesh2_gfxcon_fbsz;
byte btesh2_gfxcon_fb_dirty;

u32 *btesh2_gfxcon_conbuf;
u32 *btesh2_gfxcon_aconbuf;
int btesh2_gfxcon_cbxs;
int btesh2_gfxcon_cbys;
int btesh2_gfxcon_cbsz;
int btesh2_gfxcon_cbsz2;

short btesh2_gfxcon_con_xpos;
short btesh2_gfxcon_con_ypos;
short btesh2_gfxcon_con_svxpos;
short btesh2_gfxcon_con_svypos;
u32 btesh2_gfxcon_con_attr;
byte btesh2_gfxcon_con_dirty;
byte btesh2_gfxcon_con_disabled;

byte btesh2_gfxcon_con_ansret[64];
byte btesh2_gfxcon_con_rawkeyb=false;

int GfxDrv_PrepareFramebuf();
int FRGL_TimeMS();


u32 btesh2_gfxcon_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	int ix, ix1;
	u32 v;

	ix=reladdr>>2;
	if(ix<btesh2_gfxcon_fbsz)
	{
		GfxDrv_PrepareFramebuf();

		return(btesh2_gfxcon_framebuf[ix]);
	}
	
	ix1=ix-0x1FC000;
	if((ix1>=0) && (ix1<btesh2_gfxcon_cbsz2))
	{
		return(btesh2_gfxcon_conbuf[ix1]);
	}

	ix1=ix-0x1FE000;
	if((ix1>=0) && (ix1<256))
	{
		switch(ix1)
		{
		case 0: v=btesh2_gfxcon_fbxs; break;
		case 1: v=btesh2_gfxcon_fbys; break;
		case 2: v=btesh2_gfxcon_cbxs; break;
		case 3: v=btesh2_gfxcon_cbys; break;
		case 4: v=btesh2_gfxcon_fbsz; break;
		case 5: v=btesh2_gfxcon_cbsz; break;
		case 6: v=btesh2_gfxcon_con_xpos; break;
		case 7: v=btesh2_gfxcon_con_ypos; break;
		case 8: v=btesh2_gfxcon_con_attr; break;
		default: v=0; break;
		}
		
		return(v);
	}
	
//	printf("EMAC Get\n");
	return(0);
}

int btesh2_gfxcon_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int ix, ix1;

	ix=reladdr>>2;
	if(ix<btesh2_gfxcon_fbsz)
	{
		GfxDrv_PrepareFramebuf();

		btesh2_gfxcon_framebuf[ix]=val;
		return(0);
	}

	ix1=ix-0x1FC000;
	if((ix1>=0) && (ix1<btesh2_gfxcon_cbsz2))
	{
		btesh2_gfxcon_conbuf[ix1]=val;
		btesh2_gfxcon_con_dirty=1;
		return(0);
	}

	ix1=ix-0x1FE000;
	if((ix1>=0) && (ix1<256))
	{
		switch(ix1)
		{
		case 0: btesh2_gfxcon_fbxs=val; break;
		case 1: btesh2_gfxcon_fbys=val; break;
		case 2: btesh2_gfxcon_cbxs=val; break;
		case 3: btesh2_gfxcon_cbys=val; break;
		case 4: btesh2_gfxcon_fbsz=val; break;
		case 5: btesh2_gfxcon_cbsz=val; break;
		case 6: btesh2_gfxcon_con_xpos=val; break;
		case 7: btesh2_gfxcon_con_ypos=val; break;
		case 8: btesh2_gfxcon_con_attr=val; break;
		default: break;
		}
		
		return(0);
	}

//	printf("EMAC Set\n");
	return(0);
}

u32 btesh2_gfxcon_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_gfxcon_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(sbyte)j;
	return(j);
}

u32 btesh2_gfxcon_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_gfxcon_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(s16)j;
	return(j);
}

int btesh2_gfxcon_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=btesh2_gfxcon_SetD(sp, cpu, reladdr, val);
	return(i);
}

int btesh2_gfxcon_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=btesh2_gfxcon_SetD(sp, cpu, reladdr, val);
	return(i);
}

int btesh2_gfxcon_scrollUp()
{
	int i, j, k;
	
//	for(i=0; i<(btesh2_gfxcon_cbys-1); i++)
	for(i=0; i<btesh2_gfxcon_cbys; i++)
	{
		for(j=0; j<btesh2_gfxcon_cbxs; j++)
		{
			k=btesh2_gfxcon_conbuf[(i+1)*btesh2_gfxcon_cbxs+j];
			k|=BTESH2_GFXCON_DIRTY;
			btesh2_gfxcon_conbuf[(i+0)*btesh2_gfxcon_cbxs+j]=k;

			k=btesh2_gfxcon_aconbuf[(i+1)*btesh2_gfxcon_cbxs+j];
			btesh2_gfxcon_aconbuf[(i+0)*btesh2_gfxcon_cbxs+j]=k;
		}
	}

//	i=(btesh2_gfxcon_cbys-1);
//	memset(btesh2_gfxcon_conbuf+i*btesh2_gfxcon_cbxs,
//		0, btesh2_gfxcon_cbxs*sizeof(u32));
	return(0);
}

int btesh2_gfxcon_scrollDown()
{
	int i, j, k;
	
//	for(i=0; i<btesh2_gfxcon_cbys; i++)
	for(i=btesh2_gfxcon_cbys-1; i>0; i--)
	{
		for(j=0; j<btesh2_gfxcon_cbxs; j++)
		{
			k=btesh2_gfxcon_conbuf[(i-1)*btesh2_gfxcon_cbxs+j];
			k|=BTESH2_GFXCON_DIRTY;
			btesh2_gfxcon_conbuf[(i+0)*btesh2_gfxcon_cbxs+j]=k;

			k=btesh2_gfxcon_aconbuf[(i-1)*btesh2_gfxcon_cbxs+j];
			btesh2_gfxcon_aconbuf[(i+0)*btesh2_gfxcon_cbxs+j]=k;
		}
	}

	for(j=0; j<btesh2_gfxcon_cbxs; j++)
	{
		k=BTESH2_GFXCON_DIRTY;
		btesh2_gfxcon_conbuf[j]=k;
		btesh2_gfxcon_aconbuf[j]=0;
	}

//	i=(btesh2_gfxcon_cbys-1);
//	memset(btesh2_gfxcon_conbuf+i*btesh2_gfxcon_cbxs,
//		0, btesh2_gfxcon_cbxs*sizeof(u32));
	return(0);
}

int btesh2_gfxcon_scrollUpN(int n)
{
	while((n--)>0)
		btesh2_gfxcon_scrollUp();
	return(0);
}

int btesh2_gfxcon_scrollDownN(int n)
{
	while((n--)>0)
		btesh2_gfxcon_scrollDown();
	return(0);
}

int btesh2_gfxcon_clearScreen(int mode)
{
	int i, j, k, l;

	l=btesh2_gfxcon_con_ypos*btesh2_gfxcon_cbxs+
		btesh2_gfxcon_con_xpos;

	for(i=0; i<btesh2_gfxcon_cbys; i++)
	{
		if((mode&4) && (i!=btesh2_gfxcon_con_ypos))
			continue;

		for(j=0; j<btesh2_gfxcon_cbxs; j++)
		{
			k=i*btesh2_gfxcon_cbxs+j;
			if(((mode==0) || (mode==4)) && (k<l))
				continue;
			if(((mode==1) || (mode==5)) && (k>l))
				continue;
		
			k=BTESH2_GFXCON_DIRTY;

			if(mode==16)
			{
				k=btesh2_gfxcon_conbuf[(i+0)*btesh2_gfxcon_cbxs+j];
				k|=BTESH2_GFXCON_DIRTY;
				l=btesh2_gfxcon_aconbuf[(i+0)*btesh2_gfxcon_cbxs+j];
			}

			btesh2_gfxcon_conbuf[(i+0)*btesh2_gfxcon_cbxs+j]=k;
			btesh2_gfxcon_aconbuf[(i+0)*btesh2_gfxcon_cbxs+j]=l;
		}
	}
	return(0);
}

int BTESH2_GfxCon_AdjustFixupPrint()
{
	if(btesh2_gfxcon_con_xpos>=btesh2_gfxcon_cbxs)
	{
		btesh2_gfxcon_con_xpos-=btesh2_gfxcon_cbxs;
		btesh2_gfxcon_con_ypos++;
	}
	if(btesh2_gfxcon_con_ypos>=btesh2_gfxcon_cbys)
	{
		btesh2_gfxcon_scrollUp();
		btesh2_gfxcon_con_ypos--;
	}
	return(0);
}

int BTESH2_GfxCon_PrintCharInner(int val)
{
	int cv;
	byte x, y;

	if(!btesh2_gfxcon_conbuf)
		return(-1);

	btesh2_gfxcon_con_dirty=1;

	if(val<' ')
	{
		x=btesh2_gfxcon_con_xpos;
		y=btesh2_gfxcon_con_ypos;
		cv=btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x];
		btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x]=
			cv|BTESH2_GFXCON_DIRTY;

		switch(val)
		{
		case '\r':
			btesh2_gfxcon_con_xpos=0;
			break;
		case '\n':
			btesh2_gfxcon_con_ypos++;
			break;

		case '\b':
			btesh2_gfxcon_con_xpos--;
			break;

		case '\t':
			btesh2_gfxcon_con_xpos=(btesh2_gfxcon_con_xpos+8)&(~7);
			break;
		default:
			break;
		}

		x=btesh2_gfxcon_con_xpos;
		y=btesh2_gfxcon_con_ypos;
		cv=btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x];
		btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x]=
			cv|BTESH2_GFXCON_DIRTY;
	}else
	{
		BTESH2_GfxCon_AdjustFixupPrint();

		x=btesh2_gfxcon_con_xpos++;
		y=btesh2_gfxcon_con_ypos;
		
		if(val>256)
		{
			btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x]=
				val|BTESH2_GFXCON_DIRTY;
		}else
		{
			btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x]=
				btesh2_gfxcon_con_attr|val|BTESH2_GFXCON_DIRTY;
		}

		x=btesh2_gfxcon_con_xpos;
		y=btesh2_gfxcon_con_ypos;
		cv=btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x];
		btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x]=
			cv|BTESH2_GFXCON_DIRTY;
	}

	x=btesh2_gfxcon_con_xpos;
	y=btesh2_gfxcon_con_ypos;
	cv=btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x];
	if(!cv)
	{
		btesh2_gfxcon_conbuf[y*btesh2_gfxcon_cbxs+x]=
			btesh2_gfxcon_con_attr|' '|BTESH2_GFXCON_DIRTY;
	}

	return(0);
}

byte btesh2_gfxcon_esc;
byte btesh2_gfxcon_nextesc;
int btesh2_gfxcon_parm[16];
int btesh2_gfxcon_nparm;
int btesh2_gfxcon_parmv;

int BTESH2_GfxCon_PrintChar(int val)
{
	int x, y;
	int i, j, k;

	if((val<' ') && btesh2_gfxcon_esc)
	{
		if((btesh2_gfxcon_esc==3) ||
			(btesh2_gfxcon_esc==4))
		{
			if(val=='\n')
			{
				BTESH2_GfxCon_PrintCharInner('\r');
				BTESH2_GfxCon_PrintCharInner('\n');
				return(0);
			}

			btesh2_gfxcon_esc=0;
		}
	}

	if(btesh2_gfxcon_esc)
	{
		switch(btesh2_gfxcon_esc)
		{
		case 1:
			if(val=='[')
			{
				btesh2_gfxcon_esc=2;
				btesh2_gfxcon_nparm=0;
				btesh2_gfxcon_parmv=0;
				break;
			}

			if(val=='!')
			{
				btesh2_gfxcon_esc=3;
				btesh2_gfxcon_parmv=0;
				btesh2_gfxcon_nparm=3;
				break;
			}

			if(val=='$')
			{
				btesh2_gfxcon_esc=4;
				btesh2_gfxcon_parmv=0;
				btesh2_gfxcon_nparm=4;
				break;
			}

			if(val=='%')
			{
				btesh2_gfxcon_esc=8;
				btesh2_gfxcon_parmv=0;
				btesh2_gfxcon_nparm=4;
				break;
			}

			printf("^%c?", val);
			btesh2_gfxcon_esc=0;
			break;
		case 2:
			if((val>='0') && (val<='9'))
			{
				btesh2_gfxcon_parmv=(btesh2_gfxcon_parmv*10)+(val-'0');
				break;
			}
			if(val==';')
			{
				btesh2_gfxcon_parm[btesh2_gfxcon_nparm++]=
					btesh2_gfxcon_parmv;
				btesh2_gfxcon_parmv=0;
				break;
			}

			btesh2_gfxcon_parm[btesh2_gfxcon_nparm++]=
				btesh2_gfxcon_parmv;
			btesh2_gfxcon_esc=0;
			switch(val)
			{
			case 'A':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_con_ypos-=i;
				if(btesh2_gfxcon_con_ypos<0)
					btesh2_gfxcon_con_ypos=0;
				break;
			case 'B':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_con_ypos+=i;
				if(btesh2_gfxcon_con_ypos>=btesh2_gfxcon_cbys)
					btesh2_gfxcon_con_ypos=btesh2_gfxcon_cbys-1;
				break;
			case 'C':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_con_xpos+=i;
				if(btesh2_gfxcon_con_xpos>=btesh2_gfxcon_cbxs)
					btesh2_gfxcon_con_xpos=btesh2_gfxcon_cbxs-1;
				break;
			case 'D':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_con_xpos-=i;
				if(btesh2_gfxcon_con_xpos<0)
					btesh2_gfxcon_con_xpos=0;
				break;
			case 'E':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_con_xpos=0;
				btesh2_gfxcon_con_ypos+=i;
				if(btesh2_gfxcon_con_ypos>=btesh2_gfxcon_cbys)
					btesh2_gfxcon_con_ypos=btesh2_gfxcon_cbys-1;
				break;
			case 'F':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_con_xpos=0;
				btesh2_gfxcon_con_ypos-=i;
				if(btesh2_gfxcon_con_ypos<0)
					btesh2_gfxcon_con_ypos=0;
				break;
			case 'G':
				btesh2_gfxcon_con_xpos=btesh2_gfxcon_parm[0]-1;
				if(btesh2_gfxcon_con_xpos<0)
					btesh2_gfxcon_con_xpos=0;
				if(btesh2_gfxcon_con_xpos>=btesh2_gfxcon_cbxs)
					btesh2_gfxcon_con_xpos=btesh2_gfxcon_cbxs-1;
				break;
			case 'H':
			case 'f':
				btesh2_gfxcon_con_ypos=btesh2_gfxcon_parm[0]-1;
				btesh2_gfxcon_con_xpos=btesh2_gfxcon_parm[1]-1;
				if(btesh2_gfxcon_con_xpos<0)
					btesh2_gfxcon_con_xpos=0;
				if(btesh2_gfxcon_con_ypos<0)
					btesh2_gfxcon_con_ypos=0;
				if(btesh2_gfxcon_con_xpos>=btesh2_gfxcon_cbxs)
					btesh2_gfxcon_con_xpos=btesh2_gfxcon_cbxs-1;
				if(btesh2_gfxcon_con_ypos>=btesh2_gfxcon_cbys)
					btesh2_gfxcon_con_ypos=btesh2_gfxcon_cbys-1;
				break;
				
			case 'J':
				i=btesh2_gfxcon_parm[0];
				btesh2_gfxcon_clearScreen(i);
				if(i==2)
				{
					btesh2_gfxcon_con_xpos=0;
					btesh2_gfxcon_con_ypos=0;
				}
				break;
			case 'K':
				i=btesh2_gfxcon_parm[0];
				btesh2_gfxcon_clearScreen(i|4);
				break;

			case 'S':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_scrollUpN(i);
				break;
			case 'T':
				i=btesh2_gfxcon_parm[0];
				if(!i)i=1;
				btesh2_gfxcon_scrollDownN(i);
				break;

			case 'm':
				switch(btesh2_gfxcon_parm[0])
				{
				case 0:
//					btesh2_gfxcon_con_attr=0x0700;
					btesh2_gfxcon_con_attr=BTESH2_GFXCON_DEFAULT;
					break;
				case 1:
					btesh2_gfxcon_con_attr|=BTESH2_GFXCON_BRIGHT;
					break;

				case 3:
					btesh2_gfxcon_con_attr|=BTESH2_GFXCON_ITALIC;
					break;
				case 4:
					btesh2_gfxcon_con_attr|=BTESH2_GFXCON_UNDERLINE;
					break;

				case 5:
				case 6:
					btesh2_gfxcon_con_attr|=BTESH2_GFXCON_BLINK;
					break;
				case 7:
					btesh2_gfxcon_con_attr|=BTESH2_GFXCON_REVERSE;
					break;
				case 8:
				case 9:
					btesh2_gfxcon_con_attr|=BTESH2_GFXCON_STRIKEOUT;
					break;
				case 10: case 11: case 12: case 13:
				case 14: case 15: case 16: case 17:
				case 18: case 19: case 20:
					i=btesh2_gfxcon_parm[0]-10;
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_FONTMASK;
					btesh2_gfxcon_con_attr|=(i<<BTESH2_GFXCON_FONTSHIFT);
					break;
				case 21:
				case 22:
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_BRIGHT;
					break;
				case 23:
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_ITALIC;
					break;
				case 24:
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_UNDERLINE;
					break;
				case 25:
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_BLINK;
					break;
				case 26:
					break;
				case 27:
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_REVERSE;
					break;
				case 28:
				case 29:
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_STRIKEOUT;
					break;
				case 30: case 31: case 32: case 33:
				case 34: case 35: case 36: case 37:
					i=btesh2_gfxcon_parmv-30;
//					i=(i&2)|((i>>2)&1)|((i<<2)&4);
//					btesh2_gfxcon_con_attr&=0x0FFFF800;
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_CLRFGMASK;
					btesh2_gfxcon_con_attr|=(i<<BTESH2_GFXCON_CLRFGSHIFT);
					break;
				case 38:
					break;
				case 39:
//					btesh2_gfxcon_con_attr&=0x0FFFF800;
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_CLRFGMASK;
					btesh2_gfxcon_con_attr|=(7<<BTESH2_GFXCON_CLRFGSHIFT);
					break;
				case 40: case 41: case 42: case 43:
				case 44: case 45: case 46: case 47:
					i=btesh2_gfxcon_parmv-40;
//					i=(i&2)|((i>>2)&1)|((i<<2)&4);
//					btesh2_gfxcon_con_attr&=0x0FFF8F00;
//					btesh2_gfxcon_con_attr|=(i<<12);
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_CLRBGMASK;
					btesh2_gfxcon_con_attr|=(i<<BTESH2_GFXCON_CLRBGSHIFT);
					break;
				case 48:
					break;
				case 49:
//					btesh2_gfxcon_con_attr&=0x0FFF8F00;
//					btesh2_gfxcon_con_attr|=0x00000000;
					btesh2_gfxcon_con_attr&=~BTESH2_GFXCON_CLRBGMASK;
					break;
				default:
					break;
				}
				break;
			case 'n':
				if(btesh2_gfxcon_parm[0]==6)
				{
					sprintf((char *)btesh2_gfxcon_con_ansret,
						"\x1B[%d;%dR",
							btesh2_gfxcon_con_ypos+1,
							btesh2_gfxcon_con_xpos+1);
				}
				break;
			
			case 's':
				btesh2_gfxcon_con_svxpos=btesh2_gfxcon_con_xpos;
				btesh2_gfxcon_con_svypos=btesh2_gfxcon_con_ypos;
				break;
			case 'u':
				btesh2_gfxcon_con_xpos=btesh2_gfxcon_con_svxpos;
				btesh2_gfxcon_con_ypos=btesh2_gfxcon_con_svypos;
				break;
			
			default:
				printf("^[%c?", val);
				break;
			}
			break;
		case 3:
//			btesh2_gfxcon_parmv=(btesh2_gfxcon_parmv<<7)|(val&127);
			i=val-'!';
			if(i>=85)
			{
				switch(val)
				{
				case 'v': i='\''-'!'; break;
				case 'w': i='\"'-'!'; break;
				case 'x': i='\\'-'!'; break;
				}
			}
			btesh2_gfxcon_parmv=(btesh2_gfxcon_parmv*85)+i;
//			btesh2_gfxcon_nparm--;
//			if(!btesh2_gfxcon_nparm)
			if(!btesh2_gfxcon_nparm--)
			{
				i=btesh2_gfxcon_parmv;
//				j=	 (i&0x0000FF)|
//					((i&0x00FF00)<<12)|
//					((i&0xFF0000)>> 8)|
//					0x100E0000;

//				j=	 (i&BTESH2_GFXCON_ASCMASK)|
//					(((i&0x00FF00)>> 8)<<BTESH2_GFXCON_FONTSHIFT)|
//					(((i&0xFF0000)>>16)<<BTESH2_GFXCON_CLRFGSHIFT)|
//					BTESH2_GFXCON_DIRTY|
//					BTESH2_GFXCON_GFXCELL;

				j=(i&0x00FFFFFF)|
					BTESH2_GFXCON_DIRTY|
					BTESH2_GFXCON_GFXCELL;

				BTESH2_GfxCon_PrintCharInner(j);
				if((i>>24)&1)
				{
					btesh2_gfxcon_parmv=0;
					btesh2_gfxcon_nparm=3;
				}else
				{
					btesh2_gfxcon_esc=0;
				}
			}
			break;

		case 4:
		case 5:
			i=val-'!';
			if(i>=85)
			{
				switch(val)
				{
				case 'v': i='\''-'!'; break;
				case 'w': i='\"'-'!'; break;
				case 'x': i='\\'-'!'; break;
				}
			}
			btesh2_gfxcon_parmv=(btesh2_gfxcon_parmv*85)+i;
			if(!(btesh2_gfxcon_nparm--))
			{
				i=btesh2_gfxcon_parmv;
				x=btesh2_gfxcon_con_xpos;
				y=btesh2_gfxcon_con_ypos;

				j=(i&0x0FFFFFFF)|
					BTESH2_GFXCON_DIRTY|
					BTESH2_GFXCON_GFXCELL;

				if(btesh2_gfxcon_esc==5)
				{
					BTESH2_GfxCon_AdjustFixupPrint();
					btesh2_gfxcon_con_xpos++;

					if(btesh2_gfxcon_aconbuf)
					{
						btesh2_gfxcon_aconbuf[y*btesh2_gfxcon_cbxs+x]=
							btesh2_gfxcon_parmv;
					}

					btesh2_gfxcon_parmv=0;
					btesh2_gfxcon_nparm=4;
					btesh2_gfxcon_esc=
						btesh2_gfxcon_nextesc;
					break;
				}

				BTESH2_GfxCon_PrintCharInner(j);
				k=(i>>28)&15;
				switch(k)
				{
				case 0:
					btesh2_gfxcon_esc=0;
					break;
				case 1:
					btesh2_gfxcon_parmv=0;
					btesh2_gfxcon_nparm=4;
					btesh2_gfxcon_esc=4;
					break;
				case 2:
				case 3:
					btesh2_gfxcon_con_xpos=x;
					btesh2_gfxcon_con_ypos=y;
					btesh2_gfxcon_parmv=0;
					btesh2_gfxcon_nparm=4;
					btesh2_gfxcon_esc=5;
					btesh2_gfxcon_nextesc=(k&1)?4:0;
					break;
				default:
					btesh2_gfxcon_esc=0;
					break;
				}
			}
			break;

		case 8:
			if(val=='~')
				{ btesh2_gfxcon_con_rawkeyb=true; }
			break;

		default:
			break;
		}
		return(0);
	}

	if(val=='\n')
	{
		BTESH2_GfxCon_PrintCharInner('\r');
		BTESH2_GfxCon_PrintCharInner('\n');
		return(0);
	}

	if(val=='\x1B')
	{
		btesh2_gfxcon_esc=1;
		return(0);
	}

	if(val=='\x1F')
	{
		btesh2_gfxcon_esc=4;
		btesh2_gfxcon_parmv=0;
		btesh2_gfxcon_nparm=4;
		return(0);
	}

	BTESH2_GfxCon_PrintCharInner(val);
	return(0);
}

int BTESH2_GfxCon_PrintString(char *str)
{
	char *s;

	s=str;
	while(*s)
	{
		BTESH2_GfxCon_PrintChar(*s++);
	}
	return(0);
}

byte btesh2_gfxcon_blink;

int BTESH2_GfxCon_BlinkState(int v)
{
	int cx, cy;

	if(v!=btesh2_gfxcon_blink)
	{
		btesh2_gfxcon_blink=v;
		btesh2_gfxcon_con_dirty=1;

		cx=btesh2_gfxcon_con_xpos;
		cy=btesh2_gfxcon_con_ypos;
		btesh2_gfxcon_conbuf[cy*btesh2_gfxcon_cbxs+cx]|=BTESH2_GFXCON_DIRTY;
	}
	return(0);
}

u32 cram_tclr_rgb555(u16 pxa)
{
	u32 pxc;
//	pxc=0xFF000000|
//		((pxa&0x7C00)<<9)|
//		((pxa&0x03E0)<<6)|
//		((pxa&0x001F)<<3);

	pxc=0xFF000000|
		((pxa&0x7C00)>> 7)|
		((pxa&0x03E0)<< 6)|
		((pxa&0x001F)<<19);

	return(pxc);
}

int BTESH2_GfxCon_Redraw()
{
	static const u32 clrtab[16]={
		0xFF000000,	0xFF0000AA,	0xFF00AA00,	0xFF00AAAA,
		0xFFAA0000,	0xFFAA00AA,	0xFFAA5500,	0xFFAAAAAA,
		0xFF555555,	0xFF5555FF,	0xFF55FF55,	0xFF55FFFF,
		0xFFFF5555,	0xFFFF55FF,	0xFFFFFF55,	0xFFFFFFFF};
//	static const byte lqbitab[16]={
//		0x00, 0x03, 0x0C, 0x0F, 0x30, 0x33, 0x3C, 0x3F,
//		0xC0, 0xC3, 0xCC, 0xCF, 0xF0, 0xF3, 0xFC, 0xFF};
	int cx, cy, cv;
	int px, py, x, y;
	u32 i0, i1, i2, i3;
	u32 pca, pcb, pc;
	u64 pxv;
	int i, j, k;

	if(!btesh2_gfxcon_conbuf)
		return(-1);
	if(!btesh2_gfxcon_con_dirty || btesh2_gfxcon_con_disabled)
		return(0);

	GfxDrv_PrepareFramebuf();

	for(cy=0; cy<btesh2_gfxcon_cbys; cy++)
		for(cx=0; cx<btesh2_gfxcon_cbxs; cx++)
	{
		cv=btesh2_gfxcon_conbuf[cy*btesh2_gfxcon_cbxs+cx];
//		if(!cv &&
//			(cx!=btesh2_gfxcon_con_xpos) &&
//			(cy!=btesh2_gfxcon_con_ypos))
//				continue;
//		if(!cv)
//		if(!(cv&0xF0000000))
//		if(!(cv&0xF0008000))
//			continue;

		if(!(cv&(BTESH2_GFXCON_DIRTY|BTESH2_GFXCON_BLINK)))
			continue;

//		btesh2_gfxcon_conbuf[cy*btesh2_gfxcon_cbxs+cx]=cv&0x0FFFFFFF;
		btesh2_gfxcon_conbuf[cy*btesh2_gfxcon_cbxs+cx]=
			cv&(~BTESH2_GFXCON_DIRTY);

		if((cv&BTESH2_GFXCON_GFXCELL)==BTESH2_GFXCON_GFXCELL)
		{
			pca=clrtab[(cv>>BTESH2_GFXCON_CLRBGSHIFT)&15];
			pcb=clrtab[(cv>>BTESH2_GFXCON_CLRFGSHIFT)&15];

			if(((cv>>BTESH2_GFXCON_CLRFGSHIFT)&255)==0x55)
			{
				pc=btesh2_gfxcon_aconbuf[cy*btesh2_gfxcon_cbxs+cx];
				if(pc)
				{
					pca=cram_tclr_rgb555(pc>>16);
					pcb=cram_tclr_rgb555(pc);
				}
			}
			
			for(py=0; py<4; py++)
				for(px=0; px<4; px++)
			{
				x=cx*8+px*2;
				y=cy*8+py*2;
				
				j=((3-py)*4)+(3-px);
				k=(cv>>j)&1;
				pc=k?pcb:pca;
				btesh2_gfxcon_framebuf[(y+0)*btesh2_gfxcon_fbxs+(x+0)]=pc;
				btesh2_gfxcon_framebuf[(y+0)*btesh2_gfxcon_fbxs+(x+1)]=pc;
				btesh2_gfxcon_framebuf[(y+1)*btesh2_gfxcon_fbxs+(x+0)]=pc;
				btesh2_gfxcon_framebuf[(y+1)*btesh2_gfxcon_fbxs+(x+1)]=pc;
			}

			continue;
		}

		pxv=btesh2_gfxcon_glyphs[cv&255];
//		pca=0xFF000000;
//		pcb=0xFF7F7F7F;
//		pcb=0xFFFFFFFF;

		i3=0;
		if(cv&BTESH2_GFXCON_REVERSE)
		{
#if 0
			if((cv&BTESH2_GFXCON_GFXCELL)==BTESH2_GFXCON_GFXCELL)
			{
				pca=clrtab[(cv>>BTESH2_GFXCON_CLRBGSHIFT)&15];
				pcb=clrtab[(cv>>BTESH2_GFXCON_CLRFGSHIFT)&15];

//				i0=lqbitab[(cv    )&15];
//				i1=lqbitab[(cv>> 4)&15];
//				i2=lqbitab[(cv>>20)&15];
//				i3=lqbitab[(cv>>24)&15];

				i0=lqbitab[(cv>>(BTESH2_GFXCON_ASCSHIFT+0))&15];
				i1=lqbitab[(cv>>(BTESH2_GFXCON_ASCSHIFT+4))&15];
				i2=lqbitab[(cv>>(BTESH2_GFXCON_FONTSHIFT+0))&15];
				i3=lqbitab[(cv>>(BTESH2_GFXCON_FONTSHIFT+4))&15];

				i0=i0|(i0<<8); i1=i1|(i1<<8);
				i2=i2|(i2<<8); i3=i3|(i3<<8);
				i0=i0|(i1<<16); i1=i2|(i3<<16);
				pxv=i0|(((u64)i1)<<32);
				i3=1;
			}else
			{
				pcb=clrtab[((cv>>BTESH2_GFXCON_CLRBGSHIFT)& 7)|
					((cv>>BTESH2_GFXCON_CLRFGSHIFT)&8)];
				pca=clrtab[ (cv>>BTESH2_GFXCON_CLRFGSHIFT)& 7];
			}
#else
			pcb=clrtab[((cv>>BTESH2_GFXCON_CLRBGSHIFT)& 7)|
				((cv>>BTESH2_GFXCON_CLRFGSHIFT)&8)];
			pca=clrtab[ (cv>>BTESH2_GFXCON_CLRFGSHIFT)& 7];
#endif
		}else
		{
			pca=clrtab[(cv>>BTESH2_GFXCON_CLRBGSHIFT)& 7];
			pcb=clrtab[(cv>>BTESH2_GFXCON_CLRFGSHIFT)&15];
		}
		
		if(btesh2_gfxcon_blink)
		{
			if(cv&BTESH2_GFXCON_BLINK)
//			if((cv&BTESH2_GFXCON_BLINK) && !i3)
			{
				pxv=0;
			}
			
			if(	(cx==btesh2_gfxcon_con_xpos) &&
				(cy==btesh2_gfxcon_con_ypos))
			{
				if(pca==pcb)
				{
					if(pcb!=0xFFFFFFFF)
						{ pcb=0xFFFFFFFF; }
					else
						{ pcb=0xFF555555; }
				}
				pxv|=0x000000000000FF00ULL;
			}
		}

//		if(!i3)
		if(1)
		{
			if(cv&BTESH2_GFXCON_UNDERLINE)
				{ pxv|=0x000000000000FF00ULL; }
			if(cv&BTESH2_GFXCON_STRIKEOUT)
				{ pxv|=0x000000FF00000000ULL; }
		}

//		if((cv&BTESH2_GFXCON_BLINK) && btesh2_gfxcon_blink)
		if(!pxv || (cv&BTESH2_GFXCON_ITALIC))
		{
			for(py=0; py<8; py++)
				for(px=0; px<8; px++)
			{
				x=cx*8+px; y=cy*8+py;
				btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=pca;
			}
			if(!(cv&BTESH2_GFXCON_ITALIC))
				continue;
		}

		for(py=0; py<8; py++)
			for(px=0; px<8; px++)
		{
			x=cx*8+px;
			y=cy*8+py;
			
			if(cv&BTESH2_GFXCON_ITALIC)
				{ x+=(py-4)/2; }
			
			j=((7-py)*8)+(7-px);
			k=(pxv>>j)&1;
			pc=k?pcb:pca;
			btesh2_gfxcon_framebuf[y*btesh2_gfxcon_fbxs+x]=pc;
		}
	}

	btesh2_gfxcon_con_dirty=0;
	return(0);
}

char *test_constr=
"\x1B!K[9xK[9xK[9xK[9xK[9xK[9JK[6kK[9TK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9x"
"K[9xAFJiA<mAA<mAAFI4K[9xK[9xK[9xK[9xMBDkMBD]MBD]MBDdK[9x0@0[\n"
"\x1B!K[9xK[9xK[9xK[9xK[9JKX_iK[9xKXqMK[9xK[9x>#1`=nW!=nW!=nW!>wxBK[9xK[9x"
"AEm/A<uiA=EAA=EAA=:7AAY@K[9xK[9xMBDYLo6UB^>xLr[:Lk!2MBC50@0[\n"
"\x1B!K[9xK[9xK[9x?_@.?xNG?_>_K[9xKZ'GK[9x>#1^=jVKA=EAA=EAA=B#=j#8K[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMBDkLoN]B^>LL(;TLwB#B^>]Lm7b2';c\n"
"\x1B!K[9xK[9xK[9J?[!p>b_A>c:IKZWWK[9xK[9x=l`RH.r;H.r;H.r;A;!r=nPcK[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMAfJB^>]L)^xL#5DLs+/L#O2Ls,x2wIZ\n"
"\x1B!K[9xK[9JKX_iK[9x?]VU?V>$K[9xK[9xK[9x=l`RH.r;H.r;H.r;A;!r=nPcK[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMAfJLrlALkl$Lq=$L&IOL&'JBWOG2wIZ\n"
"\x1B!K[69KVrnKZj&K[9xKZWWK[9xK[9xK[9xK[9x=l`RH.r;H.r;H.r;A;!r>wS,K[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMAixLltjLjT!L*!]L!LJLl=RLm7a2wc@\n"
"\x1B!KZ+*KV!/KX6`KZV,K[9xK[9xK[9xK[9xK[9x=uf'=o,?=o,?=o,?=u.oK[9xK[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xK[9xM@[;Lm7pLq=5B^>]Lm7aC%6I0@0[\n"
"\x1B!K[9xKUs`KSfsK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9x"
"K[9xA>GmA=B_A=B_A?h?K[9xK[9xK[9xK[9xMAixM9o&M9o&M=lAK[9x0@0[\n"

"\x1B[0m Normal: "
"\x1B[31mRed Text "
"\x1B[32mGreen Text "
"\x1B[34mBlue Text "
"\x1B[33mYellow Text "
"\x1B[35mViolet Text\n"

"\x1B[9m Strike: "
"\x1B[31mRed Text "
"\x1B[32mGreen Text "
"\x1B[34mBlue Text "
"\x1B[33mYellow Text "
"\x1B[35mViolet Text\n"
"\x1B[0m"

"\x1B[4m U-Line: "
"\x1B[31mRed Text "
"\x1B[32mGreen Text "
"\x1B[34mBlue Text "
"\x1B[33mYellow Text "
"\x1B[35mViolet Text\n"
"\x1B[0m"

"\x1B[7mReverse: "
"\x1B[31mRed Text "
"\x1B[32mGreen Text "
"\x1B[34mBlue Text "
"\x1B[33mYellow Text "
"\x1B[35mViolet Text\n"

"\x1B[5mR-Blink: "
"\x1B[31mRed Text "
"\x1B[32mGreen Text "
"\x1B[34mBlue Text "
"\x1B[33mYellow Text "
"\x1B[35mViolet Text\n"

"\x1B[0m"
;

int BTESH2_GfxCon_Startup()
{
	btesh2_gfxcon_fbxs=640;
	btesh2_gfxcon_fbys=480;
	btesh2_gfxcon_fbsz=btesh2_gfxcon_fbxs*btesh2_gfxcon_fbys;

	btesh2_gfxcon_cbxs=80;
	btesh2_gfxcon_cbys=60;
	btesh2_gfxcon_cbsz=btesh2_gfxcon_cbxs*btesh2_gfxcon_cbys;
//	btesh2_gfxcon_conbuf=malloc(80*61*sizeof(u32));
//	memset(btesh2_gfxcon_conbuf, 0, 80*61*sizeof(u32));

	btesh2_gfxcon_conbuf=malloc(2*80*61*sizeof(u32));
	memset(btesh2_gfxcon_conbuf, 0, 2*80*61*sizeof(u32));
	btesh2_gfxcon_aconbuf=btesh2_gfxcon_conbuf+(80*61);
	btesh2_gfxcon_cbsz2=2*80*61;

	btesh2_gfxcon_con_attr=0x0700;
	
//	BTESH2_GfxCon_PrintString("Console Test Init\n");

	BTESH2_GfxCon_PrintString(test_constr);
	return(0);
}
