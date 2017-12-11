cmake_minimum_required(VERSION 3.8)
project(FYP)

#need to check if this causes any improvements as well
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -msse -msse2 -msse3 -openmp -O3")


set(CMAKE_CXX_STANDARD 11)

set(TRAINING_FILES include/esn/esn.h
                    src/esn/esn.cpp) #files required for ESN training

add_executable(TRAINING ${TRAINING_FILES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include) #including header files