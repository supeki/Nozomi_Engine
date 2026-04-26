make clean
rm -rf bin/Windows
mkdir -p bin/Windows
make -j LINUX_WIN=1
cp -r assets/data bin/Windows/data
mv bin/SDL/JADEFRACTURE.exe bin/Windows
mv assets/Windows/SDL2.dll bin/Windows
mv assets/Windows/SDL2_mixer.dll bin/Windows

make clean
rm -rf bin/Linux
mkdir -p bin/Linux
make -j LINUX=1
cp -r assets/data bin/Linux/data
mv bin/SDL/JADEFRACTURE bin/Linux

make clean
rm -rf assets/NDS/nitrofs
cp -r assets/data assets/NDS/nitrofs
rm -rf bin/NDS
mkdir -p bin/NDS
make -j NDS=1

make clean
rm -rf bin/PSP
mkdir -p bin/PSP
make -j PSP=1
mv JADEFRACTURE.elf bin/PSP
mv EBOOT.PBP bin/PSP
cp -r assets/data bin/PSP/data
rm -rf PARAM.SFO

rmdir bin/SDL
