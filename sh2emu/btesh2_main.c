#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mouse.h>
#include <SDL/SDL_keyboard.h>
#endif

#if defined(linux) || defined(__EMSCRIPTEN__)
#include <sys/time.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include "btesh2_multi.c"

byte kbbuf[256];
byte kbrov;
byte kbirov;
byte kbictrl;
u32 *btesh2_mmio;
int kirq;

u64 btesh2_msec;
u64 btesh2_tops;
double btesh2_ips;

#ifdef __linux
static struct termios old_termios;

void btesh2_ttynoncanon(void)
{
	struct termios new_termios;

	tcgetattr(0, &old_termios);
	new_termios=old_termios;
	new_termios.c_lflag&=(~ICANON);
	new_termios.c_lflag&=(~ECHO);
	new_termios.c_cc[VTIME]=0;
	new_termios.c_cc[VMIN]=1;
	tcsetattr(0, TCSANOW, &new_termios);
	return;
}

void btesh2_resettermios(void)
{
	tcsetattr(0, TCSANOW, &old_termios);
	return;
}
#endif

int FRGL_TimeMS()
{
#ifdef _WIN32
	static unsigned int init;
	unsigned int t;

	t=timeGetTime();
	if(!init)init=t;

	return((unsigned int)(t-init));
#else

#ifdef __EMSCRIPTEN__
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif

#ifndef linux
	static int init;
	int t;

	t=clock();
	t*=CLOCKS_PER_SEC/1000.0;
//	t=FRGL_TimeMS();

	if(!init)init=t;

	return((unsigned int)(t-init));
#endif
#ifdef linux
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif
#endif
}

#ifdef __EMSCRIPTEN__
static byte frgl_keymap[32];
static byte frgl_lkeymap[32];
static unsigned short frgl_keybuf2[64];
static int frgl_keybuf2_pos;
byte gfxdrv_lastkeys[2048];

static int keyb_num_shift[10]={
')', '!', '@', '#', '$', '%', '^', '&', '*', '('
};

u16 *FRGL_GetKeybuf()
{
	static u16 frgl_keybuf3[64];
	u16 *buf;

	buf=frgl_keybuf3;
	memcpy(buf, frgl_keybuf2, 128);

	frgl_keybuf2_pos=0;
	frgl_keybuf2[0]=0;

//	printf("buf %p\n", buf);

	return(buf);
}

void FRGL_EndInputFrame()
{
	memcpy(frgl_lkeymap, frgl_keymap, 32);
}

int FRGL_KeyDown(int key)
{
	if(frgl_keymap[key>>3]&(1<<(key&7)))return(1);
	return(0);
}

int GfxDev_Key_Event(int key, int down)
{
	static int skm=0, skmd=0;
	int akey, akey2, nskm;
//	keyhandler *cur;

	akey=key;
	if(FRGL_KeyDown(K_SHIFT))
	{
		if(key>='a' && key<='z')
			akey=key-'a'+'A';
		if(key>='0' && key<='9')
			akey=keyb_num_shift[key-'0'];

		if(key=='\'')akey='"';
		if(key=='[')akey='{';
		if(key==']')akey='}';
		if(key=='-')akey='_';
		if(key=='=')akey='+';
		if(key==';')akey=':';
		if(key==',')akey='<';
		if(key=='.')akey='>';
		if(key=='/')akey='?';
		if(key=='`')akey='~';
		if(key=='\\')akey='|';
	}

	nskm=0;
	if(FRGL_KeyDown(K_CTRL))
	{
		switch(akey)
		{
		case K_F1: nskm=K_SKM_F1; break;
		case K_F2: nskm=K_SKM_F2; break;
		case K_F3: nskm=K_SKM_F3; break;
		case K_F4: nskm=K_SKM_F4; break;
		case K_F5: nskm=K_SKM_F5; break;
		case K_F6: nskm=K_SKM_F6; break;
		case K_F7: nskm=K_SKM_F7; break;
		case K_F8: nskm=K_SKM_F8; break;
		case K_F9: nskm=K_SKM_F9; break;
		case K_F10: nskm=K_SKM_F10; break;
		case K_F11: nskm=K_SKM_F11; break;
		case K_F12: nskm=K_SKM_F12; break;
		default: break;
		}
	}else if(!skm)
	{
		if(!FRGL_KeyDown(K_CTRL) && !FRGL_KeyDown(K_ALT) &&
			!FRGL_KeyDown(K_SHIFT))
		{
			if(akey==K_ESC)
				nskm=K_SKM_ESC;
		}
	}

	if(down)
	{
		akey2=akey;
		if(skm && (akey>=' ') && (akey<='~'))
		{
			akey2=akey|skm;
			skmd=skm;
//			skm=0;
		}
	
		frgl_keymap[akey>>3]|=1<<(akey&7);
		if(frgl_keybuf2_pos<63)
		{
			frgl_keybuf2[frgl_keybuf2_pos++]=akey2;
			frgl_keybuf2[frgl_keybuf2_pos]=0;
		}

//		cur=frgl_keyhandlers;
//		while(cur)
//		{
//			cur->func(akey2, 1);
//			cur=cur->next;
//		}
	}else
	{
		akey2=akey;
		if(skm && skmd && (akey>=' ') && (akey<='~'))
		{
			akey2=akey|skm;
			skm=0; skmd=0;
		}
	
		frgl_keymap[akey>>3]&=~(1<<(akey&7));
		if(frgl_keybuf2_pos<63)
		{
			frgl_keybuf2[frgl_keybuf2_pos++]=akey2|0x8000;
			frgl_keybuf2[frgl_keybuf2_pos]=0;
		}

//		cur=frgl_keyhandlers;
//		while(cur)
//		{
//			cur->func(akey2, 0);
//			cur=cur->next;
//		}
	}
	
	if(nskm)
		{ skm=nskm; }
	return(0);
}

SDL_Surface *frgl_main_screen;

#if 0
static int scantokey[256]=
{
	0, K_ESC, '1','2','3','4','5','6', 
	'7','8','9','0','-','=', K_BACKSPACE, 9,
	'q','w','e','r','t','y','u','i', 
	'o','p','[',']',13 , K_CTRL, 'a','s',
	'd','f','g','h','j','k','l',';', 
	'\'', '`', K_SHIFT,'\\', 'z','x','c','v',
	'b','n','m',',','.',K_NUMPAD_DIV, K_SHIFT,'*', 
	K_ALT, ' ', K_CAPSLOCK, K_F1, K_F2, K_F3, K_F4, K_F5,
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, K_SCROLL,  K_HOME, 
	K_UPARROW, K_PGUP, '-', K_LEFTARROW,'5', K_RIGHTARROW, '+', K_END,
	K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, 0,
	K_F11, 
	K_F12, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
}; 

static int scantokeyb[256]=
{
	0, K_ESC, '1','2','3','4','5','6', 
	'7','8','9','0','-','=', K_BACKSPACE, 9,
	'q','w','e','r','t','y','u','i', 
	'o','p','[',']',13 , K_CTRL, 'a','s',
	'd','f','g','h','j','k','l',';', 
	'\'', '`', K_SHIFT,'\\', 'z','x','c','v',
	'b','n','m',',','.','/', K_SHIFT, K_NUMPAD_MUL, 
	K_ALT, ' ', 0, K_F1, K_F2, K_F3, K_F4, K_F5,
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, 0,  K_NUMPAD7, 
	K_NUMPAD8, K_NUMPAD9, K_NUMPAD_SUB, K_NUMPAD4, K_NUMPAD5, K_NUMPAD6,
	K_NUMPAD_ADD, K_NUMPAD1,
	K_NUMPAD2, K_NUMPAD3, K_NUMPAD0, K_DEL, 0, 0, 0,
	K_F11, 
	K_F12, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0
}; 
#endif

static int scantokey[256]=
{
  0,    0,    0,    0,  'a',  'b',  'c',  'd', //  0-  7
'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l', //  8- 15
'm',  'n',  'o',  'p',  'q',  'r',  's',  't', // 16- 23
'u',  'v',  'w',  'x',  'y',  'z',  '1',  '2', // 24- 31
'3',  '4',  '5',  '6',  '7',  '8',  '9',  '0', // 32- 39
 13,   27,    8,    9,  ' ',  '-',  '=',  '[', // 40- 47
']', '\\', '\\',  ';',  '`', '\'',  ',',  '.', // 48- 55
'/',  160,  135,  136,  137,  138,  139,  140, // 56- 63
141,  142,  143,  144,  145,  146,    0,  161, // 64- 71
153,  147,  151,  150,  127,  152,  149,  131, // 72- 79
130,  129,  128,    0,  207,  206,  205,  204, // 80- 87
218,  209,  210,  211,  212,  213,  214,  215, // 88- 95
216,  217,  208,  219,  '`',    0,    0,  '=', // 96-103
  0,    0,    0,    0,    0,    0,    0,    0, //104-111
  0,    0,    0,    0,    0,    0,    0,    0, //112-119
  0,    0,    0,    0,    0,    0,    0,    0, //120-127
  0,    0,    0,    0,    0,    0,    0,    0, //128-135
  0,    0,    0,    0,    0,    0,    0,    0, //136-143
  0,    0,    0,    0,    0,    0,    0,    0, //144-151
  0,    0,    0,    0,    0,    0,    0,    0, //152-159
  0,    0,    0,    0,    0,    0,    0,    0, //160-167
  0,    0,    0,    0,    0,    0,    0,    0, //168-175
  0,    0,    0,    0,    0,    0,    0,    0, //176-183
  0,    0,  ';',  '=',    0,  '-',    0,    0, //184-191
  0,    0,    0,    0,    0,    0,    0,    0, //192-199
  0,    0,    0,    0,    0,    0,    0,    0, //200-207
  0,    0,    0,    0,    0,    0,    0,    0, //208-215
  0,    0,    0,    0,    0,    0,    0,    0, //216-223
133,  134,  132,    0,  133,  134,  132,    0, //224-231
};

int GfxDrv_MapKey(int key)
{
	int al, val;

//	frgl_printf("Key: 0x%08X\n", key);

	if(key>255)
		return(0);
	val=scantokey[key];
	return(val);
}

void GfxDrv_UpdateKeyboard()
{
	byte *keys;
	int i;

#if 0
	keys=SDL_GetKeyboardState(NULL);
	if(!keys)
		return;
	
//	for(i=0; i<256; i++)
//	for(i=0; i<1024; i++)
	for(i=0; i<2048; i++)
	{
		if(keys[i] && !gfxdrv_lastkeys[i])
		{
			printf("keydown %02X\n", i);
//			GfxDev_Key_Event(i, 1);
			gfxdrv_lastkeys[i]=1;
		}else if(!keys[i] && gfxdrv_lastkeys[i])
		{
			printf("keyup %02X\n", i);
//			GfxDev_Key_Event(i, 0);
			gfxdrv_lastkeys[i]=0;
		}
	}
#endif
}

void GfxDrv_UpdateEvents()
{
	SDL_Event ev;
	int i, j;

	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
		case SDL_KEYDOWN:
			i=ev.key.keysym.scancode;
			j=GfxDrv_MapKey(i);
			if(!j)printf("EV Key Dn %02X %02X\n", i, j);
			GfxDev_Key_Event(j, 1);
			break;
		case SDL_KEYUP:
			i=ev.key.keysym.scancode;
			j=GfxDrv_MapKey(i);
			if(!j)printf("EV Key Up %02X %02X\n", i, j);
			GfxDev_Key_Event(j, 0);
			break;
		default:
			break;
		}
	}
}

int gfxdrv_locksurf=0;

void GfxDrv_BeginDrawing()
{
	if(SDL_MUSTLOCK(frgl_main_screen) && gfxdrv_locksurf)
	{
		SDL_UnlockSurface(frgl_main_screen);
		gfxdrv_locksurf=0;
	}
	SDL_Flip(frgl_main_screen);

	SDL_PumpEvents();
	GfxDrv_UpdateKeyboard();
	GfxDrv_UpdateEvents();

	if(SDL_MUSTLOCK(frgl_main_screen))
	{
		SDL_LockSurface(frgl_main_screen);
		gfxdrv_locksurf=1;
	}

	btesh2_gfxcon_framebuf=(u32 *)frgl_main_screen->pixels;
	
	BTESH2_GfxCon_BlinkState((FRGL_TimeMS()>>9)&1);
	BTESH2_GfxCon_Redraw();
}

char *test_constr=
"\x1B%K[9xK[9xK[9xK[9xK[9xK[9JK[6kK[9TK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9x"
"K[9xAFJiA<mAA<mAAFI4K[9xK[9xK[9xK[9xMBDkMBD]MBD]MBDdK[9x0@0[\n"
"\x1B%K[9xK[9xK[9xK[9xK[9JKX_iK[9xKXqMK[9xK[9x>#1`=nW!=nW!=nW!>wxBK[9xK[9x"
"AEm/A<uiA=EAA=EAA=:7AAY@K[9xK[9xMBDYLo6UB^>xLr[:Lk!2MBC50@0[\n"
"\x1B%K[9xK[9xK[9x?_@.?xNG?_>_K[9xKZ'GK[9x>#1^=jVKA=EAA=EAA=B#=j#8K[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMBDkLoN]B^>LL(;TLwB#B^>]Lm7b2';c\n"
"\x1B%K[9xK[9xK[9J?[!p>b_A>c:IKZWWK[9xK[9x=l`RH.r;H.r;H.r;A;!r=nPcK[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMAfJB^>]L)^xL#5DLs+/L#O2Ls,x2wIZ\n"
"\x1B%K[9xK[9JKX_iK[9x?]VU?V>$K[9xK[9xK[9x=l`RH.r;H.r;H.r;A;!r=nPcK[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMAfJLrlALkl$Lq=$L&IOL&'JBWOG2wIZ\n"
"\x1B%K[69KVrnKZj&K[9xKZWWK[9xK[9xK[9xK[9x=l`RH.r;H.r;H.r;A;!r>wS,K[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xMAixLltjLjT!L*!]L!LJLl=RLm7a2wc@\n"
"\x1B%KZ+*KV!/KX6`KZV,K[9xK[9xK[9xK[9xK[9x=uf'=o,?=o,?=o,?=u.oK[9xK[9xK[9x"
"AEm/H.r;H.r;H.r;H.r;AAY@K[9xK[9xM@[;Lm7pLq=5B^>]Lm7aC%6I0@0[\n"
"\x1B%K[9xKUs`KSfsK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9xK[9x"
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

void GfxDrv_Start()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	frgl_main_screen =
		SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
//		SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);
	btesh2_gfxcon_fbxs=640;
	btesh2_gfxcon_fbys=480;
	btesh2_gfxcon_fbsz=btesh2_gfxcon_fbxs*btesh2_gfxcon_fbys;

	btesh2_gfxcon_cbxs=80;
	btesh2_gfxcon_cbys=60;
	btesh2_gfxcon_cbsz=btesh2_gfxcon_cbxs*btesh2_gfxcon_cbys;
	btesh2_gfxcon_conbuf=malloc(80*61*sizeof(u32));
	memset(btesh2_gfxcon_conbuf, 0, 80*61*sizeof(u32));
	btesh2_gfxcon_con_attr=0x0700;
	
//	BTESH2_GfxCon_PrintString("Console Test Init\n");

	BTESH2_GfxCon_PrintString(test_constr);
}
#endif

byte *loadfile(char *path, int *rsz)
{
	byte *buf;
	FILE *fd;
	int sz, i;
	
	fd=fopen(path, "rb");
	if(!fd)
		return(NULL);

	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz);
	i=fread(buf, 1, sz, fd);
	fclose(fd);
	
	*rsz=sz;
	return(buf);
}

s64 btesh2_getvtime_ns(BTESH2_CpuState *cpu)
{
	static s64 lt0=0;
	s64 t0, t1;
	t0=btesh2_msec*1000000LL;
	t1=(cpu->tr_tops-btesh2_tops)*(1000000000.0/btesh2_ips);
	t0=t0+t1;
	if(t0<=lt0)
		t0=lt0+1;
	lt0=t0;
	return(t0);
}

u32 btesh2_spanmmio_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 *mmio;
	s64 t0, t1;
	u32 i;
	
	mmio=(u32 *)(sp->data);
	switch(reladdr>>2)
	{
//	case 0: i=mmio[0]; break;
//	case 1: i=mmio[1]; break;
//	case 2: i=mmio[2]; break;
//	case 3: i=mmio[3]; break;
	
	case 0x10:
		i=mmio[0x10];
//		printf("SPI_C(R): %08X\n", i);
		break;
	case 0x11:
		i=mmio[0x11];
//		printf("SPI_D(R): %08X\n", i);
		break;

#if 1
	case 0x40:
		i=0;
		if(kbirov!=kbrov)
		{
			i=kbbuf[kbirov++];
//			printf("%c", i);
		}
		break;
//	case 0x41: i=0; break;
	case 0x42:
		i=0;
		if(kbirov!=kbrov)
		{
//			printf(".");
			i|=1;
		}
		i|=4;
		if(kbictrl&0x10)
			i|=0x10;
		break;
	case 0x43: i=0; break;
#endif

//	case 0x80: i=mmio[0x80]; break;
//	case 0x81: i=mmio[0x81]; break;
//	case 0x82: i=mmio[0x82]; break;
//	case 0x83: i=mmio[0x83]; break;
//	case 0x84: i=mmio[0x84]; break;
	case 0x85:
		i=mmio[0x85];
//		i=cpu->tr_tops&((1<<20)-1);
		break;
	case 0x86:
		i=mmio[0x86];
//		i=1<<20;
		break;
//	case 0x87: i=mmio[0x87]; break;
	case 0x88:	i=0; break;
	case 0x89:
//		t0=(btesh2_msec*1024*8)+(cpu->tr_tops&8191);
//		t0=btesh2_msec*1024*8;
//		t0=btesh2_msec*1000000LL;
//		t1=(cpu->tr_tops-btesh2_tops)*(1000000000.0/btesh2_ips);
//		t0=t0+t1;

		t0=btesh2_getvtime_ns(cpu);

		i=t0>>30;
//		i=t0>>32;
//		i=(cpu->tr_tops>>30);
		break;
	case 0x8A:
//		t0=btesh2_msec*1024*10;
//		t0=(btesh2_msec*1024*16)+(cpu->tr_tops&16383);
//		t0=(btesh2_msec*1024*8)+(cpu->tr_tops&8191);

//		t0=btesh2_msec*1024*8;
//		t0=btesh2_msec*1000000LL;
//		t1=(cpu->tr_tops-btesh2_tops)*(1000000000.0/btesh2_ips);
//		t0=t0+t1;

		t0=btesh2_getvtime_ns(cpu);

		i=t0&((1<<30)-1);
//		i=(cpu->tr_tops<<2);
		break;
	
//	case 0x85:
//	case 0x86:
//		break;

	case 0xC0:	case 0xC1:
	case 0xC2:	case 0xC3:
		i=0; break;
	case 0x100:	case 0x101:
	case 0x102:	case 0x103:
		i=0; break;

//	default: i=-1; break;
	default:
		printf("MMIO_Read: %04X\n", reladdr>>2);
		i=0; break;
	}
	
	return(i);
}

int btesh2_spanmmio_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int v;
	u32 *mmio;

	mmio=(u32 *)(sp->data);
	switch(reladdr>>2)
	{
	case 0:
		printf("GPIO_V: %08X\n", val);
		mmio[0]=val;
		break;
	case 1:
		printf("GPIO_M: %08X\n", val);
		mmio[1]=val;
		break;
	case 2:
		printf("GPIO_E: %08X\n", val);
		mmio[2]=val;
		break;
	case 3:
		printf("GPIO_C: %08X\n", val);
		mmio[3]=val;
		break;
	
	case 0x10:
		printf("SPI_C(W): %08X\n", val);
		v=btesh2_spimmc_XrCtl(cpu, val);
		mmio[0x10]=v&255;
		if(v&0x10000)
			mmio[0x11]=(v>>8)&255;
		break;
	case 0x11:
		printf("SPI_D(W): %08X\n", val);
		v=btesh2_spimmc_XrByte(cpu, val);
		mmio[0x11]=v;
		break;
	
	case 0x41:
//		printf("Tx: %c\n", val);
		printf("%c", val);
		BTESH2_GfxCon_PrintChar(val);
		fflush(stdout);
		if(!kirq)
			kirq++;
		break;
//	case 0x42:
//		break;
	case 0x43:
		kbictrl=val;
		break;

	case 0x30:
		break;


	case 0xC0:	case 0xC1:
	case 0xC2:	case 0xC3:
		break;
	case 0x100:	case 0x101:
	case 0x102:	case 0x103:
		break;

	default:
		printf("MMIO_Write: %04X %08X\n", reladdr>>2, val);
		break;
	}

	return(0);
}

u32 btesh2_spanmmio_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_spanmmio_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(sbyte)j;
	return(j);
}

u32 btesh2_spanmmio_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_spanmmio_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(s16)j;
	return(j);
}

int btesh2_spanmmio_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=btesh2_spanmmio_SetD(sp, cpu, reladdr, val);
	return(i);
}

int btesh2_spanmmio_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=btesh2_spanmmio_SetD(sp, cpu, reladdr, val);
	return(i);
}

u32 btesh2_spanmmreg_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 v;

	switch(reladdr>>2)
	{
	case 0: v=cpu->regs[BTESH2_REG_PTEH]; break;
	case 1: v=cpu->regs[BTESH2_REG_PTEL]; break;
	case 2: v=cpu->regs[BTESH2_REG_TTB]; break;
	case 3: v=cpu->regs[BTESH2_REG_TEA]; break;
	case 4: v=cpu->regs[BTESH2_REG_MMUCR]; break;
	default:
		printf("MMREG: Unhandled Get A=%08X\n", reladdr);
		v=0; break;
	}
	return(v);
}

int btesh2_spanmmreg_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	switch(reladdr>>2)
	{
	case 0: cpu->regs[BTESH2_REG_PTEH]=val; break;
	case 1: cpu->regs[BTESH2_REG_PTEL]=val; break;
	case 2: cpu->regs[BTESH2_REG_TTB]=val; break;
	case 3: cpu->regs[BTESH2_REG_TEA]=val; break;
	case 4: cpu->regs[BTESH2_REG_MMUCR]=val; break;
	default:
		printf("MMREG: Unhandled Set A=%08X V=%08X\n", reladdr, val);
		break;
	}
	return(0);
}


u32 btesh2_spanemac_GetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	printf("EMAC Get\n");
	return(0);
}

int btesh2_spanemac_SetD(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	printf("EMAC Set\n");
	return(0);
}

u32 btesh2_spanemac_GetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_spanemac_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(sbyte)j;
	return(j);
}

u32 btesh2_spanemac_GetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr)
{
	u32 i, j;
	i=btesh2_spanemac_GetD(sp, cpu, reladdr&(~3));
	j=(i>>((3-(reladdr&3))*8));
	j=(s16)j;
	return(j);
}

int btesh2_spanemac_SetB(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=btesh2_spanemac_SetD(sp, cpu, reladdr, val);
	return(i);
}

int btesh2_spanemac_SetW(BTESH2_PhysSpan *sp,
	BTESH2_CpuState *cpu, u32 reladdr, u32 val)
{
	int i;
	i=btesh2_spanemac_SetD(sp, cpu, reladdr, val);
	return(i);
}


int help(char *prgname)
{
	printf("usage: %s [options] image\n", prgname);
	printf("  -sh2        Emulate SH-2\n");
	printf("  -sh4        Emulate SH-4\n");
	printf("  -map <map>  Use symbol map\n");
	printf("  -sd <image> Use SD image\n");
	printf("  -sdcl <scr> Use SDCL script\n");
	printf("  -ird <img>  Use Initrd image\n");
	return(0);
}

BTESH2_CpuState *btesh2_cpu;
int t0, t1, t2, t3;
double dt;
s64 tdt;
s64 rtops;
s32 rtmsec;
// byte sh4, kirq;
byte sh4;
int sz, err, ts;
int kb_ztick;

void frgl_main_stuffkeyb(char *str)
{
	char *s;
	int j;
	
	s=str;
	while(*s)
	{
		j=*s++;
		kbbuf[kbrov]=j;
		kbrov=(kbrov+1)&255;
		kirq=1;
	}
}

void frgl_main_iterate()
{
	BTESH2_CpuState *cpu;
	u16 *kb;
	byte *kb2;
	char *str;
	int i, j, k, l;

#ifdef __EMSCRIPTEN__
	GfxDrv_BeginDrawing();
	kb=FRGL_GetKeybuf();
	while(kb && *kb)
	{
		j=*kb++;

//		printf("%04X\n", j);

		if(j&0x8000)
			continue;

//		if(j==K_UPARROW)
//			{ frgl_main_stuffkeyb("\x1B[A"); continue; }
//		if(j==K_DOWNARROW)
//			{ frgl_main_stuffkeyb("\x1B[B"); continue; }
//		if(j==K_RIGHTARROW)
//			{ frgl_main_stuffkeyb("\x1B[C"); continue; }
//		if(j==K_LEFTARROW)
//			{ frgl_main_stuffkeyb("\x1B[D"); continue; }

		str=NULL;
		switch(j)
		{
		case K_F1:		str="\x1B""11~"; break;
		case K_F2:		str="\x1B""12~"; break;
		case K_F3:		str="\x1B""13~"; break;
		case K_F4:		str="\x1B""14~"; break;
		case K_F5:		str="\x1B""15~"; break;
		case K_F6:		str="\x1B""17~"; break;
		case K_F7:		str="\x1B""18~"; break;
		case K_F8:		str="\x1B""19~"; break;
		case K_F9:		str="\x1B""20~"; break;
		case K_F10:		str="\x1B""21~"; break;
		case K_F11:		str="\x1B""23~"; break;
		case K_F12:		str="\x1B""24~"; break;
		}

		if(FRGL_KeyDown(K_SHIFT))
		{
			switch(j)
			{
			case K_UPARROW:			str="\x1B[1;2A"; break;
			case K_DOWNARROW:		str="\x1B[1;2B"; break;
			case K_RIGHTARROW:		str="\x1B[1;2C"; break;
			case K_LEFTARROW:		str="\x1B[1;2D"; break;
			case K_END:				str="\x1B[1;2F"; break;
			case K_HOME:			str="\x1B[1;2H"; break;
			case K_DEL:				str="\x1B[3;2~"; break;
			case K_INS:				str="\x1B[2;2~"; break;
			case K_PGDN:			str="\x1B[6;2~"; break;
			case K_PGUP:			str="\x1B[5;2~"; break;
			default: break;
			}
		}else
		{
			switch(j)
			{
			case K_UPARROW:			str="\x1B[A"; break;
			case K_DOWNARROW:		str="\x1B[B"; break;
			case K_RIGHTARROW:		str="\x1B[C"; break;
			case K_LEFTARROW:		str="\x1B[D"; break;
			case K_END:				str="\x1B[F"; break;
			case K_HOME:			str="\x1B[H"; break;
			case K_DEL:				str="\x1B[3~"; break;
			case K_INS:				str="\x1B[2~"; break;
			case K_PGDN:			str="\x1B[6~"; break;
			case K_PGUP:			str="\x1B[5~"; break;
			default: break;
			}
		}

		if(str)
			{ frgl_main_stuffkeyb(str); continue; }
		
		if(j>'~')
			continue;

//		printf("%c\n", j);

//		kbbuf[kbrov]=j;
//		kbrov=(kbrov+1)&255;
//		kirq=1;

//		if(j=='\r')
//			j=0x0A0D;

		if(j=='\r')
			j='\n';

		if(FRGL_KeyDown(K_CTRL))
		{
			if((j>='a') && (j<='z'))
				{ j=1+(j-'a'); }
			else if(j=='[')j=0x1B;
			else if(j=='\\')j=0x1C;
			else if(j==']')j=0x1D;
			else if(j=='^')j=0x1E;
			else if(j=='_')j=0x1F;
		}

//		printf("K%c\n", j);
		kbbuf[kbrov]=j;
		kbrov=(kbrov+1)&255;
		kirq=1;
		
		if(j>>8)
		{
			kbbuf[kbrov]=j;
			kbrov=(kbrov+1)&255;
			kirq++;
		}
	}
	
	frgl_main_stuffkeyb((char *)btesh2_gfxcon_con_ansret);
	btesh2_gfxcon_con_ansret[0]=0;
#endif
	
	cpu=btesh2_cpu;

//	t1=clock();
	t1=FRGL_TimeMS();

	t2=t1-t3;
	if(t2>0)
	{
		t3=t1;
//		dt=t2*(1000.0/CLOCKS_PER_SEC);
		dt=t2;
		btesh2_msec+=dt;
		rtmsec+=dt;
	}

	t2=t1-t0;
	if((t2>ts) || (t2<0))
	{
#ifndef __EMSCRIPTEN__
		j=fgetc(stdin);
		if(j>0)
		{
			if(j=='`')
			{
//				err=1;
				return;
			}
			
			kbbuf[kbrov]=j;
			kbrov=(kbrov+1)&255;
			kirq=1;
		}
#endif

#if 0
		kb_ztick++;
		if(kb_ztick>16)
		{
//			kbbuf[kbrov]=0;
//			kbrov=(kbrov+1)&255;
			kirq++;

			kb_ztick=0;
		}
#endif

		tdt+=t2;
		t0=t1;
		BTESH2_CpuTimerInt(cpu);
//		return;
	}

#if 1
	t2=t1-kb_ztick;
	if((t2<0) || (t2>=250))
	{
		kirq++;
		kb_ztick=t1;
	}
#endif

	btesh2_tops=cpu->tr_tops-rtops;
	
	if(btesh2_tops>0)
	{
//		dt=rtmsec*(1.0/((double)CLOCKS_PER_SEC))+0.000001;
		dt=rtmsec*(1.0/1000.0)+0.000001;
		btesh2_ips=((double)btesh2_tops)/dt+0.000001;
	}else
	{
		btesh2_ips=999999999.0;
	}

	if((kirq>0) && (kbictrl&0x10))
	{
		BTESH2_CpuUartInt(cpu);
		kirq--;
	}

	if(t2)
	{
		btesh2_mmio[0x85]--;
		if(((s32)btesh2_mmio[0x85])<=0)
		{
			BTSH_Op_TrapIntIrq(cpu, BTESH2_EXC_AICCNTDN);
			btesh2_mmio[0x85]=16;
			btesh2_mmio[0x86]=4095;
		}
	}

	err=BTESH2_RunCpu(cpu, 10000);
//	err=BTESH2_RunCpu(cpu, 100000);
	
	if(err==BTESH2_EXC_TRAPSLEEP)
	{
		cpu->status=0;
		err=0;
		return;
	}
	
	if(err)
	{
		printf("Fault=%X\n", err);
	}
}

int main(int argc, char *argv[])
{
	BTESH2_MemoryImage *img;
	BTESH2_CpuState *cpu;
	BTESH2_PhysSpan *sp;
	byte *ibuf, *tbuf;
	char *imgname, *mapname, *sdname, *sdclname, *irdname;
	char *kerninit;
//	double dt;
//	s64 tdt;
//	s64 rtops;
//	s32 rtmsec;
//	byte sh4, kirq;
//	int t0, t1, t2, t3;
//	int sz, err, ts;
	int i, j, k, l;
	
	sh4=0;
	imgname=NULL;
	mapname=NULL;
	sdname=NULL;
	sdclname=NULL;
	irdname=NULL;
	kerninit=NULL;
	
	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-sh4"))
				{ sh4=BTESH2_ARCH_SH4; continue; }
			if(!strcmp(argv[i], "-sh2"))
				{ sh4=BTESH2_ARCH_SH2; continue; }

			if(!strcmp(argv[i], "-map"))
				{ mapname=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "-sd"))
				{ sdname=argv[i+1]; i++; continue; }
			if(!strcmp(argv[i], "-sdcl"))
				{ sdclname=argv[i+1]; i++; continue; }
			if(!strcmp(argv[i], "-ird"))
				{ irdname=argv[i+1]; i++; continue; }

			if(!strcmp(argv[i], "--help"))
				{ sh4=127; continue; }
				
			printf("unrecognized option %s\n", argv[i]);
			continue;
		}
		
		if(!imgname)
			imgname=argv[i];
	}
	
	if(sh4==127)
	{
		help(argv[0]);
		return(0);
	}

	if(sdclname)
	{
		ibuf=loadfile(sdclname, &sz);
		if(ibuf)
		{
			BTESH2_ProcessSDCL((char *)ibuf, sz);
			free(ibuf);
		}
		
		if(!imgname)
			return(0);
	}

#ifdef __EMSCRIPTEN__
	if(!imgname)
	{
		imgname="vmlinux";
//		imgname="testkern_sh4.elf";
//		sh4=BTESH2_ARCH_SH4;
	}
#endif

	if(!imgname)
	{
		printf("No image name given\n");
		return(-1);
	}

	if(sdname)
	{
		BTESH2_SetUseImage(sdname);

//		ibuf=loadfile(sdname, &sz);
//		if(ibuf)
//		{
//			printf("Loaded MMC image, %dMB\n", (sz>>20));
//			BTESH2_SPIMMC_SetImage(ibuf, sz);
//		}
	}
	
	/* Memory Map in SH4 Mode
	 * 0BCD0000: Peripheral
	 */
	
	if(sh4==BTESH2_ARCH_SH4)
	{
#if 0
		if(!imgname)
		{
			imgname="vmlinux_sh4";
			mapname="System_sh4.map";
		}
#endif

		img=BTESH2_AllocMemoryImage(0);
//		BTESH2_MemoryDefineSpan(img, 0x00000000, 0x00003FFF, NULL, "SRAM");
//		BTESH2_MemoryDefineSpan(img, 0x00000000, 0x0000FFFF, NULL, "SRAM");
//		BTESH2_MemoryDefineSpan(img, 0x10000000, 0x17FFFFFF, NULL, "DRAM");

		BTESH2_MemoryDefineSpan(img, 0x0C000000, 0x17FFFFFF, NULL, "DRAM");

		BTESH2_MemoryDefineSpan(img, 0xF4000000, 0xF400FFFF, NULL, "OCA");
		BTESH2_MemoryDefineSpan(img, 0xF5000000, 0xF500FFFF, NULL, "OCD");
		BTESH2_MemoryDefineSpan(img, 0xF6000000, 0xF600FFFF, NULL, "UTLBA");
		BTESH2_MemoryDefineSpan(img, 0xF7000000, 0xF700FFFF, NULL, "UTLBD");

		cpu=BTESH2_AllocCpuState();
		cpu->memory=img;
		cpu->arch=BTESH2_ARCH_SH4;

		cpu->GetAddrByte=BTESH2_GetAddrByteFMMU;
		cpu->GetAddrWord=BTESH2_GetAddrWordFMMU;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordFMMU;
		cpu->SetAddrByte=BTESH2_SetAddrByteFMMU;
		cpu->SetAddrWord=BTESH2_SetAddrWordFMMU;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordFMMU;

	//	cpu->regs[BTESH2_REG_VBR]=0xFAFAFAFAU;
		cpu->regs[BTESH2_REG_SR]=0x000003F3U;

		cpu->regs[BTESH2_REG_SP]=0x8C00FFFC;
//		cpu->regs[BTESH2_REG_SP]=0x0000FFFC;
	}else
	{
		img=BTESH2_AllocMemoryImage(0);
	//	BTESH2_MemoryDefineSpan(img, 0x00000000, 0x00003FFF, NULL, "SRAM");
		BTESH2_MemoryDefineSpan(img, 0x00000000, 0x0000FFFF, NULL, "SRAM");
		BTESH2_MemoryDefineSpan(img, 0x10000000, 0x17FFFFFF, NULL, "DRAM");

		cpu=BTESH2_AllocCpuState();
		cpu->memory=img;
		cpu->arch=BTESH2_ARCH_SH2;

#if 0
		if(!imgname)
		{
//			imgname="boot.elf";
//			mapname=NULL;

			imgname="vmlinux";
			mapname="System.map";
		}
#endif

	//	cpu->regs[BTESH2_REG_VBR]=0xFAFAFAFAU;
		cpu->regs[BTESH2_REG_SR]=0x000003F3U;

		cpu->regs[BTESH2_REG_SP]=0x0000FFFC;
	//	cpu->regs[BTESH2_REG_SP]=0x00003FFC;
	//	cpu->regs[BTESH2_REG_PC]=0x00003FFC;

		cpu->GetAddrByte=BTESH2_GetAddrBytePhy;
		cpu->GetAddrWord=BTESH2_GetAddrWordPhy;
		cpu->GetAddrDWord=BTESH2_GetAddrDWordPhy;
		cpu->SetAddrByte=BTESH2_SetAddrBytePhy;
		cpu->SetAddrWord=BTESH2_SetAddrWordPhy;
		cpu->SetAddrDWord=BTESH2_SetAddrDWordPhy;
	}

	if(1)
	{
		tbuf=malloc(16384);
		memset(tbuf, 0, 16384);
		btesh2_mmio=(u32 *)(tbuf);
		
		sp=BTESH2_AllocPhysSpan();
		sp->base=0xABCD0000;	sp->limit=0xABCD3FFF;
		if(cpu->arch==BTESH2_ARCH_SH4)
			{ sp->base=0x0BCD0000;	sp->limit=0x0BCD3FFF; }
		sp->data=tbuf;			sp->name="MMIO";
		sp->GetB=btesh2_spanmmio_GetB;	sp->GetW=btesh2_spanmmio_GetW;
		sp->GetD=btesh2_spanmmio_GetD;	sp->SetB=btesh2_spanmmio_SetB;
		sp->SetW=btesh2_spanmmio_SetW;	sp->SetD=btesh2_spanmmio_SetD;
		i=BTESH2_MemoryAddSpan(img, sp);
	}

	if(1)
	{
		tbuf=malloc(4096);
		memset(tbuf, 0, 4096);
//		btesh2_emac=(u32 *)(tbuf);
		
		sp=BTESH2_AllocPhysSpan();
		sp->base=0xABCE0000;	sp->limit=0xABCE0FFF;
		if(cpu->arch==BTESH2_ARCH_SH4)
			{ sp->base=0x0BCE0000;	sp->limit=0x0BCE0FFF; }
		sp->data=tbuf;			sp->name="EMAC";
		sp->GetB=btesh2_spanemac_GetB;	sp->GetW=btesh2_spanemac_GetW;
		sp->GetD=btesh2_spanemac_GetD;	sp->SetB=btesh2_spanemac_SetB;
		sp->SetW=btesh2_spanemac_SetW;	sp->SetD=btesh2_spanemac_SetD;
		i=BTESH2_MemoryAddSpan(img, sp);
	}

	if(1)
	{
		tbuf=malloc(4096);
		memset(tbuf, 0, 4096);

		sp=BTESH2_AllocPhysSpan();
		sp->base=0xFF000000;	sp->limit=0xFF000FFF;
		sp->data=tbuf;			sp->name="MMREG";
//		sp->GetB=btesh2_spanmmio_GetB;	sp->GetW=btesh2_spanmmio_GetW;
//		sp->SetB=btesh2_spanmmio_SetB;	sp->SetW=btesh2_spanmmio_SetW;
		sp->GetD=btesh2_spanmmreg_GetD;
		sp->SetD=btesh2_spanmmreg_SetD;
		i=BTESH2_MemoryAddSpan(img, sp);
	}

	if(1)
//	if(0)
	{
//		tbuf=malloc(4096);
//		memset(tbuf, 0, 4096);
		
		sp=BTESH2_AllocPhysSpan();
		sp->base=0x00800000;	sp->limit=0x00FFFFFF;
		sp->data=tbuf;			sp->name="GFXCon";
		sp->GetB=btesh2_gfxcon_GetB;	sp->GetW=btesh2_gfxcon_GetW;
		sp->GetD=btesh2_gfxcon_GetD;	sp->SetB=btesh2_gfxcon_SetB;
		sp->SetW=btesh2_gfxcon_SetW;	sp->SetD=btesh2_gfxcon_SetD;
		i=BTESH2_MemoryAddSpan(img, sp);
	}

	cpu->logpc=malloc((1<<18)*sizeof(u32));
	cpu->logsp=malloc((1<<18)*sizeof(u32));
	cpu->mlogpc=1<<18;

	ibuf=loadfile(imgname, &sz);
//	ibuf=loadfile("vmlinux", &sz);
//	ibuf=loadfile("boot.elf", &sz);
	if(!ibuf)
	{
		printf("Failed open kernel\n");
		return(-1);
	}

	i=BTESH2_BootLoadElf(cpu, ibuf, sz, 0x10000000);
	if(i<0)
	{
		printf("Load Failed\n");
		return(-1);
	}
	free(ibuf);

	if(mapname)
	{
		ibuf=loadfile(mapname, &sz);
	//	ibuf=loadfile("System.map", &sz);
		if(ibuf)
		{
			BTESH2_BootLoadMap(cpu, (char *)ibuf, sz, 0x10000000);
			free(ibuf);
		}
	}

//	kerninit="rdinit=/bin/ash root=/dev/tmpfs";
//	kerninit="rdinit=/bin/login";

	if(irdname)
	{
		ibuf=loadfile(irdname, &sz);
		if(ibuf)
		{
			t0=0x18000000-(sz+1);
			t0=t0&(~4095);
			BTESH2_MemCpyIn(cpu, t0, ibuf, sz);

			t1=t0-0x10000000;
			
			BTESH2_SetAddrDWord(cpu, 0x1003F000+0x010, t1);
			BTESH2_SetAddrDWord(cpu, 0x1003F000+0x014, sz);

			if(kerninit)
			{
				t2=0x1003F000+0x100;
				BTESH2_MemCpyIn(cpu, t2,
					(byte *)kerninit, strlen(kerninit)+1);
			}

//			BTESH2_BootLoadMap(cpu, ibuf, sz, 0x10000000);
//			free(ibuf);
		}
	}

//	l=99999999;
	l=999999999;

#ifdef __linux
	i = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, i | O_NONBLOCK);
	btesh2_ttynoncanon();
#endif

	btesh2_msec=0;
	btesh2_tops=0;
	rtops=0; rtmsec=0;

	printf("Boot PC=%08X\n", cpu->regs[BTESH2_REG_PC]);
//	t0=clock();
//	i=BTESH2_RunCpu(cpu, l);
//	t1=clock();
//	ts=CLOCKS_PER_SEC/100;

#ifdef __EMSCRIPTEN__
	ts=1;
	GfxDrv_Start();
	btesh2_cpu=cpu;
	emscripten_set_main_loop(frgl_main_iterate, 0, 1);
#endif

#ifndef __EMSCRIPTEN__
//	ts=CLOCKS_PER_SEC/1000;
	ts=1;
	err=0; kirq=0;
//	t0=clock(); 
	t0=FRGL_TimeMS();
	t2=0; tdt=0; t3=0;
	while(!err)
	{	
//		t1=clock();
		t1=FRGL_TimeMS();

		t2=t1-t3;
		if(t2>0)
		{
			t3=t1;
//			dt=t2*(1000.0/CLOCKS_PER_SEC);
			dt=t2;
			btesh2_msec+=dt;
			rtmsec+=dt;
		}

		t2=t1-t0;
		if((t2>ts) || (t2<0))
		{
#ifdef _WIN32
			if(_kbhit())
			{
				j=_getch();
				
				if(j=='`')
					{ err=1; continue; }
				
//				if(j=='\b')
//					j=0x7F;
//				if(j=='\n')
//					j='\r';

				if(j=='\r')
					j=0x0A0D;
				
	//			printf("K%c", j);
				kbbuf[kbrov]=j;
				kbrov=(kbrov+1)&255;
				kirq=1;
				
				if(j>>8)
				{
					kbbuf[kbrov]=j;
					kbrov=(kbrov+1)&255;
					kirq++;
				}
//				BTESH2_CpuUartInt(cpu);
			}
#endif

#ifdef __linux
			j=fgetc(stdin);
			if(j>0)
			{
				if(j=='`')
					{ err=1; continue; }
				
	//			printf("K%c", j);
				kbbuf[kbrov]=j;
				kbrov=(kbrov+1)&255;
//				BTESH2_CpuUartInt(cpu);
				kirq=1;
			}
#endif

			tdt+=t2;
			t0=t1;
			BTESH2_CpuTimerInt(cpu);
			
			continue;
		}

//		btesh2_msec=t1;
//		btesh2_msec+=t2;
		btesh2_tops=cpu->tr_tops-rtops;
		
		if(btesh2_tops>0)
		{
//			dt=rtmsec*(1.0/((double)CLOCKS_PER_SEC))+0.000001;
			dt=rtmsec*(1.0/1000.0)+0.000001;
			btesh2_ips=((double)btesh2_tops)/dt+0.000001;
		}else
		{
			btesh2_ips=999999999.0;
		}

		if((kirq>0) && (kbictrl&0x10))
		{
			BTESH2_CpuUartInt(cpu);
			kirq--;
		}

#if 1
		if(t2)
		{
			btesh2_mmio[0x85]--;
			if(((s32)btesh2_mmio[0x85])<=0)
			{
//				printf("AIC Fire\n");
				BTSH_Op_TrapIntIrq(cpu, BTESH2_EXC_AICCNTDN);
//				btesh2_mmio[0x85]=4095;
				btesh2_mmio[0x85]=16;
//				btesh2_mmio[0x85]=2;
				btesh2_mmio[0x86]=4095;
			}
		}
#endif

//		err=BTESH2_RunCpu(cpu, 1000);
		err=BTESH2_RunCpu(cpu, 10000);
		
		if(err==BTESH2_EXC_TRAPSLEEP)
		{
#ifdef _WIN32
			rtops=cpu->tr_tops;
			rtmsec=0;
			Sleep(1);
#endif
#ifdef __linux
//			usleep(1000);
#endif
			cpu->status=0;
			err=0;
			continue;
		}
	}

//	if(err)
	if(1)
	{
		printf("Status=%04X\n", err);
		BTESH2_DumpRegs(cpu);
	}

//	dt=tdt/((double)CLOCKS_PER_SEC);
	dt=tdt/1000.0;
	printf("%f MIPS\n", cpu->tr_tops/(dt*1000000.0));
	
//	t2=t1-t0; dt=t2/((double)CLOCKS_PER_SEC);
//	printf("%f MIPS\n", cpu->tr_tops/(dt*1000000.0));

	BTESH2_TKFAT_SyncExports();

#ifdef __linux
	btesh2_resettermios();
#endif
#endif
}
