; Listing generated by Microsoft (R) Optimizing Compiler Version 19.00.23918.0 

include listing.inc

INCLUDELIB LIBCMT
INCLUDELIB OLDNAMES

PUBLIC	main
EXTRN	str_ret0:PROC
;	COMDAT pdata
pdata	SEGMENT
$pdata$main DD	imagerel $LN4
	DD	imagerel $LN4+21
	DD	imagerel $unwind$main
pdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$main DD	010401H
	DD	0c204H
xdata	ENDS
; Function compile flags: /Ogspy
;	COMDAT main
_TEXT	SEGMENT
$T1 = 32
main	PROC						; COMDAT
; File j:\bgb.proj.j2emu\misc\strret0_m.c
; Line 17
$LN4:
	sub	rsp, 104				; 00000068H
; Line 18
	lea	rcx, QWORD PTR $T1[rsp]
	call	str_ret0
; Line 19
	xor	eax, eax
	add	rsp, 104				; 00000068H
	ret	0
main	ENDP
_TEXT	ENDS
END
