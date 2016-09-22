build.sh: Builds the file 'btesh2.elf', which is the main VM.

Usage:
	./btesh2.elf [options] [image]

Options:
	-sh2			SH-2 Emulation
	-sh4			SH-4 Emulation
	-map <name>		Symbol Map


Memory Map (SH-2)
    0000_0000..0000_FFFF: SRAM
    1000_0000..17FF_FFFF: DRAM
    ABCD_0000..ABCD_FFFF: MMIO (Memory Mapped IO, UART/GPIO)
    FF00_0000..FF00_FFFF: MMREG (Memory Mapped Registers)

SH-2 always uses a physical memory map.


Memory Map (SH-4, Physical)
//  0000_0000..0000_FFFF: SRAM
    0BCD_0000..0BCD_FFFF: MMIO (Memory Mapped IO, UART/GPIO)
    0C00_0000..17FF_FFFF: DRAM
    FF00_0000..FF00_FFFF: MMREG (Memory Mapped Registers)

Memory Map (SH-4, Logical)
	0000_0000..7FFF_FFFF: User Memory, Uses MMU
	8000_0000..9FFF_FFFF: Privledged, Physical
	A000_0000..BFFF_FFFF: Privledged, Physical, No-Cache
	C000_0000..DFFF_FFFF: Privledged, Uses MMU
	E000_0000..FFFF_FFFF: Privledged, Used internally by CPU

SH-4 MMIO is at a different location because SH-4 uses a 29-bit physical address space. As a result, ABCD_0000 and 0BCD_0000 end up being effectively the same address.

MMIO (Accessed as DWord values):
	0000: GPIO Control
	0004: GPIO Data
	..
	0040: SPI Control
	0044: SPI Data
	..
	0100: UART Rx (Reads from console)
	0104: UART Tx (Writes to console)
	0108: UART Status (Gives UART status)
	010C: UART Ctrl (Controls UART)
