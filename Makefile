# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024-2026
#
BLOCKSDS	?= /opt/blocksds/core
BLOCKSDSEXT	?= /opt/blocksds/external

# User config

NAME			:= tejinaDS
GAME_TITLE		:= tejinaDS
GAME_SUBTITLE	:= LibDSF example

# Source code paths

GFXDIRS		:= graphics
BINDIRS		:= data
AUDIODIRS	:= audio

# Libraries

LIBS		:= -ldsf -lnds9 -lmm9
LIBDIRS		:= $(BLOCKSDS)/libs/libnds \
				$(BLOCKSDS)/libs/maxmod \
			/opt/wonderful/thirdparty/blocksds/external/libdsf

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
