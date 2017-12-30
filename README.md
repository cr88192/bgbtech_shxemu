# bgbtech_shxemu
BGBTech SH-2 and SH-4 Emulator / Stuff

This is basically a quick and dirty emulator aiming to support SH-2 and SH-4.

This far, it more-or-less has SH-2 working in a basic sense (have not yet gotten Linux working on it though).
Some initial work was started on supporting SH-4, but this is far from complete.

I may also experiment with some ideas, or maybe if/when I get to it, write a custom assembler and compiler/codegen.
Thus far, this isn't really expected to be a serious project.


What is SH?
SH is also known as SuperH, which was an ISA used in several Sega consoles (32X, Saturn, and Dreamcast), as well as various other uses in consumer electronics. It is a 32-bit RISC style ISA with 16-bit instruction words, 16 GPRs, and FPU (in the SH-4).


sh2emu: SH and BJX1 emulator

sh2emu/bgbcc22: an experimental C compiler, mostly intended for BJX1
* For targeting SH2 or SH4, using GCC is recommended.

sh2emu/tk_qsrc: Quake 1 source, also includes the C library.
