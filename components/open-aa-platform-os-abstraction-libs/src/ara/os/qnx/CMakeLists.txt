# components/open-aa-platform-os-abstraction-libs/src/ara/os/qnx/CMakeLists.txt

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File Description:
# -----------------
# CMake configuration for the QNX-specific OS components.
# Manages subcomponents under ara::os::qnx.
#]=======================================================================]

#****************************************************************************************************
# Add Subdirectories for Each QNX-Specific Component
#****************************************************************************************************

# Add subdirectories for each QNX-specific component.
# This allows for modular addition of QNX-specific functionalities.
add_subdirectory(process)
# Add more QNX-specific components here as needed
# Example:
# add_subdirectory(threading)
# add_subdirectory(memory)
