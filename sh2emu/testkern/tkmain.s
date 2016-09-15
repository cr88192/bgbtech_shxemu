	.file	"tkmain.c"
	.text
	.comm	_tkmm_pagebase,4,4
	.comm	_tkmm_pageend,4,4
	.comm	_tkmm_pagebmp,4096,4
	.comm	_tkmm_maxpage,4,4
	.comm	_tkmm_pagerov,4,4
	.global	___ashrsi3
	.text
	.align 1
	.global	_TKMM_FindFreePages
	.type	_TKMM_FindFreePages, @function
_TKMM_FindFreePages:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r1
	cmp/pl	r1
	bt	.L2
	mov	#-1,r1
	bra	.L3
	nop
	.align 1
.L2:
	mov	r14,r1
	add	#-44,r1
	mov.l	.L37,r2
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	.L38,r2
	mov.l	@r2,r2
	mov.l	r2,@(56,r1)
	bra	.L4
	nop
	.align 1
.L35:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L43,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r1
	mov	r1,r3
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	#7,r2
	and	r1,r2
	mov.l	.L44,r1
	mov	r3,r4
	mov	r2,r5
	jsr	@r1
	nop
	mov	r0,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bf	.L56
	bra	.L11
	nop
.L56:
	bra	.L12
	nop
	.align 1
.L14:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r1,r3
	add	#8,r3
	mov	r14,r1
	add	#-44,r1
	mov	#-8,r2
	and	r3,r2
	mov.l	r2,@(60,r1)
	nop
.L12:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L43,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r2
	mov.w	.L42,r1
	cmp/eq	r1,r2
	bf	.L13
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/ge	r1,r2
	bf	.L14
.L13:
	bra	.L15
	nop
	.align 1
.L23:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L15:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L43,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r1
	mov	r1,r3
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	#7,r2
	and	r1,r2
	mov.l	.L44,r1
	mov	r3,r4
	mov	r2,r5
	jsr	@r1
	nop
	mov	r0,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bt	.L22
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/ge	r1,r2
	bf	.L23
.L22:
	bra	.L4
	nop
	.align 1
.L11:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r3
	add	#-44,r3
	mov	r14,r2
	add	#-44,r2
	mov.l	@(52,r3),r3
	mov.l	@(44,r2),r2
	add	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r2),r2
	mov.l	@(56,r1),r1
	cmp/gt	r1,r2
	bf	.L24
	bra	.L25
	nop
	.align 1
.L24:
	bra	.L26
	nop
	.align 1
.L42:
	.short	255
.L45:
	.align 2
.L37:
	.long	_tkmm_pagerov
.L38:
	.long	_tkmm_maxpage
.L43:
	.long	_tkmm_pagebmp
.L44:
	.long	___ashrsi3
	.align 1
.L34:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L26:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L46,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r1
	mov	r1,r3
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	#7,r2
	and	r1,r2
	mov.l	.L47,r1
	mov	r3,r4
	mov	r2,r5
	jsr	@r1
	nop
	mov	r0,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bf	.L33
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(48,r1),r1
	cmp/ge	r1,r2
	bf	.L34
.L33:
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(48,r1),r1
	cmp/ge	r1,r2
	bf	.L4
	mov.l	.L50,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	bra	.L3
	nop
	.align 1
.L4:
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/ge	r1,r2
	bt	.L55
	bra	.L35
	nop
.L55:
.L25:
	mov.l	.L50,r1
	mov.l	@r1,r1
	tst	r1,r1
	bt	.L36
	mov.l	.L50,r1
	mov	#0,r2
	mov.l	r2,@r1
	mov	r14,r8
	add	#-44,r8
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r4
	mov.l	.L51,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	bra	.L3
	nop
	.align 1
.L36:
	mov	#-1,r1
.L3:
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
.L52:
	.align 2
.L46:
	.long	_tkmm_pagebmp
.L47:
	.long	___ashrsi3
.L50:
	.long	_tkmm_pagerov
.L51:
	.long	_TKMM_FindFreePages
	.size	_TKMM_FindFreePages, .-_TKMM_FindFreePages
	.align 1
	.global	_TKMM_AllocPages
	.type	_TKMM_AllocPages, @function
_TKMM_AllocPages:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-16,r15
	mov	r15,r14
	mov	r14,r1
	add	#-48,r1
	mov.l	r4,@(48,r1)
	mov	r14,r8
	add	#-48,r8
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r4
	mov.l	.L62,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L58
	mov	#-1,r1
	bra	.L59
	nop
	.align 1
.L58:
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r3
	add	#-48,r3
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r3),r3
	mov.l	@(48,r2),r2
	add	r3,r2
	mov.l	r2,@(52,r1)
	bra	.L60
	nop
	.align 1
.L61:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L65,r3
	mov	r1,r0
	mov.b	@(r0,r3),r1
	extu.b	r1,r1
	exts.b	r1,r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	mov	#7,r7
	and	r7,r1
	mov	#1,r7
	mov	r7,r4
	mov.l	.L64,r7
	mov	r1,r0
	jsr	@r7
	nop
	mov	r0,r1
	exts.b	r1,r1
	or	r3,r1
	exts.b	r1,r1
	extu.b	r1,r3
	mov.l	.L65,r1
	mov	r2,r0
	mov.b	r3,@(r0,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L60:
	mov	r14,r2
	add	#-48,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r2),r2
	mov.l	@(52,r1),r1
	cmp/ge	r1,r2
	bf	.L61
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
.L59:
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
.L66:
	.align 2
.L62:
	.long	_TKMM_FindFreePages
.L65:
	.long	_tkmm_pagebmp
.L64:
	.long	___ashlsi3_r0
	.size	_TKMM_AllocPages, .-_TKMM_AllocPages
	.align 1
	.global	_TKMM_PageToPointer
	.type	_TKMM_PageToPointer, @function
_TKMM_PageToPointer:
	mov.l	r14,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	shll8	r1
	shll2	r1
	shll2	r1
	mov	r1,r3
	mov	r14,r1
	add	#-56,r1
	mov.l	.L69,r2
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L70:
	.align 2
.L69:
	.long	285212672
	.size	_TKMM_PageToPointer, .-_TKMM_PageToPointer
	.align 1
	.global	_TKMM_PointerToPage
	.type	_TKMM_PointerToPage, @function
_TKMM_PointerToPage:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r3
	mov	r14,r1
	add	#-56,r1
	mov.l	.L73,r2
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r4
	mov.l	.L74,r1
	jsr	@r1
	nop
	mov	r4,r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
.L75:
	.align 2
.L73:
	.long	-285212672
.L74:
	.long	___ashiftrt_r4_12
	.size	_TKMM_PointerToPage, .-_TKMM_PointerToPage
	.align 1
	.global	_TKMM_PageAlloc
	.type	_TKMM_PageAlloc, @function
_TKMM_PageAlloc:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-12,r15
	mov	r15,r14
	mov	r14,r1
	add	#-52,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r2
	mov.w	.L79,r1
	add	r2,r1
	mov	r1,r4
	mov.l	.L80,r1
	jsr	@r1
	nop
	mov	r4,r1
	mov	r14,r8
	add	#-52,r8
	mov	r1,r4
	mov.l	.L81,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	cmp/pz	r1
	bt	.L77
	mov	#0,r1
	bra	.L78
	nop
	.align 1
.L77:
	mov	r14,r8
	add	#-52,r8
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r4
	mov.l	.L82,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r1
.L78:
	mov	r1,r0
	add	#12,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L79:
	.short	4095
.L83:
	.align 2
.L80:
	.long	___ashiftrt_r4_12
.L81:
	.long	_TKMM_AllocPages
.L82:
	.long	_TKMM_PageToPointer
	.size	_TKMM_PageAlloc, .-_TKMM_PageAlloc
	.align 1
	.global	_TKMM_SizeToFxiU
	.type	_TKMM_SizeToFxiU, @function
_TKMM_SizeToFxiU:
	mov.l	r14,@-r15
	add	#-16,r15
	mov	r15,r14
	mov	r14,r1
	add	#-48,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-48,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	bra	.L85
	nop
	.align 1
.L86:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-48,r2
	shar	r1
	mov.l	r1,@(60,r2)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L85:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov	r1,r2
	mov	#1,r1
	cmp/gt	r1,r2
	bt	.L86
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	mov	#7,r3
	and	r1,r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	mov	r1,r2
	shll2	r2
	add	r2,r2
	mov	r14,r1
	add	#-48,r1
	or	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_TKMM_SizeToFxiU, .-_TKMM_SizeToFxiU
	.align 1
	.global	_TKMM_SizeToFxiD
	.type	_TKMM_SizeToFxiD, @function
_TKMM_SizeToFxiD:
	mov.l	r14,@-r15
	add	#-16,r15
	mov	r15,r14
	mov	r14,r1
	add	#-48,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-48,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	bra	.L89
	nop
	.align 1
.L90:
	mov	r14,r2
	add	#-48,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	shar	r1
	mov.l	r1,@(60,r2)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L89:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov	r1,r2
	mov	#1,r1
	cmp/gt	r1,r2
	bt	.L90
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	mov	#7,r3
	and	r1,r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	mov	r1,r2
	shll2	r2
	add	r2,r2
	mov	r14,r1
	add	#-48,r1
	or	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_TKMM_SizeToFxiD, .-_TKMM_SizeToFxiD
	.align 1
	.global	_TKMM_FxiToSize
	.type	_TKMM_FxiToSize, @function
_TKMM_FxiToSize:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-16,r15
	mov	r15,r14
	mov	r14,r1
	add	#-48,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r2
	mov	#7,r1
	cmp/gt	r1,r2
	bt	.L93
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r1
	bra	.L94
	nop
	.align 1
.L93:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r1
	mov	#7,r2
	and	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov	#8,r0
	or	r0,r2
	mov.l	r2,@(60,r1)
	mov	r14,r2
	add	#-48,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	r1,@(56,r2)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r3
	add	#-48,r3
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r3),r3
	mov.l	@(56,r2),r2
	mov	r3,r4
	mov.l	.L95,r3
	mov	r2,r0
	jsr	@r3
	nop
	mov	r0,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
.L94:
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
.L96:
	.align 2
.L95:
	.long	___ashlsi3_r0
	.size	_TKMM_FxiToSize, .-_TKMM_FxiToSize
	.comm	_tkmm_freelist,1024,4
	.align 1
	.global	_TKMM_Malloc
	.type	_TKMM_Malloc, @function
_TKMM_Malloc:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-36,r15
	mov	r15,r14
	mov	r14,r1
	add	#-28,r1
	mov.l	r4,@(28,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(28,r1),r1
	cmp/pl	r1
	bt	.L98
	mov	#0,r1
	bra	.L99
	nop
	.align 1
.L98:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(28,r1),r2
	mov.w	.L109,r1
	cmp/gt	r1,r2
	bt	.L100
	mov	r14,r1
	add	#-28,r1
	mov.w	.L110,r2
	mov.l	r2,@(28,r1)
.L100:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(28,r1),r1
	mov	r1,r3
	add	#7,r3
	mov	r14,r1
	add	#-28,r1
	mov	#-8,r2
	and	r3,r2
	mov.l	r2,@(28,r1)
	mov	r14,r8
	add	#-28,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	@(28,r1),r4
	mov.l	.L111,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-28,r1
	mov.l	.L113,r3
	mov	r14,r2
	add	#-28,r2
	mov.l	@(52,r2),r2
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bt	.L101
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	add	#16,r1
	mov.l	@r1,r2
	mov.l	.L113,r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#-2,r1
	and	r2,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	add	#16,r1
	bra	.L99
	nop
	.align 1
.L101:
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(56,r1)
	bra	.L102
	nop
	.align 1
.L109:
	.short	255
.L110:
	.short	256
.L114:
	.align 2
.L111:
	.long	_TKMM_SizeToFxiU
.L113:
	.long	_tkmm_freelist
	.align 1
.L104:
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	.L115,r3
	mov	r14,r2
	add	#-28,r2
	mov.l	@(56,r2),r2
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
.L102:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bf	.L103
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r2
	mov.w	.L116,r1
	cmp/gt	r1,r2
	bf	.L104
.L103:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bt	.L132
	bra	.L105
	nop
.L132:
	mov	r14,r8
	add	#-28,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r4
	mov.l	.L117,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r2
	mov.l	.L118,r1
	mov	r2,r3
	add	r1,r3
	mov	r14,r1
	add	#-28,r1
	mov.l	.L119,r2
	and	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r8
	add	#-28,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r4
	mov.l	.L120,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(44,r8)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	tst	r1,r1
	bf	.L106
	mov	#0,r1
	bra	.L99
	nop
	.align 1
.L116:
	.short	254
.L121:
	.align 2
.L115:
	.long	_tkmm_freelist
.L117:
	.long	_TKMM_FxiToSize
.L118:
	.long	65535
.L119:
	.long	-65536
.L120:
	.long	_TKMM_PageAlloc
	.align 1
.L106:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov	r14,r3
	add	#-28,r3
	mov.l	@(44,r3),r3
	add	r3,r2
	mov.l	r2,@(40,r1)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(40,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	add	#16,r1
	mov	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov	r3,r7
	sub	r2,r7
	mov	r7,r2
	mov.l	r2,@(32,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(32,r1),r4
	mov.l	.L125,r1
	jsr	@r1
	nop
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	#3,r2
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	#0,r2
	mov.l	r2,@(4,r1)
.L105:
	mov	r14,r8
	add	#-28,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r4
	mov.l	.L123,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r1
	mov	r1,r3
	add	#7,r3
	mov	r14,r1
	add	#-28,r1
	mov	#-8,r2
	and	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r1,r3
	add	#16,r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r2
	mov	r14,r1
	add	#-28,r1
	add	r3,r2
	mov.l	r2,@(44,r1)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(60,r2),r2
	mov.l	@r2,r2
	mov.l	r2,@(40,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(40,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	sub	r1,r2
	mov.w	.L124,r1
	cmp/gt	r1,r2
	bt	.L131
	bra	.L107
	nop
.L131:
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(36,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	#2,r2
	and	r2,r1
	tst	r1,r1
	bf	.L108
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.l	@r1,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(4,r1)
.L108:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(4,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#-3,r1
	and	r2,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(40,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	add	#16,r1
	mov	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov	r3,r7
	sub	r2,r7
	mov	r7,r2
	mov.l	r2,@(32,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(32,r1),r4
	mov.l	.L125,r1
	jsr	@r1
	nop
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	add	#16,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r2
	mov.b	@(8,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.l	.L127,r3
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov.l	.L127,r2
	shll2	r1
	add	r2,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@r1
.L107:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#-2,r1
	and	r2,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	add	#16,r1
.L99:
	mov	r1,r0
	add	#36,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L124:
	.short	256
.L128:
	.align 2
.L125:
	.long	_TKMM_SizeToFxiD
.L123:
	.long	_TKMM_FxiToSize
.L127:
	.long	_tkmm_freelist
	.size	_TKMM_Malloc, .-_TKMM_Malloc
	.align 1
	.global	_TKMM_Free
	.type	_TKMM_Free, @function
_TKMM_Free:
	mov.l	r14,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bf	.L134
	mov	#-1,r1
	bra	.L135
	nop
	.align 1
.L134:
	mov	r14,r1
	add	#-56,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	add	#-16,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	#1,r2
	or	r2,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	add	#16,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(60,r2),r2
	mov.b	@(8,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.l	.L137,r3
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov.l	.L137,r2
	shll2	r1
	add	r2,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	mov	#0,r1
.L135:
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L138:
	.align 2
.L137:
	.long	_tkmm_freelist
	.size	_TKMM_Free, .-_TKMM_Free
	.section	.rodata
	.align 2
.LC0:
	.string	"tkmm_init: heap=%dMB\n"
	.text
	.align 1
	.global	_tkmm_init
	.type	_tkmm_init, @function
_tkmm_init:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-4,r15
	mov	r15,r14
	mov.l	.L145,r1
	mov.l	@r1,r1
	tst	r1,r1
	bt	.L140
	bra	.L139
	nop
	.align 1
.L140:
	mov.l	.L145,r1
	mov	#1,r2
	mov.l	r2,@r1
	mov.l	.L154,r1
	mov.l	.L147,r2
	mov.l	r2,@r1
	mov.l	.L153,r1
	mov.l	.L149,r2
	mov.l	r2,@r1
	mov.l	.L153,r1
	mov.l	@r1,r2
	mov.l	.L154,r1
	mov.l	@r1,r1
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	shlr8	r1
	shlr2	r1
	shlr2	r1
	mov	r1,r2
	mov.l	.L152,r1
	mov.l	r2,@r1
	mov.l	.L153,r1
	mov.l	@r1,r2
	mov.l	.L154,r1
	mov.l	@r1,r1
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	shlr16	r1
	shlr2	r1
	shlr2	r1
	mov.l	.L155,r2
	mov	r2,r4
	mov	r1,r5
	mov.l	.L156,r1
	jsr	@r1
	nop
	mov	r14,r1
	add	#-60,r1
	mov	#0,r2
	mov.l	r2,@(60,r1)
	bra	.L142
	nop
	.align 1
.L143:
	mov.l	.L157,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shll2	r1
	add	r2,r1
	mov	#0,r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-60,r1
	mov	r14,r2
	add	#-60,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L142:
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r2
	mov.w	.L158,r1
	cmp/gt	r1,r2
	bf	.L143
.L139:
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
	.align 1
.L158:
	.short	255
.L159:
	.align 2
.L145:
	.long	_init.1455
.L154:
	.long	_tkmm_pagebase
.L147:
	.long	285212672
.L153:
	.long	_tkmm_pageend
.L149:
	.long	402653184
.L152:
	.long	_tkmm_maxpage
.L155:
	.long	.LC0
.L156:
	.long	_printf
.L157:
	.long	_tkmm_freelist
	.size	_tkmm_init, .-_tkmm_init
	.comm	_timer_ticks,4,4
	.align 1
	.global	_setGpioOutputs
	.type	_setGpioOutputs, @function
_setGpioOutputs:
	mov.l	r14,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov.l	.L161,r1
	mov	r14,r2
	add	#-60,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L162:
	.align 2
.L161:
	.long	-1412628480
	.size	_setGpioOutputs, .-_setGpioOutputs
	.align 1
	.global	_putc
	.type	_putc, @function
_putc:
	mov.l	r14,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov.l	.L164,r1
	mov	r14,r2
	add	#-60,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L165:
	.align 2
.L164:
	.long	-1412628220
	.size	_putc, .-_putc
	.align 1
	.global	_kbhit
	.type	_kbhit, @function
_kbhit:
	mov.l	r14,@-r15
	mov	r15,r14
	mov.l	.L168,r1
	mov.l	@r1,r1
	mov	#1,r2
	and	r2,r1
	mov	r1,r0
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L169:
	.align 2
.L168:
	.long	-1412628216
	.size	_kbhit, .-_kbhit
	.align 1
	.global	_getch
	.type	_getch, @function
_getch:
	mov.l	r14,@-r15
	mov	r15,r14
	nop
.L171:
	mov.l	.L173,r1
	mov.l	@r1,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bt	.L171
	mov.l	.L174,r1
	mov.l	@r1,r1
	mov	r1,r0
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L175:
	.align 2
.L173:
	.long	-1412628216
.L174:
	.long	-1412628224
	.size	_getch, .-_getch
	.align 1
	.global	_puts
	.type	_puts, @function
_puts:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	mov.l	r2,@(60,r1)
	bra	.L177
	nop
	.align 1
.L178:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L179,r1
	jsr	@r1
	nop
.L177:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bf	.L178
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
.L180:
	.align 2
.L179:
	.long	_putc
	.size	_puts, .-_puts
	.section	.rodata
	.align 2
.LC1:
	.string	"\b \b"
	.text
	.align 1
	.global	_gets
	.type	_gets, @function
_gets:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-12,r15
	mov	r15,r14
	mov	r14,r1
	add	#-52,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-52,r1
	mov	r14,r2
	add	#-52,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(60,r1)
.L188:
	mov	r14,r8
	add	#-52,r8
	mov.l	.L189,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r2
	mov	#8,r1
	cmp/eq	r1,r2
	bf	.L182
	mov	r14,r2
	add	#-52,r2
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r2),r2
	mov.l	@(52,r1),r1
	cmp/hi	r1,r2
	bf	.L183
	mov.l	.L190,r1
	mov	r1,r4
	mov.l	.L191,r1
	jsr	@r1
	nop
	mov	r14,r1
	add	#-52,r1
	mov	r14,r2
	add	#-52,r2
	mov.l	@(60,r2),r2
	add	#-1,r2
	mov.l	r2,@(60,r1)
.L183:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	mov	#0,r2
	mov.b	r2,@r1
	nop
	bra	.L188
	nop
	.align 1
.L182:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r2
	mov	#13,r1
	cmp/eq	r1,r2
	bf	.L185
	mov	#10,r4
	mov.l	.L194,r1
	jsr	@r1
	nop
	bra	.L186
	nop
	.align 1
.L185:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r2
	mov	#10,r1
	cmp/eq	r1,r2
	bf	.L187
	mov	#10,r4
	mov.l	.L194,r1
	jsr	@r1
	nop
	bra	.L186
	nop
	.align 1
.L187:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r4
	mov.l	.L194,r1
	jsr	@r1
	nop
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-52,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-52,r2
	mov.l	@(56,r2),r2
	exts.b	r2,r2
	mov.b	r2,@r1
	bra	.L188
	nop
	.align 1
.L186:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	mov	#0,r2
	mov.b	r2,@r1
	add	#12,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
.L195:
	.align 2
.L189:
	.long	_getch
.L190:
	.long	.LC1
.L191:
	.long	_puts
.L194:
	.long	_putc
	.size	_gets, .-_gets
	.align 1
	.global	_strcmp
	.type	_strcmp, @function
_strcmp:
	mov.l	r14,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	r5,@(56,r1)
	bra	.L197
	nop
	.align 1
.L199:
	mov	r14,r1
	add	#-56,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L197:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L198
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L198
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	cmp/eq	r1,r2
	bt	.L199
.L198:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	cmp/gt	r1,r2
	bf	.L200
	mov	#1,r1
	bra	.L201
	nop
	.align 1
.L200:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	cmp/ge	r1,r2
	bt	.L202
	mov	#-1,r1
	bra	.L201
	nop
	.align 1
.L202:
	mov	#0,r1
.L201:
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_strcmp, .-_strcmp
	.align 1
	.global	_print_hex
	.type	_print_hex, @function
_print_hex:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	shlr8	r1
	shlr2	r1
	shlr2	r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	shlr8	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	shlr2	r1
	shlr2	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shlr8	r1
	shlr2	r1
	shlr2	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shlr8	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	shlr2	r1
	shlr2	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	mov.l	.L218,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L219,r1
	jsr	@r1
	nop
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
.L220:
	.align 2
.L218:
	.long	_chrs.1501
.L219:
	.long	_putc
	.size	_print_hex, .-_print_hex
	.align 1
	.global	_print_decimal
	.type	_print_decimal, @function
_print_decimal:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mov.w	.L242,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L233,r1
	mov.w	.L242,r2
	add	r14,r2
	add	r2,r1
	mov.l	r4,@(44,r1)
	mov.w	.L258,r1
	add	r14,r1
	mov.w	.L233,r2
	mov.w	.L242,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(56,r1)
	mov.w	.L258,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L222
	mov.w	.L258,r1
	add	r14,r1
	mov.w	.L258,r2
	add	r14,r2
	mov.l	@(56,r2),r2
	neg	r2,r2
	mov.l	r2,@(56,r1)
	mov.w	.L258,r1
	add	r14,r1
	mov	#1,r2
	mov.l	r2,@(52,r1)
.L222:
	mov.w	.L258,r1
	add	r14,r1
	mov.w	.L241,r2
	mov.w	.L242,r3
	add	r14,r3
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bf	.L223
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L258,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	#48,r2
	mov.b	r2,@r1
	bra	.L224
	nop
	.align 1
.L223:
	bra	.L224
	nop
	.align 1
.L225:
	mov.w	.L258,r7
	add	r14,r7
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L254,r1
	dmuls.l	r1,r2
	sts	mach,r1
	shar	r1
	shar	r1
	mov	r1,r3
	mov	#0,r1
	cmp/gt	r2,r1
	subc	r1,r1
	sub	r1,r3
	mov	r3,r1
	shll2	r1
	add	r3,r1
	add	r1,r1
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	mov.l	r1,@(48,r7)
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L258,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.w	.L258,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	extu.b	r2,r2
	add	#48,r2
	extu.b	r2,r2
	exts.b	r2,r2
	mov.b	r2,@r1
	mov.w	.L258,r3
	add	r14,r3
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L254,r1
	dmuls.l	r1,r2
	sts	mach,r1
	shar	r1
	shar	r1
	mov	r1,r7
	mov	#0,r1
	cmp/gt	r2,r1
	subc	r1,r1
	mov	r7,r2
	sub	r1,r2
	mov	r2,r1
	mov.l	r1,@(56,r3)
.L224:
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	cmp/pl	r1
	bt	.L225
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	tst	r1,r1
	bt	.L226
	mov.w	.L258,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L258,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	#45,r2
	mov.b	r2,@r1
	bra	.L227
	nop
	.align 1
.L226:
	bra	.L227
	nop
	.align 1
.L242:
	.short	276
.L233:
	.short	-320
.L258:
	.short	212
.L241:
	.short	-272
.L259:
	.align 2
.L254:
	.long	1717986919
	.align 1
.L228:
	mov.w	.L266,r1
	add	r14,r1
	mov.w	.L266,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	add	#-1,r2
	mov.l	r2,@(60,r1)
	mov.w	.L266,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L263,r1
	jsr	@r1
	nop
.L227:
	mov.w	.L264,r1
	mov.w	.L267,r3
	add	r14,r3
	add	r3,r1
	mov.w	.L266,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	cmp/hi	r1,r2
	bt	.L228
	mov.w	.L267,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
	.align 1
.L266:
	.short	212
.L264:
	.short	-272
.L267:
	.short	276
.L268:
	.align 2
.L263:
	.long	_putc
	.size	_print_decimal, .-_print_decimal
	.align 1
	.global	_print_decimal_n
	.type	_print_decimal_n, @function
_print_decimal_n:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mov.w	.L313,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L292,r1
	mov.w	.L313,r2
	add	r14,r2
	add	r2,r1
	mov.l	r4,@(40,r1)
	mov.w	.L292,r1
	mov.w	.L313,r3
	add	r14,r3
	add	r3,r1
	mov.l	r5,@(36,r1)
	mov.w	.L312,r1
	add	r14,r1
	mov.w	.L292,r2
	mov.w	.L313,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(56,r1)
	mov.w	.L312,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	mov.w	.L312,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L270
	mov.w	.L312,r1
	add	r14,r1
	mov.w	.L312,r2
	add	r14,r2
	mov.l	@(56,r2),r2
	neg	r2,r2
	mov.l	r2,@(56,r1)
	mov.w	.L312,r1
	add	r14,r1
	mov	#1,r2
	mov.l	r2,@(48,r1)
.L270:
	mov.w	.L312,r1
	add	r14,r1
	mov.w	.L310,r2
	mov.w	.L313,r3
	add	r14,r3
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov.w	.L312,r1
	add	r14,r1
	mov.w	.L292,r2
	mov.w	.L313,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(52,r1)
	bra	.L271
	nop
	.align 1
.L272:
	mov.w	.L312,r7
	add	r14,r7
	mov.w	.L312,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L302,r1
	dmuls.l	r1,r2
	sts	mach,r1
	shar	r1
	shar	r1
	mov	r1,r3
	mov	#0,r1
	cmp/gt	r2,r1
	subc	r1,r1
	sub	r1,r3
	mov	r3,r1
	shll2	r1
	add	r3,r1
	add	r1,r1
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	mov.l	r1,@(44,r7)
	mov.w	.L312,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L312,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.w	.L312,r2
	add	r14,r2
	mov.l	@(44,r2),r2
	extu.b	r2,r2
	add	#48,r2
	extu.b	r2,r2
	exts.b	r2,r2
	mov.b	r2,@r1
	mov.w	.L312,r3
	add	r14,r3
	mov.w	.L312,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L302,r1
	dmuls.l	r1,r2
	sts	mach,r1
	shar	r1
	shar	r1
	mov	r1,r7
	mov	#0,r1
	cmp/gt	r2,r1
	subc	r1,r1
	mov	r7,r2
	sub	r1,r2
	mov	r2,r1
	mov.l	r1,@(56,r3)
	mov.w	.L312,r1
	add	r14,r1
	mov.w	.L312,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#-1,r2
	mov.l	r2,@(52,r1)
.L271:
	mov.w	.L312,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	cmp/pl	r1
	bt	.L272
	bra	.L273
	nop
	.align 1
.L274:
	mov.w	.L312,r1
	add	r14,r1
	mov.w	.L312,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	add	#-1,r2
	mov.l	r2,@(60,r1)
	mov.w	.L312,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L309,r1
	jsr	@r1
	nop
.L273:
	mov.w	.L310,r1
	mov.w	.L313,r3
	add	r14,r3
	add	r3,r1
	mov.w	.L312,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	cmp/hi	r1,r2
	bt	.L274
	mov.w	.L313,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
	.align 1
.L313:
	.short	284
.L292:
	.short	-320
.L312:
	.short	220
.L310:
	.short	-276
.L314:
	.align 2
.L302:
	.long	1717986919
.L309:
	.long	_putc
	.size	_print_decimal_n, .-_print_decimal_n
	.align 1
	.global	_printf
	.type	_printf, @function
_printf:
	mov.l	r7,@-r15
	mov.l	r6,@-r15
	mov.l	r5,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(44,r1)
	mov	r14,r2
	add	#28,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(60,r1)
	bra	.L316
	nop
	.align 1
.L326:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#37,r1
	cmp/eq	r1,r2
	bt	.L317
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L329,r1
	jsr	@r1
	nop
	bra	.L316
	nop
	.align 1
.L317:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov.b	@r1,r2
	mov	#37,r1
	cmp/eq	r1,r2
	bf	.L318
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#2,r2
	mov.l	r2,@(60,r1)
	mov	#37,r4
	mov.l	.L329,r1
	jsr	@r1
	nop
	bra	.L316
	nop
	.align 1
.L318:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	mov	#88,r3
	mov	#27,r2
	sub	r3,r1
	cmp/hi	r2,r1
	bf	.L339
	bra	.L327
	nop
.L339:
	mova	.L321,r0
	shll2	r1
	mov.l	@(r0,r1),r1
	braf	r1
	nop
.L322:
.L330:
	.align 2
.L329:
	.long	_putc
	.align 2
.L321:
	.long	.L320-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L323-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L324-.L322
	.long	.L327-.L322
	.long	.L327-.L322
	.long	.L325-.L322
	.align 1
.L323:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-44,r2
	mov.l	r3,@(48,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@r1,r1
	mov.l	r1,@(56,r2)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r4
	mov.l	.L331,r1
	jsr	@r1
	nop
	bra	.L316
	nop
	.align 1
.L320:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-44,r2
	mov.l	r3,@(48,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@r1,r1
	mov.l	r1,@(56,r2)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov	r1,r4
	mov.l	.L334,r1
	jsr	@r1
	nop
	bra	.L316
	nop
	.align 1
.L325:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-44,r2
	mov.l	r3,@(48,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@r1,r1
	mov.l	r1,@(52,r2)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r4
	mov.l	.L333,r1
	jsr	@r1
	nop
	bra	.L316
	nop
	.align 1
.L324:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-44,r2
	mov.l	r3,@(48,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@r1,r1
	mov.l	r1,@(52,r2)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r1,r4
	mov.l	.L334,r1
	jsr	@r1
	nop
	bra	.L316
	nop
	.align 1
.L327:
	nop
.L316:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L338
	bra	.L326
	nop
.L338:
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	add	#12,r15
	rts	
	nop
.L335:
	.align 2
.L331:
	.long	_print_decimal
.L334:
	.long	_print_hex
.L333:
	.long	_puts
	.size	_printf, .-_printf
	.align 1
	.global	_malloc
	.type	_malloc, @function
_malloc:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r4
	mov.l	.L342,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
.L343:
	.align 2
.L342:
	.long	_TKMM_Malloc
	.size	_malloc, .-_malloc
	.align 1
	.global	_free
	.type	_free, @function
_free:
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r4
	mov.l	.L346,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	rts	
	nop
.L347:
	.align 2
.L346:
	.long	_TKMM_Free
	.size	_free, .-_free
	.section	.rodata
	.align 2
.LC2:
	.string	"tk_main: puts\n"
	.align 2
.LC3:
	.string	"$ "
	.align 2
.LC4:
	.string	"got: "
	.align 2
.LC5:
	.string	"\n"
	.align 2
.LC6:
	.string	"quit"
	.align 2
.LC7:
	.string	"ok, exiting main loop\n"
	.align 2
.LC8:
	.string	"time"
	.align 2
.LC9:
	.string	"timer ticks=%d\n"
	.align 2
.LC10:
	.string	"die"
	.align 2
.LC11:
	.string	"ok, dying\n"
	.align 2
.LC12:
	.string	"malloc"
	.align 2
.LC13:
	.string	"got pointer A %p %p\n"
	.align 2
.LC14:
	.string	"got pointer B %p %p\n"
	.align 2
.LC15:
	.string	"don't understand %s\n"
	.text
	.align 1
	.global	_tk_main
	.type	_tk_main, @function
_tk_main:
	mov.l	r8,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mov.w	.L382,r1
	sub	r1,r15
	mov	r15,r14
	mov.l	.L385,r1
	mov	#0,r2
	mov.l	r2,@r1
	mov	#105,r4
	mov.l	.L359,r1
	jsr	@r1
	nop
	mov.l	.L360,r1
	mov	r1,r4
	mov.l	.L380,r1
	jsr	@r1
	nop
	mov.l	.L362,r1
	jsr	@r1
	nop
.L355:
	mov.l	.L363,r1
	mov	r1,r4
	mov.l	.L380,r1
	jsr	@r1
	nop
	mov.w	.L381,r1
	mov.w	.L382,r2
	add	r14,r2
	add	r2,r1
	mov	r1,r4
	mov.l	.L367,r1
	jsr	@r1
	nop
	mov.l	.L368,r1
	mov	r1,r4
	mov.l	.L380,r1
	jsr	@r1
	nop
	mov.w	.L381,r1
	mov.w	.L382,r3
	add	r14,r3
	add	r3,r1
	mov	r1,r4
	mov.l	.L380,r1
	jsr	@r1
	nop
	mov.l	.L373,r1
	mov	r1,r4
	mov.l	.L380,r1
	jsr	@r1
	nop
	mov.w	.L381,r1
	mov.w	.L382,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L377,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L384,r1
	jsr	@r1
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L349
	mov.l	.L379,r1
	mov	r1,r4
	mov.l	.L380,r1
	jsr	@r1
	nop
	bra	.L356
	nop
	.align 1
.L349:
	mov.w	.L381,r1
	mov.w	.L382,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L383,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L384,r1
	jsr	@r1
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L351
	mov.l	.L385,r1
	mov.l	@r1,r1
	mov.l	.L386,r2
	mov	r2,r4
	mov	r1,r5
	mov.l	.L387,r1
	jsr	@r1
	nop
	bra	.L352
	nop
	.align 1
.L382:
	.short	264
.L381:
	.short	-264
.L388:
	.align 2
.L385:
	.long	_timer_ticks
.L359:
	.long	_setGpioOutputs
.L360:
	.long	.LC2
.L380:
	.long	_puts
.L362:
	.long	_tkmm_init
.L363:
	.long	.LC3
.L367:
	.long	_gets
.L368:
	.long	.LC4
.L373:
	.long	.LC5
.L377:
	.long	.LC6
.L384:
	.long	_strcmp
.L379:
	.long	.LC7
.L383:
	.long	.LC8
.L386:
	.long	.LC9
.L387:
	.long	_printf
	.align 1
.L351:
	mov.w	.L428,r1
	mov.w	.L431,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L391,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L398,r1
	jsr	@r1
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L353
	mov.l	.L393,r1
	mov	r1,r4
	mov.l	.L394,r1
	jsr	@r1
	nop
	mov	#-1,r1
	mov	#-1,r2
	mov.l	r2,@r1
.L353:
	mov.w	.L428,r1
	mov.w	.L431,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L397,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L398,r1
	jsr	@r1
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L354
	mov.w	.L425,r8
	add	r14,r8
	mov.w	.L417,r1
	mov	r1,r4
	mov.l	.L418,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov.w	.L425,r8
	add	r14,r8
	mov.w	.L417,r1
	mov	r1,r4
	mov.l	.L418,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov.w	.L425,r1
	add	r14,r1
	mov.l	@(60,r1),r4
	mov.l	.L422,r1
	jsr	@r1
	nop
	mov.w	.L425,r1
	add	r14,r1
	mov.l	@(56,r1),r4
	mov.l	.L422,r1
	jsr	@r1
	nop
	mov.l	.L409,r3
	mov.w	.L425,r2
	add	r14,r2
	mov.w	.L425,r1
	add	r14,r1
	mov	r3,r4
	mov.l	@(60,r2),r5
	mov.l	@(56,r1),r6
	mov.l	.L430,r1
	jsr	@r1
	nop
	mov.w	.L425,r8
	add	r14,r8
	mov.w	.L417,r1
	mov	r1,r4
	mov.l	.L418,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov.w	.L425,r8
	add	r14,r8
	mov.w	.L417,r1
	mov	r1,r4
	mov.l	.L418,r1
	jsr	@r1
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov.w	.L425,r1
	add	r14,r1
	mov.l	@(60,r1),r4
	mov.l	.L422,r1
	jsr	@r1
	nop
	mov.w	.L425,r1
	add	r14,r1
	mov.l	@(56,r1),r4
	mov.l	.L422,r1
	jsr	@r1
	nop
	mov.l	.L423,r3
	mov.w	.L425,r2
	add	r14,r2
	mov.w	.L425,r1
	add	r14,r1
	mov	r3,r4
	mov.l	@(60,r2),r5
	mov.l	@(56,r1),r6
	mov.l	.L430,r1
	jsr	@r1
	nop
	bra	.L352
	nop
	.align 1
.L354:
	mov.l	.L427,r2
	mov.w	.L428,r1
	mov.w	.L431,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L430,r1
	jsr	@r1
	nop
	bra	.L355
	nop
	.align 1
.L352:
	bra	.L355
	nop
	.align 1
.L356:
	mov.w	.L431,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L428:
	.short	-264
.L431:
	.short	264
.L425:
	.short	200
.L417:
	.short	256
.L432:
	.align 2
.L391:
	.long	.LC10
.L398:
	.long	_strcmp
.L393:
	.long	.LC11
.L394:
	.long	_puts
.L397:
	.long	.LC12
.L418:
	.long	_TKMM_Malloc
.L422:
	.long	_TKMM_Free
.L409:
	.long	.LC13
.L430:
	.long	_printf
.L423:
	.long	.LC14
.L427:
	.long	.LC15
	.size	_tk_main, .-_tk_main
	.local	_init.1455
	.comm	_init.1455,4,4
	.section	.rodata
	.align 2
.LC16:
	.string	"0123456789ABCDEF"
	.data
	.align 2
	.type	_chrs.1501, @object
	.size	_chrs.1501, 4
_chrs.1501:
	.long	.LC16
	.ident	"GCC: (GNU) 4.8.4"
