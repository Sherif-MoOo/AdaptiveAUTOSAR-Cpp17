# CMakeLists.txt

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# Root CMake configuration file for the AdaptiveAutosarCpp17 project.
# Supports Linux and QNX aarch64 builds.
#]=======================================================================]

#****************************************************************************************************
# Project Configuration
#****************************************************************************************************

# Setup the minimum required CMake version to use. Must be >= 3.27
cmake_minimum_required(VERSION 3.27)

# Define the project name and supported languages
project(AdaptiveAutosarCpp17
    VERSION 1.0
    LANGUAGES C CXX)

#****************************************************************************************************
# Project Specific Language Settings
#****************************************************************************************************

# Set C and C++ standards
set(CMAKE_C_STANDARD "11" CACHE STRING "C Standard")
set(CMAKE_C_EXTENSIONS OFF CACHE BOOL "Disable C Extensions")
set(CMAKE_C_STANDARD_REQUIRED ON CACHE BOOL "Require C Standard")

set(CMAKE_CXX_STANDARD "17" CACHE STRING "C++ Standard")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Disable C++ Extensions")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require C++ Standard")

#****************************************************************************************************
# Include Subdirectories
#****************************************************************************************************

# Add subdirectories for components and tests
add_subdirectory(components/open-aa-std-adaptive-autosar-libs)
add_subdirectory(tests/core_platform)

#****************************************************************************************************
# Global Policies
#****************************************************************************************************

# Check if the policy is defined (for compatibility with older CMake versions)
if(POLICY CMP0177)
    cmake_policy(SET CMP0177 NEW)
endif()

#****************************************************************************************************
# Compiler Configuration (Optional)
#****************************************************************************************************

# Explicitly set the C++ compiler to g++ or another compiler if needed
# Uncomment and set if you want to enforce a specific compiler
# set(CMAKE_CXX_COMPILER "g++")
