cmake_minimum_required(VERSION 3.26) # Set the minimum required CMake version

project(GameScripts) # Set the project name

add_library(GameScripts SHARED
    scripts/Scripts.cpp
    scripts/TestScript.cpp)

set(LarrySharedLibraries
    LarryUtils
    LarryMath
    LarryECS
)

# Optionally set include directories for the library's own headers
target_include_directories(GameScripts
                           PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/libraries # If you have private headers
                           ${CMAKE_CURRENT_SOURCE_DIR}/libraries/spdlog/include # If you have private headers
                           ${CMAKE_CURRENT_SOURCE_DIR}/LarryApi/lib # If you have private headers
                           ${CMAKE_CURRENT_SOURCE_DIR}/scripts)      # Or just the current source dir

# SET(CMAKE_INSTALL_RPATH "LarryApi/lib")

target_link_directories(GameScripts PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/LarryApi/bin)

target_link_libraries(GameScripts PUBLIC ${LarrySharedLibraries})
# Optionally set compiler flags specific to the library
# target_compile_options(mylibrary PRIVATE -Wall -Wextra)

# Optionally link the library against other libraries it depends on
# target_link_libraries(mylibrary PRIVATE other_library)
