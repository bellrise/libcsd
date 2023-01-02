#!/bin/sh

meson setup --reconfigure build
ninja -C build
cp ./build/libcsd.a /usr/local/lib/libcsd.a
cp -r incl/libcsd /usr/local/include
