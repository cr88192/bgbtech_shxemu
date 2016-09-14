#!/bin/bash
export PATH=/opt/cross/bin:$PATH
export "TK_LDFL=-nostdinc -nostdlib -nostartfiles -fno-builtin \
	-Wl,-nostdlib -Wl,-e,_start -Wl,-T,shelf.x"
export "TK_CSRC=tkmain.c"

sh2-elf-as e_testkern.s -o e_testkern.o

sh2-elf-gcc -o ../testkern.elf e_testkern.o $TK_CSRC $TK_LDFL
