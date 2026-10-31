# Menjalankan uji latihan di laptop (Linux/macOS/WSL). Contoh:  make test-01   atau   make test-all
CC       ?= gcc
CXX      ?= g++
BUILD    := build
INC      := -I common -I level-01-bitwise -I level-02-can-id -I level-03-payload \
            -I level-04-scaling -I level-05-registry -I level-07-decode \
            -I level-10-commands -I level-12-scan -I level-14-oop -I level-15-bxcan
CFLAGS   := -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L $(INC)
CXXFLAGS := -Wall -Wextra -std=c++11 $(INC)

ID_SRC   := level-02-can-id/cg_id.c
PAY_SRC  := level-03-payload/payload.c
SCL_SRC  := level-04-scaling/scaling.c -lm
DEC_SRC  := level-07-decode/cg_decode.c
CMD_SRC  := level-10-commands/cg_cmd.c

.PHONY: help test-01 test-02 test-03 test-04 test-05 test-07 test-10 test-12 test-14 test-15 test-all clean

help:
	@echo "Target: test-01 test-02 test-03 test-04 test-05 test-07 test-10 test-12 test-14 test-15 test-all clean"
	@echo "Level 7, 10, 12, 14 memakai hasil Level 2, 3, 4 milikmu (selesaikan berurutan)."

$(BUILD):
	@mkdir -p $(BUILD)

test-01: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t01 level-01-bitwise/test_bits.c level-01-bitwise/bits.c && $(BUILD)/t01
test-02: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t02 level-02-can-id/test_cg_id.c $(ID_SRC) && $(BUILD)/t02
test-03: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t03 level-03-payload/test_payload.c $(PAY_SRC) && $(BUILD)/t03
test-04: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t04 level-04-scaling/test_scaling.c $(SCL_SRC) && $(BUILD)/t04
test-05: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t05 level-05-registry/test_registry.c level-05-registry/registry.c && $(BUILD)/t05
test-07: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t07 level-07-decode/test_decode.c $(DEC_SRC) $(ID_SRC) $(PAY_SRC) $(SCL_SRC) && $(BUILD)/t07
test-10: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t10 level-10-commands/test_cmd.c $(CMD_SRC) $(ID_SRC) $(PAY_SRC) $(SCL_SRC) && $(BUILD)/t10
test-12: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t12 level-12-scan/test_scan.c level-12-scan/cg_scan.c level-12-scan/mockbus.c $(CMD_SRC) $(DEC_SRC) $(ID_SRC) $(PAY_SRC) $(SCL_SRC) && $(BUILD)/t12
test-14: $(BUILD)
	$(CC) $(CFLAGS) -c $(CMD_SRC) -o $(BUILD)/cg_cmd.o
	$(CC) $(CFLAGS) -c $(DEC_SRC) -o $(BUILD)/cg_decode.o
	$(CC) $(CFLAGS) -c $(ID_SRC)  -o $(BUILD)/cg_id.o
	$(CC) $(CFLAGS) -c $(PAY_SRC) -o $(BUILD)/payload.o
	$(CC) $(CFLAGS) -c level-04-scaling/scaling.c -o $(BUILD)/scaling.o
	$(CXX) $(CXXFLAGS) -o $(BUILD)/t14 level-14-oop/test_motor.cpp level-14-oop/cg_motor.cpp \
	    $(BUILD)/cg_cmd.o $(BUILD)/cg_decode.o $(BUILD)/cg_id.o $(BUILD)/payload.o $(BUILD)/scaling.o -lm && $(BUILD)/t14
test-15: $(BUILD)
	$(CC) $(CFLAGS) -o $(BUILD)/t15 level-15-bxcan/test_bxcan.c level-15-bxcan/bxcan.c && $(BUILD)/t15

test-all: $(BUILD)
	@for t in 01 02 03 04 05 07 10 12 14 15; do \
	    echo "=============== LEVEL $$t"; $(MAKE) --no-print-directory -s test-$$t > $(BUILD)/log-$$t.txt 2>&1 \
	        && echo "LOLOS" || { echo "BELUM LOLOS (lihat: make test-$$t)"; }; \
	done

clean:
	rm -rf $(BUILD)
