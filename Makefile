# The RAMakefile, the RAM.
# Copyright Alexander Silaev and The Rusm Org.
# Licensed under GNU General Public License v3.
# SPDX-License-Identifier: GPL-3-or-later.

ARCH ?= host

DT_DIR := build/$(ARCH)

ifeq ($(ARCH), host)
    CC := gcc
    CFLAGS := -O2 -Wall -Wextra -std=c11
    ARCH_DIR := 
else ifeq ($(ARCH), e2k)
    CC := lcc
    CFLAGS := -O3 -Wall -std=c11 -ffast-math
    ARCH_DIR := src/e2k
else ifeq ($(ARCH), arm64)
    CC := aarch64-linux-gnu-gcc
    CFLAGS := -O2 -Wall -Wextra -std=c11
    ARCH_DIR := src/arm/armv8
else ifeq ($(ARCH), arm32)
    CC := arm-linux-gnueabihf-gcc
    CFLAGS := -O2 -Wall -Wextra -std=c11
    A RCH_DIR := src/arm/armv7
else ifeq ($(ARCH), riscv32)
    CC := riscv64-unknown-elf-gcc
    CFLAGS := -O2 -Wall -Wextra -std=c11 -march=rv32imac -mabi=ilp32
    ARCH_DIR := src/riscv/rv32
else ifeq ($(ARCH), riscv64)
    CC := riscv64-linux-gnu-gcc
    CFLAGS := -O2 -Wall -Wextra -std=c11
    ARCH_DIR := src/riscv/rv64
else
    $(error Unknown arch '$(ARCH)'. Available: host, e2k, arm32, arm64, riscv32, riscv64)
endif

TARGET := build/ram-$(ARCH)
ifeq ($(ARCH), host)
	TARGET := build/ram
endif

INCLUDES := -Isrc/include

COMMON_SRCS := src/main.c $(wildcard src/core/*.c)

ARCH_SRCS :=
ifneq ($(ARCH_DIR),)
	ARCH_SRCS := $(wildcard $(ARCH_DIR)/*.c)
endif

SRCS := $(COMMON_SRCS) $(ARCH_SRCS)

OBJS := $(patsubst %.c, $(DT_DIR)/%.o, $(SRCS))

.PHONY: all clean info

all: $(TARGET)
	@printf "\033[32mInfo\033[0m: Build for arch '\033[36m$(ARCH)\033[0m' complete, see the $(TARGET)\n"


$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(DT_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

info:
	@printf "\033[35m---- RAMakefile INFO ----\033[0m\n"
	@printf "Arch:\t\t$(ARCH)\n"
	@printf "Compiler:\t$(CC)\n"
	@printf "Flags:\t\t$(CFLAGS)\n"
	@printf "Output:\t\t$(TARGET)\n"
	@printf "Input:\t\t$(SRCS)\n"
	@printf "Objects:       $(OBJS)\n"
	@printf "\033[36m---- END ----\033[0m\n"

clean:
	@rm -rf $(DT_DIR) $(TARGET) build