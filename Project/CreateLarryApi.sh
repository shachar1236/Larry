#!/bin/bash

rm -r -f SandboxGame/LarryApi/include
cp -r Larry/LarryApi/include SandboxGame/LarryApi/include

# copying libraries
cp -r libraries/spdlog SandboxGame/libraries
cp -r libraries/glm SandboxGame/libraries
