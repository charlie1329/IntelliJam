cmake_minimum_required(VERSION 3.9)
project(FYP)

#need to check if this causes any improvements as well
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -msse -msse2 -msse3 -openmp -O3")


set(CMAKE_CXX_STANDARD 11)

# get boost setup properly
#set(Boost_DEBUG ON)
set(BOOST_ROOT "C:/Program Files/boost_1_66_0")
set(BOOST_INCLUDEDIR "C:/Program Files/boost/include/boost_1_66")
set(BOOST_LIBRARYDIR "C:/Program Files/boost/lib")
find_package(Boost 1.66.0 REQUIRED COMPONENTS system thread chrono)


# if boost found, include it
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

#executable for esn training
set(TRAINING_FILES include/esn/esn.h
                    src/esn/esn.cpp)

add_executable(TRAINING ${TRAINING_FILES})

#executable for testing esn functionality
set(ESN_TEST_FILES include/esn/esn.h
                    src/esn/esn.cpp
                    test/esn/esn_correctness.cpp include/runtime/timers.h)

add_executable(ESN_CORE_TEST ${ESN_TEST_FILES})

#executable for speed testing
set(ESN_SPEED_FILES include/esn/esn.h
                    src/esn/esn.cpp
                    test/esn/esn_speed.cpp)

add_executable(ESN_SPEED_TEST ${ESN_SPEED_FILES})


#executable for midi library testing
set(MIDI_TESTS include/midi/midi.h
                src/midi/midi.cpp
                test/midi/testMidi.cpp)

add_executable(MIDI_TEST ${MIDI_TESTS})

#executable for midi library example test
set(MIDI_EXAMPLE include/midi/midi.h
                 src/midi/midi.cpp
                 test/midi/twinkle_twinkle.cpp)

add_executable(MIDI_TWINKLE ${MIDI_EXAMPLE})

target_link_libraries(MIDI_TWINKLE winmm.lib)


#executable for windows midi api test
set(MIDI_WIN_FILES include/midi/midi.h
                   src/midi/midi.cpp
                   include/midi/esnToMidi.h
                   src/midi/esnToMidi.cpp
                   test/midi/midiWinTest.cpp)

add_executable(MIDI_WIN ${MIDI_WIN_FILES})
target_link_libraries(MIDI_WIN winmm.lib)

#executable
set(RUNTIME_FILES include/port_audio/pa_ringbuffer.c
                  include/port_audio/pa_win_util.c
                  include/esn/esn.h
                  src/esn/esn.cpp
                  include/midi/midi.h
                  src/midi/midi.cpp
                  include/runtime/port_processing.h
                  src/runtime/port_processing.cpp
                  include/runtime/init_close.h
                  src/runtime/init_close.cpp
                  include/runtime/globalState.h
                  include/runtime/updateThread.h
                  src/runtime/updateThread.cpp
                  include/runtime/timerThread.h
                  src/runtime/timerThread.cpp
                  include/midi/esnToMidi.h
                  src/midi/esnToMidi.cpp
                  include/runtime/timers.h
                  src/runtime/timers.cpp)

add_executable(RUNTIME ${RUNTIME_FILES})
target_link_libraries(RUNTIME ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)
target_link_libraries(RUNTIME winmm.lib)
#link up the boost libraries
if(Boost_FOUND)
    target_link_libraries(RUNTIME ${Boost_LIBRARIES})
endif()


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include) #including header files


#test for boost
#set (TEST_FILES test/boost_test.cpp)
#add_executable(TEST ${TEST_FILES})
#if(Boost_FOUND)
#    target_link_libraries(TEST ${Boost_LIBRARIES})
#endif()

#to compile with port audio, and ring buffers do the following
#port audio compilation test
#set(TARGET_FILES <my own source files>
#                    include/port_audio/pa_ringbuffer.c
#                    include/port_audio/pa_win_util.c)
#add_executable(TARGET ${TARGET_FILES})
#target_link_libraries(TARGET ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)
#target_link_libraries(TARGET winmm.lib)

#for future when I want to use port audio for things, add this line to the target
#target_link_libraries(TARGET ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)