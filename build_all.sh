# Clean obj directory to prepare for Windows (x64)
make clean

# Remove pre-existing binaries
rm -rf bin/Win64
mkdir -p bin/Win64

# Copy game data
cp -r assets/data bin/Win64/data

# Copy required licenses/credits
cp -r assets/credits/SDL2_LICENSE.txt bin/Win64/SDL2_LICENSE.txt

# Make and move Windows binary and libraries
make -j2 LINUX_WIN=1
cp -r assets/Win64/*.dll bin/Win64/

# Clean obj directory to prepare for Windows (x86)
make clean WIN_32=1

# Remove pre-existing binaries
rm -rf bin/Win32
mkdir -p bin/Win32

# Copy game data
cp -r assets/data bin/Win32/data

# Copy required licenses/credits
cp -r assets/credits/SDL2_LICENSE.txt bin/Win32/SDL2_LICENSE.txt

# Make and move Windows binary and libraries
make -j2 LINUX_WIN=1 WIN_32=1
cp -r assets/Win32/*.dll bin/Win32/

# Clean obj directory to prepare for DOS
make clean DOS=1

# Remove pre-existing binaries
rm -rf bin/DOS
mkdir -p bin/DOS

# Copy game data
cp -r assets/data bin/DOS/data
rm -rf bin/DOS/data/soundfont.sf2
rm -rf bin/DOS/data/gamecontrollerdb.txt
rm -rf bin/DOS/data/audio

# Copy required licenses/credits
# add later

# Make and move DOS binary and stuff
make -j2 DOS=1
cp -r assets/DOS/* bin/DOS/
mv bin/DOS/audio bin/DOS/data/audio

# Clean obj directory to prepare for Linux (SDL)
make clean LINUX=1

# Remove pre-existing binaries
rm -rf bin/Linux
mkdir -p bin/Linux

# Copy libraries
mkdir -p bin/Linux/lib
cp -L /lib/x86_64-linux-gnu/libSDL2-2.0.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libSDL2_mixer-2.0.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libxmp.so.4 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libvorbisfile.so.3 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libFLAC.so.14 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libmpg123.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libopusfile.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libvorbis.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libogg.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libopus.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libvorbisenc.so.2 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libmp3lame.so.0 bin/Linux/lib/
cp -L /lib/x86_64-linux-gnu/libSDL2_net-2.0.so.0 bin/Linux/lib/

# Copy game data
cp -r assets/data bin/Linux/data

# Copy required licenses/credits
cp -r assets/credits/SDL2_LICENSE.txt bin/Linux/SDL2_LICENSE.txt

# Make and move Linux binary
make -j2 LINUX=1
mv bin/SDL/game bin/Linux

# Clean obj directory to prepare for Linux (SDL) (x86)
make clean LINUX=1 LINUX_32=1

# Remove pre-existing binaries
rm -rf bin/Linux32
mkdir -p bin/Linux32

# Copy libraries
mkdir -p bin/Linux32/lib
cp -L /usr/lib/i386-linux-gnu/libSDL2-2.0.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libSDL2_mixer-2.0.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libSDL2_net-2.0.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libxmp.so.4 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libvorbisfile.so.3 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libFLAC.so.14 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libmpg123.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libopusfile.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libvorbis.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libogg.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libopus.so.0 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libvorbisenc.so.2 bin/Linux32/lib/
cp -L /usr/lib/i386-linux-gnu/libmp3lame.so.0 bin/Linux32/lib/

# Copy game data
cp -r assets/data bin/Linux32/data

# Copy required licenses/credits
cp -r assets/credits/SDL2_LICENSE.txt bin/Linux32/SDL2_LICENSE.txt

# Make and move Linux binary
make -j2 LINUX=1 LINUX_32=1

# Clean obj directory to prepare for Linux (GLFW)
make clean GLFW=1

# Remove pre-existing binaries
rm -rf bin/GLFW
mkdir -p bin/GLFW

# Copy game data
cp -r assets/data bin/GLFW/data

# Copy required licenses/credits
# cp -r assets/credits bin/GLFW/credits

# Make GLFW binary
make -j2 SDL=0 LINUX=1 GLFW=1

# Clean obj directory to prepare for NDS
make clean NDS=1

# Make a new nitrofs directory
rm -rf assets/NDS/nitrofs
mkdir -p assets/NDS/nitrofs

# Remove soundbank header
rm -rf src/interface/NDS/soundbank.h

# Copy game data
cp -r assets/data assets/NDS/nitrofs/data
rm -rf assets/NDS/icon.gif
cp -r assets/icons/icon.gif assets/NDS/icon.gif

# Remove and copy audio data
rm -rf assets/NDS/audio
cp -r assets/data/audio assets/NDS/audio

# NDS doesn't need a soundfont!
rm -rf assets/NDS/nitrofs/data/soundfont.sf2
# NDS also doesn't need the audio folder!
rm -rf assets/NDS/nitrofs/data/audio
# uhhh it also doesn't need gamecontrollerdb.txt lmfaooo
rm -rf assets/NDS/nitrofs/data/gamecontrollerdb.txt

# Remove pre-existing binaries
rm -rf bin/NDS
mkdir -p bin/NDS

# Copy required licenses/credits
cp -r assets/credits/NDS_LICENSES.txt bin/NDS/NDS_LICENSES.txt

# Make NDS binary
make -j2 NDS=1

# Clean obj directory to prepare for PSP
make clean PSP=1

# Remove pre-existing binaries
rm -rf bin/PSP
mkdir -p bin/PSP

# Copy game data
cp -r assets/data bin/PSP/data

# Copy PSP border graphic
cp -r assets/PSP/border.bmp bin/PSP/data/border.bmp

# Make and move PSP binary
make -j2 PSP=1
mv EBOOT.PBP bin/PSP

# Generate licenses for PSP ver. so my ass doesn't get sued <3
psp-create-license-directory sdl2 sdl2-mixer
mv third-party-licenses bin/PSP

# Clean up misc. files
rm -rf PARAM.SFO
rm -rf game.elf
rm -rf bin/SDL
rm -rf obj/*
