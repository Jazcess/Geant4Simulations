# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /cm/shared/apps/cmake/3.21.2/bin/cmake

# The command to remove a file.
RM = /cm/shared/apps/cmake/3.21.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/j/jss55/Software/geant4/cherenkovSim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/j/jss55/Software/geant4/cherenkovSim

# Utility rule file for CherenkovSimulation.

# Include any custom commands dependencies for this target.
include CMakeFiles/CherenkovSimulation.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CherenkovSimulation.dir/progress.make

CMakeFiles/CherenkovSimulation: cherenkovSim

CherenkovSimulation: CMakeFiles/CherenkovSimulation
CherenkovSimulation: CMakeFiles/CherenkovSimulation.dir/build.make
.PHONY : CherenkovSimulation

# Rule to build all files generated by this target.
CMakeFiles/CherenkovSimulation.dir/build: CherenkovSimulation
.PHONY : CMakeFiles/CherenkovSimulation.dir/build

CMakeFiles/CherenkovSimulation.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CherenkovSimulation.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CherenkovSimulation.dir/clean

CMakeFiles/CherenkovSimulation.dir/depend:
	cd /home/j/jss55/Software/geant4/cherenkovSim && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/j/jss55/Software/geant4/cherenkovSim /home/j/jss55/Software/geant4/cherenkovSim /home/j/jss55/Software/geant4/cherenkovSim /home/j/jss55/Software/geant4/cherenkovSim /home/j/jss55/Software/geant4/cherenkovSim/CMakeFiles/CherenkovSimulation.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CherenkovSimulation.dir/depend
