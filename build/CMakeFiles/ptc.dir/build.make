# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tomasz/Documents/Studia/PTC/projekt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tomasz/Documents/Studia/PTC/projekt/build

# Include any dependencies generated for this target.
include CMakeFiles/ptc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ptc.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ptc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ptc.dir/flags.make

CMakeFiles/ptc.dir/src/main.c.o: CMakeFiles/ptc.dir/flags.make
CMakeFiles/ptc.dir/src/main.c.o: /home/tomasz/Documents/Studia/PTC/projekt/src/main.c
CMakeFiles/ptc.dir/src/main.c.o: CMakeFiles/ptc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomasz/Documents/Studia/PTC/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ptc.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/ptc.dir/src/main.c.o -MF CMakeFiles/ptc.dir/src/main.c.o.d -o CMakeFiles/ptc.dir/src/main.c.o -c /home/tomasz/Documents/Studia/PTC/projekt/src/main.c

CMakeFiles/ptc.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ptc.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tomasz/Documents/Studia/PTC/projekt/src/main.c > CMakeFiles/ptc.dir/src/main.c.i

CMakeFiles/ptc.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ptc.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tomasz/Documents/Studia/PTC/projekt/src/main.c -o CMakeFiles/ptc.dir/src/main.c.s

# Object files for target ptc
ptc_OBJECTS = \
"CMakeFiles/ptc.dir/src/main.c.o"

# External object files for target ptc
ptc_EXTERNAL_OBJECTS =

libptc.a: CMakeFiles/ptc.dir/src/main.c.o
libptc.a: CMakeFiles/ptc.dir/build.make
libptc.a: CMakeFiles/ptc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tomasz/Documents/Studia/PTC/projekt/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libptc.a"
	$(CMAKE_COMMAND) -P CMakeFiles/ptc.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ptc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ptc.dir/build: libptc.a
.PHONY : CMakeFiles/ptc.dir/build

CMakeFiles/ptc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ptc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ptc.dir/clean

CMakeFiles/ptc.dir/depend:
	cd /home/tomasz/Documents/Studia/PTC/projekt/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tomasz/Documents/Studia/PTC/projekt /home/tomasz/Documents/Studia/PTC/projekt /home/tomasz/Documents/Studia/PTC/projekt/build /home/tomasz/Documents/Studia/PTC/projekt/build /home/tomasz/Documents/Studia/PTC/projekt/build/CMakeFiles/ptc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ptc.dir/depend

