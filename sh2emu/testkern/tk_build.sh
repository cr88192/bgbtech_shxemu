#!/bin/bash
export PATH=/opt/cross/bin:$PATH
export "TK_CFL=-nostdinc -nostdlib -nostartfiles -fno-builtin -DARCH_SH2"
export "TK_LDFL=$TK_CFL \
	-Wl,-nostdlib -Wl,-e,_start -Wl,-T,shelf.x"
export "TK_CSRC=tkmain.c"

# sh2-elf-as e_testkern.S -o e_testkern.o
sh2-elf-gcc -c e_testkern.S -o e_testkern.o $TK_CFL

sh2-elf-gcc -o ../testkern.elf e_testkern.o $TK_CSRC $TK_LDFL
