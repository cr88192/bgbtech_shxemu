#!/bin/bash
export PATH=/opt/cross/bin:$PATH
export "TK_CFL=-nostdinc -nostdlib -nostartfiles -fno-builtin \
	-DARCH_SH4 -DARCH_SH4LE"
export "TK_LDFL= $TK_CFL \
	-Wl,-nostdlib -Wl,-e,_start -Wl,-T,sh4elf.x"
export "TK_CSRC=tkmain.c"

# sh4-as e_testkern.S -o e_testkern.o
sh4-gcc -c e_testkern.S -o e_testkern.o $TK_CFL

sh4-gcc -o ../testkern_sh4.elf e_testkern.o $TK_CSRC $TK_LDFL

sh4-objdump -t ../testkern_sh4.elf > ../testkern_sh4.map

cp ../testkern_sh4.elf ../pl_ems
cp ../testkern_sh4.map ../pl_ems
