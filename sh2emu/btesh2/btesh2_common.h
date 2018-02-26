#ifndef BYTE_T
#define BYTE_T
typedef unsigned char byte;
#endif

#ifndef SBYTE_T
#define SBYTE_T
typedef signed char sbyte;
#endif

#ifndef PDLIB_INT_BITS_T
#define PDLIB_INT_BITS_T
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
#endif

#ifndef UINT_T
#define UINT_T
typedef unsigned int uint;
#endif

#ifndef F32_T
#define F32_T
typedef float f32;
#endif

#ifndef F64_T
#define F64_T
typedef double f64;
#endif

#if defined(__x86_64__) || defined(_M_X64)
#ifndef X86_64
#define X86_64
#endif
#endif

#if defined(__i386__) || defined(_M_IX86)
#ifndef X86
#define X86
#endif
#endif

#if defined(__arm__) || defined(_M_ARM)
#ifndef ARM
#define ARM
#endif
#endif

//#ifdef __linux
#ifndef _MSC_VER
#define __debugbreak()	(*((int *)-1)=-1)
#endif

#ifdef _M_X64
typedef long long nlint;
#else
typedef long nlint;
#endif

#if !defined(LITTLEENDIAN) && !defined(BIGENDIAN)
#if defined(X86) || defined(X86_64) || defined(ARM)
#define LITTLEENDIAN
#endif
#endif

#if 0

#if defined(_MSC_VER) && defined(BTEIFGL_DLL)
#ifndef BTIC4B_API
#define BTIC4B_API __declspec(dllexport)
#endif
#endif

#if defined(_MSC_VER) && defined(BTIC4B_DLL)
#ifndef BTIC4B_API
#define BTIC4B_API __declspec(dllexport)
#endif
#endif

#ifndef BTIC4B_API
#define BTIC4B_API
#endif

#endif


#ifdef _MSC_VER
#define force_inline __forceinline
#define default_inline __inline
#endif

#ifndef __EMSCRIPTEN__
#ifdef __GNUC__
#define force_inline inline
#define default_inline inline
#endif
#endif

#ifndef force_inline
#define force_inline
#define default_inline
#endif

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_TAU
#define M_TAU 6.283185307179586476925286766559
#endif


#if defined(X86)||defined(X86_64)||defined(ARM)
#define btesh2_getu16le(ptr)		(*(u16 *)(ptr))
#define btesh2_getu32le(ptr)		(*(u32 *)(ptr))
#define btesh2_getu64le(ptr)		(*(u64 *)(ptr))
#define btesh2_gets16le(ptr)		(*(s16 *)(ptr))
#define btesh2_gets32le(ptr)		(*(s32 *)(ptr))
#define btesh2_gets64le(ptr)		(*(s64 *)(ptr))
#define btesh2_setu16le(ptr, val)	(*(u16 *)(ptr))=(val)
#define btesh2_setu32le(ptr, val)	(*(u32 *)(ptr))=(val)
#define btesh2_setu64le(ptr, val)	(*(u64 *)(ptr))=(val)
#define btesh2_sets16le(ptr, val)	(*(s16 *)(ptr))=(val)
#define btesh2_sets32le(ptr, val)	(*(s32 *)(ptr))=(val)
#define btesh2_sets64le(ptr, val)	(*(s64 *)(ptr))=(val)

#define btesh2_getu8le(ptr)			(*(byte *)(ptr))
#define btesh2_gets8le(ptr)			(*(sbyte *)(ptr))

#define btesh2_setu8le(ptr, val)	(*(byte *)(ptr))=(val)
#define btesh2_sets8le(ptr, val)	(*(sbyte *)(ptr))=(val)

default_inline u16 btesh2_bswapu16(u16 val)
	{ return((val>>8)|(val<<8)); }
default_inline u32 btesh2_bswapu32(u32 val)
	{ return((val>>24)|(val<<24)|
		((val>>8)&0x0000FF00)|((val<<8)&0x00FF0000)); }
default_inline u64 btesh2_bswapu64(u64 val)
	{ return(btesh2_bswapu32(val>>32)|(((u64)btesh2_bswapu32(val))<<32)); }

#define btesh2_getu16be(ptr)		btesh2_bswapu16(*(u16 *)(ptr))
#define btesh2_getu32be(ptr)		btesh2_bswapu32(*(u32 *)(ptr))
#define btesh2_getu64be(ptr)		btesh2_bswapu64(*(u64 *)(ptr))
#define btesh2_gets16be(ptr)		((s16)btesh2_bswapu16(*(u16 *)(ptr)))
#define btesh2_gets32be(ptr)		((s32)btesh2_bswapu32(*(u32 *)(ptr)))
#define btesh2_gets64be(ptr)		((s64)btesh2_bswapu64(*(u64 *)(ptr)))

#define btesh2_getf32le(ptr)		(*(f32 *)(ptr))
#define btesh2_getf64le(ptr)		(*(f64 *)(ptr))
#define btesh2_setf32le(ptr, val)	(*(f32 *)(ptr))=(val)
#define btesh2_setf64le(ptr, val)	(*(f64 *)(ptr))=(val)

default_inline f32 btesh2_bswapf32a(u32 val)
	{ u32 t; t=btesh2_bswapu32(val); return(*(f32 *)(&t)); }
default_inline f64 btesh2_bswapf64a(u64 val)
	{ u64 t; t=btesh2_bswapu64(val); return(*(f64 *)(&t)); }

default_inline u32 btesh2_bswapf32b(f32 val)
	{ return(btesh2_bswapu32(*(u32 *)(&val))); }
default_inline u64 btesh2_bswapf64b(f64 val)
	{ return(btesh2_bswapu64(*(u64 *)(&val))); }

#define btesh2_getf32be(ptr)		btesh2_bswapf32a(*(u32 *)(ptr))
#define btesh2_getf64be(ptr)		btesh2_bswapf64a(*(u64 *)(ptr))
#define btesh2_setf32be(ptr, val)	(*(u32 *)(ptr))=btesh2_bswapf32b(val)
#define btesh2_setf64be(ptr, val)	(*(u64 *)(ptr))=btesh2_bswapf64b(val)

#else
default_inline u16 btesh2_getu16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline u32 btesh2_getu32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline u64 btesh2_getu64le(byte *ptr)
	{ return(btesh2_getu32le(ptr)|(((u64)btesh2_getu32le(ptr+4))<<32)); }
default_inline s16 btesh2_gets16le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)); }
default_inline s32 btesh2_gets32le(byte *ptr)
	{ return(ptr[0]|(ptr[1]<<8)|(ptr[2]<<16)|(ptr[3]<<24)); }
default_inline s64 btesh2_gets64le(byte *ptr)
	{ return(btesh2_getu32le(ptr)|(((s64)btesh2_gets32le(ptr+4))<<32)); }
default_inline void btesh2_setu16le(byte *ptr, u16 val)
	{ ptr[0]=val; ptr[1]=val>>8; }
default_inline void btesh2_setu32le(byte *ptr, u32 val)
	{ ptr[0]=val; ptr[1]=val>>8; ptr[2]=val>>16; ptr[3]=val>>24; }
default_inline void btesh2_setu64le(byte *ptr, u64 val)
	{ btesh2_setu32le(ptr, val); btesh2_setu32le(ptr+4, val>>32); }
default_inline void btesh2_sets16le(byte *ptr, s16 val)
	{ btesh2_setu16le(ptr, (u16)val); }
default_inline void btesh2_sets32le(byte *ptr, s32 val)
	{ btesh2_setu32le(ptr, (u32)val); }
default_inline void btesh2_sets64le(byte *ptr, s64 val)
	{ btesh2_setu64le(ptr, (u64)val); }

#define btesh2_getu8le(ptr)			(*(byte *)(ptr))
#define btesh2_gets8le(ptr)			(*(sbyte *)(ptr))

#define btesh2_setu8le(ptr, val)	(*(byte *)(ptr))=(val)
#define btesh2_sets8le(ptr, val)	(*(sbyte *)(ptr))=(val)

default_inline u16 btesh2_getu16be(byte *ptr)
	{ return(ptr[1]|(ptr[0]<<8)); }
default_inline u32 btesh2_getu32be(byte *ptr)
	{ return(ptr[3]|(ptr[2]<<8)|(ptr[1]<<16)|(ptr[0]<<24)); }
default_inline u64 btesh2_getu64be(byte *ptr)
	{ return(btesh2_getu32le(ptr+4)|(((u64)btesh2_getu32le(ptr))<<32)); }

default_inline void btesh2_setu16be(byte *ptr, u16 val)
	{ ptr[1]=val; ptr[0]=val>>8; }
default_inline void btesh2_setu32be(byte *ptr, u32 val)
	{ ptr[3]=val; ptr[2]=val>>8; ptr[1]=val>>16; ptr[0]=val>>24; }
default_inline void btesh2_setu64be(byte *ptr, u64 val)
	{ btesh2_setu32le(ptr+4, val); btesh2_setu32le(ptr+0, val>>32); }

#define btesh2_gets16be(ptr)		((s16)btesh2_getu16be(ptr))
#define btesh2_gets32be(ptr)		((s32)btesh2_getu32be(ptr))
#define btesh2_gets64be(ptr)		((s64)btesh2_getu64be(ptr))


default_inline f32 btesh2_f32fromu32bits(u32 val)
	{ return(*(f32 *)(&val)); }
default_inline f64 btesh2_f64fromu64bits(u64 val)
	{ return(*(f64 *)(&val)); }

default_inline u32 btesh2_u32fromf32bits(f32 val)
	{ return(*(u32 *)(&val)); }
default_inline u64 btesh2_u64fromf64bits(f64 val)
	{ return(*(u64 *)(&val)); }

#define btesh2_getf32le(ptr)		btesh2_f32fromu32bits(btesh2_getu32le(ptr))
#define btesh2_getf64le(ptr)		btesh2_f64fromu64bits(btesh2_getu64le(ptr))
#define btesh2_setf32le(ptr, val)	btesh2_setu32le((ptr), \
										btesh2_u32fromf32bits(val))
#define btesh2_setf64le(ptr, val)	btesh2_setu32le((ptr), \
										btesh2_u64fromf64bits(val))

#define btesh2_getf32be(ptr)		btesh2_f32fromu32bits(btesh2_getu32be(ptr))
#define btesh2_getf64be(ptr)		btesh2_f64fromu64bits(btesh2_getu64be(ptr))
#define btesh2_setf32be(ptr, val)	btesh2_setu32be((ptr), \
										btesh2_u32fromf32bits(val))
#define btesh2_setf64be(ptr, val)	btesh2_setu32be((ptr), \
										btesh2_u64fromf64bits(val))

#endif
