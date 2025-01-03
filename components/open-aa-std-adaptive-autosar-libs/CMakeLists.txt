#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# CMake configuration for the open-aa-std-adaptive-autosar-libs component.
# Defines the ara::core library.
#]=======================================================================]

#****************************************************************************************************
# Library Definition
#****************************************************************************************************

# Define an interface library since ara_core is header-only
add_library(ara_core INTERFACE)

#****************************************************************************************************
# Include Directories
#****************************************************************************************************

# Specify include directories for the interface library.
# Use generator expressions to differentiate between build and install phases.
target_include_directories(ara_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

#****************************************************************************************************
# Alias Target Definition
#****************************************************************************************************

# Create an alias target ara::core for external linkage.
add_library(ara::core ALIAS ara_core)

#****************************************************************************************************
# Installation Configuration
#****************************************************************************************************

# Install header files to the include directory
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

#****************************************************************************************************
# Export Configuration
#****************************************************************************************************

# Export the ara_core target for use in other projects.
install(TARGETS ara_core
    EXPORT ara_core_targets
)

install(EXPORT ara_core_targets
    FILE ara_coreTargets.cmake
    NAMESPACE ara::
    DESTINATION lib/cmake/ara_core
)

#****************************************************************************************************
# Package Configuration Generation
#****************************************************************************************************

# Include CMakePackageConfigHelpers for package configuration helpers
include(CMakePackageConfigHelpers)

# Generate a ConfigVersion.cmake file for versioning.
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Generate the ara_coreConfig.cmake content directly without using a .cmake.in file
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfig.cmake"
"@PACKAGE_INIT@

# Find dependencies required by ara_core
# Example: find_dependency(AnotherDependency REQUIRED)
find_dependency(ara::os::process REQUIRED)
# Include the exported targets
include(\"${CMAKE_CURRENT_LIST_DIR}/ara_coreTargets.cmake\")

# Optionally, set any additional variables or perform further configurations here
")

# Install the package configuration files.
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfigVersion.cmake"
    DESTINATION lib/cmake/ara_core
)
