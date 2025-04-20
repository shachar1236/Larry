#!/bin/bash

# a function that check if a directory exists and if not create it
function check_dir() {
    if [ ! -d $1 ]; then
        echo "Creating directory $1"
        rm -r $1
        mkdir -p $1
    else
        rm -r $1
        mkdir -p $1
    fi
}

check_dir GameOut

cp SandboxGame/LarryApi/bin/*.so GameOut/
cp SandboxGame/*.so GameOut/
cp build/LarryEngine GameOut

cp -r Shaders GameOut/Shaders
cp -r SandboxGame/media GameOut/media
