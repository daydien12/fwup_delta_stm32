# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /snap/cmake/1329/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1329/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart

# Utility rule file for ExperimentalTest.

# Include any custom commands dependencies for this target.
include CMakeFiles/ExperimentalTest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ExperimentalTest.dir/progress.make

CMakeFiles/ExperimentalTest:
	/snap/cmake/1329/bin/ctest -D ExperimentalTest

ExperimentalTest: CMakeFiles/ExperimentalTest
ExperimentalTest: CMakeFiles/ExperimentalTest.dir/build.make
.PHONY : ExperimentalTest

# Rule to build all files generated by this target.
CMakeFiles/ExperimentalTest.dir/build: ExperimentalTest
.PHONY : CMakeFiles/ExperimentalTest.dir/build

CMakeFiles/ExperimentalTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ExperimentalTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ExperimentalTest.dir/clean

CMakeFiles/ExperimentalTest.dir/depend:
	cd /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart /mnt/c/Users/nguye/OneDrive/Documents/github/fwup_delta_stm32/3.tool/2.uart/CMakeFiles/ExperimentalTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ExperimentalTest.dir/depend

