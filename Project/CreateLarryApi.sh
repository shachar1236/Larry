#!/bin/bash

# a function that check if a directory exists and if not create it
function check_dir() {
    if [ ! -d $1 ]; then
        echo "Creating directory $1"
        mkdir -p $1
    else
        rm -r -f $1
        mkdir -p $1
    fi
}

# check if SandboxGame/LarryApi/ directory exists and if not create it
check_dir SandboxGame/LarryApi

check_dir SandboxGame/LarryApi/bin

check_dir SandboxGame/LarryApi/lib
check_dir SandboxGame/LarryApi/lib/Utils
check_dir SandboxGame/LarryApi/lib/ECS
check_dir SandboxGame/LarryApi/lib/Math
check_dir SandboxGame/LarryApi/lib/EventSystem
check_dir SandboxGame/LarryApi/lib/Components
check_dir SandboxGame/LarryApi/lib/TextureLoader
check_dir SandboxGame/LarryApi/lib/Scripts
check_dir SandboxGame/LarryApi/lib/Layers
check_dir SandboxGame/LarryApi/lib/Input
check_dir SandboxGame/libraries
check_dir SandboxGame/libraries/spdlog
check_dir SandboxGame/libraries/glm

# check if build/Larry/LarryApi/ directory exists 
echo "Copying shared libraries from LarryApi/build/Larry/LarryApi/libraries/spdlog/ to build/Larry/LarryApi/lib"
cp build/Larry/LarryApi/libraries/spdlog/*.so SandboxGame/LarryApi/bin
cp build/Larry/LarryApi/*.so SandboxGame/LarryApi/bin

echo "Copying headers from Larry/src/ to SandboxGame/LarryApi/lib/"
cp Larry/src/Utils/*.h SandboxGame/LarryApi/lib/Utils
cp Larry/src/ECS/*.h SandboxGame/LarryApi/lib/ECS
cp Larry/src/ECS/*.hpp SandboxGame/LarryApi/lib/ECS
cp Larry/src/Math/*.h SandboxGame/LarryApi/lib/Math
cp Larry/src/TextureLoader/*.h SandboxGame/LarryApi/lib/TextureLoader
cp Larry/src/Input/*.h SandboxGame/LarryApi/lib/Input
cp Larry/src/Layers/*.h SandboxGame/LarryApi/lib/Layers

# now copying things that are not shared libraries
# copy all aside from EventSystem.h
cp Larry/src/EventSystem/*.h SandboxGame/LarryApi/lib/EventSystem
rm SandboxGame/LarryApi/lib/EventSystem/EventSystem.h

cp Larry/src/Application/Components/*.h SandboxGame/LarryApi/lib/Components

cp Larry/src/Scripts/*.h SandboxGame/LarryApi/lib/Scripts

# copying libraries
cp -r libraries/spdlog SandboxGame/libraries
cp -r libraries/glm SandboxGame/libraries
