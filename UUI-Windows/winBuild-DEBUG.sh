#!/bin/bash
# this is a script to build Universal UI on MINGW64 windows with GCC

cd "$(dirname "$0")"
rm -R -f ./build/windows
mkdir ./build/windows

# copy static resources into build resources folder
cp -a ./resources/. ./build/windows/resources/

# copy dll files into build folder
cp -a ./misc/dll/. ./build/windows/

g++ ./source/main.cpp ./source/glad.c -o ./build/windows/main.exe -mconsole -lopengl32 -lgdi32 -lglfw3dll -lfreetype `pkg-config --cflags freetype2` -I./source/include

echo !DONE!