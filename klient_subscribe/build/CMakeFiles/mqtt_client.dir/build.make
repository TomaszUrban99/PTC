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
CMAKE_SOURCE_DIR = /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/build

# Include any dependencies generated for this target.
include CMakeFiles/mqtt_client.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mqtt_client.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mqtt_client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mqtt_client.dir/flags.make

CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o: CMakeFiles/mqtt_client.dir/flags.make
CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o: /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/src/mqtt_client.c
CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o: CMakeFiles/mqtt_client.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o -MF CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o.d -o CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o -c /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/src/mqtt_client.c

CMakeFiles/mqtt_client.dir/src/mqtt_client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mqtt_client.dir/src/mqtt_client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/src/mqtt_client.c > CMakeFiles/mqtt_client.dir/src/mqtt_client.c.i

CMakeFiles/mqtt_client.dir/src/mqtt_client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mqtt_client.dir/src/mqtt_client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/src/mqtt_client.c -o CMakeFiles/mqtt_client.dir/src/mqtt_client.c.s

# Object files for target mqtt_client
mqtt_client_OBJECTS = \
"CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o"

# External object files for target mqtt_client
mqtt_client_EXTERNAL_OBJECTS =

libmqtt_client.a: CMakeFiles/mqtt_client.dir/src/mqtt_client.c.o
libmqtt_client.a: CMakeFiles/mqtt_client.dir/build.make
libmqtt_client.a: CMakeFiles/mqtt_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libmqtt_client.a"
	$(CMAKE_COMMAND) -P CMakeFiles/mqtt_client.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mqtt_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mqtt_client.dir/build: libmqtt_client.a
.PHONY : CMakeFiles/mqtt_client.dir/build

CMakeFiles/mqtt_client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mqtt_client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mqtt_client.dir/clean

CMakeFiles/mqtt_client.dir/depend:
	cd /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/build /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/build /home/tomasz/Documents/Studia/PTC/projekt/klient_subscribe/build/CMakeFiles/mqtt_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mqtt_client.dir/depend

