build.sh: Builds the file 'btesh2.elf', which is the main VM.

Usage:
	./btesh2.elf [options] [image]

Options:
	-sh2			SH-2 Emulation
	-sh4			SH-4 Emulation
	-map <name>		Symbol Map
	-sd <name>		Use SD card image.
	-sdcl <name>	Use SDCL command script.


SD-Card Image

Expressed as a raw image file with an MBR and a filesystem, starting with the MBR. Normally these files are the full size of the filesystem, which is limited to approx 2GB.


SDCL Command Language

mkimage <image> <size[sfx]> [options]
	image=image filename.
	size=size of image, default in sectors.
	sfx: k=KiB, M=MeB, G=GiB
	-F16: Create image using FAT16
	-F32: Create image using FAT32

Makes a new raw / single-file image.
If image exists, it is overwritten.

Single-file images are generally limited to around 2GB.


openimage <image> <size[sfx]> [options]
Open a new or existing single-file image.

If image exists, the existing image is loaded.
If image does not exist, a new image is created.
The size gives the size for a newly created image, as do the options.


opensegimg <base> <size[sfx]> [options]
Open a new or existing segmented image.
Base is given as the directory to hold the image segments.

This is intended more for larger images (above 2GB).
Currently will split the image into 512MiB segments (1M sectors).

Each segment will begin with an index table, which here will be defined as holding 256 indices, expressed as 32-bit integer values, and representing a space of 2MeB (4096 sectors).

In these indices, the low 9 bits will encode a value between 0 and 256, where:
  0=Empty cluster, should be read as zeroes, and allocated on write.
  1..256: Gives the offset of this cluster in the image, as:
    (idx-1)*2MeB + 64KiB.



addfile <iname> [ename]

Add a file to the image, iname gives the internal name.
The ename option gives an external name for the file.
If ename is not given, this will assume that the internal and external file have the same name.



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
