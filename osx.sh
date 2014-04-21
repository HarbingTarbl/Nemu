#!/bin/bash

./tools/osx/premake4 gmake
make -C build config=release
