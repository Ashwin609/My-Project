# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ashwin/grpc/examples/cpp/assignment4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ashwin/grpc/examples/cpp/assignment4/cmake/build

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/keyvalueserver.cc.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/keyvalueserver.cc.o: ../../keyvalueserver.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ashwin/grpc/examples/cpp/assignment4/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server.dir/keyvalueserver.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/keyvalueserver.cc.o -c /home/ashwin/grpc/examples/cpp/assignment4/keyvalueserver.cc

CMakeFiles/server.dir/keyvalueserver.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/keyvalueserver.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ashwin/grpc/examples/cpp/assignment4/keyvalueserver.cc > CMakeFiles/server.dir/keyvalueserver.cc.i

CMakeFiles/server.dir/keyvalueserver.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/keyvalueserver.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ashwin/grpc/examples/cpp/assignment4/keyvalueserver.cc -o CMakeFiles/server.dir/keyvalueserver.cc.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/keyvalueserver.cc.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server: CMakeFiles/server.dir/keyvalueserver.cc.o
server: CMakeFiles/server.dir/build.make
server: libkvs_grpc_proto.a
server: /lib/libgrpc++_reflection.a
server: /lib/libgrpc++.a
server: /lib/libprotobuf.a
server: /lib/libgrpc.a
server: /lib/libz.a
server: /lib/libcares.a
server: /lib/libaddress_sorting.a
server: /lib/libre2.a
server: /lib/libabsl_hash.a
server: /lib/libabsl_city.a
server: /lib/libabsl_wyhash.a
server: /lib/libabsl_raw_hash_set.a
server: /lib/libabsl_hashtablez_sampler.a
server: /lib/libabsl_exponential_biased.a
server: /lib/libabsl_statusor.a
server: /lib/libabsl_bad_variant_access.a
server: /lib/libgpr.a
server: /lib/libupb.a
server: /lib/libabsl_status.a
server: /lib/libabsl_cord.a
server: /lib/libabsl_str_format_internal.a
server: /lib/libabsl_synchronization.a
server: /lib/libabsl_stacktrace.a
server: /lib/libabsl_symbolize.a
server: /lib/libabsl_debugging_internal.a
server: /lib/libabsl_demangle_internal.a
server: /lib/libabsl_graphcycles_internal.a
server: /lib/libabsl_malloc_internal.a
server: /lib/libabsl_time.a
server: /lib/libabsl_strings.a
server: /lib/libabsl_throw_delegate.a
server: /lib/libabsl_strings_internal.a
server: /lib/libabsl_base.a
server: /lib/libabsl_spinlock_wait.a
server: /lib/libabsl_int128.a
server: /lib/libabsl_civil_time.a
server: /lib/libabsl_time_zone.a
server: /lib/libabsl_bad_optional_access.a
server: /lib/libabsl_raw_logging_internal.a
server: /lib/libabsl_log_severity.a
server: /lib/libssl.a
server: /lib/libcrypto.a
server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ashwin/grpc/examples/cpp/assignment4/cmake/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: server

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /home/ashwin/grpc/examples/cpp/assignment4/cmake/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ashwin/grpc/examples/cpp/assignment4 /home/ashwin/grpc/examples/cpp/assignment4 /home/ashwin/grpc/examples/cpp/assignment4/cmake/build /home/ashwin/grpc/examples/cpp/assignment4/cmake/build /home/ashwin/grpc/examples/cpp/assignment4/cmake/build/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend

