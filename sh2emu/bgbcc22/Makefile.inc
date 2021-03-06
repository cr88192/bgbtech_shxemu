SRCS=\
	cparse/cparse_token.c \
	cparse/cparse_misc.c \
	cparse/cparse_type.c \
	cparse/cparse_def.c \
	cparse/cparse_expr2.c \
	cparse/cparse_reduce.c \
	cparse/cparse_stmt.c \
	cparse/preproc.c \
	\
	mm/cc_tokord.c \
	\
	mm/ccx_node.c \
	mm/ccx_print.c \
	mm/ccx_parse.c \
	mm/cc_float16.c \
	mm/cc_malloc.c \
	mm/cc_thread.c \
	\
	ccxl/ccxl_cgif.c \
	ccxl/ccxl_compile.c \
	ccxl/ccxl_const.c \
	ccxl/ccxl_expr.c \
	ccxl/ccxl_global.c \
	ccxl/ccxl_index.c \
	ccxl/ccxl_infer.c \
	ccxl/ccxl_pred.c \
	ccxl/ccxl_reduce.c \
	ccxl/ccxl_register.c \
	ccxl/ccxl_ril3.c \
	ccxl/ccxl_stack.c \
	ccxl/ccxl_struct.c \
	ccxl/ccxl_trace.c \
	ccxl/ccxl_type.c \
	\
	shcc/shx_binop.c \
	shcc/shx_bufasm.c \
	shcc/shx_cgif.c \
	shcc/shx_conv.c \
	shcc/shx_elf.c \
	shcc/shx_emit.c \
	shcc/shx_emit_sec.c \
	shcc/shx_emit_lbl.c \
	shcc/shx_emit_opc.c \
	shcc/shx_emit_bra.c \
	shcc/shx_fltrom.c \
	shcc/shx_fpreg.c \
	shcc/shx_frame.c \
	shcc/shx_ldix.c \
	shcc/shx_lparith.c \
	shcc/shx_lpreg.c \
	shcc/shx_lqarith.c \
	shcc/shx_lvarith.c \
	shcc/shx_lxarith.c \
	shcc/shx_pecoff.c \
	shcc/shx_register.c \
	shcc/shx_struct.c \
	shcc/shx_vreg.c \


META_SRCS = \
	$(SRCS) \
	\
	mm/inflate.c \
	mm/deflate.c \
	mm/cc_exwad.c \
	mm/ccl_context.c \
	mm/ccl_coff.c \
	mm/ccl_elf.c \
	\
	mm/dyll_metapath.c \
	mm/dyll_metabin.c \
	bgbmeta.c

DEP_HDRS=\
	include/bgbcc_auto.h

LIBS=
APPS=bgbcc$(BINSUF)

all: libs apps

libs: $(LIBS)
apps: $(APPS)
includes: $(DEP_HDRS)

install: all
	cp *$(BINSUF) $(INST_BIN)

autohead$(BINSUF): autohead.c
	$(CC_OUTTOOL)autohead$(BINSUF) autohead.c

gen_ord$(BINSUF): gen_ord.c
	$(CC_OUTTOOL)gen_ord$(BINSUF) gen_ord.c

mm/cc_tokord.c: gen_ord$(BINSUF) bgbcc_token.txt
	./gen_ord$(BINSUF)

include/bgbcc_auto.h: autohead$(BINSUF) $(META_SRCS)
	./autohead$(BINSUF) -api BGBCC_API $(META_SRCS) > include/bgbcc_auto.h

bgbcc$(BINSUF): $(DEP_HDRS) $(META_SRCS)
	$(CC_OUTTOOL)bgbcc$(BINSUF) bgbcc_multi.c \
		$(CFLAGS_META) $(LDFLAGS_META)
		

clean:
