# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/hh/searchEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hh/searchEngine

# Include any dependencies generated for this target.
include test/CMakeFiles/configuration_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/configuration_test.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/configuration_test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/configuration_test.dir/flags.make

test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.o: test/CMakeFiles/configuration_test.dir/flags.make
test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.o: test/common/configuration_test.cc
test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.o: test/CMakeFiles/configuration_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hh/searchEngine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.o"
	cd /home/hh/searchEngine/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.o -MF CMakeFiles/configuration_test.dir/common/configuration_test.cc.o.d -o CMakeFiles/configuration_test.dir/common/configuration_test.cc.o -c /home/hh/searchEngine/test/common/configuration_test.cc

test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/configuration_test.dir/common/configuration_test.cc.i"
	cd /home/hh/searchEngine/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hh/searchEngine/test/common/configuration_test.cc > CMakeFiles/configuration_test.dir/common/configuration_test.cc.i

test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/configuration_test.dir/common/configuration_test.cc.s"
	cd /home/hh/searchEngine/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hh/searchEngine/test/common/configuration_test.cc -o CMakeFiles/configuration_test.dir/common/configuration_test.cc.s

# Object files for target configuration_test
configuration_test_OBJECTS = \
"CMakeFiles/configuration_test.dir/common/configuration_test.cc.o"

# External object files for target configuration_test
configuration_test_EXTERNAL_OBJECTS =

test/configuration_test: test/CMakeFiles/configuration_test.dir/common/configuration_test.cc.o
test/configuration_test: test/CMakeFiles/configuration_test.dir/build.make
test/configuration_test: lib/libhh_shared.so
test/configuration_test: /usr/local/lib/libspdlog.a
test/configuration_test: /usr/lib/x86_64-linux-gnu/libboost_locale.so.1.74.0
test/configuration_test: lib/libthirdparty_tinyxml2.so
test/configuration_test: /usr/lib/x86_64-linux-gnu/libboost_chrono.so.1.74.0
test/configuration_test: /usr/lib/x86_64-linux-gnu/libboost_system.so.1.74.0
test/configuration_test: /usr/lib/x86_64-linux-gnu/libboost_thread.so.1.74.0
test/configuration_test: /usr/lib/x86_64-linux-gnu/libboost_atomic.so.1.74.0
test/configuration_test: test/CMakeFiles/configuration_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hh/searchEngine/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable configuration_test"
	cd /home/hh/searchEngine/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/configuration_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/configuration_test.dir/build: test/configuration_test
.PHONY : test/CMakeFiles/configuration_test.dir/build

test/CMakeFiles/configuration_test.dir/clean:
	cd /home/hh/searchEngine/test && $(CMAKE_COMMAND) -P CMakeFiles/configuration_test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/configuration_test.dir/clean

test/CMakeFiles/configuration_test.dir/depend:
	cd /home/hh/searchEngine && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hh/searchEngine /home/hh/searchEngine/test /home/hh/searchEngine /home/hh/searchEngine/test /home/hh/searchEngine/test/CMakeFiles/configuration_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/configuration_test.dir/depend

