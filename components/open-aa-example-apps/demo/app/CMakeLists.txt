#[====================================================================]
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# CMake configuration for the demo example application.
#[====================================================================]

# ----------------------------------------------------------------------
# 1) Define the Target Name
# ----------------------------------------------------------------------
set(TARGET demo_app)

# ----------------------------------------------------------------------
# 2) Create the Executable Target
# ----------------------------------------------------------------------
add_executable(${TARGET})

# ----------------------------------------------------------------------
# 3) Specify Source Files
#    - Adds the main.cpp source file to the executable.
# ----------------------------------------------------------------------
target_sources(${TARGET}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/demo/manager/demo_manager.cpp" 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)       

# ----------------------------------------------------------------------
# 4) Include Directories
#    - Adds the necessary include directories for compiling the demo_app.
#    - Adjust the paths based on your project structure.
# ----------------------------------------------------------------------
target_include_directories(${TARGET}
    PRIVATE 
        "${PROJECT_SOURCE_DIR}/components/open-aa-example-apps/demo/app/include"
        "${PROJECT_SOURCE_DIR}/components/open-aa-std-adaptive-autosar-libs/include"
)

# ----------------------------------------------------------------------
# 5) Link Dependencies
#    - Links the executable with required libraries.
# ----------------------------------------------------------------------
target_link_libraries(${TARGET}
    PRIVATE
        ara::core::array
)

# ----------------------------------------------------------------------
# 7) Installation: Define Where to Install the Executable
#    - Installs the executable to the 'bin' directory within the installation prefix.
# ----------------------------------------------------------------------
install(TARGETS ${TARGET}
    RUNTIME DESTINATION adaptive_platform/opt/demo/bin
    # Additional options can be added here if necessary, such as COMPONENT
)

# ----------------------------------------------------------------------