cmake_minimum_required(VERSION 3.8)
project(ICA)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D EIGEN_NO_DEBUG -openmp -g")
#set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -msse -msse2 -msse3 -openmp -O3")
#for running ICA normally
set(SOURCE_FILES src/main.cpp
        include/audioFileHelpers.h
        src/audioFileHelpers.cpp
        include/ICAHelpers.h
        src/ICAHelpers.cpp
        include/ICA.h
        src/ICA.cpp
        )

#for running my unit tests
set(UNIT_TESTS test/unitICA.cpp
        include/ICAHelpers.h
        src/ICAHelpers.cpp
        include/ICA.h
        src/ICA.cpp
        include/audioFileHelpers.h
        src/audioFileHelpers.cpp
        )

#for testing my audio functions
set(AUDIO_TESTS test/audioTest.cpp
        include/audioFileHelpers.h
        src/audioFileHelpers.cpp)

#for my evolutionary algorithm stuff
set(NSGA include/nsga-ii/nonDomSort.h
        src/nsga-ii/nonDomSort.cpp
        include/nsga-ii/operators.h
        src/nsga-ii/operators.cpp
        include/nsga-ii/hyperParameters.h
        include/nsga-ii/nsga-ii.h
        src/nsga-ii/nsga-ii.cpp
        include/nsga-ii/objectives.h
        src/nsga-ii/objectives.cpp
        include/nsga-ii/nsgaMain.h
        src/nsga-ii/nsgaMain.cpp
        include/audioFileHelpers.h
        src/audioFileHelpers.cpp
        include/ICAHelpers.h
        src/ICAHelpers.cpp)


set(NSGA_TEST include/nsga-ii/nonDomSort.h
        src/nsga-ii/nonDomSort.cpp
        include/nsga-ii/operators.h
        src/nsga-ii/operators.cpp
        include/nsga-ii/hyperParameters.h
        include/nsga-ii/nsga-ii.h
        src/nsga-ii/nsga-ii.cpp
        include/nsga-ii/objectives.h
        src/nsga-ii/objectives.cpp
        include/audioFileHelpers.h
        src/audioFileHelpers.cpp
        include/ICAHelpers.h
        src/ICAHelpers.cpp
        test/nsga_test.cpp)

set(NSGA_SCHAFFER include/nsga-ii/nonDomSort.h
        src/nsga-ii/nonDomSort.cpp
        include/nsga-ii/operators.h
        src/nsga-ii/operators.cpp
        include/nsga-ii/hyperParameters.h
        include/nsga-ii/nsga-ii.h
        src/nsga-ii/nsga-ii.cpp
        include/nsga-ii/objectives.h
        src/nsga-ii/objectives.cpp
        test/nsga_schaffer_test.cpp)


set(NSGA_FONSECA include/nsga-ii/nonDomSort.h
        src/nsga-ii/nonDomSort.cpp
        include/nsga-ii/operators.h
        src/nsga-ii/operators.cpp
        include/nsga-ii/hyperParameters.h
        include/nsga-ii/nsga-ii.h
        src/nsga-ii/nsga-ii.cpp
        include/nsga-ii/objectives.h
        src/nsga-ii/objectives.cpp
        test/nsga_fonseca_fleming_test.cpp)

#main executable
add_executable(ICA ${SOURCE_FILES})

#ICA Helpers unit test executable
add_executable(ICA_UNIT ${UNIT_TESTS})

#executable for testing audio functions
add_executable(AUDIO_TEST ${AUDIO_TESTS})

#executable for evolutionary algorithm attempt
add_executable(NSGAII ${NSGA})

#executable for evolutionary algorithm test
add_executable(NSGAII_TEST ${NSGA_TEST})

#executable for evolutionary algo schaffer test
add_executable(NSGA_SCHAFFER_EXE ${NSGA_SCHAFFER})

#executable for fonseca test
add_executable(NSGA_FONSECA_EXE ${NSGA_FONSECA})


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include) #including header files for libraries
target_link_libraries(ICA ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
target_link_libraries(ICA_UNIT ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
target_link_libraries(AUDIO_TEST ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
target_link_libraries(NSGAII ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
target_link_libraries(NSGAII_TEST ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
target_link_libraries(NSGA_SCHAFFER_EXE ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
target_link_libraries(NSGA_FONSECA_EXE ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)