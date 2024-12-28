# OpenAA: Adaptive AUTOSAR C++17 Project

This is a modular and scalable open-source Adaptive AUTOSAR demo using C++17. 
The project utilizes CMake for build configuration, facilitating easy integration, testing, and future expansions.

## Repository Structure

```
.
├── CMake
│   ├── CMakeConfig
│   │   ├── gcc11_linux_aarch64_debug.cmake
│   │   ├── gcc11_linux_aarch64_release.cmake
│   │   ├── gcc11_linux_x86_64_debug.cmake
│   │   ├── gcc11_linux_x86_64_release.cmake
│   │   ├── qcc12_qnx800_aarch64_debug.cmake
│   │   ├── qcc12_qnx800_aarch64_release.cmake
│   │   ├── qcc12_qnx800_x86_64_debug.cmake
│   │   └── qcc12_qnx800_x86_64_release.cmake
│   └── Toolchain
│       ├── CMakeLogging
│       │   └── tool_chain_log_config.cmake
│       ├── gcc11_linux_aarch64_debug.cmake
│       ├── gcc11_linux_aarch64_release.cmake
│       ├── gcc11_linux_x86_64_debug.cmake
│       ├── gcc11_linux_x86_64_release.cmake
│       ├── qcc12_qnx800_aarch64_debug.cmake
│       ├── qcc12_qnx800_aarch64_release.cmake
│       ├── qcc12_qnx800_x86_64_debug.cmake
│       └── qcc12_qnx800_x86_64_release.cmake
├── CMakeLists.txt
├── CMakePresets.json
├── README.md
├── build.sh
├── components
│   └── open-aa-std-adaptive-autosar-libs
│       ├── CMakeLists.txt
│       └── include
│           └── ara
│               └── core
│                   ├── array.h
│                   └── internal
│                       └── location_utils.h
└── tests
    └── core_platform
        ├── CMakeLists.txt
        └── ara_core_array.cpp
```

## Prerequisites

Before building the project, ensure that your system meets the following requirements:

- **Operating System**: Linux (tested on Ubuntu 22.04)
- **C++ Compiler**:
- **GCC**: Version 11.4.0 or later
- **QNX QCC**: Version 12 (for QNX builds)
- **CMake**: Version 3.27 or later
- **Bash**: Version 4.0 or later
- **GNU Make**: For building targets
- **CMake Linting** (Optional): `cmakelint` for linting CMake scripts

### Installing Dependencies on Ubuntu

Install the necessary dependencies using the following commands:

```bash
sudo apt update
sudo apt install -y build-essential cmake gcc-11 g++-11
```

### Installing QNX SDP (for QNX Builds)

To build for QNX platforms, you need to install the QNX Software Development Platform (SDP). Please refer to QNX's official documentation for installation instructions.

## Building the Project

To build the project, use the `build.sh` script included in the repository. The script supports multiple configurations, different toolchains, and architectures.

### Usage

```bash
./build.sh [OPTIONS]
```

### Options:

- **`-h` or `--help`**: Show help message and exit.
- **`-c` or `--clean`**: Perform a clean build by removing build and install directories.
- **`-t` or `--build-type TYPE`**: Specify the build type (`Debug` or `Release`). Default is `Release`.
- **`-b` or `--build-target TARGET`**: Specify the build target. Options:
  - `gcc11_linux_x86_64`
  - `gcc11_linux_aarch64`
  - `qcc12_qnx800_aarch64`
  - `qcc12_qnx800_x86_64`
- **`-s` or `--sdp-path PATH`**: Specify the path to `qnxsdp-env.sh` for QNX builds.
- **`-j` or `--jobs N`**: Specify the number of parallel jobs for the build. Default is the number of CPU cores.
- **`-l` or `--lint`**: (Optional) Run CMake linting using `cmakelint`.

### Example Commands

#### Clean and Build for GCC 11 Linux x86_64 (Release)
```bash
./build.sh --clean -b gcc11_linux_x86_64 -t Release -j 8
```

#### Build for QNX aarch64le (Debug)
```bash
./build.sh -b qcc12_qnx800_aarch64 -t Debug -s /path/to/qnxsdp-env.sh -j 4
```

#### Build for GCC 11 Linux aarch64le (Release)
```bash
./build.sh --clean -b gcc11_linux_aarch64 -t Release -j 8
```

#### Clean and Build for QNX x86_64 (Release) with Linting
```bash
./build.sh --clean -b qcc12_qnx800_x86_64 -t Release -s /path/to/qnxsdp-env.sh -j 4 -l
```

## Build Targets

The project supports the following build targets:

| Build Target                 | Compiler    | Platform | Architecture | Build Type       |
|------------------------------|-------------|----------|--------------|------------------|
| `gcc11_linux_x86_64`         | GCC 11      | Linux    | x86_64       | Release/Debug    |
| `gcc11_linux_aarch64`        | GCC 11      | Linux    | aarch64le    | Release/Debug    |
| `qcc12_qnx800_aarch64`       | QCC 12      | QNX      | aarch64le    | Release/Debug    |
| `qcc12_qnx800_x86_64`        | QCC 12      | QNX      | x86_64       | Release/Debug    |

*Note: Append `Debug` to the build type when specifying `--build-type Debug` in the build script.*

## Testing the Project

The `tests` directory contains test Apps for the project. After building, tests can be executed as follows:

```bash
cd install/<build-target>
./platform_core_test/bin/ara_core_array_test [OPTION]
```

## Advanced Configuration

### Adding a New Build Target

To add a new build target, follow these steps:

1. **Create a New CMake Configuration File**:
    - Navigate to `CMake/CMakeConfig/` and create a new `.cmake` file based on existing configurations.

2. **Define the Build Target in the Build Script**:
    - Update the `build.sh` script to recognize the new build target and map it to the appropriate configuration file.

3. **Update `CMakePresets.json`**:
    - Add a new preset corresponding to the new build target for seamless integration with CMake.

### Integrating Additional Components

1. **Add a New Component Directory**:
    - Create a new directory under `components/` for your component.

2. **Define `CMakeLists.txt` for the Component**:
    - Ensure that the component's `CMakeLists.txt` properly sets up include directories, dependencies, and installation rules.

3. **Include the Component in the Root `CMakeLists.txt`**:
    - Add `add_subdirectory(components/your-component)` to include it in the build process.


## Troubleshooting

### Common Issues

1. **CMake Not Found**:
    - **Error**:
        ```
        cmake: command not found
        ```
    - **Solution**:
        Ensure that CMake is installed and added to your PATH.
        ```bash
        sudo apt install -y cmake
        ```

2. **QNX Environment Variables Not Set**:
    - **Error**:
        ```
        Error: QNX_HOST and QNX_TARGET environment variables must be set.
        ```
    - **Solution**:
        Ensure correct SDP path exisits.

3. **Toolchain File Not Found**:
    - **Error**:
        ```
        Error: Toolchain file not found: CMake/CMakeConfig/gcc11_pikeos5_armv8hf.cmake
        ```
    - **Solution**:
        Verify that the toolchain file exists in the specified directory and that the build target is correctly specified.

4. **Compilation Errors**:
    - **Cause**:
        Possible mismatches between compiler versions or missing dependencies.
    - **Solution**:
        Ensure that the correct compiler is being used and all dependencies are installed.

### Getting Help

If you encounter issues not covered in this section, feel free to open an issue on the [GitHub repository](https://github.com/Sherif-MoOo/AdaptiveAutosAR-Cpp17/issues).


## Contributing

Contributions are welcome! Please follow these steps to contribute:

1. **Fork the Repository**: Create your own fork of the project.
2. **Create a Feature Branch**: Develop your feature or bug fix in a separate branch.
   ```bash
   git checkout -b feature/my-new-feature
   ```
3. **Commit Your Changes**: Make your changes and commit them with descriptive messages.
   ```bash
   git commit -m "Add new feature XYZ"
   ```
4. **Push to Your Fork**: Push your changes to your forked repository.
   ```bash
   git push origin feature/my-new-feature
   ```
5. **Submit a Pull Request**: Open a pull request against the `master_integration` branch of the main repository.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
