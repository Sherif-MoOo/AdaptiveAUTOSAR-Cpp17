#[=======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# This software is copyright protected and proprietary to Your
# Company. You are granted only those rights as set out in the
# license conditions. All other rights remain with Your Company.
#
# File description:
# -----------------
# CMake initial-cache file for GCC 11 on Linux aarch64.
# This file sets essential CMake variables and compiler/linker flags
# to streamline the build process for Linux targets.
#=======================================================================]

#[=======================================================================[
.rst:
gcc11_linux_aarch64
-------------------
CMake initial cache file for GCC 11 on Linux aarch64.

All variables can be set as initial cache variables and passed as a file to CMake:

.. code-block:: cmake

    # Create an initial cache file (gcc11_linux_aarch64.cmake) and define in there:
    set(CMAKE_PREFIX_PATH "/usr/local/gcc-11" CACHE STRING "")
    set(CMAKE_TOOLCHAIN_FILE "/path/to/toolchain/gcc11_linux_aarch64.cmake" CACHE PATH "")

.. code-block:: shell-session

    $ cmake -C CMake/CMakeConfig/gcc11_linux_aarch64.cmake -S <project-root> -B <build-dir>

#]=======================================================================]

#[=======================================================================[
  CMake Specific Project Settings
#]=======================================================================]

#=======================================================================
# Library Building Preferences
#=======================================================================
#
# Control whether to build shared or static libraries.
# Set to ON to build shared libraries, OFF to build static libraries.
#
# Recommended: OFF for static libraries to simplify deployment.
#=======================================================================
message(STATUS "Using gcc11_linux_aarch64_release.cmake for initial cache setup.")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

#=======================================================================
# Compiler Configuration for C
#=======================================================================
#
# These flags are used by the C compiler during respective build types.
# Adjust them according to your project's requirements and target environment.
#
# Flags:
#   -Wall, -Wextra: Enable most compiler warnings.
#   -Wconversion: Warn for implicit type conversions that may alter a value.
#   -pedantic: Enforce strict ISO compliance.
#   -Wshadow: Warn when a variable shadows another variable.
#=======================================================================
set(CMAKE_CXX_FLAGS_INIT "-Wall -Wextra -Wnon-virtual-dtor -Wconversion -Wold-style-cast -pedantic -Wshadow -Wno-error=deprecated-declarations -v" CACHE STRING "Initial C++ Compiler Flags")

#-----------------------------------------------------------------------
# Build-Type Specific C Flags
#-----------------------------------------------------------------------
# Flags for Debug build: No optimization, include debug symbols.
set(CMAKE_C_FLAGS_DEBUG_INIT "-O0 -g" CACHE STRING "C Compiler Flags for Debug")

# Flags for MinSizeRel build: Optimize for size, define NDEBUG.
set(CMAKE_C_FLAGS_MINSIZEREL_INIT "-Os -DNDEBUG" CACHE STRING "C Compiler Flags for MinSizeRel")

# Flags for Release build: Optimize for speed, define NDEBUG.
set(CMAKE_C_FLAGS_RELEASE_INIT "-O3 -DNDEBUG" CACHE STRING "C Compiler Flags for Release")

# Flags for RelWithDebInfo build: Optimize with debug symbols.
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g -DNDEBUG" CACHE STRING "C Compiler Flags for RelWithDebInfo")

# Additional sanitizer flags for special build types.
set(CMAKE_C_FLAGS_ALSAN_INIT "-fsanitize=address -fno-omit-frame-pointer" CACHE STRING "C Compiler Flags for Address Sanitizer")
set(CMAKE_C_FLAGS_TSAN_INIT "-fsanitize=thread" CACHE STRING "C Compiler Flags for Thread Sanitizer")
set(CMAKE_C_FLAGS_UBSAN_INIT "-fsanitize=undefined" CACHE STRING "C Compiler Flags for Undefined Behavior Sanitizer")

#=======================================================================
# Compiler Configuration for C++
#=======================================================================
#
# These flags are used by the C++ compiler during respective build types.
# Adjust them according to your project's requirements and target environment.
#
# Flags:
#   -Wall, -Wextra: Enable most compiler warnings.
#   -Wnon-virtual-dtor: Warn if a class with virtual functions has a non-virtual destructor.
#   -Wconversion: Warn for implicit type conversions that may alter a value.
#   -Wold-style-cast: Warn about C-style casts.
#   -pedantic: Enforce strict ISO compliance.
#   -Wshadow: Warn when a variable shadows another variable.
#=======================================================================
set(CMAKE_CXX_FLAGS_INIT "-Wall -Wextra -Wnon-virtual-dtor -Wconversion -Wold-style-cast -pedantic -Wshadow" CACHE STRING "Initial C++ Compiler Flags")

#-----------------------------------------------------------------------
# Build-Type Specific C++ Flags
#-----------------------------------------------------------------------
# Flags for Debug build: No optimization, include debug symbols.
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-O0 -g" CACHE STRING "C++ Compiler Flags for Debug")

# Flags for MinSizeRel build: Optimize for size, define NDEBUG.
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "-Os -DNDEBUG" CACHE STRING "C++ Compiler Flags for MinSizeRel")

# Flags for Release build: Optimize for speed, define NDEBUG.
set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG" CACHE STRING "C++ Compiler Flags for Release")

# Flags for RelWithDebInfo build: Optimize with debug symbols.
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g -DNDEBUG" CACHE STRING "C++ Compiler Flags for RelWithDebInfo")

# Additional sanitizer flags for special build types.
set(CMAKE_CXX_FLAGS_ALSAN_INIT "-fsanitize=address -fno-omit-frame-pointer" CACHE STRING "C++ Compiler Flags for Address Sanitizer")
set(CMAKE_CXX_FLAGS_TSAN_INIT "-fsanitize=thread" CACHE STRING "C++ Compiler Flags for Thread Sanitizer")
set(CMAKE_CXX_FLAGS_UBSAN_INIT "-fsanitize=undefined" CACHE STRING "C++ Compiler Flags for Undefined Behavior Sanitizer")
set(CMAKE_CXX_FLAGS_RELEASEWITHO2_INIT "-O2 -DNDEBUG" CACHE STRING "C++ Compiler Flags for ReleaseWithO2")

#=======================================================================
# Linker Flags Configuration
#=======================================================================
#
# These flags are used by the linker during respective build types.
# Adjust them based on your project's requirements and target environment.
#=======================================================================

#=======================================================================
# Executable Linker Flags
#=======================================================================
#
# Initial flags for the executable linker applicable to all build types.
#=======================================================================
set(CMAKE_EXE_LINKER_FLAGS_INIT "" CACHE STRING "Initial Executable Linker Flags")

#-----------------------------------------------------------------------
# Build-Type Specific Executable Linker Flags
#-----------------------------------------------------------------------
# Flags for Debug build: Include debug symbols.
set(CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT "-g" CACHE STRING "Executable Linker Flags for Debug")

# Flags for MinSizeRel build: Strip symbols to reduce size.
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL_INIT "-s" CACHE STRING "Executable Linker Flags for MinSizeRel")

# Flags for Release build: No additional flags.
set(CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT "" CACHE STRING "Executable Linker Flags for Release")

# Flags for RelWithDebInfo build: Include debug symbols.
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT "-g" CACHE STRING "Executable Linker Flags for RelWithDebInfo")

#=======================================================================
# Additional CMake Settings
#=======================================================================
#
# Add any additional CMake variables and configurations below as needed.
# This section is reserved for any project-specific settings that do not
# fit into the categories above.
#
# Example:
# set(SOME_PROJECT_VARIABLE "value" CACHE STRING "Description of the variable")
#
#=======================================================================