# In my_project/core/CMakeLists.txt (or my_project/CMakeLists.txt)
cmake_minimum_required(VERSION 3.0)
project(LarryApi)

# ---------- Larry Utils -------------
add_library(LarryUtils SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/Log.cpp
            )

target_include_directories(LarryUtils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/spdlog/include)

target_include_directories(LarryUtils
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/)

target_link_libraries(LarryUtils PUBLIC spdlog)

# ---------- Larry ECS -------------
add_library(LarryECS SHARED
            ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/TypesBitmap.cpp
            )

target_include_directories(LarryECS
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../src/ECS/)

target_include_directories(LarryECS
                            PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/../src/Utils/)

target_link_libraries(LarryECS PUBLIC spdlog)

# ---------- Larry Math -------------
add_library(LarryMath SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/Math.cpp
            )

target_include_directories(LarryMath
                           PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../src/Math/)

target_include_directories(LarryMath PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../libraries/)
