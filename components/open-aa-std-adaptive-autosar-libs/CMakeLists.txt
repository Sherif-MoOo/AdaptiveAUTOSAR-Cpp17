#[=======================================================================[
Author: Sherif Mohamed

File description:
-----------------
CMake configuration for the ara_core library component.
#]=======================================================================]

#****************************************************************************************************
# Library Definition
#****************************************************************************************************

# Define an interface library since ara_core is header-only
add_library(ara_core INTERFACE)

# Specify include directories for the interface library
target_include_directories(ara_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Define process identifier as a compile definition
target_compile_definitions(ara_core INTERFACE PROCESS_IDENTIFIER="AdaptiveApp")

#****************************************************************************************************
# Alias Target Definition
#****************************************************************************************************

# Create an alias target ara::core for ara_core
add_library(ara::core ALIAS ara_core)

#****************************************************************************************************
# Installation Configuration
#****************************************************************************************************

# Install header files to the include directory
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)