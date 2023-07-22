cmake_minimum_required(VERSION 3.10)
project(msi-fan-control VERSION 1.2)
add_executable(msi-fan-control src/main.cpp src/defines.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS_INIT "-pipe -O2 -std=c11")
set(CMAKE_CXX_FLAGS_INIT "-pipe -O2 -std=c++17")

target_include_directories(msi-fan-control PUBLIC "${PROJECT_BINARY_DIR}")
