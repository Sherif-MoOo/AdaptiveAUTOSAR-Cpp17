#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# CMake configuration for the src directory of open-aa-platform-os-abstraction-libs.
# Adds subdirectories for interface and platform-specific implementations.
#]=======================================================================]

#****************************************************************************************************
# Subdirectory Inclusion
#****************************************************************************************************

# Add subdirectory for the ara::os::process interface
add_subdirectory(ara/os/interface/process)

# Conditionally add platform-specific subdirectories based on the target system
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_subdirectory(ara/os/linux/process)
elseif(CMAKE_SYSTEM_NAME STREQUAL "QNX")
    add_subdirectory(ara/os/qnx/process)
endif()
