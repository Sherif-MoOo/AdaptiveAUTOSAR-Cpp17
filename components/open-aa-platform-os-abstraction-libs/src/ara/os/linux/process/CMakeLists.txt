#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# CMake configuration for the Linux-specific ara::os::process implementation.
# Defines the implementation source files and links them to the main library.
#]=======================================================================]

#****************************************************************************************************
# Library Sources
#****************************************************************************************************

# Define the Linux-specific source files
set(LINUX_PROCESS_SOURCES
    process.cpp
)

# Add the source files to the main ara_os_process library
target_sources(ara_os_process
    PRIVATE
        ${LINUX_PROCESS_SOURCES}
)