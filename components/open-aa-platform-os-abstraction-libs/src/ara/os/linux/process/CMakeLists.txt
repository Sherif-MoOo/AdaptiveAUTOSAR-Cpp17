# components/open-aa-platform-os-abstraction-libs/src/ara/os/linux/process/CMakeLists.txt

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File Description:
# -----------------
# CMake configuration for the Linux-specific Process implementation.
# Adds platform-specific source files to the ara::os::process library.
#]=======================================================================]

#****************************************************************************************************
# Adding Platform-Specific Sources
#****************************************************************************************************

# Add platform-specific source files to the ara::os::process library.
target_sources(ara_os_process PRIVATE
    process.cpp
)
