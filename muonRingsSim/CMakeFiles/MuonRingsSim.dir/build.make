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
CMAKE_SOURCE_DIR = /home/j/jss55/Software/geant4/muonRingsSim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/j/jss55/Software/geant4/muonRingsSim

# Utility rule file for MuonRingsSim.

# Include any custom commands dependencies for this target.
include CMakeFiles/MuonRingsSim.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MuonRingsSim.dir/progress.make

CMakeFiles/MuonRingsSim: muonRingsSim

MuonRingsSim: CMakeFiles/MuonRingsSim
MuonRingsSim: CMakeFiles/MuonRingsSim.dir/build.make
.PHONY : MuonRingsSim

# Rule to build all files generated by this target.
CMakeFiles/MuonRingsSim.dir/build: MuonRingsSim
.PHONY : CMakeFiles/MuonRingsSim.dir/build

CMakeFiles/MuonRingsSim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MuonRingsSim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MuonRingsSim.dir/clean

CMakeFiles/MuonRingsSim.dir/depend:
	cd /home/j/jss55/Software/geant4/muonRingsSim && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/j/jss55/Software/geant4/muonRingsSim /home/j/jss55/Software/geant4/muonRingsSim /home/j/jss55/Software/geant4/muonRingsSim /home/j/jss55/Software/geant4/muonRingsSim /home/j/jss55/Software/geant4/muonRingsSim/CMakeFiles/MuonRingsSim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MuonRingsSim.dir/depend

