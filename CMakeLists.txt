cmake_minimum_required(VERSION 3.8)
project(FYP)

#need to check if this causes any improvements as well
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -msse -msse2 -msse3 -openmp -O3")


set(CMAKE_CXX_STANDARD 11)

#executable for esn training
set(TRAINING_FILES include/esn/esn.h
                    src/esn/esn.cpp)

add_executable(TRAINING ${TRAINING_FILES})

#executable for testing esn functionality
set(ESN_TEST_FILES include/esn/esn.h
                    src/esn/esn.cpp
                    test/esn/esn_correctness.cpp)

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


#executable
set(RUNTIME_FILES include/port_audio/pa_ringbuffer.c
                  include/port_audio/pa_win_util.c
                  src/esn/esn.cpp
                  src/midi/midi.cpp
                  include/runtime/port_processing.h
                  src/runtime/port_processing.cpp)

add_executable(RUNTIME ${RUNTIME_FILES})
target_link_libraries(RUNTIME ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)
target_link_libraries(RUNTIME winmm.lib)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include) #including header files


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