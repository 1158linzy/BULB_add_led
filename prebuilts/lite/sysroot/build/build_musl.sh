#!/bin/bash

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

set -e

readonly MUSL_DIR="${PWD}/musl"
readonly MUSL_CUR_DIR="./musl"
readonly MUSL_DEBUG_DIR="${PWD}/musl_debug"
readonly MUSL_DEBUG_CUR_DIR="./musl_debug"
readonly MUSL_PATCH="${PWD}/musl-debug.patch"
readonly MUSL_SOURCE="${PWD}/../../../../third_party/musl/*"
readonly LITEOSTOPDIR="${PWD}/../../../../kernel/liteos_a"
readonly PREFIX_M="${PWD}/../../../../kernel/liteos_m/kal/posix"
readonly OPTIMIZE_DIR="${PWD}/optimized-routines"
readonly OPTIMIZE_SOURCE="${PWD}/../../../../third_party/optimized-routines/*"

BUILD=`gcc -dumpmachine`
HOST=`gcc -dumpmachine`
TARGET=arm-linux-ohoseabi
CROSS_COMPILER=arm-linux-ohoseabi-
CFLAGS_FOR_TARGET="-O2 -Wall -fstack-protector-strong -D_FORTIFY_SOURCE=2 -Wl,-z,relro,-z,now,-z,noexecstack -Wtrampolines"

TARGET_LIBC_A="${PWD}/libc_opt.a"
TARGET_LIBC_SO="${PWD}/libc_opt.so"

PARALLEL=`grep ^processor /proc/cpuinfo|wc -l`
LITEOS_COMPILER_PATH=`${LITEOSTOPDIR}/tools/build/mk/get_compiler_path.sh  ${CROSS_COMPILER} ${LITEOSTOPDIR}`

export SYSROOT="${PWD}/../../../../prebuilts/gcc/linux-x86/arm/arm-linux-ohoseabi-gcc/target"
export PATH="${LITEOS_COMPILER_PATH}/bin:${PATH}"

# prepare to build musl
if [ -d "${MUSL_DIR}" ]; then
	rm -r ${MUSL_DIR}
fi
mkdir -p  ${MUSL_DIR}
cp -r ${MUSL_SOURCE} ${MUSL_DIR}

# prepare to build optimized-routines for improve musl performance
if [ -d "${OPTIMIZE_DIR}" ]; then
	rm -r ${OPTIMIZE_DIR}
fi
mkdir -p  ${OPTIMIZE_DIR}
cp -r ${OPTIMIZE_SOURCE} ${OPTIMIZE_DIR}

# build optimized-routines
function build_opt()
{
	pushd ${OPTIMIZE_DIR}
	# generate the config.mk file required for compiling the optimized-routines lib.
	exec 3>&1 1>config.mk
	cat << EOF
# This version of config.mk was generated by:
# build_musl.sh
# The default configuration(CFLAGS) is from optimized-routines.
# Any changes made here will be lost if build_musl.sh re-run

# Target architecture: aarch64, arm or x86_64
ARCH = arm

# Subprojects to build
SUBS = string

# Use for cross compilation with gcc/clang.
CROSS_COMPILE = ${CROSS_COMPILER}

# Compiler for the target
CC = ${LITEOS_COMPILER_PATH}/bin/${CROSS_COMPILER}gcc
CFLAGS = -std=c99 -pipe -O3 -mfpu=neon-vfpv4 -mfloat-abi=softfp -march=armv7-a
CFLAGS += -Wall -Wno-missing-braces

# Enable debug info.
#HOST_CFLAGS += -g
#CFLAGS += -g

# Use with gcc.
CFLAGS += -frounding-math -fexcess-precision=standard -fno-stack-protector
CFLAGS += -ffp-contract=fast -fno-math-errno

EOF
	exec 1>&3 3>&-
	make -j
	popd
}

# generate the final libc.so file.
function install_opt_libc()
{
	if [ -e "${TARGET_LIBC_A}" ]; then
		rm ${TARGET_LIBC_A}
	fi
	if [ -e "${TARGET_LIBC_SO}" ]; then
		rm ${TARGET_LIBC_SO}
	fi

	# make libc_opt.a libc_opt.so by makefile
	make musldir=$2 -j

	# copy libc_opt.so libc_opt.a to a specified directory to replace the original libc.so libc.a file.
	if [ -e "${TARGET_LIBC_A}" ] && [ -e "${TARGET_LIBC_SO}" ]; then
		rm $1/lib/libc.a $1/lib/libc.so
		cp ${TARGET_LIBC_A} $1/lib/libc.a
		cp ${TARGET_LIBC_SO} $1/lib/libc.so
	else
		printf "build libc_opt error!"
	fi
}

function install_debug_musl()
{
	mkdir -p  ${SYSROOT}/usr/include/debug
	mkdir -p  ${MUSL_DIR}/lib/debug
	cp -f ${MUSL_DEBUG_DIR}/include/debug.h ${SYSROOT}/usr/include/debug
	cp -f ${MUSL_DEBUG_DIR}/lib/libc.* ${MUSL_DIR}/lib/debug
}

# build musl
function build_musl()
{
	pushd $2
	if [ "$2" = "${MUSL_DEBUG_DIR}" ]; then
		patch -p1 < ${MUSL_PATCH}
	fi
	#CC="${LITEOS_COMPILER_PATH}/bin/${CROSS_COMPILER}gcc $1" ./configure  --with-headers=${SYSROOT}/usr/include  --build=${BUILD} --host=${TARGET}
	CC="${LITEOS_COMPILER_PATH}/bin/${CROSS_COMPILER}gcc $1" ./configure --prefix=${SYSROOT}/ --with-headers=${SYSROOT}/usr/include --build=${BUILD} --host=${TARGET} --libdir=${SYSROOT}/usr/lib --includedir=${SYSROOT}/usr/include
	#CC="${LITEOS_COMPILER_PATH}/bin/${CROSS_COMPILER}gcc $1 -mcpu=cortex-a7 -mfloat-abi=soft" ./configure --prefix=${SYSROOT}/ --with-heades=${SYSROOT}/usr/include --build=${BUILD} --host=${TARGET} --libdir=${SYSROOT}/usr/lib/a7_soft --includedir=${SYSROOT}/usr/include
	#CC="${LITEOS_COMPILER_PATH}/bin/${CROSS_COMPILER}gcc $1 -mcpu=cortex-a7 -mfloat-abi=softfp -mfpu=neon-vfpv4" ./configure --prefix=${SYSROOT}/ --with-heades=${SYSROOT}/usr/include --build=${BUILD} --host=${TARGET} --libdir=${SYSROOT}/usr/lib/a7_soft --includedir=${SYSROOT}/usr/include
	make -j ${PARALLEL} CROSS=${CROSS_COMPILER}
	popd
}

# prepare linux compat headers for musl
sh thirdparty_headers.sh ${MUSL_DIR}

if [ $# -eq 1 -a "$1" = "-m" ]; then
pushd ${MUSL_DIR}
CC="${LITEOS_COMPILER_PATH}/bin/${CROSS_COMPILER}gcc ${CFLAGS_FOR_TARGET}" ./configure --with-headers=${SYSROOT}/usr/include  --build=${BUILD} --host=${TARGET} --prefix=${PREFIX_M}
cp ${MUSL_DIR}/kernel_m/include/alltypes.h.in ${MUSL_DIR}/include/alltypes.h.in
cp ${MUSL_DIR}/kernel_m/arch/arm/bits/alltypes.h.in ${MUSL_DIR}/arch/arm/bits/alltypes.h.in
make -f - -j ${PARALLEL} CROSS=${CROSS_COMPILER} install-headers <<EOF
include config.mak
\$(DESTDIR)\$(includedir)/%: \$(srcdir)/kernel_m/include/%
	\$(HIDE)\$(INSTALL) -D -m 644 \$< \$@
include Makefile
EOF
popd
else
# build optimized-routines
build_opt

# build musl
build_musl "${CFLAGS_FOR_TARGET}" "${MUSL_DIR}"
# generate the final libc.so file.
install_opt_libc "${MUSL_DIR}" "${MUSL_CUR_DIR}"

# prepare for musl debug
CFLAGS_DEBUG_FOR_TARGET=${CFLAGS_FOR_TARGET}" -funwind-tables -rdynamic"
UNWIND_H="${PWD}/../../../../prebuilts/gcc/linux-x86/arm/arm-linux-ohoseabi-gcc/lib/gcc/arm-linux-musleabi/7.3.0/include/unwind*"
if [ -d "${MUSL_DEBUG_DIR}" ]; then
	rm -r ${MUSL_DEBUG_DIR}
fi
mkdir -p  ${MUSL_DEBUG_DIR}
cp -r ${MUSL_SOURCE} ${MUSL_DEBUG_DIR}
cp -f ${UNWIND_H} ${MUSL_DEBUG_DIR}/include
# prepare linux compat headers for musl debug
sh thirdparty_headers.sh ${MUSL_DEBUG_DIR}
# build musl_debug
build_musl "${CFLAGS_DEBUG_FOR_TARGET}" "${MUSL_DEBUG_DIR}"
# generate the final libc.so file.
install_opt_libc "${MUSL_DEBUG_DIR}" "${MUSL_DEBUG_CUR_DIR}"
# generate final musl debug shared library
install_debug_musl
fi

