#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btesh2_cpu.h"
#include "btesh2_fatfs.h"
#include "btesh2_keys.h"

#include "btesh2_memimg.c"

#include "btesh2_oparith.c"
#include "btesh2_opbit.c"
#include "btesh2_opjmp.c"
#include "btesh2_opmov.c"

#include "btesh2_opfpari.c"
#include "btesh2_fastmmu.c"

#include "btesh2_dbgmap.c"
#include "btesh2_spimmc.c"
#include "btesh2_sdfat.c"

#include "btesh2_decode.c"
#include "btesh2_interp.c"
#include "btesh2_ldelf.c"

#include "btesh2_gfxcon.c"
#include "btesh2_dcgfx.c"

#include "gfxdrv_w32.c"
#include "gfxdrv_input.c"
