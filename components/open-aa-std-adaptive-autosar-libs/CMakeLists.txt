#[====================================================================]
# open-aa-std-adaptive-autosar-libs
# Contains sub-libraries under ara::core, e.g., array, violation, ...
# Author: Sherif Mohamed
#[====================================================================]

cmake_minimum_required(VERSION 3.27)

# ----------------------------------------------------------------------
# 1) ARA::CORE::VIOLATION
# ----------------------------------------------------------------------
add_library(ara_core_violation STATIC
    src/ara/core/internal/violation_handler.cpp  # Source file for violation_handler
)
# Alias ara_core_violation to ara::core::violation for easier referencing
add_library(ara::core::violation ALIAS ara_core_violation)

# Provide include directories for ara::core::violation
target_include_directories(ara_core_violation PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # Path to violation_handler headers during build
    $<INSTALL_INTERFACE:include>                            # Path to violation_handler headers after installation
)

# Establish dependency: ara::core::violation depends on ara::os::process
target_link_libraries(ara_core_violation PUBLIC 
    ara::os::process
)

# ----------------------------------------------------------------------
# 2) ARA::CORE::ARRAY
# ----------------------------------------------------------------------
add_library(ara_core_array INTERFACE)
add_library(ara::core::array ALIAS ara_core_array)

# Provide include directories for ara::core::array
target_include_directories(ara_core_array INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # Path to array headers during build
    $<INSTALL_INTERFACE:include>                          # Path to array headers after installation
)

# link INTERFACE libraries
target_link_libraries(ara_core_array INTERFACE 
    ara::core::violation
)

# ----------------------------------------------------------------------
# 3) Installation of Headers
# ----------------------------------------------------------------------
# Install the ara/core headers, including array.h and internal headers
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ara/core
    DESTINATION include/ara
    FILES_MATCHING PATTERN "*.h"  # Install only header files
)

# ----------------------------------------------------------------------
# 4) Export & Package: ara_core_targets
# ----------------------------------------------------------------------
# Create a single export set for all ara::core targets to avoid duplication
install(TARGETS ara_core_violation ara_core_array
    EXPORT ara_core_targets  # Single export set for both targets
    ARCHIVE DESTINATION lib/core                    # Installation path for static libraries
    LIBRARY DESTINATION lib                         # Installation path for shared libraries (if applicable)
    RUNTIME DESTINATION bin                         # Installation path for executables (if applicable)
    INCLUDES DESTINATION include                    # Installation path for headers
)

# Export ara::core targets for CMake's package configuration
install(EXPORT ara_core_targets
    FILE ara_core_targets.cmake                 # Export file containing all ara::core targets
    NAMESPACE ara::core::                       # Namespace prefix for imported targets
    DESTINATION lib/cmake/ara_core              # Installation directory for the export file
)

# ----------------------------------------------------------------------
# 5) Package Configuration Files
# ----------------------------------------------------------------------
include(CMakePackageConfigHelpers)

# Write the package version file for ara::core
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Generate the package configuration file for ara::core
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfig.cmake" "
@PACKAGE_INIT@

include(\"\${CMAKE_CURRENT_LIST_DIR}/ara_core_targets.cmake\")
")

# Install the package configuration files for ara::core
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ara_coreConfigVersion.cmake"
    DESTINATION lib/cmake/ara_core
)

# ----------------------------------------------------------------------
# 6) Conditional Export and Install
# ----------------------------------------------------------------------
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Export targets for use within the build system when building standalone
    export(EXPORT ara_core_targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/ara_core_targets.cmake"
        NAMESPACE ara::core::
    )
endif()

# ----------------------------------------------------------------------
# 7) Additional Sub-Libraries (if any)
# ----------------------------------------------------------------------
# Future sub-libraries under ara::core can be added similarly.
# Example:
# add_library(ara_core_foo ...)
# target_include_directories(ara_core_foo INTERFACE ...)
# target_link_libraries(ara_core_foo INTERFACE ara::core::array)
# install(TARGETS ara_core_foo ...)
# install(EXPORT ara_core_targets ...)
# ... and so on.
