#[====================================================================]
# open-aa-std-adaptive-autosar-libs
# Contains sub-libraries under ara::core, e.g., array, ...
# Author: Sherif Mohamed
#[====================================================================]

cmake_minimum_required(VERSION 3.27)

# ----------------------------------------------------------------------
# 1) ARA::CORE::ARRAY
# ----------------------------------------------------------------------
add_library(ara_core_array INTERFACE)
add_library(ara::core::array ALIAS ara_core_array)

# Provide include dirs
target_include_directories(ara_core_array INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/ara/core/array>  # i.e. array.h is here
    $<INSTALL_INTERFACE:include/ara/core/array>
)

# If ara::core::array references (calls) some function from ara::os::process,
# do:
#   target_link_libraries(ara_core_array INTERFACE ara::os::process)
# This is the "two-way usage" scenario.

# ----------------------------------------------------------------------
# 2) Installation of headers
#    (We assume you have /include/ara/core/array/array.h, etc.)
# ----------------------------------------------------------------------
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ara/core
    DESTINATION include/ara
    FILES_MATCHING PATTERN "*.h"
)

# ----------------------------------------------------------------------
# 3) Export & Package: array
# ----------------------------------------------------------------------
install(TARGETS ara_core_array
    EXPORT ara_core_array_targets
)

install(EXPORT ara_core_array_targets
    FILE ara_core_arrayTargets.cmake
    NAMESPACE ara::core::
    DESTINATION lib/cmake/ara_core_array
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ara_core_arrayConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ara_core_arrayConfig.cmake" "
@PACKAGE_INIT@

# If we truly need to ensure ara::os::process is found:
# find_dependency(ara_os_process REQUIRED)

include(\"\${CMAKE_CURRENT_LIST_DIR}/ara_core_arrayTargets.cmake\")
")

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ara_core_arrayConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ara_core_arrayConfigVersion.cmake"
    DESTINATION lib/cmake/ara_core_array
)

# ----------------------------------------------------------------------
# 4) Additional sub-libraries can be added similarly:
# e.g. ara::core::foo, ara::core::string, etc.
# ----------------------------------------------------------------------