#!/bin/bash

./tools/linux/premake4 gmake
make -C build config=release
