CC = gcc

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Name of the executable (minus the extension!) Nozomi 04-15-2026
EXEC_NAME = JADEFRACTURE
# Assume it's a Windows Executable by default? :3
EXEC_EXT = .exe

# Assume Windows SDL by default Nozomi 04-15-2026
WINDOWS ?= 1
LINUX ?= 0
LINUX_WIN ?= 0
SDL ?= 1
NDS ?= 0
PSP ?= 0

ifeq ($(LINUX),1)
WINDOWS = 0
endif

ifeq ($(LINUX_WIN),1)
WINDOWS = 0
CC = x86_64-w64-mingw32-gcc
endif

ifeq ($(NDS),1)
SDL = 0
WINDOWS = 0
endif

ifeq ($(PSP),1)
WINDOWS = 0
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
			 $(DEFINES) \
			 -O3
endif

ifeq ($(WINDOWS),1)
	OPTS := $(OPTS) -I/mingw64/include
	LIBS := $(LIBS) -mwindows -lmingw32
	LDFLAGS := $(LDFLAGS) -L/mingw64/lib
	CFLAGS := $(CFLAGS) -DWINDOWS
endif

ifeq ($(LINUX),1)
	EXEC_EXT = 

	CFLAGS := $(CFLAGS) $(pkg-config sdl2 SDL2_mixer --cflags) -w
	LDFLAGS := $(LDFLAGS) $(pkg-config sdl2 SDL2_mixer --libs)
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
	
	TARGET = JADEFRACTURE
	EXTRA_TARGETS = EBOOT.PBP
	PSP_EBOOT_TITLE = JADEFRACTURE
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

	GAME_TITLE	:= JADEFRACTURE
	GAME_SUBTITLE	:= An indie RPG by
	GAME_AUTHOR	:= Marilyn Nozomi
	GAME_ICON	:= assets/NDS/icon.gif
	GAME_FULL_TITLE := $(GAME_TITLE);$(GAME_SUBTITLE);$(GAME_AUTHOR)
	NDS_NAME = $(EXEC_NAME).nds
	ELF_NAME = $(EXEC_NAME).elf
	NITROFSDIR := assets/$(INTERFACE)/nitrofs
	AUDIODIR := assets/$(INTERFACE)/audio
	AUDIOFILES := $(AUDIODIR)/presentation.it
	
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

INTERFACE_SRC = $(SRC_DIR)/interface/$(INTERFACE)
INTERFACE_OBJ = $(OBJ_DIR)/$(INTERFACE)
INTERFACE_BIN = $(BIN_DIR)/$(INTERFACE)

OBJS := $(OBJS) \
		$(OBJ_DIR)/game_main.o \
		$(OBJ_DIR)/game_gfx.o \
		$(OBJ_DIR)/game_input.o \
		$(OBJ_DIR)/game_misc.o \
		$(OBJ_DIR)/game_object.o \
		$(OBJ_DIR)/game_player.o \
		$(OBJ_DIR)/game_sound.o \
		$(OBJ_DIR)/game_video.o \
		$(OBJ_DIR)/game_world.o \
		$(INTERFACE_OBJ)/$(i_main).o \
		$(INTERFACE_OBJ)/$(i_event).o \
		$(INTERFACE_OBJ)/$(i_sound).o \
		$(INTERFACE_OBJ)/$(i_system).o \
		$(INTERFACE_OBJ)/$(i_video).o
		
ifdef i_input
OBJS := $(OBJS) $(INTERFACE_OBJ)/$(i_input).o
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
all: $(INTERFACE_BIN)/$(EXEC_NAME)$(EXEC_EXT)
endif
endif

# Clean up the objects.
clean:
	rm -rf $(OBJ_DIR)/*
	
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
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_gfx.o: $(SRC_DIR)/game_gfx.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_gfx.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_input.o: $(SRC_DIR)/game_input.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_input.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_misc.o: $(SRC_DIR)/game_misc.c $(SRC_DIR)/game_defs.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_object.o: $(SRC_DIR)/game_object.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_object.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_player.o: $(SRC_DIR)/game_player.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_object.h $(SRC_DIR)/game_player.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(OBJ_DIR)/game_sound.o: $(SRC_DIR)/game_sound.c $(SRC_DIR)/game_sound.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_video.o: $(SRC_DIR)/game_video.c $(SRC_DIR)/game_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(OBJ_DIR)/game_world.o: $(SRC_DIR)/game_world.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_gfx.h $(SRC_DIR)/game_world.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
# Make the interface objs!
$(INTERFACE_OBJ)/$(i_main).o: $(INTERFACE_SRC)/$(i_main).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(INTERFACE_OBJ)/$(i_event).o: $(INTERFACE_SRC)/$(i_event).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

ifdef i_input	
$(INTERFACE_OBJ)/$(i_input).o: $(INTERFACE_SRC)/$(i_input).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
endif

ifeq ($(NDS),1)
$(INTERFACE_OBJ)/$(i_sound).o: $(INTERFACE_SRC)/$(i_sound).c $(INTERFACE_SRC)/soundbank.h $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
else
$(INTERFACE_OBJ)/$(i_sound).o: $(INTERFACE_SRC)/$(i_sound).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
endif
	
$(INTERFACE_OBJ)/$(i_system).o: $(INTERFACE_SRC)/$(i_system).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(INTERFACE_OBJ)/$(i_video).o: $(INTERFACE_SRC)/$(i_video).c $(INTERFACE_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@