#!/bin/bash

rm -r -f SandboxGame/LarryApi/lib
cp -r Larry/LarryApi/include SandboxGame/LarryApi/lib
# rm SandboxGame/LarryApi/lib/gl.h

# copying libraries
cp -r libraries/spdlog SandboxGame/libraries
cp -r libraries/glm SandboxGame/libraries
