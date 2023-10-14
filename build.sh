#!/bin/sh

set -xe

CC=g++
CFLAGS="-Wall -Wextra -std=c++17 `pkg-config --cflags sdl2`"
LIBS="`pkg-config --libs sdl2` -lGL -I./glm/ -lGLEW"
SRC="chapter1.cc gfx.cpp game.cpp"
BIN=out

$CC $CFLAGS $SRC -o $BIN $LIBS
