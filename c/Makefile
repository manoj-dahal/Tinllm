DETECT_CC := $(if $(filter default,$(origin CC)),gcc,$(CC))
TRIPLET   := $(shell $(DETECT_CC) -dumpmachine 2>/dev/null)

MINGW  := $(findstring mingw,$(TRIPLET))
CYGWIN := $(findstring cygwin,$(TRIPLET))
DARWIN := $(findstring darwin,$(TRIPLET))
LINUX  := $(findstring linux,$(TRIPLET))
IS_WIN := $(MINGW)$(CYGWIN)

ifeq ($(TRIPLET),)
IS_WIN := $(if $(filter Windows_NT,$(OS)),1,)
ifeq ($(IS_WIN),)
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)
DARWIN  := $(findstring Darwin,$(UNAME_S))
endif
endif

TARGET_CPU := $(firstword $(subst -, ,$(TRIPLET)))
ifeq ($(TARGET_CPU),)
TARGET_CPU := $(UNAME_M)
endif
X86_64  := $(filter x86_64 amd64,$(TARGET_CPU))
AARCH64 := $(filter aarch64 arm64,$(TARGET_CPU))
PPC64   := $(filter powerpc64% ppc64%,$(TARGET_CPU))

ifneq (,$(DARWIN))
CC      = clang
OMPDIR := $(shell brew --prefix libomp 2>/dev/null)
ifneq ($(and $(OMPDIR),$(wildcard $(OMPDIR)/include/omp.h),$(wildcard $(OMPDIR)/lib/libomp.*)),)
OMPC    = -Xclang -fopenmp -I$(OMPDIR)/include
OMPL    = -L$(OMPDIR)/lib -lomp
else
OMPC    =
OMPL    =
endif
CFLAGS  = -O3 $(OMPC) -Wall -Wextra -Wno-unused-parameter -Wno-misleading-indentation -Wno-unused-function
ifneq ($(ARCH),)
CFLAGS += -mcpu=$(ARCH)
endif
LDFLAGS = -lm $(OMPL)
EXE     =
else ifneq ($(IS_WIN),)
CC      = gcc
ARCH   ?= x86-64-v3
CFLAGS  = -D_FILE_OFFSET_BITS=64 -O3 -march=$(ARCH) -fopenmp -Wall -Wextra -Wno-unused-parameter -Wno-misleading-indentation -Wno-unused-function
LDFLAGS = -lm -fopenmp -static -lpsapi
EXE     = .exe
else
CC      = gcc
ARCH   ?= native
CFLAGS  = -O3 -march=$(ARCH) -fopenmp -pthread -Wall -Wextra -Wno-unused-parameter -Wno-misleading-indentation -Wno-unused-function
LDFLAGS = -lm -fopenmp -pthread
EXE     =
endif

ifeq ($(OS),Windows_NT)
PYTHON    ?= python
else
PYTHON    ?= python3
endif

TARGET     = tin$(EXE)
V2_ENGINE  = tin_v2$(EXE)
V2_BENCH   = tin_v2_bench$(EXE)
GLM_ENGINE = glm$(EXE)
IOBENCH    = iobench$(EXE)
SHARED_LIB = libtin.so

TEST_BINS = tests/test_json$(EXE) tests/test_st$(EXE) tests/test_tier$(EXE) tests/test_grammar$(EXE) tests/test_schema_gbnf$(EXE) tests/test_decode_batch$(EXE) tests/test_idot$(EXE) tests/test_kv_alloc$(EXE) tests/test_i4_acc512$(EXE) tests/test_compat_direct$(EXE) tests/test_tin$(EXE)

all: $(TARGET) $(V2_ENGINE) $(V2_BENCH) $(GLM_ENGINE) $(IOBENCH) $(SHARED_LIB)

$(TARGET): tin.c compat.h st.h json.h tok.h tin_cache.h tin_kv.h tin_grammar.h tin_async_io.h tin_cuda.h
	$(CC) $(CFLAGS) tin.c -o $(TARGET) $(LDFLAGS)

$(V2_ENGINE): tin_v2.c compat.h st.h json.h tok.h tin_paged_kv.h tin_predictive_router.h tin_quant_nvfp4.h
	$(CC) $(CFLAGS) tin_v2.c -o $(V2_ENGINE) $(LDFLAGS)

$(V2_BENCH): tin_v2_bench.c compat.h tin_paged_kv.h tin_predictive_router.h tin_quant_nvfp4.h tin_flash_attn.h
	$(CC) $(CFLAGS) tin_v2_bench.c -o $(V2_BENCH) $(LDFLAGS)

$(GLM_ENGINE): glm.c st.h uring.h json.h tok.h tok_unicode.h compat.h grammar.h
	$(CC) $(CFLAGS) glm.c -o $(GLM_ENGINE) $(LDFLAGS)

$(SHARED_LIB): tin_lib.c compat.h tin_sampler.h
	$(CC) $(CFLAGS) -fPIC -shared tin_lib.c -o $(SHARED_LIB) $(LDFLAGS)

lib: $(SHARED_LIB)

$(IOBENCH): iobench.c compat.h
	$(CC) $(CFLAGS) iobench.c -o $(IOBENCH) $(LDFLAGS)

tests/test_tin$(EXE): tests/test_tin.c compat.h tin_cache.h tin_kv.h tin_grammar.h tin_sampler.h tin_batch.h tin_kv_persist.h tin_router.h tin_mtp.h
	mkdir -p tests
	$(CC) $(CFLAGS) tests/test_tin.c -o tests/test_tin$(EXE) $(LDFLAGS)

tests/test_json$(EXE): tests/test_json.c json.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_st$(EXE): tests/test_st.c st.h json.h compat.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_tier$(EXE): tests/test_tier.c tier.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_grammar$(EXE): tests/test_grammar.c grammar.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_schema_gbnf$(EXE): tests/test_schema_gbnf.c schema_gbnf.h grammar.h json.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_decode_batch$(EXE): tests/test_decode_batch.c decode_batch.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_idot$(EXE): tests/test_idot.c glm.c st.h uring.h json.h tok.h tok_unicode.h compat.h grammar.h tier.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_kv_alloc$(EXE): tests/test_kv_alloc.c glm.c st.h json.h tok.h tok_unicode.h compat.h grammar.h tier.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_i4_acc512$(EXE): tests/test_i4_acc512.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

tests/test_compat_direct$(EXE): tests/test_compat_direct.c compat.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

test-c: $(TEST_BINS)
	$(PYTHON) tools/run_tests.py $(TEST_BINS)

test: test-c

check: test

clean:
	$(PYTHON) tools/clean.py
	rm -f $(TARGET) $(V2_ENGINE) $(V2_BENCH) $(GLM_ENGINE) $(IOBENCH) $(SHARED_LIB)

.PHONY: all test test-c check clean lib
