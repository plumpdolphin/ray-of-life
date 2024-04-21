#!/usr/bin/bash
name_bin=rol

clear
mkdir -p ./bin
g++ $(ls src/*.c) $@ -L./lib -I./include -I./src/include -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -w -o ./bin/$name_bin && \
chmod +x ./bin/$name_bin