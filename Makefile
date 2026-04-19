CC = gcc

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Name of the executable (minus the extension!) Nozomi 04-15-2026
EXEC_NAME = JADEFRACTURE
# Assume it's a Windows Executable by default? :3
EXEC_EXT = exe

# Assume SDL by default Nozomi 04-15-2026
SDL = 1

ifdef SDL
	# src;obj;bin / SDL :D
	INTERFACE = SDL
	i_main = sdl_main
	i_event = sdl_event
	i_system = sdl_system
	i_video = sdl_video

	# Define some stuff!
	DEFINES = -DSDL
	OPTS := $(OPTS) -I. -I/mingw64/include -I/mingw64/include/SDL2
	LIBS = -lmingw32 -lSDL2main -lSDL2 -mwindows
	LDFLAGS = -L/mingw64/lib -L/mingw64/lib/SDL2 
	
	CFLAGS = $(OPTS) \
			 $(LIBS) \
			 $(DEFINES)
endif

# Nintendo DS port!
ifdef NDS
	export BLOCKSDS			?= /opt/blocksds/core
	export BLOCKSDSEXT		?= /opt/blocksds/external
	export WONDERFUL_TOOLCHAIN	?= /opt/wonderful
	ARM_NONE_EABI_PATH	?= $(WONDERFUL_TOOLCHAIN)/toolchain/gcc-arm-none-eabi/bin/

	CC = $(ARM_NONE_EABI_PATH)arm-none-eabi-gcc
	
	# src;obj;bin / NDS :D
	INTERFACE = NDS
	i_main = nds_main
	i_event = nds_event
	i_system = nds_system
	i_video = nds_video

	GAME_TITLE	:= JADEFRACTURE
	GAME_SUBTITLE	:= An indie RPG by
	GAME_AUTHOR	:= Marilyn Nozomi
	GAME_ICON	:= assets/NDS/icon.gif
	GAME_FULL_TITLE := $(GAME_TITLE);$(GAME_SUBTITLE);$(GAME_AUTHOR)
	NDS_NAME = $(EXEC_NAME).nds
	ELF_NAME = $(EXEC_NAME).elf
	NITROFSDIR := src/interface/$(INTERFACE)/nitrofs

	DEFINES	:= -DARM9 -D__NDS__
	SPECS := $(BLOCKSDS)/sys/crts/ds_arm9.specs
	OPTS := $(OPTS) -I. -I$(BLOCKSDS)/libs/libnds/include -I$(BLOCKSDS)/libs/maxmod/include
	LIBS = -lmm9 -lnds9 -lm -lc
	LDFLAGS = -L$(BLOCKSDS)/libs/libnds/lib -L$(BLOCKSDS)/libs/maxmod/lib
	SFLAGS=-g $(OPTS)

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
		$(OBJ_DIR)/game_misc.o \
		$(OBJ_DIR)/game_video.o \
		$(INTERFACE_OBJ)/$(i_main).o \
		$(INTERFACE_OBJ)/$(i_event).o \
		$(INTERFACE_OBJ)/$(i_system).o \
		$(INTERFACE_OBJ)/$(i_video).o
		
ifdef NDS
# Start Nintendo DS build requirements!
all: $(INTERFACE_BIN)/$(NDS_NAME)

# Start NitroFS!
ifneq ($(strip $(NITROFSDIR)),)
# Additional arguments for ndstool
NDSTOOL_ARGS	:= -d $(NITROFSDIR)

ifneq ($(SOURCES_AUDIO),)
	NDSTOOL_ARGS	+= -d $(SOUNDBANKDIR)
endif

# Make the NDS ROM depend on the filesystem only if it is needed
$(INTERFACE_BIN)/$(NDS_NAME): $(NITROFSDIR)
endif
# End NitroFS!
		
$(INTERFACE_BIN)/$(NDS_NAME): $(INTERFACE_BIN)/$(ELF_NAME)
	@echo "  NDSTOOL $@"
	$(BLOCKSDS)/tools/ndstool/ndstool -c $@ \
		-7 $(ARM7ELF) -9 $(INTERFACE_BIN)/$(ELF_NAME) \
		-b $(GAME_ICON) "$(GAME_FULL_TITLE)" \
		$(NDSTOOL_ARGS)


$(INTERFACE_BIN)/$(ELF_NAME): $(OBJ_DIR) $(OBJS) $(INTERFACE_OBJ) $(INTERFACE_BIN)
	@echo Linking...
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(INTERFACE_OBJ) \
	-o $(INTERFACE_BIN)/$(ELF_NAME) $(LIBS)

# End Nintendo DS build requirements!
else
# Start generic build requirements!
all: $(INTERFACE_BIN)/$(EXEC_NAME).$(EXEC_EXT)
endif

# Clean up the directories.
clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(INTERFACE_BIN)/$(EXEC_NAME).$(EXEC_EXT)
	
# Make all required directories!
$(OBJ_DIR):
	@mkdir $(OBJ_DIR)
	
$(BIN_DIR):
	@mkdir $(BIN_DIR)
	
$(INTERFACE_OBJ):
	@mkdir $(INTERFACE_OBJ)
	
$(INTERFACE_BIN):
	@mkdir $(INTERFACE_BIN)
	
# The executable file itself? :3
$(INTERFACE_BIN)/$(EXEC_NAME).$(EXEC_EXT): $(OBJ_DIR) $(INTERFACE_OBJ) $(OBJS) $(INTERFACE_BIN)
	@echo Linking...
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) \
	-o $(INTERFACE_BIN)/$(EXEC_NAME).$(EXEC_EXT) $(LIBS)
	
# Game-related objs!
$(OBJ_DIR)/game_main.o: $(SRC_DIR)/game_main.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_main.h $(SRC_DIR)/game_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_gfx.o: $(SRC_DIR)/game_gfx.c $(SRC_DIR)/game_defs.h $(SRC_DIR)/game_gfx.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_misc.o: $(SRC_DIR)/game_misc.c $(SRC_DIR)/game_defs.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(OBJ_DIR)/game_video.o: $(SRC_DIR)/game_video.c $(SRC_DIR)/game_video.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
# Make the interface objs!
$(INTERFACE_OBJ)/$(i_main).o: $(INTERFACE_SRC)/$(i_main).c $(INTERFACE_SRC)/$(i_main).h
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(INTERFACE_OBJ)/$(i_event).o: $(INTERFACE_SRC)/$(i_event).c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@
	
$(INTERFACE_OBJ)/$(i_system).o: $(INTERFACE_SRC)/$(i_system).c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(INTERFACE_OBJ)/$(i_video).o: $(INTERFACE_SRC)/$(i_video).c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(WFLAGS) -c $< -o $@