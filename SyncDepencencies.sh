#!/bin/bash

$(pwd)/Engine/vendor/shaderc/utils/git-sync-deps

# temp
apt install xorg-dev
apt install libxrandr-dev
apt install libx11-dev
apt install libxinerama-dev
apt install libxcursor-dev
apt install libxi-dev

apt install pkg-config mesa-utils libglu1-mesa-dev mesa-common-dev 
apt install libmpg123-dev libao-dev 
