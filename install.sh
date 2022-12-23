#!/bin/sh

meson setup --reconfigure build
ninja -C build
cp ./build/libmint.a /usr/local/lib/libmint.a
cp -r incl/mint /usr/local/include
