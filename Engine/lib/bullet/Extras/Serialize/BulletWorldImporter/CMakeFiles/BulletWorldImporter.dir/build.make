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
include Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/compiler_depend.make

# Include the progress variables for this target.
include Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/progress.make

# Include the compile flags for this target's objects.
include Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o -MF CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o.d -o CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp > CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.i

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btBulletWorldImporter.cpp -o CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.s

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/flags.make
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o -MF CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o.d -o CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp > CMakeFiles/BulletWorldImporter.dir/btWorldImporter.i

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/btWorldImporter.cpp -o CMakeFiles/BulletWorldImporter.dir/btWorldImporter.s

# Object files for target BulletWorldImporter
BulletWorldImporter_OBJECTS = \
"CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o" \
"CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o"

# External object files for target BulletWorldImporter
BulletWorldImporter_EXTERNAL_OBJECTS =

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btBulletWorldImporter.o
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/btWorldImporter.o
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build.make
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/src/BulletDynamics/libBulletDynamics.2.85.dylib
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/src/BulletCollision/libBulletCollision.2.85.dylib
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/Extras/Serialize/BulletFileLoader/libBulletFileLoader.2.85.dylib
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/src/LinearMath/libLinearMath.2.85.dylib
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libBulletWorldImporter.dylib"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BulletWorldImporter.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -E cmake_symlink_library libBulletWorldImporter.2.85.dylib libBulletWorldImporter.2.85.dylib libBulletWorldImporter.dylib

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.dylib: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.2.85.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.dylib

# Rule to build all files generated by this target.
Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build: Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/libBulletWorldImporter.dylib
.PHONY : Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/build

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/clean:
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter && $(CMAKE_COMMAND) -P CMakeFiles/BulletWorldImporter.dir/cmake_clean.cmake
.PHONY : Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/clean

Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend:
	cd /Users/ragora/Documents/Projects/Torque3D && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Engine/lib/bullet/Extras/Serialize/BulletWorldImporter/CMakeFiles/BulletWorldImporter.dir/depend

