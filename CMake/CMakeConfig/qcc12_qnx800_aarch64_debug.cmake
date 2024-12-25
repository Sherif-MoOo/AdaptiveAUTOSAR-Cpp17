#[=======================================================================[
# Copyright (c) 2024 by Your Company.
# All rights reserved.
#
# This software is copyright protected and proprietary to Your
# Company. You are granted only those rights as set out in the
# license conditions. All other rights remain with Your Company.
#
# File description:
# -----------------
# CMake initial-cache file for OpenAA - QNX 8.0 aarch64 using QCC-12.
# Debug build configuration with enhanced warnings and ISO compliance.
#=======================================================================]

#[=======================================================================[
.rst:
QNX800_AARCH64_QCC12_DEBUG
---------------------------
CMake initial cache file for QNX 8.0 aarch64 debug builds using QCC-12.

This file sets essential CMake variables and compiler/linker flags to support
debugging, such as including debug symbols, disabling optimizations, and enabling
maximum warnings for ISO compliance.

.. code-block:: shell-session

    # QNX dev kit can only be used with bash!
    $ bash -i
    $ source /opt/qnx800/qnxsdp-env.sh
    $ cmake -C CMake/CMakeConfig/qcc12_qnx800_aarch64_debug.cmake -S <project-root> -B <build-dir>

#]=======================================================================]

message(STATUS "Using qcc12_qnx800_aarch64_debug.cmake for debug build configuration.")

#=======================================================================
# Library Building Preferences
#=======================================================================
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")

#=======================================================================
# Compiler Configuration for C
#=======================================================================
set(CMAKE_C_FLAGS_INIT "-Wall -Wextra -Wconversion -pedantic -Wshadow -Wdouble-promotion -Wformat=2 \
-Wnull-dereference -D_QNX_SOURCE -g" CACHE STRING "Initial C Compiler Flags for Debug Build")

set(CMAKE_C_FLAGS_DEBUG_INIT "-O0 -g3" CACHE STRING "C Compiler Flags for Debug Build")

#=======================================================================
# Compiler Configuration for C++
#=======================================================================
set(CMAKE_CXX_FLAGS_INIT "-Wall -Wextra -Wnon-virtual-dtor -Wconversion -Wold-style-cast -pedantic \
-Wshadow -Wno-error=deprecated-declarations -Wdouble-promotion -Wformat=2 -D_QNX_SOURCE -g" CACHE STRING "Initial C++ Compiler Flags for Debug Build")

set(CMAKE_CXX_FLAGS_DEBUG_INIT "-O0 -g3" CACHE STRING "C++ Compiler Flags for Debug Build")

#=======================================================================
# Linker Flags Configuration
#=======================================================================
set(CMAKE_EXE_LINKER_FLAGS_INIT "-g" CACHE STRING "Initial Executable Linker Flags for Debug")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-g" CACHE STRING "Initial Shared Library Linker Flags for Debug")

#=======================================================================
# Build-Type Configuration
#=======================================================================
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (Debug, Release, etc.)")

#=======================================================================
# Additional Debugging Features
#=======================================================================
set(CMAKE_C_FLAGS_ALSAN_INIT "-fsanitize=address -fno-omit-frame-pointer" CACHE STRING "Address Sanitizer Flags for C")
set(CMAKE_CXX_FLAGS_ALSAN_INIT "-fsanitize=address -fno-omit-frame-pointer" CACHE STRING "Address Sanitizer Flags for C++")

set(CMAKE_C_FLAGS_TSAN_INIT "-fsanitize=thread" CACHE STRING "Thread Sanitizer Flags for C")
set(CMAKE_CXX_FLAGS_TSAN_INIT "-fsanitize=thread" CACHE STRING "Thread Sanitizer Flags for C++")

set(CMAKE_C_FLAGS_UBSAN_INIT "-fsanitize=undefined" CACHE STRING "Undefined Behavior Sanitizer Flags for C")
set(CMAKE_CXX_FLAGS_UBSAN_INIT "-fsanitize=undefined" CACHE STRING "Undefined Behavior Sanitizer Flags for C++")

#=======================================================================
# Notes for Debugging
#=======================================================================
# 1. Debug builds include extensive debugging information with `-g3`.
# 2. Address, thread, and undefined behavior sanitizers are enabled to catch runtime issues.
# 3. Warnings are set to maximum to enforce ISO compliance and detect potential issues.
# 4. Optimizations are disabled (`-O0`) to facilitate debugging.
#=======================================================================
