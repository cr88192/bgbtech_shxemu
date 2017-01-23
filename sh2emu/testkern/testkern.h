#ifdef ARCH_SH4

#ifndef ARCH_HAS_FPU
#define ARCH_HAS_FPU
#endif

#ifndef ARCH_HAS_MMU
#define ARCH_HAS_MMU
#endif

#endif

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef volatile u32 vol_u32;

#define GPIO_BASE 0xABCD0000
#define SPI_BASE 0xABCD0040
#define UART_BASE 0xABCD0100
#define AIC_BASE 0xABCD0200

#define UART_RX		(UART_BASE+0x00)
#define UART_TX		(UART_BASE+0x04)
#define UART_STAT	(UART_BASE+0x08)
#define UART_CTRL	(UART_BASE+0x0C)

#define SPI_CTRL	(SPI_BASE+0x00)
#define SPI_DATA	(SPI_BASE+0x04)

#define AIC_CTRL0			(AIC_BASE+0x00)
#define AIC_BRKADD			(AIC_BASE+0x04)
#define AIC_ILEVELS			(AIC_BASE+0x08)
#define AIC_CTRL1			(AIC_BASE+0x0C)
#define AIC_PIT_THROTTLE	(AIC_BASE+0x10)
#define AIC_PIT_COUNTER		(AIC_BASE+0x14)
#define AIC_CLK_PERIOD		(AIC_BASE+0x18)
#define AIC_IGNORE0			(AIC_BASE+0x1C)
#define AIC_RTC_SEC_HI		(AIC_BASE+0x20)
#define AIC_RTC_SEC_LO		(AIC_BASE+0x24)
#define AIC_RTC_NSEC		(AIC_BASE+0x28)

#define P_UART_RX	(*(vol_u32 *)UART_RX)
#define P_UART_TX	(*(vol_u32 *)UART_TX)
#define P_UART_STAT	(*(vol_u32 *)UART_STAT)
#define P_UART_CTRL	(*(vol_u32 *)UART_CTRL)

#define P_SPI_CTRL	(*(vol_u32 *)SPI_CTRL)
#define P_SPI_DATA	(*(vol_u32 *)SPI_DATA)

#define P_AIC_RTC_SEC_HI	(*(vol_u32 *)AIC_RTC_SEC_HI)
#define P_AIC_RTC_SEC_LO	(*(vol_u32 *)AIC_RTC_SEC_LO)
#define P_AIC_RTC_NSEC		(*(vol_u32 *)AIC_RTC_NSEC)

#define MMREG_BASE 0xFF000000
#define MMREG_PTEH	(MMREG_BASE+0x00)
#define MMREG_PTEL	(MMREG_BASE+0x04)
#define MMREG_TTB	(MMREG_BASE+0x08)
#define MMREG_TEA	(MMREG_BASE+0x0C)
#define MMREG_MMUCR	(MMREG_BASE+0x10)

#define P_MMREG_PTEH	(*(vol_u32 *)MMREG_PTEH)
#define P_MMREG_PTEL	(*(vol_u32 *)MMREG_PTEL)
#define P_MMREG_TTB		(*(vol_u32 *)MMREG_TTB)
#define P_MMREG_TEA		(*(vol_u32 *)MMREG_TEA)
#define P_MMREG_MMUCR	(*(vol_u32 *)MMREG_MMUCR)

#ifdef ARCH_SH4
#define TKMM_PAGEBASE	0x90000000
#define TKMM_PAGEEND	0x98000000
#else
#define TKMM_PAGEBASE	0x11000000
#define TKMM_PAGEEND	0x18000000
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef va_arg

#ifndef _VALIST
#define _VALIST
typedef __builtin_va_list va_list;
#endif

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)

#endif

#ifndef __cplusplus

#ifndef _BOOL_T
#define _BOOL_T
typedef unsigned char bool;
#endif

#ifndef true
#define true 1
#define false 0
#endif

#endif

typedef unsigned int size_t;

typedef struct TKMM_MemLnkObj_s TKMM_MemLnkObj;

struct TKMM_MemLnkObj_s {
TKMM_MemLnkObj *cnext;	//next object in chunk
TKMM_MemLnkObj *cprev;	//previous object in chunk
byte ix;				//size index value
byte fl;
u32 pad;
// int flag;
// u32 size;
//double data[1];	//start of data
u32 data[1];	//start of data
};

#define TKMM_OFFS_DATA	((int)(((TKMM_MemLnkObj *)0)->data))

#include "tk_fatfs.h"

void printf(char *str, ...);

void *malloc(int sz);
int free(void *ptr);
