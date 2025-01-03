#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# CMake configuration for the QNX-specific ara::os::process implementation.
# Defines the implementation source files and links them to the main library.
#]=======================================================================]

#****************************************************************************************************
# Library Sources
#****************************************************************************************************

# Define the QNX-specific source files
set(QNX_PROCESS_SOURCES
    process.cpp
)

# Add the source files to the main ara_os_process library
target_sources(ara_os_process
    PRIVATE
        ${QNX_PROCESS_SOURCES}
)