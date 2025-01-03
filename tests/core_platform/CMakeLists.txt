#[=======================================================================[
Author: Sherif Mohamed

File description:
-----------------
CMake configuration for the core_platform tests.
#]=======================================================================]

#****************************************************************************************************
# Test Executable Definition
#****************************************************************************************************

# Define the test executable
add_executable(ara_core_array_test
    ara_core_array.cpp
)

target_compile_definitions(ara_core INTERFACE PROCESS_IDENTIFIER="TestArray")

#****************************************************************************************************
# Linking Configuration
#****************************************************************************************************

# Link the test executable against the ara::core and ara::os::process alias libraries to inherit usage requirements
target_link_libraries(ara_core_array_test PRIVATE
    ara::core
    ara::os::process
)

#****************************************************************************************************
# Installation Configuration
#****************************************************************************************************

# Install the test executable to the bin directory
install(TARGETS ara_core_array_test
    DESTINATION platform_core_test/bin
)

#****************************************************************************************************
# Testing Configuration
#****************************************************************************************************

# Enable testing
enable_testing()

# Register the test with CTest
add_test(NAME AraCoreArrayTest
    COMMAND ara_core_array_test
)
