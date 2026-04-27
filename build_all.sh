# Clean obj directory to prepare for Windows
make clean

# Remove pre-existing binaries
rm -rf bin/Windows
mkdir -p bin/Windows

# Copy game data
cp -r assets/data bin/Windows/data

# Make and move Windows binary and libaries
make -j LINUX_WIN=1
mv bin/SDL/JADEFRACTURE.exe bin/Windows
cp -r assets/Windows/SDL2.dll bin/Windows/SDL2.dll
cp -r assets/Windows/SDL2_mixer.dll bin/Windows/SDL2_mixer.dll

# Clean obj directory to prepare for Linux
make clean

# Remove pre-existing binaries
rm -rf bin/Linux
mkdir -p bin/Linux

# Copy game data
cp -r assets/data bin/Linux/data

# Make and move Linux binary
make -j LINUX=1
mv bin/SDL/JADEFRACTURE bin/Linux

# Clean obj directory to prepare for NDS
make clean

# Make a new nitrofs directory
rm -rf assets/NDS/nitrofs
mkdir -p assets/NDS/nitrofs

# Remove soundbank header
rm -rf src/interface/NDS/soundbank.h

# Copy game data
cp -r assets/data assets/NDS/nitrofs/data

# NDS doesn't need a soundfont!
rm -rf assets/NDS/nitrofs/data/soundfont.sf2
# NDS also doesn't need the audio folder!
rm -rf assets/NDS/nitrofs/data/audio

# Remove pre-existing binaries
rm -rf bin/NDS
mkdir -p bin/NDS

# Make NDS binary
make -j NDS=1

# Clean obj directory to prepare for PSP
make clean

# Remove pre-existing binaries
rm -rf bin/PSP
mkdir -p bin/PSP

# Copy game data
cp -r assets/data bin/PSP/data

# Use separate audio dir for PSP
rm -rf bin/PSP/data/audio
cp -r assets/PSP/audio bin/PSP/data/audio

# Make and move PSP binary
make -j PSP=1
mv EBOOT.PBP bin/PSP

# Clean up misc. files
rm -rf PARAM.SFO
rm -rf JADEFRACTURE.elf
rm -rf bin/SDL
