# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ragora/Documents/Projects/Torque3D

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ragora/Documents/Projects/Torque3D

# Include any dependencies generated for this target.
include Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/compiler_depend.make

# Include the progress variables for this target.
include Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/progress.make

# Include the compile flags for this target's objects.
include Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/flags.make

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.o: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/flags.make
Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.o: Engine/lib/bullet/examples/BasicDemo/BasicExample.cpp
Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.o: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.o -MF CMakeFiles/App_BasicExample.dir/BasicExample.o.d -o CMakeFiles/App_BasicExample.dir/BasicExample.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo/BasicExample.cpp

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/App_BasicExample.dir/BasicExample.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo/BasicExample.cpp > CMakeFiles/App_BasicExample.dir/BasicExample.i

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/App_BasicExample.dir/BasicExample.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo/BasicExample.cpp -o CMakeFiles/App_BasicExample.dir/BasicExample.s

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.o: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/flags.make
Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.o: Engine/lib/bullet/examples/BasicDemo/main.cpp
Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.o: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.o -MF CMakeFiles/App_BasicExample.dir/main.o.d -o CMakeFiles/App_BasicExample.dir/main.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo/main.cpp

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/App_BasicExample.dir/main.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo/main.cpp > CMakeFiles/App_BasicExample.dir/main.i

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/App_BasicExample.dir/main.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo/main.cpp -o CMakeFiles/App_BasicExample.dir/main.s

# Object files for target App_BasicExample
App_BasicExample_OBJECTS = \
"CMakeFiles/App_BasicExample.dir/BasicExample.o" \
"CMakeFiles/App_BasicExample.dir/main.o"

# External object files for target App_BasicExample
App_BasicExample_EXTERNAL_OBJECTS =

Engine/lib/bullet/examples/BasicDemo/App_BasicExample: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/BasicExample.o
Engine/lib/bullet/examples/BasicDemo/App_BasicExample: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/main.o
Engine/lib/bullet/examples/BasicDemo/App_BasicExample: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/build.make
Engine/lib/bullet/examples/BasicDemo/App_BasicExample: Engine/lib/bullet/src/BulletDynamics/libBulletDynamics.2.85.dylib
Engine/lib/bullet/examples/BasicDemo/App_BasicExample: Engine/lib/bullet/src/BulletCollision/libBulletCollision.2.85.dylib
Engine/lib/bullet/examples/BasicDemo/App_BasicExample: Engine/lib/bullet/src/LinearMath/libLinearMath.2.85.dylib
Engine/lib/bullet/examples/BasicDemo/App_BasicExample: Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable App_BasicExample"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/App_BasicExample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/build: Engine/lib/bullet/examples/BasicDemo/App_BasicExample
.PHONY : Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/build

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/clean:
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo && $(CMAKE_COMMAND) -P CMakeFiles/App_BasicExample.dir/cmake_clean.cmake
.PHONY : Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/clean

Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/depend:
	cd /Users/ragora/Documents/Projects/Torque3D && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Engine/lib/bullet/examples/BasicDemo/CMakeFiles/App_BasicExample.dir/depend

