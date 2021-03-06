cmake_minimum_required(VERSION 3.9)
project(FYP)

#need to check if this causes any improvements as well
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -msse -msse2 -msse3 -openmp -O3")


set(CMAKE_CXX_STANDARD 11)

#removes command line window, may need to switch off for non GUI-oriented targets
set(CMAKE_CXX_FLAGS "-mwindows")

#deal with RC stuff
set(RES_FILES ${CMAKE_CURRENT_SOURCE_DIR}/intellijam.rc)
set(CMAKE_RC_COMPILER_INIT windres)
ENABLE_LANGUAGE(RC)
set(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
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

#get qt set up correctly
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ";" ${QT5_PACKAGE_DIR})
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Svg REQUIRED)
if(Qt5Widgets_FOUND)
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    include_directories(${Qt5Core_INCLUDE_DIRS})
    include_directories(${Qt5Gui_INCLUDE_DIRS})
    include_directories(${Qt5Svg_INCLUDE_DIRS})
endif()


#executable for esn training
set(TRAINING_FILES include/libsndfile/sndfile.h
                   include/esn/esn.h
                   src/esn/esn.cpp
                   include/training_old/fileToEcho.h
                   src/training_old/fileToEcho.cpp
                   include/training_old/trainNetwork.h
                   src/training_old/trainNetwork.cpp
                   include/training_old/hyperParameters.h
                   include/training_old/checkpoint.h
                   src/training_old/checkpoint.cpp
                   src/training_old/runTraining.cpp
                   include/esn/esn_outputs.h
                   src/esn/esn_outputs.cpp
                   include/esn/esn_costs.h
                   src/esn/esn_costs.cpp
                   include/training_old/simulated_annealing.h
                   src/training_old/simulated_annealing.cpp)

add_executable(TRAINING ${TRAINING_FILES})
target_link_libraries(TRAINING ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
#link up the boost libraries
if(Boost_FOUND)
    target_link_libraries(TRAINING ${Boost_LIBRARIES})
endif()


#executable for the unit tests for functionality involved with training
set(TRAINING_TEST_FILES include/libsndfile/sndfile.h
                        include/esn/esn.h
                        src/esn/esn.cpp
                        include/training_old/fileToEcho.h
                        src/training_old/fileToEcho.cpp
                        test/training_old/trainingUnit.cpp
                        include/training_old/hyperParameters.h
                        include/training_old/checkpoint.h
                        src/training_old/checkpoint.cpp
                        include/esn/esn_outputs.h
                        src/esn/esn_outputs.cpp
                        include/training_old/trainNetwork.h
                        src/training_old/trainNetwork.cpp
                        include/esn/esn_costs.h
                        src/esn/esn_costs.cpp
                        include/training_old/simulated_annealing.h
                        src/training_old/simulated_annealing.cpp)

add_executable(TRAINING_TEST ${TRAINING_TEST_FILES})
target_link_libraries(TRAINING_TEST ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
#link up the boost libraries
if(Boost_FOUND)
    target_link_libraries(TRAINING_TEST ${Boost_LIBRARIES})
endif()


#executable for testing esn functionality
set(ESN_TEST_FILES include/esn/esn.h
                    src/esn/esn.cpp
                    test/esn/esn_correctness.cpp
                    include/bridge/bridge.h
                    include/esn/esn_outputs.h
                    src/esn/esn_outputs.cpp)

add_executable(ESN_CORE_TEST ${ESN_TEST_FILES})

#executable for speed testing
set(ESN_SPEED_FILES include/esn/esn.h
                    src/esn/esn.cpp
                    test/esn/esn_speed.cpp
                    include/esn/esn_outputs.h
                    src/esn/esn_outputs.cpp)

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
        include/midi/modelToMidi.h
        src/midi/modelToMidi.cpp
                   test/midi/midiWinTest.cpp)

add_executable(MIDI_WIN ${MIDI_WIN_FILES})
target_link_libraries(MIDI_WIN winmm.lib)

#executable for whole system
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
                  include/midi/modelToMidi.h
                  src/midi/modelToMidi.cpp
                  include/runtime/timers.h
                  src/runtime/timers.cpp
                  src/runtime/runSystem.cpp
                  include/esn/esn_outputs.h
                  src/esn/esn_outputs.cpp)

add_executable(RUNTIME ${RUNTIME_FILES})
target_link_libraries(RUNTIME ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)
target_link_libraries(RUNTIME winmm.lib)
#link up the boost libraries
if(Boost_FOUND)
    target_link_libraries(RUNTIME ${Boost_LIBRARIES})
endif()

set(RUNTIME_W_GUI_FILES include/interface/controlbar.h
                        include/interface/devicebar.h
                        include/interface/mainwindow.h
                        include/interface/nametile.h
                        include/interface/nowplayingbar.h
                        include/interface/piano.h
                        include/interface/squarebutton.h
                        include/interface/vmeter.h
                        src/interface/controlbar.cpp
                        src/interface/devicebar.cpp
                        src/interface/main.cpp
                        src/interface/mainwindow.cpp
                        src/interface/nametile.cpp
                        src/interface/nowplayingbar.cpp
                        src/interface/piano.cpp
                        src/interface/squarebutton.cpp
                        src/interface/vmeter.cpp
                        include/bridge/bridge.h
                        src/bridge/bridge.cpp
                        include/port_audio/pa_ringbuffer.c
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
                        include/midi/modelToMidi.h
                        src/midi/modelToMidi.cpp
                        include/runtime/timers.h
                        src/runtime/timers.cpp
                        include/esn/esn_outputs.h
                        src/esn/esn_outputs.cpp
                        include/model/keyDetect.h
                        src/model/keyDetect.cpp
                        include/model/fpm.h src/model/fpm.cpp)

add_executable(IntelliJam ${RUNTIME_W_GUI_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/intellijam.rc)
if(Qt5Widgets_FOUND)
    target_link_libraries(IntelliJam Qt5::Widgets Qt5::Core Qt5::Svg Qt5::Gui)
endif()
target_link_libraries(IntelliJam ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)
target_link_libraries(IntelliJam winmm.lib)
#link up the boost libraries
if(Boost_FOUND)
    target_link_libraries(IntelliJam ${Boost_LIBRARIES})
endif()


set(MODEL_UNIT_FILES include/model/fpm.h
                     include/model/keyDetect.h
                     src/model/fpm.cpp
                     src/model/keyDetect.cpp
                     test/model/modelTest.cpp)
add_executable(MODEL_UNIT ${MODEL_UNIT_FILES})

set(RUNTIME_UNIT_FILES include/test/catch.hpp
                       include/runtime/init_close.h
                       include/runtime/globalState.h
                       include/runtime/port_processing.h
                       include/port_audio/pa_ringbuffer.c
                       include/port_audio/pa_win_util.c
                       test/runtime/runtimeUnitTests.cpp
                       src/runtime/init_close.cpp
                       src/esn/esn.cpp
                       src/runtime/port_processing.cpp
                       include/esn/esn_outputs.h
                       src/esn/esn_outputs.cpp)
add_executable(RUNTIME_UNIT ${RUNTIME_UNIT_FILES})
target_link_libraries(RUNTIME_UNIT ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)
target_link_libraries(RUNTIME_UNIT winmm.lib)
#link up the boost libraries
if(Boost_FOUND)
    target_link_libraries(RUNTIME_UNIT ${Boost_LIBRARIES})
endif()

set(PORT_FUZZ_TEST_FILES test/port_audio/pa_fuzz.c)
add_executable(PORT_FUZZ_TEST ${PORT_FUZZ_TEST_FILES})
target_link_libraries(PORT_FUZZ_TEST ${CMAKE_CURRENT_SOURCE_DIR}/libs/portaudio_x86.lib)


set(TRAINING_ERROR_FILES  include/libsndfile/sndfile.h
        include/esn/esn.h
        src/esn/esn.cpp
        include/training_old/fileToEcho.h
        src/training_old/fileToEcho.cpp
        include/training_old/trainNetwork.h
        src/training_old/trainNetwork.cpp
        include/training_old/hyperParameters.h
        include/training_old/checkpoint.h
        src/training_old/checkpoint.cpp
        include/esn/esn_outputs.h
        src/esn/esn_outputs.cpp
        test/training_old/calculateError.cpp
        include/esn/esn_costs.h
        src/esn/esn_costs.cpp
        include/training_old/simulated_annealing.h
        src/training_old/simulated_annealing.cpp)

add_executable(TRAINING_ERROR ${TRAINING_ERROR_FILES})
target_link_libraries(TRAINING_ERROR ${CMAKE_CURRENT_SOURCE_DIR}/libs/libsndfile-1.lib)
target_link_libraries(TRAINING_ERROR winmm.lib)
#link up the boost libraries
if(Boost_FOUND)
    target_link_libraries(TRAINING_ERROR ${Boost_LIBRARIES})
endif()

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include) #including header files

# executable for use of the LSTM Training
set(LSTM_FILES include/lstm/lstm.h
               src/lstm/lstm.cpp
               include/lstm/auxillary_functions.h
               src/lstm/auxillary_functions.cpp
               include/training_lstm/readTraining.h
               src/training_lstm/readTraining.cpp
               include/training_lstm/errorCalculation.h
               src/training_lstm/errorCalculation.cpp)
add_executable(LSTM ${LSTM_FILES})

# executable for LSTM training tests
set(LSTM_TEST_FILES include/lstm/lstm.h
                    src/lstm/lstm.cpp
                    include/lstm/auxillary_functions.h
                    src/lstm/auxillary_functions.cpp
                    include/training_lstm/readTraining.h
                    src/training_lstm/readTraining.cpp
                    test/training_lstm/trainingUnit.cpp
                    include/training_lstm/errorCalculation.h
                    src/training_lstm/errorCalculation.cpp)
add_executable(LSTM_TEST ${LSTM_TEST_FILES})

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