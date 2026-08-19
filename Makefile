# Nozomi Engine
# A game engine created by Marilyn Nozomi with assistance from xdf, save_as, and trusted others.
# See LICENSE.txt for details on outside usage of this engine, the code, and any bundled assets.

CC = gcc

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Name of the executable (minus the extension!) Nozomi 04-15-2026
EXEC_NAME ?= game
# Assume it's a Windows Executable by default? :3
EXEC_EXT ?= .exe

# Name of the game Nozomi 08-12-2026
GAME_TITLE	?= Nozomi Engine
GAME_SUBTITLE ?= Demo
GAME_AUTHOR	?= Unknown Author
GAME_VERSION ?= v1.0.0

# Assume Windows SDL by default Nozomi 04-15-2026
WINDOWS ?= 1
WIN_32 ?= 0
LINUX ?= 0
LINUX_32 ?= 0
GLFW ?= 0
SDL ?= 1
NDS ?= 0
PSP ?= 0
GBC ?= 0

ifeq ($(WIN_32),1)
WINDOWS = 0
endif

ifeq ($(LINUX),1)
WINDOWS = 0
endif

ifeq ($(GLFW),1)
SDL = 0
WINDOWS = 0
LINUX = 1
endif

ifeq ($(NDS),1)
SDL = 0
WINDOWS = 0
endif

ifeq ($(PSP),1)
WINDOWS = 0
endif

ifeq ($(GBC),1)
WINDOWS = 0
WIN_32 = 0
LINUX = 0
LINUX_32 = 0
SDL = 0
GLFW = 0
NDS = 0
PSP = 0
endif

ifeq ($(SDL),1)
	# src;obj;bin / SDL :D
	INTERFACE = SDL
	i_main = sdl_main
	i_event = sdl_event
	i_input = sdl_input
	i_sound = sdl_sound
	i_system = sdl_system
	i_video = sdl_video

	# Define some stuff!
	DEFINES = -DSDL
	OPTS := $(OPTS) -I.
	LIBS = -lSDL2main -lSDL2 -lSDL2_mixer
	LDFLAGS =  
	
	CFLAGS = $(OPTS) \
			 $(LIBS) \
			 $(DEFINES)
endif

ifeq ($(GLFW),1)
	INTERFACE = GLFW
	i_main = glfw_main
	i_event = glfw_event
	#i_input = glfw_input
	i_sound = glfw_sound
	i_system = glfw_system
	i_video = glfw_video

	DEFINES = -DGLFW
	OPTS := $(OPTS) -I.
	LIBS = -lglfw -lGL -lGLU -lSDL2 -lSDL2_mixer -lm -lc # lol, need a different audio backend
	LDFLAGS =  
	
	CFLAGS = $(OPTS) \
		$(LIBS) \
		$(DEFINES) -g3
endif

ifeq ($(WINDOWS),1)
	CC = x86_64-w64-mingw32-gcc
	OPTS := $(OPTS) -I/usr/local/x86_64-w64-mingw32/include
	LIBS := $(LIBS) -mwindows -lmingw32
	LDFLAGS := $(LDFLAGS) -L/usr/local/x86_64-w64-mingw32/lib
	CFLAGS := $(CFLAGS) -DWINDOWS
endif

ifeq ($(WIN_32),1)
	OPTS := $(OPTS) -I/usr/local/i686-w64-mingw32/include
	LIBS := $(LIBS) -mwindows -lmingw32
	LDFLAGS := $(LDFLAGS) -L/usr/local/i686-w64-mingw32/lib
	CFLAGS := $(CFLAGS) -DWINDOWS -m32
endif

ifeq ($(LINUX),1)
	EXEC_EXT = 

	LIBS := $(LIBS) -lm -lc
	CFLAGS := $(CFLAGS) $(pkg-config sdl2 SDL2_mixer --cflags) -w -DLINUX
	LDFLAGS := $(LDFLAGS) $(pkg-config sdl2 SDL2_mixer --libs)

define LINUX_DESKTOP
[Desktop Entry]
Type=Application
Version=$(GAME_VERSION)
Name=$(GAME_TITLE)
Comment=$(GAME_SUBTITLE)
Exec=$(CURDIR)/bin/Linux/$(EXEC_NAME)$(EXEC_EXT)
Icon=$(CURDIR)/assets/icons/icon.gif
Terminal=false
Categories=Games;
endef

export LINUX_DESKTOP
endif

ifeq ($(LINUX_32),1)
	CFLAGS := $(CFLAGS) -m32
endif

ifeq ($(LINUX_WIN),1)
	OPTS := $(OPTS) -I/usr/local/x86_64-w64-mingw32/include
	LIBS := $(LIBS) -mwindows -lmingw32
	LDFLAGS := $(LDFLAGS) -L/usr/local/x86_64-w64-mingw32/lib
	CFLAGS := $(CFLAGS) -DWINDOWS
endif

ifeq ($(PSP),1)	
	PSPSDK=$(shell psp-config --pspsdk-path)
	CFLAGS := $(CFLAGS) $(shell $(PSPDIR)/bin/sdl2-config --cflags) $(shell ~/pspdev/bin/psp-pkg-config SDL2_mixer --cflags)
	LIBS = $(shell $(PSPDIR)/bin/sdl2-config --libs) $(shell ~/pspdev/bin/psp-pkg-config SDL2_mixer --libs) -lGL -lGLU -lglut -lz -lpspvfpu -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgum -lpspgu -lpspaudiolib -lpspaudio -lpspvram
	
	TARGET = $(EXEC_NAME)
	EXTRA_TARGETS = EBOOT.PBP
	PSP_EBOOT_TITLE = $(GAME_TITLE)
endif

# Nintendo DS port!
ifeq ($(NDS),1)
	export BLOCKSDS			?= /opt/wonderful/thirdparty/blocksds/core
	export BLOCKSDSEXT		?= /opt/wonderful/thirdparty/blocksds/external
	export WONDERFUL_TOOLCHAIN	?= /opt/wonderful
	ARM_NONE_EABI_PATH	?= $(WONDERFUL_TOOLCHAIN)/toolchain/gcc-arm-none-eabi/bin/

	CC = $(ARM_NONE_EABI_PATH)arm-none-eabi-gcc
	
	# src;obj;bin / NDS :D
	INTERFACE = NDS
	i_main = nds_main
	i_event = nds_event
	
	# NDS input isn't handled in its own file!
	#i_input = nds_input
	i_sound = nds_sound
	i_system = nds_system
	i_video = nds_video

	GAME_ICON	?= assets/$(INTERFACE)/icon.gif
	GAME_FULL_TITLE ?= $(GAME_TITLE);$(GAME_SUBTITLE);$(GAME_AUTHOR)
	NDS_NAME = $(EXEC_NAME).nds
	ELF_NAME = $(EXEC_NAME).elf
	NITROFSDIR := assets/$(INTERFACE)/nitrofs
	AUDIODIR := assets/$(INTERFACE)/audio
	AUDIOFILES := $(AUDIODIR)/tech_demo_boss.xm
	
	DEFINES	:= -DARM9 -D__NDS__
	SPECS := $(BLOCKSDS)/sys/crts/ds_arm9.specs
	OPTS := $(OPTS) -I. -I$(BLOCKSDS)/libs/libnds/include -I$(BLOCKSDS)/libs/maxmod/include
	LIBS = -lmm9 -lnds9 -lm -lc
	LDFLAGS = -L$(BLOCKSDS)/libs/libnds/lib -L$(BLOCKSDS)/libs/maxmod/lib

	ARM7ELF	:= $(BLOCKSDS)/sys/arm7/main_core/arm7_dswifi_maxmod.elf

	CFLAGS = $(OPTS) \
			 $(LIBS) \
			 $(DEFINES) \
			 -marm \
			 -mcpu=arm946e-s+nofp \
			 -O3 \
			 -ffast-math \
			 -fomit-frame-pointer \
			 -std=gnu17 \
			 -fmodulo-sched-allow-regmoves \
			 -fno-ira-share-save-slots \
			 -Wwrite-strings \
			 -Wno-format \
			 -Wpointer-arith \
			 -fno-exceptions \
			 -specs=$(SPECS)
endif

ifeq ($(GBC),1)
	ifndef GBDK_HOME
		GBDK_HOME = ~/gbdk
	endif

	CC = $(GBDK_HOME)/bin/lcc -Wm-yo512 -msm83:gb -Wm-yC -Wm-yn"$(GAME_TITLE)"
	EXEC_EXT = .gbc

	INTERFACE = GBC
	i_main = gbc_main
	i_event = gbc_event
	i_sound = gbc_sound
	i_system = gbc_system
	i_video = gbc_video

	DEFINES	:= -DGBC
	CFLAGS := $(CFLAGS) $(DEFINES)
endif

INTERFACE_SRC = $(SRC_DIR)/interface/$(INTERFACE)
INTERFACE_OBJ = $(OBJ_DIR)/$(INTERFACE)
INTERFACE_BIN = $(BIN_DIR)/$(INTERFACE)

ifdef GAME_TITLE
CFLAGS := $(CFLAGS) -DGAME_NAME="\"$(GAME_TITLE)\""
endif

OBJS := $(OBJS) \
		$(OBJ_DIR)/game_main.o \
		$(OBJ_DIR)/game_gfx.o \
		$(OBJ_DIR)/game_font.o \
		$(OBJ_DIR)/game_input.o \
		$(OBJ_DIR)/game_misc.o \
		$(OBJ_DIR)/game_object.o \
		$(OBJ_DIR)/game_player.o \
		$(OBJ_DIR)/game_sound.o \
		$(OBJ_DIR)/game_video.o \
		$(OBJ_DIR)/game_world.o \
		$(OBJ_DIR)/game_dialogue.o \
		$(INTERFACE_OBJ)/$(i_main).o \
		$(INTERFACE_OBJ)/$(i_event).o \
		$(INTERFACE_OBJ)/$(i_sound).o \
		$(INTERFACE_OBJ)/$(i_system).o \
		$(INTERFACE_OBJ)/$(i_video).o \
		$(OBJ_DIR)/bitmap.o
		
ifdef i_input
OBJS := $(OBJS) $(INTERFACE_OBJ)/$(i_input).o
endif

ifeq ($(WINDOWS),1)
define RC_DATA
id ICON "icon.ico"

1 VERSIONINFO
FILEVERSION     1,0,0,0
PRODUCTVERSION  1,0,0,0
BEGIN
	BLOCK "StringFileInfo"
	BEGIN
		BLOCK "040904E4"
		BEGIN
			VALUE "CompanyName", "$(GAME_AUTHOR)"
			VALUE "FileDescription", "$(GAME_TITLE)"
			VALUE "FileVersion", "$(GAME_VERSION)"
			VALUE "InternalName", "$(GAME_TITLE)"
			VALUE "LegalCopyright", "(c) $(shell date +%Y) $(GAME_AUTHOR)"
			VALUE "OriginalFilename", "$(EXEC_NAME)$(EXEC_EXT)"
			VALUE "ProductName", "$(GAME_TITLE)"
			VALUE "ProductVersion", "$(GAME_VERSION)"
		END
	END
	BLOCK "VarFileInfo"
	BEGIN
		VALUE "Translation", 0x409, 1252
	END
END
endef

export RC_DATA
OBJS := $(OBJS) $(INTERFACE_OBJ)/resource.o
endif
		
ifeq ($(NDS),1)
# Start Nintendo DS build requirements!
all: $(INTERFACE_BIN)/$(NDS_NAME)

# Include NitroFS directory!
NDSTOOL_ARGS	:= -d $(NITROFSDIR)
		
$(INTERFACE_BIN)/$(NDS_NAME): $(INTERFACE_BIN)/$(ELF_NAME) $(NITROFSDIR)
	@echo "  NDSTOOL $@"
	$(BLOCKSDS)/tools/ndstool/ndstool -c $@ \
		-7 $(ARM7ELF) -9 $(INTERFACE_BIN)/$(ELF_NAME) \
		-b $(GAME_ICON) "$(GAME_FULL_TITLE)" \
		$(NDSTOOL_ARGS)

$(INTERFACE_BIN)/$(ELF_NAME): $(OBJ_DIR) $(OBJS) $(INTERFACE_OBJ) $(INTERFACE_BIN)
	@echo Linking...
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) \
	-o $(INTERFACE_BIN)/$(ELF_NAME) $(LIBS)

# Create the soundbank!
$(INTERFACE_SRC)/soundbank.h: $(INTERFACE_SRC)
	$(BLOCKSDS)/tools/mmutil/mmutil -o$(NITROFSDIR)/soundbank.bin -h$(INTERFACE_SRC)/soundbank.h -d \
	$(AUDIOFILES)

# End Nintendo DS build requirements!
else
ifeq ($(PSP),1)
include $(PSPSDK)/lib/build.mak
else
# Start generic build requirements!
ifeq ($(LINUX),1)
all: $(INTERFACE_BIN)/$(EXEC_NAME)$(EXEC_EXT) $(INTERFACE_BIN)/game.desktop
else
all: $(INTERFACE_BIN)/$(EXEC_NAME)$(EXEC_EXT)
endif
endif
endif

# Clean up the objects.
clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(INTERFACE_BIN)/*
	
# Make all required directories!
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
	
$(INTERFACE_OBJ): $(OBJ_DIR)
	mkdir -p $(INTERFACE_OBJ)
	
$(INTERFACE_BIN): $(BIN_DIR)
	mkdir -p $(INTERFACE_BIN)
	
# The executable file itself? :3
$(INTERFACE_BIN)/$(EXEC_NAME)$(EXEC_EXT): $(OBJ_DIR) $(INTERFACE_OBJ) $(OBJS) $(INTERFACE_BIN)
	@echo Linking...
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) \
	-o $(INTERFACE_BIN)/$(EXEC_NAME)$(EXEC_EXT) $(LIBS)
	
# Game-related objs!
$(OBJ_DIR)/game_main.o: $(SRC_DIR)/game_main.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_main.h $(SRC_DIR)/game_object.h $(SRC_DIR)/game_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_gfx.o: $(SRC_DIR)/game_gfx.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_gfx.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_font.o: $(SRC_DIR)/game_font.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_font.h $(SRC_DIR)/game_gfx.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_input.o: $(SRC_DIR)/game_input.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_input.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_misc.o: $(SRC_DIR)/game_misc.c $(SRC_DIR)/game_defs.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_object.o: $(SRC_DIR)/game_object.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_object.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_player.o: $(SRC_DIR)/game_player.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_object.h $(SRC_DIR)/game_player.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_sound.o: $(SRC_DIR)/game_sound.c $(SRC_DIR)/game_sound.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_video.o: $(SRC_DIR)/game_video.c $(SRC_DIR)/game_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_world.o: $(SRC_DIR)/game_world.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_gfx.h $(SRC_DIR)/game_world.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(OBJ_DIR)/game_dialogue.o: $(SRC_DIR)/game_dialogue.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_gfx.h $(SRC_DIR)/game_dialogue.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
# Make the interface objs!
$(INTERFACE_OBJ)/$(i_main).o: $(INTERFACE_SRC)/$(i_main).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(INTERFACE_OBJ)/$(i_event).o: $(INTERFACE_SRC)/$(i_event).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
ifdef i_input	
$(INTERFACE_OBJ)/$(i_input).o: $(INTERFACE_SRC)/$(i_input).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
endif
	
ifeq ($(NDS),1)
$(INTERFACE_OBJ)/$(i_sound).o: $(INTERFACE_SRC)/$(i_sound).c $(INTERFACE_SRC)/soundbank.h $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
else
$(INTERFACE_OBJ)/$(i_sound).o: $(INTERFACE_SRC)/$(i_sound).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
endif
	
$(INTERFACE_OBJ)/$(i_system).o: $(INTERFACE_SRC)/$(i_system).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
	
$(INTERFACE_OBJ)/$(i_video).o: $(INTERFACE_SRC)/$(i_video).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)

ifeq ($(WINDOWS),1)
ifeq ($(WIN_32),1)
$(INTERFACE_OBJ)/resource.o:
	rm -rf assets/resource.rc
	@echo "$$RC_DATA" > assets/resource.rc
	i686-w64-mingw32-windres assets/resource.rc -I "assets/icons" -o $(INTERFACE_OBJ)/resource.o
else
$(INTERFACE_OBJ)/resource.o:
	rm -rf assets/resource.rc
	@echo "$$RC_DATA" > assets/resource.rc
	x86_64-w64-mingw32-windres assets/resource.rc -I "assets/icons" -o $(INTERFACE_OBJ)/resource.o
endif
endif

ifeq ($(LINUX),1)
$(INTERFACE_BIN)/game.desktop:
	rm -rf $(INTERFACE_BIN)/game.desktop
	@echo "$$LINUX_DESKTOP" > $(INTERFACE_BIN)/game.desktop
endif
	
# Make the helper stuff :3

$(OBJ_DIR)/bitmap.o: $(SRC_DIR)/helpers/bitmap.c $(SRC_DIR)/helpers/bitmap.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@ $(LIBS)
