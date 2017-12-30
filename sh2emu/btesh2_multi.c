#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

// #ifdef _M_X64
#if defined(_M_X64) || defined(__x86_64__)
#define BTESH2_HASJIT
#endif

#include "btesh2_common.h"
#include "btesh2_cpu.h"
#include "btesh2_fatfs.h"
#include "btesh2_keys.h"

#include "btesh2_memimg.c"
#include "btesh2_fastmmu.c"
#include "btesh2_tlbmmu.c"

#include "btesh2_oparith.c"
#include "btesh2_opbit.c"
#include "btesh2_opjmp.c"
#include "btesh2_opmov.c"
#include "btesh2_opmov2.c"
#include "btesh2_opmov_jq.c"
#include "btesh2_opmov2_jq.c"

#include "btesh2_opfpari.c"

#include "btesh2_dbgmap.c"
#include "btesh2_spimmc.c"
#include "btesh2_sdfat.c"

#ifdef BTESH2_HASJIT
#include "bteuax64/uax_multi.c"
#include "btesh2_jit_opmov.c"
#include "btesh2_jit_opbra.c"
#include "btesh2_jitjq_mov.c"
#include "btesh2_jitjq_bra.c"
#include "btesh2_jit.c"
#else
#include "btesh2_nojit.c"
#endif

#include "btesh2_decode.c"
#include "btesh2_decode_2a.c"
#include "btesh2_decode_b1.c"
#include "btesh2_dec_b1cc0.c"
#include "btesh2_dec_b1cd.c"
#include "btesh2_dec_trace.c"
#include "btesh2_interp.c"
#include "btesh2_ldelf.c"
#include "btesh2_ldpecoff.c"

#include "softgpu/jxgpu_multi.c"
#include "btesh2_gfxcon.c"
#include "btesh2_dcgfx.c"
#include "btesh2_snddev.c"

#include "gfxdrv_w32.c"
#include "gfxdrv_input.c"
#include "sound_w32.c"
#include "sound_ems.c"

#ifdef __linux
#include "gfxdrv_dummy.c"
#endif
