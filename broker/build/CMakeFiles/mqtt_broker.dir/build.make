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
CMAKE_SOURCE_DIR = /home/tomasz/Documents/Studia/PTC/projekt/broker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tomasz/Documents/Studia/PTC/projekt/broker/build

# Include any dependencies generated for this target.
include CMakeFiles/mqtt_broker.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mqtt_broker.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mqtt_broker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mqtt_broker.dir/flags.make

CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o: CMakeFiles/mqtt_broker.dir/flags.make
CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o: /home/tomasz/Documents/Studia/PTC/projekt/broker/src/mqtt_broker.c
CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o: CMakeFiles/mqtt_broker.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomasz/Documents/Studia/PTC/projekt/broker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o -MF CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o.d -o CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o -c /home/tomasz/Documents/Studia/PTC/projekt/broker/src/mqtt_broker.c

CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tomasz/Documents/Studia/PTC/projekt/broker/src/mqtt_broker.c > CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.i

CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tomasz/Documents/Studia/PTC/projekt/broker/src/mqtt_broker.c -o CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.s

# Object files for target mqtt_broker
mqtt_broker_OBJECTS = \
"CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o"

# External object files for target mqtt_broker
mqtt_broker_EXTERNAL_OBJECTS =

libmqtt_broker.a: CMakeFiles/mqtt_broker.dir/src/mqtt_broker.c.o
libmqtt_broker.a: CMakeFiles/mqtt_broker.dir/build.make
libmqtt_broker.a: CMakeFiles/mqtt_broker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tomasz/Documents/Studia/PTC/projekt/broker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libmqtt_broker.a"
	$(CMAKE_COMMAND) -P CMakeFiles/mqtt_broker.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mqtt_broker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mqtt_broker.dir/build: libmqtt_broker.a
.PHONY : CMakeFiles/mqtt_broker.dir/build

CMakeFiles/mqtt_broker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mqtt_broker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mqtt_broker.dir/clean

CMakeFiles/mqtt_broker.dir/depend:
	cd /home/tomasz/Documents/Studia/PTC/projekt/broker/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tomasz/Documents/Studia/PTC/projekt/broker /home/tomasz/Documents/Studia/PTC/projekt/broker /home/tomasz/Documents/Studia/PTC/projekt/broker/build /home/tomasz/Documents/Studia/PTC/projekt/broker/build /home/tomasz/Documents/Studia/PTC/projekt/broker/build/CMakeFiles/mqtt_broker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mqtt_broker.dir/depend

