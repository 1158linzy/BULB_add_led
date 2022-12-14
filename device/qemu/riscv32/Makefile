#Copyright (c) 2020-2021 Huawei Device Co., Ltd.
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.

LITEOSTOPDIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
LITEOSTOPDIR := ../../../kernel/liteos_m
OPENHARMONYDIR := ../../..
LITEOSTOPDIR := $(realpath $(LITEOSTOPDIR))
OPENHARMONYDIR := $(realpath $(OPENHARMONYDIR))
export LITEOSTOPDIR

# Selective compiler
#COMPILE_NAME := riscv32-unknown-elf
COMPILE_NAME = riscv-none-embed
ifeq ($(COMPILE_NAME), riscv-none-embed)
CROSS_COMPILE = riscv-none-embed-
VERSION_NUM = 8.1.0
else
CROSS_COMPILE := riscv32-unknown-elf-
VERSION_NUM = 7.3.0
endif

# Select instruction set
MARCH := rv32imac
MABI  := ilp32

LITEOS_PLATFORM := riscv32



# System default
CFLAGS        := -I $(LITEOSTOPDIR)/utils                                  \
                 -I $(LITEOSTOPDIR)/kernel/include                         \
                 -I $(LITEOSTOPDIR)/kernel/arch/risc-v/riscv32/gcc         \
                 -I $(LITEOSTOPDIR)/kernel/arch/risc-v/riscv32/gcc/asm     \
                 -I $(LITEOSTOPDIR)/kernel/arch/include                    \
                 -I $(LITEOSTOPDIR)/components/cpup                        \
                 -I $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/include     \
                 -I $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/include/asm \
                 -I $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/driver      \
                 -I $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)

LOCAL_CSRCS   := $(wildcard $(LITEOSTOPDIR)/kernel/src/*.c)                       \
                 $(wildcard $(LITEOSTOPDIR)/kernel/src/mm/*.c)                    \
                 $(wildcard $(LITEOSTOPDIR)/kernel/arch/risc-v/riscv32/gcc/*.c)   \
                 $(wildcard $(LITEOSTOPDIR)/utils/*.c)                            \
                 $(wildcard $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/*.c)       \
                 $(wildcard $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/driver/*.c) \
                 $(wildcard $(LITEOSTOPDIR)/components/cpup/*.c)                  \
                 $(wildcard $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/libc/*.c)

LOCAL_ASMSRCS := $(wildcard $(LITEOSTOPDIR)/kernel/arch/risc-v/riscv32/gcc/*.S)           \
                 $(wildcard $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/los_start.S) \
                 $(wildcard $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/libc/*.S)

LOCAL_CSRCS += $(wildcard $(LITEOSTOPDIR)/components/bounds_checking_function/src/*.c)
CFLAGS += -I $(LITEOSTOPDIR)/components/bounds_checking_function/include

CC = $(CROSS_COMPILE)gcc
AS  = $(CROSS_COMPILE)as
AR  = $(CROSS_COMPILE)ar
LD  = $(CROSS_COMPILE)ld
GPP = $(CROSS_COMPILE)g++
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size
NM = $(CROSS_COMPILE)nm
ARFLAGS = cr
OBJ_MKDIR = if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi

HIDE = @
RM = rm -rf
MAKE = make
OUT    := out
OBJOUT := $(OUT)/obj
LIBA = $(OUT)/libs/libkernel.a
LITEOS_LIBS_DIR := -L$(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/$(OUT)/libs
LITEOS_GCCLIB := libgcc.a
LITEOS_TARGET = liteos
LITEOS_LIBDEP := -lkernel -lgcc -lc

LITEOS_SSP = -fno-stack-protector
LITEOS_COPTS_BASE  := -march=$(MARCH) -mabi=$(MABI) -static -fno-common -O2 $(LITEOS_SSP)
LITEOS_COPTS_EXTRA := -ffunction-sections -fdata-sections -fno-omit-frame-pointer -Wall -Wno-format-truncation
LITEOS_ASOPTS      := -march=$(MARCH) -mabi=$(MABI) -static -O2
LITEOS_LD_OPTS     := --build-id=none -static -nostartfiles --gc-sections

LITEOS_COMPILER_PATH := $(realpath $(shell $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/compiler_dir.sh  $(CC)))
LITEOS_COMPILER_CXX_PATH = $(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/include
LITEOS_COMPILER_CXXLIB_PATH = $(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/lib/$(LITEOS_GCCLIB)
ifeq ($(COMPILE_NAME), riscv-none-embed)
LITEOS_COMPILER_GCCLIB_PATH = -L$(LITEOS_COMPILER_PATH)/lib/gcc/$(COMPILE_NAME)/$(VERSION_NUM)/$(MARCH)/$(MABI)/ \
                              -L$(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/lib/$(MARCH)/$(MABI)/
LITEOS_LD_OPTS += -mcmodel=medany -melf32lriscv
else
LITEOS_COMPILER_GCCLIB_PATH = -L$(LITEOS_COMPILER_PATH)/lib/gcc/$(COMPILE_NAME)/$(VERSION_NUM)/ \
                              -L$(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/lib/
endif
LITEOS_COMPILER_GCC_INCLUDE = -I $(LITEOS_COMPILER_PATH)/lib/gcc/$(COMPILE_NAME)/$(VERSION_NUM)/include  \
                              -I $(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/include

LITEOS_LD_SCRIPT := $(OPENHARMONYDIR)/device/qemu/$(LITEOS_PLATFORM)/Liteos.ld
LITEOS_LDFLAGS := $(LITEOS_LD_OPTS) -T$(LITEOS_LD_SCRIPT) $(LITEOS_LIBS_DIR) $(LITEOS_COMPILER_GCCLIB_PATH)

LITEOS_CFLAGS  := $(LITEOS_COPTS_BASE) $(LITEOS_COPTS_EXTRA) -DLOSCFG_LIB_LIBC
LITEOS_ASFLAGS := $(LITEOS_ASOPTS)
LOCAL_FLAGS := $(CFLAGS) $(LITEOS_COMPILER_GCC_INCLUDE)
LOCAL_COBJS := $(patsubst %.c,$(OBJOUT)/%.o,$(LOCAL_CSRCS))
LOCAL_ASMOBJS := $(patsubst %.S,$(OBJOUT)/%.o,$(LOCAL_ASMSRCS))

all: $(LITEOS_TARGET)

$(LITEOS_TARGET): $(LIBA)
	$(LD) $(LITEOS_LDFLAGS) $(LITEOS_TABLES_LDFLAGS) --start-group $(LITEOS_LIBDEP) --end-group -o $(OUT)/$@
	$(OBJCOPY) -S $(OUT)/$@ -Obinary $(OUT)/$@.bin
	$(OBJDUMP) -hd $(OUT)/$@ >$(OUT)/$@.asm
	$(OBJDUMP) -stD $(OUT)/$@ >$(OUT)/$@.map

$(LOCAL_COBJS): $(OBJOUT)/%.o: %.c
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(CC) $(LITEOS_CFLAGS) $(LOCAL_FLAGS) -c $< -o $@

$(LOCAL_ASMOBJS): $(OBJOUT)/%.o: %.S
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(CC) $(LITEOS_ASFLAGS) $(LOCAL_FLAGS) -c $< -o $@

$(LIBA): $(LOCAL_COBJS) $(LOCAL_ASMOBJS)
	$(HIDE)$(OBJ_MKDIR)
	$(HIDE)$(AR) $(ARFLAGS) $@ $(LOCAL_COBJS) $(LOCAL_ASMOBJS)

clean:
	$(HIDE)$(RM) $(__OBJS) $(LITEOS_TARGET) $(OUT) $(BUILD) *.bak *~
	$(HIDE)echo "clean $(LITEOS_PLATFORM) finish"

cleanall:
	$(HIDE)rm -rf $(LITEOSTOPDIR)/out
	$(HIDE)echo "clean all"

.PHONY: all lib clean cleanall $(LITEOS_TARGET)
