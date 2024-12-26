#!/bin/bash

#[=======================================================================[
# OpenAA: Open Source Adaptive AUTOSAR Project
# Author: Sherif Mohamed
#
# File description:
# -----------------
# Functional programming style build script for AdaptiveAutosarCpp17.
# Integrates:
# - CMakePresets.json for build presets.
# - Configuration file (-C option) for initial cache variables.
# - Logging utility for better traceability.
# - Modular, functional programming style for maintainability.
#]=======================================================================]

#****************************************************************************************************
# Script Configuration
#****************************************************************************************************

set -e  # Exit on error

# Default build options
BUILD_TYPE="Release"                 # Default build type
BUILD_TARGET="gcc11_linux_x86_64"    # Default build target
NUM_JOBS=$(nproc)                    # Default to number of CPU cores
CLEAN_BUILD=false                    # Default: no clean build
SDP_PATH=""                          # Default: no QNX SDP path
PRESET_NAME=""                       # Preset name based on build type and target
CONFIG_FILE=""                       # Config file path

#****************************************************************************************************
# Logging Utility
#****************************************************************************************************

log() {
    local level=$1
    local message=$2
    local timestamp=$(date "+%Y-%m-%d %H:%M:%S")
    case $level in
        INFO) echo -e "\033[32m[INFO] [$timestamp] $message\033[0m" ;;
        WARN) echo -e "\033[33m[WARN] [$timestamp] $message\033[0m" ;;
        ERROR) echo -e "\033[31m[ERROR] [$timestamp] $message\033[0m" ;;
        *) echo "[UNKNOWN] [$timestamp] $message" ;;
    esac
}

#****************************************************************************************************
# Functions
#****************************************************************************************************

# Function to display usage information
usage() {
    log INFO "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -h, --help                Show this help message and exit"
    echo "  -c, --clean               Perform a clean build by removing build and install directories"
    echo "  -t, --build-type TYPE     Specify build type (Debug, Release). Default: Release"
    echo "  -j, --jobs N              Specify number of parallel jobs. Default: number of CPU cores"
    echo "  -b, --build-target TARGET Specify build target (gcc11_linux_x86_64, gcc11_linux_aarch64, qcc12_qnx_aarch64, qcc12_qnx_x86_64)"
    echo "  -s, --sdp-path PATH       Specify the path to qnxsdp-env.sh for QNX builds"
    echo ""
    exit 1
}

# Function to handle cleanup on interruption
cleanup() {
    log WARN "Build interrupted. Cleaning up..."
    exit 1
}

# Function to source QNX environment if required
setup_environment() {
    if [[ "$BUILD_TARGET" == qcc12_qnx_aarch64 || "$BUILD_TARGET" == qcc12_qnx_x86_64 ]]; then
        if [ -z "$SDP_PATH" ]; then
            log ERROR "SDP path must be provided for QNX builds."
            exit 1
        fi
        log INFO "Sourcing QNX SDP environment from: $SDP_PATH"
        source "$SDP_PATH"
    fi
}

# Function to define build parameters based on target and type
define_build_parameters() {
    case $BUILD_TARGET in
        gcc11_linux_x86_64)
            if [ "$BUILD_TYPE" == "Debug" ]; then
                PRESET_NAME="gcc11_linux_x86_64_debug"
            else
                PRESET_NAME="gcc11_linux_x86_64_release"
            fi
            CONFIG_FILE="CMake/CMakeConfig/$PRESET_NAME.cmake"
            ;;
        gcc11_linux_aarch64)
            if [ "$BUILD_TYPE" == "Debug" ]; then
                PRESET_NAME="gcc11_linux_aarch64_debug"
            else
                PRESET_NAME="gcc11_linux_aarch64_release"
            fi
            CONFIG_FILE="CMake/CMakeConfig/$PRESET_NAME.cmake"
            ;;
        qcc12_qnx_aarch64)
            if [ "$BUILD_TYPE" == "Debug" ]; then
                PRESET_NAME="qcc12_qnx800_aarch64_debug"
            else
                PRESET_NAME="qcc12_qnx800_aarch64_release"
            fi
            CONFIG_FILE="CMake/CMakeConfig/$PRESET_NAME.cmake"
            ;;
        qcc12_qnx_x86_64)
            if [ "$BUILD_TYPE" == "Debug" ]; then
                PRESET_NAME="qcc12_qnx800_x86_64_debug"
            else
                PRESET_NAME="qcc12_qnx800_x86_64_release"
            fi
            CONFIG_FILE="CMake/CMakeConfig/$PRESET_NAME.cmake"
            ;;
        *)
            log ERROR "Invalid build target: $BUILD_TARGET"
            usage
            ;;
    esac
}

# Function to clean build and install directories if requested
clean_directories() {
    if [ "$CLEAN_BUILD" = true ]; then
        local build_dir="${PWD}/build/${PRESET_NAME}"
        local install_dir="${PWD}/install/${PRESET_NAME}"
        log INFO "Cleaning build directory: $build_dir"
        rm -rf "$build_dir"
        log INFO "Cleaning install directory: $install_dir"
        rm -rf "$install_dir"
    fi
}

# Function to configure the project using CMake
configure_project() {
    if [ -f "$CONFIG_FILE" ]; then
        log INFO "Using configuration file: $CONFIG_FILE"
        cmake -C "$CONFIG_FILE" --preset "$PRESET_NAME"
    else
        log WARN "Configuration file not found: $CONFIG_FILE. Proceeding without it."
        cmake --preset "$PRESET_NAME"
    fi
}

# Function to build the project
build_project() {
    local build_dir="${PWD}/build/${PRESET_NAME}"
    log INFO "Building the project in: $build_dir"
    cmake --build "$build_dir" -- -j"$NUM_JOBS"
}

# Function to install the project
install_project() {
    local build_dir="${PWD}/build/${PRESET_NAME}"
    local install_dir="${PWD}/install/${PRESET_NAME}"
    log INFO "Installing the project to: $install_dir"
    cmake --install "$build_dir" --prefix "$install_dir"
}

#****************************************************************************************************
# Main Function
#****************************************************************************************************

main() {
    # Trap signals for cleanup on interruption
    trap cleanup SIGINT SIGTERM

    # Parse command-line arguments
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
            -j|--jobs)
                NUM_JOBS="$2"
                shift
                ;;
            -b|--build-target)
                BUILD_TARGET="$2"
                shift
                ;;
            -s|--sdp-path)
                SDP_PATH="$2"
                shift
                ;;
            *)
                log ERROR "Unknown option: $1"
                usage
                ;;
        esac
        shift
    done

    # Setup environment and define parameters
    setup_environment
    define_build_parameters

    # Clean directories if needed
    clean_directories

    # Configure, build, and install the project
    configure_project
    build_project
    install_project

    log INFO "Build completed successfully."
}

#****************************************************************************************************
# Execute Main Function
#****************************************************************************************************

main "$@"
