#!/bin/bash

#[======================================================================
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# Build and installation script for the AdaptiveAutosarCpp17 project.
# Supports configurable options and handles interruptions gracefully.
#]=======================================================================]

#****************************************************************************************************
# Script Configuration
#****************************************************************************************************

# Exit immediately if a command exits with a non-zero status
set -e

# Default build options
BUILD_TYPE="Release"
INSTALL_DIR=""
NUM_JOBS=$(nproc)
CLEAN_BUILD=false

#****************************************************************************************************
# Function Definitions
#****************************************************************************************************

# Function to display usage information
usage() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help            Show this help message and exit"
    echo "  -c, --clean           Perform a clean build by removing the build directory"
    echo "  -t, --build-type TYPE Specify build type (e.g., Debug, Release). Default: Release"
    echo "  -i, --install-dir DIR Specify installation directory. Default: install/"
    echo "  -j, --jobs N          Specify number of parallel jobs. Default: number of CPU cores"
    echo ""
    exit 1
}

# Function to handle cleanup on interruption
cleanup() {
    echo "Build interrupted. Cleaning up..."
    exit 1
}

# Trap signals for graceful termination
trap cleanup SIGINT SIGTERM

#****************************************************************************************************
# Parse Command-Line Arguments
#****************************************************************************************************

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -h|--help)
            usage
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            ;;
        -t|--build-type)
            BUILD_TYPE="$2"
            shift
            ;;
        -i|--install-dir)
            INSTALL_DIR="$2"
            shift
            ;;
        -j|--jobs)
            NUM_JOBS="$2"
            shift
            ;;
        *)
            echo "Unknown option: $1"
            usage
            ;;
    esac
    shift
done

#****************************************************************************************************
# Set Installation Directory
#****************************************************************************************************

if [ -z "$INSTALL_DIR" ]; then
    INSTALL_DIR="${PWD}/install"
fi

#****************************************************************************************************
# Define Build Directory
#****************************************************************************************************

BUILD_DIR="${PWD}/build"

#****************************************************************************************************
# Clean Build Directory if Requested
#****************************************************************************************************

if [ "$CLEAN_BUILD" = true ]; then
    if [ -d "$BUILD_DIR" ]; then
        echo "Removing existing build directory..."
        rm -rvf "$BUILD_DIR"
        if [ -d "$INSTALL_DIR" ]; then
            echo "Removing existing install directory..."
            rm -rvf "$INSTALL_DIR"
        fi
    fi
fi

#****************************************************************************************************
# Create Build Directory
#****************************************************************************************************

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

#****************************************************************************************************
# Configure the Project
#****************************************************************************************************

cmake .. \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}"

#****************************************************************************************************
# Build the Project
#****************************************************************************************************

cmake --build . -- -j"${NUM_JOBS}"

#****************************************************************************************************
# Install the Project
#****************************************************************************************************

cmake --install .

#****************************************************************************************************
# Completion Message
#****************************************************************************************************

echo "Build and installation completed successfully."
echo "Build type: ${BUILD_TYPE}"
echo "Installed files are located in ${INSTALL_DIR}"