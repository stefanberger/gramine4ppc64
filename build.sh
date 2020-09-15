#!/usr/bin/env bash
#set -x

sudo rm -rf build
mkdir build
meson build -Dmusl=disabled -Ddirect=enabled -Dtests=enabled || { exit 1; }
ninja -v -C build &
ninja_pid=$!
while :; do
	sleep 10
	kill -0 ${ninja_pid}
	if [ $? -ne 0 ]; then
		break
	fi
	echo "Building..."
done
ninja -v -C build || { exit 1; }
exit 0

