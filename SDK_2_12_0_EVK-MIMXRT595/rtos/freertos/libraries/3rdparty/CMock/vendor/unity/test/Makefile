CC = gcc
ifeq ($(shell uname -s), Darwin)
CC = clang
endif
ifeq ($(findstring clang, $(CC)), clang)
E = -Weverything
CFLAGS += $E -Wno-unknown-warning-option -Wno-missing-prototypes
CFLAGS += -Wno-unused-macros -Wno-padded -Wno-missing-noreturn
endif
CFLAGS += -std=c99 -pedantic -Wall -Wextra -Wconversion -Werror
CFLAGS += -Wno-switch-enum -Wno-double-promotion
CFLAGS += -Wbad-function-cast -Wcast-qual -Wold-style-definition -Wshadow -Wstrict-overflow \
          -Wstrict-prototypes -Wswitch-default -Wundef
#DEBUG = -O0 -g
CFLAGS += $(DEBUG)
UNITY_SUPPORT_64 = -D UNITY_SUPPORT_64
UNITY_INCLUDE_DOUBLE = -D UNITY_INCLUDE_DOUBLE
DEFINES =  -D UNITY_OUTPUT_CHAR=putcharSpy
DEFINES += -D UNITY_OUTPUT_CHAR_HEADER_DECLARATION=putcharSpy\(int\)
DEFINES += -D UNITY_OUTPUT_FLUSH=flushSpy
DEFINES += -D UNITY_OUTPUT_FLUSH_HEADER_DECLARATION=flushSpy\(void\)
DEFINES += $(UNITY_SUPPORT_64) $(UNITY_INCLUDE_DOUBLE)
SRC = ../src/unity.c tests/testunity.c build/testunityRunner.c
INC_DIR = -I ../src
COV_FLAGS = -fprofile-arcs -ftest-coverage -I ../../src
BUILD_DIR = build
TARGET = build/testunity-cov.exe

# To generate coverage, call 'make -s', the default target runs.
# For verbose output of all the tests, run 'make test'.
default: coverage
.PHONY: default coverage test clean
coverage: $(BUILD_DIR)/testunityRunner.c
	cd $(BUILD_DIR) && \
	$(CC) $(CFLAGS) $(DEFINES) $(foreach i,$(SRC), ../$i) $(COV_FLAGS) -o ../$(TARGET)
	rm -f $(BUILD_DIR)/*.gcda
	./$(TARGET) | grep 'Tests\|]]]' -A1
	cd $(BUILD_DIR) && \
	gcov unity.c | head -3
	grep '###' $(BUILD_DIR)/unity.c.gcov -C2 || true

test: $(BUILD_DIR)/testunityRunner.c
	$(CC) $(CFLAGS) $(DEFINES) $(INC_DIR) $(SRC) -o $(TARGET)
	./$(TARGET)

# Compile only, for testing that preprocessor detection works
UNITY_C_ONLY =-c ../src/unity.c -o $(BUILD_DIR)/unity.o
intDetection:
	$(CC) $(CFLAGS) $(INC_DIR) $(UNITY_C_ONLY) -D UNITY_EXCLUDE_STDINT_H
	$(CC) $(CFLAGS) $(INC_DIR) $(UNITY_C_ONLY) -D UNITY_EXCLUDE_LIMITS_H

$(BUILD_DIR)/testunityRunner.c: tests/testunity.c | $(BUILD_DIR)
	awk $(AWK_SCRIPT) tests/testunity.c > $@

AWK_SCRIPT=\
  '/^void test/{ declarations[d++]=$$0; gsub(/\(?void\)? ?/,""); tests[t++]=$$0; line[u++]=NR } \
  END{ print "\#include \"unity.h\" /* Autogenerated by awk in Makefile */" ;                   \
       for (i=0; i<d; i++) { print declarations[i] ";" }                                        \
       print "int main(void)\n{\n    UnityBegin(\"" FILENAME "\");" ;                           \
       for (i=0; i<t; i++) { print "    RUN_TEST(" tests[i] ", " line[i] ");" }                 \
       print "    return UNITY_END();\n}" }'

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(TARGET) $(BUILD_DIR)/*.gc* $(BUILD_DIR)/testunityRunner.c
