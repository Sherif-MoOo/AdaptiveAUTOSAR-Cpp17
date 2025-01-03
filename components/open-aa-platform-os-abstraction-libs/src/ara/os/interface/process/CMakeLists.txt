# components/open-aa-platform-os-abstraction-libs/src/ara/os/interface/process/CMakeLists.txt

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File Description:
# -----------------
# CMake configuration for the Process interface component of the OS Abstraction Layer.
# Defines the ara::os::process library which provides process-related functionalities.
#]=======================================================================]

#****************************************************************************************************
# Library Definition
#****************************************************************************************************

# Define the ara::os::process library as a STATIC library.
# This library includes common process functionalities.
add_library(ara_os_process STATIC)

#****************************************************************************************************
# Target Properties
#****************************************************************************************************

# Specify the C++ standard required for this library.
target_compile_features(ara_os_process PUBLIC cxx_std_17)

#****************************************************************************************************
# Include Directories
#****************************************************************************************************

# Specify include directories for the ara::os::process library.
target_include_directories(ara_os_process PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../../include>  # Include path during build
    $<INSTALL_INTERFACE:include>                                      # Include path when installed
)

#****************************************************************************************************
# Source Files
#****************************************************************************************************

# Add the process_factory.cpp source file to ara::os::process library.
target_sources(ara_os_process PRIVATE
    process_factory.cpp
)

#****************************************************************************************************
# Linking Configuration
#****************************************************************************************************

# Link ara::os::process against ara::os and ara::core to inherit their usage requirements.
target_link_libraries(ara_os_process PUBLIC
    ara::os
    ara::core
)

#****************************************************************************************************
# Alias Target Definition
#****************************************************************************************************

# Create a namespaced alias ara::os::process for the ara_os_process library.
add_library(ara::os::process ALIAS ara_os_process)

#****************************************************************************************************
# Installation Configuration
#****************************************************************************************************

# No installation commands here to prevent multiple installations.
# All installations are handled in the main CMakeLists.txt.
