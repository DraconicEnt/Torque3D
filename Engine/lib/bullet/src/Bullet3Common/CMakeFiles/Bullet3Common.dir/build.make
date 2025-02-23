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
include Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/compiler_depend.make

# Include the progress variables for this target.
include Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/progress.make

# Include the compile flags for this target's objects.
include Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/flags.make

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/flags.make
Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o: Engine/lib/bullet/src/Bullet3Common/b3AlignedAllocator.cpp
Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o -MF CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o.d -o CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3AlignedAllocator.cpp

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3AlignedAllocator.cpp > CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.i

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3AlignedAllocator.cpp -o CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.s

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.o: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/flags.make
Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.o: Engine/lib/bullet/src/Bullet3Common/b3Vector3.cpp
Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.o: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.o -MF CMakeFiles/Bullet3Common.dir/b3Vector3.o.d -o CMakeFiles/Bullet3Common.dir/b3Vector3.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3Vector3.cpp

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bullet3Common.dir/b3Vector3.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3Vector3.cpp > CMakeFiles/Bullet3Common.dir/b3Vector3.i

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bullet3Common.dir/b3Vector3.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3Vector3.cpp -o CMakeFiles/Bullet3Common.dir/b3Vector3.s

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.o: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/flags.make
Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.o: Engine/lib/bullet/src/Bullet3Common/b3Logging.cpp
Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.o: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.o -MF CMakeFiles/Bullet3Common.dir/b3Logging.o.d -o CMakeFiles/Bullet3Common.dir/b3Logging.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3Logging.cpp

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bullet3Common.dir/b3Logging.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3Logging.cpp > CMakeFiles/Bullet3Common.dir/b3Logging.i

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bullet3Common.dir/b3Logging.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/b3Logging.cpp -o CMakeFiles/Bullet3Common.dir/b3Logging.s

# Object files for target Bullet3Common
Bullet3Common_OBJECTS = \
"CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o" \
"CMakeFiles/Bullet3Common.dir/b3Vector3.o" \
"CMakeFiles/Bullet3Common.dir/b3Logging.o"

# External object files for target Bullet3Common
Bullet3Common_EXTERNAL_OBJECTS =

Engine/lib/bullet/src/Bullet3Common/libBullet3Common.2.85.dylib: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3AlignedAllocator.o
Engine/lib/bullet/src/Bullet3Common/libBullet3Common.2.85.dylib: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Vector3.o
Engine/lib/bullet/src/Bullet3Common/libBullet3Common.2.85.dylib: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/b3Logging.o
Engine/lib/bullet/src/Bullet3Common/libBullet3Common.2.85.dylib: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/build.make
Engine/lib/bullet/src/Bullet3Common/libBullet3Common.2.85.dylib: Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library libBullet3Common.dylib"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Bullet3Common.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && $(CMAKE_COMMAND) -E cmake_symlink_library libBullet3Common.2.85.dylib libBullet3Common.2.85.dylib libBullet3Common.dylib

Engine/lib/bullet/src/Bullet3Common/libBullet3Common.dylib: Engine/lib/bullet/src/Bullet3Common/libBullet3Common.2.85.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate Engine/lib/bullet/src/Bullet3Common/libBullet3Common.dylib

# Rule to build all files generated by this target.
Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/build: Engine/lib/bullet/src/Bullet3Common/libBullet3Common.dylib
.PHONY : Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/build

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/clean:
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common && $(CMAKE_COMMAND) -P CMakeFiles/Bullet3Common.dir/cmake_clean.cmake
.PHONY : Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/clean

Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/depend:
	cd /Users/ragora/Documents/Projects/Torque3D && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Engine/lib/bullet/src/Bullet3Common/CMakeFiles/Bullet3Common.dir/depend

