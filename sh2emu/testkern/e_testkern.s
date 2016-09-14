.section .text
.align 2
.global _start
.global start
.global _vector_table
.extern _tk_main
.extern _timer_ticks

start:
_start:
	nop
	mov.l start_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1

	mov.l isr_table, r0
	ldc r0, vbr
	
	mov #0, r0
	ldc r0, sr

	mov.l tk_main, r0
	jsr @r0
	nop

	bra _loop
	nop

_loop:
	bra _loop
	nop

_exit:
	mov #-1, r0
	jsr @r0
	nop

_gdb_unhandled_isr:
	mov.l start1_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _exit
	nop

_gdb_nmi_isr:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _loop
	nop

_gdb_illegalinst_isr:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _exit
	nop

_gdb_addresserr_isr:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _exit
	nop

_gdb_ihandler_emac:
	mov.l start2_leds, r0
	mov.l pio_addr, r1
	mov.l r0, @r1
	bra _loop
	nop

_gdb_trapa32_isr:
	rte
	nop

_gdb_trapa33_isr:
	rte
	nop

_gdb_trapa34_isr:
	rte
	nop

_gdb_pit_isr:
	mov.l r0, @-sp
	mov.l r1, @-sp
	mov.l ticks, r0
	mov.l @r0, r1
	add #1, r1
	mov.l r1, @r0
	mov.l @sp+, r1
	mov.l @sp+, r0
	rte
	nop

.align 4
ticks: .long _timer_ticks

_gdb_ihandler_uart1:
	rte
	nop

.align 4
pio_addr:	 .long 0xABCD0000
start_leds:   .long 0x000000ff
start1_leds:   .long 0x0000004f
start2_leds:   .long 0x0000004e

tk_main: .long _tk_main
.long 0x5B5B5B5B
isr_table: .long _vector_table
.long 0x5C5C5C5C

/* .section .vect */
.align 4
_vector_table:

.long  _start					/* 0x00: power-on reset */
.long  0xFFFC					/* 0x01 */
.long  _start					/* 0x02: manual reset */
.long  0xFFFC					/* 0x03 */
.long _gdb_illegalinst_isr  	/* 0x04: general illegal instruction */
.long _gdb_unhandled_isr		/* 0x05: (reserved) */
.long _gdb_illegalinst_isr		/* 0x06: slot illegal instruction */
.long _gdb_unhandled_isr		/* 0x07: (reserved) */
.long _gdb_unhandled_isr		/* 0x08: (reserved) */
.long _gdb_addresserr_isr		/* 0x09: CPU address error */
.long _gdb_addresserr_isr		/* 0x0A: DMAC/DTC address error */
.long _gdb_nmi_isr				/* 0x0B: NMI */
.long _gdb_unhandled_isr		/* 0x0C: UBC */
.long _gdb_unhandled_isr		/* 0x0D: (reserved) */
.long _gdb_unhandled_isr		/* 0x0E: (reserved) */
.long _gdb_unhandled_isr		/* 0x0F: (reserved) */
.long _gdb_pit_isr	 			/* 0x10: PIT */
.long _gdb_ihandler_emac		/* 0x11: (EMAC interface) */
.long _gdb_unhandled_isr		/* 0x12: (reserved) */
.long _gdb_unhandled_isr		/* 0x13: (reserved) */
.long _gdb_unhandled_isr		/* 0x14: (reserved) */

.long _gdb_unhandled_isr		/* 0x15: (reserved) */
.long _gdb_unhandled_isr		/* 0x16: (reserved) */
.long _gdb_ihandler_uart1		/* 0x17: (UART Console) */
.long _gdb_unhandled_isr		/* 0x18: (reserved) */
.long _gdb_unhandled_isr		/* 0x19: (when AIC countdown reach 0) */
.long _gdb_unhandled_isr		/* 0x1A: (reserved) */
.long _gdb_unhandled_isr		/* 0x1B: (reserved) */
.long _gdb_unhandled_isr		/* 0x1C: (reserved) */
.long _gdb_unhandled_isr		/* 0x1D: (reserved) */
.long _gdb_unhandled_isr		/* 0x1E: (reserved) */

.long _gdb_unhandled_isr		/* 0x1F: (reserved) */
.long _gdb_trapa32_isr			/* 0x20: trap 32 instruction */
.long _gdb_trapa33_isr			/* 0x21: trap 33 instruction */
.long _gdb_trapa34_isr			/* 0x22: trap 34 instruction */
.long _gdb_unhandled_isr		/* 0x23 */
.long _gdb_unhandled_isr		/* 0x24 */
.long _gdb_unhandled_isr		/* 0x25 */
.long _gdb_unhandled_isr		/* 0x26 */
.long _gdb_unhandled_isr		/* 0x27 */

.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr

.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr

.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr
.long _gdb_unhandled_isr


.end
