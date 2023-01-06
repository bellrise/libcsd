#!/bin/sh

[ -d "build" ] && {
	meson compile -C build
} || {
	meson setup build
	meson compile -C build
}

cp ./build/libcsd.a /usr/local/lib/libcsd.a
cp -r incl/libcsd /usr/include
