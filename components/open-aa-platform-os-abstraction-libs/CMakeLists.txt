#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# CMake configuration for the open-aa-platform-os-abstraction-libs component.
# Defines the ara::os::process library and its dependencies.
# Generates package configuration files without using .cmake.in templates.
#]=======================================================================]

#****************************************************************************************************
# Library Definition
#****************************************************************************************************

# Define the ara_os_process library as a STATIC library.
# Change STATIC to SHARED if a shared library is preferred.
add_library(ara_os_process STATIC)

#****************************************************************************************************
# Include Directories
#****************************************************************************************************

# Specify the public include directories for the library.
# Utilize BUILD_INTERFACE and INSTALL_INTERFACE to handle build and install paths appropriately.
target_include_directories(ara_os_process
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/components/open-aa-platform-os-abstraction-libs/include>
        $<INSTALL_INTERFACE:include>
)

#****************************************************************************************************
# Link Dependencies
#****************************************************************************************************

# Link against ara::core to provide access to ara/core/array.h and other core functionalities.
target_link_libraries(ara_os_process PUBLIC ara::core)

#****************************************************************************************************
# Source Files
#****************************************************************************************************

# Add the source files by including the src directory.
# This will process src/CMakeLists.txt and its subdirectories.
add_subdirectory(src)

#****************************************************************************************************
# Incorporate OBJECT Library Sources
#****************************************************************************************************

# Include the OBJECT library's object files directly into ara_os_process.
target_sources(ara_os_process PRIVATE
    $<TARGET_OBJECTS:ara_os_process_interface>
)

#****************************************************************************************************
# Compiler Definitions
#****************************************************************************************************

# Define any necessary compile definitions.
# Example: Define a macro if needed.
# target_compile_definitions(ara_os_process PRIVATE SOME_MACRO=1)

#****************************************************************************************************
# Alias Target Definition
#****************************************************************************************************

# Create an alias target ara::os::process for external linkage.
add_library(ara::os::process ALIAS ara_os_process)

#****************************************************************************************************
# Installation Configuration
#****************************************************************************************************

# Specify installation rules for the library.
install(TARGETS ara_os_process
    EXPORT ara_os_process_targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install header files to the include directory
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

#****************************************************************************************************
# Export Configuration
#****************************************************************************************************

# Export the ara_os_process target for use in other projects.
install(EXPORT ara_os_process_targets
    FILE ara_os_processTargets.cmake
    NAMESPACE ara::
    DESTINATION lib/cmake/ara_os_process
)

#****************************************************************************************************
# Package Configuration Generation
#****************************************************************************************************

# Include CMakePackageConfigHelpers for package configuration helpers
include(CMakePackageConfigHelpers)

# Generate a ConfigVersion.cmake file for versioning.
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ara_os_processConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Generate the ara_os_processConfig.cmake content directly without using a .cmake.in file
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ara_os_processConfig.cmake"
"@PACKAGE_INIT@

# Find dependencies required by ara_os_process
find_dependency(ara::core REQUIRED)

# Include the exported targets
include(\"${CMAKE_CURRENT_LIST_DIR}/ara_os_processTargets.cmake\")

# Optionally, set any additional variables or perform further configurations here
")

# Install the package configuration files.
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ara_os_processConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ara_os_processConfigVersion.cmake"
    DESTINATION lib/cmake/ara_os_process
)
