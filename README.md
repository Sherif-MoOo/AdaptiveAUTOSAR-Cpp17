# OpenAA: Adaptive Autosar C++17 Project

This is a modular and scalable open-source Adaptive AUTOSAR demo using C++17. 
The project utilizes CMake for build configuration, facilitating easy integration, testing, and future expansions.

## Repository Structure

```
.
├── CMake
│   ├── CMakeConfig
│   │   ├── gcc11_linux_aarch64_debug.cmake
│   │   ├── gcc11_linux_aarch64_release.cmake
│   │   ├── gcc11_linux_x86_64_debug.cmake
│   │   ├── gcc11_linux_x86_64_release.cmake
│   │   ├── qcc12_qnx800_aarch64_debug.cmake
│   │   ├── qcc12_qnx800_aarch64_release.cmake
│   │   ├── qcc12_qnx800_x86_64_debug.cmake
│   │   └── qcc12_qnx800_x86_64_release.cmake
│   └── Toolchain
│       ├── gcc11_linux_aarch64_debug.cmake
│       ├── gcc11_linux_aarch64_release.cmake
│       ├── gcc11_linux_x86_64_debug.cmake
│       ├── gcc11_linux_x86_64_release.cmake
│       ├── qcc12_qnx800_aarch64_debug.cmake
│       ├── qcc12_qnx800_aarch64_release.cmake
│       ├── qcc12_qnx800_x86_64_debug.cmake
│       └── qcc12_qnx800_x86_64_release.cmake
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── build.sh
├── components
│   └── open-aa-std-adaptive-autosar-libs
│       ├── CMakeLists.txt
│       └── include
│           └── ara
│               └── core
│                   ├── array.h
│                   └── internal
│                       └── location_utils.h
└── tests
    └── core_platform
        ├── CMakeLists.txt
        └── ara_core_array.cpp
```

## Prerequisites

Before building the project, ensure that your system meets the following requirements:

- **Operating System**: Linux (tested on Ubuntu 22.04)
- **C++ Compiler**: GCC 11.4.0 or later or QNX QCC 12
- **CMake**: Version 3.27 or later
- **Bash**: Version 4.0 or later
- **GNU Make**: For building targets

### Installing Dependencies on Ubuntu

```bash
sudo apt update
sudo apt install -y build-essential cmake gcc-11 g++-11
```

## Building the Project

To build the project, use the `build.sh` script included in the repository. The script supports multiple configurations  different toolchains and architectures.

### Usage

```bash
./build.sh [OPTIONS]
```

### Options:

- **`-c` or `--clean`**: Perform a clean build by removing build and install directories.
- **`-t` or `--build-type`**: Specify the build type (`Debug` or `Release`). Default is `Release`.
- **`-b` or `--build-target`**: Specify the build target. Options:
  - `gcc11_linux_x86_64`
  - `gcc11_linux_aarch64`
  - `qcc12_qnx800_aarch64`
  - `qcc12_qnx800_x86_64`
- **`-s` or `--sdp-path`**: Specify the path to `qnxsdp-env.sh` for QNX builds.
- **`-j` or `--jobs`**: Specify the number of parallel jobs for the build. Default is the number of CPU cores.

### Example Commands

#### Clean and Build for GCC 11 Linux x86_64 (Release)
```bash
./build.sh --clean -b gcc11_linux_x86_64 -t Release -j 8
```

#### Build for QNX AARCH64 (Debug)
```bash
./build.sh -b qcc12_qnx800_aarch64 -t Debug -s /path/to/qnxsdp-env.sh -j 4
```

#### Build for GCC 11 Linux AARCH64 (Release)
```bash
./build.sh --clean -b gcc11_linux_aarch64 -t Release -j 8
```

## Testing the Project

The `tests` directory contains unit tests for the project. After building, tests can be executed as follows:

```bash
cd build/<build-target>
./tests/core_platform/ara_core_array_test
```

## Contributing

Contributions are welcome! create a feature branch, and submit a pull request.

## License

This project is licensed under the terms and conditions outlined in the repository's LICENSE file.
