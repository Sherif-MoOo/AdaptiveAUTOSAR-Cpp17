# components/open-aa-platform-os-abstraction-libs/src/ara/os/interface/CMakeLists.txt

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File Description:
# -----------------
# CMake configuration for the OS Interface components.
# Manages subcomponents under ara::os::interface.
#]=======================================================================]

#****************************************************************************************************
# Add Subdirectories for Each Interface Component
#****************************************************************************************************

# Add subdirectories for each interface component.
# This allows for modular addition of interface components.
add_subdirectory(process)
# If there are more interface components in the future, add them here.
# Example:
# add_subdirectory(threading)
# add_subdirectory(memory)
