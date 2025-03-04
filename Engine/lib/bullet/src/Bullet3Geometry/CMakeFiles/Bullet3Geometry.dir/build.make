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
include Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/compiler_depend.make

# Include the progress variables for this target.
include Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/progress.make

# Include the compile flags for this target's objects.
include Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/flags.make

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/flags.make
Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o: Engine/lib/bullet/src/Bullet3Geometry/b3ConvexHullComputer.cpp
Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o -MF CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o.d -o CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry/b3ConvexHullComputer.cpp

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry/b3ConvexHullComputer.cpp > CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.i

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry/b3ConvexHullComputer.cpp -o CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.s

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/flags.make
Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o: Engine/lib/bullet/src/Bullet3Geometry/b3GeometryUtil.cpp
Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o -MF CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o.d -o CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o -c /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry/b3GeometryUtil.cpp

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.i"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry/b3GeometryUtil.cpp > CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.i

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.s"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry/b3GeometryUtil.cpp -o CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.s

# Object files for target Bullet3Geometry
Bullet3Geometry_OBJECTS = \
"CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o" \
"CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o"

# External object files for target Bullet3Geometry
Bullet3Geometry_EXTERNAL_OBJECTS =

Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.2.85.dylib: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3ConvexHullComputer.o
Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.2.85.dylib: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/b3GeometryUtil.o
Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.2.85.dylib: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build.make
Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.2.85.dylib: Engine/lib/bullet/src/Bullet3Common/libBullet3Common.2.85.dylib
Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.2.85.dylib: Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ragora/Documents/Projects/Torque3D/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libBullet3Geometry.dylib"
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Bullet3Geometry.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && $(CMAKE_COMMAND) -E cmake_symlink_library libBullet3Geometry.2.85.dylib libBullet3Geometry.2.85.dylib libBullet3Geometry.dylib

Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.dylib: Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.2.85.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.dylib

# Rule to build all files generated by this target.
Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build: Engine/lib/bullet/src/Bullet3Geometry/libBullet3Geometry.dylib
.PHONY : Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/build

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/clean:
	cd /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry && $(CMAKE_COMMAND) -P CMakeFiles/Bullet3Geometry.dir/cmake_clean.cmake
.PHONY : Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/clean

Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/depend:
	cd /Users/ragora/Documents/Projects/Torque3D && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry /Users/ragora/Documents/Projects/Torque3D /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry /Users/ragora/Documents/Projects/Torque3D/Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Engine/lib/bullet/src/Bullet3Geometry/CMakeFiles/Bullet3Geometry.dir/depend

