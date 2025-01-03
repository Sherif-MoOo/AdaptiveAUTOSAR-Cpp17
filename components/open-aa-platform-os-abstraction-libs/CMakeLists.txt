# components/open-aa-platform-os-abstraction-libs/CMakeLists.txt

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File Description:
# -----------------
# CMake configuration for the OS Abstraction Layer library.
# Supports Linux and QNX aarch64 builds.
# Defines the ara::os library and its individual components.
# Each component is managed independently, promoting scalability and modularity.
#]=======================================================================]

#****************************************************************************************************
# Library Definition
#****************************************************************************************************

# Define the ara_os library as an INTERFACE library.
# This serves as the base for OS abstraction functionalities.
add_library(ara_os INTERFACE)

#****************************************************************************************************
# Target Properties
#****************************************************************************************************

# Specify the C++ standard required for this library.
target_compile_features(ara_os INTERFACE cxx_std_17)

#****************************************************************************************************
# Include Directories
#****************************************************************************************************

# Specify include directories for the ara_os library.
target_include_directories(ara_os INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # Include path during build
    $<INSTALL_INTERFACE:include>                            # Include path when installed
)

#****************************************************************************************************
# Linking Configuration
#****************************************************************************************************

# Link ara_os with all OS components.
# This ensures that any target linking against ara::os also links against its components.
target_link_libraries(ara_os INTERFACE
    ara::os::process
    # ara::os::threading  # Uncomment if threading component exists
    # ara::os::memory     # Uncomment if memory component exists
    # Add other components here as they are created
)

#****************************************************************************************************
# Add Subdirectories for OS Components
#****************************************************************************************************

# Automatically add subdirectories for each OS component.
# This assumes each component has its own CMakeLists.txt.
file(GLOB OS_COMPONENT_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src/ara/os ${CMAKE_CURRENT_SOURCE_DIR}/src/ara/os/*)

foreach(component_dir ${OS_COMPONENT_DIRS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/ara/os/${component_dir}")
        add_subdirectory(src/ara/os/${component_dir})
    endif()
endforeach()

#****************************************************************************************************
# Alias Target Definition
#****************************************************************************************************

# Create a namespaced alias ara::os for the ara_os library.
# This promotes consistent and clear usage in other parts of the project.
add_library(ara::os ALIAS ara_os)

#****************************************************************************************************
# Installation Configuration
#****************************************************************************************************

# Install header files to the include/ara/os directory.
install(DIRECTORY include/
    DESTINATION include/ara/os
    FILES_MATCHING PATTERN "*.h"
)

#****************************************************************************************************
# Export Set Definition for ara_os
#****************************************************************************************************

# Export the ara_os_process target into the export set 'ara_os_targets'.
install(TARGETS ara_os_process
    EXPORT ara_os_targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

#****************************************************************************************************
# Export Set Installation
#****************************************************************************************************

# Install the export set 'ara_os_targets' for ara::os.
# This allows other projects or components to find and link against ara::os and its components.
install(EXPORT ara_os_targets
    FILE ara_os_targets.cmake
    NAMESPACE ara::
    DESTINATION lib/cmake/ara_os
)

#****************************************************************************************************
# Additional Notes
#****************************************************************************************************

# - The OS Abstraction Layer links against 'ara::core' implicitly through its components.
# - Each OS-specific component (e.g., process) must correctly link against 'ara::core'.
# - Ensure that all dependencies are correctly specified in each component's CMakeLists.txt.
