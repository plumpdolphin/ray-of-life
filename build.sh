#!/usr/bin/bash


# Binary names
name_l64='rol-linux-x64'
name_l64_bl='rol-linux-x64-borderless'


# Generic build args
src=$(ls src/*.c)
lib='-Llib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11'
inc='-Iinclude -Isrc/include' 


# Clear console for a better view of the build process
clear

# Generate icon.h
python tools/icon_gen.py -i rscs/icon.gif -o include/icon.h -r raylib.h 

# Create binary path if it does not exist
mkdir -p bin


# Linux x64 Windowed
g++ $src $inc $lib $@ -w -o bin/$name_l64 && \
chmod +x bin/$name_l64

# Linux x64 Borderless Fullscreen
g++ $src $inc $lib $@ -DBORDERLESS -w -o bin/$name_l64_bl && \
chmod +x bin/$name_l64_bl