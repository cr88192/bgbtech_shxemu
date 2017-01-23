	.file	"tkmain.c"
	.text
	.text
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
	bra	.L2
	nop
	.align 1
.L4:
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
.L2:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L3
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L3
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	cmp/eq	r1,r2
	bt	.L4
.L3:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	cmp/gt	r1,r2
	bf	.L5
	mov	#1,r1
	bra	.L6
	nop
	.align 1
.L5:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	cmp/ge	r1,r2
	bt	.L7
	mov	#-1,r1
	bra	.L6
	nop
	.align 1
.L7:
	mov	#0,r1
.L6:
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_strcmp, .-_strcmp
	.align 1
	.global	_memset
	.type	_memset, @function
_memset:
	mov.l	r14,@-r15
	add	#-24,r15
	mov	r15,r14
	mov	r14,r1
	add	#-40,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r5,@(44,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r6,@(40,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r3
	add	#-40,r3
	mov	r14,r2
	add	#-40,r2
	mov.l	@(60,r3),r3
	mov.l	@(40,r2),r2
	add	r3,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r1
	mov	#3,r0
	and	r0,r1
	mov	#0,r3
	mov	#3,r2
	sub	r3,r1
	cmp/hi	r2,r1
	bt	.L23
	mova	.L11,r0
	shll2	r1
	mov.l	@(r0,r1),r1
	braf	r1
	nop
.L12:
	.align 2
.L11:
	.long	.L24-.L12
	.long	.L13-.L12
	.long	.L14-.L12
	.long	.L15-.L12
	.align 1
.L13:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L14:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L15:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	extu.b	r2,r2
	mov.b	r2,@r1
	bra	.L24
	nop
	.align 1
.L23:
	nop
	bra	.L16
	nop
	.align 1
.L24:
	nop
.L16:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(44,r1),r1
	mov	r1,r3
	shll8	r3
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	or	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r1
	mov	r1,r3
	shll16	r3
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(52,r2),r2
	or	r3,r2
	mov.l	r2,@(52,r1)
	bra	.L17
	nop
	.align 1
.L18:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	add	#4,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(60,r2),r2
	add	#8,r2
	mov.l	r2,@(60,r1)
.L17:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#8,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	cmp/hi	r1,r2
	bf	.L18
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#4,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	cmp/hi	r1,r2
	bt	.L19
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(60,r2),r2
	add	#4,r2
	mov.l	r2,@(60,r1)
	bra	.L20
	nop
	.align 1
.L19:
	bra	.L20
	nop
	.align 1
.L21:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L20:
	mov	r14,r2
	add	#-40,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/hs	r1,r2
	bf	.L21
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r1
	mov	r1,r0
	add	#24,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_memset, .-_memset
	.align 1
	.global	_memcpy
	.type	_memcpy, @function
_memcpy:
	mov.l	r14,@-r15
	add	#-32,r15
	mov	r15,r14
	mov	r14,r1
	add	#-32,r1
	mov.l	r4,@(40,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	r5,@(36,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	r6,@(32,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r3
	add	#-32,r3
	mov	r14,r2
	add	#-32,r2
	mov.l	@(60,r3),r3
	mov.l	@(32,r2),r2
	add	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.l	r2,@(44,r1)
	mov	r14,r2
	add	#-32,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r2),r2
	mov.l	@(44,r1),r1
	xor	r2,r1
	mov	#3,r0
	and	r0,r1
	tst	r1,r1
	bt	.L51
	bra	.L26
	nop
.L51:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r1
	mov	#3,r2
	and	r2,r1
	mov	#0,r3
	mov	#3,r2
	sub	r3,r1
	cmp/hi	r2,r1
	bt	.L48
	mova	.L29,r0
	shll2	r1
	mov.l	@(r0,r1),r1
	braf	r1
	nop
.L30:
	.align 2
.L29:
	.long	.L49-.L30
	.long	.L31-.L30
	.long	.L32-.L30
	.long	.L33-.L30
	.align 1
.L31:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-32,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L32:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-32,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L33:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-32,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	bra	.L49
	nop
	.align 1
.L48:
	nop
	bra	.L35
	nop
	.align 1
.L49:
	nop
	bra	.L35
	nop
	.align 1
.L36:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#4,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.l	@(4,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#8,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.l	@(8,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#12,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.l	@(12,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(60,r2),r2
	add	#16,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	add	#16,r2
	mov.l	r2,@(56,r1)
.L35:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#16,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r1
	cmp/hi	r1,r2
	bf	.L36
	bra	.L37
	nop
	.align 1
.L38:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#4,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.l	@(4,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(60,r2),r2
	add	#8,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	add	#8,r2
	mov.l	r2,@(56,r1)
.L37:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#8,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r1
	cmp/hi	r1,r2
	bf	.L38
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#4,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r1
	cmp/hi	r1,r2
	bt	.L39
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(60,r2),r2
	add	#4,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	add	#4,r2
	mov.l	r2,@(56,r1)
	bra	.L40
	nop
	.align 1
.L39:
	bra	.L40
	nop
	.align 1
.L41:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-32,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L40:
	mov	r14,r2
	add	#-32,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r2),r2
	mov.l	@(52,r1),r1
	cmp/hs	r1,r2
	bf	.L41
	mov	r14,r1
	add	#-32,r1
	mov.l	@(40,r1),r1
	bra	.L42
	nop
	.align 1
.L26:
	bra	.L43
	nop
	.align 1
.L44:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(1,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#2,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(2,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#3,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(3,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#4,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(4,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#5,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(5,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#6,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(6,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#7,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(7,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(60,r2),r2
	add	#8,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	add	#8,r2
	mov.l	r2,@(56,r1)
.L43:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#8,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r1
	cmp/hi	r1,r2
	bf	.L44
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#4,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r1
	cmp/hi	r1,r2
	bt	.L45
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(1,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#2,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(2,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	add	#3,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov.b	@(3,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(60,r2),r2
	add	#4,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	add	#4,r2
	mov.l	r2,@(56,r1)
	bra	.L46
	nop
	.align 1
.L45:
	bra	.L46
	nop
	.align 1
.L47:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-32,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-32,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L46:
	mov	r14,r2
	add	#-32,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r2),r2
	mov.l	@(52,r1),r1
	cmp/hs	r1,r2
	bf	.L47
	mov	r14,r1
	add	#-32,r1
	mov.l	@(40,r1),r1
.L42:
	mov	r1,r0
	add	#32,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_memcpy, .-_memcpy
	.align 1
	.global	_memcmp
	.type	_memcmp, @function
_memcmp:
	mov.l	r14,@-r15
	add	#-16,r15
	mov	r15,r14
	mov	r14,r1
	add	#-48,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	r5,@(52,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	r6,@(48,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	bra	.L53
	nop
	.align 1
.L55:
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L53:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	cmp/eq	r1,r2
	bf	.L54
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	cmp/pl	r1
	bt	.L55
.L54:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	cmp/pl	r1
	bt	.L56
	mov	#0,r1
	bra	.L57
	nop
	.align 1
.L56:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	cmp/hi	r1,r2
	bf	.L58
	mov	#1,r1
	bra	.L57
	nop
	.align 1
.L58:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	cmp/hs	r1,r2
	bt	.L59
	mov	#-1,r1
	bra	.L57
	nop
	.align 1
.L59:
	mov	#0,r1
.L57:
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_memcmp, .-_memcmp
	.align 1
	.global	_strcpy
	.type	_strcpy, @function
_strcpy:
	mov.l	r14,@-r15
	add	#-16,r15
	mov	r15,r14
	mov	r14,r1
	add	#-48,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	r5,@(48,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	bra	.L61
	nop
	.align 1
.L62:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-48,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r2),r2
	mov	r14,r3
	add	#-48,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(60,r3)
	mov.b	@r2,r2
	mov.b	r2,@r1
.L61:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bf	.L62
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-48,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r2),r2
	mov	r14,r3
	add	#-48,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(60,r3)
	mov.b	@r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_strcpy, .-_strcpy
	.global	_rand_seed
	.data
	.align 2
	.type	_rand_seed, @object
	.size	_rand_seed, 8
_rand_seed:
	.long	1768515945
	.long	1768515945
	.text
	.align 1
	.global	_rand
	.type	_rand, @function
_rand:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L67,r0
	mov.l	.L67,r12
	add	r0,r12
	mov	r15,r14
	mov.l	.L71,r3
	add	r12,r3
	mov.l	@r3,r3
	mov.l	@(4,r3),r4
	mov.l	@r3,r3
	mov	#-59,r6
	mul.l	r6,r3
	sts	macl,r5
	mov.l	.L69,r6
	mul.l	r6,r4
	sts	macl,r6
	add	r5,r6
	mov	#-59,r5
	dmulu.l	r5,r4
	sts	macl,r2
	sts	mach,r1
	mov	r6,r3
	add	r1,r3
	mov	r3,r1
	mov	#0,r3
	mov	#1,r4
	clrt
	addc	r4,r2
	addc	r3,r1
	mov.l	.L71,r3
	add	r12,r3
	mov.l	@r3,r3
	mov.l	r1,@r3
	mov.l	r2,@(4,r3)
	mov.l	.L71,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@(4,r1),r2
	mov.l	@r1,r1
	mov	r1,r8
	shlr16	r8
	mov	#0,r7
	mov	r8,r1
	mov	r1,r0
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L72:
	.align 2
.L67:
	.long	_GLOBAL_OFFSET_TABLE_
.L71:
	.long	_rand_seed@GOT
.L69:
	.long	65535
	.size	_rand, .-_rand
	.align 1
	.global	_srand
	.type	_srand, @function
_srand:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L75,r0
	mov.l	.L75,r12
	add	r0,r12
	add	#-4,r15
	mov	r15,r14
	mov	r14,r3
	add	#-60,r3
	mov.l	r4,@(60,r3)
	mov	r14,r3
	add	#-60,r3
	mov.l	@(60,r3),r3
	mov	r3,r10
	mov	#0,r9
	mov.l	.L85,r3
	add	r12,r3
	mov.l	@r3,r3
	mov.l	r9,@r3
	mov.l	r10,@(4,r3)
	mov.l	.L85,r3
	add	r12,r3
	mov.l	@r3,r3
	mov.l	@(4,r3),r4
	mov.l	@r3,r3
	mov	#-59,r0
	mul.l	r0,r3
	sts	macl,r9
	mov.l	.L84,r0
	mul.l	r0,r4
	sts	macl,r0
	add	r9,r0
	mov	#-59,r9
	dmulu.l	r9,r4
	sts	macl,r6
	sts	mach,r5
	mov	r0,r3
	add	r5,r3
	mov	r3,r5
	mov	#0,r3
	mov	#1,r4
	clrt
	addc	r6,r4
	addc	r5,r3
	mov.l	.L85,r6
	add	r12,r6
	mov.l	@r6,r6
	mov.l	r3,@r6
	mov.l	r4,@(4,r6)
	mov.l	.L85,r3
	add	r12,r3
	mov.l	@r3,r3
	mov.l	@(4,r3),r4
	mov.l	@r3,r3
	mov	#-59,r6
	mul.l	r6,r3
	sts	macl,r5
	mov.l	.L84,r6
	mul.l	r6,r4
	sts	macl,r6
	add	r5,r6
	mov	#-59,r5
	dmulu.l	r5,r4
	sts	macl,r8
	sts	mach,r7
	mov	r6,r3
	add	r7,r3
	mov	r3,r7
	mov	#0,r3
	mov	#1,r4
	clrt
	addc	r8,r4
	addc	r7,r3
	mov.l	.L85,r7
	add	r12,r7
	mov.l	@r7,r7
	mov.l	r3,@r7
	mov.l	r4,@(4,r7)
	mov.l	.L85,r3
	add	r12,r3
	mov.l	@r3,r3
	mov.l	@(4,r3),r4
	mov.l	@r3,r3
	mov	#-59,r7
	mul.l	r7,r3
	sts	macl,r6
	mov.l	.L84,r7
	mul.l	r7,r4
	sts	macl,r7
	add	r6,r7
	mov	#-59,r6
	dmulu.l	r6,r4
	sts	macl,r2
	sts	mach,r1
	mov	r7,r3
	add	r1,r3
	mov	r3,r1
	mov	#0,r3
	mov	#1,r4
	clrt
	addc	r4,r2
	addc	r3,r1
	mov.l	.L85,r3
	add	r12,r3
	mov.l	@r3,r3
	mov.l	r1,@r3
	mov.l	r2,@(4,r3)
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	rts	
	nop
.L86:
	.align 2
.L75:
	.long	_GLOBAL_OFFSET_TABLE_
.L85:
	.long	_rand_seed@GOT
.L84:
	.long	65535
	.size	_srand, .-_srand
	.align 1
	.global	_rand_r
	.type	_rand_r, @function
_rand_r:
	mov.l	r14,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@r1,r2
	mov	r14,r3
	add	#-56,r3
	mov	r2,r1
	shll8	r1
	shll2	r1
	shll2	r1
	sub	r2,r1
	shll2	r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(60,r3)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_rand_r, .-_rand_r
	.comm	_tkmm_pagebase,4,4
	.comm	_tkmm_pageend,4,4
	.comm	_tkmm_pagebmp,4096,4
	.comm	_tkmm_maxpage,4,4
	.comm	_tkmm_pagerov,4,4
	.global	___ashrsi3
	.align 1
	.global	_TKMM_FindFreePages
	.type	_TKMM_FindFreePages, @function
_TKMM_FindFreePages:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L126,r0
	mov.l	.L126,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r1
	cmp/pl	r1
	bt	.L90
	mov	#-1,r1
	bra	.L91
	nop
	.align 1
.L90:
	mov	r14,r1
	add	#-44,r1
	mov.l	.L127,r2
	add	r12,r2
	mov.l	@r2,r2
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	.L128,r2
	add	r12,r2
	mov.l	@r2,r2
	mov.l	@r2,r2
	mov.l	r2,@(56,r1)
	bra	.L92
	nop
	.align 1
.L123:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L133,r2
	add	r12,r2
	mov.l	@r2,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	#7,r3
	and	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L130,r1
	bsrf	r1
.LPCS0:
	nop
	mov	r0,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bf	.L146
	bra	.L99
	nop
.L146:
	bra	.L100
	nop
	.align 1
.L102:
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
.L100:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L133,r2
	add	r12,r2
	mov.l	@r2,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r2
	mov.w	.L132,r1
	cmp/eq	r1,r2
	bf	.L101
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/ge	r1,r2
	bf	.L102
.L101:
	bra	.L103
	nop
	.align 1
.L111:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L103:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L133,r2
	add	r12,r2
	mov.l	@r2,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	#7,r3
	and	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L134,r1
	bsrf	r1
.LPCS1:
	nop
	mov	r0,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bt	.L110
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/ge	r1,r2
	bf	.L111
.L110:
	bra	.L92
	nop
	.align 1
.L99:
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
	bf	.L112
	bra	.L113
	nop
	.align 1
.L112:
	bra	.L114
	nop
	.align 1
.L132:
	.short	255
.L135:
	.align 2
.L126:
	.long	_GLOBAL_OFFSET_TABLE_
.L127:
	.long	_tkmm_pagerov@GOT
.L128:
	.long	_tkmm_maxpage@GOT
.L133:
	.long	_tkmm_pagebmp@GOT
.L130:
	.long	___ashrsi3@PLT-(.LPCS0+2-.)
.L134:
	.long	___ashrsi3@PLT-(.LPCS1+2-.)
	.align 1
.L122:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L114:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	.L136,r2
	add	r12,r2
	mov.l	@r2,r2
	mov	r1,r0
	mov.b	@(r0,r2),r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	#7,r3
	and	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L137,r1
	bsrf	r1
.LPCS2:
	nop
	mov	r0,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bf	.L121
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(48,r1),r1
	cmp/ge	r1,r2
	bf	.L122
.L121:
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(48,r1),r1
	cmp/ge	r1,r2
	bf	.L92
	mov.l	.L140,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	bra	.L91
	nop
	.align 1
.L92:
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/ge	r1,r2
	bt	.L145
	bra	.L123
	nop
.L145:
.L113:
	mov.l	.L140,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	tst	r1,r1
	bt	.L124
	mov.l	.L140,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	#0,r2
	mov.l	r2,@r1
	mov	r14,r8
	add	#-44,r8
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r4
	mov.l	.L141,r3
	bsrf	r3
.LPCS3:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	bra	.L91
	nop
	.align 1
.L124:
	mov	#-1,r1
.L91:
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L142:
	.align 2
.L136:
	.long	_tkmm_pagebmp@GOT
.L137:
	.long	___ashrsi3@PLT-(.LPCS2+2-.)
.L140:
	.long	_tkmm_pagerov@GOT
.L141:
	.long	_TKMM_FindFreePages@PLT-(.LPCS3+2-.)
	.size	_TKMM_FindFreePages, .-_TKMM_FindFreePages
	.align 1
	.global	_TKMM_AllocPages
	.type	_TKMM_AllocPages, @function
_TKMM_AllocPages:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L153,r0
	mov.l	.L153,r12
	add	r0,r12
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
	mov.l	.L154,r1
	bsrf	r1
.LPCS4:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L148
	mov	#-1,r1
	bra	.L149
	nop
	.align 1
.L148:
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
	bra	.L150
	nop
	.align 1
.L151:
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
	mov.l	.L157,r3
	add	r12,r3
	mov.l	@r3,r3
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
	mov.l	.L156,r7
	add	r12,r7
	mov	r1,r0
	jsr	@r7
	nop
	mov	r0,r1
	exts.b	r1,r1
	or	r3,r1
	exts.b	r1,r1
	extu.b	r1,r3
	mov.l	.L157,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	r2,r0
	mov.b	r3,@(r0,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L150:
	mov	r14,r2
	add	#-48,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r2),r2
	mov.l	@(52,r1),r1
	cmp/ge	r1,r2
	bf	.L151
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
.L149:
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L158:
	.align 2
.L153:
	.long	_GLOBAL_OFFSET_TABLE_
.L154:
	.long	_TKMM_FindFreePages@PLT-(.LPCS4+2-.)
.L157:
	.long	_tkmm_pagebmp@GOT
.L156:
	.long	___ashlsi3_r0@GOTOFF
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
	mov.l	.L161,r2
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
.L162:
	.align 2
.L161:
	.long	285212672
	.size	_TKMM_PageToPointer, .-_TKMM_PageToPointer
	.align 1
	.global	_TKMM_PointerToPage
	.type	_TKMM_PointerToPage, @function
_TKMM_PointerToPage:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L166,r0
	mov.l	.L166,r12
	add	r0,r12
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
	mov.l	.L167,r2
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r4
	mov.l	.L168,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L169:
	.align 2
.L166:
	.long	_GLOBAL_OFFSET_TABLE_
.L167:
	.long	-285212672
.L168:
	.long	___ashiftrt_r4_12@GOTOFF
	.size	_TKMM_PointerToPage, .-_TKMM_PointerToPage
	.align 1
	.global	_TKMM_PageAlloc
	.type	_TKMM_PageAlloc, @function
_TKMM_PageAlloc:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L174,r0
	mov.l	.L174,r12
	add	r0,r12
	add	#-12,r15
	mov	r15,r14
	mov	r14,r1
	add	#-52,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r2
	mov.w	.L175,r1
	add	r2,r1
	mov	r1,r4
	mov.l	.L176,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r1
	mov	r14,r8
	add	#-52,r8
	mov	r1,r4
	mov.l	.L177,r1
	bsrf	r1
.LPCS5:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	cmp/pz	r1
	bt	.L171
	mov	#0,r1
	bra	.L172
	nop
	.align 1
.L171:
	mov	r14,r8
	add	#-52,r8
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r4
	mov.l	.L178,r1
	bsrf	r1
.LPCS6:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r1
.L172:
	mov	r1,r0
	add	#12,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L175:
	.short	4095
.L179:
	.align 2
.L174:
	.long	_GLOBAL_OFFSET_TABLE_
.L176:
	.long	___ashiftrt_r4_12@GOTOFF
.L177:
	.long	_TKMM_AllocPages@PLT-(.LPCS5+2-.)
.L178:
	.long	_TKMM_PageToPointer@PLT-(.LPCS6+2-.)
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
	bra	.L181
	nop
	.align 1
.L182:
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
.L181:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov	r1,r2
	mov	#1,r1
	cmp/gt	r1,r2
	bt	.L182
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
	bra	.L185
	nop
	.align 1
.L186:
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
.L185:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	shar	r1
	shar	r1
	shar	r1
	mov	r1,r2
	mov	#1,r1
	cmp/gt	r1,r2
	bt	.L186
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
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L192,r0
	mov.l	.L192,r12
	add	r0,r12
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
	bt	.L189
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r1
	bra	.L190
	nop
	.align 1
.L189:
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
	mov.l	.L193,r3
	add	r12,r3
	mov	r2,r0
	jsr	@r3
	nop
	mov	r0,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
.L190:
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L194:
	.align 2
.L192:
	.long	_GLOBAL_OFFSET_TABLE_
.L193:
	.long	___ashlsi3_r0@GOTOFF
	.size	_TKMM_FxiToSize, .-_TKMM_FxiToSize
	.comm	_tkmm_freelist,1024,4
	.align 1
	.global	_TKMM_Malloc
	.type	_TKMM_Malloc, @function
_TKMM_Malloc:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L208,r0
	mov.l	.L208,r12
	add	r0,r12
	add	#-36,r15
	mov	r15,r14
	mov	r14,r1
	add	#-28,r1
	mov.l	r4,@(28,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(28,r1),r1
	cmp/pl	r1
	bt	.L196
	mov	#0,r1
	bra	.L197
	nop
	.align 1
.L196:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(28,r1),r2
	mov.w	.L209,r1
	cmp/gt	r1,r2
	bt	.L198
	mov	r14,r1
	add	#-28,r1
	mov.w	.L210,r2
	mov.l	r2,@(28,r1)
.L198:
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
	mov.l	.L211,r1
	bsrf	r1
.LPCS7:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-28,r1
	mov.l	.L213,r2
	add	r12,r2
	mov.l	@r2,r3
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
	bt	.L199
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	add	#16,r1
	mov.l	@r1,r2
	mov.l	.L213,r1
	add	r12,r1
	mov.l	@r1,r3
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
	bra	.L197
	nop
	.align 1
.L199:
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(56,r1)
	bra	.L200
	nop
	.align 1
.L209:
	.short	255
.L210:
	.short	256
.L214:
	.align 2
.L208:
	.long	_GLOBAL_OFFSET_TABLE_
.L211:
	.long	_TKMM_SizeToFxiU@PLT-(.LPCS7+2-.)
.L213:
	.long	_tkmm_freelist@GOT
	.align 1
.L202:
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	.L215,r2
	add	r12,r2
	mov.l	@r2,r3
	mov	r14,r2
	add	#-28,r2
	mov.l	@(56,r2),r2
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
.L200:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bf	.L201
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r2
	mov.w	.L216,r1
	cmp/gt	r1,r2
	bf	.L202
.L201:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bt	.L232
	bra	.L203
	nop
.L232:
	mov	r14,r8
	add	#-28,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r4
	mov.l	.L217,r1
	bsrf	r1
.LPCS8:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r2
	mov.l	.L218,r1
	mov	r2,r3
	add	r1,r3
	mov	r14,r1
	add	#-28,r1
	mov.l	.L219,r2
	and	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r8
	add	#-28,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r4
	mov.l	.L220,r2
	bsrf	r2
.LPCS9:
	nop
	mov	r0,r1
	mov.l	r1,@(44,r8)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	tst	r1,r1
	bf	.L204
	mov	#0,r1
	bra	.L197
	nop
	.align 1
.L216:
	.short	254
.L221:
	.align 2
.L215:
	.long	_tkmm_freelist@GOT
.L217:
	.long	_TKMM_FxiToSize@PLT-(.LPCS8+2-.)
.L218:
	.long	65535
.L219:
	.long	-65536
.L220:
	.long	_TKMM_PageAlloc@PLT-(.LPCS9+2-.)
	.align 1
.L204:
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
	mov.l	.L222,r1
	bsrf	r1
.LPCS10:
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
.L203:
	mov	r14,r8
	add	#-28,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r4
	mov.l	.L223,r1
	bsrf	r1
.LPCS11:
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
	mov.w	.L224,r1
	cmp/gt	r1,r2
	bt	.L231
	bra	.L205
	nop
.L231:
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
	bf	.L206
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	mov.l	@r1,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(4,r1)
.L206:
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
	mov.l	.L225,r1
	bsrf	r1
.LPCS12:
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
	mov.l	.L227,r3
	add	r12,r3
	mov.l	@r3,r3
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
	mov.l	.L227,r2
	add	r12,r2
	mov.l	@r2,r2
	shll2	r1
	add	r2,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@r1
.L205:
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
.L197:
	mov	r1,r0
	add	#36,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L224:
	.short	256
.L228:
	.align 2
.L222:
	.long	_TKMM_SizeToFxiD@PLT-(.LPCS10+2-.)
.L223:
	.long	_TKMM_FxiToSize@PLT-(.LPCS11+2-.)
.L225:
	.long	_TKMM_SizeToFxiD@PLT-(.LPCS12+2-.)
.L227:
	.long	_tkmm_freelist@GOT
	.size	_TKMM_Malloc, .-_TKMM_Malloc
	.align 1
	.global	_TKMM_Free
	.type	_TKMM_Free, @function
_TKMM_Free:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L237,r0
	mov.l	.L237,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bf	.L234
	mov	#-1,r1
	bra	.L235
	nop
	.align 1
.L234:
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
	mov.l	.L239,r3
	add	r12,r3
	mov.l	@r3,r3
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
	mov.l	.L239,r2
	add	r12,r2
	mov.l	@r2,r2
	shll2	r1
	add	r2,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	mov	#0,r1
.L235:
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L240:
	.align 2
.L237:
	.long	_GLOBAL_OFFSET_TABLE_
.L239:
	.long	_tkmm_freelist@GOT
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
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L247,r0
	mov.l	.L247,r12
	add	r0,r12
	add	#-4,r15
	mov	r15,r14
	mov.l	.L249,r1
	add	r12,r1
	mov.l	@r1,r1
	tst	r1,r1
	bt	.L242
	bra	.L241
	nop
	.align 1
.L242:
	mov.l	.L249,r1
	add	r12,r1
	mov	#1,r2
	mov.l	r2,@r1
	mov.l	.L258,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	.L251,r2
	mov.l	r2,@r1
	mov.l	.L257,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	.L253,r2
	mov.l	r2,@r1
	mov.l	.L257,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r2
	mov.l	.L258,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	shlr8	r1
	shlr2	r1
	shlr2	r1
	mov	r1,r2
	mov.l	.L256,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	r2,@r1
	mov.l	.L257,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r2
	mov.l	.L258,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	mov	r1,r2
	shlr16	r2
	shlr2	r2
	shlr2	r2
	mov.l	.L259,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L260,r1
	bsrf	r1
.LPCS13:
	nop
	mov	r14,r1
	add	#-60,r1
	mov	#0,r2
	mov.l	r2,@(60,r1)
	bra	.L244
	nop
	.align 1
.L245:
	mov.l	.L261,r1
	add	r12,r1
	mov.l	@r1,r2
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
.L244:
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r2
	mov.w	.L262,r1
	cmp/gt	r1,r2
	bf	.L245
.L241:
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
	.align 1
.L262:
	.short	255
.L263:
	.align 2
.L247:
	.long	_GLOBAL_OFFSET_TABLE_
.L249:
	.long	_init.1683@GOTOFF
.L258:
	.long	_tkmm_pagebase@GOT
.L251:
	.long	285212672
.L257:
	.long	_tkmm_pageend@GOT
.L253:
	.long	402653184
.L256:
	.long	_tkmm_maxpage@GOT
.L259:
	.long	.LC0@GOTOFF
.L260:
	.long	_printf@PLT-(.LPCS13+2-.)
.L261:
	.long	_tkmm_freelist@GOT
	.size	_tkmm_init, .-_tkmm_init
	.comm	_tkspi_ctl_speed,4,4
	.comm	_tkspi_ctl_status,4,4
	.comm	_tkspi_init_ok,1,1
	.align 1
	.global	_TKSPI_ChipSel
	.type	_TKSPI_ChipSel, @function
_TKSPI_ChipSel:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L272,r0
	mov.l	.L272,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov.l	.L280,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
	bra	.L265
	nop
	.align 1
.L266:
	mov.l	.L280,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
.L265:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	#2,r2
	and	r2,r1
	tst	r1,r1
	bf	.L266
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bt	.L267
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#1,r1
	cmp/eq	r1,r2
	bf	.L268
	mov.l	.L277,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r2
	mov.l	.L281,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	r2,@r1
	bra	.L270
	nop
	.align 1
.L268:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#2,r1
	cmp/eq	r1,r2
	bf	.L270
	mov.l	.L277,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	mov	#5,r2
	or	r1,r2
	mov.l	.L281,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	r2,@r1
	bra	.L270
	nop
	.align 1
.L267:
	mov.l	.L281,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	#1,r2
	mov.l	r2,@r1
.L270:
	mov.l	.L280,r1
	mov.l	.L281,r2
	add	r12,r2
	mov.l	@r2,r2
	mov.l	@r2,r2
	mov.l	r2,@r1
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L282:
	.align 2
.L272:
	.long	_GLOBAL_OFFSET_TABLE_
.L280:
	.long	-1412628416
.L277:
	.long	_tkspi_ctl_speed@GOT
.L281:
	.long	_tkspi_ctl_status@GOT
	.size	_TKSPI_ChipSel, .-_TKSPI_ChipSel
	.align 1
	.global	_TKSPI_SetSpeed
	.type	_TKSPI_SetSpeed, @function
_TKSPI_SetSpeed:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L287,r0
	mov.l	.L287,r12
	add	r0,r12
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bf	.L284
	mov.l	.L290,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	.L289,r2
	mov.l	r2,@r1
	bra	.L283
	nop
	.align 1
.L284:
	mov.l	.L290,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	#0,r2
	mov.l	r2,@r1
.L283:
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L291:
	.align 2
.L287:
	.long	_GLOBAL_OFFSET_TABLE_
.L290:
	.long	_tkspi_ctl_speed@GOT
.L289:
	.long	-134217728
	.size	_TKSPI_SetSpeed, .-_TKSPI_SetSpeed
	.align 1
	.global	_TKSPI_XchByte
	.type	_TKSPI_XchByte, @function
_TKSPI_XchByte:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L297,r0
	mov.l	.L297,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r4,r2
	mov	r14,r1
	add	#-8,r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov.l	.L303,r1
	mov	r14,r2
	add	#-8,r2
	mov.b	@(8,r2),r0
	mov	r0,r2
	extu.b	r2,r2
	mov.l	r2,@r1
	mov.l	.L302,r1
	mov.l	.L300,r2
	add	r12,r2
	mov.l	@r2,r2
	mov.l	@r2,r2
	mov	#2,r3
	or	r3,r2
	mov.l	r2,@r1
	mov.l	.L302,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
	bra	.L293
	nop
	.align 1
.L294:
	mov.l	.L302,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
.L293:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	#2,r0
	and	r0,r1
	tst	r1,r1
	bf	.L294
	mov.l	.L303,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	extu.b	r1,r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L304:
	.align 2
.L297:
	.long	_GLOBAL_OFFSET_TABLE_
.L303:
	.long	-1412628412
.L302:
	.long	-1412628416
.L300:
	.long	_tkspi_ctl_status@GOT
	.size	_TKSPI_XchByte, .-_TKSPI_XchByte
	.align 1
	.global	_TKSPI_ReadBasic
	.type	_TKSPI_ReadBasic, @function
_TKSPI_ReadBasic:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L310,r0
	mov.l	.L310,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r5,@(52,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r6,@(48,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r7,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r4
	mov.l	.L311,r1
	bsrf	r1
.LPCS14:
	nop
	mov	#3,r4
	mov.l	.L312,r1
	bsrf	r1
.LPCS15:
	nop
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	shlr16	r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L313,r1
	bsrf	r1
.LPCS16:
	nop
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	shlr8	r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L314,r1
	bsrf	r1
.LPCS17:
	nop
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L315,r1
	bsrf	r1
.LPCS18:
	nop
	bra	.L306
	nop
	.align 1
.L307:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r8
	mov	r14,r1
	add	#-44,r1
	mov	r8,r2
	add	#1,r2
	mov.l	r2,@(60,r1)
	mov	#0,r4
	mov.l	.L316,r1
	bsrf	r1
.LPCS19:
	nop
	mov	r0,r1
	mov.b	r1,@r8
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(44,r2),r2
	add	#-1,r2
	mov.l	r2,@(44,r1)
.L306:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r1
	tst	r1,r1
	bf	.L307
	mov	#0,r4
	mov.l	.L317,r1
	bsrf	r1
.LPCS20:
	nop
	mov	#1,r1
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L318:
	.align 2
.L310:
	.long	_GLOBAL_OFFSET_TABLE_
.L311:
	.long	_TKSPI_ChipSel@PLT-(.LPCS14+2-.)
.L312:
	.long	_TKSPI_XchByte@PLT-(.LPCS15+2-.)
.L313:
	.long	_TKSPI_XchByte@PLT-(.LPCS16+2-.)
.L314:
	.long	_TKSPI_XchByte@PLT-(.LPCS17+2-.)
.L315:
	.long	_TKSPI_XchByte@PLT-(.LPCS18+2-.)
.L316:
	.long	_TKSPI_XchByte@PLT-(.LPCS19+2-.)
.L317:
	.long	_TKSPI_ChipSel@PLT-(.LPCS20+2-.)
	.size	_TKSPI_ReadBasic, .-_TKSPI_ReadBasic
	.align 1
	.global	_TKSPI_DelayUSec
	.type	_TKSPI_DelayUSec, @function
_TKSPI_DelayUSec:
	mov.l	r14,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_TKSPI_DelayUSec, .-_TKSPI_DelayUSec
	.section	.rodata
	.align 2
.LC1:
	.string	"Err %02X\n"
	.text
	.align 1
	.global	_TKSPI_ReadData
	.type	_TKSPI_ReadData, @function
_TKSPI_ReadData:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L330,r0
	mov.l	.L330,r12
	add	r0,r12
	add	#-24,r15
	mov	r15,r14
	mov	r14,r1
	add	#-40,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r5,@(40,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	.L331,r2
	mov.l	r2,@(56,r1)
	bra	.L321
	nop
	.align 1
.L324:
	mov	r14,r8
	add	#8,r8
	mov.w	.L343,r1
	mov	r1,r4
	mov.l	.L333,r1
	bsrf	r1
.LPCS21:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	add	#8,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L343,r1
	cmp/eq	r1,r2
	bt	.L322
	bra	.L323
	nop
	.align 1
.L322:
	mov	#10,r4
	mov.l	.L335,r1
	bsrf	r1
.LPCS22:
	nop
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(56,r2),r2
	add	#-1,r2
	mov.l	r2,@(56,r1)
.L321:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bf	.L324
.L323:
	mov	r14,r1
	add	#8,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L336,r1
	cmp/eq	r1,r2
	bt	.L325
	mov	r14,r1
	add	#8,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.l	.L337,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L338,r1
	bsrf	r1
.LPCS23:
	nop
	mov	#-1,r1
	bra	.L326
	nop
	.align 1
.L325:
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(48,r1)
	bra	.L327
	nop
	.align 1
.L328:
	mov	r14,r8
	add	#8,r8
	mov.w	.L343,r1
	mov	r1,r4
	mov.l	.L340,r1
	bsrf	r1
.LPCS24:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#8,r2
	mov.b	@(7,r2),r0
	mov	r0,r2
	mov.b	r2,@r1
.L327:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#-1,r3
	mov.l	r3,@(48,r2)
	cmp/pl	r1
	bt	.L328
	mov.w	.L343,r1
	mov	r1,r4
	mov.l	.L342,r1
	bsrf	r1
.LPCS25:
	nop
	mov.w	.L343,r1
	mov	r1,r4
	mov.l	.L344,r1
	bsrf	r1
.LPCS26:
	nop
	mov	#0,r1
.L326:
	mov	r1,r0
	add	#24,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L343:
	.short	255
.L336:
	.short	254
.L345:
	.align 2
.L330:
	.long	_GLOBAL_OFFSET_TABLE_
.L331:
	.long	65536
.L333:
	.long	_TKSPI_XchByte@PLT-(.LPCS21+2-.)
.L335:
	.long	_TKSPI_DelayUSec@PLT-(.LPCS22+2-.)
.L337:
	.long	.LC1@GOTOFF
.L338:
	.long	_printf@PLT-(.LPCS23+2-.)
.L340:
	.long	_TKSPI_XchByte@PLT-(.LPCS24+2-.)
.L342:
	.long	_TKSPI_XchByte@PLT-(.LPCS25+2-.)
.L344:
	.long	_TKSPI_XchByte@PLT-(.LPCS26+2-.)
	.size	_TKSPI_ReadData, .-_TKSPI_ReadData
	.align 1
	.global	_TKSPI_WaitReady
	.type	_TKSPI_WaitReady, @function
_TKSPI_WaitReady:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L352,r0
	mov.l	.L352,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	.L353,r2
	mov.l	r2,@(60,r1)
	bra	.L347
	nop
	.align 1
.L350:
	mov	r14,r8
	add	#-8,r8
	mov.w	.L356,r1
	mov	r1,r4
	mov.l	.L355,r1
	bsrf	r1
.LPCS27:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(11,r8)
	mov	r14,r1
	add	#-8,r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L356,r1
	cmp/eq	r1,r2
	bf	.L348
	mov	#1,r1
	bra	.L349
	nop
	.align 1
.L348:
	mov	#10,r4
	mov.l	.L357,r1
	bsrf	r1
.LPCS28:
	nop
	mov	r14,r1
	add	#-56,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(60,r2),r2
	add	#-1,r2
	mov.l	r2,@(60,r1)
.L347:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bf	.L350
	mov	#0,r1
.L349:
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L356:
	.short	255
.L358:
	.align 2
.L352:
	.long	_GLOBAL_OFFSET_TABLE_
.L353:
	.long	65536
.L355:
	.long	_TKSPI_XchByte@PLT-(.LPCS27+2-.)
.L357:
	.long	_TKSPI_DelayUSec@PLT-(.LPCS28+2-.)
	.size	_TKSPI_WaitReady, .-_TKSPI_WaitReady
	.align 1
	.global	_TKSPI_WriteData
	.type	_TKSPI_WriteData, @function
_TKSPI_WriteData:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L365,r0
	mov.l	.L365,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r5,@(44,r1)
	mov.l	.L366,r1
	bsrf	r1
.LPCS29:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L360
	mov	#-1,r1
	bra	.L361
	nop
	.align 1
.L360:
	mov	r14,r8
	add	#4,r8
	mov.w	.L367,r1
	mov	r1,r4
	mov.l	.L368,r1
	bsrf	r1
.LPCS30:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(56,r1)
	bra	.L362
	nop
	.align 1
.L363:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r14,r8
	add	#4,r8
	mov	r1,r4
	mov.l	.L369,r1
	bsrf	r1
.LPCS31:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
.L362:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#-1,r3
	mov.l	r3,@(56,r2)
	cmp/pl	r1
	bt	.L363
	mov.w	.L372,r1
	mov	r1,r4
	mov.l	.L371,r1
	bsrf	r1
.LPCS32:
	nop
	mov.w	.L372,r1
	mov	r1,r4
	mov.l	.L373,r1
	bsrf	r1
.LPCS33:
	nop
	mov	#0,r1
.L361:
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L367:
	.short	254
.L372:
	.short	255
.L374:
	.align 2
.L365:
	.long	_GLOBAL_OFFSET_TABLE_
.L366:
	.long	_TKSPI_WaitReady@PLT-(.LPCS29+2-.)
.L368:
	.long	_TKSPI_XchByte@PLT-(.LPCS30+2-.)
.L369:
	.long	_TKSPI_XchByte@PLT-(.LPCS31+2-.)
.L371:
	.long	_TKSPI_XchByte@PLT-(.LPCS32+2-.)
.L373:
	.long	_TKSPI_XchByte@PLT-(.LPCS33+2-.)
	.size	_TKSPI_WriteData, .-_TKSPI_WriteData
	.align 1
	.global	_TKSPI_Deselect
	.type	_TKSPI_Deselect, @function
_TKSPI_Deselect:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L377,r0
	mov.l	.L377,r12
	add	r0,r12
	mov	r15,r14
	mov	#1,r4
	mov.l	.L378,r1
	bsrf	r1
.LPCS34:
	nop
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L379:
	.align 2
.L377:
	.long	_GLOBAL_OFFSET_TABLE_
.L378:
	.long	_TKSPI_ChipSel@PLT-(.LPCS34+2-.)
	.size	_TKSPI_Deselect, .-_TKSPI_Deselect
	.align 1
	.global	_TKSPI_Select
	.type	_TKSPI_Select, @function
_TKSPI_Select:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L384,r0
	mov.l	.L384,r12
	add	r0,r12
	mov	r15,r14
	mov	#0,r4
	mov.l	.L385,r1
	bsrf	r1
.LPCS35:
	nop
	mov.l	.L386,r1
	bsrf	r1
.LPCS36:
	nop
	mov	r0,r1
	tst	r1,r1
	bt	.L381
	mov	#1,r1
	bra	.L382
	nop
	.align 1
.L381:
	mov.l	.L387,r1
	bsrf	r1
.LPCS37:
	nop
	mov	#0,r1
.L382:
	mov	r1,r0
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L388:
	.align 2
.L384:
	.long	_GLOBAL_OFFSET_TABLE_
.L385:
	.long	_TKSPI_ChipSel@PLT-(.LPCS35+2-.)
.L386:
	.long	_TKSPI_WaitReady@PLT-(.LPCS36+2-.)
.L387:
	.long	_TKSPI_Deselect@PLT-(.LPCS37+2-.)
	.size	_TKSPI_Select, .-_TKSPI_Select
	.align 1
	.global	_TKSPI_PowerOff
	.type	_TKSPI_PowerOff, @function
_TKSPI_PowerOff:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L391,r0
	mov.l	.L391,r12
	add	r0,r12
	mov	r15,r14
	mov.l	.L392,r1
	bsrf	r1
.LPCS38:
	nop
	mov.l	.L393,r1
	bsrf	r1
.LPCS39:
	nop
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L394:
	.align 2
.L391:
	.long	_GLOBAL_OFFSET_TABLE_
.L392:
	.long	_TKSPI_Select@PLT-(.LPCS38+2-.)
.L393:
	.long	_TKSPI_Deselect@PLT-(.LPCS39+2-.)
	.size	_TKSPI_PowerOff, .-_TKSPI_PowerOff
	.section	.rodata
	.align 2
.LC2:
	.string	"TKSPI_SendCmd: Select Fail\n"
	.text
	.align 1
	.global	_TKSPI_SendCmd
	.type	_TKSPI_SendCmd, @function
_TKSPI_SendCmd:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L407,r0
	mov.l	.L407,r12
	add	r0,r12
	add	#-12,r15
	mov	r15,r14
	mov	r4,r2
	mov	r14,r1
	add	#-52,r1
	mov.l	r5,@(52,r1)
	mov	r14,r1
	add	#-4,r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r1
	add	#-4,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	cmp/pz	r1
	bt	.L396
	mov	r14,r1
	add	#-4,r1
	mov	r14,r2
	add	#-4,r2
	mov.b	@(8,r2),r0
	mov	r0,r2
	mov	#127,r3
	and	r3,r2
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r8
	add	#-4,r8
	mov	#55,r4
	mov	#0,r5
	mov.l	.L408,r1
	bsrf	r1
.LPCS40:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(13,r8)
	mov	r14,r1
	add	#-4,r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#1,r1
	cmp/hi	r1,r2
	bf	.L396
	mov	r14,r1
	add	#-4,r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	bra	.L397
	nop
	.align 1
.L396:
	mov.l	.L409,r1
	bsrf	r1
.LPCS41:
	nop
	mov.l	.L410,r2
	bsrf	r2
.LPCS42:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L398
	mov.l	.L411,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L412,r3
	bsrf	r3
.LPCS43:
	nop
	mov.w	.L422,r1
	bra	.L397
	nop
	.align 1
.L398:
	mov	r14,r1
	add	#-4,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	mov	#64,r2
	or	r2,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L414,r3
	bsrf	r3
.LPCS44:
	nop
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r1
	shlr16	r1
	shlr8	r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L415,r1
	bsrf	r1
.LPCS45:
	nop
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r1
	shlr16	r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L416,r2
	bsrf	r2
.LPCS46:
	nop
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r1
	shlr8	r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L417,r3
	bsrf	r3
.LPCS47:
	nop
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L418,r1
	bsrf	r1
.LPCS48:
	nop
	mov	r14,r1
	add	#-4,r1
	mov	#1,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov	r14,r1
	add	#-4,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L399
	mov	r14,r1
	add	#-4,r1
	mov	#-107,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
.L399:
	mov	r14,r1
	add	#-4,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#8,r1
	cmp/eq	r1,r2
	bf	.L400
	mov	r14,r1
	add	#-4,r1
	mov	#-121,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
.L400:
	mov	r14,r1
	add	#-4,r1
	mov.b	@(14,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L419,r1
	bsrf	r1
.LPCS49:
	nop
	mov	r14,r1
	add	#-4,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#12,r1
	cmp/eq	r1,r2
	bf	.L401
	mov.w	.L422,r1
	mov	r1,r4
	mov.l	.L421,r1
	bsrf	r1
.LPCS50:
	nop
.L401:
	mov	r14,r1
	add	#-4,r1
	mov	#10,r2
	mov	r2,r0
	mov.b	r0,@(15,r1)
	bra	.L402
	nop
	.align 1
.L405:
	mov	r14,r8
	add	#-4,r8
	mov.w	.L422,r1
	mov	r1,r4
	mov.l	.L423,r1
	bsrf	r1
.LPCS51:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(13,r8)
	mov	r14,r1
	add	#-4,r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	cmp/pz	r1
	bf	.L403
	bra	.L404
	nop
	.align 1
.L403:
	mov	r14,r1
	add	#-4,r1
	mov.b	@(15,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-4,r1
	add	#-1,r2
	mov	r2,r0
	mov.b	r0,@(15,r1)
.L402:
	mov	r14,r1
	add	#-4,r1
	mov.b	@(15,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L405
.L404:
	mov	r14,r1
	add	#-4,r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r1
.L397:
	mov	r1,r0
	add	#12,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L422:
	.short	255
.L424:
	.align 2
.L407:
	.long	_GLOBAL_OFFSET_TABLE_
.L408:
	.long	_TKSPI_SendCmd@PLT-(.LPCS40+2-.)
.L409:
	.long	_TKSPI_Deselect@PLT-(.LPCS41+2-.)
.L410:
	.long	_TKSPI_Select@PLT-(.LPCS42+2-.)
.L411:
	.long	.LC2@GOTOFF
.L412:
	.long	_printf@PLT-(.LPCS43+2-.)
.L414:
	.long	_TKSPI_XchByte@PLT-(.LPCS44+2-.)
.L415:
	.long	_TKSPI_XchByte@PLT-(.LPCS45+2-.)
.L416:
	.long	_TKSPI_XchByte@PLT-(.LPCS46+2-.)
.L417:
	.long	_TKSPI_XchByte@PLT-(.LPCS47+2-.)
.L418:
	.long	_TKSPI_XchByte@PLT-(.LPCS48+2-.)
.L419:
	.long	_TKSPI_XchByte@PLT-(.LPCS49+2-.)
.L421:
	.long	_TKSPI_XchByte@PLT-(.LPCS50+2-.)
.L423:
	.long	_TKSPI_XchByte@PLT-(.LPCS51+2-.)
	.size	_TKSPI_SendCmd, .-_TKSPI_SendCmd
	.align 1
	.global	_TKSPI_ReadSectors
	.type	_TKSPI_ReadSectors, @function
_TKSPI_ReadSectors:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L434,r0
	mov.l	.L434,r12
	add	r0,r12
	add	#-36,r15
	mov	r15,r14
	mov	r14,r3
	add	#-28,r3
	mov.l	r4,@(40,r3)
	mov	r14,r3
	add	#-28,r3
	mov.l	r5,@(32,r3)
	mov.l	r6,@(36,r3)
	mov	r14,r3
	add	#-28,r3
	mov.l	r7,@(28,r3)
	mov	r14,r3
	add	#-28,r3
	mov.l	@(28,r3),r7
	mov	#1,r3
	cmp/gt	r3,r7
	bt	.L447
	bra	.L426
	nop
.L447:
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r3
	add	#-24,r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov.l	@(32,r1),r1
	mov.l	r1,@(48,r3)
	mov.l	r2,@(52,r3)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(28,r2),r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	tst	r1,r1
	bt	.L427
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	mov	#55,r4
	mov	r1,r5
	mov.l	.L435,r1
	bsrf	r1
.LPCS52:
	nop
.L427:
	mov	r14,r1
	add	#-24,r1
	mov.l	@(52,r1),r1
	mov	#18,r4
	mov	r1,r5
	mov.l	.L436,r1
	bsrf	r1
.LPCS53:
	nop
	bra	.L428
	nop
	.align 1
.L429:
	mov	r14,r1
	add	#-24,r1
	mov.l	@(48,r1),r1
	mov	r1,r9
	mov	#0,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	r9,@(44,r1)
	mov	r14,r1
	add	#-28,r1
	mov.w	.L443,r2
	mov.l	@(60,r1),r4
	mov	r2,r5
	mov.l	.L438,r1
	bsrf	r1
.LPCS54:
	nop
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(60,r2),r3
	mov.w	.L443,r2
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r7
	add	#-24,r7
	mov	r14,r1
	add	#-24,r1
	mov.l	@(48,r1),r3
	mov.l	@(52,r1),r4
	mov	#0,r1
	mov	#1,r2
	clrt
	addc	r4,r2
	addc	r3,r1
	mov.l	r1,@(48,r7)
	mov.l	r2,@(52,r7)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(48,r2),r2
	add	#-1,r2
	mov.l	r2,@(48,r1)
.L428:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r1
	cmp/pl	r1
	bt	.L429
	mov	#12,r4
	mov	#0,r5
	mov.l	.L440,r1
	bsrf	r1
.LPCS55:
	nop
	bra	.L430
	nop
	.align 1
.L426:
	mov	r14,r3
	add	#-28,r3
	mov	r14,r7
	add	#-28,r7
	mov.l	@(40,r7),r7
	mov.l	r7,@(60,r3)
	mov	r14,r7
	add	#-24,r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r4
	mov.l	@(32,r3),r3
	mov.l	r3,@(48,r7)
	mov.l	r4,@(52,r7)
	mov	r14,r3
	add	#-28,r3
	mov	r14,r7
	add	#-28,r7
	mov.l	@(28,r7),r7
	mov.l	r7,@(48,r3)
	mov	r14,r3
	add	#-24,r3
	mov.l	@(48,r3),r3
	mov	r3,r2
	mov	#0,r1
	mov	r14,r3
	add	#-28,r3
	mov.l	r2,@(44,r3)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	tst	r1,r1
	bt	.L431
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	mov	#55,r4
	mov	r1,r5
	mov.l	.L441,r1
	bsrf	r1
.LPCS56:
	nop
.L431:
	mov	r14,r1
	add	#-24,r1
	mov.l	@(52,r1),r1
	mov	#17,r4
	mov	r1,r5
	mov.l	.L442,r1
	bsrf	r1
.LPCS57:
	nop
	mov	r14,r1
	add	#-28,r1
	mov.w	.L443,r2
	mov.l	@(60,r1),r4
	mov	r2,r5
	mov.l	.L444,r1
	bsrf	r1
.LPCS58:
	nop
.L430:
	mov	#0,r1
	mov	r1,r0
	add	#36,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L443:
	.short	512
.L445:
	.align 2
.L434:
	.long	_GLOBAL_OFFSET_TABLE_
.L435:
	.long	_TKSPI_SendCmd@PLT-(.LPCS52+2-.)
.L436:
	.long	_TKSPI_SendCmd@PLT-(.LPCS53+2-.)
.L438:
	.long	_TKSPI_ReadData@PLT-(.LPCS54+2-.)
.L440:
	.long	_TKSPI_SendCmd@PLT-(.LPCS55+2-.)
.L441:
	.long	_TKSPI_SendCmd@PLT-(.LPCS56+2-.)
.L442:
	.long	_TKSPI_SendCmd@PLT-(.LPCS57+2-.)
.L444:
	.long	_TKSPI_ReadData@PLT-(.LPCS58+2-.)
	.size	_TKSPI_ReadSectors, .-_TKSPI_ReadSectors
	.align 1
	.global	_TKSPI_WriteSectors
	.type	_TKSPI_WriteSectors, @function
_TKSPI_WriteSectors:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L454,r0
	mov.l	.L454,r12
	add	r0,r12
	add	#-36,r15
	mov	r15,r14
	mov	r14,r1
	add	#-28,r1
	mov.l	r4,@(40,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	r5,@(32,r1)
	mov.l	r6,@(36,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	r7,@(28,r1)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r3
	add	#-24,r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov.l	@(32,r1),r1
	mov.l	r1,@(48,r3)
	mov.l	r2,@(52,r3)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(28,r2),r2
	mov.l	r2,@(48,r1)
	bra	.L449
	nop
	.align 1
.L451:
	mov	r14,r1
	add	#-24,r1
	mov.l	@(48,r1),r1
	mov	r1,r9
	mov	#0,r8
	mov	r14,r1
	add	#-28,r1
	mov.l	r9,@(44,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	tst	r1,r1
	bt	.L450
	mov	r14,r1
	add	#-28,r1
	mov.l	@(44,r1),r1
	mov	#55,r4
	mov	r1,r5
	mov.l	.L455,r1
	bsrf	r1
.LPCS59:
	nop
.L450:
	mov	r14,r1
	add	#-24,r1
	mov.l	@(52,r1),r1
	mov	#24,r4
	mov	r1,r5
	mov.l	.L456,r1
	bsrf	r1
.LPCS60:
	nop
	mov	r14,r1
	add	#-28,r1
	mov.w	.L459,r2
	mov.l	@(60,r1),r4
	mov	r2,r5
	mov.l	.L458,r1
	bsrf	r1
.LPCS61:
	nop
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(60,r2),r3
	mov.w	.L459,r2
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r7
	add	#-24,r7
	mov	r14,r1
	add	#-24,r1
	mov.l	@(48,r1),r3
	mov.l	@(52,r1),r4
	mov	#0,r1
	mov	#1,r2
	clrt
	addc	r4,r2
	addc	r3,r1
	mov.l	r1,@(48,r7)
	mov.l	r2,@(52,r7)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(48,r2),r2
	add	#-1,r2
	mov.l	r2,@(48,r1)
.L449:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r1
	cmp/pl	r1
	bt	.L451
	mov	#0,r1
	mov	r1,r0
	add	#36,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L459:
	.short	512
.L460:
	.align 2
.L454:
	.long	_GLOBAL_OFFSET_TABLE_
.L455:
	.long	_TKSPI_SendCmd@PLT-(.LPCS59+2-.)
.L456:
	.long	_TKSPI_SendCmd@PLT-(.LPCS60+2-.)
.L458:
	.long	_TKSPI_WriteData@PLT-(.LPCS61+2-.)
	.size	_TKSPI_WriteSectors, .-_TKSPI_WriteSectors
	.section	.rodata
	.align 2
.LC3:
	.string	"TKSPI_InitDevice: Bad CMD0 %02X\n"
	.align 2
.LC4:
	.string	"TKSPI_InitDevice: Init OK, %d\n"
	.align 2
.LC5:
	.string	"TKSPI_InitDevice: Init Failed\n"
	.text
	.align 1
	.global	_TKSPI_InitDevice
	.type	_TKSPI_InitDevice, @function
_TKSPI_InitDevice:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L490,r0
	mov.l	.L490,r12
	add	r0,r12
	add	#-16,r15
	mov	r15,r14
	mov	#0,r4
	mov.l	.L491,r0
	bsrf	r0
.LPCS62:
	nop
	mov	#52,r4
	mov	#1,r5
	mov.l	.L492,r1
	bsrf	r1
.LPCS63:
	nop
	mov	r14,r1
	add	#-48,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	bra	.L462
	nop
	.align 1
.L463:
	mov.w	.L498,r1
	mov	r1,r4
	mov.l	.L494,r2
	bsrf	r2
.LPCS64:
	nop
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L462:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	#9,r1
	cmp/hi	r1,r2
	bf	.L463
	mov	r14,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov	#0,r4
	mov	#0,r5
	mov.l	.L495,r1
	bsrf	r1
.LPCS65:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	#1,r1
	cmp/eq	r1,r2
	bt	.L536
	bra	.L464
	nop
.L536:
	mov	r14,r8
	mov.w	.L496,r1
	mov	#8,r4
	mov	r1,r5
	mov.l	.L497,r2
	bsrf	r2
.LPCS66:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#1,r1
	cmp/eq	r1,r2
	bt	.L535
	bra	.L465
	nop
.L535:
	mov	r14,r1
	add	#-48,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	bra	.L466
	nop
	.align 1
.L467:
	mov.w	.L498,r1
	mov	r1,r4
	mov.l	.L499,r1
	bsrf	r1
.LPCS67:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	add	r3,r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L466:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	#3,r1
	cmp/hi	r1,r2
	bf	.L467
	mov	r14,r1
	mov.b	@(2,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#1,r1
	cmp/eq	r1,r2
	bt	.LF100
	bra	.L533
	nop
.LF100:
	mov	r14,r1
	mov.b	@(3,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L500,r1
	cmp/eq	r1,r2
	bt	.LF101
	bra	.L533
	nop
.LF101:
	mov	r14,r1
	add	#-48,r1
	mov.l	.L501,r2
	mov.l	r2,@(56,r1)
	bra	.L469
	nop
	.align 1
.L472:
	mov	r14,r8
	mov.w	.L502,r2
	mov.l	.L503,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L504,r1
	bsrf	r1
.LPCS68:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L470
	bra	.L471
	nop
	.align 1
.L498:
	.short	255
.L496:
	.short	426
.L500:
	.short	170
.L502:
	.short	169
.L505:
	.align 2
.L490:
	.long	_GLOBAL_OFFSET_TABLE_
.L491:
	.long	_TKSPI_SetSpeed@PLT-(.LPCS62+2-.)
.L492:
	.long	_TKSPI_SendCmd@PLT-(.LPCS63+2-.)
.L494:
	.long	_TKSPI_XchByte@PLT-(.LPCS64+2-.)
.L495:
	.long	_TKSPI_SendCmd@PLT-(.LPCS65+2-.)
.L497:
	.long	_TKSPI_SendCmd@PLT-(.LPCS66+2-.)
.L499:
	.long	_TKSPI_XchByte@PLT-(.LPCS67+2-.)
.L501:
	.long	131072
.L503:
	.long	1073741824
.L504:
	.long	_TKSPI_SendCmd@PLT-(.LPCS68+2-.)
	.align 1
.L470:
	mov	#10,r4
	mov.l	.L506,r1
	bsrf	r1
.LPCS69:
	nop
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#-1,r2
	mov.l	r2,@(56,r1)
.L469:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bf	.L472
.L471:
	mov	r14,r8
	mov	#58,r4
	mov	#0,r5
	mov.l	.L507,r2
	bsrf	r2
.LPCS70:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bt	.L533
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L534
.L533:
	bra	.L485
	nop
.L534:
	mov	r14,r1
	add	#-48,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	bra	.L473
	nop
	.align 1
.L474:
	mov.w	.L508,r1
	mov	r1,r4
	mov.l	.L509,r1
	bsrf	r1
.LPCS71:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	add	r3,r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L473:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	#3,r1
	cmp/hi	r1,r2
	bf	.L474
	mov	r14,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r2
	and	r2,r1
	tst	r1,r1
	bt	.L475
	mov	#67,r1
	bra	.L476
	nop
	.align 1
.L475:
	mov	#3,r1
.L476:
	mov	r14,r2
	mov	r1,r0
	mov.b	r0,@(14,r2)
	bra	.L485
	nop
	.align 1
.L465:
	mov	r14,r8
	mov.w	.L510,r1
	mov	r1,r4
	mov	#0,r5
	mov.l	.L511,r1
	bsrf	r1
.LPCS72:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#1,r1
	cmp/hi	r1,r2
	bt	.L478
	mov	r14,r1
	mov	#2,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov	r14,r1
	mov	#-87,r2
	mov	r2,r0
	mov.b	r0,@(15,r1)
	bra	.L479
	nop
	.align 1
.L478:
	mov	r14,r1
	mov	#1,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov	r14,r1
	mov	#1,r2
	mov	r2,r0
	mov.b	r0,@(15,r1)
.L479:
	mov	r14,r1
	add	#-48,r1
	mov.l	.L512,r2
	mov.l	r2,@(56,r1)
	bra	.L480
	nop
	.align 1
.L483:
	mov	r14,r1
	mov.b	@(15,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r14,r8
	mov	r1,r4
	mov	#0,r5
	mov.l	.L513,r1
	bsrf	r1
.LPCS73:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L481
	bra	.L482
	nop
	.align 1
.L508:
	.short	255
.L510:
	.short	169
.L514:
	.align 2
.L506:
	.long	_TKSPI_DelayUSec@PLT-(.LPCS69+2-.)
.L507:
	.long	_TKSPI_SendCmd@PLT-(.LPCS70+2-.)
.L509:
	.long	_TKSPI_XchByte@PLT-(.LPCS71+2-.)
.L511:
	.long	_TKSPI_SendCmd@PLT-(.LPCS72+2-.)
.L512:
	.long	131072
.L513:
	.long	_TKSPI_SendCmd@PLT-(.LPCS73+2-.)
	.align 1
.L481:
	mov	#10,r4
	mov.l	.L515,r1
	bsrf	r1
.LPCS74:
	nop
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#-1,r2
	mov.l	r2,@(56,r1)
.L480:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bf	.L483
.L482:
	mov	r14,r8
	mov.w	.L516,r1
	mov	#16,r4
	mov	r1,r5
	mov.l	.L517,r2
	bsrf	r2
.LPCS75:
	nop
	mov	r0,r1
	mov	r1,r0
	mov.b	r0,@(7,r8)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bt	.L484
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L485
.L484:
	mov	r14,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	bra	.L485
	nop
	.align 1
.L464:
	mov	r14,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov.l	.L518,r1
	add	r12,r1
	mov	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov	r2,r4
	mov.l	@(56,r1),r5
	mov.l	.L519,r1
	bsrf	r1
.LPCS76:
	nop
.L485:
	mov.l	.L520,r2
	bsrf	r2
.LPCS77:
	nop
	mov	r14,r1
	mov.b	@(14,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L486
	mov	#1,r4
	mov.l	.L521,r1
	bsrf	r1
.LPCS78:
	nop
	mov.l	.L529,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	r14,r2
	mov.b	@(14,r2),r0
	mov	r0,r2
	mov.b	r2,@r1
	mov	r14,r1
	mov.b	@(14,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.l	.L523,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L524,r1
	bsrf	r1
.LPCS79:
	nop
	bra	.L487
	nop
	.align 1
.L486:
	mov.l	.L525,r2
	bsrf	r2
.LPCS80:
	nop
	mov.l	.L529,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	#0,r2
	mov.b	r2,@r1
	mov.l	.L527,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L528,r0
	bsrf	r0
.LPCS81:
	nop
.L487:
	mov.l	.L529,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L516:
	.short	512
.L530:
	.align 2
.L515:
	.long	_TKSPI_DelayUSec@PLT-(.LPCS74+2-.)
.L517:
	.long	_TKSPI_SendCmd@PLT-(.LPCS75+2-.)
.L518:
	.long	.LC3@GOTOFF
.L519:
	.long	_printf@PLT-(.LPCS76+2-.)
.L520:
	.long	_TKSPI_Deselect@PLT-(.LPCS77+2-.)
.L521:
	.long	_TKSPI_SetSpeed@PLT-(.LPCS78+2-.)
.L529:
	.long	_tkspi_init_ok@GOT
.L523:
	.long	.LC4@GOTOFF
.L524:
	.long	_printf@PLT-(.LPCS79+2-.)
.L525:
	.long	_TKSPI_PowerOff@PLT-(.LPCS80+2-.)
.L527:
	.long	.LC5@GOTOFF
.L528:
	.long	_printf@PLT-(.LPCS81+2-.)
	.size	_TKSPI_InitDevice, .-_TKSPI_InitDevice
	.align 1
	.global	_tkfat_setChs
	.type	_tkfat_setChs, @function
_tkfat_setChs:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L539,r0
	mov.l	.L539,r12
	add	r0,r12
	add	#-24,r15
	mov	r15,r14
	mov	r14,r1
	add	#-40,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r5,@(40,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(40,r2),r2
	mov	#63,r3
	and	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(40,r1),r1
	mov	r1,r4
	mov.l	.L540,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r2
	mov	r14,r1
	add	#-40,r1
	extu.b	r2,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(40,r2),r2
	mov	r2,r4
	mov.l	.L541,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r1
	mov	r1,r3
	shll2	r3
	shll2	r3
	shll2	r3
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(60,r2),r2
	or	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(44,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov.l	@(44,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov.l	@(44,r1),r1
	add	#2,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	mov	r2,r4
	mov.l	.L542,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	add	#24,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L543:
	.align 2
.L539:
	.long	_GLOBAL_OFFSET_TABLE_
.L540:
	.long	___ashiftrt_r4_6@GOTOFF
.L541:
	.long	___ashiftrt_r4_10@GOTOFF
.L542:
	.long	___ashiftrt_r4_8@GOTOFF
	.size	_tkfat_setChs, .-_tkfat_setChs
	.align 1
	.global	_tkfat_setWord
	.type	_tkfat_setWord, @function
_tkfat_setWord:
	mov.l	r14,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r5,r2
	mov	r14,r1
	add	#-24,r1
	mov	r2,r0
	mov.w	r0,@(24,r1)
	mov	r14,r1
	add	#-24,r1
	mov.w	@(24,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-24,r2
	mov.w	@(24,r2),r0
	mov	r0,r2
	extu.w	r2,r2
	shlr8	r2
	extu.w	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_tkfat_setWord, .-_tkfat_setWord
	.align 1
	.global	_tkfat_setDWord
	.type	_tkfat_setDWord, @function
_tkfat_setDWord:
	mov.l	r14,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	r5,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	shlr8	r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	add	#2,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	shlr16	r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	add	#3,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	shlr16	r2
	shlr8	r2
	extu.b	r2,r2
	mov.b	r2,@r1
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_tkfat_setDWord, .-_tkfat_setDWord
	.align 1
	.global	_tkfat_getWord
	.type	_tkfat_getWord, @function
_tkfat_getWord:
	mov.l	r14,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	exts.w	r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	shll8	r1
	exts.w	r1,r1
	or	r2,r1
	exts.w	r1,r1
	extu.w	r1,r1
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_tkfat_getWord, .-_tkfat_getWord
	.align 1
	.global	_tkfat_getDWord
	.type	_tkfat_getDWord, @function
_tkfat_getDWord:
	mov.l	r14,@-r15
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	shll8	r1
	or	r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	add	#2,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	shll16	r1
	or	r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	add	#3,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	shll16	r1
	shll8	r1
	or	r2,r1
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_tkfat_getDWord, .-_tkfat_getDWord
	.align 1
	.global	_TKFAT_GetSectorTempBuffer
	.type	_TKFAT_GetSectorTempBuffer, @function
_TKFAT_GetSectorTempBuffer:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L561,r0
	mov.l	.L561,r12
	add	r0,r12
	add	#-36,r15
	mov	r15,r14
	mov	r14,r3
	add	#-28,r3
	mov.l	r4,@(36,r3)
	mov	r14,r3
	add	#-28,r3
	mov.l	r5,@(32,r3)
	mov	r14,r3
	add	#-28,r3
	mov.l	r6,@(28,r3)
	mov	r14,r3
	add	#-28,r3
	mov	r14,r7
	add	#-28,r7
	mov.l	@(28,r7),r7
	extu.b	r7,r7
	mov.l	r7,@(52,r3)
	mov	r14,r3
	add	#-28,r3
	mov	#0,r7
	mov.l	r7,@(56,r3)
	bra	.L551
	nop
.L562:
	.align 2
.L561:
	.long	_GLOBAL_OFFSET_TABLE_
	.align 1
.L555:
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(56,r3),r6
	mov.w	.L580,r3
	add	r6,r3
	shll2	r3
	add	r7,r3
	mov.l	@r3,r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(32,r3),r3
	cmp/eq	r3,r7
	bf	.L609
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(56,r3),r6
	mov.w	.L581,r3
	add	r6,r3
	add	r3,r3
	add	r7,r3
	mov.w	@r3,r3
	mov	r3,r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(52,r3),r3
	cmp/eq	r3,r7
	bt	.L612
.L609:
	bra	.L552
	nop
.L612:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(28,r1),r2
	mov.w	.L568,r1
	and	r2,r1
	tst	r1,r1
	bt	.L553
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r3
	mov.w	.L581,r1
	add	r3,r1
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r2
	mov.w	.L568,r1
	or	r2,r1
	exts.w	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L581,r1
	add	r7,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
.L553:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r2
	mov	r2,r1
	shll2	r1
	add	r1,r1
	sub	r2,r1
	mov	r14,r2
	add	#-28,r2
	shar	r1
	shar	r1
	shar	r1
	mov.l	r1,@(48,r2)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r3
	mov	r14,r2
	add	#-28,r2
	mov.l	@(56,r2),r7
	mov.w	.L579,r2
	add	r7,r2
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r3
	mov	r14,r2
	add	#-28,r2
	mov.l	@(56,r2),r7
	mov.w	.L580,r2
	add	r7,r2
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@(44,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r3
	mov.w	.L581,r1
	add	r3,r1
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	r2,@(40,r1)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r3
	mov.w	.L579,r1
	add	r3,r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L579,r1
	add	r7,r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r3
	mov.w	.L580,r1
	add	r3,r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L580,r1
	add	r7,r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(48,r1),r3
	mov.w	.L581,r1
	add	r3,r1
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L581,r1
	add	r7,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r3
	mov.w	.L579,r1
	add	r3,r1
	shll2	r1
	add	r2,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r3
	mov.w	.L580,r1
	add	r3,r1
	shll2	r1
	add	r2,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(40,r1),r1
	exts.w	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L581,r1
	bra	.L563
	nop
	.align 1
.L580:
	.short	175
.L581:
	.short	862
.L568:
	.short	512
.L579:
	.short	559
	.align 1
.L563:
	add	r7,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
	bra	.L554
	nop
	.align 1
.L552:
	mov	r14,r3
	add	#-28,r3
	mov	r14,r7
	add	#-28,r7
	mov.l	@(56,r7),r7
	add	#1,r7
	mov.l	r7,@(56,r3)
.L551:
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov.w	.L586,r3
	add	r7,r3
	mov.l	@(0,r3),r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(56,r3),r3
	cmp/gt	r3,r7
	bf	.L611
	bra	.L555
	nop
.L611:
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov.w	.L586,r3
	add	r7,r3
	mov.l	@(0,r3),r7
	mov.w	.L584,r3
	cmp/gt	r3,r7
	bt	.L556
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov.w	.L586,r1
	add	r2,r1
	mov.l	@(0,r1),r1
	mov	r1,r3
	add	#1,r3
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r7
	mov.w	.L586,r2
	add	r7,r2
	mov.l	r3,@(0,r2)
	mov	r14,r2
	add	#-28,r2
	mov.l	r1,@(56,r2)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r1
	shll8	r1
	add	r1,r1
	mov	r14,r10
	add	#-28,r10
	mov	r1,r4
	mov.l	.L587,r1
	bsrf	r1
.LPCS82:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r10)
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r3
	mov.w	.L588,r1
	add	r3,r1
	shll2	r1
	add	r2,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(32,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L589,r1
	add	r7,r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r1
	exts.w	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L590,r1
	add	r7,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	bra	.L557
	nop
	.align 1
.L586:
	.short	3260
.L584:
	.short	255
.L588:
	.short	559
.L589:
	.short	175
.L590:
	.short	862
.L591:
	.align 2
.L587:
	.long	_malloc@PLT-(.LPCS82+2-.)
	.align 1
.L556:
	mov	r14,r3
	add	#-28,r3
	mov.w	.L592,r7
	mov.l	r7,@(56,r3)
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(56,r3),r6
	mov.w	.L605,r3
	add	r6,r3
	add	r3,r3
	add	r7,r3
	mov.w	@r3,r3
	mov	r3,r7
	mov.w	.L594,r3
	and	r7,r3
	tst	r3,r3
	bt	.L558
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(56,r3),r6
	mov.w	.L606,r3
	add	r6,r3
	shll2	r3
	add	r7,r3
	mov.l	@r3,r5
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(56,r3),r6
	mov.w	.L604,r3
	add	r6,r3
	shll2	r3
	add	r7,r3
	mov.l	@r3,r3
	mov	r3,r2
	mov	#0,r1
	mov	r14,r3
	add	#-28,r3
	mov.l	@(36,r3),r7
	mov	r14,r3
	add	#-28,r3
	mov.l	@(56,r3),r6
	mov.w	.L605,r3
	add	r6,r3
	add	r3,r3
	add	r7,r3
	mov.w	@r3,r3
	extu.b	r3,r3
	mov	r5,r4
	mov	r1,r5
	mov	r2,r6
	mov	r3,r7
	mov.l	.L598,r1
	bsrf	r1
.LPCS83:
	nop
.L558:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r3
	mov.w	.L605,r1
	add	r3,r1
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r1
	cmp/eq	r1,r2
	bt	.L559
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r3
	mov.w	.L606,r1
	add	r3,r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r1
	mov	r1,r4
	mov.l	.L601,r1
	bsrf	r1
.LPCS84:
	nop
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r1
	shll8	r1
	add	r1,r1
	mov	r1,r4
	mov.l	.L602,r1
	bsrf	r1
.LPCS85:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L606,r1
	add	r7,r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
.L559:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(32,r1),r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L604,r1
	add	r7,r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov.l	@(52,r1),r1
	exts.w	r1,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-28,r1
	mov.l	@(56,r1),r7
	mov.w	.L605,r1
	add	r7,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov	r14,r1
	add	#-28,r1
	mov	r14,r2
	add	#-28,r2
	mov.l	@(36,r2),r3
	mov	r14,r2
	add	#-28,r2
	mov.l	@(56,r2),r7
	mov.w	.L606,r2
	add	r7,r2
	shll2	r2
	add	r3,r2
	mov.l	@r2,r2
	mov.l	r2,@(60,r1)
.L557:
	mov	r14,r1
	add	#-28,r1
	mov.l	@(32,r1),r1
	mov	r1,r9
	mov	#0,r2
	cmp/gt	r1,r2
	subc	r1,r1
	mov	r1,r8
	mov	r14,r2
	add	#-28,r2
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r2),r4
	mov	r8,r5
	mov	r9,r6
	mov.l	@(52,r1),r7
	mov.l	.L607,r1
	bsrf	r1
.LPCS86:
	nop
	mov	r14,r1
	add	#-28,r1
	mov.l	@(60,r1),r1
.L554:
	mov	r1,r0
	add	#36,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L592:
	.short	255
.L605:
	.short	862
.L594:
	.short	512
.L606:
	.short	559
.L604:
	.short	175
.L608:
	.align 2
.L598:
	.long	_TKSPI_WriteSectors@PLT-(.LPCS83+2-.)
.L601:
	.long	_free@PLT-(.LPCS84+2-.)
.L602:
	.long	_malloc@PLT-(.LPCS85+2-.)
.L607:
	.long	_TKSPI_ReadSectors@PLT-(.LPCS86+2-.)
	.size	_TKFAT_GetSectorTempBuffer, .-_TKFAT_GetSectorTempBuffer
	.align 1
	.global	_TKFAT_GetSectorStaticBuffer
	.type	_TKFAT_GetSectorStaticBuffer, @function
_TKFAT_GetSectorStaticBuffer:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L619,r0
	mov.l	.L619,r12
	add	r0,r12
	add	#-16,r15
	mov	r15,r14
	mov	r14,r1
	add	#-48,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	r5,@(52,r1)
	mov	r14,r1
	add	#-48,r1
	mov.l	r6,@(48,r1)
	mov	r14,r1
	add	#-48,r1
	mov	#0,r2
	mov.l	r2,@(60,r1)
	bra	.L614
	nop
	.align 1
.L617:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	add	#14,r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	cmp/eq	r1,r2
	bf	.L615
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	add	#110,r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r1
	bra	.L616
	nop
	.align 1
.L615:
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L614:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov.w	.L622,r1
	add	r2,r1
	mov.l	@(60,r1),r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	cmp/gt	r1,r2
	bt	.L617
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov.w	.L622,r1
	add	r2,r1
	mov.l	@(60,r1),r1
	mov	r1,r3
	add	#1,r3
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r7
	mov.w	.L622,r2
	add	r7,r2
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-48,r2
	mov.l	r1,@(60,r2)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r1
	extu.b	r1,r1
	shll8	r1
	add	r1,r1
	mov	r1,r4
	mov.l	.L623,r1
	bsrf	r1
.LPCS87:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	add	#110,r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	add	#14,r1
	shll2	r1
	add	r3,r1
	mov.l	r2,@r1
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	add	#110,r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	mov	r1,r9
	mov	#0,r2
	cmp/gt	r1,r2
	subc	r1,r1
	mov	r1,r8
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r1),r1
	extu.b	r1,r1
	mov	r3,r4
	mov	r8,r5
	mov	r9,r6
	mov	r1,r7
	mov.l	.L624,r1
	bsrf	r1
.LPCS88:
	nop
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	add	#110,r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r1
.L616:
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L622:
	.short	636
.L625:
	.align 2
.L619:
	.long	_GLOBAL_OFFSET_TABLE_
.L623:
	.long	_malloc@PLT-(.LPCS87+2-.)
.L624:
	.long	_TKSPI_ReadSectors@PLT-(.LPCS88+2-.)
	.size	_TKFAT_GetSectorStaticBuffer, .-_TKFAT_GetSectorStaticBuffer
	.align 1
	.global	_TKFAT_SetupImageMBR
	.type	_TKFAT_SetupImageMBR, @function
_TKFAT_SetupImageMBR:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L631,r0
	mov.l	.L631,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L627
	mov	r14,r1
	add	#-56,r1
	mov	#6,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(4,r1),r2
	mov.l	.L632,r1
	cmp/gt	r1,r2
	bf	.L629
	mov	r14,r1
	add	#-56,r1
	mov	#12,r2
	mov.l	r2,@(60,r1)
	bra	.L629
	nop
	.align 1
.L627:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	r2,@(60,r1)
.L629:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r0
	cmp/eq	#6,r0
	movt	r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov	#8,r2
	mov.l	r2,@(12,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(4,r1),r1
	mov	r1,r2
	add	#-8,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(16,r1)
	mov	r14,r1
	add	#-56,r1
	mov.w	.L633,r2
	mov.l	@(56,r1),r4
	mov	#0,r5
	mov	r2,r6
	mov.l	.L634,r1
	bsrf	r1
.LPCS89:
	nop
	mov	r0,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	mov.l	r1,@(44,r2)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L635,r1
	mov	r2,r4
	mov	#0,r5
	mov	r1,r6
	mov.l	.L636,r1
	bsrf	r1
.LPCS90:
	nop
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L637,r1
	add	r2,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r3
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	extu.b	r1,r2
	mov.w	.L638,r1
	add	r3,r1
	mov	r2,r0
	mov.b	r0,@(2,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L639,r1
	add	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L640,r1
	bsrf	r1
.LPCS91:
	nop
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L641,r1
	mov	r2,r3
	add	r1,r3
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(16,r1),r1
	add	r2,r1
	mov	r3,r4
	mov	r1,r5
	mov.l	.L642,r0
	bsrf	r0
.LPCS92:
	nop
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L643,r1
	add	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L644,r1
	bsrf	r1
.LPCS93:
	nop
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L645,r1
	add	r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(16,r1),r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L646,r0
	bsrf	r0
.LPCS94:
	nop
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L648,r1
	add	r2,r1
	mov	#85,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L648,r1
	add	r2,r1
	mov	#-86,r2
	mov	r2,r0
	mov.b	r0,@(15,r1)
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
	.align 1
.L633:
	.short	513
.L635:
	.short	512
.L637:
	.short	432
.L638:
	.short	448
.L639:
	.short	447
.L641:
	.short	451
.L643:
	.short	454
.L645:
	.short	458
.L648:
	.short	496
.L649:
	.align 2
.L631:
	.long	_GLOBAL_OFFSET_TABLE_
.L632:
	.long	130048
.L634:
	.long	_TKFAT_GetSectorStaticBuffer@PLT-(.LPCS89+2-.)
.L636:
	.long	_memset@PLT-(.LPCS90+2-.)
.L640:
	.long	_tkfat_setChs@PLT-(.LPCS91+2-.)
.L642:
	.long	_tkfat_setChs@PLT-(.LPCS92+2-.)
.L644:
	.long	_tkfat_setDWord@PLT-(.LPCS93+2-.)
.L646:
	.long	_tkfat_setDWord@PLT-(.LPCS94+2-.)
	.size	_TKFAT_SetupImageMBR, .-_TKFAT_SetupImageMBR
	.section	.rodata
	.align 2
.LC6:
	.string	"(none)"
	.align 2
.LC7:
	.string	"FAT16"
	.align 2
.LC8:
	.string	"FAT32"
	.align 2
.LC9:
	.string	"?"
	.text
	.align 1
	.global	_tkfat_fstnameforfsty
	.type	_tkfat_fstnameforfsty, @function
_tkfat_fstnameforfsty:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L661,r0
	mov.l	.L661,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov	#0,r3
	mov	#12,r2
	sub	r3,r1
	cmp/hi	r2,r1
	bt	.L651
	mova	.L653,r0
	shll2	r1
	mov.l	@(r0,r1),r1
	braf	r1
	nop
.L654:
.L662:
	.align 2
.L661:
	.long	_GLOBAL_OFFSET_TABLE_
	.align 2
.L653:
	.long	.L652-.L654
	.long	.L651-.L654
	.long	.L651-.L654
	.long	.L651-.L654
	.long	.L651-.L654
	.long	.L651-.L654
	.long	.L655-.L654
	.long	.L651-.L654
	.long	.L651-.L654
	.long	.L651-.L654
	.long	.L651-.L654
	.long	.L656-.L654
	.long	.L657-.L654
	.align 1
.L652:
	mov	r14,r1
	add	#-56,r1
	mov.l	.L663,r2
	add	r12,r2
	mov.l	r2,@(60,r1)
	bra	.L658
	nop
	.align 1
.L655:
	mov	r14,r1
	add	#-56,r1
	mov.l	.L664,r2
	add	r12,r2
	mov.l	r2,@(60,r1)
	bra	.L658
	nop
	.align 1
.L656:
	mov	r14,r1
	add	#-56,r1
	mov.l	.L666,r2
	add	r12,r2
	mov.l	r2,@(60,r1)
	bra	.L658
	nop
	.align 1
.L657:
	mov	r14,r1
	add	#-56,r1
	mov.l	.L666,r2
	add	r12,r2
	mov.l	r2,@(60,r1)
	bra	.L658
	nop
	.align 1
.L651:
	mov	r14,r1
	add	#-56,r1
	mov.l	.L667,r2
	add	r12,r2
	mov.l	r2,@(60,r1)
	nop
.L658:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L668:
	.align 2
.L663:
	.long	.LC6@GOTOFF
.L664:
	.long	.LC7@GOTOFF
.L666:
	.long	.LC8@GOTOFF
.L667:
	.long	.LC9@GOTOFF
	.size	_tkfat_fstnameforfsty, .-_tkfat_fstnameforfsty
	.section	.rodata
	.align 2
.LC10:
	.string	"TKFAT_ReadImageMBR\n"
	.align 2
.LC11:
	.string	"  %08X %08X %02X %s\n"
	.align 2
.LC12:
	.string	"TKFAT_ReadImageMBR: %08X %08X %02X %s\n"
	.text
	.align 1
	.global	_TKFAT_ReadImageMBR
	.type	_TKFAT_ReadImageMBR, @function
_TKFAT_ReadImageMBR:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L673,r0
	mov.l	.L673,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(40,r1)
	mov.l	.L674,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L675,r0
	bsrf	r0
.LPCS95:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r4
	mov	#0,r5
	mov	#1,r6
	mov.l	.L676,r1
	bsrf	r1
.LPCS96:
	nop
	mov	r0,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(40,r2),r2
	mov.l	r1,@(44,r2)
	mov	r14,r1
	add	#-36,r1
	mov	#0,r2
	mov.l	r2,@(60,r1)
	bra	.L670
	nop
	.align 1
.L671:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(44,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	add	#28,r1
	shll2	r1
	shll2	r1
	add	r2,r1
	add	#2,r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(44,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r1,r3
	shll2	r3
	shll2	r3
	mov.w	.L677,r1
	add	r3,r1
	add	r2,r1
	add	#2,r1
	mov	r1,r4
	mov.l	.L678,r1
	bsrf	r1
.LPCS97:
	nop
	mov	r0,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	r1,@(52,r2)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(44,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r1,r3
	shll2	r3
	shll2	r3
	mov.w	.L679,r1
	add	r3,r1
	add	r2,r1
	add	#2,r1
	mov	r1,r4
	mov.l	.L680,r1
	bsrf	r1
.LPCS98:
	nop
	mov	r0,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	r1,@(48,r2)
	mov	r14,r8
	add	#-36,r8
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r4
	mov.l	.L681,r1
	bsrf	r1
.LPCS99:
	nop
	mov	r0,r1
	mov.l	r1,@(44,r8)
	mov.l	.L682,r1
	add	r12,r1
	mov	r1,r6
	mov	r14,r3
	add	#-36,r3
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov	r14,r7
	add	#-36,r7
	mov.l	@(44,r7),r7
	mov.l	r7,@r15
	mov	r6,r4
	mov.l	@(52,r3),r5
	mov.l	@(48,r2),r6
	mov.l	@(56,r1),r7
	mov.l	.L683,r0
	bsrf	r0
.LPCS100:
	nop
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L670:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r2
	mov	#3,r1
	cmp/gt	r1,r2
	bf	.L671
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L684,r1
	add	r2,r1
	mov.b	@(2,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L685,r1
	add	r2,r1
	mov	r1,r4
	mov.l	.L686,r1
	bsrf	r1
.LPCS101:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	r2,@(12,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(44,r1),r2
	mov.w	.L687,r1
	add	r2,r1
	mov	r1,r4
	mov.l	.L688,r1
	bsrf	r1
.LPCS102:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	r2,@(16,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r0
	cmp/eq	#6,r0
	movt	r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r14,r8
	add	#-36,r8
	mov	r1,r4
	mov.l	.L689,r1
	bsrf	r1
.LPCS103:
	nop
	mov	r0,r1
	mov.l	r1,@(44,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(12,r1),r7
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.l	@(16,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.l	.L690,r1
	add	r12,r1
	mov	r1,r6
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov.l	r1,@r15
	mov	r6,r4
	mov	r7,r5
	mov	r3,r6
	mov	r2,r7
	mov.l	.L691,r1
	bsrf	r1
.LPCS104:
	nop
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L677:
	.short	452
.L679:
	.short	456
.L684:
	.short	448
.L685:
	.short	454
.L687:
	.short	458
.L692:
	.align 2
.L673:
	.long	_GLOBAL_OFFSET_TABLE_
.L674:
	.long	.LC10@GOTOFF
.L675:
	.long	_printf@PLT-(.LPCS95+2-.)
.L676:
	.long	_TKFAT_GetSectorStaticBuffer@PLT-(.LPCS96+2-.)
.L678:
	.long	_tkfat_getDWord@PLT-(.LPCS97+2-.)
.L680:
	.long	_tkfat_getDWord@PLT-(.LPCS98+2-.)
.L681:
	.long	_tkfat_fstnameforfsty@PLT-(.LPCS99+2-.)
.L682:
	.long	.LC11@GOTOFF
.L683:
	.long	_printf@PLT-(.LPCS100+2-.)
.L686:
	.long	_tkfat_getDWord@PLT-(.LPCS101+2-.)
.L688:
	.long	_tkfat_getDWord@PLT-(.LPCS102+2-.)
.L689:
	.long	_tkfat_fstnameforfsty@PLT-(.LPCS103+2-.)
.L690:
	.long	.LC12@GOTOFF
.L691:
	.long	_printf@PLT-(.LPCS104+2-.)
	.size	_TKFAT_ReadImageMBR, .-_TKFAT_ReadImageMBR
	.align 1
	.global	_TKFAT_GetFatEntry
	.type	_TKFAT_GetFatEntry, @function
_TKFAT_GetFatEntry:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L699,r0
	mov.l	.L699,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r5,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L694
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov.l	@(20,r1),r3
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r1
	mov	r1,r4
	mov.l	.L700,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r2
	mov	r14,r1
	add	#-44,r1
	add	r3,r2
	mov.l	r2,@(56,r1)
	mov	r14,r8
	add	#-44,r8
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r2),r4
	mov.l	@(56,r1),r5
	mov	#1,r6
	mov.l	.L701,r1
	bsrf	r1
.LPCS105:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r1
	extu.b	r1,r1
	add	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov	r14,r3
	add	#-44,r3
	mov.l	@(52,r3),r3
	add	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r3
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	shll8	r2
	mov	r14,r1
	add	#-44,r1
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r2
	mov.l	.L702,r1
	cmp/gt	r1,r2
	bf	.L695
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shll16	r1
	mov	r14,r2
	add	#-44,r2
	swap.w	r1,r1
	exts.w	r1,r1
	mov.l	r1,@(60,r2)
.L695:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	bra	.L696
	nop
	.align 1
.L694:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov.l	@(20,r1),r3
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r1
	mov	r1,r4
	mov.l	.L703,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r2
	mov	r14,r1
	add	#-44,r1
	add	r3,r2
	mov.l	r2,@(56,r1)
	mov	r14,r8
	add	#-44,r8
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r2),r4
	mov.l	@(56,r1),r5
	mov	#1,r6
	mov.l	.L704,r2
	bsrf	r2
.LPCS106:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(44,r1),r1
	mov	#127,r0
	and	r0,r1
	shll2	r1
	mov	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov	r14,r3
	add	#-44,r3
	mov.l	@(52,r3),r3
	add	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	shll8	r1
	add	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	add	#2,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	shll16	r1
	mov	r2,r3
	add	r1,r3
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	add	#3,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	shll16	r2
	shll8	r2
	mov	r14,r1
	add	#-44,r1
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r2
	mov.l	.L705,r1
	cmp/gt	r1,r2
	bf	.L697
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	shll2	r1
	shll2	r1
	mov	r14,r2
	add	#-44,r2
	shar	r1
	shar	r1
	shar	r1
	shar	r1
	mov.l	r1,@(60,r2)
.L697:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
.L696:
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L706:
	.align 2
.L699:
	.long	_GLOBAL_OFFSET_TABLE_
.L700:
	.long	___ashiftrt_r4_8@GOTOFF
.L701:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS105+2-.)
.L702:
	.long	65519
.L703:
	.long	___ashiftrt_r4_7@GOTOFF
.L704:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS106+2-.)
.L705:
	.long	268435439
	.size	_TKFAT_GetFatEntry, .-_TKFAT_GetFatEntry
	.align 1
	.global	_TKFAT_SetFatEntry
	.type	_TKFAT_SetFatEntry, @function
_TKFAT_SetFatEntry:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L711,r0
	mov.l	.L711,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r5,@(40,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r6,@(36,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L733
	bra	.L708
	nop
.L733:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov.l	@(20,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r4
	mov.l	.L719,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r2
	mov	r14,r1
	add	#-36,r1
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov.l	@(24,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r4
	mov.l	.L719,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r2
	mov	r14,r1
	add	#-36,r1
	add	r3,r2
	mov.l	r2,@(56,r1)
	mov	r14,r8
	add	#-36,r8
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.w	.L716,r3
	mov.l	@(44,r2),r4
	mov.l	@(60,r1),r5
	mov	r3,r6
	mov.l	.L715,r1
	bsrf	r1
.LPCS107:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	extu.b	r1,r1
	add	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov	r14,r3
	add	#-36,r3
	mov.l	@(52,r3),r3
	add	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r8
	add	#-36,r8
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.w	.L716,r3
	mov.l	@(44,r2),r4
	mov.l	@(56,r1),r5
	mov	r3,r6
	mov.l	.L717,r2
	bsrf	r2
.LPCS108:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	extu.b	r1,r1
	add	r1,r1
	mov	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov	r14,r3
	add	#-36,r3
	mov.l	@(48,r3),r3
	add	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov	r2,r4
	mov.l	.L719,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov	r2,r4
	mov.l	.L719,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	#0,r1
	bra	.L709
	nop
	.align 1
.L716:
	.short	513
.L720:
	.align 2
.L711:
	.long	_GLOBAL_OFFSET_TABLE_
.L719:
	.long	___ashiftrt_r4_8@GOTOFF
.L715:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS107+2-.)
.L717:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS108+2-.)
	.align 1
.L708:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov.l	@(20,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r4
	mov.l	.L722,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r2
	mov	r14,r1
	add	#-36,r1
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov.l	@(24,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r4
	mov.l	.L722,r1
	add	r12,r1
	jsr	@r1
	nop
	mov	r4,r2
	mov	r14,r1
	add	#-36,r1
	add	r3,r2
	mov.l	r2,@(56,r1)
	mov	r14,r8
	add	#-36,r8
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.w	.L725,r3
	mov.l	@(44,r2),r4
	mov.l	@(60,r1),r5
	mov	r3,r6
	mov.l	.L724,r1
	bsrf	r1
.LPCS109:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	#127,r2
	and	r2,r1
	shll2	r1
	mov	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov	r14,r3
	add	#-36,r3
	mov.l	@(52,r3),r3
	add	r3,r2
	mov.l	r2,@(52,r1)
	mov	r14,r8
	add	#-36,r8
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.w	.L725,r3
	mov.l	@(44,r2),r4
	mov.l	@(56,r1),r5
	mov	r3,r6
	mov.l	.L726,r1
	bsrf	r1
.LPCS110:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	#127,r2
	and	r2,r1
	shll2	r1
	mov	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov	r14,r3
	add	#-36,r3
	mov.l	@(48,r3),r3
	add	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov	r2,r4
	mov.l	.L729,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#2,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	swap.w	r1,r1
	exts.w	r1,r1
	extu.b	r1,r1
	mov.b	r1,@r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	add	#3,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov	r2,r4
	mov.l	.L730,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	add	#1,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov	r2,r4
	mov.l	.L729,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#2,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	swap.w	r1,r1
	exts.w	r1,r1
	extu.b	r1,r1
	mov.b	r1,@r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	add	#3,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov	r2,r4
	mov.l	.L730,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	#0,r1
.L709:
	mov	r1,r0
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L725:
	.short	513
.L731:
	.align 2
.L722:
	.long	___ashiftrt_r4_7@GOTOFF
.L724:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS109+2-.)
.L726:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS110+2-.)
.L729:
	.long	___ashiftrt_r4_8@GOTOFF
.L730:
	.long	___ashiftrt_r4_24@GOTOFF
	.size	_TKFAT_SetFatEntry, .-_TKFAT_SetFatEntry
	.align 1
	.global	_TKFAT_GetClusterLBA
	.type	_TKFAT_GetClusterLBA, @function
_TKFAT_GetClusterLBA:
	mov.l	r14,@-r15
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	r5,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.l	@(32,r1),r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov	r1,r3
	add	#-2,r3
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@(10,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mul.l	r1,r3
	sts	macl,r1
	add	r2,r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_TKFAT_GetClusterLBA, .-_TKFAT_GetClusterLBA
	.section	.rodata
	.align 2
.LC13:
	.string	"TKFAT_SetupImageFAT: Image LBA Size Bad\n"
	.align 2
.LC14:
	.string	"BTESH2  "
	.align 2
.LC15:
	.string	"DEFAULT    "
	.align 2
.LC16:
	.string	"FAT16   "
	.align 2
.LC17:
	.string	"TKFAT_SetupImageFAT: Created FAT16\n"
	.align 2
.LC18:
	.string	"  LBA FAT1=%08X (Offs=%08X)\n"
	.align 2
.LC19:
	.string	"  LBA FAT2=%08X (Offs=%08X)\n"
	.align 2
.LC20:
	.string	"  LBA Root=%08X (Offs=%08X)\n"
	.align 2
.LC21:
	.string	"  LBA Data=%08X (Offs=%08X)\n"
	.align 2
.LC22:
	.string	"  %d Sectors/Cluster, %d bytes\n"
	.align 2
.LC23:
	.string	"  %d total clusters\n"
	.align 2
.LC24:
	.string	"FAT32   "
	.align 2
.LC25:
	.string	"TKFAT_SetupImageFAT: Created FAT32\n"
	.align 2
.LC26:
	.string	"  FAT1 LBA=%08X Offs=%08X\n"
	.align 2
.LC27:
	.string	"  FAT2 LBA=%08X Offs=%08X\n"
	.align 2
.LC28:
	.string	"  Root LBA=%08X Offs=%08X\n"
	.align 2
.LC29:
	.string	"  Data LBA=%08X Offs=%08X\n"
	.text
	.align 1
	.global	_TKFAT_SetupImageFAT
	.type	_TKFAT_SetupImageFAT, @function
_TKFAT_SetupImageFAT:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L771,r0
	mov.l	.L771,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(36,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(16,r1),r2
	mov	#127,r1
	cmp/gt	r1,r2
	bt	.L737
	mov.l	.L772,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L773,r0
	bsrf	r0
.LPCS111:
	nop
	bra	.L736
	nop
	.align 1
.L737:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.w	.L774,r2
	mov.l	@(36,r1),r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L775,r1
	bsrf	r1
.LPCS112:
	nop
	mov	r0,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov.l	r1,@(48,r2)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	#-21,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	#127,r2
	mov	r2,r0
	mov.b	r0,@(1,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	#-112,r2
	mov	r2,r0
	mov.b	r0,@(2,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r2
	mov.w	.L777,r1
	add	r2,r1
	mov	#85,r2
	mov	r2,r0
	mov.b	r0,@(14,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r2
	mov.w	.L777,r1
	add	r2,r1
	mov	#-86,r2
	mov	r2,r0
	mov.b	r0,@(15,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L884
	bra	.L739
	nop
.L884:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#3,r2
	mov.l	.L778,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#9,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L779,r1
	bsrf	r1
.LPCS113:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#43,r2
	mov.l	.L780,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#12,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L781,r1
	bsrf	r1
.LPCS114:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#54,r2
	mov.l	.L782,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#9,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L783,r1
	bsrf	r1
.LPCS115:
	nop
	mov	r14,r1
	add	#-36,r1
	mov	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov.l	@(16,r2),r2
	mov.l	r2,@(60,r1)
	bra	.L740
	nop
	.align 1
.L774:
	.short	513
.L777:
	.short	496
.L784:
	.align 2
.L771:
	.long	_GLOBAL_OFFSET_TABLE_
.L772:
	.long	.LC13@GOTOFF
.L773:
	.long	_printf@PLT-(.LPCS111+2-.)
.L775:
	.long	_TKFAT_GetSectorStaticBuffer@PLT-(.LPCS112+2-.)
.L778:
	.long	.LC14@GOTOFF
.L779:
	.long	_memcpy@PLT-(.LPCS113+2-.)
.L780:
	.long	.LC15@GOTOFF
.L781:
	.long	_memcpy@PLT-(.LPCS114+2-.)
.L782:
	.long	.LC16@GOTOFF
.L783:
	.long	_memcpy@PLT-(.LPCS115+2-.)
	.align 1
.L747:
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov	#1,r3
	mov.l	@(52,r2),r2
	mov	r3,r4
	mov.l	.L786,r3
	add	r12,r3
	mov	r2,r0
	jsr	@r3
	nop
	mov	r0,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(16,r1),r2
	mov	r14,r8
	add	#-36,r8
	mov	r14,r1
	add	#-36,r1
	mov	r2,r4
	mov.l	@(52,r1),r5
	mov.l	.L787,r1
	bsrf	r1
.LPCS116:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
.L740:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r2
	mov.l	.L788,r1
	cmp/gt	r1,r2
	bt	.L747
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	r2,r2
	mov.w	.L789,r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov	r2,r4
	mov.l	.L790,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	mov.l	r2,@(44,r1)
	mov	r14,r1
	add	#-36,r1
	mov	#32,r2
	mov.l	r2,@(40,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L794,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L792,r0
	bsrf	r0
.LPCS117:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#14,r1
	mov	r1,r4
	mov	#2,r5
	mov.l	.L793,r1
	bsrf	r1
.LPCS118:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#17,r2
	mov.w	.L794,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L795,r0
	bsrf	r0
.LPCS119:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#19,r1
	mov	r1,r4
	mov	#0,r5
	mov.l	.L796,r1
	bsrf	r1
.LPCS120:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#22,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	extu.w	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L797,r0
	bsrf	r0
.LPCS121:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#24,r1
	mov	r1,r4
	mov	#64,r5
	mov.l	.L798,r1
	bsrf	r1
.LPCS122:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#26,r2
	mov.w	.L799,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L800,r0
	bsrf	r0
.LPCS123:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#28,r1
	mov	r1,r4
	mov	#0,r5
	mov.l	.L801,r1
	bsrf	r1
.LPCS124:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#32,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(16,r1),r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L802,r0
	bsrf	r0
.LPCS125:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r1,r9
	add	#39,r9
	mov.l	.L803,r1
	bsrf	r1
.LPCS126:
	nop
	mov	r0,r8
	mov.l	.L804,r1
	bsrf	r1
.LPCS127:
	nop
	mov	r0,r1
	mul.l	r1,r8
	sts	macl,r1
	mov	r9,r4
	mov	r1,r5
	mov.l	.L805,r0
	bsrf	r0
.LPCS128:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(56,r2),r2
	extu.b	r2,r2
	mov	r2,r0
	mov.b	r0,@(13,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#16,r1
	mov	#2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#16,r1
	mov	#-8,r2
	mov	r2,r0
	mov.b	r0,@(5,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#32,r1
	mov	#-128,r2
	mov	r2,r0
	mov.b	r0,@(4,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#32,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(5,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(48,r1),r1
	add	#32,r1
	mov	#41,r2
	mov	r2,r0
	mov.b	r0,@(6,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r1
	mov	r1,r2
	add	#2,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(20,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r1
	mov	r1,r2
	add	#2,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(24,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r1
	mov	r1,r2
	add	#2,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	add	r1,r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(28,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(28,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(32,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov	r2,r0
	mov.b	r0,@(10,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	extu.b	r1,r1
	bra	.L785
	nop
	.align 1
.L789:
	.short	511
.L794:
	.short	512
.L799:
	.short	256
.L806:
	.align 2
.L786:
	.long	___ashlsi3_r0@GOTOFF
.L787:
	.long	___ashrsi3@PLT-(.LPCS116+2-.)
.L788:
	.long	65525
.L790:
	.long	___ashiftrt_r4_9@GOTOFF
.L792:
	.long	_tkfat_setWord@PLT-(.LPCS117+2-.)
.L793:
	.long	_tkfat_setWord@PLT-(.LPCS118+2-.)
.L795:
	.long	_tkfat_setWord@PLT-(.LPCS119+2-.)
.L796:
	.long	_tkfat_setWord@PLT-(.LPCS120+2-.)
.L797:
	.long	_tkfat_setWord@PLT-(.LPCS121+2-.)
.L798:
	.long	_tkfat_setWord@PLT-(.LPCS122+2-.)
.L800:
	.long	_tkfat_setWord@PLT-(.LPCS123+2-.)
.L801:
	.long	_tkfat_setDWord@PLT-(.LPCS124+2-.)
.L802:
	.long	_tkfat_setDWord@PLT-(.LPCS125+2-.)
.L803:
	.long	_rand@PLT-(.LPCS126+2-.)
.L804:
	.long	_rand@PLT-(.LPCS127+2-.)
.L805:
	.long	_tkfat_setDWord@PLT-(.LPCS128+2-.)
	.align 1
.L785:
	add	#9,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(16,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(32,r1),r1
	sub	r1,r2
	mov	r14,r8
	add	#-36,r8
	mov	r14,r1
	add	#-36,r1
	mov	r2,r4
	mov.l	@(52,r1),r5
	mov.l	.L807,r1
	bsrf	r1
.LPCS129:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(36,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	.L810,r2
	mov.l	@(36,r1),r4
	mov	#0,r5
	mov	r2,r6
	mov.l	.L809,r1
	bsrf	r1
.LPCS130:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	.L810,r2
	mov.l	@(36,r1),r4
	mov	#1,r5
	mov	r2,r6
	mov.l	.L811,r1
	bsrf	r1
.LPCS131:
	nop
	mov.l	.L812,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L813,r0
	bsrf	r0
.LPCS132:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(20,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(20,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L814,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L815,r1
	bsrf	r1
.LPCS133:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(24,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(24,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L816,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L817,r0
	bsrf	r0
.LPCS134:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(28,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L818,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L819,r1
	bsrf	r1
.LPCS135:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(32,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(32,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L820,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L821,r0
	bsrf	r0
.LPCS136:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L822,r1
	add	r12,r1
	mov	r1,r3
	mov	r14,r1
	add	#-36,r1
	mov	r3,r4
	mov.l	@(56,r1),r5
	mov	r2,r6
	mov.l	.L823,r1
	bsrf	r1
.LPCS137:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(36,r1),r2
	mov.l	.L824,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L825,r0
	bsrf	r0
.LPCS138:
	nop
	bra	.L736
	nop
	.align 1
.L739:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#3,r2
	mov.l	.L826,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#9,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L827,r1
	bsrf	r1
.LPCS139:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#71,r2
	mov.l	.L828,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#12,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L829,r1
	bsrf	r1
.LPCS140:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#82,r2
	mov.l	.L830,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#9,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L831,r1
	bsrf	r1
.LPCS141:
	nop
	mov	r14,r1
	add	#-36,r1
	mov	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov.l	@(16,r2),r2
	mov.l	r2,@(60,r1)
	bra	.L754
	nop
.L832:
	.align 2
.L807:
	.long	___ashrsi3@PLT-(.LPCS129+2-.)
.L810:
	.long	65535
.L809:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS130+2-.)
.L811:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS131+2-.)
.L812:
	.long	.LC17@GOTOFF
.L813:
	.long	_printf@PLT-(.LPCS132+2-.)
.L814:
	.long	.LC18@GOTOFF
.L815:
	.long	_printf@PLT-(.LPCS133+2-.)
.L816:
	.long	.LC19@GOTOFF
.L817:
	.long	_printf@PLT-(.LPCS134+2-.)
.L818:
	.long	.LC20@GOTOFF
.L819:
	.long	_printf@PLT-(.LPCS135+2-.)
.L820:
	.long	.LC21@GOTOFF
.L821:
	.long	_printf@PLT-(.LPCS136+2-.)
.L822:
	.long	.LC22@GOTOFF
.L823:
	.long	_printf@PLT-(.LPCS137+2-.)
.L824:
	.long	.LC23@GOTOFF
.L825:
	.long	_printf@PLT-(.LPCS138+2-.)
.L826:
	.long	.LC14@GOTOFF
.L827:
	.long	_memcpy@PLT-(.LPCS139+2-.)
.L828:
	.long	.LC15@GOTOFF
.L829:
	.long	_memcpy@PLT-(.LPCS140+2-.)
.L830:
	.long	.LC24@GOTOFF
.L831:
	.long	_memcpy@PLT-(.LPCS141+2-.)
	.align 1
.L761:
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov	#1,r3
	mov.l	@(52,r2),r2
	mov	r3,r4
	mov.l	.L834,r3
	add	r12,r3
	mov	r2,r0
	jsr	@r3
	nop
	mov	r0,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(16,r1),r2
	mov	r14,r8
	add	#-36,r8
	mov	r14,r1
	add	#-36,r1
	mov	r2,r4
	mov.l	@(52,r1),r5
	mov.l	.L835,r1
	bsrf	r1
.LPCS142:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
.L754:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r2
	mov.l	.L836,r1
	cmp/gt	r1,r2
	bt	.L761
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	shll2	r2
	mov.w	.L837,r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov	r2,r4
	mov.l	.L838,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	mov.l	r2,@(44,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L839,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L840,r0
	bsrf	r0
.LPCS143:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#14,r1
	mov	r1,r4
	mov	#8,r5
	mov.l	.L841,r1
	bsrf	r1
.LPCS144:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#17,r1
	mov	r1,r4
	mov	#0,r5
	mov.l	.L842,r0
	bsrf	r0
.LPCS145:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#19,r1
	mov	r1,r4
	mov	#0,r5
	mov.l	.L843,r1
	bsrf	r1
.LPCS146:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#22,r1
	mov	r1,r4
	mov	#0,r5
	mov.l	.L844,r0
	bsrf	r0
.LPCS147:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#24,r1
	mov	r1,r4
	mov	#64,r5
	mov.l	.L845,r1
	bsrf	r1
.LPCS148:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#26,r2
	mov.w	.L846,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L847,r0
	bsrf	r0
.LPCS149:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#48,r2
	mov.l	.L850,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L849,r1
	bsrf	r1
.LPCS150:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#50,r2
	mov.l	.L850,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L851,r0
	bsrf	r0
.LPCS151:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#28,r1
	mov	r1,r4
	mov	#0,r5
	mov.l	.L852,r1
	bsrf	r1
.LPCS152:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#32,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(16,r1),r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L853,r0
	bsrf	r0
.LPCS153:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#36,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L854,r1
	bsrf	r1
.LPCS154:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#44,r1
	mov	r1,r4
	mov	#2,r5
	mov.l	.L855,r0
	bsrf	r0
.LPCS155:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r1,r9
	add	#67,r9
	mov.l	.L856,r1
	bsrf	r1
.LPCS156:
	nop
	mov	r0,r8
	mov.l	.L857,r1
	bsrf	r1
.LPCS157:
	nop
	mov	r0,r1
	mul.l	r1,r8
	sts	macl,r1
	mov	r9,r4
	mov	r1,r5
	mov.l	.L858,r0
	bsrf	r0
.LPCS158:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(56,r2),r2
	extu.b	r2,r2
	mov	r2,r0
	mov.b	r0,@(13,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#16,r1
	mov	#2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#16,r1
	mov	#-8,r2
	mov	r2,r0
	mov.b	r0,@(5,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#64,r1
	mov	#-128,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#64,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(1,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(52,r1),r1
	add	#64,r1
	mov	#41,r2
	mov	r2,r0
	mov.b	r0,@(2,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r1
	mov	r1,r2
	add	#8,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(20,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r1
	mov	r1,r2
	add	#8,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(24,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r1
	bra	.L833
	nop
	.align 1
.L837:
	.short	511
.L839:
	.short	512
.L846:
	.short	256
.L859:
	.align 2
.L834:
	.long	___ashlsi3_r0@GOTOFF
.L835:
	.long	___ashrsi3@PLT-(.LPCS142+2-.)
.L836:
	.long	268435445
.L838:
	.long	___ashiftrt_r4_9@GOTOFF
.L840:
	.long	_tkfat_setWord@PLT-(.LPCS143+2-.)
.L841:
	.long	_tkfat_setWord@PLT-(.LPCS144+2-.)
.L842:
	.long	_tkfat_setWord@PLT-(.LPCS145+2-.)
.L843:
	.long	_tkfat_setWord@PLT-(.LPCS146+2-.)
.L844:
	.long	_tkfat_setWord@PLT-(.LPCS147+2-.)
.L845:
	.long	_tkfat_setWord@PLT-(.LPCS148+2-.)
.L847:
	.long	_tkfat_setWord@PLT-(.LPCS149+2-.)
.L850:
	.long	65535
.L849:
	.long	_tkfat_setWord@PLT-(.LPCS150+2-.)
.L851:
	.long	_tkfat_setWord@PLT-(.LPCS151+2-.)
.L852:
	.long	_tkfat_setDWord@PLT-(.LPCS152+2-.)
.L853:
	.long	_tkfat_setDWord@PLT-(.LPCS153+2-.)
.L854:
	.long	_tkfat_setDWord@PLT-(.LPCS154+2-.)
.L855:
	.long	_tkfat_setDWord@PLT-(.LPCS155+2-.)
.L856:
	.long	_rand@PLT-(.LPCS156+2-.)
.L857:
	.long	_rand@PLT-(.LPCS157+2-.)
.L858:
	.long	_tkfat_setDWord@PLT-(.LPCS158+2-.)
	.align 1
.L833:
	mov	r1,r2
	add	#8,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	add	r1,r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(32,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(32,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	r2,@(28,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov	r2,r0
	mov.b	r0,@(10,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	extu.b	r1,r1
	add	#9,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(12,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(16,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(32,r1),r1
	sub	r1,r2
	mov	r14,r8
	add	#-36,r8
	mov	r14,r1
	add	#-36,r1
	mov	r2,r4
	mov.l	@(52,r1),r5
	mov.l	.L860,r1
	bsrf	r1
.LPCS159:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(36,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	.L866,r2
	mov.l	@(36,r1),r4
	mov	#0,r5
	mov	r2,r6
	mov.l	.L862,r1
	bsrf	r1
.LPCS160:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	.L866,r2
	mov.l	@(36,r1),r4
	mov	#1,r5
	mov	r2,r6
	mov.l	.L864,r1
	bsrf	r1
.LPCS161:
	nop
	mov	r14,r1
	add	#-36,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L768
	nop
	.align 1
.L769:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	mov	r1,r3
	add	#2,r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#3,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L865,r1
	bsrf	r1
.LPCS162:
	nop
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L768:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r2
	mov	#31,r1
	cmp/gt	r1,r2
	bf	.L769
	mov	r14,r1
	add	#-36,r1
	mov.l	.L866,r2
	mov.l	@(36,r1),r4
	mov	#33,r5
	mov	r2,r6
	mov.l	.L867,r1
	bsrf	r1
.LPCS163:
	nop
	mov.l	.L868,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L869,r0
	bsrf	r0
.LPCS164:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(20,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(20,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L870,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L871,r1
	bsrf	r1
.LPCS165:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(24,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(24,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L872,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L873,r0
	bsrf	r0
.LPCS166:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(28,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L874,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L875,r1
	bsrf	r1
.LPCS167:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(32,r1),r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(32,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L876,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L877,r0
	bsrf	r0
.LPCS168:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L878,r1
	add	r12,r1
	mov	r1,r3
	mov	r14,r1
	add	#-36,r1
	mov	r3,r4
	mov.l	@(56,r1),r5
	mov	r2,r6
	mov.l	.L879,r1
	bsrf	r1
.LPCS169:
	nop
	mov	r14,r1
	add	#-36,r1
	mov.l	@(36,r1),r1
	mov.l	@(36,r1),r2
	mov.l	.L880,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L881,r0
	bsrf	r0
.LPCS170:
	nop
.L736:
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	rts	
	nop
.L882:
	.align 2
.L860:
	.long	___ashrsi3@PLT-(.LPCS159+2-.)
.L866:
	.long	268435455
.L862:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS160+2-.)
.L864:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS161+2-.)
.L865:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS162+2-.)
.L867:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS163+2-.)
.L868:
	.long	.LC25@GOTOFF
.L869:
	.long	_printf@PLT-(.LPCS164+2-.)
.L870:
	.long	.LC26@GOTOFF
.L871:
	.long	_printf@PLT-(.LPCS165+2-.)
.L872:
	.long	.LC27@GOTOFF
.L873:
	.long	_printf@PLT-(.LPCS166+2-.)
.L874:
	.long	.LC28@GOTOFF
.L875:
	.long	_printf@PLT-(.LPCS167+2-.)
.L876:
	.long	.LC29@GOTOFF
.L877:
	.long	_printf@PLT-(.LPCS168+2-.)
.L878:
	.long	.LC22@GOTOFF
.L879:
	.long	_printf@PLT-(.LPCS169+2-.)
.L880:
	.long	.LC23@GOTOFF
.L881:
	.long	_printf@PLT-(.LPCS170+2-.)
	.size	_TKFAT_SetupImageFAT, .-_TKFAT_SetupImageFAT
	.section	.rodata
	.align 2
.LC30:
	.string	"TKFAT_ReadImageFAT: Bad Sectors/Cluster %d\n"
	.global	___ashrdi3
	.align 2
.LC31:
	.string	"TKFAT_ReadImageFAT: Read FAT%d\n"
	.align 2
.LC32:
	.string	"  Root Cluster=%08X\n"
	.text
	.align 1
	.global	_TKFAT_ReadImageFAT
	.type	_TKFAT_ReadImageFAT, @function
_TKFAT_ReadImageFAT:
	mov.l	r8,@-r15
	mov.l	r9,@-r15
	mov.l	r10,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L928,r0
	mov.l	.L928,r12
	add	r0,r12
	add	#-76,r15
	add	#-76,r15
	mov	r15,r14
	mov	r14,r1
	add	#24,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r4
	mov	r2,r5
	mov	#1,r6
	mov.l	.L929,r1
	bsrf	r1
.LPCS171:
	nop
	mov	r0,r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(48,r1),r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#19,r1
	mov	r1,r4
	mov.l	.L930,r2
	bsrf	r2
.LPCS172:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	r2,@(28,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#32,r1
	mov	r14,r8
	add	#88,r8
	mov	r1,r4
	mov.l	.L931,r3
	bsrf	r3
.LPCS173:
	nop
	mov	r0,r1
	mov.l	r1,@(24,r8)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#22,r1
	mov	r1,r4
	mov.l	.L932,r1
	bsrf	r1
.LPCS174:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	r2,@(20,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#36,r1
	mov	r14,r8
	add	#88,r8
	mov	r1,r4
	mov.l	.L933,r2
	bsrf	r2
.LPCS175:
	nop
	mov	r0,r1
	mov.l	r1,@(16,r8)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#14,r1
	mov	r1,r4
	mov.l	.L934,r3
	bsrf	r3
.LPCS176:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	r2,@(12,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#17,r1
	mov	r1,r4
	mov.l	.L935,r1
	bsrf	r1
.LPCS177:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	r2,@(8,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	r2,@(4,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#44,r1
	mov	r1,r4
	mov.l	.L936,r1
	bsrf	r1
.LPCS178:
	nop
	mov	r0,r2
	mov	r14,r1
	add	#88,r1
	mov.l	r2,@(40,r1)
	mov	r14,r1
	add	#88,r1
	mov.l	@(28,r1),r1
	tst	r1,r1
	bt	.L886
	mov	r14,r1
	add	#88,r1
	mov.l	@(24,r1),r1
	tst	r1,r1
	bt	.L886
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#64,r1
	mov.b	@(2,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#41,r1
	cmp/eq	r1,r2
	bf	.L886
	mov	r14,r1
	add	#88,r1
	mov.l	@(28,r1),r1
	mov	#64,r2
	add	r14,r2
	mov.l	r1,@(12,r2)
	mov	#0,r4
	mov	#64,r3
	add	r14,r3
	mov.l	r4,@(8,r3)
	mov	#64,r0
	add	r14,r0
	mov.l	@(12,r0),r1
	mov	#64,r0
	add	r14,r0
	mov.l	r1,@(0,r0)
	mov	#0,r3
	mov	#64,r2
	add	r14,r2
	mov.l	r3,@(4,r2)
	mov	r14,r1
	add	#88,r1
	mov.l	@(24,r1),r1
	mov.l	r1,@(60,r14)
	mov	#0,r4
	mov.l	r4,@(56,r14)
	mov.w	.L938,r2
	add	r14,r2
	mov	#64,r0
	add	r14,r0
	mov.l	@(0,r0),r1
	mov.l	@(56,r14),r0
	or	r0,r1
	mov.l	r1,@r2
	mov	#64,r3
	add	r14,r3
	mov.l	@(4,r3),r1
	mov.l	@(60,r14),r3
	or	r3,r1
	mov.l	r1,@(4,r2)
	mov	r14,r1
	add	#88,r1
	mov.l	@(20,r1),r1
	mov.l	r1,@(52,r14)
	mov	#0,r4
	mov.l	r4,@(48,r14)
	mov.l	@(52,r14),r0
	mov.l	r0,@(40,r14)
	mov	#0,r1
	mov.l	r1,@(44,r14)
	mov	r14,r1
	add	#88,r1
	mov.l	@(16,r1),r1
	mov.l	r1,@(36,r14)
	mov	#0,r2
	mov.l	r2,@(32,r14)
	mov	r14,r1
	add	#88,r1
	mov.l	@(40,r14),r2
	mov.l	@(32,r14),r3
	or	r3,r2
	mov.l	r2,@(48,r1)
	mov.l	@(44,r14),r2
	mov.l	@(36,r14),r4
	or	r4,r2
	mov.l	r2,@(52,r1)
	bra	.L887
	nop
	.align 1
.L886:
	mov	r14,r1
	add	#88,r1
	mov.l	@(28,r1),r1
	tst	r1,r1
	bt	.L888
	mov	r14,r1
	add	#88,r1
	mov.l	@(28,r1),r2
	bra	.L889
	nop
	.align 1
.L888:
	mov	r14,r1
	add	#88,r1
	mov.l	@(24,r1),r2
.L889:
	mov.w	.L938,r1
	add	r14,r1
	mov.l	r2,@(4,r1)
	mov	#0,r2
	mov.l	r2,@r1
	mov	r14,r1
	add	#88,r1
	mov.l	@(20,r1),r1
	tst	r1,r1
	bt	.L890
	mov	r14,r1
	add	#88,r1
	mov.l	@(20,r1),r2
	bra	.L891
	nop
	.align 1
.L938:
	.short	144
.L939:
	.align 2
.L928:
	.long	_GLOBAL_OFFSET_TABLE_
.L929:
	.long	_TKFAT_GetSectorStaticBuffer@PLT-(.LPCS171+2-.)
.L930:
	.long	_tkfat_getWord@PLT-(.LPCS172+2-.)
.L931:
	.long	_tkfat_getDWord@PLT-(.LPCS173+2-.)
.L932:
	.long	_tkfat_getWord@PLT-(.LPCS174+2-.)
.L933:
	.long	_tkfat_getDWord@PLT-(.LPCS175+2-.)
.L934:
	.long	_tkfat_getWord@PLT-(.LPCS176+2-.)
.L935:
	.long	_tkfat_getWord@PLT-(.LPCS177+2-.)
.L936:
	.long	_tkfat_getDWord@PLT-(.LPCS178+2-.)
	.align 1
.L890:
	mov	r14,r1
	add	#88,r1
	mov.l	@(16,r1),r2
.L891:
	mov	r14,r1
	add	#88,r1
	mov.l	r2,@(52,r1)
	mov	#0,r2
	mov.l	r2,@(48,r1)
	mov.w	.L943,r1
	add	r14,r1
	mov.l	@(4,r1),r2
	mov.l	@r1,r1
	mov	r1,r3
	tst	r3,r3
	bf	.L887
	mov	r2,r1
	tst	r1,r1
	bf	.L887
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#86,r1
	mov	r14,r8
	add	#88,r8
	mov	r1,r4
	mov.l	.L941,r1
	bsrf	r1
.LPCS179:
	nop
	mov	r0,r1
	mov.l	r1,@(28,r8)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#82,r1
	mov	r14,r8
	add	#88,r8
	mov	r1,r4
	mov.l	.L942,r2
	bsrf	r2
.LPCS180:
	nop
	mov	r0,r1
	mov.l	r1,@(24,r8)
	mov	r14,r1
	add	#88,r1
	mov.l	@(28,r1),r1
	mov.l	r1,@(28,r14)
	mov	#0,r3
	mov.l	r3,@(24,r14)
	mov.l	@(28,r14),r4
	mov.l	r4,@(16,r14)
	mov	#0,r0
	mov.l	r0,@(20,r14)
	mov	r14,r1
	add	#88,r1
	mov.l	@(24,r1),r1
	mov	r1,r10
	mov	#0,r9
	mov.w	.L943,r1
	add	r14,r1
	mov.l	@(16,r14),r2
	or	r9,r2
	mov.l	r2,@r1
	mov.l	@(20,r14),r2
	or	r10,r2
	mov.l	r2,@(4,r1)
.L887:
	mov	r14,r1
	add	#88,r1
	mov	#0,r2
	mov.l	r2,@(44,r1)
	mov	r14,r1
	add	#88,r1
	mov.l	@(8,r1),r1
	tst	r1,r1
	bt	.L892
	mov	r14,r1
	add	#88,r1
	mov.l	@(8,r1),r1
	mov	r1,r2
	shll2	r2
	add	r2,r2
	shll2	r2
	mov.w	.L944,r1
	add	r1,r2
	mov	r14,r1
	add	#88,r1
	mov	r2,r4
	mov.l	.L945,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	mov.l	r2,@(44,r1)
.L892:
	mov	r14,r1
	add	#88,r1
	mov.l	@(4,r1),r1
	tst	r1,r1
	bt	.L893
	mov	r14,r1
	add	#88,r1
	mov.l	@(4,r1),r1
	mov	r1,r2
	add	#-1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(4,r1),r1
	and	r2,r1
	tst	r1,r1
	bt	.L894
.L893:
	mov.l	.L946,r1
	add	r12,r1
	mov	r1,r2
	mov	r14,r1
	add	#88,r1
	mov	r2,r4
	mov.l	@(4,r1),r5
	mov.l	.L947,r1
	bsrf	r1
.LPCS181:
	nop
	bra	.L885
	nop
	.align 1
.L894:
	mov	r14,r1
	add	#88,r1
	mov	r14,r2
	add	#88,r2
	mov.l	@(4,r2),r2
	mov.l	r2,@(32,r1)
	mov	r14,r1
	add	#88,r1
	mov	#0,r2
	mov.l	r2,@(36,r1)
	bra	.L896
	nop
	.align 1
.L943:
	.short	144
.L944:
	.short	511
.L948:
	.align 2
.L941:
	.long	_tkfat_getDWord@PLT-(.LPCS179+2-.)
.L942:
	.long	_tkfat_getDWord@PLT-(.LPCS180+2-.)
.L945:
	.long	___ashiftrt_r4_9@GOTOFF
.L946:
	.long	.LC30@GOTOFF
.L947:
	.long	_printf@PLT-(.LPCS181+2-.)
	.align 1
.L897:
	mov	r14,r2
	add	#88,r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(32,r1),r1
	shar	r1
	mov.l	r1,@(32,r2)
	mov	r14,r1
	add	#88,r1
	mov	r14,r2
	add	#88,r2
	mov.l	@(36,r2),r2
	add	#1,r2
	mov.l	r2,@(36,r1)
.L896:
	mov	r14,r1
	add	#88,r1
	mov.l	@(32,r1),r2
	mov	#1,r1
	cmp/gt	r1,r2
	bt	.L897
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(52,r1),r1
	add	#64,r1
	mov.b	@(2,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#41,r1
	cmp/eq	r1,r2
	bf	.L898
	mov	r14,r1
	add	#88,r1
	mov.l	@(40,r1),r1
	tst	r1,r1
	bf	.L899
.L898:
	mov	r14,r1
	add	#88,r1
	mov	#1,r2
	mov.l	r2,@(40,r1)
.L899:
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(12,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(20,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(12,r1),r1
	add	r2,r1
	mov	r1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(52,r1),r1
	add	r2,r1
	mov	r1,r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(24,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(12,r1),r1
	add	r2,r1
	mov	r1,r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(52,r1),r1
	add	r1,r1
	add	r2,r1
	mov	r1,r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(28,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(28,r1),r2
	mov	r14,r1
	add	#88,r1
	mov.l	@(44,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(32,r1)
	mov	r14,r1
	add	#88,r1
	mov.l	@(4,r1),r1
	extu.b	r1,r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov	r2,r0
	mov.b	r0,@(10,r1)
	mov	r14,r1
	add	#88,r1
	mov.l	@(36,r1),r1
	extu.b	r1,r1
	add	#9,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(12,r1),r1
	mov.l	r1,@(12,r14)
	mov	#0,r2
	cmp/gt	r1,r2
	subc	r1,r1
	mov.l	r1,@(8,r14)
	mov.w	.L949,r1
	add	r14,r1
	mov.l	@(4,r1),r2
	mov.l	@r1,r1
	mov.l	@(8,r14),r3
	mov.l	@(12,r14),r4
	clrt
	addc	r4,r2
	addc	r3,r1
	mov	r14,r3
	add	#24,r3
	mov.l	@(56,r3),r3
	mov.l	@(32,r3),r3
	mov.l	r3,@(4,r14)
	mov	#0,r7
	cmp/gt	r3,r7
	subc	r3,r3
	mov.l	r3,@r14
	mov.l	@r14,r3
	mov.l	@(4,r14),r4
	clrt
	subc	r4,r2
	subc	r3,r1
	mov	r14,r8
	add	#84,r8
	mov	r14,r3
	add	#88,r3
	mov	r1,r4
	mov	r2,r5
	mov.l	@(36,r3),r6
	mov.l	.L950,r2
	bsrf	r2
.LPCS182:
	nop
	mov	r1,r2
	mov	r0,r1
	mov.l	r1,@r8
	mov.l	r2,@(4,r8)
	mov	r14,r1
	add	#84,r1
	mov.l	@(4,r1),r2
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	r2,@(36,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#88,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(40,r1)
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(9,r1)
	mov	r14,r1
	add	#84,r1
	mov.l	@r1,r3
	mov.l	@(4,r1),r4
	mov	#0,r1
	mov.l	.L951,r2
	mov	r3,r6
	mov	r1,r7
	cmp/gt	r7,r6
	bt	.L922
	mov	r3,r6
	mov	r1,r7
	cmp/ge	r7,r6
	bf	.L926
	mov	r4,r3
	mov	r2,r1
	cmp/hi	r1,r3
	bt	.L922
.L926:
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov	#1,r2
	mov	r2,r0
	mov.b	r0,@(9,r1)
.L922:
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L924
	mov	#16,r1
	bra	.L925
	nop
	.align 1
.L949:
	.short	144
.L952:
	.align 2
.L950:
	.long	___ashrdi3@PLT-(.LPCS182+2-.)
.L951:
	.long	65525
	.align 1
.L924:
	mov	#32,r1
.L925:
	mov.l	.L953,r2
	add	r12,r2
	mov	r2,r4
	mov	r1,r5
	mov.l	.L954,r1
	bsrf	r1
.LPCS183:
	nop
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(20,r1),r3
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(20,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L955,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L956,r2
	bsrf	r2
.LPCS184:
	nop
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(24,r1),r3
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(24,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L957,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L958,r3
	bsrf	r3
.LPCS185:
	nop
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(28,r1),r3
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L959,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L960,r0
	bsrf	r0
.LPCS186:
	nop
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(32,r1),r3
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(32,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L961,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L962,r1
	bsrf	r1
.LPCS187:
	nop
	mov	r14,r1
	add	#88,r1
	mov.l	@(4,r1),r1
	mov	r1,r2
	shll8	r2
	add	r2,r2
	mov.l	.L963,r1
	add	r12,r1
	mov	r1,r3
	mov	r14,r1
	add	#88,r1
	mov	r3,r4
	mov.l	@(4,r1),r5
	mov	r2,r6
	mov.l	.L964,r2
	bsrf	r2
.LPCS188:
	nop
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(36,r1),r2
	mov.l	.L965,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L966,r3
	bsrf	r3
.LPCS189:
	nop
	mov	r14,r1
	add	#24,r1
	mov.l	@(56,r1),r1
	mov.l	@(40,r1),r2
	mov.l	.L967,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L968,r0
	bsrf	r0
.LPCS190:
	nop
.L885:
	add	#76,r14
	add	#76,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r10
	mov.l	@r15+,r9
	mov.l	@r15+,r8
	rts	
	nop
.L969:
	.align 2
.L953:
	.long	.LC31@GOTOFF
.L954:
	.long	_printf@PLT-(.LPCS183+2-.)
.L955:
	.long	.LC18@GOTOFF
.L956:
	.long	_printf@PLT-(.LPCS184+2-.)
.L957:
	.long	.LC19@GOTOFF
.L958:
	.long	_printf@PLT-(.LPCS185+2-.)
.L959:
	.long	.LC20@GOTOFF
.L960:
	.long	_printf@PLT-(.LPCS186+2-.)
.L961:
	.long	.LC21@GOTOFF
.L962:
	.long	_printf@PLT-(.LPCS187+2-.)
.L963:
	.long	.LC22@GOTOFF
.L964:
	.long	_printf@PLT-(.LPCS188+2-.)
.L965:
	.long	.LC23@GOTOFF
.L966:
	.long	_printf@PLT-(.LPCS189+2-.)
.L967:
	.long	.LC32@GOTOFF
.L968:
	.long	_printf@PLT-(.LPCS190+2-.)
	.size	_TKFAT_ReadImageFAT, .-_TKFAT_ReadImageFAT
	.align 1
	.global	_TKFAT_AllocFreeCluster
	.type	_TKFAT_AllocFreeCluster, @function
_TKFAT_AllocFreeCluster:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L976,r0
	mov.l	.L976,r12
	add	r0,r12
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
	mov.l	@(36,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-48,r1
	mov	#2,r2
	mov.l	r2,@(60,r1)
	bra	.L971
	nop
	.align 1
.L974:
	mov	r14,r8
	add	#-48,r8
	mov	r14,r2
	add	#-48,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(48,r2),r4
	mov.l	@(60,r1),r5
	mov.l	.L977,r1
	bsrf	r1
.LPCS191:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-48,r1
	mov.l	@(52,r1),r1
	tst	r1,r1
	bf	.L972
	mov	r14,r2
	add	#-48,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	.L978,r3
	mov.l	@(48,r2),r4
	mov.l	@(60,r1),r5
	mov	r3,r6
	mov.l	.L979,r1
	bsrf	r1
.LPCS192:
	nop
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	bra	.L973
	nop
	.align 1
.L972:
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L971:
	mov	r14,r2
	add	#-48,r2
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/ge	r1,r2
	bf	.L974
	mov	#-1,r1
.L973:
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L980:
	.align 2
.L976:
	.long	_GLOBAL_OFFSET_TABLE_
.L977:
	.long	_TKFAT_GetFatEntry@PLT-(.LPCS191+2-.)
.L978:
	.long	268435455
.L979:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS192+2-.)
	.size	_TKFAT_AllocFreeCluster, .-_TKFAT_AllocFreeCluster
	.align 1
	.global	_TKFAT_GetWalkCluster
	.type	_TKFAT_GetWalkCluster, @function
_TKFAT_GetWalkCluster:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L990,r0
	mov.l	.L990,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r5,@(44,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r6,@(40,r1)
	mov	r7,r2
	mov	r14,r1
	add	#-4,r1
	mov	r2,r0
	mov.b	r0,@(4,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	tst	r1,r1
	bf	.L982
	mov	#-1,r1
	bra	.L983
	nop
	.align 1
.L982:
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(52,r1)
	bra	.L984
	nop
	.align 1
.L988:
	mov	r14,r8
	add	#-36,r8
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r2),r4
	mov.l	@(60,r1),r5
	mov.l	.L991,r1
	bsrf	r1
.LPCS193:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L985
	mov	r14,r1
	add	#-4,r1
	mov.b	@(4,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L986
	mov	#-1,r1
	bra	.L983
	nop
	.align 1
.L986:
	mov	r14,r8
	add	#-36,r8
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r4
	mov.l	.L992,r1
	bsrf	r1
.LPCS194:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	cmp/pl	r1
	bt	.L987
	mov	#-1,r1
	bra	.L983
	nop
	.align 1
.L987:
	mov	r14,r3
	add	#-36,r3
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r3),r4
	mov.l	@(60,r2),r5
	mov.l	@(56,r1),r6
	mov.l	.L993,r1
	bsrf	r1
.LPCS195:
	nop
.L985:
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(56,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(52,r2),r2
	add	#-1,r2
	mov.l	r2,@(52,r1)
.L984:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	cmp/pl	r1
	bt	.L988
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
.L983:
	mov	r1,r0
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L994:
	.align 2
.L990:
	.long	_GLOBAL_OFFSET_TABLE_
.L991:
	.long	_TKFAT_GetFatEntry@PLT-(.LPCS193+2-.)
.L992:
	.long	_TKFAT_AllocFreeCluster@PLT-(.LPCS194+2-.)
.L993:
	.long	_TKFAT_SetFatEntry@PLT-(.LPCS195+2-.)
	.size	_TKFAT_GetWalkCluster, .-_TKFAT_GetWalkCluster
	.align 1
	.global	_TKFAT_GetClusterFileOffs
	.type	_TKFAT_GetClusterFileOffs, @function
_TKFAT_GetClusterFileOffs:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1005,r0
	mov.l	.L1005,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r5,@(44,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r6,@(40,r1)
	mov	r7,r2
	mov	r14,r1
	add	#-4,r1
	mov	r2,r0
	mov.b	r0,@(4,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r8
	add	#-36,r8
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r4
	mov	r2,r5
	mov.l	.L1006,r1
	bsrf	r1
.LPCS196:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	#1,r2
	mov	r2,r4
	mov.l	.L1007,r2
	add	r12,r2
	mov	r1,r0
	jsr	@r2
	nop
	mov	r0,r1
	mov	r1,r3
	add	#-1,r3
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(40,r2),r2
	and	r3,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-4,r1
	mov.b	@(4,r1),r0
	mov	r0,r1
	extu.b	r1,r7
	mov	r14,r8
	add	#-36,r8
	mov	r14,r3
	add	#-36,r3
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r3),r4
	mov.l	@(44,r2),r5
	mov.l	@(60,r1),r6
	mov.l	.L1008,r1
	bsrf	r1
.LPCS197:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	cmp/pz	r1
	bt	.L1002
	mov	#-1,r1
	bra	.L1003
	nop
	.align 1
.L1002:
	mov.l	@(44,r14),r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@r1
	mov.l	@(48,r14),r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(56,r2),r2
	mov.l	r2,@r1
	mov	#0,r1
.L1003:
	mov	r1,r0
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L1009:
	.align 2
.L1005:
	.long	_GLOBAL_OFFSET_TABLE_
.L1006:
	.long	___ashrsi3@PLT-(.LPCS196+2-.)
.L1007:
	.long	___ashlsi3_r0@GOTOFF
.L1008:
	.long	_TKFAT_GetWalkCluster@PLT-(.LPCS197+2-.)
	.size	_TKFAT_GetClusterFileOffs, .-_TKFAT_GetClusterFileOffs
	.align 1
	.global	_TKFAT_ReadWriteSector
	.type	_TKFAT_ReadWriteSector, @function
_TKFAT_ReadWriteSector:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1015,r0
	mov.l	.L1015,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r5,@(52,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r6,@(48,r1)
	mov	r7,r2
	mov	r14,r1
	add	#-12,r1
	mov	r2,r0
	mov.b	r0,@(12,r1)
	mov	r14,r1
	add	#-12,r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L1011
	mov	r14,r8
	add	#-44,r8
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.w	.L1016,r3
	mov.l	@(56,r2),r4
	mov.l	@(52,r1),r5
	mov	r3,r6
	mov.l	.L1017,r1
	bsrf	r1
.LPCS198:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	r1,r2
	mov.l	@(40,r14),r1
	mov	r2,r4
	mov.l	@(36,r14),r5
	mov	r1,r6
	mov.l	.L1018,r1
	bsrf	r1
.LPCS199:
	nop
	bra	.L1012
	nop
	.align 1
.L1011:
	mov	r14,r8
	add	#-44,r8
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r2),r4
	mov.l	@(52,r1),r5
	mov	#1,r6
	mov.l	.L1019,r1
	bsrf	r1
.LPCS200:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(48,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	r1,r2
	mov.l	@(40,r14),r1
	mov.l	@(36,r14),r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L1020,r1
	bsrf	r1
.LPCS201:
	nop
.L1012:
	mov	#0,r1
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L1016:
	.short	513
.L1021:
	.align 2
.L1015:
	.long	_GLOBAL_OFFSET_TABLE_
.L1017:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS198+2-.)
.L1018:
	.long	_memcpy@PLT-(.LPCS199+2-.)
.L1019:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS200+2-.)
.L1020:
	.long	_memcpy@PLT-(.LPCS201+2-.)
	.size	_TKFAT_ReadWriteSector, .-_TKFAT_ReadWriteSector
	.align 1
	.global	_TKFAT_ReadWriteCluster
	.type	_TKFAT_ReadWriteCluster, @function
_TKFAT_ReadWriteCluster:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1027,r0
	mov.l	.L1027,r12
	add	r0,r12
	add	#-24,r15
	mov	r15,r14
	mov	r14,r1
	add	#-40,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r5,@(48,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r6,@(44,r1)
	mov	r7,r2
	mov	r14,r1
	add	#-8,r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r8
	add	#-40,r8
	mov	r14,r2
	add	#-40,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r2),r4
	mov.l	@(48,r1),r5
	mov.l	.L1028,r1
	bsrf	r1
.LPCS202:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-8,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L1023
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r1
	mov.b	@(10,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1029,r1
	mov	r2,r3
	or	r1,r3
	mov	r14,r8
	add	#-40,r8
	mov	r14,r2
	add	#-40,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r2),r4
	mov.l	@(60,r1),r5
	mov	r3,r6
	mov.l	.L1030,r1
	bsrf	r1
.LPCS203:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(44,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(56,r2),r2
	add	r1,r2
	mov.l	@(44,r14),r1
	mov	r2,r4
	mov.l	@(40,r14),r5
	mov	r1,r6
	mov.l	.L1031,r1
	bsrf	r1
.LPCS204:
	nop
	bra	.L1024
	nop
	.align 1
.L1023:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r1
	mov.b	@(10,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r3
	mov	r14,r8
	add	#-40,r8
	mov	r14,r2
	add	#-40,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r2),r4
	mov.l	@(60,r1),r5
	mov	r3,r6
	mov.l	.L1032,r1
	bsrf	r1
.LPCS205:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(44,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(56,r2),r2
	add	r1,r2
	mov.l	@(44,r14),r1
	mov.l	@(40,r14),r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L1033,r1
	bsrf	r1
.LPCS206:
	nop
.L1024:
	mov	#0,r1
	mov	r1,r0
	add	#24,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L1029:
	.short	512
.L1034:
	.align 2
.L1027:
	.long	_GLOBAL_OFFSET_TABLE_
.L1028:
	.long	_TKFAT_GetClusterLBA@PLT-(.LPCS202+2-.)
.L1030:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS203+2-.)
.L1031:
	.long	_memcpy@PLT-(.LPCS204+2-.)
.L1032:
	.long	_TKFAT_GetSectorTempBuffer@PLT-(.LPCS205+2-.)
.L1033:
	.long	_memcpy@PLT-(.LPCS206+2-.)
	.size	_TKFAT_ReadWriteCluster, .-_TKFAT_ReadWriteCluster
	.align 1
	.global	_TKFAT_ReadWriteClusterOffset
	.type	_TKFAT_ReadWriteClusterOffset, @function
_TKFAT_ReadWriteClusterOffset:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1059,r0
	mov.l	.L1059,r12
	add	r0,r12
	add	#-64,r15
	mov	r15,r14
	mov	r14,r1
	mov.l	r4,@(20,r1)
	mov	r14,r1
	mov.l	r5,@(16,r1)
	mov	r14,r1
	mov.l	r6,@(12,r1)
	mov	r7,r2
	mov	r14,r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	#64,r2
	add	r14,r2
	mov.l	@(20,r2),r1
	mov	r1,r2
	add	#-1,r2
	mov	r14,r1
	mov.l	@(12,r1),r1
	add	r1,r2
	mov	r14,r1
	mov.l	r2,@(24,r1)
	mov	r14,r1
	mov.l	@(20,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	mov.l	@(12,r1),r4
	mov	r2,r5
	mov.l	.L1060,r1
	bsrf	r1
.LPCS207:
	nop
	mov	r0,r1
	mov	r1,r8
	mov	r14,r1
	mov.l	@(24,r1),r2
	mov	r14,r1
	mov.l	@(20,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L1061,r1
	bsrf	r1
.LPCS208:
	nop
	mov	r0,r1
	cmp/eq	r1,r8
	bf	.L1048
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r0
	mov	r14,r8
	mov	r14,r3
	mov	r14,r2
	mov	r14,r1
	mov	r14,r7
	add	#36,r7
	mov.l	r7,@r15
	mov	r14,r7
	add	#28,r7
	mov.l	r7,@(4,r15)
	mov.l	@(20,r3),r4
	mov.l	@(16,r2),r5
	mov.l	@(12,r1),r6
	mov	r0,r7
	mov.l	.L1062,r1
	bsrf	r1
.LPCS209:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	mov.l	@(52,r1),r1
	cmp/pz	r1
	bt	.L1049
	mov	r14,r1
	mov.l	@(52,r1),r1
	bra	.L1057
	nop
	.align 1
.L1049:
	mov	r14,r1
	mov.l	@(36,r1),r6
	mov	r14,r1
	mov.l	@(28,r1),r7
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r3
	mov	r14,r8
	mov	r14,r1
	mov	#64,r5
	add	r14,r5
	mov.l	@(16,r5),r2
	mov.l	r2,@r15
	mov	#64,r0
	add	r14,r0
	mov.l	@(20,r0),r2
	mov.l	r2,@(4,r15)
	mov.l	@(20,r1),r4
	mov	r6,r5
	mov	r7,r6
	mov	r3,r7
	mov.l	.L1063,r1
	bsrf	r1
.LPCS210:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	r14,r1
	mov.l	@(52,r1),r1
	bra	.L1057
	nop
	.align 1
.L1048:
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r0
	mov	r14,r8
	mov	r14,r3
	mov	r14,r2
	mov	r14,r1
	mov	r14,r7
	add	#36,r7
	mov.l	r7,@r15
	mov	r14,r7
	add	#28,r7
	mov.l	r7,@(4,r15)
	mov.l	@(20,r3),r4
	mov.l	@(16,r2),r5
	mov.l	@(12,r1),r6
	mov	r0,r7
	mov.l	.L1064,r1
	bsrf	r1
.LPCS211:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov	#64,r2
	add	r14,r2
	mov.l	@(20,r2),r1
	mov	r1,r2
	add	#-1,r2
	mov	r14,r1
	mov.l	@(12,r1),r1
	mov	r2,r6
	add	r1,r6
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r7
	mov	r14,r8
	mov	r14,r2
	mov	r14,r1
	mov	r14,r3
	add	#32,r3
	mov.l	r3,@r15
	mov	r14,r3
	add	#24,r3
	mov.l	r3,@(4,r15)
	mov.l	@(20,r2),r4
	mov.l	@(16,r1),r5
	mov.l	.L1065,r1
	bsrf	r1
.LPCS212:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r1
	mov.l	@(52,r1),r1
	cmp/pz	r1
	bt	.L1051
	mov	r14,r1
	mov.l	@(52,r1),r1
	bra	.L1057
	nop
	.align 1
.L1051:
	mov	r14,r1
	mov.l	@(48,r1),r1
	cmp/pz	r1
	bt	.L1052
	mov	r14,r1
	mov.l	@(48,r1),r1
	bra	.L1057
	nop
	.align 1
.L1052:
	mov	r14,r1
	mov.l	@(20,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r2
	mov	r14,r1
	mov	#1,r3
	mov	r3,r4
	mov.l	.L1066,r3
	add	r12,r3
	mov	r2,r0
	jsr	@r3
	nop
	mov	r0,r2
	mov.l	r2,@(44,r1)
	mov	r14,r1
	mov	#64,r3
	add	r14,r3
	mov.l	@(16,r3),r2
	mov.l	r2,@(60,r1)
	mov	#64,r5
	add	r14,r5
	mov.l	@(20,r5),r2
	mov	r14,r1
	mov	#64,r7
	add	r14,r7
	mov.l	@(16,r7),r3
	add	r3,r2
	mov.l	r2,@(40,r1)
	mov	r14,r1
	mov	r14,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	mov.l	@(28,r1),r6
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r0
	mov	r14,r1
	mov.l	@(28,r1),r1
	mov	r14,r2
	mov.l	@(44,r2),r2
	mov	r2,r3
	sub	r1,r3
	mov	r14,r2
	mov	r14,r1
	mov	r14,r7
	mov.l	@(60,r7),r7
	mov.l	r7,@r15
	mov.l	r3,@(4,r15)
	mov.l	@(20,r2),r4
	mov.l	@(56,r1),r5
	mov	r0,r7
	mov.l	.L1067,r1
	bsrf	r1
.LPCS213:
	nop
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r3
	mov	r14,r8
	mov	r14,r2
	mov	r14,r1
	mov.l	@(20,r2),r4
	mov.l	@(56,r1),r5
	mov	#1,r6
	mov	r3,r7
	mov.l	.L1068,r1
	bsrf	r1
.LPCS214:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	mov.l	@(28,r1),r1
	mov	r14,r2
	mov.l	@(44,r2),r2
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	mov	r1,r2
	mov	r14,r1
	mov	r14,r3
	mov.l	@(60,r3),r3
	add	r3,r2
	mov.l	r2,@(60,r1)
	bra	.L1053
	nop
	.align 1
.L1055:
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r7
	mov	r14,r2
	mov	r14,r1
	mov	r14,r3
	mov.l	@(60,r3),r3
	mov.l	r3,@r15
	mov	r14,r3
	mov.l	@(44,r3),r3
	mov.l	r3,@(4,r15)
	mov.l	@(20,r2),r4
	mov.l	@(56,r1),r5
	mov	#0,r6
	mov.l	.L1069,r1
	bsrf	r1
.LPCS215:
	nop
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r3
	mov	r14,r8
	mov	r14,r2
	mov	r14,r1
	mov.l	@(20,r2),r4
	mov.l	@(56,r1),r5
	mov	#1,r6
	mov	r3,r7
	mov.l	.L1070,r1
	bsrf	r1
.LPCS216:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L1054
	mov	#-1,r1
	bra	.L1057
	nop
.L1071:
	.align 2
.L1059:
	.long	_GLOBAL_OFFSET_TABLE_
.L1060:
	.long	___ashrsi3@PLT-(.LPCS207+2-.)
.L1061:
	.long	___ashrsi3@PLT-(.LPCS208+2-.)
.L1062:
	.long	_TKFAT_GetClusterFileOffs@PLT-(.LPCS209+2-.)
.L1063:
	.long	_TKFAT_ReadWriteCluster@PLT-(.LPCS210+2-.)
.L1064:
	.long	_TKFAT_GetClusterFileOffs@PLT-(.LPCS211+2-.)
.L1065:
	.long	_TKFAT_GetClusterFileOffs@PLT-(.LPCS212+2-.)
.L1066:
	.long	___ashlsi3_r0@GOTOFF
.L1067:
	.long	_TKFAT_ReadWriteCluster@PLT-(.LPCS213+2-.)
.L1068:
	.long	_TKFAT_GetWalkCluster@PLT-(.LPCS214+2-.)
.L1069:
	.long	_TKFAT_ReadWriteCluster@PLT-(.LPCS215+2-.)
.L1070:
	.long	_TKFAT_GetWalkCluster@PLT-(.LPCS216+2-.)
	.align 1
.L1054:
	mov	r14,r1
	mov.l	@(44,r1),r2
	mov	r14,r1
	mov	r14,r3
	mov.l	@(60,r3),r3
	add	r3,r2
	mov.l	r2,@(60,r1)
.L1053:
	mov	r14,r1
	mov.l	@(44,r1),r1
	mov	r14,r2
	mov.l	@(60,r2),r2
	add	r1,r2
	mov	r14,r1
	mov.l	@(40,r1),r1
	cmp/hi	r1,r2
	bf	.L1055
	mov	r14,r2
	mov	r14,r1
	mov.l	@(60,r2),r2
	mov.l	@(40,r1),r1
	cmp/hs	r1,r2
	bt	.L1056
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r0
	mov	r14,r1
	mov.l	@(40,r1),r2
	mov	r14,r1
	mov.l	@(60,r1),r1
	mov	r2,r3
	sub	r1,r3
	mov	r14,r2
	mov	r14,r1
	mov	r14,r7
	mov.l	@(60,r7),r7
	mov.l	r7,@r15
	mov.l	r3,@(4,r15)
	mov.l	@(20,r2),r4
	mov.l	@(56,r1),r5
	mov	#0,r6
	mov	r0,r7
	mov.l	.L1072,r1
	bsrf	r1
.LPCS217:
	nop
.L1056:
	mov	#0,r1
.L1057:
	mov	r1,r0
	add	#64,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L1073:
	.align 2
.L1072:
	.long	_TKFAT_ReadWriteCluster@PLT-(.LPCS217+2-.)
	.size	_TKFAT_ReadWriteClusterOffset, .-_TKFAT_ReadWriteClusterOffset
	.align 1
	.global	_TKFAT_ReadWriteDirEntOffset
	.type	_TKFAT_ReadWriteDirEntOffset, @function
_TKFAT_ReadWriteDirEntOffset:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1081,r0
	mov.l	.L1081,r12
	add	r0,r12
	add	#-32,r15
	mov	r15,r14
	mov	r14,r1
	add	#-32,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	r5,@(48,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	r6,@(44,r1)
	mov	r7,r2
	mov	r14,r1
	mov	r2,r0
	mov.b	r0,@(8,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r1
	cmp/pl	r1
	bt	.L1075
	mov	#-1,r1
	bra	.L1076
	nop
	.align 1
.L1075:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#1,r1
	cmp/eq	r1,r2
	bf	.L1077
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r1),r1
	cmp/pz	r1
	bt	.L1078
	mov	#-1,r1
	bra	.L1076
	nop
	.align 1
.L1078:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r1
	mov.l	@(48,r1),r1
	add	#17,r1
	mov	r1,r4
	mov.l	.L1082,r1
	bsrf	r1
.LPCS218:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	r2,@(60,r1)
	mov	r14,r2
	add	#-32,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r2),r2
	mov.l	@(60,r1),r1
	cmp/ge	r1,r2
	bf	.L1079
	mov	#-1,r1
	bra	.L1076
	nop
	.align 1
.L1079:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r1
	mov.l	@(28,r1),r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r1),r1
	shar	r1
	shar	r1
	shar	r1
	shar	r1
	mov	r2,r6
	add	r1,r6
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r1),r1
	mov	#15,r2
	and	r2,r1
	mov	r1,r7
	shll2	r7
	add	r7,r7
	shll2	r7
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r3
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r14),r2
	mov.l	r2,@r15
	mov	#32,r2
	mov.l	r2,@(4,r15)
	mov.l	@(52,r1),r4
	mov	r6,r5
	mov	r7,r6
	mov	r3,r7
	mov.l	.L1083,r1
	bsrf	r1
.LPCS219:
	nop
	mov	#0,r1
	bra	.L1076
	nop
	.align 1
.L1077:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r1),r1
	mov	r1,r6
	shll2	r6
	add	r6,r6
	shll2	r6
	mov	r14,r1
	mov.b	@(8,r1),r0
	mov	r0,r1
	extu.b	r1,r7
	mov	r14,r8
	add	#-32,r8
	mov	r14,r2
	add	#-32,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r14),r3
	mov.l	r3,@r15
	mov	#32,r3
	mov.l	r3,@(4,r15)
	mov.l	@(52,r2),r4
	mov.l	@(48,r1),r5
	mov.l	.L1084,r1
	bsrf	r1
.LPCS220:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
.L1076:
	mov	r1,r0
	add	#32,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L1085:
	.align 2
.L1081:
	.long	_GLOBAL_OFFSET_TABLE_
.L1082:
	.long	_tkfat_getWord@PLT-(.LPCS218+2-.)
.L1083:
	.long	_TKFAT_ReadWriteSector@PLT-(.LPCS219+2-.)
.L1084:
	.long	_TKFAT_ReadWriteClusterOffset@PLT-(.LPCS220+2-.)
	.size	_TKFAT_ReadWriteDirEntOffset, .-_TKFAT_ReadWriteDirEntOffset
	.align 1
	.global	_tkfat_name2sfn
	.type	_tkfat_name2sfn, @function
_tkfat_name2sfn:
	mov.l	r14,@-r15
	add	#-24,r15
	mov	r15,r14
	mov	r14,r1
	add	#-40,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r5,@(40,r1)
	mov	r14,r1
	add	#-40,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1087
	nop
	.align 1
.L1088:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(40,r2),r2
	add	r2,r1
	mov	#32,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1087:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r2
	mov	#10,r1
	cmp/gt	r1,r2
	bf	.L1088
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-40,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1089
	nop
	.align 1
.L1092:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L1090
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#46,r1
	cmp/eq	r1,r2
	bt	.L1090
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r2
	mov	#96,r1
	cmp/gt	r1,r2
	bf	.L1091
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r2
	mov	#122,r1
	cmp/gt	r1,r2
	bt	.L1091
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	add	#-32,r2
	mov.l	r2,@(48,r1)
.L1091:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	exts.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1089:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r2
	mov	#7,r1
	cmp/gt	r1,r2
	bf	.L1092
.L1090:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bf	.L1093
	mov	#0,r1
	bra	.L1094
	nop
	.align 1
.L1093:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#46,r1
	cmp/eq	r1,r2
	bt	.L1095
	mov	#-1,r1
	bra	.L1094
	nop
	.align 1
.L1095:
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(40,r2),r2
	add	#8,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-40,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1096
	nop
	.align 1
.L1099:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L1097
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#46,r1
	cmp/eq	r1,r2
	bt	.L1097
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r2
	mov	#96,r1
	cmp/gt	r1,r2
	bf	.L1098
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r2
	mov	#122,r1
	cmp/gt	r1,r2
	bt	.L1098
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	add	#-32,r2
	mov.l	r2,@(48,r1)
.L1098:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	exts.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1096:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r2
	mov	#2,r1
	cmp/gt	r1,r2
	bf	.L1099
.L1097:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L1100
	mov	#-1,r1
	bra	.L1094
	nop
	.align 1
.L1100:
	mov	#0,r1
.L1094:
	mov	r1,r0
	add	#24,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_tkfat_name2sfn, .-_tkfat_name2sfn
	.align 1
	.global	_tkfat_name2lfn
	.type	_tkfat_name2lfn, @function
_tkfat_name2lfn:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1114,r0
	mov.l	.L1114,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(40,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r5,@(36,r1)
	mov	r14,r1
	add	#-36,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1102
	nop
	.align 1
.L1103:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	add	r1,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	add	r2,r1
	mov	#-1,r2
	mov.w	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1102:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r2
	mov.w	.L1115,r1
	cmp/gt	r1,r2
	bf	.L1103
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(56,r2),r3
	mov.w	.L1116,r2
	add	r3,r2
	mov.l	r2,@(44,r1)
	bra	.L1104
	nop
	.align 1
.L1111:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov.w	.L1117,r2
	and	r2,r1
	tst	r1,r1
	bf	.L1105
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@(52,r2),r2
	extu.w	r2,r2
	mov.w	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(48,r1)
	bra	.L1104
	nop
	.align 1
.L1105:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov.w	.L1121,r2
	and	r1,r2
	mov.w	.L1119,r1
	cmp/eq	r1,r2
	bf	.L1106
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov	#31,r3
	and	r3,r1
	mov	r1,r3
	shll2	r3
	shll2	r3
	shll2	r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r7
	add	#1,r7
	mov.l	r7,@(60,r2)
	mov.b	@r1,r1
	mov	#63,r2
	and	r1,r2
	mov	r14,r1
	add	#-36,r1
	or	r3,r2
	mov.l	r2,@(48,r1)
	bra	.L1107
	nop
	.align 1
.L1106:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov.w	.L1120,r2
	and	r1,r2
	mov.w	.L1121,r1
	cmp/eq	r1,r2
	bf	.L1108
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	shll8	r1
	shll2	r1
	shll2	r1
	extu.w	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	mov	#63,r3
	and	r3,r1
	shll2	r1
	shll2	r1
	shll2	r1
	mov	r2,r3
	or	r1,r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov.b	@r1,r1
	mov	#63,r2
	and	r1,r2
	mov	r14,r1
	add	#-36,r1
	or	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	add	#2,r2
	mov.l	r2,@(60,r1)
	bra	.L1107
	nop
	.align 1
.L1115:
	.short	255
.L1116:
	.short	512
.L1117:
	.short	128
.L1121:
	.short	224
.L1119:
	.short	192
.L1120:
	.short	240
.L1122:
	.align 2
.L1114:
	.long	_GLOBAL_OFFSET_TABLE_
	.align 1
.L1108:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov.w	.L1123,r2
	and	r1,r2
	mov.w	.L1124,r1
	cmp/eq	r1,r2
	bf	.L1107
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r1),r1
	mov	#7,r2
	and	r2,r1
	mov	r1,r2
	shll16	r2
	shll2	r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	mov	#63,r3
	and	r3,r1
	shll8	r1
	shll2	r1
	shll2	r1
	or	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov.b	@r1,r1
	mov	#63,r3
	and	r3,r1
	shll2	r1
	shll2	r1
	shll2	r1
	mov	r2,r3
	or	r1,r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	add	#2,r1
	mov.b	@r1,r1
	mov	#63,r2
	and	r1,r2
	mov	r14,r1
	add	#-36,r1
	or	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	add	#3,r2
	mov.l	r2,@(60,r1)
.L1107:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r2
	mov.l	.L1125,r1
	cmp/gt	r1,r2
	bf	.L1109
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@(48,r2),r2
	mov	r2,r4
	mov.l	.L1126,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	extu.w	r2,r3
	mov.w	.L1129,r2
	and	r3,r2
	extu.w	r2,r3
	mov.w	.L1128,r2
	add	r3,r2
	extu.w	r2,r2
	mov.w	r2,@r1
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@(48,r2),r2
	extu.w	r2,r3
	mov.w	.L1129,r2
	and	r3,r2
	extu.w	r2,r3
	mov.w	.L1130,r2
	add	r3,r2
	extu.w	r2,r2
	mov.w	r2,@r1
	bra	.L1104
	nop
	.align 1
.L1109:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@(48,r2),r2
	extu.w	r2,r2
	mov.w	r2,@r1
.L1104:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L1110
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r2),r2
	mov.l	@(44,r1),r1
	cmp/hs	r1,r2
	bt	.L1133
	bra	.L1111
	nop
.L1133:
.L1110:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	#0,r2
	mov.w	r2,@r1
	mov	#0,r1
	mov	r1,r0
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
	.align 1
.L1123:
	.short	248
.L1124:
	.short	240
.L1129:
	.short	1023
.L1128:
	.short	-10240
.L1130:
	.short	-9216
.L1131:
	.align 2
.L1125:
	.long	65535
.L1126:
	.long	___ashiftrt_r4_10@GOTOFF
	.size	_tkfat_name2lfn, .-_tkfat_name2lfn
	.align 1
	.global	_tkfat_matchlfn
	.type	_tkfat_matchlfn, @function
_tkfat_matchlfn:
	mov.l	r14,@-r15
	add	#-24,r15
	mov	r15,r14
	mov	r14,r1
	add	#-40,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	r5,@(40,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.w	@r1,r1
	extu.w	r1,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	mov.w	@r1,r1
	extu.w	r1,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	r2,@(48,r1)
	bra	.L1135
	nop
	.align 1
.L1139:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(60,r2)
	mov.w	@r1,r1
	extu.w	r1,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-40,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(56,r2)
	mov.w	@r1,r1
	extu.w	r1,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r2
	mov	#96,r1
	cmp/gt	r1,r2
	bf	.L1136
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r1),r2
	mov	#122,r1
	cmp/gt	r1,r2
	bt	.L1136
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(52,r2),r2
	add	#-32,r2
	mov.l	r2,@(52,r1)
.L1136:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r2
	mov	#96,r1
	cmp/gt	r1,r2
	bf	.L1137
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r1),r2
	mov	#122,r1
	cmp/gt	r1,r2
	bt	.L1137
	mov	r14,r1
	add	#-40,r1
	mov	r14,r2
	add	#-40,r2
	mov.l	@(48,r2),r2
	add	#-32,r2
	mov.l	r2,@(48,r1)
.L1137:
	mov	r14,r2
	add	#-40,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r2),r2
	mov.l	@(48,r1),r1
	cmp/eq	r1,r2
	bt	.L1135
	bra	.L1138
	nop
	.align 1
.L1135:
	mov	r14,r1
	add	#-40,r1
	mov.l	@(60,r1),r1
	mov.w	@r1,r1
	extu.w	r1,r1
	tst	r1,r1
	bt	.L1138
	mov	r14,r1
	add	#-40,r1
	mov.l	@(56,r1),r1
	mov.w	@r1,r1
	extu.w	r1,r1
	tst	r1,r1
	bf	.L1139
.L1138:
	mov	r14,r2
	add	#-40,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(52,r2),r2
	mov.l	@(48,r1),r1
	cmp/gt	r1,r2
	bf	.L1140
	mov	#1,r1
	bra	.L1141
	nop
	.align 1
.L1140:
	mov	r14,r2
	add	#-40,r2
	mov	r14,r1
	add	#-40,r1
	mov.l	@(48,r2),r2
	mov.l	@(52,r1),r1
	cmp/gt	r1,r2
	bf	.L1142
	mov	#-1,r1
	bra	.L1141
	nop
	.align 1
.L1142:
	mov	#0,r1
.L1141:
	mov	r1,r0
	add	#24,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_tkfat_matchlfn, .-_tkfat_matchlfn
	.align 1
	.global	_tkfat_lfnchecksum
	.type	_tkfat_lfnchecksum, @function
_tkfat_lfnchecksum:
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
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(7,r1)
	mov	r14,r1
	add	#-48,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	bra	.L1144
	nop
	.align 1
.L1145:
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	shll2	r1
	shll2	r1
	add	r1,r1
	shll2	r1
	extu.b	r1,r2
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	shlr	r1
	extu.b	r1,r1
	add	r2,r1
	extu.b	r1,r3
	mov	r14,r1
	add	#-48,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-48,r2
	mov	r1,r7
	add	#1,r7
	mov.l	r7,@(60,r2)
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	r3,r2
	mov	r2,r0
	mov.b	r0,@(7,r1)
	mov	r14,r1
	add	#-48,r1
	mov	r14,r2
	add	#-48,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L1144:
	mov	r14,r1
	add	#-48,r1
	mov.l	@(56,r1),r2
	mov	#10,r1
	cmp/gt	r1,r2
	bf	.L1145
	mov	r14,r1
	mov.b	@(7,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r0
	add	#16,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_tkfat_lfnchecksum, .-_tkfat_lfnchecksum
	.section	.rodata
	.align 2
	.type	_tkfat_tuc52, @object
	.size	_tkfat_tuc52, 128
_tkfat_tuc52:
	.long	8364
	.long	129
	.long	8218
	.long	402
	.long	8222
	.long	8230
	.long	8224
	.long	8225
	.long	710
	.long	8240
	.long	352
	.long	8249
	.long	338
	.long	141
	.long	381
	.long	65535
	.long	144
	.long	8216
	.long	8217
	.long	8220
	.long	8221
	.long	8226
	.long	8211
	.long	8212
	.long	732
	.long	8482
	.long	353
	.long	8250
	.long	339
	.long	156
	.long	382
	.long	376
	.text
	.align 1
	.global	_tkfat_asc2ucs
	.type	_tkfat_asc2ucs, @function
_tkfat_asc2ucs:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L1152,r0
	mov.l	.L1152,r12
	add	r0,r12
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	mov.w	.L1153,r2
	and	r2,r1
	tst	r1,r1
	bf	.L1148
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	bra	.L1149
	nop
	.align 1
.L1148:
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	mov	#96,r2
	and	r2,r1
	tst	r1,r1
	bt	.L1150
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	bra	.L1149
	nop
	.align 1
.L1150:
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	mov	#31,r2
	and	r2,r1
	mov.l	.L1154,r2
	add	r12,r2
	shll2	r1
	add	r2,r1
	mov.l	@r1,r1
.L1149:
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
	.align 1
.L1153:
	.short	128
.L1155:
	.align 2
.L1152:
	.long	_GLOBAL_OFFSET_TABLE_
.L1154:
	.long	_tkfat_tuc52@GOTOFF
	.size	_tkfat_asc2ucs, .-_tkfat_asc2ucs
	.align 1
	.global	_tkfat_ucs2asc
	.type	_tkfat_ucs2asc, @function
_tkfat_ucs2asc:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L1164,r0
	mov.l	.L1164,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov.w	.L1167,r2
	and	r2,r1
	tst	r1,r1
	bf	.L1157
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	bra	.L1158
	nop
	.align 1
.L1157:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	mov	#96,r2
	and	r2,r1
	tst	r1,r1
	bt	.L1159
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	bra	.L1158
	nop
	.align 1
.L1159:
	mov	r14,r1
	add	#-56,r1
	mov	#0,r2
	mov.l	r2,@(60,r1)
	bra	.L1160
	nop
	.align 1
.L1162:
	mov.l	.L1166,r1
	mov	r1,r2
	add	r12,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shll2	r1
	add	r2,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	cmp/eq	r1,r2
	bf	.L1161
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r2
	mov.w	.L1167,r1
	add	r2,r1
	bra	.L1158
	nop
	.align 1
.L1161:
	mov	r14,r1
	add	#-56,r1
	mov	r14,r2
	add	#-56,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L1160:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r2
	mov	#31,r1
	cmp/gt	r1,r2
	bf	.L1162
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	extu.b	r1,r1
.L1158:
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
	.align 1
.L1167:
	.short	128
.L1168:
	.align 2
.L1164:
	.long	_GLOBAL_OFFSET_TABLE_
.L1166:
	.long	_tkfat_tuc52@GOTOFF
	.size	_tkfat_ucs2asc, .-_tkfat_ucs2asc
	.align 1
	.global	_tkfat_emitUtf8
	.type	_tkfat_emitUtf8, @function
_tkfat_emitUtf8:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1176,r0
	mov.l	.L1176,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	r5,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#127,r1
	cmp/gt	r1,r2
	bt	.L1170
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	extu.b	r2,r2
	mov.b	r2,@r1
	bra	.L1171
	nop
	.align 1
.L1170:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov.w	.L1177,r1
	cmp/gt	r1,r2
	bt	.L1172
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	mov	r2,r4
	mov.l	.L1181,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	exts.b	r2,r2
	mov	#31,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-64,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	exts.b	r2,r2
	mov	#63,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-128,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	bra	.L1171
	nop
	.align 1
.L1172:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov.l	.L1179,r1
	cmp/gt	r1,r2
	bt	.L1173
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	mov	r2,r4
	mov.l	.L1180,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	exts.b	r2,r2
	mov	#15,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-32,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	mov	r2,r4
	mov.l	.L1181,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	exts.b	r2,r2
	mov	#63,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-128,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	exts.b	r2,r2
	mov	#63,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-128,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	bra	.L1171
	nop
	.align 1
.L1177:
	.short	2047
.L1182:
	.align 2
.L1176:
	.long	_GLOBAL_OFFSET_TABLE_
.L1181:
	.long	___ashiftrt_r4_6@GOTOFF
.L1179:
	.long	65535
.L1180:
	.long	___ashiftrt_r4_12@GOTOFF
	.align 1
.L1173:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov.l	.L1183,r1
	cmp/gt	r1,r2
	bt	.L1171
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r2
	mov	r14,r1
	add	#-56,r1
	mov	r2,r3
	add	#1,r3
	mov.l	r3,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	swap.w	r1,r1
	exts.w	r1,r1
	shar	r1
	shar	r1
	exts.b	r1,r1
	mov	#7,r3
	and	r3,r1
	exts.b	r1,r3
	mov	#-16,r1
	or	r3,r1
	exts.b	r1,r1
	extu.b	r1,r1
	mov.b	r1,@r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	mov	r2,r4
	mov.l	.L1184,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	exts.b	r2,r2
	mov	#63,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-128,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	mov	r2,r4
	mov.l	.L1185,r2
	add	r12,r2
	jsr	@r2
	nop
	mov	r4,r2
	exts.b	r2,r2
	mov	#63,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-128,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-56,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-56,r2
	mov.l	@(56,r2),r2
	exts.b	r2,r2
	mov	#63,r3
	and	r3,r2
	exts.b	r2,r3
	mov	#-128,r2
	or	r3,r2
	exts.b	r2,r2
	extu.b	r2,r2
	mov.b	r2,@r1
.L1171:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L1186:
	.align 2
.L1183:
	.long	2097151
.L1184:
	.long	___ashiftrt_r4_12@GOTOFF
.L1185:
	.long	___ashiftrt_r4_6@GOTOFF
	.size	_tkfat_emitUtf8, .-_tkfat_emitUtf8
	.align 1
	.global	_tkfat_lfn2utf8
	.type	_tkfat_lfn2utf8, @function
_tkfat_lfn2utf8:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1192,r0
	mov.l	.L1192,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(48,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r5,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(56,r1)
	bra	.L1188
	nop
	.align 1
.L1189:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(60,r2)
	mov.w	@r1,r1
	extu.w	r1,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	r2,@(52,r1)
	mov	r14,r8
	add	#-44,r8
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r2),r4
	mov.l	@(52,r1),r5
	mov.l	.L1193,r1
	bsrf	r1
.LPCS221:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
.L1188:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.w	@r1,r1
	extu.w	r1,r1
	tst	r1,r1
	bf	.L1189
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	#0,r2
	mov.b	r2,@r1
	mov	#0,r1
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L1194:
	.align 2
.L1192:
	.long	_GLOBAL_OFFSET_TABLE_
.L1193:
	.long	_tkfat_emitUtf8@PLT-(.LPCS221+2-.)
	.size	_tkfat_lfn2utf8, .-_tkfat_lfn2utf8
	.align 1
	.global	_tkfat_sfn2utf8
	.type	_tkfat_sfn2utf8, @function
_tkfat_sfn2utf8:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1220,r0
	mov.l	.L1220,r12
	add	r0,r12
	add	#-32,r15
	mov	r15,r14
	mov	r14,r1
	add	#-32,r1
	mov.l	r4,@(40,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	r5,@(36,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	r6,@(32,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(32,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	#32,r1
	cmp/hi	r1,r2
	bt	.L1196
	mov	#-1,r1
	bra	.L1197
	nop
	.align 1
.L1196:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(36,r1),r1
	mov	#32,r2
	and	r2,r1
	tst	r1,r1
	bt	.L1198
	mov	#-1,r1
	bra	.L1197
	nop
	.align 1
.L1198:
	mov	r14,r1
	add	#-32,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1199
	nop
	.align 1
.L1209:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#31,r1
	cmp/gt	r1,r2
	bf	.L1200
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#127,r1
	cmp/eq	r1,r2
	bf	.L1201
.L1200:
	mov	#-1,r1
	bra	.L1197
	nop
	.align 1
.L1201:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#32,r1
	cmp/eq	r1,r2
	bf	.L1202
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(44,r1)
	bra	.L1203
	nop
	.align 1
.L1206:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(40,r2),r2
	add	r2,r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	#32,r1
	cmp/hi	r1,r2
	bf	.L1204
	bra	.L1205
	nop
	.align 1
.L1204:
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(44,r2),r2
	add	#1,r2
	mov.l	r2,@(44,r1)
.L1203:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r1),r2
	mov	#7,r1
	cmp/gt	r1,r2
	bf	.L1206
.L1205:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(44,r1),r2
	mov	#7,r1
	cmp/gt	r1,r2
	bf	.L1202
	bra	.L1207
	nop
	.align 1
.L1202:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(36,r1),r1
	mov	#8,r2
	and	r2,r1
	tst	r1,r1
	bt	.L1208
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#64,r1
	cmp/gt	r1,r2
	bf	.L1208
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#90,r1
	cmp/gt	r1,r2
	bt	.L1208
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(48,r2),r2
	add	#32,r2
	mov.l	r2,@(48,r1)
.L1208:
	mov	r14,r8
	add	#-32,r8
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r4
	mov.l	.L1221,r1
	bsrf	r1
.LPCS222:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r8
	add	#-32,r8
	mov	r14,r2
	add	#-32,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r2),r4
	mov.l	@(48,r1),r5
	mov.l	.L1222,r2
	bsrf	r2
.LPCS223:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1199:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r2
	mov	#7,r1
	cmp/gt	r1,r2
	bt	.L1228
	bra	.L1209
	nop
.L1228:
.L1207:
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(40,r2),r2
	add	#8,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	#31,r1
	cmp/hi	r1,r2
	bt	.L1210
	mov	#-1,r1
	bra	.L1197
	nop
	.align 1
.L1210:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	#32,r1
	cmp/eq	r1,r2
	bf	.L1211
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	#0,r2
	mov.b	r2,@r1
	mov	#1,r1
	bra	.L1197
	nop
.L1223:
	.align 2
.L1220:
	.long	_GLOBAL_OFFSET_TABLE_
.L1221:
	.long	_tkfat_asc2ucs@PLT-(.LPCS222+2-.)
.L1222:
	.long	_tkfat_emitUtf8@PLT-(.LPCS223+2-.)
	.align 1
.L1211:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	#46,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-32,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1212
	nop
	.align 1
.L1218:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#31,r1
	cmp/gt	r1,r2
	bf	.L1213
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#127,r1
	cmp/eq	r1,r2
	bf	.L1214
.L1213:
	mov	#-1,r1
	bra	.L1197
	nop
	.align 1
.L1214:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#32,r1
	cmp/eq	r1,r2
	bf	.L1215
	bra	.L1216
	nop
	.align 1
.L1215:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(36,r1),r1
	mov	#16,r2
	and	r2,r1
	tst	r1,r1
	bt	.L1217
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#64,r1
	cmp/gt	r1,r2
	bf	.L1217
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r2
	mov	#90,r1
	cmp/gt	r1,r2
	bt	.L1217
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(48,r2),r2
	add	#32,r2
	mov.l	r2,@(48,r1)
.L1217:
	mov	r14,r8
	add	#-32,r8
	mov	r14,r1
	add	#-32,r1
	mov.l	@(48,r1),r4
	mov.l	.L1224,r1
	bsrf	r1
.LPCS224:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r8
	add	#-32,r8
	mov	r14,r2
	add	#-32,r2
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r2),r4
	mov.l	@(52,r1),r5
	mov.l	.L1225,r2
	bsrf	r2
.LPCS225:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-32,r1
	mov	r14,r2
	add	#-32,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1212:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(52,r1),r2
	mov	#2,r1
	cmp/gt	r1,r2
	bf	.L1218
.L1216:
	mov	r14,r1
	add	#-32,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-32,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	#0,r2
	mov.b	r2,@r1
	mov	#1,r1
.L1197:
	mov	r1,r0
	add	#32,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L1226:
	.align 2
.L1224:
	.long	_tkfat_asc2ucs@PLT-(.LPCS224+2-.)
.L1225:
	.long	_tkfat_emitUtf8@PLT-(.LPCS225+2-.)
	.size	_tkfat_sfn2utf8, .-_tkfat_sfn2utf8
	.align 1
	.global	_tkfat_lfn2bytes
	.type	_tkfat_lfn2bytes, @function
_tkfat_lfn2bytes:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1234,r0
	mov.l	.L1234,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r5,@(40,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	r6,@(36,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1230
	nop
	.align 1
.L1231:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#2,r3
	mov.l	r3,@(60,r2)
	mov.w	@r1,r1
	extu.w	r1,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	r2,@(48,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r8
	mov	r14,r1
	add	#-36,r1
	mov	r8,r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r4
	mov.l	.L1235,r1
	bsrf	r1
.LPCS226:
	nop
	mov	r0,r1
	extu.b	r1,r1
	mov.b	r1,@r8
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1230:
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(52,r2),r2
	mov.l	@(40,r1),r1
	cmp/ge	r1,r2
	bf	.L1231
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	#0,r2
	mov.b	r2,@r1
	mov	#0,r1
	mov	r1,r0
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L1236:
	.align 2
.L1234:
	.long	_GLOBAL_OFFSET_TABLE_
.L1235:
	.long	_tkfat_ucs2asc@PLT-(.LPCS226+2-.)
	.size	_tkfat_lfn2bytes, .-_tkfat_lfn2bytes
	.section	.rodata
	.align 2
.LC33:
	.string	"TKFAT_WalkDirEntNext: Reject DirEnt\n"
	.text
	.align 1
	.global	_TKFAT_WalkDirEntNext
	.type	_TKFAT_WalkDirEntNext, @function
_TKFAT_WalkDirEntNext:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1295,r0
	mov.l	.L1295,r12
	add	r0,r12
	mov.w	.L1338,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L1337,r1
	mov.w	.L1338,r0
	add	r14,r0
	add	r0,r1
	mov.l	r4,@(32,r1)
	mov.w	.L1337,r1
	mov.w	.L1338,r2
	add	r14,r2
	add	r2,r1
	mov.l	r5,@(28,r1)
	mov.w	.L1337,r1
	mov.w	.L1338,r3
	add	r14,r3
	add	r3,r1
	mov.l	@(32,r1),r1
	tst	r1,r1
	bf	.L1238
	mov.w	.L1337,r1
	mov.w	.L1338,r7
	add	r14,r7
	add	r7,r1
	mov.w	.L1337,r2
	mov.w	.L1338,r0
	add	r14,r0
	add	r0,r2
	mov.l	@(28,r2),r2
	mov.l	@(32,r2),r2
	mov.l	r2,@(32,r1)
.L1238:
	mov.w	.L1343,r1
	add	r14,r1
	mov.w	.L1337,r2
	mov.w	.L1338,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(28,r2),r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(44,r1)
	mov.w	.L1337,r1
	mov.w	.L1338,r7
	add	r14,r7
	add	r7,r1
	mov.l	@(28,r1),r1
	mov.l	@(40,r1),r1
	cmp/pz	r1
	bf	.L1239
	mov.w	.L1337,r1
	mov.w	.L1338,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(28,r1),r1
	mov.l	@(40,r1),r1
	add	#1,r1
	bra	.L1240
	nop
	.align 1
.L1239:
	mov	#0,r1
.L1240:
	mov.w	.L1343,r2
	add	r14,r2
	mov.l	r1,@(40,r2)
	mov.w	.L1343,r1
	add	r14,r1
	mov.w	.L1318,r2
	add	r14,r2
	mov.l	r2,@(36,r1)
	mov.w	.L1343,r1
	add	r14,r1
	mov.w	.L1318,r2
	add	r14,r2
	mov.l	r2,@(32,r1)
	mov.w	.L1343,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1241
	nop
	.align 1
.L1242:
	mov.w	.L1330,r1
	mov.w	.L1338,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1343,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	add	r2,r1
	mov	#-1,r2
	mov.w	r2,@r1
	mov.w	.L1332,r1
	mov.w	.L1338,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1343,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	add	r2,r1
	mov	#-1,r2
	mov.w	r2,@r1
	mov.w	.L1343,r1
	add	r14,r1
	mov.w	.L1343,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1241:
	mov.w	.L1343,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov.w	.L1329,r1
	cmp/gt	r1,r2
	bf	.L1242
	mov.w	.L1330,r1
	mov.w	.L1338,r2
	add	r14,r2
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
	mov.w	.L1332,r1
	mov.w	.L1338,r3
	add	r14,r3
	add	r3,r1
	mov	#0,r2
	mov.w	r2,@r1
	mov.w	.L1343,r1
	add	r14,r1
	mov.w	.L1343,r2
	add	r14,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(52,r1)
	bra	.L1243
	nop
	.align 1
.L1292:
	mov.w	.L1343,r8
	add	r14,r8
	mov.w	.L1337,r1
	mov.w	.L1338,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L1343,r2
	add	r14,r2
	mov.w	.L1343,r1
	add	r14,r1
	mov.w	.L1343,r7
	add	r14,r7
	mov.l	@(36,r7),r7
	mov.l	r7,@r15
	mov.l	@(32,r3),r4
	mov.l	@(44,r2),r5
	mov.l	@(52,r1),r6
	mov	#0,r7
	mov.l	.L1342,r1
	bsrf	r1
.LPCS227:
	nop
	mov	r0,r1
	mov.l	r1,@(28,r8)
	mov.w	.L1343,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	cmp/pz	r1
	bt	.L1244
	bra	.L1245
	nop
	.align 1
.L1338:
	.short	2088
.L1337:
	.short	-2112
.L1343:
	.short	2024
.L1318:
	.short	2016
.L1330:
	.short	-648
.L1332:
	.short	-1488
.L1329:
	.short	287
.L1344:
	.align 2
.L1295:
	.long	_GLOBAL_OFFSET_TABLE_
.L1342:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS227+2-.)
	.align 1
.L1244:
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L1246
	bra	.L1247
	nop
	.align 1
.L1246:
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov.w	.L1347,r1
	cmp/eq	r1,r2
	bf	.L1248
	bra	.L1247
	nop
	.align 1
.L1248:
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#15,r1
	cmp/eq	r1,r2
	bt	.L1627
	bra	.L1249
	nop
.L1627:
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1357,r1
	add	r14,r1
	mov	#63,r3
	and	r3,r2
	mov.l	r2,@(28,r1)
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	cmp/pl	r1
	bf	.L1612
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(28,r1),r2
	mov	#20,r1
	cmp/gt	r1,r2
	bf	.L1614
.L1612:
	bra	.L1250
	nop
.L1614:
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	r2,@(60,r1)
	mov.w	.L1357,r2
	add	r14,r2
	mov.w	.L1357,r1
	add	r14,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/eq	r1,r2
	bt	.L1251
	mov.w	.L1357,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1252
	nop
	.align 1
.L1357:
	.short	2024
.L1347:
	.short	229
	.align 1
.L1253:
	mov.w	.L1389,r1
	mov.w	.L1390,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	add	r2,r1
	mov	#-1,r2
	mov.w	r2,@r1
	mov.w	.L1394,r1
	add	r14,r1
	mov.w	.L1394,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1252:
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov.w	.L1364,r1
	cmp/gt	r1,r2
	bf	.L1253
	mov.w	.L1394,r1
	add	r14,r1
	mov.w	.L1394,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(56,r1)
.L1251:
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1394,r1
	add	r14,r1
	mov	#31,r3
	and	r3,r2
	mov.l	r2,@(28,r1)
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#-1,r2
	mov.w	.L1394,r3
	add	r14,r3
	mov	r2,r1
	add	r1,r1
	add	r2,r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(28,r3)
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#32,r1
	cmp/eq	r1,r2
	bt	.L1626
	bra	.L1254
	nop
.L1626:
	mov.w	.L1394,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1255
	nop
	.align 1
.L1256:
	mov.w	.L1394,r2
	add	r14,r2
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(28,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(32,r1),r2
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1377,r1
	bsrf	r1
.LPCS228:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1389,r1
	mov.w	.L1390,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1394,r1
	add	r14,r1
	mov.w	.L1394,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1255:
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#9,r1
	cmp/gt	r1,r2
	bf	.L1256
	mov.w	.L1394,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1257
	nop
	.align 1
.L1258:
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(32,r1),r2
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#14,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1388,r2
	bsrf	r2
.LPCS229:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1389,r1
	mov.w	.L1390,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1394,r1
	add	r14,r1
	mov.w	.L1394,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1257:
	mov.w	.L1394,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#11,r1
	cmp/gt	r1,r2
	bf	.L1258
	mov.w	.L1394,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1259
	nop
	.align 1
.L1389:
	.short	-648
.L1390:
	.short	2088
.L1394:
	.short	2024
.L1364:
	.short	287
.L1395:
	.align 2
.L1377:
	.long	_tkfat_asc2ucs@PLT-(.LPCS228+2-.)
.L1388:
	.long	_tkfat_asc2ucs@PLT-(.LPCS229+2-.)
	.align 1
.L1260:
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(32,r1),r2
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#28,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1400,r3
	bsrf	r3
.LPCS230:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1417,r1
	mov.w	.L1418,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1422,r1
	add	r14,r1
	mov.w	.L1422,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1259:
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#3,r1
	cmp/gt	r1,r2
	bf	.L1260
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r7
	and	r7,r1
	tst	r1,r1
	bt	.L1615
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	add	#26,r1
	mov.w	.L1417,r2
	mov.w	.L1418,r0
	add	r14,r0
	add	r0,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
.L1615:
	bra	.L1250
	nop
	.align 1
.L1254:
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L1615
	mov.w	.L1422,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1262
	nop
	.align 1
.L1263:
	mov.w	.L1422,r2
	add	r14,r2
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(28,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1422,r2
	add	r14,r2
	mov.l	@(32,r2),r2
	add	r2,r1
	add	#1,r1
	mov	r1,r4
	mov.l	.L1416,r1
	bsrf	r1
.LPCS231:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1417,r1
	mov.w	.L1418,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1422,r1
	add	r14,r1
	mov.w	.L1422,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1262:
	mov.w	.L1422,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#4,r1
	cmp/gt	r1,r2
	bf	.L1263
	mov.w	.L1422,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1264
	nop
	.align 1
.L1422:
	.short	2024
.L1417:
	.short	-648
.L1418:
	.short	2088
.L1423:
	.align 2
.L1400:
	.long	_tkfat_asc2ucs@PLT-(.LPCS230+2-.)
.L1416:
	.long	_tkfat_getWord@PLT-(.LPCS231+2-.)
	.align 1
.L1265:
	mov.w	.L1434,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1434,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1434,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1434,r2
	add	r14,r2
	add	#12,r1
	mov.l	@(32,r2),r2
	add	r2,r1
	add	#2,r1
	mov	r1,r4
	mov.l	.L1428,r2
	bsrf	r2
.LPCS232:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1429,r1
	mov.w	.L1430,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1434,r1
	add	r14,r1
	mov.w	.L1434,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1264:
	mov.w	.L1434,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#5,r1
	cmp/gt	r1,r2
	bf	.L1265
	mov.w	.L1434,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1266
	nop
	.align 1
.L1434:
	.short	2024
.L1429:
	.short	-648
.L1430:
	.short	2088
.L1435:
	.align 2
.L1428:
	.long	_tkfat_getWord@PLT-(.LPCS232+2-.)
	.align 1
.L1267:
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1458,r2
	add	r14,r2
	add	#28,r1
	mov.l	@(32,r2),r2
	add	r2,r1
	mov	r1,r4
	mov.l	.L1440,r3
	bsrf	r3
.LPCS233:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1448,r1
	mov.w	.L1449,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1458,r1
	add	r14,r1
	mov.w	.L1458,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1266:
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#1,r1
	cmp/gt	r1,r2
	bf	.L1267
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r7
	and	r7,r1
	tst	r1,r1
	bt	.L1250
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	add	#13,r1
	mov.w	.L1448,r2
	mov.w	.L1449,r0
	add	r14,r0
	add	r0,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
.L1250:
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1458,r1
	add	r14,r1
	mov	#63,r3
	and	r3,r2
	mov.l	r2,@(28,r1)
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(28,r1),r2
	mov	#32,r1
	cmp/gt	r1,r2
	bf	.L1616
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(28,r1),r2
	mov	#61,r1
	cmp/gt	r1,r2
	bf	.L1618
.L1616:
	bra	.L1268
	nop
.L1618:
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	r2,@(60,r1)
	mov.w	.L1458,r2
	add	r14,r2
	mov.w	.L1458,r1
	add	r14,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/eq	r1,r2
	bt	.L1269
	mov.w	.L1458,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1270
	nop
	.align 1
.L1458:
	.short	2024
.L1448:
	.short	-648
.L1449:
	.short	2088
.L1459:
	.align 2
.L1440:
	.long	_tkfat_getWord@PLT-(.LPCS233+2-.)
	.align 1
.L1271:
	mov.w	.L1491,r1
	mov.w	.L1492,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	add	r2,r1
	mov	#-1,r2
	mov.w	r2,@r1
	mov.w	.L1496,r1
	add	r14,r1
	mov.w	.L1496,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1270:
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov.w	.L1466,r1
	cmp/gt	r1,r2
	bf	.L1271
	mov.w	.L1496,r1
	add	r14,r1
	mov.w	.L1496,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(56,r1)
.L1269:
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1496,r1
	add	r14,r1
	mov	#31,r3
	and	r3,r2
	mov.l	r2,@(28,r1)
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#-1,r2
	mov.w	.L1496,r3
	add	r14,r3
	mov	r2,r1
	add	r1,r1
	add	r2,r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(28,r3)
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#32,r1
	cmp/eq	r1,r2
	bt	.L1624
	bra	.L1272
	nop
.L1624:
	mov.w	.L1496,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1273
	nop
	.align 1
.L1274:
	mov.w	.L1496,r2
	add	r14,r2
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(28,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(32,r1),r2
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1479,r1
	bsrf	r1
.LPCS234:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1491,r1
	mov.w	.L1492,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1496,r1
	add	r14,r1
	mov.w	.L1496,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1273:
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#9,r1
	cmp/gt	r1,r2
	bf	.L1274
	mov.w	.L1496,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1275
	nop
	.align 1
.L1276:
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(32,r1),r2
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#14,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1490,r2
	bsrf	r2
.LPCS235:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1491,r1
	mov.w	.L1492,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1496,r1
	add	r14,r1
	mov.w	.L1496,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1275:
	mov.w	.L1496,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#11,r1
	cmp/gt	r1,r2
	bf	.L1276
	mov.w	.L1496,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1277
	nop
	.align 1
.L1491:
	.short	-1488
.L1492:
	.short	2088
.L1496:
	.short	2024
.L1466:
	.short	287
.L1497:
	.align 2
.L1479:
	.long	_tkfat_asc2ucs@PLT-(.LPCS234+2-.)
.L1490:
	.long	_tkfat_asc2ucs@PLT-(.LPCS235+2-.)
	.align 1
.L1278:
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(32,r1),r2
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#28,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1502,r3
	bsrf	r3
.LPCS236:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1519,r1
	mov.w	.L1520,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1524,r1
	add	r14,r1
	mov.w	.L1524,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1277:
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#3,r1
	cmp/gt	r1,r2
	bf	.L1278
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r7
	and	r7,r1
	tst	r1,r1
	bt	.L1619
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	add	#26,r1
	mov.w	.L1519,r2
	mov.w	.L1520,r0
	add	r14,r0
	add	r0,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
	bra	.L1247
	nop
	.align 1
.L1272:
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L1625
.L1619:
	bra	.L1268
	nop
.L1625:
	mov.w	.L1524,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1280
	nop
	.align 1
.L1281:
	mov.w	.L1524,r2
	add	r14,r2
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(28,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1524,r2
	add	r14,r2
	mov.l	@(32,r2),r2
	add	r2,r1
	add	#1,r1
	mov	r1,r4
	mov.l	.L1518,r1
	bsrf	r1
.LPCS237:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1519,r1
	mov.w	.L1520,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1524,r1
	add	r14,r1
	mov.w	.L1524,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1280:
	mov.w	.L1524,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#4,r1
	cmp/gt	r1,r2
	bf	.L1281
	mov.w	.L1524,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1282
	nop
	.align 1
.L1524:
	.short	2024
.L1519:
	.short	-1488
.L1520:
	.short	2088
.L1525:
	.align 2
.L1502:
	.long	_tkfat_asc2ucs@PLT-(.LPCS236+2-.)
.L1518:
	.long	_tkfat_getWord@PLT-(.LPCS237+2-.)
	.align 1
.L1283:
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1548,r2
	add	r14,r2
	add	#12,r1
	mov.l	@(32,r2),r2
	add	r2,r1
	add	#2,r1
	mov	r1,r4
	mov.l	.L1530,r2
	bsrf	r2
.LPCS238:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1549,r1
	mov.w	.L1550,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1548,r1
	add	r14,r1
	mov.w	.L1548,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1282:
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#5,r1
	cmp/gt	r1,r2
	bf	.L1283
	mov.w	.L1548,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1284
	nop
	.align 1
.L1285:
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1548,r2
	add	r14,r2
	add	#28,r1
	mov.l	@(32,r2),r2
	add	r2,r1
	mov	r1,r4
	mov.l	.L1541,r3
	bsrf	r3
.LPCS239:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1549,r1
	mov.w	.L1550,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1548,r1
	add	r14,r1
	mov.w	.L1548,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1284:
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#1,r1
	cmp/gt	r1,r2
	bf	.L1285
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r7
	and	r7,r1
	tst	r1,r1
	bt	.L1268
	mov.w	.L1548,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	add	#13,r1
	mov.w	.L1549,r2
	mov.w	.L1550,r0
	add	r14,r0
	add	r0,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
	bra	.L1247
	nop
	.align 1
.L1268:
	bra	.L1247
	nop
	.align 1
.L1548:
	.short	2024
.L1549:
	.short	-1488
.L1550:
	.short	2088
.L1551:
	.align 2
.L1530:
	.long	_tkfat_getWord@PLT-(.LPCS238+2-.)
.L1541:
	.long	_tkfat_getWord@PLT-(.LPCS239+2-.)
	.align 1
.L1249:
	mov.w	.L1590,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.w	.L1590,r8
	add	r14,r8
	mov	r1,r4
	mov.l	.L1554,r1
	bsrf	r1
.LPCS240:
	nop
	mov	r0,r1
	mov.l	r1,@(24,r8)
	mov.w	.L1590,r2
	add	r14,r2
	mov.w	.L1590,r1
	add	r14,r1
	mov.l	@(24,r2),r2
	mov.l	@(60,r1),r1
	cmp/eq	r1,r2
	bt	.L1623
	bra	.L1286
	nop
.L1623:
	mov.w	.L1586,r1
	mov.w	.L1587,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(28,r1),r1
	mov	r1,r2
	add	#44,r2
	mov.w	.L1559,r1
	mov.w	.L1587,r3
	add	r14,r3
	add	r3,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L1561,r7
	bsrf	r7
.LPCS241:
	nop
	mov.w	.L1586,r1
	mov.w	.L1587,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(28,r1),r2
	mov.w	.L1564,r1
	add	r2,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(12,r1)
	mov.w	.L1580,r1
	mov.w	.L1587,r2
	add	r14,r2
	add	r2,r1
	mov.w	@r1,r1
	extu.w	r1,r1
	tst	r1,r1
	bt	.L1287
	mov.w	.L1580,r1
	mov.w	.L1587,r3
	add	r14,r3
	add	r3,r1
	mov.w	@r1,r1
	extu.w	r1,r2
	mov.l	.L1569,r1
	cmp/eq	r1,r2
	bt	.L1287
	mov.w	.L1586,r1
	mov.w	.L1587,r7
	add	r14,r7
	add	r7,r1
	mov.l	@(28,r1),r2
	mov.w	.L1579,r1
	add	r1,r2
	mov.w	.L1580,r1
	mov.w	.L1587,r0
	add	r14,r0
	add	r0,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L1575,r1
	bsrf	r1
.LPCS242:
	nop
.L1287:
	mov.w	.L1590,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#40,r1
	cmp/eq	r1,r2
	bf	.L1289
	mov.w	.L1586,r1
	mov.w	.L1587,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(28,r1),r2
	mov.w	.L1579,r1
	add	r1,r2
	mov.w	.L1580,r1
	mov.w	.L1587,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L1582,r1
	mov	r3,r4
	mov	r1,r5
	mov	r2,r6
	mov.l	.L1583,r3
	bsrf	r3
.LPCS243:
	nop
	bra	.L1289
	nop
	.align 1
.L1286:
	mov.w	.L1590,r1
	add	r14,r1
	mov.l	@(36,r1),r3
	mov.w	.L1590,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1586,r1
	mov.w	.L1587,r7
	add	r14,r7
	add	r7,r1
	mov.l	@(28,r1),r1
	add	#44,r1
	mov.w	.L1590,r8
	add	r14,r8
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L1589,r1
	bsrf	r1
.LPCS244:
	nop
	mov	r0,r1
	mov.l	r1,@(28,r8)
	mov.w	.L1590,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	cmp/pz	r1
	bt	.L1290
	mov.l	.L1591,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L1592,r2
	bsrf	r2
.LPCS245:
	nop
	bra	.L1247
	nop
	.align 1
.L1590:
	.short	2024
.L1586:
	.short	-2112
.L1587:
	.short	2088
.L1559:
	.short	-648
.L1564:
	.short	544
.L1580:
	.short	-1488
.L1579:
	.short	556
.L1582:
	.short	384
.L1593:
	.align 2
.L1554:
	.long	_tkfat_lfnchecksum@PLT-(.LPCS240+2-.)
.L1561:
	.long	_tkfat_lfn2utf8@PLT-(.LPCS241+2-.)
.L1569:
	.long	65535
.L1575:
	.long	_tkfat_lfn2utf8@PLT-(.LPCS242+2-.)
.L1583:
	.long	_tkfat_lfn2bytes@PLT-(.LPCS243+2-.)
.L1589:
	.long	_tkfat_sfn2utf8@PLT-(.LPCS244+2-.)
.L1591:
	.long	.LC33@GOTOFF
.L1592:
	.long	_printf@PLT-(.LPCS245+2-.)
	.align 1
.L1290:
	mov.w	.L1601,r1
	mov.w	.L1609,r3
	add	r14,r3
	add	r3,r1
	mov.l	@(28,r1),r2
	mov.w	.L1596,r1
	add	r2,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(12,r1)
.L1289:
	mov.w	.L1601,r1
	mov.w	.L1609,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(28,r1),r2
	mov.w	.L1607,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r2,r3
	mov	r1,r2
	mov	#32,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L1600,r3
	bsrf	r3
.LPCS246:
	nop
	mov.w	.L1601,r1
	mov.w	.L1609,r7
	add	r14,r7
	add	r7,r1
	mov.l	@(28,r1),r1
	mov.w	.L1607,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(40,r1)
	mov.w	.L1607,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L1293
	nop
	.align 1
.L1247:
	mov.w	.L1607,r1
	add	r14,r1
	mov.w	.L1607,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1243:
	mov.w	.L1607,r1
	add	r14,r1
	mov.l	@(52,r1),r2
	mov.l	.L1608,r1
	cmp/gt	r1,r2
	bt	.L1622
	bra	.L1292
	nop
.L1622:
.L1245:
	mov	#-1,r1
.L1293:
	mov	r1,r0
	mov.w	.L1609,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L1601:
	.short	-2112
.L1609:
	.short	2088
.L1596:
	.short	544
.L1607:
	.short	2024
.L1610:
	.align 2
.L1600:
	.long	_memcpy@PLT-(.LPCS246+2-.)
.L1608:
	.long	65535
	.size	_TKFAT_WalkDirEntNext, .-_TKFAT_WalkDirEntNext
	.align 1
	.global	_TKFAT_LookupDirEntName
	.type	_TKFAT_LookupDirEntName, @function
_TKFAT_LookupDirEntName:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L1684,r0
	mov.l	.L1684,r12
	add	r0,r12
	mov.w	.L1711,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L1710,r1
	mov.w	.L1711,r0
	add	r14,r0
	add	r0,r1
	mov.l	r4,@(40,r1)
	mov.w	.L1710,r1
	mov.w	.L1711,r2
	add	r14,r2
	add	r2,r1
	mov.l	r5,@(36,r1)
	mov.w	.L1710,r1
	mov.w	.L1711,r3
	add	r14,r3
	add	r3,r1
	mov.l	r6,@(32,r1)
	mov.w	.L1710,r1
	mov.w	.L1711,r0
	add	r14,r0
	add	r0,r1
	mov.l	r7,@(28,r1)
	mov.w	.L1717,r1
	add	r14,r1
	mov.w	.L1717,r2
	add	r14,r2
	mov.l	r2,@(44,r1)
	mov.w	.L1717,r1
	add	r14,r1
	mov.w	.L1717,r2
	add	r14,r2
	mov.l	r2,@(40,r1)
	mov.w	.L1717,r1
	add	r14,r1
	mov	#-1,r2
	mov.l	r2,@(56,r1)
	mov.w	.L1717,r8
	add	r14,r8
	mov.w	.L1710,r1
	mov.w	.L1711,r2
	add	r14,r2
	add	r2,r1
	mov.w	.L1702,r2
	mov.w	.L1711,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(28,r1),r4
	mov	r2,r5
	mov.l	.L1704,r1
	bsrf	r1
.LPCS247:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov.w	.L1717,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	cmp/pz	r1
	bt	.L2026
	bra	.L1629
	nop
.L2026:
	mov.w	.L1717,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1630
	nop
	.align 1
.L1637:
	mov.w	.L1717,r8
	add	r14,r8
	mov.w	.L1710,r1
	mov.w	.L1711,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L1710,r1
	mov.w	.L1711,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1717,r1
	add	r14,r1
	mov.w	.L1717,r7
	add	r14,r7
	mov.l	@(44,r7),r7
	mov.l	r7,@r15
	mov.l	@(40,r3),r4
	mov.l	@(36,r2),r5
	mov.l	@(52,r1),r6
	mov	#0,r7
	mov.l	.L1714,r2
	bsrf	r2
.LPCS248:
	nop
	mov	r0,r1
	mov.l	r1,@(36,r8)
	mov.w	.L1717,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	cmp/pz	r1
	bt	.L1631
	bra	.L1632
	nop
	.align 1
.L1631:
	mov.w	.L1717,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L1633
	bra	.L1634
	nop
	.align 1
.L1633:
	mov.w	.L1717,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov.w	.L1718,r1
	cmp/eq	r1,r2
	bf	.L1635
	bra	.L1634
	nop
	.align 1
.L1711:
	.short	2088
.L1710:
	.short	-2112
.L1717:
	.short	2024
.L1702:
	.short	-2068
.L1718:
	.short	229
.L1719:
	.align 2
.L1684:
	.long	_GLOBAL_OFFSET_TABLE_
.L1704:
	.long	_tkfat_name2sfn@PLT-(.LPCS247+2-.)
.L1714:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS248+2-.)
	.align 1
.L1635:
	mov.w	.L1762,r1
	add	r14,r1
	mov.l	@(44,r1),r2
	mov.w	.L1721,r1
	mov.w	.L1758,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov	#11,r6
	mov.l	.L1723,r1
	bsrf	r1
.LPCS249:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L1634
	mov.w	.L1757,r1
	mov.w	.L1758,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(32,r1),r1
	mov.w	.L1762,r2
	add	r14,r2
	mov.l	@r2,r3
	mov.l	r3,@r1
	mov.l	@(4,r2),r3
	mov.l	r3,@(4,r1)
	mov.l	@(8,r2),r3
	mov.l	r3,@(8,r1)
	mov.l	@(12,r2),r3
	mov.l	r3,@(12,r1)
	mov.l	@(16,r2),r3
	mov.l	r3,@(16,r1)
	mov.l	@(20,r2),r3
	mov.l	r3,@(20,r1)
	mov.l	@(24,r2),r3
	mov.l	r3,@(24,r1)
	mov.l	@(28,r2),r2
	mov.l	r2,@(28,r1)
	mov.w	.L1757,r1
	mov.w	.L1758,r3
	add	r14,r3
	add	r3,r1
	mov.l	@(32,r1),r1
	mov.w	.L1757,r2
	mov.w	.L1758,r0
	add	r14,r0
	add	r0,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(32,r1)
	mov.w	.L1757,r1
	mov.w	.L1758,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(32,r1),r1
	mov.w	.L1757,r2
	mov.w	.L1758,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(36,r1)
	mov.w	.L1757,r1
	mov.w	.L1758,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(32,r1),r1
	mov.w	.L1762,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(40,r1)
	mov.w	.L1757,r1
	mov.w	.L1758,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(32,r1),r1
	mov	r1,r2
	add	#44,r2
	mov.w	.L1757,r1
	mov.w	.L1758,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov.l	@(28,r1),r5
	mov.l	.L1742,r1
	bsrf	r1
.LPCS250:
	nop
	mov.w	.L1762,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L1682
	nop
	.align 1
.L1634:
	mov.w	.L1762,r1
	add	r14,r1
	mov.w	.L1762,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1630:
	mov.w	.L1762,r1
	add	r14,r1
	mov.l	@(52,r1),r2
	mov.l	.L1747,r1
	cmp/gt	r1,r2
	bt	.L2025
	bra	.L1637
	nop
.L2025:
.L1632:
	mov	#-1,r1
	bra	.L1682
	nop
	.align 1
.L1629:
	mov.w	.L1757,r1
	mov.w	.L1758,r2
	add	r14,r2
	add	r2,r1
	mov.w	.L1750,r2
	mov.w	.L1758,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(28,r1),r4
	mov	r2,r5
	mov.l	.L1752,r1
	bsrf	r1
.LPCS251:
	nop
	mov.w	.L1762,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L1638
	nop
	.align 1
.L1681:
	mov.w	.L1762,r8
	add	r14,r8
	mov.w	.L1757,r1
	mov.w	.L1758,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L1757,r1
	mov.w	.L1758,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1762,r1
	add	r14,r1
	mov.w	.L1762,r7
	add	r14,r7
	mov.l	@(44,r7),r7
	mov.l	r7,@r15
	mov.l	@(40,r3),r4
	mov.l	@(36,r2),r5
	mov.l	@(52,r1),r6
	mov	#0,r7
	mov.l	.L1761,r2
	bsrf	r2
.LPCS252:
	nop
	mov	r0,r1
	mov.l	r1,@(36,r8)
	mov.w	.L1762,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	cmp/pz	r1
	bt	.L1639
	bra	.L1640
	nop
	.align 1
.L1762:
	.short	2024
.L1721:
	.short	-2068
.L1758:
	.short	2088
.L1757:
	.short	-2112
.L1750:
	.short	-2056
.L1763:
	.align 2
.L1723:
	.long	_memcmp@PLT-(.LPCS249+2-.)
.L1742:
	.long	_strcpy@PLT-(.LPCS250+2-.)
.L1747:
	.long	65535
.L1752:
	.long	_tkfat_name2lfn@PLT-(.LPCS251+2-.)
.L1761:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS252+2-.)
	.align 1
.L1639:
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L1641
	bra	.L1642
	nop
	.align 1
.L1641:
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov.w	.L1766,r1
	cmp/eq	r1,r2
	bf	.L1643
	bra	.L1642
	nop
	.align 1
.L1643:
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#15,r1
	cmp/eq	r1,r2
	bt	.L2024
	bra	.L1644
	nop
.L2024:
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1776,r1
	add	r14,r1
	mov	#63,r3
	and	r3,r2
	mov.l	r2,@(36,r1)
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	cmp/pl	r1
	bf	.L2011
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(36,r1),r2
	mov	#20,r1
	cmp/gt	r1,r2
	bf	.L2013
.L2011:
	bra	.L1645
	nop
.L2013:
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	r2,@(60,r1)
	mov.w	.L1776,r2
	add	r14,r2
	mov.w	.L1776,r1
	add	r14,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/eq	r1,r2
	bt	.L1646
	mov.w	.L1776,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1647
	nop
	.align 1
.L1776:
	.short	2024
.L1766:
	.short	229
	.align 1
.L1648:
	mov.w	.L1808,r1
	mov.w	.L1809,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	add	r2,r1
	mov	#-1,r2
	mov.w	r2,@r1
	mov.w	.L1813,r1
	add	r14,r1
	mov.w	.L1813,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1647:
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov.w	.L1783,r1
	cmp/gt	r1,r2
	bf	.L1648
	mov.w	.L1813,r1
	add	r14,r1
	mov.w	.L1813,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(56,r1)
.L1646:
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1813,r1
	add	r14,r1
	mov	#31,r3
	and	r3,r2
	mov.l	r2,@(36,r1)
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#-1,r2
	mov.w	.L1813,r3
	add	r14,r3
	mov	r2,r1
	add	r1,r1
	add	r2,r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(36,r3)
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#32,r1
	cmp/eq	r1,r2
	bt	.L2023
	bra	.L1649
	nop
.L2023:
	mov.w	.L1813,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1650
	nop
	.align 1
.L1651:
	mov.w	.L1813,r2
	add	r14,r2
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(36,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(40,r1),r2
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1796,r1
	bsrf	r1
.LPCS253:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1808,r1
	mov.w	.L1809,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1813,r1
	add	r14,r1
	mov.w	.L1813,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1650:
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#9,r1
	cmp/gt	r1,r2
	bf	.L1651
	mov.w	.L1813,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1652
	nop
	.align 1
.L1653:
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(40,r1),r2
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#14,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1807,r2
	bsrf	r2
.LPCS254:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1808,r1
	mov.w	.L1809,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1813,r1
	add	r14,r1
	mov.w	.L1813,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1652:
	mov.w	.L1813,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#11,r1
	cmp/gt	r1,r2
	bf	.L1653
	mov.w	.L1813,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1654
	nop
	.align 1
.L1808:
	.short	-640
.L1809:
	.short	2088
.L1813:
	.short	2024
.L1783:
	.short	287
.L1814:
	.align 2
.L1796:
	.long	_tkfat_asc2ucs@PLT-(.LPCS253+2-.)
.L1807:
	.long	_tkfat_asc2ucs@PLT-(.LPCS254+2-.)
	.align 1
.L1655:
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(40,r1),r2
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#28,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1819,r3
	bsrf	r3
.LPCS255:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1836,r1
	mov.w	.L1837,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1841,r1
	add	r14,r1
	mov.w	.L1841,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1654:
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#3,r1
	cmp/gt	r1,r2
	bf	.L1655
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r0
	and	r0,r1
	tst	r1,r1
	bt	.L2014
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	add	#26,r1
	mov.w	.L1836,r2
	mov.w	.L1837,r3
	add	r14,r3
	add	r3,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
.L2014:
	bra	.L1645
	nop
	.align 1
.L1649:
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L2014
	mov.w	.L1841,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1657
	nop
	.align 1
.L1658:
	mov.w	.L1841,r2
	add	r14,r2
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(36,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1841,r2
	add	r14,r2
	mov.l	@(40,r2),r2
	add	r2,r1
	add	#1,r1
	mov	r1,r4
	mov.l	.L1835,r1
	bsrf	r1
.LPCS256:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1836,r1
	mov.w	.L1837,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1841,r1
	add	r14,r1
	mov.w	.L1841,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1657:
	mov.w	.L1841,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#4,r1
	cmp/gt	r1,r2
	bf	.L1658
	mov.w	.L1841,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1659
	nop
	.align 1
.L1841:
	.short	2024
.L1836:
	.short	-640
.L1837:
	.short	2088
.L1842:
	.align 2
.L1819:
	.long	_tkfat_asc2ucs@PLT-(.LPCS255+2-.)
.L1835:
	.long	_tkfat_getWord@PLT-(.LPCS256+2-.)
	.align 1
.L1660:
	mov.w	.L1853,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1853,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1853,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1853,r2
	add	r14,r2
	add	#12,r1
	mov.l	@(40,r2),r2
	add	r2,r1
	add	#2,r1
	mov	r1,r4
	mov.l	.L1847,r2
	bsrf	r2
.LPCS257:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1848,r1
	mov.w	.L1849,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1853,r1
	add	r14,r1
	mov.w	.L1853,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1659:
	mov.w	.L1853,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#5,r1
	cmp/gt	r1,r2
	bf	.L1660
	mov.w	.L1853,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1661
	nop
	.align 1
.L1853:
	.short	2024
.L1848:
	.short	-640
.L1849:
	.short	2088
.L1854:
	.align 2
.L1847:
	.long	_tkfat_getWord@PLT-(.LPCS257+2-.)
	.align 1
.L1662:
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1877,r2
	add	r14,r2
	add	#28,r1
	mov.l	@(40,r2),r2
	add	r2,r1
	mov	r1,r4
	mov.l	.L1859,r3
	bsrf	r3
.LPCS258:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1867,r1
	mov.w	.L1868,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1877,r1
	add	r14,r1
	mov.w	.L1877,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1661:
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#1,r1
	cmp/gt	r1,r2
	bf	.L1662
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r0
	and	r0,r1
	tst	r1,r1
	bt	.L1645
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	add	#13,r1
	mov.w	.L1867,r2
	mov.w	.L1868,r3
	add	r14,r3
	add	r3,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
.L1645:
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1877,r1
	add	r14,r1
	mov	#63,r0
	and	r0,r2
	mov.l	r2,@(36,r1)
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(36,r1),r2
	mov	#32,r1
	cmp/gt	r1,r2
	bf	.L2015
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(36,r1),r2
	mov	#61,r1
	cmp/gt	r1,r2
	bf	.L2017
.L2015:
	bra	.L1663
	nop
.L2017:
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@(13,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	r2,@(60,r1)
	mov.w	.L1877,r2
	add	r14,r2
	mov.w	.L1877,r1
	add	r14,r1
	mov.l	@(60,r2),r2
	mov.l	@(56,r1),r1
	cmp/eq	r1,r2
	bt	.L1664
	mov.w	.L1877,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1665
	nop
	.align 1
.L1877:
	.short	2024
.L1867:
	.short	-640
.L1868:
	.short	2088
.L1878:
	.align 2
.L1859:
	.long	_tkfat_getWord@PLT-(.LPCS258+2-.)
	.align 1
.L1666:
	mov.w	.L1910,r1
	mov.w	.L1911,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	add	r2,r1
	mov	#-1,r2
	mov.w	r2,@r1
	mov.w	.L1915,r1
	add	r14,r1
	mov.w	.L1915,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1665:
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov.w	.L1885,r1
	cmp/gt	r1,r2
	bf	.L1666
	mov.w	.L1915,r1
	add	r14,r1
	mov.w	.L1915,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(56,r1)
.L1664:
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r2
	mov.w	.L1915,r1
	add	r14,r1
	mov	#31,r3
	and	r3,r2
	mov.l	r2,@(36,r1)
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#-1,r2
	mov.w	.L1915,r3
	add	r14,r3
	mov	r2,r1
	add	r1,r1
	add	r2,r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(36,r3)
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r2
	mov	#32,r1
	cmp/eq	r1,r2
	bt	.L2021
	bra	.L1667
	nop
.L2021:
	mov.w	.L1915,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1668
	nop
	.align 1
.L1669:
	mov.w	.L1915,r2
	add	r14,r2
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(36,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(40,r1),r2
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#1,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1898,r1
	bsrf	r1
.LPCS259:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1910,r1
	mov.w	.L1911,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1915,r1
	add	r14,r1
	mov.w	.L1915,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1668:
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#9,r1
	cmp/gt	r1,r2
	bf	.L1669
	mov.w	.L1915,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1670
	nop
	.align 1
.L1671:
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(40,r1),r2
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#14,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1909,r2
	bsrf	r2
.LPCS260:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1910,r1
	mov.w	.L1911,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1915,r1
	add	r14,r1
	mov.w	.L1915,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1670:
	mov.w	.L1915,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#11,r1
	cmp/gt	r1,r2
	bf	.L1671
	mov.w	.L1915,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1672
	nop
	.align 1
.L1910:
	.short	-1480
.L1911:
	.short	2088
.L1915:
	.short	2024
.L1885:
	.short	287
.L1916:
	.align 2
.L1898:
	.long	_tkfat_asc2ucs@PLT-(.LPCS259+2-.)
.L1909:
	.long	_tkfat_asc2ucs@PLT-(.LPCS260+2-.)
	.align 1
.L1673:
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(40,r1),r2
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r2,r1
	add	#28,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	r1,r4
	mov.l	.L1921,r3
	bsrf	r3
.LPCS261:
	nop
	mov	r0,r1
	extu.w	r1,r2
	mov.w	.L1938,r1
	mov.w	.L1939,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1943,r1
	add	r14,r1
	mov.w	.L1943,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1672:
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#3,r1
	cmp/gt	r1,r2
	bf	.L1673
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r0
	and	r0,r1
	tst	r1,r1
	bt	.L2018
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	add	#26,r1
	mov.w	.L1938,r2
	mov.w	.L1939,r3
	add	r14,r3
	add	r3,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
	bra	.L1642
	nop
	.align 1
.L1667:
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@(12,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L2022
.L2018:
	bra	.L1663
	nop
.L2022:
	mov.w	.L1943,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1675
	nop
	.align 1
.L1676:
	mov.w	.L1943,r2
	add	r14,r2
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(36,r2),r2
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1943,r2
	add	r14,r2
	mov.l	@(40,r2),r2
	add	r2,r1
	add	#1,r1
	mov	r1,r4
	mov.l	.L1937,r1
	bsrf	r1
.LPCS262:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1938,r1
	mov.w	.L1939,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1943,r1
	add	r14,r1
	mov.w	.L1943,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1675:
	mov.w	.L1943,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#4,r1
	cmp/gt	r1,r2
	bf	.L1676
	mov.w	.L1943,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1677
	nop
	.align 1
.L1943:
	.short	2024
.L1938:
	.short	-1480
.L1939:
	.short	2088
.L1944:
	.align 2
.L1921:
	.long	_tkfat_asc2ucs@PLT-(.LPCS261+2-.)
.L1937:
	.long	_tkfat_getWord@PLT-(.LPCS262+2-.)
	.align 1
.L1678:
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1967,r2
	add	r14,r2
	add	#12,r1
	mov.l	@(40,r2),r2
	add	r2,r1
	add	#2,r1
	mov	r1,r4
	mov.l	.L1949,r2
	bsrf	r2
.LPCS263:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1968,r1
	mov.w	.L1969,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1967,r1
	add	r14,r1
	mov.w	.L1967,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1677:
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#5,r1
	cmp/gt	r1,r2
	bf	.L1678
	mov.w	.L1967,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	bra	.L1679
	nop
	.align 1
.L1680:
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r2,r8
	add	r1,r8
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	add	r1,r1
	mov.w	.L1967,r2
	add	r14,r2
	add	#28,r1
	mov.l	@(40,r2),r2
	add	r2,r1
	mov	r1,r4
	mov.l	.L1960,r3
	bsrf	r3
.LPCS264:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.w	.L1968,r1
	mov.w	.L1969,r3
	add	r14,r3
	add	r1,r3
	mov	r8,r1
	add	r1,r1
	add	r3,r1
	mov.w	r2,@r1
	mov.w	.L1967,r1
	add	r14,r1
	mov.w	.L1967,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	add	#1,r2
	mov.l	r2,@(48,r1)
.L1679:
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(48,r1),r2
	mov	#1,r1
	cmp/gt	r1,r2
	bf	.L1680
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r0
	and	r0,r1
	tst	r1,r1
	bt	.L1663
	mov.w	.L1967,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	add	#13,r1
	mov.w	.L1968,r2
	mov.w	.L1969,r3
	add	r14,r3
	add	r3,r2
	add	r1,r1
	add	r2,r1
	mov	#0,r2
	mov.w	r2,@r1
	bra	.L1642
	nop
	.align 1
.L1663:
	bra	.L1642
	nop
	.align 1
.L1967:
	.short	2024
.L1968:
	.short	-1480
.L1969:
	.short	2088
.L1970:
	.align 2
.L1949:
	.long	_tkfat_getWord@PLT-(.LPCS263+2-.)
.L1960:
	.long	_tkfat_getWord@PLT-(.LPCS264+2-.)
	.align 1
.L1644:
	mov.w	.L2004,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	mov.w	.L2004,r8
	add	r14,r8
	mov	r1,r4
	mov.l	.L1973,r1
	bsrf	r1
.LPCS265:
	nop
	mov	r0,r1
	mov.l	r1,@(32,r8)
	mov.w	.L2004,r2
	add	r14,r2
	mov.w	.L2004,r1
	add	r14,r1
	mov.l	@(32,r2),r2
	mov.l	@(60,r1),r1
	cmp/eq	r1,r2
	bf	.L1642
	mov.w	.L1976,r1
	mov.w	.L2006,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L1978,r1
	mov.w	.L2006,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L1980,r1
	bsrf	r1
.LPCS266:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L1642
	mov.w	.L1998,r1
	mov.w	.L2006,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(32,r1),r2
	mov.w	.L2004,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	mov	r2,r3
	mov	r1,r2
	mov	#32,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L1984,r3
	bsrf	r3
.LPCS267:
	nop
	mov.w	.L1998,r1
	mov.w	.L2006,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(32,r1),r1
	mov.w	.L1998,r2
	mov.w	.L2006,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(32,r1)
	mov.w	.L1998,r1
	mov.w	.L2006,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(32,r1),r1
	mov.w	.L1998,r2
	mov.w	.L2006,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(36,r1)
	mov.w	.L1998,r1
	mov.w	.L2006,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(32,r1),r1
	mov.w	.L2004,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(40,r1)
	mov.w	.L1998,r1
	mov.w	.L2006,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(32,r1),r1
	mov	r1,r2
	add	#44,r2
	mov.w	.L1998,r1
	mov.w	.L2006,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov.l	@(28,r1),r5
	mov.l	.L2000,r1
	bsrf	r1
.LPCS268:
	nop
	mov.w	.L2004,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L1682
	nop
	.align 1
.L1642:
	mov.w	.L2004,r1
	add	r14,r1
	mov.w	.L2004,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L1638:
	mov.w	.L2004,r1
	add	r14,r1
	mov.l	@(52,r1),r2
	mov.l	.L2005,r1
	cmp/gt	r1,r2
	bt	.L2020
	bra	.L1681
	nop
.L2020:
.L1640:
	mov	#-1,r1
.L1682:
	mov	r1,r0
	mov.w	.L2006,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L2004:
	.short	2024
.L1976:
	.short	-640
.L2006:
	.short	2088
.L1978:
	.short	-2056
.L1998:
	.short	-2112
.L2007:
	.align 2
.L1973:
	.long	_tkfat_lfnchecksum@PLT-(.LPCS265+2-.)
.L1980:
	.long	_tkfat_matchlfn@PLT-(.LPCS266+2-.)
.L1984:
	.long	_memcpy@PLT-(.LPCS267+2-.)
.L2000:
	.long	_strcpy@PLT-(.LPCS268+2-.)
.L2005:
	.long	65535
	.size	_TKFAT_LookupDirEntName, .-_TKFAT_LookupDirEntName
	.align 1
	.global	_TKFAT_CreateDirEntName
	.type	_TKFAT_CreateDirEntName, @function
_TKFAT_CreateDirEntName:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2061,r0
	mov.l	.L2061,r12
	add	r0,r12
	mov.w	.L2111,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L2110,r1
	mov.w	.L2111,r0
	add	r14,r0
	add	r0,r1
	mov.l	r4,@(32,r1)
	mov.w	.L2110,r1
	mov.w	.L2111,r2
	add	r14,r2
	add	r2,r1
	mov.l	r5,@(28,r1)
	mov.w	.L2110,r1
	mov.w	.L2111,r3
	add	r14,r3
	add	r3,r1
	mov.l	r6,@(24,r1)
	mov.w	.L2110,r1
	mov.w	.L2111,r6
	add	r14,r6
	add	r6,r1
	mov.l	r7,@(20,r1)
	mov.w	.L2115,r8
	add	r14,r8
	mov.w	.L2110,r1
	mov.w	.L2111,r7
	add	r14,r7
	add	r1,r7
	mov.w	.L2110,r1
	mov.w	.L2111,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2110,r1
	mov.w	.L2111,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2110,r1
	mov.w	.L2111,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(32,r7),r4
	mov.l	@(28,r3),r5
	mov.l	@(24,r2),r6
	mov.l	@(20,r1),r7
	mov.l	.L2080,r1
	bsrf	r1
.LPCS269:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov.w	.L2115,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	cmp/pz	r1
	bf	.L2028
	mov.w	.L2115,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L2059
	nop
	.align 1
.L2028:
	mov.w	.L2115,r1
	add	r14,r1
	mov.w	.L2086,r2
	add	r14,r2
	mov.l	r2,@(36,r1)
	mov.w	.L2115,r1
	add	r14,r1
	mov.w	.L2086,r2
	add	r14,r2
	mov.l	r2,@(32,r1)
	mov.w	.L2115,r1
	add	r14,r1
	mov	#-1,r2
	mov.l	r2,@(28,r1)
	mov.w	.L2115,r8
	add	r14,r8
	mov.w	.L2110,r1
	mov.w	.L2111,r2
	add	r14,r2
	add	r2,r1
	mov.w	.L2091,r2
	mov.w	.L2111,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(20,r1),r4
	mov	r2,r5
	mov.l	.L2093,r6
	bsrf	r6
.LPCS270:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov.w	.L2115,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	cmp/pz	r1
	bt	.L2353
	bra	.L2030
	nop
.L2353:
	mov.w	.L2115,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L2031
	nop
	.align 1
.L2036:
	mov.w	.L2115,r8
	add	r14,r8
	mov.w	.L2110,r1
	mov.w	.L2111,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2110,r1
	mov.w	.L2111,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2115,r1
	add	r14,r1
	mov.w	.L2115,r7
	add	r14,r7
	mov.l	@(36,r7),r7
	mov.l	r7,@r15
	mov.l	@(32,r3),r4
	mov.l	@(28,r2),r5
	mov.l	@(52,r1),r6
	mov	#0,r7
	mov.l	.L2103,r1
	bsrf	r1
.LPCS271:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov.w	.L2115,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	cmp/pz	r1
	bt	.L2032
	mov.w	.L2115,r1
	add	r14,r1
	mov.l	@(36,r1),r4
	mov	#0,r5
	mov	#32,r6
	mov.l	.L2106,r2
	bsrf	r2
.LPCS272:
	nop
	mov.w	.L2115,r8
	add	r14,r8
	mov.w	.L2110,r1
	mov.w	.L2111,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2110,r1
	mov.w	.L2111,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2115,r1
	add	r14,r1
	mov.w	.L2115,r7
	add	r14,r7
	mov.l	@(36,r7),r7
	mov.l	r7,@r15
	mov.l	@(32,r3),r4
	mov.l	@(28,r2),r5
	mov.l	@(52,r1),r6
	mov	#1,r7
	mov.l	.L2114,r3
	bsrf	r3
.LPCS273:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov.w	.L2115,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	cmp/pz	r1
	bt	.L2032
	bra	.L2033
	nop
	.align 1
.L2111:
	.short	624
.L2110:
	.short	-640
.L2115:
	.short	560
.L2086:
	.short	548
.L2091:
	.short	-604
.L2116:
	.align 2
.L2061:
	.long	_GLOBAL_OFFSET_TABLE_
.L2080:
	.long	_TKFAT_LookupDirEntName@PLT-(.LPCS269+2-.)
.L2093:
	.long	_tkfat_name2sfn@PLT-(.LPCS270+2-.)
.L2103:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS271+2-.)
.L2106:
	.long	_memset@PLT-(.LPCS272+2-.)
.L2114:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS273+2-.)
	.align 1
.L2032:
	mov.w	.L2162,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L2034
	mov.w	.L2162,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov.w	.L2119,r1
	cmp/eq	r1,r2
	bf	.L2035
.L2034:
	mov.w	.L2162,r1
	add	r14,r1
	mov.l	@(36,r1),r4
	mov	#0,r5
	mov	#32,r6
	mov.l	.L2121,r7
	bsrf	r7
.LPCS274:
	nop
	mov.w	.L2162,r1
	add	r14,r1
	mov.l	@(36,r1),r2
	mov.w	.L2123,r1
	mov.w	.L2160,r0
	add	r14,r0
	add	r0,r1
	mov	r2,r4
	mov	r1,r5
	mov	#11,r6
	mov.l	.L2125,r1
	bsrf	r1
.LPCS275:
	nop
	mov.w	.L2157,r1
	mov.w	.L2160,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2157,r1
	mov.w	.L2160,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2162,r1
	add	r14,r1
	mov.w	.L2162,r7
	add	r14,r7
	mov.l	@(36,r7),r7
	mov.l	r7,@r15
	mov.l	@(32,r3),r4
	mov.l	@(28,r2),r5
	mov.l	@(52,r1),r6
	mov	#1,r7
	mov.l	.L2132,r2
	bsrf	r2
.LPCS276:
	nop
	mov.w	.L2157,r1
	mov.w	.L2160,r3
	add	r14,r3
	add	r3,r1
	mov.l	@(24,r1),r1
	mov.w	.L2135,r2
	add	r14,r2
	mov.l	@r2,r3
	mov.l	r3,@r1
	mov.l	@(4,r2),r3
	mov.l	r3,@(4,r1)
	mov.l	@(8,r2),r3
	mov.l	r3,@(8,r1)
	mov.l	@(12,r2),r3
	mov.l	r3,@(12,r1)
	mov.l	@(16,r2),r3
	mov.l	r3,@(16,r1)
	mov.l	@(20,r2),r3
	mov.l	r3,@(20,r1)
	mov.l	@(24,r2),r3
	mov.l	r3,@(24,r1)
	mov.l	@(28,r2),r2
	mov.l	r2,@(28,r1)
	mov.w	.L2157,r1
	mov.w	.L2160,r6
	add	r14,r6
	add	r6,r1
	mov.l	@(24,r1),r1
	mov.w	.L2157,r2
	mov.w	.L2160,r7
	add	r14,r7
	add	r7,r2
	mov.l	@(32,r2),r2
	mov.l	r2,@(32,r1)
	mov.w	.L2157,r1
	mov.w	.L2160,r0
	add	r14,r0
	add	r0,r1
	mov.l	@(24,r1),r1
	mov.w	.L2157,r2
	mov.w	.L2160,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(28,r2),r2
	mov.l	r2,@(36,r1)
	mov.w	.L2157,r1
	mov.w	.L2160,r6
	add	r14,r6
	add	r6,r1
	mov.l	@(24,r1),r1
	mov.w	.L2162,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(40,r1)
	mov.w	.L2157,r1
	mov.w	.L2160,r7
	add	r14,r7
	add	r7,r1
	mov.l	@(24,r1),r1
	mov	r1,r2
	add	#44,r2
	mov.w	.L2157,r1
	mov.w	.L2160,r0
	add	r14,r0
	add	r0,r1
	mov	r2,r4
	mov.l	@(20,r1),r5
	mov.l	.L2151,r1
	bsrf	r1
.LPCS277:
	nop
	mov.w	.L2162,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L2059
	nop
	.align 1
.L2035:
	mov.w	.L2162,r1
	add	r14,r1
	mov.w	.L2162,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L2031:
	mov.w	.L2162,r1
	add	r14,r1
	mov.l	@(52,r1),r2
	mov.l	.L2156,r1
	cmp/gt	r1,r2
	bt	.L2352
	bra	.L2036
	nop
.L2352:
.L2033:
	mov	#-1,r1
	bra	.L2059
	nop
	.align 1
.L2030:
	mov.w	.L2157,r1
	mov.w	.L2160,r2
	add	r14,r2
	add	r2,r1
	mov.w	.L2159,r2
	mov.w	.L2160,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(20,r1),r4
	mov	r2,r5
	mov.l	.L2161,r6
	bsrf	r6
.LPCS278:
	nop
	mov.w	.L2162,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(40,r1)
	bra	.L2037
	nop
	.align 1
.L2162:
	.short	560
.L2119:
	.short	229
.L2123:
	.short	-604
.L2160:
	.short	624
.L2157:
	.short	-640
.L2135:
	.short	548
.L2159:
	.short	-592
.L2163:
	.align 2
.L2121:
	.long	_memset@PLT-(.LPCS274+2-.)
.L2125:
	.long	_memcpy@PLT-(.LPCS275+2-.)
.L2132:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS276+2-.)
.L2151:
	.long	_strcpy@PLT-(.LPCS277+2-.)
.L2156:
	.long	65535
.L2161:
	.long	_tkfat_name2lfn@PLT-(.LPCS278+2-.)
	.align 1
.L2038:
	mov.w	.L2202,r1
	add	r14,r1
	mov.w	.L2202,r2
	add	r14,r2
	mov.l	@(40,r2),r2
	add	#1,r2
	mov.l	r2,@(40,r1)
.L2037:
	mov.w	.L2181,r1
	mov.w	.L2199,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2202,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r1
	extu.w	r1,r1
	tst	r1,r1
	bf	.L2038
	mov.w	.L2202,r1
	add	r14,r1
	mov.l	@(40,r1),r1
	mov	r1,r2
	add	#13,r2
	mov.l	.L2170,r1
	dmuls.l	r1,r2
	sts	mach,r1
	shar	r1
	shar	r1
	mov	r1,r3
	mov	#0,r1
	cmp/gt	r2,r1
	subc	r1,r1
	mov	r3,r2
	sub	r1,r2
	mov.w	.L2202,r1
	add	r14,r1
	add	#1,r2
	mov.l	r2,@(28,r1)
	mov.w	.L2202,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(60,r1)
	mov.w	.L2202,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L2039
	nop
	.align 1
.L2040:
	mov.w	.L2202,r1
	add	r14,r1
	mov.l	@(60,r1),r2
	mov	r2,r1
	shll8	r1
	shll2	r1
	sub	r2,r1
	shll2	r1
	add	r1,r2
	mov.w	.L2181,r1
	mov.w	.L2199,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2202,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	add	r1,r1
	add	r3,r1
	mov.w	@r1,r1
	extu.w	r1,r1
	add	r1,r2
	mov.w	.L2202,r1
	add	r14,r1
	add	#1,r2
	mov.l	r2,@(60,r1)
	mov.w	.L2202,r1
	add	r14,r1
	mov.w	.L2202,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L2039:
	mov.w	.L2181,r1
	mov.w	.L2199,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2202,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r1
	extu.w	r1,r1
	tst	r1,r1
	bf	.L2040
	mov.w	.L2198,r1
	mov.w	.L2199,r7
	add	r14,r7
	add	r7,r1
	mov	r1,r4
	mov	#0,r5
	mov	#11,r6
	mov.l	.L2186,r1
	bsrf	r1
.LPCS279:
	nop
	mov.w	.L2198,r1
	mov.w	.L2199,r2
	add	r14,r2
	add	r2,r1
	mov	#32,r2
	mov.b	r2,@r1
	mov.w	.L2198,r1
	mov.w	.L2199,r3
	add	r14,r3
	add	r3,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(1,r1)
	mov.w	.L2202,r1
	add	r14,r1
	mov.l	@(60,r1),r2
	mov.w	.L2198,r1
	mov.w	.L2199,r3
	add	r14,r3
	add	r3,r1
	add	#2,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L2194,r6
	bsrf	r6
.LPCS280:
	nop
	mov.w	.L2198,r1
	mov.w	.L2199,r7
	add	r14,r7
	add	r7,r1
	mov	#58,r2
	mov	r2,r0
	mov.b	r0,@(5,r1)
	mov.w	.L2202,r8
	add	r14,r8
	mov.w	.L2198,r1
	mov.w	.L2199,r2
	add	r14,r2
	add	r2,r1
	mov	r1,r4
	mov.l	.L2200,r3
	bsrf	r3
.LPCS281:
	nop
	mov	r0,r1
	mov.l	r1,@(24,r8)
	mov.w	.L2202,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	mov.w	.L2202,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L2041
	nop
	.align 1
.L2202:
	.short	560
.L2181:
	.short	-592
.L2199:
	.short	624
.L2198:
	.short	-604
.L2203:
	.align 2
.L2170:
	.long	1321528399
.L2186:
	.long	_memset@PLT-(.LPCS279+2-.)
.L2194:
	.long	_tkfat_setDWord@PLT-(.LPCS280+2-.)
.L2200:
	.long	_tkfat_lfnchecksum@PLT-(.LPCS281+2-.)
	.align 1
.L2048:
	mov.w	.L2229,r8
	add	r14,r8
	mov.w	.L2218,r1
	mov.w	.L2219,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2218,r1
	mov.w	.L2219,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2229,r1
	add	r14,r1
	mov.w	.L2229,r7
	add	r14,r7
	mov.l	@(36,r7),r7
	mov.l	r7,@r15
	mov.l	@(32,r3),r4
	mov.l	@(28,r2),r5
	mov.l	@(52,r1),r6
	mov	#0,r7
	mov.l	.L2211,r1
	bsrf	r1
.LPCS282:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov.w	.L2229,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	cmp/pz	r1
	bt	.L2042
	mov.w	.L2229,r1
	add	r14,r1
	mov.l	@(36,r1),r4
	mov	#0,r5
	mov	#32,r6
	mov.l	.L2214,r2
	bsrf	r2
.LPCS283:
	nop
	mov.w	.L2229,r8
	add	r14,r8
	mov.w	.L2218,r1
	mov.w	.L2219,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2218,r1
	mov.w	.L2219,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2229,r1
	add	r14,r1
	mov.w	.L2229,r7
	add	r14,r7
	mov.l	@(36,r7),r7
	mov.l	r7,@r15
	mov.l	@(32,r3),r4
	mov.l	@(28,r2),r5
	mov.l	@(52,r1),r6
	mov	#1,r7
	mov.l	.L2222,r3
	bsrf	r3
.LPCS284:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov.w	.L2229,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	cmp/pz	r1
	bt	.L2042
	bra	.L2043
	nop
	.align 1
.L2042:
	mov.w	.L2229,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L2044
	mov.w	.L2229,r1
	add	r14,r1
	mov.l	@(36,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r2
	mov.w	.L2226,r1
	cmp/eq	r1,r2
	bf	.L2045
.L2044:
	mov.w	.L2229,r2
	add	r14,r2
	mov.w	.L2229,r1
	add	r14,r1
	mov.l	@(52,r2),r2
	mov.l	@(56,r1),r1
	sub	r1,r2
	mov.w	.L2229,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	cmp/ge	r1,r2
	bf	.L2046
	bra	.L2043
	nop
	.align 1
.L2046:
	bra	.L2047
	nop
	.align 1
.L2229:
	.short	560
.L2218:
	.short	-640
.L2219:
	.short	624
.L2226:
	.short	229
.L2230:
	.align 2
.L2211:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS282+2-.)
.L2214:
	.long	_memset@PLT-(.LPCS283+2-.)
.L2222:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS284+2-.)
	.align 1
.L2045:
	mov.w	.L2260,r1
	add	r14,r1
	mov.w	.L2260,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	nop
.L2047:
	mov.w	.L2260,r1
	add	r14,r1
	mov.w	.L2260,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L2041:
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(52,r1),r2
	mov.l	.L2238,r1
	cmp/gt	r1,r2
	bt	.L2351
	bra	.L2048
	nop
.L2351:
.L2043:
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(52,r1),r2
	mov.l	.L2238,r1
	cmp/gt	r1,r2
	bt	.L2347
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	cmp/pz	r1
	bf	.L2347
	mov.w	.L2260,r2
	add	r14,r2
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(52,r2),r2
	mov.l	@(56,r1),r1
	sub	r1,r2
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(28,r1),r1
	cmp/ge	r1,r2
	bt	.L2350
.L2347:
	bra	.L2049
	nop
.L2350:
	mov.w	.L2260,r1
	add	r14,r1
	mov.w	.L2260,r2
	add	r14,r2
	mov.l	@(28,r2),r2
	add	#-1,r2
	mov.l	r2,@(20,r1)
	mov.w	.L2260,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	bra	.L2050
	nop
	.align 1
.L2058:
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(32,r1),r4
	mov	#0,r5
	mov	#32,r6
	mov.l	.L2247,r7
	bsrf	r7
.LPCS285:
	nop
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(20,r1),r1
	extu.b	r1,r2
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	extu.b	r1,r1
	mov	r2,r0
	sub	r1,r0
	mov	r0,r1
	extu.b	r1,r2
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	r2,@r1
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	tst	r1,r1
	bf	.L2051
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	@r1,r1
	extu.b	r1,r1
	mov	#64,r2
	or	r2,r1
	extu.b	r1,r2
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov.b	r2,@r1
.L2051:
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov	#15,r2
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(24,r1),r1
	extu.b	r1,r2
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(32,r1),r1
	mov	r2,r0
	mov.b	r0,@(13,r1)
	mov.w	.L2260,r2
	add	r14,r2
	mov.w	.L2260,r1
	add	r14,r1
	mov.l	@(20,r2),r2
	mov.l	@(52,r1),r1
	mov	r2,r3
	sub	r1,r3
	mov	r3,r1
	mov	r1,r2
	add	#-1,r2
	mov.w	.L2260,r3
	add	r14,r3
	mov	r2,r1
	add	r1,r1
	add	r2,r1
	shll2	r1
	add	r2,r1
	mov.l	r1,@(48,r3)
	mov.w	.L2260,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(44,r1)
	bra	.L2052
	nop
	.align 1
.L2260:
	.short	560
.L2261:
	.align 2
.L2238:
	.long	65535
.L2247:
	.long	_memset@PLT-(.LPCS285+2-.)
	.align 1
.L2053:
	mov.w	.L2283,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	add	r1,r1
	mov.w	.L2283,r2
	add	r14,r2
	mov.l	@(32,r2),r2
	add	r2,r1
	mov	r1,r3
	add	#1,r3
	mov.w	.L2283,r2
	add	r14,r2
	mov.w	.L2283,r1
	add	r14,r1
	mov.l	@(48,r2),r2
	mov.l	@(44,r1),r1
	add	r2,r1
	mov.w	.L2277,r2
	mov.w	.L2278,r6
	add	r14,r6
	add	r6,r2
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r1
	extu.w	r1,r1
	mov	r3,r4
	mov	r1,r5
	mov.l	.L2268,r7
	bsrf	r7
.LPCS286:
	nop
	mov.w	.L2283,r1
	add	r14,r1
	mov.w	.L2283,r2
	add	r14,r2
	mov.l	@(44,r2),r2
	add	#1,r2
	mov.l	r2,@(44,r1)
.L2052:
	mov.w	.L2283,r1
	add	r14,r1
	mov.l	@(44,r1),r2
	mov	#4,r1
	cmp/gt	r1,r2
	bf	.L2053
	mov.w	.L2283,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(44,r1)
	bra	.L2054
	nop
	.align 1
.L2055:
	mov.w	.L2283,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	add	r1,r1
	mov.w	.L2283,r2
	add	r14,r2
	add	#12,r1
	mov.l	@(32,r2),r2
	add	r2,r1
	mov	r1,r3
	add	#2,r3
	mov.w	.L2283,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#5,r2
	mov.w	.L2283,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	add	r2,r1
	mov.w	.L2277,r2
	mov.w	.L2278,r0
	add	r14,r0
	add	r0,r2
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r1
	extu.w	r1,r1
	mov	r3,r4
	mov	r1,r5
	mov.l	.L2279,r1
	bsrf	r1
.LPCS287:
	nop
	mov.w	.L2283,r1
	add	r14,r1
	mov.w	.L2283,r2
	add	r14,r2
	mov.l	@(44,r2),r2
	add	#1,r2
	mov.l	r2,@(44,r1)
.L2054:
	mov.w	.L2283,r1
	add	r14,r1
	mov.l	@(44,r1),r2
	mov	#5,r1
	cmp/gt	r1,r2
	bf	.L2055
	mov.w	.L2283,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(44,r1)
	bra	.L2056
	nop
	.align 1
.L2283:
	.short	560
.L2277:
	.short	-592
.L2278:
	.short	624
.L2284:
	.align 2
.L2268:
	.long	_tkfat_setWord@PLT-(.LPCS286+2-.)
.L2279:
	.long	_tkfat_setWord@PLT-(.LPCS287+2-.)
	.align 1
.L2057:
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	add	r1,r1
	mov.w	.L2341,r2
	add	r14,r2
	add	#28,r1
	mov.l	@(32,r2),r2
	mov	r2,r3
	add	r1,r3
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(48,r1),r1
	mov	r1,r2
	add	#11,r2
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(44,r1),r1
	add	r2,r1
	mov.w	.L2289,r2
	mov.w	.L2339,r6
	add	r14,r6
	add	r6,r2
	add	r1,r1
	add	r2,r1
	mov.w	@r1,r1
	extu.w	r1,r1
	mov	r3,r4
	mov	r1,r5
	mov.l	.L2291,r7
	bsrf	r7
.LPCS288:
	nop
	mov.w	.L2341,r1
	add	r14,r1
	mov.w	.L2341,r2
	add	r14,r2
	mov.l	@(44,r2),r2
	add	#1,r2
	mov.l	r2,@(44,r1)
.L2056:
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(44,r1),r2
	mov	#1,r1
	cmp/gt	r1,r2
	bf	.L2057
	mov.w	.L2341,r2
	add	r14,r2
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(56,r2),r2
	mov.l	@(52,r1),r1
	mov	r2,r7
	add	r1,r7
	mov.w	.L2338,r1
	mov.w	.L2339,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2338,r1
	mov.w	.L2339,r0
	add	r14,r0
	add	r0,r1
	mov.w	.L2341,r3
	add	r14,r3
	mov.l	@(32,r3),r3
	mov.l	r3,@r15
	mov.l	@(32,r2),r4
	mov.l	@(28,r1),r5
	mov	r7,r6
	mov	#1,r7
	mov.l	.L2302,r1
	bsrf	r1
.LPCS289:
	nop
	mov.w	.L2341,r1
	add	r14,r1
	mov.w	.L2341,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#1,r2
	mov.l	r2,@(52,r1)
.L2050:
	mov.w	.L2341,r2
	add	r14,r2
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(52,r2),r2
	mov.l	@(20,r1),r1
	cmp/ge	r1,r2
	bt	.L2349
	bra	.L2058
	nop
.L2349:
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(36,r1),r4
	mov	#0,r5
	mov	#32,r6
	mov.l	.L2308,r2
	bsrf	r2
.LPCS290:
	nop
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(36,r1),r2
	mov.w	.L2310,r1
	mov.w	.L2339,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov	#11,r6
	mov.l	.L2312,r7
	bsrf	r7
.LPCS291:
	nop
	mov.w	.L2341,r2
	add	r14,r2
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(56,r2),r2
	mov.l	@(52,r1),r1
	mov	r2,r7
	add	r1,r7
	mov.w	.L2338,r1
	mov.w	.L2339,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2338,r1
	mov.w	.L2339,r0
	add	r14,r0
	add	r0,r1
	mov.w	.L2341,r3
	add	r14,r3
	mov.l	@(36,r3),r3
	mov.l	r3,@r15
	mov.l	@(32,r2),r4
	mov.l	@(28,r1),r5
	mov	r7,r6
	mov	#1,r7
	mov.l	.L2320,r1
	bsrf	r1
.LPCS292:
	nop
	mov.w	.L2338,r1
	mov.w	.L2339,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(24,r1),r1
	mov.w	.L2323,r2
	add	r14,r2
	mov.l	@r2,r3
	mov.l	r3,@r1
	mov.l	@(4,r2),r3
	mov.l	r3,@(4,r1)
	mov.l	@(8,r2),r3
	mov.l	r3,@(8,r1)
	mov.l	@(12,r2),r3
	mov.l	r3,@(12,r1)
	mov.l	@(16,r2),r3
	mov.l	r3,@(16,r1)
	mov.l	@(20,r2),r3
	mov.l	r3,@(20,r1)
	mov.l	@(24,r2),r3
	mov.l	r3,@(24,r1)
	mov.l	@(28,r2),r2
	mov.l	r2,@(28,r1)
	mov.w	.L2338,r1
	mov.w	.L2339,r3
	add	r14,r3
	add	r3,r1
	mov.l	@(24,r1),r1
	mov.w	.L2338,r2
	mov.w	.L2339,r6
	add	r14,r6
	add	r6,r2
	mov.l	@(32,r2),r2
	mov.l	r2,@(32,r1)
	mov.w	.L2338,r1
	mov.w	.L2339,r7
	add	r14,r7
	add	r7,r1
	mov.l	@(24,r1),r1
	mov.w	.L2338,r2
	mov.w	.L2339,r0
	add	r14,r0
	add	r0,r2
	mov.l	@(28,r2),r2
	mov.l	r2,@(36,r1)
	mov.w	.L2341,r2
	add	r14,r2
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(56,r2),r2
	mov.l	@(52,r1),r1
	add	r1,r2
	mov.w	.L2338,r1
	mov.w	.L2339,r3
	add	r14,r3
	add	r3,r1
	mov.l	@(24,r1),r1
	mov.l	r2,@(40,r1)
	mov.w	.L2338,r1
	mov.w	.L2339,r6
	add	r14,r6
	add	r6,r1
	mov.l	@(24,r1),r1
	mov	r1,r2
	add	#44,r2
	mov.w	.L2338,r1
	mov.w	.L2339,r7
	add	r14,r7
	add	r7,r1
	mov	r2,r4
	mov.l	@(20,r1),r5
	mov.l	.L2340,r1
	bsrf	r1
.LPCS293:
	nop
	mov.w	.L2341,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L2059
	nop
	.align 1
.L2341:
	.short	560
.L2289:
	.short	-592
.L2339:
	.short	624
.L2338:
	.short	-640
.L2310:
	.short	-604
.L2323:
	.short	548
.L2342:
	.align 2
.L2291:
	.long	_tkfat_setWord@PLT-(.LPCS288+2-.)
.L2302:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS289+2-.)
.L2308:
	.long	_memset@PLT-(.LPCS290+2-.)
.L2312:
	.long	_memcpy@PLT-(.LPCS291+2-.)
.L2320:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS292+2-.)
.L2340:
	.long	_strcpy@PLT-(.LPCS293+2-.)
	.align 1
.L2049:
	mov	#-1,r1
.L2059:
	mov	r1,r0
	mov.w	.L2343,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L2343:
	.short	624
	.size	_TKFAT_CreateDirEntName, .-_TKFAT_CreateDirEntName
	.section	.rodata
	.align 2
.LC34:
	.string	"TKFAT_CreateDirEntPathR: Failed Recurse %s\n"
	.text
	.align 1
	.global	_TKFAT_CreateDirEntPathR
	.type	_TKFAT_CreateDirEntPathR, @function
_TKFAT_CreateDirEntPathR:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2364,r0
	mov.l	.L2364,r12
	add	r0,r12
	mov.w	.L2405,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L2395,r1
	mov.w	.L2405,r2
	add	r14,r2
	add	r2,r1
	mov.l	r4,@(4,r1)
	mov.w	.L2395,r1
	mov.w	.L2405,r3
	add	r14,r3
	add	r3,r1
	mov.l	r5,@(0,r1)
	mov.w	.L2375,r1
	mov.w	.L2405,r2
	add	r14,r2
	add	r2,r1
	mov.l	r6,@(60,r1)
	mov.w	.L2375,r1
	mov.w	.L2405,r3
	add	r14,r3
	add	r3,r1
	mov.l	r7,@(56,r1)
	mov.w	.L2407,r1
	add	r14,r1
	mov.w	.L2375,r2
	mov.w	.L2405,r6
	add	r14,r6
	add	r6,r2
	mov.l	@(56,r2),r2
	mov.l	r2,@(60,r1)
	mov.w	.L2407,r1
	add	r14,r1
	mov.w	.L2404,r2
	mov.w	.L2405,r7
	add	r14,r7
	add	r7,r2
	mov.l	r2,@(56,r1)
	bra	.L2355
	nop
	.align 1
.L2357:
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	mov.w	.L2407,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov.w	.L2407,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	mov.w	.L2407,r3
	add	r14,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(60,r3)
	mov.b	@r2,r2
	mov.b	r2,@r1
.L2355:
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L2356
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#47,r1
	cmp/eq	r1,r2
	bf	.L2357
.L2356:
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	mov.w	.L2407,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	#0,r2
	mov.b	r2,@r1
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#47,r1
	cmp/eq	r1,r2
	bf	.L2358
	mov.w	.L2407,r1
	add	r14,r1
	mov.w	.L2407,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L2358:
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bf	.L2436
	bra	.L2359
	nop
.L2436:
	mov.w	.L2407,r8
	add	r14,r8
	mov.w	.L2395,r1
	mov.w	.L2405,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2395,r1
	mov.w	.L2405,r3
	add	r14,r3
	add	r3,r1
	mov.w	.L2397,r3
	mov.w	.L2405,r7
	add	r14,r7
	add	r3,r7
	mov.w	.L2404,r3
	mov.w	.L2405,r6
	add	r14,r6
	add	r6,r3
	mov.l	@(4,r2),r4
	mov.l	@(0,r1),r5
	mov	r7,r6
	mov	r3,r7
	mov.l	.L2401,r1
	bsrf	r1
.LPCS294:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	cmp/pz	r1
	bt	.L2360
	mov.l	.L2403,r1
	add	r12,r1
	mov	r1,r2
	mov.w	.L2404,r1
	mov.w	.L2405,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2406,r6
	bsrf	r6
.LPCS295:
	nop
	mov.w	.L2407,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L2362
	nop
	.align 1
.L2405:
	.short	1352
.L2395:
	.short	-1344
.L2375:
	.short	-1408
.L2407:
	.short	1288
.L2404:
	.short	-1336
.L2397:
	.short	-1080
.L2408:
	.align 2
.L2364:
	.long	_GLOBAL_OFFSET_TABLE_
.L2401:
	.long	_TKFAT_CreateDirEntName@PLT-(.LPCS294+2-.)
.L2403:
	.long	.LC34@GOTOFF
.L2406:
	.long	_printf@PLT-(.LPCS295+2-.)
	.align 1
.L2360:
	mov.w	.L2412,r1
	mov.w	.L2433,r7
	add	r14,r7
	add	r7,r1
	mov	r1,r4
	mov.l	.L2411,r1
	bsrf	r1
.LPCS296:
	nop
	mov.w	.L2412,r1
	mov.w	.L2433,r2
	add	r14,r2
	add	r2,r1
	mov.l	@(36,r1),r7
	mov.w	.L2432,r8
	add	r14,r8
	mov.w	.L2425,r1
	mov.w	.L2433,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2427,r1
	mov.w	.L2433,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2432,r1
	add	r14,r1
	mov.l	@(4,r3),r4
	mov	r7,r5
	mov.l	@(60,r2),r6
	mov.l	@(60,r1),r7
	mov.l	.L2420,r3
	bsrf	r3
.LPCS297:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov.w	.L2432,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	bra	.L2362
	nop
	.align 1
.L2359:
	mov.w	.L2432,r8
	add	r14,r8
	mov.w	.L2425,r1
	mov.w	.L2433,r3
	add	r14,r3
	add	r1,r3
	mov.w	.L2425,r1
	mov.w	.L2433,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2427,r1
	mov.w	.L2433,r6
	add	r14,r6
	add	r6,r1
	mov.w	.L2429,r7
	mov.w	.L2433,r6
	add	r14,r6
	add	r6,r7
	mov.l	@(4,r3),r4
	mov.l	@(0,r2),r5
	mov.l	@(60,r1),r6
	mov.l	.L2431,r1
	bsrf	r1
.LPCS298:
	nop
	mov	r0,r1
	mov.l	r1,@(52,r8)
	mov.w	.L2432,r1
	add	r14,r1
	mov.l	@(52,r1),r1
.L2362:
	mov	r1,r0
	mov.w	.L2433,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L2412:
	.short	-1080
.L2433:
	.short	1352
.L2432:
	.short	1288
.L2425:
	.short	-1344
.L2427:
	.short	-1408
.L2429:
	.short	-1336
.L2434:
	.align 2
.L2411:
	.long	_TKFAT_SetupDirEntNewDirectory@PLT-(.LPCS296+2-.)
.L2420:
	.long	_TKFAT_CreateDirEntPathR@PLT-(.LPCS297+2-.)
.L2431:
	.long	_TKFAT_CreateDirEntName@PLT-(.LPCS298+2-.)
	.size	_TKFAT_CreateDirEntPathR, .-_TKFAT_CreateDirEntPathR
	.align 1
	.global	_TKFAT_CreateDirEntPath
	.type	_TKFAT_CreateDirEntPath, @function
_TKFAT_CreateDirEntPath:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2442,r0
	mov.l	.L2442,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r5,@(48,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r6,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov.b	@(9,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L2438
	mov	#1,r1
	bra	.L2439
	nop
	.align 1
.L2438:
	mov	#2,r1
.L2439:
	mov	r14,r2
	add	#-44,r2
	mov.l	r1,@(60,r2)
	mov	r14,r8
	add	#-44,r8
	mov	r14,r7
	add	#-44,r7
	mov	r14,r3
	add	#-44,r3
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r7),r4
	mov.l	@(60,r3),r5
	mov.l	@(48,r2),r6
	mov.l	@(44,r1),r7
	mov.l	.L2443,r1
	bsrf	r1
.LPCS299:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L2444:
	.align 2
.L2442:
	.long	_GLOBAL_OFFSET_TABLE_
.L2443:
	.long	_TKFAT_CreateDirEntPathR@PLT-(.LPCS299+2-.)
	.size	_TKFAT_CreateDirEntPath, .-_TKFAT_CreateDirEntPath
	.align 1
	.global	_TKFAT_UpdateDirEnt
	.type	_TKFAT_UpdateDirEnt, @function
_TKFAT_UpdateDirEnt:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2448,r0
	mov.l	.L2448,r12
	add	r0,r12
	add	#-12,r15
	mov	r15,r14
	mov	r14,r1
	add	#-52,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r1
	mov.l	@(32,r1),r7
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r1
	mov.l	@(36,r1),r3
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r1
	mov.l	@(40,r1),r2
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r1
	mov	r14,r8
	add	#-52,r8
	mov.l	r1,@r15
	mov	r7,r4
	mov	r3,r5
	mov	r2,r6
	mov	#1,r7
	mov.l	.L2449,r1
	bsrf	r1
.LPCS300:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	mov	r1,r0
	add	#12,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L2450:
	.align 2
.L2448:
	.long	_GLOBAL_OFFSET_TABLE_
.L2449:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS300+2-.)
	.size	_TKFAT_UpdateDirEnt, .-_TKFAT_UpdateDirEnt
	.align 1
	.global	_TKFAT_GetDirEntCluster
	.type	_TKFAT_GetDirEntCluster, @function
_TKFAT_GetDirEntCluster:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2454,r0
	mov.l	.L2454,r12
	add	r0,r12
	add	#-12,r15
	mov	r15,r14
	mov	r14,r1
	add	#-52,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r1
	add	#26,r1
	mov	r1,r4
	mov.l	.L2455,r1
	bsrf	r1
.LPCS301:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-52,r1
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(52,r1),r1
	add	#20,r1
	mov	r1,r4
	mov.l	.L2456,r1
	bsrf	r1
.LPCS302:
	nop
	mov	r0,r1
	mov	r1,r2
	mov	r14,r1
	add	#-52,r1
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r1
	mov	r1,r2
	shll16	r2
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	or	r2,r1
	mov	r1,r0
	add	#12,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2457:
	.align 2
.L2454:
	.long	_GLOBAL_OFFSET_TABLE_
.L2455:
	.long	_tkfat_getWord@PLT-(.LPCS301+2-.)
.L2456:
	.long	_tkfat_getWord@PLT-(.LPCS302+2-.)
	.size	_TKFAT_GetDirEntCluster, .-_TKFAT_GetDirEntCluster
	.align 1
	.global	_TKFAT_SetDirEntCluster
	.type	_TKFAT_SetDirEntCluster, @function
_TKFAT_SetDirEntCluster:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2461,r0
	mov.l	.L2461,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	r5,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#26,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	extu.w	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2462,r1
	bsrf	r1
.LPCS303:
	nop
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#20,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	shlr16	r1
	extu.w	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2463,r1
	bsrf	r1
.LPCS304:
	nop
	mov	#0,r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2464:
	.align 2
.L2461:
	.long	_GLOBAL_OFFSET_TABLE_
.L2462:
	.long	_tkfat_setWord@PLT-(.LPCS303+2-.)
.L2463:
	.long	_tkfat_setWord@PLT-(.LPCS304+2-.)
	.size	_TKFAT_SetDirEntCluster, .-_TKFAT_SetDirEntCluster
	.align 1
	.global	_TKFAT_GetDirEntSize
	.type	_TKFAT_GetDirEntSize, @function
_TKFAT_GetDirEntSize:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2468,r0
	mov.l	.L2468,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	add	#28,r1
	mov	r14,r8
	add	#-56,r8
	mov	r1,r4
	mov.l	.L2469,r1
	bsrf	r1
.LPCS305:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L2470:
	.align 2
.L2468:
	.long	_GLOBAL_OFFSET_TABLE_
.L2469:
	.long	_tkfat_getDWord@PLT-(.LPCS305+2-.)
	.size	_TKFAT_GetDirEntSize, .-_TKFAT_GetDirEntSize
	.align 1
	.global	_TKFAT_SetDirEntSize
	.type	_TKFAT_SetDirEntSize, @function
_TKFAT_SetDirEntSize:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2474,r0
	mov.l	.L2474,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	r5,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#28,r2
	mov	r14,r1
	add	#-56,r1
	mov	r2,r4
	mov.l	@(56,r1),r5
	mov.l	.L2475,r1
	bsrf	r1
.LPCS306:
	nop
	mov	#0,r1
	mov	r1,r0
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2476:
	.align 2
.L2474:
	.long	_GLOBAL_OFFSET_TABLE_
.L2475:
	.long	_tkfat_setDWord@PLT-(.LPCS306+2-.)
	.size	_TKFAT_SetDirEntSize, .-_TKFAT_SetDirEntSize
	.align 1
	.global	_TKFAT_ReadWriteDirEntFile
	.type	_TKFAT_ReadWriteDirEntFile, @function
_TKFAT_ReadWriteDirEntFile:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2487,r0
	mov.l	.L2487,r12
	add	r0,r12
	add	#-44,r15
	mov	r15,r14
	mov	r14,r1
	add	#-20,r1
	mov.l	r4,@(40,r1)
	mov	r14,r1
	add	#-20,r1
	mov.l	r5,@(36,r1)
	mov	r6,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	r7,@(28,r1)
	mov	r14,r1
	add	#12,r1
	mov.b	r2,@r1
	mov	r14,r1
	add	#12,r1
	mov.b	@r1,r1
	extu.b	r1,r1
	tst	r1,r1
	bf	.L2500
	bra	.L2478
	nop
.L2500:
	mov	r14,r1
	add	#28,r1
	mov	#0,r2
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov	r14,r8
	add	#-20,r8
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r4
	mov.l	.L2488,r1
	bsrf	r1
.LPCS307:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r8
	add	#-20,r8
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r4
	mov.l	.L2489,r1
	bsrf	r1
.LPCS308:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r1
	add	#-20,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bf	.L2479
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r1
	mov.l	@(32,r1),r1
	mov	r1,r4
	mov.l	.L2490,r1
	bsrf	r1
.LPCS309:
	nop
	mov	r0,r1
	mov	r14,r2
	add	#-20,r2
	mov.l	r1,@(60,r2)
	mov	r14,r2
	add	#-20,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r2),r4
	mov.l	@(60,r1),r5
	mov.l	.L2491,r1
	bsrf	r1
.LPCS310:
	nop
	mov	r14,r1
	add	#28,r1
	mov	#1,r2
	mov	r2,r0
	mov.b	r0,@(11,r1)
.L2479:
	mov.l	@(60,r14),r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(36,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(48,r1),r1
	cmp/hi	r1,r2
	bf	.L2480
	mov.l	@(60,r14),r2
	mov	r14,r1
	add	#-20,r1
	mov	r14,r3
	add	#-20,r3
	mov.l	@(36,r3),r3
	add	r3,r2
	mov.l	r2,@(48,r1)
	mov	r14,r2
	add	#-20,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r2),r4
	mov.l	@(48,r1),r5
	mov.l	.L2492,r1
	bsrf	r1
.LPCS311:
	nop
	mov	r14,r1
	add	#28,r1
	mov	#1,r2
	mov	r2,r0
	mov.b	r0,@(11,r1)
.L2480:
	mov	r14,r1
	add	#28,r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	tst	r1,r1
	bt	.L2481
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r4
	mov.l	.L2493,r1
	bsrf	r1
.LPCS312:
	nop
.L2481:
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r1
	mov.l	@(32,r1),r7
	mov	r14,r1
	add	#-20,r1
	mov.l	@(60,r1),r3
	mov	r14,r1
	add	#-20,r1
	mov.l	@(36,r1),r2
	mov	r14,r8
	add	#-20,r8
	mov	r14,r1
	add	#-20,r1
	mov.l	@(28,r1),r1
	mov.l	r1,@r15
	mov.l	@(60,r14),r1
	mov.l	r1,@(4,r15)
	mov	r7,r4
	mov	r3,r5
	mov	r2,r6
	mov	#1,r7
	mov.l	.L2494,r1
	bsrf	r1
.LPCS313:
	nop
	mov	r0,r1
	mov.l	r1,@(44,r8)
	mov	r14,r1
	add	#-20,r1
	mov.l	@(44,r1),r1
	bra	.L2482
	nop
	.align 1
.L2478:
	mov	r14,r8
	add	#-20,r8
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r4
	mov.l	.L2495,r1
	bsrf	r1
.LPCS314:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r8
	add	#-20,r8
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r4
	mov.l	.L2496,r1
	bsrf	r1
.LPCS315:
	nop
	mov	r0,r1
	mov.l	r1,@(48,r8)
	mov	r14,r2
	add	#-20,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(36,r2),r2
	mov.l	@(48,r1),r1
	cmp/hs	r1,r2
	bf	.L2483
	mov	#0,r1
	bra	.L2482
	nop
	.align 1
.L2483:
	mov	r14,r1
	add	#-20,r1
	mov.l	@(60,r14),r2
	mov.l	r2,@(52,r1)
	mov.l	@(60,r14),r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(36,r1),r1
	add	r1,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(48,r1),r1
	cmp/hi	r1,r2
	bf	.L2484
	mov	r14,r2
	add	#-20,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	@(48,r2),r2
	mov.l	@(36,r1),r1
	sub	r1,r2
	mov	r14,r1
	add	#-20,r1
	mov.l	r2,@(52,r1)
.L2484:
	mov	r14,r1
	add	#-20,r1
	mov.l	@(40,r1),r1
	mov.l	@(32,r1),r7
	mov	r14,r1
	add	#-20,r1
	mov.l	@(60,r1),r3
	mov	r14,r1
	add	#-20,r1
	mov.l	@(36,r1),r2
	mov	r14,r8
	add	#-20,r8
	mov	r14,r1
	add	#-20,r1
	mov.l	@(28,r1),r1
	mov.l	r1,@r15
	mov	r14,r1
	add	#-20,r1
	mov.l	@(52,r1),r1
	mov.l	r1,@(4,r15)
	mov	r7,r4
	mov	r3,r5
	mov	r2,r6
	mov	#0,r7
	mov.l	.L2497,r1
	bsrf	r1
.LPCS316:
	nop
	mov	r0,r1
	mov.l	r1,@(44,r8)
	mov	r14,r1
	add	#-20,r1
	mov.l	@(44,r1),r1
	cmp/pz	r1
	bt	.L2485
	mov	r14,r1
	add	#-20,r1
	mov.l	@(44,r1),r1
	bra	.L2482
	nop
	.align 1
.L2485:
	mov	r14,r1
	add	#-20,r1
	mov.l	@(52,r1),r1
.L2482:
	mov	r1,r0
	add	#44,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L2498:
	.align 2
.L2487:
	.long	_GLOBAL_OFFSET_TABLE_
.L2488:
	.long	_TKFAT_GetDirEntCluster@PLT-(.LPCS307+2-.)
.L2489:
	.long	_TKFAT_GetDirEntSize@PLT-(.LPCS308+2-.)
.L2490:
	.long	_TKFAT_AllocFreeCluster@PLT-(.LPCS309+2-.)
.L2491:
	.long	_TKFAT_SetDirEntCluster@PLT-(.LPCS310+2-.)
.L2492:
	.long	_TKFAT_SetDirEntSize@PLT-(.LPCS311+2-.)
.L2493:
	.long	_TKFAT_UpdateDirEnt@PLT-(.LPCS312+2-.)
.L2494:
	.long	_TKFAT_ReadWriteClusterOffset@PLT-(.LPCS313+2-.)
.L2495:
	.long	_TKFAT_GetDirEntCluster@PLT-(.LPCS314+2-.)
.L2496:
	.long	_TKFAT_GetDirEntSize@PLT-(.LPCS315+2-.)
.L2497:
	.long	_TKFAT_ReadWriteClusterOffset@PLT-(.LPCS316+2-.)
	.size	_TKFAT_ReadWriteDirEntFile, .-_TKFAT_ReadWriteDirEntFile
	.section	.rodata
	.align 2
.LC35:
	.string	".          "
	.align 2
.LC36:
	.string	"..         "
	.text
	.align 1
	.global	_TKFAT_SetupDirEntNewDirectory
	.type	_TKFAT_SetupDirEntNewDirectory, @function
_TKFAT_SetupDirEntNewDirectory:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2509,r0
	mov.l	.L2509,r12
	add	r0,r12
	add	#-52,r15
	mov	r15,r14
	mov	r14,r1
	add	#-12,r1
	mov.l	r4,@(16,r1)
	mov	r14,r8
	add	#-12,r8
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r4
	mov.l	.L2510,r1
	bsrf	r1
.LPCS317:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov	r14,r1
	add	#-12,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bt	.L2502
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	#16,r2
	and	r2,r1
	tst	r1,r1
	bt	.L2503
	mov	#0,r1
	bra	.L2507
	nop
	.align 1
.L2503:
	mov	#-1,r1
	bra	.L2507
	nop
	.align 1
.L2502:
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	#16,r2
	or	r2,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r1
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov	r14,r1
	add	#-12,r1
	mov.l	@(60,r1),r1
	tst	r1,r1
	bf	.L2505
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r1
	mov.l	@(32,r1),r1
	mov	r1,r4
	mov.l	.L2511,r1
	bsrf	r1
.LPCS318:
	nop
	mov	r0,r1
	mov	r14,r2
	add	#-12,r2
	mov.l	r1,@(60,r2)
	mov	r14,r2
	add	#-12,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r2),r4
	mov.l	@(60,r1),r5
	mov.l	.L2512,r2
	bsrf	r2
.LPCS319:
	nop
.L2505:
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r4
	mov.l	.L2513,r1
	bsrf	r1
.LPCS320:
	nop
	mov	r14,r1
	add	#-12,r1
	mov	r14,r2
	add	#8,r2
	mov.l	r2,@(52,r1)
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r4
	mov	#0,r5
	mov	#32,r6
	mov.l	.L2514,r2
	bsrf	r2
.LPCS321:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r2
	mov.l	.L2515,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#12,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L2516,r1
	bsrf	r1
.LPCS322:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	#16,r2
	or	r2,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#26,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(60,r1),r1
	extu.w	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2517,r1
	bsrf	r1
.LPCS323:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#20,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	extu.w	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2518,r2
	bsrf	r2
.LPCS324:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r1
	mov.l	@(32,r1),r3
	mov	r14,r1
	add	#-12,r1
	mov.l	@(60,r1),r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov.l	r1,@r15
	mov	r3,r4
	mov	r2,r5
	mov	#0,r6
	mov	#1,r7
	mov.l	.L2519,r1
	bsrf	r1
.LPCS325:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r1
	mov.l	@(36,r1),r2
	mov	r14,r1
	add	#-12,r1
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-12,r1
	mov.l	@(56,r1),r2
	mov	#1,r1
	cmp/hi	r1,r2
	bt	.L2506
	mov	r14,r1
	add	#-12,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
.L2506:
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r2
	mov.l	.L2520,r1
	add	r12,r1
	mov	r2,r3
	mov	r1,r2
	mov	#12,r1
	mov	r3,r4
	mov	r2,r5
	mov	r1,r6
	mov.l	.L2521,r2
	bsrf	r2
.LPCS326:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov.b	@(11,r1),r0
	mov	r0,r1
	extu.b	r1,r1
	mov	#16,r2
	or	r2,r1
	extu.b	r1,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov	r2,r0
	mov.b	r0,@(11,r1)
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#26,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(56,r1),r1
	extu.w	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2522,r1
	bsrf	r1
.LPCS327:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov	r1,r2
	add	#20,r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(56,r1),r1
	shlr16	r1
	extu.w	r1,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2523,r2
	bsrf	r2
.LPCS328:
	nop
	mov	r14,r1
	add	#-12,r1
	mov.l	@(16,r1),r1
	mov.l	@(32,r1),r3
	mov	r14,r1
	add	#-12,r1
	mov.l	@(60,r1),r2
	mov	r14,r1
	add	#-12,r1
	mov.l	@(52,r1),r1
	mov.l	r1,@r15
	mov	r3,r4
	mov	r2,r5
	mov	#1,r6
	mov	#1,r7
	mov.l	.L2524,r1
	bsrf	r1
.LPCS329:
	nop
	mov	#1,r1
.L2507:
	mov	r1,r0
	add	#52,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L2525:
	.align 2
.L2509:
	.long	_GLOBAL_OFFSET_TABLE_
.L2510:
	.long	_TKFAT_GetDirEntCluster@PLT-(.LPCS317+2-.)
.L2511:
	.long	_TKFAT_AllocFreeCluster@PLT-(.LPCS318+2-.)
.L2512:
	.long	_TKFAT_SetDirEntCluster@PLT-(.LPCS319+2-.)
.L2513:
	.long	_TKFAT_UpdateDirEnt@PLT-(.LPCS320+2-.)
.L2514:
	.long	_memset@PLT-(.LPCS321+2-.)
.L2515:
	.long	.LC35@GOTOFF
.L2516:
	.long	_memcpy@PLT-(.LPCS322+2-.)
.L2517:
	.long	_tkfat_setWord@PLT-(.LPCS323+2-.)
.L2518:
	.long	_tkfat_setWord@PLT-(.LPCS324+2-.)
.L2519:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS325+2-.)
.L2520:
	.long	.LC36@GOTOFF
.L2521:
	.long	_memcpy@PLT-(.LPCS326+2-.)
.L2522:
	.long	_tkfat_setWord@PLT-(.LPCS327+2-.)
.L2523:
	.long	_tkfat_setWord@PLT-(.LPCS328+2-.)
.L2524:
	.long	_TKFAT_ReadWriteDirEntOffset@PLT-(.LPCS329+2-.)
	.size	_TKFAT_SetupDirEntNewDirectory, .-_TKFAT_SetupDirEntNewDirectory
	.section	.rodata
	.align 2
.LC37:
	.string	"%s\n"
	.text
	.align 1
	.global	_TKFAT_ListDir
	.type	_TKFAT_ListDir, @function
_TKFAT_ListDir:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2533,r0
	mov.l	.L2533,r12
	add	r0,r12
	mov.w	.L2561,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L2553,r1
	mov.w	.L2561,r2
	add	r14,r2
	add	r2,r1
	mov.l	r4,@(8,r1)
	mov.w	.L2553,r1
	mov.w	.L2561,r3
	add	r14,r3
	add	r3,r1
	mov.l	r5,@(4,r1)
	mov.w	.L2558,r1
	add	r14,r1
	mov.w	.L2540,r2
	mov.w	.L2561,r3
	add	r14,r3
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov.w	.L2558,r1
	add	r14,r1
	mov.w	.L2543,r2
	mov.l	@(60,r1),r4
	mov	#0,r5
	mov	r2,r6
	mov.l	.L2544,r1
	bsrf	r1
.LPCS330:
	nop
	mov.w	.L2558,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L2553,r2
	mov.w	.L2561,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(8,r2),r2
	mov.l	r2,@(32,r1)
	mov.w	.L2558,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L2553,r2
	mov.w	.L2561,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(4,r2),r2
	mov.l	r2,@(36,r1)
	mov.w	.L2558,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov	#-1,r2
	mov.l	r2,@(40,r1)
.L2529:
	mov.w	.L2558,r8
	add	r14,r8
	mov.w	.L2553,r1
	mov.w	.L2561,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2558,r1
	add	r14,r1
	mov.l	@(8,r2),r4
	mov.l	@(60,r1),r5
	mov.l	.L2556,r1
	bsrf	r1
.LPCS331:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov.w	.L2558,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L2527
	bra	.L2531
	nop
	.align 1
.L2527:
	mov.w	.L2558,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov	r1,r2
	add	#44,r2
	mov.l	.L2559,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L2560,r2
	bsrf	r2
.LPCS332:
	nop
	bra	.L2529
	nop
	.align 1
.L2531:
	mov	#0,r1
	mov	r1,r0
	mov.w	.L2561,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L2561:
	.short	1084
.L2553:
	.short	-1088
.L2558:
	.short	1020
.L2540:
	.short	-1076
.L2543:
	.short	1068
.L2562:
	.align 2
.L2533:
	.long	_GLOBAL_OFFSET_TABLE_
.L2544:
	.long	_memset@PLT-(.LPCS330+2-.)
.L2556:
	.long	_TKFAT_WalkDirEntNext@PLT-(.LPCS331+2-.)
.L2559:
	.long	.LC37@GOTOFF
.L2560:
	.long	_printf@PLT-(.LPCS332+2-.)
	.size	_TKFAT_ListDir, .-_TKFAT_ListDir
	.align 1
	.global	_TK_BufGetLine
	.type	_TK_BufGetLine, @function
_TK_BufGetLine:
	mov.l	r14,@-r15
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(52,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r5,@(48,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	r6,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(56,r1)
	bra	.L2564
	nop
	.align 1
.L2568:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#13,r1
	cmp/eq	r1,r2
	bf	.L2565
	bra	.L2566
	nop
	.align 1
.L2565:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#10,r1
	cmp/eq	r1,r2
	bf	.L2567
	bra	.L2566
	nop
	.align 1
.L2567:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(56,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	mov	r14,r3
	add	#-44,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(60,r3)
	mov.b	@r2,r2
	mov.b	r2,@r1
.L2564:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L2566
	mov	r14,r2
	add	#-44,r2
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r2),r2
	mov.l	@(44,r1),r1
	cmp/hs	r1,r2
	bf	.L2568
.L2566:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov	#0,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#13,r1
	cmp/eq	r1,r2
	bf	.L2569
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L2569:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#10,r1
	cmp/eq	r1,r2
	bf	.L2570
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
.L2570:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
	.size	_TK_BufGetLine, .-_TK_BufGetLine
	.align 1
	.global	_TK_SplitLine
	.type	_TK_SplitLine, @function
_TK_SplitLine:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	mova	.L2598,r0
	mov.l	.L2598,r12
	add	r0,r12
	add	#-20,r15
	mov	r15,r14
	mov	r14,r1
	add	#-44,r1
	mov.l	r4,@(44,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	.L2599,r2
	add	r12,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	.L2600,r2
	add	r12,r2
	mov.l	r2,@(52,r1)
	bra	.L2573
	nop
	.align 1
.L2595:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r2
	mov	#32,r1
	cmp/gt	r1,r2
	bt	.L2574
	bra	.L2575
	nop
	.align 1
.L2577:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
.L2575:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L2576
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r2
	mov	#32,r1
	cmp/gt	r1,r2
	bf	.L2577
.L2576:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(52,r1)
	bra	.L2573
	nop
	.align 1
.L2574:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r2
	mov	#34,r1
	cmp/eq	r1,r2
	bt	.L2613
	bra	.L2578
	nop
.L2613:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(48,r1)
	bra	.L2579
	nop
	.align 1
.L2591:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r2
	mov	#92,r1
	cmp/eq	r1,r2
	bt	.L2612
	bra	.L2580
	nop
.L2612:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	mov	#92,r3
	mov	#24,r2
	sub	r3,r1
	cmp/hi	r2,r1
	bf	.L2611
	bra	.L2581
	nop
.L2611:
	mova	.L2583,r0
	shll2	r1
	mov.l	@(r0,r1),r1
	braf	r1
	nop
.L2584:
.L2601:
	.align 2
.L2598:
	.long	_GLOBAL_OFFSET_TABLE_
.L2599:
	.long	_ta.2419@GOTOFF
.L2600:
	.long	_tb.2420@GOTOFF
	.align 2
.L2583:
	.long	.L2582-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2585-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2586-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2581-.L2584
	.long	.L2587-.L2584
	.long	.L2581-.L2584
	.long	.L2588-.L2584
	.align 1
.L2585:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	#8,r2
	mov.b	r2,@r1
	bra	.L2589
	nop
	.align 1
.L2586:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	#10,r2
	mov.b	r2,@r1
	bra	.L2589
	nop
	.align 1
.L2587:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	#13,r2
	mov.b	r2,@r1
	bra	.L2589
	nop
	.align 1
.L2588:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	#9,r2
	mov.b	r2,@r1
	bra	.L2589
	nop
	.align 1
.L2582:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	add	#1,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	#92,r2
	mov.b	r2,@r1
	bra	.L2589
	nop
	.align 1
.L2581:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-44,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	mov.b	r2,@r1
	nop
.L2589:
	bra	.L2579
	nop
	.align 1
.L2580:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-44,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	mov.b	r2,@r1
.L2579:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L2590
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r2
	mov	#34,r1
	cmp/eq	r1,r2
	bt	.L2610
	bra	.L2591
	nop
.L2610:
.L2590:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	#0,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#4,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@r1
	bra	.L2573
	nop
	.align 1
.L2578:
	mov	r14,r1
	add	#-44,r1
	mov	r14,r2
	add	#-44,r2
	mov.l	@(52,r2),r2
	mov.l	r2,@(48,r1)
	bra	.L2592
	nop
	.align 1
.L2594:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@(56,r2),r2
	mov	r14,r3
	add	#-44,r3
	mov	r2,r7
	add	#1,r7
	mov.l	r7,@(56,r3)
	mov.b	@r2,r2
	mov.b	r2,@r1
.L2592:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L2593
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r2
	mov	#32,r1
	cmp/gt	r1,r2
	bt	.L2594
.L2593:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(52,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(52,r2)
	mov	#0,r2
	mov.b	r2,@r1
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#4,r3
	mov.l	r3,@(60,r2)
	mov	r14,r2
	add	#-44,r2
	mov.l	@(48,r2),r2
	mov.l	r2,@r1
.L2573:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(56,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L2609
	bra	.L2595
	nop
.L2609:
	mov	r14,r1
	add	#-44,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-44,r2
	mov	r1,r3
	add	#4,r3
	mov.l	r3,@(60,r2)
	mov	#0,r2
	mov.l	r2,@r1
	mov.l	.L2602,r1
	add	r12,r1
	mov	r1,r0
	add	#20,r14
	mov	r14,r15
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2603:
	.align 2
.L2602:
	.long	_ta.2419@GOTOFF
	.size	_TK_SplitLine, .-_TK_SplitLine
	.comm	_timer_ticks,4,4
	.comm	_tkfat_fsimg,4,4
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
	mov.l	.L2615,r1
	mov	r14,r2
	add	#-60,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L2616:
	.align 2
.L2615:
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
	nop
.L2618:
	mov.l	.L2619,r1
	mov.l	@r1,r1
	mov	#8,r2
	and	r2,r1
	tst	r1,r1
	bf	.L2618
	mov.l	.L2620,r1
	mov	r14,r2
	add	#-60,r2
	mov.l	@(60,r2),r2
	mov.l	r2,@r1
	add	#4,r14
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L2621:
	.align 2
.L2619:
	.long	-1412628216
.L2620:
	.long	-1412628220
	.size	_putc, .-_putc
	.align 1
	.global	_kbhit
	.type	_kbhit, @function
_kbhit:
	mov.l	r14,@-r15
	mov	r15,r14
	mov.l	.L2624,r1
	mov.l	@r1,r1
	mov	#1,r2
	and	r2,r1
	mov	r1,r0
	mov	r14,r15
	mov.l	@r15+,r14
	rts	
	nop
.L2625:
	.align 2
.L2624:
	.long	-1412628216
	.size	_kbhit, .-_kbhit
	.align 1
	.global	_getch
	.type	_getch, @function
_getch:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2631,r0
	mov.l	.L2631,r12
	add	r0,r12
	mov	r15,r14
	bra	.L2627
	nop
	.align 1
.L2628:
	mov.l	.L2632,r1
	bsrf	r1
.LPCS333:
	nop
.L2627:
	mov.l	.L2633,r1
	mov.l	@r1,r1
	mov	#1,r2
	and	r2,r1
	tst	r1,r1
	bt	.L2628
	mov.l	.L2634,r1
	mov.l	@r1,r1
	mov	r1,r0
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2635:
	.align 2
.L2631:
	.long	_GLOBAL_OFFSET_TABLE_
.L2632:
	.long	_sleep_0@PLT-(.LPCS333+2-.)
.L2633:
	.long	-1412628216
.L2634:
	.long	-1412628224
	.size	_getch, .-_getch
	.align 1
	.global	_puts
	.type	_puts, @function
_puts:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2640,r0
	mov.l	.L2640,r12
	add	r0,r12
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
	bra	.L2637
	nop
	.align 1
.L2638:
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
	mov.l	.L2641,r1
	bsrf	r1
.LPCS334:
	nop
.L2637:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bf	.L2638
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2642:
	.align 2
.L2640:
	.long	_GLOBAL_OFFSET_TABLE_
.L2641:
	.long	_putc@PLT-(.LPCS334+2-.)
	.size	_puts, .-_puts
	.section	.rodata
	.align 2
.LC38:
	.string	"\b \b"
	.text
	.align 1
	.global	_gets
	.type	_gets, @function
_gets:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2653,r0
	mov.l	.L2653,r12
	add	r0,r12
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
.L2651:
	mov	r14,r8
	add	#-52,r8
	mov.l	.L2654,r1
	bsrf	r1
.LPCS335:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r2
	mov	#8,r1
	cmp/eq	r1,r2
	bt	.L2644
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r2
	mov	#127,r1
	cmp/eq	r1,r2
	bf	.L2645
.L2644:
	mov	r14,r2
	add	#-52,r2
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r2),r2
	mov.l	@(52,r1),r1
	cmp/hi	r1,r2
	bf	.L2646
	mov.l	.L2655,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2656,r1
	bsrf	r1
.LPCS336:
	nop
	mov	r14,r1
	add	#-52,r1
	mov	r14,r2
	add	#-52,r2
	mov.l	@(60,r2),r2
	add	#-1,r2
	mov.l	r2,@(60,r1)
.L2646:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	mov	#0,r2
	mov.b	r2,@r1
	nop
	bra	.L2651
	nop
	.align 1
.L2645:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r2
	mov	#13,r1
	cmp/eq	r1,r2
	bf	.L2648
	mov	#10,r4
	mov.l	.L2657,r1
	bsrf	r1
.LPCS337:
	nop
	bra	.L2649
	nop
	.align 1
.L2648:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r2
	mov	#10,r1
	cmp/eq	r1,r2
	bf	.L2650
	mov	#10,r4
	mov.l	.L2658,r1
	bsrf	r1
.LPCS338:
	nop
	bra	.L2649
	nop
	.align 1
.L2650:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(56,r1),r4
	mov.l	.L2659,r1
	bsrf	r1
.LPCS339:
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
	bra	.L2651
	nop
	.align 1
.L2649:
	mov	r14,r1
	add	#-52,r1
	mov.l	@(60,r1),r1
	mov	#0,r2
	mov.b	r2,@r1
	add	#12,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
.L2660:
	.align 2
.L2653:
	.long	_GLOBAL_OFFSET_TABLE_
.L2654:
	.long	_getch@PLT-(.LPCS335+2-.)
.L2655:
	.long	.LC38@GOTOFF
.L2656:
	.long	_puts@PLT-(.LPCS336+2-.)
.L2657:
	.long	_putc@PLT-(.LPCS337+2-.)
.L2658:
	.long	_putc@PLT-(.LPCS338+2-.)
.L2659:
	.long	_putc@PLT-(.LPCS339+2-.)
	.size	_gets, .-_gets
	.align 1
	.global	_print_hex
	.type	_print_hex, @function
_print_hex:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2663,r0
	mov.l	.L2663,r12
	add	r0,r12
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov.l	.L2678,r1
	add	r12,r1
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
	mov.l	.L2665,r1
	bsrf	r1
.LPCS340:
	nop
	mov.l	.L2678,r1
	add	r12,r1
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
	mov.l	.L2667,r1
	bsrf	r1
.LPCS341:
	nop
	mov.l	.L2678,r1
	add	r12,r1
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
	mov.l	.L2669,r1
	bsrf	r1
.LPCS342:
	nop
	mov.l	.L2678,r1
	add	r12,r1
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
	mov.l	.L2671,r1
	bsrf	r1
.LPCS343:
	nop
	mov.l	.L2678,r1
	add	r12,r1
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
	mov.l	.L2673,r1
	bsrf	r1
.LPCS344:
	nop
	mov.l	.L2678,r1
	add	r12,r1
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
	mov.l	.L2675,r1
	bsrf	r1
.LPCS345:
	nop
	mov.l	.L2678,r1
	add	r12,r1
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
	mov.l	.L2677,r1
	bsrf	r1
.LPCS346:
	nop
	mov.l	.L2678,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2679,r1
	bsrf	r1
.LPCS347:
	nop
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2680:
	.align 2
.L2663:
	.long	_GLOBAL_OFFSET_TABLE_
.L2678:
	.long	_chrs.2483@GOTOFF
.L2665:
	.long	_putc@PLT-(.LPCS340+2-.)
.L2667:
	.long	_putc@PLT-(.LPCS341+2-.)
.L2669:
	.long	_putc@PLT-(.LPCS342+2-.)
.L2671:
	.long	_putc@PLT-(.LPCS343+2-.)
.L2673:
	.long	_putc@PLT-(.LPCS344+2-.)
.L2675:
	.long	_putc@PLT-(.LPCS345+2-.)
.L2677:
	.long	_putc@PLT-(.LPCS346+2-.)
.L2679:
	.long	_putc@PLT-(.LPCS347+2-.)
	.size	_print_hex, .-_print_hex
	.align 1
	.global	_print_hex_n
	.type	_print_hex_n, @function
_print_hex_n:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2691,r0
	mov.l	.L2691,r12
	add	r0,r12
	add	#-8,r15
	mov	r15,r14
	mov	r14,r1
	add	#-56,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	r5,@(56,r1)
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#7,r1
	cmp/gt	r1,r2
	bf	.L2682
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	shlr8	r1
	shlr2	r1
	shlr2	r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2693,r1
	bsrf	r1
.LPCS348:
	nop
.L2682:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#6,r1
	cmp/gt	r1,r2
	bf	.L2683
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	shlr8	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2695,r1
	bsrf	r1
.LPCS349:
	nop
.L2683:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#5,r1
	cmp/gt	r1,r2
	bf	.L2684
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	shlr2	r1
	shlr2	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2697,r1
	bsrf	r1
.LPCS350:
	nop
.L2684:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#4,r1
	cmp/gt	r1,r2
	bf	.L2685
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr16	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2699,r1
	bsrf	r1
.LPCS351:
	nop
.L2685:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#3,r1
	cmp/gt	r1,r2
	bf	.L2686
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr8	r1
	shlr2	r1
	shlr2	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2701,r1
	bsrf	r1
.LPCS352:
	nop
.L2686:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#2,r1
	cmp/gt	r1,r2
	bf	.L2687
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr8	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2703,r1
	bsrf	r1
.LPCS353:
	nop
.L2687:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r2
	mov	#1,r1
	cmp/gt	r1,r2
	bf	.L2688
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	shlr2	r1
	shlr2	r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2705,r1
	bsrf	r1
.LPCS354:
	nop
.L2688:
	mov	r14,r1
	add	#-56,r1
	mov.l	@(56,r1),r1
	cmp/pl	r1
	bf	.L2681
	mov.l	.L2706,r1
	add	r12,r1
	mov.l	@r1,r2
	mov	r14,r1
	add	#-56,r1
	mov.l	@(60,r1),r1
	mov	#15,r3
	and	r3,r1
	add	r2,r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2707,r1
	bsrf	r1
.LPCS355:
	nop
.L2681:
	add	#8,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2708:
	.align 2
.L2691:
	.long	_GLOBAL_OFFSET_TABLE_
.L2706:
	.long	_chrs.2488@GOTOFF
.L2693:
	.long	_putc@PLT-(.LPCS348+2-.)
.L2695:
	.long	_putc@PLT-(.LPCS349+2-.)
.L2697:
	.long	_putc@PLT-(.LPCS350+2-.)
.L2699:
	.long	_putc@PLT-(.LPCS351+2-.)
.L2701:
	.long	_putc@PLT-(.LPCS352+2-.)
.L2703:
	.long	_putc@PLT-(.LPCS353+2-.)
.L2705:
	.long	_putc@PLT-(.LPCS354+2-.)
.L2707:
	.long	_putc@PLT-(.LPCS355+2-.)
	.size	_print_hex_n, .-_print_hex_n
	.align 1
	.global	_print_decimal
	.type	_print_decimal, @function
_print_decimal:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2718,r0
	mov.l	.L2718,r12
	add	r0,r12
	mov.w	.L2732,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L2723,r1
	mov.w	.L2732,r2
	add	r14,r2
	add	r2,r1
	mov.l	r4,@(44,r1)
	mov.w	.L2748,r1
	add	r14,r1
	mov.w	.L2723,r2
	mov.w	.L2732,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(44,r2),r2
	mov.l	r2,@(56,r1)
	mov.w	.L2748,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(52,r1)
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L2710
	mov.w	.L2748,r1
	add	r14,r1
	mov.w	.L2748,r2
	add	r14,r2
	mov.l	@(56,r2),r2
	neg	r2,r2
	mov.l	r2,@(56,r1)
	mov.w	.L2748,r1
	add	r14,r1
	mov	#1,r2
	mov.l	r2,@(52,r1)
.L2710:
	mov.w	.L2748,r1
	add	r14,r1
	mov.w	.L2731,r2
	mov.w	.L2732,r3
	add	r14,r3
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bf	.L2711
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L2748,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	#48,r2
	mov.b	r2,@r1
	bra	.L2712
	nop
	.align 1
.L2711:
	bra	.L2712
	nop
	.align 1
.L2713:
	mov.w	.L2748,r7
	add	r14,r7
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L2744,r1
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
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L2748,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.w	.L2748,r2
	add	r14,r2
	mov.l	@(48,r2),r2
	extu.b	r2,r2
	add	#48,r2
	extu.b	r2,r2
	exts.b	r2,r2
	mov.b	r2,@r1
	mov.w	.L2748,r3
	add	r14,r3
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L2744,r1
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
.L2712:
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	cmp/pl	r1
	bt	.L2713
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	tst	r1,r1
	bt	.L2714
	mov.w	.L2748,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L2748,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov	#45,r2
	mov.b	r2,@r1
	bra	.L2715
	nop
	.align 1
.L2714:
	bra	.L2715
	nop
	.align 1
.L2732:
	.short	276
.L2723:
	.short	-320
.L2748:
	.short	212
.L2731:
	.short	-272
.L2749:
	.align 2
.L2718:
	.long	_GLOBAL_OFFSET_TABLE_
.L2744:
	.long	1717986919
	.align 1
.L2716:
	mov.w	.L2756,r1
	add	r14,r1
	mov.w	.L2756,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	add	#-1,r2
	mov.l	r2,@(60,r1)
	mov.w	.L2756,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2753,r3
	bsrf	r3
.LPCS356:
	nop
.L2715:
	mov.w	.L2754,r1
	mov.w	.L2757,r2
	add	r14,r2
	add	r2,r1
	mov.w	.L2756,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	cmp/hi	r1,r2
	bt	.L2716
	mov.w	.L2757,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
	.align 1
.L2756:
	.short	212
.L2754:
	.short	-272
.L2757:
	.short	276
.L2758:
	.align 2
.L2753:
	.long	_putc@PLT-(.LPCS356+2-.)
	.size	_print_decimal, .-_print_decimal
	.align 1
	.global	_print_decimal_n
	.type	_print_decimal_n, @function
_print_decimal_n:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2766,r0
	mov.l	.L2766,r12
	add	r0,r12
	mov.w	.L2805,r1
	sub	r1,r15
	mov	r15,r14
	mov.w	.L2784,r1
	mov.w	.L2805,r2
	add	r14,r2
	add	r2,r1
	mov.l	r4,@(40,r1)
	mov.w	.L2784,r1
	mov.w	.L2805,r3
	add	r14,r3
	add	r3,r1
	mov.l	r5,@(36,r1)
	mov.w	.L2804,r1
	add	r14,r1
	mov.w	.L2784,r2
	mov.w	.L2805,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(40,r2),r2
	mov.l	r2,@(56,r1)
	mov.w	.L2804,r1
	add	r14,r1
	mov	#0,r2
	mov.l	r2,@(48,r1)
	mov.w	.L2804,r1
	add	r14,r1
	mov.l	@(56,r1),r1
	cmp/pz	r1
	bt	.L2760
	mov.w	.L2804,r1
	add	r14,r1
	mov.w	.L2804,r2
	add	r14,r2
	mov.l	@(56,r2),r2
	neg	r2,r2
	mov.l	r2,@(56,r1)
	mov.w	.L2804,r1
	add	r14,r1
	mov	#1,r2
	mov.l	r2,@(48,r1)
.L2760:
	mov.w	.L2804,r1
	add	r14,r1
	mov.w	.L2802,r2
	mov.w	.L2805,r3
	add	r14,r3
	add	r3,r2
	mov.l	r2,@(60,r1)
	mov.w	.L2804,r1
	add	r14,r1
	mov.w	.L2784,r2
	mov.w	.L2805,r3
	add	r14,r3
	add	r3,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(52,r1)
	bra	.L2761
	nop
	.align 1
.L2762:
	mov.w	.L2804,r7
	add	r14,r7
	mov.w	.L2804,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L2794,r1
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
	mov.w	.L2804,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.w	.L2804,r2
	add	r14,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.w	.L2804,r2
	add	r14,r2
	mov.l	@(44,r2),r2
	extu.b	r2,r2
	add	#48,r2
	extu.b	r2,r2
	exts.b	r2,r2
	mov.b	r2,@r1
	mov.w	.L2804,r3
	add	r14,r3
	mov.w	.L2804,r1
	add	r14,r1
	mov.l	@(56,r1),r2
	mov.l	.L2794,r1
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
	mov.w	.L2804,r1
	add	r14,r1
	mov.w	.L2804,r2
	add	r14,r2
	mov.l	@(52,r2),r2
	add	#-1,r2
	mov.l	r2,@(52,r1)
.L2761:
	mov.w	.L2804,r1
	add	r14,r1
	mov.l	@(52,r1),r1
	cmp/pl	r1
	bt	.L2762
	bra	.L2763
	nop
	.align 1
.L2764:
	mov.w	.L2804,r1
	add	r14,r1
	mov.w	.L2804,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	add	#-1,r2
	mov.l	r2,@(60,r1)
	mov.w	.L2804,r1
	add	r14,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2801,r3
	bsrf	r3
.LPCS357:
	nop
.L2763:
	mov.w	.L2802,r1
	mov.w	.L2805,r2
	add	r14,r2
	add	r2,r1
	mov.w	.L2804,r2
	add	r14,r2
	mov.l	@(60,r2),r2
	cmp/hi	r1,r2
	bt	.L2764
	mov.w	.L2805,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
	.align 1
.L2805:
	.short	284
.L2784:
	.short	-320
.L2804:
	.short	220
.L2802:
	.short	-276
.L2806:
	.align 2
.L2766:
	.long	_GLOBAL_OFFSET_TABLE_
.L2794:
	.long	1717986919
.L2801:
	.long	_putc@PLT-(.LPCS357+2-.)
	.size	_print_decimal_n, .-_print_decimal_n
	.align 1
	.global	_printf
	.type	_printf, @function
_printf:
	mov.l	r7,@-r15
	mov.l	r6,@-r15
	mov.l	r5,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2828,r0
	mov.l	.L2828,r12
	add	r0,r12
	add	#-28,r15
	mov	r15,r14
	mov	r14,r1
	add	#-36,r1
	mov.l	r4,@(36,r1)
	mov	r14,r2
	add	#40,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	r2,@(40,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(36,r2),r2
	mov.l	r2,@(60,r1)
	bra	.L2808
	nop
	.align 1
.L2825:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#37,r1
	cmp/eq	r1,r2
	bt	.L2809
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	mov	r1,r4
	mov.l	.L2829,r0
	bsrf	r0
.LPCS358:
	nop
	bra	.L2808
	nop
	.align 1
.L2809:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	add	#1,r1
	mov.b	@r1,r2
	mov	#37,r1
	cmp/eq	r1,r2
	bf	.L2810
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	add	#2,r2
	mov.l	r2,@(60,r1)
	mov	#37,r4
	mov.l	.L2830,r1
	bsrf	r1
.LPCS359:
	nop
	bra	.L2808
	nop
	.align 1
.L2810:
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#48,r1
	cmp/eq	r1,r2
	bf	.L2811
	mov	r14,r1
	add	#12,r1
	mov	#48,r2
	mov	r2,r0
	mov.b	r0,@(7,r1)
	mov	r14,r1
	add	#-36,r1
	mov	r14,r2
	add	#-36,r2
	mov.l	@(60,r2),r2
	add	#1,r2
	mov.l	r2,@(60,r1)
	bra	.L2812
	nop
	.align 1
.L2811:
	mov	r14,r1
	add	#12,r1
	mov	#32,r2
	mov	r2,r0
	mov.b	r0,@(7,r1)
.L2812:
	mov	r14,r1
	add	#-36,r1
	mov	#0,r2
	mov.l	r2,@(56,r1)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#47,r1
	cmp/gt	r1,r2
	bf	.L2813
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#57,r1
	cmp/gt	r1,r2
	bt	.L2813
	bra	.L2814
	nop
	.align 1
.L2815:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r2
	mov	r2,r1
	shll2	r1
	add	r2,r1
	add	r1,r1
	mov	r1,r3
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r7
	add	#1,r7
	mov.l	r7,@(60,r2)
	mov.b	@r1,r1
	mov	r1,r2
	add	#-48,r2
	mov	r14,r1
	add	#-36,r1
	add	r3,r2
	mov.l	r2,@(56,r1)
.L2814:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#47,r1
	cmp/gt	r1,r2
	bf	.L2813
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r2
	mov	#57,r1
	cmp/gt	r1,r2
	bf	.L2815
.L2813:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov	r14,r2
	add	#-36,r2
	mov	r1,r3
	add	#1,r3
	mov.l	r3,@(60,r2)
	mov.b	@r1,r1
	mov	#88,r3
	mov	#27,r2
	sub	r3,r1
	cmp/hi	r2,r1
	bf	.L2841
	bra	.L2826
	nop
.L2841:
	mova	.L2818,r0
	shll2	r1
	mov.l	@(r0,r1),r1
	braf	r1
	nop
.L2819:
.L2831:
	.align 2
.L2828:
	.long	_GLOBAL_OFFSET_TABLE_
.L2829:
	.long	_putc@PLT-(.LPCS358+2-.)
.L2830:
	.long	_putc@PLT-(.LPCS359+2-.)
	.align 2
.L2818:
	.long	.L2817-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2820-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2821-.L2819
	.long	.L2826-.L2819
	.long	.L2826-.L2819
	.long	.L2822-.L2819
	.align 1
.L2820:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-36,r2
	mov.l	r3,@(40,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@r1,r1
	mov.l	r1,@(48,r2)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(56,r1),r1
	tst	r1,r1
	bt	.L2823
	mov	r14,r2
	add	#-36,r2
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r2),r4
	mov.l	@(56,r1),r5
	mov.l	.L2832,r1
	bsrf	r1
.LPCS360:
	nop
	bra	.L2808
	nop
	.align 1
.L2823:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r4
	mov.l	.L2833,r0
	bsrf	r0
.LPCS361:
	nop
	bra	.L2808
	nop
	.align 1
.L2817:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-36,r2
	mov.l	r3,@(40,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@r1,r1
	mov.l	r1,@(48,r2)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(48,r1),r2
	mov	r14,r1
	add	#-36,r1
	mov	r2,r4
	mov.l	@(56,r1),r5
	mov.l	.L2834,r1
	bsrf	r1
.LPCS362:
	nop
	bra	.L2808
	nop
	.align 1
.L2822:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-36,r2
	mov.l	r3,@(40,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@r1,r1
	mov.l	r1,@(44,r2)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r4
	mov.l	.L2835,r0
	bsrf	r0
.LPCS363:
	nop
	bra	.L2808
	nop
	.align 1
.L2821:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(40,r1),r1
	mov	r1,r3
	add	#4,r3
	mov	r14,r2
	add	#-36,r2
	mov.l	r3,@(40,r2)
	mov	r14,r2
	add	#-36,r2
	mov.l	@r1,r1
	mov.l	r1,@(44,r2)
	mov	r14,r1
	add	#-36,r1
	mov.l	@(44,r1),r1
	mov	r1,r4
	mov.l	.L2836,r1
	bsrf	r1
.LPCS364:
	nop
	bra	.L2808
	nop
	.align 1
.L2826:
	nop
.L2808:
	mov	r14,r1
	add	#-36,r1
	mov.l	@(60,r1),r1
	mov.b	@r1,r1
	tst	r1,r1
	bt	.L2840
	bra	.L2825
	nop
.L2840:
	add	#28,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	add	#12,r15
	rts	
	nop
.L2837:
	.align 2
.L2832:
	.long	_print_decimal_n@PLT-(.LPCS360+2-.)
.L2833:
	.long	_print_decimal@PLT-(.LPCS361+2-.)
.L2834:
	.long	_print_hex_n@PLT-(.LPCS362+2-.)
.L2835:
	.long	_puts@PLT-(.LPCS363+2-.)
.L2836:
	.long	_print_hex@PLT-(.LPCS364+2-.)
	.size	_printf, .-_printf
	.align 1
	.global	_malloc
	.type	_malloc, @function
_malloc:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2845,r0
	mov.l	.L2845,r12
	add	r0,r12
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r4
	mov.l	.L2846,r1
	bsrf	r1
.LPCS365:
	nop
	mov	r0,r1
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2847:
	.align 2
.L2845:
	.long	_GLOBAL_OFFSET_TABLE_
.L2846:
	.long	_TKMM_Malloc@PLT-(.LPCS365+2-.)
	.size	_malloc, .-_malloc
	.align 1
	.global	_free
	.type	_free, @function
_free:
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2851,r0
	mov.l	.L2851,r12
	add	r0,r12
	add	#-4,r15
	mov	r15,r14
	mov	r14,r1
	add	#-60,r1
	mov.l	r4,@(60,r1)
	mov	r14,r1
	add	#-60,r1
	mov.l	@(60,r1),r4
	mov.l	.L2852,r1
	bsrf	r1
.LPCS366:
	nop
	mov	r0,r1
	mov	r1,r0
	add	#4,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	rts	
	nop
.L2853:
	.align 2
.L2851:
	.long	_GLOBAL_OFFSET_TABLE_
.L2852:
	.long	_TKMM_Free@PLT-(.LPCS366+2-.)
	.size	_free, .-_free
	.section	.rodata
	.align 2
.LC39:
	.string	"tk_main: puts\n"
	.align 2
.LC40:
	.string	"$ "
	.align 2
.LC41:
	.string	"got: "
	.align 2
.LC42:
	.string	"\n"
	.align 2
.LC43:
	.string	"quit"
	.align 2
.LC44:
	.string	"ok, exiting main loop\n"
	.align 2
.LC45:
	.string	"time"
	.align 2
.LC46:
	.string	"timer ticks=%d\n"
	.align 2
.LC47:
	.string	"rtc"
	.align 2
.LC48:
	.string	"rtc ticks=%9d:%9d\n"
	.align 2
.LC49:
	.string	"die"
	.align 2
.LC50:
	.string	"ok, dying\n"
	.align 2
.LC51:
	.string	"malloc"
	.align 2
.LC52:
	.string	"got pointer A %p %p\n"
	.align 2
.LC53:
	.string	"got pointer B %p %p\n"
	.align 2
.LC54:
	.string	"readmbr"
	.align 2
.LC55:
	.string	"init device\n"
	.align 2
.LC56:
	.string	"reading MBR\n"
	.align 2
.LC57:
	.string	"Got 55AA\n"
	.align 2
.LC58:
	.string	"No 55AA\n"
	.align 2
.LC59:
	.string	"don't understand %s\n"
	.text
	.align 1
	.global	_tk_main
	.type	_tk_main, @function
_tk_main:
	mov.l	r8,@-r15
	mov.l	r12,@-r15
	mov.l	r14,@-r15
	sts.l	pr,@-r15
	mova	.L2869,r0
	mov.l	.L2869,r12
	add	r0,r12
	mov.w	.L2902,r1
	sub	r1,r15
	mov	r15,r14
	mov.l	.L2898,r1
	add	r12,r1
	mov.l	@r1,r1
	mov	#0,r2
	mov.l	r2,@r1
	mov	#105,r4
	mov.l	.L2872,r0
	bsrf	r0
.LPCS367:
	nop
	mov.l	.L2873,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2874,r1
	bsrf	r1
.LPCS368:
	nop
	mov.l	.L2875,r2
	bsrf	r2
.LPCS369:
	nop
.L2866:
	mov.l	.L2876,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2877,r3
	bsrf	r3
.LPCS370:
	nop
	mov.w	.L2901,r1
	mov.w	.L2902,r0
	add	r14,r0
	add	r0,r1
	mov	r1,r4
	mov.l	.L2880,r1
	bsrf	r1
.LPCS371:
	nop
	mov.l	.L2881,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2882,r2
	bsrf	r2
.LPCS372:
	nop
	mov.w	.L2901,r1
	mov.w	.L2902,r3
	add	r14,r3
	add	r3,r1
	mov	r1,r4
	mov.l	.L2885,r0
	bsrf	r0
.LPCS373:
	nop
	mov.l	.L2886,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2887,r1
	bsrf	r1
.LPCS374:
	nop
	mov.w	.L2901,r1
	mov.w	.L2902,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L2890,r1
	add	r12,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2891,r2
	bsrf	r2
.LPCS375:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L2855
	mov.l	.L2892,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2893,r3
	bsrf	r3
.LPCS376:
	nop
	bra	.L2867
	nop
	.align 1
.L2855:
	mov.w	.L2901,r1
	mov.w	.L2902,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L2896,r1
	add	r12,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2897,r1
	bsrf	r1
.LPCS377:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L2857
	mov.l	.L2898,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r2
	mov.l	.L2899,r1
	add	r12,r1
	mov	r1,r4
	mov	r2,r5
	mov.l	.L2900,r2
	bsrf	r2
.LPCS378:
	nop
	bra	.L2858
	nop
	.align 1
.L2857:
	mov.w	.L2901,r1
	mov.w	.L2902,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L2903,r1
	add	r12,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2904,r3
	bsrf	r3
.LPCS379:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L2859
	mov.l	.L2905,r1
	mov.l	@r1,r3
	mov.l	.L2906,r1
	mov.l	@r1,r2
	mov.l	.L2907,r1
	add	r12,r1
	mov	r1,r4
	mov	r3,r5
	mov	r2,r6
	mov.l	.L2908,r0
	bsrf	r0
.LPCS380:
	nop
	bra	.L2858
	nop
	.align 1
.L2902:
	.short	1032
.L2901:
	.short	-1032
.L2909:
	.align 2
.L2869:
	.long	_GLOBAL_OFFSET_TABLE_
.L2898:
	.long	_timer_ticks@GOT
.L2872:
	.long	_setGpioOutputs@PLT-(.LPCS367+2-.)
.L2873:
	.long	.LC39@GOTOFF
.L2874:
	.long	_puts@PLT-(.LPCS368+2-.)
.L2875:
	.long	_tkmm_init@PLT-(.LPCS369+2-.)
.L2876:
	.long	.LC40@GOTOFF
.L2877:
	.long	_puts@PLT-(.LPCS370+2-.)
.L2880:
	.long	_gets@PLT-(.LPCS371+2-.)
.L2881:
	.long	.LC41@GOTOFF
.L2882:
	.long	_puts@PLT-(.LPCS372+2-.)
.L2885:
	.long	_puts@PLT-(.LPCS373+2-.)
.L2886:
	.long	.LC42@GOTOFF
.L2887:
	.long	_puts@PLT-(.LPCS374+2-.)
.L2890:
	.long	.LC43@GOTOFF
.L2891:
	.long	_strcmp@PLT-(.LPCS375+2-.)
.L2892:
	.long	.LC44@GOTOFF
.L2893:
	.long	_puts@PLT-(.LPCS376+2-.)
.L2896:
	.long	.LC45@GOTOFF
.L2897:
	.long	_strcmp@PLT-(.LPCS377+2-.)
.L2899:
	.long	.LC46@GOTOFF
.L2900:
	.long	_printf@PLT-(.LPCS378+2-.)
.L2903:
	.long	.LC47@GOTOFF
.L2904:
	.long	_strcmp@PLT-(.LPCS379+2-.)
.L2905:
	.long	-1412627932
.L2906:
	.long	-1412627928
.L2907:
	.long	.LC48@GOTOFF
.L2908:
	.long	_printf@PLT-(.LPCS380+2-.)
	.align 1
.L2859:
	mov.w	.L2916,r1
	mov.w	.L2917,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L2912,r1
	add	r12,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2913,r1
	bsrf	r1
.LPCS381:
	nop
	mov	r0,r1
	tst	r1,r1
	bf	.L2860
	mov.l	.L2914,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2915,r2
	bsrf	r2
.LPCS382:
	nop
	mov	#-1,r1
	mov	#-1,r2
	mov.l	r2,@r1
.L2860:
	mov.w	.L2916,r1
	mov.w	.L2917,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L2918,r1
	add	r12,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2919,r3
	bsrf	r3
.LPCS383:
	nop
	mov	r0,r1
	tst	r1,r1
	bt	.L2994
	bra	.L2861
	nop
.L2994:
	mov.w	.L2946,r8
	add	r14,r8
	mov.w	.L2938,r1
	mov	r1,r4
	mov.l	.L2922,r1
	bsrf	r1
.LPCS384:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov.w	.L2946,r8
	add	r14,r8
	mov.w	.L2938,r1
	mov	r1,r4
	mov.l	.L2925,r2
	bsrf	r2
.LPCS385:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov.w	.L2946,r1
	add	r14,r1
	mov.l	@(60,r1),r4
	mov.l	.L2927,r3
	bsrf	r3
.LPCS386:
	nop
	mov.w	.L2946,r1
	add	r14,r1
	mov.l	@(56,r1),r4
	mov.l	.L2929,r1
	bsrf	r1
.LPCS387:
	nop
	mov.l	.L2930,r1
	add	r12,r1
	mov	r1,r3
	mov.w	.L2946,r2
	add	r14,r2
	mov.w	.L2946,r1
	add	r14,r1
	mov	r3,r4
	mov.l	@(60,r2),r5
	mov.l	@(56,r1),r6
	mov.l	.L2933,r2
	bsrf	r2
.LPCS388:
	nop
	mov.w	.L2946,r8
	add	r14,r8
	mov.w	.L2938,r1
	mov	r1,r4
	mov.l	.L2936,r3
	bsrf	r3
.LPCS389:
	nop
	mov	r0,r1
	mov.l	r1,@(60,r8)
	mov.w	.L2946,r8
	add	r14,r8
	mov.w	.L2938,r1
	mov	r1,r4
	mov.l	.L2939,r1
	bsrf	r1
.LPCS390:
	nop
	mov	r0,r1
	mov.l	r1,@(56,r8)
	mov.w	.L2946,r1
	add	r14,r1
	mov.l	@(60,r1),r4
	mov.l	.L2941,r2
	bsrf	r2
.LPCS391:
	nop
	mov.w	.L2946,r1
	add	r14,r1
	mov.l	@(56,r1),r4
	mov.l	.L2943,r3
	bsrf	r3
.LPCS392:
	nop
	mov.l	.L2944,r1
	add	r12,r1
	mov	r1,r3
	mov.w	.L2946,r2
	add	r14,r2
	mov.w	.L2946,r1
	add	r14,r1
	mov	r3,r4
	mov.l	@(60,r2),r5
	mov.l	@(56,r1),r6
	mov.l	.L2947,r0
	bsrf	r0
.LPCS393:
	nop
	bra	.L2858
	nop
	.align 1
.L2916:
	.short	-1032
.L2917:
	.short	1032
.L2946:
	.short	968
.L2938:
	.short	256
.L2948:
	.align 2
.L2912:
	.long	.LC49@GOTOFF
.L2913:
	.long	_strcmp@PLT-(.LPCS381+2-.)
.L2914:
	.long	.LC50@GOTOFF
.L2915:
	.long	_puts@PLT-(.LPCS382+2-.)
.L2918:
	.long	.LC51@GOTOFF
.L2919:
	.long	_strcmp@PLT-(.LPCS383+2-.)
.L2922:
	.long	_TKMM_Malloc@PLT-(.LPCS384+2-.)
.L2925:
	.long	_TKMM_Malloc@PLT-(.LPCS385+2-.)
.L2927:
	.long	_TKMM_Free@PLT-(.LPCS386+2-.)
.L2929:
	.long	_TKMM_Free@PLT-(.LPCS387+2-.)
.L2930:
	.long	.LC52@GOTOFF
.L2933:
	.long	_printf@PLT-(.LPCS388+2-.)
.L2936:
	.long	_TKMM_Malloc@PLT-(.LPCS389+2-.)
.L2939:
	.long	_TKMM_Malloc@PLT-(.LPCS390+2-.)
.L2941:
	.long	_TKMM_Free@PLT-(.LPCS391+2-.)
.L2943:
	.long	_TKMM_Free@PLT-(.LPCS392+2-.)
.L2944:
	.long	.LC53@GOTOFF
.L2947:
	.long	_printf@PLT-(.LPCS393+2-.)
	.align 1
.L2861:
	mov.w	.L2986,r1
	mov.w	.L2987,r2
	add	r14,r2
	add	r1,r2
	mov.l	.L2951,r1
	add	r12,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2952,r1
	bsrf	r1
.LPCS394:
	nop
	mov	r0,r1
	tst	r1,r1
	bt	.L2993
	bra	.L2862
	nop
.L2993:
	mov.l	.L2953,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2954,r2
	bsrf	r2
.LPCS395:
	nop
	mov.l	.L2955,r3
	bsrf	r3
.LPCS396:
	nop
	mov.l	.L2956,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2957,r0
	bsrf	r0
.LPCS397:
	nop
	mov.w	.L2986,r1
	mov.w	.L2987,r2
	add	r14,r2
	add	r2,r1
	mov	r1,r4
	mov	#0,r5
	mov	#0,r6
	mov	#1,r7
	mov.l	.L2960,r3
	bsrf	r3
.LPCS398:
	nop
	mov.w	.L2986,r1
	mov.w	.L2987,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2966,r1
	add	r2,r1
	mov.b	@(14,r1),r0
	mov	r0,r2
	mov	#85,r1
	cmp/eq	r1,r2
	bf	.L2863
	mov.w	.L2986,r1
	mov.w	.L2987,r2
	add	r14,r2
	add	r1,r2
	mov.w	.L2966,r1
	add	r2,r1
	mov.b	@(15,r1),r0
	mov	r0,r2
	mov	#-86,r1
	cmp/eq	r1,r2
	bf	.L2863
	mov.l	.L2967,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2968,r1
	bsrf	r1
.LPCS399:
	nop
	bra	.L2864
	nop
	.align 1
.L2863:
	mov.l	.L2969,r1
	add	r12,r1
	mov	r1,r4
	mov.l	.L2970,r2
	bsrf	r2
.LPCS400:
	nop
.L2864:
	mov.l	.L2983,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	tst	r1,r1
	bf	.L2865
	mov.w	.L2976,r1
	mov	r1,r4
	mov.l	.L2973,r3
	bsrf	r3
.LPCS401:
	nop
	mov	r0,r1
	mov	r1,r2
	mov.l	.L2983,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	r2,@r1
	mov.l	.L2983,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r2
	mov.w	.L2976,r1
	mov	r2,r4
	mov	#0,r5
	mov	r1,r6
	mov.l	.L2977,r1
	bsrf	r1
.LPCS402:
	nop
	mov.l	.L2983,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	mov	r1,r4
	mov.l	.L2979,r2
	bsrf	r2
.LPCS403:
	nop
	mov.l	.L2983,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	mov	r1,r4
	mov.l	.L2981,r3
	bsrf	r3
.LPCS404:
	nop
	mov.l	.L2983,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r2
	mov.l	.L2983,r1
	add	r12,r1
	mov.l	@r1,r1
	mov.l	@r1,r1
	mov.l	@(40,r1),r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2984,r1
	bsrf	r1
.LPCS405:
	nop
	bra	.L2858
	nop
	.align 1
.L2865:
	bra	.L2858
	nop
	.align 1
.L2862:
	mov.l	.L2985,r1
	add	r12,r1
	mov	r1,r2
	mov.w	.L2986,r1
	mov.w	.L2987,r3
	add	r14,r3
	add	r3,r1
	mov	r2,r4
	mov	r1,r5
	mov.l	.L2988,r0
	bsrf	r0
.LPCS406:
	nop
	bra	.L2866
	nop
	.align 1
.L2858:
	bra	.L2866
	nop
	.align 1
.L2986:
	.short	-1032
.L2987:
	.short	1032
.L2966:
	.short	496
.L2976:
	.short	3264
.L2989:
	.align 2
.L2951:
	.long	.LC54@GOTOFF
.L2952:
	.long	_strcmp@PLT-(.LPCS394+2-.)
.L2953:
	.long	.LC55@GOTOFF
.L2954:
	.long	_printf@PLT-(.LPCS395+2-.)
.L2955:
	.long	_TKSPI_InitDevice@PLT-(.LPCS396+2-.)
.L2956:
	.long	.LC56@GOTOFF
.L2957:
	.long	_printf@PLT-(.LPCS397+2-.)
.L2960:
	.long	_TKSPI_ReadSectors@PLT-(.LPCS398+2-.)
.L2967:
	.long	.LC57@GOTOFF
.L2968:
	.long	_printf@PLT-(.LPCS399+2-.)
.L2969:
	.long	.LC58@GOTOFF
.L2970:
	.long	_printf@PLT-(.LPCS400+2-.)
.L2983:
	.long	_tkfat_fsimg@GOT
.L2973:
	.long	_malloc@PLT-(.LPCS401+2-.)
.L2977:
	.long	_memset@PLT-(.LPCS402+2-.)
.L2979:
	.long	_TKFAT_ReadImageMBR@PLT-(.LPCS403+2-.)
.L2981:
	.long	_TKFAT_ReadImageFAT@PLT-(.LPCS404+2-.)
.L2984:
	.long	_TKFAT_ListDir@PLT-(.LPCS405+2-.)
.L2985:
	.long	.LC59@GOTOFF
.L2988:
	.long	_printf@PLT-(.LPCS406+2-.)
	.align 1
.L2867:
	mov.w	.L2990,r7
	add	r7,r14
	mov	r14,r15
	lds.l	@r15+,pr
	mov.l	@r15+,r14
	mov.l	@r15+,r12
	mov.l	@r15+,r8
	rts	
	nop
	.align 1
.L2990:
	.short	1032
	.size	_tk_main, .-_tk_main
	.local	_init.1683
	.comm	_init.1683,4,4
	.section	.rodata
	.align 2
.LC60:
	.string	"0123456789ABCDEF"
	.section	.data.rel.local,"aw",@progbits
	.align 2
	.type	_hexchars.1800, @object
	.size	_hexchars.1800, 4
_hexchars.1800:
	.long	.LC60
	.local	_ta.2419
	.comm	_ta.2419,256,4
	.local	_tb.2420
	.comm	_tb.2420,512,4
	.align 2
	.type	_chrs.2483, @object
	.size	_chrs.2483, 4
_chrs.2483:
	.long	.LC60
	.align 2
	.type	_chrs.2488, @object
	.size	_chrs.2488, 4
_chrs.2488:
	.long	.LC60
	.ident	"GCC: (GNU) 4.8.4"
