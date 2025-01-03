# components/open-aa-platform-os-abstraction-libs/src/ara/os/linux/CMakeLists.txt

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File Description:
# -----------------
# CMake configuration for the Linux-specific OS components.
# Manages subcomponents under ara::os::linux.
#]=======================================================================]

#****************************************************************************************************
# Add Subdirectories for Each Linux-Specific Component
#****************************************************************************************************

# Add subdirectories for each Linux-specific component.
# This allows for modular addition of Linux-specific functionalities.
add_subdirectory(process)
# Add more Linux-specific components here as needed
# Example:
# add_subdirectory(threading)
# add_subdirectory(memory)
