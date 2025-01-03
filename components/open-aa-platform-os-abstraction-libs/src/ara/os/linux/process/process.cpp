/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/os/linux/process/process.cpp
 *  \brief      Linux-specific implementation of the ara::os::interface::process::ProcessInteraction interface.
 *
 *  \details    Implements the GetProcessName method using Linux system calls and standard libraries.
 *
 *  \note       This implementation ensures thread safety and handles potential errors gracefully.
 ***********************************************************************************************************************/

#include "ara/os/linux/process/process.h"
#include "ara/core/array.h" // Include the ara::core::Array template

#include <unistd.h>     // For getpid
#include <limits.h>     // For PATH_MAX
#include <sys/types.h>  // For pid_t
#include <fstream>      // For std::ifstream
#include <cstring>      // For std::strncpy

namespace ara {
namespace os {
namespace linux {
namespace process {

/**********************************************************************************************************************
 *  FUNCTION: ProcessInteractionImpl::GetProcessName
 *********************************************************************************************************************/
/*!
 * \brief  Retrieves the name of the current process.
 *
 * \param[out] buffer      Pointer to the buffer where the process name will be stored.
 * \param[in]  bufferSize  Size of the provided buffer in bytes.
 *
 * \return An ara::os::interface::process::ErrorCode indicating the result of the operation.
 *
 * \note   This method avoids exceptions and uses safe string operations to prevent buffer overflows.
 */
auto ProcessInteractionImpl::GetProcessName(char* buffer, std::size_t bufferSize) const noexcept -> ara::os::interface::process::ErrorCode {
    /* Validate input parameters */
    if (buffer == nullptr) {
        return ara::os::interface::process::ErrorCode::NullBuffer;
    }

    /* Define the path buffer size */
    constexpr std::size_t max_path_length = 256;
    ara::core::Array<char, max_path_length> proc_path{};
    pid_t pid = getpid();

    /* Construct the path to /proc/<pid>/comm */
    int snprintf_result = std::snprintf(proc_path.data(), max_path_length, "/proc/%d/comm", pid);
    if (snprintf_result < 0 || static_cast<std::size_t>(snprintf_result) >= max_path_length) {
        /* snprintf failed or truncated the path */
        return ara::os::interface::process::ErrorCode::RetrievalFailed;
    }

    /* Open the comm file to read the process name */
    std::ifstream comm_file(proc_path.data(), std::ios::in);
    if (!comm_file.is_open()) {
        /* Failed to open the comm file */
        return ara::os::interface::process::ErrorCode::RetrievalFailed;
    }

    /* Read the process name */
    std::string process_name;
    std::getline(comm_file, process_name);
    comm_file.close();

    /* Validate the retrieved process name */
    if (process_name.empty()) {
        /* Empty process name retrieved */
        return ara::os::interface::process::ErrorCode::RetrievalFailed;
    }

    /* Ensure the buffer is large enough (including null terminator) */
    if (process_name.length() + 1 > bufferSize) {
        return ara::os::interface::process::ErrorCode::BufferTooSmall;
    }

    /* Securely copy the process name into the buffer */
    std::strncpy(buffer, process_name.c_str(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0'; // Ensure null-termination

    return ara::os::interface::process::ErrorCode::Success;
}

/**********************************************************************************************************************
 *  FUNCTION: CreateProcessInteractionInstance
 *********************************************************************************************************************/
/*!
 * \brief  Factory function to create a Linux-specific ProcessInteraction instance.
 *
 * \return A std::unique_ptr to an ara::os::interface::process::ProcessInteraction object.
 */
auto CreateProcessInteractionInstance() -> std::unique_ptr<ara::os::interface::process::ProcessInteraction> {
    return std::make_unique<ProcessInteractionImpl>();
}

} // namespace process
} // namespace linux
} // namespace os
} // namespace ara
