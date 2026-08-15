rm -rf bin/WinCE/data
cp -r assets/data bin/WinCE/data

rm -rf bin/WinCE/data/soundfont.sf2
rm -rf bin/WinCE/data/gamecontrollerdb.txt

wine "C:\Program Files (x86)\Microsoft Device Emulator\1.0\DeviceEmulator.exe" "C:\Program Files (x86)\Windows CE Tools\wce500\Windows Mobile 5.0 Pocket PC SDK\Deviceemulation\0409\PPC_USA.bin" /memsize 128 /skin "C:\Program Files (x86)\Windows CE Tools\wce500\Windows Mobile 5.0 Pocket PC SDK\Deviceemulation\Pocket_pc\Pocket_PC.xml" /sharedfolder "Z:\home\marilyn\Nozomi_Engine"
# yes i know you can read my filepaths, idrc <3