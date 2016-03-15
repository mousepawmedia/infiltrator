#!/bin/bash

#Build Infiltrator for Windows x64 using MSYS2 and MinGW64

#Fail if anything goes haywire and returns non-0 status.
set -e

#The path to MSYS2
MSYS="C:\msys64"
#The compiler to use
COMPILER="g++"
#The target name (must be a folder in obj/ and bin/)
TARGET="Winx64"
#The version of mingw (must be subdirectory of MSYS2 path)
MINGW="mingw64"
#The compiler flags
FLAGS="-std=c++11 -Wall -O2 -mwindows"

#Clean up the directory
rm -r bin/Release/$TARGET

#Copy the necessary resource files.
cp -r winresources/$TARGET bin/Release/$TARGET

#COMPILE WINDOWS RESOURCES
windres winres/info.rc -O coff winres/info.res

#COMPILE OBJECT FILES
$COMPILER $FLAGS -Iinclude -I$MSYS\$MINGW\include -c main.cpp -o obj/Release/$TARGET/main.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/agentDatabase.cpp -o obj/Release/$TARGET/src/agentDatabase.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/agentWindow.cpp -o obj/Release/$TARGET/src/agentWindow.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/customWidgets.cpp -o obj/Release/$TARGET/src/customWidgets.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/fileEncryption.cpp -o obj/Release/$TARGET/src/fileEncryption.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/gameWindow.cpp -o obj/Release/$TARGET/src/gameWindow.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/infoWindow.cpp -o obj/Release/$TARGET/src/infoWindow.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/keyGen.cpp -o obj/Release/$TARGET/src/keyGen.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/playerList.cpp -o obj/Release/$TARGET/src/playerList.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/primaryWindow.cpp -o obj/Release/$TARGET/src/primaryWindow.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -c src/setupWindow.cpp -o obj/Release/$TARGET/src/setupWindow.o $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')

#BUILD RELEASE x64
$COMPILER $FLAGS -mwindows -Iinclude -I$MSYS\$MINGW\include -o bin/Release/$TARGET/bin/infiltrator.exe obj/Release/$TARGET/main.o obj/Release/$TARGET/src/agentDatabase.o obj/Release/$TARGET/src/agentWindow.o obj/Release/$TARGET/src/customWidgets.o obj/Release/$TARGET/src/fileEncryption.o obj/Release/$TARGET/src/gameWindow.o obj/Release/$TARGET/src/infoWindow.o obj/Release/$TARGET/src/keyGen.o obj/Release/$TARGET/src/playerList.o obj/Release/$TARGET/src/primaryWindow.o obj/Release/$TARGET/src/setupWindow.o winres/info.res $(pkg-config gtkmm-3.0 glibmm-2.4 --cflags --libs | sed 's/ -I/ -isystem /g')
