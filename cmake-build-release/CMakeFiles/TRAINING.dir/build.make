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
include CMakeFiles/TRAINING.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TRAINING.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TRAINING.dir/flags.make

CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj: ../src/esn/esn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\esn\esn.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\esn\esn.cpp

CMakeFiles/TRAINING.dir/src/esn/esn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/esn/esn.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\esn\esn.cpp > CMakeFiles\TRAINING.dir\src\esn\esn.cpp.i

CMakeFiles/TRAINING.dir/src/esn/esn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/esn/esn.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\esn\esn.cpp -o CMakeFiles\TRAINING.dir\src\esn\esn.cpp.s

CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj


CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj: ../src/training_old/fileToEcho.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\training_old\fileToEcho.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\training_old\fileToEcho.cpp

CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\training_old\fileToEcho.cpp > CMakeFiles\TRAINING.dir\src\training_old\fileToEcho.cpp.i

CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\training_old\fileToEcho.cpp -o CMakeFiles\TRAINING.dir\src\training_old\fileToEcho.cpp.s

CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj


CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj: ../src/training_old/trainNetwork.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\training_old\trainNetwork.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\training_old\trainNetwork.cpp

CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\training_old\trainNetwork.cpp > CMakeFiles\TRAINING.dir\src\training_old\trainNetwork.cpp.i

CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\training_old\trainNetwork.cpp -o CMakeFiles\TRAINING.dir\src\training_old\trainNetwork.cpp.s

CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj


CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj: ../src/training_old/checkpoint.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\training_old\checkpoint.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\training_old\checkpoint.cpp

CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\training_old\checkpoint.cpp > CMakeFiles\TRAINING.dir\src\training_old\checkpoint.cpp.i

CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\training_old\checkpoint.cpp -o CMakeFiles\TRAINING.dir\src\training_old\checkpoint.cpp.s

CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj


CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj: ../src/training_old/runTraining.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\training_old\runTraining.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\training_old\runTraining.cpp

CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\training_old\runTraining.cpp > CMakeFiles\TRAINING.dir\src\training_old\runTraining.cpp.i

CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\training_old\runTraining.cpp -o CMakeFiles\TRAINING.dir\src\training_old\runTraining.cpp.s

CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj


CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj: ../src/esn/esn_outputs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\esn\esn_outputs.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\esn\esn_outputs.cpp

CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\esn\esn_outputs.cpp > CMakeFiles\TRAINING.dir\src\esn\esn_outputs.cpp.i

CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\esn\esn_outputs.cpp -o CMakeFiles\TRAINING.dir\src\esn\esn_outputs.cpp.s

CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj


CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj: ../src/esn/esn_costs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\esn\esn_costs.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\esn\esn_costs.cpp

CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\esn\esn_costs.cpp > CMakeFiles\TRAINING.dir\src\esn\esn_costs.cpp.i

CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\esn\esn_costs.cpp -o CMakeFiles\TRAINING.dir\src\esn\esn_costs.cpp.s

CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj


CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj: ../src/training_old/simulated_annealing.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\src\training_old\simulated_annealing.cpp.obj -c C:\Users\charl\CLionProjects\FYP\src\training_old\simulated_annealing.cpp

CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\src\training_old\simulated_annealing.cpp > CMakeFiles\TRAINING.dir\src\training_old\simulated_annealing.cpp.i

CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\src\training_old\simulated_annealing.cpp -o CMakeFiles\TRAINING.dir\src\training_old\simulated_annealing.cpp.s

CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.requires

CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.provides: CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.provides

CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj


CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj: CMakeFiles/TRAINING.dir/flags.make
CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj: CMakeFiles/TRAINING.dir/includes_CXX.rsp
CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj: TRAINING_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\TRAINING.dir\TRAINING_autogen\mocs_compilation.cpp.obj -c C:\Users\charl\CLionProjects\FYP\cmake-build-release\TRAINING_autogen\mocs_compilation.cpp

CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.i"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\charl\CLionProjects\FYP\cmake-build-release\TRAINING_autogen\mocs_compilation.cpp > CMakeFiles\TRAINING.dir\TRAINING_autogen\mocs_compilation.cpp.i

CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.s"
	C:\PROGRA~2\MINGW-~1\I686-6~1.0-P\mingw32\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\charl\CLionProjects\FYP\cmake-build-release\TRAINING_autogen\mocs_compilation.cpp -o CMakeFiles\TRAINING.dir\TRAINING_autogen\mocs_compilation.cpp.s

CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.requires:

.PHONY : CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.requires

CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.provides: CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.requires
	$(MAKE) -f CMakeFiles\TRAINING.dir\build.make CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.provides.build
.PHONY : CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.provides

CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.provides.build: CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj


# Object files for target TRAINING
TRAINING_OBJECTS = \
"CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj" \
"CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj" \
"CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj" \
"CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj" \
"CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj" \
"CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj" \
"CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj" \
"CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj" \
"CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj"

# External object files for target TRAINING
TRAINING_EXTERNAL_OBJECTS =

TRAINING.exe: CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj
TRAINING.exe: CMakeFiles/TRAINING.dir/build.make
TRAINING.exe: ../libs/libsndfile-1.lib
TRAINING.exe: C:/Program\ Files/boost/lib/libboost_system-mgw63-mt-1_66.a
TRAINING.exe: C:/Program\ Files/boost/lib/libboost_thread-mgw63-mt-1_66.a
TRAINING.exe: C:/Program\ Files/boost/lib/libboost_chrono-mgw63-mt-1_66.a
TRAINING.exe: C:/Program\ Files/boost/lib/libboost_date_time-mgw63-mt-1_66.a
TRAINING.exe: C:/Program\ Files/boost/lib/libboost_atomic-mgw63-mt-1_66.a
TRAINING.exe: CMakeFiles/TRAINING.dir/linklibs.rsp
TRAINING.exe: CMakeFiles/TRAINING.dir/objects1.rsp
TRAINING.exe: CMakeFiles/TRAINING.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable TRAINING.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\TRAINING.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TRAINING.dir/build: TRAINING.exe

.PHONY : CMakeFiles/TRAINING.dir/build

CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/esn/esn.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/training_old/fileToEcho.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/training_old/trainNetwork.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/training_old/checkpoint.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/training_old/runTraining.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/esn/esn_outputs.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/esn/esn_costs.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/src/training_old/simulated_annealing.cpp.obj.requires
CMakeFiles/TRAINING.dir/requires: CMakeFiles/TRAINING.dir/TRAINING_autogen/mocs_compilation.cpp.obj.requires

.PHONY : CMakeFiles/TRAINING.dir/requires

CMakeFiles/TRAINING.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\TRAINING.dir\cmake_clean.cmake
.PHONY : CMakeFiles/TRAINING.dir/clean

CMakeFiles/TRAINING.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\charl\CLionProjects\FYP C:\Users\charl\CLionProjects\FYP C:\Users\charl\CLionProjects\FYP\cmake-build-release C:\Users\charl\CLionProjects\FYP\cmake-build-release C:\Users\charl\CLionProjects\FYP\cmake-build-release\CMakeFiles\TRAINING.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TRAINING.dir/depend

