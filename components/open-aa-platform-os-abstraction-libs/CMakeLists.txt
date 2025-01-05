#[====================================================================]
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# CMake configuration for the open-aa-platform-os-abstraction-libs component.
# Defines the ara::os::process library and its dependencies.
#[====================================================================]

# ----------------------------------------------------------------------
# 1) Create the ara_os_process library (STATIC)
# ----------------------------------------------------------------------
add_library(ara_os_process STATIC)

# Alias ara::os::process for easier referencing
add_library(ara::os::process ALIAS ara_os_process)

# ----------------------------------------------------------------------
# 2) Include Directories
#    Provide public include dirs for OS headers + references to ara::core::array
# ----------------------------------------------------------------------
target_include_directories(ara_os_process
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/components/open-aa-platform-os-abstraction-libs/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/components/open-aa-std-adaptive-autosar-libs/include>
        $<INSTALL_INTERFACE:include>
)

# ----------------------------------------------------------------------
# 3) Link Dependencies
#    Link to ara::core::array so #include "ara/core/array.h" works in process.cpp
# ----------------------------------------------------------------------
target_link_libraries(ara_os_process
    PUBLIC
        ara::core::array
)

# ----------------------------------------------------------------------
# 4) Source Directories
# ----------------------------------------------------------------------
add_subdirectory(src)

# ----------------------------------------------------------------------
# 5) Incorporate OBJECT library sources (if any)
# ----------------------------------------------------------------------
target_sources(ara_os_process PRIVATE
    $<TARGET_OBJECTS:ara_os_process_interface>
)

# ----------------------------------------------------------------------
# 6) Installation: the library + headers
# ----------------------------------------------------------------------
install(TARGETS ara_os_process
    EXPORT ara_os_process_targets
    ARCHIVE DESTINATION lib/os
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

# ----------------------------------------------------------------------
# 7) Export for External Usage (ara_os_processTargets.cmake)
# ----------------------------------------------------------------------
install(EXPORT ara_os_process_targets
    FILE ara_os_processTargets.cmake
    NAMESPACE ara::
    DESTINATION lib/cmake/ara_os_process
)

# ----------------------------------------------------------------------
# 8) Generate the Config + Version files in the *same* CMAKE_CURRENT_BINARY_DIR
# ----------------------------------------------------------------------
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ara_os_processConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Instead of writing directly to "ara_os_processConfig.cmake",
# store the path in a variable to ensure consistency.
set(ARA_OS_PROCESS_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/ara_os_processConfig.cmake")

file(WRITE "${ARA_OS_PROCESS_CONFIG_FILE}" "
@PACKAGE_INIT@

# If needed, ensure that 'ara::core::array' is found:
find_dependency(ara::core::array REQUIRED)

include(\"\${CMAKE_CURRENT_LIST_DIR}/ara_os_processTargets.cmake\")
")

# Now install them from the exact same paths
install(FILES
    "${ARA_OS_PROCESS_CONFIG_FILE}"
    "${CMAKE_CURRENT_BINARY_DIR}/ara_os_processConfigVersion.cmake"
    DESTINATION lib/cmake/ara_os_process
)

# ----------------------------------------------------------------------
# 9) Conditional Export and Install
# ----------------------------------------------------------------------
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Export targets for use within the build system when building standalone
    export(EXPORT ara_os_process_targets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/ara_os_process_targets.cmake"
        NAMESPACE ara::
    )
endif()

# ----------------------------------------------------------------------
# 10) Additional Sub-Libraries (if any)
# ----------------------------------------------------------------------
# Future sub-libraries under ara::os can be added similarly.
# Example:
# add_library(ara_os_foo ...)
# target_include_directories(ara_os_foo INTERFACE ...)
# target_link_libraries(ara_os_foo INTERFACE ara::os::process)
# install(TARGETS ara_os_foo ...)
# install(EXPORT ara_os_process_targets ...)
# ... and so on.
