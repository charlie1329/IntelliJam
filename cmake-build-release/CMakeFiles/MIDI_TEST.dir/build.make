# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.3\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.3\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\charl\CLionProjects\FYP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\charl\CLionProjects\FYP\cmake-build-release

# Include any dependencies generated for this target.
include CMakeFiles/MIDI_TEST.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MIDI_TEST.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MIDI_TEST.dir/flags.make

CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj: CMakeFiles/MIDI_TEST.dir/flags.make
CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj: CMakeFiles/MIDI_TEST.dir/includes_CXX.rsp
CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj: ../src/midi/midi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MIDI_TEST.dir\src\midi\midi.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\midi\midi.cpp

CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\midi\midi.cpp > CMakeFiles\MIDI_TEST.dir\src\midi\midi.cpp.i

CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\midi\midi.cpp -o CMakeFiles\MIDI_TEST.dir\src\midi\midi.cpp.s

CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.requires:

.PHONY : CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.requires

CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.provides: CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.requires
	$(MAKE) -f CMakeFiles\MIDI_TEST.dir\build.make CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.provides.build
.PHONY : CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.provides

CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.provides.build: CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj


CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj: CMakeFiles/MIDI_TEST.dir/flags.make
CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj: CMakeFiles/MIDI_TEST.dir/includes_CXX.rsp
CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj: ../test/midi/testMidi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MIDI_TEST.dir\test\midi\testMidi.cpp.obj -c C:\Users\charl\CLionProjects\FYP\test\midi\testMidi.cpp

CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\test\midi\testMidi.cpp > CMakeFiles\MIDI_TEST.dir\test\midi\testMidi.cpp.i

CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\test\midi\testMidi.cpp -o CMakeFiles\MIDI_TEST.dir\test\midi\testMidi.cpp.s

CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.requires:

.PHONY : CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.requires

CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.provides: CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.requires
	$(MAKE) -f CMakeFiles\MIDI_TEST.dir\build.make CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.provides.build
.PHONY : CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.provides

CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.provides.build: CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj


CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj: CMakeFiles/MIDI_TEST.dir/flags.make
CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj: CMakeFiles/MIDI_TEST.dir/includes_CXX.rsp
CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj: MIDI_TEST_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\MIDI_TEST.dir\MIDI_TEST_autogen\mocs_compilation.cpp.obj -c C:\Users\charl\CLionProjects\FYP\cmake-build-release\MIDI_TEST_autogen\mocs_compilation.cpp

CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\cmake-build-release\MIDI_TEST_autogen\mocs_compilation.cpp > CMakeFiles\MIDI_TEST.dir\MIDI_TEST_autogen\mocs_compilation.cpp.i

CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\cmake-build-release\MIDI_TEST_autogen\mocs_compilation.cpp -o CMakeFiles\MIDI_TEST.dir\MIDI_TEST_autogen\mocs_compilation.cpp.s

CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.requires:

.PHONY : CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.requires

CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.provides: CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.requires
	$(MAKE) -f CMakeFiles\MIDI_TEST.dir\build.make CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.provides.build
.PHONY : CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.provides

CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.provides.build: CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj


# Object files for target MIDI_TEST
MIDI_TEST_OBJECTS = \
"CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj" \
"CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj" \
"CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj"

# External object files for target MIDI_TEST
MIDI_TEST_EXTERNAL_OBJECTS =

MIDI_TEST.exe: CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj
MIDI_TEST.exe: CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj
MIDI_TEST.exe: CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj
MIDI_TEST.exe: CMakeFiles/MIDI_TEST.dir/build.make
MIDI_TEST.exe: CMakeFiles/MIDI_TEST.dir/linklibs.rsp
MIDI_TEST.exe: CMakeFiles/MIDI_TEST.dir/objects1.rsp
MIDI_TEST.exe: CMakeFiles/MIDI_TEST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable MIDI_TEST.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MIDI_TEST.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MIDI_TEST.dir/build: MIDI_TEST.exe

.PHONY : CMakeFiles/MIDI_TEST.dir/build

CMakeFiles/MIDI_TEST.dir/requires: CMakeFiles/MIDI_TEST.dir/src/midi/midi.cpp.obj.requires
CMakeFiles/MIDI_TEST.dir/requires: CMakeFiles/MIDI_TEST.dir/test/midi/testMidi.cpp.obj.requires
CMakeFiles/MIDI_TEST.dir/requires: CMakeFiles/MIDI_TEST.dir/MIDI_TEST_autogen/mocs_compilation.cpp.obj.requires

.PHONY : CMakeFiles/MIDI_TEST.dir/requires

CMakeFiles/MIDI_TEST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MIDI_TEST.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MIDI_TEST.dir/clean

CMakeFiles/MIDI_TEST.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\charl\CLionProjects\FYP C:\Users\charl\CLionProjects\FYP C:\Users\charl\CLionProjects\FYP\cmake-build-release C:\Users\charl\CLionProjects\FYP\cmake-build-release C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles\MIDI_TEST.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MIDI_TEST.dir/depend

