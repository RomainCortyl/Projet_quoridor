# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2024.2.2\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2024.2.2\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Projet_quoridor.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Projet_quoridor.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Projet_quoridor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Projet_quoridor.dir/flags.make

CMakeFiles/Projet_quoridor.dir/main.c.obj: CMakeFiles/Projet_quoridor.dir/flags.make
CMakeFiles/Projet_quoridor.dir/main.c.obj: CMakeFiles/Projet_quoridor.dir/includes_C.rsp
CMakeFiles/Projet_quoridor.dir/main.c.obj: C:/Users/rom1c/OneDrive/Documents/GitHub/Projet_quoridor/main.c
CMakeFiles/Projet_quoridor.dir/main.c.obj: CMakeFiles/Projet_quoridor.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Projet_quoridor.dir/main.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw_2\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Projet_quoridor.dir/main.c.obj -MF CMakeFiles\Projet_quoridor.dir\main.c.obj.d -o CMakeFiles\Projet_quoridor.dir\main.c.obj -c C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\main.c

CMakeFiles/Projet_quoridor.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Projet_quoridor.dir/main.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw_2\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\main.c > CMakeFiles\Projet_quoridor.dir\main.c.i

CMakeFiles/Projet_quoridor.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Projet_quoridor.dir/main.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw_2\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\main.c -o CMakeFiles\Projet_quoridor.dir\main.c.s

# Object files for target Projet_quoridor
Projet_quoridor_OBJECTS = \
"CMakeFiles/Projet_quoridor.dir/main.c.obj"

# External object files for target Projet_quoridor
Projet_quoridor_EXTERNAL_OBJECTS =

Projet_quoridor.exe: CMakeFiles/Projet_quoridor.dir/main.c.obj
Projet_quoridor.exe: CMakeFiles/Projet_quoridor.dir/build.make
Projet_quoridor.exe: CMakeFiles/Projet_quoridor.dir/linkLibs.rsp
Projet_quoridor.exe: CMakeFiles/Projet_quoridor.dir/objects1.rsp
Projet_quoridor.exe: CMakeFiles/Projet_quoridor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Projet_quoridor.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Projet_quoridor.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Projet_quoridor.dir/build: Projet_quoridor.exe
.PHONY : CMakeFiles/Projet_quoridor.dir/build

CMakeFiles/Projet_quoridor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Projet_quoridor.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Projet_quoridor.dir/clean

CMakeFiles/Projet_quoridor.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\cmake-build-debug C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\cmake-build-debug C:\Users\rom1c\OneDrive\Documents\GitHub\Projet_quoridor\cmake-build-debug\CMakeFiles\Projet_quoridor.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Projet_quoridor.dir/depend
